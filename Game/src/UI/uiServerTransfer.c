#include <utilitieslib/stdtypes.h>
#include "UI/uiServerTransfer.h"
#include "account/AccountData.h"
#include <utilitieslib/utils/timing.h>
#include "clientcomm/dbclient.h"
#include "UI/uiDialog.h"
#include "UI/uiLogin.h"
#include "account/AccountCatalog.h"
#include <utilitieslib/utils/Cbox.h>
#include "UI/uiScrollBar.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/textureatlas.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "cmdparse/cmdgame.h"
#include "formatter/smf_util.h"
#include "formatter/smf_main.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiClipper.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/language/MessageStore.h>
#include "clientcomm/authclient.h"
#include "graphics/ttFontUtil.h"
#include "uiListView.h"
#include "UI/uiDialog.h"
#include "sound/sound.h"
#include "UI/uiWindows.h"
#include <utilitieslib/utils/file.h>
#include "UI/uiInput.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"

// This adds three servers to the destination shard list plus tests two
// failure cases for adding shards (same shard twice and shard with no
// free slots). Useful internally.
//#define CHAR_TRANSFER_DEST_SHARD_TEST

// Column identifiers and layout information for the destination shard
// This is the longest a shard name can be including the terminating null.
#define CHAR_TRANSFER_MAX_SHARD_NAME        60

// The longest error message string we expect to be sent from the server.
#define CHARACTER_TRANSFER_MAX_ERROR_MSG    100

// picker list view widget.
#define CHAR_TRANSFER_COLUMN_SHARD            "DestShard"
#define CHAR_TRANSFER_COLUMN_FREE_SLOTS        "FreeSlots"

// This is the starting size for the memory pool for destination shard
// structures.
#define CHAR_TRANSFER_DEST_SHARD_POOL_SIZE    3

// This is how long we wait for the order server to send back the full
// list of destination shard data for character transfer.
#define CHAR_TRANSFER_GET_SHARDS_TIMEOUT    10

// This is how we track our progress through the transfer UI. It's a
// four step process for transfers. First we pick a shard (all shards
// returned by the account server except our own), then we show a
// confirmation to the user. Lastly we show a progress dialog.
typedef enum CharacterTransferType
{
    CHARACTER_TRANSFER_IDLE,
    CHARACTER_TRANSFER_WARNING_DIALOG,
    CHARACTER_TRANSFER_GET_SHARD_LIST,
    CHARACTER_TRANSFER_PICK_SHARD,
    CHARACTER_TRANSFER_SHARD_LIST_ERROR,
    CHARACTER_TRANSFER_UNDERWAY,
} CharacterTransferType;

// We build up an array of these structures as data comes back from the
// order server about all the places we can transfer to.
typedef struct DestinationShard
{
    char shardName[CHAR_TRANSFER_MAX_SHARD_NAME];
    int freeSlots;
} DestinationShard;

// We use this memory pool for holding all the destination shards passed
// back from the account server.
MP_DEFINE(DestinationShard);

static void startTransferPickShard(void);
static void cleanupTransferShardList(CharacterTransferType next_state);
static void startTransferUnderway(void);
static void drawTransferProgressDialog(void);
static bool isCharacterTransferGettingShardList(void);

static CharacterTransferType characterTransferState = CHARACTER_TRANSFER_IDLE;
static DestinationShard** characterTransferShards = NULL;
static U32 characterTransferDestShardsStartTime = 0;
static bool characterTransferShardsComplete = false;
static char characterTransferDestShardName[CHAR_TRANSFER_MAX_SHARD_NAME];
static char characterTransferErrorMsg[CHARACTER_TRANSFER_MAX_ERROR_MSG];

// Main UI control for picking the destination shard.
static UIListView* characterTransferShardList = NULL;

static StoreExitCallback s_exitcb = NULL;

static void transferInfoCancelCallback(void * data) {
    cleanupTransferShardList(CHARACTER_TRANSFER_IDLE);
}

