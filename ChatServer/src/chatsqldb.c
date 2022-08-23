#include <utilitieslib/stdtypes.h>
#include "chatsqldb.h"
#include "chatdb.h"
#include "ChatSql.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/log.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/EString.h>

typedef struct ChatDb
{
    User **users;
    Channel **channels;
    UserGmail **user_gmail;

    // used to check consistency
    U32 uUsersVer; // CRC of ParseTable
    U32 uChannelsVer;
    U32 uUserGmailVer;

    // not persisted/parsed
    StashTable usersByAuthId;
    StashTable channelsByName;
    StashTable userGmailByAuthId;

    // dirty objects
    User **dirty_users;
    Channel **dirty_channels;
    UserGmail **dirty_user_gmail;
} ChatDb;
ChatDb g_ChatDb;

void ChatDb_LoadUsers(void)
{
    U32 pti_crc;
    int i;

    if (g_ChatDb.users)
    {
        FatalErrorf("ChatDb_LoadUsers called twice");
    }

    if (!chatsql_read_users_ver(&g_ChatDb.uUsersVer))
    {
        FatalErrorf("Could not read version number from ChatServer SQL DB! Please see console/logs.");
    }

    pti_crc = ParseTableCRC(parse_user, NULL);
    if(g_ChatDb.uUsersVer != pti_crc)
    {
        LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: Users had its parse table changed, ignoring crc.  This is normal for a new publish.");
        printf("Updating parse table version for users.\n");
        chatsql_write_users_ver(pti_crc);
    }

    if(!chatsql_read_users(parse_user, &g_ChatDb.users))
    {
        FatalErrorf("users table could not be read! Could not load. Please see console/logs.");
    }

    g_ChatDb.usersByAuthId = stashTableCreateInt(2000000);
    for (i = 0; i < eaSize(&g_ChatDb.users); ++i)
    {
        if (!stashIntAddPointer(g_ChatDb.usersByAuthId, g_ChatDb.users[i]->auth_id, g_ChatDb.users[i], false))
        {
            FatalErrorf("User found twice!");
        }
    }
}

void ChatDb_LoadChannels(void)
{
    U32 pti_crc;
    int i;

    if (g_ChatDb.channels)
    {
        FatalErrorf("ChatDb_LoadChannels called twice");
    }

    if (!chatsql_read_channels_ver(&g_ChatDb.uChannelsVer))
    {
        FatalErrorf("Could not read version number from ChatServer SQL DB! Please see console/logs.");
    }

    pti_crc = ParseTableCRC(parse_channel, NULL);
    if(g_ChatDb.uChannelsVer != pti_crc)
    {
        LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: Channels had its parse table changed, ignoring crc.  This is normal for a new publish.");
        printf("Updating parse table version for channels.\n");
        chatsql_write_channels_ver(pti_crc);
    }

    if(!chatsql_read_channels(parse_channel, &g_ChatDb.channels))
    {
        FatalErrorf("channels table could not be read! Could not load. Please see console/logs.");
    }

    g_ChatDb.channelsByName = stashTableCreateWithStringKeys(40000, StashDefault);
    for (i = 0; i < eaSize(&g_ChatDb.channels); ++i)
    {
        if (!stashAddPointer(g_ChatDb.channelsByName, g_ChatDb.channels[i]->name, g_ChatDb.channels[i], false))
        {
            FatalErrorf("Channel found twice!");
        }
    }
}

void ChatDb_LoadUserGmail(void)
{
    U32 pti_crc;
    int i;

    if (g_ChatDb.user_gmail)
    {
        FatalErrorf("ChatDb_LoadUserGmail called twice");
    }

    if (!chatsql_read_user_gmail_ver(&g_ChatDb.uUserGmailVer))
    {
        FatalErrorf("Could not read version number from ChatServer SQL DB! Please see console/logs.");
    }

    pti_crc = ParseTableCRC(parse_gmailuser, NULL);
    if(g_ChatDb.uUserGmailVer != pti_crc)
    {
        LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: user_gmail had its parse table changed, ignoring crc.  This is normal for a new publish.");
        printf("Updating parse table version for user_gmail.\n");
        chatsql_write_user_gmail_ver(pti_crc);
    }

    if(!chatsql_read_user_gmail(parse_gmailuser, &g_ChatDb.user_gmail))
    {
        FatalErrorf("user_gmail table could not be read! Could not load. Please see console/logs.");
    }

    g_ChatDb.userGmailByAuthId = stashTableCreateInt(2000000);
    for (i = 0; i < eaSize(&g_ChatDb.user_gmail); ++i)
    {
        if (!stashIntAddPointer(g_ChatDb.userGmailByAuthId, g_ChatDb.user_gmail[i]->auth_id, g_ChatDb.user_gmail[i], false))
        {
            FatalErrorf("UserGmail found twice!");
        }
    }
}

