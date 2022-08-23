#include "uiChatOptions.h"
#include <utilitieslib/utils/file.h>
#include "UI/uiScrollBar.h"
#include "UI/uiWindows.h"
#include "UI/uiChatUtil.h"
#include "gameComm/wdwbase.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtil.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/ttFontUtil.h"
#include "assert.h"
#include "UIEdit.h"
#include "UI/uiFocus.h"
#include "cmdparse/cmdcommon.h"
#include "UI/uiInput.h"
#include <utilitieslib/utils/mathutil.h>
#include "filter/validate_name.h"
#include <utilitieslib/components/EString.h>
#include "UI/uiChat.h"
#include <utilitieslib/language/MessageStore.h>
#include "language/langClientUtil.h"
#include <utilitieslib/components/earray.h>
#include "win/input.h"
#include "limits.h"
#include "entity/entPlayer.h"
#include "UI/sprite/sprite_base.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiDialog.h"
#include "cmdparse/cmdgame.h"
#include "gameComm/chatClient.h"
#include "chatdb.h"
#include "graphics/font.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/components/bitfield.h>

  
#define NAME_HEADER        10
#define NAME_WIDTH        175
#define    NAME_HEIGHT        24

#define BUTTON_SPACE    40
#define    BUTTON_WIDTH    100
#define BUTTON_HEIGHT    30

#define LIST_SPACE        (BUTTON_WIDTH + 14)
#define LIST_HEADER        (57 + 20)

typedef struct AvailableChannel AvailableChannel;

typedef struct ChatTabData
{
    ChatFilter *            filter;

    UIEdit*                    nameEdit;

    int selected;
    AvailableChannel *        defaultChannel;

    bool                    init;
    ChatWindow *            newWindow;        // If a new filter, set to != NULL, and the filter will be assigned to window if OK, or Deleted on "Cancel"

    ScrollBar                selectedSB;        // for list of selected channels
    ScrollBar                availableSB;        // for list of available channels

    AvailableChannel **        available;
    ChatChannel **            leaveQueue;

}ChatTabData;

static ChatTabData gTabData;

bool canAddChannel()
{
    int count =   eaSize(&gChatChannels) 
                + eaSize(&gReservedChatChannels)
                - eaSize(&gTabData.leaveQueue);

    return (count < MAX_WATCHING);
}


typedef struct SystemChannel
{    
    char * name;
    int colorIdx;
    BOOL canSetDefault;
    int channels[8];

} SystemChannel;

