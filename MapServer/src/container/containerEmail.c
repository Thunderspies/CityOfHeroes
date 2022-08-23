#include "container/dbcontainerpack.h"
#include "entity/character_level.h"
#include <utilitieslib/utils/timing.h>
#include "dbcomm/dbcomm.h"
#include "containerEmail.h"
#include "entity/entVarUpdate.h"
#include "svr/svr_player.h"
#include "svr/svr_base.h"
#include "comm_game.h"
#include "containerbroadcast.h"
#include "gameComm/svr_chat.h"
#include <utilitieslib/components/StashTable.h>
#include "dbcomm/dbnamecache.h"
#include "entity/entity.h"
#include "comm_backend.h"
#include <utilitieslib/components/EString.h>
#include "language/langServerUtil.h"
#include "entity/entPlayer.h"
#include "entity/friends.h"
#include "entity/SgrpServer.h"
#include "script/script.h"
#include "account/AccountInventory.h"
#include "gameComm/trayCommon.h"
#include "bases/DetailRecipe.h"
#include "entity/salvage.h"
#include "auction/AuctionData.h"
#include "entity/powers.h"
#include "entity/character_base.h"
#include "entity/character_combat.h"
#include "dbcomm/shardcomm.h"
#include "plaque.h"
#include "gameSys/arenamap.h"
#include "dbcomm/logcomm.h"

struct EmailInfo
{
    int        id;
    char    *message;
};

#define MAX_RECIPIENTS 100
#define MAX_SUBJECT 80
#define MAX_BODY 3900
#define MIN_SECONDS_BETWEEN_EMAILS 15
#define MAX_EMAILS_PER_DAY 150

#define PENDING_XACT_RETRY_TIME        30

typedef struct
{
    int        recipient;
    int        state;        // 1 = new, 0 = deleted
} Recipient;

typedef struct
{
    int            message_id;

    int            sender;
    int            sender_auth;
    char        subject[MAX_SUBJECT*2];
    char        msg[MAX_BODY*2];
    U32            sent;
    Recipient    recipients[MAX_RECIPIENTS*2];
    int            influence;
    char        attachment[MAX_PATH];
} Email;

typedef struct
{
    int        sender_id;
    int        sender_auth_id;
    U32        date;
    int        message_id;
    char    subject[MAX_SUBJECT*2];
    int        ids[MAX_RECIPIENTS];
    int        count;
    int        influence;
    char    attachment[MAX_PATH];
} NewHeader;

static NewHeader    **new_headers;
static int            new_header_count,new_header_max;

LineDesc recipient_line_desc[] =
{
    {{ PACKTYPE_CONREF,    CONTAINER_ENTS,    "Recipient",    OFFSET(Recipient,recipient),INOUT(0,0),LINEDESCFLAG_INDEXEDCOLUMN },    "TODO"},
    {{ PACKTYPE_INT,    SIZE_INT8,        "State",        OFFSET(Recipient,state), },     "TODO"},
    { 0 },
};

StructDesc recipient_desc[] =
{
    sizeof(Recipient), {AT_STRUCT_ARRAY, OFFSET(Email, recipients)}, recipient_line_desc,

    "TODO"
};

LineDesc email_line_desc[] =
{
    {{ PACKTYPE_CONREF,    CONTAINER_ENTS,            "Sender",        OFFSET(Email,sender), },    "Sending Entity"},
    {{ PACKTYPE_STR_UTF8, SIZEOF2(Email,subject),    "Subject",        OFFSET(Email,subject),  INOUT(0,0),0, MAX_SUBJECT },    "Email Subject String"},
    {{ PACKTYPE_STR_UTF8, SIZEOF2(Email,msg),        "Msg",            OFFSET(Email,msg), INOUT(0,0),0, MAX_BODY },    "Email Msg String"},
    {{ PACKTYPE_DATE, 0,                        "Sent",            OFFSET(Email,sent),    },    "Time Email was Sent"},    
    {{ PACKTYPE_INT, SIZE_INT32,                    "SenderAuth",    OFFSET(Email,sender_auth), },    "Auth ID of the Sender"},

    {{ PACKTYPE_SUB, MAX_RECIPIENTS,            "Recipients",   (intptr_t)recipient_desc },    "Who gets the email"},
    { 0 },
};

StructDesc email_desc[] = 
{
    sizeof(Email), 
    {AT_STRUCT_ARRAY,{0}}, 
    email_line_desc,
    "TODO"
};


MP_DEFINE(EmailInfo);

EmailInfo *emailAllocInfo()
{
    MP_CREATE(EmailInfo, 16);
    return MP_ALLOC(EmailInfo);
}

void emailFreeInfo(EmailInfo *email_info)
{
    MP_FREE(EmailInfo, email_info);
}

char *emailTemplate()
{
    return dbContainerTemplate(email_desc);
}

