#include <utilitieslib/stdtypes.h>
#include "gameComm/wdwbase.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/player.h"
#include "entity/character_level.h"
#include "entity/character_eval.h"
#include "UI/uiWindows.h"
#include "UI/uiEmail.h"
#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiToolTip.h"
#include "UI/uiScrollBar.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiEditText.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "entity/entVarUpdate.h"
#include "UI/uiInput.h"
#include "UI/uiChat.h"
#include <utilitieslib/utils/timing.h>
#include "cmdparse/cmdgame.h"
#include "graphics/ttFont.h"
#include "assert.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/utils.h>
#include "UI/uiDialog.h"
#include "uiListView.h"
#include "UI/uiClipper.h"
#include <utilitieslib/components/earray.h>
#include "UI/uiFocus.h"
#include <utilitieslib/components/EString.h>
#include "UIEdit.h"
#include <utilitieslib/utils/SimpleParser.h>
#include "filter/profanity.h"
#include "uiEmote.h"
#include "graphics/ttFontUtil.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiOptions.h"
#include "auth/authUserData.h"
#include "formatter/smf_main.h"
#include "formatter/smf_util.h"
#include "gameComm/trayCommon.h"
#include "UI/uiEnhancement.h"
#include "bases/DetailRecipe.h"
#include "entity/character_base.h"
#include "UI/uiCursor.h"
#include "UI/uiInspiration.h"
#include "entity/powers.h"
#include "UI/uiRecipeInventory.h"
#include "UI/uiTray.h"
#include "UI/uiAuction.h"
#include "auction/AuctionData.h"
#include "storyarc/playerCreatedStoryarcValidate.h"
#include <utilitieslib/components/EString.h>
#include "UI/uiNet.h"
#include "account/AccountCatalog.h"


static StashTable message_hashes[kEmail_Count];

static int s_mailViewMode = 0;
static int s_claimAllMode = 0;

// Email composition field enums
typedef enum
{
    ECF_RECIPIENTS,
    ECF_SUBJECT,
    ECF_INFLUENCE,
    ECF_BODY,
    ECF_MAX
} EmailCompositionField;

static TextAttribs s_tEmail =
{
    /* piBold        */  (int *)(intptr_t)0,
    /* piItalic      */  (int *)(intptr_t)1,
    /* piColor       */  (int *)(intptr_t)0xffffffff,
    /* piColor2      */  (int *)(intptr_t)0,
    /* piColorHover  */  (int *)(intptr_t)0xffffffff,
    /* piColorSelect */  (int *)(intptr_t)0,
    /* piColorSelectBG*/ (int *)(intptr_t)0x333333ff,
    /* piScale       */  (int *)(intptr_t)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)(intptr_t)&smfSmall,
    /* piFont        */  (int *)(intptr_t)0,
    /* piAnchor      */  (int *)(intptr_t)0,
    /* piLink        */  (int *)(intptr_t)0x80e080ff,
    /* piLinkBG      */  (int *)(intptr_t)0,
    /* piLinkHover   */  (int *)(intptr_t)0x66ff66ff,
    /* piLinkHoverBG */  (int *)(intptr_t)0,
    /* piLinkSelect  */  (int *)(intptr_t)0,
    /* piLinkSelectBG*/  (int *)(intptr_t)0x666666ff,
    /* piOutline     */  (int *)(intptr_t)0,
    /* piShadow      */  (int *)(intptr_t)0,
};

static TextAttribs s_tEmailText =
{
    /* piBold        */  (int *)(intptr_t)0,
    /* piItalic      */  (int *)(intptr_t)1,
    /* piColor       */  (int *)(intptr_t)0xffffffff,
    /* piColor2      */  (int *)(intptr_t)0,
    /* piColorHover  */  (int *)(intptr_t)0xffffffff,
    /* piColorSelect */  (int *)(intptr_t)0,
    /* piColorSelectBG*/ (int *)(intptr_t)0x333333ff,
    /* piScale       */  (int *)(intptr_t)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)(intptr_t)&smfSmall,
    /* piFont        */  (int *)(intptr_t)0,
    /* piAnchor      */  (int *)(intptr_t)0,
    /* piLink        */  (int *)(intptr_t)0x80e080ff,
    /* piLinkBG      */  (int *)(intptr_t)0,
    /* piLinkHover   */  (int *)(intptr_t)0x66ff66ff,
    /* piLinkHoverBG */  (int *)(intptr_t)0,
    /* piLinkSelect  */  (int *)(intptr_t)0,
    /* piLinkSelectBG*/  (int *)(intptr_t)0x666666ff,
    /* piOutline     */  (int *)(intptr_t)0,
    /* piShadow      */  (int *)(intptr_t)0,
};


#define MAX_RECIPIENTS 200
#define MAX_SUBJECT 80
#define MAX_BODY 3900            // The server allows about 3900 bytes to be stored. Leave some room for escaped characters.
#define MAX_EMAIL_LENGTH 960    // However, Commands are only allocated 1000 bytes. Leave some room for escaped characters and error returns.
#define BODY_OVERHEAD 300

static int g_email_window_open;

static TrayObj g_attachment;

typedef struct
{
    char    *body;
    char    *recipients;
    bool    reparse;
} EmailMessage;

EmailMessage ** ppCertificationMails;


typedef struct
{
    char    *sender;
    char    *subject;
    char    *attachment;
    int        sent;
    U64        message_id;
    int        auth_id;
    int        refundable;
    int        influence;
    int        claim_availible;

    EmailType type;

} EmailHeader;

static EmailHeader    **g_ppHeader[MVM_COUNT];
static char *s_attachmentPtr;

static int listRebuildRequired = 0;
static U64 oldSelectionUID = 0;
static UIListView* emailListView;
static UIListView* playerEmailListView;
static UIListView* certVoucherEmailListView;
static ScrollBar emailListViewScrollbar = {WDW_EMAIL,0};

static SMFBlock *edits[ECF_MAX];
static SMFBlock *textBox;

static int s_prevCertCount = 0;
static int s_claim_delay;
static bool s_force_claim_delay;

static void attachment_claim(void *data)
{
    char * cmd=0;
    estrPrintf(&cmd, "account_certification_claim %s", s_attachmentPtr);
    cmdParse(cmd);
    estrDestroy(&cmd);    
    s_claim_delay = timerSecondsSince2000();
}

static void attachment_claim_check_confirm(void *data)
{
    const DetailRecipe *recipe = detailrecipedict_RecipeFromName(s_attachmentPtr);

    s_force_claim_delay = ((recipe->flags & RECIPE_FORCE_CLAIM_DELAY) != 0);

    if (recipe->pchDisplayClaimConfirmation)
    {
        dialog( DIALOG_YES_NO, -1, -1, -1, -1, recipe->pchDisplayClaimConfirmation, NULL, attachment_claim, NULL, NULL,0,0,0,0,0,0,0 );
        return;
    }

    attachment_claim(0);
}

void certificationClaimAllTick()
{
    if (s_claimAllMode && (timerSecondsSince2000()-s_claim_delay > 5))
    {
        Entity *e = playerPtr();
        int i;
        for (i = 0; i < eaSize(&g_ppHeader[MVM_CERTIFICATION]); ++i)
        {
            EmailHeader *header = g_ppHeader[MVM_CERTIFICATION][i];
            const DetailRecipe * pRecipe = detailrecipedict_RecipeFromName(header->attachment);
            if (pRecipe->ppchReceiveRequires && !chareval_Eval(e->pchar, pRecipe->ppchReceiveRequires, pRecipe->pchSourceFile))
            {
                //    skip things that can't be received by the client
                continue;
            }
            else
            {
                static EmailHeader *lastHeader = NULL;
                static int failedAttempts = 0;
                s_attachmentPtr = header->attachment;
                attachment_claim_check_confirm(0);
                s_claim_delay = timerSecondsSince2000();
                if (lastHeader == header)
                {
                    failedAttempts++;
                    if (failedAttempts > 3)
                    {
                        addSystemChatMsg("CertClaimAllError", INFO_USER_ERROR, 0);
                        failedAttempts = 0;
                        s_claimAllMode = 0;
                        lastHeader = NULL;
                    }
                }
                else
                {
                    failedAttempts = 0;
                }
                lastHeader = header;
                break;        //    break because the account server won't let us claim things too quickly anyways..
            }
        }
        if (i >= eaSize(&g_ppHeader[MVM_CERTIFICATION]))
        {
            // Keep claim all mode active until the dependent items have been claimed.
            // LTLBSAMU won't show up in g_ppHeader until LTLBBWAN and LTLBGSAX are claimed.
            Entity * e = playerPtr();
            AccountInventorySet* invSet = ent_GetProductInventory(e);
            AccountInventory* inventory;
            if (!invSet ||
                !(inventory = AccountInventorySet_Find(invSet, SKU("LTLBSAMU"))) ||
                inventory->granted_total <= inventory->claimed_total)
            {
                s_claimAllMode = 0;
            }
        }
    }
}

void emailHeaderListPrepareUpdate()
{
    UIListView* list = emailListView;
    if(list && list->selectedItem)
        oldSelectionUID = ((EmailHeader*)list->selectedItem)->message_id;
    listRebuildRequired = 1;
}

static void emailFreeHeader(EmailHeader *header)
{
    free(header->sender);
    free(header->subject);
    free(header->attachment);
    free(header);

}
void emailHeaderDelete( U64 id, EmailType type, int subType )
{
    int i;

    for( i = eaSize(&g_ppHeader[subType])-1; i>=0; i-- )
    {
        if( g_ppHeader[subType][i]->type == type && g_ppHeader[subType][i]->message_id == id )
        {
            EmailHeader * pDel = (EmailHeader*)eaRemove(&g_ppHeader[subType],i);
            if (message_hashes[type])
                stashRemovePointer(message_hashes[type],&pDel->message_id, NULL);
            emailFreeHeader(pDel);
            return;
        }
    }
}

void emailMessageFree(void *ptr)
{
    EmailMessage * email = (EmailMessage*)ptr;
    if( email )
    {
        SAFE_FREE(email->body);
        SAFE_FREE(email->recipients);
    }
    SAFE_FREE(email);
}

void emailResetHeaders(int quit_to_login)
{
    int i;

    for (i = 0; i < MVM_COUNT; ++i)
    {
          while( eaSize(&g_ppHeader[i]) )
            emailHeaderDelete(g_ppHeader[i][0]->message_id, g_ppHeader[i][0]->type, i );
    }

    for( i = 0; i < kEmail_Count; i++ )
    {
        if ( message_hashes[i] )
        {
            stashTableClear(message_hashes[i]); 
            stashTableClearEx(message_hashes[i], 0, emailMessageFree);
        }
        message_hashes[i] = 0;
    }
    
    uiLVClear(playerEmailListView);
    uiLVClear(certVoucherEmailListView);
    if (quit_to_login)
    {
        g_email_window_open = 0;
        s_claimAllMode = 0;
        s_prevCertCount = 0;
    }
}