SystemChannel gSystemChannels[] = 
{
    { "DamageFilter",        INFO_DAMAGE,            FALSE,    {INFO_DAMAGE,}  },
    { "CombatFilter",        INFO_COMBAT,            FALSE,    {INFO_COMBAT,} },
    { "CombatSpamFilter",    INFO_COMBAT_SPAM,        FALSE,    {INFO_COMBAT_SPAM,} },
    { "CombatErrorFilter",    INFO_COMBAT_ERROR,        FALSE,    {INFO_COMBAT_ERROR,} },
    { "CombatHitFilter",    INFO_COMBAT_HITROLLS,    FALSE,    {INFO_COMBAT_HITROLLS,} },
    { "HealFilter",            INFO_HEAL,                FALSE,    {INFO_HEAL,} },
    { "HealOtherFilter",    INFO_HEAL_OTHER,        FALSE,    {INFO_HEAL_OTHER,} },
    { "SystemFilter",        INFO_SVR_COM,            FALSE,    {INFO_SVR_COM, INFO_DEBUG_INFO,} },
    { "ErrorFilter",        INFO_USER_ERROR,        FALSE,    {INFO_USER_ERROR,} },
    { "RewardFilter",        INFO_REWARD,            FALSE,    {INFO_REWARD,} },
    { "AuctionFilter",        INFO_AUCTION,            FALSE,    {INFO_AUCTION,} },
    { "ArchitectFilter",    INFO_ARCHITECT,            FALSE,    {INFO_ARCHITECT,} },
    { "NPCFilter",            INFO_NPC_SAYS,            FALSE,    {INFO_NPC_SAYS, INFO_VILLAIN_SAYS,} },
    { "PrivateFilter",        INFO_PRIVATE_COM,        FALSE,    {INFO_PRIVATE_COM, INFO_PRIVATE_NOREPLY_COM,} },
    { "LFGFilter",            INFO_LOOKING_FOR_GROUP,    TRUE,    {INFO_LOOKING_FOR_GROUP,} },
    { "TeamFilter",            INFO_TEAM_COM,            TRUE,    {INFO_TEAM_COM,} },
    { "LevelingPactFilter",    INFO_LEVELINGPACT_COM,    TRUE,    {INFO_LEVELINGPACT_COM,} },
    { "SuperGroupFilter",    INFO_SUPERGROUP_COM,    TRUE,    {INFO_SUPERGROUP_COM,} },
    { "LocalFilter",        INFO_NEARBY_COM,        TRUE,    {INFO_NEARBY_COM,} },
    { "BroadcastFilter",    INFO_SHOUT_COM,            TRUE,    {INFO_SHOUT_COM,} },
    { "RequestFilter",        INFO_REQUEST_COM,        TRUE,    {INFO_REQUEST_COM,} },
    { "FriendFilter",        INFO_FRIEND_COM,        TRUE,    {INFO_FRIEND_COM,} },
    { "EmoteFilter",        INFO_EMOTE,                FALSE,    {INFO_EMOTE,} },
    { "AllianceFilter",        INFO_ALLIANCE_OWN_COM,    TRUE,    {INFO_ALLIANCE_OWN_COM, INFO_ALLIANCE_ALLY_COM,} },
    { "ArenaFilter",        INFO_ARENA_GLOBAL,        TRUE,    {INFO_ARENA_GLOBAL,} },
    { "PetFilter",            INFO_PET_COM,            FALSE,  {INFO_PET_COM,} },
    { "HelpFilter",            INFO_HELP,                TRUE,   {INFO_HELP,} },
    { "EventFilter",        INFO_EVENT,                FALSE,  {INFO_EVENT,} },
    { "PetDamageFilter",    INFO_PET_DAMAGE,            FALSE,    {INFO_PET_DAMAGE,}  },
    { "PetCombatFilter",    INFO_PET_COMBAT,            FALSE,    {INFO_PET_COMBAT,} },
    { "PetCombatSpamFilter",INFO_PET_COMBAT_SPAM,        FALSE,    {INFO_PET_COMBAT_SPAM,} },
    { "PetCombatHitFilter",    INFO_PET_COMBAT_HITROLLS,    FALSE,    {INFO_PET_COMBAT_HITROLLS,} },
    { "PetHealFilter",        INFO_PET_HEAL,                FALSE,    {INFO_PET_HEAL,} },
    { "PetHealOtherFilter",    INFO_PET_HEAL_OTHER,        FALSE,    {INFO_PET_HEAL_OTHER,} },
    { "HeroEventFilter",    INFO_EVENT_HERO,            FALSE,  {INFO_EVENT_HERO,} },
    { "VillainEventFilter",    INFO_EVENT_VILLAIN,            FALSE,  {INFO_EVENT_VILLAIN,} },
    { "PraetorianEventFilter",    INFO_EVENT_PRAETORIAN,    FALSE,  {INFO_EVENT_PRAETORIAN,} },
    { "CaptionEventFilter",    INFO_CAPTION,                FALSE,  {INFO_CAPTION,} },
    { "ArchitectGlobalFilter", INFO_ARCHITECT_GLOBAL,    TRUE, {INFO_ARCHITECT_GLOBAL,} },
    { "LeagueFilter",        INFO_LEAGUE_COM,        TRUE,    {INFO_LEAGUE_COM,} },
    { 0 },
};



typedef struct AvailableChannel
{

    bool                     highlighted;    
    bool                     selected;
    bool                     defaultChannel;
    ChannelType    type;

    SystemChannel *            system;        // for system channels
    ChatChannel *            user;    // for user channels

} AvailableChannel;

AvailableChannel * AvailableChannelCreate(ChannelType type, ChatChannel * channel, int idx)
{
    AvailableChannel * ac = calloc(1, sizeof(AvailableChannel));

    ac->type = type;

    if(type == ChannelType_System)
    {
        assert(idx >=0 && idx < (ARRAY_SIZE(gSystemChannels)-1));
        ac->system = &gSystemChannels[idx];
    }
    else if(type == ChannelType_User)
    {
        assert(channel);
        ac->user = channel;
    }
    else
        assert(!"Bad type");

    return ac;
}




void AvailableChannelDestroy(AvailableChannel * ac)
{
    free(ac);
}

int sortAvailableChannelFunc(const AvailableChannel** pp1, const AvailableChannel** pp2)
{    
       const AvailableChannel * ac1 = *pp1;
      const AvailableChannel * ac2 = *pp2;

    assert(ac1 != ac2); // should never have duplicates!

    if(ac1->type == ac2->type)
    {
        char * name1 = (ac1->type == ChannelType_User) ? ac1->user->name : ac1->system->name;
        char * name2 = (ac2->type == ChannelType_User) ? ac2->user->name : ac2->system->name;
        return stricmp(name1, name2);
    }
    else
    {
        if(ac1->type == ChannelType_User)
            return -1;
        else
             return 1;
    }
}


// return TRUE if chat options window is not being used
bool chatOptionsAvailable()
{
    if( ! gTabData.filter)
    {
//        assert(window_getMode(WDW_CHAT_OPTIONS) != WINDOW_DISPLAYING);
        return true;
    }
    else
    {
    //    int mode = window_getMode(WDW_CHAT_OPTIONS);
    //    assert(mode == WINDOW_GROWING || mode == WINDOW_DISPLAYING);
        return false;
    }
}

ChatFilter * currentChatOptionsFilter()
{
    return gTabData.filter;
}


