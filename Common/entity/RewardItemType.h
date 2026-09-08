/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef REWARDITEMTYPE_H
#define REWARDITEMTYPE_H

#include <utilitieslib/utils/structDefines.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/utils.h>

//------------------------------------------------------------
// types of rewards. note that Power rewards themselves 
// have types (boost, inspiration).
// if you add a new type here, change the following:
//  - struct RewardItem: (if applicable) add the fields needed to the unions.
//  - ParseRewardItemSet: add a new type to the rewarditemset on how it is parsed
//  - struct RewardAccumulator
//  - accumulator functions
//    -- s_rewardaccumulated_accumFuncs[] : alloc the accumulator
//    -- rewardaccumulator_Cleanup: destroy the accumulator 
//  - s_rewarditem_verifyFuncType
//  - void rewardApply() 
//  - rewarditemtype_NameFromId/IdFromName
//
//----------------------------------------------------------
typedef enum RewardItemType
{
    kRewardItemType_Power,
    kRewardItemType_Salvage,
    kRewardItemType_Concept,
    kRewardItemType_Proficiency,
    kRewardItemType_DetailRecipe,
    kRewardItemType_Detail,
    kRewardItemType_Token,
    kRewardItemType_RewardTable,
    kRewardItemType_RewardTokenCount,
    kRewardItemType_IncarnatePoints,
    kRewardItemType_AccountProduct,
    kRewardItemType_Count
} RewardItemType;
static INLINEDBG bool rewarditemtype_Valid(RewardItemType t) {return INRANGE(t, kRewardItemType_Power, kRewardItemType_Count  );} 
char *rewarditemtype_Str(RewardItemType t);

extern StaticDefineInt RewardItemTypeEnum[];


char const *rewarditemtype_NameFromId(RewardItemType type, int id);
int rewarditemtype_IdFromName(RewardItemType type, char const *name);



#endif //REWARDITEMTYPE_H
