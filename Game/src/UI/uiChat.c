//
// chat.c
//-----------------------------------------------------------------------------------

#include <limits.h>
#include "UI/uiKeymapping.h"
#include "uiIME.h"
#include "player/player.h"
#include "entity/entPlayer.h"
#include "player/playerState.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/ttFont.h"
#include "UI/uiWindows.h"
#include "graphics/ttFontUtil.h"
#include "language/langClientUtil.h"

#include "edit/edit_cmd.h"
#include "clientcomm/clientcomm.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/earray.h>
#include "win/input.h"
#include "win/win_init.h"

#include "entity/entclient.h"
#include "UI/uiChat.h"
#include "UI/uiGame.h"
#include "UI/uiEditText.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiConsole.h"
#include "UI/uiInput.h"
#include "UI/uiScrollBar.h"
#include "UI/uiUtil.h"
#include "UI/uiToolTip.h"
#include "UI/uiNet.h"
#include "UI/uiFriend.h"
#include "UI/uiCursor.h"
#include "UI/uiTarget.h"
#include "UI/uiContextMenu.h"
#include "uiEmote.h"
#include "UI/uiPet.h"
#include "UI/uiClipper.h"
#include "gameData/menudef.h"
#include "demo.h"
#include "UI/uiFocus.h"
#include <utilitieslib/utils/sysutil.h>
#include "UIEdit.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/utils.h>
#include "sound/sound.h"
#include <utilitieslib/utils/strings_opt.h>
#include "UI/uiChatUtil.h"
#include "entity/chatSettings.h"
#include "UI/uiWindows_init.h"
#include "UI/uiArena.h"
#include "uiChatOptions.h"
#include "UI/uiOptions.h"
#include "UI/uiDialog.h"
#include "UI/uiTextLink.h"
#include "gameComm/chatClient.h"
#include "chatdb.h"
#include "graphics/font.h"
#include <utilitieslib/utils/timing.h>
#include "graphics/textureatlas.h"
#include "entity/entity.h"
#include "entity/teamCommon.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/components/bitfield.h>
#include "entity/character_base.h"
#include "entity/boost.h"
#include "entity/powers.h"
#include "bases/DetailRecipe.h"
#include "UI/uiTray.h"
#include "UI/uiChannel.h"
#include "uiColorPicker.h"
#include "UI/uiOptions.h"
#include "UI/uiPlayerNote.h"

#include "formatter/smf_main.h"
#include <direct.h>
#include <utilitieslib/components/HashFunctions.h>
#include "arena/arenagame.h"
#include "comm_game.h"
#include "filter/validate_name.h"
#include "uiLevelingpact.h"
#include "filter/profanity.h"
#include "bases/baseedit.h"
#include <utilitieslib/utils/log.h>
#include "entity/character_target.h"

void chat_History( int up );
void chat_addHistory( char * txt );
void initChatEdit(void);
void displayBillingCountdown();
void setPaymentRemainTime(int curr_avail, int total_avail, int reservation, bool countdown, bool change_billing);


int gSentRespecMsg = 0;
int gChatLogonTimer = 0;


//---------------------------------------------------------------------------------------------------------
// Globals and Defines
//---------------------------------------------------------------------------------------------------------
#define CHAT_TEXT_LEN       ( 255 )
#define MAX_CHAT_LINES      ( 1000 )


#define CHAT_EDIT_HEIGHT                (25)
#define PRIMARY_CHAT_MINIMIZED_HEIGHT    (CHAT_EDIT_HEIGHT + PIX3)
#define    CHAT_TAB_HEIGHT                    (25)
#define CHANNEL_SPACER                    (12)

#define CHAT_HEADER                        (CHAT_TAB_HEIGHT + 10)    // empty space at top of chat text (allows very top/oldest few lines of chat to be seen when scrolling through history)

static int ignore_first_enter;
static bool s_bGobbleFirst = false;
static int s_fontSize = 0;
static bool gbSpecialQuitToLoginCase = FALSE;    // special flag used to handle a "quittologin" command (see below)
char gUserSendChannel[MAX_CHANNELNAME + 1];
static ContextMenu *chatContext = 0;
static ContextMenu *chatLinkContext = 0;
static ContextMenu *chatLinkGlobalContext = 0;
static ContextMenu *chatLinkChannelContext = 0;

static char gLastPrivate[64];

static int s_text_styles[][10] =
{
    //name,           italic, bold, outline, drapshadow, softshadow, outlinewidth, dropshadowXoffset, dropshadowYoffset, softshadowspread  },
    { INFO_EMOTE,          1,    0,       0,          0,          0,            0,                 0,                 0,                0  },
    { INFO_ADMIN_COM,      0,    1,       0,          0,          0,            0,                 0,                 0,                0  },
};

static int s_text_colors[][8] =
{
    // Channel Name                // Color1            // Color2            // ColorHover        // ColorSelect    //ColorSelectBG        // LinkSelect        // LinkSelectBG
    { INFO_COMBAT,                CLR_YELLOW,            CLR_RED,            CLR_YELLOW,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_COMBAT_SPAM,            0xff8800ff,            0xff8800ff,            0xff8800ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_COMBAT_ERROR,        0xff9966ff,            0xff9966ff,            0xff9966ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_HEAL,                0x66ff44ff,            0x44ff22ff,            0x66ff44ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_HEAL_OTHER,            0x88ffaaff,            0x44ff88ff,            0x88ffaaff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_DAMAGE,                CLR_ORANGE,            CLR_RED,            CLR_ORANGE,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_SVR_COM,                0x90ff90ff,            CLR_GREEN,            0x90ff90ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_NPC_SAYS,            0xa0a0ffff,            CLR_BLUE,            0xa0a0ffff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_VILLAIN_SAYS,        0xa0a0ffff,            CLR_BLUE,            0xa0a0ffff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_PET_COM,                CLR_WHITE,            CLR_BLUE,            CLR_WHITE,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_PRIVATE_COM,            CLR_YELLOW,            CLR_YELLOW,            CLR_YELLOW,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_TEAM_COM,            0x88ff88ff,            0x00aa00ff,            0x88ff88ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_LEVELINGPACT_COM,    0xf3f8a4ff,            0xf130c8ff,            0xff6633ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_SUPERGROUP_COM,        CLR_PARAGON,        CLR_PARAGON,        CLR_PARAGON,        0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_NEARBY_COM,            CLR_WHITE,            CLR_WHITE,            CLR_WHITE,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_SHOUT_COM,            0xbbbbbbff,            0xbbbbbbff,            0xbbbbbbff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_REQUEST_COM,            0xff44ffff,            0xa0a0ffff,            0xff44ffff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_FRIEND_COM,            CLR_YELLOW,            CLR_ORANGE,            CLR_YELLOW,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_ADMIN_COM,            CLR_RED,            CLR_RED,            CLR_RED,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_EMOTE,                CLR_WHITE,            CLR_WHITE,            CLR_WHITE,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_AUCTION,                0x00ffffff,            0x0060ffff,            0x00ffffff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_ARCHITECT,            CLR_MM_TEXT,        CLR_MM_TITLE_OPEN,    CLR_MM_TEXT,        0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_GMTELL,                CLR_ONLINE_ITEM,    CLR_ONLINE_ITEM,    CLR_ONLINE_ITEM,    0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_TAB,                    CLR_ONLINE_ITEM,    CLR_ONLINE_ITEM,    CLR_ONLINE_ITEM,    0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_REWARD,                0xffff88ff,            0xffff88ff,            0xffff88ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_ARENA,                CLR_WHITE,            CLR_WHITE,            CLR_WHITE,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_ARENA_ERROR,            CLR_YELLOW,            CLR_ORANGE,            CLR_YELLOW,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_ARENA_GLOBAL,        CLR_WHITE,            0xff44ffff,            CLR_WHITE,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_HELP,                0xccffccff,            CLR_BLUE,            0xccffccff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_ALLIANCE_OWN_COM,    0xffd700ff,            0xffd700ff,            0xffd700ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_ALLIANCE_ALLY_COM,    0xdaa520ff,            0xdaa520ff,            0xdaa520ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_USER_ERROR,            CLR_YELLOW,            CLR_WHITE,            CLR_YELLOW,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_DEBUG_INFO,            0xffaaffff,            0xffaaffff,            0xffaaffff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_COMBAT_HITROLLS,        0xffed61ff,            0xffed61ff,            0xffed61ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_EVENT,                0xff2222ff,            0xff2222ff,            0xff2222ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_CHAT_TEXT,            CLR_WHITE,            CLR_WHITE,            CLR_WHITE,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_PROFILE_TEXT,        CLR_BLACK,            CLR_BLACK,            CLR_BLACK,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_HELP_TEXT,            CLR_GREEN,            CLR_GREEN,            CLR_GREEN,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_STD_TEXT,            CLR_ONLINE_ITEM,    CLR_ONLINE_ITEM,    CLR_ONLINE_ITEM,    0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_SCRAPPER,            0xff8800ff,            0xff8844ff,            0xff8800ff,            0,                0x333333ff,            0,                    0x666666ff,            },    // Scrapper
    { INFO_CHANNEL_TEXT,        0xccffccff,            0xccffccff,            0xccffccff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_CHANNEL_TEXT1,        0xccff99ff,            0xccff99ff,            0xccff99ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_CHANNEL_TEXT2,        0x99ffccff,            0x99ffccff,            0x99ffccff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_CHANNEL_TEXT3,        0x99ff99ff,            0x99ff99ff,            0x99ff99ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_CHANNEL_TEXT4,        0x77ffffff,            0x77ffffff,            0x77ffffff,            0,                0x333333ff,            0,                    0x666666ff,            }, 
    { INFO_PET_COMBAT,            CLR_YELLOW,            CLR_RED,            CLR_YELLOW,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_PET_COMBAT_SPAM,        0xff8800ff,            0xff8800ff,            0xff8800ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_PET_HEAL,            0x66ff44ff,            0x44ff22ff,            0x66ff44ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_PET_HEAL_OTHER,        0x88ffaaff,            0x44ff88ff,            0x88ffaaff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_PET_DAMAGE,            CLR_ORANGE,            CLR_RED,            CLR_ORANGE,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_PET_COMBAT_HITROLLS,    0xffed61ff,            0xffed61ff,            0xffed61ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_EVENT_HERO,            0xbbbbffff,            0x802080ff,            0xbbbbffff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_EVENT_VILLAIN,        0xff2222ff,            0x802080ff,            0xff2222ff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_EVENT_PRAETORIAN,    0xffffffff,            0x444444ff,            0xffffffff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_ARCHITECT_GLOBAL,    CLR_MM_TEXT,        CLR_MM_TEXT,        CLR_MM_TITLE_OPEN,    0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_LOOKING_FOR_GROUP,    0x00ffffff,            0x008080ff,            0xffffffff,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_PRIVATE_NOREPLY_COM,    CLR_YELLOW,            CLR_YELLOW,            CLR_YELLOW,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_LEAGUE_COM,            CLR_MM_TEXT,        CLR_MM_TEXT,        CLR_MM_TITLE_OPEN,            0,                0x333333ff,            0,                    0x666666ff,            },
    { INFO_CAPTION,            0xa0a0ffff,            CLR_BLUE,            0xa0a0ffff,            0,                0x333333ff,            0,                    0x666666ff,            },
};



enum
{
    CHAT_TIP_QUICK,
    CHAT_TIP_LOCAL,
    CHAT_TIP_BROAD,
    CHAT_TIP_FRIEND,
    CHAT_TIP_TEAM,
    CHAT_TIP_SUPER,
    CHAT_TIP_LPACT,
    CHAT_TIP_REQ,
    CHAT_TIP_ALLIANCE,
    CHAT_TIP_TAB,
    CHAT_TIP_ARENA,
    CHAT_TIP_HELP,
    CHAT_TIP_LFG,
    CHAT_TIP_TOTAL,
};


static ToolTip chatTip[CHAT_TIP_TOTAL] = {0};
static int enterChatMode(int mode);


//---------------------------------------------------------------------------------------------------------
// Save/load from database
//---------------------------------------------------------------------------------------------------------

static void saveEntityChatSettings(Entity * e)
{
    int i,j,m;
    int tabIdx = 0;
    ChatSettings * settings = &e->pl->chat_settings;
    int found_settings[MAX_CHAT_CHANNELS] = {0};

    // do not erase settings->options, as this might have been set elsewhere
    memset(&settings->windows,    0, SIZEOF2(ChatSettings, windows));
    memset(&settings->tabs,        0, SIZEOF2(ChatSettings, tabs));

    // we never want to localize after the initial update...
    settings->options |= CSFlags_DoNotLocalize;

    // clear out old active tab setting
    settings->options &= ( ~(CSFlags_ActiveChatWindowIdx));

    // is primary chat window minimized? (store in 11 MSB bits)
    settings->primaryChatMinimized = GetChatWindow(0)->minimizedOffset;

    for( i=0; i<MAX_CHAT_CHANNELS; i++ )
    {
        // We can never be a member of a channel with no name
        if( settings->channels[i].name[0] )
        {
            for( j = 0; j < eaSize(&gChatChannels); j++ )
            {
                if( stricmp( settings->channels[i].name, gChatChannels[j]->name ) == 0 )
                {
                    found_settings[i] = 1;
                    break;
                }
            }
        }
    }

    // save chat channels
    for(i=0;i<eaSize(&gChatChannels)&&i<MAX_CHAT_CHANNELS;i++)
    {
        ChatChannel * channel = gChatChannels[i];
        int found_idx = -1;

        // The settings slot for the channel is picked in three priorities.
        // First, slot already describing that channel - just update the settings
        // Second, empty slot - add the new channel's information in
        // Third, non-empty but unused slot - replace old settings for a channel you left
        for( j = 0; j < MAX_CHAT_CHANNELS; j++ )
        {
            if( strcmp( settings->channels[j].name, channel->name ) == 0 )
            {
                found_idx = j;
                break;
            }
        }

        if( found_idx < 0 )
        {
            for( j = 0; j < MAX_CHAT_CHANNELS; j++ )
            {
                if( !( settings->channels[j].name[0] ) )
                {
                    found_idx = j;
                    break;
                }
            }
        }

        if( found_idx < 0 )
        {
            for( j = 0; j < MAX_CHAT_CHANNELS; j++ )
            {
                if( !found_settings[j] )
                {
                    found_idx = j;
                    break;
                }
            }
        }

        // Make sure we're not a member of more channels that we can store settings for
        devassert( found_idx >= 0 );

        if( found_idx >= 0 )
        {
            strncpyt(settings->channels[found_idx].name, channel->name, SIZEOF2(ChatChannelSettings, name));
            settings->channels[found_idx].optionsBF = 0;    // no options yet...    
            settings->channels[found_idx].color1 = channel->color;
            settings->channels[found_idx].color2 = channel->color2;
            channel->idx = found_idx;
        }
    }

    for(i=0;i<MAX_CHAT_WINDOWS;i++)
    {        
        ChatWindow * window = GetChatWindow(i);
        ChatFilter * filter;
        ChatFilter * selected = uiTabControlGetSelectedData(window->topTabControl);

        settings->windows[i].divider = window->divider;

        // save top tabs
        filter = uiTabControlGetFirst(window->topTabControl);
        while(filter)
        {
            // mark tab
            settings->windows[i].tabListBF |= (1 << tabIdx);

            // is this tab selected ?
            if(selected == filter)
                settings->windows[i].selectedtop = (1 << tabIdx);

            // is this the active tab?
            if(gActiveChatFilter == filter)
            {
                settings->options |= ( (i << 2) & CSFlags_ActiveChatWindowIdx);
                settings->options &= ~(CSFlags_BottomDividerSelected);
            }

            if(strlen(filter->name)>MAX_TAB_NAME_LEN)
                printf("Bad chat tab name received: %s\n", filter->name);
            strncpyt(settings->tabs[tabIdx].name, filter->name, SIZEOF2(ChatTabSettings, name));

            BitFieldCopy( filter->systemChannels, settings->tabs[tabIdx].systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE );
            settings->tabs[tabIdx].optionsBF = (1<<ChannelOption_Top);

            settings->tabs[tabIdx].defaultType = filter->defaultChannel.type;
            if(filter->defaultChannel.type == ChannelType_System)
                settings->tabs[tabIdx].defaultChannel = filter->defaultChannel.system;

            for(m=0;m<eaSize(&filter->channels);m++)
            {
                ChatChannel * channel = filter->channels[m];

                if(m>=MAX_CHAT_CHANNELS)
                {
                    printf("Filter %s has too many chat channels (%d)", filter->name, eaSize(&filter->channels));                
                    break;
                }        

                // mark channel
                settings->tabs[tabIdx].userBF |= (1 << channel->idx);
                if(   filter->defaultChannel.type == ChannelType_User
                    && filter->defaultChannel.user == channel)
                {
                    settings->tabs[tabIdx].defaultChannel = (1 << channel->idx);
                }
            }

            filter = uiTabControlGetNext(window->topTabControl);

            tabIdx++;
        }

        // I'm lazy so I'm just copying from above
        selected = uiTabControlGetSelectedData(window->botTabControl);

        // save top tabs
        filter = uiTabControlGetFirst(window->botTabControl);
        while(filter)
        {
            // mark tab
            settings->windows[i].tabListBF |= (1 << tabIdx);

            // is this tab selected ?
            if(selected == filter)
                settings->windows[i].selectedbot = (1 << tabIdx);

            // is this the active tab?
            if(gActiveChatFilter == filter)
            {
                settings->options |= ( (i << 2) & CSFlags_ActiveChatWindowIdx);
                settings->options |= CSFlags_BottomDividerSelected;
            }

            if(strlen(filter->name)>MAX_TAB_NAME_LEN)
                printf("Bad chat tab name received: %s\n", filter->name);
            strncpyt(settings->tabs[tabIdx].name, filter->name, SIZEOF2(ChatTabSettings, name));

            BitFieldCopy( filter->systemChannels, settings->tabs[tabIdx].systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE );
            settings->tabs[tabIdx].optionsBF = (1<<ChannelOption_Bottom);

            settings->tabs[tabIdx].defaultType = filter->defaultChannel.type;
            if(filter->defaultChannel.type == ChannelType_System)
                settings->tabs[tabIdx].defaultChannel = filter->defaultChannel.system;

            for(m=0;m<eaSize(&filter->channels);m++)
            {
                ChatChannel * channel = filter->channels[m];

                if(m>=MAX_CHAT_CHANNELS)
                {
                    printf("Filter %s has too many chat channels (%d)", filter->name, eaSize(&filter->channels));                
                    break;;
                }    

                // mark channel
                settings->tabs[tabIdx].userBF |= (1 << channel->idx);
                if(   filter->defaultChannel.type == ChannelType_User
                    && filter->defaultChannel.user == channel)
                {
                    settings->tabs[tabIdx].defaultChannel = (1 << channel->idx);
                }
            }

            filter = uiTabControlGetNext(window->botTabControl);

            tabIdx++;
        }
    }

    for(i=0;i<MAX_CHAT_CHANNELS;i++)
    {
        if( ! stricmp(gUserSendChannel, settings->channels[i].name))
        {
            settings->userSendChannel = i;
            break;
        }
    }
}


void sendChatSettingsToServer()
{
    Entity * e = playerPtr();
    assert(e);

    // Both saveEntityChatSettings() and sendChatSettings() will crash if playerPtr is NULL.
    // Doesn't fix the root cause of why playerPtr() is NULL, but averts the crash
    // here if that is a valid condition.
    if (e)
    {
        saveEntityChatSettings(e);

        START_INPUT_PACKET(pak, CLIENTINP_SEND_CHAT_SETTINGS);
        sendChatSettings(pak, e);
        END_INPUT_PACKET
    }
}

