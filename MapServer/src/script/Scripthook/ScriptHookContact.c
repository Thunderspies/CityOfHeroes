/*\
*
*    scripthookcontact.c - Copyright 2005 Cryptic Studios
*        All Rights Reserved
*        Confidential property of Cryptic Studios
*
*    Provides all the functions CoH script files should be referencing -
*        essentially a list of hooks into game systems.
*
*    This file contains all functions related to locations
*
 */

#include "entity/entity.h"
#include "entity/entplayer.h"
#include "gameData/store.h"
#include "storyarc/contact.h"
#include "gameData/store_net.h"
#include "storyarc/storyinfo.h"
#include "storyarc/contactdef.h"
#include "script/scriptengine.h"
#include <utilitieslib/components/earray.h>
#include "storyarc/mission.h"
#include "container/team.h"
#include "storyarc/missiongeoCommon.h"
#include "dbcomm/dbdoor.h"
#include "svr/svr_base.h"
#include "comm_game.h"
#include "entity/character_level.h"
#include "entity/character_net_server.h"
#include "storyarc/dialogdef.h"
#include <utilitieslib/components/HashFunctions.h>
#include "entity/character_eval.h"
#include "reward.h"
#include "dbcomm/logcomm.h"

#include "script/scripthook/ScriptHookInternal.h"

// *********************************************************************************
//  S C R I P T    D A T A
// *********************************************************************************

typedef struct ScriptData {
    MultiStore    **StoreList;        // used for entities that are script stores
    int            StoreBeingUsed;        // used by players who are interacting with a script store
} ScriptData;



void *ScriptCreateData()
{
    ScriptData *pData = malloc(sizeof(ScriptData));
    memset(pData, 0, sizeof(ScriptData));
    return pData;
}

void ScriptFreeData(void **sData)
{
    int i;
    ScriptData *pData = (ScriptData *) *sData;

    // clear store list
    if (pData->StoreList != NULL) {
        // script store data
        for (i = 0; i < eaSize(&pData->StoreList); i++)
        {
            MultiStore *store = pData->StoreList[i];
            eaDestroyEx(&store->ppchStores, NULL);
            free(store);
        }
        eaDestroy(&pData->StoreList);
    }

    // final cleanup
    free(*sData);
    *sData = NULL;
}


// *********************************************************************************
//  C O N T A C T S
// *********************************************************************************


// Takes a player and a contact handle and returns whether or not they currently have that contact
int DoesPlayerHaveContact(ENTITY player, ContactHandle contact)
{
    Entity* ent = EntTeamInternal(player, 0, NULL);
    if (ent)
    {
        StoryInfo* info = ent->storyInfo;
        return ContactGetInfo(info, contact)?1:0;
    }
    return 0;
}

void ScriptGivePlayerNewContact(ENTITY player, ContactHandle contact)
{
    Entity* ent = EntTeamInternal(player, 0, NULL);
    if (ent)
    {
        StoryInfo* info = StoryArcLockPlayerInfo(ent);
        ContactAdd(ent, info, contact, "Script");
        StoryArcUnlockPlayerInfo(ent);
    }
}

ContactHandle ScriptGetContactHandle(STRING contact)
{
    return ContactGetHandle(contact);
}

// Call after creating a new script driven contact to register their location with the dbserver
void ScriptRegisterNewContact(ContactHandle contact, LOCATION location)
{
    char shortname[MAX_PATH];
    char* dot;
    const char* contactName = ContactFileName(contact);
    Mat4 mat;

    if (!contactName)
        return;

    strcpy(shortname, contactName);
    dot = strrchr(shortname, '.');
    if (dot) *dot = 0;

    GetMatFromLocation(location, mat, 0);

    ContactRegisterScriptContact(shortname, mat);
}

// *********************************************************************************
//  S T O R E S
// *********************************************************************************

MultiStore *ScriptDataGetStoreBeingUsed(Entity *player, Entity *merchant)
{
    ScriptData *pPlayerData = (ScriptData *) player->scriptData;
    ScriptData *pMerchantData = (ScriptData *) merchant->scriptData;

    if ( pPlayerData != NULL && pMerchantData != NULL )
    {
        return pMerchantData->StoreList[pPlayerData->StoreBeingUsed];
    } else {
        return NULL;
    }
}

void OpenStoreExternal(Entity* merchant, Entity* client, const char *name)
{
    int i;
    ScriptData *pData = NULL;
    MultiStore *store = NULL;

    if (merchant && client && ENTTYPE(client) == ENTTYPE_PLAYER && store_Find(name))
    {
        if (merchant->scriptData == NULL)
        {
            pData = ScriptCreateData();
            eaCreate(&pData->StoreList);
            merchant->scriptData = pData;
        } else {
            pData = (ScriptData *) merchant->scriptData;
        }

        for (i = 0; i < eaSize(&pData->StoreList); i++)
        {
            store = pData->StoreList[i];
            if (StringEqual(name, store->ppchStores[0]))
            {
                break;
            } else {
                store = NULL;
            }
        }

        if (store == NULL) 
        {
            // set up a store for sending
            store = malloc(sizeof(MultiStore));

            store->iCnt = 1;
            store->idNPC = 0;

            eaCreate(&store->ppchStores);
            eaPush(&store->ppchStores, strdup(name));

            // add to list associate with this entity
            eaPush(&pData->StoreList, store);
        }

        client->pl->npcStore = merchant->owner;

        // send it off
        store_SendOpenStorePNPC(client, merchant, store);

        // mark player as using this store
        if (client->scriptData == NULL)
        {
            pData = ScriptCreateData();
            client->scriptData = pData;
        } else {
            pData = client->scriptData;
        }
        pData->StoreBeingUsed = i;
    }
}

