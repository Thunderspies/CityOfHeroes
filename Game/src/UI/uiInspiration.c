
#include "UI/uiInspiration.h"
#include "uiBaseStorage.h"
#include "UI/uiInput.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiWindows.h"
#include "UI/uiTray.h"
#include "UI/uiCursor.h"
#include "UI/uiChat.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"
#include "UI/uiNet.h"
#include "UI/uiTrade.h"
#include "UI/uiContextMenu.h"
#include "UI/uiWindows_init.h"
#include "uiGift.h"

#include "entity/character_base.h"
#include "gameComm/wdwbase.h"
#include "win/win_init.h"
#include "entity/powers.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "player/player.h"
#include "entity/entity.h"
#include "entity/entVarUpdate.h"
#include "clientcomm/clientcomm.h"
#include "cmdparse/cmdcommon.h"
#include "entity/entity_power_client.h"
#include "win/input.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdgame.h"
#include "graphics/textureatlas.h"
#include "bases/bases.h"
#include "bases/basedata.h"
#include "bases/basestorage.h"
#include "entity/character_eval.h"

#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/EntPlayer.h"
#include "entity/teamCommon.h"
#include "entity/TaskforceParams.h"
#include "UI/uiInspiration.h"
#include "UI/uiAuction.h"
#include <utilitieslib/components/estring.h>
#include "auction/AuctionData.h"
//-----------------------------------------------------------------------------------------

enum
{
    ICON_NONE,        // icon will just display
    ICON_QUEUED,    // icon has been click, must wait for mapserver to change it before modifying it
    ICON_WAITING,    // icon waiting for response from mapserver
    ICON_SHRINKING, // icon is shrinking
    ICON_GROWING,    // icon is growing
    ICON_GONE,        // icon has just dissapeared
    ICON_DRAGGING,    // used?
    ICON_FALLING,    // Icon is falling down on spot
    ICON_FALL2,     // Icon is falling two slots
    ICON_FALL3,        // icon is falling three slots
    ICON_FALL4,     // just in case
};

enum
{
    INSP_TRAY_OPEN,        // the tray is opening
    INSP_TRAY_LOCKED,    // the tray should stay open
    INSP_TRAY_CLOSED,   // the tray is closing
};

#define ICON_WD                32        // Size of inspiration icon
#define SPACER                4        // space between icons

static int trayState = INSP_TRAY_CLOSED;

static int init = TRUE; // do we update the inspirations or not?

// local copy of the inspiration slots, used when performing animations
TrayObj inspSlot[CHAR_INSP_MAX_COLS][CHAR_INSP_MAX_ROWS] = {0};

static ToolTip traySlotToolTip[CHAR_INSP_MAX_COLS*CHAR_INSP_MAX_ROWS] = {0};
static ToolTipParent trayTipParent = {0};


ContextMenu *gInspirationContext = 0;
ContextMenu *gInspirationMergeSmall = 0, *gInspirationMergeMedium = 0, *gInspirationMergeLarge = 0;

// This function is necessary due to the implementation of fixWindowHeightValuesToNewAttachPoints().
// See that function for a detailed comment as to why.
void fixInspirationTrayToDealWithRealWidthAndHeight(void)
{
    Entity *e = playerPtr();

    // These values are wrong, so undo them to get us back to a clean state
    winDefs[WDW_INSPIRATION].loc.xp -= winDefs[WDW_INSPIRATION].loc.wd * winDefs[WDW_INSPIRATION].loc.sc;
    winDefs[WDW_INSPIRATION].loc.yp -= winDefs[WDW_INSPIRATION].loc.ht * winDefs[WDW_INSPIRATION].loc.sc;

    // Now manually hack the values to deal with the fact that the authoritative source
    // of these values is somewhere totally different right now.
    if (trayState != INSP_TRAY_LOCKED)
    {
        winDefs[WDW_INSPIRATION].loc.xp += ((ICON_WD + SPACER) * e->pchar->iNumInspirationCols + 16) * winDefs[WDW_INSPIRATION].loc.sc;
        winDefs[WDW_INSPIRATION].loc.yp += (ICON_WD * 1 + 20) * winDefs[WDW_INSPIRATION].loc.sc;
    }
    else
    {
        winDefs[WDW_INSPIRATION].loc.xp += ((ICON_WD + SPACER) * e->pchar->iNumInspirationCols + 16) * winDefs[WDW_INSPIRATION].loc.sc;
        winDefs[WDW_INSPIRATION].loc.yp += (ICON_WD * e->pchar->iNumInspirationRows + 20) * winDefs[WDW_INSPIRATION].loc.sc;
    }
}
/**********************************************************************func*
* trayobj_ActivateInspiration
*
*/
static void trayobj_ActivateInspiration( int iCol, int iRow, Entity *e)
{
    START_INPUT_PACKET(pak, CLIENTINP_ACTIVATE_INSPIRATION);
    entity_ActivateInspirationSend(e, pak, iCol, iRow);
    END_INPUT_PACKET
}


