#include "entity/chatSettings.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/language/MessageStore.h>
#include "entity/entVarUpdate.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/bitfield.h>
#include "entity/character_base.h"
#if SERVER
#include "gameComm/automapServer.h"
#endif

static void addCombatChannels( Entity *e );
static void addHelpChatTab(Entity * e);

typedef enum ChannelDefault
{
    channelDefault_Global,
    channelDefault_Chat,
    channelDefault_Help,
    channelDefault_Combat,
    channelDefault_PetCombat,
    channelDefault_Old,
}ChannelDefault;

static void setDefaultChannels( int * bitfield, int default_type, int isOnBlueSide, int isPraetorian )
{
    switch( default_type )
    {
        xcase channelDefault_Global:
        {    
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_SVR_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_VILLAIN_SAYS, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_NPC_SAYS, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_AUCTION, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_ARCHITECT, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_NEARBY_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_SHOUT_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_REQUEST_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_ADMIN_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_USER_ERROR, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_DEBUG_INFO, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_GMTELL, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_PET_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_REWARD, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_CAPTION, 1 );
            if( isPraetorian )
                BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_EVENT_PRAETORIAN, 1 );
            else if (isOnBlueSide)
                BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_EVENT_HERO, 1 );
            else
                BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_EVENT_VILLAIN, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_LOOKING_FOR_GROUP, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_HELP, 1 );
        }
        xcase channelDefault_Chat:
        {
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_PRIVATE_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_PRIVATE_NOREPLY_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_TEAM_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_SUPERGROUP_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_ALLIANCE_OWN_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_ALLIANCE_ALLY_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_NEARBY_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_SHOUT_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_FRIEND_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_ADMIN_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_EMOTE, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_GMTELL, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_LEVELINGPACT_COM, 1);
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_LEAGUE_COM, 1);
        }
        xcase channelDefault_Help:
        {
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_HELP, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_SVR_COM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_GMTELL, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_LOOKING_FOR_GROUP, 1 );
        }
        xcase channelDefault_Combat:
        {
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_COMBAT, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_DAMAGE, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_USER_ERROR, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_COMBAT_SPAM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_COMBAT_ERROR, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_HEAL, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_HEAL_OTHER, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_GMTELL, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_COMBAT_HITROLLS, 1 );
        }
        xcase channelDefault_PetCombat:
        {
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_PET_COMBAT, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_PET_DAMAGE, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_PET_COMBAT_SPAM, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_PET_HEAL, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_PET_HEAL_OTHER, 1 );
            BitFieldSet( bitfield, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_PET_COMBAT_HITROLLS, 1 );
        }
    }
}



// Both windows get "Local" chat automatically assigned, and the default output channel is "Local".
static void createDefaultChannel(Entity * e, int channelType, int oldChannels, int tabIdx, int bottomPane, const char * name)
{
    ChatSettings * settings = &e->pl->chat_settings;
    ChatTabSettings * tab = &settings->tabs[tabIdx];

    // setup window
    settings->windows[0].tabListBF |= (1 << tabIdx);
    settings->windows[0].divider = .5f;

    // setup tab    
    strncpyt(tab->name, name, MAX_TAB_NAME_LEN);
    if( oldChannels )
    {
        int i;
        for( i = 0; i < 32; i++ )
            BitFieldSet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, i, (oldChannels&(1<<i)) ); 
    }
    else
        setDefaultChannels(tab->systemChannels, channelType, ENT_IS_ON_BLUE_SIDE(e), ENT_IS_PRAETORIAN(e) );

    if(bottomPane)
        tab->optionsBF = (1<<ChannelOption_Bottom);
    else
        tab->optionsBF = (1<<ChannelOption_Top);

    // try to set some reasonable default input channels, otherwise don't set default channel
    tab->defaultType = ChannelType_System;
    if( BitFieldGet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_NEARBY_COM) )
    {
        tab->defaultChannel = INFO_NEARBY_COM;
    }
    else if( BitFieldGet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_TEAM_COM) )
    {
        tab->defaultChannel = INFO_TEAM_COM;
    }
    else if(BitFieldGet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_HELP) )
    {
        tab->defaultChannel = INFO_HELP;
    }
}

