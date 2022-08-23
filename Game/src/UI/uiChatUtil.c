#include <utilitieslib/stdtypes.h>
#include "UI/uiChatUtil.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/earray.h>
#include "assert.h"
#include "gameComm/wdwbase.h"
#include "clientcomm/clientcomm.h"
#include "UI/uiConsole.h"
#include "uiChatOptions.h"
#include "graphics/ttFontUtil.h"
#include "entity/entPlayer.h"
#include "UI/uiWindows.h"
#include "UI/uiChat.h"
#include "UI/uiChannel.h"
#include "UI/uiDialog.h"
#include "cmdparse/cmdgame.h"
#include "UI/sprite/sprite_text.h"
#include "UI/uiUtil.h"
#include <utilitieslib/utils/mathutil.h>
#include "chatdb.h"
#include "UI/uiContextMenu.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "formatter/smf_render.h"
#include "player/player.h"
#include "entity/entity.h"

static ContextMenu *tabContextMenu = 0;

char tmpChannelName[MAX_CHANNELNAME];

MP_DEFINE(ChatLine);
ChatLine* ChatLine_Create(void)
{
    ChatLine *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(ChatLine, 64);
    res = MP_ALLOC( ChatLine );
    if( res )
    {
        res->pBlock = smfBlock_Create();
    }
    return res;
}

void ChatLine_Destroy( ChatLine *hItem )
{
    if(hItem)
    {
        smfBlock_Destroy(hItem->pBlock);
        SAFE_FREE(hItem->pchText);
        MP_FREE(ChatLine, hItem);
    }
}

/***********************************************************************
* ChatUser    (member of a chat channel)
*
*/

ChatUser * ChatUserCreate(char * handle, int db_id, int flags)
{
    ChatUser * user = calloc(sizeof(ChatUser),1);

    user->handle = strdup(handle);
    user->flags = flags;
    user->db_id = db_id;

    return user;
}

void ChatUserRename(ChatUser * user, char * newHandle)
{
    user->handle = realloc(user->handle, (strlen(newHandle)+1));
    strcpy(user->handle, newHandle);
}

void ChatUserDestroy(ChatUser * user)
{
    notifyRemoveUser(user);
    free(user->handle);
    free(user);
}


/***********************************************************************
* ChatChannel
*
*/



ChatChannel ** gChatChannels = NULL;
ChatChannel ** gReservedChatChannels = NULL;

ChatChannel * ChatChannelCreate(const char * name, int reserved)
{
    ChatChannel * channel = calloc(1, sizeof(ChatChannel));
    Entity *e = playerPtr();
    ChatSettings *settings;
    int i;

    if( e && e->pl )
        settings = &e->pl->chat_settings;
    else
        return 0;

    channel->name = strdup(name);
    channel->isReserved = reserved;

    eaCreate(&channel->filters);
    eaCreate(&channel->users);

    if(reserved)
        eaPush(&gReservedChatChannels, channel);
    else
        eaPush(&gChatChannels, channel);

    // look for saved color
    for(i = 0; i < MAX_CHAT_CHANNELS; i++ )
    {
        if( settings->channels[i].name && stricmp(settings->channels[i].name, name) == 0)
        {
            channel->color = settings->channels[i].color1;
            channel->color2 = settings->channels[i].color2;
        }
    }

    if(!channel->color) // use defaults
        channelSetDefaultColor(channel);

    notifyRebuildChannelWindow();

    return channel;
}

void ChatChannelDestroy(ChatChannel * channel)
{
    // let chat tab options screen know that this channel is going to be deleted
    channelDeleteNofication(channel);

    // now get rid of it

    assert(GetChannel(channel->name) || GetReservedChannel(channel->name));

    notifyChannelDestroy(channel);

    notifyRebuildChannelWindow();

    eaRemove(&gChatChannels, eaFind(&gChatChannels, channel));
    eaFindAndRemove(&gReservedChatChannels, channel);

    eaDestroy(&channel->filters);    // should I also clean from windows here?
    
    eaDestroyEx(&channel->users, ChatUserDestroy);
    
    free(channel->name);
    free(channel);
}

ChatChannel * GetChannel(const char * name)
{
    int i, size = eaSize(&gChatChannels);
    for(i=0;i<size;i++)
    {
        ChatChannel * cc = gChatChannels[i];
        if(cc && !stricmp(cc->name, name))
            return cc;
    }
    return 0;
}

