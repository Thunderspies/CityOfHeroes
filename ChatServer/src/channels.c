#include "channels.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include "shardnet.h"
#include "users.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>
#include "msgsend.h"
#include "loadsave.h"
#include <utilitieslib/components/StashTable.h>
#include "chatsqldb.h"

void channelSetUserAccessHelper(User * user, User * target, Watching * watching, char * option);

Channel *channelFindByName(char *channel_name)
{
    return chatChannelFind(channel_name);
}

Watching *channelFindWatching(User *user,char *channel_name)
{
    int        i;

    for(i=eaSize(&user->watching)-1;i>=0;i--)
    {
        if (stricmp(user->watching[i]->name,channel_name)==0)
            return user->watching[i];
    }
    return 0;
}

static int opAllowed(User *user,Watching *watching,User *target)
{    
    if (!watching)
        goto not_allowed;
    if (user->access & CHANFLAGS_ADMIN)
        return 1;
    if (!(watching->access & CHANFLAGS_OPERATOR))
        goto not_allowed;
    if (!target || !(target->access & CHANFLAGS_ADMIN))
        return 1;
not_allowed:
    sendSysMsg(user,0,1,"NotAllowed");
    return 0;
}

int channelIdx(User *user,Channel *channel)
{
    int        i;

    for(i=eaSize(&user->watching)-1;i>=0;i--)
    {
        if (user->watching[i]->channel == channel)
            break;
    }
    return i;
}

static int channelReserveIdx(User *user,Channel *channel)
{
    int        i;

    for(i=eaSize(&user->reserved)-1;i>=0;i--)
    {
        if (user->reserved[i]->channel == channel)
            break;
    }
    return i;
}

void watchingList(User *user)
{
    int        i;

    for(i=eaSize(&user->watching)-1;i>=0;i--)
        sendUserMsg(user,"Watching %s",user->watching[i]->name);
}

bool sendUserMsgJoin(User *user,User *member,Channel *channel,int refresh)
{
    Watching    *watch = channelFindWatching(member,channel->name);
    char        buf[20];

    if (user != member && member->access & CHANFLAGS_INVISIBLE)
        return 0;

    if (!watch)
        return 0;
    buf[0] = 0;
    if (member->access & CHANFLAGS_ADMIN)
        strcat(buf,"a");
    if (watch->access & CHANFLAGS_OPERATOR)
        strcat(buf,"o");
    if (!(watch->access & CHANFLAGS_SEND))
        strcat(buf,"s");
    return sendUserMsg(user,"Join %s %s %d %s %d",channel->name,member->name,userGetRelativeDbId(user, member), buf, refresh);
}

void channelSendAccess(User *user,Channel *channel)
{
    char    buf[100]="";
    if (channel->access & CHANFLAGS_OPERATOR)
        strcat(buf,"o");
    if (!(channel->access & CHANFLAGS_SEND))
        strcat(buf,"s");
    if (channel->access & CHANFLAGS_JOIN)
        strcat(buf,"j");
    sendUserMsg(user,"Channel %s %s",channel->name,buf);
}

void channelList(User *user,char *channel_name)
{
    int        i;
    Channel *channel = channelFindByName(channel_name);

    if (!channel)
    {
        sendSysMsg(user,0,1,"ChannelDoesNotExist %s",channel_name);
        return;
    }
    channelSendAccess(user,channel);
    for(i=eaSize(&channel->online)-1;i>=0;i--)
    {
        sendUserMsgJoin(user,channel->online[i],channel,1);
    }
}

void channelListMembers(User *user,char *channel_name)
{
    int        i;
    Channel *channel = channelFindByName(channel_name);

    if (!channel)
    {
        sendSysMsg(user,0,1,"ChannelDoesNotExist %s",channel_name);
        return;
    }
//    channelSendAccess(user,channel);
    for(i=eaSize(&channel->online)-1;i>=0;i--)
    {
        if (!(channel->online[i]->access & CHANFLAGS_INVISIBLE))
            sendUserMsg(user,"chanmember %s %s",channel_name,channel->online[i]->name);
    }
}

