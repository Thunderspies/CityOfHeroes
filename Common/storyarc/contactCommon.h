// contactCommon.h - contact dialog links and other stuff server and client have to agree on

#ifndef CONTACTCOMMON_H
#define CONTACTCOMMON_H

#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/structDefines.h>

#define LEN_MISSION_INTRO        256
#define LEN_CONTACT_SOL_DIALOG    1024
#define LEN_CONTACT_RESPONSE    100
#define LEN_CONTACT_DIALOG        20000
#define MAX_CONTACT_RESPONSES    11

#define ALIGNMENT_POINT_MAX_COUNT 5
#define ALIGNMENT_POINT_COOLDOWN_TIMER (20 * 60 * 60)

#define ALIGNMENT_POINT_HERO 1
#define ALIGNMENT_POINT_VIGILANTE 2
#define ALIGNMENT_POINT_VILLAIN 3
#define ALIGNMENT_POINT_ROGUE 4
#define ALIGNMENT_POINT_PARAGON 5
#define ALIGNMENT_POINT_TYRANT 6

typedef enum TipType
{
    TIPTYPE_NONE,
    // Non-tip contacts have this TipType.

    TIPTYPE_ALIGNMENT,
    // These tips will drop if no TIPTYPE_MORALITY tip drops during /alignment_tip_drop.
    // Generally used for Alignment Missions, which will increase the player's alignment points when completed.
    // These tips have a seven-length streakbreaker.
    // A specific Alignment tip will not drop for you if it has dropped in the previous seven Alignment tip drops.
    // You are only allowed to hold three Alignment and Morality tips at once.

    TIPTYPE_MORALITY,
    // These tips are prioritized to drop over Alignment tips during /alignment_tip_drop.
    // Generally used for Morality Missions, which will change the alignment of the player when completed.
    // You are only allowed to hold three Alignment and Morality tips at once.

    TIPTYPE_GRSYSTEM_MAX,
    // This is a reference value to denote the end of the tips that are dropped
    // through the standard /alignment_tip_drop method.
    // No contact should have this value.

    TIPTYPE_SPECIAL, 
    // This type goes in the tip window, but at the end of the list.
    // Also, all tips of this type will be dropped whenever /alignment_tip_drop occurs.
    // These tips do not count as the "one tip" to be dropped.
    // InteractionRequires and player level ranges still apply for when they are dropped,
    // but these tips are never revoked, even if you no longer meet those requirements.

    TIPTYPE_STANDARD_COUNT,
    // Standard count value for the enum.
    // No contact should have this value.
} TipType;

// Contact dialog types
typedef enum CDtype
{
    CD_STANDARD = 0,
    CD_FLASHBACK_LIST,
    CD_CHALLENGE_SETTINGS,
    CD_CHALLENGE_SETTINGS_TASKFORCE,
    CD_ARCHITECT,
} CDtype;

// each option a contact gives you when talking
typedef struct ContactResponseOption
{
    char responsetext[LEN_CONTACT_RESPONSE];
    int link;
    char rightResponseText[LEN_CONTACT_RESPONSE];
    int rightLink;
} ContactResponseOption;

// a response from talking to a contact
typedef struct ContactResponse
{
    char dialog[LEN_CONTACT_DIALOG];
    char SOLdialog[LEN_CONTACT_SOL_DIALOG];
    ContactResponseOption responses[MAX_CONTACT_RESPONSES];
    int responsesFilled;
    CDtype type;
    bool dialogPageAutoClose;
} ContactResponse;

// this is the list of destination types for a contact
typedef enum
{
    CONTACTDEST_CONTACT,
    CONTACTDEST_TASK,
    CONTACTDEST_MISSION,
    CONTACTDEST_ACTIVEMISSION,
    CONTACTDEST_EXPLICITTASK,
} ContactDestType;