ChatChannel * GetReservedChannel(const char * name)
{
    int i, size = eaSize(&gReservedChatChannels);
    for(i=0;i<size;i++)
    {
        ChatChannel * cc = gReservedChatChannels[i];
        if(cc && !stricmp(cc->name, name))
            return cc;
    }
    return 0;
}

// remove chat channel from all filters (need to make copy since ChatFilterRemoveChannel() modifieds the channel's filter list)
void ChatChannelCloseAndDestroy(ChatChannel * channel)
{
    int i, size;

    while( (size = eaSize(&channel->filters)) )
    {
        ChatFilterRemoveChannel(channel->filters[size - 1], channel);
    }

    // debug double-checks
    for(i=0;i<eaSize(&gChatFilters);i++)
    {
        assert(!ChatFilterHasChannel(gChatFilters[i], channel));
    }
    assert(eaSize(&channel->filters) == 0);

    ChatChannelDestroy(channel);

}



ChatUser * GetChannelUser(ChatChannel * channel, char * handle)
{
    int i;

    if(!channel || !handle)
        return 0;

    for(i=0;i<eaSize(&channel->users);i++)
    {
        if(!stricmp(channel->users[i]->handle, handle))
        {
            return channel->users[i];
        }
    }
    return 0;
}


ChatUser * GetChannelUserByDbId(ChatChannel * channel, int db_id)
{
    int i;

    if(!channel || db_id <= 0)
        return 0;

    for(i=0;i<eaSize(&channel->users);i++)
    {
        if(channel->users[i]->db_id == db_id)
            return channel->users[i];
    }
    return 0;
}

// called to add user or to update existing user
ChatUser * ChatChannelUpdateUser(ChatChannel * channel, char * handle, int db_id, int flags)
{
    ChatUser * user = GetChannelUser(channel, handle);
    if(!user)
    {
        user = ChatUserCreate(handle, db_id, flags);
        eaPush(&channel->users, user);
    }
    else
    {
        user->db_id = db_id;    // this should not change, but just in case...
        user->flags = flags;
    }

    notifyRebuildChannelUserList(channel);

    return user;
}

void ChatChannelRemoveUser(ChatChannel * channel, char * handle)
{
    ChatUser * user = GetChannelUser(channel, handle);
    if(user)
    {        
        notifyRebuildChannelUserList(channel);

        eaRemove(&channel->users, eaFind(&channel->users, user));
        ChatUserDestroy(user);
    }
}


/**********************************************************************func*
* ChatFilter
*
*/

ChatFilter** gChatFilters;    // global list of all current filters
ChatFilter* gActiveChatFilter;    // the active filter gets most system messages & also text input from user 
ChatFilter* gPrevActiveChatFilter;    // the LAST active chat filter, prior to the current one. Used only to toggle between two active tabs
ChatFilter* gWaitingForChannelFilter; // stores the active channel at the time the join command was sent. (allows some delay to occur between join & confirmation)

// USE THIS TO SET ACTIVE CHAT FILTER!!!!
void SelectActiveChatFilter(ChatFilter * filter)
{    
     if(filter)
    {
         if(filter != gActiveChatFilter)
        {    
            int i;
            gPrevActiveChatFilter = gActiveChatFilter;
            gActiveChatFilter = filter;

            if(gActiveChatFilter)
            {
                for(i=0;i<MAX_CHAT_WINDOWS;i++)
                {
                    ChatWindow * cw = GetChatWindow(i);
                    if( ChatWindowHasFilter(cw, gActiveChatFilter) == 1 )
                    {
                        cw->contextPane = 0;
                        uiTabControlSelect(cw->topTabControl, gActiveChatFilter);
                        break;
                    }
                    else if( ChatWindowHasFilter(cw, gActiveChatFilter) == 2 )
                    {
                        cw->contextPane = 1;
                        uiTabControlSelect(cw->botTabControl, gActiveChatFilter);
                        break;
                    }
                }
            }
            sendSelectedChatTabsToServer();
        }

    }
    else
    {
        // reset
        gPrevActiveChatFilter = 0;
        gActiveChatFilter = 0;
    }
}

ChatFilter * ChatFilterCreate(const char * name)
{
    ChatFilter * filter = calloc(sizeof(ChatFilter), 1);

    eaCreate(&filter->channels);
    eaCreate(&filter->pendingChannels);
    filter->name    = strdup(name);

    // add to global list
    eaPush(&gChatFilters, filter);
    return filter;
}