// passing NULL will reset all default channel settings
void setDefaultChannel(AvailableChannel * dc)
{
    // reset all default statuses
    int i;
    for(i=0;i<eaSize(&gTabData.available);i++)
    {
        gTabData.available[i]->defaultChannel = false;
    }

    gTabData.defaultChannel = 0;

    // set new default channel (if necessary)
    if(dc)
    {
        dc->defaultChannel = true;
        gTabData.defaultChannel = dc;
    }
}

// choose the first available default channel in the "Current Channel" list
void chooseDefaultChannel()
{
    if(gTabData.available)
    {
        int i;
        for(i=0;i<eaSize(&gTabData.available);i++)
        {
            AvailableChannel * ac = gTabData.available[i];
            if(    ac->selected
                && (   ac->type == ChannelType_User
                    || (ac->type == ChannelType_System && ac->system->canSetDefault)))
            {
                setDefaultChannel(ac);
                return;
            }
        }
    }
}

// remove chat channel from list (it's about to be deleted)
void channelDeleteNofication(ChatChannel * channel)
{
    int i;

    if(gTabData.available)
    {
        for(i=0;i<eaSize(&gTabData.available);i++)
        {
            AvailableChannel * ac = gTabData.available[i];
            if(   ac->type == ChannelType_User
               && ac->user == channel)
            {    
                 if(gTabData.defaultChannel == ac)
                    setDefaultChannel(0);

                AvailableChannelDestroy(ac);
                eaRemove(&gTabData.available, i);

                return;
            }
        }
    }

    if(gTabData.leaveQueue)
    {
        for(i=0;i<eaSize(&gTabData.leaveQueue);i++)
        {
            eaFindAndRemove(&gTabData.leaveQueue, channel);
        }
    }
}


AvailableChannel * AddAvailableUserChannel(ChatChannel * channel)
{
    AvailableChannel * ac = AvailableChannelCreate(ChannelType_User, channel, 0);
    eaPush(&gTabData.available, ac);

    // selected & default values
     if(ChatFilterHasChannel(gTabData.filter, channel))
    {
        ac->selected = true;

        if(   gTabData.filter->defaultChannel.type == ChannelType_User
            && gTabData.filter->defaultChannel.user == channel)
        {
            setDefaultChannel(ac);
        }
    }

    return ac;
}



void AddAvailableSystemChannel(int systemIdx)
{
    AvailableChannel * ac = AvailableChannelCreate(ChannelType_System, 0, systemIdx);
    int i;
    
    eaPush(&gTabData.available, ac);
    for( i = 0; i < 8; i++ )
    {
        if( BitFieldGet(gTabData.filter->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, gSystemChannels[systemIdx].channels[i]) )
        {
            ac->selected = true;

            if(    gTabData.filter->defaultChannel.type == ChannelType_System
                && gTabData.filter->defaultChannel.system == gSystemChannels[systemIdx].colorIdx
                && gSystemChannels[systemIdx].canSetDefault)
            {
                setDefaultChannel(ac);
            }
        }
        return;
    }
}

// return true if a single channel is highlighted/nothighlighted (depending on param) and it can be a DEFAULT channel
AvailableChannel * selectableDefaultChannel(bool highlighted)
{
    int i;
    //    int count = 0;
    AvailableChannel * validChannel = 0;

     for(i=0;i<eaSize(&gTabData.available);i++)
    {
        AvailableChannel * ac = gTabData.available[i];
        if(    ac->selected
            && ac->highlighted == highlighted)
        {
            // is it eligible to be a default channel?
            if(    (ac->type == ChannelType_System && ac->system->canSetDefault)
                || (ac->type == ChannelType_User))
            {
                validChannel = ac;
                break;
            }
        }
    }

    if(validChannel && ! validChannel->defaultChannel)
        return validChannel;
    else
        return 0;
}


void UpdateUserChannels(ChatChannel *** list)
{
    int m, size = eaSize(list);
    for(m=0;m<size;m++)
    {
        int i;
        bool found = false;
        ChatChannel * channel = (*list)[m];
        if(!channel->isLeaving)
        {
            for(i=0;i<eaSize(&gTabData.available);i++)
            {
                AvailableChannel * ac = gTabData.available[i];
                if(   ac->type == ChannelType_User
                    && ac->user == channel)
                {
                    found = true;
                }
            }
            if(!found)
                AddAvailableUserChannel(channel);
        }
    }
}

static int channelCount()
{
    return (  eaSize(&gChatChannels)
            + eaSize(&gReservedChatChannels) 
            + ARRAY_SIZE(gSystemChannels) 
            - eaSize(&gTabData.leaveQueue) 
            - 1);
}

// iterate through master list of channels & add to available channel list if necessary
void createAvailableChannelList()
{
      if(!gTabData.available)
    {    
        // new filter -- full update

        int i=0;
        eaCreate(&gTabData.available);
        // add system channels if they don't already exist
        while(gSystemChannels[i].name)
        {
            AddAvailableSystemChannel(i);
            i++;
        }
    }

    // add any new user channels (only need to if sizes are not equal)
    if(eaSize(&gTabData.available) != channelCount())
    {
        UpdateUserChannels(&gChatChannels);
        UpdateUserChannels(&gReservedChatChannels);    // probably not needed here, but just in case...
    }

    eaQSort(gTabData.available, sortAvailableChannelFunc);

    // should match up!
    assert(eaSize(&gTabData.available) == channelCount());
}