static EmailMessage *emailPrepareCacheMessage(U64* message_id, EmailType type)
{
    EmailMessage    *email;
    char            buf[100];

    if (!message_hashes[type])
        message_hashes[type] = stashTableCreateFixedSize(4, sizeof(U64));

    if (stashFindPointer(message_hashes[type], message_id, (void**)&email))
        return email;

    email = (EmailMessage*)calloc(sizeof(*email),1);
    stashAddPointer(message_hashes[type], message_id, email, false);

    if( type == kEmail_Local )
    {
        sprintf(buf,"emailread %d",*message_id);
        cmdParse(buf);
    }
    return email;
}

void emailClearAttachments( U64 id )
{
    int i;
    for(i = 0; i < eaSize(&g_ppHeader[0]); i++)
    {
        EmailHeader * header = g_ppHeader[0][i];
        if( header->message_id == id &&  header->type == kEmail_Global )
        {
            header->influence = 0;
            SAFE_FREE(header->attachment);
        }
    }
}

EmailHeader * emailHeaderGet( U64 id, EmailType type, int subType )
{
    int i;
    EmailHeader * pNew;

    for( i =0; i < eaSize(&g_ppHeader[subType]); i++ )
    {
        if( g_ppHeader[subType][i]->message_id == id && type == g_ppHeader[subType][i]->type )
            return g_ppHeader[subType][i];
    }

    pNew = (EmailHeader*)calloc(1, sizeof(EmailHeader));
    eaPush(&g_ppHeader[subType], pNew);

    return pNew;
}


void emailAddHeaderGlobal(const char * sender, int sender_id, const char * subj, const char * msg, const char * attachment, int sent, U64 id, int cert_mail, int claims, int subType )
{
    EmailHeader    *header;
    EmailMessage *email;
    char * str;
    EmailType type = cert_mail>0?kEmail_Certification:kEmail_Global;

    header = emailHeaderGet(id,type,subType);

    header->message_id    = id; 
    header->auth_id     = sender_id;
    header->sent        = sent;
    SAFE_FREE(header->sender);
    header->sender        = (char*)malloc( strlen(sender) + 3 );
    header->type        = type;
    header->claim_availible = claims;
    if (cert_mail > 1)
        header->refundable = false;
    else
        header->refundable = true;

    if( sender_id )
        sprintf( header->sender, "@%s", sender );
    else
        sprintf( header->sender, textStd(sender) ); // translate system mail senders

    SAFE_FREE(header->subject);
    header->subject = strdup(smf_DecodeAllCharactersGet((char*)unescapeString(subj)));

    if( cert_mail )
    {
        SAFE_FREE(header->attachment);
        header->attachment = strdup(attachment);
    }
    else if(attachment && *attachment ) // Format is  "PlayerType Influence AttachmentIdentifier"
    {
        // skip past unused PlayerType
        str = strchr(attachment, ' ');
        if( str )
        {
            str++;
            if( str && *str )
            {
                header->influence = atoi(str);

                str = strchr(str, ' ');
                if( str )
                {
                    str++;
                    if( *str )
                    {
                        SAFE_FREE(header->attachment);
                        header->attachment = strdup(str);
                    }
                }
            }
        }
    }

    if( !optionGet(kUO_AllowProfanity)  )
        ReplaceAnyWordProfane(header->subject);

    email = emailPrepareCacheMessage(&header->message_id, header->type);
    
    SAFE_FREE(email->body);
    email->body = strdup(msg); 

    SAFE_FREE(email->recipients);
     email->recipients = strdup(unescapeString(header->sender));

    if( cert_mail )
    {
        email->reparse = 1;
    }
    emailHeaderListPrepareUpdate();
}
static DialogCheckbox newCertCB[] = { { "HideNewCertPrompt", 0, kUO_NewCertificationPrompt } };

static bool haveChoiceDependenciesBeenMet(AccountInventorySet *invSet)
{
    AccountInventory *quadChoiceInventory = AccountInventorySet_Find(invSet, SKU("LTLBSAMU"));
    if (quadChoiceInventory != NULL && quadChoiceInventory->granted_total > quadChoiceInventory->claimed_total)
    {
        AccountInventory *meleeInventory;
        AccountInventory *rangedInventory;
        meleeInventory = AccountInventorySet_Find(invSet, SKU("LTLBBWAN"));
        if (meleeInventory != NULL && meleeInventory->granted_total > meleeInventory->claimed_total
            && !getRewardToken(playerPtr(), "Power12"))
        {
            return false;
        }

        rangedInventory = AccountInventorySet_Find(invSet, SKU("LTLBGSAX"));
        if (rangedInventory != NULL && rangedInventory->granted_total > rangedInventory->claimed_total
            && !getRewardToken(playerPtr(), "Power33"))
        {
            return false;
        }
    }

    return true;
}

// Copying the list but removing LTLBSAMU if LTLBBWAN or LTLBGSAX exist and have not been claimed yet (see COH-22821).
static int processIndependentItems(AccountInventorySet *invSet, AccountInventory ***hTmpInvArr)
{
    const SkuId quadChoiceSkuId = SKU("LTLBSAMU");
    int currentCount = 0;
    int dependenciesMet = haveChoiceDependenciesBeenMet(invSet);
    int i;
    for (i = 0; i < eaSize(&invSet->invArr); ++i)
    {
        if (dependenciesMet || !skuIdEquals(invSet->invArr[i]->sku_id, quadChoiceSkuId))
        {
            eaPush(hTmpInvArr, invSet->invArr[i]);
        }
        else
        {
            currentCount += invSet->invArr[i]->granted_total - invSet->invArr[i]->claimed_total;
        }
    }

    return currentCount;
}

void updateCertifications(int fromMap)
{
    AccountInventorySet* invSet;
    AccountInventory** tmpInvArr = NULL;
    Entity * e = playerPtr();
    int i;
    int currentCount;
    if(!e->pl->account_inv_loaded)
    {
        s_prevCertCount = 0;
        return;
    }

    invSet = ent_GetProductInventory( e );
    currentCount = processIndependentItems(invSet, &tmpInvArr);
    for (i = 0; i < eaSize(&tmpInvArr); i++)
    {
        AccountInventory* pItem = tmpInvArr[i];
        int availible = pItem->granted_total - pItem->claimed_total;
        int addSuccess = 1;

        if (pItem->invType != kAccountInventoryType_Certification && pItem->invType != kAccountInventoryType_Voucher)
            continue;

        currentCount += availible;
        if( availible > 0 )
        {
            char *estr=0;
            const char * name = 0;
            int refundable = 1;

            const DetailRecipe * pRecipe = detailrecipedict_RecipeFromName(pItem->recipe);

            if( pRecipe )
            {
                if (optionGet(kUO_HideUnclaimableCerts))
                {
                    if (e->pchar && pRecipe->ppchReceiveRequires && !chareval_Eval(e->pchar, pRecipe->ppchReceiveRequires, pRecipe->pchSourceFile))
                    {
                        addSuccess = 0;
                    }
                }

                if (e->pchar && pRecipe->ppchNeverReceiveRequires && !chareval_Eval(e->pchar, pRecipe->ppchNeverReceiveRequires, pRecipe->pchSourceFile))
                {
                    addSuccess = 0;
                }

                if (addSuccess)
                {
                    name = pRecipe->ui.pchDisplayName;
                    estrConcatf( &estr, "<scale 1.2>%s<br>", textStd(name) );
                    if (pRecipe->flags & RECIPE_NO_REFUND)
                        refundable = 2;
                }
            }
            else
            {
                name = pItem->recipe;
                estrConcatf( &estr, "<color red>Unknown Recipe: %s<br></color>", pItem->recipe );
            }

            if (addSuccess)
            {
                if( pItem->granted_total - pItem->claimed_total > 0 )
                {
                    estrConcatCharString(&estr, textStd("AvailibleString", pItem->granted_total - pItem->claimed_total));

                }
                estrConcatf( &estr, "<br></scale>" );

                if( e->access_level )
                {
                    estrConcatCharString(&estr, "<br><color yellow>Debug Only:<br>");
                    estrConcatf( &estr, "Count: %i<br>", pItem->granted_total );
                    estrConcatf( &estr, "Claimed: %i<br>", pItem->claimed_total );
                }

                emailAddHeaderGlobal(textStd("SystemMail"), 0, textStd(name), estr, pItem->recipe, 0, pItem->sku_id.u64, refundable, pItem->granted_total - pItem->claimed_total, (pItem->invType == kAccountInventoryType_Voucher) ? MVM_VOUCHER : MVM_CERTIFICATION  );
            }
            estrDestroy(&estr);
        } 
        else 
        {
            addSuccess = 0;
        }
        if (!addSuccess)
        {
            emailHeaderDelete(pItem->sku_id.u64,kEmail_Certification, (pItem->invType == kAccountInventoryType_Voucher) ? MVM_VOUCHER : MVM_CERTIFICATION);
        }
    }
    eaDestroy(&tmpInvArr);
    listRebuildRequired = true;
    if (currentCount > s_prevCertCount)
    {
        if (!optionGet(kUO_NewCertificationPrompt) && !game_state.intro_zone)
        {
            dialog( DIALOG_OK, -1, -1, -1, -1, "NewCertificationDialog", NULL, NULL, NULL, NULL,0,sendOptions,newCertCB,1,0,0,0 );
        }

    }
    s_prevCertCount = currentCount;
    if (fromMap && !s_force_claim_delay)
        s_claim_delay = 0;
}


void emailAddHeader(U64 message_id, int auth_id, char *sender,char *subject,int sent, int refundable, int influence, char * attachment)
{
    EmailHeader    *header;

    header = emailHeaderGet(message_id, kEmail_Local, MVM_MAIL);
    header->message_id    = message_id;
    header->auth_id     = auth_id;
    header->sender        = sender;
    header->sent        = sent;
    header->refundable    = refundable;
    header->influence    = influence;
    header->attachment    = attachment;
    header->type = kEmail_Local;
    
    if( !optionGet(kUO_AllowProfanity)  )
        ReplaceAnyWordProfane(subject);
    header->subject        = subject;
}