char *emailSchema()
{
    return dbContainerSchema(email_desc, "Email");
}


void emailSend(int sender_id, int auth_id, char *subject,char *message,int *db_ids,int count,int *container_id_ptr, int influence, char * pchAttachment)
{
    /*Email    email;
    char    *str;
    int        i;
    U32        cookie;

    memset(&email,0,sizeof(email));
    for(i=0;i<count;i++)
    {
        email.recipients[i].recipient = db_ids[i];
        email.recipients[i].state = 1;
    }
    Strncpyt(email.msg,message);
    Strncpyt(email.subject,subject);
    Strncpyt(email.attachment,pchAttachment?pchAttachment:"");
    email.sent = dbSecondsSince2000();
    email.sender = sender_id;
    email.sender_auth = auth_id;
    email.influence = influence;

    str = dbContainerPackage(email_desc,(char*)&email);
    cookie = dbSetDataCallback(container_id_ptr);
    dbAsyncContainerUpdate(CONTAINER_EMAIL,-1,CONTAINER_CMD_CREATE,str,cookie);
    free(str);*/
}

static void emailUpdateAccountStats(Entity *e, U32 lastEmailTime, U32 lastNumEmailsSent)
{
    Packet *pak_out;

    pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_UPDATE_EMAIL_STATS);
    pktSendBitsAuto(pak_out, e->auth_id);
    pktSendBitsAuto(pak_out, lastEmailTime);
    pktSendBitsAuto(pak_out, lastNumEmailsSent);
    pktSend(&pak_out, &db_comm_link);

    if (e->pl)
    {
        e->pl->lastEmailTime = lastEmailTime;
        e->pl->lastNumEmailsSent = lastNumEmailsSent;
    }
}

void checkPendingTransactions(Entity *e)
{
    if (e == NULL || e->pl == NULL || e->pchar == NULL)
        return;

    // checking pending emails
    if (e->pl->gmail_pending_state != ENT_GMAIL_NONE && (timerSecondsSince2000() - e->pl->gmail_pending_requestTime) > PENDING_XACT_RETRY_TIME)
    {
        if (e->pl->gmail_pending_state == ENT_GMAIL_SEND_XACT_REQUEST)
        {            
            if( e->pl->gmail_pending_influence || *e->pl->gmail_pending_attachment )
            {
                shardCommSendf(e, 1, "gmailxactrequest \"%s\" \"%s\" \"%s\" \"%i %i %s\"", e->pl->gmail_pending_to, e->pl->gmail_pending_subject, 
                e->pl->gmail_pending_body, e->pl->playerType, e->pl->gmail_pending_influence, e->pl->gmail_pending_attachment );
                e->pl->gmail_pending_requestTime = timerSecondsSince2000();
            } else {
                shardCommSendf(e, 1, "gmailxactrequest \"%s\" \"%s\" \"%s\" \"%i\"", e->pl->gmail_pending_to, e->pl->gmail_pending_subject, 
                e->pl->gmail_pending_body, e->pl->playerType);
                e->pl->gmail_pending_requestTime = timerSecondsSince2000(); 
            }
        } 
        else if (e->pl->gmail_pending_state == ENT_GMAIL_SEND_COMMIT_REQUEST)
        {
            shardCommSendf(e, 1, "gmailcommitrequest \"%s\" %i", e->pl->gmail_pending_to, e->pl->gmail_pending_xact_id );
        }
        else if (e->pl->gmail_pending_state == ENT_GMAIL_SEND_CLAIM_REQUEST)
        {
            shardCommSendf(e, 1, "GmailClaimRequest %i", e->pl->gmail_pending_mail_id);
            e->pl->gmail_pending_requestTime = timerSecondsSince2000(); 
        }
        else if (e->pl->gmail_pending_state == ENT_GMAIL_SEND_CLAIM_COMMIT_REQUEST)
        {
            shardCommSendf(e, 0, "gmailClaimConfirm %i", e->pl->gmail_pending_mail_id);    
            e->pl->gmail_pending_requestTime = timerSecondsSince2000(); 
        }
    }

    // checking pending inventory
    if (strlen(e->pl->gmail_pending_inventory) > 0)
    {
        if (emailGrantAttachment(e, e->pl->gmail_pending_inventory, 0))
        {
            strcpy(e->pl->gmail_pending_inventory, "");

            // flush to DB
            e->auctionPersistFlag = true;
        }
    }

    // rollback influence
    if (e->pl->gmail_pending_banked_influence > 0)
    {
        if (ent_canAddInfluence(e,e->pl->gmail_pending_banked_influence))
        {
            ent_AdjInfluence( e, e->pl->gmail_pending_banked_influence, "Email" );
            LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Fail:Inf %d", 
                e->pl->gmail_pending_influence);

            e->pl->gmail_pending_banked_influence = 0;

            // flush to DB
            e->auctionPersistFlag = true;
        } else {
            // check to see if we can add some of it
            S64 cap = getEntMaxInf(e);
            S64 diff = cap - (S64)e->pchar->iInfluencePoints;

            if (ent_canAddInfluence(e, (int) diff))
            {
                ent_AdjInfluence( e, e->pl->gmail_pending_banked_influence, "Email" );
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Fail:Inf %d", 
                    e->pl->gmail_pending_influence);

                e->pl->gmail_pending_banked_influence -= (int) diff;

                // flush to DB
                e->auctionPersistFlag = true;
            } else {
                plaque_SendString(e, "AuctionPlayerInfInvFull");
            }
        }
    }

} 

