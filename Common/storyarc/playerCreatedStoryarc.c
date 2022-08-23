
#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/playerCreatedStoryarcValidate.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/network/netcomp.h>
#include "gameComm/VillainDef.h"    //for villain enum
#include "entity/PCC_Critter.h"
#include "entity/CustomVillainGroup.h"
#include <utilitieslib/components/StashTable.h>
#include "storyarc/pnpcCommon.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/bitfield.h>
#include "gameComm/Npc.h"
#include "gameComm/VillainDef.h"


#ifdef SERVER
#define COSTUME_GIVE_TOKEN(tok, notifyPlayer, isPCC) costume_Award(e,tok, notifyPlayer, isPCC)
#define COSTUME_TAKE_TOKEN(tok, isPCC) costume_Unaward(e,tok, isPCC)
#elif CLIENT
#include "entity/costume_client.h"
#define COSTUME_GIVE_TOKEN(tok, notifyPlayer, isPCC) costumereward_add(tok, isPCC)
#define COSTUME_TAKE_TOKEN(tok, isPCC) costumereward_remove(tok, isPCC)
#else
#define COSTUME_GIVE_TOKEN(tok, notifyPlayer, isPCC)
#define COSTUME_TAKE_TOKEN(tok, isPCC)
#endif

StaticDefineInt MoralityEnum[] =
{
    DEFINE_INT
    { "Villainous", kMorality_Evil, "PCS_Villainous" },
    { "Rogue",      kMorality_Rogue, "PCS_Rogue" },
    { "Neutral",    kMorality_Neutral, "PCS_Neutral" },
    { "Vigilante",  kMorality_Vigilante, "PCS_Vigilante" },
    { "Heroic",     kMorality_Good, "PCS_Heroic" },
    DEFINE_END
};
StaticDefineInt AlignmentTypeEnum[] =
{
    DEFINE_INT
    { "Enemy",  kAlignment_Enemy,    "PCS_Enemy" },
    { "Ally",   kAlignment_Ally,    "PCS_Ally" },
    { "Rogue",  kAlignment_Rogue,    "PCS_Rogue" },
    DEFINE_END
};

StaticDefineInt DifficultyTypeEnum[] =
{
    DEFINE_INT
    { "Easy",   kDifficulty_Easy,    "PCS_Easy" },
    { "Medium", kDifficulty_Medium, "PCS_Medium" },
    { "Hard",   kDifficulty_Hard,    "PCS_Hard" },
    DEFINE_END
};


StaticDefineInt DifficultyWithSingleEnum[] =
{
    DEFINE_INT
    { "Easy",   kDifficulty_Easy,    "PCS_Easy"     },
    { "Medium", kDifficulty_Medium, "PCS_Medium" },
    { "Hard",   kDifficulty_Hard,    "PCS_Hard"     },
    { "Single", kDifficulty_Single,    "PCS_Single" },
    DEFINE_END
};

StaticDefineInt PacingTypeEnum[] =
{
    DEFINE_INT
    { "Flat",        kPacing_Flat,            "PCS_Flat"         },
    { "BackLoaded",  kPacing_BackLoaded,    "PCS_BackLoaded"   },
    { "RampUp",      kPacing_SlowRampUp,    "PCS_RampUp"       },
    { "Staggered",   kPacing_Staggered,        "PCS_Staggered"    },
    DEFINE_END
};

StaticDefineInt PlacementTypeEnum[] =
{
    DEFINE_INT
    { "Any",    kPlacement_Any,        "PCS_Any"     },
    { "Front",  kPlacement_Front,    "PCS_Front"  },
    { "Middle", kPlacement_Middle,    "PCS_Middle" },
    { "Back",   kPlacement_Back,    "PCS_Back"   },
    DEFINE_END
};

StaticDefineInt DetailTypeEnum[] =
{
    DEFINE_INT
    { "Ambush",        kDetail_Ambush,            "PCS_Ambush"         },
    { "Boss",          kDetail_Boss,            "PCS_Boss"           },
    { "Collection",    kDetail_Collection,        "PCS_Collection"     },
    { "DestroyObject", kDetail_DestructObject,    "PCS_DestructObject" },
    { "DefendObject",  kDetail_DefendObject,    "PCS_DefendObject"   },
    { "Patrol",        kDetail_Patrol,            "PCS_Patrol"         },
    { "Rescue",        kDetail_Rescue,            "PCS_Rescue"         },
    { "Escort",        kDetail_Escort,            "PCS_Escort"         },
    { "Ally",          kDetail_Ally,            "PCS_Ally"           },
    { "Rumble",        kDetail_Rumble,            "PCS_Rumble"         },
    { "DefeatAll",       kDetail_DefeatAll,        "PCS_DefeatAll"         },
    { "GiantMonster",  kDetail_GiantMonster,    "PCS_GiantMonster"   },
    DEFINE_END
};

StaticDefineInt ObjectLocationTypeEnum[] = 
{
    DEFINE_INT
    { "Wall",      kObjectLocationType_Wall,    "PCS_Wall"     },
    { "Floor",     kObjectLocationType_Floor,    "PCS_Floor"  },
    DEFINE_END
};

StaticDefineInt PersonCombatEnum[] = 
{
    DEFINE_INT
    { "NonCombat",         kPersonCombat_NonCombat,    "PCS_NonCombat"  }, 
    { "Pacifist",          kPersonCombat_Pacifist,    "PCS_Pacifist"   }, 
    { "FightAggressive",   kPersonCombat_Aggressive,"PCS_FightAggressive" },   
    { "FightDefensive",    kPersonCombat_Defensive,    "PCS_FightDefensive"  },  
    DEFINE_END
};