void emailCacheMessage(U64 message_id, EmailType type, char *recip_buf,char *msg)
{
    EmailMessage *email;
    EmailHeader    *header = NULL;
    int i;
    for( i =0; i < eaSize(&g_ppHeader[MVM_MAIL]); i++ )
    {
        if( g_ppHeader[MVM_MAIL][i]->message_id == message_id && type == g_ppHeader[MVM_MAIL][i]->type )
        {
            header = g_ppHeader[MVM_MAIL][i];
            break;
        }
    }

    if (!header || !message_hashes[type] || !stashFindPointer(message_hashes[type], &header->message_id, (void**)&email))
        return;
    if (email->body)
        free(email->body);
    email->body = strdup(msg);
    if (email->recipients)
        free(email->recipients);
    email->recipients = strdup(recip_buf);
}


static int drawSelectLine(F32 x,F32  y,F32 z,F32 width,char *name,char *subject,char *date, int selectable)
{
    #define NAMEPIX 100
    #define DATEPIX 50
    CBox    box;

    cprntEx(x,y,z+2,1,1, NO_MSPRINT, "%s",name);
    cprntEx(x+NAMEPIX,y,z+2,1,1, NO_MSPRINT, "%s",subject);
    prnt(x+width-(NAMEPIX + DATEPIX),y,z+2,1,1,date);
    BuildCBox(&box,x,y-FONT_HEIGHT,width,FONT_HEIGHT);

     if( selectable && mouseCollision(&box) )
    {
        drawBox( &box, z, 0, 0xffffff44 );
        if (mouseClickHit( &box, MS_LEFT ))
            return 1;
    }
    return 0;
}



#define EMAIL_HEADER_PANE_HEIGHT    150
#define EMAIL_MESSAGE_PANE_HEIGHT    200
#define EMAIL_BUTTON_PANE_HEIGHT    40

//----------------------------------------------------------------------------------------------------
// Email header pane
//----------------------------------------------------------------------------------------------------

static int emailHeaderFromCompare(const void* e1Data, const void* e2Data)
{
    const EmailHeader** e1 = (const EmailHeader**)e1Data;
    const EmailHeader** e2 = (const EmailHeader**)e2Data;
    const EmailHeader* email1 = *e1;
    const EmailHeader* email2 = *e2;

    if ((email1->type == kEmail_Certification) !=
        (email2->type == kEmail_Certification))
    {
        return (email1->type == kEmail_Certification) ? -1 : 1;
    }
    return stricmp(email1->sender, email2->sender);
}

static int emailHeaderSubjectCompare(const void* e1Data, const void* e2Data)
{
    const EmailHeader** e1 = (const EmailHeader**)e1Data;
    const EmailHeader** e2 = (const EmailHeader**)e2Data;
    const EmailHeader* email1 = *e1;
    const EmailHeader* email2 = *e2;

    if ((email1->type == kEmail_Certification) !=
        (email2->type == kEmail_Certification))
    {
        return (email1->type == kEmail_Certification) ? -1 : 1;
    }
    return stricmp(email1->subject, email2->subject);
}

static int emailHeaderDateCompare(const void* e1Data, const void* e2Data)
{
    const EmailHeader** e1 = (const EmailHeader**)e1Data;
    const EmailHeader** e2 = (const EmailHeader**)e2Data;
    const EmailHeader* email1 = *e1;
    const EmailHeader* email2 = *e2;

    if ((email1->type == kEmail_Certification) !=
        (email2->type == kEmail_Certification))
    {
        return (email1->type == kEmail_Certification) ? -1 : 1;
    }
    return email1->sent - email2->sent;
}

static int emailHeaderExpiresCompare(const void* e1Data, const void* e2Data)
{
    const EmailHeader** e1 = (const EmailHeader**)e1Data;
    const EmailHeader** e2 = (const EmailHeader**)e2Data;
    const EmailHeader* email1 = *e1;
    const EmailHeader* email2 = *e2;
    int sent1 = (email1->sender[0] == '@') ? email1->sent : 0;
    int sent2 = (email2->sender[0] == '@') ? email2->sent : 0;

    if ((email1->type == kEmail_Certification) !=
        (email2->type == kEmail_Certification))
    {
        return (email1->type == kEmail_Certification) ? -1 : 1;
    }
    return sent1 - sent2;
}

char* emailBuildDateString(char* datestr, U32 seconds)
{
    __int64 x;
    SYSTEMTIME    t;
    FILETIME    local;
    char* postfix;

    if (!seconds)
    {
        datestr[0] = 0;
        return datestr;
    }

    x = seconds;
    x *= 10000000;
    x += y2kOffset();

    FileTimeToLocalFileTime((FILETIME*)&x,&local);
    FileTimeToSystemTime(&local, &t);

    if(t.wHour > 12)
    {
        t.wHour -= 12;
        postfix = "PM";
    }
    else
    {
        postfix = "AM";
    }


    sprintf(datestr,"%02d/%02d/%02d %d:%02d ",t.wMonth,t.wDay,t.wYear-2000,t.wHour,t.wMinute);
    strcat(datestr, postfix);

    return datestr;
}

static UIBox emailHeaderDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, void* itemData, int itemIndex)
{
    EmailHeader* item = (EmailHeader*)itemData;
    UIBox box = {0};
    PointFloatXYZ pen = rowOrigin;
    float sc = list->scale;
    UIColumnHeaderIterator columnIterator;

    box.origin.x = rowOrigin.x;
    box.origin.y = rowOrigin.y;

    uiCHIGetIterator(list->header, &columnIterator);

    rowOrigin.z+=1;


    // Determine the text color of the item.
    if(item == list->selectedItem)
    {
        font_color(CLR_SELECTED_ITEM, CLR_SELECTED_ITEM);
    }
    else if (item->type == kEmail_Certification)
    {
        font_color(0xFFD373FF, 0xFFD373FF);
    }
    else
    {
        font_color(CLR_ONLINE_ITEM, CLR_ONLINE_ITEM);
    }

    for(;uiCHIGetNextColumn(&columnIterator, list->scale);)
    {
        UIColumnHeader* column = columnIterator.column;
        UIBox clipBox;
        pen.x = rowOrigin.x + columnIterator.columnStartOffset*list->scale;

        clipBox.origin.x = pen.x;
        clipBox.origin.y = pen.y;
        clipBox.height = 20*sc;

        if( columnIterator.columnIndex >= eaSize(&columnIterator.header->columns )-1 )
            clipBox.width = 1000*sc;
        else
            clipBox.width = columnIterator.currentWidth*sc;

         clipperPushRestrict(&clipBox);
        if(stricmp(column->name, EMAIL_FROM_COLUMN) == 0)
        {
            prnt(clipBox.x, clipBox.y+18*sc, rowOrigin.z, sc, sc, "%s", item->sender);
        }
        else if(stricmp(column->name, EMAIL_SUBJECT_COLUMN) == 0)
        {
            if( item->influence || item->claim_availible || (item->type != kEmail_Certification && item->attachment && strlen(item->attachment)) )
            {
                 AtlasTex * paperclip = atlasLoadTexture("email_attach");
                display_sprite( paperclip, clipBox.x, clipBox.y, rowOrigin.z, 20*sc/paperclip->height, 20*sc/paperclip->height, CLR_WHITE );
                prnt(clipBox.x + 22*sc, clipBox.y+18*sc, rowOrigin.z, sc, sc, "%s", item->subject);
            }
            else
                prnt(clipBox.x, clipBox.y+18*sc, rowOrigin.z, sc, sc, "%s", item->subject);

        }
        else if(stricmp(column->name, EMAIL_DATE_COLUMN) == 0)
        {
            char date[100];
            emailBuildDateString(date,item->sent);
            prnt(clipBox.x, clipBox.y+18*sc, rowOrigin.z, sc, sc, date);
        }
        else if (stricmp(column->name, EMAIL_EXPIRES_COLUMN) == 0)
        {
            //    if global mail
            if (item->sender[0] == '@')
            {
                char date[100];
                int expirationDate = (item->sent + GMAIL_EXPIRATION_DURATION);
                int timeSecsRemaining = expirationDate - ((int)timerSecondsSince2000());
                int daysRemaining = MAX(0, (timeSecsRemaining / (SECONDS_PER_DAY)));
                sprintf(date, "%s", textStd("EmailExpiresString", daysRemaining));
                prnt(clipBox.x, clipBox.y+18*sc, rowOrigin.z, sc, sc, date);
            }
        }
        clipperPop();
    }
    return box;
}

static void emailInitHeaderListView(UIListView** listAddr, int isPlayer)
{
    if(!*listAddr)
    {
        UIListView* list;

        *listAddr = uiLVCreate();
        list = *listAddr;

        if (isPlayer)
        {
            uiLVHAddColumnEx(list->header, EMAIL_FROM_COLUMN, "EmailFromHeader", 10, 270, 1);
            uiLVBindCompareFunction(list, EMAIL_FROM_COLUMN, emailHeaderFromCompare);

            uiLVHAddColumnEx(list->header, EMAIL_SUBJECT_COLUMN, "EmailSubjectHeader", 10, 500, 1);
            uiLVBindCompareFunction(list, EMAIL_SUBJECT_COLUMN, emailHeaderSubjectCompare);

            uiLVHAddColumn(list->header, EMAIL_DATE_COLUMN, "EmailDateHeader", 110);
            uiLVBindCompareFunction(list, EMAIL_DATE_COLUMN, emailHeaderDateCompare);

            uiLVHAddColumn(list->header, EMAIL_EXPIRES_COLUMN, "EmailExpiresHeader", 110);
            uiLVBindCompareFunction(list, EMAIL_EXPIRES_COLUMN, emailHeaderExpiresCompare);
        }
        else
        {
            uiLVHAddColumn(list->header, EMAIL_SUBJECT_COLUMN, "EmailSubjectHeader", 500);
            uiLVBindCompareFunction(list, EMAIL_SUBJECT_COLUMN, emailHeaderSubjectCompare);
        }

        uiLVFinalizeSettings(list);

        list->displayItem = emailHeaderDisplayItem;

        uiLVEnableMouseOver(list, 1);
        uiLVEnableSelection(list, 1);
        list->header->selectedColumn = uiLVHFindColumnIndex(list->header, EMAIL_DATE_COLUMN);

        {
            Wdw* window = wdwGetWindow(WDW_EMAIL);

            // The window needs to be wide enough to fit the following.
            // - The list view header
            // - The window borders
            int minWindowWidth = (int)uiLVHGetMinDrawWidth(list->header) + 2*PIX3;

            // Limit the window to the min header size.
            window->min_wd = max(window->min_wd, minWindowWidth);
            window->min_wd = max(window->min_wd, 495);

            // Magic number to make sure all button captions are drawn at the correct size.
            //window->min_wd = max(window->min_wd, 520 + 2*PIX3);
            window->min_ht = max(window->min_ht, EMAIL_HEADER_PANE_HEIGHT + EMAIL_MESSAGE_PANE_HEIGHT + 2*PIX3);
        }
    }

}

