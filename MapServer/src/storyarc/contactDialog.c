/*\
 *
 *    contactDialog.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    the main contact dialog function
 *
 */
#include "dbcomm/logcomm.h"
#include "dbcomm/dbdoor.h"
#include "contact.h"
#include "storyarcprivate.h"
#include "entity/character_level.h"
#include "entity/character_eval.h"
#include "gameSys/dooranim.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/entPlayer.h"
#include "gameData/store_net.h"
#include "comm_game.h"
#include "svr/svr_base.h"
#include "Reward.h"
#include "TeamTask.h"
#include "container/team.h"
#include "TeamReward.h"
#include "entity/costume.h"
#include "entity/character_base.h"
#include "entity/entity.h"
#include "taskRandom.h"
#include "entity/character_combat.h"
#include "dbcomm/dbmapxfer.h"
#include "cmdparse/cmdserver.h"
#include "gameComm/svr_chat.h"
#include <utilitieslib/components/StringCache.h>
#include "entity/Supergroup.h"
#include "entity/SgrpServer.h"
#include "bases/basesystems.h"
#include "player/badges_server.h"
#include "taskforce.h"
#include "entity/TaskforceParams.h"
#include "playerCreatedStoryarcServer.h"
#include "storyarc/pnpcCommon.h"
#include "mission/missionserver_meta.h"
#include <utilitieslib/components/HashFunctions.h>
#include "dbcomm/dbcomm.h"

// *********************************************************************************
//  Main contact dialog
// *********************************************************************************

#define MAX_NEWSPAPER_ARTICLES    3
#define NEWS_SEED_OFFSET        20

#define UNPACK_CONTACT_INTERACT_CONTEXT(context)                \
    int link        = context->link;                            \
    Entity* player    = context->player;                            \
    StoryInfo* info = context->info;                            \
    StoryContactInfo* contactInfo = context->contactInfo;        \
    ContactResponse* response    = context->response;            \
    ScriptVarsTable* vars        = context->vars;

void ContactTaskForceInteraction(ContactInteractionContext* context);
void ContactInteractionFormTaskForce(ContactInteractionContext* context);
void ContactInteractionPromptClue(ContactInteractionContext* context);
void ContactInteractionSayGoodbye(Entity* player, const ContactDef* contactDef, int link, ContactResponse* response, int canBeDismissed);
void ContactFrontPage(ContactInteractionContext* context, int showbusy, int hideHeadShot);

void AddResponse(ContactResponse* response, const char* text, int link)
{
    if (text && response->responsesFilled < MAX_CONTACT_RESPONSES)
    {
        strcpy(response->responses[response->responsesFilled].responsetext, text);
        strcpy(response->responses[response->responsesFilled].rightResponseText, "");
        response->responses[response->responsesFilled].link = link;
        response->responses[response->responsesFilled].rightLink = 0;
        response->responsesFilled++;
    }
}

static void AddResponseWithRight(ContactResponse *response, const char *text, int link, const char *rightText, int rightLink)
{
    if (text && rightText && response->responsesFilled < MAX_CONTACT_RESPONSES)
    {
        strcpy(response->responses[response->responsesFilled].responsetext, text);
        strcpy(response->responses[response->responsesFilled].rightResponseText, rightText);
        response->responses[response->responsesFilled].link = link;
        response->responses[response->responsesFilled].rightLink = rightLink;
        response->responsesFilled++;
    }
}

static char * getContactHeadshotSMF(Contact *pContact, Entity *player)
{
    static char * estr;
    Entity * e = pContact->entParent;

    estrClear(&estr);
    if( e && e->doppelganger )
        estrConcatf(&estr, "<img align=left src=svr:%d>", e->owner );
    else
        estrConcatf(&estr, "<img align=left src=npc:%d>", ContactNPCNumber(pContact->def, player));

    return estr;
}

// show introductory paragraph for a contact
static void ShowContactIntroInfo(ContactInteractionContext* context, Contact* contact, char* introlink)
{
    ScriptVarsTable tempvars = {0};
    char contactname[200];
    UNPACK_CONTACT_INTERACT_CONTEXT(context)

    // set up the parameters
    strcpy(contactname, saUtilTableLocalize(contact->def->displayname, vars));
    ScriptVarsTablePushVarEx(&tempvars, "Contact", contact->def, 'C', 0);
    ScriptVarsTablePushVarEx(&tempvars, "ContactName", contact->def, 'C', 0);
    ScriptVarsTablePushVarEx(&tempvars, "Hero", player, 'E', 0);
    ScriptVarsTablePushVarEx(&tempvars, "HeroName", player, 'E', 0);
    ScriptVarsTablePushVar(&tempvars, "Accept", introlink);

    strcatf(response->dialog, "<table>");
    strcatf(response->dialog, "<tr highlight=0>");
    strcatf(response->dialog, "<td>");
    strcatf(response->dialog, "<table border=0>");
    strcatf(response->dialog, "<tr highlight=0 border=0>");
    if (CONTACT_HAS_HEADSHOT(contact->def)) // here to remove the <td> when not needed, rechecked in ContactHeadshot
    {
        strcatf(response->dialog, "<td border=0>");
        strcatf(response->dialog, ContactHeadshot(contact, player, 0));
        strcatf(response->dialog, "</td>");
    }
    strcatf(response->dialog, "<td width=100 valign=center>");
    strcatf(response->dialog, "<b>%s</b>", contactname);
    strcatf(response->dialog, "</td>");
    strcatf(response->dialog, "</tr>");
    strcatf(response->dialog, "</table>");
    strcatf(response->dialog, "</td>");
    strcatf(response->dialog, "</tr>");
    strcatf(response->dialog, "<tr highlight=0>");
    strcatf(response->dialog, "<td>");
    strcatf(response->dialog, "%s",    saUtilTableAndEntityLocalize(contact->def->introduction, &tempvars, player));
    strcatf(response->dialog, "</td>");
    strcatf(response->dialog, "</tr>");
    strcatf(response->dialog, "</table>");
}

// page to choose between contacts
void ContactInteractionChooseContact(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
        ContactHandle* picks;
    Contact *contact1, *contact2, *contact3, *contact4;
    int nextLevel;
    ScriptVarsTable vars1 = {0};
    ScriptVarsTable vars2 = {0};
    ScriptVarsTable vars3 = {0};
    ScriptVarsTable vars4 = {0};

    picks = ContactPickNewContacts(player, info, contactInfo, &nextLevel, 0);
    if(!picks || !picks[0])
        return;
    contact1 = GetContact(picks[0]);
    contact2 = GetContact(picks[1]);
    contact3 = GetContact(picks[2]);
    contact4 = GetContact(picks[3]);
    if (!contact1)
    {
        log_printf("storyerrs.log", "ContactInteractionChooseContact - I got handle %i, but then didn't get def from it??", picks[0]);
        return;
    }

    // set up params
    // ScriptVars HACK: the seed here is fire-and-forget, but doesn't seem to be a big deal
    //  NB: We build the base using the scope of the Introducer, and then we push the scope of the Introducee on top of that, so it takes precedence
    saBuildScriptVarsTable(&vars1, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, NULL, 0, (unsigned int)time(NULL), false);
    ScriptVarsTablePushScope(&vars1, &contact1->def->vs);

    if (contact2)
    {
        saBuildScriptVarsTable(&vars2, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, NULL, 0, (unsigned int)time(NULL), false);
        ScriptVarsTablePushScope(&vars2, &contact2->def->vs);
    }
    if (contact3)
    {
        saBuildScriptVarsTable(&vars3, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, NULL, 0, (unsigned int)time(NULL), false);
        ScriptVarsTablePushScope(&vars3, &contact3->def->vs);
    }
    if (contact4)
    {
        saBuildScriptVarsTable(&vars4, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, NULL, 0, (unsigned int)time(NULL), false);
        ScriptVarsTablePushScope(&vars4, &contact4->def->vs);
    }

    if (contact2) // choosing between more than one contact
    {
        // two column dialog
        strcat(response->dialog, "<linkhoverbg #118866aa><link white><linkhover white><TABLE><TR>");
        strcat(response->dialog, "<A HREF=CONTACTLINK_INTRODUCE_CONTACT1><TD highlight=1>");
        ShowContactIntroInfo(context, contact1, "CONTACTLINK_INTRODUCE_CONTACT1");
        strcat(response->dialog, "</td></A>");
        strcat(response->dialog, "<A HREF=CONTACTLINK_INTRODUCE_CONTACT2><TD highlight=1>");
        ShowContactIntroInfo(context, contact2, "CONTACTLINK_INTRODUCE_CONTACT2");
        strcat(response->dialog, "</TD></A>");
        strcat(response->dialog, "</TR></TABLE></linkhoverbg>");
        if (contact3)
        {
            strcat(response->dialog, "<linkhoverbg #118866aa><link white><linkhover white><TABLE><TR>");
            strcat(response->dialog, "<A HREF=CONTACTLINK_INTRODUCE_CONTACT3><TD highlight=1>");
            ShowContactIntroInfo(context, contact3, "CONTACTLINK_INTRODUCE_CONTACT3");
            strcat(response->dialog, "</TD></A>");
            if (contact4)
            {
                strcat(response->dialog, "<A HREF=CONTACTLINK_INTRODUCE_CONTACT4><TD highlight=1>");
                ShowContactIntroInfo(context, contact4, "CONTACTLINK_INTRODUCE_CONTACT4");
                strcat(response->dialog, "</TD></A>");
            }
            strcat(response->dialog, "</TR></TABLE></linkhoverbg>");
        }

        AddResponse(response, saUtilTableAndEntityLocalize(contact1->def->introductionAccept, &vars1, player), CONTACTLINK_INTRODUCE_CONTACT1);
        AddResponse(response, saUtilTableAndEntityLocalize(contact2->def->introductionAccept, &vars2, player), CONTACTLINK_INTRODUCE_CONTACT2);
        if (contact3) AddResponse(response, saUtilTableAndEntityLocalize(contact3->def->introductionAccept, &vars3, player), CONTACTLINK_INTRODUCE_CONTACT3);
        if (contact4) AddResponse(response, saUtilTableAndEntityLocalize(contact4->def->introductionAccept, &vars4, player), CONTACTLINK_INTRODUCE_CONTACT4);
        AddResponse(response, saUtilLocalize(context->player,"DontChooseContact"), CONTACTLINK_MAIN);
    }
    else // only choosing one contact
    {
        strcat(response->dialog, saUtilTableAndEntityLocalize(contact1->def->introductionSendoff, &vars1, player));
        strcat(response->dialog, "<BR><BR>");
        ShowContactIntroInfo(context, contact1, "CONTACTLINK_MAIN");
        ContactAdd(player, info, picks[0], contactInfo->contact->def->filename);
        contactInfo->contactsIntroduced |= (nextLevel ? CI_NEXT_LEVEL : CI_SAME_LEVEL);
        ContactMarkDirty(player, info, contactInfo); // hasTask has probably changed from one to zero because there's no longer a contact to introduce
        AddResponse(response, saUtilLocalize(context->player,"TalkAboutElse"), CONTACTLINK_MAIN);
    }
}

int ContactCanIntroduceNewContacts(Entity *player, StoryInfo *info, StoryContactInfo *contactInfo)
{
   ContactHandle* picks;
   picks = ContactPickNewContacts(player, info, contactInfo, NULL, 1);
    if(!picks || !picks[0])
        return false;
    else
        return true;
}

// add some text to front page if it is possible to introduce one or more contacts
void ContactInteractionPromptNewContact(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
        ContactHandle* picks;
    const char* str;
    ScriptVarsTable tempvars = {0};

    picks = ContactPickNewContacts(player, info, contactInfo, NULL, 0);
    if(!picks || !picks[0])
        return;

    ScriptVarsTableCopy(&tempvars, vars);
    ScriptVarsTableSetSeed(&tempvars, (unsigned int)time(NULL));
    if (picks[1]) // choosing between more than one contact
        str = contactInfo->contact->def->introduceTwoContacts;
    else
    {
        const ContactDef* othercontact = ContactDefinition(picks[0]);
        ScriptVarsTablePushVar(&tempvars, "ContactName", othercontact->displayname);
        str = contactInfo->contact->def->introduceOneContact;
    }
    str = saUtilTableAndEntityLocalize(str, &tempvars, player);
    if (!str) return;

    strcat(response->dialog, str);
    strcat(response->dialog, "<BR><BR>");
    AddResponse(response, saUtilLocalize(context->player,"AskIntroduce"), CONTACTLINK_INTRODUCE);
}

void ContactInteractionIntroduceNewContact(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
        ContactHandle* picks;
    int pickIndex = link - CONTACTLINK_INTRODUCE_CONTACT1;
    int nextLevel;

    picks = ContactPickNewContacts(player, info, contactInfo, &nextLevel, 0);
    if (!picks || !picks[pickIndex])
        return;

    strcat(response->dialog, saUtilLocalize(context->player,"AreYouSure"));
    AddResponse(response, saUtilLocalize(context->player,"Yes"), CONTACTLINK_ACCEPT_CONTACT1 + pickIndex);
    AddResponse(response, saUtilLocalize(context->player,"No"), CONTACTLINK_INTRODUCE);
}

void ContactInteractionAcceptNewContact(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
        ContactHandle* picks;
    const ContactDef* def;
    int pickIndex = link - CONTACTLINK_ACCEPT_CONTACT1;
    int nextLevel;

    picks = ContactPickNewContacts(player, info, contactInfo, &nextLevel, 0);
    if (!picks || !picks[pickIndex])
        return;

    ContactAdd(player, info, picks[pickIndex], contactInfo->contact->def->filename);
    contactInfo->contactsIntroduced++;
    contactInfo->rewardContact = 0;
    ContactMarkDirty(player, info, contactInfo); // hasTask has probably changed from one to zero because there's no longer a contact to introduce
    def = ContactDefinition(picks[pickIndex]);
    contactInfo->contactsIntroduced |= (nextLevel ? CI_NEXT_LEVEL : CI_SAME_LEVEL);

    if(def->introductionSendoff)
    {
        ScriptVarsTable defvars = {0};

        // set up vars for other contact
        //  NB: We build the base using the scope of the Introducer, and then we push the scope of the Introducee on top of that, so it takes precedence
        saBuildScriptVarsTable(&defvars, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, NULL, 0, (unsigned int)time(NULL), false);
        ScriptVarsTablePushScope(&defvars, &def->vs);

        strcpy(response->dialog, saUtilTableAndEntityLocalize(def->introductionSendoff, &defvars, player));
        strcat(response->dialog, "<BR><BR>");

        if (def->SOLintroductionSendoff)
            strcpy_s(SAFESTR(response->SOLdialog), saUtilTableAndEntityLocalize(def->SOLintroductionSendoff, &defvars, player));
    }

    // show the main page
    ContactFrontPage(context, 1, 0);
}

// Check to see if a player has completed a newspaper task within the last NEWSPAPER_HIST_MAX completions
int ContactNewspaperTaskHistoryLookup(StoryInfo* info, const char* unique, int historyIndex)
{
    int i;
    if(!unique)
        return 0;
    for(i=0; i<NEWSPAPER_HIST_MAX; i++)
        if(info->newspaperTaskHistory[historyIndex][i] && !stricmp(info->newspaperTaskHistory[historyIndex][i], unique))
            return 1;
    return 0;
}

// Update the players most recent newspaper tasks completed.
int ContactNewspaperTaskHistoryUpdate(StoryInfo* info, const char* unique, int historyIndex)
{
    int idx = info->newspaperTaskHistoryIndex[historyIndex]++;
    info->newspaperTaskHistoryIndex[historyIndex] %= NEWSPAPER_HIST_MAX;
    info->newspaperTaskHistory[historyIndex][idx] = allocAddString((char*)unique);
    return 1;
}

// Checks to see if all the articles are unique, checks history on the newest article
int ContactNewspaperTasksUnique(StoryInfo* info, StoryContactInfo* contactInfo, ContactTaskPickInfo* articles, int newestArticle)
{
    int i, j;
    static char uniqueString[MAX_NEWSPAPER_ARTICLES][256];
    char* searchString;
    const char* foundString;

    // Look for an item name(Get_Task), if none, check for person name(Hostage_Task)
    for(i = 0; i <= newestArticle; i++)
    {
        ScriptVarsTable vars = {0};
        unsigned int seed = contactInfo->dialogSeed + i*NEWS_SEED_OFFSET; // Tweak the seed each time
        uniqueString[i][0] = '\0';

        // Setup the search string
        if (articles[i].taskDef->newspaperType == NEWSPAPER_HOSTAGE)
            searchString = "PERSON_NAME";
        else if (articles[i].taskDef->newspaperType == NEWSPAPER_GETITEM)
            searchString = "ITEM_NAME";
        else // a type allowed to repeat
            continue;

        // Now do a lookup to compare against the history and other tasks
        saBuildScriptVarsTable(&vars, NULL, 0, contactInfo->handle, NULL, NULL, &articles[i].sahandle, NULL, NULL, 0, seed, false);
        foundString = ScriptVarsTableLookup(&vars, searchString);

        // Already completed this task recently, only need to check the current article
        if ((i == newestArticle) && ContactNewspaperTaskHistoryLookup(info, foundString, (searchString == "PERSON_NAME")))
            return 0;
        strcpy_s(SAFESTR(uniqueString[i]), foundString);
        
        // Make sure we don't have duplicates
        for (j = 0; j < i; j++)
            if (uniqueString[i] && uniqueString[j] && !stricmp(uniqueString[i], uniqueString[j]))
                return 0;
    }
    return 1;
}