// Send a move to the server, destination of -1,-1 will delete inspiration
void trayobj_moveInspiration( int iSrcCol, int iSrcRow, int iDestCol, int iDestRow )
{
    //trade_moveInspiration( iSrcCol, iSrcRow, iDestCol, iDestRow );

    START_INPUT_PACKET(pak, CLIENTINP_MOVE_INSPIRATION);
    pktSendBitsPack( pak, 3, iSrcCol );
    pktSendBitsPack( pak, 3, iSrcRow );
    pktSendBitsPack( pak, 3, iDestCol);
    pktSendBitsPack( pak, 3, iDestRow);
    END_INPUT_PACKET
}

void inspiration_drag( int i, int j, AtlasTex *icon )
{
    trayobj_startDragging( &inspSlot[i][j], icon, NULL );
}

TrayObj * inspiration_getCur( int i, int j )
{
    return &inspSlot[i][j];
}

// This function will activate the inspiration in the given slot (used by hotkey binds)
//
void inspiration_activateSlot( int slot_num, int row_num )
{
    Entity *e = playerPtr();

    if(slot_num>=0 && slot_num < e->pchar->iNumInspirationCols
        && row_num>=0 && row_num < e->pchar->iNumInspirationRows)
    {
        trayobj_ActivateInspiration( slot_num, row_num, playerPtr() );
        inspSlot[slot_num][row_num].state = ICON_SHRINKING;
        init = FALSE;
    }
}

int inspiration_FindByName( char * pchName, int *slot, int *row )
{
    Entity * e = playerPtr();
    int i,j;

    // Display names first
    for( i = 0; i < e->pchar->iNumInspirationRows; i++  )
    {
        for( j = 0; j < e->pchar->iNumInspirationCols; j++ )
        {
            if(e->pchar->aInspirations[j][i]
            && stricmp(textStd(e->pchar->aInspirations[j][i]->pchDisplayName), pchName)==0)
            {
                *slot = j;
                *row = i;
                return 1;
            }
        }
    }

    // Internal names
    for( i = 0; i < e->pchar->iNumInspirationRows; i++  )
    {
        for( j = 0; j < e->pchar->iNumInspirationCols; j++ )
        {
            if(e->pchar->aInspirations[j][i]
            && stricmp(textStd(e->pchar->aInspirations[j][i]->pchName), pchName)==0)
            {
                *slot = j;
                *row = i;
                return 1;
            }
        }
    }
    return 0;
}

void inspiration_findAndActivateSlot( char *pch )
{
    int slot, row;
    if( inspiration_FindByName(pch, &slot, &row) )
        inspiration_activateSlot(slot, row);
}



void inspiration_setMode( int mode )
{
    int rows = playerPtr()->pchar->iNumInspirationRows;
    int cols = playerPtr()->pchar->iNumInspirationCols;
    trayState = mode;
        
    if( mode )
        window_setDims( WDW_INSPIRATION, -1, -1, -1, 20+rows*ICON_WD );
}

//---------------------------------------------------------------------------------------
//
//

static int inspirationcm_CanSendToStorage(TrayObj * obj)
{
    if(windowUp( WDW_BASE_STORAGE ))
    {
        Entity * e = playerPtr();
        RoomDetail * detail = getStorageWindowDetail();
        const BasePower *ppow = obj?e->pchar->aInspirations[obj->iset][obj->ipow]:NULL;
        char *pathInsp = ppow ? basepower_ToPath(ppow) : NULL;

        if(entity_CanAdjStoredInspiration( e, detail, ppow, pathInsp, 1 ))
            return CM_AVAILABLE;
        else
            return CM_VISIBLE;
    }
    else
        return CM_HIDE;
} 

static int inspirationcm_CanSendToTrade(TrayObj * obj)
{
    if( windowUp( WDW_TRADE ) )
        return CM_AVAILABLE;
    else
        return CM_HIDE;
} 

static int inspirationcm_CanSendToStore(TrayObj * obj)
{
    if( windowUp( WDW_STORE ) )
        return CM_AVAILABLE;
    else
        return CM_HIDE;
} 
 
static int inspirationcm_CanSendToAuction(TrayObj * obj)
{
    if( windowUp( WDW_AUCTION ) )
        return CM_AVAILABLE;
    else
        return CM_HIDE;
} 