static void sendChatChannel( int channel, char * userChannel )
{    
    int i=0;
    Entity * e = playerPtr();
    assert(e);

    strncpyt(gUserSendChannel, userChannel, sizeof(gUserSendChannel));

    if(channel == INFO_CHANNEL)
    {
        int found = 0;
        for(i=0;i<MAX_CHAT_CHANNELS;i++)
        {
            if( ! stricmp(userChannel, e->pl->chat_settings.channels[i].name))
            {    
                found = 1;
                break;
            }
        }

        if(!found)
        {
            sendChatSettingsToServer();
            return;
        }
    }

    START_INPUT_PACKET(pak, CLIENTINP_SET_CHAT_CHANNEL);
    pktSendBitsPack(pak, 1, channel);
    pktSendBitsPack(pak, 1, i);
    END_INPUT_PACKET
}

void sendSelectedChatTabsToServer()
{
    int i,k;
    bool bottom_selected = 0;
    int activeWindowIdx = 0;

    Entity * e = playerPtr();
    assert(e);

    START_INPUT_PACKET(pak, CLIENTINP_SEND_SELECTED_CHAT_TABS);
    // send selected tabs for each window
    for(i=0;i<MAX_CHAT_WINDOWS;i++)
    {
        // TODO: add bottom tab sending
        ChatWindow * window = GetChatWindow(i);
        ChatFilter * selected = uiTabControlGetSelectedData(window->topTabControl);
        int idx;

        if(selected)
        {
            if( gActiveChatFilter && selected == gActiveChatFilter)
            {
                activeWindowIdx = i;
                bottom_selected = false;
            }

            for(k=0;k<MAX_CHAT_TABS;k++)
            {
                if( ! stricmp(selected->name, e->pl->chat_settings.tabs[k].name))
                {
                     idx = (1 << k);
                    break;
                }
            }
        }
        else
            idx = 0;

        pktSendBitsPack(pak, 1, idx);

        selected = uiTabControlGetSelectedData(window->botTabControl);

        if(selected)
        {
            if( gActiveChatFilter && selected == gActiveChatFilter)
            {
                activeWindowIdx = i;
                bottom_selected = true;
            }

            for(k=0;k<MAX_CHAT_TABS;k++)
            {
                if( ! stricmp(selected->name, e->pl->chat_settings.tabs[k].name))
                {
                    idx = (1 << k);
                    break;
                }
            }
        }
        else
            idx = 0;

        pktSendBitsPack(pak, 1, idx);

    }

    // active window IDX
    pktSendBitsPack(pak, 1, activeWindowIdx);
    pktSendBits(pak, 1, bottom_selected);
    
    END_INPUT_PACKET
}



static void loadEntityChatSettings(Entity * e, int full_update)
{
    int i,k,m;
    ChatSettings * settings = &e->pl->chat_settings;
    ChatFilter ** unusedFilters = NULL;

    // get minimized setting (stored in 11 MSB options bits)
    GetChatWindow(0)->minimizedOffset = settings->primaryChatMinimized;
    if(settings->primaryChatMinimized)
    {
        WdwBase * loc = &winDefs[WDW_CHAT_BOX].loc;
        loc->ht = PRIMARY_CHAT_MINIMIZED_HEIGHT; // FORCE the height, just in case (it SHOULD be loaded/saved correctly...)
        loc->draggable_frame = FALSE;    // this hack is necessary because WDW_CHAT_BOX is always saved as having a "draggable frame" in order to save width/height info (see sendWindow())
    }

    // set the user send channel (if applicable)
    i = settings->userSendChannel;
    if(  e->pl->chatSendChannel == INFO_CHANNEL && i >= 0 && i < MAX_CHAT_CHANNELS)
        strncpyt(gUserSendChannel, settings->channels[i].name, MAX_CHANNELNAME);
    else
        gUserSendChannel[0] = 0;

    // empty out all chat windows
    for(i=0;i<MAX_CHAT_WINDOWS;i++)
    {        
        ChatWindowRemoveAllFilters(GetChatWindow(i));
    }

     ChatFilterPrepareAllForUpdate(full_update);
    SelectActiveChatFilter(0); // reset

    // now, rebuild
    for(i=0;i<MAX_CHAT_WINDOWS;i++)
    {        
        ChatWindow * window = GetChatWindow(i);

        window->divider = settings->windows[i].divider;

        // load tabs
        for(k=0;k<32;k++)
        {
            if( settings->windows[i].tabListBF & (1 << k) && settings->tabs[k].name[0])     // ignore unnamed tabs
            {
                ChatFilter * filter = GetChatFilter(settings->tabs[k].name);
                if( !filter)
                    filter = ChatFilterCreate(settings->tabs[k].name);

                if(!AnyChatWindowHasFilter(filter))
                {
                    ChatWindowAddFilter(window, filter, (settings->tabs[k].optionsBF&(1<<ChannelOption_Bottom)) );

                    filter->defaultChannel.type = ChannelType_None;
                    BitFieldCopy( settings->tabs[k].systemChannels, filter->systemChannels , SYSTEM_CHANNEL_BITFIELD_SIZE );

                    if(settings->tabs[k].defaultType == ChannelType_System)
                    {
                        filter->defaultChannel.system = settings->tabs[k].defaultChannel;
                        filter->defaultChannel.type = ChannelType_System;
                    }

                    // load channels
                     for(m=0;m<32;m++)
                    {
                        ChatChannel * channel = GetChannel(settings->channels[m].name);
                        if( channel )
                        {
                            channel->color = settings->channels[m].color1;
                            channel->color2 = settings->channels[m].color2;
                        }

                        if(settings->tabs[k].userBF & (1 << m))
                        {
                            bool isDefault = false;
                            if( settings->tabs[k].defaultType == ChannelType_User && settings->tabs[k].defaultChannel == (1 << m))
                                isDefault = true;

                            if(!full_update) 
                            {
                                if( channel )
                                    ChatFilterAddChannel(filter, channel);
                                else
                                    addChannelSlashCmd("join", settings->channels[m].name, 0, 0);
                            }
                            else
                                ChatFilterAddPendingChannel(filter, settings->channels[m].name, isDefault);    
                        }
                    }

                    if((1 << k) == settings->windows[i].selectedtop)
                    {
                        if(uiTabControlSelectEx(window->topTabControl, filter, 0))
                        {
                            // is this the ACTIVE tab?
                            if( i == ((settings->options & CSFlags_ActiveChatWindowIdx) >> 2) &&
                                !(settings->options&CSFlags_BottomDividerSelected) )
                            {
                                gActiveChatFilter = filter;
                            }
                        }
                    }

                    if((1 << k) == settings->windows[i].selectedbot)
                    {
                        if(uiTabControlSelectEx(window->botTabControl, filter, 0))
                        {
                            // is this the ACTIVE tab?
                            if( i == ((settings->options & CSFlags_ActiveChatWindowIdx) >> 2) &&
                                (settings->options&CSFlags_BottomDividerSelected) )
                            {
                                gActiveChatFilter = filter;
                            }
                        }
                    }
                }
            }
        }    
    }

    // cleanup unclaimed filters
    eaCreate(&unusedFilters);
    for(i=0;i<eaSize(&gChatFilters);i++)
    {
        if(!GetFilterParentWindow(gChatFilters[i]))
            eaPush(&unusedFilters, gChatFilters[i]);
    }

    for(i=0;i<eaSize(&unusedFilters);i++)
    {
        if( currentChatOptionsFilter() == unusedFilters[i] )
            chatTabClose(0);
        else
            ChatFilterDestroy(unusedFilters[i]);
    }
    eaDestroy(&unusedFilters);
}

static void playerSeenLegacyUINote(int keepNew)
{
    optionSet(kUO_UseOldTeamUI, !keepNew, 1);
    START_INPUT_PACKET( pak, CLIENTINP_LEGACYTEAMUI_NOTE );
    END_INPUT_PACKET
}
static void playerSeenLegacyUINoteKeep(void *data)
{
    playerSeenLegacyUINote(1);
}
static void playerSeenLegacyUINoteLegacy(void *data)
{
    playerSeenLegacyUINote(0);
}
void receiveChatSettingsFromServer(Packet * pak, Entity * e)
{
    assert(e);
    
    receiveChatSettings(pak, e);

    // localize names (if initial default settings)
    if( !(e->pl->chat_settings.options & CSFlags_DoNotLocalize))
    {
        int i;

        // need to localize default names
        for(i=0;i<MAX_CHAT_TABS;i++)
        {
            char buf[2000];
            ChatTabSettings * tabs = &e->pl->chat_settings.tabs[i];
            if(tabs->name[i])
            {
                msPrintf(menuMessages, SAFESTR(buf), tabs->name);
                strncpyt(tabs->name, buf, SIZEOF2(ChatTabSettings, name));
            }
        }
    }

    if ( !(e->pl->chat_settings.options & CSFlags_AddedLegacyUINote))
    {
        dialogStd(DIALOG_TWO_RESPONSE, "NotifyLegacyTeamUILocation", "KeepNewTeamUI", 
            "UseOldTeamUI", playerSeenLegacyUINoteKeep, playerSeenLegacyUINoteLegacy, 0 );
    }

    if( e->pl->helpChatAdded )
    {
        int i;
        // need to localize default names
        for(i=0;i<MAX_CHAT_TABS;i++)
        {
            char buf[2000];
            ChatTabSettings * tabs = &e->pl->chat_settings.tabs[i];
            if(stricmp( "DefaultHelpChat", tabs->name)==0)
            {
                msPrintf(menuMessages, SAFESTR(buf), tabs->name);
                strncpyt(tabs->name, buf, SIZEOF2(ChatTabSettings, name));
            }
        }
    }

    loadEntityChatSettings(e, 1);

    // the very first time that we get our chat settings, immediately save the
    // localized names to the dbserver
     if( !(e->pl->chat_settings.options & CSFlags_DoNotLocalize) || e->pl->helpChatAdded )
        sendChatSettingsToServer();
}

//---------------------------------------------------------------------------------------------------------
// Save/load from file
//---------------------------------------------------------------------------------------------------------

void saveChatSettings( char * pchFile )
{
    int i, count = 0;
    FILE *file;
    Entity * e = playerPtr();
    ChatSettings * settings = &e->pl->chat_settings; 
    char filename[FILENAME_MAX];
    char dir[FILENAME_MAX];

    if(pchFile)
        strcpy( filename, pchFile );
    else if(isDevelopmentMode())
        strcpy(filename, "C:/chat.txt");
    else
        sprintf(filename, "%s/chat.txt", getExecutableDir(dir));

    file = fileOpen( filename, "wt" );
    if (!file)
    {
        addSystemChatMsg("UnableChatSave", INFO_USER_ERROR, 0);
        return;
    }

    fprintf( file, "%i %i %i\n", settings->userSendChannel, settings->options, settings->primaryChatMinimized );
    
    for(i=0;i<MAX_CHAT_WINDOWS;i++)
    {
        fprintf( file, "%i %i %i %f\n", settings->windows[i].selectedtop, 
                                         settings->windows[i].selectedbot,
                                         settings->windows[i].tabListBF,
                                         settings->windows[i].divider );
    }

    for(i=0;i<MAX_CHAT_TABS;i++)
    {
        ChatTabSettings * tab = &settings->tabs[i];
        int j;
        for(j=0; j < SYSTEM_CHANNEL_BITFIELD_SIZE; j++)
            fprintf( file, "%i ", tab->systemChannels[j] );
        fprintf( file, "\"%s\" %i %i %i %i\n", tab->name, tab->userBF, tab->optionsBF, tab->defaultChannel, tab->defaultType);
    }

    for(i=0;i<MAX_CHAT_CHANNELS;i++)
    {
        ChatChannelSettings * channel = &settings->channels[i];
        fprintf( file, "\"%s\" %i %i %i\n", channel->name, channel->optionsBF, channel->color1, channel->color2);
    }

    addSystemChatMsg( textStd("ChatSettingsSaved", filename), INFO_SVR_COM, 0);
    fclose(file);
}

void loadChatSettings( char * pchFile )
{
    FILE *file;
    char buf[1000];
    Entity * e = playerPtr();
    ChatSettings *settings = &e->pl->chat_settings;
    char *args[5+SYSTEM_CHANNEL_BITFIELD_SIZE];
    int i, count;
    char filename[FILENAME_MAX];
    char dir[FILENAME_MAX];

     if(pchFile)
        strcpy( filename, pchFile );
    else if(isDevelopmentMode())
        strcpy(filename, "C:/chat.txt");
    else
        sprintf(filename, "%s/chat.txt", getExecutableDir(dir));

    file = fileOpen( filename, "rt" );
    if (!file )
    {
        if(pchFile)
            addSystemChatMsg(textStd("CantReadChat", filename), INFO_USER_ERROR, 0);
        return;
    }

    if(!fgets(buf,sizeof(buf),file))
        goto file_error;
    count = tokenize_line_safe( buf, args, 5+SYSTEM_CHANNEL_BITFIELD_SIZE, 0 );
    if( count != 3 ) 
        goto file_error;
    else
    {
        settings->userSendChannel = atoi(args[0]);
        settings->options = atoi(args[1]);
        settings->primaryChatMinimized = atoi(args[2]);
    }

    for(i=0;i<MAX_CHAT_WINDOWS;i++)
    {
        if(!fgets(buf,sizeof(buf),file))
            goto file_error;
        count = tokenize_line_safe( buf, args, 5+SYSTEM_CHANNEL_BITFIELD_SIZE, 0 );
        if( count != 4 ) 
            goto file_error;
        else
        {
            settings->windows[i].selectedtop = atoi(args[0]);
            settings->windows[i].selectedbot = atoi(args[1]);
            settings->windows[i].tabListBF = atoi(args[2]);
            settings->windows[i].divider = atof(args[3]);
        }
    }

     for(i=0;i<MAX_CHAT_TABS;i++)
    {
        ChatTabSettings * tab = &settings->tabs[i];
        if(!fgets(buf,sizeof(buf),file))
            goto file_error;
        count = tokenize_line_safe( buf, args, 5+SYSTEM_CHANNEL_BITFIELD_SIZE, 0 );
        if( count != 5+SYSTEM_CHANNEL_BITFIELD_SIZE ) 
            goto file_error;
        else
        {
            int j;
            for(j=0; j < SYSTEM_CHANNEL_BITFIELD_SIZE; j++)
                tab->systemChannels[j] = atoi(args[j]);
            strcpy_s(tab->name, MAX_TAB_NAME_LEN+1, args[j]);
            tab->userBF = atoi(args[j+1]);
            tab->optionsBF = atoi(args[j+2]); 
            tab->defaultChannel = atoi(args[j+3]);
            tab->defaultType = atoi(args[j+4]);
        }
    }

    for(i=0;i<MAX_CHAT_CHANNELS;i++)
    {
        ChatChannelSettings * channel = &settings->channels[i];
        if(!fgets(buf,sizeof(buf),file))
            goto file_error;
        count = tokenize_line_safe( buf, args, 5+SYSTEM_CHANNEL_BITFIELD_SIZE, 0 );
        if( count != 4 ) 
            goto file_error;
        else
        {
            strcpy_s(channel->name, MAX_CHANNELNAME + 1, args[0]);
            channel->optionsBF = atoi(args[1]);
            channel->color1 = atoi(args[2]);
            channel->color2 = atoi(args[3]);
        }
    }

    // didn't error so copy to ent
    //memcpy( &e->pl->chat_settings, &settings, sizeof(ChatSettings) );
     loadEntityChatSettings(e,0); 
    sendChatSettingsToServer();

    addSystemChatMsg( textStd("ChatSettingsLoaded", filename), INFO_SVR_COM, 0);
    fclose(file);
    gbSpecialQuitToLoginCase = TRUE;
    return;

file_error:
    fclose(file);
    addSystemChatMsg( textStd("InvalidChatFile"), INFO_USER_ERROR, 0 );
    return;
}

//---------------------------------------------------------------------------------------------------------
// Helper
//---------------------------------------------------------------------------------------------------------

void joinChatChannel(char * name)
{
    char buf[1000];
    sprintf(buf, "chan_join %s", name);
    cmdParse(buf);
}

void leaveChatChannel(char * name)
{
    char buf[100 + MAX_CHANNELNAME];
    sprintf(buf, "chan_leave %s", name);
    cmdParse(buf);
}

void chatChannelInviteDenied(char *channel, char *inviter)
{
    char buf[100 + MAX_CHANNELNAME];
    sprintf(buf, "chan_invitedeny \"%s\" \"%s\"", channel, inviter);
    cmdParse(buf);
}


static F32 getColorLuminosity(int color)
{
    F32 red = .3f * (float)((color>>24)&0xff);
    F32 green = .59f * (float)((color>>16)&0xff);
    F32 blue = .11f * (float)((color>>8)&0xff);
    return red+green+blue;
}

void channelSetDefaultColor(ChatChannel *channel)
{
    int iter = INFO_CHANNEL_TEXT;
    int i;

    iter += (hashStringInsensitive(channel->name) % 5);

    for(i=0; i<ARRAY_SIZE(s_text_colors); i++)
    {
        if(s_text_colors[i][0]==iter)
        {
            channel->color = s_text_colors[i][1];
            channel->color2 = s_text_colors[i][2];
        }
    }
}

char * getChannelName(int type)
{
    switch( type )
    {
        xcase INFO_GMTELL:                return textStd("chatPrivate");
        xcase INFO_PRIVATE_COM:            return textStd("chatPrivate");
        xcase INFO_PRIVATE_NOREPLY_COM:    return textStd("chatPrivate");
        xcase INFO_TEAM_COM:            return textStd("chatTeam");
        xcase INFO_SUPERGROUP_COM:        return textStd("chatSuperGroup");
        xcase INFO_LEVELINGPACT_COM:    return textStd("chatLevelingpact");
        xcase INFO_ALLIANCE_OWN_COM:    return textStd("chatAlliance");
        xcase INFO_ALLIANCE_ALLY_COM:    return textStd("chatAlliance");
        xcase INFO_SHOUT_COM:            return textStd("chatBroadcast");
        xcase INFO_REQUEST_COM:            return textStd("chatRequest");
        xcase INFO_LOOKING_FOR_GROUP:    return textStd("chatLookingForGroup");
        xcase INFO_FRIEND_COM:            return textStd("chatFriend");
        xcase INFO_ADMIN_COM:            return textStd("chatAdmin");
        xcase INFO_ARENA_GLOBAL:        return textStd("chatArena");
        xcase INFO_ARCHITECT_GLOBAL:    return textStd("chatArchitect");
        xcase INFO_HELP:                return textStd("chatHelp");
        xcase INFO_ARENA:                return textStd("chatArena");
        xcase INFO_VILLAIN_SAYS:        return textStd("chatNPC");
        xcase INFO_NPC_SAYS:            return textStd("chatNPC");
        xcase INFO_PET_SAYS:            return textStd("chatNPC");
        xcase INFO_NEARBY_COM:            return textStd("chatLocal");
        xcase INFO_LEAGUE_COM:            return textStd("chatLeague");
        xcase INFO_CAPTION:                return textStd("chatCaption");
    }

    return ""; 
}


