

#include "entity/entity.h"
#include "entity/friendCommon.h"
#include "gameComm/wdwbase.h"
#include "player/player.h"
#include "entity/entVarUpdate.h"
#include "cmdparse/cmdgame.h"
#include "language/langClientUtil.h"

#include "graphics/ttFont.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/textureatlas.h"

#include "UI/uiFriend.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiWindows.h"
#include "UI/uiInput.h"
#include "UI/uiTarget.h"
#include "UI/uiCursor.h"
#include "UI/uiChat.h"
#include "UI/uiScrollBar.h"
#include "UI/uiContextMenu.h"
#include "uiListView.h"
#include <utilitieslib/components/earray.h>
#include "UI/uiClipper.h"
#include "graphics/ttFontUtil.h"

#include "UI/uiComboBox.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiDialog.h"
#include "win/input.h"
#include "chatdb.h"
#include "gameComm/chatClient.h"
#include "UI/uiChatUtil.h"
#include "UI/uiChannel.h"
#include <utilitieslib/utils/mathutil.h>
#include "entity/character_target.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "uiLevelingpact.h"
#include "entity/teamCommon.h"

static int friendListRebuildRequired = 0;
static int globalFriendListRebuildRequired = 0;
static int globalIgnoreListRebuildRequired = 0;
static int levelingpactListRebuildRequired = 0;

static int oldFriendSelectionDBID = 0;
static int levelingpactSelectionDBID = 0;
static char oldGlobalFriendSelectionHandle[128];
static char oldGlobalIgnoreSelectionHandle[128];
static char comboSelectionTitle[128] = {0};

static UIListView* friendListView;
static UIListView *levelingpactListView;
static UIListView * globalFriendListView;
static UIListView * globalIgnoreListView;
static UIListView * channelView[MAX_WATCHING];

ContextMenu *interactHandleOffline        = 0;
ContextMenu *interactHandleExternalChat = 0;
ContextMenu *interactGlobalIgnoreList    = 0;

static float prev_wd;

extern ContextMenu *interactPlayerNotOnMap;    // borrowing from uiTarget.c
void initGlobalFriendContextMenu();
void initGlobalIgnoreContextMenu();

/* Function friendListPrepareUpdate()
 *    Should be called when the player's friend list changes.
 *  This function sets flag to have the friend listview rebuild its list.  It also
 *    saves the listview's current selection so that the selection can be restored
 *    after the list is rebuilt.
 */
void friendListPrepareUpdate()
{
    if(friendListView && friendListView->selectedItem)
        oldFriendSelectionDBID = ((Friend*)friendListView->selectedItem)->dbid;
    if(globalFriendListView && globalFriendListView->selectedItem)
    {
        strncpyt(oldGlobalFriendSelectionHandle, ((GlobalFriend*)globalFriendListView->selectedItem)->handle, sizeof(oldGlobalFriendSelectionHandle));
        globalFriendListView->selectedItem = 0;
    }

    friendListRebuildRequired = 1;
    globalFriendListRebuildRequired = 1;
}

void levelingpactListPrepareUpdate()
{
    if(levelingpactListView && levelingpactListView->selectedItem)
        levelingpactSelectionDBID = ((Friend*)levelingpactListView->selectedItem)->dbid;

    levelingpactListRebuildRequired = 1;
    channelListRebuildRequired = 1;
}

void globalIgnoreListPrepareUpdate()
{

    if(globalIgnoreListView && globalIgnoreListView->selectedItem)
    {
        strncpyt(oldGlobalIgnoreSelectionHandle, ((ChatUser*)globalIgnoreListView->selectedItem)->handle, sizeof(oldGlobalIgnoreSelectionHandle));
        globalIgnoreListView->selectedItem = 0;
    }

    globalIgnoreListRebuildRequired = 1;
}

void friendListReset()
{
    friendListPrepareUpdate();
    globalIgnoreListPrepareUpdate();
    uiLVClear(friendListView);
    uiLVClear(globalFriendListView);
    uiLVClear(globalIgnoreListView);
}

void globalFriendListRemove(GlobalFriend * gf)
{
    uiLVRemoveItem(globalFriendListView, gf);
    friendListPrepareUpdate();
}

//-------------------------------------------------------------------------------------------------------------------
// Context menu related functions
//-------------------------------------------------------------------------------------------------------------------
// Used by context menus to add the current selected entity to the friend list.
void addToFriendsList(void* useless)
{
    char buf[128];
    char* targetName = targetGetName();

    if(!targetName)
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
        return;
    }

    sprintf( buf, "friend %s", targetName);
    cmdParse( buf );
}

// Used by context menus to remove the current selected entity from the friend list.
void removeFromFriendsList(void* useless)
{
    char buf[128];
    char* targetName = targetGetName();

    if(!targetName)
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
        return;
    }

    sprintf(buf, "unfriend %s", targetName);
    cmdParse(buf);
}

// Context menu item visiblity function.
// Allows the item to be displayed if the selected target is a friend.
static int entityIsFriend(int dbid)
{
    int i;
    Entity *e = playerPtr();
    if(!dbid)
        return 0;
    for( i = 0; i < e->friendCount; i++ )
    {
        if( dbid == e->friends[i].dbid )
            return 1;
    }

    return 0;
}

int isFriend(void* foo)
{
    int dbid;

    dbid = targetGetDBID();
    if(!dbid)
        return CM_HIDE;
    
    if(entityIsFriend(dbid))
        return CM_AVAILABLE;
    else
        return CM_HIDE;
}


// Context menu item visiblity function.
// Allows the item to be displayed if the selected target is not a friend.
int isNotFriend(void *foo)
{
    if( isFriend(NULL) == CM_AVAILABLE )
        return CM_HIDE;
    else
        return CM_AVAILABLE;        
}


/* Function friendNameCompare()
 *    Compares the name of two friends.
 *    Used to sort the friend list view by the name column.
 */
int friendNameCompare(const Friend** f1, const Friend** f2)
{
    const Friend* friend1 = *f1;
    const Friend* friend2 = *f2;
    return stricmp(friend1->name, friend2->name); 
}

/* Function friendStatusCompare()
*    Compares the name of two friends.
*    Used to sort the friend list view by the status column.
*/
int friendStatusCompare(const Friend** f1, const Friend** f2)
{
    const Friend* friend1 = *f1;
    const Friend* friend2 = *f2;

    if(friend1->online > friend2->online)
    {
        return -1;
    }
    else if(friend1->online < friend2->online)
    {
        return 1;
    }
    else
        return stricmp(friend1->name, friend2->name);
}

// Leave this amount of space at the bottom of the window to display the "Remove" button.
#define FRIEND_REMOVE_AREA        40        



UIBox friendListViewDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, void* itemData, int itemIndex)
{
    Friend* item = (Friend*)itemData;
    UIBox box;
    PointFloatXYZ pen = rowOrigin;
    UIColumnHeaderIterator columnIterator; 
    int color;

    box.origin.x = rowOrigin.x;
    box.origin.y = rowOrigin.y;

    uiCHIGetIterator(list->header, &columnIterator);

    rowOrigin.z+=1;

    if(item == list->selectedItem)
    {
        color = CLR_WHITE;
    }
    else if(item->online)
    {
        color = CLR_CONSTRUCT(99, 225, 252, 255);
    }
    else
    {
        color = CLR_CONSTRUCT(120, 120, 120, 255);
    }

    font_color(color, color);

    // Iterate through each of the columns.
    for(;uiCHIGetNextColumn(&columnIterator, list->scale);)
    {
        UIColumnHeader* column = columnIterator.column;
        UIBox clipBox;
        pen.x = rowOrigin.x + columnIterator.columnStartOffset*list->scale;

        clipBox.origin.x = pen.x;
         clipBox.origin.y = pen.y;
        clipBox.height = 20*list->scale;

        if( columnIterator.columnIndex >= eaSize(&columnIterator.header->columns )-1 )
            clipBox.width = 1000;
        else
            clipBox.width = columnIterator.currentWidth;

        clipperPushRestrict(&clipBox);
        // Decide what to draw on on which column.
        if(stricmp(column->name, FRIEND_NAME_COLUMN) == 0)
        {
            // Draw the friend name.
             int rgba[4] = {color, color, color, color };
            printBasic(font_grp, pen.x, pen.y+18*list->scale, rowOrigin.z, list->scale, list->scale, 0, item->name, strlen(item->name), rgba );
        }
        else
        {
            // Draw the friend status
            if(!item->online)
                prnt(pen.x, pen.y+18*list->scale, pen.z, list->scale, list->scale, "FriendOffline", item->name);
            else
                prnt(pen.x, pen.y+18*list->scale, pen.z, list->scale, list->scale, item->mapname);
        }
        clipperPop();
    }

    box.height = 0;
    box.width = list->header->width;

    // Returning the bounding box of the item does not do anything right now.
    // See uiListView.c for more details.
    return box;
}