StaticDefineInt PersonBehaviorEnum[] = 
{
    DEFINE_INT
    { "Follow",            kPersonBehavior_Follow,        "PCS_Follow" }, 
    { "RunToNearestDoor",  kPersonBehavior_NearestDoor,    "PCS_RunToNearestDoor" },  
    { "RunAway",           kPersonBehavior_RunAway,        "PCS_RunAway" },
    { "Wander",            kPersonBehavior_Wander,        "PCS_Wander" },  
    { "DoNothing",         kPersonBehavior_Nothing,        "PCS_Nothing" },  
    DEFINE_END
};

StaticDefineInt MapLengthEnum[] = 
{
    DEFINE_INT
    {"Tiny",    kMapLength_Tiny,    "PCS_Tiny" },
    {"Small",    kMapLength_Small,    "PCS_Small" },
    {"Medium",    kMapLength_Medium,    "PCS_Medium" },
    {"Large",    kMapLength_Large,    "PCS_Large" },
    DEFINE_END
};

StaticDefineInt ContactTypeEnum[] =
{
    DEFINE_INT
    {"Default",          kContactType_Default, "PCS_Default" },
    {"Contact",          kContactType_Contact, "PCS_Contact" },
    {"Critter",          kContactType_Critter, "PCS_Critter" },
    {"Object",          kContactType_Object,  "PCS_Object" },
    {"CustomContact", kContactType_PCC,        "PCS_CustomContact" },
    DEFINE_END
};

StaticDefineInt RumbleTypeEnum[] = 
{
    DEFINE_INT
    {"Real_Fight",    kRumble_RealFight,    "PCS_RealFight"},
    {"Ally_1",        kRumble_Ally1,        "PCS_Ally1"},
    {"Ally_2",        kRumble_Ally2,        "PCS_Ally2" },
    DEFINE_END
};

StaticDefineInt ArcStatusEnum[] =
{
    DEFINE_INT
    {"InProgress",    kArcStatus_WorkInProgress,        "PCS_InProgress"},
    {"LFFeedback",    kArcStatus_LookingForFeedback,    "PCS_LFFeedback"},
    {"Final",        kArcStatus_Final,                "PCS_Final" },
    DEFINE_END
};
 