void chatTabOpen(ChatFilter * filter, ChatWindow * newWindow)
{
     if(gTabData.available)
        chatTabClose(1);

     gTabData.filter = filter;

    window_setMode(WDW_CHAT_OPTIONS, WINDOW_GROWING);

    memset(&gTabData.selectedSB,  0, sizeof(ScrollBar));
    memset(&gTabData.availableSB, 0, sizeof(ScrollBar));

    winDefs[WDW_CHAT_OPTIONS].loc.locked = 0;

     if(!gTabData.init) 
    {
        int dbgCount = 0;

        // name edit box
        gTabData.nameEdit = uiEditCreate();
            uiEditSetFont(gTabData.nameEdit, &game_12);
        uiEditSetUTF8Text(gTabData.nameEdit, gTabData.filter->name);
        gTabData.nameEdit->textColor = CLR_WHITE;
        gTabData.nameEdit->limitType = UIECL_UTF8;
        gTabData.nameEdit->limitCount = MAX_TAB_NAME_LEN;

        gTabData.nameEdit->cursor.characterIndex = INT_MAX;    // move to end of box

        gTabData.init = TRUE;    

        if(newWindow)
            uiEditTakeFocus(gTabData.nameEdit);

        gTabData.newWindow = newWindow;
    }

    setDefaultChannel(0);
}


// Generate a default tab name -- "Tab 1", "Tab 2", etc.
// Caller must deallocate string
char * generateDefaultTabName(char * base)
{
    int i = 1;
    char buf[100];
    char name[100];

    assert(base);

    // prevent name from being too long
    strncpyt(buf, base, MAX_TAB_NAME_LEN-3); // room for 2 digits & a space

    do {
        sprintf(name, "%s %d", buf, i++); 
    } while(GetChatFilter(name) && (i<=MAX_CHAT_TABS));

    assert(!GetChatFilter(name));

    return strdup(name);
}

enum {
    kEditBoxName_Valid,
    kEditBoxName_Empty,
    kEditBoxName_Duplicate,
};

bool validEditBoxName()
{
    char *name = uiEditGetUTF8Text(gTabData.nameEdit);
    int valid = kEditBoxName_Valid;

     if(name)
    {
        if(   GetChatFilter(name)
           && stricmp(gTabData.filter->name, name))
        {
            valid = kEditBoxName_Duplicate;
        }
    }
    else
        valid = kEditBoxName_Empty;

    estrDestroy(&name);
    return valid;
}

void chatTabCleanup()
{
    if(gTabData.filter)
        chatTabClose(false);
}

void chatTabClose(bool save)
{
    ChatFilter * filter = gTabData.filter;
    int i;

      if(save)
    {    
        char *name = uiEditGetUTF8Text(gTabData.nameEdit);
        if(name)
            ChatFilterRename(gTabData.filter, name);
        estrDestroy(&name);

        // leave user channels
        for(i=0;i<eaSize(&gTabData.leaveQueue);i++)
        {
            leaveChatChannel(gTabData.leaveQueue[i]->name);
        }

        ChatFilterRemoveAllChannels(filter, false);
        for(i=0;i<eaSize(&gTabData.available);i++)
        {
            AvailableChannel * ac = gTabData.available[i];
            if(ac->selected)
            {
                if(ac->type == ChannelType_System)
                {
                    int i;
                    for(i=0; i < 8; i++)
                    {
                        if(ac->system->channels[i])
                            BitFieldSet(filter->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, ac->system->channels[i], 1);
                    }

                    if(ac->defaultChannel)
                    {
                         assert(filter->defaultChannel.type == ChannelType_None);
                        filter->defaultChannel.type = ChannelType_System;
                        filter->defaultChannel.system = ac->system->colorIdx;    
                    }
                }
                else if(ac->type == ChannelType_User)
                {
                    assert(ac->user);
                    ChatFilterAddChannel(filter, ac->user);

                    // officially join channel 
                    if(ac->user->isReserved)
                        addChannelSlashCmd("join", ac->user->name, 0, filter);
                
                    if(ac->defaultChannel)
                    {
                         assert(filter->defaultChannel.type == ChannelType_None);
                        filter->defaultChannel.type = ChannelType_User;
                        filter->defaultChannel.user = ac->user;
                    }
                }
                else
                    assert(!"Bad type");
            }
        }

        if(gTabData.newWindow)
            ChatWindowAddFilter(gTabData.newWindow, filter, gTabData.newWindow->contextPane);

        sendChatSettingsToServer();    
    }
    else
    {
        // get rid of all "pending" user channels
        for(i=0;i<eaSize(&gReservedChatChannels);i++)
        {
            ChatChannel * cc = gReservedChatChannels[i];
            if(cc->isReserved)
                leaveChatChannel(cc->name);
        }

        // undo leave channel queue
        for(i=0;i<eaSize(&gTabData.leaveQueue);i++)
        {
            gTabData.leaveQueue[i]->isLeaving = 0;
        }

        if(gTabData.newWindow && filter)
        {
            // delete tab entirely
            ChatFilterRemoveAllChannels(filter, true);
            ChatFilterDestroy(filter);
        }
    }

    
    // clean up window...
    uiEditReturnFocus(gTabData.nameEdit);
    if(gTabData.nameEdit)
    {
        uiEditDestroy(gTabData.nameEdit);
        gTabData.nameEdit = 0;
    }

    eaDestroyEx(&gTabData.available, AvailableChannelDestroy);
    eaDestroy(&gTabData.leaveQueue);

    gTabData.filter = 0;

    gTabData.init = FALSE;

    window_setMode(WDW_CHAT_OPTIONS, WINDOW_SHRINKING);
}



