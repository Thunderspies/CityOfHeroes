/*\
 *
 *    storyarcprivate.h - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    provides structure definitions and includes global to
 *    story systems
 *
 */

#ifndef __STORYARCPRIVATE_H
#define __STORYARCPRIVATE_H

// every story-related file needs access to these basics:
#include "storyarcinterface.h"
#include "storyarcutil.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/scriptvars.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include "entity/entserver.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/bitfield.h>
#include <utilitieslib/utils/error.h>
#include "gameComm/sendToClient.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/SharedMemory.h>
#include <time.h>
#include <utilitieslib/utils/timing.h>
#include "gameData/PowerNameRef.h"    // for PowerNameRef
#include "gameData/store.h"            // for MultiStore
#include "language/langServerUtil.h"
#include "script/scriptengine.h"

// and they are very incestual by nature:
#include "contact.h"
#include "task.h"
#include "mission.h"
#include "storyarc.h"
#include "clue.h"
#include "storyinfo.h"
#include "storySend.h"
#include "generator/encounterPrivate.h"
#include "storyarc/storyarcCommon.h"
#include "storyarc/zowieCommon.h"
#include "pnpc.h"
#include "notoriety.h"
#include "taskRandom.h"
#include "dialogdef.h"

// useful defines for story arc stuff
#define MAX_TEAMUP_SIZE                8
#define TASKFORCE_CONTACT_LOCKTIME    60 * 2    // in seconds
#define STORYARC_TOUCH_DISTANCE        16        // number of feet to touch contact, locations, etc.
#define BEGIN_TASKFORCE_SIZE        8        // number of players required to start a task force (default)
#define MIN_TASKFORCE_SIZE            2        // if the number of players in the task force falls below this, the task force is dissolved

#define RANDOM_FAME_SAY_PROB        100        // each time an NPC is clicked on
#define RANDOM_FAME_TIME            60        // allowed to say something every 60 seconds
#define RANDOM_FAME_DIST            150        // any npc in this distance can say it

// global limits on sizes of story arc stuff
// (we're mainly limited by DB code)
#define CONTACTS_PER_PLAYER            500
#define STORYARCS_PER_PLAYER        10    // see MAX_LONG_STORYARCS
#define TASKS_PER_PLAYER            7
#define SOUVENIRCLUES_PER_PLAYER    500

#define CONTACTS_PER_TASKFORCE        1
#define STORYARCS_PER_TASKFORCE        1
#define TASKS_PER_TASKFORCE            1
#define PARAMETERS_PER_TASKFORCE    32    // see MAX_TASKFORCE_PARAMETERS

#define TASKS_PER_CONTACT            256
#define STORYARCS_PER_CONTACT        32
#define CLUES_PER_STORYARC            32
#define KEYCLUES_PER_MISSION        32    // make sure this is reflected in Teamup variable
#define TASKS_PER_EPISODE            32
#define TASK_MAX_VISIT_LOCS            32
#define MAX_OBJECTIVE_INFOS            15    // make sure this is reflected in bits used in StoryObjectiveInfo
#define MAX_TASK_CHILDREN            32    // make sure this is reflected in bits used in StoryObjectiveInfo
#define MAX_UNIQUE_ITEMS            8    // per contact relationship level
#define MAX_UNIQUE_TASKS            128 
#define MAX_LONG_STORYARCS            5    // unless the storyarc is marked as MINI, max # is limited by this

#define MAX_STORE_ITEMS                50    // has to reflect link numbers

// contact stature level conventions:
//        -999 .. -1                Tutorial contacts
//        0                        No Man's land - all contacts here should have NoContactIntroductions
//        1 .. 999                Normal contact tree, currently only 10 stature levels here.  Each stature level is
//                                actually multiplied by ten.  eg. Stature level 2.5 contacts have 25 in data files
//        1000+                    Hollows contact tree
//        2000+                    Shadow Shard contact tree
//
//        Basically any 1000-level block will do LastContactName with each other.  If the player
//        doesn't have a contact in that range, LastContactName will default to the standard 1-999 range.
//        Contacts in no man's land are verified to be NoContactIntroductions.
#define MIN_TUTORIAL_CONTACTLEVEL    -999
#define MAX_TUTORIAL_CONTACTLEVEL    -1
#define MIN_STANDARD_CONTACTLEVEL    1
#define    MAX_STANDARD_CONTACTLEVEL    999
#define NO_MANS_CONTACTLEVEL        0

#define SUBTASK_BITFIELD_SIZE        ((MAX_TASK_CHILDREN+31) / 32)
#define CLUE_BITFIELD_SIZE            ((CLUES_PER_STORYARC+31) / 32)
#define EP_TASK_BITFIELD_SIZE        ((TASKS_PER_EPISODE+31) / 32)
#define TASK_BITFIELD_SIZE            ((TASKS_PER_CONTACT+31) / 32)
#define STORYARC_BITFIELD_SIZE        ((STORYARCS_PER_CONTACT+31) / 32)
#define VISIT_LOCS_BITFIELD_SIZE    ((TASK_MAX_VISIT_LOCS+31) / 32)
#define UNIQUE_ITEM_BITFIELD_SIZE    ((CONTACT_RELATIONSHIP_MAX * MAX_UNIQUE_ITEMS + 31) / 32)
#define UNIQUE_TASK_BITFIELD_SIZE    ((MAX_UNIQUE_TASKS+31) / 32)
#define KEYCLUE_BITFIELD_SIZE        ((KEYCLUES_PER_MISSION+31) / 32)

#define MAX_SA_NAME_LEN                256

// *********************************************************************************
//  Mission Definition
// *********************************************************************************


extern StaticDefineInt ParseLocationTypeEnum[];

extern StaticDefineInt ParseMissionPlaceEnum[];

typedef enum MissionInteractOutcome {
    MIO_NONE,
    MIO_REMOVE
} MissionInteractOutcome;

typedef enum VillainPacingEnum {
    PACING_UNMODIFIABLE,
    PACING_FLAT,
    PACING_SLOWRAMPUP,
    PACING_SLOWRAMPDOWN,
    PACING_STAGGERED,
    PACING_FRONTLOADED,
    PACING_BACKLOADED,
    PACING_HIGHNOTORIETY,
    NUM_PACING_TYPES
} VillainPacingEnum;

///////////////////////////////////////////////////////////////////////////////
// server side version of StoryarcAlliance in uiContactDialog.h
typedef enum StoryarcAlliance {
    SA_ALLIANCE_BOTH,
    SA_ALLIANCE_HERO,
    SA_ALLIANCE_VILLAIN,
} StoryarcAlliance;


// for MissionObjective.flags
#define MISSIONOBJECTIVE_SHOWWAYPOINT            (1)
#define MISSIONOBJECTIVE_REWARDONCOMPLETESET    (1 << 1)

typedef struct MissionObjective {
    char*        name;
    char*        groupName;                // Name of the side mission group this objective belongs to, NULL if not a side mission
    char*        filename;
    const char*    description;
    const char*    singulardescription;
    char**        descRequires;
    char**        activateRequires;        // What the objective needs to happen before it will be clickable
    char**        charRequires;            // Requirements the player must meet before it will be clickable
    const char*    charRequiresFailedText;    // Text to display if the player fails 
    char*        model;            // var
    char*        modelDisplayName;
    int            count;                    // How many duplicate objectives should be placed?
    bool        simultaneousObjective;    // is this part of a simultaneous set? - all objectives with same <name> will be part of the set
    bool        skillCheck;             // Is this a skill check?
    bool        scriptControlled;        // Is this objective not completable by a player clicking on it, but requires a script to complete it?
    int            level;                    // can override the mission level, 1-based
    U32            flags;

    char*        effectInactive;
    char*        effectRequires;
    char*        effectActive;
    char*        effectCooldown;
    char*        effectCompletion;
    char*        effectFailure;

    LocationTypeEnum locationtype;
    MissionPlaceEnum room;
    StoryReward** reward;        // per-objective reward

    int        interactDelay;
    char*    interactBeginString;        // var
    char*    interactInterruptedString;    // var
    char*    interactCompleteString;        // var
    char*    interactActionString;        // var
    char*    interactWaitingString;        // var
    char*    interactResetString;        // var

    char*    forceFieldVillain;            // var
    int        forceFieldRespawn;
    char*    forceFieldObjective;        // var

    char *  locationName;                //Custom location name

    char*    interactDisallowString;        // not var, currently only used by supergroup raids

    MissionInteractOutcome interactOutcome;
} MissionObjective;
extern TokenizerParseInfo ParseMissionObjective[];

