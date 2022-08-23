#ifndef PLAYER_CREATED_STORYARC_H
#define PLAYER_CREATED_STORYARC_H
#include <utilitieslib/utils/textparser.h>
#include "comm_backend.h"
#include "mission/MissionSearch.h"

typedef struct cCostume cCostume;
typedef struct Costume Costume;
typedef struct CustomNPCCostume CustomNPCCostume;
typedef struct PCC_Critter PCC_Critter;
typedef struct CompressedCVG CompressedCVG;
typedef struct Entity Entity;
typedef enum MoralityType
{
    kMorality_Evil = 1,
    kMorality_Neutral,
    kMorality_Good,
    kMorality_Vigilante,
    kMorality_Rogue,
    kMorality_Count,
}MoralityType;

#define MORALITYTYPE_ALL ((1<<kMorality_Count)-1)

typedef enum AlignmentType
{
    kAlignment_Enemy,
    kAlignment_Ally,
    kAlignment_Rogue,
    kAlignment_Count,
}AlignmentType;

#define ALIGNMENTTYPE_ALL ((1<<kAlignment_Count)-1)


typedef enum DifficultyType
{
    kDifficulty_Easy,
    kDifficulty_Medium,
    kDifficulty_Hard,
    kDifficulty_Single,
    kDifficulty_Count,
}DifficultyType;


typedef enum PacingType
{
    kPacing_Flat,
    kPacing_BackLoaded,
    kPacing_SlowRampUp,
    kPacing_Staggered,
    kPacing_Count,
}PacingType;

typedef enum PlacementType
{
    kPlacement_Any,
    kPlacement_Front,
    kPlacement_Middle,
    kPlacement_Back,
    kPlacement_Count,
}PlacementType;

typedef enum ObjectLocationType
{
    kObjectLocationType_Person,
    kObjectLocationType_Wall,
    kObjectLocationType_Floor,
    kObjectLocationType_Roof,
    kObjectLocationType_Count,
}ObjectLocationType;

typedef enum PersonCombat
{
    kPersonCombat_NonCombat,
    kPersonCombat_Pacifist,
    kPersonCombat_Aggressive,
    kPersonCombat_Defensive,
    kPersonCombat_Count,
}PersonCombat;

typedef enum PersonBehavior 
{
    kPersonBehavior_Follow,
    kPersonBehavior_NearestDoor,
    kPersonBehavior_RunAway,
    kPersonBehavior_Wander,
    kPersonBehavior_Nothing, 
    kPersonBehavior_Count, 
}PersonBehavior;

typedef enum MapLength
{
    kMapLength_Tiny = 15,
    kMapLength_Small = 30,
    kMapLength_Medium = 45,
    kMapLength_Large = 60,
}MapLength;


typedef enum ArcLength
{
    kArcLength_VeryShort,    // < 30
    kArcLength_Short,        // 30-60
    kArcLength_Medium,        // 60-120
    kArcLength_Long,        // 120-180
    kArcLength_VeryLong,    // 180+
    kArcLength_Count,
}ArcLength;

#define MISSIONSEARCH_ARCLENGTH_ALL ((1<<kArcLength_Count)-1)

typedef enum ContactType
{
    kContactType_Default,
    kContactType_Contact,
    kContactType_Critter,
    kContactType_Object,
    kContactType_PCC,
    kContactType_Count,
}ContactType;

typedef enum RumbleType
{
    kRumble_RealFight,
    kRumble_Ally1,
    kRumble_Ally2,
}RumbleType;

typedef enum ArcStatus
{
    kArcStatus_WorkInProgress,
    kArcStatus_LookingForFeedback,
    kArcStatus_Final,
    kArcStatus_Count,
}ArcStatus;


typedef enum PCStoryComplaint
{
    kComplaint_Complaint,
    kComplaint_Comment,
    kComplaint_BanMessage,
}PCStoryComplaint;

#define MISSIONSEARCH_ARCSTATUS_ALL ((1<<kArcStatus_Count)-1)

typedef enum PlayerCreatedDetailType PlayerCreatedDetailType;
typedef enum MissionServerPurchaseFlags MissionServerPurchaseFlags;

#if defined(CLIENT)
// Clientside version of the inventory. It only includes things we want to show
// in the UI or to make clientside mission checking easier - ie. before the full
// validation on the server.
typedef struct MissionInventory
{
    U32 banned_until;
    U32 claimable_tickets;
    U32 publish_slots;
    U32 publish_slots_used;
    U32 permanent;
    U32 uncirculated_slots;
    U32 invalidated;
    U32 playableErrors;
    char **unlock_keys;
    StashTable stKeys;
    bool dirty;
} MissionInventory;

