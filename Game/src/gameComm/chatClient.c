//***********************************************

#include <utilitieslib/components/EString.h>
#include "gameComm/chatClient.h"
#include "gameComm/chatClient.h"
#include <utilitieslib/components/StashTable.h>
#include "UI/uiFriend.h"
#include "UI/uiChatUtil.h"
#include "UI/uiChat.h"
#include "assert.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/earray.h>
#include "UI/uiConsole.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiDialog.h"
#include "UI/sprite/sprite_text.h"
#include <utilitieslib/utils/timing.h>
#include "UI/uiChannel.h"
#include "UI/uiTarget.h"
#include "UI/uiEmail.h"
#include "render/tex.h"
#include "graphics/textureatlas.h"

#include "entity/entPlayer.h"
#include "player/player.h"
#include "chatdb.h"
#include "cmdparse/cmdgame.h"
#include "arena/arenagame.h"
#include "UI/uiChannelSearch.h"
#include "UI/uiWindows.h"
#include "UI/uiPlaque.h"
#include "UI/uiPlayerNote.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/entity.h"

static char **gPendingInvites;
static int gChatServerRunning;
static bool gCanChangeChatHandle;
static bool gDisableFeedback;

extern int gPrintShardCommCmds;


// special function that returns true if we are connected to the chatserver (allegedly :) )
int UsingChatServer()
{
    return gChatServerRunning;
}

bool canChangeChatHandle()
{
    return gCanChangeChatHandle;
}

// used for group invite slash commands
bool canInviteToChannel(char * channelName)
{
    if(UsingChatServer())
    {
        ChatChannel * channel = GetChannel(channelName);
        
        if(channel)
        {
            if(channelInviteAllowed(channel))
                return 1;
            else
                addSystemChatMsg(textStd("NoChannelInvitePermission", channelName), INFO_USER_ERROR, 0 );
        }
        else
            addSystemChatMsg(textStd("NotInChannel", channelName), INFO_USER_ERROR, 0 );
    }
    else
        addSystemChatMsg( textStd("NotConnectedToChatServer"), INFO_USER_ERROR, 0 );

    return 0;
}

void changeHandleDlgHandler()
{
    char buf[1000];
    sprintf(buf, "change_handle %s", dialogGetTextEntry());
    cmdParse(buf);
}






void chatClientRename()
{
    gCanChangeChatHandle = 1;    
}



// Change chat handle/name
void chatClientName(char * oldHandle, char * newHandle)
{
    int i,k;
    char * targetHandle;

     if(stricmp(oldHandle, game_state.chatHandle) == 0)
    {
        strncpyt(game_state.chatHandle, newHandle, SIZEOF2(GameState, chatHandle));
        changeChatHandleSucceeded();
        gCanChangeChatHandle = 0;
    }
    else
    {
        GlobalFriend * gf;
        // someone elses name changed
        addSystemChatMsg( textStd("UserHandleHasBeenChangeTo", oldHandle, newHandle), INFO_SVR_COM, 0 );    
    
        // update friend list
        gf = GetGlobalFriend(oldHandle);
        if(gf)
            strncpyt(gf->handle, newHandle, SIZEOF2(GlobalFriend, handle));
    }

    for(i=0;i<eaSize(&gChatChannels);i++)
    {
        ChatChannel * channel = gChatChannels[i];
        for(k=0;k<eaSize(&channel->users);k++)
        {
            if(!stricmp(channel->users[k]->handle, oldHandle))
                ChatUserRename(channel->users[k], newHandle);
        }
    }

    UpdateGlobalIgnore(oldHandle, newHandle);

    targetHandle = targetGetHandle();
    if(targetHandle && ! stricmp(targetHandle, oldHandle))
        targetSetHandle(newHandle);
}


void chatClientServerStatus(char * arg)
{
    gChatServerRunning = atoi(arg);
}