// create top & bottom channels
static void createDefaultChatSettings(Entity * e, int topChannels, int botChannels)
{
    memset(&e->pl->chat_settings, 0, sizeof(ChatSettings));

    if(!topChannels && !botChannels ) // new player
    {
        createDefaultChannel( e, channelDefault_Global, 0, 0, 0, "DefaultGlobalChat");
        createDefaultChannel( e, channelDefault_Chat, 0, 1, 1, "DefaultChatChat");
    }
    else // old users converting to new system..
    {
        createDefaultChannel( e, 0, e->pl->topChatChannels, 0, 0, "DefaultTopChat");
        createDefaultChannel( e, 0, e->pl->botChatChannels, 1, 1, "DefaultBotChat");
    }

    createDefaultChannel( e, channelDefault_Help, 0, 2, 0, "DefaultHelpChat");
    createDefaultChannel( e, channelDefault_Combat, 0, 3, 0, "DefaultCombatChat");
    if(stricmp( e->pchar->pclass->pchName, "Class_Mastermind")== 0)
    {
        createDefaultChannel( e, channelDefault_PetCombat, 0, 4, 0, "DefaultPetCombat");
    }
    e->pl->chatSendChannel = INFO_TAB;
    e->pl->chat_settings.options |= CSFlags_AddedLegacyUINote;
}

static void addHelpChatTab(Entity * e)
{
    ChatSettings * settings = &e->pl->chat_settings;
    int i, addIdx = -1;

    e->pl->helpChatAdded = true; // even if we don't add it, mark that we tried

    // if help channel is already in any tab, forget it
    for( i = 0; i < MAX_CHAT_TABS; i++ )
    {
        ChatTabSettings * tab = &settings->tabs[i];
        if( BitFieldGet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_HELP) )
            return;
        if( !BitFieldCount(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, 1) && addIdx == -1 ) // no channels and we haven't found empty tab yet
            addIdx = i;
    }
#ifdef SERVER

    if( addIdx >= 0 )
        createDefaultChannel( e, channelDefault_Help, 0, addIdx, 0, "DefaultHelpChat" );

#endif
}

void updatePraetorianEventChannel(Entity *e)
{
    ChatSettings * settings = &e->pl->chat_settings;
    int i;

    // if help channel is already in any tab, forget it
    for( i = 0; i < MAX_CHAT_TABS; i++ )
    {
        ChatTabSettings * tab = &settings->tabs[i];
        if( BitFieldGet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_EVENT_PRAETORIAN) )
        {
            BitFieldSet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_EVENT_PRAETORIAN,0);
            if(ENT_IS_ON_BLUE_SIDE(e))
                BitFieldSet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_EVENT_HERO,1);
            else
                BitFieldSet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE,INFO_EVENT_VILLAIN,1);
            return;
        }
    }
}

// this adds the new combat filters to any tab that had 
static void addCombatChannels( Entity *e )
{
    ChatSettings * settings = &e->pl->chat_settings;
    int i;

    for(i=0;i<MAX_CHAT_TABS;i++)
    {
        ChatTabSettings * tab = &settings->tabs[i];

        if( BitFieldGet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_COMBAT) )
        {
            BitFieldSet( tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_COMBAT_SPAM, 1 );
            BitFieldSet( tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_COMBAT_ERROR, 1 );
            BitFieldSet( tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_HEAL, 1 );
            BitFieldSet( tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_HEAL_OTHER, 1 );
            BitFieldSet( tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_REWARD, 1 );
        }

        if( BitFieldGet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_SVR_COM) )
            BitFieldSet( tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_REWARD, 1 );
    }

    e->pl->chat_settings.options |= CSFlags_AddedCombat;
}


// only used by server to get updated active tab/window index
void receiveSelectedChatTabs(Packet * pak, Entity * e)
{
    int i;
    ChatSettings * settings = &e->pl->chat_settings;

    // selected tabs for each window...
    for(i=0;i<MAX_CHAT_WINDOWS;i++)
    {
        e->pl->chat_settings.windows[i].selectedtop = pktGetBitsPack(pak, 1);
        e->pl->chat_settings.windows[i].selectedbot = pktGetBitsPack(pak, 1);
    }


    i = pktGetBitsPack(pak, 1);
    i = MINMAX(i, 0, (MAX_CHAT_WINDOWS - 1));

    if( pktGetBits(pak, 1) )
        settings->options |= CSFlags_BottomDividerSelected;
    else
        settings->options &= ~(CSFlags_BottomDividerSelected);

    settings->options &= ( ~(CSFlags_ActiveChatWindowIdx));      // clear out old settings
    settings->options |= (i << 2) & CSFlags_ActiveChatWindowIdx;
}

