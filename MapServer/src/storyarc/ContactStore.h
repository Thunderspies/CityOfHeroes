/*\
 *
 *    ContactStore.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    handles everything related to contacts offering store items
 *
 */

#ifndef CONTACTSTORE_H
#define CONTACTSTORE_H

#include "storyarcinterface.h"

// *********************************************************************************
//  Contact store functions
// *********************************************************************************

void ContactValidateStoreItems(ContactDef* contact); // run on preload
ContactStoreItems* ContactStoreGetItems(ContactDef* contact, ContactRelationship relationship);
const ContactStoreItem* ContactStoreGetItem(ContactDef* contact, int itemNum, ContactRelationship* relationship);
int ContactStoreGetTotalItemCount(StoryContactInfo* contactInfo);
int ContactStoreGetItemCount(ContactDef* contact, ContactRelationship relationship);
void ContactStoreSetItemBought(StoryContactInfo* contactInfo, ContactRelationship relationship, ContactStoreItem* item);
int ContactStoreGetItemAvailable(StoryContactInfo* contactInfo, ContactRelationship relationship, ContactStoreItem* item);

#endif
