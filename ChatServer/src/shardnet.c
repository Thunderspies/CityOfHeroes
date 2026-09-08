#include <conio.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include "loadsave.h"
#include "users.h"
#include "channels.h"
#include "comm_backend.h"
#include "friends.h"
#include "csr.h"
#include "ignore.h"
#include <utilitieslib/components/earray.h>
#include "msgsend.h"
#include <utilitieslib/network/net_masterlist.h>
#include "filter/profanity.h"
#include "shardnet.h"
#include <utilitieslib/components/EString.h>
#include "filter/reserved_names.h"
#include "monitor.h"
#include "admin.h"
#include <utilitieslib/network/net_linklist.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/language/AppLocale.h>
#include "messagefile.h"
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/utils/sysutil.h>
#include "chatsqldb.h"
#include <utilitieslib/utils/textparser.h>

#define LOGID "shardnet"
#define ENABLE_LOGGING 0

char    g_exe_name[MAX_PATH];

int g_chatLocale;
ChatServerCfg g_chatcfg;

ChatStats g_stats;

NetLinkList    net_links;
NetLinkList aux_links;

void shardDisconnectAll()
{
    netLinkListDisconnect(&aux_links);
    netLinkListDisconnect(&net_links);
}

#define INITIAL_CLIENT_COUNT 2000

StashTable    cmd_hashes;

char *getShardName(NetLink *link)
{
    ClientLink    *client;

    if (!link)
        return "";
    client = link->userData;
    return client->shard_name;
}

int clientCreateCb(NetLink *link)
{
    ClientLink    *client = link->userData;

    client->link = link;
    netLinkSetMaxBufferSize(link, BothBuffers, 8*1024*1024); // Set max size to auto-grow to
    netLinkSetBufferSize(link, BothBuffers, 1*1024*1024);

    client->user_db_ids = stashTableCreateInt(4);

    return 1;
}

int clientDeleteCb(NetLink *link)
{
    ClientLink    *client = link->userData;

    if (client->aggregate_pak)
    {
        pktFree(client->aggregate_pak);
        client->aggregate_pak = 0;
    }
    userLogoutByLink(link);

    eaFindAndRemove(&admin_clients, client);
    if(client == admin_upload)
        admin_upload = 0;

    return 1;
}

static int s_shutting_down = 0;

void chatServerShutdown(User * unused, char * msg)
{
    if (!s_shutting_down)
    {
        s_shutting_down = 1;
        printf("Received shutdown command...\n");

        if(msg)
        {
            printf("\tSending shutdown message to players (%s)...\n", msg);
            csrSendAllAnon(msg);
        }
    }
}

int freeAuthId()
{
    static int    i=1;

    for(;;i++)
    {
        if (!userFindByAuthId(i))
            return i;
    }
}

static User *addUsers()
{
    int        i,j,id;
    char    name[100],chan_name[32],friend_name[100];
    User    *user,*myfriend;

    for(i=0;i<1000;i++)
    {
        id = freeAuthId();
        sprintf(name,"username%d",id);
        user = userAdd(freeAuthId(),name);

        sprintf(chan_name,"chan%d",i);
        channelCreate(user,chan_name,"");
        sprintf(chan_name,"chan%d",i%25);
        channelJoin(user,chan_name,"");
        sprintf(chan_name,"chan%d",1+i/1000);
        channelJoin(user,chan_name,"");

        for(j=id-10;j<id;j++)
        {
            sprintf(friend_name,"username%d",j);
            myfriend = userFindByAuthId(j);
            if (myfriend)
            {
                friendRequest(user,friend_name);
                friendRequest(myfriend,name);
                userSend(user,friend_name,"alksjdhfaskldjfhasdkljfhalksdjfhlakjsdhflkajsdalksjdhfaskldjfhasdkljfhalksdjfhlakjsdhflkajsdalksjdhfaskldjfhasdkljfhalksdjfhlakjsdhflkajsdalksjdhfaskldjfhasdkljfhalksdjfhlakjsdhflkajsdalksjdhfaskldjfhasdkljfhalksdjfhlakjsdhflkajsdalksjdhfaskldjfhasdkljfhalksdjfhlakjsdhflkajsd");
            }
        }
    }
    return user;
}

static int shardLogin(ClientLink *client,int auth_id,char *shardname)
{
    if (!client->shard_name[0])
        strncpy(client->shard_name,shardname,sizeof(client->shard_name));
    client->linkType = kChatLink_Shard;
    return 1;
}

static int gameLogin(ClientLink *client,int auth_id,char *name,U32 hash[4],char *access_level_str, char *db_id, char *hidestate)
{
    User    *user        = userFindByAuthId(auth_id);
    int access_level    = atoi(access_level_str);
    int id, hidefield =0;
    
    if (!user)
        user = userAdd(auth_id,name);

    id = (user->publink_id ? user->publink_id : user->auth_id);

#if 0 // CD: these are never handled by the client, no need to send them.
    if (user->link && id)
    {    
        sendMsg(user->link,id,"KickedByGameLogin");
        sendMsg(user->link,id,"Disconnect");
    }
#endif

    user->publink_id = 0;
    // only save password if they have admin status
    if( ((access_level < 0 && user->access & CHANFLAGS_ADMIN) || access_level > 0) && (0 != memcmp(user->hash, hash, sizeof(user->hash))))
    {
        memcpy(user->hash, hash, sizeof(user->hash));
        chatUserInsertOrUpdate(user);
    }

    userLogin(user,client->link,client->linkType,access_level,atoi(db_id));

    // This is super hackey way of setting initial hide state
    hidefield = atoi( hidestate );
    if( hidefield & (1<<4) )
        userInvisible(user);
    else
        userVisible(user);

    if( hidefield & (1<<3) )
        userFriendHide(user);
    else
        userFriendUnHide(user);

    
    return 1;
}