void chatClientSysMsg(char * args[], int count)
{
     if(count >= 2)
    {
        char * type = args[1];
        
        if(count >= 4)
        {
            if( ! stricmp(type, "UserMsgSent"))
            {
                char msg[MAX_MESSAGE + MAX_PLAYERNAME + 100];
                 sprintf( msg, "-->@%s: %s", args[2], unescapeString(args[3]));
                addSystemChatMsg(msg, INFO_PRIVATE_COM, 0);
                return;
            }
            else if( !stricmp(type, "CannotInviteAlreadyMember"))
            {
                addSystemChatMsg(textStd("CannotInviteAlreadyMember", args[2], args[3]), INFO_USER_ERROR, 0 );
                return;
            }        
            else if( !stricmp(type, "FriendsListFull"))
            {
                if( ! stricmp(args[2], game_state.chatHandle))
                    addSystemChatMsg(textStd("MyGlobalFriendsListFull", args[3]), INFO_USER_ERROR, 0 );
                else
                    addSystemChatMsg(textStd("TargetGlobalFriendsListFull", args[3]), INFO_USER_ERROR, 0 );
                return;
            }
            else if( !stricmp(type, "YouInvited"))
            {
                addSystemChatMsg(textStd("YouInvited", args[3], args[2]), INFO_SVR_COM, 0 );
                return;
            }
            else if( !stricmp(type, "YouInvitedGroup"))
            {
                addSystemChatMsg(textStd("YouInvitedGroup", atoi(args[3])), INFO_SVR_COM, 0 );
                return;
            }
            else if( !stricmp(type, "ChannelNameNotAllowed"))
            {
                char * reason = args[3];
                if( ! stricmp(reason, "Length"))
                    addSystemChatMsg(textStd("InvalidChannelLength", MAX_CHANNELNAME), INFO_USER_ERROR, 0 );
                else if( ! stricmp(reason, "MultipleSymbols"))
                    addSystemChatMsg(textStd("InvalidChannelMultipleSymbols"), INFO_USER_ERROR, 0 );
                else
                    addSystemChatMsg(textStd("ChannelNameNotAllowed", args[2]), INFO_USER_ERROR, 0 );
                    
                ChatFilterAssignPendingChannel(0, args[2]);
                return;
            }
            else if( !stricmp(type, "UserNameNotAllowed"))
            {
                char * reason = args[3];
                if( ! stricmp(reason, "Length"))
                    addSystemChatMsg(textStd("InvalidHandleLength", MAX_PLAYERNAME), INFO_USER_ERROR, 0 );
                else if( ! stricmp(reason, "MultipleSymbols"))
                    addSystemChatMsg(textStd("InvalidHandleMultipleSymbols"), INFO_USER_ERROR, 0 );
                else
                    addSystemChatMsg(textStd("UserNameNotAllowed", args[2]), INFO_USER_ERROR, 0 );

                return;
            }
            else if( !stricmp(type, "CannotInviteAlreadyInvited"))
            {
                addSystemChatMsg(textStd("CannotInviteAlreadyInvited", args[2], args[3]), INFO_USER_ERROR, 0 );
                return;
            }
            else if( !stricmp(type, "CannotInviteMaxInvites"))
            {
                addSystemChatMsg(textStd("CannotInviteMaxInvites", args[2], args[3]), INFO_USER_ERROR, 0 );
                return;
            }
            else if ( !stricmp(type, "Ignoring"))
            {
                addSystemChatMsg(textStd("Ignoring", args[2]), INFO_SVR_COM, 0);
                AddGlobalIgnore(args[2], atoi(args[3]));
                return;
            }
            else if ( !stricmp(type, "IgnoringUpdate"))    // same as "Ignoring" but don't print message
            {
                AddGlobalIgnore(args[2], atoi(args[3]));
                return;
            }
            else if( !stricmp(type, "CannotAcceptChatInviteFromYou"))
            {
                addSystemChatMsg(textStd("CannotAcceptChatInviteFromYou", args[2], args[3]), INFO_USER_ERROR, 0 );
                return;
            }
        }


        if(count >= 3)
        {
            if( ! stricmp(type, "FriendReq"))
            {
                addSystemChatMsg( textStd("SentGlobalFriendReqest", args[2]), INFO_SVR_COM, 0 );    
                return;
            }
            else if( ! stricmp(type, "Unfriend"))
            {
                addSystemChatMsg( textStd( "NotifyGlobalFriendDecline", args[2] ), INFO_SVR_COM, 0 );    
                return;
            }
            else if( ! stricmp(type, "AlreadyFriendsWith"))
            {
                addSystemChatMsg( textStd("YouAreAlreadyFriendsWith", args[2]), INFO_USER_ERROR, 0 );    
                return;
            }
            else if( ! stricmp(type, "NotFriendsWith"))
            {
                addSystemChatMsg( textStd("YouAreNotFriendsWith", args[2]), INFO_USER_ERROR, 0 );    
                return;
            }
            else if( ! stricmp(type, "ChannelDoesNotExist"))
            {
                addSystemChatMsg( textStd("ChannelDoesNotExist", args[2]), INFO_USER_ERROR, 0 );    
                return;
            }
            else if( ! stricmp(type, "JoinChannelDoesNotExist"))
            {
                Strncpyt(tmpChannelName,args[2]);
                dialogStd( DIALOG_YES_NO, textStd("JoinChannelAgainDialog", tmpChannelName), NULL, NULL, createChannelDlgHandlerNoEntry, NULL, MAX_CHANNELNAME );
                ChatFilterAssignPendingChannel(0, args[2]);
                return;
            }
            else if( ! stricmp(type, "CreateChannelAlreadyExists"))
            {
                Strncpyt(tmpChannelName,args[2]);
                dialogStd( DIALOG_YES_NO, textStd("CreateChannelAgainDialog", args[2]) , NULL, NULL, joinChannelDlgHandlerNoEntry, NULL, MAX_CHANNELNAME );
                ChatFilterAssignPendingChannel(0, args[2]);
                return;
            }
            else if( !stricmp(type, "NotJoinableChannel"))
            {
                addSystemChatMsg( textStd("UnableToJoinPrivateChannel", args[2]), INFO_USER_ERROR, 0 );
                ChatFilterAssignPendingChannel(0, args[2]);    
                return;
            }
            else if( !stricmp(type, "ChannelFull"))
            {
                addSystemChatMsg( textStd("UnableToJoinChannelFull", args[2]), INFO_USER_ERROR, 0 );    
                ChatFilterAssignPendingChannel(0, args[2]);
                return;
            }
            else if( !stricmp(type, "PlayerNotOnlineMessageQueued"))
            {
                 addSystemChatMsg(textStd("PlayerNotOnlineMessageQueued", args[2]), INFO_SVR_COM, 0 );
                return;
            }
            else if( !stricmp(type, "PlayerNotOnlineMailboxFull"))
            {
                addSystemChatMsg(textStd("PlayerNotOnlineMailboxFull", args[2]), INFO_USER_ERROR, 0 );
                return;
            }
            else if( !stricmp(type, "WatchingTooManyChannels"))
            {
                addSystemChatMsg(textStd("WatchingTooManyChannels", args[2]), INFO_USER_ERROR, 0 );
                ChatFilterAssignPendingChannel(0, args[2]);
                return;
            }
            else if( !stricmp(type, "NotInChannel"))
            {
                addSystemChatMsg(textStd("NotInChannel", args[2]), INFO_USER_ERROR, 0 );
                return;
            }
            else if( !stricmp(type, "InvalidMode"))
            {
                addSystemChatMsg(textStd("InvalidMode", args[2]), INFO_USER_ERROR, 0 );
                return;
            }
            else if( !stricmp(type, "ChannelModeChange"))
            {
                // TODO

                return;
            }
            else if( !stricmp(type, "Silenced"))
            {
                int minutes = atoi(args[3]);
                int days = minutes / (60 * 24);

                if( ! stricmp(args[2], game_state.chatHandle))
                {
                    if(days)
                        addSystemChatMsg(textStd("YouHaveBeenSilencedDays", days), INFO_ADMIN_COM, 0 );
                    else
                        addSystemChatMsg(textStd("YouHaveBeenSilencedMinutes", minutes), INFO_ADMIN_COM, 0 );

                }
                else
                {
                    if(days)
                        addSystemChatMsg(textStd("YouHaveSilencedUserDays", args[2], days), INFO_SVR_COM, 0 );
                    else
                        addSystemChatMsg(textStd("YouHaveSilencedUserMinutes", args[2], minutes), INFO_SVR_COM, 0 );
                }

                return;
            }
            else if( !stricmp(type, "HandleAlreadyInUse"))
            {
                changeChatHandleAgainDialog(args[2]);
                return;
            }
            else if( !stricmp(type, "CsrHandleAlreadyInUse"))
            {
                addSystemChatMsg(textStd("CsrHandleAlreadyInUse", args[2]), INFO_USER_ERROR, 0 );
                return;
            }
            else if( !stricmp(type, "UnignoredBy"))
            {
                addSystemChatMsg(textStd("UnignoredBy", args[2]), INFO_SVR_COM, 0 );
                return;
            }
            else if( !stricmp(type, "IgnoredBy"))
            {
                addSystemChatMsg(textStd("IgnoredBy", args[2]), INFO_SVR_COM, 0 );
                return;
            }
            else if ( !stricmp(type, "Unignore"))
            {
                addSystemChatMsg(textStd("StoppedIgnoring", args[2]), INFO_SVR_COM, 0);
                RemoveGlobalIgnore(args[2]);
                return;
            }
            else if ( !stricmp(type, "NotIgnore"))
            {
                addSystemChatMsg(textStd("NotIgnoring"), INFO_SVR_COM, 0);
                return;
            }
            else if( !stricmp(type, "IgnoreListFullListeningTo"))
            {
                addSystemChatMsg(textStd("IgnoreListFullListeningTo", args[2]), INFO_USER_ERROR, 0 );
                return;
            }
            else if( !stricmp(type, "CannotInviteMaxChannels"))
            {
                addSystemChatMsg(textStd("CannotInviteMaxChannels", args[2]), INFO_USER_ERROR, 0 );
                return;
            }
            else if( !stricmp(type, "CannotInviteUser"))
            {
                addSystemChatMsg(textStd("CannotInviteUser", args[2]), INFO_USER_ERROR, 0);
                return;
            }
            else if( !stricmp(type, "InvalidCommand"))
            {
                addSystemChatMsg(textStd("BadCommand", args[2]), INFO_USER_ERROR, 0);
                return;
            }
            else if( !stricmp(type, "WrongNumberOfParams"))
            {
                addSystemChatMsg(textStd("WrongParams", args[2]), INFO_USER_ERROR, 0);
                return;
            }
            else if( !stricmp(type, "ParameterTooLong"))
            {
                addSystemChatMsg(textStd("ParamTooLong", args[2]), INFO_USER_ERROR, 0);
                return;
            }
            else if( !stricmp(type, "CannotInviteMaxMembers"))
            {
                addSystemChatMsg(textStd("CannotInviteMaxMembers", args[2]), INFO_USER_ERROR, 0);
                return;
            }
            else if( !stricmp(type, "chantimoutset"))
            {
                int days = atoi(args[2]);
                if(days)
                    addSystemChatMsg(textStd("ChanTimeoutSet", days), INFO_SVR_COM, 0);
                else
                    addSystemChatMsg(textStd("ChanTimeoutSetOff"), INFO_SVR_COM, 0);
                return;
            }
        }

        if(count >= 2)
        {
            if( !stricmp(type, "AlreadyRenamed"))
            {
                addSystemChatMsg(textStd("ChatHandleAlreadyRenamed"), INFO_USER_ERROR, 0 );
                return;
            }
            else if( !stricmp(type, "NotAllowed"))
            {
                addSystemChatMsg(textStd("ChatActionNotAllowed"), INFO_USER_ERROR, 0 );
                return;
            }
            else if( !stricmp(type, "UnableToJoinHidden"))
            {
                addSystemChatMsg(textStd("UnableToJoinHidden"), INFO_USER_ERROR, 0 );
                return;
            }
            else if( !stricmp(type, "DecliningAll"))
            {
                addSystemChatMsg(textStd("DecliningAll"), INFO_USER_ERROR, 0 );
                return;
            }
            else if( !stricmp(type, "NotifySilenced"))
            {
                  int min = atoi(args[2]);
                int hour = min / 60;
                min =  min % 60;
                addSystemChatMsg(textStd("NofityGlobalChatSilenced", game_state.chatHandle, hour, min), INFO_ADMIN_COM, 0 );
                return;
            }
            else if( !stricmp(type, "AdminSilenceAll") || !stricmp(type, "AdminUnsilenceAll"))
            {
                addSystemChatMsg(textStd(type), INFO_ADMIN_COM, 0 );
                return;
            } 
            else if( !stricmp(type, "YouMayRenameYourChatHandle"))
            {
                addSystemChatMsg(textStd(type), INFO_ADMIN_COM, 0);
                return;
            }
            else if ( !stricmp(type, "SpamThresholdSet") )
            {
                addSystemChatMsg("Spam Threshold Set", INFO_DEBUG_INFO, 0); 
                return;
            }
            else if ( !stricmp(type, "SpamMultiplierSet") )
            {
                addSystemChatMsg("Spam Multiplier Set", INFO_DEBUG_INFO, 0);
                return;
            }
            else if ( !stricmp(type, "SpamDurationSet") )
            {
                addSystemChatMsg("Spam Duration Set", INFO_DEBUG_INFO, 0);
                return;
            }
            else if( !stricmp(type, "AttachmentAlreadyClaimed"))
            {
                addSystemChatMsg(textStd(type), INFO_USER_ERROR, 0);
                return;
            }
            else if( !stricmp(type, "CannotDeleteEmailWithAttachment"))
            {
                addSystemChatMsg(textStd(type), INFO_USER_ERROR, 0);
                return;
            }
        } 

        if(gPrintShardCommCmds)
            conPrintf("DID NOT HANDLE: SysMsg %s (%d total params)", type, count);
    }
    else if(gPrintShardCommCmds)
        conPrintf("DID NOT HANDLE: SysMsg (No paramters)");
}