//calling this function reseeds the random number generator
int ContactGenerateNewspaperArticles(ContactInteractionContext* context, ContactTaskPickInfo* todaysArticles)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    int i = 0, uniqueMax = 1000;

    // Pick one at a time til there are 3 unique random tasks
    saSeed(contactInfo->dialogSeed);
    while ((i < MAX_NEWSPAPER_ARTICLES) && uniqueMax)
    {
        if (ContactPickNewspaperTask(player, info, contactInfo, &todaysArticles[i]) && ContactNewspaperTasksUnique(info, contactInfo, todaysArticles, i))
            i++;
        else
            uniqueMax--;
    }

    // We failed to generate unique tasks, just get some
    if (!uniqueMax)
    {
        uniqueMax = 1;
        for (i = 0; i < MAX_NEWSPAPER_ARTICLES; i++)
            if (!ContactPickNewspaperTask(player, info, contactInfo, &todaysArticles[i]))
                uniqueMax = 0; // flag an error
        log_printf("storyerrs.log",    "ContactGenerateNewspaperArticles: I couldn't generate unique tasks for player: %s (level: %i,mapid: %i,seed: %i,failsafe?: %i)", player->name, character_GetExperienceLevelExtern(player->pchar), player->map_id, contactInfo->contactIntroSeed, uniqueMax);
    }        
    return uniqueMax;
}

// Used by ContactInteractionPromptNewTasks()
// if the contact is not a newspaper, task indicates what length of task the task is
// if the contact is a newspaper, task indicates which of possible broker tasks was selected
static void ContactTaskShowPrompt(ContactInteractionContext* context, ContactTaskPickInfo* pickInfo, int task)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    const char *detailintro, *yestext, *notext;
    ScriptVarsTable taskvars = {0};
    char accept[64];// = taskLength == TASK_SHORT? "CONTACTLINK_ACCEPTSHORT" : "CONTACTLINK_ACCEPTLONG";
    unsigned int seed = contactInfo->dialogSeed; // task seed will become our dialog seed

    if (task == TASK_HEIST)
        sprintf(accept,"CONTACTLINK_ACCEPT_HEIST");
    else if (CONTACT_IS_PVPCONTACT(contactInfo->contact->def))
    {
        if (task)
            sprintf(accept,"CONTACTLINK_ACCEPT_PVPMISSION%d",task);
        else
            sprintf(accept,"CONTACTLINK_ACCEPT_PVPPATROLMISSION");
    }
    else if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
    {
        sprintf(accept,"CONTACTLINK_ACCEPT_NEWSPAPER_TASK_%d",task+1);
        seed += task*NEWS_SEED_OFFSET; // Tweak the seed each time for newspaper tasks
    }
    else if (task==TASK_SHORT)
        sprintf(accept,"CONTACTLINK_ACCEPTSHORT");
    else
        sprintf(accept,"CONTACTLINK_ACCEPTLONG");

    // set up task vars
    if (isStoryarcHandle(&pickInfo->sahandle))
        seed = StoryArcGetSeed(info, &pickInfo->sahandle);

    if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
    {
        ContactTaskPickInfo newspaperArticles[MAX_NEWSPAPER_ARTICLES];
        if (!ContactGenerateNewspaperArticles(context, newspaperArticles))
            return;

        saBuildScriptVarsTable(&taskvars, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, &newspaperArticles[task], 0, seed, false);
    }    
    else
        saBuildScriptVarsTable(&taskvars, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, pickInfo, 0, seed, false);

    if( !TaskForceIsOnArchitect(player) )
        ScriptVarsTablePushVar(&taskvars, "Accept", accept);

    if (task != TASK_HEIST)    
        strcatf(response->dialog, "<a href=%s><tr><td highlight=0>", accept);
    if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
    {
        char * lowerHeadline = strupr(strdup(saUtilTableAndEntityLocalize(pickInfo->taskDef->headline_text, &taskvars, player)));
        strcatf(response->dialog, "<span align=center><font scale=1.25><b>%s</b></font></span><BR>", lowerHeadline);
    }

    TaskAddTitle(pickInfo->taskDef, response->dialog, player, &taskvars);
    detailintro = saUtilTableAndEntityLocalize(pickInfo->taskDef->detail_intro_text, &taskvars, player);
    if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def)) 
        strcatf(response->dialog, "<i>%s</i>", detailintro);
    else
        strcat(response->dialog, detailintro);
    if (task != TASK_HEIST)    
        strcat(response->dialog, "</td></tr></a>");

    if (pickInfo->taskDef->SOLdetail_intro_text) 
        strcpy_s(SAFESTR(response->SOLdialog), saUtilTableAndEntityLocalize(pickInfo->taskDef->SOLdetail_intro_text, &taskvars, player));

    if( CONTACT_IS_PLAYERMISSIONGIVER(contactInfo->contact->def) && player->pchar )
    {
        int adjust = playerDifficulty(player)->levelAdjust; 
        int combatlevel    = CLAMP(player->pchar->iCombatLevel, CLAMP(pickInfo->taskDef->minPlayerLevel-adjust, 0, MAX_PLAYER_LEVEL-1), 
                                                                 CLAMP(pickInfo->taskDef->maxPlayerLevel-adjust, 0, MAX_PLAYER_LEVEL-1) );
        char * levelwarning = localizedPrintf(player, "PCCMissionLevelWarning", pickInfo->taskDef->minPlayerLevel+1, pickInfo->taskDef->maxPlayerLevel+1, adjust, combatlevel+1, combatlevel+adjust+1 );
        strcatf(response->dialog, "</table><br><br><color ArchitectError>%s</color>", levelwarning );
    }

    yestext = saUtilTableAndEntityLocalize(pickInfo->taskDef->yes_text, &taskvars, player);
    if (task == TASK_HEIST)
    {
        AddResponse(response, yestext, CONTACTLINK_ACCEPT_HEIST);
        notext = saUtilTableAndEntityLocalize(pickInfo->taskDef->no_text, &taskvars, player);
        AddResponse(response, notext, CONTACTLINK_DECLINE_HEIST);
    }
    else if (CONTACT_IS_PVPCONTACT(contactInfo->contact->def))
        AddResponse(response, yestext, CONTACTLINK_ACCEPT_PVPPATROLMISSION+task);
    else if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
        AddResponse(response, yestext, CONTACTLINK_ACCEPT_NEWSPAPER_TASK_1+task);
    else
        AddResponse(response, yestext, task == TASK_SHORT? CONTACTLINK_ACCEPTSHORT : CONTACTLINK_ACCEPTLONG);
}


// player has too many tasks right now to work for contact
//   NOTE - only valid if !ContactHasNewTasks
int PlayerTooBusy(ContactInteractionContext* context)
{
    // if player has too many tasks period
    if (eaSize(&context->info->tasks) >= TASKS_PER_PLAYER)
        return 1;

    // otherwise, that wasn't the reason
    return 0;
}

int PlayerHasTooManyStoryarcs(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    int n, max;

    // restrict the player to MaxStoryarcs
    n = eaSize(&info->storyArcs);
    max = StoryArcMaxStoryarcs(info);
    if (n >= max)
        return 1;

    return 0;
}


// if player levels farther, he will be able to get more tasks
//   NOTE - only valid if !ContactHasNewTasks
int PlayerNeedsLevelForTask(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    ContactTaskPickInfo shortTask;
    ContactTaskPickInfo longTask;

    // if player was limited by their level
    ContactPickTask(player, info, contactInfo, TASK_SHORT, &shortTask, true);
    ContactPickTask(player, info, contactInfo, TASK_LONG, &longTask, true);
    if (shortTask.minLevel && longTask.minLevel)
        return MIN(shortTask.minLevel, longTask.minLevel);
    else
        return MAX(shortTask.minLevel, longTask.minLevel);
}

// if player levels farther, he will be able to get more tasks
//   NOTE - only valid if !ContactHasNewTasks
static const char *PlayerNeedsRequiresForTask(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    ContactTaskPickInfo shortTask;
    ContactTaskPickInfo longTask;

    // if player was limited by their level
    ContactPickTask(player, info, contactInfo, TASK_SHORT, &shortTask, true);
    ContactPickTask(player, info, contactInfo, TASK_LONG, &longTask, true);

    if (shortTask.requiresFailed)
    {
        return shortTask.requiresFailed;
    } else if (longTask.requiresFailed) {
        return longTask.requiresFailed;
    } else {
        return NULL;
    }
}


// if player levels farther, he will be able to get another intro
//    NOTE - only valid if !ContactTimeForNewContact
int PlayerNeedsLevelForIntro(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    int needLevel = 0;

    ContactTimeForNewContact(player, contactInfo, 1, &needLevel);
    return needLevel;
}

// Returns true if the player has met the requirements to receive a heist
int ContactShouldGiveHeist(StoryContactInfo *contactInfo)
{
    return contactInfo && CONTACT_IS_BROKER(contactInfo->contact->def) && (contactInfo->contactPoints >= contactInfo->contact->def->heistCP);
}

// Contact has no regular tasks to offer at any point
int ContactHasOnlyStoryArcTasks(const ContactDef* contactdef)
{
    // Just check for non-unique tasks
    int i , n = eaSize(&contactdef->tasks);
    for (i = 0; i < n; i++)
        if (!contactdef->tasks[i]->deprecated && !TASK_IS_UNIQUE(contactdef->tasks[i]))
            return 0;
    return 1;
}

int ContactHasNewTasksForPlayer(Entity *pPlayer, StoryContactInfo *contactInfo, StoryInfo *pInfo)
{
    ContactTaskPickInfo shortTask;
    ContactTaskPickInfo longTask;
    StoryTaskInfo* task;

    task = ContactGetAssignedTask(pPlayer->db_id, contactInfo, pInfo);
    if (task)
        return 0;

    if( CONTACT_IS_PLAYERMISSIONGIVER(contactInfo->contact->def) )    
        return playerCreatedStoryarc_GetTask( pPlayer, pInfo, contactInfo, &shortTask );

    ContactPickTask(pPlayer, pInfo, contactInfo, TASK_SHORT, &shortTask, true);
    ContactPickTask(pPlayer, pInfo, contactInfo, TASK_LONG, &longTask, true);

    return shortTask.sahandle.context || longTask.sahandle.context;
}

// contact has tasks to offer the player right now
int ContactHasNewTasks(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)

    return ContactHasNewTasksForPlayer(player, contactInfo, info);
}

/* Function ContactInteractionPromptNewTasks()
*    Adds new response options for the player to go on tasks if tasks are available
*    to the player.
*    if the contact is a broker, index determines which villain group mission to show
*/
void ContactInteractionPromptNewTasks(ContactInteractionContext* context, int index, const char *forcedMissionName)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    ContactTaskPickInfo shortTask={0};
    ContactTaskPickInfo longTask={0};
    int hasNewTasks;
    StoryTaskInfo* task;
    ContactTaskPickInfo newspaperArticles[MAX_NEWSPAPER_ARTICLES];
    //char othertext[LEN_CONTACT_DIALOG];

    memset(info->savedForcedMissionName, 0, FORCED_MISSION_NAME_LENGTH);

    // show clue intro if we have one
    ContactInteractionPromptClue(context);

    // if we have a task open current, don't prompt for any more
    task = ContactGetAssignedTask(player->db_id, contactInfo, info);
    if (!task)
    {
        // otherwise, picks tasks and show intro text for them
        if (forcedMissionName)
        {
            ContactPickSpecificTask(player, info, contactInfo, forcedMissionName, &shortTask);
            strcpy_s(info->savedForcedMissionName, FORCED_MISSION_NAME_LENGTH, forcedMissionName);
            memset(&longTask,0,sizeof(longTask));
        }
        else if (index == TASK_HEIST)
        {
            ContactPickHeistTask(player, info, contactInfo, &shortTask);
            memset(&longTask,0,sizeof(longTask));
        }
        else if (CONTACT_IS_PVPCONTACT(contactInfo->contact->def))
        {
            ContactPickPvPTask(player, info, contactInfo, index, &shortTask);
            memset(&longTask,0,sizeof(longTask));
        }
        else if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
        {
            if (ContactGenerateNewspaperArticles(context, newspaperArticles))
                shortTask = newspaperArticles[index];
            else
                memset(&shortTask,0,sizeof(shortTask));
            memset(&longTask,0,sizeof(longTask));
        }
        else if (CONTACT_IS_SGCONTACT(contactInfo->contact->def))
        {
            ContactPickTask(player, info, contactInfo, TASK_SUPERGROUP, &shortTask, false);
            memset(&longTask,0,sizeof(longTask));
        }
        else if ( CONTACT_IS_PLAYERMISSIONGIVER(contactInfo->contact->def))
        {
            playerCreatedStoryarc_GetTask( player, info, contactInfo, &shortTask );
            memset(&longTask,0,sizeof(longTask));
        }
        else
        {
            ContactPickTask(player, info, contactInfo, TASK_SHORT, &shortTask, false);
            ContactPickTask(player, info, contactInfo, TASK_LONG, &longTask, false);
            ContactHideNonUrgentTask(&shortTask, &longTask);
        }

        hasNewTasks = shortTask.sahandle.context || longTask.sahandle.context || shortTask.sahandle.bPlayerCreated; 
        if(!hasNewTasks)
        {
            if (shortTask.requiresFailed)
            {
                strcat(response->dialog, saUtilLocalize(context->player, shortTask.requiresFailed));
                strcat(response->dialog, "<BR>");
            } else if (longTask.requiresFailed) {
                strcat(response->dialog, saUtilLocalize(context->player, longTask.requiresFailed));
                strcat(response->dialog, "<BR>");
            } else {
                strcat(response->dialog, saUtilLocalize(context->player,"NothingToDo"));
                strcat(response->dialog, "<BR>");
            }

            // make sure to reseed here in case we need a different seed to get on story
            if (!CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
                contactInfo->dialogSeed = time(NULL);

            // Reset contactpoints if we can't find tasks so that contact doesn't become unusable
            if(index == TASK_HEIST)
            {
                contactInfo->contactPoints = 0;
                ContactMarkDirty(player, info, contactInfo);
                log_printf("contactdialog.log", "ContactInteractionPromptNewTasks couldn't find a heist task to give.");
            }
        }
        else
        {
            // Don't create a link for heists
            if (index != TASK_HEIST)
                strcat(response->dialog, "<linkhoverbg #118866aa><link white><linkhover white><table>");
            if (shortTask.sahandle.context || shortTask.sahandle.bPlayerCreated )
            {
                if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def) || CONTACT_IS_PVPCONTACT(contactInfo->contact->def) || index == TASK_HEIST)
                    ContactTaskShowPrompt(context, &shortTask, index);
                else
                    ContactTaskShowPrompt(context, &shortTask, TASK_SHORT);
            }
            if ( (shortTask.sahandle.context || shortTask.sahandle.bPlayerCreated ) &&  
                 (longTask.sahandle.context || longTask.sahandle.bPlayerCreated ) )
            {
                strcat(response->dialog, "<tr><td align=center>");
                strcat(response->dialog, saUtilLocalize(context->player,"TaskDivider"));
                strcat(response->dialog, "</td></tr>");
            }
            if (longTask.sahandle.context || longTask.sahandle.bPlayerCreated ) 
            {
                ContactTaskShowPrompt(context, &longTask, TASK_LONG);
            }
            if (index != TASK_HEIST)
                strcat(response->dialog, "</table>");
        }
    }


    if (index != TASK_HEIST && !CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
    {
        if (contactInfo->contact->def->taskDeclineOverride)
            AddResponse(response, saUtilScriptLocalize(contactInfo->contact->def->taskDeclineOverride), CONTACTLINK_MAIN);
        else
            AddResponse(response, saUtilLocalize(context->player, "TalkAboutElse"), CONTACTLINK_MAIN);
    }
}


/**************************************************************************************************/
/**************************************************************************************************/

void ContactInteractionShowTaskDetail(ContactInteractionContext* context, int taskLength)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    ContactTaskPickInfo pickInfo;

    ContactPickTask(player, info, contactInfo, taskLength, &pickInfo, true);
}