bool verifyPassword(User * user, U32 hash[4])
{
    return(0 == memcmp(hash, user->hash, SIZEOF2(User, hash)));
}


static int pubLogin(ClientLink *client,int publink_id,char *name,U32 hash[4])
{
    char    buf[200];
    User    *user = userFindByName(name);
    NetLink    *link = client->link;

    if (!user)
        return 0;
    
    if (user->link)
    {
        if (!user->publink_id)
        {
            sendMsg(link,publink_id,"AlreadyConnectedInGame");
            return 0;
        }
        else
        {
            sendMsg(link,user->publink_id,"KickedByNewConnection");
            sendMsg(link,user->publink_id,"Disconnect");
        }
    }
    sprintf(buf,"AuthId %d",user->auth_id);
    sendMsg(link,publink_id,buf);
    user->publink_id = publink_id;
    userLogin(user,link,client->linkType,-1,-1);
    return 1;
}

void AdminLoginFail(NetLink * link, char * msg)
{
    char buf[1000];
    strcpy(buf, "AdminLoginFail ");
    strcat(buf, msg);
    sendMsg(link,0,buf);
    msgFlushPacket(link);
    netSendDisconnect(link, 0);
}

static int adminLogin(ClientLink *client,int publink_id,char *name,U32 hash[4])
{
    User    *user = userFindByName(name);
    NetLink    *link = client->link;

    if (!user)
    {
        AdminLoginFail(link, "AdminBadHandle");
        return 0;
    }

    if(!verifyPassword(user, hash))
    {
        AdminLoginFail(link, "AdminBadPassword");
        return 0;
    }

    if(! (user->access & CHANFLAGS_ADMIN))
    {
        AdminLoginFail(link,"AdminBadAccessLevel");
        return 0;
    }

    if(adminUserOnline(user))
    {
        AdminLoginFail(link,"AlreadyConnectedViaChatAdmin");
        return 0;
    }
    else if (user->link)
    {
        if (!user->publink_id)
        {
            AdminLoginFail(link,"AlreadyConnectedInGame");
            return 0;
        }
        else
        {
            sendMsg(user->link,user->publink_id,"KickedByNewConnection");
            sendMsg(user->link,user->publink_id,"Disconnect");
        }
    }

    client->linkType        = kChatLink_Admin;
    client->uploadStatus    = UPLOAD_READY;
    client->uploadHint        = 0;
    client->adminUser        = user;

    eaPush(&admin_clients, client);

    // user is actually logged in once they receive user & channel lists (see adminTick())

    return 1;
}


void adminLoginFinal(ClientLink * client)
{
    User * user = client->adminUser;

    // someone may have logged in to this user while we were downloading the update
    if (user->link)
    {
        int id = (user->publink_id ? user->publink_id : user->auth_id);

        sendMsg(user->link,id,"KickedByNewConnection");
        sendMsg(user->link,id,"Disconnect");
    }
    user->publink_id = 0;    // not needed by ChatAdmin client
    userLogin(user,client->link,client->linkType,user->access_level,-1);
}

void devPing(User *user, char *cmd)
{
    if (user->publink_id)
        sendMsg(user->link,user->publink_id,cmd);
    else
        sendMsg(user->link,user->auth_id,cmd);
}



typedef struct{
    U32        access_level;
    char    *cmdname;
    int     (*handler)(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash);
    U32        cmd_sizes[6];
    int        no_user;
    int        genericHandler;
    int        num_args;
    
    int        recv_count;    // (debug only) number of times this command was received from client
} ShardCmd;

static int gameLoginCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    return gameLogin(client, auth_id, args[0], hash, args[2], args[3], args[4]);
}

static int pubLoginCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    return pubLogin(client, auth_id, args[0], hash);
}

static int adminLoginCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    return adminLogin(client, auth_id, args[0], hash);
}

static int shardLoginCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    return shardLogin(client, auth_id, args[0]);
}

static int userLogoutCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userLogout(user);
    return 1;
}

static int userSendCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userSend(user, args[0], args[1]);
    return 1;
}

static int userXactRequestGmailCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userXactRequestGmail(user, args[0], args[1], args[2], args[3]);
    return 1;
}

static int userCommitRequestGmailCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userCommitRequestGmail(user, args[0], args[1]);
    return 1;
}

static int systemSendGmailCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    systemSendGmail(user, args[0], args[1], args[2], args[3], args[4]);
    return 1;
}

static int userGmailDeleteCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userGmailDelete(user, args[0]);
    return 1;
}

static int userGmailReturnCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userGmailReturn(user, args[0]);
    return 1;
}

static int userGmailClaimCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userGmailClaim(user, args[0]);
    return 1;
}

static int userGmailClaimConfirmCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userGmailClaimConfirm(user, args[0]);
    return 1;
}

static int userNameCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userName(user, args[0]);
    return 1;
}

static int chatServerShutdownCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    chatServerShutdown(user, args[0]);
    return 1;
}

static int userCsrNameCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userCsrName(user, args[0], args[1]);
    return 1;
}

static int userCsrSilenceCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userCsrSilence(user, args[0], args[1]);
    return 1;
}

static int userCsrRenameableCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userCsrRenameable(user, args[0]);
    return 1;
}

static int channelKillCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelKill(user, args[0]);
    return 1;
}

static int csrStatusCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    csrStatus(user, args[0]);
    return 1;
}

static int csrSilenceAllCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    csrSilenceAll(user);
    return 1;
}

static int csrUnsilenceAllCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    csrUnsilenceAll(user);
    return 1;
}