typedef struct MissionObjectiveSet {
    char** objectives;    // earray of strings
} MissionObjectiveSet;

typedef struct MissionKeyDoor {
    char*  name;
    char** objectives;    // earray of strings
} MissionKeyDoor;

// for MissionDef.flags
#define MISSION_NOTELEPORTONCOMPLETE            (1)
#define MISSION_NOBASEHOSPITAL                    (1 << 1)
#define MISSION_COEDTEAMSALLOWED                (1 << 2)
#define MISSION_PRAETORIAN_TUTORIAL                (1 << 3)
#define MISSION_COUNIVERSETEAMSALLOWED            (1 << 4)

typedef struct MissionDef {
    const char*                filename;
    const char*                entrytext;            // var
    const char*                exittextfail;        // var
    const char*                exittextsuccess;    // var
    MapSetEnum            mapset;                // not var
    int                    maplength;            // not var
    const char*                mapfile;            // not var
    const char*                villaingroup;        // not var unless of type VG_VAR
    const char*                villaingroupVar;    // var if VG_VAR used
    const char*                cityzone;            // not var
    const char*                doortype;            // not var
    const char*                locationname;        // var, localized
    const char*                foyertype;            // not var
    const char*                doorNPC;            // not var
    const char*                doorNPCDialog;        // not var
    const char*                doorNPCDialogStart;    // not var
    VillainPacingEnum    villainpacing;        // not var
    int                    missiontime;        // not var
    int                    missionLevel;        // not var
    U32                    flags;
    VillainGroupType    villainGroupType;    //not var, if random villain group, determines what type is selected

    // Side Missions
    const char**                sideMissions;        // array of potential side missions
    int                    numSideMissions;    // number of optional side objectives within a mission

    // specialty stuff
    const char*                race;                // if set, this is a race.  Use string to keep statistics
    DayNightCycleEnum    daynightcycle;

    F32                    fFogDist;
    Vec3                fogColor;                    

    int                    showItemThreshold;
    int                    showCritterThreshold;

    const char**        loadScreenPages;

    // active environment stuff
    int                    numrandomnpcs;
    const ScriptDef**            scripts;

    // objectives
    const char*                pchDefeatAllText;  // If set this is the defeat all objective text.
    const SpawnDef**            spawndefs;
    const MissionObjective**    objectives;
    const MissionObjectiveSet** objectivesets;
    const MissionObjectiveSet** failsets;        
    const MissionKeyDoor**    keydoors;
    const StoryClue**            keyclues;
    int CVGIdx;
    SCRIPTVARS_STD_FIELD
} MissionDef;
extern TokenizerParseInfo ParseMissionDef[];

// *********************************************************************************
//  Story rewards - general reward structure that let's you say dialog & give stuff
// *********************************************************************************

typedef struct StoryReward {
    const char*    rewardDialog;
    const char*    SOLdialog;            // Say Out Loud
    const char*    caption;
    const char**    primaryReward;
    const char**    secondaryReward;
    const char*    famestring;            // string added to a random-fame dialog system
    const char*    badgeStat;
    const char*    architectBadgeStat;
    const char*    architectBadgeStatTest;
    const char**    addclues;
    const char**    removeclues;
    const char**    addTokens;
    const char**    addTokensToAll;
    const char**    removeTokens;
    const char**    removeTokensFromAll;
    const char**    popHelp;
    const char**    abandonContacts;    // abandons any tasks you have from the listed contacts
    const char**    rewardSoundName;
    int*            rewardSoundChannel;        // uses sound_common.h's enum
    float*            rewardSoundVolumeLevel;
    int                rewardSoundFadeChannel;    // uses sound_common.h's enum
    float            rewardSoundFadeTime;
    const char*        rewardPlayFXOnPlayer;
    const char**    souvenirClues;
    const char*        floatText;
    int        newContactReward;
    int        contactPoints;
    int        costumeSlot;
    int        respecToken;
    int        bonusTime;
    const RewardSet** rewardSets;

} StoryReward;
extern TokenizerParseInfo ParseStoryReward[];

// *********************************************************************************
//  Task & TaskSet definitions
// *********************************************************************************

typedef struct StoryTaskSet {
    const char*        filename;            // contacts will use this to find a set
    const StoryTask** tasks;                // each task in order
} StoryTaskSet;

// values for StoryTask.flags:
#define TASK_REQUIRED                (1)
#define TASK_LONG                    (1 << 1)
#define TASK_SHORT                    (1 << 2)
#define TASK_REPEATONFAILURE        (1 << 3)
#define TASK_REPEATONSUCCESS        (1 << 4)
#define TASK_URGENT                    (1 << 5)    // urgent tasks will be given out before non-urgent ones
#define TASK_UNIQUE                    (1 << 6)    // unique tasks will not be reissued, even across contact sets
#define TASK_DONTRETURNTOCONTACT    (1 << 7)
#define TASK_HEIST                    (1 << 8)
#define TASK_NOTEAMCOMPLETE            (1 << 9)
#define TASK_SUPERGROUP                (1 << 10)
#define TASK_ENFORCETIMELIMIT        (1 << 11)   // Task will still boot you from a mission even when completed
#define TASK_DELAYEDTIMERSTART        (1 << 12)   // Task timer does not start until entering the mission
#define TASK_NOAUTOCOMPLETE            (1 << 13)   // Task cannot be automatically completed
#define TASK_FLASHBACK                (1 << 14)   // Task shows up in flashback list
#define TASK_FLASHBACK_ONLY            (1 << 15)   // Task is not granted by normal contacts
#define TASK_PLAYERCREATED            (1 << 16)   // Task is not granted by normal contacts
#define TASK_ABANDONABLE            (1 << 17)    // Deprecated
#define TASK_NOT_ABANDONABLE        (1 << 18)    // Task can be abandoned and picked up again later
#define TASK_AUTO_ISSUE                (1 << 19)

extern StaticDefineInt ParseTaskFlag[];

// Cleaner access to these flags
#define TASK_IS_REQUIRED(pDef)                ((pDef) && (pDef)->flags & TASK_REQUIRED)
#define TASK_IS_LONG(pDef)                    ((pDef) && (pDef)->flags & TASK_LONG)
#define TASK_IS_SHORT(pDef)                    ((pDef) && (pDef)->flags & TASK_SHORT)
#define TASK_CAN_REPEATONFAILURE(pDef)        ((pDef) && (pDef)->flags & TASK_REPEATONFAILURE)
#define TASK_CAN_REPEATONSUCCESS(pDef)        ((pDef) && (pDef)->flags & TASK_REPEATONSUCCESS)
#define TASK_IS_URGENT(pDef)                ((pDef) && (pDef)->flags & TASK_URGENT)
#define TASK_IS_UNIQUE(pDef)                ((pDef) && (pDef)->flags & TASK_UNIQUE)
#define TASK_IS_NORETURN(pDef)                ((pDef) && (pDef)->flags & TASK_DONTRETURNTOCONTACT)
#define TASK_IS_HEIST(pDef)                    ((pDef) && (pDef)->flags & TASK_HEIST)
#define TASK_IS_TEAMCOMPLETE(pDef)            ((pDef) && !((pDef)->flags & TASK_NOTEAMCOMPLETE))
#define TASK_IS_SGMISSION(pDef)                ((pDef) && (pDef)->flags & TASK_SUPERGROUP)
#define TASK_FORCE_TIMELIMIT(pDef)            ((pDef) && (pDef)->flags & TASK_ENFORCETIMELIMIT)
#define TASK_DELAY_START(pDef)                ((pDef) && (pDef)->flags & TASK_DELAYEDTIMERSTART)
#define TASK_NO_AUTO_COMPLETE(pDef)            ((pDef) && (pDef)->flags & TASK_NOAUTOCOMPLETE)
#define TASK_CAN_FLASHBACK(pDef)            ((pDef) && (pDef)->flags & TASK_FLASHBACK)
#define TASK_IS_FLASHBACK_ONLY(pDef)        ((pDef) && (pDef)->flags & TASK_FLASHBACK_ONLY)
#define TASK_IS_PLAYER_CREATED(pDef)        ((pDef) && (pDef)->flags & TASK_PLAYERCREATED)
#define TASK_IS_ABANDONABLE(pDef)            ((pDef) && !((pDef)->flags & TASK_NOT_ABANDONABLE))
#define TASK_IS_AUTO_ISSUE(pDef)            ((pDef) && (pDef->flags & TASK_AUTO_ISSUE))

