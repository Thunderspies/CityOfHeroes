/*\
 *
 *    contactInteraction.h/c - Copyright 2003-2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    IO functions for contact dialogs
 *    trainer and unknown contact dialog functions
 *    the normal contact dialog is in contactDialog.c
 *
 */

#include "contact.h"
#include "storyarcprivate.h"
#include "storySend.h"
#include "comm_game.h"
#include "svr/svr_player.h"
#include "gameComm/svr_chat.h"
#include "gameSys/dooranim.h"            // for SERVER_INTERACT_DISTANCE
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/character_combat.h"
#include "entity/character_animfx.h"
#include "entity/powers.h"
#include "filter/titles.h"
#include "entity/entPlayer.h"
#include "gameSys/arenakiosk.h"                    // ArenaKioskCheckPlayer
#include "entity/entity.h"
#include "entity/pvp.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "player/badges.h"
#include "auth/authUserData.h"
#include "entity/SgrpServer.h"
#include "container/team.h"
#include "player/badges_server.h"
#include "entity/SgrpRewards.h"
#include "gameSys/arenamap.h"
#include "auth/authUserData.h"                // authUserMonthsPlayed()
#include "cmdparse/cmdserver.h"
#include "entity/character_inventory.h"
#include "taskforce.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "dbcomm/staticMapInfo.h"
#include "door.h"
#include "entity/character_eval.h"
#include <utilitieslib/components/HashFunctions.h>
#include "gameSys/cutScene.h"
#include "entity/character_target.h"
#include "contactDialog.h"
#include "gameData/store_net.h"
#include "dbcomm/logcomm.h"

// *********************************************************************************
//  Contact dialog IO
// *********************************************************************************
void ContactInteractionEnd(Entity* player);

void ContactGeneralInteract(Entity* player, ContactHandle handle, int link)
{
    ContactResponse response;
    int result, unknownLink;

    memset(&response, 0, sizeof(ContactResponse));

    unknownLink = ContactCannotInteract(player, handle, link, player->storyInfo->interactCell);

    if (unknownLink)
    {
        result = ContactUnknownInteraction(player, handle, unknownLink, &response);
    }
    else
    {
        result = ContactInteraction(player, handle, link, &response);
    }

    if (result > 0 && !response.dialogPageAutoClose)
    {
        START_PACKET(pak, player, SERVER_CONTACT_DIALOG_OPEN)
            ContactResponseSend(&response, pak);
        END_PACKET

        if (response.SOLdialog[0] != 0 && erGetEnt(player->storyInfo->interactEntityRef) && 
            !player->storyInfo->interactCell)
        {
            Entity *pContact = erGetEnt(player->storyInfo->interactEntityRef);
            // Everything placed in SOLdialog is already saUtilTableAndEntityLocalize'd
            saUtilEntSays(pContact, 1, response.SOLdialog);
        }
    }
    else if (!result || response.dialogPageAutoClose)
    {
        ContactInteractionEnd(player);
    }
    // else do nothing if result < 0.
}

void ContactResponseSend(ContactResponse* response, Packet* pak)
{
    int i;

    pktSendBitsPack(pak, 1, response->type);
    pktSendString(pak, response->dialog);
    pktSendBitsPack(pak, 1, response->responsesFilled);
    for(i = 0; i < response->responsesFilled; i++)
    {
        ContactResponseOption* r = &response->responses[i];
        pktSendBitsPack(pak, 1, r->link);
        pktSendBitsAuto(pak, r->rightLink);
        pktSendString(pak, r->responsetext);
        pktSendString(pak, r->rightResponseText);
    }
}

void ContactDialogResponsePkt(Packet* pakIn, Entity* player)
{
    int link = pktGetBitsPack(pakIn, 1);

    LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "ContactInteract:Response Received response %d to contact %d", link, player->storyInfo->interactTarget);

    ContactGeneralInteract(player, player->storyInfo->interactTarget, link);
}

void ContactOpen(Entity* player, const Vec3 pos, ContactHandle handle, Entity* contact)
{
    LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "ContactInteract:GenericOpen Initiating interaction with contact %d (%s)", handle, contact ? contact->name : "NULL");

    player->storyInfo->interactEntityRef = erGetRef(contact);
    player->storyInfo->interactTarget = handle;
    player->storyInfo->interactDialogTree = 0;
    player->storyInfo->interactCell = 0;
    player->storyInfo->interactPosLimited = 1;
    copyVec3(pos, player->storyInfo->interactPos);
    ContactGeneralInteract(player, handle, CONTACTLINK_HELLO);
}

void ContactCellCall(Entity* player, ContactHandle handle)
{
    const ContactDef *contactDef = ContactDefinition(handle);

    LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "ContactInteract:CellCall Initiating interaction with contact %d", handle);

    if (!player || !player->storyInfo || !contactDef)
    {
        LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "ContactInteract:CellCall:Fail Bad data received with contact %d, cancelling interaction", handle);
        return;
    }

    // Only do this if we're not already interacting with this contact
    if (player->storyInfo->interactTarget != handle)
    {
        if (CONTACT_IS_NEWSPAPER(contactDef))
        {
            if (contactDef->alliance == ALLIANCE_HERO)
                saUtilEntSays(player, 0, "<em ListenPoliceBand>");
            else
                saUtilEntSays(player, 0, "<em newspaper>");
            player->storyInfo->interactEntityRef = 0;
            player->storyInfo->interactTarget = handle;
            player->storyInfo->interactDialogTree = 0;
            player->storyInfo->interactCell = 0;
            player->storyInfo->interactPosLimited = 0;
            ContactGeneralInteract(player, handle, CONTACTLINK_HELLO);
        }
        else
        {
            player->storyInfo->interactEntityRef = 0;
            player->storyInfo->interactTarget = handle;
            player->storyInfo->interactDialogTree = 0;
            player->storyInfo->interactCell = 1;
            player->storyInfo->interactPosLimited = 0;

            // Try to finish a item delivery task if possible.
            if(DeliveryTaskTargetInteract(player, 0, handle))
                return;

            ContactGeneralInteract(player, handle, CONTACTLINK_HELLO);
        }
    }
}

/*
void ContactFlashbackSendoff(Entity* player, const Vec3 pos, ContactHandle handle)
{
    player->storyInfo->interactTarget = handle;
    player->storyInfo->interactCell = 0;
    player->storyInfo->interactPosLimited = 1;
    copyVec3(pos, player->storyInfo->interactPos);
    ContactGeneralInteract(player, handle, CONTACTLINK_ACCEPT_FLASHBACK);
}
*/

// Using cell phone interface for newspapers now
// void ContactNewspaperInteract(ClientLink* client)
// {
//     ContactHandle contact = ContactGetNewspaper(client->entity);
//     ContactDef *contactDef;
// 
//     if (!contact) return;
// 
//     contactDef = ContactDefinition(contact);
//     if (contactDef->alliance == ALLIANCE_HERO)
//         saUtilEntSays(client->entity, 0, "<em ListenPoliceBand>");
//     else
//         saUtilEntSays(client->entity, 0, "<em newspaper>");
//     client->entity->storyInfo->interactTarget = contact;
//     client->entity->storyInfo->interactDialogTree = 0;
//     client->entity->storyInfo->interactCell = 0;
//     client->entity->storyInfo->interactPosLimited = 0;
//     ContactGeneralInteract(client->entity, contact, CONTACTLINK_HELLO);
// }

//If your contact is something in a base
void ContactBaseInteract( Entity * e, char* contactfile)
{
    ContactHandle contact = 0;

    if( !e || !e->storyInfo )
        return;

    if( contactfile )
        contact = ContactGetHandleLoose(contactfile);
    if (!contact)
        return;

    LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "ContactInteract:Base Initiating interaction with contact %d", contact);

    e->storyInfo->interactEntityRef = 0;
    e->storyInfo->interactTarget = contact;
    e->storyInfo->interactDialogTree = 0;
    e->storyInfo->interactCell = 0;
    e->storyInfo->interactPosLimited = 1;
    copyVec3(ENTPOS(e), e->storyInfo->interactPos);

    ContactGeneralInteract( e, contact, CONTACTLINK_HELLO);
}

void ContactDebugInteract(ClientLink* client, char* contactfile, int cellcall)
{
    ContactHandle contact;
    contact = ContactGetHandleLoose(contactfile);
    if (!contact)
    {
        conPrintf(client, localizedPrintf(0,"CouldntFindContact", contactfile));
        return;
    }

    LOG_ENT(client->entity, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "ContactInteract:Debug Initiating interaction with contact %d", contact);

    client->entity->storyInfo->interactEntityRef = 0;
    client->entity->storyInfo->interactTarget = contact;
    client->entity->storyInfo->interactDialogTree = 0;
    client->entity->storyInfo->interactCell = cellcall;
    client->entity->storyInfo->interactPosLimited = 0;
    ContactGeneralInteract(client->entity, contact, CONTACTLINK_HELLO);
}



// Make sure the server knows that the interaction has stopped.
void ContactInteractionClose(Entity* player)
{
    player->storyInfo->interactEntityRef = 0;
    player->storyInfo->interactTarget = 0;
    START_PACKET(pak, player, SERVER_CONTACT_DIALOG_CLOSE);
    END_PACKET
}

// The teleport contact links will close the contact interaction themselves, so we
// only need to close the interaction if the player did not teleport away
// LH: Added this because telelporting would trigger and 2nd call to teleport that chose randomly
void ContactInteractionEnd(Entity* player)
{
    // tutorial contacts have to teleport player when they get the close dialog command
    const ContactDef* contactdef = ContactDefinition(player->storyInfo->interactTarget);
    if (contactdef && CONTACT_IS_TELEPORTONCOMPLETE(contactdef))
    {
        StoryInfo* info = StoryInfoFromPlayer(player);
        StoryContactInfo* contactInfo = ContactGetInfo(info, player->storyInfo->interactTarget);
        if (contactInfo && contactInfo->contactRelationship >= CONTACT_CONFIDANT)
        {
            // randomly choose a city for them
            ContactResponse response;
            memset(&response, 0, sizeof(ContactResponse));
            if (ENT_IS_VILLAIN(player))
                ContactInteraction(player, player->storyInfo->interactTarget, rand() % 2? CONTACTLINK_NEWPLAYERTELEPORT_MI_KALINDA : CONTACTLINK_NEWPLAYERTELEPORT_MI_BURKE, &response);
            else
                ContactInteraction(player, player->storyInfo->interactTarget, rand() % 2? CONTACTLINK_NEWPLAYERTELEPORT_AP : CONTACTLINK_NEWPLAYERTELEPORT_GC, &response);

        }
        else
        {
            ContactInteractionClose(player);
        }
    } 
    else
    {
        ContactInteractionClose(player);
    }
}

#define CONTACT_PROCESS_INTERVAL    30    // only check every N ticks for a particular player

// check to see if player has moved too far away from contact they are interacting with
void ContactProcess()
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;
    static int tick = 0;

    tick = (++tick) % CONTACT_PROCESS_INTERVAL;
    for (i=tick; i<players->count; i += CONTACT_PROCESS_INTERVAL)
    {
        StoryInfo* info = players->ents[i]->storyInfo;
        Entity *interactEntity = erGetEnt(info->interactEntityRef);

        // if talking to contact, distance limited, and too far
        if (info->interactPosLimited &&
            distance3Squared(info->interactPos, ENTPOS(players->ents[i])) > SERVER_INTERACT_DISTANCE * SERVER_INTERACT_DISTANCE)
        {
            // If we ran too far away, and the player has the store window open
            // tell it to close itself.
            if (players->ents[i]->pl->npcStore < 0)
            {
                players->ents[i]->pl->npcStore = 0;
                START_PACKET(pak, players->ents[i], SERVER_STORE_CLOSE);
                END_PACKET
            }
            ContactInteractionEnd(players->ents[i]);
        }
        // also close the window if talking to a contact, and the contact becomes hostile
        if (info->interactTarget && interactEntity && ENTTYPE(interactEntity) == ENTTYPE_CRITTER && interactEntity->contactOrPnpc)
        {
            if (players->ents[i]->pchar && interactEntity->pchar && character_TargetIsFoe(players->ents[i]->pchar, interactEntity->pchar))
            {
                ContactInteractionEnd(players->ents[i]);
            }
        }


        // also hook distance checks to arena kiosk in here - rename this function
        // if we keep putting other distance checks here
        ArenaKioskCheckPlayer(players->ents[i]);
    }
}

