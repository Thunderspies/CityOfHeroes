/*\
 *
 *    dialogdef.h/c - Copyright 2007 NCSoft
 *        All Rights Reserved
 *        Confidential property of NCSoft
 *
 *     functions to handle static dialog defs, and
 *    verify them on load
 *
 */

#include "entity/entity.h"
#include "storyarcprivate.h"
#include "dialogdef.h"
#include <utilitieslib/components/HashFunctions.h>
#include "entity/character_eval.h"
#include "entity/character_level.h"
#include "storyarc/contactCommon.h"
#include "Reward.h"
#include "storyarc/pnpcCommon.h"
#include "contactDialog.h"
#include "dbcomm/dbcomm.h"
#include "TeamReward.h"
#include "comm_game.h"
#include "entity/character_tick.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>

// *********************************************************************************
//  Load and parse mission defs
// *********************************************************************************

StaticDefineInt ParseDialogPageDefFlags[] =
{
    DEFINE_INT
    { "CompleteDeliveryTask",        DIALOGPAGE_COMPLETEDELIVERYTASK },
    { "CompleteAnyTask",            DIALOGPAGE_COMPLETEANYTASK },
    { "AutoClose",                    DIALOGPAGE_AUTOCLOSE },
    { "NoHeadshot",                    DIALOGPAGE_NOHEADSHOT },
    DEFINE_END
};

StaticDefineInt ParseDialogPageListDefFlags[] =
{
    DEFINE_INT
    { "Random",                        DIALOGPAGELIST_RANDOM },
    { "InOrder",                    DIALOGPAGELIST_INORDER },
    DEFINE_END
};

TokenizerParseInfo ParseDialogAnswerDef[] = {
    { "{",                    TOK_START,        0},
    { "Text",                TOK_STRING(DialogAnswerDef, text,0) },
    { "Page",                TOK_STRING(DialogAnswerDef, page,0) },
    { "Requires",             TOK_STRINGARRAY(DialogAnswerDef, pRequires) },
    { "}",                    TOK_END,            0},
    { "", 0, 0 }
};

TokenizerParseInfo ParseDialogPageDef[] = {
    { "{",                    TOK_START,        0},
    { "Name",                TOK_STRUCTPARAM | TOK_STRING(DialogPageDef, name, 0) },
    { "MissionName",        TOK_STRING(DialogPageDef, missionName, 0) },
    { "ContactFilename",    TOK_STRING(DialogPageDef, contactFilename, 0) },
    { "TeleportDest",        TOK_STRING(DialogPageDef, teleportDest, 0) },
    { "Text",                TOK_STRING(DialogPageDef, text, 0) },
    { "SayOutLoudText",        TOK_STRING(DialogPageDef, sayOutLoudText, 0) },
    { "Requires",             TOK_STRINGARRAY(DialogPageDef, pRequires) },
    { "Objectives",             TOK_STRINGARRAY(DialogPageDef, pObjectives) },
    { "Rewards",             TOK_STRINGARRAY(DialogPageDef, pRewards) },
    { "AddClues",             TOK_STRINGARRAY(DialogPageDef, pAddClues) },
    { "RemoveClues",         TOK_STRINGARRAY(DialogPageDef, pRemoveClues) },
    { "AddTokens",            TOK_STRINGARRAY(DialogPageDef, pAddTokens) },
    { "RemoveTokens",        TOK_STRINGARRAY(DialogPageDef, pRemoveTokens) },
    { "AddTokensToAll",        TOK_STRINGARRAY(DialogPageDef, pAddTokensToAll) },
    { "RemoveTokensFromAll",TOK_STRINGARRAY(DialogPageDef, pRemoveTokensFromAll) },
    { "AbandonContacts",    TOK_STRINGARRAY(DialogPageDef, pAbandonContacts) },
    { "Answer",                TOK_STRUCT(DialogPageDef, pAnswers, ParseDialogAnswerDef) },
    { "Flags",                TOK_FLAGS(DialogPageDef,flags,0), ParseDialogPageDefFlags },

    { "}",                    TOK_END,            0},
    { "", 0, 0 }
};

TokenizerParseInfo ParseDialogPageListDef[] = {
    { "{",                    TOK_START,        0},
    { "Name",                TOK_STRUCTPARAM | TOK_STRING(DialogPageListDef, name, 0) },
    { "List",                TOK_STRINGARRAY(DialogPageListDef, pPages) },
    { "Fallback",            TOK_STRING(DialogPageListDef, fallbackPage,0) },
    { "Requires",             TOK_STRINGARRAY(DialogPageListDef, pRequires) },
    { "Flags",                TOK_FLAGS(DialogPageListDef,flags,0), ParseDialogPageListDefFlags },
    { "}",                    TOK_END,            0},
    { "", 0, 0 }
};