#ifdef SERVER
void unpackChatSettings(Entity *e)
{
    ChatSettings * settings = &e->pl->chat_settings;
    int i;

    memset(settings->systemChannels, 0, sizeof(U32)*SYSTEM_CHANNEL_BITFIELD_SIZE);    // we'll build it up as we send info
    for(i=0;i<MAX_CHAT_TABS;i++)
    {
        ChatTabSettings * tab = &settings->tabs[i];
        if(tab->systemBF)
            BitFieldOr(&tab->systemBF, tab->systemChannels, 1);
        tab->systemBF = 0;
        BitFieldOr(tab->systemChannels, settings->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE);
    }
}
#endif

void receiveChatSettings( Packet *pak, Entity *e )
{
    //    int top, bot;
    ChatSettings * settings = &e->pl->chat_settings;
    int i;

#ifdef SERVER
    memset(settings->systemChannels, 0, sizeof(U32)*SYSTEM_CHANNEL_BITFIELD_SIZE);    // we'll build it up as we receive info
#endif

    // OLD CHAT STUFF
    // TODO: move to chat settings struct
#ifdef CLIENT 
    e->pl->helpChatAdded = pktGetBits(pak,1); 
#endif

    e->pl->chatSendChannel = pktGetBitsPack( pak, 1 );
    settings->userSendChannel = pktGetBitsPack(pak, 1);

    // NEW CHAT STUFF
    settings->options = pktGetBitsPack(pak, 1);
    settings->primaryChatMinimized = pktGetBitsPack(pak, 1);

    for(i=0;i<MAX_CHAT_WINDOWS;i++)
    {
        settings->windows[i].selectedtop  = pktGetBitsPack(pak, 1);
        settings->windows[i].selectedbot  = pktGetBitsPack(pak, 1);
        settings->windows[i].tabListBF = pktGetBitsPack(pak, 1);
        settings->windows[i].divider = pktGetF32( pak );
    }

    for(i=0;i<MAX_CHAT_TABS;i++)
    {
        ChatTabSettings * tab = &settings->tabs[i];

        strncpyt(tab->name, pktGetString(pak), SIZEOF2(ChatTabSettings, name));
        pktGetBitsArray(pak, SYSTEM_CHANNEL_BITFIELD_SIZE*32, &tab->systemChannels );
        tab->userBF            = pktGetBitsPack(pak, 1);
        tab->optionsBF        = pktGetBitsPack(pak, 1);
        tab->defaultChannel    = pktGetBitsPack(pak, 1);
        tab->defaultType    = pktGetBitsPack(pak, 1);
#ifdef SERVER
        BitFieldOr(tab->systemChannels, settings->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE);    // not sent across network, but server needs a local copy for quick filtering
#endif 
    }

    for(i=0;i<MAX_CHAT_CHANNELS;i++)
    {
        ChatChannelSettings * channel = &settings->channels[i];

        strncpyt(channel->name, pktGetString(pak), SIZEOF2(ChatChannelSettings, name));
        channel->optionsBF = pktGetBitsPack(pak, 1);
        channel->color1 = pktGetBits(pak, 32);
        channel->color2 = pktGetBits(pak, 32);
    }
}



void sendChatSettings( Packet *pak, Entity *e )
{
    ChatSettings * settings = &e->pl->chat_settings;
    int i;
    int debugTabFlags = 0;

    // handle default settings -- this also conveniently converts old players to the new chat system
    if(! (e->pl->chat_settings.options & CSFlags_DoNotLocalize))
    {
        memset(settings, 0, sizeof(ChatSettings));
        // convert channels
        createDefaultChatSettings(e, e->pl->topChatChannels, e->pl->botChatChannels );
    }

    if(!(e->pl->chat_settings.options & CSFlags_AddedCombat ) )
        addCombatChannels(e);

    if(!(e->pl->chat_settings.options & CSFlags_AddedArchitect))
    {
        for(i = 0; i < MAX_CHAT_TABS; i++)
        {
            ChatTabSettings * tab = &settings->tabs[i];
            if( BitFieldGet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_SVR_COM) )
                BitFieldSet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_ARCHITECT, 1);
        }
        e->pl->chat_settings.options |= CSFlags_AddedArchitect;
    }

    if(!(e->pl->chat_settings.options & CSFlags_AddedLeague))
    {
        for(i = 0; i < MAX_CHAT_TABS; i++)
        {
            ChatTabSettings * tab = &settings->tabs[i];
            if( BitFieldGet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_SVR_COM) )
                BitFieldSet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_LEAGUE_COM, 1);
        }
        e->pl->chat_settings.options |= CSFlags_AddedLeague;
    }

    // This block doesn't make a ton of sense, but I'm hijacking the old turnstile server channel
    // for the new player chat Looking for Group channel.
    if(!(e->pl->chat_settings.options & CSFlags_AddedLookingForGroup))
    {
        for(i = 0; i < MAX_CHAT_TABS; i++)
        {
            ChatTabSettings * tab = &settings->tabs[i];
            if( BitFieldGet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_SVR_COM) )
                BitFieldSet(tab->systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, INFO_LOOKING_FOR_GROUP, 1);
        }
        e->pl->chat_settings.options |= CSFlags_AddedLookingForGroup;
    }