TokenizerParseInfo ParsePlayerCreatedDetail[] = 
{
    { "{",           TOK_START },
    { "Name",        TOK_STRING(PlayerCreatedDetail,pchName,"Enter Name") },
    { "DetailType",  TOK_INT(PlayerCreatedDetail,eDetailType,0), DetailTypeEnum },
    { "OverrideGroup", TOK_STRING(PlayerCreatedDetail, pchOverrideGroup, 0), 0 },
    { "Description", TOK_STRING(PlayerCreatedDetail, pchDisplayInfo, 0),0},
    { "Clue",                TOK_STRING(PlayerCreatedDetail, pchClueName, 0) },
    { "ClueDescription",    TOK_STRING(PlayerCreatedDetail, pchClueDesc, 0) },

    { "Entity",         TOK_STRING(PlayerCreatedDetail,pchVillainDef,0),0 },
    { "EntityGroup",    TOK_STRING(PlayerCreatedDetail,pchEntityGroupName,0) },
    { "EntityGroupSet",    TOK_INT(PlayerCreatedDetail,iEntityVillainSet,0) },
    { "EntityType",     TOK_STRING(PlayerCreatedDetail,pchEntityType,0) },

    { "SupportEntity",    TOK_STRING(PlayerCreatedDetail,pchSupportVillainDef,0),0 },
    { "SupportEntityType",TOK_STRING(PlayerCreatedDetail,pchSupportVillainType,0),0 },
    { "BattleType",          TOK_INT(PlayerCreatedDetail,eRumbleType, 0), RumbleTypeEnum },

    { "EnemyGroupType", TOK_STRING(PlayerCreatedDetail, pchEnemyGroupType, 0), 0 },
    { "VillainGroup",   TOK_STRING(PlayerCreatedDetail,pchVillainGroupName,0) },
    { "VillainGroupSet",TOK_INT(PlayerCreatedDetail,iVillainSet,0) },

    { "EnemyGroupType2", TOK_STRING(PlayerCreatedDetail, pchEnemyGroupType2, 0), 0 },
    { "VillainGroup2",   TOK_STRING(PlayerCreatedDetail,pchVillainGroup2Name,0) },
    { "VillainGroup2Set",TOK_INT(PlayerCreatedDetail,iVillainSet2,0) },

    { "Placement",        TOK_INT(PlayerCreatedDetail,ePlacement, 0), PlacementTypeEnum },
    { "Quantity",        TOK_INT(PlayerCreatedDetail,iQty,1) },
    { "Alignment",        TOK_INT(PlayerCreatedDetail,eAlignment,0), AlignmentTypeEnum },
    { "Difficulty",        TOK_INT(PlayerCreatedDetail,eDifficulty,0), DifficultyWithSingleEnum },

    { "InactiveSpeech",           TOK_STRING(PlayerCreatedDetail,pchDialogInactive,0) },
    { "ActiveSpeech",             TOK_STRING(PlayerCreatedDetail,pchDialogActive,0) },

    { "CompassDescription",           TOK_STRING(PlayerCreatedDetail,pchObjectiveDescription,0) },
    { "CompassDescriptionSingular",   TOK_STRING(PlayerCreatedDetail,pchObjectiveSingularDescription,0) },
    { "RewardText",                      TOK_STRING(PlayerCreatedDetail,pchRewardDescription,0) },
    { "DefaultAnimation",              TOK_STRING(PlayerCreatedDetail,pchDefaultStartingAnim,0)    },

    { "FirstBloodSpeech",         TOK_STRING(PlayerCreatedDetail,pchDialogAttack,0) },
    { "ThreeFourthsHealthSpeech", TOK_STRING(PlayerCreatedDetail,pchDialog75,0) },
    { "HalfHealthSpeech",         TOK_STRING(PlayerCreatedDetail,pchDialog50,0) },
    { "QuarterHealthSpeech",      TOK_STRING(PlayerCreatedDetail,pchDialog25,0) },
    { "DeathSpeech",              TOK_STRING(PlayerCreatedDetail,pchDeath,0) },
    { "DefeatedPlayerSpeech",     TOK_STRING(PlayerCreatedDetail,pchKills,0) },
    { "HeldSpeech",                  TOK_STRING(PlayerCreatedDetail,pchHeld,0) },
    { "FreedSpeech",              TOK_STRING(PlayerCreatedDetail,pchFreed,0) },
    { "AfraidSpeech",              TOK_STRING(PlayerCreatedDetail,pchAfraid,0) },
    
    { "BossRunsAwaySpeech",              TOK_STRING(PlayerCreatedDetail,pchRunAway,0) },
    { "BossRunsWhenHPIsBelow",          TOK_STRING(PlayerCreatedDetail,pchRunAwayHealthThreshold, 0) },
    { "BossRunsWhenAllyCountIsBelow", TOK_STRING(PlayerCreatedDetail,pchRunAwayTeamThreshold, 0) },
    { "BossAnimation",                  TOK_STRING(PlayerCreatedDetail,pchBossStartingAnim,0)    },
    { "BossOnlyRequiredForComplete",  TOK_INT(PlayerCreatedDetail,bBossOnly,0)    },

    { "ObjectInteractionTime",  TOK_INT(PlayerCreatedDetail,iInteractTime,0) },
    { "ObjectBeginInteraction", TOK_STRING(PlayerCreatedDetail,pchBeginText,0) },
    { "ObjectInteractionBar",   TOK_STRING(PlayerCreatedDetail,pchInteractBarText,0) },
    { "ObjectInterrupted",      TOK_STRING(PlayerCreatedDetail,pchInterruptText,0) },
    { "ObjectComplete",         TOK_STRING(PlayerCreatedDetail,pchCompleteText,0) },
    { "ObjectWaiting",          TOK_STRING(PlayerCreatedDetail,pchObjectWaiting,0) },
    { "ObjectReset",            TOK_STRING(PlayerCreatedDetail,pchObjectReset,0) },
    { "ObjectLocation",         TOK_INT(PlayerCreatedDetail,eObjectLocation,0), ObjectLocationTypeEnum },
    { "ObjectRemoveOnComplete", TOK_INT(PlayerCreatedDetail,bObjectiveRemove,0) },
    { "ObjectFadeIn",            TOK_INT(PlayerCreatedDetail,bObjectiveFadein,0) },

    { "Trigger", TOK_STRING(PlayerCreatedDetail, pchDetailTrigger, 0 ) },

    { "Patroller1Speech", TOK_STRING(PlayerCreatedDetail, pchPatroller1, 0 ) },
    { "Patroller2Speech", TOK_STRING(PlayerCreatedDetail, pchPatroller2, 0 ) },

    { "GuardInactiveSpeech", TOK_STRING(PlayerCreatedDetail, pchGuard1, 0 ) },
    { "GuardActiveSpeech", TOK_STRING(PlayerCreatedDetail, pchGuard2, 0 ) },

    { "Attacker1Speech",     TOK_STRING(PlayerCreatedDetail, pchAttackerDialog1, 0 ) },
    { "Attacker2Speech",     TOK_STRING(PlayerCreatedDetail, pchAttackerDialog2, 0) },
    { "AttackerAlertSpeech", TOK_STRING(PlayerCreatedDetail, pchAttackerHeroAlertDialog, 0 ) },

    { "PersonCombatStance",   TOK_INT(PlayerCreatedDetail,ePersonCombatType,0), PersonCombatEnum },

    { "PersonInactiveSpeech",  TOK_STRING(PlayerCreatedDetail,pchPersonInactive, 0),  },
    { "PersonActiveSpeech",    TOK_STRING(PlayerCreatedDetail,pchPersonActive, 0),  },
    { "PersonSucceedsOn",      TOK_STRING(PlayerCreatedDetail,pchPersonSuccess, 0),  }, // Rescue

    { "PersonRescuedSpeech",             TOK_STRING(PlayerCreatedDetail,pchPersonSayOnRescue, 0),  },
    { "PersonStrandedSpeech",            TOK_STRING(PlayerCreatedDetail,pchPersonSayOnStranded, 0),  },
    { "PersonReacquiredSpeech",          TOK_STRING(PlayerCreatedDetail,pchPersonSayOnReRescue, 0),  },
    { "PersonClickAfterReaquireSpeech",  TOK_STRING(PlayerCreatedDetail,pchPersonSayOnClickAfterRescue, 0),  },
    { "PersonArriveAtDestinationSpeech", TOK_STRING(PlayerCreatedDetail,pchPersonSayOnArrival, 0),  },

    { "PersonAnimation",           TOK_STRING(PlayerCreatedDetail,pchPersonStartingAnimation,0) },
    { "PersonRescuedAnimation",    TOK_STRING(PlayerCreatedDetail,pchRescueAnimation,0) },
    { "PersonStrandedAnimation",   TOK_STRING(PlayerCreatedDetail,pchStrandedAnimation,0)    },
    { "PersonRecapturedAnimation", TOK_STRING(PlayerCreatedDetail,pchReRescueAnimation,0) },
    { "PersonArrivedAnimation",    TOK_STRING(PlayerCreatedDetail,pchArrivalAnimation, 0)    },

    { "PersonRescuedBehavior",      TOK_INT(PlayerCreatedDetail,ePersonRescuedBehavior,0), PersonBehaviorEnum },
    { "PersonArrivedBehavior",    TOK_INT(PlayerCreatedDetail,ePersonArrivedBehavior,0), PersonBehaviorEnum    },
    { "PersonBetraysOnArrival",   TOK_INT(PlayerCreatedDetail,bPersonBetrays,0) },
    { "PersonBetrayalTrigger",    TOK_STRING(PlayerCreatedDetail,pchBetrayalTrigger,0) },
    { "PersonSaysOnBetrayal",     TOK_STRING(PlayerCreatedDetail,pchSayOnBetrayal,0) },

    { "EscortDestination",            TOK_STRING(PlayerCreatedDetail,pchEscortDestination, 0) },
    { "EscortMapWayPoint",            TOK_STRING(PlayerCreatedDetail,pchEscortWaypoint, 0),  },
    { "EscortObjectiveText",        TOK_STRING(PlayerCreatedDetail,pchEscortObjective, 0),  },
    { "EscortObjectiveTextSingular",TOK_STRING(PlayerCreatedDetail,pchEscortObjectiveSingular, 0),  },

    { "CustomCritterIdx",    TOK_INT(PlayerCreatedDetail, customCritterId,0) },
    { "CostumeIdx", TOK_INT(PlayerCreatedDetail, costumeIdx, 0)},
    { "CustomVillainGroupIdx",    TOK_INT(PlayerCreatedDetail, CVGIdx, 0) },
    { "CustomVillainGroupIdx2",    TOK_INT(PlayerCreatedDetail, CVGIdx2, 0) },
    { "CustomNPCCostumeIdx", TOK_INT(PlayerCreatedDetail, pchCustomNPCCostumeIdx, 0), 0 },
    { "}", TOK_END },
    {0},
};

