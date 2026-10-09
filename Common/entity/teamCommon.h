/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef TEAMCOMMON_H
#define TEAMCOMMON_H

#include <utilitieslib/stdtypes.h>

#if SERVER
#    include "storyarc/storyarcinterface.h"
#endif

typedef struct StoryInfo StoryInfo;

typedef char MemberName[64];

#define MAX_TEAM_MEMBERS            8
#define MAX_TASKFORCE_MEMBERS        MAX_TEAM_MEMBERS
#define TF_NAME_LEN                    64
#define MAX_TASKFORCE_PARAMETERS    32
#define MAX_RAID_MEMBERS            MAX_TEAM_MEMBERS
#define MAX_LEVELINGPACT_MEMBERS    2
#define LEVELINGPACT_MAXLEVEL        5
#define LEVELINGPACT_VERSION        1 //update this every time you need old pacts due to a change in the structure.
#define MAX_LEAGUE_TEAMS            6
#define MAX_LEAGUE_MEMBERS            (MAX_LEAGUE_TEAMS * MAX_TEAM_MEMBERS)


typedef struct TeamMembers
{
    int                leader;
    int                count;
    int                rotors[3];        // used for round-robin rewarding.
    int                *onMapserver;    // client side variable
    int                *mapIds;        // I think this is only used for LevelingPacts ARM
    int                *ids;
    MemberName        *names;
    int                *oldmapIds;        // I think this is only used for LevelingPacts ARM
} TeamMembers;

typedef enum TaskForceParameter
{
    TFPARAM_INVALID = 0,
    TFPARAM_DEATHS = 1,
    TFPARAM_TIME_LIMIT = 2,
    TFPARAM_TIME_COUNTER = 3,
    TFPARAM_DEBUFF_PLAYERS = 4,
    TFPARAM_DISABLE_POWERS = 5,
    TFPARAM_DISABLE_ENHANCEMENTS = 6,
    TFPARAM_BUFF_ENEMIES = 7,
    TFPARAM_NO_INSPIRATIONS = 8,

    // This should be the first thing after the actual parameters. It is used
    // to figure out how many real parameters there are.
    TFPARAM_NUM,

    // Do not add any parameters below this value. They are for internal
    // handling of existing parameters (ie. initial and current state).
    TFPARAM_MAX_USABLE_PARAMETER = 20,

    // These are set at the beginning of the taskforce. They hold the actual
    // number of deaths allowed, the type of deaths challenge selected (eg.
    // 5 total, 3 per team member) and the length of the time limit rather
    // than the server expiry time.
    TFPARAM_START_TIME = 24,
    TFPARAM_DEATHS_MAX_VALUE = 25,
    TFPARAM_DEATHS_SETUP = 26,
    TFPARAM_TIME_LIMIT_SETUP = 27,

    // These bitfields store which challenges have been passed and failed.
    // The success one gets filled out when the last mission is completed
    // and the failure one gets filled out as the taskforce goes.
    TFPARAM_SUCCESS_BITS = 31,
    TFPARAM_FAILURE_BITS = 32
} TaskForceParameter;

typedef enum TFParamDeaths
{
    // Set this if the challenge is disabled.
    TFDEATHS_INFINITE,

    // Set these for the total number of allowed deaths BEFORE the challenge
    // fails.
    TFDEATHS_NONE,
    TFDEATHS_ONE,
    TFDEATHS_THREE,
    TFDEATHS_FIVE,

    // Set these for the number of allowed deaths overall before failure.
    // So on an 8 man team it would be 8, 24 or 40 total deaths to fail the
    // challenge respectively.
    TFDEATHS_ONE_PER_MEMBER,
    TFDEATHS_THREE_PER_MEMBER,
    TFDEATHS_FIVE_PER_MEMBER
} TFParamDeaths;

typedef enum TFParamTimeLimits
{
    TFTIMELIMITS_NONE,
    TFTIMELIMITS_BRONZE,
    TFTIMELIMITS_SILVER,
    TFTIMELIMITS_GOLD
} TFParamTimeLimits;

typedef enum TFParamPowers
{
    // Set this if the challenge is disabled.
    TFPOWERS_ALL,

    // No temporary/veteran powers.
    TFPOWERS_NO_TEMP,

    // No travel powers, whether from pool powers, temporaries, etc.
    TFPOWERS_NO_TRAVEL,

    // Disables pool, epic, patron and temporary powers. Accolades like the
    // Crey Pistol are unaffected, however.
    TFPOWERS_ONLY_AT,

    // Disables ancillary and patron pool powers. Other pools and temporary
    // powers etc are allowed.
    TFPOWERS_NO_EPIC_POOL
} TFParamPowers;