// values for StoryTask.tokenFlags:
#define    TOKEN_FLAG_RESET_AT_START    (1)                // reset token at start of mission

#define TOKENCOUNT_RESET_AT_START(pDef)        ((pDef) && (pDef)->tokenFlags & TOKEN_FLAG_RESET_AT_START)

// for StoryTask.type:
typedef enum TaskType
{
    TASK_NOTYPE,
    TASK_MISSION,
    TASK_RANDOMENCOUNTER,
    TASK_RANDOMNPC,
    TASK_KILLX,
    TASK_DELIVERITEM,
    TASK_VISITLOCATION, // Poorly named, more like "click on stuff".  Similar to zowie task
    TASK_CRAFTINVENTION,
    TASK_COMPOUND,
    TASK_CONTACTINTRO,
    TASK_TOKENCOUNT,
    TASK_ZOWIE,
    TASK_GOTOVOLUME,    // go to places and enter volumes
} TaskType;
extern StaticDefineInt ParseTaskType[];

// For StoryTask
typedef enum NewspaperType
{
    NEWSPAPER_NOTYPE,
    NEWSPAPER_GETITEM,
    NEWSPAPER_HOSTAGE,
    NEWSPAPER_DEFEAT,
    NEWSPAPER_HEIST,
    // IMPORTANT: If you add a new type, update newspaperPickRandomType

    // All new types go above this line
    NEWSPAPER_COUNT,
} NewspaperType;
extern StaticDefineInt ParseNewspaperType[];

typedef enum DeliveryMethod
{
    DELIVERYMETHOD_NOTYPE,
    DELIVERYMETHOD_ANY,
    DELIVERYMETHOD_CELLONLY,
    DELIVERYMETHOD_INPERSONONLY,

    DELIVERYMETHOD_COUNT,
} DeliveryMethod;
extern StaticDefineInt ParseDeliveryMethod[];

typedef enum PvPType
{
    PVP_NOTYPE,
    PVP_PATROL,
    PVP_HERODAMAGE,
    PVP_HERORESIST,
    PVP_VILLAINDAMAGE,
    PVP_VILLAINRESIST,
} PvPType;
extern StaticDefineInt ParsePvPType[];

// tasks are used to define anything you may have to 'do'
// - basically anything that can be replaced by a story arc:
// random encounters as well as contact missions
typedef struct StoryTask 
{
    const char*    filename;                // what filename this task came from
    const char*    logicalname;            // (optional) used for debugging purposes
    TaskType type;
    U32        flags;
    bool    deprecated;                // if set, we should not issue this task
    const StoryClue** clues;
    int        minPlayerLevel;            // level restrictions on giving this task
    int        maxPlayerLevel;

    int        forceTaskLevel;            // if set, this task is always generated at this level
    int        timeoutMins;            // how many minutes the player has to complete the task (0 if not restricted)
    int        villainGroup;            // villain group this task is associated with, 0 if none
    int        villainGroupType;        // villain group type, this task is associated with, 0 if none

    const char    *taskTitle;                // title to display for this task, also used for title if flashback mission
    const char    *taskSubTitle;            // subtitle to display for this task
    const char    *taskIssueTitle;        // issuetitle to display for this task

    const char    *flashbackDescription;    // description of this task.  Used for flashback currently
    const char    **completeRequires;        // requires statement that evaluates true if the character has completed this task
    
    const char    **flashbackRequires;    // expression must evaluate to true for a player to have access to this storyarc in flashback
    const char    *flashbackRequiresFailed;

    const char    **assignRequires;        // requires statement that must evaluate to true in order for this task to be assigned
    const char    *assignRequiresFailed;    // text displayed to player if the assignRequires is not met and there are no other avaible tasks to assign them

    const char    **flashbackTeamRequires;    // if expression is false for anyone on team, storyarc is still in Ouro list, but gives error message
    const char    *flashbackTeamFailed;        // passed up to the constructed story arc only when this task is set for Flashback appending
    
    const char    **teamRequires;            // requires statement that must evaluate to true by all members of the team in order for this task to be set active 
    const char    *teamRequiresFailed;    // text displayed to player if the teamRequires is not met

    const char  **autoIssueRequires;    // requires statement that must evaluate to true by the player for the task to be auto-issued to the player when they get the contact

    // Special Type for Newspapers and PvPTasks
    PvPType                pvpType;
    NewspaperType        newspaperType;

    StoryarcAlliance    alliance;    // which sides can do this task?

    // missions and spawndefs
    const char*    missionfile;
    const char*    missioneditorfile;        // file where the mission def from the mission editor resides
    const MissionDef*    missionref;            // we should have only one ref, one def, or none
    const MissionDef** missiondef;
    const char*    spawnfile;
    const SpawnDef* spawnref;                // we should have only one ref, one def, or none
    const SpawnDef** spawndefs;

    const DialogDef **dialogDefs;

    const char*    detail_intro_text;                // allows {Accept}, var
    const char*    SOLdetail_intro_text;            // allows {Accept}, var
    const char*    headline_text;                    // var
    const char*    yes_text;                        // var
    const char*    no_text;                        // var
    const char*    sendoff_text;                    // var
    const char*    SOLsendoff_text;                // var
    const char*    decline_sendoff_text;            // var
    const char*    SOLdecline_sendoff_text;        // var
    const char*    inprogress_description;            // var, can't use contact params
    const char*    taskComplete_description;        // var
    const char*    taskFailed_description;            // var
    const char*    busy_text;                        // var
    const char*    SOLbusy_text;                    // var
    const char*    task_abandoned;                    // deprecated

    const StoryReward** taskBegin;                // Issued when this subtask starts
    const StoryReward** taskBeginFlashback;        // Issued when this subtask starts if in flashback mode
    const StoryReward** taskBeginNoFlashback;        // Issued when this subtask starts if not in flashback mode
    const StoryReward** taskSuccess;                // Issued when the subtask ends on success
    const StoryReward** taskSuccessFlashback;        // Issued when the subtask ends on success if in flashback mode
    const StoryReward** taskSuccessNoFlashback;    // Issued when the subtask ends on success if not in flashback mode
    const StoryReward** taskFailure;                // Issued when the subtask ends on failure
    const StoryReward** taskAbandon;                // Issued when the subtask is abandoned (to be begun again later)
    const StoryReward** returnSuccess;            // Issued when the player returns to contact on success
    const StoryReward** returnSuccessFlashback;    // Issued when the player returns to contact on success if in flashback mode
    const StoryReward** returnSuccessNoFlashback;    // Issued when the player returns to contact on sucesss if not in flashback mode
    const StoryReward** returnFailure;            // Issued when the player returns to contact on failure

    // The following info shows up on the player's mini-map
    const char*    taskLocationName;
    const char*    taskLocationMap;

    // for TASK_KILLX
    const char*    villainType;            // var
    int        villainCount;
    const char*    villainMap;
    const char*    villainNeighborhood;
    const char*    villainVolumeName;
    const char*    villainDescription;
    const char*    villainSingularDescription;

    const char*    villainType2;            // var
    int        villainCount2;
    const char*    villainMap2;
    const char*    villainNeighborhood2;
    const char*    villainVolumeName2;
    const char*    villainDescription2;
    const char*    villainSingularDescription2;

    // for TASK_DELIVERITEM, TASK_CONTACTINTRO, and all other task types
    const char**            deliveryTargetNames;        // var, not var if TASK_CONTACTINTRO
    const char**            deliveryTargetDialogs;
    const char**            deliveryTargetDialogStarts;
    const DeliveryMethod*    deliveryTargetMethods;

    // for TASK_VISITLOCATION
    const char**    visitLocationNames;        // Which locations should the player visit?

    // for TASK_CRAFTINVENTION
    const char*    craftInventionName;        // Which invention should the player craft?, NULL for any

    // for TASK_TOKENCOUNT
    const char*    tokenName;                // Which token are we tracking?
    int        tokenCount;                // How many are required?
    const char*    tokenProgressString;    // What do we display to the player to show progress?
    int        tokenFlags;                // Additional flags to modify this mission

    // for TASK_ZOWIE
    const char*    zowieType;                // Which zowie are we tracking?
    const char*    zowiePoolSize;            // How many of each should glow?  This is really an array of ints encoded as a string, it should match one for one with the
                                    // entries in zowieType above.
    int        zowieCount;                // How many are required?
    const char*    zowieDisplayName;        // What the player sees on the minimap name
    const char*    zowieDescription;        // What do we display to the player to show progress?
    const char*    zowieSingularDescription; // Like Description, but when there's only one left

    // for TASK_GOTOVOLUME
    const char*    volumeMapName;            // What map is the volume on?
    const char*    volumeName;                // What volume do we need to enter?
    const char*    volumeDescription;        // What do we display to the player in the nav bar?

    int        interactDelay;        
    const char*    interactBeginString;        // var
    const char*    interactInterruptedString;    // var
    const char*    interactCompleteString;        // var
    const char*    interactActionString;        // var

    int  maxPlayers;

    // for TASK_COMPOUND
    const StoryTask** children;            // any subtasks I have

    SCRIPTVARS_STD_FIELD
} StoryTask;
extern TokenizerParseInfo ParseStoryTask[];