void ContactInteractionAcceptTask(ContactInteractionContext* context, int taskLength)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    ContactTaskPickInfo pickInfo={0};
    const char *sendoff;
    int limitLevel;
    ContactTaskPickInfo newspaperArticles[MAX_NEWSPAPER_ARTICLES];

    // only issue the task if appropriate
    ScriptVarsTable taskvars = {0};
    unsigned int seed = contactInfo->dialogSeed;    // task seed will become our dialog seed
    int taskAddResult;
    int villainGroup;

    if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def) && taskLength!=TASK_HEIST)
    {
        if (!ContactGenerateNewspaperArticles(context, newspaperArticles))
            return;
        seed += taskLength*NEWS_SEED_OFFSET; // Tweak the seed each time for newspaper tasks
        villainGroup = newspaperArticles[taskLength].villainGroup;
    } else 
        villainGroup = 0;

    if (info->savedForcedMissionName[0])
    {
        ContactPickSpecificTask(player, info, contactInfo, info->savedForcedMissionName, &pickInfo);
    }
    else if (taskLength == TASK_HEIST)
    {
        ContactPickHeistTask(player, info, contactInfo, &pickInfo);
        contactInfo->contactPoints = 0;
        ContactDetermineRelationship(contactInfo);
        ContactMarkDirty(player, info, contactInfo);
    }
    else if (CONTACT_IS_PVPCONTACT(contactInfo->contact->def))
        ContactPickPvPTask(player, info, contactInfo, taskLength, &pickInfo);
    else if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
        pickInfo = newspaperArticles[taskLength];
    else if (CONTACT_IS_SGCONTACT(contactInfo->contact->def))
        ContactPickTask(player, info, contactInfo, TASK_SUPERGROUP, &pickInfo, false);
    else if (CONTACT_IS_PLAYERMISSIONGIVER(contactInfo->contact->def))
    {
        playerCreatedStoryarc_GetTask( player, info, contactInfo, &pickInfo );
    }
    else
        ContactPickTask(player, info, contactInfo, taskLength, &pickInfo, false);

    if (!pickInfo.sahandle.context && !pickInfo.sahandle.bPlayerCreated) 
        return;

    // set up task vars
    if (isStoryarcHandle(&pickInfo.sahandle))
        seed = StoryArcGetSeed(info, &pickInfo.sahandle);

    limitLevel = contactInfo->contact->def->maxPlayerLevel + contactInfo->contact->def->taskForceLevelAdjust;
    if (CONTACT_IS_SGCONTACT(contactInfo->contact->def))
        taskAddResult=!TaskAddToSupergroup(player, &pickInfo.sahandle, seed, limitLevel, villainGroup);
    else
        taskAddResult=!TaskAdd(player, &pickInfo.sahandle, seed, limitLevel, villainGroup);

    if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def) && taskLength != TASK_HEIST)
    {
        contactInfo->brokerHandle = StoryArcGetCurrentBroker(player, info);
        saBuildScriptVarsTable(&taskvars, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, &pickInfo, 0, seed, false);
    }
    else
    {
        StoryInfo * si = StoryInfoFromPlayer(player);
        int n = eaSize(&si->tasks);
        if (n > 0)
            saBuildScriptVarsTable(&taskvars, player, player->db_id, contactInfo->handle, si->tasks[n-1], NULL, NULL, NULL, NULL, 0, 0, false);
        else
            saBuildScriptVarsTable(&taskvars, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, &pickInfo, 0, seed, false);
    }

    TaskAddTitle(pickInfo.taskDef, response->dialog, player, &taskvars);
    sendoff = saUtilTableAndEntityLocalize(pickInfo.taskDef->sendoff_text, &taskvars, player);
    strcat(response->dialog, sendoff);
    strcat(response->dialog, "<BR><BR>");

    if (pickInfo.taskDef->SOLsendoff_text) {
        sendoff = saUtilTableAndEntityLocalize(pickInfo.taskDef->SOLsendoff_text, &taskvars, player);
        strcpy_s(SAFESTR(response->SOLdialog), sendoff);
    }

    // issue the task and reseed the dialog queue
    if (taskAddResult)
    {
        strcpy(response->dialog, saUtilLocalize(context->player,"TaskIssueError"));
        strcat(response->dialog, "<BR>");
    }
    contactInfo->dialogSeed = (unsigned int)time(NULL);

    if (info->taskforce) 
        TaskForceSelectTask(player);

    // show the rest of the front page
    ContactFrontPage(context, 0, 0);
}

// wrap up a task if one is being completed.
// show thanks text, give rewards, etc.
int ContactInteractionTaskCompletion(ContactInteractionContext* context)
{
    int returnMe = 0;
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
        StoryTaskInfo* task;

    // see if we have a task that has been completed
    task = ContactGetAssignedTask(player->db_id, contactInfo, info);
    if (!task || !TaskCompletedAndIncremented(task) || TASK_IS_NORETURN(task->def))
        return 0;

    // now we have a completed task, give reward and then close
    returnMe = TaskRewardCompletion(player, task, contactInfo, response);
    TaskClose(player, contactInfo, task, response);
    return returnMe;
}

// show busy text if we have an incomplete task
int ContactInteractionShowBusy(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
        StoryTaskInfo* task;

    // see if we have a task that has been completed
    task = ContactGetAssignedTask(player->db_id, contactInfo, info);
    if (!task)
        return 0;

    if (!TaskCompletedAndIncremented(task)) // show busy text
    {
        ScriptVarsTable taskvars = {0};
        const char* busy_text;

        saBuildScriptVarsTable(&taskvars, player, player->db_id, contactInfo->handle, task, NULL, NULL, NULL, NULL, 0, 0, false);

        if (task->def->busy_text[0] == '[')
        {
            // This is a dialog tree hook, entering special case code here
            Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
            char *busyCopy = strdup(task->def->busy_text + 1);
            char *dialogName = strtok(busyCopy, ":");
            char *pageName = strtok(0, "]"); // note that there may be more characters after the ] that are ignored
            if (dialogName && pageName) // otherwise this isn't a valid dialog tree hook, continue with normal contact
            {
                const DialogDef *dialog = DialogDef_FindDefFromContact(contactInfo->contact->def, dialogName);
                int hash = hashStringInsensitive(pageName);
                memset(response, 0, sizeof(ContactResponse)); // reset the response
                
                if (!dialog)
                    dialog = DialogDef_FindDefFromStoryTask(task->def, dialogName);
                
                if (DialogDef_FindAndHandlePage(player, target, dialog, hash, 0, response, contactInfo->handle, &taskvars))
                {
                    free(busyCopy);
                    return 1;
                }
            }
            free(busyCopy);
        }

        TaskAddTitle(task->def, response->dialog, player, &taskvars);
        busy_text = saUtilTableAndEntityLocalize(task->def->busy_text, &taskvars, player);
        strcat(response->dialog, busy_text);
        strcat(response->dialog, "<BR><BR>");
        if (task->def->SOLbusy_text) {
            busy_text = saUtilTableAndEntityLocalize(task->def->SOLbusy_text, &taskvars, player);
            strcpy_s(SAFESTR(response->SOLdialog), busy_text);
        }

        // check to see if mission can be auto-completed
        if (TaskCanBeAutoCompleted(context->player, task))
        {
            // add auto-complete option here
            AddResponse(response, saUtilLocalize(context->player, "MissionStuckRetry_Option"), CONTACTLINK_AUTO_COMPLETE);        
        }

        if (TaskCanBeAbandoned(context->player, task) && !CONTACT_IS_PLAYERMISSIONGIVER(contactInfo->contact->def))
        {
            AddResponse(response, saUtilLocalize(context->player, "MissionAbandon_Option"), CONTACTLINK_ABANDON);
        }
    }

    return 0;
}

#define ENT_MISSION_STUCK_TOKEN "CompleteStuckMission"
#define ENT_MISSION_STUCK_RETRY_DELAY (60*60*24*3) // 3 days
#define ENT_MISSION_STUCK_BETA_SHARD_RETRY_DELAY (60*10)    // 10 minutes for beta shard

// get confirmation for auto-completing a task
void ContactInteractionAutoComplete(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    StoryTaskInfo* task;
    RewardToken *t = getRewardToken(player, ENT_MISSION_STUCK_TOKEN);

    // see if we have a task 
    task = ContactGetAssignedTask(player->db_id, contactInfo, info);
    if (!task)
        return;

    if (TaskCanBeAutoCompleted(player, task))
    {
        U32 retrydelay = ENT_MISSION_STUCK_RETRY_DELAY;

        if( cfg_IsBetaShard() )
            retrydelay = ENT_MISSION_STUCK_BETA_SHARD_RETRY_DELAY;

        if( t && dbSecondsSince2000() < retrydelay + t->timer ) 
        {
            char datestr[256];
            timerMakeDateStringFromSecondsSince2000(datestr, retrydelay + t->timer);
            strcat(response->dialog, localizedPrintf(player, "MissionStuckRetry_TooSoon", datestr));
        }
        else if (character_IsSidekick(player->pchar,1) || character_IsExemplar(player->pchar,1) ||
                    character_CalcExperienceLevel(player->pchar) != player->pchar->iCombatLevel)
        {
            strcat(response->dialog, localizedPrintf(player, "MissionStuckRetry_Sidekick"));
        } 
        else
        {
            if( cfg_IsBetaShard() )
                strcat(response->dialog, localizedPrintf(player, "MissionStuckRetry_ConfirmBeta"));
            else
                strcat(response->dialog, localizedPrintf(player, "MissionStuckRetry_Confirm"));

            AddResponse(response, saUtilLocalize(player, "MissionStuckRetry_ConfirmYes"), CONTACTLINK_AUTO_COMPLETE_CONFIRM);        
            AddResponse(response, saUtilLocalize(player, "MissionStuckRetry_ConfirmNo"), CONTACTLINK_MAIN);        
        }
    } else {
        strcat(response->dialog, localizedPrintf(player, "MissionStuckRetry_UnableToAbort"));
    }
}


// task auto-complete is confirmed, complete current task
void ContactInteractionAutoCompleteConfirm(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    StoryTaskInfo* task;
    RewardToken *t = getRewardToken(player, ENT_MISSION_STUCK_TOKEN);

    // see if we have a task 
    task = ContactGetAssignedTask(player->db_id, contactInfo, info);
    if (!task)
        return;

    if(!devassert(player))
        return;
    
    if (TaskCanBeAutoCompleted(player, task))
    {
        U32 retrydelay = ENT_MISSION_STUCK_RETRY_DELAY;

        if( cfg_IsBetaShard() )
            retrydelay = ENT_MISSION_STUCK_BETA_SHARD_RETRY_DELAY;

        if( t && dbSecondsSince2000() < retrydelay + t->timer ) 
        {
            char datestr[256];
            timerMakeDateStringFromSecondsSince2000(datestr, retrydelay + t->timer);
            strcat(response->dialog, localizedPrintf(player, "MissionStuckRetry_TooSoon", datestr));
        }
        else
        {
            TaskDebugCompleteByTask(player, task, 1);
            giveRewardToken(player, ENT_MISSION_STUCK_TOKEN);
            strcat(response->dialog, localizedPrintf(player, "MissionStuckRetry_Success"));
        }
    } else {
        strcat(response->dialog, localizedPrintf(player, "MissionStuckRetry_UnableToAbort"));
    }
}

void ContactInteractionAbandonTask(ContactInteractionContext *context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    StoryTaskInfo *task = ContactGetAssignedTask(player->db_id, contactInfo, info);
    if (!task)
        return;

    if (!devassert(player))
        return;

    if (TaskCanBeAbandoned(player, task))
    {
        TaskAbandon(player, info, contactInfo, task, 0);
        strcat(response->dialog, localizedPrintf(player, "MissionAbandon_Success"));
    } else {
        strcat(response->dialog, localizedPrintf(player, "MissionAbandon_Unable"));
    }
}

// get confirmation for dismissing a contact
void ContactInteractionDismiss(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)

    if (ContactCanBeDismissed(player, contactInfo))
    {
        if (contactInfo->contact->def->dismissConfirmTextOverride)
            strcat(response->dialog, localizedPrintf(player, contactInfo->contact->def->dismissConfirmTextOverride));
        else
            strcat(response->dialog, localizedPrintf(player, "DismissContact_Confirm"));

        if (contactInfo->contact->def->dismissConfirmYesOverride)
            AddResponse(response, saUtilLocalize(player, contactInfo->contact->def->dismissConfirmYesOverride), CONTACTLINK_DISMISS_CONFIRM);        
        else
            AddResponse(response, saUtilLocalize(player, "DismissContact_ConfirmYes"), CONTACTLINK_DISMISS_CONFIRM);        

        if (contactInfo->contact->def->dismissConfirmNoOverride)
            AddResponse(response, saUtilLocalize(player, contactInfo->contact->def->dismissConfirmNoOverride), CONTACTLINK_MAIN);        
        else
            AddResponse(response, saUtilLocalize(player, "DismissContact_ConfirmNo"), CONTACTLINK_MAIN);        
    } else {
        if (contactInfo->contact->def->dismissUnableOverride)
            strcat(response->dialog, localizedPrintf(player, contactInfo->contact->def->dismissUnableOverride));
        else
            strcat(response->dialog, localizedPrintf(player, "DismissContact_UnableToDismiss"));
    }

}


// contact dismissal is confirmed
void ContactInteractionDismissConfirm(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)

    if(!devassert(player))
        return;

    if (ContactCanBeDismissed(player, contactInfo))
    {

        if (contactInfo->contact->def->dismissSuccessOverride)
            strcat(response->dialog, localizedPrintf(player, contactInfo->contact->def->dismissSuccessOverride));
        else
            strcat(response->dialog, localizedPrintf(player, "DismissContact_Success"));

        dbLog("ContactDismiss", player, "Player dismissed \"%s\" contact.", contactInfo->contact->def->filename);
        ContactDismiss(player, contactInfo->handle, 0);
    } else {
        if (contactInfo->contact->def->dismissUnableOverride)
            strcat(response->dialog, localizedPrintf(player, contactInfo->contact->def->dismissUnableOverride));
        else
            strcat(response->dialog, localizedPrintf(player, "DismissContact_UnableToDismiss"));
    }
}

void ContactInteractionDisplayWebStore(ContactInteractionContext *context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)

    if (player && player->pl)
    {
        START_PACKET(pak, player, SERVER_WEB_STORE_OPEN_PRODUCT);
        pktSendString(pak, player->pl->webstore_link);
        END_PACKET
    }
}

void ContactInteractionCheckRelationship(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)

    if (ContactDetermineRelationship(contactInfo))
    {
        const char* str = NULL;
        if (contactInfo->contactRelationship == CONTACT_CONFIDANT)
            str = contactInfo->contact->def->relToConfidant;
        else if (contactInfo->contactRelationship == CONTACT_FRIEND)
            str = contactInfo->contact->def->relToFriend;
        str = saUtilTableAndEntityLocalize(str, vars, player);
        if (str)
        {
            strcat(response->dialog, str);
            strcat(response->dialog, "<BR><BR>");
        }
    }
}

// LH: Teleport on complete contacts no longer offer the leave option if you have completed them
void ContactInteractionSayGoodbye(Entity* player, const ContactDef* contactDef, int link, ContactResponse* response, int canBeDismissed)
{
    // overrides because contact def is no longer there (but not NULL)
    if (link == CONTACTLINK_DISMISS_CONFIRM || link == CONTACTLINK_AUTO_COMPLETE_CONFIRM)
    {
        AddResponse(response, saUtilLocalize(player, "Leave"), CONTACTLINK_BYE);
    }
    else
    {
        const char *leaveText;
        const char *dismissOption;

        if (response->responsesFilled)
        {
            AddResponse(response, "", CONTACTLINK_MAIN); // blank line before "Leave"
        }

        if (contactDef->leaveTextOverride)
        {
            leaveText = contactDef->leaveTextOverride;
        }
        else if (CONTACT_IS_TIP(contactDef))
        {
            leaveText = "TipGoodbye";
        }
        else if (CONTACT_IS_NEWSPAPER(contactDef))
        {
            if (contactDef->alliance == ALLIANCE_HERO)
            {
                leaveText = "ClosePoliceRadio"; // Does NOT V_ translate, based off of playerTypeByLocation!
            }
            else
            {
                leaveText = "CloseNewspaper"; // Does NOT V_ translate, based off of playerTypeByLocation!
            }
        }
        else
        {
            leaveText = "Leave";
        }

        if (canBeDismissed && link != CONTACTLINK_DISMISS)
        {
            if (contactDef->dismissOptionOverride)
            {
                dismissOption = contactDef->dismissOptionOverride;
            }
            else
            {
                dismissOption = "DismissContact";
            }

            AddResponseWithRight(response,    saUtilLocalize(player, leaveText), CONTACTLINK_BYE,
                                            saUtilLocalize(player, dismissOption), CONTACTLINK_DISMISS);
        }
        else
        {
            AddResponse(response, saUtilLocalize(player, leaveText), CONTACTLINK_BYE);
        }
    }
}

// we have a clue that needs an intro.  show it
static void ContactInteractionShowClueIntro(ContactResponse* response, ScriptVarsTable* vars, const StoryClue* clue)
{
    const char *introstring = saUtilTableLocalize(clue->introstring, vars);
    if (introstring)
    {
        strcat(response->dialog, introstring);
        strcat(response->dialog, "<BR><BR>");
    }
}