static void inspirationcm_MoveToStorage( TrayObj * obj )
{
    Entity *e = playerPtr();
    if( !obj )
        return;

    if( windowUp( WDW_BASE_STORAGE ) )
    {
        RoomDetail * detail = getStorageWindowDetail();
        const BasePower *ppow = obj?e->pchar->aInspirations[obj->iset][obj->ipow]:NULL;
        if( ppow && detail->info->eFunction == kDetailFunction_StorageInspiration)
        {
            inspirationAddToStorageContainer(ppow,detail->id,obj->iset,obj->ipow,1);
        }
    }
}

static void inspirationcm_MoveToTrade( TrayObj * obj )
{
    Entity *e = playerPtr();
    if( !obj )
        return;
    if( windowUp( WDW_TRADE ) )
        inspMarkForTrade( e, obj->iset, obj->ipow );
}

static void inspirationcm_MoveToAuction( TrayObj * obj )
{
    Entity *e = playerPtr();
    if ( e->pchar->aInspirations[obj->iset][obj->ipow] )
    {
        char *name = basepower_ToPath(e->pchar->aInspirations[obj->iset][obj->ipow]);
        char * pchIdent = auction_identifier( kTrayItemType_Inspiration, name, 0);
        int id = auctionData_GetID(kTrayItemType_Inspiration, name, 0);

        auction_addItem(pchIdent, (obj->iset << 16 | (obj->ipow & 0x0000FFFF)), 1, 0, AuctionInvItemStatus_Stored, id);
        addItemToLocalInventory( pchIdent, (obj->iset << 16 | (obj->ipow& 0x0000FFFF)), AuctionInvItemStatus_Stored, 0, 1, 0, 0, 0, false, 1 );
    }
}

void inspirationcm_Remove( void * data )
{
    Entity *e = playerPtr();
    TrayObj *ts = (TrayObj*)data;
    trayobj_moveInspiration( ts->iset, ts->ipow, -1, -1 ); // effectively a delete command
}

void inspirationDeleteByName( char * name )
{
    int slot, row;
    if(inspiration_FindByName( name, &slot, &row ))
        trayobj_moveInspiration( slot, row, -1, -1 ); // effectively a delete command
    
}


int inspirationcm_CanMergeSmall( TrayObj *to )
{
    int i = 0, j = 0, bFound = 0, count = 0;
    Entity *e = playerPtr();
    const BasePower *pInsp = to?e->pchar->aInspirations[to->iset][to->ipow]:NULL;
    int cols = e->pchar->iNumInspirationCols;
    int rows = e->pchar->iNumInspirationRows;

    if(!pInsp)
        return CM_HIDE;

    // make sure this inspiration qualifies
    while(g_smallInspirations[i] && !bFound)
    {
        if( stricmp(g_smallInspirations[i], pInsp->pchName)==0 )
            bFound = 1;
        i++;
    }

    if(!bFound)
        return CM_HIDE;

    // are there 3?
    for( i = 0; i < cols; i++ )
    {
        for( j = 0; j < rows; j++ )
        {
            if( pInsp == e->pchar->aInspirations[i][j] )
                count++;
        }
    }

    if( count >= 3 )
        return CM_AVAILABLE;

    return CM_HIDE;
}


int inspirationcm_CanMergeMedium( TrayObj *to  )
{
    int i = 0, j = 0, bFound = 0, count = 0;
    Entity *e = playerPtr();
    const BasePower *pInsp = to?e->pchar->aInspirations[to->iset][to->ipow]:NULL;
    int cols = e->pchar->iNumInspirationCols;
    int rows = e->pchar->iNumInspirationRows;

    if(!pInsp)
        return CM_HIDE;

    while(g_mediumInspirations[i] && !bFound)
    {
        if( stricmp(g_mediumInspirations[i], pInsp->pchName)==0 )
            bFound = 1;
        i++;
    }

    if(!bFound)
        return CM_HIDE;

    for( i = 0; i < cols; i++ )
    {
        for( j = 0; j < rows; j++ )
        {
            if( pInsp == e->pchar->aInspirations[i][j] )
                count++;
        }
    }

    if( count >= 3 )
        return CM_AVAILABLE;

    return CM_HIDE;
}
int inspirationcm_CanMergeLarge( TrayObj *to  )
{
    int i = 0, j = 0, bFound = 0, count = 0;
    Entity *e = playerPtr();
    const BasePower *pInsp = to?e->pchar->aInspirations[to->iset][to->ipow]:NULL;
    int cols = e->pchar->iNumInspirationCols;
    int rows = e->pchar->iNumInspirationRows;

    if(!pInsp)
        return CM_HIDE;

    while(g_largeInspirations[i] && !bFound)
    {
        if( stricmp(g_largeInspirations[i], pInsp->pchName)==0 )
            bFound = 1;
        i++;
    }

    if(!bFound)
        return CM_HIDE;

    for( i = 0; i < cols; i++ )
    {
        for( j = 0; j < rows; j++ )
        {
            if( pInsp == e->pchar->aInspirations[i][j] )
                count++;
        }
    }

    if( count >= 3 )
        return CM_AVAILABLE;

    return CM_HIDE;
}