TokenizerParseInfo ParseDialogDef[] = {
    { "{",                    TOK_START,        0},
    { "",                    TOK_CURRENTFILE(DialogDef, filename) },
    { "Name",                TOK_STRUCTPARAM | TOK_STRING(DialogDef, name, 0) },
    { "Filename",            TOK_CURRENTFILE(DialogDef, filename) },
    { "QualifyRequires",    TOK_STRINGARRAY(DialogDef, pQualifyRequires) },
    { "Page",                TOK_STRUCT(DialogDef, pPages, ParseDialogPageDef) },
    { "MissionPage",        TOK_STRUCT(DialogDef, pPages, ParseDialogPageDef) },
    { "ContactPage",        TOK_STRUCT(DialogDef, pPages, ParseDialogPageDef) },
    { "PageList",            TOK_STRUCT(DialogDef, pPageLists, ParseDialogPageListDef) },

    { "}",                    TOK_END,            0},
    { "", 0, 0 }
};

// *********************************************************************************
//  Contact dialog definitions functions
// *********************************************************************************

static char *DialogDefAllocateStringLocalizeWithVars(const char *str, ScriptVarsTable *vars, Entity *e)
{
    if (!str)
        return NULL;

    if (e) 
    {
        return strdup(saUtilTableAndEntityLocalize(str, vars, e));
    } else {
        return NULL;
    }
}

static int DialogDefRandomNumber(int min, int max)
{
    U32 rand = rule30Rand();
    int dif;
    if( min > max )
    {
        NUMBER t=min;
        min=max;
        max=t;
    }
    dif = max - min + 1;
    return min + rand % dif;
}

static const DialogDef *DialogDef_FindDefFromList(const DialogDef **pDialogDefs, const char *name)
{
    const DialogDef *pDialog = NULL;
    int count, i;

    count = eaSize(&pDialogDefs);
    for (i = 0; i < count && !pDialog; i++)
    {
        if (!stricmp(name, pDialogDefs[i]->name))
            pDialog = pDialogDefs[i];
    }

    return pDialog;
}

const DialogDef *DialogDef_FindDefFromStoryTask(const StoryTask *task, const char *name)
{
    if (task == NULL || task->dialogDefs == NULL)
        return NULL;

    return DialogDef_FindDefFromList(task->dialogDefs, name);
}


const DialogDef *DialogDef_FindDefFromSpawnDef(const SpawnDef *pSpawnDef, const char *name)
{
    if (pSpawnDef == NULL || pSpawnDef->dialogDefs == NULL)
        return NULL;

    return DialogDef_FindDefFromList(pSpawnDef->dialogDefs, name);
}

const DialogDef *DialogDef_FindDefFromContact(const ContactDef *pContactDef, const char *name)
{
    if (pContactDef == NULL || pContactDef->dialogDefs == NULL)
        return NULL;

    return DialogDef_FindDefFromList(pContactDef->dialogDefs, name);
}

const DialogPageDef *DialogDef_FindPageFromDef(int hash, const DialogDef *pDialog)
{
    int count, i;
    const DialogPageDef *pPage = NULL;

    if (!pDialog)
        return NULL;

    // look for page in dialog
    count = eaSize(&pDialog->pPages);
    for (i = 0; i < count && !pPage; i++)
    {
        if (hash == hashStringInsensitive(pDialog->pPages[i]->name))
            pPage = pDialog->pPages[i];
    }

    return pPage;
}

const DialogPageListDef *DialogDef_FindPageListFromDef(int hash, const DialogDef *pDialog)
{
    int count, i;
    const DialogPageListDef *pPageList = NULL;

    if (!pDialog)
        return NULL;

    // look for pagelist in dialog
    count = eaSize(&pDialog->pPageLists);
    for (i = 0; i < count && !pPageList; i++)
    {
        if (hash == hashStringInsensitive(pDialog->pPageLists[i]->name))
            pPageList = pDialog->pPageLists[i];
    }

    return pPageList;
}

