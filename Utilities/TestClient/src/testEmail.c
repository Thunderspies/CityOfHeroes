#include "testEmail.h"
#include "testClientInclude.h"
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/utils.h>
#include "clientcomm/clientcomm.h"
#include <utilitieslib/components/StashTable.h>
#include "gameData/randomCharCreate.h"
#include "gameData/randomName.h"
#include "entity/entity.h"

#define commAddInput(s) printf("commAddInput(\"%-.70s%s\");\n", s, (strlen(s)>70)?"...":""); commAddInput(s);

typedef enum EmailType
{
    kEmail_Local,
    kEmail_Global,
    kEmail_Certification,
    kEmail_Count,
}EmailType;

typedef struct
{
    char    *body;
    char    *recipients;
} EmailMessage;

typedef struct
{
    char    *sender;
    char    *subject;
    int        sent;
    U64        message_id;
    int        auth_id;
} EmailHeader;

static EmailHeader    *email_headers;
static int            email_header_count,email_header_max;


/*
    { 0, "emailheaders",    SCMD_EMAILHEADERS, {{0}}, 0,
                            "request email headers" },
    { 0, "emailread",        SCMD_EMAILREAD, {{TYPE_INT, &tmp_int}}, CMDF_HIDEVARS,
                            "Request message <message num>" },
    { 0, "emaildelete",        SCMD_EMAILDELETE, {{TYPE_INT, &tmp_int}}, CMDF_HIDEVARS,
                            "Delete message <message num>" },
    { 0, "emailsend",        SCMD_EMAILSEND, {{TYPE_STR, &tmp_str},{TYPE_STR, &tmp_str2},{TYPE_SENTENCE, &tmp_str3}}, 0,
                            "send message <player names> <subject> <body>" },
*/

static EmailMessage *emailPrepareCacheMessage(U64* message_id);


void printEmail(EmailMessage *em)
{
    printf("Email To: %s\nMessage:\n%s\n", em->recipients, em->body);
}


static int dirty=true;
static int waiting=false;
bool checkHeaderRequest()
{
    static int countdown = 1000;
    if (--countdown <= 0) {
        dirty = true;
        countdown = 1000;
    }
    if (countdown == 950) {
        // If it's been a while, perhaps we have no emails...
        dirty = false;
        waiting = false;
    }

    if (dirty && !waiting) {
        commAddInput("emailheaders");
        waiting = true;
        countdown = 1000;
    }
    if (dirty)
        return false;
    return true;
}

void testEmailRead()
{
    static int countdown = 20;

    if (!checkHeaderRequest())
        return;

    if (--countdown > 0) {
        return;
    }
    countdown = 20;

    if (email_header_count) {
        emailPrepareCacheMessage(&email_headers[rand() % email_header_count].message_id);
    }

}

void testEmailDelete()
{
    static int countdown = 30;

    if (!checkHeaderRequest())
        return;

    if (--countdown > 0) {
        return;
    }
    countdown = 30;

    if (email_header_count) {
        char buf[256];
        sprintf(buf, "emaildelete %d", email_headers[rand() % email_header_count].message_id);
        commAddInput(buf);
        dirty = true;
    }
}

char *validChars="abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ\"';:,<.>/?\\|}{[]+=_-`~!@#%^&*() \n\t";

char *randomMessage(int size)
{
    static int index=0;
    static char rets[4][60000];
    char *ret = rets[index++%ARRAY_SIZE(rets)];
    int i;

    for (i=0; i<size; i++) {
        ret[i] = validChars[rand()%strlen(validChars)];
    }
    ret[i]=0;
    return ret;
}

char *randomPlayer()
{
    int i;
    int count=0;
    // check all nearbye players
    for (i=1; i<entities_max; i++) {
        if (!(entity_state[i] & ENTITY_IN_USE ) )
            continue;
        if ( ENTTYPE_BY_ID(i) != ENTTYPE_PLAYER )
            continue;
        count++;
    }
    count = rand()%count;
    for (i=1; i<entities_max; i++) {
        if (!(entity_state[i] & ENTITY_IN_USE ) )
            continue;
        if ( ENTTYPE_BY_ID(i) != ENTTYPE_PLAYER )
            continue;
        if (count==0)
            return entities[i]->name;
        count--;
    }
    assert(false);
    return NULL;
}

void crash1() // causes mapserver crash - fixed!
{
    char buf[60000];
    sprintf(buf, "t TEST, %s", escapeString(randomMessage(2000)));
    commAddInput(buf);
}
void crash2() // causes dbserver errors - fixed!
{
    char buf[60000];
    sprintf(buf, "emailsend TEST \"%s\" dummy", escapeString(randomMessage(64)));
    commAddInput(buf);
}
void crash3() // causes mapserver crash - fixed!
{
    char buf[60000];
    sprintf(buf, "emailsend TEST \"blarg\" %s", escapeString(randomMessage(50000)));
    commAddInput(buf);
}
void crash4() // causes dbserver errors - fixed!
{
    char buf[60000];
    sprintf(buf, "emailsend TEST \"blarg\" %s", escapeString(randomMessage(4000)));
    commAddInput(buf);
}