char *emailSendToNames(int sender_id, int auth_id,char *subject,char *message,char *args[], int count, int influence, char * pchAttachment)
{
    static    char        *bad_names = 0;
    static    int            err_count,err_max;
    int                    i,db_ids[MAX_RECIPIENTS],unique_count=0,id,unique_global_count=0;
    StashTable    dup_recips;
    NewHeader            *header;
 
    err_count = 0;
    dup_recips = stashTableCreateInt(4);
    if (count > MAX_RECIPIENTS)
    {
        return localizedPrintf(0, "EmailTooManyRecip", count - MAX_RECIPIENTS);
    }

    for(i=0;i<count;i++)
    {
        if(*args[i] == '@' )
        {
            Entity * e = entFromDbId(sender_id);

            if( !chatServerRunning() )
            {
                return localizedPrintf(0, "NotConnectedToChatServer");
            }
            else
            {
                const char * str = getHandleFromString(args[i]);
                if( e && e->pl && str ) 
                {
                    int strlength = (int)strlen(str);
                    if (strlength > MAX_PLAYERNAME)    //    in chatClient, it loses the e-mail if it is longer than MAX_PLAYERNAME
                    {
                        return localizedPrintf(e,"EmailRecipientTooLongError");
                    } 
                    else if (e->pl->gmail_pending_state != ENT_GMAIL_NONE || 
                                *e->pl->gmail_pending_inventory != 0 || e->pl->gmail_pending_banked_influence > 0)
                    {
                        checkPendingTransactions(e);
                        return localizedPrintf(e,"GmailOutstandingPendingTransaction");
                    } else {
                        char * subj = strdup( escapeString(subject) );
                        if( influence || (pchAttachment && *pchAttachment) )
                            shardCommSendf(e, 1, "gmailxactrequest \"%s\" \"%s\" \"%s\" \"%i %i %s\"", str, subj, escapeString(message), e->pl->playerType, influence, pchAttachment );
                        else 
                            shardCommSendf(e, 1, "gmailxactrequest \"%s\" \"%s\" \"%s\" \"%i\"", str, subj, escapeString(message), e->pl->playerType);
                            
                        // log transaction for attachment emails to ensure deliver
                        if (e->pl)
                        {
                            e->pl->gmail_pending_mail_id = 0;
                            e->pl->gmail_pending_xact_id = 0;
                            e->pl->gmail_pending_state = ENT_GMAIL_SEND_XACT_REQUEST;
                            e->pl->gmail_pending_influence = influence;
                            e->pl->gmail_pending_requestTime = timerSecondsSince2000();
                            strcpy_s(e->pl->gmail_pending_subject, MAX_GMAIL_SUBJECT, subj);
                            strcpy_s(e->pl->gmail_pending_to, 32, str);
                            strcpy_s(e->pl->gmail_pending_attachment, 255, pchAttachment);

                            if (e->pl->gmail_pending_body)
                                estrDestroy(&e->pl->gmail_pending_body);
                            e->pl->gmail_pending_body = estrCloneCharString(escapeString(message));

                            // flush to DB
                            e->auctionPersistFlag = true;
                        }
                        free(subj);
                    }
                }
                else
                    return localizedPrintf(e,"EmailFormatError");
            }
        }
        else
        {
            id = dbPlayerIdFromName(args[i]);
            if (id <= 0)
            {
                if (err_count)
                    estrConcatf(&bad_names, ", %s", args[i] );
                else
                    estrConcatCharString(&bad_names, args[i]);

                err_count++;
                continue;
            }

            if (stashIntAddInt(dup_recips, id, id, false))
            {
                db_ids[unique_count++] = id;
            }
        }
    }

    stashTableDestroy(dup_recips);
    if (err_count)
        return localizedPrintf(0, "EmailMissingRecipeints", bad_names );

    if( unique_count )
    {
        header = calloc(sizeof(NewHeader),1);
        dynArrayAddp(&new_headers,&new_header_count,&new_header_max,header);
        header->sender_id    = sender_id;
        header->sender_auth_id    = auth_id;
        header->date        = dbSecondsSince2000();
        header->count        = unique_count;
        header->influence    = influence;
        Strncpyt(header->attachment , pchAttachment);
        assert(unique_count <= ARRAY_SIZE(db_ids));
        memcpy(header->ids,db_ids,sizeof(int) * unique_count);
        Strncpyt(header->subject,subject);
        emailSend(sender_id,auth_id,subject,message,db_ids,unique_count,&header->message_id, influence, pchAttachment);
    }

    return 0;
}

