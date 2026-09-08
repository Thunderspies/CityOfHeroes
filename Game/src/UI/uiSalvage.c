/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "uiSalvage.h"
#include "bases/basestorage.h"
#include "uiGrowBig.h"
#include "entity/salvage.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include "bases/DetailRecipe.h"
#include "auth/authUserData.h"

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/badges.h"
#include "player/badges_client.h"
#include "player/inventory_client.h"

#include "gameComm/wdwbase.h"
#include "UI/uiUtil.h"
#include "uiSMFView.h"
#include "UI/uiWindows.h"
#include "UI/uiUtilGame.h"
#include "UI/uiScrollBar.h"
#include "UI/uiTabControl.h"
#include "UI/uiInput.h"
#include "UI/uiInfo.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "player/player.h"
#include "cmdparse/cmdgame.h"

#include "UI/uiBadges.h"
#include "player/badges_client.h"

#include "formatter/smf_main.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "render/tex.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/StashTable.h>

#include <utilitieslib/language/MessageStore.h>
#include "entity/salvage.h"
#include "entity/character_base.h"
#include "entity/character_inventory.h"
#include "entity/character_eval.h"
#include "UI/uiComboBox.h"
#include "UI/uiContextMenu.h"
#include "UI/uiNet.h"
#include <utilitieslib/utils/error.h>
#include "UI/uiCursor.h"
#include "UI/uiTray.h"
#include "UI/uiTrade.h"
#include "entity/entVarUpdate.h"
#include "UI/uiChat.h"
#include "uiBaseStorage.h"
#include "UI/uiGift.h"
#include "bases/bases.h"
#include "UI/uiDialog.h"
#include "UI/uiOptions.h"
#include "sound/sound.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "uiAmountSlider.h"
#include "UI/uiStoredSalvage.h"
#include "entity/salvage.h"

typedef struct salvageSMF
{
    SMFBlock *smf;
    const SalvageItem *sal;
    float    sc;
}salvageSMF;

static salvageSMF ** smfList = 0;

ContextMenu * s_SalvageContext = 0;
ContextMenu * s_SalvageContextNoGift = 0;

static bool s_bReparse = true;


void salvageReparse() {
    s_bReparse = true;
}


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
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

#define ITEM_FOOTER    30
#define ITEM_SHORT_FOOTER 12

#define ICON_WD 48
#define COL_WD 96
#define ROW_HT 96
#define SPACER_WD 10

#define iTabCategoryAll 0
#define CATEGORY_ALL "AllTab"

static int tabNamesSort(const void* name1Data, const void* name2Data);
static void s_tabSelectCb(void*data);

// For processing the "Open" command from the context menu.
const SalvageItem *s_SalvageToOpen = NULL;
static char *s_SalvageToOpen_DlgPrompt = NULL;

// For processing the "Use Now" command from the context menu.
const SalvageInventoryItem *s_SalvageToUse = NULL;
static char *s_SalvageToUse_DlgPrompt = NULL;

static DialogCheckbox openDCB[] = { { "HideOpenSalvagePrompt", 0, kUO_HideOpenSalvagePrompt}, };



char * g_salvageColor[] = 
{
    "Gray",        //kSalvageRarity_Ubiquitous
    "White",    //kSalvageRarity_Common
    "#FFFF00",    //kSalvageRarity_Uncommon
    "#FF5500",    //kSalvageRarity_Rare
    "Purple",    //kSalvageRarity_VeryRare
    "Purple",    //kSalvageRarity_Unique
    "Purple",    //kSalvageRarity_Count
};
//----------------------------------------
// returns earray of matching src
// NOTE: static allocation,
//----------------------------------------
SalvageInventoryItem **SalvageInventoryItem_GatherMatching(SalvageInventoryItem **salvageInv, char *category )
{
    static SalvageInventoryItem **src = NULL;
    int i;
    eaSetSize(&src, 0);

    if(salvageInv && category)
    {
        if(0 == stricmp(CATEGORY_ALL, category))
        {
            for( i = eaSize(&salvageInv) - 1; i >= 0; --i)
            {
                SalvageInventoryItem *item = salvageInv[i];
                if( item && item->salvage)
                {
                    eaPush(&src, item);
                }
            }
        }
        else
        {


            // get the matching
            for( i = eaSize(&salvageInv) - 1; i >= 0; --i)
            {
                SalvageInventoryItem *item = salvageInv[i];
                if( item && item->salvage
                    && 0 == stricmp( item->salvage->pchDisplayTabName, category ))
                {
                    eaPush(&src, item);
                }
            }
        }
    }

    // --------------------
    // finally

    return src;
}


static salvageSMF * findOrAddSMF(float sc, SalvageItem const *salvage )
{
    int i, count, foundi = -1;
    salvageSMF * new_smf = NULL;

    if( !smfList )
        eaCreate(&smfList);

    // first look for string in list
    count = eaSize(&smfList);
    for( i = 0; i < count; i++ )
    {
        if(salvage == smfList[i]->sal)
        {
            if (sc == smfList[i]->sc)
                return smfList[i];
            else
            {
                foundi = i;
                new_smf = smfList[i];
            }

        }
    }

    // no dice, add it to list
    if (!new_smf)
    {
        new_smf = calloc(1,sizeof(salvageSMF));
        new_smf->smf = smfBlock_Create();
    }
    new_smf->sc = sc;
    new_smf->sal = salvage;
    gTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE*sc*1.1));
    if (foundi == -1)
        eaPush( &smfList, new_smf );
    return new_smf;
}


static int salvage_context_wdw = 0;

static const SalvageInventoryItem * salvageFromTrayObject( const TrayObj * to )
{
    Entity * e = playerPtr();
    const SalvageInventoryItem * sal = NULL;

    if( to->type == kTrayItemType_Salvage && e && e->pchar )
        sal = eaGet(&e->pchar->salvageInv,to->invIdx);
    else if ( to->type == kTrayItemType_StoredSalvage )
        sal = getStoredSalvageFromName(to->nameSalvage);
    else if( to->type == kTrayItemType_PersonalStorageSalvage && e && e->pchar )
        sal = eaGet(&e->pchar->storedSalvageInv,to->invIdx);
    return sal;
}

static void cmsalvage_Info( void * data )
{
    const SalvageInventoryItem *sal = salvageFromTrayObject(data);
    if(sal)
        info_Salvage(sal->salvage);
}


///////////////////////////////////////////////////////////////////////

const SalvageInventoryItem *deleteme_sal;
static char *deleteme_sal_string = NULL;
static DialogCheckbox deleteDCB[] = { { "HideDeleteSalvagePrompt", 0, kUO_HideDeleteSalvagePrompt}, };
DialogCheckbox deleteUsefulDCB[] = { { "HideUsefulSalvagePrompt", 0, kUO_HideUsefulSalvagePrompt}, };

static void salvage_delete(void * data)
{
    Entity  * e = playerPtr();
    if(deleteme_sal && deleteme_sal->salvage && !(deleteme_sal->salvage->flags & SALVAGE_NODELETE))
    {
        if( windowUp( WDW_BASE_STORAGE ) && WDW_BASE_STORAGE == salvage_context_wdw )
        {
             RoomDetail *detail = getStorageWindowDetail();
            basestorage_SendSalvageAdjust( detail->id, deleteme_sal->salvage->pchName, -1 );
        } 
        else
        {
            bool is_stored_salvage = (WDW_STOREDSALVAGE == salvage_context_wdw);
            sendSalvageDelete( deleteme_sal->salvage->pchName, 1, is_stored_salvage );
            if(!is_stored_salvage)
                character_AdjustSalvage(e->pchar, (const SalvageItem*) deleteme_sal->salvage, -1, NULL, false );
            else
                character_AdjustStoredSalvage(e->pchar, (const SalvageItem*) deleteme_sal->salvage, -1, NULL, false );
        }
    }
}