const DialogPageDef *DialogDef_FindPageFromPageList(Entity *pPlayer, const DialogPageListDef *pPageList, const DialogDef *pDialog, StoryTaskInfo *pTask)
{
    int count, i, hash, first, start;
    const DialogPageDef *pPage = NULL;

    if (!pPlayer || !pPlayer->pchar || !pPageList || !pPageList->pPages)
        return NULL;

    // check pagelist requires
    if (pPageList->pRequires != NULL && !chareval_EvalWithTask(pPlayer->pchar, pPageList->pRequires, pTask, pDialog->filename))
        return NULL;

    count = eaSize(&pPageList->pPages);

    // how are we supposed to pick a page?
    if (pPageList->flags & DIALOGPAGELIST_INORDER)
    {
        for (i = 0; i < count; i++)
        {
            hash = hashStringInsensitive(pPageList->pPages[i]);

            // check page requires
            pPage = DialogDef_FindPageFromDef(hash, pDialog);

            if (pPage && (!pPage->pRequires || chareval_EvalWithTask(pPlayer->pchar, pPage->pRequires, pTask, pDialog->filename)))
                return pPage;

        }
    } else if (pPageList->flags & DIALOGPAGELIST_RANDOM) {
        start = true;
        first = i = DialogDefRandomNumber(0, count - 1);

        while (start || i != first)
        {

            hash = hashStringInsensitive(pPageList->pPages[i]);

            // check page requires
            pPage = DialogDef_FindPageFromDef(hash, pDialog);

            if (pPage && (!pPage->pRequires || chareval_EvalWithTask(pPlayer->pchar, pPage->pRequires, pTask, pDialog->filename)))
                return pPage;

            i++;
            if (i >= count)
                i = 0;
            start = false;
        }
    }

    // haven't found anything yet, try fallback
    if (pPageList->fallbackPage != NULL)
    {
        hash = hashStringInsensitive(pPageList->fallbackPage);
        pPage = DialogDef_FindPageFromDef(hash, pDialog);

        if (pPage && (!pPage->pRequires || chareval_EvalWithTask(pPlayer->pchar, pPage->pRequires, pTask, pDialog->filename)))
            return pPage;
    }

    return NULL;
}