UIBox levelingpactListViewDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, void* itemData, int itemIndex)
{
    Friend* item = (Friend*)itemData;
    UIBox box;
    PointFloatXYZ pen = rowOrigin;
    UIColumnHeaderIterator columnIterator; 
    int color;

    box.origin.x = rowOrigin.x;
    box.origin.y = rowOrigin.y;

    uiCHIGetIterator(list->header, &columnIterator);

    rowOrigin.z+=1;

    if(item == list->selectedItem)
    {
        color = CLR_WHITE;
    }
    else
    {
        color = CLR_CONSTRUCT(99, 225, 252, 255);
    }

    font_color(color, color);

    // Iterate through each of the columns.
    for(;uiCHIGetNextColumn(&columnIterator, list->scale);)
    {
        UIColumnHeader* column = columnIterator.column;
        UIBox clipBox;
        pen.x = rowOrigin.x + columnIterator.columnStartOffset*list->scale;

        clipBox.origin.x = pen.x;
        clipBox.origin.y = pen.y;
        clipBox.height = 20*list->scale;

        if( columnIterator.columnIndex >= eaSize(&columnIterator.header->columns )-1 )
            clipBox.width = 1000;
        else
            clipBox.width = columnIterator.currentWidth;

        clipperPushRestrict(&clipBox);
        // Decide what to draw on on which column.
        if(stricmp(column->name, FRIEND_NAME_COLUMN) == 0)
        {
            // Draw the friend name.
            int rgba[4] = {color, color, color, color };
            printBasic(font_grp, pen.x, pen.y+18*list->scale, rowOrigin.z, list->scale, list->scale, 0, item->name, strlen(item->name), rgba );
        }
        
        clipperPop();
    }

    box.height = 0;
    box.width = list->header->width;

    // Returning the bounding box of the item does not do anything right now.
    // See uiListView.c for more details.
    return box;
}



/* Function friendWindow()
*    Responsible for displaying the friends list window.
*/


int oldFriendWindow(float x, float y, float z, float wd, float ht, float scale, int color, int bcolor, void * data)
{
     Entity *e = playerPtr();
    PointFloatXYZ pen;
    UIBox windowDrawArea;
    UIBox listViewDrawArea;
    static ScrollBar sb = {WDW_FRIENDS,0};

    wdwGetWindow(WDW_FRIENDS)->loc.draggable_frame = RESIZABLE;


    font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);
    // Do everything common windows are supposed to do.
    //    if ( !window_getDims( WDW_FRIENDS, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ))
    //        return 0;

    // Draw the base friends list frame.
    //    drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, bcolor );

    // Where in the window can we draw?
    windowDrawArea.x = x;
    windowDrawArea.y = y;
    windowDrawArea.width = wd;
    windowDrawArea.height = ht;
    uiBoxAlter(&windowDrawArea, UIBAT_SHRINK, UIBAD_ALL, PIX3*scale);

    listViewDrawArea = windowDrawArea;
    uiBoxAlter(&listViewDrawArea, UIBAT_SHRINK, UIBAD_BOTTOM, FRIEND_REMOVE_AREA*scale);

    // The list view will be drawn inside the listViewDrawArea and above the window frame.
    pen.x = listViewDrawArea.x;
    pen.y = listViewDrawArea.y;
    pen.z = z+1;                    // Draw on top of the frame

    // Do nothing if the friendlist is empty.
    if( e->friendCount == 0 )
    {
        int fontHeight = ttGetFontHeight(font_grp, scale, scale);
        clipperPushRestrict(&windowDrawArea);
        cprnt( pen.x + wd/2, pen.y+fontHeight, pen.z, scale, scale, "NoFriendsString" );
        clipperPop();
        return 0;
    }

    // Draw and handle all scroll bar functionaltiy if the friendlist is larger than the window.
    // This is seperate from the list view because the scroll bar is always outside the draw area
    // of the list view.
    if(friendListView)
    {
        int fullDrawHeight = uiLVGetFullDrawHeight(friendListView);
        int currentHeight = uiLVGetHeight(friendListView);
        if(friendListView)
        {
            if(fullDrawHeight > currentHeight)
            {
                doScrollBar( &sb, currentHeight, fullDrawHeight, wd, PIX3+R10*scale, z+2, 0, &windowDrawArea  );
                friendListView->scrollOffset = sb.offset;
            }
            else
            {
                friendListView->scrollOffset = 0;
            }
        }
    }

    // Make sure the list view is initialized.
    if(!friendListView)
    {
        Wdw* friendsWindow = wdwGetWindow(WDW_FRIENDS);

        friendListView = uiLVCreate();

        uiLVHAddColumnEx(friendListView->header, FRIEND_NAME_COLUMN, "friendName", 10, 270, 1);
        uiLVBindCompareFunction(friendListView, FRIEND_NAME_COLUMN, friendNameCompare);

        uiLVHAddColumn(friendListView->header, FRIEND_STATUS_COLUMN, "friendStatus", 0);
        uiLVBindCompareFunction(friendListView, FRIEND_STATUS_COLUMN, friendStatusCompare);

        uiLVFinalizeSettings(friendListView);

        friendListView->displayItem = friendListViewDisplayItem;

        uiLVEnableMouseOver(friendListView, 1);
        uiLVEnableSelection(friendListView, 1);
    }

    // How tall and wide is the list view supposed to be?
    uiLVSetDrawColor(friendListView, window_GetColor(WDW_FRIENDS));
    uiLVSetHeight(friendListView, listViewDrawArea.height);
      uiLVHFinalizeSettings(friendListView->header);
    uiLVHSetWidth(friendListView->header, listViewDrawArea.width);
    uiLVSetScale( friendListView, scale );
    // Rebuild the entire friends list if required.
    if(friendListView && friendListRebuildRequired)
    {
        int i;

        uiLVClear(friendListView);
        for(i = 0; i < e->friendCount; i++)
        {
            uiLVAddItem(friendListView, &e->friends[i]);
            if(e->friends[i].dbid == oldFriendSelectionDBID)
                uiLVSelectItem(friendListView, i);
        }

        uiLVSortBySelectedColumn(friendListView);

        friendListRebuildRequired = 0;
    }

    // Disable mouse over on window resize.
    {
        Wdw* friendsWindow = wdwGetWindow(WDW_FRIENDS);
        if(friendsWindow->drag_mode)
            uiLVEnableMouseOver(friendListView, 0);
        else
            uiLVEnableMouseOver(friendListView, 1);
    }

    // Draw the draw the list and clip everything to inside the proper drawing area of the list view.
    clipperPushRestrict(&listViewDrawArea);
    uiLVDisplay(friendListView, pen);
    clipperPop();

    // Handle list view input.
    uiLVHandleInput(friendListView, pen);

    // Did the user pick someone from the friends list?
    if(friendListView->newlySelectedItem && friendListView->selectedItem)
    {
        // Which entity was selected?
        Friend* item = friendListView->selectedItem;
        Entity* player = entFromDbId(item->dbid);

        if(player)
        {
            if(player != playerPtr() && character_TargetIsFriend(playerPtr()->pchar, player->pchar))
            {
                targetSelect(player);
            }
        }
        else
        {
            targetSelect2(item->dbid, item->name);
        }

        friendListView->newlySelectedItem = 0;
    }

    // If the target is a friend...
    if(entityIsFriend(targetGetDBID()))
    {
        int i;
        if(current_target)
        {
            for( i = 0; i < eaSizeUnsafe(&friendListView->items); i++ )
            {
                Friend *item = friendListView->items[i];
                if(item->dbid == current_target->db_id)
                    friendListView->selectedItem = item;
            }
        }
    }
    else
        friendListView->selectedItem = 0;

    if(friendListView->selectedItem)
    {
        int index;

        index = eaFind(&friendListView->items, friendListView->selectedItem);
        if(index != -1 && uiMouseCollision(friendListView->itemWindows[index]) && mouseRightClick())
        {    
            if(current_target)
                contextMenuForTarget(current_target);
            else
                contextMenuForOffMap();
        }
    }

    // Clip and draw the remove friend button
    listViewDrawArea.y += listViewDrawArea.height;
    listViewDrawArea.height = FRIEND_REMOVE_AREA*scale;
    clipperPushRestrict(&listViewDrawArea);
    if(D_MOUSEHIT == drawStdButton(x + wd/2, y + ht - (20+PIX3)*scale, z, 75*scale, 26*scale, window_GetColor(WDW_FRIENDS), "RemoveString", scale, !((int)friendListView->selectedItem)))
    {
        char buf[128];
        if(friendListView && friendListView->selectedItem)
        {
            sprintf( buf, "unfriend %s", ((Friend*)friendListView->selectedItem)->name );
            cmdParse( buf );
        }
    }
    clipperPop();

    return 0;
}



//--------------------------------------------------------------------------------------------------
// BEGIN "GLOBAL" FRIEND WINDOW (PROTOTYPE)
//



GlobalFriend ** gGlobalFriends = 0;
Friend levelingPactMembers[MAX_LEVELINGPACT_MEMBERS] = {0};
bool gGlobalFriendMaximized = 0;
// to have minimized/maximized friend list modes, we'll keep two header structures & swap as necessary
UIListViewHeader * gGlobalMinHeader = 0;
UIListViewHeader * gGlobalMaxHeader = 0;


GlobalFriend * GetGlobalFriendOnShardByPlayerName(const char * name)
{
    int i;
    for(i=0;i<eaSize(&gGlobalFriends);i++)
    {
        if(    ! stricmp(name, gGlobalFriends[i]->name)
            // TODO: ADD A "Same shard" bool to GlobalFriend struct & cache this result
            && ! stricmp(game_state.chatShard, gGlobalFriends[i]->shard) )
        {
            return gGlobalFriends[i];
        }
    }
    return 0;
}

GlobalFriend * GetGlobalFriend(const char * handle)
{
    //int i;
    //for(i=0;i<eaSize(&gGlobalFriends);i++)
    //{
    //    if(!stricmp(handle, gGlobalFriends[i]->handle))
    //        return gGlobalFriends[i];
    //}
    //return 0;

    // DEBUG VERSION
    int i;
    int count = 0;
    GlobalFriend * gf = 0;

    for(i=0;i<eaSize(&gGlobalFriends);i++)
    {
        if(!stricmp(handle, gGlobalFriends[i]->handle))
        {
            gf = gGlobalFriends[i];
            count++;
        }
    }

    if(gf)
        assert(count == 1);
    else
        assert(count == 0);

    return gf;

}
MP_DEFINE(GlobalFriend);
GlobalFriend * GlobalFriendCreate(const char * handle, int db_id)
{
    GlobalFriend * gf;
    MP_CREATE(GlobalFriend, 8);
    gf = MP_ALLOC(GlobalFriend);        
    strncpyt(gf->handle, handle, SIZEOF2(GlobalFriend, handle));
    gf->relativeDbId = db_id;

    assert(!GetGlobalFriend(handle));
    eaPush(&gGlobalFriends, gf);
    
    return gf;
}