#define DEFAULT_CLR_BACK 0xF0F0F011
#define DEFAULT_CLR_BORD 0x8d8d8dff

void handleNameEditBox(CBox * box, float scale)
{
    int color        = DEFAULT_CLR_BORD;
    int colorBack    = DEFAULT_CLR_BACK;
    static float t = 0;
    CBox selBox = *box;

      t += TIMESTEP*.08;           // timer for the pulsing backgorund

    // actual selection frame is slightly wider than edit collision box
    selBox.lx -= R10*scale;
     selBox.hx += R10*scale;

     if( mouseCollision( &selBox ))
    {
        color       = CLR_SELECTION_FOREGROUND;
        colorBack   = CLR_SELECTION_BACKGROUND;
    }

    // check for a mouse click
    if( mouseDownHit( box, MS_LEFT ))
    {
        uiEditTakeFocus(gTabData.nameEdit);
         t = PI/2; // reset the timer so it flashes black when you click
    }


     colorBack = CLR_SELECTION_BACKGROUND; //(15 + 15*sin(t));

    drawFlatFrame( PIX3, R10, box->lx-R10*scale, box->ly-3*scale, (gTabData.nameEdit->z-1), box->hx - box->lx + 2*R10*scale, box->hy - box->ly + 6*scale, scale, color, colorBack );

     uiEditProcess(gTabData.nameEdit);

    // Get rid of multiple spaces in the name
    {
         char *text = uiEditGetUTF8Text(gTabData.nameEdit);
         if(text)
        {
            char space[2] = " ";
            char *pch;
            int len;

            if( !isDevelopmentMode())
            {
                stripNonRegionChars(text, getCurrentLocale());
            }
 
             len = strlen(text);
            // if the string is only whitespace, delete it
            if( strspn(text, space ) == len )
            {
                text[0] = '\0';
            }

            while((pch=strstr(text, "  "))!=NULL)
            {
                strcpy(pch, pch+1);
            }

            {
                int idx = gTabData.nameEdit->cursor.characterIndex;
                 uiEditSetUTF8Text(gTabData.nameEdit, text);
                uiEditSetCursorPos(gTabData.nameEdit, idx);
            }
            estrDestroy(&text);
         }
    }



       if(gTabData.nameEdit->contentCharacterCount == 0)
    {
        int color = ((CLR_WHITE & 0xFFFFFF00) | ((int)(120+120*sinf(t))));
         font( &profileFont );
         font_color( color, color );
           prnt( gTabData.nameEdit->boundsText.x, box->ly + 18*scale, gTabData.nameEdit->z, scale, scale, "EnterTabNameHere" );
    }
}

#define ENTRY_SPACE (R10*2)

bool isChannelHighlighted(bool selected)
{
    int i;
      for(i=0;i<eaSize(&gTabData.available);i++)
    {
        AvailableChannel * ac = gTabData.available[i];
        if(   ac->selected == selected
            && ac->highlighted)
        {
            return true;
        }
    }

     return false;
}

AvailableChannel * selectedUserChannel()
{
    int i;
    for(i=0;i<eaSize(&gTabData.available);i++)
    {
        AvailableChannel * ac = gTabData.available[i];
        if(   ac->highlighted 
           && ac->type == ChannelType_User)
        {
            return ac;
        }
    }
    return 0;
}



int highlightedChannelCount(bool selected)
{
    int i;
    int count = 0;
    for(i=0;i<eaSize(&gTabData.available);i++)
    {
        AvailableChannel * ac = gTabData.available[i];
        if(   ac->selected == selected
            && ac->highlighted)
        {
            count++;
        }
    }

    return count;
}

int selectedChannelCount()
{
    int i;
    int count = 0;
    for(i=0;i<eaSize(&gTabData.available);i++)
    {
        AvailableChannel * ac = gTabData.available[i];
        if(   ac->selected )
            count++;
    }

    return count;
}