static int csrSendAllCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    csrSendAll(user, args[0]);
    return 1;
}

static int csrRenameAllCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    csrRenameAll(user);
    return 1;
}

static int csrRemoveAllCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    csrRemoveAll(user);
    return 1;
}

static int csrCheckMailSentCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    csrCheckMailSent(user, args[0]);
    return 1;
}

static int csrCheckMailReceivedCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    csrCheckMailReceived(user, args[0]);
    return 1;
}

static int csrBounceMailSentCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    csrBounceMailSent(user, args[0], args[1]);
    return 1;
}

static int csrBounceMailReceivedCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    csrBounceMailReceived(user, args[0], args[1]);
    return 1;
}

static int friendRequestCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    friendRequest(user, args[0]);
    return 1;
}

static int friendRemoveCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    friendRemove(user, args[0]);
    return 1;
}

static int friendListCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    friendList(user);
    return 1;
}

static int friendStatusCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    friendStatus(user, args[0]);
    return 1;
}

static int userInvisibleCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userInvisible(user);
    return 1;
}

static int userVisibleCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userVisible(user);
    return 1;
}

static int userFriendHideCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userFriendHide(user);
    return 1;
}

static int userFriendUnHideCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userFriendUnHide(user);
    return 1;
}

static int userTellHideCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userTellHide(user);
    return 1;
}

static int userTellUnHideCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userTellUnHide(user);
    return 1;
}

static int userClearMessageHashCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userClearMessageHash(user);
    return 1;
}

static int userGmailFriendOnlySetCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userGmailFriendOnlySet(user);
    return 1;
}

static int userGmailFriendOnlyUnsetCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userGmailFriendOnlyUnset(user);
    return 1;
}

static int ignoreCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    ignore(user, args[0]);
    return 1;
}

static int unIgnoreCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    unIgnore(user, args[0]);
    return 1;
}

static int ignoreSpammerCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    ignoreSpammer(user, args[0]);
    return 1;
}

static int ignoreAuthCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    ignoreAuth(user, args[0]);
    return 1;
}

static int unIgnoreAuthCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    unIgnoreAuth(user, args[0]);
    return 1;
}

static int ignoreAuthSpammerCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    ignoreAuthSpammer(user, args[0]);
    return 1;
}

static int ignoreListCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    ignoreList(user);
    return 1;
}

static int unSpamMeCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    unSpamMe(user);
    return 1;
}

static int setSpammerThresholdCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    setSpammerThreshold(user, args[0]);
    return 1;
}

static int setSpammerMultiplierCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    setSpammerMultiplier(user, args[0]);
    return 1;
}

static int setSpammerDurationCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    setSpammerDuration(user, args[0]);
    return 1;
}

static int getGlobalCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    getGlobal(user, args[0]);
    return 1;
}

static int getGlobalSilentCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    getGlobalSilent(user, args[0]);
    return 1;
}

static int getLocalCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    getLocal(user, args[0]);
    return 1;
}

static int getLocalInviteCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    getLocalInvite(user, args[0]);
    return 1;
}

static int getLocalLeagueInviteCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    getLocalLeagueInvite(user, args[0]);
    return 1;
}

static int channelJoinCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelJoin(user, args[0], args[1]);
    return 1;
}

static int channelCreateCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelCreate(user, args[0], args[1]);
    return 1;
}

static int channelLeaveCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelLeave(user, args[0], 0);
    return 1;
}

static int channelInviteDenyCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelInviteDeny(user, args[0], args[1]);
    return 1;
}

static int channelSendCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelSend(user, args[0], args[1]);
    return 1;
}

static int channelSetUserAccessCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelSetUserAccess(user, args[0], args[1], args[2]);
    return 1;
}

static int channelSetAccessCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelSetAccess(user, args[0], args[1]);
    return 1;
}

static int channelCsrMembersAccessCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelCsrMembersAccess(user, args[0], args[1]);
    return 1;
}

static int channelInviteCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelInvite(user, args, count);
    return 1;
}

static int channelCsrInviteCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelCsrInvite(user, args, count);
    return 1;
}

static int watchingListCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    watchingList(user);
    return 1;
}

static int channelListMembersCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelListMembers(user, args[0]);
    return 1;
}

static int channelSetMotdCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelSetMotd(user, args[0], args[1]);
    return 1;
}

static int channelFindCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelFind(user, args[0], args[1]);
    return 1;
}

static int channelSetDescriptionCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelSetDescription(user, args[0], args[1]);
    return 1;
}

static int channelSetTimoutCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    channelSetTimout(user, args[0], args[1]);
    return 1;
}

static int devPingCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    devPing(user, args[0]);
    return 1;
}

static int userSetGmailXactDebugCommand(ClientLink *client, int auth_id, User *user, char **args, int count, U32 *hash)
{
    userSetGmailXactDebug(user, args[0]);
    return 1;
}