void createEmailFromDbServer( Packet * pak )
{
    char *name, *namebuff, *title, *message;
    int author_auth_id, commenter_auth_id;

    strdup_alloca(name,  pktGetString(pak));
    strdup_alloca(title, pktGetString(pak));
    
    author_auth_id = pktGetBitsAuto(pak);
    commenter_auth_id = pktGetBitsAuto(pak);

    strdup_alloca(message, pktGetString(pak));

    if( authIdFromName(name) != author_auth_id )
        return; // author must've been re-named


    estrCreate(&namebuff);
    estrConcatf(&namebuff, "\"%s\"", name );
    emailSendToNames(0, commenter_auth_id, localizedPrintf(0,"ArchitectFeedback", title), message, &namebuff, 1, 0, 0);
    estrDestroy(&namebuff);
}

void createSystemEmail( Entity *e, char* senderName, char* subject, char* msg, int influence, char *attachment, int delaytime )
{
    shardCommSendf(e, 0, "SystemGmail \"%s\" \"%s\" \"%s\" \"%i %i %s\" \"%i\"", senderName, subject, msg, e->pl->playerType, influence, attachment, delaytime );
}

void emailCheckNewHeaders()
{
    int            i;
    char        buf[1000];
    NewHeader    *header;

    for(i=new_header_count-1;i>=0;i--)
    {
        header = new_headers[i];
        if (header->message_id > 0)
        {
            sprintf(buf,"%s%d %d \"%s\" %d %i \"%s\"",DBMSG_EMAIL_MSG,header->message_id, header->sender_auth_id, escapeString(header->subject),header->date, header->influence, header->attachment);
            dbBroadcastMsg(    CONTAINER_ENTS, header->ids, INFO_ADMIN_COM, header->sender_id, header->count, buf);
            memmove(&new_headers[i],&new_headers[i+1],(--new_header_count - i) * sizeof(NewHeader*));
            free(header);
        }
    }
}

void emailSendNewHeader(int db_id,char *str,int sender_id)
{
    char    *args[10];
    int auth;
    int redacted = 0;
    Entity    *e = entFromDbIdEvenSleeping(db_id);

    if (!e)
        return;
    tokenize_line(str,args,0);

    auth = atoi(args[1]); 

    if( e->pl->disableEmail || 
        isIgnored(e,sender_id) || 
        (auth && isIgnoredAuth(e, auth)) || 
        (!auth && e->pl->ArchitectBlockComments) ||
        (e->pl->friendSgEmailOnly && !(isFriend(e, sender_id) || sgroup_IsMember(e, sender_id)) ))
    {
        emailDeleteMessage(e,atoi(args[0]));
        return;
    }

    START_PACKET( pak_out, e, SERVER_SEND_EMAIL_HEADERS )
    pktSendBitsPack(pak_out,1,0); // delta update
    pktSendBitsPack(pak_out,1,1); // 1 update
    pktSendBits( pak_out, 1, 1 );

    pktSendBits(pak_out,32,atoi(args[0]));
    pktSendBits( pak_out, 1, redacted );

    if( !sender_id )
    {
        pktSendBits(pak_out,32,0);
        pktSendString(pak_out,localizedPrintf(0,"EmailFromSystem"));
    }
    else
    {
        pktSendBits(pak_out,32,auth);
        pktSendString(pak_out,dbPlayerNameFromId(sender_id));
    }
    pktSendString(pak_out,args[2]);
    pktSendBits(pak_out,32,atoi(args[3]));
    END_PACKET
}