// *********************************************************************************
//  Contact definitions
// *********************************************************************************

typedef struct StoryArcRef
{
    char*            filename;    // filename from script file
    StoryTaskHandle    sahandle;        // associated handle
} StoryArcRef;

typedef struct StoryTaskInclude
{
    char*            filename;    // filename of the .taskset file
} StoryTaskInclude;

typedef struct ContactStoreItem {
    PowerNameRef power;
    int price;
    bool unique;
} ContactStoreItem;

typedef struct ContactStoreItems
{
    const ContactStoreItem** items;
} ContactStoreItems;

typedef struct ContactStore {
    ContactStoreItems acquaintanceItems;
    ContactStoreItems friendItems;
    ContactStoreItems confidantItems;
} ContactStore;
extern TokenizerParseInfo ParseContactStore[];

// for ContactDef.flags
typedef enum ContactFlagBitfieldValues
{
    CONTACT_DEPRECATED,
    CONTACT_TASKFORCE,
    CONTACT_TRAINER,
    CONTACT_TUTORIAL,
    CONTACT_TELEPORTONCOMPLETE,
    CONTACT_ISSUEONINTERACT,
    CONTACT_AUTOHIDE,
    CONTACT_NOFRIENDINTRO,
    CONTACT_NOCOMPLETEINTRO,
    CONTACT_DONTINTRODUCEME,
    CONTACT_TAILOR,
    CONTACT_NOHEADSHOT,
    CONTACT_NOABOUTPAGE,
    CONTACT_RESPEC,
    CONTACT_NOTORIETY,
    CONTACT_TEACHER,
    CONTACT_PVPSWITCH,
    CONTACT_BROKER,
    CONTACT_SCRIPT,
    CONTACT_NEWSPAPER,
    CONTACT_PVP,
    CONTACT_NOCELLPHONE,
    CONTACT_REGISTRAR,
    CONTACT_SUPERGROUP,
    CONTACT_METACONTACT,
    CONTACT_ISSUEONZONEENTER,
    CONTACT_HIDEONCOMPLETE,
    CONTACT_FLASHBACK,
    CONTACT_PLAYERMISSIONGIVER,
    CONTACT_STARTWITHCELLPHONE,
    CONTACT_TIP,
    CONTACT_DISMISSABLE,
    CONTACT_NOINTRODUCTIONS,
    CONTACT_AUTODISMISS,
    kContactFlagBitfieldValues_Max,
} ContactFlagBitfieldValues;

extern int g_disableAutoDismiss; // for testing purposes only

#define CONTACT_HAS_FLAG(pDef, flagIndex)    ((pDef)->flags[flagIndex / 32] & (1 << (flagIndex % 32)))

// access these all through defines so we don't have to worry about old-style vs. new-style flags
#define CONTACT_IS_DEPRECATED(pDef)            ((pDef)->deprecated            || CONTACT_HAS_FLAG(pDef, CONTACT_DEPRECATED))
#define CONTACT_IS_TASKFORCE(pDef)            ((pDef)->taskforceContact    || CONTACT_HAS_FLAG(pDef, CONTACT_TASKFORCE))
#define CONTACT_IS_TRAINER(pDef)            ((pDef)->canTrain            || CONTACT_HAS_FLAG(pDef, CONTACT_TRAINER))
#define CONTACT_IS_TUTORIAL(pDef)            ((pDef)->tutorialContact    || CONTACT_HAS_FLAG(pDef, CONTACT_TUTORIAL))
#define CONTACT_IS_TELEPORTONCOMPLETE(pDef)    ((pDef)->teleportOnComplete    || CONTACT_HAS_FLAG(pDef, CONTACT_TELEPORTONCOMPLETE))
#define CONTACT_IS_ISSUEONINTERACT(pDef)    ((pDef)->autoIssue            || CONTACT_HAS_FLAG(pDef, CONTACT_ISSUEONINTERACT))
#define CONTACT_IS_AUTOHIDE(pDef)            ((pDef)->autoHide            || CONTACT_HAS_FLAG(pDef, CONTACT_AUTOHIDE))
#define CONTACT_IS_NOFRIENDINTRO(pDef)        ((pDef)->noFriendIntro        || CONTACT_HAS_FLAG(pDef, CONTACT_NOFRIENDINTRO)\
                                                                        || CONTACT_HAS_FLAG(pDef, CONTACT_NOINTRODUCTIONS)\
                                                                        || CONTACT_HAS_FLAG(pDef, CONTACT_TASKFORCE))
#define CONTACT_IS_NOCOMPLETEINTRO(pDef)    (                               CONTACT_HAS_FLAG(pDef, CONTACT_NOCOMPLETEINTRO)\
                                                                        || CONTACT_HAS_FLAG(pDef, CONTACT_NOINTRODUCTIONS)\
                                                                        || CONTACT_HAS_FLAG(pDef, CONTACT_TASKFORCE))
#define CONTACT_IS_DONTINTRODUCEME(pDef)    (                               CONTACT_HAS_FLAG(pDef, CONTACT_DONTINTRODUCEME)\
                                                                        || CONTACT_HAS_FLAG(pDef, CONTACT_NOINTRODUCTIONS)\
                                                                        || CONTACT_HAS_FLAG(pDef, CONTACT_TASKFORCE))