static void cmsalvage_Delete( void * data )
{
    Entity  * e = playerPtr();
    DetailRecipe **ppRecipes=0;

    deleteme_sal = salvageFromTrayObject(data);

    if(!deleteme_sal || !deleteme_sal->salvage )
        return;

    estrClear(&deleteme_sal_string);
    estrConcatCharString(&deleteme_sal_string, textStd("ReallyDeleteSalvage", textStd(deleteme_sal->salvage->ui.pchDisplayName)));
        
    if( !optionGet(kUO_HideUsefulSalvagePrompt) && character_IsSalvageUseful(e->pchar,deleteme_sal->salvage,&ppRecipes) )
    {
        int i;
        estrConcatf(&deleteme_sal_string, "<br><br>%s<br>", textStd("SalvageDropUseful") );
        for( i = 0; i < eaSize(&ppRecipes); i++ )
            estrConcatf(&deleteme_sal_string, "%s<br>", textStd(ppRecipes[i]->ui.pchDisplayName) );
        eaDestroy(&ppRecipes);

        dialog( DIALOG_YES_NO, -1, -1, -1, -1, deleteme_sal_string, NULL, salvage_delete, NULL, NULL,0,sendOptions,deleteUsefulDCB,1,0,0,0 );
    }
    else
    {
        if (optionGet(kUO_HideDeleteSalvagePrompt))
            salvage_delete(0);
        else
            dialog( DIALOG_YES_NO, -1, -1, -1, -1, deleteme_sal_string, NULL, salvage_delete, NULL, NULL,0,sendOptions,deleteDCB,1,0,0,0 );
    }        

    sndPlay( "Reject5", SOUND_GAME );
}


static void salvage_deletestack(void * data)
{
    Entity  * e = playerPtr();
    if(deleteme_sal && deleteme_sal->salvage && !(deleteme_sal->salvage->flags & SALVAGE_NODELETE))
    {
        if( windowUp( WDW_BASE_STORAGE ) && WDW_BASE_STORAGE == salvage_context_wdw )
        {
            RoomDetail *detail = getStorageWindowDetail();
            basestorage_SendSalvageAdjust( detail->id, deleteme_sal->salvage->pchName, - (int) deleteme_sal->amount );
        } 
        else
        {
            bool is_stored_salvage = (WDW_STOREDSALVAGE == salvage_context_wdw);
            sendSalvageDelete( deleteme_sal->salvage->pchName,  deleteme_sal->amount, is_stored_salvage);
            if(!is_stored_salvage)
                character_AdjustSalvage(e->pchar, (const SalvageItem*) deleteme_sal->salvage, -((int) deleteme_sal->amount), NULL, false );
            else
                character_AdjustStoredSalvage(e->pchar, (const SalvageItem*) deleteme_sal->salvage, -((int) deleteme_sal->amount), NULL, false );
        }
    }
}

static void cmsalvage_DeleteStack( void * data )
{
    Entity  * e = playerPtr();
    DetailRecipe **ppRecipes=0;

    deleteme_sal = salvageFromTrayObject(data);

    if(!deleteme_sal || !deleteme_sal->salvage )
        return;

    estrClear(&deleteme_sal_string);
    estrConcatCharString(&deleteme_sal_string, textStd("ReallyDeleteSalvageStack", textStd(deleteme_sal->salvage->ui.pchDisplayName)));

    if( !optionGet(kUO_HideUsefulSalvagePrompt) && character_IsSalvageUseful(e->pchar,deleteme_sal->salvage,&ppRecipes) )
    {
        int i;
        estrConcatf(&deleteme_sal_string, "<br><br>%s<br>", textStd("SalvageDropUseful") );
        for( i = 0; i < eaSize(&ppRecipes); i++ )
            estrConcatf(&deleteme_sal_string, "%s<br>", textStd(ppRecipes[i]->ui.pchDisplayName) );
        eaDestroy(&ppRecipes);

        dialog( DIALOG_YES_NO, -1, -1, -1, -1, deleteme_sal_string, NULL, salvage_deletestack, NULL, NULL,0,sendOptions,deleteUsefulDCB,1,0,0,0 );
    }
    else
    {
        if (optionGet(kUO_HideDeleteSalvagePrompt))
            salvage_deletestack(0);
        else
            dialog( DIALOG_YES_NO, -1, -1, -1, -1, deleteme_sal_string, NULL, salvage_deletestack, NULL, NULL,0,sendOptions,deleteDCB,1,0,0,0 );
    }    
    sndPlay( "Reject5", SOUND_GAME );
}

///////////////////////////////////////////////////////////////////////

static const char *cmsalvage_Name( void * data )
{
    const SalvageInventoryItem *sal = salvageFromTrayObject(data);
    return sal?sal->salvage->ui.pchDisplayName:"Unknown";
}
static const char *cmsalvage_Desc( void * data )
{
    const SalvageInventoryItem *sal = salvageFromTrayObject(data);
    return sal?sal->salvage->ui.pchDisplayShortHelp:"Unknown";
}

static int cmsalvage_canMove( void * data )
{
    int wdw_cnt = 0;
    Entity *e = playerPtr();
    RoomDetail *detail = getStorageWindowDetail();
    const SalvageInventoryItem *sal = salvageFromTrayObject(data);

    if(!salvage_context_wdw || !detail || !data || !detail || !sal )
        return CM_HIDE;

    if( salvage_context_wdw == WDW_BASE_STORAGE ||
        salvage_context_wdw == WDW_TRADE )
    {
        if(entity_CanAdjStoredSalvage( e, detail, (SalvageItem*)sal->salvage, -1))
            return CM_AVAILABLE;
        else
            return CM_VISIBLE;
    }

    if( salvage_context_wdw == WDW_SALVAGE)
    {
        if(windowUp( WDW_BASE_STORAGE )) wdw_cnt++;
        if(windowUp( WDW_TRADE )) wdw_cnt++;

        if( wdw_cnt == 1 )
        {
            if(entity_CanAdjStoredSalvage( e, detail, (SalvageItem*)sal->salvage, 1))
                return CM_AVAILABLE;
            else
                return CM_VISIBLE;
        }
    }

    return CM_HIDE;
    
}

static int cmsalvage_canDelete( void * data )
{
    const SalvageInventoryItem *sal = salvageFromTrayObject(data);

    if ( !sal || !sal->salvage || sal->salvage->flags & SALVAGE_NODELETE)
        return CM_HIDE;

    return CM_AVAILABLE;
}

static int cmsalvage_canOpen( void * data )
{
    const SalvageInventoryItem *sal = salvageFromTrayObject(data);

    if ( !sal || !sal->salvage || sal->salvage->ppchRewardTables == NULL || ( sal->salvage->flags & SALVAGE_IMMEDIATE ) )
        return CM_HIDE;

    return CM_AVAILABLE;
}

static int cmsalvage_canUseNow( void * data )
{
    //
    //    Context menu visibility callback.
    //
    //    Determine if this is a salvage item which can have
    //    the "Use Now" context menu option.
    //
    const SalvageInventoryItem *sal = salvageFromTrayObject(data);

    if ( !sal || !sal->salvage || sal->salvage->ppchRewardTables == NULL || (! ( sal->salvage->flags & SALVAGE_IMMEDIATE )))
        return CM_HIDE;

    return CM_AVAILABLE;
}