void GlobalFriendDestroyAll()
{
    while(eaSize(&gGlobalFriends)>0)
    {
        GlobalFriendDestroy(gGlobalFriends[0]);
    }
}

void GlobalFriendDestroy(GlobalFriend * gf)
{
    eaRemove(&gGlobalFriends, eaFind(&gGlobalFriends, gf));
    MP_FREE(GlobalFriend,gf);
}

void GlobalFriendUpdate(GlobalFriend * gf, GFStatus status, const char * name, const char * shard, const char * map)
{
    gf->status = status;
    strncpyt(gf->name,    (name ? name : ""),        SIZEOF2(GlobalFriend, name));
    strncpyt(gf->shard, (shard ? shard : ""),    SIZEOF2(GlobalFriend, shard));
    strncpyt(gf->map,    (map ? map : ""),        SIZEOF2(GlobalFriend, map));
}



/* Function friendNameCompare()
*    Compares the name of two friends.
*    Used to sort the friend list view by the name column.
*/
//int globalFriendNameCompare(const GlobalFriend** f1, const GlobalFriend** f2)
//{
//    return stricmp((*f1)->name, (*f2)->name); 
//}


// ----------------------------------------------------------
// CONTEXT MENUS
// ------------------


//-------------------------------------------------------------------------------------------------------------------
// Context menu related functions
//-------------------------------------------------------------------------------------------------------------------
// Used by context menus to add the current selected entity to the global friend list.

void addToGlobalFriendsList(void* useless)
{
    char buf[128];
    char* targetName = targetGetName();
    char * targetHandle = targetGetHandle();
    
    if(targetName)
    {
        sprintf( buf, "gfriend %s", targetName);
        cmdParse( buf );
    }
    else if(targetHandle)
    {
        sprintf( buf, "gfriend @%s", targetHandle);
        cmdParse( buf );
    }
    else
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
    }
}

static char * s_pchRemoveMe;
static bool s_bHandle;
static void gfriend_RemoveYes(void * data)
{
    if (s_bHandle)
    {
        cmdParsef( "gunfriend @%s", s_pchRemoveMe );
    }
    else
    {
        cmdParsef( "gunfriend %s", s_pchRemoveMe );
    }
}

void gfriend_Remove( char * name, bool bHandle )
{ 
    SAFE_FREE(s_pchRemoveMe);
    s_pchRemoveMe = strdup(name);
    s_bHandle = bHandle;
    dialogStd(DIALOG_YES_NO, textStd("ReallyRemoveFriend", s_pchRemoveMe), 0, 0, gfriend_RemoveYes, 0, 0 ); 
}

// Used by context menus to remove the current selected entity from the global friend list.
void removeFromGlobalFriendsList(void* useless)
{
    char* targetName = targetGetName();
    char * targetHandle = targetGetHandle();

    if(targetName)
        gfriend_Remove( targetName, false );
    else if(targetHandle)
        gfriend_Remove( targetHandle, true );
    else
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
}


int isGlobalFriend(void* foo)
{    
    char * handle = targetGetHandle();
    char * name = targetGetName();

    if(!UsingChatServer())
        return CM_HIDE;

    if(handle && GetGlobalFriend(handle) &&  (0 != stricmp(handle, game_state.chatHandle)))
        return CM_AVAILABLE;
    else if(name && GetGlobalFriendOnShardByPlayerName(name) && (0 != stricmp(name, playerPtr()->name)))
        return CM_AVAILABLE;
    else
        return CM_HIDE;
}


// Context menu item visiblity function.
// Allows the item to be displayed if the selected target is not a friend.
int isNotGlobalFriend(void *foo)
{
    char * handle = targetGetHandle();
    char * name = targetGetName();

    if(!UsingChatServer())
        return CM_HIDE;

    if(handle && !GetGlobalFriend(handle) &&  (0 != stricmp(handle, game_state.chatHandle)))
        return CM_AVAILABLE;
    else if(name && !GetGlobalFriendOnShardByPlayerName(name) && (0 != stricmp(name, playerPtr()->name)))
        return CM_AVAILABLE;
    else
        return CM_HIDE;
}

bool playerIsGlobalFriendOnShard(char * name)
{
    int i;
    for( i = 0; i < eaSizeUnsafe(&globalFriendListView->items); i++ )
    {
        GlobalFriend *item = globalFriendListView->items[i];
        if(    item->onSameShard 
            && !stricmp(item->name, name))
        {
            return true;
        }
    }
    return false;
}




UIBox globalFriendListViewDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, void* itemData, int itemIndex)
{
    GlobalFriend* item = (GlobalFriend*)itemData;
    UIBox box;
    PointFloatXYZ pen = rowOrigin;
    UIColumnHeaderIterator columnIterator; 
    int color;

    box.origin.x = rowOrigin.x;
    box.origin.y = rowOrigin.y;

    uiCHIGetIterator(list->header, &columnIterator);

    rowOrigin.z+=1;
    if( !item )
    {
        // wtf, force a rebuild
        friendListPrepareUpdate();
        return box;
    }

    if(item == list->selectedItem)
    {
        color = CLR_WHITE;
    }
    else if(item->status == GFStatus_Offline)
    {
        color = CLR_OFFLINE_ITEM;
    }
    else if(item->status == GFStatus_Online)
    {
        color = CLR_ONLINE_ITEM;
    }
    else if(item->onSameShard)
    {
        color = CLR_CONSTRUCT(150, 225, 100, 255);
    }
    else
    {
        // online, but different shard
        color = CLR_CONSTRUCT(120, 170, 75, 255);
    }

    font_color(color, color);

    // Iterate through each of the columns.
    for(;uiCHIGetNextColumn(&columnIterator, list->scale);)
    {
        char buf[1000];
        char * str = 0;
        char * trans = 0;
        UIColumnHeader* column = columnIterator.column;
        UIBox clipBox;
        int rgba[4] = {color, color, color, color };
        font_color( color, color );


        // Decide what to draw on on which column.
          if(stricmp(column->name, FRIEND_HANDLE_COLUMN) == 0)
            str = item->handle;
        else if(stricmp(column->name, FRIEND_SHARD_COLUMN) == 0)
            trans = item->shard;
        else if(stricmp(column->name, FRIEND_ARCHETYPE_COLUMN) == 0)
            trans = item->archetype;
        else if(stricmp(column->name, FRIEND_ORIGIN_COLUMN) == 0)
            trans = item->origin;
        else if(stricmp(column->name, FRIEND_MAP_COLUMN) == 0)
                trans = item->map;
        else if(stricmp(column->name, FRIEND_PRIMARY_COLUMN) == 0)
             trans = item->powerset1;
        else if(stricmp(column->name, FRIEND_SECONDARY_COLUMN) == 0)
            trans = item->powerset2;
         else if(stricmp(column->name, FRIEND_LEVEL_COLUMN) == 0)
        {
            if(item->xpLevel >0)
                str = itoa(item->xpLevel, buf, 10);
        }
        else if(stricmp(column->name, FRIEND_TEAMSIZE_COLUMN) == 0)
        {
            if(item->teamSize == 1 || (item->teamSize == 0 && item->status == GFStatus_Playing))
                 trans = "FriendListTeamSizeSolo";
            else if(item->teamSize >= 2)
                str = itoa(item->teamSize, buf, 10);
        }
        else
        {
            // Draw the friend status
            if(item->status == GFStatus_Offline)
                trans = "FriendOffline";
            else if(item->status == GFStatus_Online)
                trans = "FriendOnline";
            else
                str = item->name;
        }

        // do we need to localize it?
        if(trans)
        {
            msPrintf(menuMessages, SAFESTR(buf), trans); 
            str = &buf[0];
        }

        // Draw the friend name.
        if(str)
        {
            pen.x = rowOrigin.x + columnIterator.columnStartOffset*list->scale;

            clipBox.origin.x = pen.x;
            clipBox.origin.y = pen.y;
            clipBox.height = 20*list->scale;

            if( columnIterator.columnIndex >= eaSize(&columnIterator.header->columns )-1 )
                clipBox.width = 1000;
            else
                clipBox.width = columnIterator.currentWidth;

            clipperPushRestrict(&clipBox);

             if(str_wd_notranslate(&game_12, list->scale , list->scale, str) <= clipBox.width)
                font(&game_12);
            else
                font(&game_9);

            if(stricmp(column->name, FRIEND_MAP_COLUMN) == 0)
                cprntEx( pen.x, pen.y+18*list->scale, rowOrigin.z, list->scale, list->scale, NO_PREPEND, trans );
            else
                printBasic(font_grp, pen.x, pen.y+18*list->scale, rowOrigin.z, list->scale, list->scale, 0, str, strlen(str), rgba );

            clipperPop();
        }
    }

    box.height = 0;
    box.width = list->header->width;

    // Returning the bounding box of the item does not do anything right now.
    // See uiListView.c for more details.
    return box;
}


int globalFriendHandleCompare(const GlobalFriend** f1, const GlobalFriend** f2)
{
    return stricmp((*f1)->handle, (*f2)->handle); 
}

// utility function for comparing by string & breaking ties by comparing handles (insuring a consistent sort every time)
int globalFriendCompareString(const GlobalFriend** f1, const GlobalFriend ** f2, const char * str1, const char * str2)
{
    int ret = stricmp(str1, str2); 
    if(ret)
        return ret;
    else
        return globalFriendHandleCompare(f1, f2);
}