#elif SERVER

#define INVENTORY_ORDER_TIMEOUT_SECS        30

typedef struct MissionInventoryOrder
{
    U32 tickets;
    MissionServerPurchaseFlags flags;
    U32 time;
} MissionInventoryOrder;

typedef struct MissionInventory
{
    U32 tickets;
    U32 lifetime_tickets;
    U32 overflow_tickets;
    U32 badge_flags;
    U32 publish_slots;
    U32 publish_slots_used;
    U32 permanent;
    U32 uncirculated_slots;
    U32 created_completions;
    U32 banned_until;
    U32 invalidated;
    U32 playableErrors;
    char **unlock_keys;
    StashTable stKeys;
    MissionInventoryOrder **pending_orders;
} MissionInventory;
#endif


// This was originally multiple structures, there is a lot of repeated info near the top
// not sure if I want to make mulitple subtypes or not
typedef struct PlayerCreatedDetail
{
    char * pchName;
    PlayerCreatedDetailType eDetailType;
    char * pchOverrideGroup;
    int pchCustomNPCCostumeIdx;
    char * pchDisplayInfo;

    char *pchVillainDef; // This is the main character
    char *pchEntityType; // can probably be an enum
    char *pchEntityGroupName; // the person or boss can be in different group
    int iEntityVillainSet;
    

    char *pchSupportVillainDef; // Any supporting character
    char *pchSupportVillainType; 
    RumbleType eRumbleType;

    char *pchEnemyGroupType;
    char *pchVillainGroupName;
    int iVillainSet;

    char *pchEnemyGroupType2;
    char *pchVillainGroup2Name; // For rumbles
    int iVillainSet2;

    int ePlacement;

    int iQty;
    AlignmentType eAlignment;
    DifficultyType eDifficulty;

    char *pchDialogInactive;
    char *pchDialogActive;

    char *pchObjectiveDescription;
    char *pchObjectiveSingularDescription;
    char *pchRewardDescription;
    char *pchDefaultStartingAnim;

    char *pchClueName;
    char *pchClueDesc;

    char *pchDetailTrigger;

    int  customCritterId;
    int     CVGIdx;
    int     CVGIdx2;
    //--- Above this is generic to any type of thing
    
    // Boss
    char *pchDialogAttack;
    char *pchDialog75;
    char *pchDialog50;
    char *pchDialog25;
    char *pchDeath;
    char *pchKills;
    char *pchHeld;
    char *pchFreed;
    char *pchAfraid;
    char *pchRunAway;
    char *pchRunAwayHealthThreshold;
    char *pchRunAwayTeamThreshold;
    char *pchBossStartingAnim;
    int  bBossOnly;

    // Collection
    int iInteractTime;
    int eObjectLocation;
    int bObjectiveRemove;
    int bObjectiveFadein;
    char *pchBeginText;
    char *pchInterruptText;
    char *pchCompleteText;
    char *pchInteractBarText;
    char *pchObjectWaiting;
    char *pchObjectReset;

    // Defend
    char *pchAttackerDialog1;
    char *pchAttackerDialog2;
    char *pchAttackerHeroAlertDialog;

    // Destroy
    char *pchGuard1;
    char *pchGuard2;

    // Patrol
    char *pchPatroller1;
    char *pchPatroller2;

    // Person
    int ePersonCombatType;
    char *pchEscortDestination;
    int bEscortToObject;
    char *pchPersonInactive;
    char *pchPersonActive;

    char *pchPersonSuccess;
    char *pchPersonSayOnRescue;
    char *pchPersonSayOnArrival;
    char *pchPersonSayOnReRescue;
    char *pchPersonSayOnStranded;
    char *pchPersonSayOnClickAfterRescue;
    char *pchEscortWaypoint;
    char *pchEscortStatus;
    char *pchEscortObjective;
    char *pchEscortObjectiveSingular;

    char *pchPersonStartingAnimation;
    char *pchRescueAnimation;
    char *pchReRescueAnimation;
    char *pchArrivalAnimation;
    char *pchStrandedAnimation;

    char * pchBetrayalTrigger;
    char * pchSayOnBetrayal;
    int bPersonBetrays; // deprecated


    PersonBehavior ePersonRescuedBehavior;
    PersonBehavior ePersonArrivedBehavior;

    int costumeIdx;

}PlayerCreatedDetail;


typedef struct PlayerCreatedStoryArc PlayerCreatedStoryArc;