static void salvage_open(void *data)
{
    Entity *e = playerPtr();
    if(!s_SalvageToOpen)
        return;

    if (inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_DOWN)
    {
        addSystemChatMsg( textStd("AccountServerDown"), INFO_SVR_COM, 0 );
        return;
    }

    sendSalvageOpen( s_SalvageToOpen->pchName, 1 );
    s_SalvageToOpen = NULL;
}

void salvage_confirmOpen(const SalvageItem *salvage)
{
    Entity *e = playerPtr();

    s_SalvageToOpen = salvage;

    if (s_SalvageToOpen != NULL && s_SalvageToOpen->ppchOpenRequires != NULL && !chareval_Eval(e->pchar, s_SalvageToOpen->ppchOpenRequires, "DEFS/INVENTION/SALVAGE.SALVAGE"))
    {
        dialogStd(DIALOG_OK, s_SalvageToOpen->pchDisplayOpenRequiresFail, NULL, NULL, NULL, NULL, 0);
        return;
    }

    estrClear(&s_SalvageToOpen_DlgPrompt);
    estrConcatCharString(&s_SalvageToOpen_DlgPrompt, textStd("ReallyOpenSalvage", textStd(s_SalvageToOpen->ui.pchDisplayName)));

    if (optionGet(kUO_HideOpenSalvagePrompt))
        salvage_open(0);
    else
        dialog( DIALOG_YES_NO, -1, -1, -1, -1, s_SalvageToOpen_DlgPrompt, NULL, salvage_open, NULL, NULL,0,sendOptions,openDCB,1,0,0,0 );
}

static void cmsalvage_Open( void * data )
{
    const SalvageInventoryItem *salvItem = salvageFromTrayObject(data);
    if (salvItem)
        salvage_confirmOpen(salvItem->salvage);
}

static void salvage_ExecUseNowOp(void *data)
{
    //
    //    Execute the "Use Now" operation
    //
    //    We've gotten an OK from the MapServer and the user has confirmed
    //    that he wants to go ahead with it.
    //
    char buffer[512];
    Entity  * e = playerPtr();
    if(!s_SalvageToUse)
        return;

    if (inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_DOWN)
    {
        addSystemChatMsg( textStd("AccountServerDown"), INFO_SVR_COM, 0 );
        return;
    }
    
    sprintf_s( buffer, ARRAY_SIZE(buffer), "open_salvage_by_name %s", s_SalvageToUse->salvage->pchName );
    cmdParse( buffer );
    estrClear(&s_SalvageToUse_DlgPrompt);
    s_SalvageToUse = NULL;
}

static void cmsalvage_UseNow( void * data )
{
    s_SalvageToUse = salvageFromTrayObject(data);
    if (( s_SalvageToUse != NULL ) && ( s_SalvageToUse->salvage != NULL ))
    {
        //
        //    User response handler for the context menu.
        //
        //    Send a request to the MapServer to verify that this is a valid
        //    operation for this user. MapServer will approve/reject and send
        //    a string ID we can display to the user in a confirmation dialog.
        //    
        sendGetSalvageImmediateUseStatus( s_SalvageToUse->salvage->pchName );
        //
        // Not done yet. This is just a request to the MapServer to see if
        // we can use this item. Until the MapServer responds, we need to
        // "remember" this item. So don't clear s_SalvageToUse.
        //
    }
}

void uiSalvage_ReceiveSalvageImmediateUseResp( const char* salvageName, U32 flags /*SalvageImmediateUseStatus*/, const char* msgId )
{
    // Got a reply back from the MapServer about the salvage item the
    // user wanted to immediately use.
    if (( flags & kSalvageImmediateUseFlag_NotApplicable ) ||
        ( s_SalvageToUse == NULL ) ||
        ( strcmp( salvageName, s_SalvageToUse->salvage->pchName ) != 0 ))
    {
        // This one can't be used immediately, or the item it is responding to
        // is not the one we're waiting to hear about.
        return;
    }

    // Translate the msg ID we got back from the MapServer and use it as
    // a dialog prompt string.
    estrDestroy( &s_SalvageToUse_DlgPrompt );
    estrPrintCharString( &s_SalvageToUse_DlgPrompt, textStd(msgId) ); 
    
    if ( flags & kSalvageImmediateUseFlag_MissingPrereqs )
    {
        dialog( DIALOG_OK, -1, -1, -1, -1, s_SalvageToUse_DlgPrompt, NULL, NULL, NULL, NULL,0,sendOptions,NULL,1,0,0,0 );
    }
    else
    {
        dialog( DIALOG_YES_NO, -1, -1, -1, -1, s_SalvageToUse_DlgPrompt, NULL, salvage_ExecUseNowOp, NULL, NULL,0,sendOptions,NULL,1,0,0,0 );
    }
}

static char * cmsalvage_moveOneText( void * data )
{
    if( windowUp( WDW_BASE_STORAGE ) && WDW_BASE_STORAGE != salvage_context_wdw )
        return "CMSalvageMoveOneToStorage";
    if( windowUp( WDW_TRADE ) && WDW_TRADE != salvage_context_wdw )
        return "CMSalvageMoveOneToTrade";
    if( WDW_SALVAGE != salvage_context_wdw )
        return "CMSalvageMoveOneToInventory";
    
    // shouldn't happen
    return "";
}

static char * cmsalvage_moveStackText( void * data )
{
    if( windowUp( WDW_BASE_STORAGE ) && WDW_BASE_STORAGE != salvage_context_wdw )
        return "CMSalvageMoveStackToStorage";
    if( windowUp( WDW_TRADE ) && WDW_TRADE != salvage_context_wdw )
        return "CMSalvageMoveStackToTrade";
    if( WDW_SALVAGE != salvage_context_wdw )
        return "CMSalvageMoveStackToInventory";

    // shouldn't happen
    return "";
}



static void cmsalvage_MoveOne( void * data )
{
    const SalvageInventoryItem *sal = salvageFromTrayObject(data);
    Entity * e = playerPtr();

    if( windowUp( WDW_BASE_STORAGE ) && WDW_BASE_STORAGE != salvage_context_wdw )
        salvageAddToStorageContainer(sal,1);
    else if( windowUp( WDW_TRADE ) && WDW_TRADE != salvage_context_wdw )
        salvMarkForTrade(e, character_FindSalvageInvIdx(e->pchar, sal), 1);
    else if( WDW_SALVAGE != salvage_context_wdw )
    {
        if( salvage_context_wdw == WDW_TRADE )
            salvRemoveFromTrade(character_FindSalvageInvIdx(e->pchar, sal), 1);
        if( salvage_context_wdw == WDW_BASE_STORAGE )
            salvageAddToStorageContainer(sal,-1);
    }
}

static void cmsalvage_MoveStack( void * data )
{
    const SalvageInventoryItem *sal = salvageFromTrayObject(data);
    Entity * e = playerPtr();

    if( windowUp( WDW_BASE_STORAGE ) && WDW_BASE_STORAGE != salvage_context_wdw )
        salvageAddToStorageContainer(sal,10000);
    else if( windowUp( WDW_TRADE ) && WDW_TRADE != salvage_context_wdw )
        salvMarkForTrade(e, character_FindSalvageInvIdx(e->pchar, sal), 0);
    else if( WDW_SALVAGE != salvage_context_wdw )
    {
        if( salvage_context_wdw == WDW_TRADE )
            salvRemoveFromTrade(character_FindSalvageInvIdx(e->pchar, sal), 0);
        if( salvage_context_wdw == WDW_BASE_STORAGE)
            salvageAddToStorageContainer(sal,-10000);
    }
}

/////////////////////////////////////////////////////////////////////////////////
static void salvage_sell(void * data)
{
    Entity  * e = playerPtr();
    if(!deleteme_sal)
        return;

    sendSalvageSell( deleteme_sal->salvage->pchName, 1 );
}