// show intro text to identify a clue if one is available
void ContactInteractionPromptClue(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
        StoryTaskInfo* task;
    StoryArcInfo* storyarc;

    // look at current task
    task = ContactGetAssignedTask(player->db_id, contactInfo, info);
    if (task && task->clueNeedsIntro && task->clueNeedsIntro < eaSize(&task->def->clues))
    {
        ScriptVarsTable taskvars = {0};
        saBuildScriptVarsTable(&taskvars, player, player->db_id, contactInfo->handle, task, NULL, NULL, NULL, NULL, 0, 0, false);

        ContactInteractionShowClueIntro(response, &taskvars, task->def->clues[task->clueNeedsIntro-1]);
    }

    // look at current story arc
    storyarc = StoryArcGetInfo(info, contactInfo);
    if (storyarc && storyarc->clueNeedsIntro && storyarc->clueNeedsIntro < eaSize(&storyarc->def->clues))
    {
        ScriptVarsTable storyvars = {0};
        saBuildScriptVarsTable(&storyvars, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, storyarc, NULL, 0, 0, false);

        ContactInteractionShowClueIntro(response, &storyvars, storyarc->def->clues[storyarc->clueNeedsIntro-1]);
    }
}

// show details of a clue on the identify page
static void ContactInteractionShowClueDetail(ContactResponse* response, ScriptVarsTable* vars, const StoryClue* clue)
{
    const char* name;
    const char* detail;

    // show clue picture
    if (clue->iconfile)
    {
        char pic[200];
        sprintf(pic, "<img src=%s align=left><BR>", ClueIconFile(clue));
        strcat(response->dialog, pic);
    }

    // show clue name
    name = saUtilTableLocalize(clue->displayname, vars);
    if (name)
    {
        strcat(response->dialog, "<B>");
        strcat(response->dialog, name);
        strcat(response->dialog, "</B><BR><BR>");
    }
    detail = saUtilTableLocalize(clue->detailtext, vars);
    if (detail)
    {
        strcat(response->dialog, detail);
        strcat(response->dialog, "<BR><BR>");
    }
}

// show details of each clue that needs to be identified
void ContactInteractionShowClue(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
        StoryTaskInfo* task;
    StoryArcInfo* storyarc;

    // look at current task
    task = ContactGetAssignedTask(player->db_id, contactInfo, info);
    if (task && task->clueNeedsIntro && task->clueNeedsIntro < eaSize(&task->def->clues))
    {
        ScriptVarsTable taskvars = {0};
        saBuildScriptVarsTable(&taskvars, player, player->db_id, contactInfo->handle, task, NULL, NULL, NULL, NULL, 0, 0, false);

        ContactInteractionShowClueDetail(response, &taskvars, task->def->clues[task->clueNeedsIntro-1]);
    }

    // look at current story arc
    storyarc = StoryArcGetInfo(info, contactInfo);
    if (storyarc && storyarc->clueNeedsIntro && storyarc->clueNeedsIntro < eaSize(&storyarc->def->clues))
    {
        ScriptVarsTable storyvars = {0};
        saBuildScriptVarsTable(&storyvars, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, storyarc, NULL, 0, 0, false);

        ContactInteractionShowClueDetail(response, &storyvars, storyarc->def->clues[storyarc->clueNeedsIntro-1]);
    }

    // link back to main page
    AddResponse(response, saUtilLocalize(context->player,"TalkAboutElse"), CONTACTLINK_MAIN);
}

// give a page long description of contact
void ContactInteractionAbout(ContactInteractionContext* context)
{
    int default_desc = 1;
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    
    // show contact icon
    strcatf(response->dialog, ContactHeadshot(contactInfo->contact, player, 0));

    if( CONTACT_IS_PLAYERMISSIONGIVER(contactInfo->contact->def) )
    {
        StoryArcInfo* storyarc = StoryArcGetInfo(info, contactInfo);
        if (storyarc && storyarc->def && storyarc->def->architectAboutContact )
        {
            strcatf(response->dialog, "%s<BR><BR>", saUtilTableAndEntityLocalize(storyarc->def->architectAboutContact, vars, player));
            default_desc = 0;
        }
    }

    if( default_desc )
    {
        strcatf(response->dialog, "<B>%s</B><BR><BR>", saUtilTableAndEntityLocalize(contactInfo->contact->def->displayname, vars, player));
        if (contactInfo->contact->def->profession)
            strcatf(response->dialog, "<B>%s</B><BR><BR>", saUtilTableAndEntityLocalize(contactInfo->contact->def->profession, vars, player));
        if (contactInfo->contact->def->description)
            strcatf(response->dialog, "%s<BR><BR>", saUtilTableAndEntityLocalize(contactInfo->contact->def->description, vars, player));
        if (!contactInfo->interactionTemp) // player doesn't really have this contact
        {
            strcat(response->dialog, saUtilLocalize(context->player,"RelationshipPrompt"));
            strcatf(response->dialog, "<B>%s</B><BR><BR>", ContactRelationshipGetName(player, contactInfo));
        }
    }

    // link back to main page
    AddResponse(response, saUtilLocalize(context->player,"TalkAboutElse"), CONTACTLINK_MAIN);
}

// returns 1 if I was an intro target, and adds appropriate text.
//  may set seenPlayer
int ContactInteractIntroTarget(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    int i, n;
    
    // look for a contact intro task for myself
    n = eaSize(&info->tasks);
    for(i = 0; i < n; i++)
    {
        StoryTaskInfo* task = info->tasks[i];
        if (task->def->type == TASK_CONTACTINTRO)
        {    
            ContactHandle target = 0;
            const char* deliveryTargetName;
            const PNPCDef* pnpc;

            ScriptVarsTable vars = {0};
            saBuildScriptVarsTable(&vars, player, player->db_id, TaskGetContactHandle(info, task), task, NULL, NULL, NULL, NULL, 0, 0, false);

            deliveryTargetName = ScriptVarsTableLookup(&vars, task->def->deliveryTargetNames[0]);
            pnpc = PNPCFindDef(deliveryTargetName);
            if (!pnpc)
            {
                log_printf("storyerrs.log", "ContactInteractIntroTarget: I couldn't find the pnpc %s for task %s (%i,%i,%i,%i)",deliveryTargetName, 
                    task->def->logicalname, SAHANDLE_ARGSPOS(task->sahandle));
                continue;
            }
            target = ContactGetHandle(pnpc->contact);
            if (target == contactInfo->handle)
            {
                // get the issuing contact
                ContactHandle issueHandle = TaskGetContactHandle(info, task);
                StoryContactInfo* issueContact = ContactGetInfo(info, issueHandle);
                if (!issueContact)
                {
                    log_printf("storyerrs.log", "ContactInteractIntroTarget: I couldn't find issuing contact for task %s (%i,%i,%i,%i)",
                        task->def->logicalname, SAHANDLE_ARGSPOS(task->sahandle));
                    continue;
                }

                // close task immediately, add target dialog to response string
                TaskComplete(player, info, task, response, 1);
                if (eaFind(&info->tasks, task) != -1) {
                    TaskRewardCompletion(player, task, issueContact, response);
                    TaskClose(player, issueContact, task, response);
                }
                contactInfo->seenPlayer = 1;    // don't give normal hello string
                return 1;
            }
        }
    }
    return 0;
}

char* ContactHeadshot(Contact* contact, Entity * e, int aboutlink)
{
    static char buf[200];
    if (CONTACT_HAS_HEADSHOT(contact->def))
    {
        buf[0] = 0;
        if (aboutlink && CONTACT_HAS_ABOUTPAGE(contact->def)) 
            strcpy(buf, "<a href=CONTACTLINK_ABOUT>");
        strcatf(buf, getContactHeadshotSMF(contact, e));
        if (aboutlink && CONTACT_HAS_ABOUTPAGE(contact->def)) 
            strcat(buf, "</a>");
    }
    else buf[0] = 0;
    return buf;
}

void ContactInteractionDeclineHeist(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    const char *sendoff;
    ContactTaskPickInfo pickInfo;

    ContactPickHeistTask(player, info, contactInfo, &pickInfo);
    contactInfo->contactPoints = 0;
    ContactDetermineRelationship(contactInfo);
    ContactMarkDirty(player, info, contactInfo);
    
    if (!pickInfo.sahandle.context)
    {
        log_printf("storyerrs.log", "ContactInteractionDeclineHeist: Couldn't find the heist that was prompted\n");
        return;
    }
    
    sendoff = saUtilTableAndEntityLocalize(pickInfo.taskDef->decline_sendoff_text, vars, player);
    strcat(response->dialog, sendoff);
    strcat(response->dialog, "<BR><BR>");

    if (pickInfo.taskDef->SOLdecline_sendoff_text) {
        sendoff = saUtilTableAndEntityLocalize(pickInfo.taskDef->SOLdecline_sendoff_text, vars, player);
        strcpy_s(SAFESTR(response->SOLdialog), sendoff);
    }

    // show the rest of the front page
    ContactFrontPage(context, 0, 0);
}

// Open a dialog for the player with the subcontact
void ContactInteractionSubContact(ContactInteractionContext* context, int subContact)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)

    // Open a new dialog with the sub contact
    if (subContact >= 0 && subContact < eaSize(&contactInfo->contact->def->subContacts))
    {
        ContactHandle handle = ContactGetHandleLoose(contactInfo->contact->def->subContacts[subContact]);
        ContactInteractionEnd(player);
        ContactOpen(player, ENTPOS(player), handle, NULL);
    }
}

// Setup the links to all the sub contacts
void ContactSetupMetaLinks(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    int i;
    for (i = 0; i < eaSize(&contactInfo->contact->def->subContactsDisplay); i++)
    {
        ContactHandle subHandle = ContactGetHandleLoose(contactInfo->contact->def->subContacts[i]);
        int cannotInteract = ContactCannotMetaLink(player, subHandle);
        if(cannotInteract == 0)
            AddResponse(response, saUtilTableAndEntityLocalize(contactInfo->contact->def->subContactsDisplay[i], vars, player), CONTACTLINK_SUBCONTACT1+i);
    }
}

// Setup the PvP Contact FrontPage Links
void ContactSetupPvPLinks(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    if (!ContactGetAssignedTask(player->db_id, contactInfo, info) && ContactHasNewTasks(context))
    {                
        ContactTaskPickInfo patrolTask;
        char option[20];
        int i;

        // Offer the temp stealth task if it hasn't been done
        if (ContactPickPvPTask(player, info, contactInfo, 0, &patrolTask))
            AddResponse(response, saUtilTableAndEntityLocalize("PVPPATROL", vars, player), CONTACTLINK_SHOW_PVPPATROLMISSION);

        // Offer the 4 tasks that will always be offered
        for (i=0; i<4; i++)
        {
            sprintf(option, "PVPMISSION%i", i+1);
            AddResponse(response, saUtilTableAndEntityLocalize(option, vars, player), CONTACTLINK_SHOW_PVPMISSION1+i);
        }
    }
}

// Setup the Newspaper FrontPage Links
void ContactSetupNewspaperLinks(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    if (!ContactGetAssignedTask(player->db_id, contactInfo, info) && !PlayerTooBusy(context))
    {
        // Only show if the player isn't ready for a heist
        ContactHandle broker = StoryArcGetCurrentBroker(player, info);
        if (broker && !ContactShouldGiveHeist(ContactGetInfo(info, broker)))
        {
            // Prompt the possible articles to the player, generation done inside
            int i;
            for (i = 0; i < MAX_NEWSPAPER_ARTICLES; i++)
            {
                ContactInteractionPromptNewTasks(context, i, NULL);
                strcat(response->dialog, "<BR>");
            }
        }
    }
}

// Setup extra links for contacts who are teleport on complete
void ContactSetupTeleportOnCompleteLinks(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    // Heroes: Atlas Park, Villains: Contact Kalinda
    AddResponse(response, saUtilLocalize(player,"TutorialOptionOne"), ENT_IS_VILLAIN(player)?CONTACTLINK_NEWPLAYERTELEPORT_MI_KALINDA : CONTACTLINK_NEWPLAYERTELEPORT_AP);
    // Heroes: Galaxy City, Villains: Contact Burke
    if (ENT_IS_VILLAIN(player)) {
        AddResponse(response, saUtilLocalize(player, "TutorialOptionTwo"), CONTACTLINK_NEWPLAYERTELEPORT_MI_BURKE);
    }
}

