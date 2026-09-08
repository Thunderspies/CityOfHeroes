/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef CONTAINERINVENTORY_H
#define CONTAINERINVENTORY_H

#include <utilitieslib/stdtypes.h>
#include "entity/character_inventory.h"

typedef struct StuffBuff StuffBuff;
typedef struct Entity Entity;



typedef struct DBGenericInventoryItem
{
    InventoryType type;
    int id;
    int amount;
} DBGenericInventoryItem;

void dbgenericinventoryitem_Destroy( DBGenericInventoryItem *item );

typedef struct DBConceptInventoryItem
{
    int defId;
    int amount;
    F32 afVars[4];
} DBConceptInventoryItem;

void dbconceptinventoryitem_Destroy( DBConceptInventoryItem *item );

// ------------------------------------------------------------
// entity db/server functions

void inventory_Template(StuffBuff *psb);

void entity_packageInventory(Entity *e, StuffBuff *psb);
bool entity_unpackInventory(DBGenericInventoryItem ***hGenItems, 
                            DBConceptInventoryItem ***hConceptItems, 
                            Entity *e, char *table, int row, char *col, char *val);
InventoryType inventory_typeFromTable(char *table);


#endif //CONTAINERINVENTORY_H