static void cmsalvage_Sell( void * data )
{
    Entity  * e = playerPtr();
    deleteme_sal = salvageFromTrayObject(data);

    dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallySellSalvage", NULL, salvage_sell, NULL, NULL,0,sendOptions,NULL,1,0,0,0 );
    sndPlay( "Reject5", SOUND_GAME );
}

static int cmsalvage_canSell( void * data )
{
    const SalvageInventoryItem *sal = salvageFromTrayObject(data);
    if (sal && sal->salvage->sellAmount > 0)
        return CM_AVAILABLE;
    else 
        return CM_HIDE;
} 
static void initSalavageContext()
{
    s_SalvageContext = contextMenu_Create(NULL);
    s_SalvageContextNoGift = contextMenu_Create(NULL);

    // the commands
    contextMenu_addVariableTitle( s_SalvageContext, cmsalvage_Name, 0);
    contextMenu_addVariableText( s_SalvageContext, cmsalvage_Desc, 0);
    contextMenu_addCode( s_SalvageContext, cmsalvage_canOpen, 0, cmsalvage_Open, 0, "CMOpenString", 0  );
    contextMenu_addCode( s_SalvageContext, alwaysAvailable, 0, cmsalvage_Info, 0, "CMInfoString", 0  );
    contextMenu_addCode( s_SalvageContext, cmsalvage_canDelete, 0, cmsalvage_Delete, 0, "CMRemoveSalvage", 0  );
    contextMenu_addCode( s_SalvageContext, cmsalvage_canDelete, 0, cmsalvage_DeleteStack, 0, "CMRemoveSalvageStack", 0  );
    contextMenu_addVariableTextCode( s_SalvageContext, cmsalvage_canMove, 0, cmsalvage_MoveOne, 0, cmsalvage_moveOneText, 0, 0  );
    contextMenu_addVariableTextCode( s_SalvageContext, cmsalvage_canMove, 0, cmsalvage_MoveStack, 0, cmsalvage_moveStackText, 0, 0  );

    contextMenu_addVariableTitle( s_SalvageContextNoGift, cmsalvage_Name, 0);
    contextMenu_addVariableText( s_SalvageContextNoGift, cmsalvage_Desc, 0);
    contextMenu_addCode( s_SalvageContextNoGift, cmsalvage_canOpen, 0, cmsalvage_Open, 0, "CMOpenString", 0  );
    contextMenu_addCode( s_SalvageContextNoGift, cmsalvage_canUseNow, 0, cmsalvage_UseNow, 0, "CMUseNowString", 0  );
    contextMenu_addCode( s_SalvageContextNoGift, alwaysAvailable, 0, cmsalvage_Info, 0, "CMInfoString", 0  );
    contextMenu_addCode( s_SalvageContextNoGift, cmsalvage_canDelete, 0, cmsalvage_Delete, 0, "CMRemoveSalvage", 0  );
    contextMenu_addCode( s_SalvageContextNoGift, cmsalvage_canDelete, 0, cmsalvage_DeleteStack, 0, "CMRemoveSalvageStack", 0  );
    contextMenu_addVariableTextCode( s_SalvageContextNoGift, cmsalvage_canMove, 0, cmsalvage_MoveOne, 0, cmsalvage_moveOneText, 0, 0  );
    contextMenu_addVariableTextCode( s_SalvageContextNoGift, cmsalvage_canMove, 0, cmsalvage_MoveStack, 0, cmsalvage_moveStackText, 0, 0  );

    gift_addToContextMenu(s_SalvageContext);
}

void salvageContextMenu(SalvageInventoryItem *sal, int wdw)
{
    int mx, my;
    static TrayObj to;
    if(!s_SalvageContext)
        initSalavageContext();

    if( wdw == WDW_SALVAGE )
    {
        to.type = kTrayItemType_Salvage;
        to.invIdx = character_FindSalvageInvIdx(playerPtr()->pchar,sal);
    }
    else if( wdw == WDW_STOREDSALVAGE )
    {
        to.type = kTrayItemType_PersonalStorageSalvage;
        to.invIdx = eaFind(&playerPtr()->pchar->storedSalvageInv,sal);
    }
    else
    {
        to.type = kTrayItemType_StoredSalvage;
        to.nameSalvage = sal->salvage->pchName;
    }

     rightClickCoords(&mx,&my);
    salvage_context_wdw = wdw;
    if( wdw == WDW_BASE_STORAGE || wdw == WDW_STOREDSALVAGE || 
        (sal->salvage && sal->salvage->flags & SALVAGE_NOTRADE))
        contextMenu_setAlign( s_SalvageContextNoGift, mx, my, CM_LEFT, CM_TOP, &to);
    else
        contextMenu_setAlign( s_SalvageContext, mx, my, CM_LEFT, CM_TOP, &to);
}

static char *s_sorttype_strs[] =
{
    "kSortType_Name_Ascending",
    "kSortType_Name_Descending",
    "kSortType_Amount_Ascending",
    "kSortType_Amount_Descending",
    "kSortType_Type_Ascending",
    "kSortType_Type_Descending",
    "kSortType_Rarity_Ascending",
    "kSortType_Rarity_Descending",
};
STATIC_ASSERT(ARRAY_SIZE(s_sorttype_strs) == kSalvageSortType_Count);

static bool sorttype_Valid(SalvageSortType s)
{
    return INRANGE0( s, kSalvageSortType_Count );
}

static char *sorttype_Str(SalvageSortType type)
{
    return verify(INRANGE0( type, ARRAY_SIZE(s_sorttype_strs))) ? s_sorttype_strs[type] : "<invalidtype>";
}

static int sorttype_FromStr(char const *str)
{
    int i = ARRAY_SIZE(s_sorttype_strs);
    if( str)
    {
        for( i = 0; i < ARRAY_SIZE( s_sorttype_strs )
                 && 0 != strcmp(str,sorttype_Str(i)); ++i )
        {
            // EMPTY
        }
    }
    return i;
}

SalvageSortType uiSalvage_SortTypeFromCombo(ComboBox *sortbyCombo)
{
    if(sortbyCombo)
    {
        ComboCheckboxElement *element = eaGet(&sortbyCombo->elements,sortbyCombo->currChoice);
        return sorttype_FromStr(SAFE_MEMBER(element,title));
    }
    return kSalvageSortType_Count;
}


MP_DEFINE(SalvageTabState);
static SalvageTabState* salvagetabstate_Create(void)
{
    SalvageTabState *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(SalvageTabState, 64);
    res = MP_ALLOC( SalvageTabState );
    if( verify( res ))
    {
    }
    // --------------------
    // finally, return

    return res;
}

static void salvagetabstate_Destroy( SalvageTabState *hItem )
{
    if(hItem)
    {
        MP_FREE(SalvageTabState, hItem);
    }
}

static SalvageWindowState s_state = {0};

static SalvageInventoryItem** s_GetPlayerSalvageInv()
{
    Character *pchar = playerPtr()->pchar;
    return pchar->salvageInv;
}


void salvageTabColorFunc(uiTabData foo, bool selected)
{
    Character *pchar = playerPtr()->pchar;
    int idx = (int) foo;

    if(selected)
    {
        if (idx == s_state.inventionInventoryIdx && 
            (pchar->salvageInvCurrentCount >= character_GetInvTotalSize(pchar, kInventoryType_Salvage)))
        {
            font_color(CLR_RED, CLR_RED);
        }
        else
        {
            font_color(CLR_WHITE, CLR_WHITE);
        }
    }
    else
    {
        if (idx == s_state.inventionInventoryIdx && 
            (pchar->salvageInvCurrentCount >= character_GetInvTotalSize(pchar, kInventoryType_Salvage)))
        {
            font_color(CLR_DARK_RED, CLR_DARK_RED);
        }
        else
        {
            font_color(CLR_TAB_INACTIVE, CLR_TAB_INACTIVE);
        }
    }
}

