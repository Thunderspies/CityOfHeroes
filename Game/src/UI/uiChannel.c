#include "UI/uiChannel.h"
#include "UI/uiWindows.h"
#include "gameComm/wdwbase.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiBox.h"
#include "graphics/font.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "UI/uiChatUtil.h"
#include "graphics/ttFont.h"
#include "UI/uiClipper.h"
#include <utilitieslib/components/earray.h>
#include "UI/uiScrollBar.h"
#include "UI/uiComboBox.h"
#include <utilitieslib/utils/utils.h>
#include "assert.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiInput.h"
#include "UI/sprite/sprite_base.h"
#include "chatdb.h"
#include "UI/uiTarget.h"
#include "gameComm/chatClient.h"
#include "UI/uiContextMenu.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiFriend.h"
#include "UI/uiChat.h"
#include "win/input.h"
#include "uiListView.h"
#include "UI/uiDialog.h"
#include "formatter/smf_main.h"
#include "player/player.h"
#include "clientcomm/clientcomm.h"
#include "entity/entity.h"
#include <utilitieslib/utils/mathutil.h>
#include "graphics/textureatlas.h"
#include "entity/Supergroup.h"
#include <utilitieslib/language/MessageStoreUtil.h>


extern ChatFilter* gActiveChatFilter;

static ComboBox comboChannel = {0};
static ScrollBar sb = {0};
static ChatChannel * gCurrentChannel;

static UIListView* userLV = 0;
bool channelListRebuildRequired = true;
static bool userListRebuildRequired = true;
static char oldUserHandleSelection[128];



// --------------------------------------------------------------------------------
// Rebuild/Reset 
// --------------------------------------------------------------------------------

void notifyChannelDestroy(ChatChannel *channel)
{
    if (gCurrentChannel == channel)
        gCurrentChannel = 0;
}

void notifyRebuildChannelWindow()
{
    channelListRebuildRequired = true;
    notifyRebuildChannelUserList(0);
}

void notifyRemoveUser(ChatUser * user)
{
    if( userLV && user == ((ChatUser*)userLV->selectedItem))
    {
        userLV->selectedItem    = 0;
        userListRebuildRequired = 1;
    }
}

void notifyRebuildChannelUserList(ChatChannel * channel)
{
    // always rebuilds if channel is NULL
    if( !channel || channel == gCurrentChannel)
    {
        if(userLV && userLV->selectedItem && ((ChatUser*)userLV->selectedItem)->handle)
            strcpy(oldUserHandleSelection, ((ChatUser*)userLV->selectedItem)->handle);
        else
            strcpy(oldUserHandleSelection, "");

         userListRebuildRequired = 1;
    }
}

void resetChannelWindow()
{
    // TODO (when quitting to login screen...

    memset(&sb, 0, sizeof(ScrollBar));

    uiLVClear(userLV);

    notifyRebuildChannelWindow();
    notifyRebuildChannelUserList(0);

}


// -------------------------------------------------------------------------------------------
// Channel Invite Menu
// -------------------------------------------------------------------------------------------
static ContextMenu * gChannelInviteMenu;

int canInvite_CM(void * foo)
{
    if(!UsingChatServer())
        return CM_HIDE;

    if(channelInviteAllowed(gCurrentChannel))
        return CM_AVAILABLE;
    else
        return CM_VISIBLE;
}

int canInviteGF_CM(void * foo)
{
    if(!UsingChatServer())
        return CM_HIDE;

    if(channelInviteAllowed(gCurrentChannel) && eaSize(&gGlobalFriends))
        return CM_AVAILABLE;
    else
        return CM_VISIBLE;
}

int canInviteTeam_CM(void * foo)
{
    Entity * e = playerPtr();

    if(!UsingChatServer())
        return CM_HIDE;

    if(channelInviteAllowed(gCurrentChannel) && (e->teamup || e->taskforce))
        return CM_AVAILABLE;
    else
        return CM_VISIBLE;
}