void testEmailSpam()
{
    char buf[60000];
    int i;
    for (i=0; i<50; i++) {
        sprintf(buf, "emailsend TEST \"blarg\" %s", escapeString(randomMessage(100)));
        commAddInput(buf);
    }
}

void testEmailSend(int force)
{
    char buf[60000];
    char *recip;

    if (rand()%2) {
        // Real players
        recip = randomPlayer();
    } else {
        // Make names
        recip = randomName();
    }
    sprintf(buf, "emailsend \"\\q%s\\q\" \"%s\" ", recip, escapeString(randomMessage(rand()%24)));
    strcat(buf, escapeString(randomMessage(rand()%2000)));
    commAddInput(buf);
}

void checkEmail(int force)
{
    if (!ask_user && !force) {
        if (!(g_testMode & TEST_EMAIL))
            return;
        testEmailRead();
        testEmailDelete();
        testEmailSend(0);
    } else {
        int i;
        if (!checkHeaderRequest())
            return;
        if (!force)
            return;
        // User-invoked
        if (email_header_count==0) {
            printf("You have no email.\n");
        } else {
            printf("Emails:\n");
            for (i=0; i<email_header_count; i++) {
                printf("\t%d) %s, \"%s\"\n", i+1, email_headers[i].sender, email_headers[i].subject);
            }
        }
        printf("Email Options:\n");
        printf("  1) Cancel\n");
        printf("  2) Send\n");
        printf("  3) Send Random\n");
        printf("  4) Spam\n");
        if (email_header_count) {
            printf("  5) Read\n");
            printf("  6) Delete\n");
        }
        i = promptRanged("Choose an option", email_header_count?6:4);
        if (i==0)
            return;
        if (i==1) {
            char rec[256];
            char msg[2048]={0};
            char sub[1024];
            char temp[2048]={0};
            printf("To:");
            gets_s(rec, sizeof(rec));
            printf("Subject:");
            gets_s(sub, sizeof(sub));
            do {
                strcat(msg, temp);
                printf("Msg [Blank line to end]:");
                gets_s(temp, sizeof(temp));
            } while (temp[0]);
            sprintf(temp, "emailsend \"\\q%s\\q\" \"%s\" %s", rec, sub, msg);
            commAddInput(temp);
        } else if (i==2) {
            testEmailSend(1);
        } else if (i==3) {
            testEmailSpam();
        } else if (i==4) {
            EmailMessage *em;
            i = promptRanged("Read which message", email_header_count);
            em = emailPrepareCacheMessage(&email_headers[i].message_id);
            if (em!=NULL) {
                printEmail(em);
            }
        } else if (i==5) {
            char buf[1024];
            i = promptRanged("Delete which message", email_header_count);
            sprintf(buf, "emaildelete %d", email_headers[i].message_id);
            commAddInput(buf);
            dirty = true;
        }
    }
}


static StashTable message_hashes;


void emailHeaderListPrepareUpdate()
{
    printf("");
}

void emailResetHeaders()
{
    int        i;

    for(i=0;i<email_header_count;i++)
    {
        free(email_headers[i].sender);
        free(email_headers[i].subject);
    }
    email_header_count =0;

    dirty = false;
    waiting = false;
}

void emailAddHeader(U64 message_id,int auth_id,char *sender,char *subject,int sent, int redacted, int influence, char * attachment)
{
    EmailHeader    *header;

    header = dynArrayAdd(&email_headers,sizeof(email_headers[0]),&email_header_count,&email_header_max,1);
    header->message_id    = message_id;
    header->sender        = sender;
    header->sent        = sent;
    header->subject        = subject;
    header->auth_id        = auth_id;

    dirty = false;
    waiting = false;
}

void emailCacheMessage(int message_id, EmailType type,char *recip_buf,char *msg)
{
    EmailMessage *email = NULL;

    stashIntFindPointer(message_hashes, message_id, &email);

    assert(email);
    email->body = strdup(msg);
    email->recipients = strdup(recip_buf);

    printEmail(email);
}

static EmailMessage *emailPrepareCacheMessage(U64* message_id)
{
    EmailMessage    *email;
    char            buf[100];

    if (!message_hashes)
    {
        message_hashes = stashTableCreateFixedSize(4, sizeof(U64));
    }
    if (stashFindPointer(message_hashes, message_id, &email))
        return email;
    email = calloc(sizeof(*email),1);
    stashAddPointer(message_hashes, message_id, email, false);
    sprintf(buf,"emailread %d",message_id);
    commAddInput(buf);
    return NULL;
}

static void emailFreeHeader(EmailHeader *header)
{
    free(header->sender);
    free(header->subject);
    memset(header,0,sizeof(*header));
}

void emailSetNewMessageStatus(int status,char *msg) {
    testClientRecordEmailStatus(status);
    if (!status) {
        printf("Email not sent, because these recipients do not exist:\n%s\n",msg);
    }
}