// *********************************************************************************
//  Unknown contact and trainer handlers
// *********************************************************************************

// returns WHY the player can't interact with contact
int ContactCannotInteract(Entity* player, ContactHandle handle, int link, int cellcall)
{
    StoryInfo* info;
    StoryContactInfo* contactInfo;
    int result = 0; // default to ok
    const ContactDef *contactDef = ContactDefinition(handle);
    ContactAlliance playerCA = ENT_IS_VILLAIN(player)?ALLIANCE_VILLAIN:ALLIANCE_HERO;
    ContactUniverse playerCU = ENT_IS_IN_PRAETORIA(player)?UNIVERSE_PRAETORIAN:UNIVERSE_PRIMAL;
    ContactAlliance playerCAByLocation = ENT_IS_ON_RED_SIDE(player)?ALLIANCE_VILLAIN:ALLIANCE_HERO;

    if(!player)
        return CONTACTLINK_BYE;

    info = StoryInfoFromPlayer(player);
    contactInfo = ContactGetInfo(info, handle);

    // is this a delivery task contact?
    if (player->storyInfo->interactDialogTree)
    {
        result = link;
    }
    // player cheating?  data problem?
    else if (!contactDef)
    {
        result = CONTACTLINK_BYE;
    }
    else if (CONTACT_IS_REGISTRAR(contactDef))
    {
        // Make sure contact is of the same alliance as the player; Unpsecified counts as neutral.
        //  Unspecified is really an error, and is reported to designers when they make their bins.
        if (!PlayerAllianceMatchesDefAlliance(playerCA, contactDef->alliance))
        {
            result = CONTACTLINK_WRONGALLIANCE;
        }
        else if (!PlayerUniverseMatchesDefUniverse(playerCU, contactDef->universe))
        {
            result = CONTACTLINK_WRONGUNIVERSE;
        }
        else if (entIsTrial(player) || (contactDef->interactionRequires && !chareval_Eval(player->pchar, contactDef->interactionRequires, contactDef->filename)))
        {
            result = CONTACTLINK_FAILEDREQUIRES;
        }
        else
        {
            result = link;
        }
    }
    // see if the the contact is a trainer
    else if ( CONTACT_IS_SPECIAL(contactDef) )
    {
        result = link;
    }
    // meta contacts will talk to anyone, regardless of being in taskforce mode or not
    else if (CONTACT_IS_METACONTACT(contactDef))
    {
        result = 0;
    }
    // task force mode checks
    else if (PlayerInTaskForceMode(player))
    {
        // players cannot interact with a different contact than their task force contact
        if (handle != PlayerTaskForceContact(player))
        {
            result = CONTACTLINK_WRONGMODE;
        }
        // in task force mode, you have to be the leader to talk to the contact
        else if (player->taskforce->members.leader != player->db_id)
        {
            result = CONTACTLINK_NOTLEADER;
        }
        // otherwise ok
    }
    // task force
    else if (CONTACT_IS_TASKFORCE(contactDef))
    {
        result = 0;
    }
    // Supergroup contacts require you to have permission to get a SG mission, and you must be in a SG
    else if (CONTACT_IS_SGCONTACT(contactDef))
    {
        if (!player->supergroup)
            result = CONTACTLINK_NOSG;
        // Remove this for now, I don;t think SG contacts exist in game
        //else if (!sgroup_hasPermission(player, SG_PERM_GET_SGMISSION))
        //    result = CONTACTLINK_NOSGPERMS;
        else
            result = 0;    // SG Contact will talk to anyone in a supergroup
    }
    // Newspaper has special interaction conditions
    else if (CONTACT_IS_NEWSPAPER(contactDef))
    {
        ContactHandle broker = StoryArcGetCurrentBroker(player, info);
        if (!broker || ContactShouldGiveHeist(ContactGetInfo(info, broker)))
            result = CONTACTLINK_NEWS_ERROR;
        else if (!ContactCorrectAlliance(SAFE_MEMBER(player->pchar, playerTypeByLocation), contactDef->alliance))
            result = CONTACTLINK_NEWS_ERROR;
        else // check levels
        {
            const ContactDef* brokerDef = ContactDefinition(broker);
            int level = character_GetExperienceLevelExtern(player->pchar);
            if ((level < brokerDef->minPlayerLevel) || (level > brokerDef->maxPlayerLevel))
                result = CONTACTLINK_NEWS_ERROR;
        }
        // Dont have the paper yet
        if (!contactInfo)
            result = CONTACTLINK_DONTKNOW;
        // Close the paper
        if ((link == CONTACTLINK_BYE) || !contactInfo)
            saUtilEntSays(player, 0, "<em none>");
    }
    // see if the player knows the contact at all
    else if (!contactInfo)
    {
        // Default to not knowing the contact, unless we add them
        result = CONTACTLINK_DONTKNOW;

        // Check to see if the contact required a badge
        if (contactDef->requiredBadge)
        {
            const BadgeDef* required = badge_GetBadgeByName(contactDef->requiredBadge);
            if (required && (required->iIdx >= 0))
            {
                int badgeField = player->pl->aiBadges[required->iIdx];
                if (BADGE_IS_OWNED(badgeField))
                {
                    if (CONTACT_IS_ISSUEONINTERACT(contactDef)
                        && !PlayerInTaskForceMode(player)
                        && (!contactDef->interactionRequires || chareval_Eval(player->pchar, contactDef->interactionRequires, contactDef->filename)))
                    {
                        // Badge autoissue contacts require the minimum level before introduction
                        if (character_GetExperienceLevelExtern(player->pchar) >= contactDef->minPlayerLevel)
                        {
                            ContactAdd(player, info, handle, "issue on interact badge contact");
                            contactInfo = ContactGetInfo(info, handle);
                        }
                        else
                        {
                            result = CONTACTLINK_BADGEREQUIRED_TOOLOW;
                        }
                    }
                }
                else // Give them some information on their progress
                {
                    // Badge completion data is stored in millionths rather
                    // than percentage points now.
                    F32 percentCompleted = (BADGE_COMPLETION(badgeField) / 10000.0f);
                    if (percentCompleted < 25.0f)
                        result = CONTACTLINK_BADGEREQUIRED_FIRST;
                    else if (percentCompleted < 50.0f)
                        result = CONTACTLINK_BADGEREQUIRED_SECOND;
                    else if (percentCompleted < 75.0f)
                        result = CONTACTLINK_BADGEREQUIRED_THIRD;
                    else
                        result = CONTACTLINK_BADGEREQUIRED_FOURTH;
                }
            }
        }
        // maybe I'm talking to an issue on interact contact? (don't issue if badge required)
        else if (CONTACT_IS_ISSUEONINTERACT(contactDef) && link != CONTACTLINK_BYE
                 && !PlayerInTaskForceMode(player)
                 && (!contactDef->interactionRequires || chareval_Eval(player->pchar, contactDef->interactionRequires, contactDef->filename)))
        {
            ContactAdd(player, info, handle, "issue on interact");
            contactInfo = ContactGetInfo(info, handle);
        }

        // If the contact has been added, we are good to go
        if (contactInfo)
        {
            // COPY/PASTED FROM BELOW
            // Make sure contact is of the same alliance as the player; Unpsecified counts as neutral.
            //  Unspecified is really an error, and is reported to designers when they make their bins.
            if (CONTACT_IS_BROKER(contactDef) && !PlayerAllianceMatchesDefAlliance(playerCAByLocation, contactDef->alliance))
            {
                result = CONTACTLINK_WRONGALLIANCE;
            }
            else if (!CONTACT_IS_BROKER(contactDef) && !PlayerAllianceMatchesDefAlliance(playerCA, contactDef->alliance))
            {
                result = CONTACTLINK_WRONGALLIANCE;
            }
            else if (!PlayerUniverseMatchesDefUniverse(playerCU, contactDef->universe))
            {
                result = CONTACTLINK_WRONGUNIVERSE;
            }
            else if ((contactDef->interactionRequires && !chareval_Eval(player->pchar, contactDef->interactionRequires, contactDef->filename)))
            {
                result = CONTACTLINK_FAILEDREQUIRES;
            }
            else
            {
                result = 0;
            }
        }
    }
    // COPY/PASTED TO ABOVE
    // Make sure contact is of the same alliance as the player; Unpsecified counts as neutral.
    //  Unspecified is really an error, and is reported to designers when they make their bins.
    else if (CONTACT_IS_BROKER(contactDef) && !PlayerAllianceMatchesDefAlliance(playerCAByLocation, contactDef->alliance))
    {
        result = CONTACTLINK_WRONGALLIANCE;
    }
    else if (!CONTACT_IS_BROKER(contactDef) && !PlayerAllianceMatchesDefAlliance(playerCA, contactDef->alliance))
    {
        result = CONTACTLINK_WRONGALLIANCE;
    }
    else if (!PlayerUniverseMatchesDefUniverse(playerCU, contactDef->universe))
    {
        result = CONTACTLINK_WRONGUNIVERSE;
    }
    else if ((contactDef->interactionRequires && !chareval_Eval(player->pchar, contactDef->interactionRequires, contactDef->filename)))
    {
        result = CONTACTLINK_FAILEDREQUIRES;
    }

    // can't call contacts
    if (cellcall && contactInfo && (OnMissionMap() || OnArenaMap() || !CONTACT_CAN_USE_CELL(contactInfo) || 
        server_visible_state.isPvP ))
        result = CONTACTLINK_BADCELLCALL;

    // pass BYE link on to interaction function
    if (result && link == CONTACTLINK_BYE)
        return CONTACTLINK_BYE;

    // otherwise, ok
    return result;
}

// returns WHY the player can't get that MetaContact link
// this can fail MetaContacts and TaskForce contacts whereas ContactCannotInteract always allows those through
int ContactCannotMetaLink(Entity* player, ContactHandle handle)
{
    int result = 0; // default to ok
    const ContactDef *contactDef = ContactDefinition(handle);
    ContactAlliance playerCA = ENT_IS_VILLAIN(player)?ALLIANCE_VILLAIN:ALLIANCE_HERO;
    ContactUniverse playerCU = ENT_IS_IN_PRAETORIA(player)?UNIVERSE_PRAETORIAN:UNIVERSE_PRIMAL;

    if(!player || !contactDef)
        return CONTACTLINK_BYE;

    if (!PlayerAllianceMatchesDefAlliance(playerCA, contactDef->alliance))
    {
        result = CONTACTLINK_WRONGALLIANCE;
    }
    else if (!PlayerUniverseMatchesDefUniverse(playerCU, contactDef->universe))
    {
        result = CONTACTLINK_WRONGUNIVERSE;
    }
    else if ((contactDef->interactionRequires && !chareval_Eval(player->pchar, contactDef->interactionRequires, contactDef->filename)))
    {
        result = CONTACTLINK_FAILEDREQUIRES;
    }

    return result;
}

