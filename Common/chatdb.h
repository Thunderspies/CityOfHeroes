#ifndef _CHAT_DB_H
#define _CHAT_DB_H

#include <utilitieslib/utils/textparser_types.h>


#include <utilitieslib/stdtypes.h>
#include <utilitieslib/network/netio.h>
#ifdef CHATSERVER
#include "loadsave.h"
#endif
#include "chatdefs.h"


typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct User User;

extern ParseTable parse_channel[];
extern ParseTable parse_user[];
extern ParseTable parse_gmailuser[];

typedef enum
{
    CHANFLAGS_JOIN        = 1,
    CHANFLAGS_SEND        = 2,
    CHANFLAGS_OPERATOR    = 4,
    CHANFLAGS_ADMIN        = 8,
    CHANFLAGS_RENAMED    = 16,
    CHANFLAGS_INVISIBLE    = 32,
    CHANFLAGS_FRIENDHIDE= 64,
    CHANFLAGS_TELLHIDE  = 128,
    CHANFLAGS_INVITEHIDE = 256,
    CHANFLAGS_SPAMMER   = 512,
    CHANFLAGS_GMAILFRIENDONLY = 1024,
    CHANFLAGS_COMMON    = CHANFLAGS_JOIN | CHANFLAGS_SEND,
    CHANFLAGS_COPYMASK    = CHANFLAGS_COMMON | CHANFLAGS_OPERATOR,
} ChannelAccess;

typedef enum
{
    EMAIL_STATUS_READY = 0,
    EMAIL_STATUS_PENDING_COMMIT,
    EMAIL_STATUS_PENDING_CLAIM,
    EMAIL_STATUS_DELETED,
} EmailStatus;

typedef struct
{
    int                id;
    U32                sent;
    U32                from;
    U32                to;
    U32                originalSender;
    char            *senderName;
    char            *body;
    char            *subj;
    char            *attachment;
    U32                claim_request;
    EmailStatus        status;
} Email;

typedef struct 
{
    int id;
    int sentTo;
    U32    sent;
    char * attachment;
    U32 attachmentClaimTimestamp;
}SentMail;

typedef struct Channel
{
    char            *name;
    ChannelAccess    access;
    Email            **email;
    char            *description;
    User            **members;
    User            **online;
    User            **reserved;
    U32                *invites;
    U32                timeout;
    U8                adminSent : 1;
    U8                dirty : 1;
} Channel;

typedef struct
{
    char            *name;
    U32                last_read;
    ChannelAccess    access;
    Channel            *channel;
} Watching;

struct User
{
    U32                auth_id;
    char            *name;
    ChannelAccess    access;
    U32                last_online;
    U32                *ignore;
    U32                *ignoredBy;
    Watching        **watching;
    Watching        **reserved;
    Email            **email;
    Email            **gmail;
    SentMail        **sent_mail;  // For CSR
    int                gmail_id;
    U32                *friends;
    User            **befrienders;
    U32                *befriend_reqs;
    U32                silenced;
    Channel            **invites;
    char            *status;
    NetLink            *link;
    int                publink_id;
    int                db_id;
    U32                message_hash; // hashString'ed message file

    union{        // md5 hash of user's auth password
        U32            hash[4];
        struct        { U32 hash0, hash1, hash2, hash3;};
    };

    U8                access_level;

    int                reported_spam_count;
    int                spam_ignore_count;
    U32                chat_ban_expire;

    U8                adminSent : 1;
    U8                dirty : 1;
};

typedef struct UserGmail
{
    U32                auth_id;
    int                gmail_id;
    char            *name;
    Email            **gmail;
    SentMail        **sent_mail;  // For CSR
    U8                dirty : 1;
} UserGmail;

enum {kChatId_Handle    = 0,
      kChatId_DbId        = 1,
      kChatId_AuthId    = 2,
      NUM_CHAT_ID_TYPES};

#endif