#ifdef SERVER
    if(!(e->pl->chat_settings.options & CSFlags_MovedVisitedMaps))
    {
        automapserver_sendAllStaticMaps(e);
        e->pl->chat_settings.options |= CSFlags_MovedVisitedMaps;
    }
    if(!(e->pl->chat_settings.options & CSFlags_ClearMARTYHistory))
    {
        clearMARTYHistory(e);
        e->pl->chat_settings.options |= CSFlags_ClearMARTYHistory;
    }
    if( !e->pl->helpChatAdded )
    {
        addHelpChatTab(e);
        pktSendBits(pak,1,1);
    }
    else
        pktSendBits(pak,1,0);
#endif



    // TODO: move to chat settings struct
    pktSendBitsPack( pak, 1, e->pl->chatSendChannel);
    pktSendBitsPack( pak, 1, settings->userSendChannel);

    pktSendBitsPack( pak, 1, settings->options);
    pktSendBitsPack( pak, 1, settings->primaryChatMinimized); 

    for(i=0;i<MAX_CHAT_WINDOWS;i++)
    {
        pktSendBitsPack(pak, 1, settings->windows[i].selectedtop);
        pktSendBitsPack(pak, 1, settings->windows[i].selectedbot);
        pktSendBitsPack(pak, 1, settings->windows[i].tabListBF);
        pktSendF32(pak, settings->windows[i].divider );

        // debug code to find error with overlapping tab flags
#ifdef CLIENT
        assert(! (debugTabFlags & settings->windows[i].tabListBF)); // make sure all of our flags are unique
#endif 
        debugTabFlags |= settings->windows[i].tabListBF;
    }

    for(i=0;i<MAX_CHAT_TABS;i++)
    {
        ChatTabSettings * tab = &settings->tabs[i];

        pktSendString(pak, tab->name);
        pktSendBitsArray(pak,SYSTEM_CHANNEL_BITFIELD_SIZE*32, tab->systemChannels);
        pktSendBitsPack(pak, 1, tab->userBF);
        pktSendBitsPack(pak, 1, tab->optionsBF);
        pktSendBitsPack(pak, 1, tab->defaultChannel);
        pktSendBitsPack(pak, 1, tab->defaultType);
    }

    for(i=0;i<MAX_CHAT_CHANNELS;i++)
    {
        ChatChannelSettings * channel = &settings->channels[i];

        pktSendString(pak, channel->name);
        pktSendBitsPack(pak, 1, channel->optionsBF);
        pktSendBits(pak, 32, channel->color1 );
        pktSendBits(pak, 32, channel->color2 );
    }
}



// returns pointer to beginning of handle if an @ is found before the first non-whitespace character.
// Else, returns NULL
// Valid handle begins with first non-whitespace character immediately after '@' character, ignoring whitespace in front of & after '@' character
//
const char * getHandleFromString(const char * str)
{
    if(!str)
        return 0;

    while(*str)
    {
        if(*str == '@')
        {
            // found an asterisk, skip over whitespace & return
            do{
                ++str;
                if(! *str)
                    return 0;
                else if(!isspace((unsigned char)*str))
                    return str;
            } while(1);
        }
        else if(!isspace((unsigned char)*str))
            return 0;

        ++str;
    }

    return 0;
}


bool isWatchedChannel( Entity *e, int type )
{
    if(!e || !e->pl)
        return 0;

    if( type==INFO_ADMIN_COM || type==INFO_GMTELL || type==INFO_PET_SAYS ||
        type==INFO_PET_COM || type==INFO_ARENA || type==INFO_ARENA_ERROR || 
        type==INFO_CAPTION || type==INFO_PRIVATE_NOREPLY_COM)
    {
        return 1;
    }

#if SERVER
    return BitFieldGet(e->pl->chat_settings.systemChannels, SYSTEM_CHANNEL_BITFIELD_SIZE, type);
#endif
    return 0;
}