#define CONTACT_IS_TAILOR(pDef)                ( (pDef)->canTailor            || CONTACT_HAS_FLAG(pDef, CONTACT_TAILOR))
#define CONTACT_HAS_HEADSHOT(pDef)            !(                               CONTACT_HAS_FLAG(pDef, CONTACT_NOHEADSHOT))
#define CONTACT_HAS_ABOUTPAGE(pDef)            !(                               CONTACT_HAS_FLAG(pDef, CONTACT_NOABOUTPAGE))
#define CONTACT_IS_RESPEC(pDef)                (                               CONTACT_HAS_FLAG(pDef, CONTACT_RESPEC))
#define CONTACT_IS_NOTORIETY(pDef)            (                               CONTACT_HAS_FLAG(pDef, CONTACT_NOTORIETY))
#define CONTACT_IS_TEACHER(pDef)            (                               CONTACT_HAS_FLAG(pDef, CONTACT_TEACHER))
#define CONTACT_IS_PVPSWITCH(pDef)            (                               CONTACT_HAS_FLAG(pDef, CONTACT_PVPSWITCH))
#define CONTACT_IS_BROKER(pDef)                (                               CONTACT_HAS_FLAG(pDef, CONTACT_BROKER))
#define CONTACT_IS_SCRIPT(pDef)                (                               CONTACT_HAS_FLAG(pDef, CONTACT_SCRIPT))
#define CONTACT_IS_NEWSPAPER(pDef)            (                               CONTACT_HAS_FLAG(pDef, CONTACT_NEWSPAPER))
#define CONTACT_IS_PVPCONTACT(pDef)            (                               CONTACT_HAS_FLAG(pDef, CONTACT_PVP))
#define CONTACT_IS_NOCELLPHONE(pDef)        (                               CONTACT_HAS_FLAG(pDef, CONTACT_NOCELLPHONE))
#define CONTACT_IS_REGISTRAR(pDef)            (                               CONTACT_HAS_FLAG(pDef, CONTACT_REGISTRAR))
#define CONTACT_IS_SGCONTACT(pDef)            (                               CONTACT_HAS_FLAG(pDef, CONTACT_SUPERGROUP))
#define CONTACT_IS_METACONTACT(pDef)        (                               CONTACT_HAS_FLAG(pDef, CONTACT_METACONTACT))
#define CONTACT_IS_ISSUEONZONEENTER(pDef)    (                               CONTACT_HAS_FLAG(pDef, CONTACT_ISSUEONZONEENTER))
#define CONTACT_IS_HIDEONCOMPLETE(pDef)        (                               CONTACT_HAS_FLAG(pDef, CONTACT_HIDEONCOMPLETE))
#define CONTACT_IS_FLASHBACK(pDef)            (                               CONTACT_HAS_FLAG(pDef, CONTACT_FLASHBACK))
#define CONTACT_IS_PLAYERMISSIONGIVER(pDef)    (                               CONTACT_HAS_FLAG(pDef, CONTACT_PLAYERMISSIONGIVER))
#define CONTACT_IS_STARTWITHCELLPHONE(pDef)    (                               CONTACT_HAS_FLAG(pDef, CONTACT_STARTWITHCELLPHONE))
#define CONTACT_IS_GRSYSTEM_TIP(pDef)        ((pDef)->tipType == TIPTYPE_ALIGNMENT || (pDef)->tipType == TIPTYPE_MORALITY ||\
                                                                           CONTACT_HAS_FLAG(pDef, CONTACT_TIP))
#define CONTACT_IS_TIP(pDef)                ((pDef)->tipType != TIPTYPE_NONE || CONTACT_HAS_FLAG(pDef, CONTACT_TIP))
#define CONTACT_IS_DISMISSABLE(pDef)        (                               CONTACT_HAS_FLAG(pDef, CONTACT_DISMISSABLE))
#define CONTACT_IS_AUTODISMISSED(pDef)        (                               CONTACT_HAS_FLAG(pDef, CONTACT_AUTODISMISS)\
                                                                        && !g_disableAutoDismiss)

#define CONTACT_IS_SPECIAL(pDef) ((pDef)->canTrain \
                                    || CONTACT_HAS_FLAG(pDef, CONTACT_TRAINER)\
                                    || CONTACT_HAS_FLAG(pDef, CONTACT_NOTORIETY)\
                                    || CONTACT_HAS_FLAG(pDef, CONTACT_PVPSWITCH)\
                                    || CONTACT_HAS_FLAG(pDef, CONTACT_TEACHER)\
                                    || CONTACT_HAS_FLAG(pDef, CONTACT_SCRIPT)\
                                    || CONTACT_HAS_FLAG(pDef, CONTACT_REGISTRAR)\
                                    || CONTACT_HAS_FLAG(pDef, CONTACT_FLASHBACK))


#define CONTACT_CAN_USE_CELL(pCon)    ( \
    ( (pCon)->contactRelationship>=CONTACT_CELL_RELATIONSHIP || CONTACT_IS_STARTWITHCELLPHONE((pCon)->contact->def) ) && \
    (!CONTACT_IS_NOCELLPHONE((pCon)->contact->def)) )

typedef enum ContactSocialStatus
{
    STATUS_UNDEFINED,
    STATUS_LOW,
    STATUS_MEDIUM,
    STATUS_HIGH
} ContactSocialStatus;

typedef struct SubContact
{
    char* name;
    char* displaystring;
} SubContact;
extern TokenizerParseInfo ParseSubContact[];

#define CONTACT_DEF_FLAGS_SIZE 2
STATIC_ASSERT(kContactFlagBitfieldValues_Max <= (CONTACT_DEF_FLAGS_SIZE * 32));