static void emailHeaderRebuildListView(UIListView* list)
{
    // Clear the list.  This also kills the current selection.
    uiLVClear(list);

    if(list && eaSize(&g_ppHeader[s_mailViewMode]) )
    {
        int i;

        // Iterate over all supergroup memebers.
        for(i = 0; i < eaSize(&g_ppHeader[s_mailViewMode]); i++)
        {
            uiLVAddItem(list, g_ppHeader[s_mailViewMode][i]);

            // Reselect the previously selected item here.
            if(g_ppHeader[s_mailViewMode][i]->message_id == oldSelectionUID)
                uiLVSelectItem(list, i);
        }

        // Resort the list
        uiLVSortBySelectedColumn(list);

        listRebuildRequired = 0;
    }
}

int hasEmail(int type)
{
    return eaSize(&g_ppHeader[type]);
}

static void emailDrawHeaderPane(UIBox drawArea, float z, float scale)
{
    PointFloatXYZ pen;
    UIListView* list;

    font( &game_12 ); 
    emailInitHeaderListView(&playerEmailListView, 1);
    emailInitHeaderListView(&certVoucherEmailListView, 0);

    if(!playerEmailListView || !certVoucherEmailListView)
        return;

    if (!emailListView)
        emailListView = playerEmailListView;

    list = emailListView;
    pen.x = drawArea.x;
    pen.y = drawArea.y;
    pen.z = z; 

    if(listRebuildRequired)
        emailHeaderRebuildListView(emailListView);

    // Disable mouse over on window resize.
    {
        Wdw* window = wdwGetWindow(WDW_EMAIL);
        if(window->drag_mode)
            uiLVEnableMouseOver(list, 0);
        else
            uiLVEnableMouseOver(list, 1);
    }

    // How tall and wide is the list view supposed to be?
    uiLVSetDrawColor(list, window_GetColor(WDW_EMAIL));
    uiLVSetHeight(list, drawArea.height);
    uiLVHSetWidth(list->header, drawArea.width);

    clipperPushRestrict(&drawArea);
    uiLVSetScale(list, scale);
    uiLVDisplay(list, pen);
    clipperPop();

    // Handle list view input.
    uiLVHandleInput(list, pen);
}

static DialogCheckbox voucherCB[] = { { "HideVoucherPrompt", 0, kUO_VoucherPrompt } };
//----------------------------------------------------------------------------------------------------
// Email message pane
//----------------------------------------------------------------------------------------------------
static int check_inventory_space(Character *pchar, char *attachment)
{
    AuctionItem * pItem;
    if (!stashFindPointer(auctionData_GetDict()->stAuctionItems, attachment, &pItem) || !pItem)
    {
        return 0;
    }

    switch( pItem->type )
    {
        xcase kTrayItemType_None: // ok
        xcase kTrayItemType_SpecializationInventory: 
        {
            if (character_BoostInventoryFull(pchar))
            {
                return 0;
            }
        }
        xcase kTrayItemType_Inspiration:
        {
            if (character_InspirationInventoryFull(pchar))
            {
                return 0;
            }
        }
        xcase kTrayItemType_Recipe:
        case kTrayItemType_Salvage:
        {
            InventoryType invtype = InventoryType_FromTrayItemType(pItem->type);
            int invid = genericinv_GetInvIdFromItem(invtype,pItem->pItem);
            if (!character_CanAddInventory(pchar, invtype, invid, 1) )
            {
                return 0;
            }
        }
    }

    return 1;
}
static void emailDrawReadMessagePane(UIBox drawAreaIn, int clr, float z, float sc)
{
    UIListView* list = emailListView;
    UIBox drawArea = drawAreaIn;
    Entity * e = playerPtr();

    uiBoxAlter(&drawArea, UIBAT_SHRINK, (UIBoxAlterDirection)(UIBAD_LEFT | UIBAD_RIGHT), 5*sc);

    // If no email message is selected for display, select the first message.
    if(!list->selectedItem &&  eaSize(&g_ppHeader[s_mailViewMode]))
    {
        uiLVSelectItem(list, 0);
        list->newlySelectedItem = 1;
    }

    if( eaSize(&g_ppHeader[s_mailViewMode]) )
    {
        EmailMessage    *message;
        EmailHeader        *header = (EmailHeader*)list->selectedItem;
        PointFloatXYZ pen = {drawArea.x, drawArea.y, z};
        float scale = 1.0;
        int textHeight = ttGetFontHeight(font_grp, scale*sc, scale*sc);

        message = emailPrepareCacheMessage(&header->message_id, header->type);
        if (message && message->body)
        {
            AtlasTex* divider_mid = atlasLoadTexture("Headerbar_VertDivider_MID.tga");
            AtlasTex* divider_end = atlasLoadTexture("Headerbar_HorizDivider_R.tga");
            char* fromLabel = "EmailFromField";
            char* subjectLabel = "EmailSubjectField";
            int fieldLabelWidth, attachmentWidth;
            UIBox clipBox;
            static ScrollBar readMessageBar = {0, 0, 0, 0, -1, 0, 0};

            fieldLabelWidth = max(str_wd(font_grp, sc, sc, fromLabel), str_wd(font_grp, sc, sc, subjectLabel)) + 3*sc;

            clipBox = drawArea;
            

            // Reset the scrollbar when opening a message.
            if(list->newlySelectedItem || !textBox || message->reparse)
            {
                readMessageBar.offset = 0;
                list->newlySelectedItem = 0;

                if(!textBox) 
                    textBox = smfBlock_Create();

                if( !optionGet(kUO_AllowProfanity)  )
                    ReplaceAnyWordProfane(message->body);

                smf_SetRawText(textBox, message->body, 0);
            }

            if( header->claim_availible || header->influence || (header->attachment && header->type != kEmail_Certification) )
            {
                F32 ty = pen.y;        

                if( header->type == kEmail_Certification )
                {
                    const DetailRecipe * pRecipe = detailrecipedict_RecipeFromName(header->attachment);
                    int licLocked = false;
                    attachmentWidth = 100*sc;

                    if( pRecipe )
                    {
                        AtlasTex *pIcon = atlasLoadTexture(pRecipe->ui.pchIcon);
                        if( pIcon )
                        {
                            display_sprite(pIcon, pen.x + clipBox.width - 50*sc - pIcon->width/2*sc, ty+10*sc, z+1, sc, sc, CLR_WHITE );
                            ty += (pIcon->height+10)*sc;
                        }
                        licLocked = (pRecipe->flags & RECIPE_INVENTION_LIC_REQUIRED) != 0 && !AccountHasStoreProduct(&e->pl->account_inventory, SKU("coliinvn"));
                    }

                     if( header->attachment  )
                    {
                        float adjustWidth = (s_mailViewMode == MVM_CERTIFICATION) ? 30*sc : 80*sc;
                        if( D_MOUSEHIT == drawStdButton(pen.x + clipBox.width + 0.5f*(adjustWidth-80*sc)- attachmentWidth/2, ty + textHeight + 10*sc, z+1, adjustWidth, 20*sc, clr, "ClaimString", .8f, (int)(timerSecondsSince2000()-s_claim_delay) < 5 || licLocked) )
                        {
                            s_attachmentPtr = header->attachment;
                            if (s_mailViewMode == MVM_VOUCHER && !optionGet(kUO_VoucherPrompt))
                            {
                                dialog( DIALOG_YES_NO, -1, -1, -1, -1, "VoucherClaimDialog", NULL, attachment_claim_check_confirm, NULL, NULL,0,sendOptions,voucherCB,1,0,0,0 );
                            }
                            else
                            {
                                attachment_claim_check_confirm(0);
                            }
                        }
                        if (s_mailViewMode == MVM_CERTIFICATION)
                        {
                            if( D_MOUSEHIT == drawStdButton(pen.x + clipBox.width +0.5f*(adjustWidth-80*sc)+40*sc - attachmentWidth/2, ty + textHeight + 10*sc, z+1, 80*sc-(adjustWidth+5*sc), 20*sc, clr, "ClaimAllString", .8f, (int)(timerSecondsSince2000()-s_claim_delay) < 5 || licLocked || s_claimAllMode) )
                            {
                                s_claimAllMode = 1;
                            }

                        }
                    }
                }
                else
                {
                    attachmentWidth = 100*sc;
                     if( header->attachment )
                    {
                        AuctionItem * pItem;
                        if(stashFindPointer(g_AuctionItemDict.stAuctionItems, header->attachment, (void**)&pItem) )
                        {
                            displayAuctionItemIcon( pItem, pen.x + clipBox.width - 90*sc, ty + 10*sc, z+1, sc );
                            ty += 80*sc;
                        }
                    }

                       if( header->influence )
                    {
                        if (ent_canAddInfluence( playerPtr(), header->influence ))
                            font_color(CLR_WHITE, CLR_WHITE);
                        else
                            font_color(CLR_RED, CLR_RED);
                        cprnt(pen.x + clipBox.width - attachmentWidth/2, ty + textHeight+4*sc, z+1, sc, sc, textStd("InfColonValue", header->influence) );
                        ty += 12*sc;
                    }

                    if( D_MOUSEHIT == drawStdButton(pen.x + clipBox.width - attachmentWidth/2, ty + textHeight + 10*sc, z+1, 80*sc, 20*sc, clr, "ClaimString", .8f, 0) )
                    {
                        // check so see if they can claim the influence
                        if (header->influence > 0 && !ent_canAddInfluence( playerPtr(), header->influence ))
                        {
                            dialogStd(DIALOG_OK, "CannotClaimInf", NULL, NULL,0,0,1);
                        }
                        else if (header->attachment && *header->attachment && !check_inventory_space(e->pchar, header->attachment))
                        {
                            dialogStd(DIALOG_OK, "CannotClaimItem", NULL, NULL,0,0,1);
                        }
                        else
                        {
                            char * cmd=0;
                            estrPrintf(&cmd, "gmail_claim %i", (int)header->message_id);
                            cmdParse(cmd);
                            estrDestroy(&cmd);    
                        }
                    }
                }
                ty += 32*sc;

                drawFrame( PIX3, R10, drawArea.x + drawArea.width - attachmentWidth - 3*PIX3*sc, drawArea.y - PIX3*sc, z, attachmentWidth + 6*PIX3*sc, ty - pen.y, sc, clr, 0x00000088 );
                   clipBox.width -= attachmentWidth*sc;
                drawArea.width -= attachmentWidth*sc;
            }

            clipperPushRestrict(&clipBox);

            pen.y += textHeight;
            font_color(CLR_WHITE,CLR_WHITE);
            prnt(pen.x, pen.y, z, scale*sc, scale*sc,"EmailFromField");
            font_color(CLR_ONLINE_ITEM,CLR_ONLINE_ITEM);
            prnt(pen.x + fieldLabelWidth , pen.y, z, scale*sc, scale*sc, "%s", header->sender);

            pen.y += textHeight;
            font_color(CLR_WHITE,CLR_WHITE);
            prnt(pen.x, pen.y, z, scale*sc, scale*sc,"EmailSubjectField");
            font_color(CLR_ONLINE_ITEM,CLR_ONLINE_ITEM);
            if( !optionGet(kUO_AllowProfanity)  )
                ReplaceAnyWordProfane(header->subject);
            prnt(pen.x + fieldLabelWidth , pen.y, z, scale*sc, scale*sc, "%s", header->subject);

            pen.y += 5*sc;
            {
                float midLength;
                clipBox.y = pen.y;
                clipBox.height = 2*sc;
                midLength = clipBox.width - divider_end->width;
                clipperPushRestrict(&clipBox);
                if(midLength)
                    display_sprite(divider_mid, clipBox.x, clipBox.y, z, midLength/divider_mid->width, sc, CLR_WHITE);
                display_sprite(divider_end, clipBox.x + midLength, clipBox.y, z, sc, sc, CLR_WHITE);
                clipperPop();
            }


            font_color(CLR_ONLINE_ITEM,CLR_ONLINE_ITEM);
            pen.y += 10*sc;
            clipBox = drawArea;
            clipBox.y = pen.y;
            clipBox.height = drawArea.height - (pen.y - drawArea.y);

             clipperPushRestrict(&clipBox);

            s_tEmailText.piScale = (int *)((intptr_t)(1.0f*sc*SMF_FONT_SCALE));
            s_tEmailText.piColor = s_tEmailText.piColor2 = (int*)(CLR_ONLINE_ITEM);

            smf_SetScissorsBox(textBox, clipBox.x, clipBox.y, clipBox.width, clipBox.height );
            smf_SetFlags(textBox, SMFEditMode_ReadOnly, SMFLineBreakMode_None, SMFInputMode_None,
                MAX_BODY, SMFScrollMode_InternalScrolling, SMFOutputMode_StripAllTags, 
                SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0,
                "EmailTabNavigationSet", -1); 
            smf_Display(textBox, clipBox.x, clipBox.y, z,  clipBox.width, clipBox.height, message->reparse, message->reparse, &s_tEmailText, 0);
            message->reparse = 0;
            clipperPop();
            clipperPop();
        }
    }
}