int globalFriendNameCompare(const GlobalFriend** f1, const GlobalFriend** f2)
{
    return globalFriendCompareString(f1, f2, (*f1)->name, (*f2)->name);
}

int globalFriendShardCompare(const void* f1Data, const void* f2Data)
{
    const GlobalFriend** f1 = (const GlobalFriend**)f1Data;
    const GlobalFriend** f2 = (const GlobalFriend**)f2Data;
    return globalFriendCompareString(f1, f2, (*f1)->shard, (*f2)->shard);
}

int globalFriendMapCompare(const void* f1Data, const void* f2Data)
{
    const GlobalFriend** f1 = (const GlobalFriend**)f1Data;
    const GlobalFriend** f2 = (const GlobalFriend**)f2Data;
    return globalFriendCompareString(f1, f2, (*f1)->map, (*f2)->map);
}

int globalFriendLevelCompare(const void* f1Data, const void* f2Data)
{
    const GlobalFriend** f1 = (const GlobalFriend**)f1Data;
    const GlobalFriend** f2 = (const GlobalFriend**)f2Data;
    int level1 = (*f1)->xpLevel;
    int level2 = (*f2)->xpLevel;
     if(level1 < level2)
        return -1;
    else if(level1 > level2)
        return 1;
    else
        return globalFriendHandleCompare(f1, f2);
}

int globalFriendTeamCompare(const void* f1Data, const void* f2Data)
{
    const GlobalFriend** f1 = (const GlobalFriend**)f1Data;
    const GlobalFriend** f2 = (const GlobalFriend**)f2Data;
    int size1 = (*f1)->teamSize;
    int size2 = (*f2)->teamSize;
    if(size1 < size2)
        return -1;
    else if(size1 > size2)
        return 1;
    else
        return globalFriendHandleCompare(f1, f2);
}

int globalFriendArchetypeCompare(const void* f1Data, const void* f2Data)
{
    const GlobalFriend** f1 = (const GlobalFriend**)f1Data;
    const GlobalFriend** f2 = (const GlobalFriend**)f2Data;
    return globalFriendCompareString(f1, f2, (*f1)->archetype, (*f2)->archetype);
}

int globalFriendOriginCompare(const void* f1Data, const void* f2Data)
{
    const GlobalFriend** f1 = (const GlobalFriend**)f1Data;
    const GlobalFriend** f2 = (const GlobalFriend**)f2Data;
    return globalFriendCompareString(f1, f2, (*f1)->origin, (*f2)->origin);
}


/* Function friendStatusCompare()
*    used to sort players by status. If they're playing, sort by player name
*/
int globalFriendStatusCompare(const void* f1Data, const void* f2Data)
{
    const GlobalFriend** f1 = (const GlobalFriend**)f1Data;
    const GlobalFriend** f2 = (const GlobalFriend**)f2Data;
    const GlobalFriend* friend1 = *f1;
    const GlobalFriend* friend2 = *f2;

    if(friend1->status > friend2->status)
        return -1;
    else if(friend1->status < friend2->status)
        return 1;
    else
        return globalFriendNameCompare(f1,f2);
}




typedef struct GlobalFriendColumns
{
    char *                    title;
    char *                    displayTitle;
    bool                    minimizedColumn;    // true if should also be displayed in "minimized" list view
    float                    minWidth;
    UIListViewItemCompare    compareFunc;

}GlobalFriendColumns;

//<PLAYER NAME> <XP LEVEL> <MAP NAME> <ARCHETYPE> <ORIGIN> <PRIMARY SET> <SECONDARY SET>

static int globalFriendHandleCompareCallback(const void *a, const void *b)
{
    return globalFriendHandleCompare((const GlobalFriend**)a, (const GlobalFriend**)b);
}

GlobalFriendColumns gFriendColumns [] = 
{
    {FRIEND_HANDLE_COLUMN,        "friendHandle",        1,    110,    globalFriendHandleCompareCallback    },
    {FRIEND_STATUS_COLUMN,        "friendStatus",        1,    100,    globalFriendStatusCompare    },
//    {FRIEND_NAME_COLUMN,        "friendName",        0,    60,    globalFriendNameCompare        },
    {FRIEND_SHARD_COLUMN,        "friendShard",        0,    65,    globalFriendShardCompare    },
    {FRIEND_MAP_COLUMN,            "friendMap",        0,    130,    globalFriendMapCompare        },
    {FRIEND_LEVEL_COLUMN,        "friendLevel",        0,    18,    globalFriendLevelCompare    },
    {FRIEND_ARCHETYPE_COLUMN,    "friendArchetype",    0,    60,    globalFriendArchetypeCompare},
    {FRIEND_ORIGIN_COLUMN,        "friendOrigin",        0,    65,    globalFriendOriginCompare    },
//    {FRIEND_PRIMARY_COLUMN,        "friendPrimary",    0,    60,    globalFriendPrimaryCompare    },
//    {FRIEND_SECONDARY_COLUMN,    "friendSecondary",    0,    60,    globalFriendSecondaryCompare},
    {FRIEND_TEAMSIZE_COLUMN,    "friendTeamSize",    0,    18,    globalFriendTeamCompare},
    
    { 0 },
};

void SetGlobalFriendWidth( Wdw *wdw)
{
    Wdw* friendsWindow = wdwGetWindow(WDW_FRIENDS);
    float x,y,z,wd,ht,scale;
    UIBox box = { 0 };
     PointFloatXYZ zero = {0}; 
    int minWindowWidth, minWindowHeight;

      if(!gGlobalFriendMaximized || window_getMode(WDW_FRIENDS) != WINDOW_DISPLAYING )
        return;

    // need to run through display code once to make sure dimensions are correct
    clipperPushRestrict(&box); // push on zero size clip box
    uiLVDisplay(globalFriendListView, zero); // run through drawing routine once
    clipperPop(); // pop off full clip

    // The window needs to be wide enough to fit the following.
    // - The list view header
    // - The window borders
    minWindowWidth = (int)uiLVHGetMinDrawWidth(globalFriendListView->header) + 2*PIX3;

    // The window needs to be tall enough to fit the following:
    // - The list view, with header and sort arrow
    // - Two items in the list view
    // - The remove button
    // - The window borders
    minWindowHeight = (int)uiLVGetMinDrawHeight(globalFriendListView) + (2*globalFriendListView->rowHeight + FRIEND_REMOVE_AREA + 2*PIX3);
    
    window_getDims(WDW_FRIENDS, &x, &y, &z, &wd, &ht, &scale, 0, 0);
    wdw->loc.draggable_frame = VERTONLY;
    wd = minWindowWidth*scale;
    window_setDims(WDW_FRIENDS, -1, -1, wd, -1); 
}

// used by add friend dialog
static void addGlobalFriend(void * data)
{
    char buf[1000];
    sprintf(buf, "gfriend %s", dialogGetTextEntry());
    cmdParse(buf);
}

// Context menu item visiblity function.
// Allows the item to be displayed if the selected target is a leveling pact member.
static int entityIsLevelingpactMember(int dbid)
{
    int i;
    Entity *e = playerPtr();
    if(!dbid)
        return 0;
    for( i = 0; i < e->levelingpact->count; i++ )
    {
        if( dbid == e->levelingpact->members.ids[i] )
            return 1;
    }

    return 0;
}

int isLevelingpactMember(void* foo)
{
    int dbid;

    dbid = targetGetDBID();
    if(!dbid)
        return CM_HIDE;

    if(entityIsLevelingpactMember(dbid))
        return CM_AVAILABLE;
    else
        return CM_HIDE;
}


// Context menu item visiblity function.
// Allows the item to be displayed if the selected target is not a friend.
int isNotLevelingpactMember(void *foo)
{
    if( isLevelingpactMember(NULL) == CM_AVAILABLE )
        return CM_HIDE;
    else
        return CM_AVAILABLE;        
}



#define BUTTON_WIDTH    (55)
#define BUTTON_SPACE    (5)
#define BUTTON_HEIGHT    (24)
#define BUTTON_FOOTER    (BUTTON_HEIGHT/2 + PIX3 + 5)


