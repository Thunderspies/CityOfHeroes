/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef CHARACTER_NET_H__
#define CHARACTER_NET_H__

#include "entity/character_inventory.h"

// forward decls
typedef struct Entity Entity;
typedef struct Packet Packet;
typedef struct BasePower BasePower;
typedef struct Power Power;
typedef struct Boost Boost;
typedef struct PowerDictionary PowerDictionary;
typedef struct SalvageInventoryItem SalvageInventoryItem;
typedef struct Character Character;
typedef struct ConceptItem ConceptItem;
typedef struct PowerRef PowerRef;

void basepower_Send(Packet *pak, const BasePower *ppow);
const BasePower *basepower_Receive(Packet *pak, const PowerDictionary *pdict, Entity *receivingEntity);

// ARM NOTE: We should deprecate use of this function, replacing it with character_OwnsPowerByUID().
Power *SafeGetPowerByIdx(Character *p, int ibuild, int iset, int ipow);

void character_SendBoosts(Packet *pak, Character *pchar);
void character_SendPowers(Packet *pak, Character *pchar);

ConceptItem* conceptitem_Receive(ConceptItem *resItem, Packet *pak);
void conceptitem_Send(ConceptItem *p, Packet *pak);

void character_inventory_Send(Character *p, Packet *pak, InventoryType type, int idx);
void character_inventory_Receive(Character *p, Packet *pak );

void character_SendInventories(Character *pchar, Packet *pak );
void character_ReceiveInventories(Character *pchar, Packet *pak);
void character_SendInventorySizes(Character *pchar, Packet *pak );
void character_ReceiveInventorySizes(Character *pchar, Packet *pak );

void boost_Send(Packet *pak, Character *pchar, int idx);

void character_ReceiveBoosts(Packet *pak, Character *pchar);
void character_ReceivePowers(Packet *pak, Character *pchar);
void boost_Receive(Packet *pak, Character *pchar);

void character_SendPowerBoost(Character *pchar, Packet *pak, Boost *pboost, int boostIdx);
void character_ReceivePowerBoost(Character *pchar, Packet *pak, Power *ppow, int isGlobalBoost, int boostIdx);

void powerRef_Send(PowerRef* ppowRef, Packet* pak);
void powerRef_Receive(PowerRef* ppowRef, Packet* pak);

#if SERVER
void salvageinv_ReverseEngineer_Receive(Packet *pak, Character *pchar);
#elif CLIENT // SERVER
void salvageinv_ReverseEngineer_Send(Packet *pak, int inv_idx );
#endif // CLIENT

#endif /* #ifndef CHARACTER_NET_H__ */

/* End of File */