static void transferConfirmOkCallback(void * data)
{
    startTransferUnderway();
}

static void transferStartReceiveCharacterCounts(void)
{
    // We need to know when we made the request so we can time out
    // if it takes too long.
    characterTransferDestShardsStartTime = timerSecondsSince2000();
    characterTransferShardsComplete = false;

    // Set up a memory pool to hold all the destination shards.
    MP_CREATE( DestinationShard, CHAR_TRANSFER_DEST_SHARD_POOL_SIZE );
}

static void startCharacterTransfer()
{
    sndPlay( "select", SOUND_GAME );

    transferStartReceiveCharacterCounts();

#if defined( CHAR_TRANSFER_DEST_SHARD_TEST )
    characterTransferAddCharacterCount( "Pinnacle", 9, 0 );
    characterTransferAddCharacterCount( "Freedom", 5, 0 );
    characterTransferAddCharacterCount( "Champion", 11, 0 );

    // Now test the invalid cases. All slots used, duplicate shard.
    characterTransferAddCharacterCount( "Virtue", 12, 0 );
    characterTransferAddCharacterCount( "Freedom", 6, 0 );
#endif

    dbGetCharacterCountsAsync();
}

static void transferInfoOkCallback(void * data)
{
    // Transfers have the extra step of the player needing to pick a shard.
    // For everything else, it must either be a generic product (range is
    // inclusive) or a special predefined product (range is inclusive at
    // the bottom and exclusive at the top). See ProductId in AccountData.h
    // for the definitions of all these.
    characterTransferState = CHARACTER_TRANSFER_GET_SHARD_LIST;

    startCharacterTransfer();
}

static void handleTransferInfoDialog(void)
{
    static SMFBlock* s_smfTitle = NULL;
    static SMFBlock* s_smfDialog = NULL;
    static int use_mockup = 0;

    dialogStd(DIALOG_YES_NO, "XferTokenSpendConfirmation", 0, 0, transferInfoOkCallback, transferInfoCancelCallback, 0);
}

static void finishTransferGetShardList(void)
{
    if( eaSize( &characterTransferShards ) > 0 )
    {
        startTransferPickShard();
    }
    else
    {
        // We got no suitable destination shards so they get the
        // error dialog instead.
        characterTransferReceiveError( "charTransferNoDestShard" );
    }
}

static void handleTransferGetShardList(void)
{
    if( characterTransferShardsComplete )
        finishTransferGetShardList();
    else
    {
        // We can't stall too long waiting for the order server to come
        // back with the destination shard information for us.
        if( (timerSecondsSince2000() - characterTransferDestShardsStartTime) >    CHAR_TRANSFER_GET_SHARDS_TIMEOUT )
        {
            strcpy_s( characterTransferErrorMsg, CHARACTER_TRANSFER_MAX_ERROR_MSG, "charTransferPreBusy" );
            cleanupTransferShardList( CHARACTER_TRANSFER_SHARD_LIST_ERROR );
        }
    }
}

