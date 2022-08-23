#include "shardcomm.h"
#include <utilitieslib/network/netio.h>
#include "dbcomm.h"
#include "entity/entity.h"
#include "svr/svr_player.h"
#include "svr/svr_base.h"
#include "comm_game.h"
#include <utilitieslib/utils/utils.h>
#include "logcomm.h"
#include "entity/entPlayer.h"
#include "shardcommtest.h"
#include <utilitieslib/utils/timing.h>
#include "cmdparse/cmdserver.h"
#include "gameComm/sendToClient.h"
#include "language/langServerUtil.h"
#include <utilitieslib/components/EString.h>
#include "chatdb.h"
#include "gameComm/svr_chat.h"
#include "entity/teamCommon.h"
#include <utilitieslib/components/HashFunctions.h>
#include "container/containerEmail.h"
#include "gameSys/arenamap.h"
#include "plaque.h"

// for status message
#include "entity/character_level.h"
#include "entity/character_base.h"
#include "entity/origins.h"
#include <utilitieslib/components/earray.h>
#include "dbnamecache.h"
#include "comm_backend.h"
#include "staticMapInfo.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/mathutil.h>
#include "logcomm.h"
static Packet    *aggregate_pak;

static int chatserver_running;

int chatServerRunning()
{
    return (chatserver_running && log_comm_link.connected);
}

void flushAggregate()
{
    if (aggregate_pak)
        pktSend(&aggregate_pak,&log_comm_link);
}

int getAuthId(Entity *e)
{
    if (!e)
        return 0;
    devassert (e->auth_id);
    return e->auth_id;
}

Entity *entFromAuthId(int auth_id)
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_OWNED];
    int            i,ent_auth;
    Entity        *e;

    for (i=0; i<players->count; i++)
    {
        e = players->ents[i];
        ent_auth = e->auth_id;
        if (!ent_auth)
            ent_auth = getAuthId(e);
        if (ent_auth == auth_id)
            return e;
    }
    return 0;
}

static void sendChatServerDownToAllClients()
{
    int i;
    PlayerEnts    *players = &player_ents[PLAYER_ENTS_ACTIVE];

    for(i=0;i<players->count;i++)
    {
        START_PACKET(pak, players->ents[i], SERVER_SHARDCOMM);
        pktSendString(pak,"ChatServerStatus 0");
        END_PACKET
    }
}