void ChatFilterDestroy(ChatFilter * filter)
{
    if(!filter)
         return;

    // remove from global list
    eaRemove(&gChatFilters, eaFind(&gChatFilters, filter));

    eaDestroy(&filter->channels);

    // chat queue
    eaDestroyEx(&filter->chatQ.ppLines, ChatLine_Destroy);
    eaDestroyEx(&filter->pendingChannels, 0);

    free(filter->name);

    if(gWaitingForChannelFilter == filter)
        gWaitingForChannelFilter = 0;
    
    if(gActiveChatFilter == filter)
        ChooseDefaultActiveFilter(0);
    if(gPrevActiveChatFilter == filter)
        gPrevActiveChatFilter = 0;

    // and finally...
    free(filter);
}


void ChatFilterAddChannel(ChatFilter * filter, ChatChannel * channel)
{
    // make sure it's not already a member
    assert(!ChatFilterHasChannel(filter, channel));
    assert(channel->name);

    // now, add channel...
    eaPush(&filter->channels, channel);
    eaPush(&channel->filters, filter);

    //notifyRebuildChannelWindow(filter);
}

void ChatFilterRemoveAllChannels(ChatFilter * filter, bool leaveUnassigned)
{    
    assert(filter);

    filter->defaultChannel.system    = 0;
    filter->defaultChannel.user        = 0;
    filter->defaultChannel.type        = ChannelType_None;    
    memset(filter->systemChannels, 0, SYSTEM_CHANNEL_BITFIELD_SIZE*sizeof(int));

    if(leaveUnassigned)
    {
        // leave any chat channels in this filter, unless they are shared with another chat filter                
        int i, size=eaSize(&filter->channels);
        for(i=0;i<size;i++)
        {
            if(eaSize(&filter->channels[i]->filters) == 1)
            {
                leaveChatChannel(filter->channels[i]->name);
            }
        }
    }

    while(eaSize(&filter->channels))
    {
        ChatFilterRemoveChannel(filter, filter->channels[0]);
    }
}

void ChatFilterRemoveChannel(ChatFilter * filter, ChatChannel * channel)
{
    // make sure it actually belongs to this window
     assert(ChatFilterHasChannel(filter, channel));

    //notifyRebuildChannelWindow(filter);

    if(      filter->defaultChannel.type == ChannelType_User
        && filter->defaultChannel.user == channel)
    {
        filter->defaultChannel.type = ChannelType_None;
        filter->defaultChannel.user = 0;
    }
    
    // now, remove channel
    eaRemove(&channel->filters, eaFind(&channel->filters, filter));
    eaRemove(&filter->channels, eaFind(&filter->channels, channel));

    assert(!ChatFilterHasChannel(filter, channel));


}

bool ChatFilterHasChannel(ChatFilter * filter, ChatChannel * channel)
{
    bool res;

    if(!filter || !channel)
        return 0;

    res = (-1 != eaFind(&filter->channels, channel));

    // extra debug consistency checks
    if(res)
    {
        assert(0 <= eaFind(&filter->channels, channel));
        assert(0 <= eaFind(&channel->filters, filter));    

    }
    else
    {
        assert(-1 == eaFind(&filter->channels, channel));
        assert(-1 == eaFind(&channel->filters, filter));
    }

    return res;

}

void ChatFilterRename(ChatFilter * filter, const char * name)
{
    assert(name);
    assert(strlen(name) <= MAX_TAB_NAME_LEN);

    // only update if changed
    if(stricmp(filter->name, name))
    {
        assert(!GetChatFilter(name));    // make sure this name is free

        free(filter->name);
        filter->name = strdup(name);

        ChatWindowUpdateFilter(filter);    // update parent chat window (if we belong to it)
    }
}
void ChatFilterClear(ChatFilter * filter)
{
      if(filter)
    {
           eaClearEx(&filter->chatQ.ppLines, ChatLine_Destroy);
        filter->chatQ.size = 0;
        filter->reformatText = TRUE;
    }
}



// debug only, so it can be slow...
ChatFilter * GetChatFilter(const char * filterName)
{
    int i, size = eaSize(&gChatFilters);

    // loop through all chat windows & look for a name match
    for(i=0; i<size;i++)
    {
        ChatFilter * filter = gChatFilters[i];
         if(filter && !strnicmp(filter->name, filterName, MAX_TAB_NAME_LEN))
            return filter;
    }

    return 0;
}