static void handleTransferPickShard(float screenScaleX, float screenScaleY)
{
    U32 button_color;
    PointFloatXYZ list_pos;
    const char* server_name;
    char banner_buf[100];
    float screenScale = MIN(screenScaleX, screenScaleY);
    char *PayString = "OkString";
    button_color = SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE );

    server_name = msGetUnformattedMessageConst( menuMessages, auth_info.servers[gSelectedDbServer].name );

    strcpy_s( banner_buf, ARRAY_SIZE_CHECKED( banner_buf ),    textStd( "charTransferPickShardBanner", server_name ) );

    drawMenuFrame( R12, 82*screenScaleX, 159*screenScaleY, 130, 400*screenScaleX, 550*screenScaleY, CLR_WHITE, 0x44444444, 0 );

    font_color( CLR_WHITE, CLR_WHITE );
    cprntEx( 282*screenScaleX, 191*screenScaleY, 140, screenScale, screenScale, ( NO_MSPRINT | CENTER_X | CENTER_Y ), banner_buf );

    drawMenuFrame( R12, 82*screenScaleX, 159*screenScaleY, 130, 400*screenScaleX, 64*screenScaleY, CLR_WHITE, 0x44444444, 0 );

    if( D_MOUSEHIT == drawStdButton( 165*screenScaleX, 684*screenScaleY, 140, 50*screenScaleX, 20*screenScaleY, button_color, "CancelString", screenScale, 0 ) )
        cleanupTransferShardList( CHARACTER_TRANSFER_IDLE );

    if (db_info.shardXferFreeOnly)
    {
        cprntEx( 360*screenScaleX, 665*screenScaleY, 140, screenScale, screenScale, ( NO_MSPRINT | CENTER_X | CENTER_Y ), textStd("AccountOrderShardXferToken", db_info.shardXferTokens) );
        if( D_MOUSEHIT == drawStdButton( 400*screenScaleX, 684*screenScaleY, 140, 50*screenScaleX, 20*screenScaleY, button_color, "paymentToken" , screenScale, (db_info.shardXferTokens > 0) ?  0 : BUTTON_LOCKED) )
        {
            db_info.shardXferTokens--;
            cleanupTransferShardList( CHARACTER_TRANSFER_UNDERWAY );
        }
    }
    else
    {
        if( D_MOUSEHIT == drawStdButton( 400*screenScaleX, 684*screenScaleY, 140, 50*screenScaleX, 20*screenScaleY,    button_color, PayString, screenScale, 0 ) )
        {
            cleanupTransferShardList( CHARACTER_TRANSFER_UNDERWAY );
        }
    }
    if( characterTransferShardList )
    {
        static ScrollBar sb;
        CBox cbox;

        list_pos.x = 102*screenScaleX;
        list_pos.y = 231*screenScaleY;
        list_pos.z = 140;
        uiLVHSetWidth(characterTransferShardList->header, 360*screenScaleX);
        uiLVSetHeight(characterTransferShardList, 400 *screenScaleY);
        uiLVSetScale(characterTransferShardList, screenScale);

        BuildCBox(&cbox, list_pos.x, list_pos.y, 360*screenScaleX, 400 *screenScaleY);

        uiLVSetDrawColor( characterTransferShardList, button_color );
        uiLVDisplay( characterTransferShardList, list_pos );
        uiLVHandleInput( characterTransferShardList, list_pos );
        {
            int fullDrawHeight = uiLVGetFullDrawHeight(characterTransferShardList);
            int currentHeight = uiLVGetHeight(characterTransferShardList);
            if(fullDrawHeight > currentHeight)
            {
                doScrollBarEx( &characterTransferShardList->verticalScrollBar, (currentHeight-40)/screenScaleY, fullDrawHeight, 
                    (cbox.hx+10*screenScale)/screenScaleX, (cbox.ly+20)/screenScaleY, 20, &cbox, 0, screenScaleX, screenScaleY);
                characterTransferShardList->scrollOffset = characterTransferShardList->verticalScrollBar.offset*screenScaleY;
            }
            else
            {
                characterTransferShardList->scrollOffset = 0;
            }
        }

    }
}

static void storeDialogCallbackExit(void *data)
{
    characterTransferState = CHARACTER_TRANSFER_IDLE;

    if (s_exitcb)
    {
        s_exitcb();
    }
}

static void handleTransferShardListError(void *data)
{
    // There was nowhere they could go to - either we didn't
    // get sent any other shards, or they're all full.
    dialogStd( DIALOG_OK, characterTransferErrorMsg, NULL, NULL, storeDialogCallbackExit, NULL, 0 );
}

static void handleTransferUnderway(void)
{
    static SMFBlock* s_smfTitle = NULL;
    static SMFBlock* s_smfDialog = NULL;

    // This is a 'successfully requested' prompt. The transfer
    // will happen on its own time and the AccountServer will send us a
    // success or failure message eventually.
    dialogStd( DIALOG_OK, "charTransferInProgressWait", NULL, NULL,    storeDialogCallbackExit, NULL, 0 );
}