TokenizerParseInfo ParsePlayerCreatedMission[] = 
{
    { "Title",         TOK_STRING(PlayerCreatedMission,pchTitle,"PC Mission Title") },
    { "SubTitle",      TOK_STRING(PlayerCreatedMission,pchSubTitle,"PC Mission SubTitle") },
    { "UniqueId",      TOK_SERVER_ONLY|TOK_INT(PlayerCreatedMission,uID,0) },
    { "Clue",                TOK_STRING(PlayerCreatedMission, pchClueName, 0) },
    { "ClueDescription",    TOK_STRING(PlayerCreatedMission, pchClueDesc, 0) },
    { "StartClue",                TOK_STRING(PlayerCreatedMission, pchStartClueName, 0) },
    { "StartClueDescription",    TOK_STRING(PlayerCreatedMission, pchStartClueDesc, 0) },

    { "FogDist",    TOK_F32(PlayerCreatedMission, fFogDist, 0) },
    { "FogColor",    TOK_STRING(PlayerCreatedMission, pchFogColor, 0) },

    { "MinLevel",            TOK_INT(PlayerCreatedMission,iMinLevel,1) },
    { "MaxLevel",            TOK_INT(PlayerCreatedMission,iMaxLevel,54) },
    { "MinLevelAuto",        TOK_SERVER_ONLY|TOK_INT(PlayerCreatedMission,iMinLevelAuto,1) },
    { "MaxLevelAuto",        TOK_SERVER_ONLY|TOK_INT(PlayerCreatedMission,iMaxLevelAuto,54) },

    { "MapName",            TOK_STRING(PlayerCreatedMission,pchMapName,0) },
    { "MapSet",                TOK_STRING(PlayerCreatedMission,pchMapSet,0) },
    { "UniqueMapCategory",  TOK_STRING(PlayerCreatedMission,pchUniqueMapCat,0) },
    { "MapLength",            TOK_INT(PlayerCreatedMission,eMapLength,30),MapLengthEnum },

    { "EnemyGroupType", TOK_STRING(PlayerCreatedMission, pchEnemyGroupType, 0), 0 },
    { "VillainGroup",  TOK_STRING(PlayerCreatedMission,pchVillainGroupName,"Empty") },
    { "VillainGroupSet",TOK_INT(PlayerCreatedMission,iVillainSet,0) },
    { "Pacing",        TOK_INT(PlayerCreatedMission,ePacing,1), PacingTypeEnum },
    { "CompletionTime",TOK_F32(PlayerCreatedMission,fTimeToComplete,30), 0 },

    { "ContactIntro",           TOK_STRING(PlayerCreatedMission,pchIntroDialog,"PC Nice to Meet You!") },
    { "ContactSendOff",           TOK_STRING(PlayerCreatedMission,pchSendOff,"PC Send Off!") },    
    { "ContactDeclineSendOff", TOK_STRING(PlayerCreatedMission,pchDeclineSendOff,"PC Decline Send Off!") },
    { "ContactStillBusy",       TOK_STRING(PlayerCreatedMission,pchStillBusyDialog,"PC Finish your mission before bothering me.") },
    { "ContactReturnSuccess",  TOK_STRING(PlayerCreatedMission,pchTaskReturnSuccess,"PC Good job on that mission!") },
    { "ContactReturnFail",       TOK_STRING(PlayerCreatedMission,pchTaskReturnFail,"PC Too Bad you messed that up!") },

    { "MissionAccept",         TOK_STRING(PlayerCreatedMission,pchAcceptDialog,"PC I want to do this mission!") },
    { "CompassActiveTask",   TOK_STRING(PlayerCreatedMission,pchActiveTask,"PC Win This Mission!") },

    { "MissionEntryDialog",   TOK_STRING(PlayerCreatedMission,pchEnterMissionDialog,"PC Welcome to the Mission!") },
    { "MissionSuccessDialog", TOK_STRING(PlayerCreatedMission,pchTaskSuccess,"PC You won the mission!") },
    { "MissionFailDialog",    TOK_STRING(PlayerCreatedMission,pchTaskFailed,"PC You lost the mission!") },
    { "MissionDefeatAll",    TOK_STRING(PlayerCreatedMission,pchDefeatAll,0) },

    { "MissionNoTeleportExit",    TOK_INT( PlayerCreatedMission, bNoTeleportExit, 0) },
    { "MissionRequirements",   TOK_STRINGARRAY(PlayerCreatedMission,ppMissionCompleteDetails) },
    { "MissionFailRequirements",   TOK_STRINGARRAY(PlayerCreatedMission,ppMissionFailDetails) },
    { "CustomVillainGroupIdx",        TOK_INT(PlayerCreatedMission, CVGIdx, 0 )    },
    { "Detail",     TOK_STRUCT(PlayerCreatedMission, ppDetail, ParsePlayerCreatedDetail) },


    { "EndMission", TOK_END },

    {0},
};