void OpenStore(ENTITY npc, ENTITY player, STRING name)
{
    Entity* merchant = EntTeamInternal(npc, 0, NULL);
    Entity* client = EntTeamInternal(player, 0, NULL);

    if (merchant && client)
    {
        OpenStoreExternal(merchant, client, name);
    }
}

// *********************************************************************************
//  A U C T I O N
// *********************************************************************************

void OpenAuction(ENTITY npc, ENTITY player)
{
    Entity* client = EntTeamInternal(player, 0, NULL);
    Entity* auction_npc = EntTeamInternal(npc, 0, NULL);

    if (client && auction_npc)
    {
        character_toggleAuctionWindow(client, 1, auction_npc->owner);
    }
}

// *********************************************************************************
//  D I A L O G S
// *********************************************************************************

// in task.c
extern int *remoteDialogLookupContactHandleKeys;
extern void **remoteDialogLookupTables;

// Doesn't work with actual, mission-granting Contacts!
int ContactHandleDialogDef(ENTITY player, ENTITY target, STRING dialog, STRING startPage, STRING filename, NUMBER link, ContactResponse* response)
{
    const DialogDef *pDialog = NULL;
    DialogPageDef *pPage = NULL;
    DialogPageListDef *pPageList = NULL;
    Entity* pPlayer = EntTeamInternal(player, 0, NULL);
    Entity* pTarget = EntTeamInternal(target, 0, NULL);
    int hash;
    DialogContext *pContext = NULL;
    StoryTaskInfo *storyTaskInfo = NULL;
    ScriptVarsTable vars = {0};

    if (!pPlayer || !pTarget)
        return false;

    if(StringEmpty(filename))
    {
        LOG_ENT(pPlayer, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "ContactInteract:Script Starting dialog tree interaction with entity %s, using tree %s starting on page %s", target, dialog, startPage);

        if (OnMissionMap() && g_activemission && g_activemission->task)
        {
            pDialog = DialogDef_FindDefFromStoryTask(g_activemission->task, dialog);

            // let's see if we can get it from the compound parent
            if (!pDialog && pPlayer->teamup && pPlayer->teamup->activetask && pPlayer->teamup->activetask->compoundParent != NULL)
                pDialog = DialogDef_FindDefFromStoryTask(pPlayer->teamup->activetask->compoundParent, dialog);
        } 
        
        if (!pDialog && pTarget->encounterInfo && 
                    pTarget->encounterInfo->parentGroup &&
                    pTarget->encounterInfo->parentGroup->active &&
                    pTarget->encounterInfo->parentGroup->active->spawndef)
        {
            // I'm part of an active spawn - perhaps the dialog is attached to the spawndef
            pDialog = DialogDef_FindDefFromSpawnDef(pTarget->encounterInfo->parentGroup->active->spawndef, dialog);
        }
    }
    else
    {
        LOG_ENT(pPlayer, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "ContactInteract:Script Starting global dialog tree interaction with entity %s, using file %s, tree %s starting on page %s", target, filename, dialog, startPage);

        pDialog = DialogDef_FindDefFromGlobalDefs(filename, dialog);
    }

    if (!pDialog)
        return false;


    if (link == CONTACTLINK_HELLO)
    {
        // start page
        hash = hashStringInsensitive(startPage);
    }
    else if (link == CONTACTLINK_BYE)
    {
        // leave!
        return false;
    }
    else 
    {
        // link is the hash    
        hash = link;
    }

    pContext = (DialogContext *) malloc(sizeof(DialogContext));
    memset(pContext, 0, sizeof(DialogContext));

    pContext->pDialog = pDialog;

    if (g_activemission && pPlayer->teamup && pPlayer->teamup->activetask &&
        isSameStoryTaskPos(&g_activemission->sahandle, &pPlayer->teamup->activetask->sahandle))
    {
        pContext->ownerDBID = pPlayer->teamup->activetask->assignedDbId;
        StoryHandleCopy(&pContext->sahandle, &pPlayer->teamup->activetask->sahandle);
    }

    if(g_currentScript)
    {
        // Copy for our immediate resolution of current page
        ScriptVarsTableScopeCopy(&vars, &g_currentScript->initialvars);

        // Store a copy for the DialogContext to use later
        ScriptVarsTableScopeCopy(&pContext->vars, &g_currentScript->initialvars);
    }

    // store it on the NPCs
    if (pTarget->dialogLookup == NULL)
    {
        pTarget->dialogLookup = (void *) stashTableCreateInt(4);
    }

    // set up dialog context for this conversation
    stashIntAddPointer(pTarget->dialogLookup, pPlayer->db_id, pContext, true);

    pPlayer->storyInfo->interactEntityRef = erGetRef(pTarget);
    pPlayer->storyInfo->interactTarget = 0;
    pPlayer->storyInfo->interactDialogTree = 1;
    pPlayer->storyInfo->interactCell = 0;
    pPlayer->storyInfo->interactPosLimited = 0;

    // Just add the basics, everything else has been taken from the script
    saBuildScriptVarsTable(&vars, pPlayer, pPlayer->db_id, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, false);

    return DialogDef_FindAndHandlePage(pPlayer, pTarget, pDialog, hash, NULL, response, 0, &vars);
}
