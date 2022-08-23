/*
 *
 *    TurnstileServerCommon.h - Copyright 2010 NC Soft
 *        All Rights Reserved
 *        Confidential property of NC Soft
 *
 *    Turnstile system for matching players for large raid content
 *
 */

#ifndef TURNSTILE_SERVER_COMMON_H
#define TURNSTILE_SERVER_COMMON_H

#include <utilitieslib/stdtypes.h>
#include "TurnstileCommon.h"

#define EVENT_READY_BASE_DELAY 60    // base delay to wait for players to respond to event ready message.  Other timeouts are computed from this.

typedef struct QueueGroup QueueGroup;
typedef struct NetLink NetLink;
typedef struct MissionInstance MissionInstance;
typedef enum TUT_MissionType
{
    TUT_MissionType_Contact,
    TUT_MissionType_Instance,
    TUT_MissionType_Count,
}TUT_MissionType;

typedef struct MissionInstance
{
    QueueGroup *queuedGroups;        // QueueGroups assigned to this mission during startup.  This reverts to NULL when the event starts.
    int eventId;
    int mapId;                        // mapID of this instance's map
    Vec3 pos;
    int mapOwner;                    // dbid of the map's owner.  This is the player that has the placeholder team and league
    int numPlayer;                    // Total playercount
    int numDeclined;                // number of players declining the accept
    int numAccept;                    // number of players accepted (in the event or queued to enter the event)
    int maxLevel;                    // Highest iLevel of accepting players
    int checkForSub;                //    indicates whether the turnstile event will allow subbing at this point
    U32 lastCheckTime;                //    indicates the last time we tried to refill this event
    int numRefillAttempts;            //    how many refill attempts has this instance had
    int *timeoutPlayerId;
    int *timeoutPlayerTimestamp;    
    int *banIDList;                    // List of auth_id's which have been banned
    NetLink *link;                    // Netlink to this instance's dbserver
    U32 eventStartTime;
    MissionInstance * masterInstance;
    int playersAlerted;
} MissionInstance;

// Mission definition.  Most of this is slurped out of data\defs\turnstile_server.def
typedef struct TurnstileMission
{
    char *name;                        // Mission name.  This is really an internal identifier.
    TurnstileCategory category;        // Which clientside category this event is displayed in.
    int preformedMinimumPlayers;    // MinimumPlayers is the minimum needed to start this
    int pickupMinimumPlayers;    // MinimumPlayers to start automatically
    int maximumPlayers;                // MaximumPlayers is the maximum this content can handle
    int startDelay;                    // Length of time we hold the mission above minimum but below maximum waiting to fill it
    int levelShift;                    // Optional level shift for static mobs
    int forceMissionLevel;            // Force the level of the turnstile mission to a fixed value (for auto-exemplar/sidekicking trials)
    int minimumTank;                // Minimum tank count needed for a viable group
    int minimumBuff;                // Minimum buff count needed
    int minimumDPS;                    // Minimum DPS count needed
    int minimumMelee;                // Optional minimum melee DPS needed
    int minimumRanged;                // Optional minimum ranged DPS needed
    char *required;                    // Requires statement that determines if the player is eligible
    char *hideIf;                    // Requires statement that hides if the player is the state
    TUT_MissionType    type;            // What do do with the group when it's assembled
    char **mapName;                    // This gives the mapname for both instances and zone missions - if multiple are specified one will be picked randomly
    char *script;                    // For instance events, this is the name of a scriptdef to fire
    Vec3 pos;                        // For zone events, where in the zone to send them to - abs x,y,z
    char *location;                    // For zone events, where in the zone to send them to - beacon name
    float radius;                    // For zone events, the spread around pos to land players.
                                    // This avoids a monumental fustercluck when players zone to the starting location
    char **linkToStr;
    int *linkTo;
    int isLink;
    int missionID;
    int playerGang;
    bool autoQueueMyLinks;
// Members below this line are zero filled by the struct parser, but are otherwise unused.  They contain the running data
// for each mission

    MissionInstance **instanceList;    // Earray of instances of this event that are running.
    int totalQueued;                // Current number of players queued
    float avgWait;                    // Average wait time before getting into this mission
    U32 lastStartTime;                // timersecondsSince2000() at which we last started an instance of this.
    U32 possibleStartTime;            // timersecondsSince2000() at which it became possible to start this event.
} TurnstileMission;

