#ifndef PLAYERCREATEDSTORYARCTYPES_H
#define PLAYERCREATEDSTORYARCTYPES_H

#include <utilitieslib/stdtypes.h>

AUTO_ENUM;
typedef enum SpecialAction
{
    kAction_None,
    kAction_EnumMorality,
    kAction_EnumAlignment,
    kAction_EnumDifficulty,
    kAction_EnumDifficultyWithSingle,
    kAction_EnumPacing,
    kAction_EnumPlacement,
    kAction_EnumDetail,
    kAction_EnumPersonCombat,
    kAction_EnumPersonBehavior,
    kAction_EnumMapLength,
    kAction_EnumContactType,
    kAction_EnumRumbleType,
    kAction_EnumArcStatus,

    kAction_BuildVillainGroupList,
    kAction_BuildEntityList,
    kAction_BuildSupportEntityList,
    kAction_BuildBossEntityList,
    kAction_BuildObjectEntityList,
    kAction_BuildAnimList,
    kAction_BuildModelList,
    kAction_BuildMapList,
    kAction_BuildContactList,
    kAction_BuildModelListContact,
    kAction_BuildEntityListContact,
    kAction_BuildObjectEntityListContact,
    kAction_BuildLevelList,

    kAction_BuildCustomVillainGroupList,
    kAction_BuildCustomCritterList,
    kAction_BuildCustomCritterAndContactList,
    kAction_BuildAmbushTrigger,
    kAction_BuildDestinationList,
    kAction_BuildGiantMonsterEntityList,
    kAction_BuildDoppelEntityList,
} SpecialAction;

AUTO_ENUM;
typedef enum PlayerCreatedDetailType
{
    kDetail_Ambush = 1,
    kDetail_Boss,
    kDetail_Collection,
    kDetail_DestructObject,
    kDetail_DefendObject,
    kDetail_Patrol,
    kDetail_Rescue,
    kDetail_Escort,
    kDetail_Ally,
    kDetail_Rumble,
    kDetail_DefeatAll,
    kDetail_GiantMonster,
    kDetail_Count,
} PlayerCreatedDetailType;

#endif // PLAYERCREATEDSTORYARCTYPES_H