//
// Dumps all the new stuff a player is going to get for their next level.
//
static void ContactInteractionShowTrainingImprovements(Entity* player, int link, ContactResponse* response )
{
    int i;
    char ach[128];
    bool bPowerIntro = false;
    int iLevel;
    int iMaxLevel;

    iLevel = character_GetLevel(player->pchar);
    iMaxLevel = character_PowersGetMaximumLevel(player->pchar);

    sprintf(ach, "Train_%02d_%s", iLevel+1+1, player->pchar->pclass->pchName );

    if( stricmp(ach, saUtilLocalize(player,ach)) == 0 )
        sprintf(ach, "Train_%02d", iLevel+1+1);

    strcat(response->dialog, saUtilLocalize(player,ach));

    for(i=1; i<eaSize(&player->pchar->ppPowerSets); i++)
    {
        int j;
        PowerSet *pset = player->pchar->ppPowerSets[i];

         if(stricmp(pset->psetBase->pcatParent->pchName, "Temporary_Powers")==0 || stricmp(pset->psetBase->pcatParent->pchName, "Inherent")==0)
            continue;

        for(j=0; j<eaSize(&pset->ppPowers); j++)
        {
            if(pset->ppPowers[j]!=NULL)
            {
                Power *ppow = pset->ppPowers[j];
                int iNumBoosts;

                iNumBoosts = ppow->iNumBoostsBought +
                    CountForLevel(iLevel+1-ppow->iLevelBought, g_Schedules.aSchedules.piFreeBoostSlotsOnPower);
                iNumBoosts = min(iNumBoosts, ppow->ppowBase->iMaxBoosts);

                if(eaSize(&ppow->ppBoosts) < iNumBoosts)
                {
                    if(!bPowerIntro)
                    {
                        strcat(response->dialog, saUtilLocalize(player,"PowersWillGetSlots"));
                        strcat(response->dialog, "<table><tr border=0><td width=5 border=0></td><td width=95>");
                        bPowerIntro = true;
                    }
                    else
                    {
                        strcat(response->dialog, ", ");
                    }

                    strcat(response->dialog, localizedPrintf(player,ppow->ppowBase->pchDisplayName));
                }
            }
        }
    }
    if(bPowerIntro)
    {
        strcat(response->dialog, "</td></tr>");
        strcat(response->dialog, "</table>");
        strcat(response->dialog, "<br><br>");
    }

    if (player->pchar->iLevel >= 1)
    {
        strcat(response->dialog, saUtilLocalize(player,"YouWillGain"));
        strcat(response->dialog, "<table><tr border=0><td width=5 border=0></td><td width=95 border=0></td></tr>");
        {
            int iTmp;
            int inherentCount = 0;
            // Figure out if there are any new inherent powers to give out.
            const PowerCategory *pcat = powerdict_GetCategoryByName(&g_PowerDictionary, "Inherent");
            if(pcat)
            {
                int i;
                for(i=0; i<eaSize(&pcat->ppPowerSets); i++)
                {
                    int j;
                    for(j=0; j<eaSize(&pcat->ppPowerSets[i]->ppPowers); j++)
                    {
                        if(pcat->ppPowerSets[i]->piAvailable[j]==iLevel+1
                            && character_IsAllowedToHavePower(player->pchar, pcat->ppPowerSets[i]->ppPowers[j]))
                        {
                            const BasePower *ppowBase = pcat->ppPowerSets[i]->ppPowers[j];

                            if (!ppowBase || !ppowBase->bShowInInfo)
                                continue;

                            if (!inherentCount)
                                strcpy(ach, localizedPrintf(player,ppowBase->pchDisplayName));

                            inherentCount++;
                        }
                    }
                }

                if (inherentCount)
                {
                    strcat(response->dialog, "<tr border=0><td border=0></td><td border=0>");
                    if (inherentCount == 1)
                    {
                        strcat(response->dialog, saUtilLocalize(player,"NewInherentPower", ach));
                    }
                    else
                    {
                        strcat(response->dialog, saUtilLocalize(player,"NewInherentPowers"));
                    }
                    strcat(response->dialog, "</td></tr>");
                }
            }

            iTmp = CountForLevel(iLevel+1, g_Schedules.aSchedules.piInspirationRow);
            if(iTmp>player->pchar->iNumInspirationRows)
            {
                strcat(response->dialog, "<tr border=0><td border=0></td><td border=0>");
                strcat(response->dialog, saUtilLocalize(player,"InspRowIncrease"));
                strcat(response->dialog, "</td></tr>");
            }
            iTmp = CountForLevel(iLevel+1, g_Schedules.aSchedules.piInspirationCol);
            if(iTmp>player->pchar->iNumInspirationCols)
            {
                strcat(response->dialog, "<tr border=0><td border=0></td><td border=0>");
                strcat(response->dialog, saUtilLocalize(player,"InspColIncrease"));
                strcat(response->dialog, "</td></tr>");
            }
            iTmp = character_CountBuyBoostForLevel(player->pchar, iLevel+1);
            if(iTmp>0)
            {
                strcat(response->dialog, "<tr border=0><td border=0></td><td border=0>");
                strcat(response->dialog, iTmp==1 ? saUtilLocalize(player,"PowerAssignSlot") : saUtilLocalize(player,"PowerAssignMultipleSlots", iTmp));
                strcat(response->dialog, "</td></tr>");
            }
            if(character_CanBuyPowerForLevel(player->pchar, iLevel+1))
            {
                strcat(response->dialog, "<tr border=0><td border=0></td><td border=0>");
                strcat(response->dialog, saUtilLocalize(player,"PowerNewPower"));
                strcat(response->dialog, "</td></tr>");
            }

            // checking for bonus inventory slots
            iTmp = character_GetInvTotalSizeByLevel(iLevel+1, kInventoryType_Salvage) - character_GetInvTotalSizeByLevel(iMaxLevel, kInventoryType_Salvage);
            if(iTmp>0)
            {
                strcat(response->dialog, "<tr border=0><td border=0></td><td border=0>");
                strcat(response->dialog, iTmp==1 ? saUtilLocalize(player,"NewSalvageInvSing") : saUtilLocalize(player,"NewSalvageInvPlur", iTmp));
                strcat(response->dialog, "</td></tr>");
            }
            iTmp = character_GetInvTotalSizeByLevel(iLevel+1, kInventoryType_Recipe) - character_GetInvTotalSizeByLevel(iMaxLevel, kInventoryType_Recipe);
            if(iTmp>0)
            {
                strcat(response->dialog, "<tr border=0><td border=0></td><td border=0>");
                strcat(response->dialog, iTmp==1 ? saUtilLocalize(player,"NewRecipeInvSing") : saUtilLocalize(player,"NewRecipeInvPlur", iTmp));
                strcat(response->dialog, "</td></tr>");
            }
            iTmp = character_GetInvTotalSizeByLevel(iLevel+1, kInventoryType_StoredSalvage) - character_GetInvTotalSizeByLevel(iMaxLevel, kInventoryType_StoredSalvage);
            if(iTmp>0)
            {
                strcat(response->dialog, "<tr border=0><td border=0></td><td border=0>");
                strcat(response->dialog, iTmp==1 ? saUtilLocalize(player,"NewStoredInvSing") : saUtilLocalize(player,"NewStoredInvPlur", iTmp));
                strcat(response->dialog, "</td></tr>");
            }
            iTmp = character_GetAuctionInvTotalSizeByLevel(iLevel+1) - character_GetAuctionInvTotalSizeByLevel(iMaxLevel);
            if(iTmp>0)
            {
                strcat(response->dialog, "<tr border=0><td border=0></td><td border=0>");
                strcat(response->dialog, iTmp==1 ? saUtilLocalize(player,"NewAuctionInvSing") : saUtilLocalize(player,"NewAuctionInvPlur", iTmp));
                strcat(response->dialog, "</td></tr>");
            }

        }
        if (class_IsRespecLevelUp(player->pchar->pclass, iLevel))
        {
            strcat(response->dialog, "<tr border=0><td border=0></td><td border=0>");
            strcat(response->dialog, saUtilLocalize(player, "RespecLevelUp"));
            strcat(response->dialog, "</td></tr>");
        }
        strcat(response->dialog, "</table><br><br>");
    }
}

static InfoResponse(Entity* player, ContactResponse *response, int id)
{
    static char pchBufText[16] = "InfoText_\0\0";
    static char pchBufPrompt[16] = "InfoPrompt_\0\0";
    static char *pchEndText = NULL;
    static char *pchEndPrompt = NULL;
    int i;
    int itemCount = 7; 

    if(!pchEndPrompt)
    {
        pchEndPrompt = pchBufPrompt+strlen(pchBufPrompt);
        pchEndText = pchBufText+strlen(pchBufText);
    }

    if(id>0)
    {
        *pchEndText = '0'+id;
        strcat(response->dialog, saUtilLocalize(player, pchBufText));
    }

    if (player->pchar->iLevel >= 2)
    {
        for(i=0; i<=itemCount; i++)
        {
            if(i!=id)
            {
                const char* localizedText;
                *pchEndPrompt = '0'+i;
                localizedText = saUtilLocalize(player,pchBufPrompt);

                // Only add the response if we have a string for this prompt
                if (stricmp(localizedText, pchBufPrompt) != 0)
                    AddResponse(response, localizedText, CONTACTLINK_INFO_1+i-1);
            }
        }
    }

    AddResponse(response, saUtilLocalize(player,"AwayIGo"), CONTACTLINK_BYE);
}