typedef enum TFArchitectFlags 
{
    ARCHITECT_NONE = 0,
    ARCHITECT_NORMAL = (1<<0),
    ARCHITECT_TEST = (1<<1),
    ARCHITECT_DEVCHOICE = (1<<2),
    ARCHITECT_HALLOFFAME = (1<<3),
    ARCHITECT_FEATURED = (1<<4),
    ARCHITECT_DEVCHOICE_NORMALREWARD = (1<<5),
}TFArchitectFlags;

typedef struct Costume Costume;
typedef struct CustomNPCCostume CustomNPCCostume;
typedef struct RewardToken RewardToken;

typedef struct TaskForce
{
    TeamMembers members;
    char        name[TF_NAME_LEN];
    int            level;                                    // 1 based
    int            levelAdjust;
    int            deleteMe;
    U32            parametersSet;
    U32            parameters[MAX_TASKFORCE_PARAMETERS];
    
    int            architect_id;
    int            architect_flags;
    int            architect_authid; // only set if the auth region matches

    char        architect_contact_name[32];
    int            architect_contact_override;
    int            architect_useCachedCostume;
    Costume        *architect_contact_costume;
    CustomNPCCostume *architect_contact_npc_costume;
    
#ifdef SERVER
    int            exemplar_level;                            // 0 based (player level - 1)
    int            minTeamSize;                            // 
    StoryInfo*    storyInfo;
    char        *playerCreatedArc;                        // estring
#endif
} TaskForce;

typedef enum {
    MAPINSTANCE_MISSION = 1,
    MAPINSTANCE_ARENA = 2,
} MapInstanceType;

#ifdef SERVER
typedef struct TeamupTaskSelect {
    int                    *memberValid;
    StoryTaskHandle        activeTask;
    int team_mentor_dbid;
} TeamupTaskSelect;
#endif

#define ACTIVE_PLAYER_PROBATION_PERIOD_SECONDS 10

typedef struct Teamup
{
    TeamMembers    members;

    // for sidekicking
    int                    team_level;
    int                    team_mentor;

    // for active player reward tokens
    // very similar to team_mentor, but not identical
    // The client's e->teamup->activePlayerDbid actually comes from the server's e->teamup_activePlayer->activePlayerDbid.
    int                    activePlayerDbid;

#ifdef SERVER
    int                    map_id;
    int                    map_playersonmap;
    MapInstanceType        map_type;

    StoryTaskInfo*        activetask;

    // the Active Player tokens from the player that is the current Active Player of the team
    // We need to copy the tokens to the teamup because teammates on other mapservers won't
    // be able to access that teammate's tokens directly via his Entity
    RewardToken**        activePlayerRewardTokens;

    // this flag is compared against the same flag on the Entity to see if I need to 
    // refresh the status of each entity because the active player information on the team changed
    int                    activePlayerRevision;

    // for 10 seconds after a task owner change, the new owner is put in probation
    int                    probationalActivePlayerDbid;    

    // probation ends at this timestamp
    U32                    probationalActivePlayerDbidExpiration;

    char                mission_status[256];
    ContactHandle        mission_contact;
    U32                    keyclues;
    int                    kheldianCount;        // NOTE: mapserver disconnects can cause this to be inaccurate
    U32                    lastambushtime;
    TeamupTaskSelect    taskSelect;
    int                    teamSwapLock;

    int                    maximumPlayerCount;

#endif
} Teamup;

typedef struct LevelingPact
{
#if STATSERVER
    int            dbid;
    int            *memberids;
    int            requestSent;
#endif
    int version;
    TeamMembers    members;
    int            count; // this is necessary if, for instance, a member is deleted while the statserver isn't connected
    U32            experience;
    U32            timeLogged; //this is how much time the members have been logged in looking for xp.
    U32            influence[MAX_LEVELINGPACT_MEMBERS];
    U32            deprecatedInfamy[MAX_LEVELINGPACT_MEMBERS];    // can't remove from the database, but no longer used
} LevelingPact;

typedef struct League
{
#if STATSERVER
    int            container_id;
    int            instance_id;
    int            requestSent;
    int            *teamLeaderList;
    int            *teamLockList;
    int turnstileTeamMapping[MAX_LEAGUE_TEAMS];                //    maps the desired team to the actual leader
#endif
    TeamMembers    members;                    //    used on server
    //    this represents a team structure for the league
    //    the members array is the authority if you are in the league
    //    if for some reason, these 2 do not match, the members array is correct
    int teamLeaderIDs[MAX_LEAGUE_MEMBERS];                                        //    list of teamup ids for each member in the league 1:1 mapping
    int lockStatus[MAX_LEAGUE_MEMBERS];                        //    lock status of every person in league
    int revision;
#if CLIENT
    int **teamRoster;                                        //    teamlist organization of members. atm, only used client side
    int *focusGroupRosterIndex;
#endif
} League;

#endif //TEAMCOMMON_H