static void chatAddColorAndName(char ** str, int type, int color1, int color2, int colorhover, int colorselect, int colorselectbg, int linkselect, int linkselectbg)
{
    int setColor2 = 0;

    if(!color1 && !color2 && !colorhover && !colorselect && !colorselectbg && !linkselect && !linkselectbg)
    {
        int i;
        for(i=0; i<ARRAY_SIZE(s_text_colors); i++)
        {
            if(s_text_colors[i][0]==type)
            {
                color1 = s_text_colors[i][1];
                color2 = s_text_colors[i][2];
                colorhover = s_text_colors[i][3];
                colorselect = s_text_colors[i][4];
                colorselectbg = s_text_colors[i][5];
                linkselect = s_text_colors[i][6];
                linkselectbg = s_text_colors[i][7];
            }
        }
    }

    //if(color1 && color2 && colorhover && colorselect && colorselectbg && linkselect && linkselectbg)
    {
        int linkhover;
        linkhover = (color1|color2);

        brightenColor( &linkhover, 40);

        if( getColorLuminosity(color1|color2) > 240 ) // darken linkhover if its too bright
            linkhover = 0x66ff6600;

        color1 = (color1>>8)&0xffffff;

        // Color2 is an override of color1 for the bottom of the text. If it's
        // not set then color1 will set the whole text colour.
        if(color2)
        {
            color2 = (color2>>8)&0xffffff;
            setColor2 = 1;
        }

        colorhover = (colorhover>>8)&0xffffff;
        colorselect = (colorselect>>8)&0xffffff;
        colorselectbg = (colorselectbg>>8)&0xffffff;
        linkselect = (linkselect>>8)&0xffffff;
        linkselectbg = (linkselectbg>>8)&0xffffff;
        linkhover = (linkhover>>8)&0xffffff;

        if(setColor2)
            estrConcatf(str, "<color #%x><color2 #%x><colorhover #%x><colorselect #%x><colorselectbg #%x><link #%x><linkhover #%x><linkselect #%x><linkselectbg #%x>", color1, color2, colorhover, colorselect, colorselectbg, color1, linkhover, linkselect, linkselectbg);
        else
            estrConcatf(str, "<color #%x><colorhover #%x><colorselect #%x><colorselectbg #%x><link #%x><linkhover #%x><linkselect #%x><linkselectbg #%x>", color1, colorhover, colorselect, colorselectbg, color1, linkhover, linkselect, linkselectbg);
    }
    estrConcatCharString(str, getChannelName(type));
}

static int isPlayerChannel( int channel )
{ 
    return  (channel >= INFO_PET_COM && channel <= INFO_FRIEND_COM) || 
            (channel >= INFO_CHANNEL_TEXT && channel <= INFO_CHANNEL_TEXT4) || 
             channel == INFO_PRIVATE_COM ||channel == INFO_ALLIANCE_OWN_COM || 
             channel == INFO_ALLIANCE_ALLY_COM || channel == INFO_ARENA_GLOBAL || channel == INFO_ARCHITECT_GLOBAL ||
             channel == INFO_HELP ||    channel == INFO_EMOTE || channel == INFO_ARENA || 
             channel == INFO_PET_SAYS || channel == INFO_GMTELL || 
             channel == INFO_LEVELINGPACT_COM || channel == INFO_PRIVATE_NOREPLY_COM ||
             channel == INFO_LEAGUE_COM || channel == INFO_LOOKING_FOR_GROUP;
}



void SetLastPrivate(char * name, bool isHandle)
{
    if(isHandle)
        sprintf(gLastPrivate, "@%s", name);
    else
        strcpy(gLastPrivate, name);
}

void GetTextColorForType(int type, int rgba[4])
{
    int i;
    for(i=0;i<eaSize(&gChatChannels);i++)  
    {
        ChatChannel * cc = gChatChannels[i];
        if(cc && (cc->idx+INFO_CHANNEL_TEXT) == type )
        {
            rgba[0] = cc->color2;
            rgba[1] = cc->color;
            rgba[2] = cc->color;
            rgba[3] = cc->color2;
            return;
        }
    }

    for(i=0; i<ARRAY_SIZE(s_text_colors); i++)
    {
        if(s_text_colors[i][0]==type)
        {
            rgba[0] = s_text_colors[i][2];
            rgba[1] = s_text_colors[i][1];
            rgba[2] = s_text_colors[i][1];
            rgba[3] = s_text_colors[i][2];
        }
    }
}


void GetTextStyleForType(int type, TTFontRenderParams *pparams)
{
    int i;
    for(i=0; i<ARRAY_SIZE(s_text_styles); i++)
    {
        if(s_text_styles[i][0]==type)
        {
            pparams->italicize         = s_text_styles[i][1];
            pparams->bold              = s_text_styles[i][2];
            pparams->outline           = s_text_styles[i][3];
            pparams->dropShadow        = s_text_styles[i][4];
            pparams->softShadow        = s_text_styles[i][5];
            pparams->outlineWidth      = s_text_styles[i][6];
            pparams->dropShadowXOffset = s_text_styles[i][7];
            pparams->dropShadowYOffset = s_text_styles[i][8];
            pparams->softShadowSpread  = s_text_styles[i][9];
        }
    }
}

//---------------------------------------------------------------------------------------------------------
// Infobox
//---------------------------------------------------------------------------------------------------------
void InfoBoxDbg(INFO_BOX_MSGS msg_type, char *txt, ...){
    va_list arg;

    va_start(arg, txt);
    InfoBoxVA(msg_type, txt, 1, arg);
    va_end(arg);
}


void InfoBox(INFO_BOX_MSGS msg_type, char *text, ...)
{
    va_list arg;

    va_start(arg, text);
    InfoBoxVA(msg_type, text, 0, arg);
    va_end(arg);
}

void InfoBoxVA(INFO_BOX_MSGS msg_type, char *text, int debug, va_list arg)
{
    char *buffer = vaTranslate(text, arg);

    if(msg_type == INFO_DEBUG_INFO)
        addSystemChatMsg( buffer, msg_type, 0 );
    else if(playerPtr())
        addSystemChatMsg( buffer, msg_type, playerPtr()->svr_idx );
}

//---------------------------------------------------------------------------------------------------------|
//  Context Menu /////////////////////////////////////////////////////////////////////////////////////////|
//---------------------------------------------------------------------------------------------------------|

typedef struct ChatChannelPair
{
    int info_com;
    char *cm_text;
}ChatChannelPair;

ChatChannelPair chatChannels[] =
{
    { INFO_NEARBY_COM, "CMLocal" },
    { INFO_SHOUT_COM, "CMBroadcast" },
    { INFO_TEAM_COM, "CMTeam"},
    { INFO_LEAGUE_COM, "CMLeague" },
    { INFO_SUPERGROUP_COM, "CMSupergroup"},
    { INFO_REQUEST_COM, "CMRequest" }, 
    { INFO_FRIEND_COM, "CMFriends" },
    { INFO_ALLIANCE_OWN_COM, "CMAlliance" },
    { INFO_HELP, "CMHelpChannel" },
    { INFO_LOOKING_FOR_GROUP, "CMLookingForGroup" },
    { INFO_TAB, "CMActiveTab" },
    { 0, NULL }    //    Terminator pair
};

void chat_exit() { enterChatMode(0); }

static int chatcm_isChannel(void *chan)
{
    Entity *e = playerPtr();
    int *channel = (int*)chan;

    if(!e) return CM_AVAILABLE;

    if( e->pl->chatSendChannel == *channel )
        return CM_CHECKED;
    else
        return CM_AVAILABLE;
}

static void chatcm_setChannel(void *chan)
{
    Entity * e = playerPtr();
    int *channel = (int*)chan;

    if(!e) return;

    e->pl->chatSendChannel = *channel;
    sendChatChannel( e->pl->chatSendChannel, "" );
}

static void chatcm_setUserChannel(void * data)
{
    int idx = ((int)data)-1;

    Entity * e = playerPtr();

    if(!e) return;

    assert(idx < eaSize(&gChatChannels));

    e->pl->chatSendChannel = INFO_CHANNEL;
     sendChatChannel( e->pl->chatSendChannel, gChatChannels[idx]->name );
}


static void chatcm_leaveChannel(void * data)
{
    int idx = ((int)data)-1;
    
    Entity * e = playerPtr();

    if(!e) return;

    assert(idx < eaSize(&gChatChannels) && idx >= 0);

    leaveChatChannel(gChatChannels[idx]->name);
}

static void openFriendsMenu(void *foo) { window_setMode( WDW_FRIENDS, WINDOW_GROWING ); }

//---------------------------------------------------------------------------------------------------------|
//  Links /////////////////////////////////////////////////////////////////////////////////////////////////|
//---------------------------------------------------------------------------------------------------------|

void chatLinkcm_sendTell( char *pchName )
{
    char buf[CHAT_TEXT_LEN];
    if(!enterChatMode(1))
        return;
    setChatEditText(NULL);
    sprintf( buf, "/t %s, ", pchName );
    setChatEditText(buf);
}

void chatLinkcm_sendTellToTeamLeader( char *pchName )
{
    char buf[CHAT_TEXT_LEN];
    if(!enterChatMode(1))
        return;
    setChatEditText(NULL);
    sprintf( buf, "/ttl %s, ", pchName );
    setChatEditText(buf);
}
void chatLinkcm_sendTellToLeagueLeader( char *pchName )
{
    char buf[CHAT_TEXT_LEN];
    if(!enterChatMode(1))
        return;
    setChatEditText(NULL);
    sprintf( buf, "/tll %s, ", pchName );
    setChatEditText(buf);
}
static void chatLinkcm_inviteTeam( char *pchName ) { cmdParsef( "i %s", pchName); }
static void chatLinkcm_inviteLeague( char *pchName ) { cmdParsef( "li %s", pchName); }
static void chatLinkcm_Friend( char *pchName ) { cmdParsef( "friend %s", pchName); }
static void chatLinkcm_Note( char * pchName ) { playerNote_GetWindow( pchName, 0 ); }
static void chatLinkcm_ignore( char *pchName ) { cmdParsef( "ignore %s", pchName); }
static void chatLinkcm_ignoreSpammer( char *pchName ) { cmdParsef( "ignore_spammer %s", pchName); }
static void chatLinkcm_getGlobalName( char *pchName ) { cmdParsef( "get_global_name %s", pchName); }

static char* chatLinkcm_sendTellName( char *pchName ) { return textStd( "CMchatlinkSendTell", pchName ); }
static char* chatLinkcm_sendTellToTeamLeaderName( char *pchName ) { return textStd( "CMchatlinkSendTellToTeamLeader", pchName ); }
static char* chatLinkcm_sendTellToLeagueLeaderName( char *pchName ) { return textStd( "CMchatlinkSendTellToLeagueLeader", pchName ); }
static char* chatLinkcm_inviteTeamName( char *pchName ) { return textStd( "CMInviteTeamString" ); }
static char* chatLinkcm_inviteLeagueName( char *pchName ) { return textStd( "CMInviteLeagueString" ); }
static char* chatLinkcm_FriendName( char *pchName ) { return textStd( "CMAddFriendString" ); }
static char* chatLinkcm_NoteName( char *pchName ) { return (playerNote_Get(playerNote_getGlobalName(pchName),0)? textStd( "CMEditNoteString" ):textStd( "CMAddNoteString" )); }
static char* chatLinkcm_ignoreName( char *pchName ) { return textStd( "CMchatlinkIgnore", pchName ); }
static char* chatLinkcm_ignoreSpammerName( char *pchName ) { return textStd( "CMchatlinkIgnoreSpammer", pchName ); }



void chatLink( char *pchName )
{
    int x, y;
    inpMousePos( &x, &y);
    contextMenu_set( chatLinkContext, x, y, pchName );
}


static void chatLinkGlobalcm_sendTell( char * pchName )
{
    char buf[CHAT_TEXT_LEN];
    if(!enterChatMode(1))
        return;
    setChatEditText(NULL);
    sprintf( buf, "/t @%s, ", pchName );
    setChatEditText(buf);
}
static void chatLinkGlobalcm_Friend( char * pchName ){ cmdParsef( "gfriend %s", pchName); }
static void chatLinkGlobalcm_ignore( char * pchName ){ cmdParsef( "ignore @%s", pchName); }
static void chatLinkGlobalcm_Note( char * pchName ) { playerNote_GetWindow( pchName, 1 ); }
static char* chatLinkGlobalcm_NoteName( char *pchName ) { return (playerNote_Get(pchName,0)?textStd( "CMEditNoteString" ):textStd( "CMAddNoteString" )); }
static void chatLinkGlobalcm_inviteTeam( char * pchName ){ cmdParsef( "get_local_invite %s", pchName); }
static void chatLinkGlobalcm_inviteLeague( char * pchName ){ cmdParsef( "get_local_league_invite %s", pchName); }
static void chatLinkGlobalcm_getLocalName( char *pchName ) { cmdParsef( "get_local_name %s", pchName); }
static void chatLinkGlobalcm_ignoreSpammer( char *pchName ) { cmdParsef( "ignore_spammer  @%s", pchName); }
static char* chatLinkGlobalcm_ignoreSpammerName( char *pchName ) { return textStd( "CMchatlinkIgnoreSpammer", pchName ); }

static ChatChannel *s_pContextChannel;
static int chatLinkGlobal_CanMod( void * foo )
{
    if( !s_pContextChannel )
        return CM_HIDE;
    if( isOperator(s_pContextChannel->me->flags) )
        return CM_AVAILABLE;
    return CM_HIDE;
}

static char * chatLinkcm_SilenceText( char * pchName )
{
    ChatUser * user;
    if( !s_pContextChannel )
        return textStd("CMSilence");
    user = GetChannelUser( s_pContextChannel, pchName );
    if( isSilenced(user) )
        return textStd("CMUnSilence");
    else
        return textStd("CMSilence");
}

static void chatLinkGlobal_Silence( char * pchName )
{ 
    if(s_pContextChannel)
    {
        ChatUser *user = GetChannelUser( s_pContextChannel, pchName );
        cmdParsef("chan_usermode \"%s\" \"%s\" \"%s\"", s_pContextChannel->name, pchName, isSilenced(user)?"+send":"-send" ); 
    }
}

static void chatLinkGlobal_Op( char * pchName ){ if(s_pContextChannel)cmdParsef("chan_usermode \"%s\" \"%s\" \"%s\"", s_pContextChannel->name, pchName, "+operator" ); }
static void chatLinkGlobal_Unsilence( char * pchName ){ if(s_pContextChannel)cmdParsef("chan_usermode \"%s\" \"%s\" \"%s\"", s_pContextChannel->name, pchName, "+operator" ); }
static void chatLinkGlobal_Kick( char * pchName ){ if(s_pContextChannel)cmdParsef("chan_usermode \"%s\" \"%s\" \"%s\"", s_pContextChannel->name, pchName, "-join" ); }

void chatLinkGlobal( char *pchName, char * pchChannel )
{
    int x, y;
    inpMousePos( &x, &y);
    s_pContextChannel = GetChannel(pchChannel);
    contextMenu_set( chatLinkGlobalContext, x, y, pchName );
}

static int isChannelOperator( char * pchChannel )
{
    ChatChannel * cc = GetChannel( pchChannel );

    if( cc && isOperator(cc->me->flags) )
        return CM_AVAILABLE;
    else
        return CM_HIDE;
}

static void chatLinkChannelcm_send( char * pchChannel )
{
    char buf[CHAT_TEXT_LEN];
    if(!enterChatMode(1))
        return;
    setChatEditText(NULL);
    sprintf( buf, "/send \"%s\" ", pchChannel );
    setChatEditText(buf);
}

static void chatLinkChannelcm_setMotd( char * pchChannel )
{
    char buf[CHAT_TEXT_LEN];
    if(!enterChatMode(1))
        return;
    setChatEditText(NULL);
    sprintf( buf, "/chan_motd \"%s\" ", pchChannel );
    setChatEditText(buf);
}

static void chatLinkChannelcm_setDescription( char * pchChannel )
{
    char buf[CHAT_TEXT_LEN];
    if(!enterChatMode(1))
        return;
    setChatEditText(NULL);
    sprintf( buf, "/chan_desc \"%s\" ", pchChannel );
    setChatEditText(buf);
}

static void chatLinkChannelcm_members( char * pchChannel ) { cmdParsef( "chan_members %s", pchChannel); }
static void chatLinkChannelcm_leave( char * pchChannel ) { cmdParsef( "chan_leave %s", pchChannel); }

void chatLinkChannel( char *pchName )
{
    int x, y;
    inpMousePos( &x, &y);
    contextMenu_set( chatLinkChannelContext, x, y, pchName );
}
//---------------------------------------------------------------------------------------------------------|
//  Chat //////////////////////////////////////////////////////////////////////////////////////////////////|
//---------------------------------------------------------------------------------------------------------|

// must be called during startup before networking is enabled
void initChatUIOnce()
{
    int i;

    eaCreate(&gChatChannels);

    for(i=0; i<MAX_CHAT_WINDOWS; i++)
        ChatWindowInit(i);

    SelectActiveChatFilter(0);
    gbSpecialQuitToLoginCase = FALSE;

    initChatEdit();
}

//***********************************************

void chat_log( char * txt )
{
#if 0
    // This unused code is similar to the original code, but keeps the file open
    // instead of closing it after each write.
    // The original problem was a performance issue when many chat messages were
    // getting logged.
    static FILE * file = 0;
    static char last_filename[MAX_PATH] = "";

    char filename[MAX_PATH];
    SYSTEMTIME    sys_time;

    if( !optionGet(kUO_LogChat) )
        return;

    GetLocalTime(&sys_time);
    
    // Make the file name.
    sprintf(filename, "%schatlog %02d-%02d-%04d.txt", getLogDir(), sys_time.wMonth, sys_time.wDay, sys_time.wYear);

    // Only open the file if not already opened or the date
    // string changed
    if (!file || !file->fptr || strcmp(filename, last_filename) != 0)
    {
        if( file )
        {
            fclose( file );
            file = 0;
        }

        file = fopen(filename, "a+t" );

        if( !file )
        {
            // make sure the directory exists and try again
            makeDirectoriesForFile(filename);
            file = fopen(filename, "a+t" );
        }

        if ( !file )
        {
            optionSet(kUO_LogChat,0,0);
            addSystemChatMsg( textStd("CouldNotOpenChatLog"), INFO_USER_ERROR, 0 );
            return;
        }

        strcpy(last_filename, filename);
    }

    // Add a date/time stamp to the text
    if (fprintf(file, "%02d-%02d-%04d %02d:%02d:%02d %s\n", sys_time.wMonth, sys_time.wDay, sys_time.wYear,
                                                            sys_time.wHour, sys_time.wMinute, sys_time.wSecond,
                                                            txt) <= 0)
    {
        optionSet(kUO_LogChat,0,0);
        addSystemChatMsg( textStd("ChatLogHardDiskFull"), INFO_USER_ERROR, 0 );
        return;
    }
#else
    // This code uses the log functionality in CoH\libs\UtilitiesLib\utils\log.c
    // to run do the log file writes asynchronously in a separate logging thread
    // with caching and log file rotating and occasional flushing.
    // Much more robust, but we don't know if the write fails.
    // The log functionality also forces a '.log' extension on the log file.
    char filename[MAX_PATH];
    static char *buf = NULL;
    SYSTEMTIME    sys_time;

    if( !optionGet(kUO_LogChat) )
        return;

    GetLocalTime(&sys_time);

    // Make the file name.  logPutMsg() will add a '.log' to the end
    sprintf(filename, "%schatlog %02d-%02d-%04d", getLogDir(), sys_time.wMonth, sys_time.wDay, sys_time.wYear);

    // Add a date/time stamp to the text
    estrPrintf(&buf, "%02d-%02d-%04d %02d:%02d:%02d %s\n",
               sys_time.wMonth, sys_time.wDay, sys_time.wYear,
               sys_time.wHour, sys_time.wMinute, sys_time.wSecond,
               txt);

    logPutMsgTxt(buf, filename);
#endif
}


void addChatMsgDirect(char * s, int type)
{
    int i;
    int size = eaSize(&gChatFilters);
    for(i=0;i<size;i++)
    {
        ChatFilter * filter = gChatFilters[i];
        if( type == INFO_ADMIN_COM || BitFieldGet(filter->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, type) ||
            (type == INFO_PRIVATE_NOREPLY_COM && BitFieldGet(filter->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_PRIVATE_COM)))
            addChatMsgToQueue(s, type, filter);
    }
}