static void emailHeaders_cb(Packet *pak,int db_id,int count)
{
    int        i,message_id,sent,sender_id,auth_id, redacted = 0;
    char    subject[MAX_SUBJECT*2];
    Entity    *e = entFromDbId(db_id);
    if (!e)
        return;

    START_PACKET( pak_out, e, SERVER_SEND_EMAIL_HEADERS )
    pktSendBitsPack(pak_out,1,1); // full update, not delta
    pktSendBitsPack(pak_out,1,count);
    for(i=0;i<count;i++)
    {
        message_id    = pktGetBitsPack(pak,1);
        Strncpyt(subject,pktGetString(pak));
        sender_id    = pktGetBitsPack(pak,1);
        auth_id      = pktGetBitsPack(pak,1);
        sent        = pktGetBits(pak,32);

        if( e->pl->disableEmail || 
            isIgnored(e,sender_id) || 
            (auth_id && isIgnoredAuth(e, auth_id)) || 
            (!auth_id && e->pl->ArchitectBlockComments) ||    
            (e->pl->friendSgEmailOnly && !(isFriend(e, sender_id) || sgroup_IsMember(e, sender_id))))
        {
            emailDeleteMessage(e, message_id);
            pktSendBits( pak_out, 1, 0 );
            continue;
        }
        else
        {
            pktSendBits( pak_out, 1, 1 );
        }

        pktSendBits(pak_out,32,message_id);
        pktSendBits( pak_out, 1, redacted ); // redacted bit

        if( !sender_id )
        {
            pktSendBits(pak_out,32,0);
            pktSendString(pak_out,localizedPrintf(0,"EmailFromSystem"));
        }
        else
        {
            pktSendBits(pak_out,32,auth_id);
            pktSendString(pak_out,dbPlayerNameFromId(sender_id));
        }

        if( redacted )
            pktSendString(pak_out, localizedPrintf(0,"EmailSubjectRedacted"));
        else
            pktSendString(pak_out,subject);

        pktSendBits(pak_out,32,sent);
    }
    END_PACKET
}

static void emailMessage_cb(Packet *pak,int db_id,int count)
{
    Entity    *e;

    if (!count)
        return;
    e = entFromDbId(db_id);
    if (!e)
        return;
    free(e->email_info->message);
    e->email_info->id        = pktGetBitsPack(pak,1);
    e->email_info->message    = strdup(pktGetString(pak));
}

static void emailRecipient_cb(Packet *pak,int db_id,int count)
{
    int        i,recipient,deleted,msg_id;
    Entity    *e = entFromDbId(db_id);

    if (!e || !e->email_info->message)
        return;
    if (!count)
        goto done;

    START_PACKET( pak_out, e, SERVER_SEND_EMAIL_MESSAGE )
    pktSendBits(pak_out,32,e->email_info->id);
    pktSendString(pak_out,e->email_info->message);
    pktSendBitsPack(pak_out,1,count);
    for(i=0;i<count;i++)
    {
        msg_id        = pktGetBitsPack(pak,1);
        recipient    = pktGetBitsPack(pak,1);
        deleted        = pktGetBitsPack(pak,1);
        assert(msg_id == e->email_info->id);
        if( !recipient )
            pktSendString(pak_out,localizedPrintf(0,"EmailFromSystem"));
        else
            pktSendString(pak_out,dbPlayerNameFromId(recipient));
    }
    END_PACKET
done:
    free(e->email_info->message);
    e->email_info->message = 0;
}

void emailGetHeaders(int db_id)
{
    char search[1000];

    sprintf(search,"INNER JOIN Recipients ON Email.ContainerId = Recipients.ContainerId WHERE Recipients.recipient = %d AND Recipients.State > 0",db_id);
    dbReqCustomData(CONTAINER_EMAIL,"Email",0,search,"ContainerId, Subject, Sender, SenderAuth, Sent ",emailHeaders_cb,db_id);
}

void emailGetMessage(int db_id,int message_id)
{
    char search[1000];

    sprintf(search,"INNER JOIN Recipients ON Email.ContainerId = Recipients.ContainerId WHERE Email.ContainerId = %d AND Recipients.recipient = %d AND Recipients.State > 0",message_id,db_id);
    dbReqCustomData(CONTAINER_EMAIL,"Email",0,search,"ContainerId, Msg",emailMessage_cb,db_id);
    sprintf(search,"WHERE ContainerId = %d",message_id);
    dbReqCustomData(CONTAINER_EMAIL,"Recipients",0,search,"ContainerId, Recipient, State",emailRecipient_cb,db_id);
}

void emailDeleteMessage(Entity * e,int message_id)
{
    if( message_id < 0 ) // global mail ids are negative clientside to prevent collision with old email
        shardCommSendf(e, 1, "GmailDelete %i", -message_id );
    else
    {
        int        idx=0;
        char    sql_command[1000];
        char    *recip_clear_state =    "Update Recipients\n"
                                        "set State = 0\n"
                                        "where ContainerId = %d\n"
                                        "And Recipient = %d\n\n",

                *email_del    =             "DECLARE @id AS Integer DECLARE @result AS Integer\n"
                                        "SET @id = %d\n"
                                        "SET @result = ISNULL((SELECT COUNT(*) As Expr1\n"
                                        "FROM Recipients\n"
                                        "GROUP BY State, ContainerId\n"
                                        "HAVING (ContainerId = @id) AND Not (State = 0)), 0)\n"
                                        "IF @result = 0 BEGIN\n"
                                        "DELETE FROM Recipients\n"
                                        "WHERE ContainerId = @id\n"
                                        "DELETE FROM Email\n"
                                        "WHERE ContainerId = @id \n"
                                        "END\n";

        idx += sprintf(sql_command+idx,recip_clear_state,message_id,e->db_id);
        idx += sprintf(sql_command+idx,email_del,message_id);
        dbExecuteSql(sql_command);
    }
}