int canInviteSupergroup_CM(void * foo)
{
    Entity *e;
    Supergroup *sg;
    int myRank;
    int i;

    if(!UsingChatServer())
    {
        return CM_HIDE;
    }
    
    if ((e = playerPtr()) == NULL || (sg = e->supergroup) == NULL)
    {
        return CM_HIDE;
    }

    myRank = 0;
    for (i = eaSize(&sg->memberranks) - 1; i >= 0; i--)
    {
        if (sg->memberranks[i] && sg->memberranks[i]->dbid == e->db_id)
        { 
            myRank = sg->memberranks[i]->rank;
            break;
        }
    }

    // DGNOTE 3/16/2009 SG Channel invite
    // Change this test to look at permissions if we elect to add a permission bit for this.
    if (channelInviteAllowed(gCurrentChannel) && myRank >= 4) // leader
    {
        return CM_AVAILABLE;
    }
    else
    {
        return CM_VISIBLE;
    }
}

void chanInviteDlgHandler(void * data)
{
    if(gCurrentChannel)
    {
        char buf[1000];
        sprintf(buf, "chan_invite \"%s\"%s", gCurrentChannel->name, dialogGetTextEntry());
        cmdParse(buf);
    }
}


void chanInvite_CM(void * unused)
{
    if(gCurrentChannel)
    {
        dialogNameEdit(textStd("InviteToChannelDialog", gCurrentChannel->name) , NULL, NULL, chanInviteDlgHandler, NULL, MAX_CHANNELNAME, DLGFLAG_ALLOW_HANDLE );
    }
}

void chanInviteGF_CM(void * unused)
{
    if(gCurrentChannel)
    {
        char buf[1000];
        sprintf(buf, "chan_invite_gf %s", gCurrentChannel->name);
        cmdParse(buf);
    }
}

void chanInviteTeam_CM(void * unused)
{
    if(gCurrentChannel)
    {
        char buf[1000];
        sprintf(buf, "chan_invite_team %s", gCurrentChannel->name);
        cmdParse(buf);
    }
}


void chanInviteSupergroup_CM(void * rank)
{
    if(gCurrentChannel)
    {
        char buf[1000];
        sprintf(buf, "chan_invite_sg \"%s\" %d", gCurrentChannel->name, (int)rank);
        cmdParse(buf);
    }
}

ContextMenu * getChanInviteSupergroupMenu()
{
    static ContextMenu * s_menu = 0;

    if(!s_menu)
    {
        s_menu = contextMenu_Create( NULL );
        contextMenu_addCode(s_menu, alwaysAvailable, 0, chanInviteSupergroup_CM, (void*) 4, "ChanInviteSGLeader",        0); 
        contextMenu_addCode(s_menu, alwaysAvailable, 0, chanInviteSupergroup_CM, (void*) 3, "ChanInviteSGCommander",    0); 
        contextMenu_addCode(s_menu, alwaysAvailable, 0, chanInviteSupergroup_CM, (void*) 2, "ChanInviteSGCaptain",        0); 
        contextMenu_addCode(s_menu, alwaysAvailable, 0, chanInviteSupergroup_CM, (void*) 1, "ChanInviteSGLieutenant",    0); 
        contextMenu_addCode(s_menu, alwaysAvailable, 0, chanInviteSupergroup_CM, (void*) 0, "ChanInviteSGAllMembers",    0); 
    }

    return s_menu;
}

void openChannelInviteMenu( float x, float y )
{
    if( !gChannelInviteMenu )
    {
        gChannelInviteMenu = contextMenu_Create(NULL);
        contextMenu_addCode(gChannelInviteMenu, canInvite_CM,            0,        chanInvite_CM,            0, "CMChanInvite",                0);
        contextMenu_addCode(gChannelInviteMenu, canInviteGF_CM,            0,        chanInviteGF_CM,        0, "CMChanInviteGlobalFriend",    0);
        contextMenu_addCode(gChannelInviteMenu, canInviteTeam_CM,        0,        chanInviteTeam_CM,        0, "CMChanInviteTeam",            0);
        contextMenu_addCode(gChannelInviteMenu, canInviteSupergroup_CM,    0, 0, 0, "CMChanInviteSupergroup", getChanInviteSupergroupMenu());
    }

    contextMenu_set( gChannelInviteMenu, x, y, NULL );
}