int newFriendWindow(float x, float y, float z, float wd, float ht, float scale, int color, int bcolor, void *data)
{
    Entity *e = playerPtr();
    PointFloatXYZ pen;
    UIBox windowDrawArea;
    UIBox listViewDrawArea;
    float yPos = y + 10, buttonWd;
     static bool init = false;
    int buttonColor;

    static ScrollBar sb = {WDW_FRIENDS,0};

    if(!init)
    {
        initGlobalFriendContextMenu();
        init = true;
    }

       font(&game_12);

    // Where in the window can we draw?
     windowDrawArea.x = x;
    windowDrawArea.y = y;
    windowDrawArea.width = wd;
    windowDrawArea.height = ht;
    uiBoxAlter(&windowDrawArea, UIBAT_SHRINK, UIBAD_ALL, PIX3*scale);

    listViewDrawArea = windowDrawArea;
    uiBoxAlter(&listViewDrawArea, UIBAT_SHRINK, UIBAD_BOTTOM, (BUTTON_HEIGHT + BUTTON_SPACE*2)*scale);

     //drawUiBox(&listViewDrawArea, z+100, CLR_RED);
 
    // The list view will be drawn inside the listViewDrawArea and above the window frame.
    pen.x = listViewDrawArea.x;
    pen.y = listViewDrawArea.y;
    pen.z = z+1;                    // Draw on top of the frame

    // Just draw buttons if friends list is empty
    if( eaSize(&gGlobalFriends) == 0 )
    {
        int fontHeight = ttGetFontHeight(font_grp, scale, scale);
        clipperPushRestrict(&windowDrawArea);
        cprnt( pen.x + wd/2, pen.y+fontHeight, pen.z, scale, scale, "NoFriendsString" );
        clipperPop();

    }
    else
    {
        // Draw and handle all scroll bar functionaltiy if the friendlist is larger than the window.
        // This is seperate from the list view because the scroll bar is always outside the draw area
        // of the list view.
        if(globalFriendListView)
        {
            int fullDrawHeight = uiLVGetFullDrawHeight(globalFriendListView);
            int currentHeight = uiLVGetHeight(globalFriendListView);
            if(globalFriendListView)
            {
                if(fullDrawHeight > currentHeight)
                {
                       doScrollBar( &sb, currentHeight-25*scale, fullDrawHeight, wd, (PIX3+R10+30)*scale, z+2, 0, &windowDrawArea );
                    globalFriendListView->scrollOffset = sb.offset;
                }
                else
                {
                    globalFriendListView->scrollOffset = 0;
                }
            }
        }

        // Make sure the list view is initialized.
        if(!globalFriendListView)
        {
            GlobalFriendColumns * col = &gFriendColumns[0];
            
            globalFriendListView = uiLVCreate();

            gGlobalMaxHeader = globalFriendListView->header;
            gGlobalMinHeader = uiLVHCreate();
            
            
            while(col->title)
            {
                if(col->minimizedColumn)
                    uiLVHAddColumnEx(gGlobalMinHeader, col->title, col->displayTitle, col->minWidth, 270, 1);
                uiLVHAddColumnEx(gGlobalMaxHeader,  col->title, col->displayTitle, col->minWidth, 270, 1);

                uiLVBindCompareFunction(globalFriendListView, col->title, col->compareFunc);

                col++; // pointer arithmetic
            }

            globalFriendListView->displayItem = globalFriendListViewDisplayItem;
            
            uiLVEnableMouseOver(globalFriendListView, 1);
            uiLVEnableSelection(globalFriendListView, 1);

            globalFriendListView->header = gGlobalMinHeader;
        }

        // How tall and wide is the list view supposed to be?
        uiLVSetDrawColor(globalFriendListView, window_GetColor(WDW_FRIENDS));
        uiLVSetHeight(globalFriendListView, listViewDrawArea.height);
          uiLVHFinalizeSettings(globalFriendListView->header);

           uiLVHSetWidth(globalFriendListView->header,listViewDrawArea.width);
        uiLVSetScale( globalFriendListView, scale );
        // Rebuild the entire friends list if required.
         if(globalFriendListView && globalFriendListRebuildRequired)
        {
            int i;

            uiLVClear(globalFriendListView);
            for(i = 0; i < eaSize(&gGlobalFriends); i++)
            {
                uiLVAddItem(globalFriendListView, gGlobalFriends[i]);
                if(!stricmp(gGlobalFriends[i]->handle, oldGlobalFriendSelectionHandle))
                    uiLVSelectItem(globalFriendListView, i);
            }

            strcpy(oldGlobalFriendSelectionHandle, "");
            
            uiLVSortBySelectedColumn(globalFriendListView);

            globalFriendListRebuildRequired = 0;
        }

        // Disable mouse over on window resize.
        {
            Wdw* friendsWindow = wdwGetWindow(WDW_FRIENDS);
            if(friendsWindow->drag_mode)
                uiLVEnableMouseOver(globalFriendListView, 0);
            else
                uiLVEnableMouseOver(globalFriendListView, 1);
        }

        // Draw the draw the list and clip everything to inside the proper drawing area of the list view.
        clipperPushRestrict(&listViewDrawArea);
        uiLVDisplay(globalFriendListView, pen);
        clipperPop();

        // Handle list view input.
        uiLVHandleInput(globalFriendListView, pen);


        // Did the user pick someone from the friends list?
        if(globalFriendListView->newlySelectedItem && globalFriendListView->selectedItem)
        {
            // Which entity was selected?
            GlobalFriend* item = globalFriendListView->selectedItem;
            if(item->dbid)
            {
                Entity* player = entFromDbId(item->dbid);

                if(player)
                {
                    if(player != playerPtr() && character_TargetIsFriend(playerPtr()->pchar, player->pchar))
                    {
                        targetSelect(player);
                    }
                }
                else
                {
                    targetSelect2(item->dbid, item->name);
                }

                targetSetHandle(item->handle);
            }
            else
            {
                targetSelectHandle(item->handle, 0);
            }

            globalFriendListView->newlySelectedItem = 0;
        }

        // If the target is a friend...
        // If the target is a global friend...
        if(targetGetName() && playerIsGlobalFriendOnShard(targetGetName()))
        {
            int i;
            if(current_target)
            {
                for( i = 0; i < eaSizeUnsafe(&globalFriendListView->items); i++ )
                {
                    GlobalFriend *item = globalFriendListView->items[i];
                    
                    globalFriendListView->selectedItem = 0;
                    
                    if(    item->onSameShard 
                        && item->dbid == current_target->db_id)
                    {
                        globalFriendListView->selectedItem = item;
                        break;
                    }
                }
            }
        }
        // TODO: The "GetGlobalFriend()" call is not very efficient...
        else if(current_target || !targetGetHandle() || ! GetGlobalFriend(targetGetHandle()))
            globalFriendListView->selectedItem = 0;

        if(globalFriendListView->selectedItem)
        {
            int index;

            index = eaFind(&globalFriendListView->items, globalFriendListView->selectedItem);
            if(index != -1 && uiMouseCollision(globalFriendListView->itemWindows[index]) && mouseRightClick())
            {    
                if(current_target)
                    contextMenuForTarget(current_target);
                else
                {
                    GlobalFriend * gf = globalFriendListView->selectedItem;
                    int x,y;

                    inpMousePos( &x, &y);

                    if(gf->status == GFStatus_Playing && gf->onSameShard)
                    {
                        contextMenu_set( interactPlayerNotOnMap, x, y, NULL );
                    }
                    else if(gf->status == GFStatus_Playing || gf->status == GFStatus_Online)
                    {
                        contextMenu_set( interactHandleExternalChat, x, y, NULL );
                    }
                    else if(gf->status == GFStatus_Offline)
                    {
                        contextMenu_set( interactHandleOffline, x, y, NULL );
                    }
                }
            }
        }
    }

    // Clip and draw the remove friend button
      listViewDrawArea.y += listViewDrawArea.height;
      listViewDrawArea.height = FRIEND_REMOVE_AREA*scale;
     clipperPushRestrict(&listViewDrawArea);

     // visible/invisible
    if(game_state.gFriendHide)
        buttonColor = CLR_RED;
    else
        buttonColor = window_GetColor(WDW_FRIENDS);

    buttonWd = MIN( BUTTON_WIDTH*scale, (wd - PIX3*6)/3 );

       if(D_MOUSEHIT == drawStdButton(x + wd/2 - (buttonWd + BUTTON_SPACE)*scale, y + ht - BUTTON_FOOTER*scale, z, buttonWd, BUTTON_HEIGHT*scale, buttonColor, (game_state.gFriendHide ? "GlobalFriendVisible" : "GlobalFriendInvisible"), scale, 0))
    {
        char buf[128];

        if(game_state.gFriendHide)
            sprintf( buf, "gunhide");
        else
            sprintf( buf, "ghide");
        cmdParse( buf );
    }

    // ADD
    if(D_MOUSEHIT == drawStdButton(x + wd/2, y + ht - BUTTON_FOOTER*scale, z, buttonWd, BUTTON_HEIGHT*scale, window_GetColor(WDW_FRIENDS), "AddGlobalFriend", scale, 0))
    {
        dialogNameEdit(textStd("InviteGlobalFriendPrompt"),"InviteGlobalFriend","CancelString", addGlobalFriend, NULL, MAX_PLAYERNAME, DLGFLAG_ALLOW_HANDLE );
    }

     if(eaSize(&gGlobalFriends))
    {
         // Minimized/Maximized Toggle
        float xScale = 1.5f;
        float yScale = 0.8f;
        int arrowY;
        AtlasTex * arrow;
        if(gGlobalFriendMaximized)
            arrow = atlasLoadTexture( "teamup_arrow_contract.tga");
        else
            arrow = atlasLoadTexture( "teamup_arrow_expand.tga");
        arrowY = y + ht - (8 + (BUTTON_FOOTER + (BUTTON_HEIGHT - arrow->height*yScale)/2))*scale;
        if(D_MOUSEHIT == drawGenericButtonEx(arrow, x + wd - (5 + arrow->width*xScale)*scale, arrowY, z+1, xScale*scale, yScale*scale, color, color, 1))
        {
            if(globalFriendListView)
            {
                if(gGlobalFriendMaximized)
                {
                    globalFriendListView->header = gGlobalMinHeader;
                    window_setDims(WDW_FRIENDS, -1, -1, prev_wd, -1 );
                }
                else
                {
                    globalFriendListView->header = gGlobalMaxHeader;
                    prev_wd = wd;
                }

                gGlobalFriendMaximized = ! gGlobalFriendMaximized;
            }
        }
    
        // REMOVE Button
          if(D_MOUSEHIT == drawStdButton(x + wd/2 + (buttonWd + BUTTON_SPACE)*scale, y + ht - BUTTON_FOOTER*scale, z, buttonWd*scale, BUTTON_HEIGHT*scale, window_GetColor(WDW_FRIENDS), "RemoveGlobalFriend", scale, !((int)globalFriendListView->selectedItem)))
        {
            if(globalFriendListView && globalFriendListView->selectedItem)
            {
                gfriend_Remove( ((GlobalFriend*)globalFriendListView->selectedItem)->handle, true );
            }
        }
    }

      clipperPop();
    return 0;
}
//
//    END "GLOBAL" FRIEND WINDOW (PROTOTYPE)
//--------------------------------------------------------------------------------------------------

ChatUser ** gGlobalIgnores;


int GetGlobalIgnoreIdx(char * handle)
{
    if(handle)
    {
        int i;
        for(i=eaSize(&gGlobalIgnores)-1;i>=0;i--)
        {
            if(!stricmp(gGlobalIgnores[i]->handle, handle))
                return i;
        }
    }

    return -1;
}