int checkSpecialCommand(Entity *e,char *str, U32 repeat)
{
    char    *args[MAX_IGNORES] = {0},buf[5000];
    int        i,count;

    strncpy(buf,str,sizeof(buf)-1);
    count = tokenize_line_safe(buf, args, ARRAY_SIZE(args), 0);
    if (!count)
        return 1;
    for(i=0;i<count;i++)
        strcpy(args[i],unescapeString(args[i]));
    if (stricmp(args[0],"ChatServerStatus")==0)
    {
        chatserver_running = atoi(args[1]);
        if (chatserver_running)
        {
            shardCommReconnect();
        }
        else
        {
            sendChatServerDownToAllClients();
        }
    }
    if (!e)
        return 1;

    if(stricmp(args[0], "UserMsg")==0 && count >= 3)
    {
        LOG_ENT(e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "UserMsg To: '%s' From: '%s' Msg: %s", e->pl->chat_handle, args[1], unescapeString(args[2]))
    }
    else if(stricmp(args[0], "ChanMsg")==0 && count == 4)
    {
        if(!repeat)    
            LOG_ENT(e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "ChanSend Channel: '%s' From: '%s' Msg: %s", args[1], args[2], args[3]);
    }
    else if(stricmp(args[0], "StoredMsg")==0 && count >= 4)
    {
        char buf[200];
        timerMakeDateStringFromSecondsSince2000(buf, atoi(args[1]));
        LOG_ENT(e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "StoredMsg %s To: '%s' From: '%s' Msg: %s", buf, e->pl->chat_handle, args[2], unescapeString(args[3]));
    }
    else if (stricmp(args[0],"Login")==0)
    {    
        if(count >= 3 && args[2])
        {
            strncpy(e->pl->chat_handle,args[2],sizeof(e->pl->chat_handle)-1);
            LOG_ENT(e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "Global Login" );
        }
        else
            LOG_ENT(e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "Login Bad Login Message received from chat server: %s.  Only %d arguments.", str, count);
    }
    else if (stricmp(args[0],"Name")==0)
    {
        if(count >= 3 && stricmp(e->pl->chat_handle,args[1])==0)
        {
            strncpy(e->pl->chat_handle,args[2],sizeof(e->pl->chat_handle)-1);
            LOG_ENT(e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "Chat handle received: %s", e->pl->chat_handle );
        }
        else
            LOG_ENT(e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "ChatSvr:Name Bad Name Message received from chat server: %s.  Only %d arguments.", str, count);
    }
    else if (stricmp(args[0],"IgnoreAuth")==0)
    {
        memset( e->ignoredAuthId, 0, sizeof(e->ignoredAuthId) );
        for( i = 1;  i < count && i < ARRAY_SIZE(e->ignoredAuthId); i++ )
            e->ignoredAuthId[i] = atoi(args[i]);
        serverParseClient("emailheaders",e->client); 
        return 1;
    }
    else if (stricmp(args[0],"SendSpamPetition")==0)
    {
        U32 time = dbSecondsSince2000();
        if(count >= 2)
            e->chat_ban_expire = MAX(e->chat_ban_expire,time + atoi(args[1]));
        if( count >=3 && args[2] && atoi(args[2]) > 0 )
            e->pl->send_spam_petition = 1;
        return 1;
    }
    else if( stricmp(args[0],"ClearSpam")==0 )
    {
        LOG_ENT(e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "Cleard of Spammer Status" );
        e->chat_ban_expire = 0;
        e->pl->send_spam_petition = 0;
        e->pl->is_a_spammer = 0;
        return 1;
    }
    else if ( stricmp(args[0],"GMail")==0)
    {
        if(count>=8)
        {
            char buf[200];
            timerMakeDateStringFromSecondsSince2000(buf, atoi(args[6]));
            LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Gmail %s To: '%s' From: '%s' Subj: %s Msg: %s Attachment: %s Id: %s", buf, e->pl->chat_handle, args[1], args[3], unescapeString(args[4]), args[5], args[7] );

        }
        else
        {
            LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Gmail Bad GMail Message received from chat server: %s.  Only %d arguments.", str, count);
        }
    }
    else if( stricmp(args[0],"GMailClaimAck")==0 )
    {
        char * str;
        int mail_id = atoi(args[1]);
        if (e && e->pl && !OnArenaMap())
        {
            e->pl->gmail_pending_state = ENT_GMAIL_SEND_CLAIM_COMMIT_REQUEST;
            e->pl->gmail_pending_requestTime = timerSecondsSince2000();

            // need to break out inf & attachments
            str = strchr(args[2], ' ');
            if( str )
            {
                str++;
                if( *str )
                {
                    e->pl->gmail_pending_influence = atoi(str);
                }
            }

            str = strchr(str, ' ');
            if( str )
            {
                str++;
                if( *str )
                {
                    strcpy_s(e->pl->gmail_pending_attachment, 255, str);
                }
            }
        
            e->auctionPersistFlag = true;
            LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Gmail Acknowledge %i", mail_id );
            shardCommSendf(e, 0, "gmailClaimConfirm %i", mail_id);    
        }

        /*
        int success = emailClaimItems( e, args[2], mail_id ); 

        for (i = 0; i < MAX_GMAIL; ++i)
        {
            GMailClaimState *mail = &e->pl->gmailClaimState[i];

            if (mail->mail_id == mail_id)
            {
                if (success)
                    mail->claim_state = ENT_GMAIL_CLAIM_WAIT_CLEAR;
                else
                {
                    mail->mail_id = 0;
                    mail->claim_state = ENT_GMAIL_CLAIM_NONE;
                }

                shardCommSendf(e, 0, "gmailClear %i %i", mail_id, success );    //    don't need to send this is success is 0 because gmailClear won't do anything with it
                break;
            }
        }
*/
        return 1;
    }
    else if ( stricmp(args[0], "GMailNotFound")==0 )
    {
        int email_id = atoi(args[1]);

        if (email_id > 0)
        {
            e->pl->gmail_pending_state = ENT_GMAIL_NONE;
            e->pl->gmail_pending_requestTime = timerSecondsSince2000();
            strcpy_s(e->pl->gmail_pending_attachment, 255, "");
            e->auctionPersistFlag = true;
            LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Gmail Not Found %i", email_id );
        }
        return 1;
    }
    else if ( stricmp(args[0], "GMailClaimCommitAck")==0 )
    {
        int mail_id = atoi(args[1]);
        char buff[100];

        if (mail_id > 0)
        {
            // transaction denied - rollback attachments
            if (strlen(e->pl->gmail_pending_attachment) > 0)
            {
                if (!emailGrantAttachment(e, e->pl->gmail_pending_attachment, 0))
                {
                    strcpy(e->pl->gmail_pending_inventory, e->pl->gmail_pending_attachment);
                }
            }

            // rollback influence
            if (e->pl->gmail_pending_influence > 0)
            {
                if (ent_canAddInfluence(e,e->pl->gmail_pending_influence))
                {
                    ent_AdjInfluence( e, e->pl->gmail_pending_influence, "Email" );
                    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Fail:Inf %d", e->pl->gmail_pending_influence);
                } else {
                    e->pl->gmail_pending_banked_influence = e->pl->gmail_pending_influence;
                }
            }

            e->pl->gmail_pending_state = ENT_GMAIL_NONE;
            e->pl->gmail_pending_requestTime = timerSecondsSince2000();
            strcpy_s(e->pl->gmail_pending_attachment, 255, "");
            e->auctionPersistFlag = true;

            LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Gmail Claim Commited %i", mail_id );
            // let client know to remove claim option
            sprintf(buff, "GmailClaim %i %i", mail_id, 0);    //    last parameter is unused
            START_PACKET(pak, e, SERVER_SHARDCOMM);
            pktSendString(pak,buff);
            END_PACKET;

        }
        return 1;
    }
    else if ( stricmp(args[0], "GMailXactRequestAck")==0 )
    {
        // don't need to relay - if lost, the pending xact will resend and find the new home of the ent
        if (e && e->pl && !OnArenaMap())
        {
            if (atoi(args[1]) == 0)
            {
                // transaction denied - rollback attachments
                if (strlen(e->pl->gmail_pending_attachment) > 0)
                {
                    if (!emailGrantAttachment(e, e->pl->gmail_pending_attachment, 0))
                    {
                        strcpy(e->pl->gmail_pending_inventory, e->pl->gmail_pending_attachment);
                    }
                }

                // rollback influence
                if (e->pl->gmail_pending_influence > 0)
                {
                    if (ent_canAddInfluence(e,e->pl->gmail_pending_influence))
                    {
                        ent_AdjInfluence( e, e->pl->gmail_pending_influence, "Email" );
                        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Fail:inf %d", e->pl->gmail_pending_influence);
                    } else {
                        e->pl->gmail_pending_banked_influence = e->pl->gmail_pending_influence;
                        plaque_SendString(e, "AuctionPlayerInfInvFull");
                    }
                }

                // clear pending
                e->pl->gmail_pending_mail_id = 0;
                e->pl->gmail_pending_xact_id = 0;
                e->pl->gmail_pending_state = ENT_GMAIL_NONE;
                e->pl->gmail_pending_influence = 0;
                e->pl->gmail_pending_requestTime = 0;
                strcpy_s(e->pl->gmail_pending_subject, MAX_GMAIL_SUBJECT, "");
                strcpy_s(e->pl->gmail_pending_to, 32, "");
                strcpy_s(e->pl->gmail_pending_attachment, 255, "");
                if (e->pl->gmail_pending_body)
                    estrDestroy(&e->pl->gmail_pending_body);

                // flush to DB
                e->auctionPersistFlag = true;

            } else {
                e->pl->gmail_pending_xact_id = atoi(args[2]);    
                e->pl->gmail_pending_state = ENT_GMAIL_SEND_COMMIT_REQUEST;

                // send request to commit
                shardCommSendf(e, 1, "gmailcommitrequest \"%s\" %i", e->pl->gmail_pending_to, e->pl->gmail_pending_xact_id );

            }
        }
        return 1;
    }
    else if ( stricmp(args[0], "GMailCommitRequestAck")==0 )
    {
        // all done - clear pending
        e->pl->gmail_pending_mail_id = 0;
        e->pl->gmail_pending_xact_id = 0;
        e->pl->gmail_pending_state = ENT_GMAIL_NONE;
        e->pl->gmail_pending_influence = 0;
        e->pl->gmail_pending_requestTime = 0;
        strcpy_s(e->pl->gmail_pending_subject, MAX_GMAIL_SUBJECT, "");
        strcpy_s(e->pl->gmail_pending_to, 32, "");
        strcpy_s(e->pl->gmail_pending_attachment, 255, "");
        if (e->pl->gmail_pending_body)
            estrDestroy(&e->pl->gmail_pending_body);
        // flush to DB
        e->auctionPersistFlag = true;
        return 1;
    }
    else if ( stricmp(args[0], "LoginEnd")==0 )
    {
        /*
        for (i = 0; i < MAX_GMAIL; ++i)
        {
            GMailClaimState *mail = &e->pl->gmailClaimState[i];
            if (mail->claim_state == ENT_GMAIL_CLAIM_WAIT_CLEAR)
            {
                //    clear the item from the clients view
                char buff[100];
                sprintf(buff, "GmailClaim %i %i", mail->mail_id, 0);    //    last parameter is unused
                START_PACKET(pak, e, SERVER_SHARDCOMM);
                pktSendString(pak,buff);
                END_PACKET;

                //    clear the item from the chatservers view
                shardCommSendf(e, 0, "gmailClear %i %i", mail->mail_id, 1 );
            }
            else if (mail->claim_state == ENT_GMAIL_CLAIM_TRYING)
            {
                //    retry the claim so it'll get updated
                shardCommSendf(e, 0, "gmailClaim %i", mail->mail_id);
            }
        }
        */
    }

    return 0;
}