// used by uiTabControl to update text when tab is selected
void onFilterSelected(ChatFilter * filter)
{
    filter->reformatText = true;
    SelectActiveChatFilter(filter);
}

void onFilterMoved(ChatFilter * mover, ChatFilter * movee)
{
    sendChatSettingsToServer();
}


void chooseFilterFontColor(ChatFilter * filter, bool selected)
{
       if(selected)
         font_color(CLR_WHITE, CLR_WHITE);
     else if(filter->hasNewMsg)
        font_color(0xddddddff, 0xddddddff);    // slightly darker "white"
    else
        font_color(0x777777ff, 0x777777ff);  // slighly darker than normal inactive tab text
}

//
// PENDING CHANNELS    - those that belong to filter but we haven't received a "join" command yet
//
void ChatFilterPrepareAllForUpdate(int full_update)
{
    int i,k,size;
    for(k=0;k<eaSize(&gChatFilters);k++)
    {
        ChatFilter * filter = gChatFilters[k];
        
        // save channel names
        size = eaSize(&filter->channels);
        for(i=0;i<size;i++)
        {
            bool isDefault = false;

            if(    filter->defaultChannel.type == ChannelType_User    && ! stricmp(filter->channels[i]->name, filter->defaultChannel.user->name))
            {
                isDefault = true;
            }

            ChatFilterAddPendingChannel(filter, filter->channels[i]->name, isDefault);
        }

        // empty out all current channels 
        while(eaSize(&filter->channels))
        {
            ChatFilterRemoveChannel(filter, filter->channels[0]);
        }

    //    filter->updatedOnce = true;
    }

    // destroy all chat channels -- we'll rebuild them as we get chat server messages
    if(full_update)
    {
        eaClearEx(&gChatChannels, ChatChannelDestroy);
        eaClearEx(&gReservedChatChannels, ChatChannelDestroy);
    }
}


void ChatFilterAddPendingChannel(ChatFilter * filter, char * channelName, bool isDefault)
{
    ChatChannel * channel = GetChannel(channelName);

    if (!filter)
        return;

    if(!channel || !ChatFilterHasChannel(filter, channel))
    {
        char * p;
        int i;

        // is it in the pending channel list already?
        for(i=0;i<eaSize(&filter->pendingChannels);i++)
        {
            if( ! stricmp(channelName, filter->pendingChannels[i]))
            {
                return;
            }
        }

        // add it... 
        p = strdup(channelName);

        eaPush(&filter->pendingChannels, p);
        if(isDefault)
            filter->pendingDefaultChannel = p;
    }
}

// should only send one or the other (either channel to add OR name to remove)
void ChatFilterAssignPendingChannel(ChatChannel * addChannel, char * removeName)
{
    int i,k,r;
    char * name = (addChannel ? addChannel->name : removeName);
    int size = eaSize(&gChatFilters);
    bool save = false;

    assert(name);
    for(i=0;i<size;i++)
    {
        ChatFilter * filter = gChatFilters[i];
        for(k=0;k<eaSize(&filter->pendingChannels);k++)
        {
            if(!stricmp(filter->pendingChannels[k], name))
            {
                if(addChannel && !ChatFilterHasChannel(filter, addChannel))
                {
                    save = true;
                    ChatFilterAddChannel(filter, addChannel);

                    if(   (   filter->pendingDefaultChannel && ! stricmp(filter->pendingDefaultChannel, filter->pendingChannels[k]))
                       || ( ! filter->pendingDefaultChannel && filter->defaultChannel.type == ChannelType_None)) 
                    {
                        filter->pendingDefaultChannel = 0;
                        filter->defaultChannel.type = ChannelType_User;
                        filter->defaultChannel.user = addChannel;
                    }
                }
                free(filter->pendingChannels[k]);
                filter->pendingChannels[k] = 0;
            }
        }

        // prune
        while(-1 != (r = eaFind(&filter->pendingChannels, 0)))
        {
            eaRemove(&filter->pendingChannels, r);
        }
    }

    if(save)
        sendChatSettingsToServer();
}


/**********************************************************************func*
* ChatWindow
*
*/