int GetGlobalIgnoreIdxByDbId(int db_id)
{
    if(db_id > 0)
    {
        int i;
        for(i=eaSize(&gGlobalIgnores)-1;i>=0;i--)
        {
            if(gGlobalIgnores[i]->db_id == db_id)
                return i;
        }
    }

    return -1;
}

void AddGlobalIgnore(char * handle, int db_id)
{
    int idx;
    if((idx = GetGlobalIgnoreIdx(handle)) < 0)
    {
        eaPush(&gGlobalIgnores, ChatUserCreate(handle, db_id, 0));
        globalIgnoreListPrepareUpdate();
    }
    else
    {
        gGlobalIgnores[idx]->db_id = db_id;    // update only
    }
}

void UpdateGlobalIgnore(char * oldHandle, char * newHandle)
{
    int idx;
    if((idx = GetGlobalIgnoreIdx(oldHandle)) >= 0)
    {
        ChatUserRename(gGlobalIgnores[idx], newHandle);
    }
}

void RemoveGlobalIgnore(char * handle)
{
    int idx;
    if((idx = GetGlobalIgnoreIdx(handle)) >= 0)
    {
        ChatUser * user;
        globalIgnoreListPrepareUpdate();
        
        user = eaRemove(&gGlobalIgnores, idx);
        ChatUserDestroy(user);
    }
}

void RemoveAllGlobalIgnores()
{
    globalIgnoreListPrepareUpdate();
    while(eaSize(&gGlobalIgnores))
    {
        ChatUser * user = eaRemove(&gGlobalIgnores, 0);
        if(user)
            ChatUserDestroy(user);
    }

}



// Global Ignore Context menu stuff
int isGlobalIgnore(void* foo)
{    
    char * handle    = targetGetHandle();
    int       db_id    = targetGetDBID();
    char * name        = targetGetName();

    if(!UsingChatServer())
        return CM_HIDE;

    if(handle && GetGlobalIgnoreIdx(handle) >= 0)
        return CM_AVAILABLE;
    else if(name && db_id && GetGlobalIgnoreIdxByDbId(db_id) >= 0)
        return CM_AVAILABLE;
    else
        return CM_HIDE;
}

int isNotGlobalIgnore(void* foo)
{    
    char * handle = targetGetHandle();
    int       db_id  = targetGetDBID();
    char * name      = targetGetName();

    if(!UsingChatServer())
        return CM_HIDE;

    if(handle && GetGlobalIgnoreIdx(handle) < 0)
        return CM_AVAILABLE;
    else if(name && db_id && GetGlobalIgnoreIdxByDbId(db_id) < 0)
        return CM_AVAILABLE;
    else
        return CM_HIDE;
}

// Used by context menus to add the current selected entity to the global ignore list.
void addToGlobalIgnoreList(void* useless)
{
    char buf[128];
    char* targetName = targetGetName();
    char * targetHandle = targetGetHandle();

    if(targetName)
    {
        sprintf( buf, "gignore %s", targetName);
        cmdParse( buf );
    }
    else if(targetHandle)
    {
        sprintf( buf, "gignore @%s", targetHandle);
        cmdParse( buf );
    }
    else
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
    }
}


// Used by context menus to remove the current selected entity from the global ignore list.
void removeFromGlobalIgnoreList(void* useless)
{
    char buf[128];
    char* targetName = targetGetName();
    char * targetHandle = targetGetHandle();


    if(targetName)
    {
        sprintf( buf, "gunignore %s", targetName);
        cmdParse( buf );
    }
    else if(targetHandle)
    {
        sprintf( buf, "gunignore @%s", targetHandle);
        cmdParse( buf );
    }
    else
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
    }
}


static void addGlobalIgnoreDlgHandler(void * data)
{
    char buf[1000];
    sprintf(buf, "gignore %s", dialogGetTextEntry());
    cmdParse(buf);
}



UIBox globalIgnoreListViewDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, void* itemData, int itemIndex)
{
    ChatUser * item = (ChatUser *)itemData;
    UIBox box;
    PointFloatXYZ pen = rowOrigin;
    UIColumnHeaderIterator columnIterator; 
    int color;

    box.origin.x = rowOrigin.x;
    box.origin.y = rowOrigin.y;

    uiCHIGetIterator(list->header, &columnIterator);

    rowOrigin.z+=1;

    if(item == list->selectedItem)
        color = CLR_WHITE;
    else 
        color = CLR_ONLINE_ITEM;
    
    font_color(color, color);

    // Iterate through each of the columns.
    for(;uiCHIGetNextColumn(&columnIterator, list->scale);)
    {
        char * trans = 0;
        UIColumnHeader* column = columnIterator.column;
        UIBox clipBox;
        int rgba[4] = {color, color, color, color };


        // Decide what to draw on on which column.
        if(stricmp(column->name, FRIEND_HANDLE_COLUMN) == 0)
        {
            pen.x = rowOrigin.x + columnIterator.columnStartOffset*list->scale;

            clipBox.origin.x = pen.x;
            clipBox.origin.y = pen.y;
            clipBox.height = 20*list->scale;

            if( columnIterator.columnIndex >= eaSize(&columnIterator.header->columns )-1 )
                clipBox.width = 1000;
            else
                clipBox.width = columnIterator.currentWidth;

            clipperPushRestrict(&clipBox);
 
            if(str_wd_notranslate(&game_12, list->scale , list->scale, item->handle) <= clipBox.width)
                font(&game_12);
            else
                font(&game_9);

            printBasic(font_grp, pen.x, pen.y+18*list->scale, rowOrigin.z, list->scale, list->scale, 0, item->handle, strlen(item->handle), rgba );

            clipperPop();
        }
    }

    box.height = 0;
    box.width = list->header->width;

    // Returning the bounding box of the item does not do anything right now.
    // See uiListView.c for more details.
    return box;
}

int chatUserHandleCompare(const ChatUser** c1, const ChatUser** c2)
{
    return stricmp((*c1)->handle, (*c2)->handle); 
}