// Archtype settings.  These map each archetype to it's possible roles.  melee should really be limited to just Yes / No,
// so there are some runtime code checks for this.
typedef struct ATDesc
{
    char *className;                // Internal name of the AT, e.g. Class_Scrapper
    int dps;
    int tank;
    int buff;
    int melee;
} ATDesc;

// Info for a dbserver.  Slurped out of data\server\db\turnstile_server.cfg
typedef struct DBServerCfg
{
    char *serverName;                // A slight misnomer, actually the ip address / fqdn to connect to
    char *serverPublicName;            // The public ip address / fqdn that clients connect to
    char *shardName;                // The shard name, e.g. "pinnacle"
    U32 shardID;                    // Numeric shard ID
    int authShardID;                // Authserver's notion of this guy's shard ID.  In the presence of this, I'm not 100% convinced that we need our own shard ID
    U32 serverAddr;                    // Address that serverPublicName translates to
    U32 serverPort;                    // Port number clients connect to
    int active;
} DBServerCfg;

// Grandaddy data structures that hold everything
typedef struct TurnstileConfigCfg
{
    DBServerCfg **dbservers;        // List of dbservers we can connect to
    int fastEMA;                    // DEBUG FLAG - makes the average wait time EMA use a half life of 10 players
    float overallAverage;            // Not config data per se, but there is a lot of sense in placing this here
} TurnstileConfigCfg;

typedef struct TurnstileConfigDef
{
    ATDesc **atDesc;                // AT descriptions
    TurnstileMission **missions;    // Mission data
} TurnstileConfigDef;


extern TurnstileConfigCfg turnstileConfigCfg;
extern TurnstileConfigDef turnstileConfigDef;

#define    INITIAL_WAIT_TIME            (5.0f * 60.0f)        // 5 minutes in seconds.

#define TS_GROUP_FLAGS_WILLING_TO_SUB (1 << 0)
#define TS_GROUP_FLAGS_WANTS_TO_OWN_GROUP (1 << 1)

float updateAverageWaitTime(float oldTime, int numPlayers, int seconds);

int turnstileParseTurnstileConfig(int loadFlags);
void turnstileDestroyConfig();
DBServerCfg *turnstileFindShardByName(char *shardName);
DBServerCfg *turnstileFindShardByNumber(int shardID);

#define        XFER_TYPE_PAID                    0
#define        XFER_TYPE_VISITOR_OUTBOUND        1
#define        XFER_TYPE_VISITOR_BACK            2
// This last one is a bit of a misnomer.  It's not a transfer per se, since the character is already present on the remote shard.  This is used when logging in
// For the operation of getting a message through the auth server to the remote 
#define        XFER_TYPE_VISITOR_LOGIN            3

#define        TURNSTILE_LOAD_CONFIG            (1 << 0)
#define        TURNSTILE_LOAD_DEF                (1 << 1)

#define        SHARD_VISITOR_DATA_SIZE            64

typedef enum TIS_Status{
    TIS_NONE,
    TIS_EVENT_LOCKED_FOR_GROUP,
    TIS_LOOKING,
    TIS_WAITING_FOR_RESPONSE,
    TIS_ALL_RESPONDED,
} TIS_Status;

typedef enum TGS_Status{            //    turnstile group substitution status
    TGS_NONE,                        //    default, not willing to sub
    TGS_WILLING,                    //    server asked, and player said yes
} TGS_Status;
#endif // TURNSTILE_COMMON_H