TokenizerParseInfo ParsePlayerStoryArc[] = 
{
    { "StartStoryArc",        TOK_START },
    { "Title",                TOK_STRING(PlayerCreatedStoryArc,pchName,"PC StoryArc Title") },
    { "UniqueId",            TOK_SERVER_ONLY|TOK_INT(PlayerCreatedStoryArc,uID,0) },
    { "WriterId",            TOK_SERVER_ONLY|TOK_INT(PlayerCreatedStoryArc,writer_dbid,0) },
    { "Rating",               TOK_SERVER_ONLY|TOK_INT(PlayerCreatedStoryArc,rating,0) },
    { "Euro",               TOK_SERVER_ONLY|TOK_INT(PlayerCreatedStoryArc,european,0) },
    { "Author",               TOK_SERVER_ONLY|TOK_STRING(PlayerCreatedStoryArc,pchAuthor,0) },
    { "Status",                TOK_INT(PlayerCreatedStoryArc,status, 0), ArcStatusEnum },
    { "LocaleID",           TOK_INT(PlayerCreatedStoryArc,locale_id,0) },
    { "KeywordBits",        TOK_INT(PlayerCreatedStoryArc,keywords, 0), 0 },

    { "Description",        TOK_STRING(PlayerCreatedStoryArc,pchDescription,"PC Enter Description") },
    { "ContactDisplayName",    TOK_STRING(PlayerCreatedStoryArc,pchContactDisplayName,0) },
    { "ContactType",        TOK_INT(PlayerCreatedStoryArc,eContactType, 0), ContactTypeEnum },
    { "ContactCategory",    TOK_STRING(PlayerCreatedStoryArc,pchContactCategory, 0) },
    { "Contact",            TOK_STRING(PlayerCreatedStoryArc,pchContact,0) },
    { "ContactGroup",        TOK_STRING(PlayerCreatedStoryArc,pchContactGroup, 0) },
    { "ContactCostumeIdx",    TOK_INT(PlayerCreatedStoryArc, costumeIdx, 0), 0 },
    { "CustomCritterIdx",    TOK_INT(PlayerCreatedStoryArc, customContactId, 0) },
    { "CustomNPCCostumeIdx",        TOK_INT(PlayerCreatedStoryArc, customNPCCostumeId, 0) },
    { "AboutContact",        TOK_STRING(PlayerCreatedStoryArc, pchAboutContact, 0) },

    { "Morality",            TOK_INT(PlayerCreatedStoryArc,eMorality,1), MoralityEnum },
    { "SouvenirClue",        TOK_STRING(PlayerCreatedStoryArc, pchSouvenirClueName, 0) },
    { "SouvenirClueDesc",    TOK_STRING(PlayerCreatedStoryArc, pchSouvenirClueDesc, 0) },

    { "StartMission",        TOK_STRUCT(PlayerCreatedStoryArc, ppMissions, ParsePlayerCreatedMission) },

    { "CustomVillainGroup",    TOK_STRUCT(PlayerCreatedStoryArc, ppCustomVGs, parse_CompressedCVG) },
    { "CustomCritter",        TOK_STRUCT(PlayerCreatedStoryArc, ppCustomCritters, parse_PCC_Critter) },
    { "NPCCostume",            TOK_STRUCT(PlayerCreatedStoryArc, ppNPCCostumes, ParseCustomNPCCostume) },
    { "Costumes",            TOK_STRUCT(PlayerCreatedStoryArc, ppBaseCostumes, ParseCostume) },
    { "EndStoryArc", TOK_END },
    {0},
};