void uiSalvage_initSortCombo(WindowName wdw, ComboBox *sortbyCombo, bool include_type, bool include_rarity)
{
    static bool shared_created = false;
    static ComboCheckboxElement **trElements = NULL;
    static ComboCheckboxElement **tElements = NULL;
    static ComboCheckboxElement **rElements = NULL;
    static ComboCheckboxElement **elements = NULL;

    F32 wd_combo = 164.f;
    F32 ht_combo = 20.f;
    char **strs = NULL;

    ComboCheckboxElement **elts;
    int i;

    if(!shared_created)
    {
        for( i = 0; i < kSalvageSortType_Count; ++i )
        {
            comboboxSharedElement_add( &trElements, NULL, sorttype_Str(i), sorttype_Str(i), i, NULL );
            if(i == kSalvageSortType_Type_Ascending || i == kSalvageSortType_Type_Descending)
                eaPush(&tElements, trElements[i]);
            else if(i == kSalvageSortType_Rarity_Ascending || i == kSalvageSortType_Rarity_Descending)
                eaPush(&rElements, trElements[i]);
            else
            {
                eaPush(&tElements, trElements[i]);
                eaPush(&rElements, trElements[i]);
                eaPush(&elements, trElements[i]);
            }
        }
        shared_created = true;
    }

    if(!sortbyCombo)
        return;

    elts = include_type ? (include_rarity ? trElements : tElements)
                        : (include_rarity ? rElements : elements);
    comboboxClear(sortbyCombo);
    comboboxTitle_init(sortbyCombo, 0,0,0, wd_combo, ht_combo, ht_combo*eaSize(&elts), wdw );
    sortbyCombo->elements = elts;
    sortbyCombo->reverse = TRUE;
}

static void salvagetabstate_DestroyAdapter(void* arg0)
{
    salvagetabstate_Destroy((SalvageTabState *)arg0);
}

static void salvagewindowstate_Init(SalvageWindowState *state)
{
    int i;
    int nCombos = 0;
    cStashTableIterator iHash;
    StashElement eltHash;

    ZeroStruct( state );
    state->tc = uiTabControlCreate(TabType_Undraggable, s_tabSelectCb, 0, 0, salvageTabColorFunc, 0 );

    state->GetSalvageInv = s_GetPlayerSalvageInv;

    // ----------
    // do the tabs
    
    {
        char **tabs = NULL;
        
        eaClearEx(&state->tabStates, salvagetabstate_DestroyAdapter);
        
        state->inventionInventoryIdx = -1;
        
        // tab for 'all' salvage
        eaPush(&tabs, CATEGORY_ALL);
        
        // the rest of the tabs
        stashGetIteratorConst(g_SalvageDict.itemsFromTabName, &iHash);
        for(i = iTabCategoryAll + 1;stashGetNextElementConst(&iHash, &eltHash); ++i)
        {
            const char *cat = stashElementGetStringKey(eltHash);
            eaPushConst(&tabs,cat);
        }
        nCombos = i;
        
        // sort the tab names
        qsort(tabs + 1, nCombos - 1, sizeof(tabs[0]), tabNamesSort);
        
        // add the tabs
        for( i = 0; i < nCombos; ++i )
        {
            bool include_type = false;
            bool include_rarity = false;
            char *cat = tabs[i];
            
            uiTabControlAdd(state->tc, cat, (void*)i);
            verify(i == eaPush( &state->tabStates, salvagetabstate_Create()));

            if (stricmp("P1273912828", cat) == 0)
                state->inventionInventoryIdx = i;

            // init the sort-by combo
            if(i == iTabCategoryAll)
            {
                include_type = true;
                include_rarity = true;
            }
            else // determine if this tab has more than one 'type' and rarity
            {
                const char *type = NULL;
                SalvageRarity rarity;
                int j;

                const SalvageItem **items = stashFindPointerReturnPointerConst(g_SalvageDict.itemsFromTabName, cat);
                int count = eaSize(&items);
                bool type_found = false; // in case NULL is a legitimate pchDisplayShortHelp
                bool rarity_found = false;

                for(j = 0; j < count && !(include_type && include_rarity); ++j)
                {
                    if(items[j])
                    {
                        if(!type_found)
                        {
                            type = items[j]->ui.pchDisplayShortHelp;
                            type_found = true;
                        }
                        else if(((!type || !items[j]->ui.pchDisplayShortHelp) && type != items[j]->ui.pchDisplayShortHelp) ||
                            strcmp(type,items[j]->ui.pchDisplayShortHelp))
                        {
                            include_type = true;
                        }

                        if(!rarity_found)
                        {
                            rarity = items[j]->rarity;
                            rarity_found = true;
                        }
                        else if(rarity != items[j]->rarity)
                        {
                            include_rarity = true;
                        }
                    }
                }
            }

            uiSalvage_initSortCombo(WDW_SALVAGE, &state->tabStates[i]->sortbyCombo, include_type, include_rarity);
        }
        
        eaDestroy(&tabs);
    }
    
    // ----------
    // items
    
    eaCreate(&state->ppItems);
    
    // ----------
    // init the growbig
    
    uigrowbig_Init( &state->growbig, 1.1, 10 );
}



static void s_fillFromInv(SalvageWindowState *s, SalvageInventoryItem **src)
{
    int size = eaSize(&src);
    int i;

    if (src != NULL)
    {
        eaSetCapacity( &s->ppItems, size + eaSize( &s->ppItems ));

        // need to iterate as some may not be valid.
        for( i = 0; i < size; ++i )
        {
            if( salvageinventoryitem_Valid( src[i] ) )
            {
                eaPush(&s->ppItems, src[i]);
            }
        }
    }
}


//------------------------------------------------------------
//  sort the items given the current tab state
//----------------------------------------------------------
#define UISAL_NAME        0
#define UISAL_TABNAME    1
#define UISAL_SHORTHELP    2

// remember pointers to all the string table strings for a super-fast salvage window
static const char* s_getTranslated(SalvageInventoryItem const *sal, int which)
{
    static const char **names = NULL;
    static const char **tabnames = NULL;
    static const char **shorthelps = NULL;

    const char *ret = NULL;

    if(!names)
    {
        int i;
        int count = eaSize(&g_SalvageDict.itemsById);

        eaSetSizeConst(&names,count);
        eaSetSizeConst(&tabnames,count);
        eaSetSizeConst(&shorthelps,count);

        for(i = 0; i < count; ++i)
        {
            SalvageItem const *salvage = eaGetConst(&g_SalvageDict.itemsById,i);
            if(salvage)
            {
                names[i] = textStdUnformattedConst(salvage->ui.pchDisplayName);
                tabnames[i] = textStdUnformattedConst(salvage->pchDisplayTabName);
                shorthelps[i] = textStdUnformattedConst(salvage->ui.pchDisplayShortHelp);
            }
        }
    }

    if(sal && sal->salvage && EAINRANGE((int)sal->salvage->salId,names)) // always the same size
    {
        if(which == UISAL_NAME)
            ret = names[sal->salvage->salId];
        else if(which == UISAL_TABNAME)
            ret = tabnames[sal->salvage->salId];
        else if(which == UISAL_SHORTHELP)
            ret = shorthelps[sal->salvage->salId];
    }

    return ret ? ret : "";
}