typedef struct PlayerCreatedMission
{
    char *pchTitle;
    char *pchSubTitle;

    char *pchClueName;
    char *pchClueDesc;

    char *pchStartClueName;
    char *pchStartClueDesc;

    U32 uID;

    F32 fFogDist;
    char *pchFogColor;

    int iMinLevel; // These are set by player
    int iMaxLevel; //

    int iMinLevelAuto; // These are not set by player
    int iMaxLevelAuto; //


    char *pchMapName;    // if mapname is random or not set, use the set and length to determine the misison
    char *pchMapSet; // Type of map to use barring actual file
    char *pchUniqueMapCat; // just saved for the UI
    int eMapLength;    // In Minutes
    int    bNoTeleportExit;

    char *pchEnemyGroupType;
    char *pchVillainGroupName;
    int     CVGIdx;
    int iVillainSet;
    PacingType ePacing;

    F32 fTimeToComplete;

    char *pchIntroDialog;
    char *pchAcceptDialog;
    char *pchDeclineDialog;
    char *pchSendOff;
    char *pchDeclineSendOff;
    char *pchActiveTask;
    char *pchEnterMissionDialog;
    char *pchStillBusyDialog;

    char *pchTaskSuccess;
    char *pchTaskFailed;

    char *pchTaskReturnSuccess;
    char *pchTaskReturnFail;

    char * pchDefeatAll;

    char **ppMissionCompleteDetails;
    char **ppMissionFailDetails;

    PlayerCreatedDetail **ppDetail;
    PlayerCreatedStoryArc *pParentArc;

}PlayerCreatedMission;


typedef struct PlayerCreatedStoryArc
{
    char *pchName;
    char *pchDescription;
    char *pchAuthor;
    char *pchSouvenirClueName;
    char *pchSouvenirClueDesc;
    char *pchGuestAuthorBio;
    char *pchAboutContact;
    int status;

    ContactType eContactType;
    char *pchContactDisplayName;
    char *pchContact; 
    char *pchContactCategory;
    char *pchContactGroup;
    int iGroupSet;
    int costumeIdx;
    int customContactId;
    int customNPCCostumeId;

    int eMorality;
    int keywords;
    int writer_dbid;
    int locale_id;
    int european;

    U32 uID;
    U32 rating;

    PlayerCreatedMission **ppMissions;
    CompressedCVG **ppCustomVGs;
    PCC_Critter **ppCustomCritters;
    const cCostume **ppBaseCostumes;
    CustomNPCCostume **ppNPCCostumes;
    
}PlayerCreatedStoryArc;

extern StaticDefineInt PacingTypeEnum[]; 
extern StaticDefineInt PlacementTypeEnum[]; 
extern StaticDefineInt DetailTypeEnum[];
extern StaticDefineInt MapLengthEnum[];
extern StaticDefineInt ArcStatusEnum[];

extern TokenizerParseInfo ParsePlayerCreatedClue[];
extern TokenizerParseInfo ParsePlayerCreatedDetail[];
extern TokenizerParseInfo ParsePlayerCreatedMission[];
extern TokenizerParseInfo ParsePlayerStoryArc[];

PlayerCreatedStoryArc* playerCreatedStoryArc_FromString(char *str, Entity *e, int validate, char **failErrors, char **playableErrors, int *hasErrors);
int playerCreatedStoryArc_ValidateFromString(char *str, int allowErrors);
char* playerCreatedStoryArc_ToString(PlayerCreatedStoryArc * pArc);

char* playerCreatedStoryArc_Receive(Packet * pak);
void playerCreatedStoryArc_Send(Packet * pak, PlayerCreatedStoryArc *pArc);

PlayerCreatedStoryArc *playerCreatedStoryArc_Load( char * pchFile );
int playerCreatedStoryArc_Save( PlayerCreatedStoryArc* pArc, char * pchFile );
void playerCreatedStoryArc_Destroy(PlayerCreatedStoryArc* pArc);

typedef enum SpecialAction SpecialAction;
StaticDefineInt * playerCreatedSDIgetByName( SpecialAction action );

typedef struct MissionSearchHeader MissionSearchHeader;
void playerCreatedStoryArc_FillSearchHeader(PlayerCreatedStoryArc *arc, MissionSearchHeader *header, char *author);
int    playerCreatedStoryArc_NPCIdByCostumeIdx(const char *DefName, int costumeIdx, int **returnList);

int playerCreatedStoryArc_rateLimiter(U32 *lastRequested, U32 numberRequests, U32 timespan, U32 currentTime);
void playerCreatedStoryArc_PCCSetCritterCostume( PCC_Critter *pcc, PlayerCreatedStoryArc *pArc);
void playerCreatedStoryArc_AwardPCCCostumePart( Entity *e, const char *pchUnlockKey);
void playerCreatedStoryArc_UnawardPCCCostumePart( Entity *e, const char *pchUnlockKey );
#endif