// the main first contact page.
// optionally shows busy text if you have an incomplete task
void ContactFrontPage(ContactInteractionContext* context, int showbusy, int hideHeadShot)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    int pastfailsafe = ContactPlayerPastFailsafeLevel(contactInfo, player);
    int activestoryarc = StoryArcGetInfo(info, contactInfo) != NULL;
    int didcontactintro = 0;
    char othertext[LEN_CONTACT_DIALOG];

    // Reward the player for completing previously assigned task.
    if (ContactInteractionTaskCompletion(context))
        return;

    if (showbusy) 
    {
        if (ContactInteractionShowBusy(context))
            return;
    }
    
    ContactInteractionCheckRelationship(context);

    // all the remaining strings should be actually random..
    vars->seed = (unsigned int)time(NULL);

    // decide if I was the target of an intro task
    ContactInteractIntroTarget(context); // may set seenPlayer

    // display first visit string once only(newspapers and sgcontacts don't show the first visit string)
    // only display first visit string if we don't already have something specific to say (can happen from delivery target dialog tree -> mission grant)
    if (response && !response->dialog[0] && !contactInfo->seenPlayer && !CONTACT_IS_SGCONTACT(contactInfo->contact->def) && !CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
    {
        const char *hello;
        if (contactInfo->contact->def->firstVisit && contactInfo->contact->def->firstVisit[0] == '[')
        {
            // This is a dialog tree hook, entering special case code here
            Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
            char *helloCopy = strdup(contactInfo->contact->def->firstVisit + 1);
            char *dialogName = strtok(helloCopy, ":");
            char *pageName = strtok(0, "]"); // note that there may be more characters after the ] that are ignored
            if (dialogName && pageName) // otherwise this isn't a valid dialog tree hook, continue with normal contact
            {
                const DialogDef *dialog = DialogDef_FindDefFromContact(contactInfo->contact->def, dialogName);
                int hash = hashStringInsensitive(pageName);
                memset(response, 0, sizeof(ContactResponse)); // reset the response
                if (DialogDef_FindAndHandlePage(player, target, dialog, hash, 0, response, contactInfo->handle, vars))
                {
                    contactInfo->seenPlayer = 1;
                    free(helloCopy);
                    return;
                }
            }
            free(helloCopy);
        }

        hello = saUtilTableAndEntityLocalize(contactInfo->contact->def->firstVisit, vars, player);
        if (hello)
        {
            strcat(response->dialog, hello);
            strcat(response->dialog, "<BR><BR>");
        }

        hello = saUtilTableAndEntityLocalize(contactInfo->contact->def->SOLfirstVisit, vars, player);
        if (hello)
            strcpy_s(SAFESTR(response->SOLdialog), hello);

        contactInfo->seenPlayer = 1;

        // First time a broker sees a player he must issue the newspaper as long as the player doesn't have it
        if (CONTACT_IS_BROKER(contactInfo->contact->def) && !ContactGetInfo(info, ContactGetNewspaper(player)))
        {
            ContactHandle newspaper = ContactGetNewspaper(player);
            const ContactDef *paperDef = ContactDefinition(newspaper);
            if (paperDef)
            {
                // This is adding the Newspaper vars on top of the the prior context ScriptVarsTable
                saBuildScriptVarsTable(vars, player, player->db_id, newspaper, NULL, NULL, NULL, NULL, NULL, 0, 0, false);

                strcat(response->dialog, saUtilTableAndEntityLocalize(paperDef->introductionSendoff, vars, player));
                strcat(response->dialog, "<BR><BR>");
                ContactAdd(player, info, newspaper, paperDef->filename);
                ContactStatusSendAll(player);        // forces a refresh of all the brokers to update their active status
            }
            else
            {
                // Shouldn't happen, but just in case
                // dont mark the contact so the player still gets the paper when they fix the data
                contactInfo->seenPlayer = 0;
            }
        }
    }
    
    // display zero contact points string
    if (!contactInfo->contactPoints)
    {
        const char* zero = saUtilTableAndEntityLocalize(contactInfo->contact->def->zeroRelPoints, vars, player);
        if (zero)
        {
            strcat(response->dialog, zero);
            strcat(response->dialog, "<BR><BR>");
        }

        zero = saUtilTableAndEntityLocalize(contactInfo->contact->def->SOLzeroRelPoints, vars, player);
        if (zero)
        {
            strcpy_s(SAFESTR(response->SOLdialog), zero); 
        }
    }

    // display never issued task string
    if (!BitFieldCount(contactInfo->taskIssued, TASK_BITFIELD_SIZE, 1))
    {
        const char* never = saUtilTableAndEntityLocalize(contactInfo->contact->def->neverIssuedTask, vars, player);
        if (never)
        {
            strcat(response->dialog, never);
            strcat(response->dialog, "<BR><BR>");
        }
        never = saUtilTableAndEntityLocalize(contactInfo->contact->def->SOLneverIssuedTask, vars, player);
        if (never)
        {
            strcpy_s(SAFESTR(response->SOLdialog), never); 
        }
    }

    if (ContactShouldGiveHeist(contactInfo))
        ContactInteractionPromptNewTasks(context, TASK_HEIST, NULL);
    ContactInteractionPromptClue(context);
    ContactInteractionPromptNewContact(context);

    // display "failsafe" string
    if(!activestoryarc && pastfailsafe && contactInfo->contact->def->failsafeString && ContactTimeForNewContact(player, contactInfo, 1, NULL))
    {
        strcat(response->dialog, saUtilTableAndEntityLocalize(contactInfo->contact->def->failsafeString, vars, player));
        strcat(response->dialog, "<BR>");
        if (contactInfo->contact->def->SOLfailsafeString)
            strcpy_s(SAFESTR(response->SOLdialog), saUtilTableAndEntityLocalize(contactInfo->contact->def->SOLfailsafeString, vars, player));
    }

    // only display the hello string if we didn't have something more specific to say already
    if (!response->dialog[0])
    {
        // give reasons that player can't get a task
        if (!ContactHasNewTasks(context))
        {
            int tasklevel = PlayerNeedsLevelForTask(context);
            int introlevel = PlayerNeedsLevelForIntro(context);
            const ContactDef* lastcontact;
            char levelstr[10];
            const char* why = contactInfo->contact->def->hello;
            const char* SOLwhy = contactInfo->contact->def->SOLhello;
            const char *pRequires = PlayerNeedsRequiresForTask(context);

            if ((PlayerTooBusy(context) || PlayerHasTooManyStoryarcs(context)) && contactInfo->contact->def->playerBusy)
            {
                if (contactInfo->contact->def->playerBusy[0] == '[')
                {
                    // This is a dialog tree hook, entering special case code here
                    Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
                    char *playerBusyCopy = strdup(contactInfo->contact->def->playerBusy + 1);
                    char *dialogName = strtok(playerBusyCopy, ":");
                    char *pageName = strtok(0, "]"); // note that there may be more characters after the ] that are ignored
                    if (dialogName && pageName) // otherwise this isn't a valid dialog tree hook, continue with normal contact
                    {
                        const DialogDef *dialog = DialogDef_FindDefFromContact(contactInfo->contact->def, dialogName);
                        int hash = hashStringInsensitive(pageName);
                        memset(response, 0, sizeof(ContactResponse)); // reset the response
                        if (DialogDef_FindAndHandlePage(player, target, dialog, hash, 0, response, contactInfo->handle, vars))
                        {
                            free(playerBusyCopy);
                            return;
                        }
                    }
                    free(playerBusyCopy);
                }

                why = contactInfo->contact->def->playerBusy;

                if (contactInfo->contact->def->SOLplayerBusy)
                    SOLwhy = contactInfo->contact->def->SOLplayerBusy;
            } 
            else if (tasklevel && contactInfo->contact->def->comeBackLater)
            {
                if (contactInfo->contact->def->comeBackLater[0] == '[')
                {
                    // This is a dialog tree hook, entering special case code here
                    Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
                    char *comeBackLaterCopy = strdup(contactInfo->contact->def->comeBackLater + 1);
                    char *dialogName = strtok(comeBackLaterCopy, ":");
                    char *pageName = strtok(0, "]"); // note that there may be more characters after the ] that are ignored
                    if (dialogName && pageName) // otherwise this isn't a valid dialog tree hook, continue with normal contact
                    {
                        const DialogDef *dialog = DialogDef_FindDefFromContact(contactInfo->contact->def, dialogName);
                        int hash = hashStringInsensitive(pageName);
                        memset(response, 0, sizeof(ContactResponse)); // reset the response
                        if (DialogDef_FindAndHandlePage(player, target, dialog, hash, 0, response, contactInfo->handle, vars))
                        {
                            free(comeBackLaterCopy);
                            return;
                        }
                    }
                    free(comeBackLaterCopy);
                }

                why = contactInfo->contact->def->comeBackLater;
                sprintf(levelstr, "%i", tasklevel);
                ScriptVarsTablePushVar(vars, "Level", levelstr);
                if (contactInfo->contact->def->SOLcomeBackLater)
                    SOLwhy = contactInfo->contact->def->SOLcomeBackLater;
            }
            else if (introlevel && contactInfo->contact->def->comeBackLater)
            {
                if (contactInfo->contact->def->comeBackLater[0] == '[')
                {
                    // This is a dialog tree hook, entering special case code here
                    Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
                    char *comeBackLaterCopy = strdup(contactInfo->contact->def->comeBackLater + 1);
                    char *dialogName = strtok(comeBackLaterCopy, ":");
                    char *pageName = strtok(0, "]"); // note that there may be more characters after the ] that are ignored
                    if (dialogName && pageName) // otherwise this isn't a valid dialog tree hook, continue with normal contact
                    {
                        const DialogDef *dialog = DialogDef_FindDefFromContact(contactInfo->contact->def, dialogName);
                        int hash = hashStringInsensitive(pageName);
                        memset(response, 0, sizeof(ContactResponse)); // reset the response
                        if (DialogDef_FindAndHandlePage(player, target, dialog, hash, 0, response, contactInfo->handle, vars))
                        {
                            free(comeBackLaterCopy);
                            return;
                        }
                    }
                    free(comeBackLaterCopy);
                }

                why = contactInfo->contact->def->comeBackLater;
                sprintf(levelstr, "%i", introlevel);
                ScriptVarsTablePushVar(vars, "Level", levelstr);
                if (contactInfo->contact->def->SOLcomeBackLater)
                    SOLwhy = contactInfo->contact->def->SOLcomeBackLater;
            }
            else if (pRequires != NULL) 
            {
                why = pRequires;
            }
            //3.9.06 This is a temporary thing to catch the three SGs who have too many Items Of Power for their base size
            //because apparently the designers forgot to limit it.
            else if ( CONTACT_IS_SGCONTACT( contactInfo->contact->def ) && (player->supergroup && playerIsNowInAnIllegalBase() ) ) 
            {
                why = saUtilLocalize( 0, "TempMsgForBadBases" );
            }
            //Otherwise assume the contact just has no tasks left
            else if (contactInfo->contact->def->noTasksRemaining) 
            {
                if (contactInfo->contact->def->noTasksRemaining[0] == '[')
                {
                    // This is a dialog tree hook, entering special case code here
                    Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
                    char *noTasksRemainingCopy = strdup(contactInfo->contact->def->noTasksRemaining + 1);
                    char *dialogName = strtok(noTasksRemainingCopy, ":");
                    char *pageName = strtok(0, "]"); // note that there may be more characters after the ] that are ignored
                    if (dialogName && pageName) // otherwise this isn't a valid dialog tree hook, continue with normal contact
                    {
                        const DialogDef *dialog = DialogDef_FindDefFromContact(contactInfo->contact->def, dialogName);
                        int hash = hashStringInsensitive(pageName);
                        memset(response, 0, sizeof(ContactResponse)); // reset the response
                        if (DialogDef_FindAndHandlePage(player, target, dialog, hash, 0, response, contactInfo->handle, vars))
                        {
                            free(noTasksRemainingCopy);
                            return;
                        }
                    }
                    free(noTasksRemainingCopy);
                }

                why = contactInfo->contact->def->noTasksRemaining;
                if (contactInfo->contact->def->SOLnoTasksRemaining)
                    SOLwhy = contactInfo->contact->def->SOLnoTasksRemaining;
            }

            lastcontact = ContactGetLastContact(player, contactInfo->handle, contactInfo->contact->def->stature);
            if (lastcontact) 
            {
                ScriptVarsTablePushVarEx(vars, "LastContact", lastcontact, 'C', 0);
                ScriptVarsTablePushVarEx(vars, "LastContactName", lastcontact, 'C', 0);
            }
            strcat(response->dialog, saUtilTableAndEntityLocalize(why, vars, player));
            strcat(response->dialog, "<BR>");

            if (SOLwhy)
                strcpy_s(SAFESTR(response->SOLdialog), saUtilTableAndEntityLocalize(SOLwhy, vars, player));
        }
        // otherwise, display normal hello string
        else
        {
            const char *hello;
            if (contactInfo->contact->def->hello[0] == '[')
            {
                // This is a dialog tree hook, entering special case code here
                Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
                char *helloCopy = strdup(contactInfo->contact->def->hello + 1);
                char *dialogName = strtok(helloCopy, ":");
                char *pageName = strtok(0, "]"); // note that there may be more characters after the ] that are ignored
                if (dialogName && pageName) // otherwise this isn't a valid dialog tree hook, continue with normal contact
                {
                    const DialogDef *dialog = DialogDef_FindDefFromContact(contactInfo->contact->def, dialogName);
                    int hash = hashStringInsensitive(pageName);
                    memset(response, 0, sizeof(ContactResponse)); // reset the response
                    if (DialogDef_FindAndHandlePage(player, target, dialog, hash, 0, response, contactInfo->handle, vars))
                    {
                        free(helloCopy);
                        return;
                    }
                }
                free(helloCopy);
            }

            hello = saUtilTableAndEntityLocalize(contactInfo->contact->def->hello, vars, player);
            if (hello)
            {
                strcat(response->dialog, hello);
                strcat(response->dialog, "<BR><BR>");
            }

            hello = saUtilTableAndEntityLocalize(contactInfo->contact->def->SOLhello, vars, player);
            if (hello)
            {
                strcpy_s(SAFESTR(response->SOLdialog), hello);
            }
        }
    }

    // prepend the contact icon to whatever I have at this point
    if( !hideHeadShot)
    {
        strcpy(othertext, response->dialog);
        strcpy(response->dialog, ContactHeadshot(contactInfo->contact, player, 1));
        strcat(response->dialog, othertext);
    }

    // done with random strings
    vars->seed = contactInfo->dialogSeed;

    // set up main links
    if (!info->destroyTaskForce)
    {
        // Setup the various mission links
        if (CONTACT_IS_METACONTACT(contactInfo->contact->def))
            ContactSetupMetaLinks(context);
        else if (CONTACT_IS_PVPCONTACT(contactInfo->contact->def))
            ContactSetupPvPLinks(context);
        else if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
            ContactSetupNewspaperLinks(context);
        else if (!CONTACT_IS_BROKER(contactInfo->contact->def) && ContactHasNewTasks(context) && (activestoryarc || !pastfailsafe))
        {
            if(contactInfo->contact->def->askAboutTextOverride)
                AddResponse(response, saUtilScriptLocalize(contactInfo->contact->def->askAboutTextOverride), CONTACTLINK_MISSIONS);
            else if(CONTACT_IS_TIP(contactInfo->contact->def))
            {
                // This functionality is currently unused, all current tips use dialog trees
                AddResponse(response, saUtilLocalize(context->player,"AskMissionsTip"), CONTACTLINK_MISSIONS);
            }
            else
                AddResponse(response, saUtilLocalize(context->player,"AskMissions"), CONTACTLINK_MISSIONS);
        }

        // Handle the rest of the links
        if (CONTACT_IS_TELEPORTONCOMPLETE(contactInfo->contact->def) && contactInfo->contactRelationship >= CONTACT_CONFIDANT)
            ContactSetupTeleportOnCompleteLinks(context);
        if (contactInfo->contact->def->stores.iCnt>0 && !player->storyInfo->interactCell)
            AddResponse(response, saUtilLocalize(context->player,"AskStore"), CONTACTLINK_GOTOSTORE);
        if (CONTACT_IS_TAILOR(contactInfo->contact->def) && contactInfo->contactRelationship >= CONTACT_FRIEND )
            AddResponse(response, saUtilLocalize(context->player,"AskTailor"), CONTACTLINK_GOTOTAILOR);
        if (CONTACT_HAS_ABOUTPAGE(contactInfo->contact->def) && !CONTACT_IS_TIP(contactInfo->contact->def))
            AddResponse(response, saUtilLocalize(context->player,"AskAbout"), CONTACTLINK_ABOUT);
        if (CONTACT_IS_RESPEC(contactInfo->contact->def) && playerCanRespec(player) )
            AddResponse(response, saUtilLocalize(context->player,"GotoRespec"), CONTACTLINK_GOTORESPEC);
    }

    // contacts that don't have tasks then need to reseed after showing the front page..
    // the new seed may be required to reroll for some tasks
    if (!ContactHasNewTasks(context) && !CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
        contactInfo->dialogSeed = (unsigned int)time(NULL);
}

// this teleports the player and closes the dialog
void ContactTeleportNewPlayer(ContactInteractionContext* context, int whichCity)
{
    Vec3 nowhere = {0};
    DoorAnimEnter(context->player, NULL, nowhere, whichCity, 
        NULL, "NewPlayer", XFER_FROMINTRO | XFER_FINDBESTMAP, NULL);
    ContactInteractionClose(context->player);
}

void Contact_OpenTailor( Entity *e )
{
    if( e->pl->ultraTailor )
    {
        character_ShutOffAllTogglePowers(e->pchar);
    }

    START_PACKET( pak, e, SERVER_TAILOR_OPEN );
    pktSendBits( pak, 32, e->pl->freeTailorSessions);
    pktSendBits( pak,  1, e->pl->ultraTailor );
    pktSendBits( pak,  1, getRewardToken( e, TAILOR_VET_REWARD_TOKEN_NAME ) ? 1 : 0 );
    pktSendBits( pak,  1, 0);
    END_PACKET 
}

void Contact_OpenRespec( Entity *e )
{
    if( playerCanRespec(e) )
    {
        e->pl->isRespeccing = 1;
        START_PACKET( pak, e, SERVER_RESPEC_OPEN );
        END_PACKET 
    }
}

// main interaction function.. queried by link, returns a text response and more links
// may put player on mission, etc.
// Returns 0 if the interaction has ended.
int ContactInteraction(Entity* player, ContactHandle contactHandle, int link, ContactResponse* response)
{
    StoryContactInfo* contactInfo;
    ContactInteractionContext context = {0};
    ScriptVarsTable vars = {0};
    StoryInfo* info;
    static StoryContactInfo taskforceInfo;

    if (!response)        
        return 0;
     memset(response, 0, sizeof(ContactResponse));

    // get write access to storyinfo
    info = StoryArcLockInfo(player);

    // verify parameters and that I know this player
    contactInfo= ContactGetInfo(info, contactHandle);
    if (!contactInfo)
    {
        Contact* contact = GetContact(contactHandle);
        // only allow limited links for pre-interaction
        int tfAllowedLink = (link == CONTACTLINK_MAIN || link == CONTACTLINK_HELLO || link == CONTACTLINK_BYE ||
                             link == CONTACTLINK_ABOUT || link == CONTACTLINK_FORMTASKFORCE);

        // if this is a task force contact, put together a temp info struct
        if (contact && 
            ((CONTACT_IS_TASKFORCE(contact->def) && tfAllowedLink) || 
              CONTACT_IS_SGCONTACT(contact->def) || 
              CONTACT_IS_METACONTACT(contact->def)))
        {
            memset(&taskforceInfo, 0, sizeof(taskforceInfo));
            taskforceInfo.contact = contact;
            taskforceInfo.dialogSeed = (U32)time(NULL);
            taskforceInfo.interactionTemp = 1;
            taskforceInfo.handle = contactHandle;
            contactInfo = &taskforceInfo;
        }
        else
        {
            StoryArcUnlockInfo(player);
            return 0;
        }
    }

    // Setup a ContactInteractionContext
    context.link = link;
    context.player = player;
    context.info = info;
    context.contactInfo = contactInfo;
    context.response = response;
    context.vars = &vars;

    saBuildScriptVarsTable(&vars, player, player->db_id, contactHandle, NULL, NULL, NULL, NULL, NULL, 0, contactInfo->dialogSeed, false);

    ContactSetNotify(player, info, contactInfo, 0);
    if( CONTACT_IS_PLAYERMISSIONGIVER(contactInfo->contact->def) )
        response->type = CD_ARCHITECT;


    // handle BYE link before we do task force lock-checking
    if (link == CONTACTLINK_BYE)
    {
        if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
            saUtilEntSays(player, 0, "<em none>");

        StoryArcUnlockInfo(player);
        return 0;
    }

    switch (link)
    {
    case CONTACTLINK_HELLO:
    case CONTACTLINK_MAIN: // copied to DeliveryTaskContact()!
        {
            if (CONTACT_IS_TASKFORCE(contactInfo->contact->def) && contactInfo->interactionTemp)
                ContactTaskForceInteraction(&context);
            else
                ContactFrontPage(&context, 1, CONTACT_IS_PLAYERMISSIONGIVER(contactInfo->contact->def));
        }
        break;

    case CONTACTLINK_MISSIONS: // copied to DeliveryTaskContact()!
        ContactInteractionPromptNewTasks(&context, -1, NULL);
        break;

    case CONTACTLINK_SHOW_PVPPATROLMISSION:
    case CONTACTLINK_SHOW_PVPMISSION1:
    case CONTACTLINK_SHOW_PVPMISSION2:
    case CONTACTLINK_SHOW_PVPMISSION3:
    case CONTACTLINK_SHOW_PVPMISSION4:
        ContactInteractionPromptNewTasks(&context, link-CONTACTLINK_SHOW_PVPPATROLMISSION, NULL);
        break;

    case CONTACTLINK_SUBCONTACT1:
    case CONTACTLINK_SUBCONTACT2:
    case CONTACTLINK_SUBCONTACT3:
    case CONTACTLINK_SUBCONTACT4:
    case CONTACTLINK_SUBCONTACT5:
    case CONTACTLINK_SUBCONTACT6:
    case CONTACTLINK_SUBCONTACT7:
    case CONTACTLINK_SUBCONTACT8:
        StoryArcUnlockInfo(player);
        ContactInteractionSubContact(&context, link-CONTACTLINK_SUBCONTACT1);
        return -1; // don't close dialog because we branched to another contact
        break;

    case CONTACTLINK_ACCEPT_PVPPATROLMISSION:
    case CONTACTLINK_ACCEPT_PVPMISSION1:
    case CONTACTLINK_ACCEPT_PVPMISSION2:
    case CONTACTLINK_ACCEPT_PVPMISSION3:
    case CONTACTLINK_ACCEPT_PVPMISSION4:
        ContactInteractionAcceptTask(&context, link-CONTACTLINK_ACCEPT_PVPPATROLMISSION);
        break;

    case CONTACTLINK_LONGMISSION:
    case CONTACTLINK_ACCEPTLONG:
        ContactInteractionAcceptTask(&context, TASK_LONG);
        break;

    case CONTACTLINK_SHORTMISSION:
    case CONTACTLINK_ACCEPTSHORT:
        ContactInteractionAcceptTask(&context, TASK_SHORT);
        break;

    case CONTACTLINK_ACCEPT_NEWSPAPER_TASK_1:
    case CONTACTLINK_ACCEPT_NEWSPAPER_TASK_2:
    case CONTACTLINK_ACCEPT_NEWSPAPER_TASK_3:
        ContactInteractionAcceptTask(&context, link-CONTACTLINK_ACCEPT_NEWSPAPER_TASK_1);
        break;

    case CONTACTLINK_INTRODUCE: // copied to DeliveryTaskContact()!
        ContactInteractionChooseContact(&context);
        break;

    case CONTACTLINK_INTRODUCE_CONTACT1:
    case CONTACTLINK_INTRODUCE_CONTACT2:
    case CONTACTLINK_INTRODUCE_CONTACT3:
    case CONTACTLINK_INTRODUCE_CONTACT4:
        ContactInteractionIntroduceNewContact(&context);
        break;

    case CONTACTLINK_ACCEPT_CONTACT1:
    case CONTACTLINK_ACCEPT_CONTACT2:
    case CONTACTLINK_ACCEPT_CONTACT3:
    case CONTACTLINK_ACCEPT_CONTACT4:
        ContactInteractionAcceptNewContact(&context);
        break;

    case CONTACTLINK_GOTOSTORE: // copied to DeliveryTaskContact()!
        if (!info->interactCell)
        {
            player->pl->npcStore = -contactHandle; // <0 means contact instead of npc.
            store_SendOpenStoreContact(player, contactInfo->contactRelationship, &contactInfo->contact->def->stores);
        }
        ContactFrontPage(&context, 1, 0);
        break;

    case CONTACTLINK_GOTOTAILOR:
        Contact_OpenTailor( player );
        ContactFrontPage(&context, 1, 0);
        break;

    case CONTACTLINK_GOTORESPEC:
        Contact_OpenRespec( player );
        break;

    case CONTACTLINK_ABOUT: // copied to DeliveryTaskContact()!
        ContactInteractionAbout(&context);
        break;

    case CONTACTLINK_FORMTASKFORCE:
        {
            const StoryArc* arc;

            // taskforce contacts will have the taskforce as their first
            // storyarc
            if (contactInfo->contact->def->storyarcrefs
                && contactInfo->contact->def->storyarcrefs[0])
            {
                arc = StoryArcDefinition(&contactInfo->contact->def->storyarcrefs[0]->sahandle);
                TaskForceSendTimeLimitsToClient(player, arc);
            }

            response->type = CD_CHALLENGE_SETTINGS_TASKFORCE;
        }
        break;

    case CONTACTLINK_IDENTIFYCLUE:
        ContactInteractionShowClue(&context);
        break;

    case CONTACTLINK_NEWPLAYERTELEPORT_MI_KALINDA:
        ContactTeleportNewPlayer(&context, MAPID_MERCY_ISLAND);
        StoryArcUnlockInfo(player);
        return -1; // ignore dialog, we closed shop in ContactTeleportNewPlayer()
    case CONTACTLINK_NEWPLAYERTELEPORT_MI_BURKE:
        player->db_flags |= DBFLAG_ALT_CONTACT;
        ContactTeleportNewPlayer(&context, MAPID_MERCY_ISLAND);
        StoryArcUnlockInfo(player);
        return -1; // ignore dialog, we closed shop in ContactTeleportNewPlayer()
    case CONTACTLINK_NEWPLAYERTELEPORT_AP:
        ContactTeleportNewPlayer(&context, MAPID_ATLAS_PARK);
        StoryArcUnlockInfo(player);
        return -1; // ignore dialog, we closed shop in ContactTeleportNewPlayer()
    case CONTACTLINK_NEWPLAYERTELEPORT_GC:
        ContactTeleportNewPlayer(&context, MAPID_GALAXY_CITY);
        StoryArcUnlockInfo(player);
        return -1; // ignore dialog, we closed shop in ContactTeleportNewPlayer()

    case CONTACTLINK_ACCEPT_HEIST:
        ContactInteractionAcceptTask(&context, TASK_HEIST);
        break;
    case CONTACTLINK_DECLINE_HEIST:
        ContactInteractionDeclineHeist(&context);
        break;

    case CONTACTLINK_AUTO_COMPLETE: // copied to DeliveryTaskContact()!
        ContactInteractionAutoComplete(&context);
        break;
    case CONTACTLINK_AUTO_COMPLETE_CONFIRM:
        ContactInteractionAutoCompleteConfirm(&context);
        break;

    case CONTACTLINK_ABANDON: // copied to DeliveryTaskContact()!
        ContactInteractionAbandonTask(&context);
        break;

    case CONTACTLINK_DISMISS: // copied to DeliveryTaskContact()!
        ContactInteractionDismiss(&context);
        break;
    case CONTACTLINK_DISMISS_CONFIRM:
        ContactInteractionDismissConfirm(&context);
        break;
    case CONTACTLINK_WEBSTORE:
        ContactInteractionDisplayWebStore(&context);
        StoryArcUnlockInfo(player);
        return 0;

    default:
        if (contactInfo->contact->def->dialogDefs)
        {
            Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
            int dialogCount = eaSize(&contactInfo->contact->def->dialogDefs);
            int dialogIndex = 0;
            memset(response, 0, sizeof(ContactResponse)); // reset the response
            for (dialogIndex = 0; dialogIndex < dialogCount; dialogIndex++)
            {
                if (DialogDef_FindAndHandlePage(player, target, contactInfo->contact->def->dialogDefs[dialogIndex], link, 0, response, contactInfo->handle, &vars))
                {
                    break;
                }
            }
            if (dialogIndex == dialogCount)
            {
                strcat(response->dialog, saUtilLocalize(player,"ContactLinkError"));
                strcat(response->dialog, "<BR>");
            }
        }
        else
        {
            strcat(response->dialog, saUtilLocalize(player,"ContactLinkError"));
            strcat(response->dialog, "<BR>");
        }
    }

    if (!CONTACT_IS_TELEPORTONCOMPLETE(contactInfo->contact->def) || !contactInfo->contactRelationship >= CONTACT_CONFIDANT)
    {
        ContactInteractionSayGoodbye(player, contactInfo->contact->def, link, response, ContactCanBeDismissed(player, contactInfo));
    }

    StoryArcUnlockInfo(player);
    return 1;
}

// *********************************************************************************
//  Task force contact dialog
// *********************************************************************************

// returns 1 if all team members of player are on this mapserver, 0 if no team
// TODO:  Remove this for the copy/pasted version from storyarc.c
static int AllTeamMembersPresent(Entity* player)
{
    int i;

    if (!player->teamup_id) return 0;
    for (i = 0; i < player->teamup->members.count; i++)
    {
        if (!entFromDbId(player->teamup->members.ids[i]))
            return 0;
    }
    return 1;
}

// returns 1 if all team members are within the given level requirements
// TODO:  Remove this for the copy/pasted version from storyarc.c
static int AllTeamMembersMeetLevels(Entity* player, int minlevel, int maxlevel)
{
    int i;

    if (!player->teamup_id) return 0;
    for (i = 0; i < player->teamup->members.count; i++)
    {
        Entity* e = entFromDbId(player->teamup->members.ids[i]);

        if (!e) return 0;
        //if (character_GetCombatLevelExtern(e->pchar) > maxlevel) return 0; letting higher level on now, exemplaring them
        if (character_GetExperienceLevelExtern(e->pchar) < minlevel) return 0;
    }
    return 1;
}

static char **WhoFailsRewardCondition(Entity *player, const char **expr, const char *filename)
{
    char **retval = NULL;
    int i;

    if (!player->teamup_id) 
        return retval;

    for (i = 0; i < player->teamup->members.count; i++)
    {
        Entity* e = entFromDbId(player->teamup->members.ids[i]);

        if (!e || !e->pchar) 
            continue;

        if (!chareval_Eval(e->pchar, expr, filename))
        {
            if (retval == NULL) 
            {
                eaCreate(&retval);
            }
            eaPush(&retval, e->name);
        }
    }

    return retval;
}

typedef enum
{
    CANTTF_OK = 0,
    CANTTF_PLAYERLEVELLOW,
    CANTTF_PLAYERLEVELHIGH,
    CANTTF_NEEDTEAM,
    CANTTF_TEAMTOOSMALL,
    CANTTF_NEEDTEAMLEADER,
    CANTTF_NEEDTEAMONMAP,
    CANTTF_BADTEAMLEVEL,
    CANTTF_NEEDREWARDTOKEN,
    CANTTF_NEEDREWARDBADGE,
    CANTTF_FAILREQUIRES,
    CANTTF_FAILTEAMREQUIRES,
} WhyPlayerCantFormTaskForce;

static WhyPlayerCantFormTaskForce PlayerCantFormTaskForce(Entity* player, const ContactDef* contact, int isFlashback, int isArchitect)
{
    int teamsize = 1; 

    if (contact->requiredToken && 0 == getRewardToken(player, contact->requiredToken))
        return CANTTF_NEEDREWARDTOKEN;

    if (contact->requiredBadge && !badge_OwnsBadge(player, contact->requiredBadge))
        return CANTTF_NEEDREWARDBADGE;

    if (contact->interactionRequires && !chareval_Eval(player->pchar, contact->interactionRequires, contact->filename))
        return CANTTF_FAILREQUIRES;

    if (character_GetSecurityLevelExtern(player->pchar) < contact->minPlayerLevel)
        return CANTTF_PLAYERLEVELLOW;

    // team size is not checked for flashbacks
    if (!isFlashback && !isArchitect)
    {
        if (player->teamup)
            teamsize = player->teamup->members.count;
        if ( teamsize < contact->minTaskForceSize && 
            (0==g_encounterForceTeam || g_encounterForceTeam < contact->minTaskForceSize ) ) //debug hack
            return CANTTF_TEAMTOOSMALL;
    }

    if (player->teamup && player->teamup->members.leader != player->db_id)
        return CANTTF_NEEDTEAMLEADER;

    if (player->teamup && !AllTeamMembersPresent(player) && !isArchitect)
        return CANTTF_NEEDTEAMONMAP;

    if (contact && contact->storyarcrefs && contact->storyarcrefs[0] && !isArchitect)
    {
        const StoryArc *arc = StoryArcDefinition(&contact->storyarcrefs[0]->sahandle);
        if (arc && arc->teamRequires && !AllTeamMembersPassRequires(player, arc->teamRequires, arc->filename))
        {
            return CANTTF_FAILTEAMREQUIRES;
        }
    }

    if (player->teamup && !AllTeamMembersMeetLevels(player, contact->minPlayerLevel, contact->maxPlayerLevel) && !isArchitect)
        return CANTTF_BADTEAMLEVEL;

    return CANTTF_OK;
}

static void AddCantFormReason(int reason, const ContactDef *pDef, char *dialog, ScriptVarsTable *vars)
{
    if (reason == CANTTF_NEEDREWARDBADGE)
    {
        const char* dontHaveToken = saUtilTableLocalize(pDef->badgeNeededString, vars);
        if (!dontHaveToken)
            dontHaveToken = textStd("TaskforceGenericNeedRewardBadge");
        if (dontHaveToken)
        {
            strcat(dialog, dontHaveToken);
            strcat(dialog, "<BR><BR>");
        }
    }

    if (reason == CANTTF_FAILREQUIRES)
    {
        const char * dontPassRequires = saUtilTableLocalize(pDef->interactionRequiresFailString, vars);
        if(!dontPassRequires)
            dontPassRequires = textStd("TaskforceGenericFailRequires");
        if (dontPassRequires)
        {
            strcat(dialog, dontPassRequires);
            strcat(dialog, "<BR><BR>");
        }
    }

    if (reason == CANTTF_NEEDREWARDTOKEN)
    {
        const char* dontHaveToken = saUtilTableLocalize(pDef->dontHaveToken, vars);
        if (!dontHaveToken)
            dontHaveToken = textStd("TaskforceGenericNeedRewardToken");
        if (dontHaveToken)
        {
            strcat(dialog, dontHaveToken);
            strcat(dialog, "<BR><BR>");
        }
    }

    if (reason == CANTTF_PLAYERLEVELLOW)
    {
        const char* levelTooLow = saUtilTableLocalize(pDef->levelTooLow, vars);
        if (!levelTooLow)
            levelTooLow = textStd("TaskforceGenericPlayerLevelTooLow");
        if (levelTooLow)
        {
            strcat(dialog, levelTooLow);
            strcat(dialog, "<BR><BR>");
        }
    }

    if (reason == CANTTF_PLAYERLEVELHIGH)
    {
        const char* levelTooHigh = saUtilTableLocalize(pDef->levelTooHigh, vars);
        if (!levelTooHigh)
            levelTooHigh = textStd("TaskforceGenericPlayerLevelTooHigh");
        if (levelTooHigh)
        {
            strcat(dialog, levelTooHigh);
            strcat(dialog, "<BR><BR>");
        }
    }

    if (reason == CANTTF_NEEDTEAM || reason == CANTTF_TEAMTOOSMALL)
    {
        const char* needLargeTeam = saUtilTableLocalize(pDef->needLargeTeam, vars);
        if (!needLargeTeam)
            needLargeTeam = textStd("TaskforceGenericTeamTooSmall");
        if (needLargeTeam)
        {
            strcat(dialog, needLargeTeam);
            strcat(dialog, "<BR><BR>");
        }
    }

    if (reason == CANTTF_NEEDTEAMLEADER)
    {
        const char* needTeamLeader = saUtilTableLocalize(pDef->needTeamLeader, vars);
        if (!needTeamLeader)
            needTeamLeader = textStd("TaskforceGenericNeedTeamLeader");
        if (needTeamLeader)
        {
            strcat(dialog, needTeamLeader);
            strcat(dialog, "<BR><BR>");
        }
    }

    if (reason == CANTTF_NEEDTEAMONMAP)
    {
        const char* needTeamOnMap = saUtilTableLocalize(pDef->needTeamOnMap, vars);
        if (!needTeamOnMap)
            needTeamOnMap = textStd("TaskforceGenericNeedTeamOnMap");
        if (needTeamOnMap)
        {
            strcat(dialog, needTeamOnMap);
            strcat(dialog, "<BR><BR>");
        }
    }

    if (reason == CANTTF_BADTEAMLEVEL)
    {
        const char* badTeamLevel = saUtilTableLocalize(pDef->badTeamLevel, vars);
        if (!badTeamLevel)
            badTeamLevel = textStd("TaskforceGenericBadTeamLevel");
        if (badTeamLevel)
        {
            strcat(dialog, badTeamLevel);
            strcat(dialog, "<BR><BR>");
        }
    }

    if (reason == CANTTF_FAILTEAMREQUIRES)
    {
        const char *dontPassRequires = NULL;
        if (pDef && pDef->storyarcrefs && pDef->storyarcrefs[0])
        {
            const StoryArc *arc = StoryArcDefinition(&pDef->storyarcrefs[0]->sahandle);
            if (arc)
            {
                dontPassRequires = saUtilTableLocalize(arc->teamRequiresFailed, vars);
            }
        }

        if(!dontPassRequires)
        {
            dontPassRequires = textStd("TaskforceGenericFailRequires");
        }

        if (dontPassRequires)
        {
            strcat(dialog, dontPassRequires);
            strcat(dialog, "<BR><BR>");
        }
    }
}

// player is talking to a task force contact before they have formed a task force
void ContactTaskForceInteraction(ContactInteractionContext* context) 
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
        int canMakeTaskForce = 0;
    char othertext[LEN_CONTACT_DIALOG];
    WhyPlayerCantFormTaskForce cantform;

    vars->seed = (unsigned int)time(NULL);    // all these strings are random
    cantform = PlayerCantFormTaskForce(player,contactInfo->contact->def, false, 0);

    AddCantFormReason(cantform, contactInfo->contact->def, response->dialog, vars);

    if (cantform == CANTTF_OK)
    {
        const char* prompt;
        char **requiresList;
        int i, count;

        ScriptVarsTablePushVar(vars, "Accept", "CONTACTLINK_FORMTASKFORCE");
        prompt = saUtilTableAndEntityLocalize(contactInfo->contact->def->taskForceInvite, vars, player); 
        ScriptVarsTablePopVar(vars);
        if (prompt)
        {
            strcat(response->dialog, prompt);
            strcat(response->dialog, "<BR><BR>");
        }

        // checking for requires
        if (contactInfo->contact->def->taskForceRewardRequires != NULL)
        {
            requiresList = WhoFailsRewardCondition(player, contactInfo->contact->def->taskForceRewardRequires, contactInfo->contact->def->filename);

            if (requiresList != NULL)
            {
                strcat(response->dialog, saUtilTableAndEntityLocalize(contactInfo->contact->def->taskForceRewardRequiresText, vars, player));
                strcat(response->dialog, "<BR>");
        
                count = eaSize(&requiresList);
                for (i = 0; i < count; i++)
                {
                    strcat(response->dialog, "&nbsp;&nbsp;&nbsp;"); 
                    strcat(response->dialog, requiresList[i]);
                    strcat(response->dialog, "<BR>");
                }

                eaDestroy(&requiresList);
            }
        }

        canMakeTaskForce = 1;
    }

    // prepend the contact icon to whatever I have at this point
    strcpy(othertext, response->dialog);
    sprintf(response->dialog, ContactHeadshot(contactInfo->contact, player, 1));
    strcat(response->dialog, othertext);

    // set up link to form task force
    if (canMakeTaskForce)
        AddResponse(response, saUtilLocalize(player,"FormTaskForce"), CONTACTLINK_FORMTASKFORCE);
    if (CONTACT_HAS_ABOUTPAGE(contactInfo->contact->def))
        AddResponse(response, saUtilLocalize(player,"AskAbout"), CONTACTLINK_ABOUT);
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void TaskforceSendResponse(Entity *player, ContactResponse *response)
{
    START_PACKET(pak, player, SERVER_CONTACT_DIALOG_OPEN)
    ContactResponseSend(response, pak);
    END_PACKET
}


void ContactInteractionAcceptFlashback(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)
    const StoryTask        *pTaskDef = NULL;
    StoryTaskHandle        sahandle;
    const char            *sendoff;
    ScriptVarsTable        taskvars = {0};
    StoryInfo            *pInfo = NULL;

    memset(&sahandle, 0, sizeof(StoryTaskHandle));

    if (player && player->taskforce && player->taskforce->storyInfo)
    {
        int count = eaSize(&player->taskforce->storyInfo->storyArcs);

        if (count > 0)
        {
            sahandle.context = player->taskforce->storyInfo->storyArcs[0]->sahandle.context;
            sahandle.subhandle = player->taskforce->storyInfo->storyArcs[0]->sahandle.subhandle;
        }
        else 
            return;
    }
    else 
        return;

    // show contact icon
    strcatf(response->dialog, ContactHeadshot(contactInfo->contact, player, 0));

    pTaskDef = TaskParentDefinition(&sahandle);
    if (!pTaskDef)
        return;

    saBuildScriptVarsTable(&taskvars, player, player->db_id, contactInfo->handle, NULL, NULL, &sahandle, NULL, NULL, 0, contactInfo->dialogSeed, false);

    TaskAddTitle(pTaskDef, response->dialog, player, &taskvars);
    sendoff = saUtilTableAndEntityLocalize(pTaskDef->sendoff_text, &taskvars, player); 
    strcat(response->dialog, sendoff);
    strcat(response->dialog, "<BR><BR>");

    if (pTaskDef->SOLsendoff_text) 
    {
        sendoff = saUtilTableAndEntityLocalize(pTaskDef->SOLsendoff_text, &taskvars, player);
        strcpy_s(SAFESTR(response->SOLdialog), sendoff);
    }

    strcat(response->dialog, "<BR><BR>");
    strcat(response->dialog, textStd("ClickTheBigCrystalString"));

}