int channelSize(Channel * channel)
{
    return (    eaSize(&channel->reserved) 
            +    eaSize(&channel->members)
            +    eaiSize(&channel->invites));
}

static void channelJoinSub(User *user,char *channel_name,int create,int reserve, int forceJoin)
{
    Channel        *channel = channelFindByName(channel_name);
    Watching    *watching;
    Watching    ***list;
    User        ***channelList;
    int            idx,idx2;

    if(forceJoin)
        create = reserve = 0;    // force to 0

    if( !forceJoin && (!channel || channelReserveIdx(user, channel) < 0))
    {
        if (   ( ! reserve && (MAX_WATCHING <= userWatchingCount(user))) 
            || (   reserve && (MAX_WATCHING <= eaSize(&user->reserved))))
        {
            sendSysMsg(user,0,1,"WatchingTooManyChannels %s", (channel ? channel->name : channel_name));
            return;
        }
    }
    
    if (!channel)
    {
        if (!create)
        {
            sendSysMsg(user,0,1,"JoinChannelDoesNotExist %s",channel_name);
            return;
        }
        if (!userCanTalk(user))
        {
            sendSysMsg(user,0,1,"NotAllowed");
            return;
        }
        channel = ParserAllocStruct(sizeof(Channel));
        channel->name = ParserAllocString(channel_name);
        channel->description = ParserAllocString("");
        channel->access = CHANFLAGS_COMMON;
        chatChannelInsertOrUpdate(channel);
        sendAdminMsg("ChanAdd %s", channel->name); 
    }

    if (   ! forceJoin
        &&   channelSize(channel) >= MAX_MEMBERS
        &&   channelReserveIdx(user,channel) < 0
        &&   eaFind(&user->invites,channel) < 0)            
    {
        sendSysMsg(user,0,1,"ChannelFull %s",channel_name);
        return;
    }

    if(reserve)
    {
        list = &user->reserved;
        channelList = &channel->reserved;
        idx = channelReserveIdx(user,channel);
        watching = eaGet(&user->reserved, idx);
    }
    else
    {
        list = &user->watching;
        channelList = &channel->members;
        idx = channelIdx(user,channel);
        watching = eaGet(&user->watching, idx);
    }

       if (idx < 0)
    {
        if (   ! forceJoin
            && ! (channel->access & CHANFLAGS_JOIN) 
            && ! (user->access & CHANFLAGS_ADMIN) 
            && eaFind(&user->invites, channel) < 0)
        {
            sendSysMsg(user,0,1,"NotJoinableChannel %s",channel_name);
            return;
        }

        eaFindAndRemove(&user->invites, channel);
        eaiFindAndRemove(&channel->invites, user->auth_id);

        if(!reserve && ((idx2 = channelReserveIdx(user,channel)) >= 0))
        {
            watching = eaRemove(&user->reserved, idx2);
        }
        else
        {
            watching = ParserAllocStruct(sizeof(*watching));
            watching->name = ParserAllocString(channel_name);
            watching->channel = channel;
            watching->access = (channel->access & CHANFLAGS_COPYMASK) | CHANFLAGS_JOIN;
        }
        eaPush(list, watching);
    }
    else if(!create)
    {
         sendSysMsg(user,0,1,"AlreadyChannelMember");
        return;
    }

    if (eaFind(channelList,user) < 0)
    {
        if(channelSize(channel) == 0)
            watching->access |= CHANFLAGS_OPERATOR;

        eaPush(channelList,user);            
        if(!reserve)
            eaFindAndRemove(&channel->reserved, user);
    }

    if(reserve)
        sendUserMsg(user,"Reserve %s",channel->name);
    else
        channelOnline(channel,user,0);

    chatUserInsertOrUpdate(user);
}

void channelJoin(User *user,char *channel_name,char *option)
{
    int reserve = 0;
    if(!stricmp(option,"reserve"))
        reserve = 1;
    channelJoinSub(user,channel_name,0,reserve,0);
}