void chatClientInvalidUser(char * handle)
{
    if( ! stricmp(handle, game_state.chatHandle))
        addSystemChatMsg( textStd("InvalidUserSelf"), INFO_USER_ERROR, 0);
    else
        addSystemChatMsg( textStd("InvalidUser", handle), INFO_USER_ERROR, 0 );    
}

void chatClientUserMsg(char * handle, char * msg)
{
    char buf[100 + MAX_MESSAGE + MAX_PLAYERNAME];

    if( amOnArenaMap() ) // bounce back tell if player is in arena match
    {
        sprintf(buf, "tell @%s, %s", handle, textStd("PlayerIsInEvent"));
        cmdParse(buf);
    }
    else
    {
         sprintf(buf, "@%s: %s", handle, unescapeString(msg));
        addSystemChatMsg(buf, INFO_PRIVATE_COM, 0);
        SetLastPrivate(handle, 1);
    }
}


void chatClientStoredMsg(char * secStr, char * handle, char * msg)
{
    char buf[100 + MAX_MESSAGE + MAX_PLAYERNAME];
    char dateStr[200];
    int time = atoi(secStr);
    
    timerMakeDateStringFromSecondsSince2000(dateStr, time);

    sprintf(buf, "%s Message From @%s : %s", dateStr, handle, unescapeString(msg));
    addSystemChatMsg(buf, INFO_PRIVATE_COM, 0);
    SetLastPrivate(handle, 1);
}