/**
* The definition for a contact, instanced as a @ref Contact on in a map.
*/
typedef struct ContactDef
{
    const char* filename;
    const char* displayname;            // var unless where attached to an PNPC, can be var for call only contacts/tips
    const char* profession;
    const char* description;
    Gender gender;
    ContactSocialStatus status;
    ContactAlliance alliance;
    ContactUniverse universe;
    TipType tipType;                    //is this a what type of tip contact is this?  See TipType enumeration for more details.
    const char* tipTypeStr;                //    string that will get translated
    const char* hello;                    // generic greeting
    const char* SOLhello;                // Say Out Loud: generic greeting
    const char* dontknow;                // displayed if contact has not been introduced by the player
    const char* SOLdontknow;            // Say Out Loud: displayed if contact has not been introduced by the player
    const char* firstVisit;                // displayed instead of hello if this is the first time the player has visited this contact
    const char* SOLfirstVisit;            // Say Out Loud: displayed instead of hello if this is the first time the player has visited this contact
    const char* noTasksRemaining;        // displayed instead of hello if contact doesn't have any tasks remaining for player
    const char* SOLnoTasksRemaining;    // Say Out Loud: displayed instead of hello if contact doesn't have any tasks remaining for player
    const char* playerBusy;                // displayed instead of hello if player is full up on tasks
    const char* SOLplayerBusy;            // Say Out Loud: displayed instead of hello if player is full up on tasks
    const char* comeBackLater;            // displayed instead of hello if player isn't right level for tasks
    const char* SOLcomeBackLater;        // Say Out Loud: displayed instead of hello if player isn't right level for tasks
    const char* zeroRelPoints;            // Displayed if you have 0 relationship points with contact (for tutorial contacts)
    const char* SOLzeroRelPoints;        // Say Out Loud: Displayed if you have 0 relationship points with contact (for tutorial contacts)
    const char* neverIssuedTask;        // Displayed if you have never taken a task (for tutorial contacts)
    const char* SOLneverIssuedTask;        // Say Out Loud: Displayed if you have never taken a task (for tutorial contacts)
    const char* relToFriend;            // Displayed once when the contact relationship changes to friend.
    const char* relToConfidant;            // Displayed once when the contact relationship changes to confidant.
    const char* introduction;            // allows {Accept} and {ContactName}, used to intro this contact
    const char* introductionAccept;        // Response text for this contact's introduction.
    const char* introductionSendoff;    // Contact's reply after player chose to be introduced.d
    const char* SOLintroductionSendoff;    // Say Out Loud: Contact's reply after player chose to be introduced.d
    const char* introduceOneContact;    // allows {ContactName}, Displayed when I am going to introduce a single contact
    const char* introduceTwoContacts;    // Displayed when I am going to introduce two contacts (give choice)
    const char* failsafeString;            // Displayed when the character is too high a level to talk to me
    const char* SOLfailsafeString;        // Say Out Loud: Displayed when the character is too high a level to talk to me
    const char* locationName;            // The description of a contact's location
    const char* wrongAlliance;            // Displayed if the player is not of the same alliance as the Contact
    const char* wrongUniverse;            // Displayed if the player is not from the same universe as the Contact
    const char* SOLwrongAlliance;        // Say Out Loud: Displayed if the player is not of the same alliance as the Contact
    const char* SOLwrongUniverse;        // Say Out Loud: Displayed if the player is not from the same universe as the Contact

    //UI overrides
    const char *callTextOverride;            // Text that will display in the "investigate" button of the contact UI
    const char *askAboutTextOverride;        // Text that will display in the "Ask about available contacts" button
    const char *leaveTextOverride;            // Text that will display in the "leave" button of the contact UI
    const char *imageOverride;                // "none" or a model name that will be used for the contact image.
    const char *dismissOptionOverride;        // Text that will display in the "Dismiss this contact" option on the front page of the contact dialog.
    const char *dismissConfirmTextOverride;    // Text that will display in the top half of the dismissal confirmation page of the contact dialog.
    const char *dismissConfirmYesOverride;    // Text that will display in the "Yes, really dismiss this contact" option on the dismissal confirmation page.
    const char *dismissConfirmNoOverride;    // Text that will display in the "No thanks" option in the dismissal confirmation page.
    const char *dismissSuccessOverride;        // Text that will display in the top half of the dismissal success page of the contact dialog.
    const char *dismissUnableOverride;        // Text that will display in the top half of the contact dialog if an error occurs during the dismissal process.
    const char *taskDeclineOverride;        // Text that will display in the "Talk about something else" option in the task introduction page of the contact dialog.
    const char *noCellOverride;                // Text that will display when the player attempts to interact with the contact via cell phone when not allowed.

    F32 minutesExpire;                //the number of minutes that a contact is valid before it auto-revokes

    // Subcontacts for MetaContacts
    const char** subContacts;
    const char** subContactsDisplay;

    // Only exists if a script has given a map token to flag this contact as usable
    const char* mapTokenRequired;            // Players cannot have this contact unless the maps have this token

    // Requires Statement Requirements
    const char** interactionRequires;                // You must meet the requires statement before the contact will talk to you
    int interactionRequiresTipIndex;        // If not zero, then this is a tip and it shares the same interactionRequires statement with all other contact defs that have the same index
    const char* interactionRequiresFailString;    // Displayed to let player know this contact doesn't meet the requires statement


    // Badge Requirements
    const char* requiredBadge;                // Badge required before the contact will talk to you
    const char* badgeNeededString;            // Displayed to let player know this contact requires a badge
    const char* SOLbadgeNeededString;        // Say Out Loud: Displayed to let player know this contact requires a badge
    const char* badgeFirstTick;                // 0-25% complete with badge, use this text
    const char* badgeSecondTick;            // 25-50% complete with badge, use this text
    const char* badgeThirdTick;            // 50-75% complete with badge, use this text
    const char* badgeFourthTick;                // 75-100% complete with badge, use this text

    const char* dontHaveToken;                // (task force) player doesn't have required token to start TF
    const char* SOLdontHaveToken;            // (task force) Say Out Loud: player doesn't have required token to start TF
    const char* levelTooLow;                // (task force) player level is too low
    const char* levelTooHigh;                // (task force) player level is too high
    const char* needLargeTeam;                // (task force) player needs to be on team of >= 5
    const char* needTeamLeader;                // (task force) player needs to be team leader 
    const char* needTeamOnMap;                // (task force) player's team needs to be on this map
    const char* badTeamLevel;                // (task force) player's team doesn't meet the level requirements
    const char* taskForceInvite;            // (task force) prompt the player to create a task force
    const char** taskForceRewardRequires;    // (task force) requires expression for final reward of this task force
    const char* taskForceRewardRequiresText;// (task force) message given to players when there are players who fail the requires expression for final reward of this task force
    const char* taskForceName;                // (task force, var) the name given to the task force created
    int   taskForceLevelAdjust;                // (task force) level to adjust the players task force by (positive or negative)
    const char* requiredToken;                // (task force) this token is required to start TF

    const StoryTask** mytasks;            // tasks actually loaded with this contact
    const StoryTaskInclude** taskIncludes; // references to task sets - on load, references copied to tasks array
    const StoryTask** tasks;            // references only -> mytasks + taskIncludes
    const StoryArcRef** storyarcrefs;    // story arc files that belong to this contact
    const DialogDef **dialogDefs;

    int stature;            // Which level is this contact on in the contact tree?
    const char* statureSet;        // Never introduce players to another contact of the same stature + set.
    const char* nextStatureSet;    // Always introduce players to another contact of higher stature in this set.
    const char* nextStatureSet2;    // If present, contact will attempt to give players choice between nextStatureSet and nextStatureSet2
    ContactHandle brokerFriend; // Used for brokers, stores the handle of the other broker in the zone
    int contactsAtOnce;        // How many contacts will this contact attempt to introduce at once?
    int friendCP;            // When does a player become a friend of this contact?
    int confidantCP;        // When does a player become a confidant of this contact?
    int completeCP;            // When should a player be introduce to a contact of a higher stature?
    int heistCP;            // When should I start offering heists to a player?
    int completePlayerLevel;
    int failsafePlayerLevel;// When should the contact refuse to give a player any more tasks?
    int minPlayerLevel;        // What character level range this contact is designed for
    int maxPlayerLevel;        //        (limits villain levels on missions)
    int minTaskForceSize;    // changes BEGIN_TASKFORCE_SIZE (mainly for debugging)
    ContactStore store;        // What kind of specializations does this contact sell?
    MultiStore stores;        // What kind of specializations does this contact sell?

    int accessibleContactValue;

    const char* old_text;            // DEPRECATED

    // flags - all deprecated, should be using new <flags> field
    bool deprecated;            // if non-zero, this contact will not be issued
    bool taskforceContact;    // If non-zero, this contact is a taskforce contact
    bool noFriendIntro;        // if non-zero, this contact will not attempt to introduce a player to a friend
    bool tutorialContact;    // if non-zero, this contact is allowed looser restrictions on language to accomodate starter contacts
    bool teleportOnComplete; // if non-zero, this contact will do a new player teleport if player has completed contact and closes the dialog
    bool autoIssue;            // if non-zero, this contact will be issued as soon as the player clicks on them
    bool autoHide;            // if non-zero, this contact doesn't show in the contact list unless the player has a task for them
    bool canTrain;            // If non-zero, the contact can train heroes up in level
    bool canTailor;

    const StoryReward** dismissReward;

    // all the flags are in here now
    U32 flags[CONTACT_DEF_FLAGS_SIZE];                // new flags field

    const VillainGroupEnum * knownVillainGroups;    // for newspaper tasks, IDs of all known villain groups

    SCRIPTVARS_STD_FIELD
} ContactDef;
extern ContactDef** contactDefs;

/**
* An instance of a @ref ContactDef with map specific data. 
*  
* @note There is a one to one mapping between this and @ref 
*       ContactDef.  When a given contact does not exist on this
*       map, the parent pointers will be NULL, but the @ref def
*       pointer will be valid.
*/
typedef struct Contact
{
    const ContactDef *def;
    const PNPCDef *pnpcParent;
    Entity *entParent;                // the pointer to the Entity that is this contact, null if not on this mapserver
} Contact;
extern Contact** contacts;

// *********************************************************************************
//  Story Arc definitions
// *********************************************************************************

typedef struct StoryArcList {
    StoryArc** storyarcs;
} StoryArcList;

// for StoryArc.flags
#define STORYARC_DEPRECATED            (1)
#define STORYARC_MINI                (1 << 1)
#define STORYARC_REPEATABLE            (1 << 2)
#define STORYARC_FLASHBACK            (1 << 3)
#define STORYARC_NOFLASHBACK        (1 << 4)        // This story arc cannot be used as a flashback
#define STORYARC_SCALABLE_TF        (1 << 5)        // AV and boss reduction even on a taskforce
#define STORYARC_FLASHBACK_ONLY        (1 << 6)        // This story arc is only available in flashback

#define STORYARC_IS_DEPRECATED(pDef)    ((pDef)->deprecated    || (pDef)->flags & STORYARC_DEPRECATED)

typedef struct StoryEpisode 
{
    StoryTask**    tasks;
    StoryReward** success;
} StoryEpisode;

typedef struct StoryArc 
{
    char*                filename;
    bool                deprecated;
    U32                    uID;
    StoryClue**            clues;
    StoryEpisode**        episodes;
    U32                    flags;
    int                    minPlayerLevel;                // level restrictions on giving this storyarc
    int                    maxPlayerLevel;
    F32                    flashbackCostMulitplier;    // deprecated
    StoryarcAlliance    alliance;                    // which sides can do this storyarc?  
    char*                name;
    char*                flashbackDescription;
    char*                architectAboutContact;

    char**                assignRequires;                // expression must evaluate to true for a player to have access to this storyarc
    char*                assignRequiresFailed;        // displayed if player could get this story arc but does not meet requires

    char**                completeRequires;            // expression the evaluates to true when this SA has been completed

    char**                flashbackRequires;            // expression must evaluate to true for a player to have access to this storyarc in flashback
    char*                flashbackRequiresFailed;

    char**                flashbackTeamRequires;        // if expression is false for anyone on team, storyarc is still in Ouro list, but gives error message
    char*                flashbackTeamFailed;

    char**                teamRequires;                // expression must evaluate to true for a player to be on a team with any task in this storyarc active
    char*                teamRequiresFailed;            // text displayed to player if the teamRequires is not met

    StoryReward**        rewardFlashbackLeft;
    SCRIPTVARS_STD_FIELD
} StoryArc;