StaticDefineInt * playerCreatedSDIgetByName( SpecialAction eAction )
{
    switch( eAction )
    {
        xcase kAction_EnumMorality: return MoralityEnum;
        xcase kAction_EnumAlignment: return AlignmentTypeEnum;
        xcase kAction_EnumDifficulty: return DifficultyTypeEnum;
        xcase kAction_EnumDifficultyWithSingle: return DifficultyWithSingleEnum;
        xcase kAction_EnumPacing: return PacingTypeEnum;
        xcase kAction_EnumPlacement: return PlacementTypeEnum;
        xcase kAction_EnumDetail: return DetailTypeEnum;
        xcase kAction_BuildModelList: return ObjectLocationTypeEnum;
        xcase kAction_EnumPersonCombat: return PersonCombatEnum;
        xcase kAction_EnumPersonBehavior: return PersonBehaviorEnum;
        xcase kAction_EnumMapLength: return MapLengthEnum;
        xcase kAction_EnumContactType: return ContactTypeEnum;
        xcase kAction_EnumRumbleType: return RumbleTypeEnum;
        xcase kAction_EnumArcStatus: return ArcStatusEnum;
    }
    return 0;
}

PlayerCreatedStoryArc* playerCreatedStoryArc_FromString(char *str, Entity *creator, int validate, char **failErrors, char **playableErrors, int *hasErrors)
{
    PlayerCreatedStoryArc *pArc = StructAllocRaw(sizeof(PlayerCreatedStoryArc));
    if(ParserReadTextEscaped(&str, ParsePlayerStoryArc, pArc))
    {
#ifndef TEST_CLIENT
        //we only do fixup if we're requesting playable error results.
        int doFixup = !!playableErrors;
        PlayerCreatedStoryarcValidation valid;
#ifndef SERVER
        doFixup = 0;    //only ever do this on the server
#endif
        if(validate)
        {
            valid = playerCreatedStoryarc_GetValid(pArc, str, failErrors, playableErrors, creator, doFixup);
            if(hasErrors)
                *hasErrors = valid != kPCS_Validation_OK;
        }
        if(!validate || valid == kPCS_Validation_OK || (doFixup && valid==kPCS_Validation_ERRORS))
            return pArc;
#endif
    }

    StructDestroy(ParsePlayerStoryArc, pArc);
    return 0;
}

int playerCreatedStoryArc_ValidateFromString(char *str, int allowErrors)
{
    PlayerCreatedStoryArc *pArc = StructAllocRaw(sizeof(PlayerCreatedStoryArc));
    int valid = 0;
    if(ParserReadTextEscaped(&str, ParsePlayerStoryArc, pArc))
    {
        PlayerCreatedStoryarcValidation isvalid= playerCreatedStoryarc_GetValid(pArc, str, 0, 0, 0, 0);

        if(isvalid == kPCS_Validation_OK || (allowErrors && isvalid==kPCS_Validation_ERRORS))
            valid = 1;
    }

    StructDestroy(ParsePlayerStoryArc, pArc);
    return valid;
}

char * playerCreatedStoryArc_Receive(Packet * pak)
{
    return (char*)pktGetZipped(pak,0);
}


char * playerCreatedStoryArc_ToString(PlayerCreatedStoryArc * pArc)
{
    static char  * str = NULL;
    if(!str)
        estrCreate(&str);
    else
        estrClear(&str);
    ParserWriteTextEscaped( &str, ParsePlayerStoryArc, pArc, 0, 0 );
    return str;
}

#ifndef TEST_CLIENT
void playerCreatedStoryArc_Send(Packet * pak, PlayerCreatedStoryArc *pArc)
{
    char * estr;
    estrCreate(&estr);
    estrConcatCharString(&estr, playerCreatedStoryArc_ToString(pArc));
    pktSendZipped(pak, estrLength(&estr), estr );
    estrDestroy(&estr);
}
#endif

PlayerCreatedStoryArc *playerCreatedStoryArc_Load( char * pchFile )
{
    PlayerCreatedStoryArc *pArc = StructAllocRaw(sizeof(PlayerCreatedStoryArc)); 

    if( ParserLoadFiles(NULL, pchFile, NULL, 0, ParsePlayerStoryArc, pArc, NULL, NULL, NULL) )
        return pArc;

    StructDestroy(ParsePlayerStoryArc, pArc);
    return NULL;
}

int playerCreatedStoryArc_Save( PlayerCreatedStoryArc * pArc, char * pchFile )
{
    mkdirtree(pchFile);
    if( ParserWriteTextFile(pchFile, ParsePlayerStoryArc, pArc, 0, 0) )
        return 1;
    return 0;
}

void playerCreatedStoryArc_Destroy(PlayerCreatedStoryArc *pArc)
{
    if(pArc)
        StructDestroy(ParsePlayerStoryArc, pArc);
}

#include "mission/MissionSearch.h"