int levelingpactWindow(float x, float y, float z, float wd, float ht, float scale, int color, int bcolor, void * data)
{
    Entity *e = playerPtr();
    PointFloatXYZ pen;
    UIBox windowDrawArea;
    UIBox listViewDrawArea;
    static ScrollBar sb = {WDW_FRIENDS,0};

    wdwGetWindow(WDW_FRIENDS)->loc.draggable_frame = RESIZABLE;


    font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);
    // Do everything common windows are supposed to do.
    //    if ( !window_getDims( WDW_FRIENDS, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ))
    //        return 0;

    // Draw the base friends list frame.
    //    drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, bcolor );

    // Where in the window can we draw?
    windowDrawArea.x = x;
    windowDrawArea.y = y;
    windowDrawArea.width = wd;
    windowDrawArea.height = ht;
    uiBoxAlter(&windowDrawArea, UIBAT_SHRINK, UIBAD_ALL, PIX3*scale);

    listViewDrawArea = windowDrawArea;
    uiBoxAlter(&listViewDrawArea, UIBAT_SHRINK, UIBAD_BOTTOM, FRIEND_REMOVE_AREA*scale);

    // The list view will be drawn inside the listViewDrawArea and above the window frame.
    pen.x = listViewDrawArea.x;
    pen.y = listViewDrawArea.y;
    pen.z = z+1;                    // Draw on top of the frame

    // Do nothing if the friendlist is empty.
    if( SAFE_MEMBER2(e,levelingpact,count) == 0 )
    {
        int fontHeight = ttGetFontHeight(font_grp, scale, scale);
        clipperPushRestrict(&windowDrawArea);
        cprnt( pen.x + wd/2, pen.y+fontHeight, pen.z, scale, scale, "LevelingPactNotAMember" );
        clipperPop();
        return 0;
    }

    // Draw and handle all scroll bar functionaltiy if the friendlist is larger than the window.
    // This is seperate from the list view because the scroll bar is always outside the draw area
    // of the list view.
    if(levelingpactListView)
    {
        int fullDrawHeight = uiLVGetFullDrawHeight(levelingpactListView);
        int currentHeight = uiLVGetHeight(levelingpactListView);
        if(levelingpactListView)
        {
            if(fullDrawHeight > currentHeight)
            {
                doScrollBar( &sb, currentHeight, fullDrawHeight, wd, PIX3+R10*scale, z+2, 0, &windowDrawArea  );
                levelingpactListView->scrollOffset = sb.offset;
            }
            else
            {
                levelingpactListView->scrollOffset = 0;
            }
        }
    }

    // Make sure the list view is initialized.
    if(!levelingpactListView)
    {

        levelingpactListView = uiLVCreate();

        uiLVHAddColumnEx(levelingpactListView->header, FRIEND_NAME_COLUMN, "levelingPactName", 10, 270, 1);
        uiLVBindCompareFunction(levelingpactListView, FRIEND_NAME_COLUMN, friendNameCompare);

        //uiLVHAddColumn(levelingpactListView->header, FRIEND_STATUS_COLUMN, "friendStatus", 0);
        //uiLVBindCompareFunction(levelingpactListView, FRIEND_STATUS_COLUMN, friendStatusCompare);

        uiLVFinalizeSettings(levelingpactListView);

        levelingpactListView->displayItem = levelingpactListViewDisplayItem;

        uiLVEnableMouseOver(levelingpactListView, 1);
        uiLVEnableSelection(levelingpactListView, 1);
    }

    // How tall and wide is the list view supposed to be?
    uiLVSetDrawColor(levelingpactListView, window_GetColor(WDW_FRIENDS));
    uiLVSetHeight(levelingpactListView, listViewDrawArea.height);
    uiLVHFinalizeSettings(levelingpactListView->header);
    uiLVHSetWidth(levelingpactListView->header, listViewDrawArea.width);
    uiLVSetScale( levelingpactListView, scale );
    // Rebuild the entire friends list if required.
    if(levelingpactListView && levelingpactListRebuildRequired)
    {
        int i;

        uiLVClear(levelingpactListView);
        for(i = 0; i < e->levelingpact->count; i++)
        {
            levelingPactMembers[i].dbid = e->levelingpact->members.ids[i];
            //BUG: We actually need to find out whether or not they're online
            levelingPactMembers[i].online = e->levelingpact->members.onMapserver[i];
            levelingPactMembers[i].map_id = e->levelingpact->members.mapIds[i];
            levelingPactMembers[i].name = e->levelingpact->members.names[i];
            uiLVAddItem(levelingpactListView, &levelingPactMembers[i]);
            if(e->levelingpact->members.ids[i] == levelingpactSelectionDBID)
                uiLVSelectItem(levelingpactListView, i);
        }

        uiLVSortBySelectedColumn(levelingpactListView);

        levelingpactListRebuildRequired = 0;
    }

    // Disable mouse over on window resize.
    {
        Wdw* levelingpactWindow = wdwGetWindow(WDW_FRIENDS);
        if(levelingpactWindow->drag_mode)
            uiLVEnableMouseOver(levelingpactListView, 0);
        else
            uiLVEnableMouseOver(levelingpactListView, 1);
    }

    // Draw the list and clip everything to inside the proper drawing area of the list view.
    clipperPushRestrict(&listViewDrawArea);
    uiLVDisplay(levelingpactListView, pen);
    clipperPop();

    // Handle list view input.
    uiLVHandleInput(levelingpactListView, pen);

    // Did the user pick someone from the friends list?
    if(levelingpactListView->newlySelectedItem && levelingpactListView->selectedItem)
    {
        // Which entity was selected?
        Friend* item = levelingpactListView->selectedItem;
        Entity* player = entFromDbId(item->dbid);

        if(player)
        {
            //still checking to make sure the target is a friend?
            if(player != playerPtr() && character_TargetIsFriend(playerPtr()->pchar, player->pchar))
            {
                targetSelect(player);
            }
        }
        else
        {
            targetSelect2(item->dbid, item->name);
        }

        levelingpactListView->newlySelectedItem = 0;
    }

    // If the target is a friend...
    if(entityIsLevelingpactMember(targetGetDBID()))
    {
        int i;
        if(current_target)
        {
            for( i = 0; i < eaSizeUnsafe(&levelingpactListView->items); i++ )
            {
                Friend *item = levelingpactListView->items[i];
                if(item->dbid == current_target->db_id)
                    levelingpactListView->selectedItem = item;
            }
        }
    }
    else
        levelingpactListView->selectedItem = 0;

    if(levelingpactListView->selectedItem)
    {
        int index;

        index = eaFind(&levelingpactListView->items, levelingpactListView->selectedItem);
        if(index != -1 && uiMouseCollision(levelingpactListView->itemWindows[index]) && mouseRightClick())
        {    
            if(current_target)
                contextMenuForTarget(current_target);
            else
                contextMenuForOffMap();
        }
    }

    // Clip and draw the leave pact button
    listViewDrawArea.y += listViewDrawArea.height;
    listViewDrawArea.height = FRIEND_REMOVE_AREA*scale;
    clipperPushRestrict(&listViewDrawArea);
    if(D_MOUSEHIT == drawStdButton(x + wd/2, y + ht - (20+PIX3)*scale, z, 100*scale, 26*scale, window_GetColor(WDW_FRIENDS), "LevelingPactLeaveBtn", scale, !((int)levelingpact_IsInPact(NULL))))
    {
        levelingpact_quitWindow(NULL);
    }
    clipperPop();

    return 0;
}

int globalIgnoreWindow(float x, float y, float z, float wd, float ht, float scale, int color, int bcolor, void * data)
{
    Entity *e = playerPtr();
    PointFloatXYZ pen;
    UIBox windowDrawArea;
    UIBox listViewDrawArea;
    float yPos = y + 10;
    static bool init = false;
    float dx;

    static ScrollBar sb = {WDW_FRIENDS,0};

    if(!init)
    {
        initGlobalIgnoreContextMenu();
        init = true;
    }

    font(&game_12);

    // Where in the window can we draw?
    windowDrawArea.x = x;
    windowDrawArea.y = y;
    windowDrawArea.width = wd;
    windowDrawArea.height = ht;
    uiBoxAlter(&windowDrawArea, UIBAT_SHRINK, UIBAD_ALL, PIX3*scale);

    listViewDrawArea = windowDrawArea;
    uiBoxAlter(&listViewDrawArea, UIBAT_SHRINK, UIBAD_BOTTOM, (BUTTON_HEIGHT + BUTTON_SPACE*2)*scale);

    //drawUiBox(&listViewDrawArea, z+100, CLR_RED);

    // The list view will be drawn inside the listViewDrawArea and above the window frame.
    pen.x = listViewDrawArea.x;
    pen.y = listViewDrawArea.y;
    pen.z = z+1;                    // Draw on top of the frame

    // Just draw buttons if friends list is empty
    if( eaSize(&gGlobalIgnores) == 0 )
    {
        int fontHeight = ttGetFontHeight(font_grp, scale, scale);
        clipperPushRestrict(&windowDrawArea);
        cprnt( pen.x + wd/2, pen.y+fontHeight, pen.z, scale, scale, "NoGlobalIgnoresString" );
        clipperPop();

    }
    else
    {

        // Draw and handle all scroll bar functionaltiy if the friendlist is larger than the window.
        // This is seperate from the list view because the scroll bar is always outside the draw area
        // of the list view.
        if(globalIgnoreListView)
        {
            int fullDrawHeight = uiLVGetFullDrawHeight(globalIgnoreListView);
            int currentHeight = uiLVGetHeight(globalIgnoreListView);
            if(globalIgnoreListView)
            {
                if(fullDrawHeight > currentHeight)
                {
                    doScrollBar( &sb, currentHeight-25*scale, fullDrawHeight, wd, (PIX3+R10+30)*scale, z+2, 0, &windowDrawArea );
                    globalIgnoreListView->scrollOffset = sb.offset;
                }
                else
                {
                    globalIgnoreListView->scrollOffset = 0;
                }
            }
        }

        // Make sure the list view is initialized.
        if(!globalIgnoreListView)
        {
            GlobalFriendColumns * col = &gFriendColumns[0];

            globalIgnoreListView = uiLVCreate();

            uiLVHAddColumnEx(globalIgnoreListView->header, FRIEND_HANDLE_COLUMN, "globalIgnoreHandle", 10, 270, 0);
            uiLVBindCompareFunction(globalIgnoreListView, FRIEND_HANDLE_COLUMN, chatUserHandleCompare);

            globalIgnoreListView->displayItem = globalIgnoreListViewDisplayItem;

            uiLVEnableMouseOver(globalIgnoreListView, 1);
            uiLVEnableSelection(globalIgnoreListView, 1);
        }

        // How tall and wide is the list view supposed to be?
        uiLVSetDrawColor(globalIgnoreListView, window_GetColor(WDW_FRIENDS));
        uiLVSetHeight(globalIgnoreListView, listViewDrawArea.height);
         uiLVHFinalizeSettings(globalIgnoreListView->header);
        uiLVHSetWidth(globalIgnoreListView->header, listViewDrawArea.width);
        uiLVSetScale( globalIgnoreListView, scale );
        // Rebuild the entire list if required.
        if(globalIgnoreListView && globalIgnoreListRebuildRequired)
        {
            int i;

            uiLVClear(globalIgnoreListView);
            for(i = 0; i < eaSize(&gGlobalIgnores); i++)
            {
                uiLVAddItem(globalIgnoreListView, gGlobalIgnores[i]);
                if(!stricmp(gGlobalIgnores[i]->handle, oldGlobalIgnoreSelectionHandle))
                    uiLVSelectItem(globalIgnoreListView, i);
            }

            strcpy(oldGlobalIgnoreSelectionHandle, "");

            uiLVSortBySelectedColumn(globalIgnoreListView);

            globalIgnoreListRebuildRequired = 0;
        }

        // Disable mouse over on window resize.
        {
            Wdw* friendsWindow = wdwGetWindow(WDW_FRIENDS);
            if(friendsWindow->drag_mode)
                uiLVEnableMouseOver(globalIgnoreListView, 0);
            else
                uiLVEnableMouseOver(globalIgnoreListView, 1);
        }

        // Draw the list and clip everything to inside the proper drawing area of the list view.
        clipperPushRestrict(&listViewDrawArea);
        uiLVDisplay(globalIgnoreListView, pen);
        clipperPop();

        // Handle list view input.
        uiLVHandleInput(globalIgnoreListView, pen);


        // Did the user pick someone from the ignore list?
        if(globalIgnoreListView->newlySelectedItem && globalIgnoreListView->selectedItem)
        {
            // Which entity was selected?
            ChatUser * item = globalIgnoreListView->selectedItem;
            Entity * player;
            if(item->db_id && (player = entFromDbId(item->db_id)))
            {
                if(player != playerPtr() && character_TargetIsFriend(playerPtr()->pchar, player->pchar))
                {
                    targetSelect(player);
                }
                targetSetHandle(item->handle); 
            }
            else
            {
                targetSelectHandle(item->handle, item->db_id);
            }

            globalIgnoreListView->newlySelectedItem = 0;
        }

        // If the target is on the ignore list
        if(GetGlobalIgnoreIdxByDbId(gSelectedDBID) >= 0 || GetGlobalIgnoreIdx(targetGetHandle()) >= 0)
        {
            int i;
            if(current_target)
            {    
                char * handle = targetGetHandle();
                globalIgnoreListView->selectedItem = 0;    
                for( i = 0; i < eaSizeUnsafe(&globalIgnoreListView->items); i++ )
                {
                    ChatUser *item = globalIgnoreListView->items[i];

                    if(    (item->db_id && item->db_id == current_target->db_id)
                        || (handle && !stricmp(item->handle, targetGetHandle())))
                    {
                        globalIgnoreListView->selectedItem = item;
                        break;
                    }

                }
            }
        }
        else
            globalIgnoreListView->selectedItem = 0;

        if(globalIgnoreListView->selectedItem)
        {
            int index;

             index = eaFind(&globalIgnoreListView->items, globalIgnoreListView->selectedItem);
            if(index != -1 && uiMouseCollision(globalIgnoreListView->itemWindows[index]) && mouseRightClick())
            {    
                int x,y;
                inpMousePos( &x, &y);
                contextMenu_set( interactGlobalIgnoreList, x, y, NULL );
            }
        }
    }

    // Clip and draw the remove friend button
    listViewDrawArea.y += listViewDrawArea.height;
    listViewDrawArea.height = FRIEND_REMOVE_AREA*scale;
    clipperPushRestrict(&listViewDrawArea);

     // ADD
    if(eaSize(&gGlobalIgnores))
            dx = wd/2 - (BUTTON_SPACE + BUTTON_WIDTH/2)*scale;
    else
        dx = wd/2;
     if(D_MOUSEHIT == drawStdButton(x + dx, y + ht - BUTTON_FOOTER*scale, z, BUTTON_WIDTH*scale, BUTTON_HEIGHT*scale, window_GetColor(WDW_FRIENDS), "AddString", scale, 0))
    {
        dialogNameEdit(textStd("AddGlobalIgnorePrompt"),"AddGlobalIgnore","CancelString", addGlobalIgnoreDlgHandler, NULL, MAX_PLAYERNAME, DLGFLAG_ALLOW_HANDLE );
    }


    if(eaSize(&gGlobalIgnores))
    {
        // REMOVE Button
         if(D_MOUSEHIT == drawStdButton(x + wd/2 + (BUTTON_WIDTH/2 + BUTTON_SPACE)*scale, y + ht - BUTTON_FOOTER*scale, z, BUTTON_WIDTH*scale, BUTTON_HEIGHT*scale, window_GetColor(WDW_FRIENDS), "RemoveString", scale, !((int)globalIgnoreListView->selectedItem)))
        {
            char buf[128];
            if(globalIgnoreListView && globalIgnoreListView->selectedItem)
            {
                 sprintf( buf, "gunignore @%s", ((ChatUser*)globalIgnoreListView->selectedItem)->handle );
                cmdParse( buf );
            }
        }
    }

    clipperPop();
    return 0;
}