// -------------------------------------------------------------------------------------------
// Context Menu utilities -- usable by both context menus & button code
// -------------------------------------------------------------------------------------------
ContextMenu *interactChannelUser = 0;

bool isOperator(int flags)
{
    return (   flags & CHANFLAGS_OPERATOR
            || flags & CHANFLAGS_ADMIN);
}

bool isUserOperator(ChatUser * user)
{
    return (user && isOperator(user->flags));
}

bool isAdmin(int flags)
{
    return (flags & CHANFLAGS_ADMIN);
}


bool notMe(ChatUser * user)
{
    if(gCurrentChannel)
    {
        assert(gCurrentChannel->me);
        return (gCurrentChannel->me != user);
    }
    else
        return 0;
}

bool isMe(ChatUser * user)
{
    if(gCurrentChannel)
    {
        assert(gCurrentChannel->me);
        return (gCurrentChannel->me == user);
    }
    else
        return 0;
}

// operators can act other operators & below, admins can act on ANYONE (including other admins)
bool canModify(ChatUser * user)
{
    if(user)
    {
         if(isAdmin(user->flags))
            return isAdmin(gCurrentChannel->me->flags);
        else
            return isOperator(gCurrentChannel->me->flags);
    }

    return false;
}

int canMakeOperator_CM(void * foo)
{
    ChatUser * user = GetChannelUser(gCurrentChannel, targetGetHandle());

    if(!UsingChatServer())
        return CM_HIDE;

     if(canModify(user) && notMe(user) && !(user ? isAdmin(user->flags) : 0))
        return CM_AVAILABLE;
    else
        return CM_HIDE;
}

int canModify_CM(void * foo)
{
     ChatUser * user = GetChannelUser(gCurrentChannel, targetGetHandle());
         
    if(!UsingChatServer())
        return CM_HIDE;

    if(canModify(user) && notMe(user))
        return CM_AVAILABLE;
    else
        return CM_HIDE;
}


int isSilenced(ChatUser * user)
{
    if(user)
         return ! (user->flags & CHANFLAGS_SEND);
    else
        return 0;
}

int isSilenced_CM(void * foo)
{
    ChatUser * user = GetChannelUser(gCurrentChannel, targetGetHandle());

    if(!UsingChatServer())
        return CM_HIDE;

    if(isSilenced(user))
        return CM_AVAILABLE;

    return CM_HIDE;
}

int isUnsilenced(ChatUser * user)
{
    if(user)
        return (user->flags & CHANFLAGS_SEND);
    else
        return 0;
}

int isUnsilenced_CM(void * foo)
{
    ChatUser * user = GetChannelUser(gCurrentChannel, targetGetHandle());

    if(!UsingChatServer())
        return CM_HIDE;

    if(isUnsilenced(user))
        return CM_AVAILABLE;

    return CM_HIDE;
}


int canSilence(ChatUser * user)
{
    if(user)
        return (canModify(user) && isUnsilenced(user) && notMe(user));
    else
        return 0;
}

int canSilence_CM(void * foo)

{    
    ChatUser * user = GetChannelUser(gCurrentChannel, targetGetHandle());    

    if(!UsingChatServer())
        return CM_HIDE;

    if(canSilence(user))
        return CM_AVAILABLE;

    return CM_HIDE;
}

int canUnsilence(ChatUser * user)
{
    if(user)
        return (canModify(user) && isSilenced(user));
    else
        return 0;
}



int canUnsilence_CM(void * foo)
{        
    ChatUser * user = GetChannelUser(gCurrentChannel, targetGetHandle());

    if(!UsingChatServer())
        return CM_HIDE;

    if(canUnsilence(user))
        return CM_AVAILABLE;

    return CM_HIDE;
}