extern TokenizerParseInfo ParseStoryClue[];

// *********************************************************************************
//  Task info - active state info for tasks
// *********************************************************************************

// additional info for each objective that has been accomplished for a task
typedef struct StoryObjectiveInfo
{
    unsigned int    set : 1;                                // am I using this entry?
    unsigned int    compoundPos : 6;                        // compound number of task that this objective was in (limited by MAX_TASK_CHILDREN)
    unsigned int    num : 7;                                // 0-based number of what item it was (limited by MAX_OBJECTIVE_INFOS)
    unsigned int    success : 1;                            // was the objective succeeded or failed?

    // types of info
    unsigned int    missionobjective : 1;                    // this represents a mission objective
    unsigned int    spawndef : 1;                            // this represents a spawndef for the task

    unsigned int    unused : 15;                            // must be a 32-bit structure
} StoryObjectiveInfo;

// for StoryTaskInfo.state:
typedef enum TaskState
{
    TASK_NONE,        
    TASK_ASSIGNED,    
    TASK_MARKED_SUCCESS,
    TASK_MARKED_FAILURE,
    TASK_SUCCEEDED,    
    TASK_FAILED,
} TaskState;
#define TASK_INPROGRESS(state) ((state == TASK_ASSIGNED)? 1: 0)
#define TASK_MARKED(state) ((state >= TASK_MARKED_SUCCESS && state <= TASK_MARKED_FAILURE)? 1: 0)
#define TASK_COMPLETE(state) ((state >= TASK_SUCCEEDED)? 1: 0)
extern char* g_taskStateNames[];

typedef struct StoryClueInfo
{
    StoryTaskHandle        sahandle;
    int                    clueIndex;
    char*                varName;
    char*                varValue;
    ScriptVarsScope*    varScope;
} StoryClueInfo;

typedef struct StoryTaskInfo
{
    // volatile/derived fields
    const StoryTask* def;                                    // definition of task, or child if task is compound
    const StoryTask* compoundParent;                        // if this is a compound task, the def of the parent, otherwise NULL
    int                doorId;                                    // id of door attached to mission

    // db tracked fields
    StoryTaskHandle    sahandle;
    int                playerCreatedID; // temporary holder to allow tasks to get in and out of DB

    unsigned int    seed;                                    // seed to generate all aspects of task
    TaskState        state;                                    // state of task
    U32                timeout;        // if non-zero, time when mission timer expires
    U32                timezero;        // if non-zero, time when mission timer equals zero
    S32                timerType;        // 1 counts up, -1 counts down
    U32                failOnTimeout;    // if true, mission fails when mission timer expires

    U32                haveClues[CLUE_BITFIELD_SIZE];            // bitfield of clues I have
    int                clueNeedsIntro;                            // index+1 of clue that needs an intro dialog (or zero)
    int                spawnGiven;                                // has the task replaced a random encounter?
    int                level;                                    // what level was this task spawned at?
    int                skillLevel;                                // what skill level was this task spawned at?
    int                old_notoriety;                                // deprecated

    StoryDifficulty difficulty;                            // what are my current difficulty settings

    U32                completeSideObjectives;                    // Tracks completed side objectives we don't want to regive
    int                assignedDbId;                            // the player the task was assigned to
    U32                assignedTime;                            // when the task was assigned
    int                doorMapId;                                // map that door is on
    Vec3            doorPos;                                // actual position of door
    int                missionMapId;                            // map id that this task is attached to
    StoryObjectiveInfo    completeObjectives[MAX_OBJECTIVE_INFOS];    // mission objective info records (persists across compound children)
    U32                subtaskSuccess[SUBTASK_BITFIELD_SIZE];    // bitfield of whether each subtask succeeded
    VillainGroupEnum    villainGroup;                        // villain group (if mission specified random group)
    MapSetEnum            mapSet;                                // map set (if mission specified random set)

    // Team Completion Data stored to the DB
    int                teamCompleted;                            // This task was teamcompleted and hasn't received a prompt answer(save the level)

    // task progress tracking
    int                curKillCount;                            // TASK_KILLX only, how many required
    int                curKillCount2;                            // TASK_KILLX only, how many required
    int                nextLocation;                            // TASK_VISITLOCATION only, index of location to be touched

    // not saved to db
    int                zowieIndices[MAX_GLOWING_ZOWIE_COUNT];

    // MAK -
    //  the following strings are being deprecated.  we want to move away from
    //  restrictions on how a task can change after it's been published.  
    //  consequently, each of these fields will just be looked up dynamically as required.
    //
    //  these will be removed from the database after Update 3 is stable
    char            villainType[MAX_SA_NAME_LEN];            // villain group or villain name to kill
    char            villainType2[MAX_SA_NAME_LEN];            // villain group or villain name to kill
    char            deliveryTargetName[MAX_SA_NAME_LEN];    

    char deprecatedStr[MAX_SA_NAME_LEN]; // used for deprecated database columns
    
} StoryTaskInfo;

// *********************************************************************************
//  Contact info - active state info for contacts
// *********************************************************************************

#define    CI_NONE                0
#define CI_SAME_LEVEL        1
#define CI_NEXT_LEVEL        (1 << 1)
typedef int ContactsIntroduced;

// contact relationship levels
typedef enum ContactRelationship
{
    CONTACT_NO_RELATIONSHIP,
    CONTACT_ACQUAINTANCE,
    CONTACT_FRIEND,
    CONTACT_CONFIDANT,
    CONTACT_RELATIONSHIP_MAX,
} ContactRelationship;
#define CONTACT_CELL_RELATIONSHIP    CONTACT_FRIEND

typedef struct StoryContactInfo
{
    // volatile fields/derived fields
    Contact*        contact;
    unsigned int    dirty            : 1;                    // Do I need to send the player another update about myself?
    unsigned int    deleteMe        : 1;                    // This info has been marked for deletion, delete after notifying client
    unsigned int    interactionTemp : 1;                    // created only for one dialog interaction
    
    // db tracked fields
    //int                lock_dbid;                                // the dbid of the person who has locked this contact (taskforce only)
    //U32                lock_time;                                // when the person locked the contact (taskforce only)
    ContactHandle    handle;                                    // persistent handle
    ContactHandle    brokerHandle;                            // Which broker the newspaper is currently attached to
    U32                taskIssued[TASK_BITFIELD_SIZE];            // the tasks I have been issued
    U32                storyArcIssued[STORYARC_BITFIELD_SIZE];    // the story arcs I have been issued
    U32                itemsBought[UNIQUE_ITEM_BITFIELD_SIZE];    // the unique items I have bought
    unsigned int    dialogSeed;                                // how I will generate tasks
    unsigned int    contactIntroSeed;                        // How I will introduce contacts to the player.
    int                contactPoints;                            // How many contact points I currently have
    ContactRelationship contactRelationship;                // What is my last known relationship with this contact?
    ContactsIntroduced contactsIntroduced;                    // How many contacts this contact has introduced?
    unsigned int    seenPlayer;                                // Have I said FirstVisit string yet?
    unsigned int    notifyPlayer;                            // Do I want to talk to the player?
    unsigned int    rewardContact;                            // Flagged if contact should introduce a new contact
    U32                revokeTime;                                // time left before the contact will be revoked.
} StoryContactInfo;

// *********************************************************************************
//  Story Arc info - active state info for storyarcs
// *********************************************************************************

typedef struct StoryArcInfo
{
    // volatile fields/derived fields
    const StoryArc* def;
    const ContactDef* contactDef;

    // db tracked fields
    StoryTaskHandle    sahandle;                                // persistent handle
    ContactHandle    contact;                                // the contact associated with this story arc
    int                episode;                                // index of episode I am on
    unsigned int    seed;                                    // random seed for story arc vars
    int                clueNeedsIntro;                            // index+1 of clue that needs an intro dialog (or zero)
    int                playerCreatedID;                        // This is the same ID as the sahandle context, it just needs to be in another
                                                            // location to save and read from the data base.  

    U32                haveClues[CLUE_BITFIELD_SIZE];            // bitfield of clues I have
    U32                taskComplete[EP_TASK_BITFIELD_SIZE];    // what tasks I have completed for this episode
    U32                taskIssued[EP_TASK_BITFIELD_SIZE];        // what tasks have been issued for this episode
} StoryArcInfo;