static int TrainerContact(Entity* player, const ContactDef* def, int link, ContactResponse* response)
{
    bool bSaidPreface = false;

    if (player->storyInfo->interactCell) return 0;
    switch (link)
    {
    case CONTACTLINK_HELLO:
    case CONTACTLINK_INFO_0:
    case CONTACTLINK_ENDTRAIN:
        // Force a sanity pass for their level and restart
        //   their auto powers. This is done in case something
        //   bad happens to their powers, which happens occasionally
        //   on the Test Server. It is not strictly needed, but lets
        //   players "self-heal" problems with their hero.
        // If they lose a non-inherent power, the trainer will catch
        //   it below and let them choose what they want.
        character_LevelFinalize(player->pchar, true);
        character_ActivateAllAutoPowers(player->pchar);
        character_EnterStance(player->pchar, NULL, 1);

        // allow the player to change unlimitedly at level 50,
        // otherwise allow the player to change at COMMON_TITLE_LEVEL and every 10 levels thereafter
        if (player->pchar->iLevel >= UNLIMITED_TITLE_LEVEL ||
            player->pchar->iLevel >= COMMON_TITLE_LEVEL && player->pl->iTitlesChosen <= (player->pchar->iLevel - COMMON_TITLE_LEVEL)/10)
        {
            bSaidPreface = true;
            strcat(response->dialog, saUtilLocalize(player, "WellDone", player->name));
            strcat(response->dialog, " ");
            strcat(response->dialog, saUtilLocalize(player,"EarnedATitle"));
            strcat(response->dialog, "<BR><BR>");

            AddResponse(response, saUtilLocalize(player,"ChooseTitleNow"), CONTACTLINK_CHOOSE_TITLE);
        }

        // If they can level, or if they didn't fully level before,
        // then let them train up.
        if(character_CanLevel(player->pchar))
        {
            int iMaxLevel = character_CalcExperienceLevel(player->pchar);

            // DGNOTE 10/1/2008 - not yet on this.  We need to make this smart enough to spot things like the
            // level/respec that VEAT's do at L24, and send the appropriate packet for that.  Further, if and when
            // we add other oddities to the level up system, they'll need to be handled here.
            if(0 && link == CONTACTLINK_ENDTRAIN)
            {
                player->pl->levelling = 1;
                START_PACKET(pak, player, SERVER_LEVEL_UP);
                pktSendBits(pak, kPowerSystem_Numbits, kPowerSystem_Powers);
                pktSendBitsPack(pak,4,character_GetLevel(player->pchar));
                END_PACKET

                character_VerifyPowerSets(player->pchar);
                AddResponse(response, saUtilLocalize(player,"AwayIGo"), CONTACTLINK_BYE);
                // Break out here, they're going to go level again
                break;
            }

            if(iMaxLevel>player->pchar->iLevel+1)
            {
                if(!bSaidPreface && rand()%3 == 0)
                {
                    strcat(response->dialog, saUtilLocalize(player,"WellDone", player->name));
                }
                // DGNOTE 9/19/2008 - This and the next strcat will need some work to handle level zero
                // characters leveling to 1.
                strcat(response->dialog, saUtilLocalize(player,"ReadyToLevelMulti", player->name,
                    iMaxLevel+1,
                    player->pchar->iLevel+1+1)); // +1 is since it's zero-based
            }
            else
            {
                strcat(response->dialog, saUtilLocalize(player,"ReadyToLevel", player->name,
                    player->pchar->iLevel+1+1)); // +1 is since it's zero-based
            }
            strcat(response->dialog, "<br><br>");

            if (character_PowersGetMaximumLevel(player->pchar) == MULTIBUILD_NOTIFY_LEVEL)
            {
                // DGNOTE 9/19/2008 - This could do to be integrated into ContactInteractionShowTrainingImprovements(), except we need to ensure it only shows when
                // build zero hits level 10, not just any build.
                strcat(response->dialog, textStd("MultiBuildsAvailable"));
                strcat(response->dialog, "<br><br>");
            }

            // Show them what they'll get.
            ContactInteractionShowTrainingImprovements(player, link, response);

            if(iMaxLevel==player->pchar->iLevel+1)
            {
                strcat(response->dialog, saUtilLocalize(player,"AfterTrainingLevel",
                    character_ExperienceNeeded(player->pchar)));
            }

            if (class_IsRespecLevelUp(player->pchar->pclass, player->pchar->iLevel))
            {
                AddResponse(response, saUtilLocalize(player,"TrainRespecNow"), CONTACTLINK_TRAINRESPEC);
            }
            else
            {
                AddResponse(response, saUtilLocalize(player,"TrainNow"), CONTACTLINK_TRAIN);
            }

            if (character_MaxBuildsAvailable(player) > 1)
            {
                AddResponse(response, textStd("SelectBuildOption"), CONTACTLINK_SWITCH_BUILD);
            }
            AddResponse(response, saUtilLocalize(player,"DontTrainNow"), CONTACTLINK_BYE);
        }
        else if(character_CanBuyPower(player->pchar)
            || character_CanBuyBoost(player->pchar))
        {
            strcat(response->dialog, saUtilLocalize(player,"SkippedTraining", player->name,
                player->pchar->iLevel+1)); // +1 since it's zero-based
            strcat(response->dialog, "<BR><BR>");
            strcat(response->dialog, saUtilLocalize(player,"AfterTrainingLevel",
                character_ExperienceNeeded(player->pchar)));
            strcat(response->dialog, "<BR><BR>");
            strcat(response->dialog, saUtilLocalize(player,"PromptTrain"));

            AddResponse(response, saUtilLocalize(player,"TrainNow"), CONTACTLINK_TRAIN);

            if (character_MaxBuildsAvailable(player) > 1)
            {
                AddResponse(response, textStd("SelectBuildOption"), CONTACTLINK_SWITCH_BUILD);
            }

            AddResponse(response, saUtilLocalize(player,"DontTrainNow"), CONTACTLINK_BYE);
        }
        else
        {
            int iXP = character_ExperienceNeeded(player->pchar);

            if(iXP && !bSaidPreface)
            {
                strcat(response->dialog, saUtilLocalize(player,"DoingWell", player->name));
                strcat(response->dialog, "<BR><BR>");
            }

            if(iXP)
            {
                strcat(response->dialog, saUtilLocalize(player,"NeedToLevel",
                    iXP, player->pchar->iLevel+1+1)); // +1 since it's zero-based
                strcat(response->dialog, "<BR><BR>");
                strcat(response->dialog, saUtilLocalize(player,"GoOutThere"));
            }
            else
            {
                strcat(response->dialog, saUtilLocalize(player,"AtMaxLevel"));
            }

            if (character_MaxBuildsAvailable(player) > 1)
            {
                AddResponse(response, textStd("SelectBuildOption"), CONTACTLINK_SWITCH_BUILD);
            }

            InfoResponse(player, response, 0);
        }
        break;

    case CONTACTLINK_CHOOSE_TITLE:
        if(player->pchar->iLevel >= UNLIMITED_TITLE_LEVEL ||
           player->pchar->iLevel >= COMMON_TITLE_LEVEL && player->pl->iTitlesChosen <= (player->pchar->iLevel - COMMON_TITLE_LEVEL)/10)
        {
            START_PACKET(pak, player, SERVER_NEW_TITLE);
            pktSendBits(pak, 1, player->pchar->iLevel >= ORIGIN_TITLE_LEVEL);
            pktSendBits(pak, 1, accountLoyaltyRewardHasNode(player->pl->loyaltyStatus, "BaseTeleporter"));
            END_PACKET

            AddResponse(response, saUtilLocalize(player,"AwayIGo"), CONTACTLINK_BYE);
        }
        break;

    case CONTACTLINK_TRAIN:
        {
            player->pl->levelling = 1;
            START_PACKET(pak, player, SERVER_LEVEL_UP);
            pktSendBits(pak, kPowerSystem_Numbits, kPowerSystem_Powers);
            pktSendBitsPack(pak,4,character_GetLevel(player->pchar));
            END_PACKET

            character_VerifyPowerSets(player->pchar);
            AddResponse(response, saUtilLocalize(player,"AwayIGo"), CONTACTLINK_BYE);
        }
        break;

    case CONTACTLINK_TRAINRESPEC:
        {
            // Need to know we'll be getting an extra level's worth of stuff
            // and that we'll be in the respec menu for the time being.
            player->pl->levelling = 1;
            player->pl->isRespeccing = 1;
            START_PACKET(pak, player, SERVER_LEVEL_UP_RESPEC);
            pktSendBits(pak, kPowerSystem_Numbits, kPowerSystem_Powers);
            pktSendBitsPack(pak,4,character_GetLevel(player->pchar));
            END_PACKET

            AddResponse(response, saUtilLocalize(player,"AwayIGo"), CONTACTLINK_BYE);
        }
        break;

    case CONTACTLINK_SWITCH_BUILD:
        {
            int i;
            int max_build;
            int cur_build;
            int seconds;
            int minutes;

            // Maybe alter this so that accesslevel 10 characters don't have to wait?
            seconds = player->pchar->iBuildChangeTime - timerSecondsSince2000();
            
            if (seconds > 0 && player->access_level == 0)
            {
                minutes = seconds / 60;
                seconds = seconds % 60;

                strcat(response->dialog, textStd("CantChangeBuild"));
                strcat(response->dialog, " ");
                if (minutes == 0)
                {
                    // No minutes text at all
                }
                else if (minutes == 1)
                {
                    strcat(response->dialog, textStd("MinutesSingular", minutes));
                    strcat(response->dialog, " ");
                }
                else
                {
                    strcat(response->dialog, textStd("MinutesPlural", minutes));
                    strcat(response->dialog, " ");
                }
                if (seconds == 1)
                {
                    strcat(response->dialog, textStd("SecondsSingular", seconds));
                }
                else
                {
                    strcat(response->dialog, textStd("SecondsPlural", seconds));
                }
                AddResponse(response, textStd("BuildContinue"), CONTACTLINK_HELLO);
                break;
            }

            max_build = character_MaxBuildsAvailable(player);
            cur_build = character_GetActiveBuild(player);
            strcat(response->dialog, textStd("SelectYourBuild"));
            strcat(response->dialog, "<br><br>");

            // build change menu
            strcat(response->dialog, "<linkhoverbg #118866aa><link white><linkhover white>");
            strcat(response->dialog, "<table>");
            for (i = 0; i < max_build; i++)
            {
                if (i != cur_build)
                {
                    const char* link = StaticDefineIntRevLookup(ParseContactLink, CONTACTLINK_SELECTBUILD_1+i);
                    strcatf(response->dialog, "<a href=%s>", link);
                    strcat(response->dialog, "<tr width=90%><td width=50% highlight=0>");
                    strcatf(response->dialog, "<a href=%s>", link);
                    if (player->pl->buildNames[i][0])
                    {
                        strcat(response->dialog, textStd("SelectBuildNamedStr", player->pl->buildNames[i]));
                    }
                    else
                    {
                        strcat(response->dialog, textStd("SelectBuildStr", i + 1));
                    }
                    strcat(response->dialog, "</a>");
                    strcat(response->dialog, "</td><td width=40% highlight=0>");
                    strcatf(response->dialog, "<a href=%s>", link);
                    // If we get flak about "unused" builds showing 1 rather than zero, detect that fact here and adjust
                    strcat(response->dialog, textStd("BuildSecurityLevel", player->pchar->iBuildLevels[i] + 1));
                    strcat(response->dialog, "</a>");
                    strcat(response->dialog, "</td></tr>");
                    strcat(response->dialog, "</a>");
                }
                else
                {
                    strcat(response->dialog, "<tr width=90%><td width=50% highlight=0>");
                    if (player->pl->buildNames[i][0])
                    {
                        strcat(response->dialog, textStd("CurrentBuildNamedStr", player->pl->buildNames[i]));
                    }
                    else
                    {
                        strcat(response->dialog, textStd("CurrentBuildStr", i + 1));
                    }
                    strcat(response->dialog, "</td><td width=40% highlight=0>");
                    // If we get flak about "unused" builds showing 1 rather than zero, detect that fact here and adjust
                    strcat(response->dialog, textStd("BuildSecurityLevel", player->pchar->iBuildLevels[i] + 1));
                    strcat(response->dialog, "</td></tr>");
                }
            }
            strcat(response->dialog, "</table>");

            AddResponse(response, textStd("RenameBuildStr"), CONTACTLINK_RENAME_BUILD);
            AddResponse(response, saUtilLocalize(player,"DontTrainNow"), CONTACTLINK_HELLO);
        }
        break;

    case CONTACTLINK_INFO_1:
    case CONTACTLINK_INFO_2:
    case CONTACTLINK_INFO_3:
    case CONTACTLINK_INFO_4:
    case CONTACTLINK_INFO_5:
    case CONTACTLINK_INFO_6:
    case CONTACTLINK_INFO_7:
    case CONTACTLINK_INFO_8:
        InfoResponse(player, response, (link - CONTACTLINK_INFO_1) + 1);
        break;

    case CONTACTLINK_SELECTBUILD_1:
    case CONTACTLINK_SELECTBUILD_2:
    case CONTACTLINK_SELECTBUILD_3:
    case CONTACTLINK_SELECTBUILD_4:
    case CONTACTLINK_SELECTBUILD_5:
    case CONTACTLINK_SELECTBUILD_6:
    case CONTACTLINK_SELECTBUILD_7:
    case CONTACTLINK_SELECTBUILD_8:
        {
            int new_build = link - CONTACTLINK_SELECTBUILD_1;
            int cur_build = character_GetActiveBuild(player);

            if (new_build < character_MaxBuildsAvailable(player) && new_build != cur_build)
            {
                if (player->pl->buildNames[cur_build][0])
                {
                    strcat(response->dialog, textStd("CurrentBuildNamedStr", player->pl->buildNames[cur_build]));
                }
                else
                {
                    strcat(response->dialog, textStd("CurrentBuildStr", cur_build + 1));
                }
                strcat(response->dialog, "<br><br>");
                if (player->pl->buildNames[new_build][0])
                {
                    strcat(response->dialog, textStd("ConfirmBuildSwitchNamedStr", player->pl->buildNames[new_build]));
                }
                else
                {
                    strcat(response->dialog, textStd("ConfirmBuildSwitchStr", new_build + 1));
                }
                AddResponse(response, textStd("BuildConfirm"), new_build + CONTACTLINK_CONFIRMBUILD_1);
                AddResponse(response, textStd("BuildCancel"), CONTACTLINK_HELLO);
                break;
            }
            return 0;
            break;
        }

    case CONTACTLINK_CONFIRMBUILD_1:
    case CONTACTLINK_CONFIRMBUILD_2:
    case CONTACTLINK_CONFIRMBUILD_3:
    case CONTACTLINK_CONFIRMBUILD_4:
    case CONTACTLINK_CONFIRMBUILD_5:
    case CONTACTLINK_CONFIRMBUILD_6:
    case CONTACTLINK_CONFIRMBUILD_7:
    case CONTACTLINK_CONFIRMBUILD_8:
    {
        int iBuild = link - CONTACTLINK_CONFIRMBUILD_1;

        if (iBuild < character_MaxBuildsAvailable(player) && iBuild != character_GetActiveBuild(player))
        {
            character_SelectBuild(player, iBuild, 1);
        }
        return 0;
        break;
    }

    case CONTACTLINK_RENAME_BUILD:
    {
        START_PACKET(pak, player, SERVER_RENAME_BUILD);
        END_PACKET

        return 0;
        break;
    }

    case CONTACTLINK_BYE:
        {
            // Done with conversation.
            return 0;
        }
        break;
    }
    return 1;
}