int shardMessageCallback(Packet *pak,int cmd,NetLink *link)
{
    char    *str;
    U32        auth_id,repeat;
    Entity    *e;

    switch ( cmd )
    {
    case LOGSERVER_SHARDCHAT:
        while(!pktEnd(pak))
        {
            auth_id = pktGetBits(pak,32);
            repeat = pktGetBits(pak,1);
            str = pktGetString(pak);
            e = entFromAuthId(auth_id);
             if (!checkSpecialCommand(e,str,repeat) && e)
            {
                START_PACKET(pak, e, SERVER_SHARDCOMM);
                pktSendString(pak,str);
                END_PACKET;
            }
        }
    break;
    default:
        verify(0 && "invalid switch value.");
    };

    if(cmd != LOGSERVER_SHARDCHAT)
        return 0;

    return 1;
}

void shardCommSendf(Entity *e, bool check_banned, char *fmt, ...)
{
    char str[5000];
    va_list ap;

    va_start(ap, fmt);
    _vsnprintf(str, sizeof(str), fmt, ap);
    va_end(ap);
    str[ARRAY_SIZE(str)-1]='\0';
    
    shardCommSend(e, check_banned, str);
}


void shardCommSend(Entity *e, bool check_banned, char *str)
{
    if((check_banned && chatBanned(e,str)) || entIsTrial(e))
    {
        // banned or trial accounts have limited functionality
        // we need to check here since the chatserver doesn't know about authbits
        char * restricted_commands[] = { "Friend", "Join", "Create", "Send", "ChanDesc", "ChanMotd", "GMail" };
        int i;

        for( i = 0; i < ARRAY_SIZE(restricted_commands); i++)
        {    
            if(strnicmp( str, restricted_commands[i], strlen(restricted_commands[i]) ) ==0)
            {
                chatSendToPlayer(e->db_id, textStd("TrialAccountsCannotDoThis"), INFO_SVR_COM, 0);
                return;
            }
        }
    }

    if (e && e->pl)
    {
        if (!AccountCanWhisper(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
        {
            char * restricted_commands[] = { "SendUser", "GmailXactRequest" };
            int i;

            for( i = 0; i < ARRAY_SIZE(restricted_commands); i++)
            {    
                if(strnicmp( str, restricted_commands[i], strlen(restricted_commands[i]) ) ==0)
                {
                    chatSendToPlayer(e->db_id, textStd("InvalidPermissionTier3"), INFO_SVR_COM, 0);
                    return;
                }
            }
        }

        if (!AccountCanPublicChannelChat(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
        {
            char * restricted_commands[] = { "Send", "Join" };
            int i;

            for( i = 0; i < ARRAY_SIZE(restricted_commands); i++)
            {    
                if(strnicmp( str, restricted_commands[i], strlen(restricted_commands[i]) ) ==0)
                {
                    chatSendToPlayer(e->db_id, textStd("InvalidPermissionTier4"), INFO_SVR_COM, 0);
                    return;
                }
            }
        }

        if (!AccountCanCreatePublicChannel(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
        {
            char * restricted_commands[] = { "Create", "UserMode", "ChanMode", "Invite", "ChanMotd", "ChanDesc", "ChanSetTimeout",  };
            int i;

            for( i = 0; i < ARRAY_SIZE(restricted_commands); i++)
            {    
                if(strnicmp( str, restricted_commands[i], strlen(restricted_commands[i]) ) ==0)
                {
                    chatSendToPlayer(e->db_id, textStd("InvalidPermissionVIP"), INFO_SVR_COM, 0);
                    return;
                }
            }
        }

    }



    shardCommSendInternal(getAuthId(e), str);
}

void shardCommSendInternal(int id, const char *str)
{
    if (!chatServerRunning())
        return;

    if (!aggregate_pak)
        aggregate_pak = pktCreateEx(&log_comm_link,LOGCLIENT_SHARDCHAT);

    pktSendBits(aggregate_pak,32,id); //fixme
    pktSendString(aggregate_pak,str);
    if (aggregate_pak->stream.size > 500000)
        flushAggregate();
}



void shardCommStatus(Entity *e)
{
    if(e && e->pchar && e->pchar->entParent)
    {
        char    buf[1000];
        char *    primary = 0;
        char *    secondary = 0;
        int        teamSize = (e->teamup ? e->teamup->members.count : 0);

        // format is <PLAYER NAME> <DBID> <MAP NAME> <ARCHETYPE> <ORIGIN> <TEAM SIZE> <XP LEVEL>
        // NAME and DBID are required
        sprintf(buf,  "\"%s\" %d \"%s\" \"%s\" \"%s\" %d %d", 
            e->name, 
            e->db_id,
            getTranslatedMapName(db_state.map_id), 
            e->pchar->pclass->pchDisplayName,
            e->pchar->porigin->pchDisplayName,
            teamSize,
            character_CalcExperienceLevel(e->pchar)+1 );

        shardCommSendf(e, true, "Status \"%s\"", escapeString(buf));
    }
}

void shardCommConnect(Entity *e)
{
    shardCommSend(e,true,"LinkEnt");
}

void shardCommLogout(Entity *e)
{
    shardCommSend(e,true,"UnlinkEnt");
    shardCommSend(e,true,"Logout");
}

void shardCommLogin(Entity *e,int force)
{
    char    buf[1000];
    char    passwordMD5[100],passwordMD5_double_escape[100];

    //if (!force && e->is_map_xfer)
    //    return;

    if(force || ! e->is_map_xfer)
        shardCommConnect(e);
    strcpy(passwordMD5, escapeData((char*)e->passwordMD5, sizeof(e->passwordMD5)));
    strcpy(passwordMD5_double_escape,escapeString(passwordMD5));
    sprintf(buf,"Login \"%s\" \"%s\" %d %d %i", escapeString(e->name),passwordMD5_double_escape,e->access_level,e->db_id, e->pl->hidden);
    shardCommSend(e,true,buf);
    shardCommStatus(e);
}

void shardCommReconnect()
{
    int i;
    PlayerEnts    *players = &player_ents[PLAYER_ENTS_ACTIVE];

    for(i=0;i<players->count;i++)
        shardCommLogin(players->ents[i],1);

    if(gChatServerTestClients)
        shardCommTestReconnect();
}

void shardCommMonitor()
{
    if(!gChatServerTestClients)
        netLinkMonitor(&log_comm_link, 0, shardMessageCallback);
    else
        netLinkMonitor(&log_comm_link, 0, shardTestMessageCallback);

    lnkBatchSend(&log_comm_link);
    if (!chatServerRunning())
    {
        if(chatserver_running && !log_comm_link.connected)
        {
            // logserver crash...
            chatserver_running = 0;
            sendChatServerDownToAllClients();
        }

        if (aggregate_pak)
            pktFree(aggregate_pak);
        aggregate_pak = 0;
    }
    if(gChatServerTestClients && chatServerRunning())
        shardCommTestTick();
    flushAggregate();
}


void handleGenericShardCommCmd(ClientLink * client, Entity * e, char * cmd, char * name)
{
    if(!e)
        return;

    if(!chatServerRunning())
    {
        conPrintf(client,localizedPrintf(e,"NotConnectedToChatServer"));
        return;
    }

    if(name)
    {
        const char * str = getHandleFromString(name);

        if(str)
            shardCommSendf(e, true, "%s \"%s\"", cmd, str);
        else
        {
            if (dbPlayerIdFromName(name) < 0)
                conPrintf(client,localizedPrintf(e,"PlayerDoesntExist",name));
            else
            {
                char buf[1000];
                sprintf(buf,"chat_cmd_relay \"%s\" %d %d %s", escapeString(cmd), getAuthId(e), e->db_id, name);
                serverParseClient(buf, 0);
            }
        }
    }
    else
        shardCommSendf(e,true,"%s",cmd);
}

void handleGenericAuthShardCommCmd(ClientLink * client, Entity * e, char * cmd, int authId)
{
    if(!e)
        return;

    if(!chatServerRunning())
    {
        conPrintf(client,localizedPrintf(e,"NotConnectedToChatServer"));
        return;
    }

    if(authId)
        shardCommSendf(e, true, "%s %i", cmd, authId);
    else
        shardCommSendf(e,true,"%s",cmd);
}



void handleSgChannelInviteHelper(Packet *pak)
{
    char * channel, *cmd=0;
    int i, sender_auth_id, sender_db_id, count;

    sender_auth_id    = pktGetBitsPack(pak,1);
    sender_db_id    = pktGetBitsPack(pak,1);
    channel            = strdup(pktGetString(pak));
    count            = pktGetBitsPack(pak,1);

    if(count)
    {
        estrConcatf(&cmd, "invite \"%s\" %d", escapeString(channel), kChatId_AuthId);
        for(i=0;i<count;i++)
        {
            char    *auth_name    = pktGetString(pak);
            int        auth_id        = pktGetBitsPack(pak, 1);
            devassert(auth_id);
            estrConcatf(&cmd, " %d", auth_id);
        }
        shardCommSendInternal(sender_auth_id, cmd);
    }
    else if(sender_db_id)
        chatSendToPlayer(sender_db_id, localizedPrintf(0,"NoSuperGroupMembersToInvite"), INFO_USER_ERROR, 0 );

    estrDestroy(&cmd);
    free(channel);
}