void channelCreate(User *user,char *channel_name, char *option)
{
    const char * reason;
    int reserve = 0;
    if(!stricmp(option,"reserve"))
        reserve = 1;
    if (channelFindByName(channel_name))
    {
        sendSysMsg(user,0,1,"CreateChannelAlreadyExists %s",channel_name);
        return;
    }
    if( (reason = InvalidName(channel_name, 0)))
    {
        sendSysMsg(user,0,1,"ChannelNameNotAllowed %s %s",channel_name, reason);
        return;
    }
    channelJoinSub(user,channel_name,1,reserve,0);
}

//Syntax: <channel name> <type> <name0> ... <nameN>
// Where N >= 0
// 'type' specifies the format of the names (kChatId_AuthId, kChatId_DbId, kChatId_Handle)
void channelInvite(User *user,char **args, int count)
{
    if(count < 3)
    {
        sendSysMsg(user,0,1,"WrongNumberOfParams Invite");    
    }
    else
    {
        char        *channel_name = args[0];
        int            idType = atoi(args[1]);
        Watching    *watching = channelFindWatching(user,channel_name);
        User        *target;
        int            i, inviteCount=0;

        if (!opAllowed(user,watching,0))
            return;

        if(idType < 0 || idType >= NUM_CHAT_ID_TYPES)
        {
            sendSysMsg(user,0,0,"CannotInviteBadIdType %s", watching->channel->name);
            return;
        }
        if(channelSize(watching->channel) + (count-1) > MAX_MEMBERS)
        {
            sendSysMsg(user,0,0,"CannotInviteMaxMembers %s", watching->channel->name);
            return;
        }

        args  += 2;
        count -= 2;

          for(i=0;i<count;i++)
        {
            target = userFindIgnorable(user, args[i], idType);
            if (!target)
            {
                //if(idType == kChatId_AuthId)
                //{
                //    // add pending channel invite
                //    addPendingChannelInvite(atoi(args[i]), channel_name);
                //}
                continue;
            }
            
            if (target->access & CHANFLAGS_INVISIBLE || target->access & CHANFLAGS_INVITEHIDE)  
            {
                sendSysMsg(user,0,0,"UnableToJoinHidden");
                return;
            }
            else if(userWatchingCount(target) >= MAX_WATCHING)
            {
                sendSysMsg(user,0,0,"CannotInviteMaxChannels %s", target->name);
            }
            else if(channelIdx(target, watching->channel) >= 0)
            {
                if(count == 1) // don't send for group invites
                    sendSysMsg(user,0,1,"CannotInviteAlreadyMember %s %s", target->name, watching->channel->name );
            }
            else if(eaFind(&target->invites, watching->channel) >= 0)
            {
                sendSysMsg(user,0,1,"CannotInviteAlreadyInvited %s %s", target->name, watching->channel->name );
            }
            else if(eaSize(&target->invites) > MAX_INVITES)
            {
                sendSysMsg(user,0,1,"CannotInviteMaxInvites %s %s", target->name, watching->channel->name );
            }
            else
            {
                eaPush(&target->invites, watching->channel);
                eaiPush(&watching->channel->invites, target->auth_id);
                sendUserMsg(target,"Invite %s %s",channel_name,user->name);
                if(count == 1)
                    sendSysMsg(user,0,0,"YouInvited %s %s", channel_name, target->name);
                updateCrossShardStats(user, target);
                inviteCount++;
                chatChannelInsertOrUpdate(watching->channel);
            }
        }

        if(inviteCount && count > 1)
            sendSysMsg(user,0,0,"YouInvitedGroup %s %d", channel_name, inviteCount);
    }
}