static ShardCmd cmds[] =
{
    { 0, "Login",                gameLoginCommand,                {MAX_PLAYERNAME, 500, MAX_MESSAGE, 64, 12 },        1 },
    { 0, "PubLogin",            pubLoginCommand,                {MAX_PLAYERNAME, 500,         },                        1 },
    { 1, "AdminLogin",            adminLoginCommand,                {MAX_PLAYERNAME, 500        },                        1 },
    { 0, "ShardLogin",            shardLoginCommand,                {256},                                                1 },
    { 0, "Logout",                userLogoutCommand,                 },
    { 0, "SendUser",            userSendCommand,                {MAX_PLAYERNAME,    MAX_MESSAGE} },

    { 0, "GmailXactRequest",    userXactRequestGmailCommand,    {MAX_PLAYERNAME,  200, MAX_MESSAGE, MAX_PATH } },
    { 0, "GmailCommitRequest",    userCommitRequestGmailCommand,    {MAX_PLAYERNAME, MAX_PATH } },
    { 0, "SystemGmail",            systemSendGmailCommand,        {MAX_PLAYERNAME, 200, MAX_MESSAGE, MAX_PATH, 12 } },
    { 0, "GmailDelete",            userGmailDeleteCommand,        { 12 } },
    { 0, "GmailReturn",            userGmailReturnCommand,        { 12 } },

    { 0, "GmailClaimRequest",    userGmailClaimCommand,            { 12 } },
    { 0, "GmailClaimConfirm",    userGmailClaimConfirmCommand,    { 12 } },

    { 0, "Name",                userNameCommand,                 {MAX_PLAYERNAME} },
    { 3, "Shutdown",            chatServerShutdownCommand,        {MAX_MESSAGE} },

    { 2, "CsrName",                userCsrNameCommand,             {MAX_PLAYERNAME,    MAX_PLAYERNAME} },
    { 2, "CsrSilence",            userCsrSilenceCommand,            {MAX_PLAYERNAME,    10} },
    { 2, "CsrGrantRename",        userCsrRenameableCommand,        {MAX_PLAYERNAME} },
    { 2, "CsrChanKill",            channelKillCommand,            {MAX_CHANNELNAME} },
    { 1, "CsrStatus",            csrStatusCommand,                {MAX_PLAYERNAME} },
    { 3, "CsrSilenceAll",        csrSilenceAllCommand,            },
    { 3, "CsrUnsilenceAll",        csrUnsilenceAllCommand,        },
    { 3, "CsrSendAll",            csrSendAllCommand,                {MAX_MESSAGE} },
    { 4, "CsrRenameAll",        csrRenameAllCommand,            },
    { 4, "CsrRemoveAll",        csrRemoveAllCommand,            {MAX_PLAYERNAME} },
    { 2, "CsrCheckMailSent",     csrCheckMailSentCommand,        {MAX_PLAYERNAME} },
    { 2, "CsrCheckMailReceived", csrCheckMailReceivedCommand,    {MAX_PLAYERNAME} },
    { 4, "CsrBounceMailSent",     csrBounceMailSentCommand,        {MAX_PLAYERNAME, 12} },
    { 4, "CsrBounceMailReceived", csrBounceMailReceivedCommand,    {MAX_PLAYERNAME, 12} },

    { 0, "Friend",            friendRequestCommand,            {MAX_PLAYERNAME} },
    { 0, "UnFriend",        friendRemoveCommand,            {MAX_PLAYERNAME} },
    { 0, "Friends",            friendListCommand,                },
    { 0, "Status",            friendStatusCommand,            {MAX_FRIENDSTATUS} },
    { 0, "Invisible",        userInvisibleCommand,            },
    { 0, "Visible",            userVisibleCommand,            },
    { 0, "FriendHide",        userFriendHideCommand,            },
    { 0, "FriendunHide",    userFriendUnHideCommand,        },
    { 0, "TellHide",        userTellHideCommand,            },
    { 0, "TellUnHide",        userTellUnHideCommand,            },
    { 0, "GlobalMotd",        userClearMessageHashCommand,    },

    { 0, "GMailFriendOnlySet",        userGmailFriendOnlySetCommand,        },
    { 0, "GMailFriendOnlyUnset",    userGmailFriendOnlyUnsetCommand,    },

    { 0, "Ignore",            ignoreCommand,                    {MAX_PLAYERNAME} },
    { 0, "Unignore",        unIgnoreCommand,                {MAX_PLAYERNAME} },
    { 0, "IgnoreSpammer",    ignoreSpammerCommand,            {MAX_PLAYERNAME} },
    { 0, "IgnoreAuth",        ignoreAuthCommand,                {12} },
    { 0, "UnignoreAuth",    unIgnoreAuthCommand,            {12} },
    { 0, "IgnoreAuthSpammer",        ignoreAuthSpammerCommand,        {12} },
    { 0, "Ignoring",        ignoreListCommand,                },
    { 0, "unSpamMe",        unSpamMeCommand                },

    { 1, "setSpamThreshold", setSpammerThresholdCommand,    {12} },
    { 1, "setSpamMultiplier",setSpammerMultiplierCommand,    {12} },
    { 1, "setSpamDuration",  setSpammerDurationCommand,    {12} },

    { 0, "GetGlobal",        getGlobalCommand,                {12} },  // Request comes from mapserver
    { 0, "GetGlobalSilent",    getGlobalSilentCommand,        {12} },  // Request comes from mapserver
    { 0, "GetLocal",        getLocalCommand,                {MAX_PLAYERNAME} }, // Request comes directly from client
    { 0, "GetLocalInvite",    getLocalInviteCommand,            {MAX_PLAYERNAME} }, // Request comes directly from client
    { 0, "GetLocalLeagueInvite",    getLocalLeagueInviteCommand,            {MAX_PLAYERNAME} }, // Request comes directly from client
    { 0, "Join",            channelJoinCommand,            {MAX_CHANNELNAME, 64} },
    { 0, "Create",            channelCreateCommand,            {MAX_CHANNELNAME, 64} },
    { 0, "Leave",            channelLeaveCommand,            {MAX_CHANNELNAME} },
    { 0, "DenyInvite",        channelInviteDenyCommand,        {MAX_CHANNELNAME, MAX_PLAYERNAME} },
    { 0, "Send",            channelSendCommand,            {MAX_CHANNELNAME, MAX_MESSAGE} },
    { 0, "UserMode",        channelSetUserAccessCommand,    {MAX_CHANNELNAME, MAX_PLAYERNAME, 64} },
    { 0, "ChanMode",        channelSetAccessCommand,        {MAX_CHANNELNAME, 64} },
    { 3, "CsrMembersMode",    channelCsrMembersAccessCommand,{MAX_CHANNELNAME, 64} },
    { 0, "Invite",            channelInviteCommand,            {0},    0,    1},
    { 1, "CsrInvite",        channelCsrInviteCommand,        {0},    0,  1},
    { 0, "Watching",        watchingListCommand,            },
    { 0, "ChanList",        channelListMembersCommand,        {MAX_CHANNELNAME} },
    { 0, "ChanMotd",        channelSetMotdCommand,            {MAX_CHANNELNAME, MAX_MESSAGE} },
    { 0, "ChanFind",        channelFindCommand,            {64, MAX_MESSAGE} },
    { 0, "ChanDesc",        channelSetDescriptionCommand,    {MAX_CHANNELNAME, MAX_CHANNELDESC} },
    { 0, "ChanSetTimeout",    channelSetTimoutCommand,        {MAX_CHANNELNAME, 12} },

    { 9, "DevPing",            devPingCommand,                {MAX_MESSAGE} },
    { 9, "GmailXactDebug",    userSetGmailXactDebugCommand,    {12} },

    { 0 },
};