void playerCreatedStoryArc_FillSearchHeader(PlayerCreatedStoryArc *arc, MissionSearchHeader *header, char *author)
{
     int i, j, length = 0;
    int uniqueVGroups = 0;


#ifndef TEST_CLIENT
    StructClear(parse_MissionSearchHeader, header);
#endif
    ZeroStruct(header);

    if(!arc) // leave it cleared
        return;

    if(author)
    {
        strncpyt(header->author, author, MISSIONSEARCH_AUTHORLEN);
    }
    if( arc->pchName )
        strncpyt(header->title, arc->pchName, MISSIONSEARCH_TITLELEN);
    if(arc->pchDescription)
        strncpyt(header->summary, arc->pchDescription, MISSIONSEARCH_SUMMARYLEN);
    header->arcStatus = arc->status;

    header->levelRangeLow = 0;
    header->levelRangeHigh = 0;

     for(i = 0; i < eaSize(&arc->ppMissions); i++)
    {
        int vId;
        int partialLength = 0;
        MissionHeaderDescription *pDesc = StructAllocRaw(sizeof(MissionHeaderDescription));

        if(!arc->ppMissions[i] || !arc->ppMissions[i]->pchVillainGroupName)
            break; // bad

        if( arc->ppMissions[i]->iMaxLevel && arc->ppMissions[i]->iMaxLevel < arc->ppMissions[i]->iMinLevelAuto )
            header->hazards |= (1<<HAZARD_HIGHLEVEL_DOWN);

        if (arc->ppMissions[i]->CVGIdx)
        {
            vId = villainGroupGetEnum("CustomCritter");
        }
        else
            vId = villainGroupGetEnum(arc->ppMissions[i]->pchVillainGroupName);

        BitFieldSet( header->villaingroups, MISSIONSEARCH_VG_BITFIELDSIZE, vId, 1 );

        for( j = 0; j < eaSize(&arc->ppMissions[i]->ppDetail); j++ )
        {
            header->missionType |= (1<<arc->ppMissions[i]->ppDetail[j]->eDetailType);
            pDesc->detailTypes |= (1<<arc->ppMissions[i]->ppDetail[j]->eDetailType);

            if( arc->ppMissions[i]->ppDetail[j]->eDetailType == kDetail_GiantMonster )
                header->hazards |= (1<<HAZARD_GIANT_MONSTER);
            if( arc->ppMissions[i]->ppDetail[j]->eDetailType == kDetail_Boss )
            {
                if ( !arc->ppMissions[i]->ppDetail[j]->customCritterId )
                {
                    const VillainDef * pDef;
                    
                    if( !arc->ppMissions[i]->ppDetail[j]->pchVillainDef )
                        continue;
                    
                    pDef = villainFindByName(arc->ppMissions[i]->ppDetail[j]->pchVillainDef);

                    if (!pDef)
                    {
                        continue;
                    }

                    if( pDef->rank == VR_ARCHVILLAIN || pDef->rank == VR_ARCHVILLAIN2 || pDef->rank == VR_BIGMONSTER )
                        header->hazards |= (1<<HAZARD_ARCHVILLAIN);
                    if( pDef->rank == VR_ELITE )
                        header->hazards |= (1<<HAZARD_ELITEBOSS);
                    if( pDef->rank == VR_PET )
                    {
                        const char * pchClass = pDef->characterClassName; 
                        if( strstriConst(pchClass, "arch") || strstriConst(pchClass, "monster") )
                            header->hazards |= (1<<HAZARD_ARCHVILLAIN);
                        else if( strstriConst( pchClass, "elite") )
                            header->hazards |= (1<<HAZARD_ELITEBOSS);
                    }
                }
            }
        }
        for( j = 0; j < eaSize(&arc->ppMissions[i]->ppMissionCompleteDetails); j++ )
        {
            if( stricmp( arc->ppMissions[i]->ppMissionCompleteDetails[j], "DefeatAllVillains") == 0 ||
                stricmp( arc->ppMissions[i]->ppMissionCompleteDetails[j], "DefeatAllVillainsInEndRoom") == 0 )
            {
                header->missionType |= (1<<kDetail_DefeatAll);
                pDesc->detailTypes |= (1<<kDetail_DefeatAll);
            }
        }

        partialLength = arc->ppMissions[i]->eMapLength;
        partialLength = (partialLength)?partialLength:45;
        if( arc->ppMissions[i]->pchDefeatAll )
            partialLength*=2;
        length += partialLength;
        pDesc->size = arc->ppMissions[i]->eMapLength;
        pDesc->min = arc->ppMissions[i]->iMinLevel?arc->ppMissions[i]->iMinLevel:arc->ppMissions[i]->iMinLevelAuto;
        pDesc->max = arc->ppMissions[i]->iMaxLevel?arc->ppMissions[i]->iMaxLevel:arc->ppMissions[i]->iMaxLevelAuto; 

        eaPush(&header->missionDesc, pDesc);
    }

    for( i = eaSize(&arc->ppCustomCritters)-1; i>=0; i-- )
    {
        PCC_Critter *pCritter = arc->ppCustomCritters[i];

        if( pCritter->rankIdx == PCC_RANK_ARCH_VILLAIN )
            header->hazards |= (1<<HAZARD_ARCHVILLAIN);
        if( pCritter->rankIdx == PCC_RANK_ELITE_BOSS )
            header->hazards |= (1<<HAZARD_ELITEBOSS);

        if( (pCritter->difficulty[0] == PCC_DIFF_CUSTOM) || (pCritter->difficulty[1] == PCC_DIFF_CUSTOM) )
            header->hazards |= (1<<HAZARD_CUSTOM_POWERS);

        if( pCritter->difficulty[0] == PCC_DIFF_EXTREME || pCritter->difficulty[1] == PCC_DIFF_EXTREME )
        {
            switch( pCritter->rankIdx )
            {
                case PCC_RANK_MINION: header->hazards |= (1<<HAZARD_EXTREME_MINIONS); break;
                case PCC_RANK_LT: header->hazards |= (1<<HAZARD_EXTREME_LT); break;
                case PCC_RANK_BOSS: header->hazards |= (1<<HAZARD_EXTREME_BOSS); break;
                case PCC_RANK_ELITE_BOSS: header->hazards |= (1<<HAZARD_EXTREME_ELITEBOSS); break;
                case PCC_RANK_ARCH_VILLAIN: header->hazards |= (1<<HAZARD_EXTREME_ARCHVILLAIN); break;
            }
        }
    }
    for (i = eaSize(&arc->ppCustomVGs)-1; i>=0; --i)
    {
        for (j = (eaSize(&arc->ppCustomVGs[i]->existingVillainList)-1); j>= 0; --j)
        {
            const VillainDef * pDef = villainFindByName(arc->ppCustomVGs[i]->existingVillainList[j]->pchName);

            if (!pDef)
            {
                continue;
            }

            if( pDef->rank == VR_ARCHVILLAIN || pDef->rank == VR_ARCHVILLAIN2 || pDef->rank == VR_BIGMONSTER )
                header->hazards |= (1<<HAZARD_ARCHVILLAIN);
            if( pDef->rank == VR_ELITE )
                header->hazards |= (1<<HAZARD_ELITEBOSS);
            if( pDef->rank == VR_PET )
            {
                const char * pchClass = pDef->characterClassName; 
                if( strstriConst(pchClass, "arch") || strstriConst(pchClass, "monster") )
                    header->hazards |= (1<<HAZARD_ARCHVILLAIN);
                else if( strstriConst( pchClass, "elite") )
                    header->hazards |= (1<<HAZARD_ELITEBOSS);
            }
        }
    }

    //TODO: switch these to their actual value instead of their bitfield.
    // in order to do this, we'll have to make everything backwards compatible.
    // Right now we've got to treat these separately from everything else.
    if( length <= 30 )
        header->arcLength |= (1<<kArcLength_VeryShort);
    else if( length <= 60 )
        header->arcLength |= (1<<kArcLength_Short);
    else if( length <= 120 )
        header->arcLength |= (1<<kArcLength_Medium);
    else if( length <= 180 )
        header->arcLength |= (1<<kArcLength_Long);
    else
        header->arcLength |= (1<<kArcLength_VeryLong);

    header->morality = (1<<arc->eMorality);
    header->locale_id = arc->locale_id;
}