static const BasePower * s_pInsp;

int inspirationcm_CanMerge( BasePower *pInsp )
{
    if( pInsp && s_pInsp && stricmp( pInsp->pchName, s_pInsp->pchName ) == 0 )
        return CM_VISIBLE;

    return CM_AVAILABLE;

}

void inspirationcm_Merge( BasePower *pInsp )
{
    char * str;
    estrCreate(&str);
    estrConcatf(&str, "mergeInsp %s %s", s_pInsp->pchName, pInsp->pchName );
    cmdParse(str);
    estrDestroy(&str);
}

void inspirationMerge( char * pchSourceIn, char * pchDestIn )
{
    // All this does is make a pass to search translated names and convert them to internal names for server send
    const BasePower *pInsp;
    char *pchDestOut = pchDestIn, *pchSrcOut = pchSourceIn, *str;
    int i=0;

    if(!pchSourceIn || !pchDestIn)
        return;

    while( g_smallInspirations[i] )
    {
        pInsp = basePower_inspirationGetByName("Small", g_smallInspirations[i]  );
        if( stricmp(textStd(pInsp->pchDisplayName),pchSourceIn)==0)
            pchSrcOut =  g_smallInspirations[i];
        if( stricmp(textStd(pInsp->pchDisplayName),pchDestIn)==0)
            pchDestOut =  g_smallInspirations[i];
        i++;
    }
    i = 0;
    while( g_mediumInspirations[i] )
    {
        pInsp = basePower_inspirationGetByName("Medium", g_mediumInspirations[i]  );
        if( stricmp(textStd(pInsp->pchDisplayName),pchSourceIn)==0)
            pchSrcOut =  g_mediumInspirations[i];
        if( stricmp(textStd(pInsp->pchDisplayName),pchDestIn)==0)
            pchDestOut =  g_mediumInspirations[i];
        i++;
    }
    i = 0;
    while( g_largeInspirations[i] )
    {
        pInsp = basePower_inspirationGetByName("Large", g_largeInspirations[i]  );
        if( stricmp(textStd(pInsp->pchDisplayName),pchSourceIn)==0)
            pchSrcOut =  g_largeInspirations[i];
        if( stricmp(textStd(pInsp->pchDisplayName),pchDestIn)==0)
            pchDestOut =  g_largeInspirations[i];
        i++;
    }

    estrCreate(&str);
    estrConcatf(&str, "mergeInsp %s %s", pchSrcOut, pchDestOut );
    cmdParse(str);
    estrDestroy(&str);

}

static void inspirationcm_init()
{
    int i=0;
    gInspirationContext = contextMenu_Create( NULL );
    gInspirationMergeSmall = contextMenu_Create( NULL ); 
    gInspirationMergeMedium = contextMenu_Create( NULL ); 
    gInspirationMergeLarge = contextMenu_Create( NULL ); 
    contextMenu_addVariableTitle( gInspirationContext, traycm_PowerText, 0);
    contextMenu_addVariableText( gInspirationContext, traycm_PowerInfo, 0);
    contextMenu_addCode( gInspirationContext, alwaysAvailable, 0, inspirationcm_Remove,    0, "CMRemoveInspiration", 0  );
    contextMenu_addCode( gInspirationContext, alwaysAvailable, 0, traycm_Info,            0, "CMInfoString",    0  );

    contextMenu_addCode( gInspirationContext, inspirationcm_CanSendToStorage,    0, inspirationcm_MoveToStorage,        0, "CMMoveToStorage", 0 );
    contextMenu_addCode( gInspirationContext, inspirationcm_CanSendToTrade,        0, inspirationcm_MoveToTrade,        0, "CMMoveToTrade", 0 );
    contextMenu_addCode( gInspirationContext, inspirationcm_CanSendToAuction,    0, inspirationcm_MoveToAuction,        0, "CMMoveToAuction", 0 );

    contextMenu_addCode( gInspirationContext, inspirationcm_CanMergeSmall, 0, 0, 0, "CMMergeInsp", gInspirationMergeSmall );
    contextMenu_addCode( gInspirationContext, inspirationcm_CanMergeMedium, 0, 0, 0, "CMMergeInsp", gInspirationMergeMedium );
    contextMenu_addCode( gInspirationContext, inspirationcm_CanMergeLarge, 0, 0, 0, "CMMergeInsp", gInspirationMergeLarge );

    while( g_smallInspirations[i] )
    {
        const BasePower * pInsp = basePower_inspirationGetByName( "Small", g_smallInspirations[i] );
        AtlasTex * pTex = atlasLoadTexture(pInsp->pchIconName);
        contextMenu_addIconCode( gInspirationMergeSmall, inspirationcm_CanMerge, (void*)pInsp, inspirationcm_Merge, (void*)pInsp, pInsp->pchDisplayName, 0, pTex );
        i++;
    }
    i = 0;
    while( g_mediumInspirations[i] )
    {
        const BasePower * pInsp = basePower_inspirationGetByName( "Medium", g_mediumInspirations[i] );
        AtlasTex * pTex = atlasLoadTexture(pInsp->pchIconName);
        contextMenu_addIconCode( gInspirationMergeMedium, inspirationcm_CanMerge, (void*)pInsp, inspirationcm_Merge, (void*)pInsp, pInsp->pchDisplayName, 0, pTex );
        i++;
    }
    i = 0;
    while( g_largeInspirations[i] )
    {
        const BasePower * pInsp = basePower_inspirationGetByName( "Large", g_largeInspirations[i] );
        AtlasTex * pTex = atlasLoadTexture(pInsp->pchIconName);
        contextMenu_addIconCode( gInspirationMergeLarge, inspirationcm_CanMerge, (void*)pInsp, inspirationcm_Merge, (void*)pInsp, pInsp->pchDisplayName, 0, pTex );
        i++;
    }

    gift_addToContextMenu(gInspirationContext);
}