static int PvPSwitchContact(Entity* player, Contact* contact, int link, ContactResponse* response)
{
    bool curswitch = player->pl->pvpSwitch;
    char availablestr[500];
    bool available = false;

    memset(response, 0, sizeof(ContactResponse));

    if (player->storyInfo->interactCell) return 0;

    if (player->teamup && player->teamup->members.count > 1)
    {
        strncpyt(availablestr, saUtilLocalize(player,"PvPSwitchContactTeam"), ARRAY_SIZE(availablestr));
    }
    else if (player->pl->inviter_dbid)
    {
        strncpyt(availablestr, saUtilLocalize(player,"PvPSwitchContactInvite"), ARRAY_SIZE(availablestr));
    }
    else
    {
        available = true;
        strncpyt(availablestr, saUtilLocalize(player,"PvPSwitchContactOK"), ARRAY_SIZE(availablestr));
    }

    switch (link)
    {
    case CONTACTLINK_HELLO:
    case CONTACTLINK_MAIN:
        // hello, etc.
        strcpy(response->dialog, ContactHeadshot(contact, player, 0));
        strcat(response->dialog, saUtilLocalize(player,"PvPSwitchContactCurrent", player, saUtilLocalize(player,curswitch?"PvPSwitchContactCurrentOn":"PvPSwitchContactCurrentOff")));
        strcat(response->dialog, "<br><br>");
        strcat(response->dialog, availablestr);
        if (available) AddResponse(response, saUtilLocalize(player,curswitch?"PvPSwitchContactToggleOff":"PvPSwitchContactToggleOn"), CONTACTLINK_PVPSWITCH);
        break;

    case CONTACTLINK_PVPSWITCH:
        strcpy(response->dialog, ContactHeadshot(contact, player, 0));

        if (available)
        {
            pvp_SetSwitch(player, !curswitch);
            strcat(response->dialog, saUtilLocalize(player,"PvPSwitchContactChanged", player, saUtilLocalize(player,player->pl->pvpSwitch?"PvPSwitchContactCurrentOn":"PvPSwitchContactCurrentOff")));
        }
        else
        {
            strcat(response->dialog, availablestr);
        }
        break;

    case CONTACTLINK_BYE:
    default:
        return 0;
    }

    AddResponse(response, saUtilLocalize(player,"Leave"), CONTACTLINK_BYE);
    return 1;
}


#define INF_TO_PRESTIGE_BATCH_INFLUENCE (1000000)
#define INF_TO_PRESTIGE_BATCH_PRESTIGE (2000)

static bool ConvertInfToPrestige(Entity *e, U32 inf)
{
    U32 batches = (inf/INF_TO_PRESTIGE_BATCH_INFLUENCE);
    U32 prestige = batches*INF_TO_PRESTIGE_BATCH_PRESTIGE;
    int roundedinf = batches*INF_TO_PRESTIGE_BATCH_INFLUENCE;

    if(!e || !e->pchar || !e->supergroup || !e->pl || batches==0 || e->pchar->iInfluencePoints<roundedinf)
        return false;

    if( e->supergroup && teamLock(e, CONTAINER_SUPERGROUPS ))
    {
        dbLog("Character:ConvertInfToPrestige", e,
            "Converted %u inf to %u prestige for supergroup %d:%s",
            inf, prestige, e->supergroup_id, e->supergroup->name);

        ent_AdjInfluence(e, -roundedinf, NULL);
        e->pl->prestige += prestige;
        e->supergroup->prestige += prestige;
        e->supergroup_update = true;

        badge_StatAdd(e, "inf.convert", roundedinf);
        badge_RecordPrestigeBought(e, prestige);
        teamUpdateUnlock(e,CONTAINER_SUPERGROUPS);

        sgroup_UpdateAllMembersAnywhere(e->supergroup);

        return true;
    }

    return false;
}

static int RegistrarContact(Entity* player, Contact* contact, int link, ContactResponse* response)
{
    bool bSaidPreface = false;
    int rent = (player && player->supergroup) ? player->supergroup->upkeep.prtgRentDue : 0;
    int base_cost = 0; // designers decided no base cost, for now.


    if (player->storyInfo->interactCell) return 0;

    switch (link)
    {
    case CONTACTLINK_HELLO:
    case CONTACTLINK_INFO_0:
        {
            strcpy(response->dialog, ContactHeadshot(contact, player, 0));
              strcat(response->dialog, saUtilLocalize(player, "RegistrarGreeting", player->name ));

            if( player->supergroup )
            {

                // ----------
                // rent payment

                if( sgroup_canPayRent(player) && player && player->pl && 
                    AccountCanCreateSG(&player->pl->account_inventory, player->pl->loyaltyPointsEarned, player->pl->account_inventory.accountStatusFlags))
                {
                    // Want to pay rent?
                    strcat(response->dialog, "<br><br>" );
                    strcat(response->dialog, saUtilLocalize(player, "RegistrarBaseNotice"));
                    AddResponse(response, saUtilLocalize(player,"RegistrarPayRentResponse"), CONTACTLINK_PAY_RENT);
                }

                // ----------
                // purchase base

                if( sgroup_hasPermission( player, SG_PERM_BASE_EDIT ) && player && player->pl && 
                    AccountCanCreateSG(&player->pl->account_inventory, player->pl->loyaltyPointsEarned, player->pl->account_inventory.accountStatusFlags))
                {
                    if( !player->supergroup )
                    {
                        // Create a Base?
                        strcat(response->dialog, "<br><br>" );
                        strcat(response->dialog, saUtilLocalize( player,"RegistrarNoBaseYet"));
                        AddResponse(response, saUtilLocalize(player,"RegistrarBuyBaseResponse"), CONTACTLINK_BUY_BASE);
                    }

                }
                else
                {
                    // you can't do anything special
                }
                // DPG 9/24/2009 - TTP 20263
                // Moved this outside of any permissions checks, so that all players have the option of purchasing prestige
                // Influence to Prestige conversion
                if(player->pchar->iInfluencePoints >= INF_TO_PRESTIGE_BATCH_INFLUENCE)
                {
                    AddResponse(response, saUtilLocalize(player,"RegistrarConvertInfStartResponse"), CONTACTLINK_CONVERT_INF_START);
                }

            }
            else
            {
                if(player->pchar->iLevel<9)
                {
                    // cant create supergroup yet
                    strcat(response->dialog, "<br><br>" );
                    strcat(response->dialog, saUtilLocalize(player, "RegistrarTooLowForCreate"));
                }
                else if (player && player->pl && 
                    AccountCanCreateSG(&player->pl->account_inventory, player->pl->loyaltyPointsEarned, player->pl->account_inventory.accountStatusFlags))
                {
                    // ask if you would like to create sg
                    strcat(response->dialog, "<br><br>" );
                    strcat(response->dialog, saUtilLocalize(player, "RegistrarCanCreate"));
                    AddResponse(response, saUtilLocalize(player,"RegistrarCreateSgResponse"), CONTACTLINK_CREATE_SG);
                }
            }

            AddResponse(response, saUtilLocalize(player,"RegistrarViewSuperGroupListResponse"), CONTACTLINK_VIEW_SGLIST);
            AddResponse(response, saUtilLocalize(player,"RegistrarAwayIGoResponse"), CONTACTLINK_BYE);

        }break;

    case CONTACTLINK_FAILEDREQUIRES:
        {
            strcpy(response->dialog, ContactHeadshot(contact, player, 0));
            strcat(response->dialog, saUtilLocalize(player, "RegistrarCantUseTrial", player->name ));
            
            AddResponse(response, saUtilLocalize(player,"RegistrarAwayIGoResponse"), CONTACTLINK_BYE);
        }break;
    case CONTACTLINK_CREATE_SG:
        {
            START_PACKET(pak, player, SERVER_REGISTER_SUPERGROUP);
            pktSendString(pak, contact->def->displayname);
            END_PACKET
            return 0;
        }break;

    case CONTACTLINK_PAY_RENT:
        {
            strcpy(response->dialog, ContactHeadshot(contact, player, 0));

            if(!player->supergroup)
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarNotInGroup"));
                AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_BYE);
                break;
            }

            if( rent > 0 )
            {
                if( sgroup_canPayRent(player) )
                {
                    if( player->supergroup->prestige >= rent )
                    {
                        strcat(response->dialog, saUtilLocalize(player, "RegistrarRentDue", rent, player->supergroup->prestige) );
                        AddResponse(response, saUtilLocalize(player,"RegistrarPayNowResponse"), CONTACTLINK_PAID_RENT);
                        AddResponse(response, saUtilLocalize(player,"RegistrarNoNevermindResponse"), CONTACTLINK_HELLO);
                    }
                    else
                    {
                        strcat(response->dialog, saUtilLocalize(player, "RegistrarRentDueTooMuch", rent, player->supergroup->prestige) );
                        AddResponse(response, saUtilLocalize(player,"RegistrarOhNevermindResponse"), CONTACTLINK_HELLO);
                    }
                }
                else
                {
                    strcat(response->dialog, saUtilLocalize(player, "RegistrarCantPayRent") );
                    AddResponse(response, saUtilLocalize(player,"RegistrarOhNevermindResponse"), CONTACTLINK_HELLO);

                }
            }
            else
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarNoRentDue") );
                AddResponse(response, saUtilLocalize(player,"RegistrarOhNevermindResponse"), CONTACTLINK_HELLO);
            }


        }break;

    case CONTACTLINK_PAID_RENT:
        {
            int rentPaid = sgroup_payRent( player );
            strcpy(response->dialog, ContactHeadshot(contact, player, 0));

            if(!player->supergroup)
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarNotInGroup"));
                AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_BYE);
                break;
            }

            if( rentPaid > 0 )
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarRentPaid", rentPaid, player->supergroup->prestige) );
            }
            else
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarRentFailed" ) );
            }

            AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_HELLO);
        }break;

    case CONTACTLINK_BUY_BASE:
        {
             strcpy(response->dialog, ContactHeadshot(contact, player, 0));

            if(!player->supergroup)
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarNotInGroup"));
                AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_BYE);
                break;
            }

            if( player->supergroup->prestige >= base_cost )
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarNewBaseCost",base_cost, player->supergroup->prestige) );
                AddResponse(response, saUtilLocalize(player,"RegistrarBuyBaseNowResponse"), CONTACTLINK_BOUGHT_BASE);
                AddResponse(response, saUtilLocalize(player,"RegistrarNoNevermindResponse"), CONTACTLINK_HELLO);
            }
            else
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarNewBaseCostTooMuch",base_cost, player->supergroup->prestige) );
                AddResponse(response, saUtilLocalize(player,"RegistrarOhNevermindResponse"), CONTACTLINK_HELLO);
            }
        }break;

    case CONTACTLINK_BOUGHT_BASE:
        {
            strcpy(response->dialog, ContactHeadshot(contact, player, 0));

            if(!player->supergroup)
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarNotInGroup"));
                AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_BYE);
                break;
            }

            if( sgroup_buyBase( player ) )
                strcat(response->dialog, saUtilLocalize(player, "RegistrarBaseBought", rent, player->supergroup->prestige) );
            else
                strcat(response->dialog, saUtilLocalize(player, "RegistrarBaseBuyFailed") );

            AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_HELLO);
        }break;

    case CONTACTLINK_CONVERT_INF_START:
        {
            U32 uInf;

            strcpy(response->dialog, ContactHeadshot(contact, player, 0));

            if(!player->supergroup)
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarNotInGroup"));
                AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_BYE);
                break;
            }

            strcat(response->dialog, saUtilLocalize(player, "RegistrarConvertInfInfo", INF_TO_PRESTIGE_BATCH_INFLUENCE,INF_TO_PRESTIGE_BATCH_PRESTIGE,player->supergroup->name,player->pchar->iInfluencePoints) );

            if(player->pchar->iInfluencePoints >= INF_TO_PRESTIGE_BATCH_INFLUENCE)
            {
                AddResponse(response,
                    saUtilLocalize(player,"RegistrarConvertInfResponse", INF_TO_PRESTIGE_BATCH_INFLUENCE, INF_TO_PRESTIGE_BATCH_PRESTIGE),
                    CONTACTLINK_CONVERT_INF_1);
            }
            if(player->pchar->iInfluencePoints >= 10*INF_TO_PRESTIGE_BATCH_INFLUENCE)
            {
                AddResponse(response,
                    saUtilLocalize(player,"RegistrarConvertInfResponse", 10*INF_TO_PRESTIGE_BATCH_INFLUENCE, 10*INF_TO_PRESTIGE_BATCH_PRESTIGE),
                    CONTACTLINK_CONVERT_INF_10);
            }
            if(player->pchar->iInfluencePoints >= 100*INF_TO_PRESTIGE_BATCH_INFLUENCE)
            {
                AddResponse(response,
                    saUtilLocalize(player,"RegistrarConvertInfResponse", 100*INF_TO_PRESTIGE_BATCH_INFLUENCE, 100*INF_TO_PRESTIGE_BATCH_PRESTIGE),
                    CONTACTLINK_CONVERT_INF_100);
            }

            uInf = player->pchar->iInfluencePoints/INF_TO_PRESTIGE_BATCH_INFLUENCE;
            if(uInf>0 && uInf!=1 && uInf!=10 && uInf!=100)
            {
                U32 uPres = uInf*INF_TO_PRESTIGE_BATCH_PRESTIGE;
                uInf *= INF_TO_PRESTIGE_BATCH_INFLUENCE;
                AddResponse(response,
                    saUtilLocalize(player,"RegistrarConvertInfResponse", uInf, uPres),
                    CONTACTLINK_CONVERT_INF_ALL);
            }

            AddResponse(response, saUtilLocalize(player,"RegistrarOhNevermindResponse"), CONTACTLINK_HELLO);
        }
        break;

    case CONTACTLINK_CONVERT_INF_1:
        {
            strcpy(response->dialog, ContactHeadshot(contact, player, 0));

            if(!player->supergroup)
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarNotInGroup"));
                AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_BYE);
                break;
            }

            if(ConvertInfToPrestige(player, INF_TO_PRESTIGE_BATCH_INFLUENCE))
                strcat(response->dialog, saUtilLocalize(player, "RegistrarConvertedInf", INF_TO_PRESTIGE_BATCH_INFLUENCE, INF_TO_PRESTIGE_BATCH_PRESTIGE) );
            else
                strcat(response->dialog, saUtilLocalize(player, "RegistrarConvertUnable") );

            AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_HELLO);
        }
        break;

    case CONTACTLINK_CONVERT_INF_10:
        {
            strcpy(response->dialog, ContactHeadshot(contact, player, 0));

            if(!player->supergroup)
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarNotInGroup"));
                AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_BYE);
                break;
            }

            if(ConvertInfToPrestige(player, 10*INF_TO_PRESTIGE_BATCH_INFLUENCE))
                strcat(response->dialog, saUtilLocalize(player, "RegistrarConvertedInf", 10*INF_TO_PRESTIGE_BATCH_INFLUENCE, 10*INF_TO_PRESTIGE_BATCH_PRESTIGE) );
            else
                strcat(response->dialog, saUtilLocalize(player, "RegistrarConvertUnable") );

            AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_HELLO);
        }
        break;

    case CONTACTLINK_CONVERT_INF_100:
        {
            strcpy(response->dialog, ContactHeadshot(contact, player, 0));

            if(!player->supergroup)
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarNotInGroup"));
                AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_BYE);
                break;
            }

            if(ConvertInfToPrestige(player, 100*INF_TO_PRESTIGE_BATCH_INFLUENCE))
                strcat(response->dialog, saUtilLocalize(player, "RegistrarConvertedInf", 100*INF_TO_PRESTIGE_BATCH_INFLUENCE, 100*INF_TO_PRESTIGE_BATCH_PRESTIGE) );
            else
                strcat(response->dialog, saUtilLocalize(player, "RegistrarConvertUnable") );

            AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_HELLO);
        }
        break;

    case CONTACTLINK_CONVERT_INF_ALL:
        {
            U32 uInf = (player->pchar->iInfluencePoints/INF_TO_PRESTIGE_BATCH_INFLUENCE);
            U32 uPres = uInf*INF_TO_PRESTIGE_BATCH_PRESTIGE;
            uInf *= INF_TO_PRESTIGE_BATCH_INFLUENCE;

            strcpy(response->dialog, ContactHeadshot(contact, player, 0));

            if(!player->supergroup)
            {
                strcat(response->dialog, saUtilLocalize(player, "RegistrarNotInGroup"));
                AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_BYE);
                break;
            }

            if(ConvertInfToPrestige(player, uInf))
                strcat(response->dialog, saUtilLocalize(player, "RegistrarConvertedInf", uInf, uPres) );
            else
                strcat(response->dialog, saUtilLocalize(player, "RegistrarConvertUnable") );

            AddResponse(response, saUtilLocalize(player,"RegistrarOkResponse"), CONTACTLINK_HELLO);
        }
        break;

    case CONTACTLINK_VIEW_SGLIST:
        {
            sgroup_sendRegistrationList(player,0,NULL);
            return 0;
        }break;

    case CONTACTLINK_BYE:
        {
            // Done with conversation.
            return 0;
        }break;
    }
    return 1;
}