ChatWindow gChatWindows[MAX_CHAT_WINDOWS];
int gActiveChatWindowIdx = 0;

// returns array index, but adds bounds checking
ChatWindow * GetChatWindow(int idx)
{
    if(idx >= 0 && idx < MAX_CHAT_WINDOWS)
        return &gChatWindows[idx];
    else 
        return NULL;
}


// necessary function because chat Windef enums are not consecutive
int ChatWindowIdxToWindefIdx(int chatWindowIdx)
{
    if(chatWindowIdx == 0)
        return WDW_CHAT_BOX;
    else
        return WDW_CHAT_CHILD_1 + chatWindowIdx - 1;
}

int WindefToChatWindowIdx(int windefIdx)
{
    if(windefIdx == WDW_CHAT_BOX)
        return 0;
    else
        return windefIdx - WDW_CHAT_CHILD_1 + 1;
}

ChatWindow * GetChatWindowFromWindefIndex(int idx)
{
    return GetChatWindow(WindefToChatWindowIdx(idx));
}

void ChatWindowAddFilter(ChatWindow * cw, ChatFilter * filter, int bottom)
{
    int color = 0, activeColor = 0;

    assert(filter);

    if (!stricmp(filter->name, "Help") || !stricmp(filter->name, "DefaultHelpText"))
    {
        color = 0xff8800ff;
        // don't override activeColor
    }

    // now, add channel...
    if( bottom )
        uiTabControlAddColorOverride(cw->botTabControl, filter->name, filter, color, activeColor);
    else
        uiTabControlAddColorOverride(cw->topTabControl, filter->name, filter, color, activeColor);
}

void ChatWindowRemoveFilter(ChatWindow * cw, ChatFilter * filter)
{
    int pane;
    assert(cw && filter);

    // make sure it actually belongs to this window
    pane = ChatWindowHasFilter(cw, filter);

    // now, remove filter
    if( pane == 1)
        uiTabControlRemove(cw->topTabControl, filter);    
    else if( pane == 2 )
        uiTabControlRemove(cw->botTabControl, filter);
}


void ChatWindowRemoveAllFilters(ChatWindow * cw)
{
    uiTabControlRemoveAll(cw->topTabControl);
    uiTabControlRemoveAll(cw->botTabControl);
}




ChatWindow * GetFilterParentWindow(ChatFilter * filter)
{
    int i;
    
    assert(filter);

    for(i=0;i<MAX_CHAT_WINDOWS;i++)
    {
        ChatWindow * window = GetChatWindow(i);
        if(ChatWindowHasFilter(window, filter))
        {
            return window;
        }
    }

    return 0;
}


void ChatWindowUpdateFilter(ChatFilter * filter)
{
    // currently, we just need to update the tab text
    ChatWindow * window = GetFilterParentWindow(filter);
    
    if(window)
    {
        if(ChatWindowHasFilter(window,filter)==1)
            uiTabControlRename(window->topTabControl, filter->name, filter);
        else if(ChatWindowHasFilter(window,filter)==2)
            uiTabControlRename(window->botTabControl, filter->name, filter);
    }
}

int ChatWindowHasFilter(ChatWindow * cw, ChatFilter * filter)
{
    if(filter)
    {
        if( uiTabControlHasData(cw->topTabControl, filter) )
            return 1;
        if( uiTabControlHasData(cw->botTabControl, filter) )
            return 2;

        return 0;
    }
    else 
        return 0;
}

int AnyChatWindowHasFilter(ChatFilter * filter)
{
    int i;

    for(i=0;i<MAX_CHAT_WINDOWS;i++)
    {
        if(ChatWindowHasFilter(GetChatWindow(i), filter))
            return 1;
    }

    return 0;
}

