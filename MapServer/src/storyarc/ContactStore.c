/*\
 *
 *    ContactStore.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    handles everything related to contacts offering store items
 *
 */

#include "contact.h"
#include "storyarcprivate.h"
#include "entity/powers.h"

// *********************************************************************************
//  Contact store parsing
// *********************************************************************************

TokenizerParseInfo ParseContactStoreItem[] =
{
    { "{",        TOK_START,        0}, 
    { "}",        TOK_END,            0}, 
    { "Power",    TOK_EMBEDDEDSTRUCT(ContactStoreItem, power, ParsePowerNameRef) },
    { "Price",    TOK_INT(ContactStoreItem, price,0) },
    { "Unique", TOK_BOOLFLAG(ContactStoreItem, unique,0) },
    { "", 0, 0 }
};

TokenizerParseInfo ParseContactStoreItems[] =
{
    { "{",        TOK_START,        0}, 
    { "}",        TOK_END,            0}, 
    { "Item",    TOK_STRUCT(ContactStoreItems, items, ParseContactStoreItem) },
    { "", 0, 0 }
};

TokenizerParseInfo ParseContactStore[] =
{
    { "{",        TOK_START,        0}, 
    { "}",        TOK_END,            0}, 
    { "Acquaintance",    TOK_EMBEDDEDSTRUCT(ContactStore, acquaintanceItems, ParseContactStoreItems) },
    { "Friend",            TOK_EMBEDDEDSTRUCT(ContactStore, friendItems, ParseContactStoreItems) },
    { "Confidant",        TOK_EMBEDDEDSTRUCT(ContactStore, confidantItems, ParseContactStoreItems) },
    { "", 0, 0 }
};

// *********************************************************************************
//  Contact store functions
// *********************************************************************************
int ContactStoreGetItemCount(ContactDef* contact, ContactRelationship relationship);

ValidateStore(const char* filename, ContactStoreItems* items)
{
    const BasePower* power;
    int i, n;
    int uniquecount = 0;

    // run through all the items in the list and throw away those that are not actually powers
    n = eaSize(&items->items);
    for (i = n-1; i >= 0; i--)
    {
        ContactStoreItem* item = cpp_const_cast(ContactStoreItem*)(items->items[i]);
        power = powerdict_GetBasePowerByName(&g_PowerDictionary, 
            item->power.powerCategory, item->power.powerSet, item->power.power);
        if (!power)
        {
            printf("Store has invalid item: %s.%s.%s\n", 
                item->power.powerCategory, item->power.powerSet, item->power.power);
            printf("  File: %s\n", filename);
            //ErrorFilenamef(filename, "Store has invalid item: %s.%s.%s", 
            //    item->power.powerCategory, item->power.powerSet, item->power.power);
            eaRemoveConst(&items->items, i);
            if (!isSharedMemory(item))
                StructFree(item);
        }
        if (item->unique) uniquecount++;
    }
    if (uniquecount > MAX_UNIQUE_ITEMS)
    {
        ErrorFilenamef(filename, "Store has too many unique items");
    }
}

void ContactValidateStoreItems(ContactDef* contact)
{
    ValidateStore(contact->filename, &contact->store.confidantItems);
    ValidateStore(contact->filename, &contact->store.friendItems);
    ValidateStore(contact->filename, &contact->store.acquaintanceItems);
}

ContactStoreItems* ContactStoreGetItems(ContactDef* contact, ContactRelationship relationship)
{
    switch(relationship)
    {
    case CONTACT_CONFIDANT:
        return &contact->store.confidantItems;
    case CONTACT_FRIEND:
        return &contact->store.friendItems;
    case CONTACT_ACQUAINTANCE:
        return &contact->store.acquaintanceItems;
    }

    log_printf("storyerrs.log", "ContactStoreGetItems: I was asked for relationship %i items", relationship);
    return NULL;
}