void characterTransferHandleProgress(float screenScaleX, float screenScaleY)
{
    switch (characterTransferState)
    {
    case CHARACTER_TRANSFER_WARNING_DIALOG:
        handleTransferInfoDialog();
        break;

    case CHARACTER_TRANSFER_GET_SHARD_LIST:
        handleTransferGetShardList();
        break;

    case CHARACTER_TRANSFER_PICK_SHARD:
        handleTransferPickShard(screenScaleX, screenScaleY);
        break;

    case CHARACTER_TRANSFER_SHARD_LIST_ERROR:
        handleTransferShardListError(NULL);
        break;

    case CHARACTER_TRANSFER_UNDERWAY:
        handleTransferUnderway();
        break;

    case CHARACTER_TRANSFER_IDLE:
    default:
        break;
    }
}

UIBox displayShardListViewItem(UIListView* list, PointFloatXYZ row_origin, void* user_settings, void* dest_shardData, int item_index)
{
    DestinationShard* dest_shard = (DestinationShard*)dest_shardData;
    UIBox retval;
    UIColumnHeaderIterator column_iterator;
    float xpos;
    float ypos;
    int str_width;
    char slots_str[12];

    devassert( list );
    devassert( dest_shard );
    devassert( dest_shard->shardName );

    if( dest_shard == list->selectedItem )
    {
        font_color( CLR_GREEN, CLR_GREEN );
    }
    else
    {
        font_color( CLR_WHITE, CLR_WHITE );
    }

    if( list && dest_shard && dest_shard->shardName )
    {
        uiCHIGetIterator( list->header, &column_iterator );
        uiCHIGetNextColumn( &column_iterator, list->scale );
        xpos = row_origin.x + ( column_iterator.columnStartOffset * list->scale ) + 30;
        ypos = row_origin.y + 18.0f * list->scale;

        cprntEx( xpos, ypos, 140 + 1, list->scale, list->scale, 0, dest_shard->shardName );

        sprintf_s( slots_str, 12, "%d", dest_shard->freeSlots );
        strDimensionsBasic(font_get(), 1.0f, 1.0f, slots_str, strlen(slots_str), &str_width, NULL, NULL);

        uiCHIGetNextColumn( &column_iterator, list->scale );
        xpos = row_origin.x + ( column_iterator.columnStartOffset * list->scale ) +    90 - str_width;
        ypos = row_origin.y + 18.0f * list->scale;

        cprntEx( xpos, ypos, 140 + 1, list->scale, list->scale, NO_MSPRINT, "%d", dest_shard->freeSlots );
    }

    retval.origin.x = row_origin.x;
    retval.origin.y = row_origin.y;
    retval.height = 0;
    retval.width = list->header->width;

    return retval;
}



static int destinationShardsCompareNames(const DestinationShard** first, const DestinationShard** second)
{
    int retval;

    // Everything in the array should have been pre-screened.
    devassert( first );
    devassert( *first );
    devassert( second );
    devassert( *second );

    retval = 0;
    if( first && *first && second && *second )
    {
        retval = stricmp( (*first)->shardName, (*second)->shardName );
    }

    return retval;
}

static int destinationShardsCompareFreeSlots(const DestinationShard** first, const DestinationShard** second)
{
    int retval;

    // Everything in the array should have been pre-screened.
    devassert( first );
    devassert( *first );
    devassert( second );
    devassert( *second );

    retval = 0;

    if( first && *first && second && *second )
    {
        // Note that the sort order is reversed here because we want the
        // natural order to be that the shards with the most slots come
        // first and those with the least come last.
        retval = (*second)->freeSlots - (*first)->freeSlots;
    }

    return retval;
}