//----------------------------------------------------------------------------------------------------
// Email buttons pane
//----------------------------------------------------------------------------------------------------
#define EBC_NEW            (1 << 0)
#define EBC_REPLY        (1 << 1)
#define EBC_REPLY_ALL    (1 << 2)
#define EBC_FORWARD        (1 << 3)
#define EBC_DELETE        (1 << 4)
#define EBC_SEND        (1 << 5)
#define EBC_CANCEL        (1 << 6)
#define EBC_SPAM        (1 << 7)
typedef U32 EmailButtonCommands;

static EmailButtonCommands emailReadWindwDrawCommandButtons(UIBox drawArea, float z, int lock, float sc)
{
    static ToolTip warning;
    char* caption;
    int buttonGroupGap = 20*sc;
    int buttonGap = 5*sc;
    int groupButtonCount = 6;
    UIBox buttonBox, groupBounds;
    int intendedButtonHeight = 26*sc;
    int flags = (lock ? BUTTON_LOCKED : 0);
    int hasSelectedHeader = (emailListView->selectedItem ? 1 : 0);
    EmailButtonCommands result = 0;
    EmailHeader*    header = (EmailHeader*)emailListView->selectedItem;
    Entity *e = playerPtr();

    groupBounds = drawArea;
    groupBounds.y += (EMAIL_BUTTON_PANE_HEIGHT*sc - intendedButtonHeight)/2;
    groupBounds.height = intendedButtonHeight;

    buttonBox = groupBounds;
    buttonBox.width = ((float)groupBounds.width - ((groupButtonCount-1) * buttonGap))/groupButtonCount;

     // Draw the "New" button
    caption = "NewString";
    if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_EMAIL), caption, sc, flags))
    {
        result |= EBC_NEW;
    }
    if (lock)
    {
        CBox box;
        uiBoxToCBox(&buttonBox, &box);
        if (clipperIntersects(&buttonBox) && mouseCollision(&box))
        {
            addToolTip( &warning );
            if( entIsTrial( playerPtr() ) )
            {
                forceDisplayToolTip( &warning, &box, textStd("CannotEmailTrialAccount"), MENU_GAME, WDW_EMAIL );
            }
            else if (e && e->pl && !AccountHasEmailAccess(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
            {
                forceDisplayToolTip( &warning, &box, textStd("CannotEmailNoPermission"), MENU_GAME, WDW_EMAIL );
            }
            else
            {
                forceDisplayToolTip( &warning, &box, textStd("EmailUnlockedAtLevel10"), MENU_GAME, WDW_EMAIL );
            }
        }
    } else { 
        removeToolTip( &warning );
    }



    if(!hasSelectedHeader || header->type == kEmail_Certification || !header->auth_id )
        flags |= BUTTON_LOCKED;

    // Draw the "Reply" button
    caption = "ReplyStringword";
    buttonBox.x += buttonBox.width + buttonGap;
    if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_EMAIL), caption, sc, flags))
    {
        result |= EBC_REPLY;
    }

    // Draw the "Reply all" button
    caption = "Replyall";
    buttonBox.x += buttonBox.width + buttonGap;
    if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_EMAIL), caption, sc, flags))
    {
        result |= EBC_REPLY_ALL;
    }

    // Draw the "Forward" button

    flags = flags &= ~BUTTON_LOCKED;
    if(lock || !hasSelectedHeader || header->type == kEmail_Certification )
        flags |= BUTTON_LOCKED;

    caption = "ForwardString";
    buttonBox.x += buttonBox.width + buttonGap;
    if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_EMAIL), caption, sc, flags))
    {
        result |= EBC_FORWARD;
    }

    if(hasSelectedHeader)
        flags &= ~BUTTON_LOCKED;

    // Draw the "Delete" button
    if( header && header->type == kEmail_Certification )
    {
        flags |= BUTTON_LOCKED;
        caption = "DeleteString";
    }
     else if( header && (header->influence || header->attachment && *header->attachment) )
    {
        Entity * e = playerPtr();
        if( !header->auth_id || header->auth_id == e->auth_id )
        {
            flags |= BUTTON_LOCKED;
        }
        else
            caption = "ReturnToSender";
    }
    else
        caption = "DeleteString";

    buttonBox.x += buttonBox.width + buttonGap;
    if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_EMAIL), caption, sc, flags))
    {
        result |= EBC_DELETE;
        emailListView->newlySelectedItem = 1;
    }

    if(hasSelectedHeader )
        flags &= ~BUTTON_LOCKED;

     if(!header || !header->auth_id || header->type == kEmail_Certification)
         flags |= BUTTON_LOCKED;
    // Draw the "Delete" button
    caption = "IgnoreSpammer";
    buttonBox.x += buttonBox.width + buttonGap;
    if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_EMAIL), caption, sc, flags))
    {
        result |= EBC_SPAM;
        emailListView->newlySelectedItem = 1;
    }

    return result;
}

static char* emailRemoveSubjectPrefix(char* subject)
{
    if (strnicmp(subject,"Re: ",4) == 0 || strnicmp(subject, "Fw: ", 4)==0)
        return subject+4;

    return subject;
}

static void emailHandleButtonCommands(EmailButtonCommands command)
{
    switch(command)
    {
    case EBC_NEW:
        {
            int i;
            for(i = 0; i < ECF_MAX; i++ )
                smf_SetRawText(edits[i], "", 0);
            window_setMode(WDW_EMAIL_COMPOSE, WINDOW_GROWING );
        }
        break;
    case EBC_REPLY:
        {
            EmailHeader*    header = (EmailHeader*)emailListView->selectedItem;
            EmailMessage*    message = emailPrepareCacheMessage(&header->message_id, header->type);
            char    *chop = strdup(message->body);
            
            smf_SetRawText(edits[ECF_RECIPIENTS], header->sender, 0);
            estrPrintf(&edits[ECF_SUBJECT]->rawString, "Re: %s", emailRemoveSubjectPrefix(header->subject));
            if (strlen(chop) > MAX_BODY/2)
                chop[MAX_BODY/2] = 0;
              estrPrintf(&edits[ECF_BODY]->rawString, "<br>----<br>%s<br>%s<br>----<br>", textStd("EmailForwardBlock",header->sender), chop);
            free(chop);
            edits[ECF_RECIPIENTS]->editedThisFrame = edits[ECF_BODY]->editedThisFrame = 1;

            window_setMode(WDW_EMAIL_COMPOSE, WINDOW_GROWING );
        }
        break;
    case EBC_REPLY_ALL:
        {
            EmailHeader*    header = (EmailHeader*)emailListView->selectedItem;
            EmailMessage*    message = emailPrepareCacheMessage(&header->message_id, header->type);
            char    *chop = strdup(message->body);

            smf_SetRawText(edits[ECF_RECIPIENTS], message->recipients, 1 );
            
            if (!strstri(message->recipients, header->sender) && (strlen(header->sender) + strlen(message->recipients) + 1 < MAX_RECIPIENTS))
                estrPrintf(&edits[ECF_RECIPIENTS]->rawString, "%s; %s", header->sender, message->recipients);

            estrPrintf(&edits[ECF_SUBJECT]->rawString, "Re: %s", emailRemoveSubjectPrefix(header->subject));
            if (strlen(chop) > MAX_BODY/2)
                chop[MAX_BODY/2] = 0;
            estrPrintf(&edits[ECF_BODY]->rawString, "<br>----<br>%s<br>%s<br>----<br>", textStd("EmailForwardBlock",header->sender), chop);
            free(chop);
            edits[ECF_RECIPIENTS]->editedThisFrame = edits[ECF_BODY]->editedThisFrame = edits[ECF_SUBJECT]->editedThisFrame = 1;

            window_setMode(WDW_EMAIL_COMPOSE, WINDOW_GROWING );
        }
        break;
    case EBC_FORWARD:
        {
            EmailHeader*    header = (EmailHeader*)emailListView->selectedItem;
            EmailMessage*    message = emailPrepareCacheMessage(&header->message_id, header->type);
            char    *chop = strdup(message->body);

            smf_SetRawText(edits[ECF_RECIPIENTS], "", 1 );

            estrPrintf(&edits[ECF_SUBJECT]->rawString, "Fw: %s", emailRemoveSubjectPrefix(header->subject));
            if (strlen(chop) > MAX_BODY/2)
                chop[MAX_BODY/2] = 0;
            estrPrintf(&edits[ECF_BODY]->rawString,textStd("EmailForwardBlock",header->sender,chop));
            free(chop);
            edits[ECF_RECIPIENTS]->editedThisFrame = edits[ECF_BODY]->editedThisFrame = edits[ECF_SUBJECT]->editedThisFrame = 1;
            window_setMode(WDW_EMAIL_COMPOSE, WINDOW_GROWING );
        }
        break;
    case EBC_SPAM:
    case EBC_DELETE:
        {
            EmailHeader* header = (EmailHeader*)emailListView->selectedItem;
            int msgUID = header->message_id;
            int headerIndex = eaFind(&emailListView->items, header);

            char buf[100];
            assert(headerIndex >= 0);

            if( command == EBC_SPAM )
            {
                if( header->auth_id )
                    cmdParsef("ignore_spammer_auth %i, %s", header->auth_id, header->sender); 
                else
                    cmdParsef("ignore_spammer %s", header->sender); 
            }

            // Delete the selected message.
            if( header )
            {
                if( header->type == kEmail_Certification  )
                {
                    if( header->claim_availible )
                        sprintf(buf, "account_certification_refund %s", header->attachment);
                    else
                        sprintf(buf, "account_certification_delete %s" , header->attachment);
                }
                else if( header->type == kEmail_Global && (header->influence || header->attachment && *header->attachment) )
                    sprintf(buf, "gmail_return %d", msgUID);
                else
                    sprintf(buf, "emaildelete %d", (header->type==kEmail_Global)?-msgUID:msgUID);
            }
            cmdParse(buf);

            // Select the next message.
            if(!uiLVSelectItem(emailListView, headerIndex+1))
            {
                uiLVSelectItem(emailListView, headerIndex-1);
            }

            // Free the deleted header.
            emailHeaderDelete(header->message_id, header->type, s_mailViewMode);
            emailHeaderListPrepareUpdate();    // Rebuild the email header list.
        }
        break;
    case EBC_SEND:
        break;
    case EBC_CANCEL:
        break;
    }
}