static int DialogDef_DisplayPage(Entity *pPlayer, Entity *pTarget, ContactResponse* response, const DialogPageDef *pPage, 
                               const DialogDef *pDialog, StoryTaskInfo *pTask, int hash, ContactHandle contactHandle, ScriptVarsTable *vars)  
{
    int count, i, taskIndex;
    const DialogPageDef *pAnswerPage = NULL;
    const DialogPageListDef *pPageList = NULL;
    StoryInfo *info;
    bool didILockIt = false;
    char *dialogString;
    char *sayOutLoudString;

    if (!pPage || !pPlayer || !pPlayer->storyInfo || !pPlayer->pchar || !pPlayer->pl)
        return false;

    if (pTarget && ENTTYPE(pTarget) != ENTTYPE_NPC && ENTTYPE(pTarget) != ENTTYPE_CRITTER)
        return false;

    // check page requires
    if (pPage->pRequires != NULL && !chareval_EvalWithTask(pPlayer->pchar, pPage->pRequires, pTask, pDialog->filename))
        return false;

    // This is a blatant hack, but necessary because sometimes this function is called inside of a lock, sometimes not
    if (!StoryArcInfoIsLocked())
    {
        info = StoryArcLockInfo(pPlayer);
        didILockIt = true;
    }
    else
    {
        info = StoryInfoFromPlayer(pPlayer);
    }

    dbLog("DialogTree:Display", pPlayer, "displaying page %s of dialog %s.  contactHandle == %d, pTarget == %s", pPage->name, pDialog->name, contactHandle, pTarget ? pTarget->name : "(NULL)");

    pPlayer->pl->webstore_link[0] = '\0';

    if (pPage->missionName && contactHandle) // This is a MissionPage, do special code
    {
        // Copied from ContactInteraction
        ContactInteractionContext context = {0};
        StoryContactInfo *contactInfo = ContactGetInfo(info, contactHandle);
        ScriptVarsTable vars = {0};
        const ContactDef *contactDef = ContactDefinition(contactHandle);

        if (!contactInfo && !PlayerInTaskForceMode(pPlayer)
            && CONTACT_IS_ISSUEONINTERACT(contactDef)
            && (!contactDef->interactionRequires || chareval_Eval(pPlayer->pchar, contactDef->interactionRequires, contactDef->filename)))
        {
            ContactAdd(pPlayer, info, contactHandle, "Issued on interact in Dialog Tree");
            contactInfo = ContactGetInfo(info, contactHandle);
        }
        
        if (contactInfo)
        {
            pPlayer->storyInfo->interactDialogTree = 0;

            // Setup a ContactInteractionContext
            context.link = hash;
            context.player = pPlayer;
            context.info = info;
            context.contactInfo = contactInfo;
            context.response = response;
            context.vars = &vars;

            saBuildScriptVarsTable(&vars, pPlayer, pPlayer->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, NULL, 0, contactInfo->dialogSeed, false);

            ContactInteractionPromptNewTasks(&context, -1, pPage->missionName);

            if (didILockIt)
            {
                StoryArcUnlockInfo(pPlayer);
            }

            return true;
        }
    }

    // introduce a new contact
    if (pPage->contactFilename)
    {
        ContactHandle newContactHandle = ContactGetHandle(pPage->contactFilename);
        // pPlayer->storyInfo is used here ON PURPOSE,
        // since we can't introduce contacts to the player's taskforce storyInfo.
        // This means we will add contacts to the standard contact list while on TFs!
        ContactAdd(pPlayer, pPlayer->storyInfo, newContactHandle, "Dialog Tree");
        // don't return, keep displaying the page!
    }

    // if on mission, 
    if (OnMissionMap())
    {
        // set any objectives
        if (pPage->pObjectives != NULL)
        {
            count = eaSize(&pPage->pObjectives);
            for (i = 0; i < count; i++)
            {
                MissionObjectiveCompleteEach(pPage->pObjectives[i], true, true);
            }
        }

        // give & remove clues
        if (pPage->pAddClues != NULL)
        {
            count = eaSize(&pPage->pAddClues);
            for (i = 0; i < count; i++)
            {
                if (!MissionAddRemoveKeyClue(pPlayer, pPage->pAddClues[i], true))
                    ClueAddRemoveForCurrentMission(pPlayer, pPage->pAddClues[i], true);
            }
        }
        if (pPage->pRemoveClues != NULL)
        {
            count = eaSize(&pPage->pRemoveClues);
            for (i = 0; i < count; i++)
            {
                if (!MissionAddRemoveKeyClue(pPlayer, pPage->pRemoveClues[i], false))
                    ClueAddRemoveForCurrentMission(pPlayer, pPage->pRemoveClues[i], false);
            }
        }
    }
    else
    {
        int dummy = 0;

        // give & remove clues (can't do key clues!)
        if (pPage->pAddClues != NULL && pTask)
        {
            count = eaSize(&pPage->pAddClues);
            for (i = 0; i < count; i++)
            {
                ClueAddRemove(pPlayer, info, pPage->pAddClues[i], 
                                TaskGetClues(&pTask->sahandle), TaskGetClueStates(info, pTask), 
                                pTask, &dummy, true);
            }
        }
        if (pPage->pRemoveClues != NULL && pTask)
        {
            count = eaSize(&pPage->pRemoveClues);
            for (i = 0; i < count; i++)
            {
                ClueAddRemove(pPlayer, info, pPage->pRemoveClues[i], 
                                TaskGetClues(&pTask->sahandle), TaskGetClueStates(info, pTask), 
                                pTask, &dummy, false);
            }
        }
    }

    // add and remove tokens
    StoryRewardHandleTokens(pPlayer, pPage->pAddTokens, pPage->pAddTokensToAll, pPage->pRemoveTokens, pPage->pRemoveTokensFromAll);

    // abandon all tasks from specified contacts
    for (i = eaSize(&pPage->pAbandonContacts) - 1; i >= 0; i--)
    {
        // pPlayer->storyInfo is used here ON PURPOSE,
        // since it doesn't make sense to abandon missions in a TF's storyInfo.
        // This means we will abandon missions from the standard mission list while on TFs!
        for (taskIndex = eaSize(&pPlayer->storyInfo->tasks) - 1; taskIndex >= 0; taskIndex--)
        {
            StoryContactInfo *contactInfo = TaskGetContact(pPlayer->storyInfo, pPlayer->storyInfo->tasks[taskIndex]);
            if (contactInfo && contactInfo->contact->def)
            {
                char *contactName = strrchr(contactInfo->contact->def->filename, '/');
                if (contactName)
                {
                    contactName++;
                    if (!strnicmp(contactName, pPage->pAbandonContacts[i], strlen(pPage->pAbandonContacts[i])))
                    {
                        TaskAbandon(pPlayer, pPlayer->storyInfo, contactInfo, pPlayer->storyInfo->tasks[taskIndex], 0);
                    }
                }
            }
        }
    }

    // give rewards
    if (pPage->pRewards != NULL)
    {
        count = eaSize(&pPage->pRewards);
        for (i = 0; i < count; i++)
        {
            rewardFindDefAndApplyToEnt(pPlayer, (const char**)eaFromPointerUnsafe(pPage->pRewards[i]), VG_NONE, 
                                        character_GetCombatLevelExtern(pPlayer->pchar), false, 
                                        REWARDSOURCE_SCRIPT, NULL);
        }
    }

    // check special flags
    if (((pPage->flags & DIALOGPAGE_COMPLETEDELIVERYTASK) || (pPage->flags & DIALOGPAGE_COMPLETEANYTASK))
        && (pTarget ? (pTarget->persistentNPCInfo && pTarget->persistentNPCInfo->pnpcdef) : contactHandle))
    {
        int iSize;
        int taskCursor;

        // Cannot rely on the pTask parameter, it can be NULL
        iSize = eaSize(&info->tasks);
        for(taskCursor = 0; taskCursor < iSize; taskCursor++)
        {
            StoryTaskInfo* task;
            const char *targetName = 0;
            const char *deliveryTargetName;
            int deliveryTargetIndex, deliveryTargetCount;
            ContactResponse dummyResponse = {0};
            ScriptVarsTable vars = {0};

            task = info->tasks[taskCursor];

            saBuildScriptVarsTable(&vars, pPlayer, pPlayer->db_id, TaskGetContactHandle(info, task), task, NULL, NULL, NULL, NULL, 0, 0, false);

            if (TASK_DELIVERITEM != task->def->type && !(pPage->flags & DIALOGPAGE_COMPLETEANYTASK))
                continue;
            if (TaskCompleted(task))
                continue;
            if (!task->def->deliveryTargetNames)
                continue;
            deliveryTargetCount = eaSize(&task->def->deliveryTargetNames);
            if (!deliveryTargetCount)
                continue;

            if (pTarget && pTarget->persistentNPCInfo && pTarget->persistentNPCInfo->pnpcdef)
            {
                targetName = pTarget->persistentNPCInfo->pnpcdef->name;
            }
            else
            {
                Contact *contact = GetContact(contactHandle);

                if (contact)
                {
                    if (contact->pnpcParent)
                    {
                        targetName = contact->pnpcParent->name;
                    }
                    else
                    {
                        char filename[1024];
                        const PNPCDef *pnpcDef;
                        char *dotPtr;

                        strcpy_s(filename, 1024, contact->def->filename);
                        dotPtr = strrchr(filename, '.');
                        dotPtr[1] = 'n';
                        dotPtr[2] = 'p';
                        dotPtr[3] = 'c';
                        dotPtr[4] = '\0';

                        pnpcDef = PNPCFindDefFromFilename(filename);

                        if (pnpcDef)
                        {
                            targetName = pnpcDef->name;
                        }
                    }
                }
            }

            if (!targetName)
                continue;

            for (deliveryTargetIndex = 0; deliveryTargetIndex < deliveryTargetCount; deliveryTargetIndex++)
            {
                deliveryTargetName = ScriptVarsTableLookup(&vars, task->def->deliveryTargetNames[deliveryTargetIndex]);
                if (stricmp(targetName, deliveryTargetName) != 0)
                    continue;
                if (((!task->def->deliveryTargetMethods || task->def->deliveryTargetMethods[deliveryTargetIndex] == DELIVERYMETHOD_INPERSONONLY) && pPlayer->storyInfo->interactCell)
                    || (task->def->deliveryTargetMethods && task->def->deliveryTargetMethods[deliveryTargetIndex] == DELIVERYMETHOD_CELLONLY && !pPlayer->storyInfo->interactCell))
                    continue;

                TaskComplete(pPlayer, info, task, &dummyResponse, 1);
                break;
            }
        }
    }

    // do teleporting
    if (pPage->teleportDest)
        character_NamedTeleport(pPlayer->pchar, NULL, pPage->teleportDest);

    if(pPage->flags & DIALOGPAGE_AUTOCLOSE) // Don't actually show the page, we just wanted to make sure we processed all page effects
    {
        response->dialogPageAutoClose = true;

        if (didILockIt)
        {
            StoryArcUnlockInfo(pPlayer);
        }

        return true;
    }

    // put picture in
    if(!(pPage->flags & DIALOGPAGE_NOHEADSHOT))
    {
        if (!pTarget)
        {
            sprintf(response->dialog, ContactHeadshot(GetContact(pPlayer->storyInfo->interactTarget), pPlayer, 1));
        }
        else
        {
            if( contactHandle )
                sprintf(response->dialog, ContactHeadshot(GetContact(contactHandle), pPlayer, 1));
            else
            {
                int found = 0, i, n = eaSize(&g_contactlist.contactdefs);
                for (i = 0; i < n && !found; i++)
                {
                    if(g_contacts[i].entParent == pTarget)
                    {
                        sprintf(response->dialog, ContactHeadshot(&g_contacts[i], pPlayer, 1));
                        found = 1;
                    }
                }        
                if(!found)
                {
                    if( pTarget->doppelganger )
                        sprintf(response->dialog, "<img align=left src=svr:%d>", pTarget->owner );
                    else
                        sprintf(response->dialog, "<img align=left src=npc:%d>", pTarget->npcIndex);
                }
            }
        }
    }

    // put text in
    dialogString = DialogDefAllocateStringLocalizeWithVars(pPage->text, vars, pPlayer);
    if (dialogString)
    {
        strcat(response->dialog, dialogString);
        free(dialogString);
    }

    // say out loud text (if any)
    sayOutLoudString = DialogDefAllocateStringLocalizeWithVars(pPage->sayOutLoudText, vars, pPlayer);
    if (sayOutLoudString)
    {
        saUtilEntSays(pTarget, !pTarget->pchar || pTarget->pchar->iAllyID==kAllyID_Good, sayOutLoudString);
        free(sayOutLoudString);
    }

    // put answers in
    count = eaSize(&pPage->pAnswers);
    for (i = 0; i < count; i++)
    {
        const DialogAnswerDef *pAnswer = pPage->pAnswers[i];
        int hash;

        if (pAnswer)
        {
            // check answer requires
            if (pAnswer->pRequires != NULL)
                if (!chareval_EvalWithTask(pPlayer->pchar, pAnswer->pRequires, pTask, pDialog->filename))
                    continue;

            // calc hash
            if (!stricmp(pAnswer->page, "[Close]")) 
            {
                hash = CONTACTLINK_BYE;
            } 
            else if (!stricmp(pAnswer->page, "[startmission]"))
            {
                // start the mission for this player
                hash = CONTACTLINK_STARTMISSION; 
            } 
            else if (!stricmp(pAnswer->page, "[main]"))
            {
                // return to the contact front page
                hash = CONTACTLINK_MAIN; 
            } 
            else if (!stricmp(pAnswer->page, "[grantmission]"))
            {
                // go to the contact new task window
                hash = CONTACTLINK_MISSIONS; 
            } 
            else if (!stricmp(pAnswer->page, "[introducecontact]"))
            {
                // go to the contact introduce new contacts window
                hash = CONTACTLINK_INTRODUCE; 
            } 
            else if (!stricmp(pAnswer->page, "[store]"))
            {
                // go to the contact store
                hash = CONTACTLINK_GOTOSTORE; 
            } 
            else if (!stricmp(pAnswer->page, "[about]"))
            {
                // go to the contact about window
                hash = CONTACTLINK_ABOUT; 
            } 
            else if (!stricmp(pAnswer->page, "[autocompletemission]"))
            {
                // go to the contact auto-complete mission window
                hash = CONTACTLINK_AUTO_COMPLETE;
            }
            else if (!stricmp(pAnswer->page, "[abandonmission]"))
            {
                // abandon the current mission and return to the hello page
                hash = CONTACTLINK_ABANDON;
            } 
            else if (!stricmp(pAnswer->page, "[dismisscontact]"))
            {
                // go to the contact dismissal confirmation window
                hash = CONTACTLINK_DISMISS;
            }
            else if (!strnicmp(pAnswer->page, "[webstore:", 10))
            {
                if (pPlayer->pl->webstore_link[0] != '\0')
                {
                    ErrorFilenamef(pTask->def->filename, "More than one webstore link on the same dialog page is not allowed!");
                }
                strncpy(pPlayer->pl->webstore_link, &pAnswer->page[10], 8);
                pPlayer->pl->webstore_link[8] = '\0';

                hash = CONTACTLINK_WEBSTORE;
            }
            else
            {
                hash = hashStringInsensitive(pAnswer->page);

                // check page requires
                pAnswerPage = DialogDef_FindPageFromDef(hash, pDialog);

                if (pAnswerPage)
                {
                    if (pAnswerPage->pRequires && !chareval_EvalWithTask(pPlayer->pchar, pAnswerPage->pRequires, pTask, pDialog->filename))
                        continue;
                } else {
                    pPageList = DialogDef_FindPageListFromDef(hash, pDialog);
                    if (!pPageList)
                        continue;

                    if (pPageList->pRequires && !chareval_EvalWithTask(pPlayer->pchar, pPageList->pRequires, pTask, pDialog->filename))
                        continue;

                }
            }

            // add to list
            dialogString = DialogDefAllocateStringLocalizeWithVars(pAnswer->text, vars, pPlayer);
            AddResponse(response, dialogString, hash);
            free(dialogString);
        }
    }

    if (didILockIt)
    {
        StoryArcUnlockInfo(pPlayer);
    }

    return true;
}