/**********************************************************************func*
* Target Context Menu
*
*/

bool channelInviteAllowed(ChatChannel * channel)
{
    if(channel)
    {
        if(eaSize(&channel->users) >= MAX_MEMBERS)
            return 0;    // FULL
        else if(channel->flags & CHANFLAGS_JOIN)
            return 1;    // PUBLIC
        else if (channel->me)
            return isOperator(channel->me->flags);    // OPERATOR
    }

    return 0;
}



int canInviteTargetToChannel(void * data)
{
    char * handle = targetGetHandle();
    char * name = targetGetName();
    int db_id = targetGetDBID();
    int idx = ((int)data)-1;

    if(      idx < eaSize(&gChatChannels)
        &&     idx >= 0
        &&   channelInviteAllowed(gChatChannels[idx]))
    {
        if(handle && ! GetChannelUser(gChatChannels[idx], handle))
            return CM_AVAILABLE;
        if(db_id && ! GetChannelUserByDbId(gChatChannels[idx], db_id))
            return CM_AVAILABLE;
    }

    return CM_HIDE;
}

int canInviteTargetToAnyChannel(void * foo)
{
    int i, size = eaSize(&gChatChannels);

    if(!UsingChatServer())
        return CM_HIDE;

    for(i=1;i<=size;i++)
    {
        if(canInviteTargetToChannel((void*)i))
            return CM_AVAILABLE;
    }

    return CM_HIDE;
}

void inviteTargetToChannel(void * data)
{
    char    *targetHandle = targetGetHandle();
    char    *targetName = targetGetName();
    int        targetDbId = targetGetDBID();
    int        idx = ((int)data)-1;

    if(    idx >=  eaSize(&gChatChannels)
        || idx < 0
        || ! gChatChannels[idx])
    {
        return;
    }

    if(targetName)
    { //go by name first, name is updated when right clicking
        char buf[200];
        sprintf( buf, "chan_invite \"%s\"%s", gChatChannels[idx]->name, targetName);
        cmdParse( buf );
    }
    else if(targetHandle)
    {
        char buf[200];
        sprintf( buf, "chan_invite \"%s\"@%s", gChatChannels[idx]->name, targetHandle);
        cmdParse( buf );
    }
    else if(targetDbId > 0)
    {
        sendShardCmd("invite", "%s %d %d", gChatChannels[idx]->name, kChatId_DbId, targetDbId);
    }
    else
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
    }
}

ContextMenu * getChannelInviteMenu()
{
    int i;

    static ContextMenu * s_menu = 0;
    
    if(!s_menu)
    {
        s_menu = contextMenu_Create( NULL );

        for(i=1;i<=MAX_WATCHING;i++)
        {
            contextMenu_addVariableTextCode(s_menu, canInviteTargetToChannel, (void*) i, inviteTargetToChannel, (void*) i, chatcm_userChannelText, (void*) i, 0); 
        }
    }

    return s_menu;
}

//
//int isUnsilenced_CM(ChatUser * user)
//{
//    if(!UsingChatServer())
//        return CM_HIDE;
//
//    if(gCurrentChannel)
//    {
//        if(!user)
//            user = GetChannelUser(gCurrentChannel, targetGetHandle());
//
//        if( ! isSilenced(user))
//            return CM_AVAILABLE;
//    }
//
//    return CM_HIDE;
//}


void changeUserMode_Long(char * handle, char * mode)
{
    if(handle && gCurrentChannel && mode)
    {
        char buf[1000];
         sprintf(buf, "chan_usermode \"%s\" \"%s\" \"%s\"", gCurrentChannel->name, handle, mode);
        cmdParse(buf);
    }
}

void changeChannelMode_Long(char * channel, char * mode)
{
    if(channel && mode)
    {
        char buf[1000];
         sprintf(buf, "chan_mode \"%s\" %s", channel, mode);
        cmdParse(buf);
    }
}