/////////////////////////////////////////////////////////////////////////////////////
//
void ArchitectStartStatus(Entity *player)
{
    char    *text = NULL;
    int        allRewards = player && player->pl && AccountCanGainAllMissionArchitectRewards(ent_GetProductInventory( player ), player->pl->loyaltyPointsEarned, player->pl->account_inventory.accountStatusFlags);

    // don't do anything if the player isn't in an architect mission
    if (!TaskForceIsOnArchitect(player) )
        return;

    estrCreate(&text);

    if ( allRewards )
        estrConcatCharString(&text, textStd("EnteringArchitectTaskForceWarning"));
    else 
        estrConcatCharString(&text, textStd("EnteringArchitectTaskForceWarningFewerRewards"));


    START_PACKET( pak1, player, SERVER_RECEIVE_DIALOG_WIDTH );
    pktSendString( pak1, text);
    pktSendF32( pak1, 350.0f);
    END_PACKET

    estrDestroy(&text);
}

void TaskForceArchitectStart( Entity * e, char* pchPlayerArc, int mission_id, int test_mode, int arc_flags, int authorid )
{
    StoryInfo* info;
    StoryContactInfo taskforceInfo;
    ContactHandle contactHandle = 0;
    Contact* contact = NULL;

    StoryTaskHandle        sahandle = {0};
    StoryArc*            pArc = NULL;
    StoryTask            *pTaskDef = NULL;

    int                    contact_idx = 0;

    int                    cantform = CANTTF_OK;
    int                    timeLimit = 0;
    int                    i;
    TFArchitectFlags    flags = arc_flags;

    char * contactfile = "Player_Created/MissionArchitectContact.contact";

    contactHandle = ContactGetHandleLoose(contactfile);
    contact = GetContact(contactHandle);

    if (!contact)
        return; // error

    memset(&taskforceInfo, 0, sizeof(taskforceInfo));
    taskforceInfo.contact = contact;
    taskforceInfo.dialogSeed = (U32)time(NULL);
    taskforceInfo.interactionTemp = 1;
    taskforceInfo.handle = contactHandle;

    // get write access to storyinfo
    info = StoryArcLockInfo(e); 

    if ((cantform = PlayerCantFormTaskForce(e, contact->def, 0, 1)) != CANTTF_OK)
    {
        StoryArcUnlockInfo(e);
        return; // error
    }

    // need to place player on team
    teamCreate(e, CONTAINER_TEAMUPS);

    if (test_mode)
    {
        flags |= ARCHITECT_TEST;
    }

    // make a task force
    if (!TaskForceCreate(e, contact->def, 0, 0, 0, 0, 0, 0, 0, 0, 0, pchPlayerArc, mission_id, flags, authorid ) )
    {
        StoryArcUnlockInfo(e);
        return; // failure to create a task force?
    }

    // fixup the contact context now - everything from now on is in taskforce mode
    info = StoryInfoFromPlayer(e);
    ContactAdd(e, info, contactHandle, "TaskForce Architect");

    // save off taskforce size
    e->taskforce->minTeamSize = 1; // minTaskForceSize defaults to 8 if not explicitly set in contact. 

    // log
    LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Taskforce:Architect Task force created. Taskforce ID: %d Arc ID: %d", e->taskforce_id, mission_id);

    if (!StoryArcAddPlayerCreated(e, info, e->taskforce_id))
    {
        StoryArcUnlockInfo(e);
        TaskForceDestroy(e);
        return; // something very wrong
    }
    e->pl->pendingArchitectTickets = 0;
    e->pl->architectMissionsCompleted = 0;
    // give them the mark of the flashback and a status window for the flashback settings
    // apply to entire team
    for (i = 0; i < e->teamup->members.count; i++)
    {
        Entity *teammate = entFromDbId(e->teamup->members.ids[i]);
        if (teammate)
        {
            RewardToken *pToken = NULL;

            // reward token
            if ((pToken = getRewardToken(teammate, "OnArchitect")) == NULL) 
                pToken = giveRewardToken(teammate, "OnArchitect");

            if (pToken)
            {
                pToken->val = 1;
                pToken->timer = timerSecondsSince2000();
            }

            // give them a status window for the flashback settings
            if (teammate != e)
                ArchitectStartStatus(teammate);

        }
    }


    StoryArcUnlockInfo(e);
}