static void startTransferPickShard(void)
{
    int count;

    // We can't stay in the old state because it'll time out or do
    // something else we don't want.
    characterTransferState = CHARACTER_TRANSFER_PICK_SHARD;

    // Make sure we don't leak resources from an old shard list.
    if( characterTransferShardList )
    {
        uiLVDestroy( characterTransferShardList );
    }

    characterTransferShardList = uiLVCreate();
    devassert( characterTransferShardList );
    devassert( characterTransferShardList && characterTransferShardList->header );

    if( characterTransferShardList && characterTransferShardList->header )
    {
        uiLVHAddColumnEx( characterTransferShardList->header, CHAR_TRANSFER_COLUMN_SHARD, "charTransferShard", 160, 160, 0 );
        uiLVBindCompareFunction( characterTransferShardList, CHAR_TRANSFER_COLUMN_SHARD, destinationShardsCompareNames );

        uiLVHAddColumnEx( characterTransferShardList->header, CHAR_TRANSFER_COLUMN_FREE_SLOTS, "charTransferFreeSlots", 120, 120, 0 );
        uiLVBindCompareFunction( characterTransferShardList, CHAR_TRANSFER_COLUMN_FREE_SLOTS, destinationShardsCompareFreeSlots );

        for( count = 0; count < eaSize( &characterTransferShards ); count++ )
        {
            uiLVAddItem( characterTransferShardList, eaGet( &characterTransferShards, count ) );
        }

        uiLVHSetWidth( characterTransferShardList->header, 360 );
        uiLVSetScale( characterTransferShardList, 1.0f );
        uiLVSetWidth( characterTransferShardList, 360 );
        uiLVSetHeight( characterTransferShardList,    400 );
        uiLVFinalizeSettings( characterTransferShardList );

        uiLVEnableSelection( characterTransferShardList, 1 );
        uiLVEnableMouseOver( characterTransferShardList, 1 );

        characterTransferShardList->displayItem = displayShardListViewItem;

        characterTransferShardList->header->selectedColumn = 1;
        uiLVSortBySelectedColumn( characterTransferShardList );

        uiLVSelectItem( characterTransferShardList, 0 );
    }
    else
    {
        // We couldn't put up the server list, so saying there was nowhere
        // for them to go is as graceful an exit as any.
        dialogStd( DIALOG_OK, "charTransferNoDestShard", NULL, NULL,
            storeDialogCallbackExit, NULL, 0 );
    }
}

static void cleanupTransferShardList(CharacterTransferType next_state)
{
    DestinationShard* dest_shard;

    dest_shard = eaGet( &characterTransferShards, 0 );

    if( characterTransferShardList )
    {
        dest_shard = characterTransferShardList->selectedItem;

        uiLVDestroy( characterTransferShardList );
        characterTransferShardList = NULL;
    }

    if( dest_shard )
    {
        strcpy_s( characterTransferDestShardName,
            CHAR_TRANSFER_MAX_SHARD_NAME, dest_shard->shardName );
    }

    eaDestroy( &characterTransferShards );
    MP_DESTROY( DestinationShard );

    switch( next_state )
    {
        // These are the valid states we can transition to.
    case CHARACTER_TRANSFER_UNDERWAY:
        startTransferUnderway();
        break;

    case CHARACTER_TRANSFER_SHARD_LIST_ERROR:
    case CHARACTER_TRANSFER_IDLE:
        characterTransferState = next_state;
        break;

    default:
        // Assert in a descriptive fashion. 
        devassert( next_state == CHARACTER_TRANSFER_GET_SHARD_LIST || next_state == CHARACTER_TRANSFER_IDLE );

        characterTransferState = CHARACTER_TRANSFER_IDLE;
        break;
    }

    // Make sure we leave to a different master state if we're exiting
    // from the transfer state overall.
    if (characterTransferState == CHARACTER_TRANSFER_IDLE && s_exitcb)
    {
        s_exitcb();
    }
}

static void startTransferUnderway(void)
{
    dbRedeemShardXferToken(auth_info.uid, db_info.players[gPlayerNumber].name, gPlayerNumber, characterTransferDestShardName);

    characterTransferState = CHARACTER_TRANSFER_UNDERWAY;
}