static int FlashbackContact(Entity* player, ContactHandle contactHandle, int link, ContactResponse* response)
{ 
    ScriptVarsTable vars = {0};
    Contact* contact = GetContact(contactHandle);
    char buf[1000];
    Vec3 pos;
    int mapID;
    const MapSpec* mapSpec;
    int haveloc = 0;
    int characterLevel = character_GetSecurityLevelExtern(player->pchar); // +1 since iLevel is zero-based.


    if (!contact) 
        return 0;

    // don't let players already on TF interact with Flashback Contact
    if (PlayerInTaskForceMode(player) && link != CONTACTLINK_BYE && !TaskForceIsOnFlashback(player)) 
    {
        strcat(response->dialog, saUtilLocalize(player,"PlayerInSGMode"));
        strcat(response->dialog, "<BR>");
        AddResponse(response, saUtilLocalize(player,"Leave"), CONTACTLINK_BYE);
        return 1;
    }
        
    // can't call the flashback contacts on cell phones
    if (player->storyInfo->interactCell) 
        return 0;

    // ScriptVars HACK: there has been no seed established for this interaction context
    saBuildScriptVarsTable(&vars, player, player->db_id, contactHandle, NULL, NULL, NULL, NULL, NULL, 0, 0, false);

    if (link != CONTACTLINK_BYE && player->pchar && 
        character_GetSecurityLevelExtern(player->pchar) < contact->def->minPlayerLevel)
    {
        if (contact->def->comeBackLater[0] == '[')
        {
            // This is a dialog tree hook, entering special case code here
            Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
            char *comeBackLaterCopy = strdup(contact->def->comeBackLater + 1);
            char *dialogName = strtok(comeBackLaterCopy, ":");
            char *pageName = strtok(0, "]"); // note that there may be more characters after the ] that are ignored
            if (dialogName && pageName) // otherwise this isn't a valid dialog tree hook, continue with normal contact
            {
                const DialogDef *dialog = DialogDef_FindDefFromContact(contact->def, dialogName);
                int hash = hashStringInsensitive(pageName);
                memset(response, 0, sizeof(ContactResponse)); // reset the response
                if (DialogDef_FindAndHandlePage(player, target, dialog, hash, 0, response, contactHandle, &vars))
                {
                    free(comeBackLaterCopy);
                    return 1;
                }
            }
            free(comeBackLaterCopy);
        }

        strcat(response->dialog, saUtilTableAndEntityLocalize(contact->def->comeBackLater, &vars, player));
        strcat(response->dialog, "<BR>");
        AddResponse(response, saUtilLocalize(player,"Leave"), CONTACTLINK_BYE);
        return 1;
    }

    switch (link)
    {
    case CONTACTLINK_HELLO:
        {
            strcpy(response->dialog, ContactHeadshot(contact, player, 0));
            if (TaskForceIsOnFlashback(player))
            {
                strcat(response->dialog, textStd("TeleportToContactString")); 
                AddResponse(response, saUtilLocalize(player,"FlashbackShowTeleport"), CONTACTLINK_FLASHBACK_TELEPORT);
            } else {
                strcat(response->dialog, saUtilTableAndEntityLocalize(contact->def->hello, &vars, player));
                AddResponse(response, textStd("FlashbackShowList"), CONTACTLINK_FLASHBACK_LIST);
            }
            AddResponse(response, saUtilLocalize(player,"Leave"), CONTACTLINK_BYE);

        }
        break;
    case CONTACTLINK_FLASHBACK_LIST:
        {
            response->type = CD_FLASHBACK_LIST;

            AddResponse(response, saUtilLocalize(player,"Leave"), CONTACTLINK_BYE);
        }
        break;
    case CONTACTLINK_FLASHBACK_TELEPORT:
        if (!PlayerTaskForceContact(player))
        {
            strcat(response->dialog, textStd("DontKnowWhereContactIs"));
            return 1;
        }
        haveloc = ContactFindLocation(PlayerTaskForceContact(player), &mapID, &pos);
        if (!haveloc)
        {
            strcat(response->dialog, textStd("DontKnowWhereContactIs"));
            return 1;
        }

        // use teleport helper function
        if(mapID == db_state.base_map_id)
            mapID = db_state.map_id;

        mapSpec = MapSpecFromMapId(mapID);

        if (player)
        {
            if (mapSpec)
            {
                if (characterLevel < mapSpec->entryLevelRange.min)
                {
                    strcat(response->dialog, localizedPrintf(player, "TooLowToEnter", 
                            mapSpec->entryLevelRange.min) );
                    return 1;
                }
                else if (characterLevel > mapSpec->entryLevelRange.max)
                {
                    strcat(response->dialog, localizedPrintf(player,"TooHighToEnter", 
                            mapSpec->entryLevelRange.max));
                    return 1;
                }
            }

            if (haveloc) 
            {
                sprintf(buf, "mapmovepos \"%s\" %d %f %f %f 0 1", player->name, mapID, pos[0], pos[1], pos[2]);
                serverParseClient(buf, NULL);
            }

            // Done with conversation.
            return 0;
        }

        break;
    case CONTACTLINK_BYE:
        {
            // Done with conversation.
            return 0;
        }break;
    }
    return 1;
}

static StoryTaskInfo *DeliveryTaskContactDialogFindTask(Entity *pPlayer, DialogContext *pContext)
{
    Entity *pOwner = entFromDbId(pContext->ownerDBID);

    // check for active task first
    if (pPlayer->teamup && pPlayer->teamup->activetask)
    {
        StoryTaskInfo *pTask = pPlayer->teamup->activetask;
        if ( isSameStoryTask(&pTask->sahandle, &pContext->sahandle ) &&    pContext->ownerDBID == pTask->assignedDbId)
            return pTask;
    }

    if (pOwner != NULL)
        return TaskGetInfo(pOwner, &pContext->sahandle);

    return NULL;
}

// in task.c
extern int *remoteDialogLookupContactHandleKeys;
extern void **remoteDialogLookupTables;