// *********************************************************************************
//  Story Info - hangs off character or task force - repository for other infos
// *********************************************************************************

#define FORCED_MISSION_NAME_LENGTH 128

typedef struct StoryInfo
{
    StoryContactInfo**    contactInfos;                // Max size = CONTACTS_PER_PLAYER
    StoryArcInfo**        storyArcs;                    // Max size = STORYARCS_PER_PLAYER
//    StoryClueInfo**        storyArcClueInfo;            // Max size = STORYARCS_PER_PLAYER * CLUES_PER_STORYARC
    StoryTaskInfo**        tasks;                        // Max size = TASKS_PER_PLAYER
    const SouvenirClue**    souvenirClues;                //
    U32                    uniqueTaskIssued[UNIQUE_TASK_BITFIELD_SIZE];    // unique tasks this player has been issued

    StoryDifficulty        difficulty;

    ContactHandle        interactTarget;                // Player currently interacting with this contact.
    Vec3                interactPos;                // where the contact is
    EntityRef            interactEntityRef;            // EntityRef for the contact
    unsigned int        interactDialogTree : 1;        // if player is interacting via dialog tree
    unsigned int        interactCell : 1;            // if player is talking on cell phone
    unsigned int        interactPosLimited : 1;        // if contact dialog will be limited by distance to interactPos
    unsigned int        taskforce : 1;                // is this taskforce info?
    unsigned int        contactInfoChanged : 1;        // The player should be sent some contact status updates
    unsigned int        visitLocationChanged : 1;    // The player should be sent visit location update.
    unsigned int        clueChanged : 1;            // The player should be sent the new list of clues.
    unsigned int        keyclueChanged : 1;            // The player should be sent a new list of key clues.
    unsigned int        destroyTaskForce : 1;        // the task force should be destroyed once we are complete
    unsigned int        contactFullUpdate : 1;        // need to refresh entire contact list (task indexes changed)

    // Now tracked to the player, to update the rest of the team
    int                    taskStatusChanged;            // the player's team should be updated with task info - MAK these flags need to be cleaned up

    // the following is volatile and may be changed outside of lock/unlock (therefore can't be done on a TF):
    unsigned int        souvenirClueChanged : 1;    // The player should be sent the new list of souvenir clues.

    // Fields to allow derivation of the correct mission exit string
    StoryTaskHandle        exithandle;
    int                    exitMissionOwnerId;
    int                    exitMissionSuccess; // 0 = failed, 1 = success, 2 = alreadyshown

    // Moving the contact/task selection concept out of StoryContactInfo/StoryTaskInfo and into here
    // selectedContact and selectedTask can still collide with each other,
    // but at least multiple contacts or multiple tasks won't collide anymore
    StoryContactInfo    *selectedContact;
    StoryTaskInfo        *selectedTask;

    // This contact selection is delayed until the next time the Entity ticks,
    // which might be on a different map than when it was set, depending on if the Entity
    // immediately mapmoved right after setting this or not.
    ContactHandle        delayedSelectedContact;

    // link to flashback storyarc when player is initiating a flashback
    int        flashbackArc;    

    // Maintains a lists of the most recent items and a list of most
    // recent people that were part of a newspaper task that
    // a player has completed.  Works like a queue, NTHindex holds the
    // index of the oldest item. DO NOT CHANGE NEWSPAPER_HIST_MAX/TASK_NUM or you
    // might break the database.  If you change it, make sure to manually
    // take care of how this is stored in the database
#define NEWSPAPER_HIST_MAX            (20)
#define NEWSPAPER_TASK_NUM            (2)
    const char* newspaperTaskHistory[NEWSPAPER_TASK_NUM][NEWSPAPER_HIST_MAX];
    int newspaperTaskHistoryIndex[NEWSPAPER_TASK_NUM];

    char savedForcedMissionName[FORCED_MISSION_NAME_LENGTH];
} StoryInfo;

// *********************************************************************************
//  Mission Info - only one on a mission map server
// *********************************************************************************

typedef struct MissionInfo {
    const StoryTask* task;
    const MissionDef* def;
    int                ownerDbid;
    int                supergroupDbid;
    int                taskforceId;

    ContactHandle    contact;
    StoryTaskHandle  sahandle;

    unsigned int    seed;
    ScriptVarsTable vars;
    U32                timeout;        // if non-zero, time when mission timer expires
    U32                timezero;        // if non-zero, time when mission timer equals zero
    S32                timerType;        // 1 counts up, -1 counts down
    U32                failOnTimeout;    // if true, mission fails when mission timer expires
    U32                starttime;        // when first player logged on
    int                baseLevel;        // base task level, excluding notoriety and pacing
    int                missionLevel;    // rolled once, based on task level & notoriety
    int                missionPacing;    // rolled once, based on task pacing & notoriety
    int                skillLevel;        // rolled once, based on task skill level

    StoryDifficulty difficulty;

    VillainGroupEnum missionGroup;    // what villain group will be used for mission
    U32                completeSideObjectives; // side objectives to disable as they have been completed

    // objectives reached by players
    StoryObjectiveInfo    completeObjectives[MAX_OBJECTIVE_INFOS];    // mission objective info records (persists across compound children)

    EncounterAlliance missionAllyGroup;    // All Mission Encounters are designed for this alliance group

    char *currentServerMusicName;
    float currentServerMusicVolume;

    // state of mission
    unsigned int    initialized :1;    // the map has been initialized
    unsigned int    completed :1;    // the players have completed the mission and may now warp out
    unsigned int    success :1;        // if completed is true, mission is succeeded if true, failed if false
    unsigned int    flashback :1;    // if true, mission is flashback mission.  Used for boss reduction/AV reduction
    unsigned int    scalableTf :1;    // if true, mission is part of a taskforce but AV/boss reduction should be performed
} MissionInfo;

// *********************************************************************************
//  Misc structs
// *********************************************************************************
typedef struct StoryLocation
{
    Vec3 coord;
    int mapID;
    ContactDestType type;
    char locationName[MAX_SA_NAME_LEN];
} StoryLocation;

typedef struct ContactTaskPickInfo
{
    const StoryTask*    taskDef;
    StoryTaskHandle        sahandle;
    int                    minLevel;
    VillainGroupEnum    villainGroup;
    const char            *requiresFailed;
} ContactTaskPickInfo;

// *********************************************************************************
//  general story handle functions
// *********************************************************************************

static INLINEDBG int StoryHandleToIndex(const StoryTaskHandle * handle) { return handle->bPlayerCreated?handle->subhandle:handle->subhandle - 1; }
static INLINEDBG int StoryIndexToHandle(int index) { return index + 1; }
static INLINEDBG int StoryHandleIsValid(int handle) { return (handle ? 1 : 0); }
static INLINEDBG int ContactHandleIsValid(int handle) { return (handle > 0 ? 1 : 0); }

static INLINEDBG int isStoryarcHandle(const StoryTaskHandle * sahandle) { return (sahandle->bPlayerCreated || sahandle->context < 0); }
static INLINEDBG int isSameStoryTask(const StoryTaskHandle * a, const StoryTaskHandle * b ) { return(a->context==b->context && a->subhandle==b->subhandle); }
static INLINEDBG int isSameStoryTaskPos(const StoryTaskHandle * a, const StoryTaskHandle * b ) { return(a->context==b->context && a->subhandle==b->subhandle && a->compoundPos==b->compoundPos); }

#define SAHANDLE_ARGS(x) x.context, x.subhandle
#define SAHANDLE_ARGSPOS(x) x.context, x.subhandle, x.compoundPos, x.bPlayerCreated
#define SAHANDLE_SCANARGS(x) &x.context, &x.subhandle
#define SAHANDLE_SCANARGSPOS(x) &x.context, &x.subhandle, &x.compoundPos, &x.bPlayerCreated

void StoryArcFlashbackMission(StoryTask* task);

#endif // __STORYARCPRIVATE_H
