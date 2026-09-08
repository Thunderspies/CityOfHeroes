/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef REWARDSLOT_H
#define REWARDSLOT_H

#include <utilitieslib/utils/textparser_types.h>
#include "entity/RewardItemType.h"

typedef struct Packet Packet;

//------------------------------------------------------------
//  Inventions need items slotted to power them up. this 
// structure describes a single such item.
//----------------------------------------------------------
typedef struct RewardSlot
{
    RewardItemType type;
    char const *name;
} RewardSlot;

static INLINEDBG bool rewardslot_Valid(RewardSlot const *s) {return s && s->name && rewarditemtype_Valid(s->type);} 
bool rewardslot_Matches(RewardSlot const *slot, RewardItemType type, char const *name, int level ) ;
int rewardslot_ValidCount(RewardSlot const *slots, int size);

extern TokenizerParseInfo ParseRewardSlot[];
RewardSlot* rewardslot_Init(RewardSlot* slot, RewardItemType type, char const *name);

void rewardslot_Send( RewardSlot *slot, Packet *pak );
RewardSlot* rewardslot_Receive( RewardSlot *slot, Packet *pak );

#endif //REWARDSLOT_H