int DialogDef_FindAndHandlePage(Entity *pPlayer, Entity *pTarget, const DialogDef *pDialog, int hash, StoryTaskInfo *pTask, ContactResponse *response, ContactHandle contactHandle, ScriptVarsTable *vars)
{
    const DialogPageDef *pPage = NULL;
    const DialogPageListDef *pPageList = NULL;

    // let's see if we can find the requested page
    pPage = DialogDef_FindPageFromDef(hash, pDialog);

    if (!pPage)
    {
        pPageList = DialogDef_FindPageListFromDef(hash, pDialog);
        if (!pPageList)
            return false;

        pPage = DialogDef_FindPageFromPageList(pPlayer, pPageList, pDialog, pTask);

        if (!pPage)
            return false;
    }

    if (pDialog->pQualifyRequires && !chareval_EvalWithTask(pPlayer->pchar, pDialog->pQualifyRequires, pTask, pDialog->filename))
        return false;

    return DialogDef_DisplayPage(pPlayer, pTarget, response, pPage, pDialog, pTask, hash, contactHandle, vars);
}

static bool DialogDef_PageExists(const char *page, const DialogDef *pDialog)
{
    const DialogPageDef *pPage = NULL;
    const DialogPageListDef *pPageList = NULL;
    int hash = 0;

    if(page && page[0] == '[') // page is a special link type
        return true;

    hash = hashStringInsensitive(page);
    // let's see if we can find the requested page
    pPage = DialogDef_FindPageFromDef(hash, pDialog);

    if (!pPage)
    {
        pPageList = DialogDef_FindPageListFromDef(hash, pDialog);
        if (!pPageList)
            return false;
    }
    return true;
}