static void CreateEditsIfNoneExist()
{
    if( !edits[0] )
    {
        int i;
        for( i = 0; i < ECF_MAX; i++)
            edits[i] = smfBlock_Create();
    }
}

int emailWindow()
{
    F32        x,y,z,w,h,scale;
    int        color, bcolor;
    #define HDR_X (10+PIX3)
    UIBox drawArea, headerDrawArea, messageDrawArea, buttonDrawArea;

    if (!g_email_window_open)
    {
        g_email_window_open = 1;
        cmdParse("emailheaders");
    }

    CreateEditsIfNoneExist();

    // Common window behaviors.
     if (!window_getDims(WDW_EMAIL,&x,&y,&z,&w,&h,&scale,&color,&bcolor))
    {
        return 0;
    }
    drawArea.x = x;
    drawArea.y = y;
    drawArea.width = w;
    drawArea.height = h;

    if( h < winDefs[WDW_EMAIL].min_ht )
            window_setDims( WDW_EMAIL, -1, -1, -1, winDefs[WDW_EMAIL].min_ht );


    // Draw the email window frame.
    drawJoinedFrame2Panel(    PIX3, R10, x, y, z, scale, w, EMAIL_HEADER_PANE_HEIGHT*scale, drawArea.height - EMAIL_HEADER_PANE_HEIGHT*scale,
                              color, bcolor, &headerDrawArea, &messageDrawArea);

     if(window_getMode(WDW_EMAIL) != WINDOW_DISPLAYING)
        return 0;

    {
        UIListView* list = emailListView;
        int fullDrawHeight = uiLVGetFullDrawHeight(list);
        int currentHeight = uiLVGetHeight(list) - uiLVGetMinDrawHeight(list)/2;
        if(list)
        {
            if(fullDrawHeight > currentHeight)
            {
                doScrollBar(&emailListViewScrollbar, currentHeight, fullDrawHeight, headerDrawArea.width, headerDrawArea.y - y + (PIX3+R10)*scale, z+2, 0, &headerDrawArea );
                list->scrollOffset = emailListViewScrollbar.offset;
            }
            else
            {
                list->scrollOffset = 0;
            }
        }
    }

    // Draw the header pane.
    emailDrawHeaderPane(headerDrawArea, z+1, scale);

    uiBoxAlter(&messageDrawArea, UIBAT_SHRINK, UIBAD_BOTTOM, EMAIL_BUTTON_PANE_HEIGHT*scale);
    emailDrawReadMessagePane(messageDrawArea, color, z+1, scale);

    {
        EmailButtonCommands command;
        Entity * e = playerPtr();
        int iCharLevel = character_CalcExperienceLevel(e->pchar);
        int tierLocked = e->pl != NULL && !AccountHasEmailAccess(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags);
        buttonDrawArea = messageDrawArea;
        buttonDrawArea.y += buttonDrawArea.height;
        buttonDrawArea.height = EMAIL_BUTTON_PANE_HEIGHT*scale;
        command = emailReadWindwDrawCommandButtons(buttonDrawArea, z+1, (iCharLevel < 9) || entIsTrial(e) || tierLocked, scale);
        emailHandleButtonCommands(command);
    }
    return 0;
}

static int stringEmpty(char *str)
{
    char    *s;

    if (!str)
        return 1;
    for(s=str;*s;s++)
    {
        if (!isspace((unsigned char)*s) && *s != '"' && *s != ';')
            return 0;
    }
    return 1;
}

// convert email syntax to programmer friendly syntax
// " the dude ; killtron" -> "\"the dude\" \"killtron\""
int fixupRecipients(char *str,char *buf)
{
    int count = 0;
    size_t idx=0,start,end;

    buf[0] = 0;
    for(;*str;str+=end+start)
    {
        // This macro takes an argument, which it does not use, but it is
        // the standard way the player name length is specified - not chat
        // handle length, which is below.
        size_t maxname = MAX_PLAYER_NAME_LEN(0);

        start = strspn(str,",; \"");
        end = strcspn(str+start,";,\"");

        while (str[start+end-1] == ' ' && end)
            end--;

        // The global chat handle marker shouldn't count against the valid
        // name length.
        if (str[start] == '@')
            maxname = MAX_PLAYERNAME + 1;

        if (end > maxname)
            return -1;

        if (end)
        {
            strcat(buf,"\"");
            strncat(buf,str+start,end);
            strcat(buf,"\" ");
            count++;
        }
    }

    return count;
}


//----------------------------------------------------------------------------------------------------
// Email composition window
//----------------------------------------------------------------------------------------------------

static EmailCompositionField activeField;
static int composeWindowInit;

static int emailSelectActiveField(EmailCompositionField field)
{
    if(field >= ECF_MAX)
        return 0;

    smf_SetCursor( edits[field], 0 );
    activeField = field;

    return 1;
}

#include "win/input.h"
static void trayobj_DrawIcon( TrayObj * pObj, F32 x, F32 y, F32 z, F32 sc )
{
    static uiEnhancement *toolTipEnhancement = NULL;
       Entity *e = playerPtr();
    CBox box;
    AtlasTex * icon = 0;
    AtlasTex * icon2 = 0;

    switch( pObj->type )
    {
        xcase kTrayItemType_Recipe:
        {
            const DetailRecipe *recipe = detailrecipedict_RecipeFromId(pObj->invIdx);
            if ( recipe )
                   icon = uiRecipeDrawIcon( recipe, recipe->level, x, y+7*sc, z, sc*.65f, WDW_AUCTION, 1, CLR_WHITE);
        }
        xcase kTrayItemType_SpecializationInventory:
        {
            if ( e->pchar->aBoosts[pObj->ispec] )
            {
                uiEnhancement *enhancement =uiEnhancementCreate( e->pchar->aBoosts[pObj->ispec]->ppowBase->pchFullName, e->pchar->aBoosts[pObj->ispec]->iLevel );
                uiEnhancementSetColor(enhancement, CLR_WHITE);
                icon = atlasLoadTexture(e->pchar->aBoosts[pObj->ispec]->ppowBase->pchIconName);
                  icon2 = uiEnhancementDrawCentered( enhancement, x + 5*sc, y+5*sc, z, sc*.75, sc*.75, MENU_GAME, WDW_AUCTION, 1, 0 );
                if (enhancement->isDisplayingToolTip)
                {
                    uiEnhancementFree(&toolTipEnhancement);
                    toolTipEnhancement = enhancement;
                }
                else
                    uiEnhancementFree(&enhancement); 
            }
        }
        xcase kTrayItemType_Salvage:
        {
            const SalvageItem *salvage = e->pchar->salvageInv[pObj->invIdx]->salvage;
            if( salvage )
            {
                icon = atlasLoadTexture(salvage->ui.pchIcon);
                display_sprite( icon, x+2*sc, y, z, sc*.9f, sc*.9f, CLR_WHITE);
            }
        }
        xcase kTrayItemType_Inspiration:
        {                    
            if ( e->pchar->aInspirations[pObj->iset][pObj->ipow] )
            {
                 icon = atlasLoadTexture(e->pchar->aInspirations[pObj->iset][pObj->ipow]->pchIconName);
                  display_sprite( icon, x + 14*sc, y+15*sc, z, sc, sc, CLR_WHITE);
            }
        }
    }

     BuildCBox(&box, x, y, 60*sc, 60*sc );
    if( mouseCollision(&box) )
        setCursor( "cursor_hand_open.tga", NULL, FALSE, 2, 2 );
    if( mouseLeftDrag(&box) )
    {
        trayobj_startDragging( pObj, icon, icon2 ); 
    }

}

static void dragReceiveBoost( Entity * e ) 
{
    if ( e->pchar->aBoosts[cursor.drag_obj.ispec] )
    {
        const BasePower *ppow = e->pchar->aBoosts[cursor.drag_obj.ispec]->ppowBase;
        if (!detailrecipedict_IsBoostTradeable(ppow, e->pchar->aBoosts[cursor.drag_obj.ispec]->iLevel, "", ""))
        {
            addSystemChatMsg( textStd("CannotTradeEnhancement"), INFO_USER_ERROR, 0 );
        } 
        else if (e->pchar->aBoosts[cursor.drag_obj.ispec]->iNumCombines > 0) 
        {
            addSystemChatMsg( textStd("CantEmailCombined"), INFO_USER_ERROR, 0 );
        } 
        else 
        {
            trayobj_copy(&g_attachment, &cursor.drag_obj, 0);
            if( !edits[ECF_SUBJECT]->rawString || !strlen(edits[ECF_SUBJECT]->rawString) )
            {
                smf_SetRawText(edits[ECF_SUBJECT], textStd(ppow->pchDisplayName), 1 );
            }

            if (ppow->bBoostAccountBound)
            {
                // can only send to self
                char recipient[256];
                sprintf_s(recipient, 255, "@%s", game_state.chatHandle);
                smf_SetRawText(edits[ECF_RECIPIENTS], recipient, 0);
            }
        }
    }
}