static void addTextWithLinks( char **estr, char * pchIn )
{
    char * beginToken=0;
    char * endToken=0;
    char * start, *s;
    char buf[4096], token[4096];

    start = s = dealWithPercents(pchIn);

    while(*s)
    {
        if(*s=='[')
            beginToken = s;
        if(beginToken && *s==']' )
        {
            strncpyt(token, beginToken+1, MIN(4096, s-beginToken)); // subtract the brackets off
            if( isValidLinkToken( token ) )
            {
                strncpyt(buf, start, MIN(4096, beginToken-start+1));
                estrConcatf(estr, "%s<a href='cmd:link_info %s'><b>[%s]</b></a>", buf, token, token );
                start = s+1;
            }
            beginToken = 0;
        }
        s++;
    }

    // copy leftovers
    strncpyt(buf, start, MIN(4096, s-start+1));
    estrConcatCharString(estr, buf);
}



static void chatPrepareSystem( char * pchIn, char **estr, int type, int id )
{
    if( isPlayerChannel(type) )
    {
        char *str = NULL;
        char *name = NULL, *after = NULL, *arrow = NULL, *SGName = NULL;
        int motd = 0;

        estrConcatCharString(&str, stripPlayerHTML(pchIn, 1));
        smf_EncodeAllCharactersInPlace(&str);

        //int vnr;
         chatAddColorAndName(estr, type, 0, 0, 0, 0, 0, 0, 0);

        // special hack for supergorup MoTD
        if( type == INFO_SUPERGROUP_COM )
        {
            char *p1 = strstri( str, "--" );
            char *p2 = strchr( str, ':' );
            if (!p2 || (p1 && p1 <= p2))
            {
                motd = 1;
            }
        }
        
        if (type != INFO_EMOTE && !motd)
        {
            name = strtok(str, ":");
            after = strtok(NULL, "");
            if(name)
                arrow = strstri(name, "--&gt;");
        }

        // Special hack for coalition chat
         if (type == INFO_ALLIANCE_ALLY_COM  )
        {
            char * SGNameExists = name?strstri(name, ")"):NULL;
            if( SGNameExists )
            {
                SGName = strstri(name, "(");
                if( SGName && *(SGName+1) )
                    name = SGNameExists + 1;
            }
        }



        if( name && after )// && (vnr == VNR_Valid || vnr == VNR_Reserved || vnr ==  VNR_Titled) )
        {
            Entity *e = playerPtr();

            if(optionGet(kUO_StaticColorsPerName))
            {
                int color = hashString((arrow?(name+6):name), 0);
                while( getColorLuminosity(color) < 127 )
                    brightenColor(&color, 5);
                estrConcatf(estr, "<link #%x><color2 #%x>", (color>>8)&0xffffff, (color>>8)&0xffffff );
            }

            if( (type == INFO_PRIVATE_COM || type == INFO_PRIVATE_NOREPLY_COM) && e && e->pl && e->pl->hidden&(1<<HIDE_TELL) )
            {
                estrConcatCharString(estr, textStd("HiddenTellAddition"));
            }

            if(arrow) 
            {
                name+=6;
                if( *name == '@' )
                    estrConcatf(estr, "--><a href='cmd:link_interact_global \"%s\" 0'>%s</a>: ", name+1, name);
                else
                    estrConcatf(estr, "--><a href='cmd:link_interact %s'>%s</a>: ", name, name);
            }
            else
            {
                if( type == INFO_PRIVATE_COM || type == INFO_GMTELL )    // no INFO_PRIVATE_NOREPLY_COM because we don't want you to be able to reply to this.
                    SetLastPrivate(name, 0);

                if (SGName)
                {
                    if( *name == '@' )
                        estrConcatf(estr, "(%s) <a href='cmd:link_interact_global \"%s\" 0'>%s</a>: ", SGName, name+1, name);
                    else
                        estrConcatf(estr, "(%s) <a href='cmd:link_interact %s'>%s</a>: ", SGName, name, name);
                } else {
                    if( *name == '@' )
                        estrConcatf(estr, "<a href='cmd:link_interact_global \"%s\" 0'>%s</a>: ", name+1, name);
                    else
                        estrConcatf(estr, "<a href='cmd:link_interact %s'>%s</a>: ", name, name);
                }
            }

            if(optionGet(kUO_StaticColorsPerName))
                estrConcatCharString(estr, "</link></color2>");

            if( type == INFO_PRIVATE_COM || type == INFO_GMTELL || type == INFO_PRIVATE_NOREPLY_COM)
            {
                if( *name == '@' )
                    playerNote_LogPrivate( name+1, after, 0, 1, (int)arrow );
                else
                      playerNote_LogPrivate( name, after, 0, 0, (int)arrow );
            }

            addTextWithLinks(estr,after);
        }
        else if (type == INFO_EMOTE || motd )
            addTextWithLinks(estr,str); 
        else
            addTextWithLinks(estr,pchIn); //this should never happen

        estrDestroy(&str);
    }
    else
    {
        chatAddColorAndName(estr, type, 0, 0, 0, 0, 0, 0, 0);
        addTextWithLinks(estr,pchIn);
    }
}

void addGlobalChatMsg( char *pchChannel, char *pchPlayer, char *pchText, INFO_BOX_MSGS type )
{
    ChatChannel * cc = GetChannel(pchChannel);
    char *str = NULL;
    char *eStrMsg = NULL;

    if(amOnArenaMap() || !cc) 
        return;

    estrConcatf(&str, "[%s]%s: %s", pchChannel, pchPlayer, pchText );
    chat_log(str);
    estrClear(&str);
    
    if(type < 0)
    {
        type = INFO_CHANNEL_TEXT+cc->idx;
        chatAddColorAndName(&str, type, cc->color, cc->color2, cc->color, cc->color, 0, cc->color, 0);
    }
    else
        chatAddColorAndName(&str, type, 0, 0, 0, 0, 0, 0, 0);
    
    // add title
    estrConcatf(&str, "[<a href='cmd:link_channel %s'>%s</a>]", pchChannel,  pchChannel );

    // add name

    if( pchPlayer )
    {
        if(optionGet(kUO_StaticColorsPerName))
        {
            int color = hashString(pchPlayer, 0);
            while( getColorLuminosity(color) < 127 )
                brightenColor(&color, 5);
            estrConcatf(&str, "<link #%x><color2 #%x>", (color>>8)&0xffffff, (color>>8)&0xffffff );
        }
        estrConcatf(&str, "<a href='cmd:link_interact_global \"%s\" %s'>%s</a>: ", pchPlayer, pchChannel, pchPlayer );
        if(optionGet(kUO_StaticColorsPerName))
            estrConcatStaticCharArray(&str, "</link></color2>");
    }

    estrConcatCharString(&eStrMsg, stripPlayerHTML(pchText, 1));
    smf_EncodeAllCharactersInPlace(&eStrMsg);

    // add message
    addTextWithLinks(&str,eStrMsg); 

    // see if any filters are receiving text from this channel
    if(eaSize(&cc->filters))
    {
        int i;
        for(i=0;i<eaSize(&cc->filters);i++)
        {
            if(cc->filters[i])
                addChatMsgToQueue(str, type, cc->filters[i]);
        }
    }
    estrDestroy(&str);
    estrDestroy(&eStrMsg);
}

void addSystemChatMsg(char *s, int type, int id)
{
    addSystemChatMsgEx(s, type, DEFAULT_BUBBLE_DURATION, id, 0);
}

static int isChatHidden(void)
{
    int retval = 0;

    if(shell_menu() || baseedit_Mode())
        retval = 1;
    else if((window_getMode(WDW_CHAT_BOX) != WINDOW_DISPLAYING || GetChatWindow(0)->minimizedOffset) &&
        window_getMode(WDW_CHAT_CHILD_1) != WINDOW_DISPLAYING &&
        window_getMode(WDW_CHAT_CHILD_2) != WINDOW_DISPLAYING &&
        window_getMode(WDW_CHAT_CHILD_3) != WINDOW_DISPLAYING &&
        window_getMode(WDW_CHAT_CHILD_4) != WINDOW_DISPLAYING)
    {
        retval = 1;
    }

    return retval;
}

void addSystemChatMsgEx(char *s, int type, float duration, int id, Vec2 position)
{
    Entity *e = playerPtr();
    Entity *eSrc;
    char *str;

      if(type==INFO_VILLAIN_SAYS || type==INFO_NPC_SAYS || type==INFO_PET_SAYS || type==INFO_PET_COM )
        eSrc = entFromId(id);
    else 
        eSrc = entFromDbId(id);

    if(eSrc && ENTTYPE(eSrc))
        id = eSrc->svr_idx; 

    switch( type )
    {
        case INFO_PRIVATE_NOREPLY_COM:
        case INFO_GMTELL:
        case INFO_PRIVATE_COM:
            sndPlay( "type", SOUND_GAME );
            break;
        case INFO_TEAM_COM:
            sndPlay( "m2", SOUND_GAME );
            break;
        case INFO_LEAGUE_COM:
            sndPlay( "LeagueChat", SOUND_GAME );
            break;
        case INFO_PET_SAYS:
            {
                 if(optionGet(kUO_ChatDisablePetSay))
                    return;
                 if(!eSrc || !(optionGet(kUO_ChatEnablePetTeamSay) || entIsPet( eSrc )) )
                    return;
            }break;
        case INFO_PAYMENT_REMAIN_TIME:
            {
                int curr_avail, total_avail, reservation, countdown, change_billing;
                // Special case, parse and let others handle it
                sscanf(s,"%u %u %u %u %u",&curr_avail, &total_avail, &reservation, &countdown, &change_billing);
                setPaymentRemainTime(curr_avail,total_avail,reservation,countdown,change_billing);
                return;
            } break;
        case INFO_DEBUG_INFO:
            {
                estrCreate(&str);
                chatAddColorAndName(&str, type, 0, 0, 0, 0, 0, 0, 0);
                estrConcatCharString(&str, s);
                addChatMsgDirect(str, type);    // add to all matching filters
                chat_log(str);
                estrDestroy(&str);
                return;
            }
        default: break;
    }

    estrCreate(&str);
    estrConcatCharString(&str, s);

    if(!optionGet(kUO_AllowProfanity))
        ReplaceAnyWordProfane(str);

    if (entity_TargetIsInVisionPhase(e, eSrc))
    {
        if( (playerPtr() && id && type != INFO_ALLIANCE_OWN_COM && type != INFO_ALLIANCE_ALLY_COM) &&
            (isPlayerChannel(type) ||  
            type==INFO_NPC_SAYS||
            type==INFO_PET_SAYS||
            type==INFO_VILLAIN_SAYS||
            type==INFO_PET_COM)  ) // probably need more conditionals here
            addChatMsgsFloat(str, type, duration, id, 0);

        if (type==INFO_CAPTION)
            addChatMsgsFloat(str, type, duration, id, position);
    }

    estrClear( &str );

    estrConcatf(&str, "%s%s", getChannelName(type), s );
    chat_log( str );
    estrClear( &str );

    // Italic hack for emote channel
    if( type == INFO_EMOTE )
        estrConcatStaticCharArray(&str, "<i>");
    chatPrepareSystem(s, &str, type, id );
    if( type == INFO_EMOTE )
        estrConcatStaticCharArray(&str, "</i>");

    if (!e && (!gChatFilters || eaSize(&gChatFilters) == 0))
    {
        addChatMsgToQueue(str, type, 0); // Fallback
    }
    else
    {
        if( type == INFO_ARENA_ERROR && window_getMode(WDW_ARENA_CREATE) != WINDOW_DISPLAYING )
            type = INFO_USER_ERROR;

        if( type == INFO_GMTELL )
            addChatMsgToQueue(str, type, 0);    // add to active filter
        else if ( type == INFO_ARENA || type == INFO_ARENA_ERROR )
            addArenaChatMsg( str, type );
        else
            addChatMsgDirect(str, type);    // add to all matching filters

        if( e && e->pl && type != INFO_PRIVATE_NOREPLY_COM)
        {
            // auto-reply if afk
            if( gLastPrivate && gLastPrivate[0] && (type == INFO_PRIVATE_COM||type == INFO_GMTELL) && !strstri( str, "-->" ) )
            {
                if(entIsTrial(e))
                {
                    char *message = estrTemp();
                    estrConcatf(&message, "tellTrialMessage \"%s\"", gLastPrivate);
                    cmdParse( message );    
                    estrDestroy(&message);
                }
                // auto-reply if afk
                else if(e->pl->afk)
                {
                    cmdParse( textStd( "AutoReplyString", gLastPrivate, e->pl->afk_string?e->pl->afk_string:"" ) );
                }
            }
            // Can't safely parse the incoming message to look for replies
            // because (a) they might not have the --> and (b) what text they
            // do have gets localised.
#if 0
            else if( (type == INFO_PRIVATE_COM && !strstri( str, "-->" )) && isChatHidden() )
            {
                // auto-reply if chat window is not visible
                cmdParse( textStd( "AutoReplyChatHidden", gLastPrivate, e->pl->afk_string ) );
            }
#endif
        }
    }

    estrDestroy(&str);
}


void addChatMsgsFloat(char *txt, INFO_BOX_MSGS msg, float duration, int svr_idx, Vec2 position)
{
    ParseChatText(txt, msg, duration, svr_idx, position);
     demoSetEntIdx(svr_idx);
     demoRecord("Chat %d %d \"%s\"",msg,kDemoChatType_Normal,txt);    // MJP force to system channel for now
}


void clearChatMsgs()
{
    int i;
    for(i=0; i<MAX_CHAT_WINDOWS; i++)
    {
        ChatWindow * window = GetChatWindow(i);

        ChatFilter * filter = uiTabControlGetFirst(window->topTabControl);
        while(filter)
        {
            ChatFilterClear(filter);
            filter = uiTabControlGetNext(window->topTabControl);
        }

        filter = uiTabControlGetFirst(window->botTabControl);
        while(filter)
        {
            ChatFilterClear(filter);
            filter = uiTabControlGetNext(window->botTabControl);
        }        
    }
}

void chatCleanup()
{
    int i;

    chatTabCleanup();
    clearChatMsgs();

    // now, delete all filters, channels, etc
    for(i=0;i<MAX_CHAT_WINDOWS;i++)
        ChatWindowRemoveAllFilters(GetChatWindow(i));    

    eaDestroyEx(&gChatFilters, ChatFilterDestroy);
    eaDestroyEx(&gChatChannels, ChatChannelDestroy);

    gUserSendChannel[0] = 0;
    gbSpecialQuitToLoginCase = TRUE;
}


void addChatMsgToQueue( char *txt, INFO_BOX_MSGS msg, ChatFilter * filter)
{
    chatQueue * chatQ;
    ChatLine * pLine;
    char *scaleptr;
    char *scaleend = NULL;
    char *slashscaleptr = NULL;
    
    if(!filter)
    {
        // use default active filter
        if(gActiveChatFilter)
            filter = gActiveChatFilter;
        else
            return;
    }

    pLine = ChatLine_Create();
    pLine->pchText = strdup(txt);
    
    // As long as we can find either a <scale ... > or a </scale> sequence in the current text buffer
    while (((scaleptr = strstr(pLine->pchText, "<scale")) != NULL && (scaleend = strstr(scaleptr, ">")) != NULL) || (slashscaleptr = strstr(pLine->pchText, "</scale>")) != NULL)
    {
        // Test both of these for non-NULL.
        if (scaleptr && scaleend)
        {
            // Point to the following character
            scaleend++;
            
            // Use memmove since it is guaranteed to get overlapping copies right.  strcpy makes no such promises.
            memmove(scaleptr, scaleend, strlen(scaleend) + 1);
        }
        // This test is probably not needed.  The only way we can arrive here and actually do the test is if scaleptr or scaleend were NULL.  In that instance if slashscaleptr
        // is NULL as well, we would have dropps out of the while loop.
        else if (slashscaleptr)
        {
            // Find the end of the sequence
            scaleend = &slashscaleptr[8 /* == strlen("</scale>") */];
            
            // And memmove.  Remember what I said about strcpy and overlapping copies ...
            memmove(slashscaleptr, scaleend, strlen(scaleend) + 1);
        }
    }

    chatQ = &filter->chatQ;
    if( chatQ->size >= MAX_CHAT_LINES )
    {
        ChatLine_Destroy( eaRemove( &chatQ->ppLines, 0 ) );
    }

    eaPush( &chatQ->ppLines, pLine );
    chatQ->size = eaSize( &chatQ->ppLines );

    filter->hasNewMsg = TRUE;
}

//---------------------------------------------------------------------------------------------------------/|/
#define TEXT_XOFF           10
#define TEXT_YOFF           8
#define TEXT_Y_STEP         12

SMFBlock* chatEdit;
bool chatEditReparse;
bool chatEditShouldSendInput;
EditTextBlock           chat_text;
char                    actual_chat_text[ CHAT_TEXT_LEN ];

int chat_mode() {return smfBlock_HasFocus(chatEdit); }

static int chatOnLostFocus(void* owner, void* requester)
{
    chat_text.selected = FALSE;
    if(chatEdit)
    {
        smf_OnLostFocus(chatEdit, requester);
    }
    return 1;
}

static int enterChatMode(int mode)
{
    int result;

    initChatEdit();
    if (!mode)
    {
        result = 0;
        smf_OnLostFocus(chatEdit, NULL);
    }
    else
    {
        result = 1;
        if (!uiInFocus(chatEdit))
        {
            smf_SelectAllText(chatEdit);
        }
        chat_text.selected = result;
    }

    return result;
}

void chatEditInputHandler()
{
    KeyInput* input;

    // Handle keyboard input.
    input = inpGetKeyBuf();
    if(input && s_bGobbleFirst)
    {
        s_bGobbleFirst = false;
        inpGetNextKey(&input);
    }

     while(input )
    {
        if(input->type == KIT_EditKey)
        {
            switch(input->key)
            {
            case INP_ESCAPE:
                if( strlen(chatEdit->outputString) > 0 )
                {
                     smf_SetRawText(chatEdit, "", false);
                    smf_SetCursor(chatEdit, 0);
                    // HACK HACK BAD HACK to turn off SMF's input handling!
                    // Need to integrate this into a better input handling system.
                    smfSearch_currentCommand = SMFSearchCommand_None;
                }
                break;
            case INP_TILDE:
                enterChatMode(0);
                break;
            case INP_UP:
                chat_History(1);
                break;
            case INP_DOWN:
                chat_History(0);
                break;
            case INP_NUMPADENTER: /* fall through */
            case INP_RETURN:
                chatEditShouldSendInput = true;
                break;
            default:
                //uiFormattedEdit_DefaultKeyboardHandler(edit, input);
                break;
            }
        }
        else
        {
            //uiFormattedEdit_DefaultKeyboardHandler(edit, input);
        }
        inpGetNextKey(&input);
    }
}

void chatEditSendInput()
{
    if(ignore_first_enter)
    {
        ignore_first_enter = FALSE;
        return;
    }
    else
    {
        char* chatText = chatEdit->outputString;
        if(chatText && estrLength(&chatText))
        {
            chat_addHistory(chatText);

            // if the first line was a slash, attempt to parse as a command
            if(chatText[0] == '/' )
            {
                char* cmd = chatText+1;
                cmdParse(cmd);
            }
            else if (chatText[0] == ';')
            {
                char* cmd;
                estrInsert(&chatText, 1, "e ", 2);
                cmd = chatText+1;
                cmdParse(cmd);
            }
            else
            {
                uiChatSendToCurrentChannel(chatText);
            }

            smf_SetRawText(chatEdit, "", false);
            smf_OnLostFocus(chatEdit, NULL);
        }
    }
    enterChatMode(0);
}

void initChatEdit(void)
{
     if(!chatEdit)
    {
        chatEdit = smfBlock_Create();
        smf_SetFlags(chatEdit, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine,
            SMFInputMode_AnyTextNoTagsOrCodes, 255, SMFScrollMode_InternalScrolling,
            SMFOutputMode_StripFlaggedTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None,
            SMAlignment_Left, 0, 0, 0);
        smf_SetCharacterInsertSounds(chatEdit, "type2", "mouseover");
        s_fontSize = optionGet(kUO_ChatFontSize);
    }
}