int uiSalvageSort(const void* item1, const void* item2, void const *context)
{
    SalvageInventoryItem const *lhs = *(SalvageInventoryItem**)item1;
    SalvageInventoryItem const *rhs = *(SalvageInventoryItem**)item2;
    SalvageSortType sortBy = (SalvageSortType)context;

    if( verify(lhs && rhs && lhs->salvage && rhs->salvage && sorttype_Valid( sortBy )))
    {
        switch ( sortBy )
        {
            case kSalvageSortType_Name_Ascending:
                return strcmp(s_getTranslated(lhs,UISAL_NAME),s_getTranslated(rhs,UISAL_NAME));

            case kSalvageSortType_Name_Descending:
                return strcmp(s_getTranslated(rhs,UISAL_NAME),s_getTranslated(lhs,UISAL_NAME));

            case kSalvageSortType_Amount_Ascending:
                return lhs->amount - rhs->amount;

            case kSalvageSortType_Amount_Descending:
                return rhs->amount - lhs->amount;
    
            case kSalvageSortType_Type_Ascending:
            {
                int res = strcmp(s_getTranslated(lhs,UISAL_TABNAME),s_getTranslated(rhs,UISAL_TABNAME));
                if(!res)
                    res = strcmp(s_getTranslated(lhs,UISAL_SHORTHELP),s_getTranslated(rhs,UISAL_SHORTHELP));
                return res;
            }

            case kSalvageSortType_Type_Descending:
            {
                int res = strcmp(s_getTranslated(rhs,UISAL_TABNAME),s_getTranslated(lhs,UISAL_TABNAME));
                if(!res)
                    res = strcmp(s_getTranslated(rhs,UISAL_SHORTHELP),s_getTranslated(lhs,UISAL_SHORTHELP));
                return res;
            }

            case kSalvageSortType_Rarity_Ascending:
                return lhs->salvage->rarity - rhs->salvage->rarity;

            case kSalvageSortType_Rarity_Descending:
                return rhs->salvage->rarity - lhs->salvage->rarity;

            default:
                verify(0); // invalid state
        };
        STATIC_INFUNC_ASSERT(kSalvageSortType_Count == 8);
    }

    return 0;
}


static void s_initSalvageTabState(SalvageInventoryItem **salvageInv, int selectedTab, SalvageSortType sort)
{
    if( verify( s_state.tc && EAINRANGE(selectedTab, s_state.tabStates) && INRANGE0(sort, kSalvageSortType_Count)))
    {
        SalvageTabState *tabstate = s_state.tabStates[selectedTab];
        tabstate->sortby = sort;
        combobox_setId(&tabstate->sortbyCombo, sort, TRUE );

        // --------------------
        // fill the items from player's inventory

        eaSetSize( &s_state.ppItems, 0);
        {
            char *category = uiTabControlGetTabName(s_state.tc, selectedTab);
            SalvageInventoryItem **src = NULL;

            if (selectedTab == s_state.inventionInventoryIdx)
                category = "P1273912828";
            src = SalvageInventoryItem_GatherMatching(salvageInv, category);
            s_fillFromInv(&s_state, src);
        }

        // --------------------
        // sort

        // first sort by name ascending
        stableSort( s_state.ppItems, eaSize(&s_state.ppItems), sizeof(*s_state.ppItems), (void*)kSalvageSortType_Name_Ascending, uiSalvageSort);

        // then the given sort
        stableSort( s_state.ppItems, eaSize(&s_state.ppItems), sizeof(*s_state.ppItems), (void*)s_state.tabStates[selectedTab]->sortby, uiSalvageSort);
    }
}


static void s_tabSelectCb(void*data)
{
    int tab = (int)data;
    if(verify( EAINRANGE(tab, s_state.tabStates) && s_state.tabStates[tab]))
    {
        s_initSalvageTabState( s_state.GetSalvageInv(), tab, s_state.tabStates[tab]->sortby );
    }
}

int salvagePrintName(SalvageItem const *salvage, float x, float y, float z, float wd, float sc)
{
    char buf[1024];
    char *color = "White";
    salvageSMF *smf;

    if(!salvage)
        return 0;

    color = g_salvageColor[salvage->rarity];
    sprintf_s(buf, 1024, "<color %s>%s</color>", color, textStd(salvage->ui.pchDisplayName));

    smf = findOrAddSMF(sc,salvage);
    if (smf->smf->iLastHeight == 0)
    {
        smf->smf->iLastHeight = smf_ParseAndFormat(smf->smf, buf, 0, 0, 0, wd, 1000, false, false, false, &gTextAttr, NULL );
        smf->smf->pBlock->pos.alignHoriz = SMAlignment_Center;
        return smf_ParseAndDisplay( smf->smf, buf, x, y, z, wd, smf->smf->iLastHeight, false, true, &gTextAttr, NULL, 0, true);
    }
    else
    {
        return smf_ParseAndDisplay( smf->smf, buf, x, y, z, wd, smf->smf->iLastHeight, false, false, &gTextAttr, NULL, 0, true);
    }
}

#define TAB_HT        20
#define BOTTOM_HT   24

const char* salvage_display(SalvageInventoryItem *sal, uiGrowBig *growbig, int wdw, float x, float y, float z, float sc, CBox *resClientArea, 
                      SalvageDragCb salvage_drag_cb, SalvageClickCb salvage_click_cb, int hide_name)
{
    const char *res = NULL; 
    //char *name = NULL;
    AtlasTex *icon = NULL;
    AtlasTex *overlay = NULL;
    int color = CLR_WHITE;
    float ty, tx;
    F32 z_icon = z + ARRAY_SIZE( growbig->items ) + 10; // the max z an icon will occupy
    F32 z_amount = z_icon + 1;
    Character *pchar = playerPtr()->pchar;
    static char *infoText = NULL;  // estr

    if( !verify( sal && sal->salvage && growbig && pchar))
    {
        return res;
    }

    ty = y + PIX3*2*sc;
    tx = x + (COL_WD/2 - ICON_WD/2)*sc;

    // --------------------
    // print the amount

    font( &game_9 );
    font_color(0x00deffff, 0x00deffff);
    prnt( tx + -4*sc, ty + 6*sc, z_amount, sc, sc, "%d", sal->amount );

    // --------------------
    // draw the icon

    icon = atlasLoadTexture(sal->salvage->ui.pchIcon);

    // draw the icon
    if( icon )
    {
        // first get the default scale
        F32 xsc = ((F32)ICON_WD)/icon->width;

        // get the over scale
        int zmod = ARRAY_SIZE(growbig->items);
        F32 overScale = uigrowbig_GetScale( growbig, sal, &zmod );

        xsc *= overScale; // set if mouse over
        display_sprite( icon, tx-(ICON_WD*(sc*(overScale-1.0)))/2.0, ty-(ICON_WD*(sc*(overScale-1.0)))/2.0, z_icon-zmod, sc*xsc, sc*xsc, color );
    }

    if(!hide_name)
        salvagePrintName(sal->salvage,x+(SPACER_WD*sc),y+(ICON_WD+6)*sc,z,(COL_WD-SPACER_WD*2)*sc,sc);


    // --------------------
    // handle interaction

    {
        CBox box;
         BuildCBox( &box, x, y, COL_WD*sc, ROW_HT*sc); // always square

        if( mouseCollision(&box) )
        {
            char *rarity = "Common";

            // set the info text
            res = sal->salvage->ui.pchDisplayShortHelp;
            setCursor( "cursor_hand_open.tga", NULL, FALSE, 2, 2 );


            // 
            if (infoText == NULL)
                estrCreate(&infoText);
            else
                estrClear(&infoText);
            switch (sal->salvage->rarity)
            {
            case kSalvageRarity_Ubiquitous:
                rarity = "Ubiquitous";
                break;
            case kSalvageRarity_Common:
            default:
                rarity = "Common";
                break;
            case kSalvageRarity_Uncommon:
                rarity = "Uncommon";
                break;
            case kSalvageRarity_Rare:
                rarity = "Rare";
                break;
            case kSalvageRarity_VeryRare:
                rarity = "VeryRare";
                break;
            case kSalvageRarity_Unique:
                rarity = "Unique";
                break;
            }
            estrConcatf(&infoText,"%s (%s)", textStd(res), textStd(rarity));

            if (sal->salvage->flags & SALVAGE_NOTRADE)
                estrConcatf(&infoText, " %s", textStd("SalvageIsNoTrade"));

            if (sal->salvage->flags & SALVAGE_NOAUCTION)
                estrConcatStaticCharArray(&infoText, " salvage not auctionable");

            if (sal->salvage->flags & SALVAGE_NODELETE)
                estrConcatf(&infoText, " %s", textStd("SalvageIsNoDelete"));

            res = infoText;
        }
        // set the context menu
        if( wdw && mouseClickHit( &box, MS_RIGHT ) )
        {
            salvageContextMenu(sal, wdw);
        }

        if (salvage_drag_cb != NULL && 
            mouseLeftDrag( &box ) && 
            verify(!cursor.dragging)) // just for my understanding
        {
            salvage_drag_cb(sal, icon, sc * (((F32)ICON_WD)/icon->width));
        }
        else if(mouseClickHit(&box,MS_LEFT))
        {        
            if( windowUp( WDW_BASE_STORAGE ) )
                salvageAddToStorageContainer(sal,1);
            if( windowUp( WDW_TRADE ) )
                salvMarkForTrade(playerPtr(), character_FindSalvageInvIdx(playerPtr()->pchar, sal), 1);
            if (salvage_click_cb != NULL)
                salvage_click_cb(sal);
        }
/*
        else if( mouseDoubleClickHit(&box, MS_LEFT))
        {
            if(windowUp( WDW_BASE_STORAGE ) )
                salvageAddToStorageContainer(sal,10000);
            if(windowUp( WDW_TRADE ) )
                salvMarkForTrade(playerPtr(), character_FindSalvageInvIdx(playerPtr()->pchar, sal), 10000);
        }
*/        // --------------------
        // open the info window

        // --------------------
        // save the dims

        if( resClientArea )
        {
            *resClientArea = box;
        }
    }

    // --------------------
    // finally

    return res;
}