static void DialogDef_VerifyPageList(const char *filename, const DialogDef *pDialog, const DialogPageListDef *pPageList)
{
    int i;
    if(!pPageList->pPages)
        ErrorFilenamef(filename, "Dialog %s Pagelist %s has no pages.", pDialog->name, pPageList->name);
    else
    {
        for(i = eaSize(&pPageList->pPages) - 1; i >= 0; i--)
        {
            if(!DialogDef_PageExists(pPageList->pPages[i], pDialog))
                ErrorFilenamef(filename, "Dialog %s Pagelist %s points to Page %s which doesn't exist.", pDialog->name, pPageList->name, pPageList->pPages[i]);
        }
    }

    if(!(pPageList->flags & DIALOGPAGELIST_RANDOM || pPageList->flags & DIALOGPAGELIST_INORDER))
        ErrorFilenamef(filename, "Dialog %s Pagelist %s has no InOrder/Random flag.", pDialog->name, pPageList->name);

    // Too many errors to leave this in
    //if(!pPageList->fallbackPage)
    //    ErrorFilenamef(filename, "Dialog %s Pagelist %s has no Fallback Page.", pDialog->name, pPageList->name);
    
    if(pPageList->fallbackPage)
    {
        if(!DialogDef_PageExists(pPageList->fallbackPage, pDialog))
            ErrorFilenamef(filename, "Dialog %s Pagelist %s Fallback Page %s doesn't exist.", pDialog->name, pPageList->name, pPageList->fallbackPage);
    }

    if(pPageList->pRequires)
    {
        chareval_ValidateWithTask(pPageList->pRequires, filename);
    }
}