// this is the list of integer links that the contact dialog uses
// NOTE change uicontactDialog.c if you modify this list
typedef enum
{
    // zero contactlink means NULL
    CONTACTLINK_HELLO = 1,
    CONTACTLINK_MAIN,
    CONTACTLINK_BYE,
    CONTACTLINK_MISSIONS,
    CONTACTLINK_LONGMISSION,
    CONTACTLINK_SHORTMISSION,
    CONTACTLINK_ACCEPTLONG,
    CONTACTLINK_ACCEPTSHORT,
    CONTACTLINK_INTRODUCE,
    CONTACTLINK_INTRODUCE_CONTACT1,
    CONTACTLINK_INTRODUCE_CONTACT2,
    CONTACTLINK_INTRODUCE_CONTACT3,
    CONTACTLINK_INTRODUCE_CONTACT4,
    CONTACTLINK_ACCEPT_CONTACT1,
    CONTACTLINK_ACCEPT_CONTACT2,
    CONTACTLINK_ACCEPT_CONTACT3,
    CONTACTLINK_ACCEPT_CONTACT4,
    CONTACTLINK_GOTOSTORE,
    CONTACTLINK_TRAIN,
    CONTACTLINK_WRONGMODE,
    CONTACTLINK_DONTKNOW,
    CONTACTLINK_FAILEDREQUIRES,
    CONTACTLINK_BADGEREQUIRED_FIRST,
    CONTACTLINK_BADGEREQUIRED_SECOND,
    CONTACTLINK_BADGEREQUIRED_THIRD,
    CONTACTLINK_BADGEREQUIRED_FOURTH,
    CONTACTLINK_BADGEREQUIRED_TOOLOW,
    CONTACTLINK_NOTLEADER,
    CONTACTLINK_BADCELLCALL,
    CONTACTLINK_ABOUT,
    CONTACTLINK_IDENTIFYCLUE,
    CONTACTLINK_NEWPLAYERTELEPORT_AP,
    CONTACTLINK_NEWPLAYERTELEPORT_GC,
    CONTACTLINK_NEWPLAYERTELEPORT_MI_KALINDA,
    CONTACTLINK_NEWPLAYERTELEPORT_MI_BURKE,
    CONTACTLINK_FORMTASKFORCE,
    CONTACTLINK_CHOOSE_TITLE,
    CONTACTLINK_GOTOTAILOR,
    CONTACTLINK_GOTORESPEC,
    CONTACTLINK_TEACH,
    CONTACTLINK_ENROLL_1,
    CONTACTLINK_ENROLL_2,
    CONTACTLINK_ENROLL_3,
    CONTACTLINK_ENROLL_END,
    CONTACTLINK_PVPSWITCH,
    CONTACTLINK_INFO_0,
    CONTACTLINK_INFO_1,
    CONTACTLINK_INFO_2,
    CONTACTLINK_INFO_3,
    CONTACTLINK_INFO_4,
    CONTACTLINK_INFO_5,
    CONTACTLINK_INFO_6,
    CONTACTLINK_INFO_7,
    CONTACTLINK_INFO_8,
    CONTACTLINK_ACCEPT_NEWSPAPER_TASK_1,
    CONTACTLINK_ACCEPT_NEWSPAPER_TASK_2,
    CONTACTLINK_ACCEPT_NEWSPAPER_TASK_3,
    CONTACTLINK_ACCEPT_HEIST,
    CONTACTLINK_DECLINE_HEIST,
    CONTACTLINK_SHOW_PVPPATROLMISSION,
    CONTACTLINK_SHOW_PVPMISSION1,
    CONTACTLINK_SHOW_PVPMISSION2,
    CONTACTLINK_SHOW_PVPMISSION3,
    CONTACTLINK_SHOW_PVPMISSION4,
    CONTACTLINK_ACCEPT_PVPPATROLMISSION,
    CONTACTLINK_ACCEPT_PVPMISSION1,
    CONTACTLINK_ACCEPT_PVPMISSION2,
    CONTACTLINK_ACCEPT_PVPMISSION3,
    CONTACTLINK_ACCEPT_PVPMISSION4,
    CONTACTLINK_WRONGALLIANCE,
    CONTACTLINK_WRONGUNIVERSE,
    CONTACTLINK_CREATE_SG,
    CONTACTLINK_PAY_RENT,
    CONTACTLINK_PAID_RENT,
    CONTACTLINK_BUY_BASE,
    CONTACTLINK_BOUGHT_BASE,
    CONTACTLINK_VIEW_SGLIST,
    CONTACTLINK_NOSG,
    CONTACTLINK_NOSGPERMS,
    CONTACTLINK_NEWS_ERROR,
    CONTACTLINK_SUBCONTACT1,
    CONTACTLINK_SUBCONTACT2,
    CONTACTLINK_SUBCONTACT3,
    CONTACTLINK_SUBCONTACT4,
    CONTACTLINK_SUBCONTACT5,
    CONTACTLINK_SUBCONTACT6,
    CONTACTLINK_SUBCONTACT7,
    CONTACTLINK_SUBCONTACT8,// 8 should be enough?
    CONTACTLINK_CONVERT_INF_START,
    CONTACTLINK_CONVERT_INF_1,
    CONTACTLINK_CONVERT_INF_10,
    CONTACTLINK_CONVERT_INF_100,
    CONTACTLINK_CONVERT_INF_ALL,
    CONTACTLINK_AUTO_COMPLETE,
    CONTACTLINK_AUTO_COMPLETE_CONFIRM,
    CONTACTLINK_FLASHBACK_LIST,
    CONTACTLINK_FLASHBACK_TELEPORT,
    CONTACTLINK_TRAINRESPEC,
    CONTACTLINK_ENDTRAIN,
    CONTACTLINK_STARTMISSION,
    CONTACTLINK_SWITCH_BUILD,
    CONTACTLINK_SELECTBUILD_1,
    CONTACTLINK_SELECTBUILD_2,
    CONTACTLINK_SELECTBUILD_3,
    CONTACTLINK_SELECTBUILD_4,
    CONTACTLINK_SELECTBUILD_5,
    CONTACTLINK_SELECTBUILD_6,
    CONTACTLINK_SELECTBUILD_7,
    CONTACTLINK_SELECTBUILD_8,
    CONTACTLINK_CONFIRMBUILD_1,
    CONTACTLINK_CONFIRMBUILD_2,
    CONTACTLINK_CONFIRMBUILD_3,
    CONTACTLINK_CONFIRMBUILD_4,
    CONTACTLINK_CONFIRMBUILD_5,
    CONTACTLINK_CONFIRMBUILD_6,
    CONTACTLINK_CONFIRMBUILD_7,
    CONTACTLINK_CONFIRMBUILD_8,
    CONTACTLINK_RENAME_BUILD,
    CONTACTLINK_DIFFICULTY_LEVEL,
    CONTACTLINK_DIFFICULTY_LEVEL_MINUS1,
    CONTACTLINK_DIFFICULTY_LEVEL_EVEN,
    CONTACTLINK_DIFFICULTY_LEVEL_PLUS1,
    CONTACTLINK_DIFFICULTY_LEVEL_PLUS2,
    CONTACTLINK_DIFFICULTY_LEVEL_PLUS3,
    CONTACTLINK_DIFFICULTY_LEVEL_PLUS4,
    CONTACTLINK_DIFFICULTY_TEAMSIZE,
    CONTACTLINK_DIFFICULTY_TEAMSIZE_1,
    CONTACTLINK_DIFFICULTY_TEAMSIZE_2,
    CONTACTLINK_DIFFICULTY_TEAMSIZE_3,
    CONTACTLINK_DIFFICULTY_TEAMSIZE_4,
    CONTACTLINK_DIFFICULTY_TEAMSIZE_5,
    CONTACTLINK_DIFFICULTY_TEAMSIZE_6,
    CONTACTLINK_DIFFICULTY_TEAMSIZE_7,
    CONTACTLINK_DIFFICULTY_TEAMSIZE_8,
    CONTACTLINK_DIFFICULTY_ALLOW_BOSS,
    CONTACTLINK_DIFFICULTY_DISALLOW_BOSS,
    CONTACTLINK_DIFFICULTY_ALWAYS_AV,
    CONTACTLINK_DIFFICULTY_NEVER_AV,
    CONTACTLINK_ABANDON,
    CONTACTLINK_DISMISS,
    CONTACTLINK_DISMISS_CONFIRM,
    CONTACTLINK_WEBSTORE,
    // STOP - put changes to this table into ParseContactLink as well
} ContactLink;