#define PARSEFAIL(msg,arg) {    if(!user)                                            \
                                    user = userFindByAuthId(auth_id);                \
                                if(!user)                                            \
                                    return 0;                                        \
                                sendSysMsg(user,0,1,"%s %s",msg,arg);                    \
                                return 1; }


static char* concatArgs(char **args, int count)
{
    static bool in_recursion = false;
    static char *s = NULL;
    int i;
    
    assert(!in_recursion); // not recursive or reentrant
    in_recursion = true;
    estrClear(&s);
    if(args)
    {
        for( i = 0; i < count; ++i ) 
        {
            estrConcatf(&s,"%i=%s,",i,args[i]);
        }
    }
    in_recursion = false;
    return s;
}

int processCmd(ClientLink *client,int auth_id,char **args,int count,User **user_p)
{
    ShardCmd    *cmd = stashFindPointerReturnPointer(cmd_hashes,args[0]);
    int            i;
    User        *user=0;

    *user_p = 0;
    if (!cmd)
        PARSEFAIL("InvalidCommand",args[0]);

    cmd->recv_count++;
    if (!cmd->no_user)
    {
        user = userFindByAuthId(auth_id);
        if (!user)
            return 0;
        *user_p = user;
        if (user->access_level < cmd->access_level)
            PARSEFAIL("InvalidCommand",args[0]);
    }
    
    count--;
    if(!cmd->genericHandler)
    {
        if (cmd->num_args != count)
            PARSEFAIL("WrongNumberOfParams",args[0]);
        args++;
        for(i=0;i<count;i++)
        {
            if (strlen(args[i]) > cmd->cmd_sizes[i])
                PARSEFAIL("ParameterTooLong",args[i]);
        }
    }
    else
        args++;

    if (!user)
    {
        char    str[100];
        U32        hash[4];
        memset(str, 0, sizeof(str));
        if(args[1])
            memcpy(hash, unescapeString(args[1]), sizeof(hash));
        else
            memset(hash, 0, sizeof(hash));


        LOG_DEBUG("login\tclient=%x\tauth_id=%i\tcount=%i\targs=%s",client,auth_id,count,concatArgs(args,count));

        return cmd->handler(client, auth_id, user, args, count, hash);
    }
    else if(cmd->genericHandler)
    {

        LOG_DEBUG("generic_handler\tclient=%x\tauth_id=%i\tcount=%i\targs=%s",client,auth_id,count,concatArgs(args,count));
        cmd->handler(client, auth_id, user, args, count, NULL);
    }
    else switch(cmd->num_args)
    {
        xcase 0:
             LOG_DEBUG("0argcmd\tclient=%x\tauth_id=%i\tcount=%i\targs=%s",client,auth_id,count,concatArgs(args,count));
            cmd->handler(client, auth_id, user, args, count, NULL);
        xcase 1:
             LOG_DEBUG( "1argcmd\tclient=%x\tauth_id=%i\tcount=%i\targs=%s",client,auth_id,count,concatArgs(args,count));
            cmd->handler(client, auth_id, user, args, count, NULL);
        xcase 2:
             LOG_DEBUG("2argcmd\tclient=%x\tauth_id=%i\tcount=%i\targs=%s",client,auth_id,count,concatArgs(args,count));
            cmd->handler(client, auth_id, user, args, count, NULL);
        xcase 3:
             LOG_DEBUG( "3argcmd\tclient=%x\tauth_id=%i\tcount=%i\targs=%s",client,auth_id,count,concatArgs(args,count));
            cmd->handler(client, auth_id, user, args, count, NULL);
        xcase 4:
            LOG_DEBUG( "4argcmd\tclient=%x\tauth_id=%i\tcount=%i\targs=%s",client,auth_id,count,concatArgs(args,count));
            cmd->handler(client, auth_id, user, args, count, NULL);
        xcase 5:
            LOG_DEBUG( "5argcmd\tclient=%x\tauth_id=%i\tcount=%i\targs=%s",client,auth_id,count,concatArgs(args,count));
            cmd->handler(client, auth_id, user, args, count, NULL);
    }
    return 1;
}

static char *log_str;

