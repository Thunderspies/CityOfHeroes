/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "entity/RewardSlot.h"
#include <utilitieslib/components/MemoryPool.h>
#include "entity/salvage.h"
#include "entity/Concept.h"
#include "entity/Proficiency.h"
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/utils/textparser.h>

//------------------------------------------------------------
//  Create a rewardslot from the pool
//----------------------------------------------------------
MP_DEFINE(RewardSlot);
static RewardSlot* rewardslot_Create(RewardItemType type, char const *name)
{
    RewardSlot *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(RewardSlot, 64);
    res = MP_ALLOC( RewardSlot );
    if( verify( res ))
    {
        res->type = type;
        res->name = name;
    }

    // --------------------
    // finally, return

    return res;
}

//------------------------------------------------------------
//  cleanup
//----------------------------------------------------------
static void rewardslot_Destroy(RewardSlot*p)
{
    MP_FREE(RewardSlot,p);
}

RewardSlot* rewardslot_Init(RewardSlot* slot, RewardItemType type, char const *name)
{
    if( verify( rewarditemtype_Valid( type) ))
    {
        if( !slot )
        {
            slot = rewardslot_Create( type, name );
        }
        else
        {
            ZeroStruct(slot);
            slot->type = type;
            slot->name = name;
        }
    }
    return slot;
}

TokenizerParseInfo ParseRewardSlot[] =
{
    {    "RewardItemType",        TOK_STRUCTPARAM | TOK_INT(RewardSlot, type, 0), RewardItemTypeEnum},
    {    "Name",            TOK_STRUCTPARAM|TOK_STRING(RewardSlot, name, 0)},
    {    "\n",            TOK_END,        0                                        },
    {    "", 0, 0 }
};



//------------------------------------------------------------
//  The number of valid slots in the passed array
//----------------------------------------------------------
int rewardslot_ValidCount(RewardSlot const *slots, int size)
{
    int numPowerupSlots;
    for( numPowerupSlots = 0; numPowerupSlots < size && rewardslot_Valid( &slots[numPowerupSlots] ); ++numPowerupSlots )
    {
        // EMPTY
    }
    return numPowerupSlots;
}


//------------------------------------------------------------
//  try and match a rewardslot by type.
// for most types this is a pure type and name match, but
// for proficiencies, a slot is a match if it matches by name and type,
// or the origins match, and the slot's rarity is equal to the passed level.
//----------------------------------------------------------
bool rewardslot_Matches(RewardSlot const *slot, RewardItemType type, char const *name, int matchLevel)
{
    // this always matches
    bool res = slot && slot->type == type && slot->name && 0 == stricmp(slot->name, name);

    // proficiencies have special de-levelling code
    if( !res && slot->type == type && type == kRewardItemType_Proficiency )
    {
        ProficiencyItem const *pa = proficiency_GetItem(name);
        ProficiencyItem const *slotprof = proficiency_GetItem(slot->name);
        res = pa && slotprof && pa->origin == slotprof->origin && slotprof->rarity == matchLevel;
    }

    // --------------------
    // finally

    return res;
}

#define REWARDSLOT_TYPE_PACKBITS 1
#define REWARDSLOT_ID_PACKBITS 1
void rewardslot_Send( RewardSlot *slot, Packet *pak )
{
    int type = 0;
    char id = 0;

    if( verify( slot && pak ))
    {
        type = slot->type;
        id = rewarditemtype_IdFromName( slot->type, slot->name );
    }

    pktSendBitsPack( pak, REWARDSLOT_TYPE_PACKBITS, type );
    pktSendBitsPack( pak, REWARDSLOT_ID_PACKBITS, id);
}

RewardSlot* rewardslot_Receive( RewardSlot *slot, Packet *pak )
{
    int type = pktGetBitsPack( pak, REWARDSLOT_TYPE_PACKBITS);
    int id = pktGetBitsPack( pak, REWARDSLOT_ID_PACKBITS);
    char const *name = rewarditemtype_NameFromId( type, id );

    slot = rewardslot_Init( slot, type, name );
    return slot;
}

/* End of File */