static int ContactStoreGetUniqueBit(const ContactDef* contact, ContactRelationship relationship, ContactStoreItem* item)
{
    const ContactStoreItems* itemlist = NULL;
    int bit = 0;
    int i, n;

    // sanity check
    if (!item->unique) return -1;

    // look at relationship to decide what bit range we are in
    switch (relationship)
    {
    case CONTACT_CONFIDANT:
        itemlist = &contact->store.confidantItems;
        bit = 2 * MAX_UNIQUE_ITEMS;
        break;
    case CONTACT_FRIEND:
        itemlist = &contact->store.friendItems;
        bit = 1 * MAX_UNIQUE_ITEMS;
        break;
    case CONTACT_ACQUAINTANCE:
        itemlist = &contact->store.acquaintanceItems;
        bit = 0;
        break;
    }
    
    // find the item in this list to determine bit
    n = eaSize(&itemlist->items);
    for (i = 0; i < n; i++)
    {
        if (itemlist->items[i] == item)
            return bit;
        else if (itemlist->items[i]->unique)
            bit++;
    }
    return -1; // couldn't find item
}

const ContactStoreItem* ContactStoreGetItem(ContactDef* contact, int itemNum, ContactRelationship* relationship)
{
    int itemCount = 0;
    ContactRelationship relCursor;
    ContactStoreItems* items;
    const ContactStoreItem* targetItem;

    for(relCursor = CONTACT_ACQUAINTANCE; relCursor < CONTACT_RELATIONSHIP_MAX; relCursor++)
    {
        itemCount += ContactStoreGetItemCount(contact, relCursor);
        if(itemCount > itemNum)
            break;
    }

    // Is the specified item a valid item?
    if(itemNum + 1 > itemCount)
        return NULL;

    items = ContactStoreGetItems(contact, relCursor);
    if (!items) return NULL;
    itemCount -= eaSize(&items->items);
    targetItem = eaGetConst(&items->items, itemNum - itemCount);
    *relationship = relCursor;

    return targetItem;
}

// Function ContactStoreGetItemCount()
//    Returns the total number of items a player has access to at a particular
//    relationship level.
int ContactStoreGetTotalItemCount(StoryContactInfo* contactInfo)
{
    int itemCount = 0;

    switch(contactInfo->contactRelationship)
    {
    case CONTACT_CONFIDANT:
        itemCount += eaSize(&contactInfo->contact->def->store.confidantItems.items);
    case CONTACT_FRIEND:
        itemCount += eaSize(&contactInfo->contact->def->store.friendItems.items);
    case CONTACT_ACQUAINTANCE:
        itemCount += eaSize(&contactInfo->contact->def->store.acquaintanceItems.items);
    }    

    return itemCount;
}

// Function ContactStoreGetItemCount()
//    Returns the number of items a particular contact store has at the specified relationship level.
int ContactStoreGetItemCount(ContactDef* contactInfo, ContactRelationship relationship)
{
    ContactStoreItems* items = ContactStoreGetItems(contactInfo, relationship);
    if (!items) return 0;
    return eaSize(&items->items);
}

// for a unique item, set the bought flag on the contact info
void ContactStoreSetItemBought(StoryContactInfo* contactInfo, ContactRelationship relationship, ContactStoreItem* item)
{
    int bit = ContactStoreGetUniqueBit(contactInfo->contact->def, relationship, item);
    if (bit >= 0)
        BitFieldSet(contactInfo->itemsBought, UNIQUE_ITEM_BITFIELD_SIZE, bit, 1);
}

int ContactStoreGetItemAvailable(StoryContactInfo* contactInfo, ContactRelationship relationship, ContactStoreItem* item)
{
    int bit = ContactStoreGetUniqueBit(contactInfo->contact->def, relationship, item);
    if (bit < 0) return 0;
    return !BitFieldGet(contactInfo->itemsBought, UNIQUE_ITEM_BITFIELD_SIZE, bit);
}