//Syntax: <channel name> <type> <name0> ... <nameN>
// Where N >= 0
// 'type' specifies the format of the names (kChatId_AuthId, kChatId_DbId, kChatId_Handle)
void channelCsrInvite(User *user,char **args, int count)
{
    if(count < 3)
    {
        sendSysMsg(user,0,1,"WrongNumberOfParams CsrInvite");    
    }
    else
    {
        char        *channel_name = args[0];
        int            i, idType = atoi(args[1]);
        char        namebuf[MAX_CHANNELNAME+1];
        char        numbuf[100];
        Watching    *watching;

        if(idType < 0 || idType >= NUM_CHAT_ID_TYPES)
        {
            sendSysMsg(user,0,0,"CannotInviteBadIdType %s", channel_name);
            return;
        }

        for(i=1;;i++)
        {
            strncpy(namebuf,channel_name,ARRAY_SIZE(namebuf)-1);
            itoa(i, numbuf, 10);
            namebuf[MAX_CHANNELNAME - strlen(numbuf)] = '\0';
            strcat(namebuf,numbuf);

            if(!chatChannelFind(namebuf))
                break;
        }

        channelCreate(user, namebuf, "");

        watching = channelFindWatching(user,namebuf);
        
        if(!watching)
            return;

        watching->channel->access &= ~(CHANFLAGS_JOIN);    // private

        args  += 2;
        count -= 2;

        for(i=0;i<count;i++)
        {
            User * target = userFind(user, args[i], idType);

            if (!target)
            {
                if (count == 1)
                    sendSysMsg(user,0,1,"CannotInviteUser %s", args[i]);
            }
            else if(channelIdx(target, watching->channel) >= 0)
            {
                if(count == 1) // don't send for group invites
                    sendSysMsg(user,0,1,"CannotCsrInviteAlreadyMember %s %s", target->name, watching->channel->name );
            }
            else
            {
                channelJoinSub(target, watching->channel->name, 0, 0, 1);
            }
        }
    }
}

// assign a new operator if no online channel operators exist
void chooseOperatorIfNecessary(Channel * channel)
{
    int size = eaSize(&channel->members);

    if(size)
    {
        User * target;
        Watching * watching;
        int i, idx;
        for(i=0;i<size;i++)
        {
            target = channel->members[i];
            if((idx = channelIdx(target, channel)) >= 0)
            {
                if(target->watching[idx]->access & CHANFLAGS_OPERATOR)
                    return;
            }
        }

        // didn't find an operator
        target = channel->members[0];
        idx = channelIdx(target, channel);
        watching = target->watching[idx];
        channelSetUserAccessHelper(0, target, watching, "+Operator");
    }
}

void channelOffline(Channel *channel,User *user)
{
    int        i,resend=0;

       if (eaFind(&channel->online,user) < 0)
        return;

    for(i=eaSize(&channel->online)-1;i>=0;i--)
    {
        if (resend)
            resendMsg(channel->online[i]);
        else
            resend = sendUserMsg(channel->online[i],"Leave %s %s 0",channel->name,user->name);

        updateCrossShardStats(user, channel->online[i]);
    }
    eaFindAndRemove(&channel->online,user);
}

void channelDeleteIfEmpty(Channel *channel)
{
    if (channelSize(channel) <= 0)
    {
        sendAdminMsg("ChanRemove %s", channel->name);
        // first, delete items not stored in channel parse struct info
        eaDestroy(&channel->members);
        eaDestroy(&channel->reserved);
        eaDestroy(&channel->online);
        chatChannelDelete(channel);
    }
}

void channelLeave(User *user,char *channel_name, int isKicking)
{
    Channel        *channel = channelFindByName(channel_name);
    Watching    *watching;
    int            idx;

    if (!channel)
        return;
    if ((idx = channelIdx(user,channel)) >= 0)
    {
        watching = user->watching[idx];
        ParserFreeString(watching->name);
        ParserFreeStruct(watching);
        eaRemove(&user->watching,idx);
        channelOffline(channel,user);
    }
    else if((idx = channelReserveIdx(user, channel)) >= 0)
    {
        watching = user->reserved[idx];
        ParserFreeString(watching->name);
        ParserFreeStruct(watching);
        eaRemove(&user->reserved,idx);
        sendUserMsg(user,"Leave %s %s %d",channel->name,user->name,isKicking);
    }
    eaFindAndRemove(&user->invites, channel);
    chatUserInsertOrUpdate(user);

    eaiFindAndRemove(&channel->invites,user->auth_id);
    eaFindAndRemove(&channel->members,user);
    eaFindAndRemove(&channel->reserved,user);
    chooseOperatorIfNecessary(channel);

    chatChannelInsertOrUpdate(channel);
    channelDeleteIfEmpty(channel);
}