void inspirationContext( CBox *box, TrayObj * to )
{
    int rx, ry;
    int wd, ht;
    CMAlign alignVert;
    CMAlign alignHoriz;
    Entity *e = playerPtr();

    if(!gInspirationContext)
        inspirationcm_init();

    windowClientSizeThisFrame( &wd, &ht );

    rx = (box->lx+box->hx)/2;
    alignHoriz = CM_CENTER;

    ry = box->ly<ht/2 ? box->hy : box->ly;
    alignVert = box->ly<ht/2 ? CM_TOP : CM_BOTTOM;

    s_pInsp = to?e->pchar->aInspirations[to->iset][to->ipow]:NULL;
    contextMenu_setAlign( gInspirationContext, rx, ry, alignHoriz, alignVert, to );
}


//---------------------------------------------------------------------------------------
//
//
static void inspirationUpdate( Entity *e, int i, int j )
{
    inspSlot[i][j].scale = 1.f;
    inspSlot[i][j].fall_offset = 0;
    inspSlot[i][j].iset = i;
    inspSlot[i][j].ipow = j;
    inspSlot[i][j].state = ICON_NONE;

    if( e->pchar->aInspirations[i][j] && e->pchar->aInspirations[i][j]->pchName && e->pchar->aInspirations[i][j]->pchIconName )
    {
        inspSlot[i][j].icon = atlasLoadTexture( e->pchar->aInspirations[i][j]->pchIconName );
        inspSlot[i][j].type = kTrayItemType_Inspiration;
    }
    else
    {
        inspSlot[i][j].icon = white_tex_atlas;
        inspSlot[i][j].type = kTrayItemType_None;
    }
}


//
//
void inspirationNewData( int iCol, int iRow )
{
    Entity *e = playerPtr();

     if( inspSlot[iCol][iRow].state == ICON_QUEUED )
    {
        int i = 0;

        inspSlot[iCol][iRow].state = ICON_SHRINKING;

        // after its gone all icons above it will fall one slot
        for( i = iRow+1; i < e->pchar->iNumInspirationRows; i++ )
            inspSlot[iCol][i].state = ICON_FALLING;

    }

    if( inspSlot[iCol][iRow].state == ICON_WAITING )
        inspSlot[iCol][iRow].state = ICON_NONE;
}