void setChatEditText(char *pch)
{
    int pos = 0;

    initChatEdit();
    smf_SetRawText(chatEdit, pch, false);

    if(pch)
        pos = strlen(pch);

    smf_SetCursor(chatEdit, pos);
}

void addChatEditText( const char * pch )
{
    char * str, *edit;

    if(!enterChatMode(1))
        return;

    estrCreate(&str);
    initChatEdit();

    edit = chatEdit->outputString;
    if(edit)
        estrConcatCharString(&str, edit);
    if(pch)
        estrConcatCharString(&str, pch);
    smf_SetRawText(chatEdit, str, false);
    smf_SetCursor(chatEdit, strlen(str));

    estrDestroy(&str);
}


// unless specified, prefer using player names over chat handles
void chatInitiateTell(void * data)
{
    int preferHandle = (int)data;

    char buf[CHAT_TEXT_LEN];
    char * handle  = targetGetHandle();

    if(!enterChatMode(1))
        return;
    setChatEditText(NULL);

    if(preferHandle && handle)
        sprintf( buf, "/t @%s, ", handle );
    else if( current_target )
        sprintf( buf, "/t %s, ", current_target->name );
    else if ( gSelectedDBID )
        sprintf( buf, "/t %s, ", gSelectedName );
    else if ( handle )
        sprintf( buf, "/t @%s, ", handle );
    else
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
        return;
    }

    setChatEditText(buf);
}

void chatInitiateTellToTeamLeader(void *data)
{

    char buf[CHAT_TEXT_LEN];

    if(!enterChatMode(1))
        return;
    setChatEditText(NULL);

    if( current_target )
        sprintf( buf, "/ttl %s, ", current_target->name );
    else if ( gSelectedDBID )
        sprintf( buf, "/ttl %s, ", gSelectedName );
    else
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
        return;
    }

    setChatEditText(buf);
}

void chatInitiateTellToLeagueLeader(void *data)
{

    char buf[CHAT_TEXT_LEN];

    if(!enterChatMode(1))
        return;
    setChatEditText(NULL);

    if( current_target )
        sprintf( buf, "/tll %s, ", current_target->name );
    else if ( gSelectedDBID )
        sprintf( buf, "/tll %s, ", gSelectedName );
    else
    {
        addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
        return;
    }

    setChatEditText(buf);
}

void chatInitiateReply()
{
    char buf[CHAT_TEXT_LEN];

    s_bGobbleFirst = true;
    if(!enterChatMode(1))
        return;
    setChatEditText(NULL);

    if(gLastPrivate[0])
        sprintf( buf, "/t %s, ", gLastPrivate );
    else
        return;

    setChatEditText(buf);
}

#define MAX_CHAT_HIST   10

static char ** chatHistory = 0;
static int historyPosition = 0;

void chat_addHistory( char * txt )
{
    char * str = malloc( sizeof(char)*(strlen(txt)+1) );
    strcpy( str, txt );

    if( !chatHistory )
        eaCreate( &chatHistory );

    eaInsert( &chatHistory, str, 0 );

    if( eaSize(&chatHistory) >= MAX_CHAT_HIST )
    {
        char * freeMe = eaPop(&chatHistory);
        if( freeMe )
            free( freeMe );
    }

    historyPosition = -1;
}

void chat_History( int up )
{
    if(!chatHistory)
        return;

    if(up)
    {
        historyPosition++;
        if(historyPosition >= eaSize(&chatHistory))
            historyPosition = 0;
    }
    else
    {
        historyPosition--;

        if(historyPosition < 0)
            historyPosition = eaSize(&chatHistory)-1;
        if(historyPosition < 0)
            historyPosition = 0;
    }

    if(chatHistory[historyPosition])
    {
        smf_SetRawText(chatEdit, chatHistory[historyPosition], true);
        smf_SetCursor(chatEdit, strlen(chatHistory[historyPosition]));
    }
}


void uiChatSendToUserChannel(char * pch, ChatChannel * channel)
{
    char buf[2000];
    sprintf(buf, "send \"%s\" %s", channel->name, pch);
    cmdParse(buf);
}


static void createFilterDialogHandler(void *data)
{
    addFilter(GetChatWindow(0));    // add a channel to the primary chat window
}

//
//
void uiChatSendToCurrentChannel(char *pch)
{
    int channel = playerPtr()->pl->chatSendChannel;
     char buffer[CHAT_TEXT_LEN + 32];

    // handle active "tab" mode --- will either send to user channel or system channel
      if(channel == INFO_TAB)
    {
        if(gActiveChatFilter)
        {
            if(gActiveChatFilter->defaultChannel.type == ChannelType_System)
                channel = gActiveChatFilter->defaultChannel.system;    // will be processed below
            else 
            {
                 if(gActiveChatFilter->defaultChannel.type == ChannelType_User)
                {
                    assert(gActiveChatFilter->defaultChannel.user);
                    uiChatSendToUserChannel(pch, gActiveChatFilter->defaultChannel.user);
                }
                else
                    addSystemChatMsg( textStd("NoDefaultChannelToSendInput", gActiveChatFilter->name), INFO_USER_ERROR, 0 );
                    
                return;    // non-system default channels needn't go any further
            }
        }
        else
        {
            // tab mode, but no active tab or window set
             if(eaSize(&gChatFilters) == 0)
                dialogStd(DIALOG_YES_NO, textStd("NoAvailableFilterToSendInput"), NULL, NULL, createFilterDialogHandler, NULL, DLGFLAG_GAME_ONLY);
            else
                addSystemChatMsg( textStd("NoActiveTabError"), INFO_USER_ERROR, 0 );
            return;    
        }
    }
    else if(channel == INFO_CHANNEL)
    {
        ChatChannel * channel = GetChannel(gUserSendChannel);
        if(channel)
            uiChatSendToUserChannel(pch, channel);
        else
            addSystemChatMsg( textStd("DoNotBelongToChannelError", gUserSendChannel), INFO_USER_ERROR, 0 );
        
        return;
    }

    // handle system channels
    switch(channel)
    {
        case INFO_NEARBY_COM:
            strcpy( buffer, "l ");
            break;
        case INFO_SHOUT_COM:
            strcpy( buffer, "b ");
            break;
        case INFO_TEAM_COM:
            strcpy( buffer, "g ");
            break;
        case INFO_FRIEND_COM:
            strcpy( buffer, "f ");
            break;
        case INFO_SUPERGROUP_COM:
            strcpy( buffer, "sg ");
            break;
        case INFO_LEVELINGPACT_COM:
            strcpy(buffer, "lp ");
            break;
        case INFO_REQUEST_COM:
            strcpy( buffer, "req ");
            break;
        case INFO_ALLIANCE_OWN_COM:
        case INFO_ALLIANCE_ALLY_COM:
            strcpy( buffer, "c ");
            break;
        case INFO_HELP:
            strcpy( buffer, "hc ");
            break;
        case INFO_ARCHITECT_GLOBAL:
            strcpy( buffer, "ma ");
            break;
        case INFO_ARENA_GLOBAL:
            strcpy( buffer, "ac ");
            break;
        case INFO_LEAGUE_COM:
            strcpy(buffer, "lc ");
            break;
        case INFO_LOOKING_FOR_GROUP:
            strcpy(buffer, "lfg ");
            break;
        default:
            strcpy( buffer, "l ");
            break;
    }

    if( optionGet(kUO_ChatBubbleColor1) != 0 ) // black is default
    {
        char hexColor[16];
        strcat(buffer, "<color #");
        sprintf( hexColor, "%08x", optionGet(kUO_ChatBubbleColor1) );
        hexColor[6] = 0;
        strcat(buffer, hexColor);
        strcat(buffer, ">");
    }

    if( optionGet(kUO_ChatBubbleColor2) != -1 ) // white is default
    {
        char hexColor[16];
        strcat(buffer, "<bgcolor #");
        sprintf( hexColor, "%08x", optionGet(kUO_ChatBubbleColor2) );
        hexColor[6] = 0;
        strcat(buffer, hexColor);
        strcat(buffer, ">");
    }

    strncat_count(buffer, pch, sizeof(buffer));
    cmdParse(buffer);
}

//---------------------------------------------------------------------------------------------------------|
//
//



int okToInput(int scanCode)
{
    int retval = TRUE;

    // Can't input if we're binding a key or in the frontend without a menu
    // open because keybinds don't work in the front end and there's no menu
    // to process the input.
    if( (shell_menu() && !contextMenu_IsActive()) || g_isBindingKey)
    {
        retval = FALSE;
    }
    // We also can't pass the input on if this is an editing key/key comobo
    // and a text block has focus, or if this is any key and the text block
    // with focus is writable.
    else if( !uiNoOneHasFocus() )
    {
        // Firstly, things which accept typing input take over all the keys.
        if( uiFocusIsEditable() )
        {
            retval = FALSE;
        }
        // Arrow keys always move the cursor if it's set anywhere. PageUp,
        // PageDown, Home and End should also move the cursor and Escape
        // cancels the selection and focus.
        else if( scanCode == INP_UP || scanCode == INP_DOWN ||
            scanCode == INP_LEFT || scanCode == INP_RIGHT ||
            scanCode == INP_ESCAPE || scanCode == INP_HOME ||
            scanCode == INP_END || scanCode == INP_PRIOR ||
            scanCode == INP_NEXT )
        {
            retval = FALSE;
        }
        // When numlock is off the number pad does cursor movement and we
        // should take it over. If it's on, they'll either type numbers or
        // be used by the keybind system.
        else if( !inpIsNumLockOn() )
        {
            if( scanCode == INP_NUMPAD2 || scanCode == INP_NUMPAD4 ||
                scanCode == INP_NUMPAD6 || scanCode == INP_NUMPAD8 ||
                scanCode == INP_NUMPAD1 || scanCode == INP_NUMPAD7 ||
                scanCode == INP_NUMPAD3 || scanCode == INP_NUMPAD9 )
            {
                retval = FALSE;
            }
        }
        // Similarly, CTRL-C, CTRL-V and CTRL-X are always copy, paste
        // and cut (for consistency even in non-editable contexts). CTRL-A
        // is also always select all.
        else if( inpLevel(INP_CONTROL) )
        {
            if( scanCode == INP_C || scanCode == INP_V ||
                scanCode == INP_X || scanCode == INP_A )
            {
                retval = FALSE;
            }
        }
    }

    return retval;
}

void uiChatStartChatting(char *pch, bool bGobbleFirst)
{
    if(!editMode() && okToInput(0))
    {
        if(!enterChatMode(1))
            return;

        if(bGobbleFirst)
        {
            s_bGobbleFirst = true;
        }

        if (!uiInFocus(chatEdit))
        {
            smf_SelectAllText(chatEdit);
        }

        if (pch)
        {
            setChatEditText(pch);
        }
    }
}
//---------------------------------------------------------------------------------------------------------|

void chatChannelSet(char * ch )
{
    int len = strlen(ch);
    Entity * e= playerPtr();

    if(!e) return;

    if( len > 1 )
    {
        int i;
        for(i=0;i<eaSize(&gChatChannels);i++)
        {
            ChatChannel * channel = gChatChannels[i];
            if( ! stricmp(ch, channel->name))  
            {
                e->pl->chatSendChannel = INFO_CHANNEL;
                strcpy(gUserSendChannel, channel->name);            
                sendChatChannel(INFO_CHANNEL, gUserSendChannel);
                return;
            }
        }

        addSystemChatMsg( textStd("ChatChannelError"), INFO_USER_ERROR, 0 );
    }
    else
    {
        if( ch[0] == 'l' || ch[0] == 'L' )
            e->pl->chatSendChannel= INFO_NEARBY_COM;
        if( ch[0] == 'b' || ch[0] == 'B' )
            e->pl->chatSendChannel= INFO_SHOUT_COM;
        if( ch[0] == 'r' || ch[0] == 'R' )
            e->pl->chatSendChannel= INFO_REQUEST_COM;
        if( ch[0] == 't' || ch[0] == 'T' )
            e->pl->chatSendChannel= INFO_TEAM_COM;
        if( ch[0] == 's' || ch[0] == 'S' )
            e->pl->chatSendChannel= INFO_SUPERGROUP_COM;
        if( ch[0] == 'p' || ch[0] == 'P' )
            e->pl->chatSendChannel= INFO_LEVELINGPACT_COM;
        if( ch[0] == 'f' || ch[0] == 'F' )
            e->pl->chatSendChannel= INFO_FRIEND_COM;
        if( ch[0] == 'a' || ch[0] == 'A' )
                e->pl->chatSendChannel= INFO_TAB;
        if( ch[0] == 'c' || ch[0] == 'C' )
            e->pl->chatSendChannel= INFO_ALLIANCE_OWN_COM;
        if( ch[0] == 'g' || ch[0] == 'G' )
            e->pl->chatSendChannel= INFO_LEAGUE_COM;

        sendChatChannel(e->pl->chatSendChannel, "");
    }
}

void chatChannelCycle()
{
    Entity *e = playerPtr();

    if(!e) return;

    e->pl->chatSendChannel++;

    // hack since INFO_TAB is not adjacent to the other tab channels    
    if(e->pl->chatSendChannel > INFO_TAB)
        e->pl->chatSendChannel = INFO_TEAM_COM;
    else if( e->pl->chatSendChannel > INFO_FRIEND_COM) 
        e->pl->chatSendChannel = INFO_TAB;
}

int drawSelectorButton(int * xp, int y, int z, float scale, int clr, Entity * e, int tip, int info, char * text, char * smallTex, char * largeTex)
{
    AtlasTex * tex;
    int hit = 0;
    int x = *xp;

    if( e->pl->chatSendChannel == info )
    {
        tex = atlasLoadTexture( largeTex );
        z += 1;
    }
    else
    {
        tex = atlasLoadTexture( smallTex );
    }

     BuildCBox( &chatTip[tip].bounds, x - tex->width*scale/2, y - tex->height*scale, scale*tex->width, scale*tex->height );
    setToolTip( &chatTip[tip], &chatTip[tip].bounds, text, 0, MENU_GAME, WDW_CHAT_BOX );
    if( D_MOUSEHIT == drawGenericButton( tex, x - tex->width*scale/2, y - tex->height*scale, z, scale, clr, CLR_WHITE, e->pl->chatSendChannel != info ) )
    {
        e->pl->chatSendChannel = info;
        sendChatChannel( e->pl->chatSendChannel, "" );    // won't use buttons for user channels
        hit = 1;
    }

    *xp += CHANNEL_SPACER*scale;

    return hit;
}

static void chatChannelSelector( int x, int y, int z, float scale, int clr )
{
    Entity *e = playerPtr();

    int hit = 0;

    if(!e) return;

    hit |= drawSelectorButton(&x, y, z, scale, clr, e, CHAT_TIP_LOCAL,    INFO_NEARBY_COM,        "LocalFilter",        "chat_channel_sm_local.tga",    "chat_channel_lg_local.tga" );
    hit |= drawSelectorButton(&x, y, z, scale, clr, e, CHAT_TIP_BROAD,    INFO_SHOUT_COM,            "BroadcastFilter",    "chat_channel_sm_broad.tga",    "chat_channel_lg_broad.tga" );
    hit |= drawSelectorButton(&x, y, z, scale, clr, e, CHAT_TIP_TEAM,    INFO_TEAM_COM,            "TeamFilter",        "chat_channel_sm_team.tga",        "chat_channel_lg_team.tga" );
    if(levelingpact_IsInPact(NULL))
        hit |= drawSelectorButton(&x, y, z, scale, clr, e, CHAT_TIP_LPACT,    INFO_LEVELINGPACT_COM,    "LevelingPactFilter",        "chat_channel_sm_pactlevel.tga",        "chat_channel_lg_pactlevel.tga" );
    else if(SAFE_MEMBER2(e, pl,chatSendChannel) == INFO_LEVELINGPACT_COM)
    {
        e->pl->chatSendChannel = INFO_NEARBY_COM;
        sendChatChannel( e->pl->chatSendChannel, "" );
    }
    hit |= drawSelectorButton(&x, y, z, scale, clr, e, CHAT_TIP_LPACT,    INFO_LEAGUE_COM,        "LeagueFilter",        "chat_channel_sm_league.tga",    "chat_channel_lg_league.tga"    );
    hit |= drawSelectorButton(&x, y, z, scale, clr, e, CHAT_TIP_SUPER,    INFO_SUPERGROUP_COM,    "SupergroupFilter",    "chat_channel_sm_super.tga",    "chat_channel_lg_super.tga"        );
    hit |= drawSelectorButton(&x, y, z, scale, clr, e, CHAT_TIP_REQ,    INFO_REQUEST_COM,        "RequestFilter",    "chat_channel_sm_request.tga",    "chat_channel_lg_request.tga"    );
     hit |= drawSelectorButton(&x, y, z, scale, clr, e, CHAT_TIP_FRIEND, INFO_FRIEND_COM,        "FriendFilter",        "chat_channel_sm_friend.tga",    "chat_channel_lg_friend.tga"    );
    hit |= drawSelectorButton(&x, y, z, scale, clr, e, CHAT_TIP_ALLIANCE,INFO_ALLIANCE_OWN_COM,    "AllianceFilter",    "chat_channel_sm_alliance.tga",    "chat_channel_lg_alliance.tga"    );
    hit |= drawSelectorButton(&x, y, z, scale, clr, e, CHAT_TIP_HELP,    INFO_HELP,                "HelpFilter",        "chat_channel_sm_help.tga",        "chat_channel_lg_help.tga"        );
    hit |= drawSelectorButton(&x, y, z, scale, clr, e, CHAT_TIP_LFG,    INFO_LOOKING_FOR_GROUP,    "LFGFilter",        "chat_channel_sm_lfg.tga",        "chat_channel_lg_lfg.tga"        );
    hit |= drawSelectorButton(&x, y, z, scale, clr, e, CHAT_TIP_TAB,    INFO_TAB,                "ActiveTabFilter",    "chat_channel_sm_activetab.tga","chat_channel_lg_activetab.tga" );
    if( hit )
        collisions_off_for_rest_of_frame = TRUE;

    return;
}

static int chatCurrentChannel( float x, float y, float z, float sc, int color )
{
    Entity * e = playerPtr();
    CBox box;
    char * name = "UNKNOWN";
    char buf[200];
    int wd;
    int rgba[4];
    float vsc = 1.f;
    ChatChannel * channel = GetChannel(gUserSendChannel);

      if(e)
    {    
        // if chat channel is selected, but not available, default to "local"
         if(    e->pl->chatSendChannel == INFO_CHANNEL 
            &&    ( ! channel || ! UsingChatServer()))
        {
            e->pl->chatSendChannel = INFO_NEARBY_COM;
        }
          else if(channel && UsingChatServer())
        {
            if( e->pl->chatSendChannel != INFO_CHANNEL )
                sendChatChannel(INFO_CHANNEL, gUserSendChannel);
            e->pl->chatSendChannel = INFO_CHANNEL;
        }
    

        switch( e->pl->chatSendChannel )
        {
        case INFO_NEARBY_COM:
            name = "LocalColon";
        xcase INFO_SHOUT_COM:
            name = "BroadcastColon";
        xcase INFO_TEAM_COM:
            name = "TeamColon";
        xcase INFO_SUPERGROUP_COM:
            name = "SupergroupColon";
        xcase INFO_LEVELINGPACT_COM:
            name = "LevelingpactColon";
        xcase INFO_ALLIANCE_OWN_COM:
        case INFO_ALLIANCE_ALLY_COM:
            name = "AllianceSend";
        xcase INFO_REQUEST_COM:
            name = "RequestColon";
        xcase INFO_FRIEND_COM:
            name = "FriendsColon";
        xcase INFO_TAB:
            {
                 if(gActiveChatFilter)
                    sprintf(buf, "%s:", gActiveChatFilter->name); 
                else
                    strcpy(buf, "NoTab");    // This should never happen... ?
                name = &buf[0];
                break;
            }
        xcase INFO_CHANNEL:
             {
                sprintf(buf, "%s:", gUserSendChannel);
                name = &buf[0];
            }
        xcase INFO_LEAGUE_COM:
            name = "LeagueColon";
        xcase INFO_HELP:
            name = "HelpColon";
        xcase INFO_LOOKING_FOR_GROUP:
            name = "LookingForGroupColon";
        }
    }

    wd = str_wd( &game_12, sc, sc, name ) + PIX3*2*sc;

    BuildCBox( &box, x+PIX3*sc, y, wd, (CHAT_EDIT_HEIGHT-PIX3)*sc );

    if( channel )
    {
        font_color( channel->color, channel->color2 );
    }
    else
    {
          GetTextColorForType( e->pl->chatSendChannel==INFO_CHANNEL?INFO_CHANNEL_TEXT:e->pl->chatSendChannel, rgba );
        font_color( rgba[1], rgba[0] );
    }

    if( mouseCollision( &box ) )
    {
           vsc = 1.1f;

        if( mouseClickHit( &box, MS_LEFT) )
            contextMenu_set( chatContext, x, y, NULL );
    }

    font( &game_12 );
      
       prnt( x + (TEXT_XOFF+PIX3)*sc - wd*sc*(vsc-1)/2, y + (CHAT_EDIT_HEIGHT-TEXT_YOFF)*sc, z, sc*vsc, sc*vsc, name );
 
    return wd + (2*PIX3)*sc;
}