void channelInviteDeny(User *user, char *channel_name, char *inviter_name)
{
    Channel        *channel = channelFindByName(channel_name);
    User        *inviter = userFindByName(inviter_name);

    if (!channel || !inviter)
        return;

    sendSysMsg(inviter,0,1,"CannotAcceptChatInviteFromYou %s %s",user->name, channel_name);
}


void channelSend(User *user,char *channel_name,char *msg)
{
    Watching    *watching;
    Channel        *channel;
    int            i,resend=0;

    if (!userCanTalk(user))
        return;
    watching = channelFindWatching(user,channel_name);
    if (!watching)
    {
        sendSysMsg(user,0,1,"NotInChannel %s",channel_name);
        return;
    }
    if (!(watching->access & CHANFLAGS_SEND))
    {
        sendSysMsg(user,channel_name,1,"NoPermissionToSendInChannel");
        return;
    }
    channel = watching->channel;
    for(i=eaSize(&channel->online)-1;i>=0;i--)
    {
        if (eaiFind(&channel->online[i]->ignore,user->auth_id) >= 0)
            continue;
        if (resend)
            resendMsg(channel->online[i]);
        else
            resend = sendUserMsg(channel->online[i],"ChanMsg %s %s %s",channel->name,user->name,msg);
    }
}

int changeAccess(User *user,ChannelAccess *access_p,char **args,int count)
{
    unsigned char    *s;
    U32                flag;
    int                i;
    ChannelAccess    access = *access_p;

    for(i=0;i<count;i++)
    {
        s = args[i];
        while(*s && isspace(*s))
        {
            s++;
        }
        if(!*s)
            break;
        if (*s != '-' && *s != '+')
            break;
        s++;
        flag = 0;
        if (stricmp(s,"Join")==0)
            flag = CHANFLAGS_JOIN;
        else if (stricmp(s,"Send")==0)
            flag = CHANFLAGS_SEND;
        else if (stricmp(s,"Operator")==0)
            flag = CHANFLAGS_OPERATOR;
        else
            break;
        if (s[-1] == '-')
            access &= ~flag;
        else
            access |= flag;
    }
    if (i < count)
    {
        if(user)
            sendSysMsg(user,0,1,"InvalidMode %s",args[i]);
        return 0;
    }
    *access_p = access;
    return 1;
}

// user can be NULL, all others must be valid
void channelSetUserAccessHelper(User * user, User * target, Watching * watching, char * option)
{
    Channel * channel = watching->channel;

    if (!changeAccess(user,&watching->access,&option,1))
        return;

    if(target->link)
    {
        int i,resend;

        for(resend = 0, i = eaSize(&channel->online)-1; i >= 0; i--)
        {
            if (resend)
                resendMsg(channel->online[i]);
            else if(user)
                resend = sendSysMsg(channel->online[i],channel->name,0,"UserModeChange %s %s %s",user->name,target->name,option);
            else
                resend = sendSysMsg(channel->online[i],channel->name,0,"UserModeChange %s %s %s",target->name,target->name,option);
        }
        for(resend = 0, i = eaSize(&channel->online)-1; i >= 0; i--)
        {
            if (resend)
                resendMsg(channel->online[i]);
            else
                resend = sendUserMsgJoin(channel->online[i],target,channel,1);
        }
    }
    if (!(watching->access & CHANFLAGS_JOIN))
        channelLeave(target,channel->name,1);
    chatUserInsertOrUpdate(target);
}

void channelSetUserAccess(User *user,char *channel_name,char *user_name,char *option)
{
    Watching    *watching = channelFindWatching(user,channel_name);

    User        *target;

    // allow users to unsilence themselves if they are an op
    if (stricmp(option,"+send")==0)
        target = userFindByName(user_name);
    else
        target = userFindByNameSafe(user,user_name);

    if (!target || !opAllowed(user,watching,target))
        return;
    watching = channelFindWatching(target,channel_name);
    if (!watching)
    {
        sendSysMsg(user,channel_name,1,"NotInChannel %s",user_name);
        return;
    }
    
    channelSetUserAccessHelper(user, target, watching, option);
}