// player has decided to form a task force
void TaskforceAccept(Entity *player, TFParamTimeLimits timeLimits, int limitedLives, TFParamPowers powerLimits,
                        bool debuff, bool buff, bool noEnhancements, bool noInspirations, char *fileID )
{
    ContactResponse response;
    ContactInteractionContext context = {0};
    ScriptVarsTable vars = {0};
    StoryInfo* info;
    StoryContactInfo *contactInfo;
    StoryContactInfo taskforceInfo;
    ContactHandle contactHandle = 0;
    Contact* contact = NULL;

    StoryTaskHandle        sahandle = {0};
    const StoryArc*        pArc = NULL;
    const StoryTask        *pTaskDef = NULL;
    int                    isFlashback = 0;
    int                    contact_idx = 0;

    int                    cantform = CANTTF_OK;
    int                    timeLimit = 0;
    int                    i;

    if (fileID != NULL)
    {
        isFlashback = 1;

        sahandle = StoryArcGetHandle(fileID);
        sahandle.subhandle = 1;
        pArc = StoryArcDefinition(&sahandle);
        contactHandle = StoryArcFindContactHandle(&sahandle);

        pTaskDef = TaskParentDefinition(&sahandle);
        if (!pTaskDef)
            return;
    } 
    else 
    {
        contactHandle = player->storyInfo->interactTarget;
    }

    contact = GetContact(contactHandle);

    if (!contact)
    {
        return; // error
    }

    if (!pArc && !fileID)
    {
        // taskforce contacts will have the taskforce as their first
        // storyarc
        if (contact->def->storyarcrefs && contact->def->storyarcrefs[0])
        {
            pArc = StoryArcDefinition(&contact->def->storyarcrefs[0]->sahandle);
        }
    }

    memset(&taskforceInfo, 0, sizeof(taskforceInfo));
    taskforceInfo.contact = contact;
    taskforceInfo.dialogSeed = (U32)time(NULL);
    taskforceInfo.interactionTemp = 1;
    taskforceInfo.handle = contactHandle;
    contactInfo = &taskforceInfo;

    memset(&response, 0, sizeof(ContactResponse));
 
    // get write access to storyinfo
    info = StoryArcLockInfo(player); 

    // Setup a ContactInteractionContext
    context.link = CONTACTLINK_MAIN;
    context.player = player;
    context.info = info;
    context.contactInfo = contactInfo;
    context.response = &response;
    context.vars = &vars;

    saBuildScriptVarsTable(&vars, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, NULL, 0, contactInfo->dialogSeed, false);

    if ((cantform = PlayerCantFormTaskForce(player, contactInfo->contact->def, isFlashback, 0)) != CANTTF_OK)
    {
        AddCantFormReason(cantform, contactInfo->contact->def, response.dialog, &vars);
        AddResponse(&response, saUtilLocalize(context.player,"Leave"), CONTACTLINK_BYE);

        StoryArcUnlockInfo(player);
        TaskforceSendResponse(player, &response);
        return; // error
    }

    // need to place player on team
    teamCreate(player, CONTAINER_TEAMUPS);

    // Select the right time limit for this story arc given the chosen
    // option.
    timeLimit = TaskForceGetStoryArcTimeLimit(pArc, timeLimits);

    // make a task force
    if (!TaskForceCreate(player, contactInfo->contact->def, limitedLives, timeLimit, true, debuff, powerLimits, 
                            noEnhancements, buff, noInspirations, isFlashback, NULL, 0, 0, 0))
    {
        strcpy(response.dialog, saUtilLocalize(player,"ContactLinkError"));
        StoryArcUnlockInfo(player);
        TaskforceSendResponse(player, &response);
        return; // failure to create a task force?
    }

    // fixup the contact context now - everything from now on is in taskforce mode
    info = context.info = StoryInfoFromPlayer(player);
    ContactAdd(player, info, contactInfo->handle, "TaskForce Accept");
    contactInfo = context.contactInfo = ContactGetInfo(info, contactInfo->handle);
    if (!contactInfo)
    {
        strcpy(response.dialog, saUtilLocalize(player,"ContactLinkError"));
        StoryArcUnlockInfo(player);
        TaskForceDestroy(player);
        TaskforceSendResponse(player, &response);
        return; // something very wrong
    }
    ScriptVarsTableClear(&vars);
    saBuildScriptVarsTable(&vars, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, NULL, 0, contactInfo->dialogSeed, false);

    // save off taskforce size
    // minTaskForceSize defaults to 8 if not explicitly set in contact.
    player->taskforce->minTeamSize = isFlashback ? 1 : contactInfo->contact->def->minTaskForceSize;

    // log
    LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Taskforce:Accept Task force created.");

    if (isFlashback)
    {
        // put the players on the story arc
        if (!AddStoryArc(info, contactInfo, &sahandle))
        {
            strcpy(response.dialog, saUtilLocalize(player,"ContactLinkError"));
            StoryArcUnlockInfo(player);
            TaskForceDestroy(player);
            TaskforceSendResponse(player, &response);
            return; // something very wrong
        }

        // give them the mark of the flashback
        for (i = 0; i < player->teamup->members.count; i++)
        {
            Entity *teammate = entFromDbId(player->teamup->members.ids[i]);
            if (teammate)
            {
                RewardToken *pToken = NULL;

                // reward token
                if ((pToken = getRewardToken(teammate, "OnFlashback")) == NULL) 
                {
                    pToken = giveRewardToken(teammate, "OnFlashback");
                }        
                if (pToken)
                {
                    pToken->val = 1;
                    pToken->timer = timerSecondsSince2000();
                }
            }
        }

        // put the players on the first task
        {
            StoryArcInfo* storyarc = info->storyArcs[0];
            StoryEpisode* episode = storyarc->def->episodes[storyarc->episode];
            StoryTask* pickList[TASKS_PER_CONTACT];
            int numPicks = 0;

            // make level adjustments to taskforce if the storyarc level requirements are different than the
            //        contact level requirements
            if (storyarc->def->maxPlayerLevel <= player->taskforce->exemplar_level)
            {
                player->taskforce->exemplar_level = storyarc->def->maxPlayerLevel - 1;
            }
            player->taskforce->level = CLAMP( character_CalcExperienceLevel(player->pchar)+1, storyarc->def->minPlayerLevel, storyarc->def->maxPlayerLevel);

            numPicks = TaskMatching(player, episode->tasks, storyarc->taskIssued, 0, 0, 0, 0, 
                                        info, pickList, NULL, NULL, 0, NULL);
            if (numPicks)
            {
                int sel = 0; // just pick the first one, since that's the one that's already been sent to the client for the selection window.
                StoryArcGetTaskSubHandle(&storyarc->sahandle, pickList[sel]);
                TaskAdd(player, &storyarc->sahandle, storyarc->seed, 0, 0);
            }
        }

        TaskForceSelectTask(player); 
        ContactInteractionAcceptFlashback(&context);

        // give them a status window for the flashback settings
        // apply to entire team
        for (i = 0; i < player->teamup->members.count; i++)
        {
            Entity *teammate = entFromDbId(player->teamup->members.ids[i]);
            if (teammate)
            {
                if (teammate != player)
                    TaskForceStartStatus(teammate);
            }
        }
    } 
    else 
    {
        // put the players on the story arc
        if (!StoryArcForceAdd(player, info, contactInfo))
        {
            strcpy(response.dialog, saUtilLocalize(player,"ContactLinkError"));
            StoryArcUnlockInfo(player);
            TaskForceDestroy(player);
            TaskforceSendResponse(player, &response);
            return; // something very wrong
        }

        // put the players on the first task
        ContactInteractionAcceptTask(&context, TASK_LONG);
    }

    // add leave response
    AddResponse(&response, saUtilLocalize(context.player,"Leave"), CONTACTLINK_BYE);

    // send response as if a contact
    StoryArcUnlockInfo(player);
    TaskforceSendResponse(player, &response);
}

void ContactInteractionFormTaskForce(ContactInteractionContext* context)
{
    UNPACK_CONTACT_INTERACT_CONTEXT(context)

        if (PlayerCantFormTaskForce(player, contactInfo->contact->def, false, 0))
        {
            strcpy(response->dialog, saUtilLocalize(player,"ContactLinkError"));
            return; // error
        }

        // make a task force
        if (!TaskForceCreate(player, contactInfo->contact->def, TFDEATHS_INFINITE, 0, false, false, 0, false, false, false, false,0,0,0,0))
        {
            strcpy(response->dialog, saUtilLocalize(player,"ContactLinkError"));
            return; // failure to create a task force?
        }

        // fixup the contact context now - everything from now on is in taskforce mode
        info = context->info = StoryInfoFromPlayer(player);
        ContactAdd(player, info, contactInfo->handle, "TaskForce Form");
        contactInfo = context->contactInfo = ContactGetInfo(info, contactInfo->handle);
        if (!contactInfo)
        {
            strcpy(response->dialog, saUtilLocalize(player,"ContactLinkError"));
            TaskForceDestroy(player);
            return; // something very wrong
        }
        ScriptVarsTableClear(vars);
        saBuildScriptVarsTable(vars, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, NULL, 0, contactInfo->dialogSeed, false);

        // log
        LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0,"Taskforce:Accept Task force created.");

        // put the players on the story arc
        if (!StoryArcForceAdd(player, info, contactInfo))
        {
            strcpy(response->dialog, saUtilLocalize(player,"ContactLinkError"));
            TaskForceDestroy(player);
            return; // something very wrong
        }

        // put the players on the first task
        ContactInteractionAcceptTask(context, TASK_LONG);
}