// SYNTAX: <channel> <msg>
void chatClientChanSysMsg(char *args[], int count)
{
    char * channel = args[1];
    char * type = args[2];

    if(count >= 6)
    {
        if( ! stricmp(type, "UserModeChange"))
        {
            char * admin    = args[3];
            char * target    = args[4];
            char * mode        = args[5];

            if( ! stricmp(target, game_state.chatHandle))
            {
                // action done to you
                if(strstri(mode, "-join"))
                    addGlobalChatMsg(channel, 0, textStd("YouWereKickedFromChannel", channel, admin), -1);
                if(strstri(mode, "-send"))
                    addGlobalChatMsg(channel, 0, textStd("YouWereSilencedInChannel", channel, admin), -1);
                if(strstri(mode, "+send"))
                    addGlobalChatMsg(channel, 0, textStd("YouWereUnilencedInChannel", channel, admin), -1);
                if(strstri(mode, "-operator"))
                    addGlobalChatMsg(channel, 0, textStd("YouUnmadeChannelOperator", channel, admin), -1);
                if(strstri(mode, "+operator"))
                    addGlobalChatMsg(channel, 0, textStd("YouBecameChannelOperator", channel, admin), -1);
            }
            else
            {
                if(strstri(mode, "-join"))
                    addGlobalChatMsg(channel, 0, textStd("UserKickedFromChannel", target, channel, admin), -1);
                if(strstri(mode, "-send"))
                    addGlobalChatMsg(channel, 0, textStd("UserSilencedInChannel", target, channel, admin), -1);
                if(strstri(mode, "+send"))
                    addGlobalChatMsg(channel, 0, textStd("UserUnsilencedInChannel", target, channel, admin), -1);
                if(strstri(mode, "-operator"))
                    addGlobalChatMsg(channel, 0, textStd("UserUnmadeChannelOperator", target, channel, admin), -1);
                if(strstri(mode, "+operator"))
                    addGlobalChatMsg(channel, 0, textStd("UserBecameChannelOperator", target, channel, admin), -1);
            }
        }
    } 
     else if(count >= 3)
    {
           if( ! stricmp(type, "NoPermissionToSendInChannel"))
        {
            addGlobalChatMsg(channel, 0, textStd("NoPermissionToSendInChannel", args[1]), -1 );
            return;
        }
    }
}



int computeFlags(char * flagStr)
{
    int flag = 0;

    if(strstri(flagStr, "o"))
        flag |= CHANFLAGS_OPERATOR;
    if( ! strstri(flagStr, "s"))
        flag |= CHANFLAGS_SEND;
    if(strstri(flagStr, "j"))
        flag |= CHANFLAGS_JOIN;
    if(strstri(flagStr, "a"))
        flag |= CHANFLAGS_ADMIN;

    return flag;
}

// debug only (copied from chatserver\channels.c)
AtlasTex * flagIcon(int flags)
{
    if (flags & CHANFLAGS_ADMIN)
        return atlasLoadTexture( "Temporary_HolyShotgunShells.tga" );
     if (flags & CHANFLAGS_OPERATOR)
        return atlasLoadTexture( "MissionPicker_icon_star.tga" );
    if (! (flags & CHANFLAGS_SEND))
        return atlasLoadTexture( "Temporary_GimpStick.tga" );

    return NULL;
}

void chatClientChannel(char * channelName, char * flags)
{
    ChatChannel * cc = GetChannel(channelName);

    if(!cc)
        cc = ChatChannelCreate(channelName, 0);

    if(cc)
        cc->flags = computeFlags(flags);
}

void chatClientChannelInfo(char * channelName, char * channelMembers, char *channelOnline, char* channelDesc)
{
    int members = atoi(channelMembers);
    int online = atoi(channelOnline);
    receiveChannelData(channelName,channelDesc,members,online);
}


void chatClientJoin(char * channelName, char * handle, char * db_id, char * flags, char * refresh)
{
     ChatChannel * cc = GetChannel(channelName);
    ChatUser * user;

    if(!cc)
        cc = GetReservedChannel(channelName);
    if(!cc)
        cc = ChatChannelCreate(channelName, 0);
    if(!cc)
        return;

    user  = ChatChannelUpdateUser(cc, handle, atoi(db_id), computeFlags(flags));
    if(!stricmp(game_state.chatHandle, handle))
    {
        if(!cc->me && !gDisableFeedback)
        {
            addSystemChatMsg( textStd("JoinedChannelNotification", channelName), INFO_SVR_COM, 0 );
            if (window_getMode(WDW_CHANNEL_SEARCH) != WINDOW_DOCKED)
            {
                // Refresh channel screen if open
                chanRequestSearch(0,0);
            }
        }

        if(cc->isReserved)
        {
            assert(GetReservedChannel(cc->name));
            assert( ! GetChannel(cc->name));
            eaFindAndRemove(&gReservedChatChannels, cc);
            eaPush(&gChatChannels, cc);
            cc->isReserved = 0;
            notifyRebuildChannelWindow();
            assert( ! GetReservedChannel(cc->name));
            assert(GetChannel(cc->name));
        }

        cc->me = user;
        ChatFilterAssignPendingChannel(cc, 0);
        if(!atoi(refresh))
            chatChannelSet(cc->name); 
    }
}