void channelSetAccess(User *user,char *channel_name,char *option)
{
    Watching    *watching = channelFindWatching(user,channel_name);

    if (!opAllowed(user,watching,0))
        return;
    if (!changeAccess(user,&watching->channel->access,&option,1))
        return;

    {
        int        i;
        Channel    *channel = watching->channel;

        for(i=eaSize(&channel->online)-1;i>=0;i--)
        {
            if (eaiFind(&channel->online[i]->ignore,user->auth_id) >= 0)
                continue;
            sendSysMsg(channel->online[i],channel_name,0,"ChannelModeChange %s %s",user->name,option);
            channelSendAccess(channel->online[i],channel);
        }
        chatChannelInsertOrUpdate(channel);
    }
}

// this doesn't do any auth checking, but it should only be accessible to admins
void channelCsrMembersAccess(User *user, char *channel_name, char *option)
{
    int        i;
    Channel *channel = channelFindByName(channel_name);

    if(!channel)
        return;

    for(i = eaSize(&channel->members)-1; i >= 0; i--)
    {
        User    *member = channel->members[i];
        int        idx = channelIdx(member,channel);

        if(EAINRANGE(idx,member->watching))
        {
            Watching *watching = member->watching[idx];

            if (!changeAccess(user,&watching->access,&option,1))
                return;

// Leaving this out for now
            if(member->link)
                sendSysMsg(member,channel->name,0,"UserModeChange %s %s %s",user->name,member->name,option);

            if (!(watching->access & CHANFLAGS_JOIN))
                channelLeave(member,channel->name,1);
            chatUserInsertOrUpdate(member);
        }
    }
}

void channelSetMotd(User *user,char *channel_name,char *motd)
{
    Watching    *watching = channelFindWatching(user,channel_name);
    Email        *email;
    Channel        *channel;
    char        datestr[100];
    int            i,resend=0;

    if (!opAllowed(user,watching,0))
        return;
    channel = watching->channel;
    email = ParserAllocStruct(sizeof(*email));
    email->sent = timerSecondsSince2000();
    email->body = ParserAllocString(motd);
    email->from = user->auth_id;
    if (eaSize(&channel->email) >= MAX_CHANMOTD)
        emailRemove(&channel->email,0);
    emailPush(&channel->email, 0, user, 0, 0, motd, 0, 0 );

    itoa(email->sent,datestr,10);
    for(i=eaSize(&channel->online)-1;i>=0;i--)
    {
        sendUserMsg(channel->online[i],"chanmotd %s %s %s %s",datestr,channel->name,user->name,motd);
    }
    chatChannelInsertOrUpdate(channel);
}

static void channelGetMotd(User *user,Channel *channel)
{
    int            i,num_emails;
    Email        *email;
    User        *from;
    char        datestr[100];

    if (!channel)
        return;
    num_emails = eaSize(&channel->email);
    for(i=0;i<num_emails;i++)
    {
        email = channel->email[i];
        if (email->sent < user->last_online && i+1 != num_emails)
            continue;
        from = userFindByAuthId(email->from);
        if(from)
        {
            itoa(email->sent,datestr,10);
            sendUserMsg(user,"chanmotd %s %s %s %s",datestr,channel->name,from->name,email->body);
        }
    }
    user->last_online = timerSecondsSince2000();
}

void channelSetDescription(User *user, char *channel_name, char *description)
{
    Watching    *watching = channelFindWatching(user,channel_name);
    Channel        *channel;
    int            i;

    if (!opAllowed(user,watching,0))
        return;
    channel = watching->channel;

    ParserFreeString(channel->description);
    channel->description = ParserAllocString(description);
    
    for(i=eaSize(&channel->online)-1;i>=0;i--)
    {
        sendUserMsg(channel->online[i],"chandesc %s %s %s ",channel->name,user->name,description);
    }
    chatChannelInsertOrUpdate(channel);
}