bool characterTransferAddCharacterCount(char* destShard, int unlockedCount, int totalCount, int accountSlotCount, int serverSlotCount)
{
    bool retval;
    int count;
    bool done;
    DestinationShard* new_dest_shard;
    int freeSlots;

    retval = false;

    // Being returned negative used slots is a protocol error. Zero used
    // is fine, because that means they're all free. More slots used than
    // the player can normally own is also okay because there are non-UI
    // ways for characters to be created.
    devassert( unlockedCount >= 0 );
    devassert( totalCount >= 0 );

    // Don't let the free slots go negative just in case we decide to
    // add full servers to the list greyed out or something.
    freeSlots = MIN(accountSlotCount + getVIPAdjustedBaseSlots() - unlockedCount, serverSlotCount - totalCount); // Make sure both the account and server limits are good.
    freeSlots = MAX(freeSlots, 0);

    done = false;

    // Just skip all the work if we were called out of sync (eg. because
    // we timed out just before the data arrived).
    if( !isCharacterTransferGettingShardList() )
    {
        done = true;
    }

    // We shouldn't offer the server we're already on because that would
    // effectively be renaming rather than transferring.
    if( !done && isProductionMode() &&
        ( stricmp( auth_info.servers[gSelectedDbServer].name, destShard ) == 0 ) )
    {
        done = true;
    }

    count = 0;

    // Make sure we don't already have an entry for this destination shard.
    while( !done && count < eaSize( &characterTransferShards ) )
    {
        if( stricmp( characterTransferShards[count]->shardName, destShard ) == 0 )
        {
            done = true;
        }

        count++;
    }

    // Duplicate or empty servers don't need to be added because they are
    // never suitable destinations and as such can never be shown to the
    // player.
    if( !done && freeSlots > 0 )
    {
        new_dest_shard = MP_ALLOC( DestinationShard );
        devassert( new_dest_shard );

        if ( new_dest_shard )
        {
            strcpy_s( new_dest_shard->shardName,
                CHAR_TRANSFER_MAX_SHARD_NAME, destShard );
            new_dest_shard->freeSlots = freeSlots;

            eaPush( &characterTransferShards, new_dest_shard );

            retval = true;
        }
    }

    return retval;
}

bool characterTransferReceiveCharacterCountsDone(void)
{
    bool retval = false;

    // Ignore attempted finalisations if we're not expecting data.
    if( isCharacterTransferGettingShardList() )
    {
        if( eaSize( &characterTransferShards ) > 0 )
        {
            retval = true;
        }
    }

    // Whether we got a valid list or not, the account server won't be
    // sending us any more shard information.
    characterTransferShardsComplete = true;

    return retval;
}

bool characterTransferReceiveError(char* errorMsg)
{
    bool retval;

    devassert( errorMsg );

    retval = false;

    if( errorMsg && isCharacterTransferGettingShardList() )
    {
        strcpy_s( characterTransferErrorMsg,
            CHARACTER_TRANSFER_MAX_ERROR_MSG, errorMsg );
        cleanupTransferShardList( CHARACTER_TRANSFER_SHARD_LIST_ERROR );

        retval = true;
    }

    return retval;
}

static bool isCharacterTransferGettingShardList(void)
{
    bool retval;

    retval = false;

    if( characterTransferState == CHARACTER_TRANSFER_WARNING_DIALOG ||
        characterTransferState == CHARACTER_TRANSFER_GET_SHARD_LIST )
    {
        retval = true;
    }

    return retval;
}

void serverTransferStartRedeem(StoreExitCallback exitCB)
{
    if (exitCB)
    {
        s_exitcb = exitCB;
    }

    characterTransferState = CHARACTER_TRANSFER_WARNING_DIALOG;
}

bool characterTransferAreSlotsHidden(void)
{
    return characterTransferState == CHARACTER_TRANSFER_PICK_SHARD;
}