// for the salvage_display callback
void salvage_dragEx(SalvageInventoryItem *sal, AtlasTex *icon, float sc)
{
    Entity *e = playerPtr();
    int iInv = e ? character_FindSalvageInvIdx( e->pchar, sal) : -1;
    
    // drag the concept
    if(iInv != -1)
        salvage_drag(iInv,icon,sc);
}

void salvage_drag(int index, AtlasTex *icon, float sc) {
    TrayObj to;
    to.type = kTrayItemType_Salvage;
    to.invIdx = index;
    to.scale = sc;
    trayobj_startDraggingEx(&to, icon, NULL, sc);
}


static void salvagecm_reHelper( SalvageInventoryItem *sal, int amount )
{
    if( verify( sal && sal->salvage && playerPtr() && salvage_ValidRarity( sal->salvage->rarity ) && (U32)amount <= sal->amount ))
    {
        int i;
        Character *pchar = playerPtr()->pchar;
        int invId = character_FindSalvageInvIdx( pchar, sal );

        if( verify( invId >= 0 ) )
        {
            for( i = 0; i < amount; ++i )
            {
                crafting_reverseengineer_send( invId );
            }

            //todo: if this uses up all the inventory, need to grey out the buttons on the info box
        }
    }
}

static void salvagecm_reOne( void *data )
{
    SalvageInventoryItem *sal = (SalvageInventoryItem*)data;
    salvagecm_reHelper( sal, 1 );
}

static void salvagecm_reAll( void *data )
{
    SalvageInventoryItem *sal = (SalvageInventoryItem*)data;
    salvagecm_reHelper( sal, sal->amount );
}


static int tabNamesSort(const void* name1Data, const void* name2Data)
{
    const char** name1 = (const char**)name1Data;
    const char** name2 = (const char**)name2Data;
    static char temp[64];
    strcpy(temp,textStd((char *)*name1));
    return stricmp( temp,textStd((char *)*name2) );
}

static float s_salvage_flash;

void salvageWindowFlash(void)
{
    s_salvage_flash = .0001f;
}

void UISalvage_DisplayItems(WindowName wdw, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc, ScrollBar *sb, SalvageInventoryItem **items, uiGrowBig *growbig, bool stored_salvage, CBox *resBoxItms, CBox *resBoxBtm, SalvageDragCb salvage_drag_cb)
{
    F32 edge_off = (R10+PIX3)*sc;
    int i;
    int size = eaSize(&items);
    U32 scaledIconWd = COL_WD * sc;
    const char *infoToDisplay = NULL;
    SalvageInventoryItem *mouseOverSalvage = NULL;
    CBox boxItms = {0};
    CBox boxBtm = {0};
    F32 ht_top = (PIX3+TAB_HT)*sc;
    F32 wd_border = PIX3*sc;
    F32 x_items = x+PIX3*sc;
    F32 ht_btm = BOTTOM_HT *sc; // there are a lot of assumptions in here
    F32 wd_items = wd - wd_border*2;
    F32 wd_btm = wd_items;
    F32 x_btm = x_items;
    F32 y_btm = y + ht;
    
    // take a little more off the border for scroll bar and growing
    U32 NUM_COLS = (wd_items - wd_border)/scaledIconWd;
    
    // --------------------
    // handle the items
    
    set_scissor(1);
    scissor_dims( x_items, y, wd_items, ht );
    BuildCBox( &boxItms, x_items, y, wd_items, ht);
    
    for( i = 0; i < size; ++i )
    {
        int row = i/NUM_COLS;
        int col = i%NUM_COLS;
        F32 col_off = COL_WD*col*sc;
        F32 row_off = ROW_HT*row*sc;
        SalvageInventoryItem *salInv = items[i];
        const char *info = NULL;
        // display the item, unless it's off screen
        if (y-sb->offset+row_off + ROW_HT*sc < y)
            continue;
        else if (y-sb->offset+row_off > y+ht)
            continue;
        info = salvage_display( salInv, growbig, wdw, boxItms.left+col_off, y-sb->offset+row_off, z, sc, NULL, salvage_drag_cb, NULL, 0 );
        
        if( info )
        {
            infoToDisplay = info;
            mouseOverSalvage = salInv;
        }
    }
    set_scissor(0);
    
    //scrollbar
    {
        F32 y_wdw = 0, x_wdw = 0;
        F32 doc_bottom = y + (size+NUM_COLS-1)/NUM_COLS*scaledIconWd;
        window_getDims(wdw, &x_wdw, &y_wdw, NULL, NULL, NULL, NULL, NULL, NULL);
        doScrollBar( sb, ht - edge_off*2, doc_bottom - y, wd, y - y_wdw + edge_off, z+5, 0, 0 );
    }
    
    // grow the current icon
    uigrowbig_Update(growbig, mouseOverSalvage );
    
    // --------------------
    // handle the area at the bottom
    
    BuildCBox( &boxBtm, x_btm, y_btm, wd_btm, ht_btm);
    
    // --------------------
    // display info text
    {
        float x_txt_off = 10;
        float y_txt_off_from_bottom = 1*PIX4;
        F32 x_txt = boxBtm.left + x_txt_off*sc;
        F32 y_txt = boxBtm.bottom - y_txt_off_from_bottom*sc;
        F32 wd_txt = 256*sc;
        F32 ht_txt = 22*sc;
        int colorfg = CLR_MOUSEOVER_FOREGROUND;
        int colorbg = CLR_MOUSEOVER_BACKGROUND;
        
        // if there is text, draw it
        if( infoToDisplay )
        {
            font( &game_9 );
            font_color(CLR_WHITE, CLR_WHITE);
            prnt( x_txt, y_txt, z+1, sc, sc, infoToDisplay );
        }
        
        // drawn inventory count
//         if (game_state.enable_invention)
//         {
//             int x = (boxBtm.right - 230.f*sc);
//             Character *pchar = playerPtr()->pchar;
//             int cur_count = stored_salvage?pchar->storedSalvageInvCurrentCount : pchar->salvageInvCurrentCount;
            
//             font( &game_9 );
//             if (cur_count < character_GetInvTotalSize(pchar, kInventoryType_Salvage))
//                 font_color(CLR_WHITE, CLR_WHITE);
//             else
//                 font_color(CLR_RED, CLR_RED);
//             prnt( x, y_txt, z+1, sc, sc, "%d/%d", cur_count, character_GetInvTotalSize(pchar, stored_salvage?kInventoryType_StoredSalvage : kInventoryType_Salvage) );
//         }
        
    }
    if(resBoxBtm)
        *resBoxBtm = boxBtm;
    if(resBoxItms)
        *resBoxItms = boxItms;
}