void ChooseDefaultActiveFilter(ChatFilter * noVisibleChoice)
{
    int i;
    ChatFilter * filter = 0;

    // first pass, choose one that is SELECTED & VISIBLE
    for(i=0;i<MAX_CHAT_WINDOWS;i++)
    {
        ChatWindow * window = GetChatWindow(i);
        if(      window_getMode(window->windefIdx) == WINDOW_DISPLAYING
            && ! window->minimizedOffset)
        {
            if( window->contextPane )
                filter = uiTabControlGetSelectedData(window->botTabControl);
            else
                filter = uiTabControlGetSelectedData(window->topTabControl);

            if(filter)
            {
                SelectActiveChatFilter(filter);
                return;
            }
        }
    }

    // no other visible window is available. Use specified window if given.
    if(noVisibleChoice)
    {
        SelectActiveChatFilter(noVisibleChoice);
        return;
    }

    // else, one final attempt to find active window ---> choose even minimized tabs
    for(i=0;i<MAX_CHAT_WINDOWS;i++)
    {
        filter = uiTabControlGetSelectedData(GetChatWindow(i)->topTabControl);
        if(filter)
        {
            SelectActiveChatFilter(filter);
            return;
        }
        filter = uiTabControlGetSelectedData(GetChatWindow(i)->botTabControl);
        if(filter)
        {
            SelectActiveChatFilter(filter);
            return;
        }
    }

    // no filters available!
    SelectActiveChatFilter(0);
}




// Slash Commands

void addChannelSlashCmd(char * cmd, char * channelName, char * option, ChatFilter * filter)
{
    ChatChannel * cc = GetChannel(channelName);
    if(!cc)
        cc = GetReservedChannel(channelName);

    if(!filter)
        filter = gActiveChatFilter;
    
    if(cc && !cc->isReserved)
    {
        addSystemChatMsg( textStd("AlreadyBelongToChatChannel", channelName), INFO_USER_ERROR, 0 );
    }
    else
    {
        if(strlen(channelName) > MAX_CHANNELNAME)
        {
            addSystemChatMsg( textStd("ChatChannelNameTooLong", MAX_CHANNELNAME), INFO_USER_ERROR, 0 );
        }
        else
        {
            sendShardCmd(cmd, "%s %s", channelName, (option ? option : "0"));
            if(filter)
                ChatFilterAddPendingChannel(filter, channelName, 0);
        }
    }
}


void chatFilterOpen(const char * filterName, int windowIdx, int pane)
{
    ChatWindow * window;
    ChatFilter * filter;

    if(GetChatFilter(filterName))
    {
        addSystemChatMsg( textStd("ChatTabAlreadyExistsError", filterName), INFO_USER_ERROR, 0 );    
        return;
    }

    if(!chatOptionsAvailable())
    {
        addSystemChatMsg( textStd("OpenTabOptionsBusyError"), INFO_USER_ERROR, 0 );    
        return;
    }

    if(windowIdx < 0 || windowIdx >= MAX_CHAT_WINDOWS)
        windowIdx = 0;

    window = GetChatWindow(windowIdx);
    filter = ChatFilterCreate(filterName);
    ChatWindowAddFilter(window, filter, pane);

    assert(GetChatFilter(filterName));
}

void chatFilterClose(const char * filterName)
{
    ChatFilter * filter = GetChatFilter(filterName);
    ChatWindow * window;

    if(!filter)
    {
        addSystemChatMsg( textStd("TabDoesNotExistError", filterName), INFO_USER_ERROR, 0 );
        return;
    }

    if(filter == currentChatOptionsFilter())
    {
        addSystemChatMsg( textStd("CloseTabBusyError"), INFO_USER_ERROR, 0 );
        return;
    }

    window = GetFilterParentWindow(filter);
    if(window)
        ChatWindowRemoveFilter(window, filter);

    ChatFilterDestroy(filter);
}

void chatFilterToggle()
{
    if(gPrevActiveChatFilter)
    {
        SelectActiveChatFilter(gPrevActiveChatFilter);
    }
}

// open a chat window (if it's closed). 
void OpenChatWindow(ChatWindow * window)
{
    if( ! windowUp(window->windefIdx))
    {
        window_setMode(window->windefIdx, WINDOW_GROWING);
    }
}


void chatFilterSelect(char * filterName)
{
    ChatFilter * filter = GetChatFilter(filterName);
    if(filter)
    {
        OpenChatWindow(GetFilterParentWindow(filter));
        SelectActiveChatFilter(filter);
    }
    else
    {
        addSystemChatMsg( textStd("ChatTabDoesNotExist", filterName), INFO_USER_ERROR, 0 );
    }
}