// called when the client sends a dialog response to a delivery target dialog tree
// TODO:  Remove direct references to player->storyInfo in this code and any other places,
//        use a function that handles taskforces properly.  I'm fairly sure this is potentially
//        bad, but not 100% sure....  ARM
int DeliveryTaskContact(Entity* player, int link, ContactResponse* response)
{
    DialogContext *pContext = NULL;
    StoryTaskInfo *pTask = NULL;
    int remoteIndex = 0;
    ContactInteractionContext context = {0};
    ScriptVarsTable vars = {0};
    StoryInfo *info = 0;
    StoryContactInfo *contactInfo = 0;
    Entity *interactEntity = erGetEnt(player->storyInfo->interactEntityRef);
    int returnMe = false;

    if (!response || !player)
        return false;

    memset(response, 0, sizeof(ContactResponse));

    if (!player->storyInfo || !player->storyInfo->interactDialogTree) 
        return false;

    // Find/remove the context
    if (interactEntity && interactEntity->dialogLookup)
    {
        if (link == CONTACTLINK_BYE)
            stashIntRemovePointer(interactEntity->dialogLookup, player->db_id, &pContext);
        else
            stashIntFindPointer(interactEntity->dialogLookup, player->db_id, &pContext);

        if (!pContext)
            return false;
    }
    else if (player->storyInfo->interactTarget)
    {
        StashTable remoteTable;

        if (!remoteDialogLookupTables)
            eaCreate(&remoteDialogLookupTables);

        if (-1 != (remoteIndex = eaiFind(&remoteDialogLookupContactHandleKeys, player->storyInfo->interactTarget)))
        {
            remoteTable = remoteDialogLookupTables[remoteIndex];
        }
        else
        {
            remoteIndex = eaiSize(&remoteDialogLookupContactHandleKeys);
            eaiPush(&remoteDialogLookupContactHandleKeys, player->storyInfo->interactTarget);
            remoteTable = stashTableCreateInt(4);
            eaPush(&remoteDialogLookupTables, remoteTable);
        }

        if (link == CONTACTLINK_BYE)
            stashIntRemovePointer(remoteTable, player->db_id, &pContext);
        else
            stashIntFindPointer(remoteTable, player->db_id, &pContext);

        if (!stashGetOccupiedSlots(remoteTable))
        {
            StashTable theTable = cpp_reinterpret_cast(StashTable)(eaRemove(&remoteDialogLookupTables, remoteIndex));
            eaiRemove(&remoteDialogLookupContactHandleKeys, remoteIndex);
            stashTableDestroy(theTable);
        }

        if (!pContext)
            return false;
    }
    else
    {
        return false;
    }

    if (player->storyInfo->interactTarget &&
        (link == CONTACTLINK_MAIN
         || link == CONTACTLINK_MISSIONS
         || link == CONTACTLINK_INTRODUCE
         || link == CONTACTLINK_GOTOSTORE
         || link == CONTACTLINK_ABOUT
         || link == CONTACTLINK_AUTO_COMPLETE
         || link == CONTACTLINK_ABANDON
         || link == CONTACTLINK_DISMISS
         || link == CONTACTLINK_WEBSTORE))
    {
        const ContactDef *contactDef = ContactDefinition(player->storyInfo->interactTarget);

        info = StoryArcLockInfo(player);

        contactInfo = ContactGetInfo(info, player->storyInfo->interactTarget);

        if (!contactInfo && !PlayerInTaskForceMode(player)
            && CONTACT_IS_ISSUEONINTERACT(contactDef)
            && (!contactDef->interactionRequires || chareval_Eval(player->pchar, contactDef->interactionRequires, contactDef->filename)))
        {
            ContactAdd(player, info, player->storyInfo->interactTarget, "Issued on interact in Delivery Task Contact");
            contactInfo = ContactGetInfo(info, player->storyInfo->interactTarget);
        }

        if (!contactInfo || !contactInfo->contact->def)
        {
            StoryArcUnlockInfo(player);
            return false;
        }

        // Setup a ContactInteractionContext
        context.link = link;
        context.player = player;
        context.info = info;
        context.contactInfo = contactInfo;
        context.response = response;
        context.vars = &vars;

        saBuildScriptVarsTable(&vars, player, player->db_id, contactInfo->handle, NULL, NULL, NULL, NULL, NULL, 0, contactInfo->dialogSeed, false);
    }

    switch (link)
    {
        default:
            pTask = DeliveryTaskContactDialogFindTask(player, pContext); // okay if it's NULL
            if(pContext->vars.numscopes > 0) // VAR table comes from the script VAR table
            {
                ScriptVarsTableScopeCopy(&vars, &pContext->vars);
                saBuildScriptVarsTable(&vars, player, player->db_id, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, false);
            }
            else if(pTask) // VAR table comes from the task+contact
            {
                saBuildScriptVarsTable(&vars, player, player->db_id, TaskGetContactHandle(player->storyInfo, pTask), pTask, NULL, NULL, NULL, NULL, 0, 0, false);        
            }
            else // no VAR table
            {
                saBuildScriptVarsTable(&vars, player, player->db_id, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, false);
            }
            returnMe = DialogDef_FindAndHandlePage(player, interactEntity, pContext->pDialog, link, pTask, response, player->storyInfo->interactTarget, &vars);
            break;

        case CONTACTLINK_BYE:
            SAFE_FREE(pContext);
            break;

        case CONTACTLINK_STARTMISSION:            
            pTask = DeliveryTaskContactDialogFindTask(player, pContext);
            if (pTask)
            {
                // start mission
                EnterMission(player, pTask);

                if (!db_state.local_server)// interacting entity has already been freed
                {
                    // remove stash pointer
                    if (interactEntity)
                        stashIntRemovePointer(interactEntity->dialogLookup, player->db_id, &pContext);
                    else
                    {
                        stashIntRemovePointer(remoteDialogLookupTables[remoteIndex], player->db_id, &pContext);

                        if (!stashGetOccupiedSlots(cpp_reinterpret_cast(StashTable)(remoteDialogLookupTables[remoteIndex])))
                        {
                            StashTable theTable = cpp_reinterpret_cast(StashTable)(eaRemove(&remoteDialogLookupTables, remoteIndex));
                            eaiRemove(&remoteDialogLookupContactHandleKeys, remoteIndex);
                            stashTableDestroy(theTable);
                        }
                    }
                    SAFE_FREE(pContext);
                }
            }
            break;

        case CONTACTLINK_HELLO:
            break;

        case CONTACTLINK_MAIN: // copied from ContactInteraction()!
            if (player->storyInfo->interactTarget) // if not, contact is NULL!
            {
                player->storyInfo->interactDialogTree = 0;
                {
                    if (CONTACT_IS_TASKFORCE(contactInfo->contact->def) && contactInfo->interactionTemp)
                        ContactTaskForceInteraction(&context);
                    else
                        ContactFrontPage(&context, 1, CONTACT_IS_PLAYERMISSIONGIVER(contactInfo->contact->def));
                }
                returnMe = 1;
            }
            break;

        case CONTACTLINK_MISSIONS: // copied from ContactInteraction()!
            if (player->storyInfo->interactTarget) // if not, context is empty!
            {
                player->storyInfo->interactDialogTree = 0;
                ContactInteractionPromptNewTasks(&context, -1, NULL);
                returnMe = 1;
            }
            break;

        case CONTACTLINK_INTRODUCE: // copied from ContactInteraction()!
            if (player->storyInfo->interactTarget) // if not, context is empty!
            {
                player->storyInfo->interactDialogTree = 0;
                ContactInteractionChooseContact(&context);
                returnMe = 1;
            }
            break;

        case CONTACTLINK_GOTOSTORE: // copied from ContactInteraction()!
            if (player->storyInfo->interactTarget) // if not, context is empty!
            {
                player->storyInfo->interactDialogTree = 0;
                if (!info->interactCell)
                {
                    player->pl->npcStore = -player->storyInfo->interactTarget; // <0 means contact instead of npc.
                    store_SendOpenStoreContact(player, contactInfo->contactRelationship, &contactInfo->contact->def->stores);
                }
                ContactFrontPage(&context, 1, 0);
                returnMe = 1;
            }
            break;

        case CONTACTLINK_ABOUT: // copied from ContactInteraction()!
            if (player->storyInfo->interactTarget) // if not, context is empty!
            {
                player->storyInfo->interactDialogTree = 0;
                ContactInteractionAbout(&context);
                returnMe = 1;
            }
            break;

        case CONTACTLINK_AUTO_COMPLETE: // copied from ContactInteraction()!
            if (player->storyInfo->interactTarget) // if not, context is empty!
            {
                player->storyInfo->interactDialogTree = 0;
                ContactInteractionAutoComplete(&context);
                returnMe = 1;
            }
            break;

        case CONTACTLINK_ABANDON: // copied from ContactInteraction()!
            if (player->storyInfo->interactTarget) // if not, context is empty!
            {
                player->storyInfo->interactDialogTree = 0;
                ContactInteractionAbandonTask(&context);
                returnMe = 1;
            }
            break;

        case CONTACTLINK_DISMISS: // copied from ContactInteraction()!
            if (player->storyInfo->interactTarget) // if not, context is empty!
            {
                player->storyInfo->interactDialogTree = 0;
                ContactInteractionDismiss(&context);
                returnMe = 1;
            }
            break;
        case CONTACTLINK_WEBSTORE: // rewritten to function for non-contacts
            START_PACKET(pak, player, SERVER_WEB_STORE_OPEN_PRODUCT);
            pktSendString(pak, player->pl->webstore_link);
            END_PACKET

            returnMe = 0;
            break;
    }

    if (info)
    {
        StoryArcUnlockInfo(player);
    }
    return returnMe;
}


void ContactInteractionSayGoodbye(Entity* player, const ContactDef* contactDef, int link, ContactResponse* response, int canBeDismissed);