void changeUserMode(char * mode)
{
    changeUserMode_Long(targetGetHandle(), mode);
}

void initChannelUserContextMenu()
{
    interactChannelUser = contextMenu_Create( 0 );
    contextMenu_addCode( interactChannelUser, isNotGlobalFriend,    0, addToGlobalFriendsList,        0,            "CMAddGlobalFriendString",0 );
    contextMenu_addCode( interactChannelUser, isGlobalFriend,        0, removeFromGlobalFriendsList, 0,            "CMRemoveGlobalFriendString", 0 );
    contextMenu_addCode( interactChannelUser, alwaysAvailable,        0, chatInitiateTell,            (void*)1,    "CMChatString", 0 );

    contextMenu_addCode( interactChannelUser, canMakeOperator_CM,    0, changeUserMode,    "-join",        "CMKickFromChannelString", 0 );
    contextMenu_addCode( interactChannelUser, canSilence_CM,        0, changeUserMode,    "-send",        "CMSilenceString", 0 );
    contextMenu_addCode( interactChannelUser, canUnsilence_CM,        0, changeUserMode,    "+send",        "CMUnsilenceString", 0 );
    contextMenu_addCode( interactChannelUser, canModify_CM,            0, changeUserMode,    "+operator",    "CMMakeOperatorString", 0 );

    contextMenu_addCode( interactChannelUser, canInviteTargetToAnyChannel, 0, 0, 0, "CMChannelInvite", getChannelInviteMenu());
}

//------------------------------------------------------------------------------------
// User ListView 
//------------------------------------------------------------------------------------

#define USER_HANDLE_COLUMN    "UserHandleColumn"

int compareUserHandle(const ChatUser** f1, const ChatUser** f2)
{
    return stricmp((*f1)->handle, (*f2)->handle); 
}



UIBox userLVDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, ChatUser* item, int itemIndex)
{
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
    else if(isAdmin(item->flags))
        color = 0xff8888ff; // light red
    else if(isOperator(item->flags))
        color = CLR_ONLINE_ITEM;
    else
        color = CLR_WHITE;

    font_color(color, color);

    // Iterate through each of the columns.
    for(;uiCHIGetNextColumn(&columnIterator, list->scale);)
    {
        UIColumnHeader* column = columnIterator.column;
        UIBox clipBox;
        int rgba[4] = {color, color, color, color };

        // Decide what to draw on on which column.
         if(stricmp(column->name, USER_HANDLE_COLUMN) == 0)
        {
            AtlasTex * flag = flagIcon(item->flags);

            pen.x = rowOrigin.x + columnIterator.columnStartOffset*list->scale;

             clipBox.origin.x = pen.x;
            clipBox.origin.y = pen.y;
             clipBox.height = 22*list->scale;

            if( columnIterator.columnIndex >= eaSize(&columnIterator.header->columns )-1 )
                clipBox.width = 1000;
            else
                clipBox.width = columnIterator.currentWidth;

            clipperPushRestrict(&clipBox);
            
            if(flag)
            {
                float sc = (20.f*list->scale)/flag->width;
                 display_sprite( flag, pen.x, pen.y + (24*list->scale - flag->width*sc)/2, rowOrigin.z, sc, sc, CLR_WHITE);
            }

             pen.x += 23*list->scale;

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



void confirmLeaveChannelDialog(void * data)
{
    if(gCurrentChannel)
    {
        leaveChatChannel(gCurrentChannel->name);
    }
}

//-------------------------------------------------------------
// Channel Window
//-------------------------------------------------------------

// for "no channels" message
static TextAttribs s_taText =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)CLR_WHITE,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)CLR_WHITE,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_14,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)CLR_WHITE,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)CLR_WHITE,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

#define LIST_HEADER (46)

#define B_WD    (73)
#define B_SPACE    (8)
#define B_HT    (26)
#define B_FOOTER    (B_HT/2 + PIX3 + 5)
#define USER_BUTTON_AREA (80 + PIX3)