void GetQuickChatZone(int *pX, int *pY, int *pW, int *pH)
{
    float x, y, z, wd, ht, scale;
    int color, bcolor;

    // what is this for?
    if(!window_getDims(WDW_CHAT_BOX, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor))
    {
        int w, h;
        windowClientSizeThisFrame(&w, &h);
        *pX = w/2;
        *pY = h/3;
    }
    else
    {
        AtlasTex *texQuickChat = atlasLoadTexture("quick_chat.tga");

         *pX = x + wd - 2*PIX3*scale - texQuickChat->width*scale; // hack
        *pY = y + ht - CHAT_EDIT_HEIGHT*scale;
        *pW = (PIX3 + texQuickChat->width)*scale;
        *pH = (CHAT_EDIT_HEIGHT - PIX3)*scale;
    }
}

// always set primary chat mode to maximized
void resetPrimaryChatSize()
{
    if(GetChatWindow(0)->minimizedOffset)
        hidePrimaryChat();
}

// toggle between min/max modeo
void hidePrimaryChat()
{
    float x,y,wd,ht,scale;

    ChatWindow * window = GetChatWindow(0);

    if(!window_getDims(WDW_CHAT_BOX, &x, &y, 0, &wd, &ht, &scale, 0, 0) || window_getMode(WDW_CHAT_BOX) != WINDOW_DISPLAYING )
        return;

      if(window->minimizedOffset)    
    {
        // maximize the window
        ht        += (window->minimizedOffset)*scale;
        y        -= (window->minimizedOffset)*scale;
        window->minimizedOffset = 0;
        winDefs[WDW_CHAT_BOX].loc.draggable_frame = TRUE;
    }
    else
    {
        // minimize the window
           window->minimizedOffset = ht/scale - (PRIMARY_CHAT_MINIMIZED_HEIGHT);  // store unscaled value
        ht        -= (window->minimizedOffset)*scale;
        winDefs[WDW_CHAT_BOX].loc.draggable_frame = FALSE;

        if( ! winDefs[WDW_CHAT_BOX].below)
            y += (window->minimizedOffset)*scale;
    
    }

     window_setDims(WDW_CHAT_BOX, x, y, wd, ht);

    window_UpdateServer( WDW_CHAT_BOX );
    //if(window->minimizedOffset)    
    //    winDefs[WDW_CHAT_BOX].loc.draggable_frame = FALSE;

    sendChatSettingsToServer();
}


enum
{
    MIDDLE_BAR_CLICKED,
    MIDDLE_BAR_EXPANDING,
    MIDDLE_BAR_OPEN,
    MIDDLE_BAR_CLOSING,
    MIDDLE_BAR_CLOSED,

};

#define MIDDLE_BAR_SPEED    (1.f)
#define MIDDLE_BAR_MIN        (R10+PIX3)*2

static int chat_middleBarState( float x, float y, float z, float * middleBarHt, float top, float max_ht, float cx, float wd, float scale, int color, int id, float excludeStart, float excludeWd )
{
    CBox box, exclude;
    AtlasTex * mid   = atlasLoadTexture( "Chat_separator_handle.tga" );
    AtlasTex * down  = atlasLoadTexture( "Chat_separator_arrow_down.tga" );
    AtlasTex * up    = atlasLoadTexture( "Chat_separator_arrow_up.tga" );

    static int state=MIDDLE_BAR_CLOSED;
    static int win = 0;
    static float clickLoc;

    // wow this is wasteful array
    static float sc[MAX_WINDOW_COUNT] = {0.0f};

    if(*middleBarHt == 0)
        return 0;

    y -= mid->height/2;

    BuildCBox( &box, cx + R10*scale, y - mid->width*scale, wd - R10*2*scale, mid->width*2*scale );
    BuildCBox( &exclude, excludeStart, y - mid->width*scale, excludeWd, mid->width*2*scale );

    if( !win || win == id )
    {
        if ( state != MIDDLE_BAR_CLICKED && (*middleBarHt > 0.f && *middleBarHt < 1.f ) )
        {
            if( !mouseCollision(&exclude) && mouseCollision(&box ) )
            {
                state = MIDDLE_BAR_EXPANDING;
                setCursor( "cursor_win_vertresize.tga", NULL, FALSE, 10, 15 );
            }
            else
                state = MIDDLE_BAR_CLOSING;
        }

        if( !isDown( MS_LEFT ) && state == MIDDLE_BAR_CLICKED )
        {
            state = MIDDLE_BAR_OPEN;

            if( *middleBarHt != 0 && *middleBarHt != 1 )
            {
                if ( (*middleBarHt*max_ht) < MIDDLE_BAR_MIN )
                    *middleBarHt = ((float)MIDDLE_BAR_MIN/max_ht);
                if ( (*middleBarHt*max_ht) > max_ht-MIDDLE_BAR_MIN )
                    *middleBarHt = (max_ht-MIDDLE_BAR_MIN)/max_ht;
            }
            else
                chatSendAllTabsToTop(id);

            sendChatSettingsToServer();
            win = 0;
        }
        else if ( !mouseCollision(&exclude) && mouseDownHit( &box, MS_LEFT ) && (*middleBarHt > 0.f && *middleBarHt < 1.f ) )
        {
            state = MIDDLE_BAR_CLICKED;
            win = id;
            clickLoc = gMouseInpCur.y - max_ht + (*middleBarHt*max_ht);
        }

        if ( state == MIDDLE_BAR_EXPANDING )
        {
            sc[id] += TIMESTEP*MIDDLE_BAR_SPEED;
            if( sc[id] > 1.0f )
            {
                sc[id] = 1.0f;
                state = MIDDLE_BAR_OPEN;
            }
        }
        if( state == MIDDLE_BAR_CLOSING )
        {
            sc[id] -= TIMESTEP*MIDDLE_BAR_SPEED;
            if( sc[id] < 0.0f )
            {
                sc[id] = 0.0f;
                state = MIDDLE_BAR_CLOSED;
            }
        }

        if( state == MIDDLE_BAR_CLICKED )
        {
            sc[id] = 1.f;
            *middleBarHt = (clickLoc+max_ht-gMouseInpCur.y)/max_ht;
            setCursor( "cursor_win_vertresize.tga", NULL, FALSE, 10, 15 );
        }
    }

    // bounds check
    if ( (*middleBarHt*max_ht) < MIDDLE_BAR_MIN )
        *middleBarHt = ((float)MIDDLE_BAR_MIN/max_ht);
    if ( (*middleBarHt*max_ht) > max_ht-MIDDLE_BAR_MIN )
        *middleBarHt = (max_ht-MIDDLE_BAR_MIN)/max_ht;

    y = top+max_ht - *middleBarHt*max_ht - mid->height*scale/2;

    if( *middleBarHt > 0.f && *middleBarHt < 1.f )
    {
        if( state != MIDDLE_BAR_CLOSED  && (!win || win == id) )
        {
            display_sprite( up, x - sc[id]*up->width*scale/2, y - (mid->height/2 + sc[id]*up->height + 5 - PIX3)*scale, z+2, sc[id]*scale, sc[id]*scale, color );
            display_sprite( down, x - sc[id]*down->width*scale/2, y + (mid->height/2 + 5 + PIX3)*scale, z+2, sc[id]*scale, sc[id]*scale, color );
        }

        display_sprite( mid, x - mid->width*scale/2, y - (mid->height/2 - PIX3)*scale, z+5, scale, scale, color );
    }

    return state == MIDDLE_BAR_CLICKED;
}


/**********************************************************************func*
* Context Menu Commands
*
*/

void addFilter(ChatWindow * window)
{    
    ChatFilter * filter = ChatFilterCreate("");
    chatTabOpen(filter, window);
}

void deleteFilter(ChatWindow * window)
{
    ChatFilter * filter = window->contextPane?uiTabControlGetSelectedData(window->botTabControl):uiTabControlGetSelectedData(window->topTabControl);

    if(filter)
    {
        ChatFilterRemoveAllChannels(filter, true);
        ChatWindowRemoveFilter(window, filter);
        ChatFilterDestroy(filter);
        sendChatSettingsToServer();
    }
}

void editFilter(ChatWindow * window)
{
    ChatFilter * filter;
    
    if(window->contextPane)
        filter = uiTabControlGetSelectedData(window->botTabControl);
    else
        filter = uiTabControlGetSelectedData(window->topTabControl);

    if(filter)
    {
        chatTabOpen(filter, false);
    }
}

void clearFilter(ChatWindow * window)
{
    ChatFilter * filter = window->contextPane?uiTabControlGetSelectedData(window->botTabControl):uiTabControlGetSelectedData(window->topTabControl);

    if(filter)
    {
        ChatFilterClear(filter);
    }
}

void clearAllChat()
{
    int i;
    for(i=0;i<eaSize(&gChatFilters);i++)
        ChatFilterClear(gChatFilters[i]);
}


void openChatOptionsMenu(int windowIdx)
{
    float x, y;
    ChatWindow * window = GetChatWindow(windowIdx);
    childButton_getCenter( wdwGetWindow(window->windefIdx), (windowIdx ? 0 : 4), &x, &y );
    contextMenu_set( window->menu, x, y, NULL );
}

void createChannelDlgHandler(void *data)
{
    char buf[1000];
    sprintf(buf, "chan_create %s", dialogGetTextEntry());
    cmdParse(buf);

    ChatFilterAddPendingChannel(gActiveChatFilter, dialogGetTextEntry(), false);
}

void createChannelDlgHandlerNoEntry(void *data)
{
    char buf[1000];
    sprintf(buf, "chan_create %s", tmpChannelName);
    cmdParse(buf);

    ChatFilterAddPendingChannel(gActiveChatFilter, tmpChannelName, false);
}

void createChannelDialog(void * foo)
{
    dialogNameEdit(textStd("CreateChannelDialog"), NULL, NULL,  createChannelDlgHandler, NULL, MAX_CHANNELNAME, 0);
}


void joinChannelDlgHandler(void *data)
{
    joinChatChannel(dialogGetTextEntry());
    ChatFilterAddPendingChannel(gActiveChatFilter, dialogGetTextEntry(), false);
}

void joinChannelDlgHandlerNoEntry(void *data)
{
    joinChatChannel(tmpChannelName);
    ChatFilterAddPendingChannel(gActiveChatFilter, tmpChannelName, false);
}

void joinChannelDialog(void * foo)
{
    dialogNameEdit(textStd("JoinChannelDialog"), NULL, NULL,  joinChannelDlgHandler, NULL, MAX_CHANNELNAME, 0);
}


// Context Menu Visible Functions -- core logic + context menu wrappers
int hasSelectedFilter(ChatWindow * cw)
{
    if( !cw->contextPane && uiTabControlGetSelectedData(cw->topTabControl))
        return CM_AVAILABLE;
    if( cw->contextPane && uiTabControlGetSelectedData(cw->botTabControl))
        return CM_AVAILABLE;
    else
        return CM_VISIBLE;
}

int canEditFilter_CM(ChatWindow * cw)
{
    if(hasSelectedFilter(cw) && chatOptionsAvailable() )
        return CM_AVAILABLE;
    else
        return CM_VISIBLE;
}

int canDeleteFilter_CM(ChatWindow * cw)
{
    ChatFilter * filter = cw->contextPane?uiTabControlGetSelectedData(cw->botTabControl):uiTabControlGetSelectedData(cw->topTabControl);

    if(filter && filter != currentChatOptionsFilter() )
        return CM_AVAILABLE;
    else
        return CM_VISIBLE;
}

bool canAddFilter()
{
    return (eaSize(&gChatFilters) < MAX_CHAT_TABS);
}

int canAddFilter_CM(void * foo)
{
    if(canAddFilter() && chatOptionsAvailable() )
        return CM_AVAILABLE;
    else
        return CM_VISIBLE;
}



int canAddChannel_CM(void * foo)
{
    if(!UsingChatServer())
        return CM_HIDE;

    if(canAddChannel())
        return CM_AVAILABLE;
    else
        return CM_VISIBLE;
}

int belongToChannels_CM(void * data)
{
    int doNotHide = (int) data;
    if(!UsingChatServer())
        return CM_HIDE;

    if(eaSize(&gChatChannels))
        return CM_AVAILABLE;
    else if(doNotHide)
        return CM_VISIBLE;
    else
        return CM_HIDE;
}

int canLeaveChannel_CM(void * data)
{
    int idx = ((int)data)-1;

    if(!UsingChatServer())
        return CM_HIDE;

    if(eaSize(&gChatChannels) > idx)
        return CM_AVAILABLE;
    else
        return CM_HIDE;
}

int canDisplayChannel_CM(void * data)
{
    int idx = ((int)data)-1;
    Entity * e = playerPtr();

    if(!e)
        return CM_HIDE;

    if(!UsingChatServer())
        return CM_HIDE;

    if( gChatChannels && idx < eaSize(&gChatChannels))
    {
        if(e->pl->chatSendChannel == INFO_CHANNEL && ! stricmp(gChatChannels[idx]->name, gUserSendChannel))
        {
            return CM_CHECKED;
        }
        else
            return CM_AVAILABLE;
    }
    else
        return CM_HIDE;
}

char *chatcm_userChannelText(void * data)
{
    int idx = ((int)data)-1;

    if( gChatChannels && idx < eaSize(&gChatChannels))
        return gChatChannels[idx]->name;
    else
        return NULL;
}

void addChannelWindowToMenu(ContextMenu *menu);

int isUsingChatServer_CM(void *foo)
{
    if(!UsingChatServer())
        return CM_HIDE;

    return CM_AVAILABLE;
}
static void addChatChannelsToCM(ContextMenu *cm)
{
    int i = 0;
    while (chatChannels[i].cm_text)
    {
        if (chatChannels[i].info_com == INFO_LEVELINGPACT_COM)
            contextMenu_addCheckBox( cm, levelingpact_IsInPact, NULL, chatcm_setChannel, &chatChannels[i].info_com, chatChannels[i].cm_text );
        else
            contextMenu_addCheckBox( cm, chatcm_isChannel, &chatChannels[i].info_com, chatcm_setChannel, &chatChannels[i].info_com, chatChannels[i].cm_text );
        i++;
    }
}
void initContextMenus()
{
    int i,k;
    static ContextMenu * sLeaveChannelSubMenu;

    chatContext = contextMenu_Create( NULL );
    contextMenu_addTitle( chatContext, "CMChatChannel" );
    contextMenu_addCheckBox( chatContext, chatcm_isChannel, &chatChannels[0].info_com, chatcm_setChannel, &chatChannels[0].info_com, chatChannels[0].cm_text );
    addChatChannelsToCM(chatContext);

    contextMenu_addDividerVisible(chatContext, belongToChannels_CM );

    for(i=1;i<=MAX_WATCHING;i++)
        contextMenu_addVariableTextCheckBox(chatContext, canDisplayChannel_CM, (void*) i, chatcm_setUserChannel, (void*) i, chatcm_userChannelText, (void*) i); 

    for( i = 0; i < CHAT_TIP_TOTAL; i++ )
        addToolTip( &chatTip[i] );

    // leave channel sub-menu
    sLeaveChannelSubMenu = contextMenu_Create(NULL);
    for(i=1;i<=MAX_WATCHING;i++)
        contextMenu_addVariableTextCode( sLeaveChannelSubMenu, canLeaveChannel_CM, (void*)i, chatcm_leaveChannel, (void*) i, chatcm_userChannelText, (void*) i, 0);

    for(i=0; i<MAX_CHAT_WINDOWS; i++)
    {    
        ChatWindow * window = GetChatWindow(i);
        window->menu = contextMenu_Create(NULL);

        contextMenu_addTitle( window->menu, "CMChatChannel" );
        addChatChannelsToCM(window->menu);

        contextMenu_addDividerVisible(window->menu, belongToChannels_CM );

        for(k=1;k<=MAX_WATCHING;k++)
            contextMenu_addVariableTextCheckBox(window->menu, canDisplayChannel_CM, (void*) k, chatcm_setUserChannel, (void*) k, chatcm_userChannelText, (void*) k); 

        contextMenu_addDivider(window->menu);

        contextMenu_addCode(window->menu, canAddChannel_CM,                0,        createChannelDialog,    window, textStd("CMCreateChannel"),    0);
        contextMenu_addCode(window->menu, canAddChannel_CM,                0,        joinChannelDialog,        window, textStd("CMJoinChannel"),    0);
        contextMenu_addCode(window->menu, belongToChannels_CM,    (void*) 1,        0,                        0,        textStd("CMLeaveChannel"),    sLeaveChannelSubMenu);

        contextMenu_addDividerVisible(window->menu, isUsingChatServer_CM );

        addChannelWindowToMenu(window->menu);

        contextMenu_addDividerVisible(window->menu, isUsingChatServer_CM );

        contextMenu_addCode(window->menu, canAddFilter_CM,        0,        addFilter,                window, textStd("CMAddChatTab"),    0);
        contextMenu_addCode(window->menu, canEditFilter_CM,        window,    editFilter,                window, textStd("CMEditChatTab"),    0);
        contextMenu_addCode(window->menu, canDeleteFilter_CM,    window,    deleteFilter,            window, textStd("CMDeleteChatTab"),    0);
        contextMenu_addCode(window->menu, hasSelectedFilter,    window,    clearFilter,            window, textStd("CMClearChatTab"),    0);
    }

    chatLinkContext = contextMenu_Create( NULL );
    contextMenu_addVariableTextCode( chatLinkContext, alwaysAvailable, 0, chatLinkcm_sendTell, 0, chatLinkcm_sendTellName, 0, 0 );
    contextMenu_addVariableTextCode( chatLinkContext, alwaysAvailable, 0, chatLinkcm_sendTellToTeamLeader, 0, chatLinkcm_sendTellToTeamLeaderName, 0, 0 );
    contextMenu_addVariableTextCode( chatLinkContext, alwaysAvailable, 0, chatLinkcm_inviteTeam, 0, chatLinkcm_inviteTeamName, 0, 0 );

    contextMenu_addVariableTextCode( chatLinkContext, alwaysAvailable, 0, chatLinkcm_inviteLeague, 0, chatLinkcm_inviteLeagueName, 0, 0);
    contextMenu_addVariableTextCode( chatLinkContext, alwaysAvailable, 0, chatLinkcm_sendTellToTeamLeader, 0, chatLinkcm_sendTellToLeagueLeaderName, 0, 0 );

    contextMenu_addCode( chatLinkContext, alwaysAvailable, 0, chatLinkcm_getGlobalName, 0, textStd("CMGetGlobal"), 0 );
    contextMenu_addVariableTextCode( chatLinkContext, alwaysAvailable, 0, chatLinkcm_Friend, 0, chatLinkcm_FriendName, 0, 0 );
    contextMenu_addVariableTextCode( chatLinkContext, alwaysAvailable, 0, chatLinkcm_Note, 0, chatLinkcm_NoteName, 0, 0 );
    contextMenu_addDivider(chatLinkContext);
    contextMenu_addVariableTextCode( chatLinkContext, alwaysAvailable, 0, chatLinkcm_ignore, 0, chatLinkcm_ignoreName, 0, 0 );
    contextMenu_addVariableTextCode( chatLinkContext, alwaysAvailable, 0, chatLinkcm_ignoreSpammer, 0, chatLinkcm_ignoreSpammerName, 0, 0 );

    chatLinkGlobalContext = contextMenu_Create( NULL );
    contextMenu_addVariableTextCode( chatLinkGlobalContext, alwaysAvailable, 0, chatLinkGlobalcm_sendTell, 0, chatLinkcm_sendTellName, 0, 0 );
    contextMenu_addVariableTextCode( chatLinkGlobalContext, alwaysAvailable, 0, chatLinkGlobalcm_inviteTeam, 0, chatLinkcm_inviteTeamName, 0, 0 );
    contextMenu_addVariableTextCode( chatLinkGlobalContext, alwaysAvailable, 0, chatLinkGlobalcm_inviteLeague, 0, chatLinkcm_inviteLeagueName, 0, 0);
    contextMenu_addCode( chatLinkGlobalContext, alwaysAvailable, 0, chatLinkGlobalcm_getLocalName, 0, textStd("CMGetLocal"), 0 );
    contextMenu_addVariableTextCode( chatLinkGlobalContext, alwaysAvailable, 0, chatLinkGlobalcm_Friend, 0, chatLinkcm_FriendName, 0, 0 );
    contextMenu_addVariableTextCode( chatLinkGlobalContext, alwaysAvailable, 0, chatLinkGlobalcm_Note, 0, chatLinkGlobalcm_NoteName, 0, 0 );
    
    contextMenu_addDividerVisible(chatLinkGlobalContext, chatLinkGlobal_CanMod);
    //contextMenu_addCode( chatLinkGlobalContext, chatLinkGlobal_CanMod, 0, chatLinkGlobal_Op, 0,  textStd("CMMakeOp"), 0 ); -- Chris Bruce doesn't like this here
    contextMenu_addVariableTextCode( chatLinkGlobalContext, chatLinkGlobal_CanMod, 0, chatLinkGlobal_Silence, 0, chatLinkcm_SilenceText, 0, 0 );
    contextMenu_addCode( chatLinkGlobalContext, chatLinkGlobal_CanMod, 0, chatLinkGlobal_Kick, 0,  textStd("CMKick"), 0 );
    contextMenu_addDivider(chatLinkGlobalContext);
    contextMenu_addVariableTextCode( chatLinkGlobalContext, alwaysAvailable, 0, chatLinkGlobalcm_ignore, 0, chatLinkcm_ignoreName, 0, 0 );
    contextMenu_addVariableTextCode( chatLinkGlobalContext, alwaysAvailable, 0, chatLinkGlobalcm_ignoreSpammer, 0, chatLinkGlobalcm_ignoreSpammerName, 0, 0 );


    chatLinkChannelContext = contextMenu_Create( NULL );
    contextMenu_addCode(chatLinkChannelContext, alwaysAvailable, 0,    chatLinkChannelcm_send, 0, textStd("CMSendMessage"), 0);
    contextMenu_addCode(chatLinkChannelContext, alwaysAvailable, 0,    chatLinkChannelcm_members, 0, textStd("CMListMembers"), 0);
    contextMenu_addCode(chatLinkChannelContext, alwaysAvailable, 0,    startColorPickerWindow, 0, textStd("CMSetChannelColor"), 0);
    contextMenu_addDividerVisible(chatLinkChannelContext, isChannelOperator);
    contextMenu_addCode(chatLinkChannelContext, isChannelOperator, 0, chatLinkChannelcm_setMotd, 0, textStd("CMSetMotd"), 0);
    contextMenu_addCode(chatLinkChannelContext, isChannelOperator, 0, chatLinkChannelcm_setDescription, 0, textStd("CMSetDescription"), 0);
    contextMenu_addDivider(chatLinkChannelContext);
    contextMenu_addCode( chatLinkChannelContext, alwaysAvailable, 0, chatLinkChannelcm_leave, 0, textStd("CMLeaveChannel"), 0 );

}