static void emailDragRecieve(CBox *box)
{
    Entity * e = playerPtr();

    if ( cursor.dragging  ) 
    {
        if( !isDown( MS_LEFT ) && mouseCollision(box) )
        {
            switch(cursor.drag_obj.type)
            {
                xcase kTrayItemType_Recipe:
                {
                    const DetailRecipe *recipe = detailrecipedict_RecipeFromId(cursor.drag_obj.invIdx);
                    if ( recipe )
                    {
                        if (recipe->flags & RECIPE_NO_TRADE)
                            addSystemChatMsg( textStd("CannotTradeRecipe"), INFO_USER_ERROR, 0 );
                        else
                        {
                            trayobj_copy(&g_attachment, &cursor.drag_obj, 0);
                            if( !edits[ECF_SUBJECT]->rawString || !strlen(edits[ECF_SUBJECT]->rawString) )
                            {
                                smf_SetRawText(edits[ECF_SUBJECT], textStd(recipe->ui.pchDisplayName), 1 );
                            }
                        }
                    }
                }
                xcase kTrayItemType_Salvage:
                {
                    if ( e->pchar->salvageInv[cursor.drag_obj.invIdx] )
                    {
                        const SalvageItem *salvage = e->pchar->salvageInv[cursor.drag_obj.invIdx]->salvage;
                        if ( salvage )
                        {
                            if (salvage->flags & SALVAGE_NOTRADE)
                            {
                                addSystemChatMsg( textStd("CannotTradeSalvage"), INFO_USER_ERROR, 0 );
                            } 
                            else 
                            {
                                trayobj_copy(&g_attachment, &cursor.drag_obj, 0);
                                if( !edits[ECF_SUBJECT]->rawString || !strlen(edits[ECF_SUBJECT]->rawString) )
                                {
                                    smf_SetRawText(edits[ECF_SUBJECT], textStd(salvage->ui.pchDisplayName), 1 );
                                }
                            }
                        }
                    }
                }
                xcase kTrayItemType_SpecializationInventory:
                    dragReceiveBoost(e);
                xcase kTrayItemType_Inspiration:
                {
                    const BasePower *ppow = e->pchar->aInspirations[cursor.drag_obj.iset][cursor.drag_obj.ipow];
                    if (ppow && inspiration_IsTradeable(ppow, "", ""))    // you're allowed to drag in any account bound items
                                                                        // you just may not be allowed to send the email
                    {
                        trayobj_copy(&g_attachment, &cursor.drag_obj, 0);

                        if( !edits[ECF_SUBJECT]->rawString || !strlen(edits[ECF_SUBJECT]->rawString) )
                        {
                            smf_SetRawText(edits[ECF_SUBJECT], textStd(e->pchar->aInspirations[cursor.drag_obj.iset][cursor.drag_obj.ipow]->pchDisplayName), 1 );
                        }
                    }
                    else
                    {
                        dialogStd(DIALOG_OK, "EmailAttachmentsCantSendUntradeableItems", NULL, NULL,0,0,1);
                    }
                }
            }

            trayobj_stopDragging();
        }
    }
}

void emailDrawComposeMessagePane(UIBox drawAreaIn, float z, float sc)
{
    char* toLabel = "EmailToField";
     char* subjectLabel = "EmailSubjectField";
    int fieldLabelWidth[2] = {str_wd(font_grp, sc, sc, toLabel)+ 3*sc, str_wd(font_grp, sc, sc, subjectLabel) + 3*sc};
    AtlasTex* divider_mid = atlasLoadTexture("Headerbar_VertDivider_MID.tga");
    AtlasTex* divider_end = atlasLoadTexture("Headerbar_HorizDivider_R.tga");
    UIBox drawArea = drawAreaIn;
    int    fontHeight = ttGetFontHeight(font_grp, sc, sc);
    F32 tx, ty, twd, tht, text_ht;
     F32 attachment_wd = 60*sc;
    F32 influence_wd = 80*sc;
    CBox box;

    CreateEditsIfNoneExist();

    // this needs to happen before we set the cursor.
     smf_SetFlags(edits[ECF_RECIPIENTS], SMFEditMode_ReadWrite, SMFLineBreakMode_None, 
        SMFInputMode_None, MAX_RECIPIENTS, SMFScrollMode_ExternalOnly, SMFOutputMode_StripAllTags,
        SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, "EmailTabNavigationSet", -1); 

    // Initialize all the edit boxes
    if (!composeWindowInit)
    {
        Wdw* window = wdwGetWindow(WDW_EMAIL_COMPOSE);

        smf_SetCursor(edits[0], 0);
        composeWindowInit = 1;
    }

     uiBoxAlter(&drawArea, UIBAT_SHRINK, (UIBoxAlterDirection)(UIBAD_LEFT | UIBAD_RIGHT | UIBAD_TOP),
        10*sc);
    clipperPushRestrict(&drawArea);    // restrict to draw area inside window
    tx = drawArea.x;
    ty = drawArea.y;
    twd = drawArea.width;
    tht = drawArea.height;
    s_tEmail.piScale = (int *)((intptr_t)(1.0f*sc*SMF_FONT_SCALE));

    // The "To" field.
    font_color(CLR_WHITE,CLR_WHITE);
     prnt(tx, ty + 20*sc, z, sc, sc, toLabel);


       text_ht = smf_Display(edits[ECF_RECIPIENTS], tx + fieldLabelWidth[0] + 5*sc, ty+4*sc, z+1, twd - fieldLabelWidth[0] - attachment_wd - 20*sc, 0, 0, 0, &s_tEmail, 0);
     drawTextEntryFrame( tx + fieldLabelWidth[0], ty, z, twd - fieldLabelWidth[0] - attachment_wd - 10*sc, text_ht +6*sc, sc, 2 );

    // The Attchment Area
     BuildCBox(&box, tx + twd - attachment_wd, ty, attachment_wd, attachment_wd );
    emailDragRecieve(&box);

    drawTextEntryFrame( tx + twd - attachment_wd, ty, z, attachment_wd, attachment_wd, sc, 3);
      if(g_attachment.type != kTrayItemType_None)
        trayobj_DrawIcon( &g_attachment, tx + twd - attachment_wd, ty, z, sc );
    else
    {
        font_outl(0);
        font_ital(1);
           cprnt(tx + twd - attachment_wd/2 - 2*sc, ty + attachment_wd/2 + 5*sc, z+2, sc*.8, sc*.8, "AttachmentString");
        font_outl(1);
        font_ital(0);
    }

    font( &game_12 );
    font_color(CLR_WHITE, CLR_WHITE);
     // The "Subject" field.
      ty += text_ht + 17*sc;
    prnt(tx, ty + 20*sc, z, sc, sc, subjectLabel);
    smf_SetFlags(edits[ECF_SUBJECT], SMFEditMode_ReadWrite, SMFLineBreakMode_None,
        SMFInputMode_None, MAX_SUBJECT, SMFScrollMode_ExternalOnly, SMFOutputMode_StripAllTags, 
        SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 
        "EmailTabNavigationSet", -1); 
       text_ht = smf_Display(edits[ECF_SUBJECT], tx + fieldLabelWidth[1] + 5*sc, ty + 4*sc, z+1, twd - fieldLabelWidth[1] - attachment_wd - influence_wd- 30*sc, 0, 0, 0, &s_tEmail, 0);
    drawTextEntryFrame( tx + fieldLabelWidth[1], ty, z, twd - fieldLabelWidth[1] - attachment_wd - influence_wd - 20*sc, text_ht +6*sc, sc, 2);

    // Influence Entry
     if( !strlen(edits[ECF_INFLUENCE]->outputString ) && !smfBlock_HasFocus(edits[ECF_INFLUENCE]) )
    {
        font_outl(0);
        font_ital(1);
         cprnt(tx + twd - attachment_wd - influence_wd/2 - 10*sc, ty + 17*sc, z+2, sc, sc, "InfluenceString");
        font_outl(1);
        font_ital(0);
    }
     smf_SetFlags(edits[ECF_INFLUENCE], SMFEditMode_ReadWrite, SMFLineBreakMode_None, 
        SMFInputMode_NonnegativeIntegers, 0, SMFScrollMode_ExternalOnly, SMFOutputMode_None,
        SMFDisplayMode_NumbersWithCommas, SMFContextMenuMode_None, SMAlignment_Left, 0, 
        "EmailTabNavigationSet", -1); 
     smf_Display(edits[ECF_INFLUENCE], tx + twd - attachment_wd - influence_wd - 5*sc, ty + 4*sc, z+1, influence_wd, 0, 0, 0, &s_tEmail, 0);
    drawTextEntryFrame( tx + twd - attachment_wd - influence_wd - 10*sc, ty, z, influence_wd, 22*sc, sc, 2 );

       ty += text_ht + 10*sc;
     if( ty - drawArea.y < 70*sc )
        ty = drawArea.y + 70*sc;

    // Writing Area
    smf_SetScissorsBox(edits[ECF_BODY], tx, ty+2*sc, twd, drawArea.height - (ty - drawArea.y)-4*sc);
     smf_SetFlags(edits[ECF_BODY], SMFEditMode_ReadWrite, SMFLineBreakMode_None, 
        SMFInputMode_None, MAX_BODY, SMFScrollMode_InternalScrolling, SMFOutputMode_StripAllTags,
        SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0,
        "EmailTabNavigationSet", -1); 
      smf_Display(edits[ECF_BODY], tx + 5*sc, ty + 4*sc, z+1, twd - 10*sc, drawArea.height - (ty - drawArea.y) - 10*sc, 0, 0, &s_tEmail, 0);
     drawTextEntryFrame( tx, ty, z, twd, drawArea.height - (ty - drawArea.y), sc, 2 );

    clipperPop();                    // restrict to draw area inside window
}