void chatFilterCycle(int windowIdx, bool backward)
{
    ChatWindow * window = GetChatWindow(windowIdx);
    uiTabControl * tc;
    int paneSwitch;

    if(!window)
    {
        addSystemChatMsg( textStd("InvalidChatWindowIndex"), INFO_USER_ERROR, 0 );
        return;
    }

    if(window->contextPane)
        tc = window->botTabControl;
    else
        tc = window->topTabControl;

    if(backward)
        paneSwitch = uiTabControlSelectPrev(tc);
    else
        paneSwitch = uiTabControlSelectNext(tc);

    window->contextPane = !window->contextPane;
    if(window->contextPane)
        tc = window->botTabControl;
    else
        tc = window->topTabControl;

    SelectActiveChatFilter(uiTabControlGetSelectedData(tc));
}

void chatFilterGlobalCycle(bool backward)
{
    ChatWindow * window;

    if(!eaSize(&gChatFilters))
    {
        addSystemChatMsg( textStd("NoChatFilters"), INFO_USER_ERROR, 0 );
        return;
    }
    
    if(gActiveChatFilter)
    {
        ChatFilter *    filter;
        uiTabControl    *tc;
        int                idx;

        window    = GetFilterParentWindow(gActiveChatFilter);

        if( window->contextPane )
            tc = window->botTabControl;
        else
            tc = window->topTabControl;

        filter = uiTabControlGetSelectedData(tc);

        idx = window->idx;

        if(backward)
        {
            if(filter == uiTabControlGetFirst(tc))
            {
                do {

                    idx--;
                    if(idx < 0)
                        idx = (MAX_CHAT_WINDOWS - 1);
                    window = GetChatWindow(idx);

                } while( ! uiTabControlGetFirst(tc));    // must eventually stop since we have at least one tab (somewhere)

                uiTabControlSelect(tc, uiTabControlGetLast(tc));    
            }
            else
                uiTabControlSelectPrev(tc);
        }
        else
        {
            // forward
            if(filter == uiTabControlGetLast(tc))
            {
                do {

                    idx++;
                    if(idx >= MAX_CHAT_WINDOWS)
                        idx = 0;
                    window = GetChatWindow(idx);
    
                } while( ! uiTabControlGetFirst(tc));    // must eventually stop since we have at least one tab (somewhere)
                
                uiTabControlSelect(tc, uiTabControlGetFirst(tc));    
            }
            else
                uiTabControlSelectNext(tc);
        }

        SelectActiveChatFilter(uiTabControlGetSelectedData(tc));
    }
    else
    {
        SelectActiveChatFilter(gChatFilters[0]);    // default
        window = GetFilterParentWindow(gChatFilters[0]);
    }

    OpenChatWindow(window); // make sure that the window is open
}

// -----------------------------------------------------------------------------

void deleteFilterCM(ChatFilter * filter)
{
    ChatWindow *cw = GetFilterParentWindow(filter);
    if(filter)
    {
        if( currentChatOptionsFilter() == filter )
            chatTabClose(0);

        ChatFilterRemoveAllChannels(filter, false);
        ChatWindowRemoveFilter(cw, filter);
        ChatFilterDestroy(filter);
        sendChatSettingsToServer();
    }
}

void editFilterCM(ChatFilter * filter)
{
    if(filter)
    {
        chatTabOpen(filter, false);
    }
}

void clearFilterCM(ChatFilter * filter )
{
    if(filter)
    {
        ChatFilterClear(filter);
    }
}

int canMoveBottomCM( ChatFilter * filter)
{
    ChatWindow *cw = GetFilterParentWindow(filter);

    if(!cw)
        return CM_HIDE;

    if( ChatWindowHasFilter(cw,filter) == 2 )
        return CM_HIDE; // the tab is already on the bottom, can't go there

    if(uiTabControlNumTabs(cw->topTabControl) == 1)
        return CM_VISIBLE;    // tab is on the top, but there is only one, 
                            //so moving to bottom would collapse everything to top tab resulting in no net change
    return CM_AVAILABLE;
}

int canMoveTopCM( ChatFilter * filter)
{
    ChatWindow *cw = GetFilterParentWindow(filter);

    if(!cw)
        return CM_HIDE;

    if( ChatWindowHasFilter(cw,filter) == 1 )
        return CM_HIDE; // the tab is already on the top, can't go there

    return CM_AVAILABLE;
}

void switchPaneCM( ChatFilter * filter)
{
    ChatWindow *cw = GetFilterParentWindow(filter);

    if(!cw)
        return;

    if( ChatWindowHasFilter(cw,filter) == 1 )
    {
        if(uiTabControlMoveTabPublic(cw->botTabControl,cw->topTabControl,filter))
        {
            cw->divider = .5f;
            sendChatSettingsToServer();
        }
    }
    else if( ChatWindowHasFilter(cw,filter) == 2 )
        uiTabControlMoveTabPublic(cw->topTabControl,cw->botTabControl,filter);
}