static void checkFlushLog()
{
    static char    log_name[MAX_PATH];
    static int    timer;
    int            len = estrLength(&log_str);

    if (!timer)
        timer = timerAlloc();
    if (len && (timerElapsed(timer) > 120 || len > 200000))
    {
        extern void logPutMsgSub(char *msg_str,int len,char *filename,int zip_file,int zipped,int text_file, char *msgDelim);

        if (!log_name[0])
        {
            char datestr[100],*s;

            timerMakeDateStringFromSecondsSince2000(datestr,timerSecondsSince2000());
            for(s=datestr;*s;s++)
            {
                if (*s == ':' || *s == ' ')
                    *s = '-';
            }
            sprintf(log_name,"chat_%s_1",datestr);
        }

        timerStart(timer);
        logPutMsgSub(log_str,len+1,log_name,1,0,0, NULL);
        estrClear(&log_str);
    }
}

static void logCmd(char *datestr,int auth_id,User *user,char *str)
{
    estrConcatf(&log_str, "%s %d \"%s\" %s\r\n",datestr, auth_id,user ? user->name : "",str);
    checkFlushLog();
}


int handleAuxMessage(Packet *pak,int cmd, NetLink *link)
{
    switch(cmd)
    {
    case SHARDCOMM_AUX_RESERVED_NAMES:
        receiveReservedNames(pak);
        receiveProfanity(pak);
        return 1;
    }
    printf("Did not handle Reserved Name Message (cmd # %d)\n", cmd);
    return 0;
}

int handleClientMsg(Packet *pak,int cmd, NetLink *link)
{
    char        *str,*next_str,*args[1000],datestr[100];
    static char    orig_str[10000];
    int            i,count,auth_id;
    ClientLink    *client = link->userData;
    User        *user;

    timerMakeLogDateStringFromSecondsSince2000(datestr,timerSecondsSince2000());
    switch( cmd )
    {
        case SHARDCOMM_CMD:
        {
            while(!pktEnd(pak))
            {
                if(client->linkType != kChatLink_Admin)
                    auth_id = pktGetBits(pak,32);
                else
                    auth_id = (client->adminUser ?  client->adminUser->auth_id : -1);    // ChatAdmin client don't send auth_id

                str = pktGetString(pak);
                strncpy(orig_str,str,sizeof(orig_str)-1);
                count = tokenize_line_safe(str,args,ARRAY_SIZE(args),&next_str);
                for(i=0;i<count;i++)
                    unescapeStringInplace(args[i]);
                if (count)
                {
                    if (!processCmd(link->userData,auth_id,args,count,&user))
                        sendMsg(link,auth_id,"Disconnect");
                    logCmd(datestr,0,user,orig_str);
                }
                g_stats.processed_count++;
            }
            return 1;
        }break;
        case SHARDCOMM_SET_LOG_LEVELS:
        {
            int i;
            for( i = 0; i < LOG_COUNT; i++ )
            {
                int log_level = pktGetBitsPack(pak,1);
                logSetLevel(i,log_level);
            }
            return 1;
        }break;
        case SHARDCOMM_TEST_LOG_LEVELS:
        {
            LOG( LOG_DEPRECATED, LOG_LEVEL_ALERT, LOG_CONSOLE, "ChatServer Log Line Test: Alert" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "ChatServer Log Line Test: Important" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "ChatServer Log Line Test: Verbose" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_DEPRECATED, LOG_CONSOLE, "ChatServer Log Line Test: Deprecated" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_DEBUG, LOG_CONSOLE, "ChatServer Log Line Test: Debug" );
            return 1;
        }break;

        default: 
            return adminHandleClientMsg(pak, cmd, link);
    }
}


void shardNetInit()
{
    netLinkListAlloc(&net_links,INITIAL_CLIENT_COUNT,sizeof(ClientLink),clientCreateCb);
    netInit(&net_links,0,DEFAULT_CHATSERVER_PORT);
    net_links.destroyCallback = clientDeleteCb;
    NMAddLinkList(&net_links, handleClientMsg);
}

static void shardCmdInit()
{
    int        i,j;

    cmd_hashes = stashTableCreateWithStringKeys(100,StashDeepCopyKeys);
    for(i=0;cmds[i].cmdname;i++)
    {
        for(j=0;cmds[i].cmd_sizes[j];j++)
            cmds[i].num_args++;
        stashAddPointer(cmd_hashes,cmds[i].cmdname,&cmds[i], false);
    }
}


void computeRateCallback(NetLink* link)
{
    g_stats.send_rate += pktRate(&link->sendHistory);
    g_stats.recv_rate += pktRate(&link->recvHistory);
}

void updateInfo()
{
    char    buf[1000];
    char    buf2[100],buf3[100];

    static int    timer;
    float time;
  
     if (timer && timerElapsed(timer) < 1)
        return;

    if (!timer)
        timer = timerAlloc();

    g_stats.send_rate = 0;
    g_stats.recv_rate = 0;
    NMForAllLinks(computeRateCallback);

    time = timerElapsed(timer);
    timerStart(timer);

    g_stats.crossShardRate = g_stats.sendmsg_count ? (g_stats.cross_shard_msgs / (F32)g_stats.sendmsg_count) : 0;
    g_stats.invalidRate = g_stats.sendmsg_count ? (g_stats.error_msgs_sent / (F32) g_stats.sendmsg_count) : 0;
    g_stats.sendMsgRate = g_stats.sendmsg_count / time;
    g_stats.recvMsgRate = g_stats.processed_count / time;
    
    sprintf(buf,"ChatServer  Users %d  Channels %d  Online %d  Links %d  Admin %d Monitors %d Cross %.0f%% Invalid %.2f%% Send: %s/s  Rcv: %s/s (MSG Send: %d/s  Rcv: %d /s)",
        chatUserGetCount(),
        chatChannelGetCount(),
        g_online_count,
        net_links.links->size,
        eaSize(&admin_clients),
        monitor_links.links->size,
        100 * g_stats.crossShardRate,
        100 * g_stats.invalidRate,
        printUnit(buf2, g_stats.send_rate),
        printUnit(buf3, g_stats.recv_rate),
        g_stats.sendMsgRate,
           g_stats.recvMsgRate);
    setConsoleTitle(buf);    
    
    g_stats.processed_count = 0;
    g_stats.sendmsg_count = 0;
    g_stats.cross_shard_msgs = 0;
    g_stats.error_msgs_sent = 0;
}