void chatClientReserve(char * channelName)
{    
    // if we get a reserved message for a channel that already exists on the client, we have a problem, so ignore
    ChatChannel * cc = GetChannel(channelName);
    if(!cc)
    {
        cc = ChatChannelCreate(channelName, 1);
        if(cc)
            ChatFilterAssignPendingChannel(cc, 0);
    }
}

void chatClientLeave(char * channelName, char * handle, char *isKicking)
{
    ChatChannel * cc = GetChannel(channelName);
    if(!cc)
        cc = GetReservedChannel(channelName);

    if(cc)
    {
        if( ! stricmp(game_state.chatHandle, handle))
        {            
            if(!gDisableFeedback && ! cc->isReserved)
            {
                if (atoi(isKicking))
                    addSystemChatMsg( textStd("KickedFromChannelNotification", channelName), INFO_SVR_COM, 0 );
                else
                    addSystemChatMsg( textStd("LeftChannelNotification", channelName), INFO_SVR_COM, 0 );
                if (window_getMode(WDW_CHANNEL_SEARCH) != WINDOW_DOCKED)
                {
                    // Refresh channel screen if open
                    chanRequestSearch(0,0);
                }
            }

            ChatChannelCloseAndDestroy(cc);
             sendChatSettingsToServer();
        }
        else
            ChatChannelRemoveUser(cc, handle);
    }
    else if (atoi(isKicking))
        addSystemChatMsg( textStd("ChannelLeaveErr", channelName), INFO_USER_ERROR, 0 );
}

void sendChannelInviteAccept(void *data)
{
    if(eaSize(&gPendingInvites))
    {
        joinChatChannel(gPendingInvites[0]);
        free(eaRemove(&gPendingInvites, 0));
    }
}

void sendChannelInviteDecline(void *data)
{
    if(eaSize(&gPendingInvites))
    {
        leaveChatChannel(gPendingInvites[0]);
        free(eaRemove(&gPendingInvites, 0));
    }
}