static void DialogDef_VerifyPage(const char *filename, const DialogDef *pDialog, const DialogPageDef *pPage)
{
    int i;
    if(pPage->pAnswers)
    {
        for(i = eaSize(&pPage->pAnswers) - 1; i >= 0; i--)
        {
            const DialogAnswerDef *answer = pPage->pAnswers[i];
            if(!answer->page)
                ErrorFilenamef(filename, "Dialog %s Page %s has an answer with no page.", pDialog->name, pPage->name);
            else
            {
                if(!DialogDef_PageExists(answer->page, pDialog))
                    ErrorFilenamef(filename, "Dialog %s Page %s has an answer to Page %s which doesn't exist.", pDialog->name, pPage->name, answer->page);
            }

            if(answer->pRequires)
            {
                chareval_ValidateWithTask(answer->pRequires, filename);
            }
        }
    }
    
    if(pPage->teleportDest && !(pPage->flags & DIALOGPAGE_AUTOCLOSE))
        ErrorFilenamef(filename, "Dialog %s Page %s has a teleport and must be set to autoclose.", pDialog->name, pPage->name);

    if(pPage->pRequires)
    {
        chareval_ValidateWithTask(pPage->pRequires, filename);
    }
}

void DialogDef_Verify(const char *filename, const DialogDef *pDialog)
{
    int i;
    if(!pDialog->pPages)
        ErrorFilenamef(filename, "Dialog %s has no pages.", pDialog->name);
    else
    {
        for(i = eaSize(&pDialog->pPages) - 1; i >= 0; i--)
        {
            DialogDef_VerifyPage(filename, pDialog, pDialog->pPages[i]);
        }
    }

    if(pDialog->pPageLists)
    {
        for(i = eaSize(&pDialog->pPageLists) - 1; i >= 0; i--)
        {
            DialogDef_VerifyPageList(filename, pDialog, pDialog->pPageLists[i]);
        }
    }

    if(pDialog->pQualifyRequires)
    {
        chareval_ValidateWithTask(pDialog->pQualifyRequires, filename);
    }
}

void DialogDefs_Verify(const char *filename, const DialogDef **pDialogs)
{
    int i;
    for(i = eaSize(&pDialogs) - 1; i >= 0; i--)
    {
        DialogDef_Verify(filename, pDialogs[i]);
    }
}

// Global DialogDefs
typedef struct DialogDefList
{
    const DialogDef**    dialogs;
    cStashTable            haDialogCRCs;
} DialogDefList;

TokenizerParseInfo ParseDialogDefList[] = {
    { "Dialog",        TOK_STRUCT(DialogDefList,dialogs,ParseDialogDef) },
    { "", 0, 0 }
};

SHARED_MEMORY DialogDefList g_dialogdeflist = {0};