void ChatWindowInit(int windowIdx)
{
    ChatWindow * window = GetChatWindow(windowIdx);

    memset(window, 0, sizeof(ChatWindow));

    window->idx = windowIdx;

    window->windefIdx = ChatWindowIdxToWindefIdx(windowIdx); 

    window->topsb.wdw = window->windefIdx;
    window->botsb.wdw = window->windefIdx;

    window->last_mode = WINDOW_UNINITIALIZED;

    if( !tabContextMenu )
    {
        tabContextMenu = contextMenu_Create(NULL);
        contextMenu_addCode(tabContextMenu, alwaysAvailable,        0,        editFilterCM,            0, textStd("CMEditChatTab"),        0);
        contextMenu_addCode(tabContextMenu, alwaysAvailable,        0,        deleteFilterCM,            0, textStd("CMDeleteChatTab"),        0);
        contextMenu_addCode(tabContextMenu, alwaysAvailable,        0,        clearFilterCM,            0, textStd("CMClearChatTab"),        0);
        contextMenu_addCode(tabContextMenu, canMoveBottomCM,        0,        switchPaneCM,            0, textStd("CMMoveBottomChatTab"),    0);
        contextMenu_addCode(tabContextMenu, canMoveTopCM,            0,        switchPaneCM,            0, textStd("CMMoveTopChatTab"),        0);
    }

    window->topTabControl = uiTabControlCreate(TabType_ChatTab, (uiTabActionFunc) onFilterSelected, (uiTabActionFunc2) onFilterMoved, 0, (uiTabFontColorFunc) chooseFilterFontColor, tabContextMenu);
    window->botTabControl = uiTabControlCreate(TabType_ChatTab, (uiTabActionFunc) onFilterSelected, (uiTabActionFunc2) onFilterMoved, 0, (uiTabFontColorFunc) chooseFilterFontColor, tabContextMenu);

    uiTabControlSetParentWindow(window->topTabControl, window->windefIdx);
    uiTabControlSetParentWindow(window->botTabControl, window->windefIdx);
}




// -----------------------------------------------------------------------
// Chat Handle Dialogs (used to display & change)
// -----------------------------------------------------------------------
#include "gameComm/chatClient.h"

static void changeChatHandleDlgHandler(void * data)
{
    char * handle = dialogGetTextEntry();
    if(handle && strlen(handle))
    {
        char buf[1000];
        sprintf(buf, "changehandle %s", handle);
        cmdParse(buf);
    }
}

static void changeChatHandleDialog(void * data)
{
    dialogNameEdit(textStd("ChangeChatHandleDialog"), "OkString", "CancelString", changeChatHandleDlgHandler, 0, MAX_PLAYERNAME, 0);
}

void displayChatHandleDialog(void * foo)
{
    if(canChangeChatHandle())
        dialogStd(DIALOG_TWO_RESPONSE, textStd("DisplayChatHandleDialog", game_state.chatHandle), "OkString", "ChangeChatHandle", NULL, changeChatHandleDialog, DLGFLAG_GAME_ONLY);
    else
        dialogStd(DIALOG_OK, textStd("DisplayChatHandleDialog", game_state.chatHandle), NULL, NULL, NULL, NULL, DLGFLAG_GAME_ONLY);

}

void changeChatHandleAgainDialog(char * badHandle)
{
    dialogNameEdit(textStd("ChangeChatHandleAgainDialog", badHandle) , NULL, NULL, changeChatHandleDlgHandler, NULL, MAX_PLAYERNAME, 0 );
}

void changeChatHandleSucceeded()
{
    dialogStd(DIALOG_OK, textStd("ChangeHandleSucceededDialog", game_state.chatHandle), NULL, NULL, NULL, NULL, DLGFLAG_GAME_ONLY);
}


//---------------------------------------------------------------------------------------------------------------
// CW stuff added
//---------------------------------------------------------------------------------------------------------------

void chatSendAllTabsToTop(int winid)
{
    ChatWindow *chatWindow = GetChatWindowFromWindefIndex(winid);
    uiTabControlMergeControls(chatWindow->topTabControl, chatWindow->botTabControl);
}