//----------------------------------------------------------------------------------------------------------------------------------------------
// debug ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------------

// Copies last X lines from specified chat window into the clipboard
void DebugCopyChatToClipboard(char * tabName)
{
    int i;
    int begin;
    int end;
    int size = 0;
    char * buf = NULL;
    ChatFilter * filter = GetChatFilter(tabName);

    if(!filter)
    {
        addSystemChatMsg( textStd("TabDoesNotExistError", tabName), INFO_USER_ERROR, 0 );
        return;
    }

    // will copy the last X lines
    begin = 0;
    end   = filter->chatQ.size;


    // get required size
    for(i = begin; i < end; i++)
    {
        size += strlen(filter->chatQ.ppLines[i]->pchText);
        size += 1; // newline
    }

    buf = malloc((sizeof(char) * size) + 1);

    strcpy(buf, "");

    // copy to clipboard
    for(i = begin; i < end; i++)
    {
        strcat(buf, stripPlayerHTML(filter->chatQ.ppLines[i]->pchText, 1) );
        strcat(buf, "\n");
    }

    winCopyToClipboard(buf);

    addSystemChatMsg( textStd("CopiedXLinesToClipboard", max(0, (end - begin))), INFO_USER_ERROR, 0 );

    free(buf);
}

//----------------------------------------------------------------------------------------------------------------------------------------------
// Billing  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------------

// Billing time remaining functions
U32 billingUpdateTime = 0; //The seconds since 2000 of last billing update
U32 billingTimeLeft = 0; //How many seconds were left on billing on last update
U32 billingLastDisplayed = 0; //When was this last displayed? (only display once per second)
bool billingCountdown = FALSE; //Are we counting down the time left?

void setPaymentRemainTime(int curr_avail, int total_avail, int reservation, bool countdown, bool change_billing) 
{
    int hours, minutes, seconds;
    int t_hours, t_minutes, t_seconds;

    billingCountdown = countdown;
    billingUpdateTime = timerSecondsSince2000();
    billingLastDisplayed = billingUpdateTime;
    billingTimeLeft = curr_avail;

    hours = curr_avail / (60 * 60);
    minutes = (curr_avail / 60) % 60;
    seconds = curr_avail % 60;

    t_hours = total_avail / (60 * 60);
    t_minutes = (total_avail / 60) % 60;
    t_seconds = total_avail % 60;

    if (change_billing)
        addSystemChatMsg( textStd("BillingTargetChanged"),INFO_SVR_COM,0);
    addSystemChatMsg( textStd("PaymentTotalTime",hours,minutes,seconds,reservation),INFO_SVR_COM,0);
    //addSystemChatMsg( textStd("PaymentTotalTime",t_hours,t_minutes,t_seconds,reservation),INFO_SVR_COM,0);
}

void displayBillingCountdown()
{
    int hours, minutes, seconds;
    int timeleft = (int)billingTimeLeft - (int)(timerSecondsSince2000() - billingUpdateTime);

    if (!billingCountdown || timeleft < 0)
        return;

    hours = timeleft / (60 * 60);
    minutes = (timeleft / 60) % 60;
    seconds = timeleft % 60;

    // Only display this message every so often, and only once each time it ticks off
    if ((seconds % 10) != 0 || timeleft == billingLastDisplayed)
        return;

    billingLastDisplayed = timeleft;
    addSystemChatMsg( textStd("PaymentRemainTime",hours,minutes,seconds),INFO_USER_ERROR,0);
}