#define BUTTON_HT    (21) 
#define BUTTON_WD    (70) 
#define BUTTON_GAP    (3) 

static int __cdecl compareChannelNames(const ChatChannel** c1, const ChatChannel** c2)
{
    return stricmp((*c1)->name, (*c2)->name);
}

void selectedTabCallback( )
{
    notifyRebuildChannelUserList(0);
    gCurrentChannel = gChatChannels[comboChannel.currChoice];
}

int channelWindow(float x, float y, float z, float wd, float ht, float sc, int color, int bcolor, void * data)
{

    UIBox windowDrawArea;
    UIBox lvDrawArea;
       ChatUser * user = 0, *newUser;
    PointFloatXYZ pen;
    int op, size;
    float buttonWD, buttonHT, buttonX, buttonXOffset, buttonY, buttonYOffset;
    char * handle;
    
    static ChatChannel * last_chatChannel = 0;
    static bool init = false;
    static int lastWidth = 0;

    // update current channel
    gCurrentChannel = data;
    if( gCurrentChannel != last_chatChannel )
        userListRebuildRequired = true;
    last_chatChannel = gCurrentChannel;

    if (!gCurrentChannel || !gCurrentChannel->me)
    {
        // reset and close (stolen from the !UsingChatServer block below)
        resetChannelWindow();
        return 0;
    }

    // see if we are an op
    op = isOperator(gCurrentChannel->me->flags);

     if(!init)
    {
        initChannelUserContextMenu();
        init = true;
    }

    if( ! UsingChatServer() )
    {
        // reset and close
        resetChannelWindow();
        return 0;
    }
    
    font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);

    windowDrawArea.x = x;
    windowDrawArea.y = y;
    windowDrawArea.width = wd;
    windowDrawArea.height = ht;
    uiBoxAlter(&windowDrawArea, UIBAT_SHRINK, UIBAD_ALL, PIX3*sc);

     lvDrawArea = windowDrawArea;

    if(op)
        uiBoxAlter(&lvDrawArea, UIBAT_SHRINK, UIBAD_BOTTOM, USER_BUTTON_AREA*sc);
    else
         uiBoxAlter(&lvDrawArea, UIBAT_SHRINK, UIBAD_BOTTOM, (BUTTON_HT+PIX3)*sc);

     // The list view will be drawn inside the lvDrawArea and above the window frame.
     pen.x = lvDrawArea.x;
     pen.y = lvDrawArea.y;
    pen.z = z+1;                    // Draw on top of the frame
 
    // Do nothing if no filter is selected
     if( !gCurrentChannel || 0 == eaSize(&gChatChannels))
    {
         s_taText.piScale = (int *)((int)(sc*SMF_FONT_SCALE));
        smf_ParseAndDisplay(NULL, textStd("DoNotBelongToAnyChannels"), windowDrawArea.x + R10*sc, windowDrawArea.y + ht/8, z+1, 
            windowDrawArea.width - 2*R10*sc, 120*sc, 1, 0, &s_taText, NULL, 0, true);
        return 0;
    }

    /// -----------------------------------------
     // Draw and handle all scroll bar functionality if the user list is larger than the window.
    // This is separate from the list view because the scroll bar is always outside the draw area
    // of the list view.
      if(userLV)
    {
        int fullDrawHeight = uiLVGetFullDrawHeight(userLV);
        int currentHeight = uiLVGetHeight(userLV);
        if(userLV)
        {
             if(fullDrawHeight > currentHeight)
            {
                 doScrollBar( &sb, currentHeight-15*sc, fullDrawHeight-15*sc, x+wd, y+(PIX3+R10+22)*sc, z+5, 0, &lvDrawArea );
                userLV->scrollOffset = sb.offset;
            }
            else
                userLV->scrollOffset = 0;
        }
    }

    // Make sure the list view is initialized.
    if(!userLV)
     {
        userLV = uiLVCreate();
        if( !(gCurrentChannel->flags & CHANFLAGS_JOIN) )
            uiLVHAddColumn(userLV->header,  USER_HANDLE_COLUMN, textStd("PrivateChannelUserListViewCaption", eaSize(&gCurrentChannel->users)), 0 );
        else
             uiLVHAddColumn(userLV->header,  USER_HANDLE_COLUMN, textStd("ChannelUserListViewCaption", eaSize(&gCurrentChannel->users)), 0 );
        uiLVBindCompareFunction(userLV, USER_HANDLE_COLUMN, compareUserHandle);
        userLV->displayItem = userLVDisplayItem;
        uiLVEnableMouseOver(userLV, 1);
        uiLVEnableSelection(userLV, 1);
    }

    // How tall and wide is the list view supposed to be?
    uiLVSetDrawColor(userLV, window_GetColor(WDW_FRIENDS));
    uiLVSetHeight(userLV, lvDrawArea.height);
      uiLVHSetWidth(userLV->header, lvDrawArea.width);
    uiLVSetScale( userLV, sc );
     uiLVHSetColumnWidth(userLV->header, USER_HANDLE_COLUMN, (wd - 2*PIX3*sc));
    size = eaSize(&gCurrentChannel->users);
    // Rebuild the entire friends list if required.
     if(userLV && userListRebuildRequired)
    {
        int i; 

        uiLVClear(userLV);
        for(i = 0; i < size; i++)
        {
            uiLVAddItem(userLV, gCurrentChannel->users[i]);
            if(!stricmp(gCurrentChannel->users[i]->handle, oldUserHandleSelection))
                uiLVSelectItem(userLV, i);
        }

        strcpy(oldUserHandleSelection, "");

        uiLVSortBySelectedColumn(userLV);
        uiLVHSetColumnWidth(userLV->header, USER_HANDLE_COLUMN, (wd - 2*PIX3*sc));
        userListRebuildRequired = 0;
    }

    if( !(gCurrentChannel->flags & CHANFLAGS_JOIN) )
        uiLVHSetColumnCaption(userLV->header, USER_HANDLE_COLUMN, textStd("PrivateChannelUserListViewCaption", size));
    else
        uiLVHSetColumnCaption(userLV->header, USER_HANDLE_COLUMN, textStd("ChannelUserListViewCaption", size));

    // Disable mouse over on window resize.
    {
         Wdw* channelWindow = wdwGetWindow(WDW_FRIENDS);
        if(channelWindow->drag_mode)
            uiLVEnableMouseOver(userLV, 0);
        else
            uiLVEnableMouseOver(userLV, 1);
    }

    // Draw the draw the list and clip everything to inside the proper drawing area of the list view.
      clipperPushRestrict(&lvDrawArea);
     uiLVDisplay(userLV, pen);
    clipperPop();

    // Handle list view input.
     uiLVHandleInput(userLV, pen);
    user = userLV->selectedItem;


    // Did the user pick someone from the user list?
    if(userLV->newlySelectedItem && user)
    {
         targetSelectHandle(user->handle, user->db_id);
        userLV->newlySelectedItem = 0;
    }

    // Did the user pick someone else outside the window?
    handle = targetGetHandle();
    if( !handle || (userLV->selectedItem && stricmp(handle, ((ChatUser*)userLV->selectedItem)->handle)))
    {
        userLV->selectedItem = 0;
    }

    // did the user choose a channel member outside the channel window (ie in the global friend window)
    if((newUser = GetChannelUser(gCurrentChannel, handle)))
    {
        userLV->selectedItem = newUser;
    }

    // Context Menu...
    if(user)
    {
        int index = eaFind(&userLV->items, user);
        if(index != -1 && uiMouseCollision(userLV->itemWindows[index]) && mouseRightClick())
        {
             int x, y;
            inpMousePos( &x, &y);
             contextMenu_set( interactChannelUser, x, y, NULL );
        }
    }
    
    if(!gCurrentChannel->me)
    {
        // i never got a "join" message...
        printf("Deleted channel %s because never recieved a 'join' command from server\n", gCurrentChannel->name);
        ChatChannelCloseAndDestroy(gCurrentChannel);
        return 0;
    }

       buttonWD = MIN(150,((wd - 2*PIX3*sc) - (3*BUTTON_GAP)*sc)/2);
    buttonHT = BUTTON_HT*sc;

     buttonX = x + wd/2;

      if(isOperator(gCurrentChannel->me->flags))
        buttonXOffset = buttonWD/2 + BUTTON_GAP*sc;
    else
        buttonXOffset = 0;
     buttonY = y + ht - ((PIX3 + BUTTON_GAP)*sc + buttonHT/2);
      buttonYOffset = buttonHT + BUTTON_GAP*sc;


    // OPERATOR COMMANDS
       if(isOperator(gCurrentChannel->me->flags))
    {
         bool silenced = isSilenced(user);
        bool locked;

        // SILENCE/UNSILENCE Button
          if(silenced)
            locked = !canUnsilence(user);
        else
            locked = !canSilence(user);
          if(D_MOUSEHIT == drawStdButton(buttonX + buttonXOffset, buttonY - 0*buttonYOffset, z, MIN(150,buttonWD), buttonHT, color, (silenced ? "UnsilenceButton" : "SilenceButton"), sc, locked))
        {
            changeUserMode_Long(user->handle, (silenced ? "+send" : "-send"));
        }

        // KICK Button
         locked = (!canModify(user) || isMe(user));
            if(D_MOUSEHIT == drawStdButton(buttonX + buttonXOffset, buttonY - 1*buttonYOffset, z, MIN(150,buttonWD), buttonHT, color, "KickUserButton", sc, locked))
        {
            changeUserMode_Long(user->handle, "-join");
        }

        // MAKE OPERATOR Button
        locked = (!canModify(user) || isUserOperator(user));
           if(D_MOUSEHIT == drawStdButton(buttonX + buttonXOffset, buttonY - 2*buttonYOffset, z, MIN(150,buttonWD), buttonHT, color, "MakeOperatorButton", sc, locked))
        {
            changeUserMode_Long(user->handle, "+operator");
         }


        // MAKE PRIVATE Button
        if( gCurrentChannel->flags & CHANFLAGS_JOIN)
        {
            if(D_MOUSEHIT == drawStdButton(buttonX - buttonXOffset, buttonY - 2*buttonYOffset, z, MIN(150,buttonWD), buttonHT, color, "MakeChannelPrivate", sc, 0))
            {
                changeChannelMode_Long(gCurrentChannel->name, "-join");
            }
        }
        // MAKE PUBLIC Button
        else //if(gCurrentChannel->flags & CHANFLAGS_JOIN)
        {
            if(D_MOUSEHIT == drawStdButton(buttonX - buttonXOffset, buttonY - 2*buttonYOffset, z, MIN(150,buttonWD), buttonHT, color, "MakeChannelPublic", sc, 0))
            {
                changeChannelMode_Long(gCurrentChannel->name, "+join");
            }
        }


        // INVITE Button -- allow if enough room and either 1) it's public channel or 2) you're an operator/admin
        if(channelInviteAllowed(gCurrentChannel))
        {
            if(D_MOUSEHIT == drawStdButton(buttonX - buttonXOffset, buttonY - 1*buttonYOffset, z, MIN(150,buttonWD), buttonHT, color, "ChannelInviteButton", sc, 0))
            {
                openChannelInviteMenu( buttonX - buttonXOffset, buttonY - 1*buttonYOffset );
            }
        }
    }

    // LEAVE Button
      if(D_MOUSEHIT == drawStdButton(buttonX - buttonXOffset, buttonY - 0*buttonYOffset, z, MIN(150,buttonWD), buttonHT, color, "LeaveChannelButton", sc, 0))
    {
        dialogStd(DIALOG_YES_NO, textStd("ConfirmLeaveChannelDialog"), NULL, NULL, confirmLeaveChannelDialog, NULL, DLGFLAG_GAME_ONLY);
    }
        

    return 0;
}