void updateCrossShardStats(User * from, User * to)
{
    if(from->link && to->link && from->link != to->link)
        g_stats.cross_shard_msgs++;
}

void displayUsageStats()
{
       int i, size, onlineCount=0, adminCount=0, sum=0;
    int memberSum=0, onlineSum=0, publicCount=0;
    int friendSum=0, ignoreSum=0, watchingSum=0, emailSum=0, gmailSum=0, befrienderSum=0;
    int onlineFriendSum=0, onlineIgnoreSum=0, onlineWatchingSum=0, onlineEmailSum=0, onlineGmailSum=0, onlineBefrienderSum=0, onlineAdminCount=0;
    int memberBreakdown[4] = {0};
    User **users;
    Channel **channels;

    users = chatUserGetAll();
    size = eaSize(&users);
    for(i=0;i<size;i++)
    {
        User *user = users[i];

        friendSum        += eaiSize(&user->friends);
        ignoreSum        += eaiSize(&user->ignore);
        watchingSum        += eaSize(&user->watching);
        emailSum        += eaSize(&user->email);
        gmailSum        += eaSize(&user->gmail);
        befrienderSum    += eaSize(&user->befrienders);
        if(user->access & CHANFLAGS_OPERATOR)
            adminCount++;

        if(user->link)
        {
            onlineFriendSum        += eaiSize(&user->friends);
            onlineIgnoreSum        += eaiSize(&user->ignore);
            onlineWatchingSum    += eaSize(&user->watching);
            onlineEmailSum        += eaSize(&user->email);
            onlineGmailSum        += eaSize(&user->gmail);
            onlineBefrienderSum    += eaSize(&user->befrienders);
            if(user->access & CHANFLAGS_ADMIN)
                onlineAdminCount++;
            onlineCount++;
        }

    }

      printf("\n\n*******\nUsage Statistics\n\n");
     printf("Users (%d) -- Avg Online/Total Stats\n", size);
     printf("\tFriends: %.1f/%.1f\n",        onlineFriendSum / (float)onlineCount,        friendSum / (float)size);
    printf("\tIgnores: %.1f/%.1f\n",        onlineIgnoreSum / (float)onlineCount,        ignoreSum / (float)size);
    printf("\tWatching: %.1f/%.1f\n",        onlineWatchingSum / (float)onlineCount,    watchingSum / (float)size);
    printf("\tEmails: %.1f/%.1f\n",            onlineEmailSum / (float)onlineCount,        emailSum / (float)size);
    printf("\tGmails: %.1f/%.1f\n",            onlineGmailSum / (float)onlineCount,        gmailSum / (float)size);
    printf("\tBefrienders: %.1f/%.1f\n",    onlineBefrienderSum / (float)onlineCount, befrienderSum / (float)size);
      printf("\tTotal Admins: %d/%d\n",    adminCount, onlineAdminCount);

    channels = chatChannelGetAll();
    size = eaSize(&channels);
    for(i=0;i<size;i++)
    {
        int members = eaSize(&channels[i]->members);
        memberSum += members;
        if(members <= 10)
            memberBreakdown[0]++;
        else if(members <= 40)
            memberBreakdown[1]++;
        else if(members <= 100)
            memberBreakdown[2]++;
        else
            memberBreakdown[3]++;

        onlineSum += eaSize(&channels[i]->online);
    
        if(channels[i]->access & CHANFLAGS_JOIN)
            publicCount++;
    }
    printf("Channels (%d)\n", size);
    printf("\tAvg Members: %.1f\n", memberSum / ((float) size));
    printf("\tAvg Online Members: %.1f\n", onlineSum / ((float) size));
    printf("\tPublic\\Private: %d\\%d\n", publicCount, (size - publicCount));
    printf("\tSize Breakdown:\n");
    printf("\t\t  1-10: %-5d %.1f%%\n", memberBreakdown[0], (100 * (memberBreakdown[0] / (float)size)));
    printf("\t\t 11-40: %-5d %.1f%%\n", memberBreakdown[1], (100 * (memberBreakdown[1] / (float)size)));
    printf("\t\t41-100: %-5d %.1f%%\n", memberBreakdown[2], (100 * (memberBreakdown[2] / (float)size)));
     printf("\t\t  101+: %-5d %.1f%%\n", memberBreakdown[3], (100 * (memberBreakdown[3] / (float)size)));
    

    printf("Commands\n");
    sum = 0;
    for(i=0;cmds[i].cmdname;i++)
         sum += cmds[i].recv_count;
     for(i=0;cmds[i].cmdname;i++)
    {
        if(cmds[i].recv_count)
        {
               printf("\t%-20s %-10d %5.1f%%\n", cmds[i].cmdname, cmds[i].recv_count, (100 * ((float)cmds[i].recv_count) / sum));
            cmds[i].recv_count = 0;
        }
    }
}