void inspirationUnqueue()
{
    int i,j;
    Entity * e = playerPtr();

    for( i = 0; i < e->pchar->iNumInspirationCols; i++ )
    {
        for( j = 0; j < e->pchar->iNumInspirationRows; j++ )
        {
            if( inspSlot[i][j].state == ICON_QUEUED )
                inspSlot[i][j].state = ICON_NONE;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
//

// Updates inspiration based off the clients pchar.  This is not done when animations are played
//
void updateInspirationTray( Entity * e, int cols, int rows )
{
    int i, j, k, falling = FALSE;

     for( i = 0; i < cols; i++  )
    {
        for( j = 0; j < rows; j++ )
        {
             if( inspSlot[i][j].state == ICON_NONE || inspSlot[i][j].state == ICON_GONE )
            {
                 for( k = j+1; k < e->pchar->iNumInspirationRows; k++ )
                {
                    if( inspSlot[i][k].state >= ICON_FALLING )
                        falling = TRUE;
                }

                if( falling )
                    continue;

                inspirationUpdate( e, i, j );
            }
         }
    }
}



#define SPACER                4        // space between icons
#define ICON_SHRINK_SPEED    .2f    // how fast the icons shrinks
#define ICON_FALL_SPEED        (ICON_WD+SPACER)*ICON_SHRINK_SPEED    // how fast the icons fall
//
//
static void drawInspirationSlots( Entity *e, int col, int row, float x, float y, float z, float scale, float wd, float ht, int bcolor )
{
     int color = 0x00000088;
     TrayObj *ts;
       AtlasTex *back = atlasLoadTexture( "Insp_ring_back.tga" );
       AtlasTex *border = atlasLoadTexture( "Insp_ring.tga" );
    CBox box;
    float sc = 1.f;
      const BasePower *ppow = 0;
    static TrayObj to_tmp = {0};
    bool disabled = (e->pl->taskforce_mode && TaskForceIsParameterEnabled(e, TFPARAM_NO_INSPIRATIONS));

    // get the current slot and power
      ts = &inspSlot[col][row];

    if( ts->type == kTrayItemType_Inspiration )
        ppow = e->pchar->aInspirations[ts->iset][ts->ipow];

    if( !ppow || !character_ModesAllowPower(e->pchar, ppow) || inspIsMarkedForTrade(e, col, row) ||
        (!e->access_level && ppow->ppchActivateRequires && !chareval_Eval(e->pchar, ppow->ppchActivateRequires, ppow->pchSourceFile)) )
        disabled = 1;

    // if its shrinking, shrink it
     if( ts->state == ICON_SHRINKING )
     {
        ts->scale -= TIMESTEP*ICON_SHRINK_SPEED;
        if( ts->scale < 0 )
        {
            ts->state = ICON_GONE;
            ts->scale = 0;
        }
    }

    // icon is falling
       if( ts->state >= ICON_FALLING )
    {
        // steps is how far it is falling
        int steps = ts->state - ICON_FALLING + 1;
        init = FALSE;

        if( ts->fall_offset == (steps*(ICON_WD+SPACER)+SPACER)*scale )
        {
            ts->state = ICON_GONE;
            init = TRUE;
        }
        else
        {
            ts->fall_offset += TIMESTEP*ICON_FALL_SPEED;

            if( ts->fall_offset > (steps*(ICON_WD+SPACER)+SPACER)*scale )
            {
                ts->fall_offset = (steps*(ICON_WD+SPACER)+SPACER)*scale;
            }
        }
    }
 
    // get the slot collision box
       BuildCBox( &box, x+(col*(ICON_WD+SPACER)-SPACER/2)*scale, y+ht-((row+1)*ICON_WD)*scale, (ICON_WD+SPACER)*scale, ICON_WD*scale );

     // if this slot has an inspiration in it and is visible
       if( ts->type == kTrayItemType_Inspiration && (ht - (row+1)*(ICON_WD+((row>0)*SPACER))*scale) >= 0 )
     {
        static float timer = 0;
        z += 1;

         // mouse is over inspiration
           if( mouseCollision( &box ) && ts->state == ICON_NONE && window_getMode(WDW_INSPIRATION) == WINDOW_DISPLAYING )
        {
            ts->scale = 1.1f; // embiggen it

            // drag timer is on a small delay
               if( isDown( MS_LEFT ) && !cursor.dragging  )
                ts->scale = .9f;

            if( mouseLeftClick() )
            {
                if (window_getMode(WDW_TRADE) == WINDOW_DISPLAYING)
                {
                    inspMarkForTrade(playerPtr(),col,row);
                }
                else 
                {
                    if ( !disabled )
                    {
                        ts->scale = 1.f;

                        // there can only be one
                        inspirationUnqueue();
                        ts->state = ICON_QUEUED;

                        if( e->pchar->ppowQueued && e->pchar->ppowQueued->ppowBase != e->pchar->aInspirations[col][row] )
                            entity_CancelQueuedPower(e);

                        trayobj_ActivateInspiration( col, row, playerPtr() );
                        trayobj_stopDragging(); // huh?
                        init = FALSE;
                    }
                }
            }

            if( mouseRightClick() )
            {
                buildInspirationTrayObj( &to_tmp, col, row );
                inspirationContext( &box, &to_tmp );
            }
        }
        else if ( ts->scale == 1.1f )
            ts->scale = 1.0f;

        if( mouseLeftDrag( &box ) && ts->state == ICON_NONE )
        {
            trayobj_startDragging( ts, ts->icon, NULL );
        }

         if( e->pchar->aInspirations[col][row] )
            setToolTip( &traySlotToolTip[row*e->pchar->iNumInspirationCols+col], &box, e->pchar->aInspirations[col][row]->pchDisplayName, 0, MENU_GAME, WDW_INSPIRATION );
        else
            clearToolTip( &traySlotToolTip[row*e->pchar->iNumInspirationCols+col] );
    }
    else
        clearToolTip( &traySlotToolTip[row*e->pchar->iNumInspirationCols+col] );

       if( cursor.dragging && ts->state != ICON_QUEUED )
    {
        if( cursor.drag_obj.type == kTrayItemType_Inspiration )
            ts->state = ICON_NONE;

        if( mouseCollision( &box )  )
         {
            if( cursor.drag_obj.type == kTrayItemType_Inspiration )
            {
                sc = .9f;
                color = 0x44444444;
                
                if( !isDown( MS_LEFT ))
                {
                    if( cursor.drag_obj.iset != col || cursor.drag_obj.ipow != row )
                    {
                        int i, steps = 0;
                        
                        inspSlot[cursor.drag_obj.iset][cursor.drag_obj.ipow].type = ts->type;
                        inspSlot[cursor.drag_obj.iset][cursor.drag_obj.ipow].icon = ts->icon;
                        inspSlot[cursor.drag_obj.iset][cursor.drag_obj.ipow].state = ICON_WAITING;
                        
                        ts->type = kTrayItemType_Inspiration;
                        ts->icon = cursor.drag_obj.icon;
                        ts->state = ICON_WAITING;
                        
                        if( inspSlot[cursor.drag_obj.iset][cursor.drag_obj.ipow].type == kTrayItemType_None )
                        {
                            for( i = (cursor.drag_obj.ipow+1); i<e->pchar->iNumInspirationRows; i++ )
                            {
                                inspSlot[cursor.drag_obj.iset][i].state = ICON_FALLING;
                                init = FALSE;
                            }
                        }
                        
                        //find how far we are falling
                        for( i = row-1; i >= 0; i-- )
                        {
                            if( inspSlot[col][i].type != kTrayItemType_Inspiration )
                                steps++;
                        }
                        
                        if( steps )
                        {
                            ts->state = ICON_FALLING + steps - 1;
                            init = FALSE;
                        }
                        
                        trayobj_moveInspiration( cursor.drag_obj.iset, cursor.drag_obj.ipow, col, row );
                    }
                    trayobj_stopDragging();
                }
                if( ts->iset == cursor.drag_obj.iset && ts->ipow == cursor.drag_obj.ipow )
                    sc *= .5f;
            }
            else if( ts->type == kTrayItemType_None 
                     && cursor.drag_obj.type == kTrayItemType_StoredInspiration 
                     )
            {
                if( !isDown( MS_LEFT ) )
                {
                    inspirationAddToStorageContainer(cursor.drag_obj.ppow, cursor.drag_obj.idDetail, col, row, -1 );
                    trayobj_stopDragging(); 
                }
                else
                {
                    sc *= .5f;
                }
            }
        }
    }

       if( ts->scale != 0 && ts->type==kTrayItemType_Inspiration )
    {
        int color = CLR_WHITE;

        if ( disabled )
            color = CLR_GREYED;

        display_sprite( ts->icon, x + (col*(ICON_WD+SPACER) + ICON_WD/2*(1-ts->scale*sc))*scale,
                         y + ts->fall_offset + ht + (-(row+1)*(ICON_WD) + ICON_WD/2*(1-ts->scale*sc))*scale,
                         z+1, ts->scale*sc*scale, ts->scale*sc*scale, color );

         if( ts->state == ICON_QUEUED )
        {
            if( row == 0 )
            {
                box.ly+=1*scale;
                box.hy-=2*scale;
            }
            bcolor = CLR_GREEN;
        }
    }

       display_sprite( back, x + (col*(ICON_WD+SPACER) + ICON_WD/2*(1-sc))*scale, y + ht + (-(row+1)*(ICON_WD) + ICON_WD/2*(1-sc))*scale, z,
                      ((float)ICON_WD/back->width)*sc*scale, ((float)ICON_WD/back->height)*sc*scale, CLR_WHITE );
    display_sprite( border, x + (col*(ICON_WD+SPACER) + ICON_WD/2*(1-sc))*scale, y + ht + (-(row+1)*(ICON_WD) + ICON_WD/2*(1-sc))*scale, z,
        ((float)ICON_WD/border->width)*sc*scale, ((float)ICON_WD/border->height)*sc*scale, bcolor );

      if( row == 0 )
      {
        char hotkey[128];
        AtlasTex * spr;
        STR_COMBINE_BEGIN(hotkey);
        STR_COMBINE_CAT("tray_ring_number_F");
        STR_COMBINE_CAT_D(col+1);
        STR_COMBINE_CAT(".tga");
        STR_COMBINE_END();
        spr = atlasLoadTexture( hotkey );
        display_sprite( spr, x + col*(ICON_WD+SPACER)*scale, y + ht - spr->height*scale, z+5, scale, scale, CLR_WHITE );
    }
}

//---------------------------------------------------------------------------------------------------------------------
#define INSP_TRAY_SPEED 15

static float s_insp_flash;
void inspirationWindowFlash(void)
{
    s_insp_flash = .0001f;
}

//
//
int inspirationWindow()
{

    Entity *e = playerPtr();
    float x, y, z, wd, ht, scale;
    int i, j, color, bcolor;

     AtlasTex * arrow;
    static int first = FALSE;

     int cols = e->pchar->iNumInspirationCols;
     int rows = e->pchar->iNumInspirationRows;

    if( !window_getDims( WDW_INSPIRATION, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
        return 0;


    if( s_insp_flash )
    {
        int flash = sinf(s_insp_flash)*128;
        bcolor |= ((flash*2)<<24)|(flash<<16)|(flash<<8)|(flash);
        s_insp_flash += TIMESTEP/4;
        if( s_insp_flash > PI )
            s_insp_flash = 0.f;
    }
     drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, bcolor );


    if( !first )
    {
        for( i = 0; i < CHAR_INSP_MAX_COLS*CHAR_INSP_MAX_ROWS; i++ )
        {
            memset( &traySlotToolTip[i], 0, sizeof(ToolTip) );
            addToolTip( &traySlotToolTip[i] );
        }


        first = TRUE;
    }

    updateInspirationTray( e, cols, rows );

      set_scissor( TRUE );
     scissor_dims( x + PIX3*scale, y + PIX3*scale, wd - PIX3*2*scale, ht - PIX3*2*scale );

    for( i = 0; i < cols; i++ )
    {
        for( j = 0; j < rows; j++ )
        {
            drawInspirationSlots( e, i, j, x+10*scale, y-10*scale, z, scale, wd-20*scale, ht, color );
        }
    }

    set_scissor( FALSE );

    wd = (cols*ICON_WD + (cols-1)*SPACER + 20)*scale;

    if( trayState != INSP_TRAY_LOCKED )
    {
        if( okToInput(0) && inpLevel( INP_SHIFT ))
            trayState = INSP_TRAY_OPEN;
        else
            trayState = INSP_TRAY_CLOSED;
    }

    if( trayState == INSP_TRAY_CLOSED )
    {
        if( (int)ht > (int)((20+ICON_WD)*scale ))
        {
            ht -= (int)(TIMESTEP*INSP_TRAY_SPEED);
        }

        if( (int)ht < (int)((20+ICON_WD)*scale ))
        {
            ht = (int)((20 + ICON_WD)*scale);
        }
    }
    else if( trayState == INSP_TRAY_OPEN || trayState == INSP_TRAY_LOCKED )
    {
        if( ht < (20+rows*ICON_WD)*scale )
        {
            ht += (int)(TIMESTEP*INSP_TRAY_SPEED);
        }

        if( ht > (20+rows*ICON_WD)*scale )
        {
            ht = (int)(20 + rows*ICON_WD)*scale;
        }
    }

      if( window_getMode(WDW_INSPIRATION) == WINDOW_DISPLAYING )
        window_setDims( WDW_INSPIRATION, -1, -1, wd, ht );

     if( rows > 1 && window_getMode( WDW_INSPIRATION ) == WINDOW_DISPLAYING )
    {
        Wdw *wdw;

          if( trayState == INSP_TRAY_CLOSED )
            arrow = atlasLoadTexture( "Jelly_arrow_up.tga" );
        else
            arrow = atlasLoadTexture( "Jelly_arrow_down.tga" );

        wdw = wdwGetWindow( WDW_TRAY );

        if( (!wdw->below && !wdw->flip) || (wdw->below && wdw->flip) )
        {
             if( D_MOUSEHIT == drawGenericButton( arrow, x + (wd - arrow->width*scale)/2, y - (JELLY_HT + arrow->height)*scale/2, z+5, scale, (color & 0xffffff00) | (0x88), (color & 0xffffff00) | (0xff), 1 ) )
            {
                if( trayState == INSP_TRAY_LOCKED )
                    trayState = INSP_TRAY_CLOSED;
                else
                    trayState = INSP_TRAY_LOCKED;
                sendInspirationMode( trayState );
            }
        }
        else
        {
             if( D_MOUSEHIT == drawGenericButton( arrow, x + (wd - arrow->width*scale)/2, y + ht + (JELLY_HT - arrow->height)*scale/2, z+5, scale, (color & 0xffffff00) | (0x88), (color & 0xffffff00) | (0xff), 1 ) )
            {
                if( trayState == INSP_TRAY_LOCKED )
                    trayState = INSP_TRAY_CLOSED;
                else
                    trayState = INSP_TRAY_LOCKED;
                sendInspirationMode( trayState );
            }
        }
    }

     return 0;
}