void salvageDrawCallback(char *userData, float x, float y, float z, float sc, int clr)
{
    const SalvageItem *salvage = salvage_GetItem(userData);
    if(salvage)
        display_sprite(atlasLoadTexture(salvage->ui.pchIcon), x, y, z, sc, sc, clr);
}


int salvageWindow()
{
    CBox boxItms = {0};    
    CBox boxBtm = {0};    
    F32 wd_btm = 0, ht_btm = 0;
    float x, y, z, wd, ht, sc;
//     F32 view_y;
    F32 edge_off;
    int color, bcolor;
    static ScrollBar sb = { WDW_SALVAGE, 0};
    static bool drewLast = false;
    Character *pchar;
    SalvageTabState *tabState = NULL;
    int curTab = -1;

     if(!window_getDims(WDW_SALVAGE, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
    {
        drewLast = false;
        s_bReparse = 1;
        return 0;
    }

    /*
    if( window_getMode(WDW_WORKSHOP) != WINDOW_DOCKED )
    {
        window_setMode( WDW_SALVAGE, WINDOW_DOCKED );
        return 0;
    }
    */

    pchar = playerPtr()->pchar;
    if( !s_state.tc )
    {
        salvagewindowstate_Init(&s_state);
    }

    // ----------------------------------------
    //post-init

    curTab = uiTabControlGetSelectedIdx( s_state.tc );
    tabState = s_state.tabStates[curTab];
    edge_off = (R10+PIX3)*sc;

    // --------------------
    // window frame
    if( s_salvage_flash )
    {
        int flash = sinf(s_salvage_flash)*128;
        bcolor |= ((flash*2)<<24)|(flash<<16)|(flash<<8)|(flash);
        s_salvage_flash += TIMESTEP/4;
        if( s_salvage_flash > PI )
            s_salvage_flash = 0.f;
    }
    drawFrame(PIX3, R10, x, y, z, wd, ht-(BOTTOM_HT-2)*sc, sc, color, bcolor);
    drawFrame(PIX2, R10, x, y+(ht-(BOTTOM_HT*sc)), z, wd, (BOTTOM_HT)*sc, sc, color, bcolor);
    
    // if not ready yet, return
      if(window_getMode(WDW_SALVAGE) != WINDOW_DISPLAYING)
        return 0;

    // --------------------
    // check for an inventory state change
    // When salvage inventory changes, s_bReparse will get set

    if( !drewLast || verify(EAINRANGE( curTab, s_state.tabStates )))
    {
        int size = 0;
        int i;
        bool invalidFound = false;

        // check if any pointed to items have become invalid
        for( i = eaSize( &s_state.ppItems ) - 1; i >= 0 && !invalidFound; --i)
        {
            invalidFound = !salvageinventoryitem_Valid(s_state.ppItems[i]);
        }

        if( s_bReparse || invalidFound)
        {
            s_initSalvageTabState( s_state.GetSalvageInv(), curTab, tabState->sortby );
            s_bReparse = false;
        }
    }
    else
    {
        //invalid state
        drewLast = 0;
        return verify(0);
    }

    // --------------------
    // tabs
    drawTabControl(s_state.tc, x+edge_off*sc, y, z, wd - edge_off*2*sc, 15, sc, color, color, TabDirection_Horizontal);

    // display window internals
    UISalvage_DisplayItems(WDW_SALVAGE, x,y+(PIX3+TAB_HT)*sc,z,wd,ht - BOTTOM_HT*sc - (PIX3+TAB_HT)*sc - PIX3*sc,sc,&sb,s_state.ppItems, &s_state.growbig, FALSE, &boxItms, &boxBtm, salvage_dragEx);
    wd_btm = CBoxWidth(&boxBtm);
    ht_btm = CBoxHeight(&boxBtm);

    // --------------------
    // update the sort

    {
        // do to feature of combobox, need to multiply by inverse scale
        F32 x_adj = (wd_btm - 158.f*sc)/sc;
        F32 y_adj = (ht - ht_btm)/sc;
        
        combobox_setLoc(&tabState->sortbyCombo, x_adj, y_adj, -1);
        combobox_display(&tabState->sortbyCombo);
        
        if( tabState->sortbyCombo.newlySelectedItem )
        {
            SalvageSortType sort = uiSalvage_SortTypeFromCombo(&tabState->sortbyCombo);
            if(sort != kSalvageSortType_Count)
                s_initSalvageTabState( s_state.GetSalvageInv(), curTab, sort);
            tabState->sortbyCombo.newlySelectedItem = FALSE;
        }
    }

    // drawn inventory count
    if (s_state.inventionInventoryIdx >= 0)
    {
        char name[256];
        sprintf_s(name, 256, "%s (%d/%d)", textStd("P1273912828"), pchar->salvageInvCurrentCount, character_GetInvTotalSize(pchar, kInventoryType_Salvage) );
        uiTabControlRename(s_state.tc, name, (void *) s_state.inventionInventoryIdx);
    }

    // --------------------
    // drag and drop interaction
    
    // drop storage salvage 
    if ( cursor.dragging && mouseCollision(&boxItms) 
         && !isDown(MS_LEFT)) 
    {
        switch ( cursor.drag_obj.type )
        {
        case kTrayItemType_StoredSalvage:
        {
            Character *pchar = playerPtr()->pchar;
            const char *nameSalvage = cursor.drag_obj.nameSalvage;
            int idDetail = cursor.drag_obj.idDetail;
            
            salvageAddToStorageContainer(getStoredSalvageFromName(nameSalvage), -10000);
            trayobj_stopDragging();
        }
        break;
        case kTrayItemType_PersonalStorageSalvage:
        {
            Character *pchar = playerPtr()->pchar;
            SalvageInventoryItem *sal = eaGet(&pchar->storedSalvageInv,cursor.drag_obj.invIdx);
            if(sal&&sal->salvage)
                amountSliderWindowSetupAndDisplay(1,sal->amount, NULL, salvageDrawCallback, storedsalvage_MoveToPlayerInv, cpp_const_cast(char*)(sal->salvage->pchName), 0);
        }
        break;
        default: // some unknown type
            break;
        };
    }

    // -----
    // finally

    drewLast = true;
    return 0;
}

/* End of File */