// un-highlight all channels in given list (either "selected" or "unselected" list)
void clearHighlighted()
{
    int i;
    for(i=0;i<eaSize(&gTabData.available);i++)
    {
        gTabData.available[i]->highlighted = false;
    }
}

// auto suggest name -- use the first channel name in the 'selected' list
void autoSuggestName()
{    
    int i;
    for(i=0;i<eaSize(&gTabData.available);i++)
    {
        AvailableChannel * ac = gTabData.available[i];
        if(ac->selected)
        {
            if(ac->type == ChannelType_System)
            {
                char buff[200];
                msPrintf( menuMessages, SAFESTR(buff), ac->system->name );
                uiEditSetUTF8Text(gTabData.nameEdit, buff );
            }
            else
                uiEditSetUTF8Text(gTabData.nameEdit, ac->user->name);

            return;
        }
    }
}


// move channels to selected or unselected column
void moveHighlightedChannels(bool selected)
{
    int i;
    for(i=0;i<eaSize(&gTabData.available);i++)
    {
        AvailableChannel * ac = gTabData.available[i];
        if(   ac->highlighted 
            && ac->selected == selected)
        {
            ac->selected = !selected;

             if(selected && ac->defaultChannel)
                setDefaultChannel(0);

             ac->highlighted = false;
        }

    }

    // choose a default channel (if necessary)
    if(!gTabData.defaultChannel)
        chooseDefaultChannel();

    // special actions if we just added a single channel to the current tab list
    if( ! selected)
    {
        // suggest a default name (if necessary)
         if(gTabData.nameEdit->contentUTF8ByteCount == 0)
        {
            autoSuggestName();
        }
    }
}