static BOOL s_CtrlHandler(DWORD fdwCtrlType)
{
    switch (fdwCtrlType)
    {
        #define QUIT_CASE(event)                                            \
            case event:                                                        \
                printf("Control Event " #event " (%i)", event);    \
                chatServerShutdown(NULL, NULL);                                \
                break;

        QUIT_CASE(CTRL_C_EVENT);
        QUIT_CASE(CTRL_BREAK_EVENT);
        QUIT_CASE(CTRL_CLOSE_EVENT);
        QUIT_CASE(CTRL_LOGOFF_EVENT);
        QUIT_CASE(CTRL_SHUTDOWN_EVENT);

        #undef QUIT_CASE
    }
    return FALSE;
}

static ParseTable parse_ChatServerCfg[] =
{
    { "{",            TOK_START,       0 },
    { "SqlLogin",    TOK_FIXEDSTR(ChatServerCfg, sqlLogin) },
    { "SqlDbName",    TOK_FIXEDSTR(ChatServerCfg, sqlDbName) },
    { "}",            TOK_END,         0 },
    { 0 },
};

static bool chatSvrCfgLoad(ChatServerCfg *cfg)
{
     char fnbuf[MAX_PATH];
     char *fn = fileLocateRead("server/db/chat_server.cfg",fnbuf);
    return fn && ParserLoadFiles(NULL,fn,NULL,0,parse_ChatServerCfg,cfg,NULL,NULL,NULL);
}

int main(int argc,char **argv)
{
    int        i, noreserved = 1;
    char    datestr[100];

    strcpy(g_exe_name,argv[0]);

    memCheckInit();

    EXCEPTION_HANDLER_BEGIN
    setAssertMode(ASSERTMODE_DEBUGBUTTONS | ASSERTMODE_FULLDUMP | ASSERTMODE_DATEDMINIDUMPS | ASSERTMODE_ZIPPED);
    memMonitorInit();

    if(isDevelopmentMode())
    {
        noreserved = 0;
    }

    consoleInit(110, 128, 0);
    setWindowIconColoredLetter(compatibleGetConsoleWindow(), 'C', 0xffff00);

    locOverrideIDInRegistryForServersOnly(0);

    for(i=1;i<argc;i++)
    {
        if(stricmp(argv[i],"-quit")==0)
        {
            exit(0);
        }
        else if (stricmp(argv[i], "-ignorespammer")==0)
        {
            if(i+1 < argc && isdigit((U8)argv[i+1][0]))
                ignore_spammer_threshold = atoi(argv[++i]);
            else
                ignore_spammer_threshold = 100; // default

            if(i+1 < argc && isdigit((U8)argv[i+1][0]))
                ignore_spammer_multiplier = atoi(argv[++i]);
            else
                ignore_spammer_multiplier = 5; // default

            if(i+1 < argc && isdigit((U8)argv[i+1][0]))
                ignore_spammer_duration = atoi(argv[++i]);
            else
                ignore_spammer_duration = 24*60*60; // default
        }
        else if (stricmp(argv[i],"-noreserved")==0)
        {
            noreserved = 0;
        }
        else if (stricmp(argv[i],"-locale")==0)
        {
            locOverrideIDInRegistryForServersOnly(locGetIDByNameOrID(argv[++i]));
        }
    }

    if(!chatSvrCfgLoad(&g_chatcfg))
    {
        FatalErrorf("error loading chat config");
        exit(1);
    }

    logSetMsgQueueSize(16 * 1024 * 1024);
    chatDbInit();
    shardCmdInit();

    sockStart();
    packetStartup(0,0);

    if(noreserved)
    {
        // handle reserved name input from a special-purpose mapserver
        netLinkListAlloc(&aux_links,200,sizeof(ClientLink),0);
        netInit(&aux_links,0,DEFAULT_CHATSERVER_AUX_PORT);
        net_links.destroyCallback = 0;
        NMAddLinkList(&aux_links, handleAuxMessage);

        printf("shard chat waiting for reserved names from mapserver..\n(Use \"-noreserved\" flag to skip, or run mapserver with \"-chatservernames localhost\"\n");
        while( ! GetReservedNameCount())
        {
            NMMonitor(200);
        }
    }

    g_chatLocale = locGetIDInRegistry();

    LoadProfanity();
    shardNetInit();
    chatMonitorInit();

    SetConsoleCtrlHandler((PHANDLER_ROUTINE) s_CtrlHandler, TRUE); // add to list

    printf("shard chat waiting for users..\n");
    estrConcatf(&log_str,"%s chatserver started\r\n",timerMakeLogDateStringFromSecondsSince2000(datestr,timerSecondsSince2000()));

    while (!s_shutting_down)
    {
        checkFlushLog();
        NMMonitor(20);
        for(i=0;i<net_links.links->size;i++)
            msgFlushPacket(net_links.links->storage[i]);
        if (_kbhit()) { // debug hack for printing out memory monitor stuff
            switch (_getch()) {
                xcase 'm':
                    memMonitorDisplayStats();
                xcase 'd':
                    printf("dumping memory table to c:/memlog.txt..\n");
                    memCheckDumpAllocs();
                    printf("done.\n");
                xcase 's':
                    displayUsageStats();
                xcase 'X':
                    chatServerShutdown(0,0);
            }
        }
        updateInfo();
        monitorTick();
        adminTick();
        messageFileTick();
        ChatDb_FlushChanges();
    }
    
    netForEachLink(&net_links, msgFlushPacket);
    lnkFlushAll();

    {
        int        i;
        User **users;
        printf("\tLogging off users...\n");
        users = chatUserGetAll();
        for(i = eaSize(&users)-1; i >= 0; --i)
        {
            if (users[i] && users[i]->link)
                userLogout(users[i]);    // we don't care if the logout messages actually go to the clients                            
        }
    }

    // Flush the db and exit()
    printf("\tSaving database...\n");
    chatDBShutDown();

    EXCEPTION_HANDLER_END 
    return 0;
}