static void tokenizeRecipients(char *args[], int *count, char *recips)
{
    char* s;

    for(s=recips;*s;s++) 
    {
        if (*s == ';' || *s == ',')
            *s = ' ';
    }

    *count = tokenize_line_safe(recips,args,MAX_RECIPIENTS,0);
}

void emailHandleSendCmd(struct Entity* e, char* subj, char* recips, char* body, int influence, int type, int idx, struct ClientLink* client)
{
    if (!e)
        return;

    if (entIsTrial(e))
    {
        chatSendToPlayer(client->entity->db_id, clientPrintf( client, "CannotEmailTrialAccount"), 
                         INFO_SVR_COM, 0);
    }
    else if (e->pchar && (character_CalcExperienceLevel(e->pchar) < 9))
    {
        chatSendToPlayer(client->entity->db_id, clientPrintf(client, "EmailUnlockedAtLevel10"), 
                         INFO_SVR_COM, 0);
    }
    else if (e->pl && !AccountHasEmailAccess(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
    {
        chatSendToPlayer(client->entity->db_id, clientPrintf(client, "CannotEmailNoPermission"), 
            INFO_SVR_COM, 0);
    }
    else if (e->chat_ban_expire >= timerSecondsSince2000())
    {
        chatSendToPlayer(client->entity->db_id, clientPrintf(client, "CannotEmailChatBanned"), INFO_SVR_COM, 0);
    }
    else
    {
        int count, iCol=0, iRow=0;
        char *args[MAX_RECIPIENTS], *err_buf;
        U32 now = SecondsSince2000();
        U32 today = timerDayFromSecondsSince2000(now);
        U32 lastEmailTime = client->entity->pl->lastEmailTime;
        U32 lastNumEmailsSent = client->entity->pl->lastNumEmailsSent;
        U32 lastEmailDay = (U32)timerDayFromSecondsSince2000(lastEmailTime);
        U32 emailsSentToday = ((today > lastEmailDay) ? 0 : lastNumEmailsSent);
        char * pchAttachment = 0;
        const DetailRecipe *pRec = 0;
        const SalvageItem *pSalvage = 0;
        int validHistory = 1;

        // Ignore their history if it has been corrupted.
        if (lastEmailTime > now + 300 || lastNumEmailsSent < 0)
        {
            validHistory = 0;
            emailsSentToday = 0;
        } 

        if (validHistory && (now - lastEmailTime < MIN_SECONDS_BETWEEN_EMAILS))
        {
            chatSendToPlayer(client->entity->db_id, clientPrintf(client, "MinTimeBetweenEmails", MIN_SECONDS_BETWEEN_EMAILS), INFO_SVR_COM, 0 );
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Spam user tried to send an email too soon after another one.");
            return;
        }

        tokenizeRecipients(args, &count, recips);

        if (validHistory && (emailsSentToday + count > MAX_EMAILS_PER_DAY))
        {
            chatSendToPlayer(client->entity->db_id, clientPrintf(client, "TooManyEmailsToday", MAX_EMAILS_PER_DAY), INFO_SVR_COM, 0 );
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Spam user has sent too many emails today.");
            return;
        }

        if( influence || type )
        {
            if( count > 1 )
            {
                chatSendToPlayer(client->entity->db_id, clientPrintf(client, "EmailAttachmentsOnlyGetOneSender"), INFO_SVR_COM, 0 );
                return;
            }
            if( *args[0] != '@' )
            {
                chatSendToPlayer(client->entity->db_id, clientPrintf(client, "EmailAttachmentsOnlyAllowedForGlobal"), INFO_SVR_COM, 0 );
                return;
            }
        }

        if( influence > 999999999 )
        {
            chatSendToPlayer(client->entity->db_id, clientPrintf(client, "EmailInfluenceCap"), INFO_SVR_COM, 0 );
            return;
        }

        if( influence < 0 || influence > e->pchar->iInfluencePoints )
        {
            chatSendToPlayer(client->entity->db_id, clientPrintf(client, "EmailNotEnoughInfluence"), INFO_SVR_COM, 0 );
            return;
        }

        switch(type)
        {
            xcase kTrayItemType_None: // ok
            xcase kTrayItemType_Recipe:
            {
                int recipeCount;
                pRec = recipe_GetItemById(idx);
                recipeCount = character_RecipeCount(e->pchar, pRec);

                // make sure character actually has something in slot he wants to trade
                if( !(pRec && recipeCount > 0 && !(pRec->flags & RECIPE_NO_TRADE)) || !character_CanRecipeBeChanged( e->pchar, pRec, -1)  )
                {
                    chatSendToPlayer(client->entity->db_id, clientPrintf(client, "RecipeNotTradeable"), INFO_SVR_COM, 0 );
                    return; // trying to trade something not available
                }

                pchAttachment = auction_identifier(type, pRec->pchName, pRec->level);

                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Sent:Rec %s (level %d) from %s to %s", 
                    pRec->pchName, pRec->level, e->name, args[0]);
            }
            xcase kTrayItemType_Salvage: 
            {
                if( e->pchar->salvageInv && idx >= 0 && idx < eaSize(&e->pchar->salvageInv) &&
                    e->pchar->salvageInv[idx] && e->pchar->salvageInv[idx]->salvage )
                {
                    pSalvage = e->pchar->salvageInv[idx]->salvage;

                    if( (e->pchar->salvageInv[idx]->salvage->flags & SALVAGE_NOTRADE) || !character_CanAdjustSalvage( e->pchar, e->pchar->salvageInv[idx]->salvage, -1 ) )
                    {
                        chatSendToPlayer(client->entity->db_id, clientPrintf(client, "SalvageNotTradeable"), INFO_SVR_COM, 0 ); 
                        return; // trying to trade something not available
                    }
                
                    pchAttachment = auction_identifier(type, e->pchar->salvageInv[idx]->salvage->pchName, 0);
                }
                else
                {
                    return;
                }

                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Sent:Sal %s from %s to %s",
                    pSalvage->pchName, e->name, args[0]);
            }
            xcase kTrayItemType_SpecializationInventory:
            {
                if( !(idx >= 0 && idx < CHAR_BOOST_MAX && e->pchar->aBoosts[idx] && e->pchar->aBoosts[idx]->ppowBase) )
                    return;
                else
                {
                    const BasePower *ppow = e->pchar->aBoosts[idx]->ppowBase;
                    int level = e->pchar->aBoosts[idx]->iLevel;
                    if (!detailrecipedict_IsBoostTradeable(ppow, level, e->pl->chat_handle, &(args[0][1])))
                    {
                        //    enhancement isn't tradeable (because recipe isn't)
                        chatSendToPlayer(client->entity->db_id, clientPrintf(client, "EnhancementNotTradeable"), INFO_SVR_COM, 0 ); 
                        return;
                    }
                    else
                    {
                        pchAttachment = auction_identifier(type, basepower_ToPath(ppow), e->pchar->aBoosts[idx]->iLevel);
                    }

                    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Sent:Enh %s (level %d) from %s to %s",
                        ppow->pchName, level, e->name, args[0]);
                }
            }
            xcase kTrayItemType_Inspiration:
            {
                iRow = idx%10;
                iCol = idx/10;
                if( !(iCol >= 0 && iCol < e->pchar->iNumInspirationCols && iRow >= 0 && iRow < e->pchar->iNumInspirationRows &&    e->pchar->aInspirations[iCol][iRow]) )
                {
                    chatSendToPlayer(client->entity->db_id, clientPrintf(client, "InspirationsMoved"), INFO_SVR_COM, 0 ); 
                    return;
                }
                else
                {
                    if( character_IsInspirationSlotInUse(e->pchar, iCol, iRow ) )
                    {
                        chatSendToPlayer(client->entity->db_id, clientPrintf(client, "CannotTradeRechargeOrActivePow"), INFO_SVR_COM, 0 ); 
                        return;
                    }
                    else if (!inspiration_IsTradeable(e->pchar->aInspirations[iCol][iRow], e->pl->chat_handle, &(args[0][1])))
                    {
                        chatSendToPlayer(client->entity->db_id, clientPrintf(client, "InspirationNotTradeable"), INFO_SVR_COM, 0 ); 
                        return;
                    }

                    pchAttachment = auction_identifier(type, basepower_ToPath(e->pchar->aInspirations[iCol][iRow]), 0);
                }

                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Sent:Insp %s from %s to %s",
                    e->pchar->aInspirations[iCol][iRow]->pchName, e->name, args[0]);
            }
            xdefault:
                return; // unrecognized type
        }

        err_buf = emailSendToNames(e->db_id, e->auth_id, subj, body, args, count, influence, pchAttachment?pchAttachment:"");
        START_PACKET( pak, e, SERVER_SEND_EMAIL_MESSAGE_STATUS );
        if (err_buf)
        {
            pktSendBitsPack(pak,1,0);
            pktSendString(pak,err_buf);
        }
        else
        {
            emailUpdateAccountStats(e, now, emailsSentToday + count);

            // Email was sent, subtract influence
            ent_AdjInfluence( e, -influence, "Email" );
            LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Sent:Inf %d from %s to %s", 
                influence, e->name, args[0]);

            switch(type)
            {
                xcase kTrayItemType_None: // ok
                xcase kTrayItemType_Recipe: 
                {
                    character_AdjustRecipe( e->pchar, pRec, -1, "email");
                }
                xcase kTrayItemType_SpecializationInventory: 
                {
                    character_DestroyBoost( e->pchar, idx, "email" );
                }
                xcase kTrayItemType_Salvage: 
                {
                    character_AdjustSalvage( e->pchar, pSalvage, -1, "email", false);
                }
                xcase kTrayItemType_Inspiration:
                {
                    character_RemoveInspiration( e->pchar, iCol, iRow, "email" );
                    character_CompactInspirations(e->pchar);
                }
            }
            pktSendBitsPack(pak,1,1);

            // flush to DB
            e->auctionPersistFlag = true;

        }
        END_PACKET
    }
}