void drawChannel(float x, float* yp, float z, float scale, float wd, AvailableChannel * ac)
{
      float ht = ENTRY_SPACE*scale;
     float y = *yp;
    bool clicked = false;
    CBox box;
    char name[1000];
    int rgba[4];

    // only system channels need to be localized
    if(ac->type == ChannelType_System)
    {
        GetTextColorForType(ac->system->colorIdx, rgba);
        msPrintf(menuMessages, SAFESTR(name), ac->system->name);
    }
    else
    {
        if(UsingChatServer())
        {
            GetTextColorForType(INFO_CHANNEL_TEXT+ac->user->idx, rgba);
        }
        else
        {
            // user channel & disconnected from chatserver
            rgba[1] = CLR_GREY;
            rgba[0] = CLR_WHITE;
        }
         strcpy(name, ac->user->name);
    }

    font_color( rgba[1], rgba[0]);

    // for now, we'll just add the "Default" text tag
    if(ac->selected && ac->defaultChannel)
        strcat(name, textStd("DefaultParen"));


     font(&game_12);
    cprntEx(x + R6*scale, y+15*scale, z+1, scale, scale, (NO_MSPRINT), name);
    BuildCBox( &box, x, y, wd, ht);
  
    if(ac->highlighted && mouseDoubleClickHit(&box, MS_LEFT))
    {
        clearHighlighted();    
        ac->highlighted = true;
        moveHighlightedChannels(ac->selected);
    }
      else if( mouseDownHit( &box, MS_LEFT ) )//|| mouseClickHit( &box, MS_LEFT ) )
    {
        clearHighlighted();
        ac->highlighted = true;
    }

    if( mouseCollision(&box) )
    {
        drawFlatFrame( PIX2, R6, x, y, z+21, wd, (ENTRY_SPACE)*scale, scale, CLR_SELECTION_FOREGROUND, 0 );
    }

     if(ac->highlighted)
    {
           drawFlatFrame( PIX2, R6, x, y, z+20, wd, (ENTRY_SPACE)*scale, scale, CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
    }
     
      *yp += ENTRY_SPACE*scale;
}






void drawChannelList(float x, float y, float z, float scale, float wd, float ht, int color, int bcolor, ScrollBar * sb, bool selected)
{
    int i;
    float startY;
    float docHt = 0;
    CBox box;

     drawFrame( PIX3, R10, x, y, z+1, wd, ht, scale, color, bcolor );
    
    startY = y;
     y += PIX3*scale;

       scissor_dims(x+PIX3*scale, y, wd-2*PIX3*scale, ht-(PIX3*2*scale));
    BuildCBox(&box, x, y, wd, ht );
      set_scissor(TRUE);

     y += R10*scale;
     y -= sb->offset;

    for(i=0;i<eaSize(&gTabData.available);i++)
     {
        AvailableChannel * ac = gTabData.available[i];
        if(selected == ac->selected)
        {
            // draw it!
             drawChannel(x+(R10*scale), &y, z, scale, wd-(R10*2*scale), ac);
            docHt += ENTRY_SPACE;
        }
    }

    set_scissor(FALSE);

    doScrollBar(sb, ht-((12+R10*2)*scale), (docHt*scale), x + wd, startY+(6+R10*scale), z+2, &box, 0 );
}





//-----------------------------------------------------------------
// Join/Create dialogs (nearly identical to dialogs in uiChat.c, but assign to different filter.
//               I decided to go with a second set to make sure that channels added in the chat options
//               screen went to the selected filter, and not the "Active/Green Filter".
//-----------------------------------------------------------------

// if we are just undoing a (queued) command to leave a channel, simply dequeue it
static bool undoQueuedLeave(char * name)
{
    int i,size=eaSize(&gChatChannels);
    for(i=0;i<size;i++)
    {
        ChatChannel * cc = gChatChannels[i];
        if(   cc
           && cc->isLeaving
           && ! stricmp(name, cc->name))
        {
            AvailableChannel * ac;
            cc->isLeaving = 0;
            eaFindAndRemove(&gTabData.leaveQueue, cc);
            ac = AddAvailableUserChannel(cc);
            ac->selected = true;
            return 1;    // found!
        }
    }
    return 0;
}

static void chatOptionsCreateChannelDlgHandler(void * data)
{
    char * name = dialogGetTextEntry();
    if(name && !undoQueuedLeave(name))
    {
        addChannelSlashCmd("create", name, "reserve", gTabData.filter);
    }
}

static void chatOptionsCreateChannelDialog(void * data)
{
    dialogNameEdit(textStd("CreateChannelDialog") , NULL, NULL, chatOptionsCreateChannelDlgHandler, NULL, MAX_CHANNELNAME, 0 );
}


static void chatOptionsJoinChannelDlgHandler(void * data)
{
    char * name = dialogGetTextEntry();
    if(name && !undoQueuedLeave(name))
    {
        addChannelSlashCmd("join", name, "reserve", gTabData.filter);
    }
}

static void chatOptionsJoinChannelDialog(void * data)
{
    dialogNameEdit(textStd("JoinChannelDialog"), NULL, NULL, chatOptionsJoinChannelDlgHandler, NULL, MAX_CHANNELNAME, 0 );
}




//-----------------------------------------------------------------
// main tab options window code
//-----------------------------------------------------------------
int chatTabWindow()
{
    float x, y, z, wd, ht, scale;
    int color, bcolor;
 
    static ScrollBar current_sb = {0};
     static ScrollBar available_sb = {0};
    AvailableChannel * validChannel;
    UIBox bounds;
    CBox box;
     int locked, valid;
    float listWidth, listHeight, buttonCenterY;
 
    
    // did somebody close me? (one example is during a mapmove, window will always be saved as "closed" since it's mode is ALWAYS_CLOSED)
    if(gTabData.filter && ! windowUp(WDW_CHAT_OPTIONS))
    {
        gTabData.filter = 0;
        chatTabClose(false);
        return 0;
    }


      if( !window_getDims( WDW_CHAT_OPTIONS, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
    {
        return 0;
    }

       if(window_getMode(WDW_CHAT_OPTIONS) != WINDOW_DISPLAYING)
    {
        return 0;
    }

 //    xyprintf(1,1,"%f x %f (%.1fx)", wd, ht, scale);

    listWidth = (wd - LIST_SPACE*scale) / 2;
    listHeight = (ht - LIST_HEADER*scale);

    gTabData.nameEdit->z = z + 10;
    uiEditSetFontScale(gTabData.nameEdit, 1.25*scale);
  
    assert(gTabData.filter);


    drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, bcolor );

      font( &game_12 );
      font_color( CLR_WHITE, CLR_WHITE );
      cprntEx( x + wd/2 - (LIST_SPACE/2*scale + listWidth/2), y + (LIST_HEADER - 10)*scale, z, scale, scale, (CENTER_X | CENTER_Y), "SelectedChannels" );
      cprntEx( x + wd/2 + (LIST_SPACE/2*scale + listWidth/2), y + (LIST_HEADER - 10)*scale, z, scale, scale, (CENTER_X | CENTER_Y), "AvailableChannels" );

 
 
    // name edit box
    uiBoxDefine(&bounds,    x + wd/2 - (NAME_WIDTH / 2)*scale,
                            y + NAME_HEADER*scale,
                            NAME_WIDTH*scale,
                               NAME_HEIGHT*scale);
//    uiBoxAlter(&bounds, UIBAT_SHRINK, UIBAD_ALL, 5);
      uiEditSetBounds(gTabData.nameEdit, bounds);
    uiBoxToCBox(&bounds, &box);
//     drawBox(&box, z+10, CLR_ORANGE, 0);
    handleNameEditBox(&box, scale);

    // duplicate name warning message
    valid = validEditBoxName();
    if(valid == kEditBoxName_Duplicate)
    {
        font_color( CLR_RED, CLR_RED );
          cprntEx( x + wd/2, y + (LIST_HEADER - 26)*scale, z, scale, scale, (CENTER_X | CENTER_Y), "ChatTabNameAlreadyInUse" );
    }
    
    // channel filter categories
     createAvailableChannelList();
       drawChannelList(x + wd/2 - (listWidth + LIST_SPACE/2*scale),  y + LIST_HEADER*scale, z+1, scale, listWidth, listHeight, color, bcolor, &gTabData.selectedSB,  true);
    drawChannelList(x + wd/2 + (LIST_SPACE/2)*scale,              y + LIST_HEADER*scale, z+1, scale, listWidth, listHeight, color, bcolor, &gTabData.availableSB, false);
    // now, the buttons...

       buttonCenterY = ((LIST_HEADER - 30)*scale + listHeight/2);

       if(isChannelHighlighted(true))
    {
        // REMOVE Button
         locked = !isChannelHighlighted(true);
          if( D_MOUSEHIT == drawStdButton( x + wd/2, y + buttonCenterY - 5*BUTTON_SPACE/2*scale, z, BUTTON_WIDTH*scale, BUTTON_HEIGHT*scale, CLR_BLUE, "RemoveChannelFromTab", scale, locked ) )
         {
            int i;
            moveHighlightedChannels(true);

            // auto-highlight the first channel in the list
            for(i=0;i<eaSize(&gTabData.available);i++)
            {
                AvailableChannel * ac = gTabData.available[i];
                if(ac->selected)
                {
                    ac->highlighted = true;
                     break;
                }
            }
        }
    }
    else
    {     
        // ADD Button
            if( D_MOUSEHIT == drawStdButton( x + wd/2, y + buttonCenterY - 5*BUTTON_SPACE/2*scale, z, BUTTON_WIDTH*scale, BUTTON_HEIGHT*scale, CLR_BLUE, "AddChannelToTab", scale, !isChannelHighlighted(false) ) )
        {
            int i;
            moveHighlightedChannels(false);
            // auto-highlight the first channel in the list
            for(i=0;i<eaSize(&gTabData.available);i++)
            {
                AvailableChannel * ac = gTabData.available[i];
                if(!ac->selected)
                {
                    ac->highlighted = true;
                    break;
                }
            }
        }
    }

    // OK Button -- must have a valid/unique name
    locked = (valid != kEditBoxName_Valid);
    if( D_MOUSEHIT == drawStdButton( x + wd/2, y + listHeight + (LIST_HEADER - 3*BUTTON_SPACE/2 - 10)*scale, z, BUTTON_WIDTH*scale, BUTTON_HEIGHT*scale, CLR_GREEN, "OKString", scale*1.3f, locked) )
     {
        chatTabClose(true);
    }

    // CANCEL Button -- undo all changes & close
     if( D_MOUSEHIT == drawStdButton( x + wd/2, y + listHeight + (LIST_HEADER - BUTTON_SPACE/2 - 10)*scale, z, BUTTON_WIDTH*scale, BUTTON_HEIGHT*scale, CLR_GREEN, "CancelString", scale*1.3f, 0 ) )
    {
        chatTabClose(false);
    }

    // SET DEFAULT Button
       validChannel = selectableDefaultChannel(true);
     if( D_MOUSEHIT == drawStdButton( x + wd/2, y + buttonCenterY - 3*BUTTON_SPACE/2*scale, z, BUTTON_WIDTH*scale, BUTTON_HEIGHT*scale, CLR_BLUE, "SetDefaultChannel", scale, (validChannel==0) ) )
    {
        setDefaultChannel(validChannel);
    }

    if(UsingChatServer())
    {
        // CREATE CHANNEL Button
        locked = !canAddChannel();
        if( D_MOUSEHIT == drawStdButton( x + wd/2, y + buttonCenterY - BUTTON_SPACE/2*scale, z, BUTTON_WIDTH*scale, BUTTON_HEIGHT*scale, CLR_BLUE, "CreateChatChannelButton", scale, locked ) )
        {
            chatOptionsCreateChannelDialog(0);
        }

        // JOIN CHANNEL Button
         locked = !canAddChannel();
         if( D_MOUSEHIT == drawStdButton( x + wd/2, y + buttonCenterY + BUTTON_SPACE/2*scale, z, BUTTON_WIDTH*scale, BUTTON_HEIGHT*scale, CLR_BLUE, "JoinChatChannelButton", scale, locked ) )
        {
            chatOptionsJoinChannelDialog(0);
        }

        // LEAVE CHANNEL Button -- must be done last, as it might destroy/free a AvailableChannel in the list
        locked = ! (validChannel = selectedUserChannel());
        if( D_MOUSEHIT == drawStdButton( x + wd/2, y + buttonCenterY + 3*BUTTON_SPACE/2*scale, z, BUTTON_WIDTH*scale, BUTTON_HEIGHT*scale, CLR_BLUE, "LeaveChatChannelButton", scale, locked ) )
        {            
            // immediately leave "reserved" channels, as we haven't even officially joined yet.
            // Otherwise, add the channel to the leave queue (will only leave if user clicks OK)
            if(validChannel->user->isReserved)
            {
                leaveChatChannel(validChannel->user->name);
            }
            else
            {
                validChannel->user->isLeaving = 1;
                eaPush(&gTabData.leaveQueue, validChannel->user);
                eaFindAndRemove(&gTabData.available, validChannel);
                AvailableChannelDestroy(validChannel);
            }
        }
    }

     return 0;
}
