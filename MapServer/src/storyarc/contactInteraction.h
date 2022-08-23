/*\
 *
 *    contactInteraction.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    IO functions for contact dialogs
 *    trainer and unknown contact dialog functions
 *    the normal contact dialog is in contactDialog.c
 *
 */

#ifndef CONTACTINTERACTION_H
#define CONTACTINTERACTION_H

#include "storyarcinterface.h"

// *********************************************************************************
//  Contact dialog IO
// *********************************************************************************
void ContactResponseSend(ContactResponse* response, Packet* pak);
void ContactInteractionEnd(Entity* player);
void ContactInteractionClose(Entity* player);
void ContactOpen(Entity* player, const Vec3 pos, ContactHandle handle, Entity *contact);    // start the client interaction dialog
void ContactFlashbackSendoff(Entity* player, const Vec3 pos, ContactHandle handle);

// *********************************************************************************
//  Unknown contact and trainer handlers
// *********************************************************************************

int ContactCannotInteract(Entity* player, ContactHandle handle, int link, int cellcall);
int ContactCannotMetaLink(Entity* player, ContactHandle handle);
int ContactUnknownInteraction(Entity* player, ContactHandle contactHandle, int link, ContactResponse* response);

#endif