void channelSetTimout(User *user, char *channel_name, char *timeout)
{
    Watching    *watching = channelFindWatching(user,channel_name);
    Channel        *channel;
    int            i, time = atoi(timeout);

    if (!opAllowed(user,watching,0) || (time != 0 && time < 30) || time > 3600 )
        return;

    channel = watching->channel;
    channel->timeout = time;
    sendSysMsg(user,0,0,"chantimoutset %d", time);
    chatChannelInsertOrUpdate(channel);

    if( time )
    {
        U32 now = timerSecondsSince2000();
        for(i=eaSize(&channel->members)-1;i>=0;i--)
        {
            if( (int)(now - channel->members[i]->last_online) > (time * 86400))
                 channelLeave(channel->members[i],channel_name, 1);
        }
    }
}

void channelOnline(Channel *channel,User *user, int refresh)
{
    int        i;

    if (eaFind(&channel->online,user) >= 0)
    {
        if(refresh)
        {
            sendUserMsgJoin(user,user,channel,1);
            channelList(user, channel->name);
        }
        return;
    }
    sendUserMsgJoin(user,user,channel,refresh);
    for(i=eaSize(&channel->online)-1;i>=0;i--)
    {
        sendUserMsgJoin(channel->online[i],user,channel,refresh);
        updateCrossShardStats(user, channel->online[i]);
    }
    channelList(user,channel->name);
    eaPush(&channel->online,user);
    channelGetMotd(user,channel);
}

void channelKill(User *user,char *channel_name)
{
    int        i;
    Channel    *channel = channelFindByName(channel_name);

    if (!channel)
    {
        sendSysMsg(user,0,1,"ChannelDoesNotExist %s",channel_name);
        return;
    }
    for(i=eaSize(&channel->online)-1;i>=0;i--)
        sendUserMsg(channel->online[i],"ChanKill %s %s",channel_name,user->name);
    for(i=eaSize(&channel->members)-1;i>=0;i--)
        channelLeave(channel->members[i],channel_name,0);
}

static int chanPopCompare(const void* chan1Data, const void* chan2Data)
{
    const Channel ** chan1 = (const Channel **)chan1Data;
    const Channel ** chan2 = (const Channel **)chan2Data;
    return eaSize(&(*chan2)->online) - eaSize(&(*chan1)->online);
}

void channelFind(User *user,char *mode,char *substring)
{
    int        i,found=0;
    Channel    *chan;

    if (strcmp(mode,"list") == 0)
    {
        char **str = &substring;
        char *token;
        while (token = strsep(str,","))
        {
            int members = 0,online = 0;
            char *description = "";
            chan = channelFindByName(token);
            if (chan)
            {
                members = eaSize(&chan->members);
                online = eaSize(&chan->online);
                description = chan->description;
            }
            sendUserMsg(user,"Chaninfo %s %d %d %s",token,members,online,description);
            found++;
        }
    }
    else if (strcmp(mode,"memberof") == 0)
    {
        for(i=eaSize(&user->watching)-1;i>=0;i--)
        {
            chan = user->watching[i]->channel;
            if (chan)
                sendUserMsg(user,"Chaninfo %s %d %d %s",chan->name,eaSize(&chan->members),eaSize(&chan->online),chan->description);
            found++;
        }
    }
    else //mode = "substring"
    {    
        int size;
        Channel **channels = chatChannelGetAll();
        Channel **chanlist = NULL;

        for(i = eaSize(&channels)-1; i >= 0; --i)
        {
            chan = channels[i];
    
            if ((chan->access & CHANFLAGS_JOIN) && strstri(chan->name,substring))
            {
                eaPush(&chanlist,chan);
            }            
        }

        eaQSort(chanlist, chanPopCompare);

        size = eaSize(&chanlist);
        for(i = 0; i < size; i++)
        {    
            char *pName;
            char *pDesc;

            chan = chanlist[i];

            if (chan->name == NULL)
                pName = "";
            else 
                pName = chan->name;

            if (chan->description == NULL)
                pDesc = "";
            else 
                pDesc = chan->description;

            sendUserMsg(user,"Chaninfo %s %d %d %s", pName, eaSize(&chan->members), eaSize(&chan->online), pDesc);
            found++;
            if (found >= 20)
                break;
        }
        eaDestroy(&chanlist);
    }
    if (found == 0)
    {
        sendUserMsg(user,"Chaninfo %s %d %d %s","",0,0,"");
    }
}