/**************************************************************************************************/
/**************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////

// *********************************************************************************
//  Contact task picking
// *********************************************************************************

// return 0 if this contact can't issue any tasks to the player
int ContactCanIssueTasks(int player_id, StoryInfo* info, StoryContactInfo* contactInfo)
{
    // See if the player has too many tasks assigned to them.
    if (eaSize(&info->tasks) >= TASKS_PER_PLAYER)
        return 0;

    // Has the contact already assigned a task to the player?
    if (ContactGetAssignedTask(player_id, contactInfo, info))
        return 0;

    return 1;
}

// if either of the picked tasks is urgent, hide the other one.  Prefer long urgent tasks
void ContactHideNonUrgentTask(ContactTaskPickInfo* shortpick, ContactTaskPickInfo* longpick)
{
    if (longpick->sahandle.context && TASK_IS_URGENT(longpick->taskDef))
    {
        shortpick->taskDef = NULL;
        shortpick->sahandle.context = shortpick->sahandle.subhandle = 0;
    }
    else if (shortpick->sahandle.context && TASK_IS_URGENT(shortpick->taskDef))
    {
        longpick->taskDef = NULL;
        longpick->sahandle.context = longpick->sahandle.subhandle = 0;
    }
}

int ContactPickPvPTask(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, int index, ContactTaskPickInfo* pickinfo)
{
    StoryTask* pickList[TASKS_PER_CONTACT];
    int numPicks;
    int selected;
    int subhandle;
    PvPType tasktype;

    // make sure we can actually give one
    memset(pickinfo, 0, sizeof(ContactTaskPickInfo));

    if (!ContactCanIssueTasks(player->db_id, info, contactInfo))
        return 0;

    // Look at PvPType enum if you don't understand this section
    // Search for a task demending on what it modifies
    if (index == 0)
        tasktype = PVP_PATROL;
    else if (ENT_IS_VILLAIN(player)) // for villains
    {
        if (index == 1) tasktype = PVP_HERODAMAGE;
        else if (index == 2) tasktype = PVP_HERORESIST;
        else if (index == 3) tasktype = PVP_VILLAINDAMAGE;
        else if (index == 4) tasktype = PVP_VILLAINRESIST;
    }
    else // for heroes
    {
        if (index == 1) tasktype = PVP_VILLAINDAMAGE;
        else if (index == 2) tasktype = PVP_VILLAINRESIST;
        else if (index == 3) tasktype = PVP_HERODAMAGE;
        else if (index == 4) tasktype = PVP_HERORESIST;
    }

    // Find matching tasks
    numPicks = TaskMatching(player, contactInfo->contact->def->tasks, contactInfo->taskIssued,
                            info->uniqueTaskIssued, 0, character_GetExperienceLevelExtern(player->pchar), 
                            &pickinfo->minLevel, info, pickList, NULL, &tasktype, player->supergroup?1:0, 
                            NULL);
    if (!numPicks)
        return 0;

    // Pick a random task from those that matched
    saSeed(contactInfo->dialogSeed);
    selected = saRoll(numPicks);

    subhandle = ContactGetTaskSubHandle(contactInfo->handle, pickList[selected]);
    if (!pickList[selected])
    {
        log_printf("storyerrs.log", "ContactPickPvPTask: I did a task matching and ended up with %i picks.  I selected %i, but ended up with a NULL task at that location", 
            numPicks, selected);
        return 0;
    }
    if (!subhandle)
    {
        log_printf("storyerrs.log", "ContactPickPvPTask: I did a task matching and ended up with %s, but I can't get a subhandle from this task!", pickList[selected]->logicalname);
        return 0;
    }

    // save result
    pickinfo->taskDef = pickList[selected];
    StoryHandleSet( &pickinfo->sahandle, contactInfo->handle, subhandle, 0, 0);
    return 1;
}

// This table determines the probably that a newspaper type is given
typedef struct NewspaperChance
{
    NewspaperType    type;        // Which newspaper type this is
    int                chance;        // The probability this type will be chosen
} NewspaperChance;

// Current probabilities for newspaper type as requested by design
NewspaperChance g_newspaperChances[NEWSPAPER_COUNT] =
{
    {    NEWSPAPER_NOTYPE,        0    },
    {    NEWSPAPER_GETITEM,        33    },
    {    NEWSPAPER_HOSTAGE,        32    },
    {    NEWSPAPER_DEFEAT,        32    },
    {    NEWSPAPER_HEIST,        3    },
};

static NewspaperType newspaperPickRandomType(void)
{
    int randSelection = saRoll(100) + 1;
    int i, curTally = 0;
    
    for (i = 0; i < NEWSPAPER_COUNT; i++)
    {
        int pickValue = curTally + g_newspaperChances[i].chance;
        if (g_newspaperChances[i].chance && randSelection <= pickValue)
            return g_newspaperChances[i].type;
        curTally += g_newspaperChances[i].chance;
    }
    // This should never happen but default to something reasonable
    Errorf("contactDialog.c - Failed to randomly pick a newspaper type, is the table broken?");
    return NEWSPAPER_GETITEM;
}

int ContactPickNewspaperTask(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, ContactTaskPickInfo* pickinfo)
{
    StoryTask* pickList[ TASKS_PER_CONTACT ];
    int i, numPicks, selected, subhandle;
    const int* allGroups = contactInfo->contact->def->knownVillainGroups;
    int level = character_GetExperienceLevelExtern(player->pchar);
    VillainGroupEnum selectedVG;
    NewspaperType taskType = newspaperPickRandomType();

    // make sure we can actually give one
    memset(pickinfo, 0, sizeof(ContactTaskPickInfo));

    // Find matching tasks
    numPicks = TaskMatching(player, contactInfo->contact->def->tasks, 0, info->uniqueTaskIssued,
                                0, level, &pickinfo->minLevel, info, pickList, &taskType, 0, 
                                player->supergroup?sgrp_emptyMounts(player->supergroup):0, NULL);
    if (!numPicks)
        return 0;

    // Pick one randomly, then find a villain group that matchs
    selected = saRoll(numPicks);
    selectedVG = getVillainGroupFromTask(pickList[selected]);

    // If the villain group is random, Generate a list of possible Villain Groups
    if (!stricmp("Random", villainGroupGetName(selectedVG)))
    {
        int* possibleGroups;
        int n = eaiSize(&allGroups);
        eaiCreate(&possibleGroups);
        for (i = 0; i < n; i++)
            if (isVillainGroupAcceptableGivenLevel(player, allGroups[i], level) &&
                isVillainGroupAcceptableGivenMapSet(player, allGroups[i], getMapSetFromTask(pickList[selected]), level) &&
                isVillainGroupAcceptableGivenVillainGroupType(player, allGroups[i], getVillainGroupTypeFromTask(pickList[selected])))
                    eaiPush(&possibleGroups, i);
        
        // Randomly pick a villain group from the possible
        n = eaiSize(&possibleGroups);
        if (n == 0)
        {
            log_printf("storyerrs.log", "ContactPickNewspaperTask: Failed to match newspaper task with any group; level: %i, type: %i, selected: %i", level, taskType, selected);
            eaiDestroy(&possibleGroups);
            return 0;
        }
        selectedVG = allGroups[possibleGroups[saRoll(n)]];
        eaiDestroy(&possibleGroups);
    }

    subhandle = ContactGetTaskSubHandle(contactInfo->handle, pickList[selected]);
    if (!pickList[selected])
    {
        log_printf("storyerrs.log", "ContactPickNewspaperTask: I did a task matching and ended up with %i picks.  I selected %i, but ended up with a NULL task at that location", 
            numPicks, selected);
        return 0;
    }
    if (!subhandle)
    {
        log_printf("storyerrs.log", "ContactPickNewspaperTask: I did a task matching and ended up with %s, but I can't get a subhandle from this task!", pickList[selected]->logicalname);
        return 0;
    }

    // save result
    pickinfo->taskDef = pickList[selected];
    pickinfo->sahandle.context = contactInfo->handle;
    pickinfo->sahandle.subhandle = subhandle;
    pickinfo->villainGroup = selectedVG;
    return 1;
}

// chooses a task to allocate to player given task length.
// returns task in info parameter
int ContactPickHeistTask(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, ContactTaskPickInfo* pickinfo)
{
    StoryTask* pickList[TASKS_PER_CONTACT];
    int numPicks;
    int sel;
    int subhandle;

    // make sure we can actually give one
    memset(pickinfo, 0, sizeof(ContactTaskPickInfo));
    if (!ContactCanIssueTasks(player->db_id, info, contactInfo))
        return 0;

    // Find all heist tasks
    numPicks = TaskMatching(player, contactInfo->contact->def->tasks, contactInfo->taskIssued, info->uniqueTaskIssued, TASK_HEIST, 
                                character_GetExperienceLevelExtern(player->pchar), NULL, info, pickList, NULL, NULL, 
                                player->supergroup?1:0, NULL);

    if (!numPicks) return 0;
    sel = saSeedRoll(contactInfo->dialogSeed, numPicks);
    subhandle = ContactGetTaskSubHandle(contactInfo->handle, pickList[sel]);
    if (!pickList[sel])
    {
        log_printf("storyerrs.log", "ContactPickTask: I did a task matching and ended up with %i picks.  I selected %i, but ended up with a NULL task at that location", 
            numPicks, sel);
        return 0;
    }
    if (!subhandle)
    {
        log_printf("storyerrs.log", "ContactPickTask: I did a task matching and ended up with %s, but I can't get a subhandle from this task!", pickList[sel]->logicalname);
        return 0;
    }

    // save result
    pickinfo->taskDef = pickList[sel];
    pickinfo->sahandle.context = contactInfo->handle;
    pickinfo->sahandle.subhandle = subhandle;
    return 1;
}

// chooses a task to allocate to player given task length.
// returns task in info parameter
int ContactPickTask(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, int taskLength, ContactTaskPickInfo* pickinfo, int justChecking)
{
    StoryTask* pickList[TASKS_PER_CONTACT];
    int numPicks;
    int sel;
    int pastfailsafe = ContactPlayerPastFailsafeLevel(contactInfo, player);

    // make sure we can actually give one
    memset(pickinfo, 0, sizeof(ContactTaskPickInfo));
    if (taskLength != TASK_SHORT && taskLength != TASK_LONG && taskLength != TASK_SUPERGROUP )
    {
        log_printf("storyerrs.log", "ContactPickTask: my parameter was bad: %i, should have been %i or %i (short, long)",
            taskLength, TASK_SHORT, TASK_LONG);
        return 0;
    }

    if (!ContactCanIssueTasks(player->db_id, info, contactInfo))
        return 0;

    // see if we can find any normal tasks first
    numPicks = TaskMatching(player, contactInfo->contact->def->tasks, contactInfo->taskIssued, 
                            info->uniqueTaskIssued, 0, character_GetExperienceLevelExtern(player->pchar), 
                            NULL, info, pickList, NULL, NULL, player->supergroup?1:0,
                            &pickinfo->requiresFailed);

    // allow story arc to replace task if it wants to
    if (!StoryArcGetTask(player, info, contactInfo, taskLength, pickinfo, pastfailsafe, numPicks? 0: 1, &pickinfo->requiresFailed, justChecking))
    {
        // otherwise choose one from contact
        int subhandle;
        char *pFailed;

        if (pastfailsafe) 
            return 0;

        if (PlayerInTaskForceMode(player)) 
            return 0;

        // try to select an urgent task first
        numPicks = TaskMatching(player, contactInfo->contact->def->tasks, contactInfo->taskIssued,
                                info->uniqueTaskIssued, taskLength | TASK_URGENT, character_GetExperienceLevelExtern(player->pchar), 
                                &pickinfo->minLevel, info, pickList, NULL, NULL, player->supergroup?1:0,
                                &pFailed);
        if (!numPicks) // otherwise, a normal task
            numPicks = TaskMatching(player, contactInfo->contact->def->tasks, contactInfo->taskIssued,
                                    info->uniqueTaskIssued, taskLength, character_GetExperienceLevelExtern(player->pchar), 
                                    &pickinfo->minLevel, info, pickList, NULL, NULL, player->supergroup?1:0,
                                    &pFailed);

        if (!numPicks) 
        {
            if (pickinfo->requiresFailed == NULL && pFailed != NULL)
                pickinfo->requiresFailed = pFailed;
            return 0;
        }

        sel = saSeedRoll(contactInfo->dialogSeed + taskLength, numPicks);
        subhandle = ContactGetTaskSubHandle(contactInfo->handle, pickList[sel]);
        if (!pickList[sel])
        {
            log_printf("storyerrs.log", "ContactPickTask: I did a task matching and ended up with %i picks.  I selected %i, but ended up with a NULL task at that location", 
                numPicks, sel);
            return 0;
        }
        if (!subhandle)
        {
            log_printf("storyerrs.log", "ContactPickTask: I did a task matching and ended up with %s, but I can't get a subhandle from this task!", pickList[sel]->logicalname);
            return 0;
        }

        // save result
        pickinfo->taskDef = pickList[sel];
        StoryHandleSet(&pickinfo->sahandle, contactInfo->handle, subhandle, 0, 0);
    }

    return 1;
}

int ContactPickSpecificTask(Entity *player, StoryInfo* info, StoryContactInfo* contactInfo, const char *taskName, ContactTaskPickInfo* pickinfo)
{
    StoryTaskHandle tempTaskHandle;
    const StoryTask *tempTask;
    const StoryTask *parentTask;
    int pastfailsafe = ContactPlayerPastFailsafeLevel(contactInfo, player);
    int subhandle;
    int playerAlliance = ENT_IS_VILLAIN(player) ? SA_ALLIANCE_VILLAIN : SA_ALLIANCE_HERO;
    int player_level = character_GetExperienceLevelExtern(player->pchar);

    // make sure we can actually give one
    memset(pickinfo, 0, sizeof(ContactTaskPickInfo));

    if (!ContactCanIssueTasks(player->db_id, info, contactInfo))
        return 0;

    if (pastfailsafe) 
        return 0;

    if (PlayerInTaskForceMode(player)) 
        return 0;

    subhandle = TaskFromLogicalName(contactInfo->contact->def, taskName);

    if (!subhandle)
        return 0;

    // save result
    StoryHandleSet(&tempTaskHandle, contactInfo->handle, subhandle, 0, 0);
    parentTask = tempTask = ContactTaskDefinition(&tempTaskHandle);

    LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "ContactPickSpecificTask looking for %s, found %s, file %s", taskName, tempTask->logicalname, tempTask->filename);

    // ********************************************
    // These checks are copied from TaskMatching()!
    // ********************************************

    if (tempTask->type == TASK_COMPOUND)
        tempTask = tempTask->children[0];

    // don't issue if task is deprecated
    if (tempTask->deprecated) 
        return 0;

    // can't assign flashback only missions - these are assigned only as converted story arcs
    if (TASK_IS_FLASHBACK_ONLY(tempTask))
        return 0;

    // check type
    if (tempTask->type == TASK_RANDOMENCOUNTER) 
        return 0;
    if (tempTask->type == TASK_RANDOMNPC) 
        return 0;

    // check alliance
    if (tempTask->alliance != SA_ALLIANCE_BOTH && playerAlliance != tempTask->alliance)
        return 0;

    // intro tasks require we don't have contact already
    if (tempTask->type == TASK_CONTACTINTRO)
    {
        const PNPCDef* pnpc = PNPCFindDef(tempTask->deliveryTargetNames[0]);
        ContactHandle target;
        if (!pnpc || !pnpc->contact) 
            return 0;
        target = ContactGetHandle(pnpc->contact);
        if (ContactGetInfo(info, target)) 
            return 0;
    }

    // check unique tasks
    if (TASK_IS_UNIQUE(parentTask))
    {
        if (info->uniqueTaskIssued && BitFieldGet(info->uniqueTaskIssued, UNIQUE_TASK_BITFIELD_SIZE, UniqueTaskGetIndex(parentTask->logicalname)))
            return 0;
    }

    // don't check flags

    // check bitfield to see if you already have the mission
    if (BitFieldGet(contactInfo->taskIssued, (eaSize(&contactInfo->contact->def->tasks) + 7) / 8, StoryHandleToIndex(&tempTaskHandle))) 
        return 0;

    // check player level
    if (parentTask->minPlayerLevel > player_level)
        return 0;
    if (parentTask->maxPlayerLevel < player_level)
        return 0;
    
    // check requires
    if (parentTask->assignRequires && player && player->pchar)
    {
        if (!chareval_Eval(player->pchar, parentTask->assignRequires, parentTask->filename))
        {
            if (pickinfo && pickinfo->requiresFailed == NULL)
            {
                pickinfo->requiresFailed = parentTask->assignRequiresFailed;
            }
            return 0;
        }
    }

    // ************************************************
    // END These checks are copied from TaskMatching()!
    // ************************************************

    // save result
    StoryHandleSet(&pickinfo->sahandle, contactInfo->handle, subhandle, 0, 0);
    pickinfo->taskDef = ContactTaskDefinition(&pickinfo->sahandle);

    return 1;
}