typedef enum ContactAlliance
{
    ALLIANCE_UNSPECIFIED,
    ALLIANCE_NEUTRAL,
    ALLIANCE_HERO,
    ALLIANCE_VILLAIN,
    ALLIANCE_COUNT
} ContactAlliance;

typedef enum ContactUniverse
{
    UNIVERSE_UNSPECIFIED,
    UNIVERSE_BOTH,
    UNIVERSE_PRIMAL,
    UNIVERSE_PRAETORIAN,
    UNIVERSE_COUNT
} ContactUniverse;

#define ContactCorrectAlliance(playerType, contactAlliance) (contactAlliance == ALLIANCE_NEUTRAL || \
    contactAlliance == ALLIANCE_UNSPECIFIED || \
    (contactAlliance == ALLIANCE_HERO && playerType ==kPlayerType_Hero) || \
    (contactAlliance == ALLIANCE_VILLAIN && playerType ==kPlayerType_Villain))

#define ContactCorrectUniverse(praetorianProgress, contactUniverse) (contactUniverse == UNIVERSE_BOTH || \
    ((contactUniverse == UNIVERSE_PRIMAL || contactUniverse == UNIVERSE_UNSPECIFIED) && (praetorianProgress == kPraetorianProgress_PrimalBorn || praetorianProgress == kPraetorianProgress_PrimalEarth || praetorianProgress == kPraetorianProgress_NeutralInPrimalTutorial)) || \
    (contactUniverse == UNIVERSE_PRAETORIAN && (praetorianProgress == kPraetorianProgress_Tutorial || praetorianProgress == kPraetorianProgress_Praetoria)))

#define PlayerAllianceMatchesDefAlliance(playerAlliance, defAlliance) \
    ((defAlliance == ALLIANCE_NEUTRAL || defAlliance == ALLIANCE_UNSPECIFIED ) || \
    (playerAlliance == defAlliance))

#define PlayerUniverseMatchesDefUniverse(playerUniverse, defUniverse) \
    (defUniverse == UNIVERSE_BOTH || \
    ((defUniverse == UNIVERSE_PRIMAL || defUniverse == UNIVERSE_UNSPECIFIED) && playerUniverse == UNIVERSE_PRIMAL) || \
    (defUniverse == UNIVERSE_PRAETORIAN && playerUniverse == UNIVERSE_PRAETORIAN))

extern StaticDefineInt ParseContactLink[];

#endif