void ChatDb_FlushChanges(void)
{
    int i;
    static char *estr = NULL;

    for (i = 0; i < eaSize(&g_ChatDb.dirty_users); i++)
    {
        User *user = g_ChatDb.dirty_users[i];
        estrClear(&estr);
        if (devassert(ParserWriteText(&estr,parse_user,user,0,0)))
            devassert(chatsql_insert_or_update_user(user->auth_id, &estr));
        user->dirty = 0;
    }

    for (i = 0; i < eaSize(&g_ChatDb.dirty_channels); i++)
    {
        Channel *channel = g_ChatDb.dirty_channels[i];
        estrClear(&estr);
        if (devassert(ParserWriteText(&estr,parse_channel,channel,0,0)))
            devassert(chatsql_insert_or_update_channel(channel->name, &estr));
        channel->dirty = 0;
    }

    for (i = 0; i < eaSize(&g_ChatDb.dirty_user_gmail); i++)
    {
        UserGmail *gmuser = g_ChatDb.dirty_user_gmail[i];
        estrClear(&estr);
        if (devassert(ParserWriteText(&estr,parse_gmailuser,gmuser,0,0)))
            devassert(chatsql_insert_or_update_user_gmail(gmuser->auth_id, &estr));
        gmuser->dirty = 0;
    }

    eaSetSize(&g_ChatDb.dirty_users, 0);
    eaSetSize(&g_ChatDb.dirty_channels, 0);
    eaSetSize(&g_ChatDb.dirty_user_gmail, 0);
}

User *chatUserFind(U32 auth_id)
{
    User *user;
    return stashIntFindPointer(g_ChatDb.usersByAuthId, auth_id, &user) ? user : NULL;
}

void chatUserInsertOrUpdate(User *user)
{
    if (devassert(user))
    {
        if (!stashIntFindPointer(g_ChatDb.usersByAuthId, user->auth_id, NULL))
        {
            eaPush(&g_ChatDb.users, user);
            stashIntAddPointer(g_ChatDb.usersByAuthId, user->auth_id, user, false);
        }

        if (!user->dirty)
        {
            user->dirty = 1;
            eaPush(&g_ChatDb.dirty_users, user);
        }
    }
}

User **chatUserGetAll(void)
{
    return g_ChatDb.users;
}

U32 chatUserGetCount(void)
{
    return eaSize(&g_ChatDb.users);
}

Channel *chatChannelFind(const char *name)
{
    Channel *channel;
    return stashFindPointer(g_ChatDb.channelsByName, name, &channel) ? channel : NULL;
}

void chatChannelInsertOrUpdate(Channel *channel)
{
    if (devassert(channel))
    {
        if (!stashFindPointer(g_ChatDb.channelsByName, channel->name, NULL))
        {
            eaPush(&g_ChatDb.channels, channel);
            stashAddPointer(g_ChatDb.channelsByName, channel->name, channel, false);
        }

        if (!channel->dirty)
        {
            channel->dirty = 1;
            eaPush(&g_ChatDb.dirty_channels, channel);
        }
    }
}

void chatChannelDelete(Channel *channel)
{
    devassert(eaFindAndRemove(&g_ChatDb.channels, channel) != -1);
    if (channel->dirty)
        devassert(eaFindAndRemove(&g_ChatDb.dirty_channels, channel) != -1);

    devassert(stashRemovePointer(g_ChatDb.channelsByName, channel->name, NULL) != -1);
    devassert(chatsql_delete_channel(channel->name) != -1);

    StructDestroy(parse_channel, channel);
}

Channel **chatChannelGetAll(void)
{
    return g_ChatDb.channels;
}

U32 chatChannelGetCount(void)
{
    return eaSize(&g_ChatDb.channels);
}

UserGmail *chatUserGmailFind(U32 auth_id)
{
    UserGmail *gmuser;
    return stashIntFindPointer(g_ChatDb.userGmailByAuthId, auth_id, &gmuser) ? gmuser : NULL;
}

void chatUserGmailInsertOrUpdate(UserGmail *gmuser)
{
    if (devassert(gmuser))
    {
        if (!stashIntFindPointer(g_ChatDb.userGmailByAuthId, gmuser->auth_id, NULL))
        {
            eaPush(&g_ChatDb.user_gmail, gmuser);
            stashIntAddPointer(g_ChatDb.userGmailByAuthId, gmuser->auth_id, gmuser, false);
        }

        if (!gmuser->dirty)
        {
            gmuser->dirty = 1;
            eaPush(&g_ChatDb.dirty_user_gmail, gmuser);
        }
    }
}
