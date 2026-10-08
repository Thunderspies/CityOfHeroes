#ifndef _UIEMAIL_H
#define _UIEMAIL_H

typedef enum EmailType
{
    // Value 0 is reserved for retired local mail.
    kEmail_Global = 1,
    kEmail_Certification,
    kEmail_Count,
}EmailType;

typedef enum MailViewMode
{
    MVM_MAIL,
    MVM_CERTIFICATION,
    MVM_VOUCHER,
    MVM_COUNT,
}MailViewMode;

void emailHeaderListPrepareUpdate();
void emailResetHeaders(int quit_to_login);
int  emailWindow();
int  emailComposeWindow();
void emailSetNewMessageStatus(int status,char *msg);
int hasEmail(int type);
void emailAddHeaderGlobal(const char * sender, int sender_id, const char * subj, const char * msg, const char * attachment, int sent, U64 id, int cert, int claims, int subType );
void emailClearAttachments( U64 id );
int globalEmailIsFull();
char* emailBuildDateString(char* datestr, U32 seconds);

void updateCertifications(int fromMap);
void email_setView(char *view);
int email_getView();
void certificationClaimAllTick();

#define EMAIL_FROM_COLUMN        "EmailFrom"
#define EMAIL_SUBJECT_COLUMN    "EmailSubject"
#define EMAIL_DATE_COLUMN        "EmailDate"
#define EMAIL_EXPIRES_COLUMN    "EmailExpiration"
// End mkproto
#endif