void initGlobalFriendContextMenu()
{
    interactHandleOffline = contextMenu_Create( 0 );
    contextMenu_addCode( interactHandleOffline,        isGlobalFriend,        0, removeFromGlobalFriendsList, 0,          "CMRemoveGlobalFriendString",        0 );
    contextMenu_addCode( interactHandleOffline,        alwaysAvailable,    0, chatInitiateTell,            (void*)1, "CMSendOfflineMessageString",        0 );

    interactHandleExternalChat = contextMenu_Create( 0 );
    contextMenu_addCode( interactHandleExternalChat, isGlobalFriend,    0, removeFromGlobalFriendsList, 0,            "CMRemoveGlobalFriendString",    0 );
    contextMenu_addCode( interactHandleExternalChat, alwaysAvailable,    0, chatInitiateTell,            (void*)1,    "CMChatString",                    0 );
    // ideas: invite to channel, create & chat in private channel
}

void initGlobalIgnoreContextMenu()
{
    interactGlobalIgnoreList = contextMenu_Create( 0 );
    contextMenu_addCode( interactGlobalIgnoreList, isGlobalIgnore,        0, removeFromGlobalIgnoreList,        0,            "CMRemoveGlobalIgnoreString",    0 );
}


typedef int(*tabWindowFunc)(float x, float y, float z, float wd, float ht, float scale, int color, int bcolor, void * data);

typedef struct TabFunctionData
{
    tabWindowFunc func;
    void *data;
} TabFunctionData;

TabFunctionData oldFriend = { oldFriendWindow, 0 };
TabFunctionData newFriend = { newFriendWindow, 0 };
TabFunctionData globalIgnore = { globalIgnoreWindow, 0 };
TabFunctionData levelingpact = { levelingpactWindow, 0};

TabFunctionData chatMembers[MAX_WATCHING] = {    
                                                { channelWindow, 0 }, //0
                                                { channelWindow, 0 }, //1
                                                { channelWindow, 0 }, //2
                                                { channelWindow, 0 }, //3
                                                { channelWindow, 0 }, //4
                                                { channelWindow, 0 }, //5
                                                { channelWindow, 0 }, //6
                                                { channelWindow, 0 }, //7
                                                { channelWindow, 0 }, //8
                                                { channelWindow, 0 }, //9
                                                { channelWindow, 0 }, //10
                                                { channelWindow, 0 }, //11
                                                { channelWindow, 0 }, //12
                                                { channelWindow, 0 }, //13
                                                { channelWindow, 0 }, //14 (meaning 15 total)
};
STATIC_ASSERT(MAX_WATCHING == 15);  //MS: if this number changes,change the above, or cause a crash.
static ComboBox comboFriend;

#define TAB_HEADER (25)


void friendClearMaximize(void)
{
    if(gGlobalFriendMaximized)
    {
        float x, y, ht;
        window_getDims( WDW_FRIENDS, &x, &y, NULL, NULL, &ht, NULL, NULL, NULL );
        globalFriendListView->header = gGlobalMinHeader;
        window_setDims(WDW_FRIENDS, x, y, prev_wd, ht );
        gGlobalFriendMaximized = false;
    }
}

void channelWindowOpen( void * unused )
{
     window_setMode( WDW_FRIENDS, WINDOW_GROWING ); 
    if( eaSize(&comboFriend.elements) > 3 && comboFriend.currChoice <= 2 )
        comboFriend.currChoice = 3;
}

void selectChannelWindow(char * channel)
{
    int i;
    strcpy(comboSelectionTitle, channel);
    for (i = eaSize(&comboFriend.elements) - 1; i >= 0; i--)
    {
        if (strcmp(channel,comboFriend.elements[i]->title) == 0 && comboFriend.currChoice !=i)
        {
            comboFriend.currChoice = i;
            window_setMode( WDW_FRIENDS, WINDOW_GROWING ); 
            return;
        }
    }
}



int friendWindow()
{
    float x, y, z, wd, ht, scale;
    int color, bcolor;

    static bool init = false;

    
    static ComboCheckboxElement **cce;

    Wdw * wdw = wdwGetWindow(WDW_FRIENDS);
    Entity *e = playerPtr();

     if(!init)
         comboboxTitle_init( &comboFriend, 0, 0, 10, 100, 20, 600, WDW_FRIENDS );

    if( !init || channelListRebuildRequired)
    {
        int i;
        int chan_count = eaSize(&gChatChannels);

        // clear all channel tabs
        comboboxRemoveAllElements( &comboFriend );
        comboboxSharedElement_add( &cce, NULL, textStd("FriendTab"), textStd("FriendTab"), 0, (void*) &oldFriend );
        if(UsingChatServer())
        {
            comboboxSharedElement_add( &cce, NULL, textStd("GlobalFriendTab"), textStd("GlobalFriendTab"), 0, (void*) &newFriend );
            comboboxSharedElement_add( &cce, NULL, textStd("GlobalIgnoreTab"), textStd("GlobalIgnoreTab"), 0, (void*) &globalIgnore );
        }
        if(e && e->levelingpact_id)
            comboboxSharedElement_add( &cce, NULL, textStd("LevelingpactTab"), textStd("LevelingpactTab"), 0, (void*) &levelingpact );


        // make sure the right channel tabs exist
        for ( i = 0; i < chan_count; i++ )
        {
            chatMembers[i].data = gChatChannels[i];
            comboboxSharedElement_add( &cce, NULL, gChatChannels[i]->name, gChatChannels[i]->name, 0, &chatMembers[i] );
        }
        comboFriend.elements = cce;
        channelListRebuildRequired = false;
        init = true;
        //load up the correct tab
        selectChannelWindow(comboSelectionTitle);
    }

    if(comboFriend.changed)
    {
        char *newSelection = comboFriend.elements[comboFriend.currChoice]->title;
        if(newSelection!= 0)
            strcpy(comboSelectionTitle, newSelection);
    }

    // Do everything common windows are supposed to do.
    if ( !window_getDims( WDW_FRIENDS, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) || !playerPtr())
        return 0;

    if(wdw->loc.draggable_frame)
        wdw->loc.draggable_frame = RESIZABLE;

     if(globalFriendListView && !globalFriendListRebuildRequired)
    {
        // switching over from old friends list
        // If there is a pending rebuild, the values in the list view are crap and may crash
        SetGlobalFriendWidth(wdw);
    }

    // Draw the base friends list frame.
     drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, bcolor );

    
     if(UsingChatServer() || (e && e->levelingpact_id))
    {
        TabFunctionData * tabfuncdata;

        if( comboFriend.changed )
            friendClearMaximize();

        // chat server mode enabled
           comboFriend.wd = comboFriend.dropWd = wd/scale;
        combobox_display( &comboFriend );
        tabfuncdata = comboFriend.elements[comboFriend.currChoice]->data;

        return tabfuncdata->func(x, y+TAB_HEADER, z, wd, ht-TAB_HEADER, scale, color, bcolor, tabfuncdata->data);
    }
    else
    {
        return oldFriendWindow(x, y, z, wd, ht, scale, color, bcolor, NULL);
    }
}