//----------------------------------------------------------------------------------------------------------------------------------------------
// master function in charge of all the chat drawing ///////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------------
int chatWindow()
{
      AtlasTex *texQuickChat        = atlasLoadTexture( "quick_chat.tga" );
    AtlasTex *texQuickChatOvr    = atlasLoadTexture( "quick_chat_detail.tga" );
    CBox box;
     ChatWindow * chatWindow = 0;
    ChatFilter * topfilter  = 0;
    ChatFilter * botfilter = 0; // second pane
    Entity *e = playerPtr();

    float x, y, z, wd, currChanWd=0, ht, scale, divider_ht = 0;
    float topVisHeight = 0, botVisHeight = 0;

    int color, bcolor;
    int xqc, yqc, wqc=0, hqc;
    int iFontHeight;
    int chatHeight;
     int offsetY = 0;
    int mode;
    int build;
    bool drawChat = true, dividerGrabbed = false;
    bool hasActiveFilter;
    static bool init = false;
    static int lastFontSize = 0, lastDrag = 0;
 
    if (!e)
        return 0;

    if(!init)
    {
        initContextMenus();
        init = true;
    }

    assert(e->pchar);
    build = e->pchar->iCurBuild;

    if(!gSentRespecMsg)
    {
        int i;
         cmdParse("respec_status");
        cmdParse("tailor_status");
        cmdParse("auc_loginupdate");
        cmdParse("architect_loginupdate");
        addSystemChatMsg( textStd("WelcomeToGame",e->name), INFO_SVR_COM, 0 );

        for( i = 0; i < MAX_CHAT_TABS; i++ )
        {
            ChatTabSettings * tab = &e->pl->chat_settings.tabs[i];
            if( stricmp( tab->name, "Help" ) == 0 && BitFieldGet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_HELP) )
                addSystemChatMsg( textStd("WelcomeHelpChannel",e->name), INFO_SVR_COM, 0 );
        }

        addSystemChatMsg( textStd("WelcomeGMOTD"), INFO_SVR_COM, 0 );
        gSentRespecMsg = true;
    }

    if(getCurrentLocale() == 3)// Korea
    {
        static int messages_sent = 0;
        int current_time = timerSecondsSince2000();
        
        if(!gChatLogonTimer)
        {
            gChatLogonTimer = current_time;
            messages_sent = 0;
            addSystemChatMsg( textStd("PlayingTooLongIsBad"), INFO_ADMIN_COM, 0 );
        }

        if( (current_time - gChatLogonTimer) > 3600 )
        {
            messages_sent++;
            if(messages_sent > 2) // >=3h
                addSystemChatMsg( textStd("YouHaveBeenPlayingTooLong",messages_sent), INFO_ADMIN_COM, 0 );
            else
                addSystemChatMsg( textStd("YouHaveBeenPlayingAWhile",messages_sent), INFO_ADMIN_COM, 0 );
            gChatLogonTimer = current_time;
        }
    }

    displayBillingCountdown();

       if( !window_getDims( gCurrentWindefIndex, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
    {
        if(gCurrentWindefIndex == WDW_CHAT_BOX)
            enterChatMode(0);
        return 0;
    }

    BuildCBox(&box, x, y, wd, ht );
     if( cursor.dragging && mouseCollision(&box) && !isDown(MS_LEFT) )
    {
        TrayObj * to = &cursor.drag_obj;
         if( to->type == kTrayItemType_Inspiration )
        {
            const BasePower *ppow = e->pchar->aInspirations[to->iset][to->ipow];
            addChatEditText(getTextLinkForPower(ppow,0));
            trayobj_stopDragging();
        }
        else if( to->type == kTrayItemType_SpecializationInventory )
        {
            const BasePower *ppow = e->pchar->aBoosts[to->ispec]->ppowBase;
            int iLevel = e->pchar->aBoosts[to->ispec]->iLevel;
            addChatEditText(getTextLinkForPower(ppow,iLevel));
            trayobj_stopDragging();
        }
        else if( to->type == kTrayItemType_Power )
        {
            const BasePower *ppow = e->pchar->ppPowerSets[to->iset]->ppPowers[to->ipow]->ppowBase;
            TrayObj *last = getTrayObj(e->pl->tray, to->icat, to->itray, to->islot, build, true);
            if(last)
                trayobj_copy( last, &cursor.drag_obj, TRUE );

            addChatEditText(getTextLinkForPower(ppow,0));
            trayobj_stopDragging();

        }
        else if( to->type == kTrayItemType_Salvage )
        {
            const SalvageItem *salvage = e->pchar->salvageInv[to->invIdx]->salvage;
            addChatEditText(getTextLinkForSalvage((SalvageItem*)salvage));
            trayobj_stopDragging();
        }
        else if( to->type == kTrayItemType_Recipe )
        {
            const DetailRecipe *recipe = detailrecipedict_RecipeFromId(to->invIdx);
            addChatEditText(getTextLinkForRecipe(recipe));
            trayobj_stopDragging();
        }
    }
 
    // check to see if font size has changed.
    if ( (font_grp && lastFontSize != font_grp->renderParams.renderSize) || lastDrag != winDefs[gCurrentWindefIndex].drag_mode )
    {
        // if it has, please resize everything
        if(topfilter)
            topfilter->reformatText = true;
        if(botfilter)
            botfilter->reformatText = true;
        lastFontSize = optionGet(kUO_ChatFontSize);
        lastDrag = winDefs[gCurrentWindefIndex].drag_mode;
    }

     chatWindow = GetChatWindowFromWindefIndex(gCurrentWindefIndex);
    if(!gActiveChatFilter) // make sure there is an active filter (if possible)
        ChooseDefaultActiveFilter(0);
    
    hasActiveFilter = ChatWindowHasFilter(chatWindow, gActiveChatFilter);

    // If active filter is this window, and we're minimized, pick another visible active filter.
    // If visible filter exists, we'll stick with the current one.
    mode = window_getMode(gCurrentWindefIndex);
    if(   hasActiveFilter && 
        ( chatWindow->minimizedOffset || ( mode != WINDOW_DISPLAYING && mode != WINDOW_GROWING)))
    {
        ChooseDefaultActiveFilter(gActiveChatFilter);
    }

    // special case: don't draw chat if primary chat window is minimized
       if(chatWindow->idx == 0 && chatWindow->minimizedOffset)
    {
        drawChat = false;
    }

      chatHeight = ht;
    if(chatWindow->idx == 0)
        chatHeight -= CHAT_EDIT_HEIGHT*scale;

    if(drawChat)
    {
          divider_ht = chatWindow->divider*chatHeight;
        dividerGrabbed = chat_middleBarState( x + wd/2, y + chatHeight - divider_ht, z, &chatWindow->divider, y, chatHeight, x, wd, scale, color, gCurrentWindefIndex, x + R10*scale, uiTabControlGetWidth(chatWindow->botTabControl) );
        if (dividerGrabbed && !gWindowDragging)
        {
            gWindowDragging = true;
        }
        divider_ht = chatWindow->divider*chatHeight;
    }

    // only draw edit box in "master" window
    if(chatWindow->idx == 0)
    {
        if(chatWindow->minimizedOffset)
            drawFrame( PIX3, R10, x, y, z, wd, (CHAT_EDIT_HEIGHT+PIX3)*scale, scale, color, bcolor );
        else
        {
            if( chatWindow->divider)
                drawJoinedFrame3Panel(PIX3, R10, x, y, z, scale, wd, chatHeight-divider_ht, divider_ht, CHAT_EDIT_HEIGHT*scale, color, bcolor);
            else
                drawJoinedFrame2Panel(PIX3, R10, x, y, z, scale, wd, chatHeight, CHAT_EDIT_HEIGHT*scale, color, bcolor, 0,0);  
        }
    }
    else
    {
        if( chatWindow->divider)
            drawJoinedFrame2Panel(PIX3, R10, x, y, z, scale, wd, chatHeight-divider_ht, divider_ht, color, bcolor, 0,0);
        else
            drawFrame( PIX3, R10, x, y, z, wd, chatHeight, scale, color, bcolor );
    }

    // this will turn off collision for rest of function
    if( chatWindow->idx == 0 && windowUp(gCurrentWindefIndex) && !chatWindow->minimizedOffset)
        chatChannelSelector( x + (R10 + PIX3)*scale, y + ht - (CHAT_EDIT_HEIGHT - PIX3)*scale, z+5, scale, color );

    // get static info for current chat window
      if(drawChat)
    {
          int activeColorTop, activeColorBot;
         int phantom = false;
         int inactiveColor = color|(color&0xff);
          if(gActiveChatFilter && hasActiveFilter)
        {
            if( chatWindow->contextPane )    
            {
                activeColorBot = (CLR_GREEN&0xFFFFFF00)|(color&0xff); // allow active tab to only fade so much)
                activeColorTop = color|(color&0xff);
            }
            else
            {
                activeColorTop = (CLR_GREEN&0xFFFFFF00)|(color&0xff); // allow active tab to only fade so much)
                activeColorBot = color |(color&0xff);
            }
        }
        else
             activeColorBot = activeColorTop = color|(color&0xff);

        // draw phantom frame if applicable
           if( !chatWindow->divider && cursor.dragging && cursor.drag_obj.type == kTrayItemType_Tab && uiTabControlChatPhantomAllowable(chatWindow->topTabControl, &cursor.drag_obj ) )
        {
              BuildCBox( &box, x, y + ht/2, wd, ht/2 );
              if( mouseCollision(&box) )
            {
                phantom = true;
                if( chatWindow->idx == 0 )
                    drawJoinedFrame3Panel(PIX3, R10, x, y, z+1, scale, wd, chatHeight/2, chatHeight/2, CHAT_EDIT_HEIGHT*scale, (color&0xffffff00)|0x88, 0);
                else
                    drawJoinedFrame2Panel(PIX3, R10, x, y, z+1, scale, wd, chatHeight/2, chatHeight/2, (color&0xffffff00)|0x88, 0, 0,0);              
            }
        }

        // don't draw tab if window is faded out
         if( phantom )
        {
             topfilter = drawTabControl(chatWindow->topTabControl, x + R10*scale, y, z + 20, wd-(2*R10*scale),  chatHeight/2, scale, inactiveColor, activeColorTop, TabDirection_Horizontal );  
            botfilter = drawTabControl(chatWindow->botTabControl, x + R10*scale, y+chatHeight/2-PIX3*scale, z + 20, wd-(2*R10*scale), chatHeight/2, scale, inactiveColor, activeColorBot, TabDirection_Horizontal );
            if(botfilter)
            {
                chatWindow->divider = .5f;
                sendChatSettingsToServer();
            }
        }
        else
        {
            topfilter = drawTabControl(chatWindow->topTabControl, x + R10*scale, y, z + 20, wd-(2*R10*scale), chatHeight-divider_ht, scale, inactiveColor, activeColorTop, TabDirection_Horizontal );  
             botfilter = drawTabControl(chatWindow->botTabControl, x + R10*scale, y+chatHeight-divider_ht-PIX3*scale, z + 20, wd-(2*R10*scale), divider_ht, scale, inactiveColor, activeColorBot, TabDirection_Horizontal );  
        }

         if(!botfilter && chatWindow->divider)
        {
            chatWindow->divider = 0.f;
            sendChatSettingsToServer();
        }
           if(!topfilter && botfilter)
        {
            chatSendAllTabsToTop(gCurrentWindefIndex);
            chatWindow->divider = 0.f;
            sendChatSettingsToServer();
        }

        if( window_getMode(gCurrentWindefIndex)!= WINDOW_DISPLAYING )
            return 0;

        // update active window
          BuildCBox(&box, x+PIX3*scale, y, wd-(2*PIX3*scale), chatHeight-(PIX3)*scale);
         if(chatWindow->idx == 0)
               box.lowerRight.y -= (PIX3+2)*scale;    // don't collide with quick select buttons

           BuildCBox(&box, x+PIX3*scale, y, wd-(2*PIX3*scale), chatHeight-(PIX3+1)*scale-divider_ht);
           if(topfilter)
        {
            if(mouseClickHit( &box, MS_LEFT ) || mouseClickHit( &box, MS_RIGHT))
            {
                chatWindow->contextPane = 0;
                SelectActiveChatFilter(topfilter);
            }
        }
           BuildCBox(&box, x+PIX3*scale, y+(chatHeight-divider_ht)-(PIX3-1)*scale, wd-(2*PIX3*scale), divider_ht);
        if(botfilter)
        {
            if(mouseClickHit( &box, MS_LEFT ) || mouseClickHit( &box, MS_RIGHT))
            {
                chatWindow->contextPane = 1;
                SelectActiveChatFilter(botfilter);
            }
        }
     }

    if( divider_ht )
    {
          botVisHeight = divider_ht - 2*(R10+PIX3)*scale;
           topVisHeight = chatHeight - divider_ht - 2*(R10+2*PIX3)*scale;
    }
    else
        topVisHeight = chatHeight - (2*R10+2*PIX3)*scale;

    // context menu
    BuildCBox( &box, x, y, wd, chatHeight-divider_ht );
     if( mouseClickHit( &box, MS_RIGHT ) )
    {
        int rx, ry;
        rightClickCoords( &rx, &ry );
        chatWindow->contextPane = 0;
        contextMenu_set( chatWindow->menu, rx, ry, (void*) chatWindow->idx);
    }
    if(mouseClickHit(&box, MS_LEFT))
        chatWindow->contextPane = 0;

       BuildCBox( &box, x, y+chatHeight-divider_ht, wd, divider_ht-PIX3*scale );

    if( mouseClickHit( &box, MS_RIGHT ) )
    {
        int rx, ry;
        rightClickCoords( &rx, &ry );
        chatWindow->contextPane = 1;
        contextMenu_set( chatWindow->menu, rx, ry, (void*) chatWindow->idx);
    }
    if(mouseClickHit(&box, MS_LEFT))
        chatWindow->contextPane = 1;

    // If no tabs exist, place a warning message & a button for user to create one
    // Don't do it to primary chat window if it's minimized...
    if( !topfilter && !botfilter && ((chatWindow->idx != 0) || !chatWindow->minimizedOffset) && window_getMode(WDW_CHAT_BOX) == WINDOW_DISPLAYING)
    {
        UIBox area;
         int c = (CLR_WHITE & 0xFFFFFF00) | (color & 0xFF);    // use window's opacity settings
        area.x = x;
        area.y = y;
        area.width = wd;
        area.height = ht;

        font(&game_12);
        font_color(c, c);
        uiBoxAlter(&area, UIBAT_SHRINK, UIBAD_ALL, PIX3*scale);
        clipperPushRestrict(&area);
          cprnt( area.x + area.width/2, area.y + area.height/4, z+1, scale, scale, "NoTabsInWindow" );
        clipperPop();

         if(eaSize(&gChatFilters) < MAX_CHAT_TABS)
        {
            if(D_MOUSEHIT == drawStdButton(x + wd/2, y + ht/2, z+1, 78*scale, 26*scale, color, "AddTabButton", scale, !chatOptionsAvailable()))
            {
                addFilter(chatWindow);
            }
        }
    }

    font(&chatFont);
      iFontHeight = ttGetFontHeight(font_grp, scale, scale);

    // draw the edit box (master chat window only)
      if(chatWindow->idx == 0)
    {
        int mode = window_getMode(gCurrentWindefIndex);

         if( mode == WINDOW_DISPLAYING || mode == WINDOW_GROWING)
        {
              currChanWd = chatCurrentChannel( x, y + ht - CHAT_EDIT_HEIGHT*scale, z+5, scale, color );

            // Quickchat icon
             GetQuickChatZone(&xqc, &yqc, &wqc, &hqc);
             display_sprite( texQuickChat,xqc+wqc/2-(texQuickChat->width/2)*scale, yqc+hqc/2-(texQuickChat->height/2)*scale,z+5, scale, scale, color);
             display_sprite( texQuickChatOvr,xqc+wqc/2-(texQuickChatOvr->width/2)*scale, yqc+hqc/2-(texQuickChatOvr->height/2)*scale,z+6, scale, scale, CLR_WHITE);

            BuildCBox( &box, xqc, yqc, wqc, hqc);
               //drawBox(&box, z+10000, CLR_RED, 0);
             setToolTip( &chatTip[CHAT_TIP_QUICK], &box, "quickChatTip", 0, MENU_GAME, gCurrentWindefIndex );

               if( mouseDownHit( &box, MS_LEFT ) )
                cmdParse("QuickChat");

            // minimize/maximize arrow
            {
                AtlasTex * arrow;
                 Wdw * wdw = wdwGetWindow( WDW_CHAT_BOX );
                float arrowY;

                if( (chatWindow->minimizedOffset && !wdw->below) || (!chatWindow->minimizedOffset && wdw->below) )
                    arrow = atlasLoadTexture( "Jelly_arrow_up.tga" );
                else
                    arrow = atlasLoadTexture( "Jelly_arrow_down.tga" );

                 if( (!wdw->below && !wdw->flip) || (wdw->below && wdw->flip) )
                    arrowY = y - (JELLY_HT + arrow->height)*scale/2;
                else
                    arrowY = y + ht + (JELLY_HT - arrow->height)*scale/2;

                 if( D_MOUSEHIT == drawGenericButton( arrow, x + wd - 40*scale, arrowY, z, scale, (color & 0xffffff00) | (0x88), (color & 0xffffff00) | (0xff), 1 ) )
                    cmdParse( "hideprimarychat" );
            }
        }

           BuildCBox( &box, x + currChanWd, y + ht - CHAT_EDIT_HEIGHT*scale, wd-wqc-currChanWd-PIX3*scale, (CHAT_EDIT_HEIGHT-PIX3)*scale );

        if (smfBlock_HasFocus(chatEdit))
               drawFrame( PIX3, R10, x, y + ht - (CHAT_EDIT_HEIGHT + PIX3)*scale, z-1, wd, (CHAT_EDIT_HEIGHT + 4)*scale, scale, 0, 0x00ff003f );

          if (mouseClickHit(&box, MS_LEFT) && !smfBlock_HasFocus(chatEdit))
        {
            enterChatMode(1);
            smf_SetCursor(chatEdit, smfBlock_GetDisplayLength(chatEdit));
            ignore_first_enter = FALSE;
        }

        if( mouseDown( MS_LEFT ) && !mouseCollision(&box) )
            enterChatMode(0);

        if(s_fontSize!=optionGet(kUO_ChatFontSize))
        {
            s_fontSize = optionGet(kUO_ChatFontSize);
            chatFont.renderParams.renderSize = chatFont.renderParams.defaultSize = s_fontSize;
        }

        // ok now we can do the happy text, start out with the edit text panel
        if(chatEdit)
        {
            int iDelta = 8;

            if(s_fontSize>16) // When font size is 20, the delta needs to be around 6
                iDelta = 6;   // When it is 16 or less, it needs to be 8

            chatEditShouldSendInput = false;
               if (smfBlock_HasFocus(chatEdit))
            {
                chatEditInputHandler(chatEdit);
            }

            smf_SetScissorsBox(chatEdit, x+currChanWd+(2*PIX3*scale), y+ht-iFontHeight-(iDelta*scale),
                wd-currChanWd-wqc-(5+2*PIX3)*scale, CHAT_EDIT_HEIGHT*scale);
            smf_Display(chatEdit, x+currChanWd+(2*PIX3*scale), y+ht-iFontHeight-(iDelta*scale), z + 1, 
                wd-currChanWd-wqc-(5+2*PIX3)*scale, CHAT_EDIT_HEIGHT*scale, chatEditReparse, 0, &gTextAttr_Chat, 0);
            
            chatEditReparse = false;
            if (chatEditShouldSendInput)
            {
                chatEditReparse = true;
                chatEditSendInput();
            }

            // special case --- if we use a command like /quittologin, the chat settings will have been 
            // reset deep within uiEditProcess (via a cmdParse() call)
            if(gbSpecialQuitToLoginCase)
            {
                gbSpecialQuitToLoginCase = FALSE;
                return 0;
            }
        }
    }

     if( window_getMode(gCurrentWindefIndex) == WINDOW_DISPLAYING )
    {
        if(chatWindow->last_mode != WINDOW_DISPLAYING )
        {
            if(topfilter)
                topfilter->reformatText = TRUE;
            if(botfilter)
                botfilter->reformatText = TRUE;
        }
    }

      chatWindow->last_mode = window_getMode( gCurrentWindefIndex );
      if( drawChat && (topfilter||botfilter) )
    {
        ChatLine ** ppLines;
        F32 cur_y, curr_ht = 0;
        int i;
    
        F32 upperTop = y + PIX3 - 20;
        F32 upperBottom = y + PIX3*3 + chatHeight - divider_ht + 20;
        F32 lowerTop = y + PIX3 + (chatHeight-divider_ht) - 20;
        F32 lowerBottom = y + PIX3*3 + chatHeight + 21;

          gTextAttr_Chat.piScale = (int *)((int)(scale*SMF_FONT_SCALE*((F32)s_fontSize)/12.f) );

        if( topfilter )
        {
            ppLines = topfilter->chatQ.ppLines;
            for(i=0; i<eaSize(&ppLines); i++)
            {
                smf_SetScissorsBox(ppLines[i]->pBlock, x + TEXT_XOFF, y + PIX3, wd-(R10+PIX3)*2, chatHeight - PIX3*2 - 1 - divider_ht);
            }
        }

        if( divider_ht && botfilter )
        {
            ppLines = botfilter->chatQ.ppLines;
            for(i=0; i<eaSize(&ppLines); i++)
            {
                smf_SetScissorsBox(ppLines[i]->pBlock, x + TEXT_XOFF, y + PIX3 + (chatHeight-divider_ht), wd-(R10+PIX3)*2, divider_ht - PIX3*2 - 1);
            }
        }

        if(window_getMode(gCurrentWindefIndex) == WINDOW_DISPLAYING )
        {
            if(topfilter)
                chatWindow->topsb.offset = topfilter->scrollOffset;
            if(botfilter)
                chatWindow->botsb.offset = botfilter->scrollOffset;

            if( topfilter )
            {
                topfilter->docHt = 2*R10*scale;
                ppLines = topfilter->chatQ.ppLines;
    
                for(i=0; i<eaSize(&ppLines); i++)
                {
                    topfilter->docHt += ppLines[i]->ht;
                }

                topfilter->reformatText = FALSE;
                topfilter->hasNewMsg = FALSE;
            }

            if( botfilter )
            {
                botfilter->docHt = 2*R10*scale;
                ppLines = botfilter->chatQ.ppLines;

                for(i=0; i<eaSize(&ppLines); i++)
                {
                    botfilter->docHt += ppLines[i]->ht;
                }

                botfilter->reformatText = FALSE;
                botfilter->hasNewMsg = FALSE;
            }

            if( topfilter )
            {
                CBox box;

                if (dividerGrabbed)
                {
                    chatWindow->topsb.offset = topfilter->scrollOffset = INT_MAX; // force to bottom
                }

                BuildCBox(&box, x + TEXT_XOFF, y + PIX3, wd - TEXT_XOFF - PIX3, chatHeight - PIX3*2 - 1 - divider_ht );
                    doScrollBar( &chatWindow->topsb, topVisHeight, (topfilter->docHt)+(!botfilter?PIX3*scale:0), 0, (R10+PIX3)*scale, z+2, &box, 0 );
                topfilter->scrollOffset = chatWindow->topsb.offset;
            }

            if( divider_ht && botfilter )
            {
                CBox box;

                if (dividerGrabbed)
                {
                    chatWindow->botsb.offset = botfilter->scrollOffset = INT_MAX; // force to bottom
                }

                BuildCBox(&box, x + TEXT_XOFF, y + PIX3 + (chatHeight-divider_ht), wd - TEXT_XOFF - PIX3, divider_ht - PIX3*2 - 1 );
                    doScrollBar( &chatWindow->botsb, botVisHeight, (botfilter->docHt), 0, chatHeight-divider_ht+(R10+PIX3)*scale, z+2, &box, 0 );
                botfilter->scrollOffset = chatWindow->botsb.offset;
            }

            // NOTE: Let smf_ParseAndFormat determine whether you need to parse and format.
            // Call it every time, because smf_Interact() is in there now...
            if ( topfilter )//&& (topfilter->hasNewMsg || topfilter->reformatText || dividerGrabbed) )
            {
                curr_ht = 0;
                ppLines = topfilter->chatQ.ppLines;

                for(i=0; i<eaSize(&ppLines); i++)
                {
                    int scrollDiff;
                    int bReformat = topfilter->reformatText || !ppLines[i]->bParsed || !ppLines[i]->ht;
                    cur_y = y + curr_ht - chatWindow->topsb.offset + CHAT_HEADER*scale;
                    if (bReformat || (cur_y + ppLines[i]->ht > upperTop && cur_y < upperBottom))
                    {
                        if (!ppLines[i]->pBlock->editMode)
                        {
                            smf_SetRawText(ppLines[i]->pBlock, ppLines[i]->pchText, false);
                            smf_SetFlags(ppLines[i]->pBlock, SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespaceWithInverseTabbing,
                                0, 0, SMFScrollMode_ExternalOnly, 
                                SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 
                                "uiChat.c::chatWindow::topPane", 0, 0);
                        }
                        ppLines[i]->ht = smf_ParseAndFormat(ppLines[i]->pBlock, ppLines[i]->pchText, x + TEXT_XOFF, cur_y, z+1, wd-(R10+PIX3)*2, -1, true, bReformat, bReformat, &gTextAttr_Chat, 0 );
                        ppLines[i]->bParsed = 1;
                    }

                    scrollDiff = smf_GetInternalScrollDiff(ppLines[i]->pBlock);
                    topfilter->scrollOffset += scrollDiff;
                    chatWindow->topsb.offset += scrollDiff;

                    curr_ht += ppLines[i]->ht;
                }

                if (topfilter->docHt != curr_ht + 2*R10*scale && // If total text height has changed, and
                    chatWindow->topsb.offset + topVisHeight >= topfilter->docHt && // The pre-parse text height was not greater than the scroll + screen height (if it was, we were explicitly scrolled up some distance from the bottom), and
                    chatWindow->topsb.offset + topVisHeight <= curr_ht + 2*R10*scale) // The post-parse text is not less than the scroll + screen height (if it is, there's no need to do the push-to-bottom, since we already display the bottom).
                {
                    chatWindow->topsb.offset = topfilter->scrollOffset += (curr_ht + 2*R10*scale - topfilter->docHt);
                }
            }

            if ( botfilter )//&& (botfilter->hasNewMsg || botfilter->reformatText || dividerGrabbed) )
            {
                curr_ht = 0;
                ppLines = botfilter->chatQ.ppLines;

                for(i=0; i<eaSize(&ppLines); i++)
                {
                    int scrollDiff;
                    int bReformat = botfilter->reformatText || !ppLines[i]->bParsed || !ppLines[i]->ht;
                    cur_y = y + curr_ht + (chatHeight-divider_ht) - chatWindow->botsb.offset + CHAT_HEADER*scale - PIX3;
                    if (bReformat || (cur_y + ppLines[i]->ht > lowerTop && cur_y < lowerBottom))
                    {
                        if (!ppLines[i]->pBlock->editMode)
                        {
                            smf_SetRawText(ppLines[i]->pBlock, ppLines[i]->pchText, false);
                            smf_SetFlags(ppLines[i]->pBlock, SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespaceWithInverseTabbing,
                                0, 0, SMFScrollMode_ExternalOnly,
                                SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left,
                                "uiChat.c::chatWindow::bottomPane", 0, 0);
                        }
                        ppLines[i]->ht = smf_ParseAndFormat(ppLines[i]->pBlock, ppLines[i]->pchText, x + TEXT_XOFF, cur_y, z+1, wd-(R10+PIX3)*2, -1, true, bReformat, bReformat, &gTextAttr_Chat, 0 );
                        ppLines[i]->bParsed = 1;
                    }

                    scrollDiff = smf_GetInternalScrollDiff(ppLines[i]->pBlock);
                    botfilter->scrollOffset += scrollDiff;
                    chatWindow->botsb.offset += scrollDiff;

                    curr_ht += ppLines[i]->ht;
                }

                if (botfilter->docHt != curr_ht + 2*R10*scale && // If total text height has changed, and
                    chatWindow->botsb.offset + botVisHeight >= botfilter->docHt && // The pre-parse text height was not greater than the scroll + screen height (if it was, we were explicitly scrolled up some distance from the bottom), and
                    chatWindow->botsb.offset + botVisHeight <= curr_ht + 2*R10*scale) // The post-parse text is not less than the scroll + screen height (if it is, there's no need to do the push-to-bottom, since we already display the bottom).
                {
                    chatWindow->botsb.offset = botfilter->scrollOffset += (curr_ht + 2*R10*scale - botfilter->docHt);
                }
            }
        }
    
        if( topfilter )
        {
            curr_ht = 0;
            ppLines = topfilter->chatQ.ppLines;
            for(i=0; i<eaSize(&ppLines); i++)
            {
                F32 cur_y = y + curr_ht - chatWindow->topsb.offset + CHAT_HEADER*scale;
                if( cur_y + ppLines[i]->ht > upperTop && cur_y < upperBottom ) // 120 is fudge factor
                    curr_ht += smf_Display(ppLines[i]->pBlock, x + TEXT_XOFF, cur_y, z+1, wd-(R10+PIX3)*2, -1, 
                                            0, 0, &gTextAttr_Chat, 0); 
                else
                    curr_ht += ppLines[i]->ht;
            }
        }

        if( divider_ht && botfilter )
        {
            curr_ht = 0;
            ppLines = botfilter->chatQ.ppLines;
            for(i=0; i<eaSize(&ppLines); i++)
            {
                F32 cur_y = y + curr_ht + (chatHeight-divider_ht) - chatWindow->botsb.offset + CHAT_HEADER*scale - PIX3;
                if( cur_y + ppLines[i]->ht > lowerTop && cur_y < lowerBottom )
                    curr_ht += smf_Display(ppLines[i]->pBlock, x + TEXT_XOFF, cur_y, z+1, wd-(R10+PIX3)*2, -1, 
                                            0, 0, &gTextAttr_Chat, 0); 
                else
                    curr_ht += ppLines[i]->ht;
            }
        }
    }


    return 0;
}

void setHeroVillainEventChannel(int isHero)
{
    int i;
    //loop through all windows
    for (i = 0; i < MAX_CHAT_WINDOWS; ++i)
    {
        ChatWindow * window = GetChatWindow(i);
        uiTabControl * chatTabControls[2] = {window->topTabControl, window->botTabControl};
        int j;

        //loop through top and bottom
        for (j = 0; j < 2; ++j)
        {
            ChatFilter * filter = uiTabControlGetFirst(chatTabControls[j]);
            // loop through all tabs
            while (filter)
            {
                if (!isHero)
                {
                    // only change if hero events are shown in this filter
                    if (BitFieldGet(filter->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_EVENT_HERO))
                    {
                        BitFieldSet(filter->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_EVENT_HERO, 0);
                        BitFieldSet(filter->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_EVENT_VILLAIN, 1);
                    }
                }
                else
                {
                    // only change if villain events are shown in this filter
                    if (BitFieldGet(filter->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_EVENT_VILLAIN))
                    {
                        BitFieldSet(filter->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_EVENT_HERO, 1);
                        BitFieldSet(filter->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_EVENT_VILLAIN, 0);
                    }
                }
                filter = uiTabControlGetNext(chatTabControls[j]);
            }
        }
    }

    sendChatSettingsToServer();
}