static void DialogDef_Reprocess(const char* relpath)
{
    char cleanedName[MAX_PATH];
    int i, n = eaSize(&g_dialogdeflist.dialogs);
    strcpy_s(SAFESTR(cleanedName), strstri((char*)relpath, SCRIPTS_DIR));
    forwardSlashes(_strupr(cleanedName));
    for (i = 0; i < n; i++)
    {
        if (!stricmp(cleanedName, g_dialogdeflist.dialogs[i]->filename))
        {
            const DialogDef *dialog = g_dialogdeflist.dialogs[i];
            saUtilPreprocessScripts(ParseDialogDef, cpp_const_cast(DialogDef*)dialog);
            DialogDef_Verify(dialog->filename, dialog);
        }
    }
}

static void DialogDefList_DestroyHashes(DialogDefList *dict)
{
    if (dict->haDialogCRCs)
    {
        stashTableDestroyConst(dict->haDialogCRCs);
        dict->haDialogCRCs = NULL;
    }
}

extern U32 Crc32cLowerStringHash(U32 hash, const char * s);

static bool DialogDefList_CreateHashes(DialogDefList *dict, bool shared_memory)
{
    bool ret = true;
    int i, n = eaSize(&dict->dialogs);
    
    assert(!dict->haDialogCRCs);
    dict->haDialogCRCs = stashTableCreate(stashOptimalSize(n), stashShared(shared_memory), StashKeyTypeInts, sizeof(int));

    for(i = 0; i < n; i++)
    {
        int CRC;
        char *buf = estrTemp();
        const DialogDef *dialog = dict->dialogs[i];
        
        if(dialog->filename && dialog->name)
        {
            estrPrintf(&buf, "%s%s", dialog->filename, dialog->name);
            CRC = Crc32cLowerStringHash(0, buf);

            if(!stashIntAddPointerConst(dict->haDialogCRCs, CRC, dialog, false))
            {
                ErrorFilenamef(dialog->filename,"Duplicate global dialog def: %s %s", dialog->filename, dialog->name);
                ret = false;
            }
        }
        estrDestroy(&buf);
    }

    return ret;
}

static bool DialogDefList_FinalProcess(ParseTable* pti, void* structptr, bool shared_memory)
{
    DialogDefList * dict = (DialogDefList *)structptr;
    return DialogDefList_CreateHashes(dict, shared_memory);
}

// Reloads the latest version of a def, finds it, then processes
// MAKE SURE to do all the pre/post/etc stuff here that gets done during regular loading
static void DialogDefList_ReloadCallback(const char* relpath, int when)
{
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    if (ParserReloadFile(relpath, ParseDialogDefList, sizeof(DialogDefList), (void*)&g_dialogdeflist, NULL, NULL))
    {
        DialogDef_Reprocess(relpath);
        DialogDefList_DestroyHashes((DialogDefList*)&g_dialogdeflist);
        DialogDefList_CreateHashes((DialogDefList*)&g_dialogdeflist, false);
    }
    else
    {
        Errorf("Error reloading dialogdef: %s\n", relpath);
    }
}

static void DialogDefList_SetupCallbacks()
{
    if (!isDevelopmentMode())
        return;
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "scripts.loc/*.dialogdef", DialogDefList_ReloadCallback);
}

void DialogDefList_Load(void)
{
    int flags = PARSER_SERVERONLY;
    if (g_dialogdeflist.dialogs) // Reload!
        flags |= PARSER_FORCEREBUILD;

    ParserUnload(ParseDialogDefList, &g_dialogdeflist, sizeof(g_dialogdeflist));

    loadstart_printf("Loading global dialog definitions.. ");
    if (!ParserLoadFilesShared("SM_dialogdefs.bin", SCRIPTS_DIR, ".dialogdef", "dialogdefs.bin",
        flags, ParseDialogDefList, &g_dialogdeflist, sizeof(g_dialogdeflist), NULL, NULL, saUtilPreprocessScripts, NULL, DialogDefList_FinalProcess))
    {
        log_printf("scripterr", "Error loading global dialog definitions\n");
        printf("DialogDefList_Load: Error loading global dialog definitions\n");
    }
    DialogDefList_SetupCallbacks();
    loadend_printf("%i global dialog definitions", eaSize(&g_dialogdeflist.dialogs));
}

const DialogDef* DialogDef_FindDefFromGlobalDefs(const char* filename, const char* dialogName)
{
    char buf[MAX_PATH];
    int CRC;
    char *search = estrTemp();
    const DialogDef* dialog = NULL;

    if (!filename || !dialogName) return NULL;

    assert(g_dialogdeflist.haDialogCRCs);

    saUtilCleanFileName(buf, filename);
    estrPrintf(&search, "%s%s", filename, dialogName);
    CRC = Crc32cLowerStringHash(0, search);

    if( !stashIntFindPointerConst(g_dialogdeflist.haDialogCRCs, CRC, &dialog) ) {
        Errorf("Global Dialog Def not found: %s %s\n", filename, dialogName);
    }

    estrDestroy(&search);

    return dialog;
}