// main interaction function for contacts which aren't in the player's list.
// queried by link, returns a text response and more links
// Returns 0 if the interaction has ended.
int ContactUnknownInteraction(Entity* player, ContactHandle contactHandle, int link, ContactResponse* response)
{
    Contact* contact = GetContact(contactHandle);
    StoryContactInfo *contactInfo;
    ScriptVarsTable vars = {0};

    if (!response)
    {
        log_printf("storyerrs.log", "ContactUnknownInteraction: I received a NULL response parameter");
        return 0;
    }

    // trapping the delivery task contacts
    if (player->storyInfo->interactDialogTree)
        return DeliveryTaskContact(player, link, response);

    if (!contact) 
        return 0;

    memset(response, 0, sizeof(ContactResponse));

    saBuildScriptVarsTable(&vars, player, player->db_id, contactHandle, NULL, NULL, NULL, NULL, NULL, 0, (unsigned int)time(NULL), false);

    //If you're not the right alignment/universe, you don't need to go through
    //the specific contact types.  It's also crazy to add a special
    //wrong alignment path for each of these (and all new) types of 
    //contacts, which is the direction we were going.
    if(link == CONTACTLINK_WRONGALLIANCE || link == CONTACTLINK_WRONGUNIVERSE)
    {
        const char *wrongStr = NULL;
        // Wrong alliance/universe work the same way, just have different
        // source data.
        if (link == CONTACTLINK_WRONGALLIANCE)
            wrongStr = contact->def->wrongAlliance;
        else if (link == CONTACTLINK_WRONGUNIVERSE)
            wrongStr = contact->def->wrongUniverse;

        if (wrongStr)
        {
            if (wrongStr[0] == '[')
            {
                // This is a dialog tree hook, entering special case code here
                Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
                char *wrongCopy = strdup(wrongStr + 1);
                char *dialogName = strtok(wrongCopy, ":");
                char *pageName = strtok(0, "]"); // note that there may be more characters after the ] that are ignored
                if (dialogName && pageName) // otherwise this isn't a valid dialog tree hook, continue with normal contact
                {
                    const DialogDef *dialog = DialogDef_FindDefFromContact(contact->def, dialogName);
                    int hash = hashStringInsensitive(pageName);
                    memset(response, 0, sizeof(ContactResponse)); // reset the response
                    if (DialogDef_FindAndHandlePage(player, target, dialog, hash, 0, response, contactHandle, &vars))
                    {
                        free(wrongCopy);
                        return 1;
                    }
                }
                free(wrongCopy);
            }

            strcat(response->dialog, saUtilTableLocalize(wrongStr, &vars));
        }
        else
        {
            if (link == CONTACTLINK_WRONGALLIANCE)
                strcat(response->dialog, textStd("DefaultWrongAllianceString"));
            else if (link == CONTACTLINK_WRONGUNIVERSE)
                strcat(response->dialog, textStd("DefaultWrongUniverseString"));
        }

        strcat(response->dialog, "<BR>");

        if (link == CONTACTLINK_WRONGALLIANCE && contact->def->SOLwrongAlliance)
            strcpy_s(SAFESTR(response->SOLdialog), saUtilTableAndEntityLocalize(contact->def->SOLwrongAlliance, &vars, player));
        else if (link == CONTACTLINK_WRONGUNIVERSE && contact->def->SOLwrongUniverse)
            strcpy_s(SAFESTR(response->SOLdialog), saUtilTableAndEntityLocalize(contact->def->SOLwrongUniverse, &vars, player));

        AddResponse(response, saUtilLocalize(player,"Leave"), CONTACTLINK_BYE);
        return 1;
    }

    

    if (CONTACT_IS_NOTORIETY(contact->def))
        return difficultyContact(player, contact, link, response);
     else if (CONTACT_IS_TRAINER(contact->def))
        return TrainerContact(player, contact->def, link, response);
    else if (CONTACT_IS_PVPSWITCH(contact->def))
        return PvPSwitchContact(player, contact, link, response);
    else if (CONTACT_IS_SCRIPT(contact->def))
        return ScriptScriptedContactClick(player, link, response);
    else if (CONTACT_IS_REGISTRAR(contact->def))
        return RegistrarContact(player, contact, link, response);
    else if (CONTACT_IS_FLASHBACK(contact->def))
        return FlashbackContact(player, contactHandle, link, response);
    else
    {
        switch (link)
        {
        case CONTACTLINK_BYE:
                // Done with conversation.
                return 0;

        case CONTACTLINK_BADCELLCALL:
            if (contact->def->noCellOverride)
            {
                strcat(response->dialog, saUtilLocalize(player, contact->def->noCellOverride));
            }
            else if (OnInstancedMap() || server_visible_state.isPvP)
            {
                strcat(response->dialog, saUtilLocalize(player,"NoCellConnection"));
            }
            else
            {
                strcat(response->dialog, saUtilLocalize(player,"PlayerNotAllowedCell"));
            }
            break;

        case CONTACTLINK_WRONGMODE:
            strcat(response->dialog, saUtilLocalize(player,"PlayerInSGMode"));
            break;

        case CONTACTLINK_NOSG:
            strcat(response->dialog, saUtilLocalize(player,"SGContactNoSG"));
            break;

        case CONTACTLINK_NOSGPERMS:
            strcat(response->dialog, saUtilLocalize(player,"SGContactNoSGPerms"));
            break;

        case CONTACTLINK_NOTLEADER:
            strcat(response->dialog, saUtilLocalize(player,"PlayerNotSGLeader"));
            break;

        case CONTACTLINK_BADGEREQUIRED_FIRST:
        case CONTACTLINK_BADGEREQUIRED_SECOND:
        case CONTACTLINK_BADGEREQUIRED_THIRD:
        case CONTACTLINK_BADGEREQUIRED_FOURTH:
            {
                strcat(response->dialog, saUtilTableLocalize(contact->def->badgeNeededString, &vars));
                strcat(response->dialog, "<BR>");
                if (link >=CONTACTLINK_BADGEREQUIRED_FOURTH && contact->def->badgeFourthTick)
                    strcat(response->dialog, saUtilTableLocalize(contact->def->badgeFourthTick, &vars));
                else if (link >=CONTACTLINK_BADGEREQUIRED_THIRD && contact->def->badgeThirdTick)
                    strcat(response->dialog, saUtilTableLocalize(contact->def->badgeThirdTick, &vars));
                else if (link >=CONTACTLINK_BADGEREQUIRED_SECOND && contact->def->badgeSecondTick)
                    strcat(response->dialog, saUtilTableLocalize(contact->def->badgeSecondTick, &vars));
                else if (link >=CONTACTLINK_BADGEREQUIRED_FIRST && contact->def->badgeFirstTick)
                    strcat(response->dialog, saUtilTableLocalize(contact->def->badgeFirstTick, &vars));

                if (contact->def->SOLbadgeNeededString)
                    strcpy_s(SAFESTR(response->SOLdialog), saUtilTableAndEntityLocalize(contact->def->SOLbadgeNeededString, &vars, player));

                break;
            }
        case CONTACTLINK_BADGEREQUIRED_TOOLOW:
            {
                char levelstr[10];

                sprintf(levelstr, "%i", contact->def->minPlayerLevel);
                ScriptVarsTablePushVar(&vars, "Level", levelstr);

                if (contact->def->comeBackLater[0] == '[')
                {
                    // This is a dialog tree hook, entering special case code here
                    Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
                    char *comeBackLaterCopy = strdup(contact->def->comeBackLater + 1);
                    char *dialogName = strtok(comeBackLaterCopy, ":");
                    char *pageName = strtok(0, "]"); // note that there may be more characters after the ] that are ignored
                    if (dialogName && pageName) // otherwise this isn't a valid dialog tree hook, continue with normal contact
                    {
                        const DialogDef *dialog = DialogDef_FindDefFromContact(contact->def, dialogName);
                        int hash = hashStringInsensitive(pageName);
                        memset(response, 0, sizeof(ContactResponse)); // reset the response
                        if (DialogDef_FindAndHandlePage(player, target, dialog, hash, 0, response, contactHandle, &vars))
                        {
                            free(comeBackLaterCopy);
                            return 1;
                        }
                    }
                    free(comeBackLaterCopy);
                }

                strcat(response->dialog, saUtilTableLocalize(contact->def->comeBackLater, &vars));
                break;
            }
        case CONTACTLINK_FAILEDREQUIRES:
            {
                if(contact->def->interactionRequiresFailString)
                {
                    strcat(response->dialog, saUtilTableLocalize(contact->def->interactionRequiresFailString, &vars));
                }

                break;
            }
        case CONTACTLINK_NEWS_ERROR:
            {
                StoryInfo* info = StoryInfoFromPlayer(player);
                ContactHandle broker = StoryArcGetCurrentBroker(player, info);
                const ContactDef* brokerDef = ContactDefinition(broker);
                int level = character_GetExperienceLevelExtern(player->pchar);
                char levelstr[10];
                const char* why;

                sprintf(levelstr, "%i", level);
                ScriptVarsTablePushVar(&vars, "Level", levelstr);

                if (!broker || !ContactCorrectAlliance(SAFE_MEMBER(player->pchar, playerTypeByLocation), contact->def->alliance))
                {
                    if (contact->def->alliance == ALLIANCE_HERO)
                    {
                        why = saUtilLocalize(player, "NoBrokerPoliceRadio"); // Does NOT V_ translate, based off of playerTypeByLocation!
                    }
                    else
                    {
                        why = saUtilLocalize(player, "NoBrokerNewspaper"); // Does NOT V_ translate, based off of playerTypeByLocation!
                    }
                }
                else if (level < brokerDef->minPlayerLevel)
                {
                    if (contact->def->comeBackLater[0] == '[')
                    {
                        // This is a dialog tree hook, entering special case code here
                        Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
                        char *comeBackLaterCopy = strdup(contact->def->comeBackLater + 1);
                        char *dialogName = strtok(comeBackLaterCopy, ":");
                        char *pageName = strtok(0, "]"); // note that there may be more characters after the ] that are ignored
                        if (dialogName && pageName) // otherwise this isn't a valid dialog tree hook, continue with normal contact
                        {
                            const DialogDef *dialog = DialogDef_FindDefFromContact(contact->def, dialogName);
                            int hash = hashStringInsensitive(pageName);
                            memset(response, 0, sizeof(ContactResponse)); // reset the response
                            if (DialogDef_FindAndHandlePage(player, target, dialog, hash, 0, response, contactHandle, &vars))
                            {
                                free(comeBackLaterCopy);
                                return 1;
                            }
                        }
                        free(comeBackLaterCopy);
                    }

                    why = saUtilTableLocalize(contact->def->comeBackLater, &vars);
                }
                else if (level > brokerDef->maxPlayerLevel)
                {
                    why = saUtilTableLocalize(contact->def->failsafeString, &vars);
                }
                else // if (ContactShouldGiveHeist(ContactGetInfo(info, broker)))
                {
                    ScriptVarsTablePushVar(&vars, "BrokerName", ContactDisplayName(brokerDef,0));
                    why = saUtilTableLocalize(contact->def->noTasksRemaining, &vars);
                }
                
                strcat(response->dialog, why);
            }
            break;

        case CONTACTLINK_DONTKNOW:
        default:
            {
                const ContactDef* lastcontact;
                
                lastcontact = ContactGetLastContact(player, contactHandle, contact->def->stature);
                if (lastcontact)
                {
                    ScriptVarsTablePushVarEx(&vars, "LastContact", lastcontact, 'C', 0);
                    ScriptVarsTablePushVarEx(&vars, "LastContactName", lastcontact, 'C', 0);
                }

                if (contact->def->dontknow[0] == '[')
                {
                    // This is a dialog tree hook, entering special case code here
                    Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
                    char *dontknowCopy = strdup(contact->def->dontknow + 1);
                    char *dialogName = strtok(dontknowCopy, ":");
                    char *pageName = strtok(0, "]"); // note that there may be more characters after the ] that are ignored
                    if (dialogName && pageName) // otherwise this isn't a valid dialog tree hook, continue with normal contact
                    {
                        const DialogDef *dialog = DialogDef_FindDefFromContact(contact->def, dialogName);
                        int hash = hashStringInsensitive(pageName);
                        memset(response, 0, sizeof(ContactResponse)); // reset the response
                        if (DialogDef_FindAndHandlePage(player, target, dialog, hash, 0, response, contactHandle, &vars))
                        {
                            free(dontknowCopy);
                            return 1;
                        }
                    }
                    free(dontknowCopy);
                }

                strcpy(response->dialog, saUtilTableAndEntityLocalize(contact->def->dontknow, &vars, player));
                if (contact->def->SOLdontknow)
                    strcpy_s(SAFESTR(response->SOLdialog), saUtilTableAndEntityLocalize(contact->def->SOLdontknow, &vars, player));
            }
        }

        ContactInteractionSayGoodbye(player, contact->def, link, response, (contactInfo = ContactGetInfo(player->storyInfo, contactHandle)) && ContactCanBeDismissed(player, contactInfo));
    }

    return 1;
}

// *********************************************************************************
//  Contact dialog debugging
// *********************************************************************************

// debug only way of interacting with contact
void ContactDebugDialog(Entity* player, ContactHandle handle, int link)
{
    ContactResponse response;
    ContactInteraction(player, handle, link, &response);
    chatSendToPlayer(player->db_id, response.dialog, INFO_DEBUG_INFO, 0);
}

// get the provided short or long task from the contact
void ContactDebugAcceptTask(ClientLink* client, char* contact, int longtask)
{
    ContactHandle contacthandle;

    TaskDebugClearAllTasks(client->entity);
    contacthandle = ContactGetHandleLoose(contact);
    if (!contacthandle)
    {
        conPrintf(client, localizedPrintf(0,"CouldntFindContact", contact));
        return;
    }
    else
    {
        ContactDebugDialog(client->entity, contacthandle, CONTACTLINK_ACCEPTLONG);
    }
}