int emailGrantAttachment( Entity *e, char *attachment, int emailIndex)
{
    AuctionItem * pItem;
    if( !stashFindPointer(auctionData_GetDict()->stAuctionItems, attachment, &pItem) || !pItem )
        return 0;

    switch( pItem->type )
    {
        xcase kTrayItemType_None: // ok
        xcase kTrayItemType_SpecializationInventory: 
        {
            if(character_AddBoost(e->pchar, pItem->enhancement, pItem->lvl, 0, "email") == -1 )
            {
                plaque_SendString(e, "AuctionPlayerEnhancementInvFull");
                return 0;
            }
            sendInfoBox(e, INFO_SVR_COM, "ThingClaimed", pItem->enhancement->pchDisplayName);
            LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Claim:Enh %s (level %d) from email #%d",
                pItem->enhancement->pchName, pItem->lvl, emailIndex);
        }
        xcase kTrayItemType_Inspiration:
        {
            if(character_AddInspiration(e->pchar, pItem->inspiration, "email") == -1)
            {
                plaque_SendString(e, "AuctionPlayerInspInvFull");
                return 0;
            }
            sendInfoBox(e, INFO_SVR_COM, "ThingClaimed", pItem->inspiration->pchDisplayName);
            LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Claim:Insp %s from email #%d",
                pItem->inspiration->pchName, emailIndex);
        }
        xcase kTrayItemType_Recipe:
        case kTrayItemType_Salvage:
        {
            InventoryType invtype = InventoryType_FromTrayItemType(pItem->type);
            int invid = genericinv_GetInvIdFromItem(invtype,pItem->pItem);
            if(!character_CanAddInventory(e->pchar, invtype, invid,1) )
            {
                plaque_SendString(e, pItem->type == kTrayItemType_Recipe ? "AuctionPlayerRecipeInvFull" : "AuctionPlayerSalvageInvFull");
                return 0;
            }
            else if(character_AdjustInventory(e->pchar, invtype, invid, 1, "email", false) < 0)
            {
                plaque_SendString(e, pItem->type == kTrayItemType_Recipe ? "AuctionPlayerRecipeInvFull" : "AuctionPlayerSalvageInvFull");
                return 0;
            }
            if( pItem->type == kTrayItemType_Recipe )
            {
                sendInfoBox(e, INFO_SVR_COM, "ThingClaimed", pItem->recipe->ui.pchDisplayName);
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Claim:Rec %s (level %d) from email #%d", pItem->recipe->pchName, pItem->lvl, emailIndex);
            }
            else
            {
                sendInfoBox(e, INFO_SVR_COM, "ThingClaimed", pItem->salvage->ui.pchDisplayName);
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Claim:Sal %s from email #%d", pItem->salvage->pchName, emailIndex);
            }
        }
    }
    return 1;
}