static EmailButtonCommands emailComposeDrawCommandButtons(UIBox drawArea, float z, float sc )
{
    char* caption;
    int buttonGroupGap = 20*sc;
    int buttonGap = 5;
    int groupButtonCount = 2;
    UIBox buttonBox, groupBounds;
    int intendedButtonHeight = 26*sc;
    int intendedButtonWidth = 100*sc;
    int flags = 0;
    EmailButtonCommands result = 0;

    groupBounds = drawArea;
    groupBounds.y += (EMAIL_BUTTON_PANE_HEIGHT*sc - intendedButtonHeight)/2;
    groupBounds.height = intendedButtonHeight;

    buttonBox = groupBounds;
    buttonBox.width = intendedButtonWidth;
    buttonBox.x += (groupBounds.width - intendedButtonWidth * groupButtonCount)/2;

    // Draw the "Send" button
    caption = "SendString";
    if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_EMAIL), caption, sc, flags))
    {
        result |= EBC_SEND;
    }

    // Draw the "Cancel" button
    caption = "CancelString";
    buttonBox.x += buttonBox.width + buttonGap;
     if(D_MOUSEHIT == uiBoxDrawStdButton(buttonBox, z, window_GetColor(WDW_EMAIL), caption, sc, flags))
    {
        result |= EBC_CANCEL;
    }

    return result;
}

void emailSendToServer(char* recipients, char* subject, char* message, int influence, TrayObj *pAttachment )
{
    char* cmd = NULL;

    switch( pAttachment->type )
    {
        xcase kTrayItemType_None:
        {
            if( influence )
                estrPrintf(&cmd, "emailsendattachment \"%s\" \"%s\" %i %i %i %s", recipients, subject, influence, 0, 0, message );
            else
                estrPrintf(&cmd, "emailsend \"%s\" \"%s\" %s", recipients, subject, message );
        }
        xcase kTrayItemType_Recipe:        estrPrintf(&cmd, "emailsendattachment \"%s\" \"%s\" %i %i %i %s ", recipients, subject, influence, pAttachment->type, pAttachment->invIdx, message );
        xcase kTrayItemType_Salvage:     estrPrintf(&cmd, "emailsendattachment \"%s\" \"%s\" %i %i %i %s ", recipients, subject, influence, pAttachment->type, pAttachment->invIdx, message );
        xcase kTrayItemType_SpecializationInventory:    estrPrintf(&cmd, "emailsendattachment \"%s\" \"%s\" %i %i %i %s", recipients, subject, influence, pAttachment->type, pAttachment->ispec, message );
        xcase kTrayItemType_Inspiration:    estrPrintf(&cmd, "emailsendattachment \"%s\" \"%s\" %i %i %i %s", recipients, subject, influence, pAttachment->type, (pAttachment->iset*10)+pAttachment->ipow, message  );
    }
    cmdParse(cmd);
    estrDestroy(&cmd);
}

int globalEmailIsFull()
{
    return eaSize(&g_ppHeader[MVM_MAIL]) >= MAX_GMAIL; 
}

static void emailComposeHandleSendButton()
{
    char    *recips = NULL,*subj = NULL,*body = NULL,recip_buf[MAX_RECIPIENTS*2] = "";
    char    *errorMsg = NULL;
    int        influence, recip_cnt, fieldLength = 0;
    Entity *e = playerPtr();

    estrPrintEString(&recips, edits[ECF_RECIPIENTS]->outputString);
    if (stringEmpty(recips))
    {
        dialogStd(DIALOG_OK,"EmailNoRecipientError", NULL, NULL,0,0,1);
        goto Cleanup;
    }

    estrPrintEString(&subj, edits[ECF_SUBJECT]->outputString);
    if (stringEmpty(subj))
    {
        dialogStd(DIALOG_OK,"EmailNoSubjectError", NULL, NULL,0,0,1);
        goto Cleanup;
    }

    influence = atoi_ignore_nonnumeric(edits[ECF_INFLUENCE]->outputString);

    if( influence > 999999999 )
    {
        dialogStd(DIALOG_OK,"EmailInfluenceCap", NULL, NULL,0,0,1);
        return;
        
    }

    if( influence < 0 || influence > e->pchar->iInfluencePoints )
    {
        dialogStd(DIALOG_OK,"EmailNotEnoughInfluence", NULL, NULL,0,0,1);
        return;
    }

    recip_cnt = fixupRecipients(recips, recip_buf);

    if (recip_cnt < 0)
    {
        dialogStd(DIALOG_OK,"EmailRecipientTooLongError", NULL, NULL,0,0,1);
        goto Cleanup;
    }

    if( recip_cnt > 2 && (influence > 0 || g_attachment.type != kTrayItemType_None) )
    {
        dialogStd(DIALOG_OK,"EmailAttachmentsOnlyGetOneSender", NULL, NULL,0,0,1);
        goto Cleanup;
    }

    if ( g_attachment.type == kTrayItemType_SpecializationInventory)
    {
        Boost *pBoost = e->pchar->aBoosts[g_attachment.ispec];

        if (pBoost != NULL && !detailrecipedict_IsBoostTradeable(pBoost->ppowBase, pBoost->iLevel, game_state.chatHandle, &(recips[1])))
        {
            dialogStd(DIALOG_OK,"EmailAttachmentsCanOnlySendAccountItemsToYourself", NULL, NULL,0,0,1);
            goto Cleanup;
        }
    }

    if ( g_attachment.type == kTrayItemType_Inspiration)
    {
        const BasePower *ppow = e->pchar->aInspirations[g_attachment.iset][g_attachment.ipow];

        if (ppow != NULL && !inspiration_IsTradeable(ppow, game_state.chatHandle, &(recips[1])))
        {
            dialogStd(DIALOG_OK,"EmailAttachmentsCanOnlySendAccountItemsToYourself", NULL, NULL,0,0,1);
            goto Cleanup;
        }
    }

    estrPrintCharString(&recips, (char*)escapeString(recip_buf));
    
    fieldLength += estrLength(&recips);
    if(estrLength(&recips) >= MAX_RECIPIENTS)
    {
        char* str = textStd("EmailRecipientsTooLong", 1 + estrLength(&recips) - MAX_RECIPIENTS);
        dialogStd(DIALOG_OK, str, NULL, NULL,0,0,1);
        goto Cleanup;
    }


    estrPrintCharString(&subj, (char*)escapeString(subj));
    fieldLength += estrLength(&subj);
    if(estrLength(&subj) >= MAX_SUBJECT)
    {
        char* str = textStd("EmailSubjectTooLong", 1 + estrLength(&subj) - MAX_SUBJECT);
        dialogStd(DIALOG_OK, str, NULL, NULL,0,0,1);
        goto Cleanup;
    }

    estrPrintEString(&body, edits[ECF_BODY]->rawString);
    if(!body || isEmptyStr(body))
    {
        char* str = textStd("EmailMissingBody");
        dialogStd(DIALOG_OK, str, NULL, NULL,0,0,1);
        goto Cleanup;
    }

    estrPrintCharString(&body, (char*)escapeString(body));
    fieldLength += estrLength(&body);
    if(estrLength(&body) >= MAX_BODY)
    {
        char* str = textStd("EmailBodyTooLong", 1 + estrLength(&body) - MAX_BODY);
        dialogStd(DIALOG_OK, str, NULL, NULL,0,0,1);
        goto Cleanup;
    }
    if ( fieldLength >= MAX_EMAIL_LENGTH)
    {
        char* str = textStd("EmailBodyTooLong", 1 + fieldLength - MAX_EMAIL_LENGTH);
        dialogStd(DIALOG_OK, str, NULL, NULL,0,0,1);
        goto Cleanup;
    }
    emailSendToServer(recips, subj, body, influence, &g_attachment );

Cleanup:
    estrDestroy(&recips);
    estrDestroy(&subj);
    estrDestroy(&body);
    estrDestroy(&errorMsg);
}

void emailComposeHandleButtonCommands(EmailButtonCommands command)
{
    switch(command)
    {
    case EBC_SEND:
        emailComposeHandleSendButton();
        break;
    case EBC_CANCEL:
        {
            int i;
             for(i=0; i<ECF_MAX; i++)
                smf_SetRawText(edits[i], "", 0 );
            g_attachment.type = kTrayItemType_None;

            window_setMode(WDW_EMAIL_COMPOSE, WINDOW_SHRINKING);
        }
        break;
    }
}

int emailComposeWindow()
{
    F32        x,y,z,w,h,scale;
    int        color, bcolor;
    UIBox    windowDrawArea, drawArea;
    EmailButtonCommands command;
    Entity * e = playerPtr();

    if( entIsTrial(e) )
        window_setMode(WDW_EMAIL_COMPOSE,WINDOW_DOCKED);

     if (!window_getDims(WDW_EMAIL_COMPOSE,&x,&y,&z,&w,&h,&scale,&color, &bcolor))
    {
        composeWindowInit = 0;
        g_attachment.type = kTrayItemType_None;
        uiReturnFocus(edits[activeField]);
        return 0;
    }

    font(&game_12);
    uiBoxDefine(&windowDrawArea, x, y, w, h );
     drawFrame(PIX3, R10, x, y, z, w, h, scale, color, bcolor);
     uiBoxAlter(&drawArea, UIBAT_SHRINK, UIBAD_ALL, PIX3*2*scale);
    drawArea = windowDrawArea;
     uiBoxAlter(&drawArea, UIBAT_SHRINK, UIBAD_BOTTOM, EMAIL_BUTTON_PANE_HEIGHT*scale);
    emailDrawComposeMessagePane(drawArea, z+1, scale);

    drawArea.y += drawArea.height;
    drawArea.height = windowDrawArea.height - (drawArea.y - windowDrawArea.y);
    clipperPushRestrict(&drawArea);
    command = emailComposeDrawCommandButtons(drawArea, z+1, scale);
    clipperPop();
    emailComposeHandleButtonCommands(command);
    return 0;
}

void removePercents(char *s)
{
    char *c=s;
    while (c = strstr(c, "%")) {
        *c='@';
    }
}


void emailSetNewMessageStatus(int status,char *msg)
{
    if (status)
        window_setMode(WDW_EMAIL_COMPOSE,WINDOW_SHRINKING);
    else
        dialogStd(DIALOG_OK,msg, NULL, NULL,0,0,1);
}

void email_setView(char *view)
{
    int currentMode = s_mailViewMode;
    if (!view || !emailListView || !emailListView->header || !eaSize(&emailListView->header->columns))
        return;
    if (stricmp(view, "Mail") == 0)
    {
        s_mailViewMode = MVM_MAIL;
    }
    else if (stricmp(view, "Certification") == 0)
    {
        s_mailViewMode = MVM_CERTIFICATION;
    }
    else if (stricmp(view, "Voucher") == 0)
    {
        s_mailViewMode = MVM_VOUCHER;
    }
    if (s_mailViewMode != currentMode)
    {
        if (currentMode == MVM_MAIL)
        {
            emailListView = certVoucherEmailListView;
        }
        if (s_mailViewMode == MVM_MAIL)
        {
            emailListView = playerEmailListView;
        }

        listRebuildRequired = 1;
    }
}

int email_getView()
{
    return s_mailViewMode;
}