void chatClientInvite(char * channelName, char * inviter)
{
    Entity *e = playerPtr();

    if (e && e->pl && AccountCanPublicChannelChat(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
    {
        eaPush(&gPendingInvites, strdup(channelName));
        dialogStd( DIALOG_ACCEPT_CANCEL, textStd( "OfferChannelInvite", inviter, channelName ), "YesString",  "NoString", sendChannelInviteAccept, sendChannelInviteDecline, 1 );
    } else {
        addSystemChatMsg( textStd("CannotAcceptChatInvite", inviter, channelName), INFO_SVR_COM, 0 );
        leaveChatChannel(channelName);
        chatChannelInviteDenied(channelName, inviter);
    }
}

void chatClientInviteReminder(char * channelName)
{
    eaPush(&gPendingInvites, strdup(channelName));
    dialogStd( DIALOG_ACCEPT_CANCEL, textStd( "OfferChannelInviteReminder", channelName ), "YesString",  "NoString", sendChannelInviteAccept, sendChannelInviteDecline, 1 );
}

// Login
// SYNTAX: <name>
void chatClientLogin(char * refresh, char * handle, char * shard)
{
    gDisableFeedback = atoi(refresh);

    Strncpyt(game_state.chatHandle, handle);
    Strncpyt(game_state.chatShard, shard);

    // flush all client data
    verbose_printf("Chatserver login: %s, %s\n", handle, shard);

    // clean out channel window
    resetChannelWindow();

    // friends
    friendListPrepareUpdate();
    GlobalFriendDestroyAll();

    // channels
    ChatFilterPrepareAllForUpdate(1);

    // ignore list
    RemoveAllGlobalIgnores();

    // get rid of outstanding invitations
    dialogRemove("OfferChannelInvite",            sendChannelInviteAccept, sendChannelInviteDecline);
    dialogRemove("OfferChannelInviteReminder",    sendChannelInviteAccept, sendChannelInviteDecline);
    if(gPendingInvites)
        eaClearEx(&gPendingInvites, 0);

    // print chat handle
    if( ! gDisableFeedback)
        addSystemChatMsg( textStd("UsingChatHandleMessage", game_state.chatHandle), INFO_SVR_COM, 0 );    

    gChatServerRunning = true;

    gCanChangeChatHandle = 0;
}


void chatClientLoginEnd()
{
    gDisableFeedback = 0;
}

void chatClientChanMsg(char * channelName,  char * userName, char * msg)
{
    addGlobalChatMsg( channelName, userName, msg, -1);
}

void chatClientChanMotd(char *secStr, char * channelName, char * postingUserName, char *msg)
{
    char dateStr[200];
     char * str;
    estrCreate(&str);
    timerMakeDateStringFromSecondsSince2000(dateStr, atoi(secStr));
    estrPrintf(&str, "%s: %s (%s): %s", postingUserName, textStd("MOTDWasSet"), dateStr, msg);
    addGlobalChatMsg( channelName, 0, str, -1 );
    estrDestroy(&str);
}

void chatClientChanMember(char * channelName, char * userName)
{
     char * str;
    estrCreate(&str);
    estrConcatf(&str, "<%s>", userName);
    addGlobalChatMsg(channelName, 0, str, -1 );
    estrDestroy(&str);
}

void chatClientChanDesc(char * channelName, char * postingUserName, char *description)
{
    char * str;
    estrCreate(&str);
    estrConcatf(&str, "%s: %s: %s", postingUserName, textStd("DescriptionWasSet"),description);
    addGlobalChatMsg(channelName, 0, str, -1 );
    estrDestroy(&str);

    if (window_getMode(WDW_CHANNEL_SEARCH) != WINDOW_DOCKED)
    {
        // Refresh channel screen if open
        chanRequestSearch(0,0);
    }
}

void chatClientWatching(char * channelName)
{
     ChatChannel * channel = GetChannel(channelName);
    if(channel)
    {
        int i;
        int count = eaSize(&channel->users);
        conPrintf(textStd("WatchingChannelMembers", channelName, count));
        for(i=0;i<count;i++)
        {
            conPrintf("\t\t--> %s", channel->users[i]->handle);
        }
    }
    else
        conPrintf(textStd("WatchingChannel", channelName));

}

void chatClientChannelKill(char * channelName, char * handle)
{
    addGlobalChatMsg(channelName, 0, textStd("ChannelClosedByAdmin", channelName), INFO_ADMIN_COM );
}


void chatClientCsrSendAll(char * handle, char * msg)
{
    addSystemChatMsg(msg, INFO_ADMIN_COM, 0 );
}

void chatClientInvisible() 
{
    // not sending chat message anymore
}

void chatClientVisible()
{
   // not sending chat message anymore
}

void chatClientFriendHide()
{
    game_state.gFriendHide = true;
}

void chatClientFriendUnHide()
{
    game_state.gFriendHide = false;
}

void chatClientTellHide()
{
    game_state.gTellHide = true;
}

void chatClientTellUnHide()
{
    game_state.gTellHide = false;
}


// --------------------------------------------
// FRIEND STUFF
// --------------------------------------------



typedef struct StatusParams
{
    int size;    
    int offset;
    int type;

}StatusParams;


#define GFITEM(x)    SIZEOF2(GlobalFriend, x), OFFSETOF(GlobalFriend, x) 


StatusParams gStatusParams[] =
{
    {GFITEM(name),        PARSETYPE_STR},
    {GFITEM(dbid),        PARSETYPE_S32},        
    {GFITEM(map),        PARSETYPE_STR},
    {GFITEM(archetype),    PARSETYPE_STR},
    {GFITEM(origin),    PARSETYPE_STR},
    {GFITEM(teamSize),    PARSETYPE_S32},
    {GFITEM(xpLevel),    PARSETYPE_S32},
    { 0 }
};


    

//SYNTAX: friend <STATUS> <SHARD>
void chatClientFriend(char * handle, char * db_id, char * status, char * shard)
{
    GlobalFriend * gf;

    // format is   <PLAYER NAME> <XP LEVEL> <MAP NAME> <ARCHETYPE> <ORIGIN> <PRIMARY SET> <SECONDARY SET>
    gf = GetGlobalFriend(handle);
    if(!gf)
        gf = GlobalFriendCreate(handle, atoi(db_id));
    else
    {
        // TODO ... find bettter way...
        gf->archetype[0] = 0;
        gf->map[0] = 0;
        gf->name[0] = 0;
        gf->origin[0] = 0;
        gf->powerset1[0] = 0;
        gf->powerset2[0] = 0;
        gf->shard[0] = 0;
        gf->xpLevel = 0;
        gf->teamSize = 0;
        gf->dbid = 0;
        gf->onSameShard = 0;

        gf->relativeDbId = atoi(db_id);
    }


    if(!stricmp(status, ""))
    {
        gf->status = GFStatus_Offline;
    }
    else if(!stricmp(status, "Online"))
    {
        gf->status = GFStatus_Online;
    }
    else
    {
        char buf[1000];
        char *info[100] = {0};
        int vals;
        int i=0;
        StatusParams * param = &gStatusParams[0];

        gf->status = GFStatus_Playing;

        // shard is separate
        if(shard)
        {
            strncpyt(gf->shard, shard, SIZEOF2(GlobalFriend, shard));
            if(!stricmp(shard, game_state.chatShard))
                gf->onSameShard = true;
        }

        // get rest of status info
        Strncpyt(buf, unescapeString(status));
        vals = tokenize_line_safe(buf,info,(ARRAY_SIZE(gStatusParams)-1),0);    // skip the first double-quote

        #define ADDR(p,offset) ((char *)p + offset)

        while(param->type)
        {
            if(i < vals)
            {
                switch(param->type)
                {
                    case PARSETYPE_STR:
                        strncpyt(ADDR(gf,param->offset), info[i], param->size);
                        break;
                    case PARSETYPE_S32:
                        *((int*)(ADDR(gf,param->offset))) = atoi(info[i]);
                        break;
                    default:
                        assert(!"Bad Type!");
                }
            }

            i++;
            param++;
        }
    }

    // rebuild friends list
    friendListPrepareUpdate();
}


void chatClientUnfriend(char * handle)
{
    GlobalFriend * gf;

    gf = GetGlobalFriend(handle);
    if(!gf)
    {
        addSystemChatMsg(textStd("BadUnfriend",  handle), INFO_USER_ERROR, 0);
        return;
    }

    // rebuild friends list
    globalFriendListRemove(gf);
    GlobalFriendDestroy(gf);

}


void chatClientCsrStatus(char *handle,char *shard,char *status, char *time, char *authid, char *watching)
{
    addSystemChatMsg( textStd("CsrChatStatusMessage",handle,shard,status,time,authid,watching), INFO_SVR_COM, 0 );
}

void chatClientCsrMailTotal( char * playerName, char * inbox, char * sent )
{
    addSystemChatMsg( textStd("CsrMailTotal",playerName,inbox,sent), INFO_SVR_COM, 0 );
    addSystemChatMsg( textStd("CsrBounceExplanation"), INFO_SVR_COM, 0  );
}

void charClientCsrMailInbox( char * id, char * subject, char * attachment, char * from, char * sent )
{
    char date[100];
    emailBuildDateString(date,atoi(sent));
    addSystemChatMsg( textStd("CsrMailInbox", id, date, from, subject, attachment ), INFO_SVR_COM, 0 );
}

void charClientCsrMailSent( char * id, char * sent, char * attachment, char * sentTo, char * status )
{
    char date[100];
    emailBuildDateString(date,atoi(sent));
    addSystemChatMsg( textStd("CsrMailSent", id, date, attachment, sentTo, status ), INFO_SVR_COM, 0 );
}

void chatClientBounceMailResult( char * success )
{
    if( atoi(success) )
        addSystemChatMsg( textStd("CsrMailBounceSuccess"), INFO_SVR_COM, 0 );
    else
        addSystemChatMsg( textStd("CsrMailBounceFail"), INFO_SVR_COM, 0 );
}

void chatClientAccessLevel(char *accessLevel)
{
    // do nothing, this command is used by the ChatAdmin tool
}


static char gHandle[128];

void sendGlobalFriendDecline(void *data)
{
    char buf[1000];
    sprintf(buf, "gunfriend @%s", gHandle);
    cmdParse(buf);
}

void sendGlobalFriendAccept(void *data)
{
    char buf[1000];
    sprintf(buf, "gfriend @%s", gHandle);
    cmdParse(buf);
}
 
void chatClientFriendReq(char * handle)
{
    strncpyt(gHandle, handle, sizeof(gHandle));

    // TODO: hook up to dialog box
    dialogStd( DIALOG_ACCEPT_CANCEL, textStd( "OfferGlobalFriend", gHandle ), "YesString",  "NoString", sendGlobalFriendAccept, sendGlobalFriendDecline, 1 );
}


void chatClientWhoGlobalEx( char * globalName, char * status, int flags )
{
    char buf[1000];
    char *info[100] = {0};

    Strncpyt(buf, unescapeString(status));
    tokenize_line_safe(buf,info,(ARRAY_SIZE(gStatusParams)-1),0);    // skip the first double-quote

    playerNote_addGlobalName( info[0], globalName );
    if(!flags)
        addSystemChatMsg( textStd("GlobalNameIs", info[0], globalName ), INFO_SVR_COM, 0 );
}

void chatClientWhoGlobal( char * globalName, char * status ){ chatClientWhoGlobalEx( globalName, status, 0 ); }
void chatClientWhoGlobalHidden( char * globalName, char * status ){ chatClientWhoGlobalEx( globalName, status, 1 ); }

void chatClientWhoLocalEx( char * globalName, char * status, int invite )
{
    char buf[1000];
    char *info[100] = {0};

    Strncpyt(buf, unescapeString(status));
    tokenize_line_safe(buf,info,(ARRAY_SIZE(gStatusParams)-1),0);    // skip the first double-quote
    playerNote_addGlobalName( info[0], globalName );

    switch (invite)
    {
    case 2:
        cmdParsef("li %s", info[0] );
        break;
    case 1:
        cmdParsef("i %s", info[0] );
        break;
    case 0:
        addSystemChatMsg( textStd("LocalNameIs", globalName, info[0] ), INFO_SVR_COM, 0 );
    default:
        break;

    }
}

void chatClientWhoLocal( char * globalName, char * status, int invite ){ chatClientWhoLocalEx( globalName, status, 0 ); }
void chatClientWhoLocalInvite( char * globalName, char * status ){ chatClientWhoLocalEx( globalName, status, 1 ); }
void chatClientWhoLocalLeagueInvite( char * globalName, char * status ){ chatClientWhoLocalEx( globalName, status, 2 ); }

static void chatClientGmail( char * sender, char * sender_auth, char * subj, char * msg, char * attachment, char * date, char * id )
{
    emailAddHeaderGlobal( sender, atoi(sender_auth), subj, msg, attachment, atoi(date), atoi(id), 0, 0, 0 );
}

static void chatClientGmailClaim( char * id, char * unused){ emailClearAttachments( atoi(id) ); }

// --------------------------------------------------------
// Chat Server Command Handler List
// --------------------------------------------------------



typedef void (*handler0)();
typedef void (*handler1)(char *cmd1);
typedef void (*handler2)(char *cmd1,char *cmd2);
typedef void (*handler3)(char *cmd1,char *cmd2,char *cmd3);
typedef void (*handler4)(char *cmd1,char *cmd2,char *cmd3, char *cmd4);
typedef void (*handler5)(char *cmd1,char *cmd2,char *cmd3, char *cmd4, char * cmd5);
typedef void (*handler6)(char *cmd1,char *cmd2,char *cmd3, char *cmd4, char * cmd5, char *cmd6);
typedef void (*handler7)(char *cmd1,char *cmd2,char *cmd3, char *cmd4, char * cmd5, char *cmd6, char *cmd7);
typedef struct{
        char    *cmdname;
        void    (*handler)();
        U32        cmd_sizes[8];
        int        num_args;
} ClientShardCmd;

static ClientShardCmd cmds[] =
{
        {    "Name",                chatClientName,            {MAX_PLAYERNAME, MAX_PLAYERNAME}},    
        {    "Login",            chatClientLogin,        {32, MAX_PLAYERNAME, MAX_PLAYERNAME}},    
        {    "LoginEnd",            chatClientLoginEnd,        },    
        {    "Renameable",        chatClientRename        },    
        {    "ChatServerStatus",    chatClientServerStatus,    100},

        {    "UserMsg",            chatClientUserMsg,        {MAX_PLAYERNAME, MAX_MESSAGE}},
        {    "StoredMsg",        chatClientStoredMsg,    {64, MAX_PLAYERNAME, MAX_MESSAGE}},

                
        {    "Channel",            chatClientChannel,        {MAX_CHANNELNAME, 64}},
        {    "ChanInfo",            chatClientChannelInfo,    {MAX_CHANNELNAME, 64, 64, MAX_CHANNELDESC}},
        {    "Join",                chatClientJoin,            {MAX_CHANNELNAME, MAX_PLAYERNAME, 64, 64, 1}},
        {    "Reserve",            chatClientReserve,        {MAX_CHANNELNAME}},
        {    "Leave",            chatClientLeave,        {MAX_CHANNELNAME, MAX_PLAYERNAME, 10}},
        {    "Invite",            chatClientInvite,        {MAX_CHANNELNAME, MAX_PLAYERNAME}},
        {    "InviteReminder",    chatClientInviteReminder,    {MAX_CHANNELNAME}},
        {    "ChanMsg",            chatClientChanMsg,        {MAX_CHANNELNAME, MAX_PLAYERNAME,MAX_MESSAGE}},
        {    "ChanMotd",            chatClientChanMotd,        {100, MAX_CHANNELNAME, MAX_PLAYERNAME, MAX_MESSAGE}},
        {    "ChanDesc",            chatClientChanDesc,        {MAX_CHANNELNAME, MAX_PLAYERNAME, MAX_CHANNELDESC}},
        {    "chanmember",        chatClientChanMember,    {MAX_CHANNELNAME, MAX_PLAYERNAME}},

        {    "ChanKill",            chatClientChannelKill,    {MAX_CHANNELNAME, MAX_PLAYERNAME}},
        {    "CsrSendAll",        chatClientCsrSendAll,    {MAX_MESSAGE, MAX_MESSAGE}},

        {    "Watching",            chatClientWatching,        {MAX_CHANNELNAME}},

        {    "Invisible",        chatClientInvisible        },
        {    "Visible",            chatClientVisible        },
        {    "FriendHide",        chatClientFriendHide    },
        {    "FriendUnHide",        chatClientFriendUnHide    },
        {    "TellHide",            chatClientTellHide        },
        {    "TellUnHide",        chatClientTellUnHide    },

        {    "FriendReq",        chatClientFriendReq,    {MAX_PLAYERNAME}},
        {    "Friend",            chatClientFriend,        {MAX_PLAYERNAME, 64, MAX_FRIENDSTATUS, MAX_FRIENDSTATUS}},
        {    "UnFriend",            chatClientUnfriend,        {MAX_PLAYERNAME}},
        
        {    "InvalidUser",        chatClientInvalidUser,    {MAX_PLAYERNAME}},

        {    "AccessLevel",        chatClientAccessLevel,    {64}},
        {    "CsrStatus",        chatClientCsrStatus,    {MAX_PLAYERNAME,64,MAX_FRIENDSTATUS,64,64,64}},        
        {    "WhoGlobal",        chatClientWhoGlobal,        {MAX_PLAYERNAME, MAX_FRIENDSTATUS}},
        {    "WhoGlobalHidden",    chatClientWhoGlobalHidden,    {MAX_PLAYERNAME, MAX_FRIENDSTATUS}},
        {    "WhoLocal",            chatClientWhoLocal,            {MAX_PLAYERNAME, MAX_FRIENDSTATUS}},
        {    "WhoLocalInvite",    chatClientWhoLocalInvite,    {MAX_PLAYERNAME, MAX_FRIENDSTATUS}},
        {    "WhoLocalLeagueInvite",    chatClientWhoLocalLeagueInvite,    {MAX_PLAYERNAME, MAX_FRIENDSTATUS}},
        
        {    "Gmail",            chatClientGmail, {MAX_PLAYERNAME, 64,  MAX_SUBJECT, MAX_MESSAGE, MAX_PATH, 64, 12 }},
        {    "GmailClaim",        chatClientGmailClaim, { 12, MAX_PATH }},

        {    "CsrMailTotal",     chatClientCsrMailTotal, {MAX_PLAYERNAME, 12, 12}},
        {   "CsrMailInbox",        charClientCsrMailInbox,    {12, MAX_SUBJECT,MAX_PATH,MAX_PLAYERNAME, 12}},
        {   "CsrMailSent",        charClientCsrMailSent,    {12, 12, MAX_PATH, MAX_PLAYERNAME, MAX_MESSAGE}},
        {   "CsrBounceMailResult", chatClientBounceMailResult, {12}},

        { 0 },
};

static StashTable gClientCmds;




void chatClientInit()
{
    int        i,j;

    gClientCmds = stashTableCreateWithStringKeys(100,StashDeepCopyKeys);
    for(i=0;cmds[i].cmdname;i++)
    {
        for(j=0;cmds[i].cmd_sizes[j];j++)
            cmds[i].num_args++;

        stashAddPointer(gClientCmds,cmds[i].cmdname,&cmds[i], false);
    }

    // sanity checks -- since i have set some of these to higher-than-current values in the database, i just wanna make sure
    // that new chatserver settings don't break the code
    assert(MAX_CHAT_CHANNELS >= MAX_WATCHING);
    assert(MAX_CHANNELNAME < SIZEOF2(ChatChannelSettings, name));
}


// returns whether the cmd was processed successfully.
bool processShardCmd(char **args,int count)
{
    Entity *e = playerPtr();

    if(!stricmp(args[0], "SysMsg"))
    {
        chatClientSysMsg(args, count);
    }
    else if(!stricmp(args[0], "ChanSysMsg"))
    {
        chatClientChanSysMsg(args, count);
    }
    else if(!stricmp(args[0], "SMFMsg")) // Formatted popup, just use the plaque UI
    {
        plaquePopup((char*)unescapeString(args[1]),0);
    }
       else
    {
        ClientShardCmd    *cmd = stashFindPointerReturnPointer(gClientCmds,args[0]);
         int    i;

        if (!cmd)
        {
            if( e && e->access_level )
                addSystemChatMsg(textStd("BadCommand",args[0]), INFO_USER_ERROR, 0);
            return false;
        }
        count--;
        if (cmd->num_args != count)
        {
            if( e && e->access_level )
                addSystemChatMsg(textStd("WrongParamsLong",args[0], count, cmd->num_args), INFO_USER_ERROR, 0);
            return false;
        }
        args++;
        for(i=0;i<count;i++)
        {
            if (strlen(args[i]) > cmd->cmd_sizes[i])
            {
                if( e && e->access_level )
                    addSystemChatMsg(textStd("ParamTooLong",args[i]), INFO_USER_ERROR, 0);
                return false;
            }
        }
        switch(cmd->num_args)
        {
            xcase 0:
                ((handler0)cmd->handler)();
            xcase 1:
                ((handler1)cmd->handler)(args[0]);
            xcase 2:
                ((handler2)cmd->handler)(args[0],args[1]);
            xcase 3:
                ((handler3)cmd->handler)(args[0],args[1],args[2]);
            xcase 4:
                ((handler4)cmd->handler)(args[0],args[1],args[2],args[3]);
            xcase 5:
                ((handler5)cmd->handler)(args[0],args[1],args[2],args[3], args[4]);
            xcase 6:
                ((handler6)cmd->handler)(args[0],args[1],args[2],args[3], args[4], args[5]);
            xcase 7:
                ((handler7)cmd->handler)(args[0],args[1],args[2],args[3], args[4], args[5], args[6]);
        }
    }
    return true;
}