int emailClaimItems( Entity *e, char * attachment, int emailIndex ) // format - "PlayerType Influence AuctionIdentifier"
{
    int influence = 0, max = MAX_INFLUENCE;
    char * str;

    if (OnArenaMap())
    {
        sendInfoBox(e, INFO_SVR_COM, "CantClaimItemsInArena");
        return 0;    // not allowed to claim any attachments while in the Arena.
    }

    if(!attachment || !*attachment )
        return 1;  // success I guess, this way we'll send message to clear email if needed

    // skip past unused PlayerType
    str = strchr(attachment, ' ');
    if( str )
    {
        str++;
        if( *str )
        {
            influence = atoi(str);
            if( !ent_canAddInfluence( e, influence ) )
            {
                plaque_SendString(e, "InfluenceCapHit");
                return 0;
            }
        }
    }
    
    str = strchr(str, ' ');
    if( str )
    {
        str++;
        if( *str )
        {
            if (!emailGrantAttachment(e, str, emailIndex))
                return 0;
        }
    }

    // item passed validation, so we can give influence now
    ent_AdjInfluence( e, influence, "Email" );
    sendInfoBox(e, INFO_SVR_COM, "InfluenceClaimed", influence);
    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Emal]:Claim:Inf %d from email #%d", influence, emailIndex);

    return 1;
}