int    playerCreatedStoryArc_NPCIdByCostumeIdx(const char *DefName, int costumeIdx, int **returnList )
{
    int j, k, **ids, *temp;
    const NPCDef *npcDef;
    int costume;
    const VillainDef *vDef;

    if(returnList)
        ids = returnList;
    else
        ids = &temp;

    if(!DefName)
        return 0;

    vDef = villainFindByName(DefName);
    if(!vDef)
    {
        int index;
        npcFindByName( DefName, &index);
        return index;
    }
    eaiCreate(ids);
    for(j = 0; j < eaSize(&vDef->levels) && (eaiSize(ids) < costumeIdx+1 || costumeIdx<0); j++)
    {
        for(k = eaSize(&vDef->levels[j]->costumes)-1; k >=0; k--)
        {
            int index;
            npcDef = npcFindByName( vDef->levels[j]->costumes[k], &index);
            eaiPushUnique(ids, index);
        }
    }
    if(eaiSize(ids) && (costumeIdx < 0 || eaiSize(ids) < costumeIdx+1))    //we didn't find it
        costume = (*ids)[0];
    else if(eaiSize(ids))
        costume = (*ids)[costumeIdx];
    else
        costume = 0;
    if(!returnList)
        eaiDestroy(ids);
    return costume;
}

int playerCreatedStoryArc_rateLimiter(U32 *lastRequested, U32 numberRequests, U32 timespan, U32 time)
{
    U32 i;

    for(i = 0; i < numberRequests; i++)
    {
        if(lastRequested[i]+timespan <= time)
        {
            lastRequested[i] = time;
            return 1;
        }
    }
    return 0;
}
void playerCreatedStoryArc_AwardPCCCostumePart( Entity *e, const char *pchUnlockKey)
{
    MMUnlockableCostumeSetContent *pCostumeSet = 0;
    if ( playerCreatedStoryArc_IsCostumeKey( pchUnlockKey, &pCostumeSet) )
    {
        int i;
        for (i = 0; i < eaSize(&pCostumeSet->costume_keys); ++i)
        {
            COSTUME_GIVE_TOKEN( pCostumeSet->costume_keys[i], 0, 1);
        }
    }
}
void playerCreatedStoryArc_UnawardPCCCostumePart( Entity *e, const char *pchUnlockKey )
{
    MMUnlockableCostumeSetContent *pCostumeSet = 0;
    if ( playerCreatedStoryArc_IsCostumeKey( pchUnlockKey, &pCostumeSet) )
    {
        int i;
        for (i = 0; i < eaSize(&pCostumeSet->costume_keys); ++i)
        {
            COSTUME_TAKE_TOKEN( pCostumeSet->costume_keys[i], 1);
        }
    }
}

void playerCreatedStoryArc_PCCSetCritterCostume( PCC_Critter *pcc, PlayerCreatedStoryArc *pArc)
{
    //    This does not do any validation
    if (!pcc->pccCostume)
    {
        int i;
        CostumeDiff *diffCostume;
        diffCostume = pcc->compressedCostume;
        pcc->pccCostume = costume_clone(pArc->ppBaseCostumes[diffCostume->costumeBaseNum]);
        pcc->pccCostume->appearance = diffCostume->appearance;
        for (i = 0; i < eaSize(&diffCostume->differentParts); i++)
        {
            StructCopy(ParseCostumePart, diffCostume->differentParts[i]->part, pcc->pccCostume->parts[diffCostume->differentParts[i]->index], 0, 0);
        }
    }
}
