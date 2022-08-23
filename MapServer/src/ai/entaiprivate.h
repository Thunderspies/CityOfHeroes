
#ifndef ENTAIPRIVATE_H
#define ENTAIPRIVATE_H

#include <utilitieslib/utils/wininclude.h>
#include <float.h>
#include <utilitieslib/assert/assert.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#include <utilitieslib/utils/utils.h>

#include "entaivars.h"
#include <entity/entity_enum.h>

#define AI_MINIMUM_STANDOFF_DIST    0.5f
#define AI_ROUTE_STANDOFF_DIST        10    // leader standoff
#define AI_FOLLOW_STANDOFF_DIST        10    // follower standoff
#define AI_RUN_TO_CREATOR_DIST        30    // if further away, run when returning to creator

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct AIBehavior                AIBehavior;
typedef struct AIPriorityAction            AIPriorityAction;
typedef struct AIPriorityDoActionParams    AIPriorityDoActionParams;
typedef struct Character                Character;
typedef struct NavPathWaypoint            NavPathWaypoint;

//----------------------------------------------------------------
// entai.c stuff.
//----------------------------------------------------------------

typedef int (*PickBeaconScoreFunction) (Entity*,Beacon*);

typedef struct AIState {
    U32            totalTimeslice;
    U32            activeCritters;
    U32            alwaysKnockPlayers;
    U32            noNPCGroundSnap : 1;
} AIState;

extern AIState ai_state;

void            aiTurnBodyByDirection(Entity* e, AIVars* ai, Vec3 direction);
void            aiTurnBodyByPYR(Entity* e, AIVars* ai, Vec3 pyr);
int                aiGetAttackerCount(Entity* e);

bool            aiSetCurrentPower(Entity* e, AIPowerInfo* info, int enterStance, int overrideDoNotUsePowersFlag);
void            aiDiscardCurrentPower(Entity* e);
int                aiPowerTargetAllowed(Entity* e, AIPowerInfo* info, Entity* target, bool buffCheck);
int                aiUsePower(Entity* e, AIPowerInfo* info, Entity* target);
int                aiUsePowerOnLocation(Entity* e, AIPowerInfo* info, const Vec3 target);
void            aiSetTogglePower(Entity* e, AIPowerInfo* info, int set);
float            aiGetPowerRange(Character* pchar, AIPowerInfo* info);
float            aiGetPreferredAttackRange(Entity * e, AIVars * ai);

int                aiPointCanSeePoint(Vec3 src, F32 srcHeight, Vec3 dst, F32 dstHeight, F32 radius);
int                aiGetPointGroundOffset(const Vec3 src, F32 offset, F32 max, Vec3 out);
int                aiEntityCanSeeEntity(Entity* src, Entity* dst, F32 distance);
float            aiYawBetweenPoints(Vec3 p1, Vec3 p2);
int                aiPointInViewCone(const Mat4 mat, const Vec3 pos, F32 maxCosine);
AIPowerInfo*    aiChooseAttackPower(Entity* e, AIVars* ai, F32 minRange, F32 maxRange, int requireRange, int groupFlags, int stanceID);
AIPowerInfo*    aiChooseTogglePower(Entity* e, AIVars* ai, int groupFlags);
AIPowerInfo*    aiChoosePower(Entity* e, AIVars* ai, int groupFlags);
int                aiIsLegalAttackTarget(Entity* e, Entity* target);

typedef enum ChoosePowerChoiceFlags {
    CHOOSEPOWER_ANY,
    CHOOSEPOWER_TOGGLEONLY,
    CHOOSEPOWER_NOTTOGGLE,
} ChoosePowerChoiceFlags;

AIPowerInfo*    aiChoosePowerEx(Entity* e, AIVars* ai, int groupFlags, ChoosePowerChoiceFlags flags);

int                aiIsMyFriend(Entity* me, Entity* notMe);
int                aiIsMyEnemy(Entity* me, Entity* notMe);

void            aiSetAttackTarget(Entity* e, Entity* target, AIProxEntStatus* status, const char* reason, int doNotAlertEncounter, int doNotForgetTarget );
void            aiDiscardAttackTarget(Entity* e, const char* reason);
float            aiGetJumpHeight(Entity* e);
void            aiSetFlying(Entity* e, AIVars* ai, int isFlying);
void            aiSetTimeslice(Entity* e, int timeslice);
void            aiSetFOVAngle(Entity* e, AIVars* ai, F32 fovAngle);

typedef int        (*AIFindBeaconApproveFunction)(Beacon* beacon);

Beacon*            aiFindBeaconInRange(Entity* e, AIVars* ai,
                                    const Vec3 centerPos,
                                    float rxz,
                                    float ry,
                                    float minRange,
                                    float maxRange,
                                    AIFindBeaconApproveFunction approveFunc);

void            aiRemoveAvoidNodes(Entity* e, AIVars* ai);
void            aiUpdateAvoid(Entity* e, AIVars* ai);
int                aiShouldAvoidEntity(Entity* e, AIVars* ai, Entity* target, AIVars* aiTarget);
int                aiShouldAvoidBeacon(Entity* e, AIVars* ai, Beacon* beacon);

//----------------------------------------------------------------
// entaiMovement.c stuff.
//----------------------------------------------------------------

void             aiStartPath(Entity* e, int preventShortcut, int doNotRun);
void             aiTurnBody(Entity* e, AIVars* ai, int forceFaceTarget);
void             aiFollowTarget(Entity* e, AIVars* ai);
void            aiFollowPathToEnt(Entity *e, AIVars *ai, Entity *target);
void            aiFollowPathWire(Entity* e, AIVars* ai, const Vec3 target_pos, AIFollowTargetReason reason);
void            aiMotionSwitchToNone(Entity* e, AIVars* ai, const char* reason);
void            CreateComplexWire(Entity* e, AIVars* ai, Vec3* points, U32 numpoints, F32 resolution, F32 curve_dist);


//----------------------------------------------------------------
// entaiPath.c stuff.
//----------------------------------------------------------------

typedef int (*BeaconScoreFunction)(Beacon* src, BeaconConnection* conn);

typedef struct AIPathLogEntry {
    EntityRef    entityRef;
    
    int        entType;

    Vec3        sourcePos;
    Vec3        targetPos;

    S64            cpuCycles;
    U32            abs_time;
    
    int            searchResult;
    Vec3        sourceBeaconPos;
    Vec3        targetBeaconPos;
    
    F32            jumpHeight;
    U32            canFly : 1;
    U32            success : 1;
} AIPathLogEntry;

typedef struct AIPathLogEntrySet {
    AIPathLogEntry    entries[100];
    int                cur;
} AIPathLogEntrySet;

extern S64 aiPathLogCycleRanges[];
extern AIPathLogEntrySet* aiPathLogSets[];

int                aiPathLogGetSetCount(void);

void            aiConstructSimplePath(Entity* e, BeaconScoreFunction func);
void            aiConstructPathToTarget(Entity* e);
Beacon*            aiFindRandomNearbyBeacon(Entity* e, Beacon* lastBeacon, Array* beaconSet, PickBeaconScoreFunction beaconScoreFunc);
Beacon*            aiFindClosestBeacon(const Vec3 centerPos);
int                aiCanWalkBetweenPointsQuickCheck(const Vec3 src, float srcHeight, const Vec3 dst, float dstHeight);

void            aiSetFollowTargetEntity(Entity* e, AIVars* ai, Entity* target, AIFollowTargetReason reason);
void            aiSetFollowTargetBeacon(Entity* e, AIVars* ai, Beacon* target, AIFollowTargetReason reason);
void            aiSetFollowTargetPosition(Entity* e, AIVars* ai, const Vec3 pos, AIFollowTargetReason reason);
void            aiSetFollowTargetDoor(Entity* e, AIVars* ai, DoorEntry* door, AIFollowTargetReason reason);
void            aiSetFollowTargetMovingPoint(Entity* e, AIVars* ai, Mat4 point, AIFollowTargetReason reason);

void            aiCheckFollowStatus(Entity* e, AIVars* ai);
const char*        aiGetFollowTargetReasonText(AIFollowTargetReason reason);
void            aiDiscardFollowTarget(Entity* e, const char* reason, bool reached);

void            aiPathUpdateBlockedWaypoint(NavPathWaypoint* wp);

//----------------------------------------------------------------
// entaiProximity.c stuff.
//----------------------------------------------------------------

typedef struct AIProxEntStatus            AIProxEntStatus;
typedef struct AIProxEntStatusTable        AIProxEntStatusTable;

typedef struct AIProxEntStatus {
    struct {
        struct AIProxEntStatus*        next;
        struct AIProxEntStatus*        prev;
    } tableList;

    struct {
        struct AIProxEntStatus*        next;
        struct AIProxEntStatus*        prev;
    } entityList;

    struct {
        struct AIProxEntStatus*        next;
        struct AIProxEntStatus*        prev;
    } avoidList;

    Entity*                        entity;
    int                            db_id;
    
    AIProxEntStatusTable*        table;

    struct {
        F32                        toMe;
        U32                        toMeCount;
        F32                        toMyFriends;
        F32                        fromMe;
        F32                        toMeFromMeRatio;
    } damage;

    struct {
        U32                        posKnown;
        U32                        attackMe;
        U32                        attackMyFriend;
        U32                        powerUsedOnMyFriend;
    } time;
    
    struct {
        U32                        begin;
        U32                        duration;
    } invalid;
    
    F32                            dangerValue;
    U32                            assignedAttackers;
    F32                            targetPrefFactor;

    //struct {
    //    unsigned int            begin;
    //    unsigned int            duration;
    //} scared;

    struct {
        U32                        begin;
        U32                        duration;
    } taunt;

    struct {
        U32                        begin;
        U32                        duration;
    } placate;

    U32                            isTeamStatus        : 1;
    U32                            hasAttackedMe        : 1;
    U32                            hasAttackedMyFriend : 1;
    U32                            primaryTarget        : 1; // Flag (for COT) that means we prefer to target him way above the rest
    U32                            neverForgetMe        : 1;
    U32                            inAvoidList            : 1;
} AIProxEntStatus;

typedef struct AIProxEntStatusTable {
    AIProxEntStatus*            statusHead;
    AIProxEntStatus*            statusTail;
    StashTable                    dbIdToStatusTable;
    StashTable                    entToStatusTable;
    
    struct {
        AIProxEntStatus*        head;
    } avoid;

    union {
        Entity*                    entity;
        AITeam*                    team;
    } owner;
    
    U32                            isTeamTable : 1;
} AIProxEntStatusTable;

AIProxEntStatusTable*    createAIProxEntStatusTable(void* owner, int teamTable);
void                    destroyAIProxEntStatusTable(AIProxEntStatusTable* table);
AIProxEntStatus*        aiGetProxEntStatus(AIProxEntStatusTable* table, Entity* proxEnt, int create);
void                    aiRemoveProxEntStatus(AIProxEntStatusTable* table, Entity* proxEnt, int db_id, int fullRemove);
void                    aiProximityRemoveEntitysStatusEntries(Entity* e, AIVars* ai, int fullRemove);
void                    aiProximityAddToAvoidList(AIProxEntStatus* status);
void                    aiProximityRemoveFromAvoidList(AIProxEntStatus* status);

//----------------------------------------------------------------
// entaiMessage.c stuff.
//----------------------------------------------------------------

typedef void* (*AIMessageHandler)(Entity* e, AIMessage msg, AIMessageParams params);

void                aiSetMessageHandler(Entity* e, AIMessageHandler handler);

void*                 aiSendMessagePtr(Entity* e, AIMessage msg, void* data);
void*                 aiSendMessageInt(Entity* e, AIMessage msg, int data);

//----------------------------------------------------------------
// entaicritter.c stuff.
//----------------------------------------------------------------

void aiCallForHelp(Entity* me, AIVars* ai, F32 radius, const char* targetName);
U32 aiCritterGetActivityDataSize(void);
void aiCritterActivityChanged(Entity* e, AIVars* ai, int oldActivity);
void aiCritterChooseRandomPower(Entity* e);
void aiCritterEntityWasDestroyed(Entity* e, AIVars* ai, Entity* entDestroyed);
void aiCritter(Entity* e, AIVars* ai);
void aiCritterCheckProximity(Entity* e, int findTarget);
void aiCritterUsePowerNow(Entity * e);
int aiCritterSetCurrentPowerByName(Entity* e, const char* powerName);
Entity * aiSetAttackTargetToAggroPlayer(Entity *e, AIVars *ai, int critterAggroIndex);
Entity *aiGetSecondaryTarget(Entity *e, AIVars *ai, const Power *ppow);

int aiTogglePowerNowByName(Entity* e, const char* powerName, int on);
int aiUsePowerNowByName(Entity* e, const char* powerName, Entity* target);
int aiNeverChooseThisPower(Entity* e, const char* powerName);

AIPowerInfo * aiCritterFindPowerByName(AIVars* ai, const char* powerName);

//----------------------------------------------------------------
// entaiPlayer.c stuff.
//----------------------------------------------------------------

void aiPlayer(Entity* e, AIVars* ai);

//----------------------------------------------------------------
// entaiCar.c stuff.
//----------------------------------------------------------------

void aiCar(Entity* e, AIVars* ai);

//----------------------------------------------------------------
// entaiNPC.c stuff.
//----------------------------------------------------------------

U32 aiNPCGetActivityDataSize(void);
void aiNPCActivityChanged(Entity* e, AIVars* ai, int oldActivity);
void aiNPCEntityWasDestroyed(Entity* e, AIVars* ai, Entity* entDestroyed);
void* aiNPCMsgHandlerDefault(Entity* e, AIMessage msg, AIMessageParams params);
void aiNPC(Entity* e, AIVars* ai);
void aiNPCPriorityDoActionFunc(Entity* e, const AIPriorityAction* action, const AIPriorityDoActionParams* params);
void aiNPCDoActivity(Entity* e, AIVars* ai);

//----------------------------------------------------------------
// entaiDoor.c stuff.
//----------------------------------------------------------------

void aiDoor(Entity* e, AIVars* ai);
void aiDoorPriorityDoActionFunc(Entity* e, const AIPriorityAction* action, const AIPriorityDoActionParams* params);

//----------------------------------------------------------------
// entaiFeeling.c stuff.
//----------------------------------------------------------------

AIFeelingModifier*    createAIFeelingModifier(void);
void                 destroyAIFeelingModifier(AIFeelingModifier* modifier);
void                aiFeelingDestroy(Entity* e, AIVars* ai);
void                 aiFeelingUpdateTotals(Entity* e, AIVars* ai);
void                 aiFeelingAddValue(Entity* e, AIVars* ai, AIFeelingSet feelingSet, AIFeelingID feelingID, float amount);
void                 aiFeelingSetValue(Entity* e, AIVars* ai, AIFeelingSet feelingSet, AIFeelingID feelingID, float amount);
void                 aiFeelingAddModifier(Entity* e, AIVars* ai, AIFeelingModifier* modifier);
void                 aiFeelingAddModifierEx(Entity* e, AIVars* ai, AIFeelingID feelingID, float levelBegin, float levelEnd, unsigned int timeBegin, unsigned int timeEnd);

//----------------------------------------------------------------
// entaiEvent.c stuff.
//----------------------------------------------------------------

typedef struct AIPowerUsageInfo {
    Entity*                    source;
    Entity*                    target;
    BasePower*                basePower;
} AIPowerUsageInfo;

void aiEventClearHistory(AIVars* ai);

//----------------------------------------------------------------
// entaiTeam.c stuff.
//----------------------------------------------------------------

void aiTeamEntityRemoved(AITeam* team, Entity* removedEnt);
void aiTeamJoin(AITeamMemberInfo* teamToJoin, AITeamMemberInfo* newMember);
void aiTeamLeave(AITeamMemberInfo* member);
void aiTeamThink(AITeamMemberInfo* member);
Entity * aiTeamGetLeader(AIVars * ai);
Entity * aiGetArchitectBuffer(AIVars *ai);
AITeam* aiTeamLookup(int teamid);
void aiTeamRunOutOfDoor(AITeam* team);
void aiRemoveTeamMemberPower(AITeam* team, AITeamMemberInfo* member, AIPowerInfo* info);
void aiAddTeamMemberPower(AITeam* team, AITeamMemberInfo* member, AIPowerInfo* info);

#define MAX_DOOR_SEARCH_DIST    75.0

//----------------------------------------------------------------
// entaiPower.c stuff.
//----------------------------------------------------------------

typedef struct AIAvoidInstance AIAvoidInstance;

typedef struct AIAvoidInstance {
    AIAvoidInstance*    next;
    F32                    radius;
    S32                    maxLevelDiff;
    U32                    uid;
} AIAvoidInstance;

AIPowerInfo* createAIPowerInfo(void);
void destroyAIPowerInfo(AIPowerInfo* info);

AIBasePowerInfo* createAIBasePowerInfo(void);
void destroyAIBasePowerInfo(AIBasePowerInfo* info);

AIPowerInfo * aiFindAIPowerInfoByPowerName( Entity * e, const char * powerName );

void aiSetCurrentBuff(AIPowerInfo **pbuff, AIPowerInfo *buff);
void aiClearCurrentBuff(AIPowerInfo **pbuff);

void aiAddAvoidInstance(Entity* e, AIVars* ai, int maxLevelDiff, F32 radius, U32 uid);
void aiRemoveAvoidInstance(Entity* e, AIVars* ai, U32 uid);
void aiRemoveAllAvoidInstances(Entity* e, AIVars* ai);

//----------------------------------------------------------------
// entaiInstance.c stuff.
//----------------------------------------------------------------

typedef struct AIConfigPowerAssignment {
    char*                nameFrom;
    char*                nameTo;
    int                    fromType;

    int                    toBit;
} AIConfigPowerAssignment;

typedef struct AIConfigTargetPref {
    const char*            archetype;
    F32                    factor;
} AIConfigTargetPref;

typedef struct AIConfigPowerConfig {
    char*                powerName;                // name of the power
    char**                allowedTargets;            // villainDef names of critters this power is allowed to be used on

    U32                    groundAttack;            // makes sure this power is only used while on the ground
    U32                    allowMultipleBuffs;        // lets this power get around the "only one person in a team uses the same buff"
    U32                    doNotToggleOff;            // lets this power get around the "only one person in a team uses the same buff"
    F32                    critterOverridePreferenceMultiplier;    //    critter override preference for the power
} AIConfigPowerConfig;

typedef struct AIConfigSetting {
    int field;
    
    union {
        int        s32;
        float    f32;
    } value;
    
    char* string;
} AIConfigSetting;

typedef struct AIConfigAOEUsage {
    F32 fConfigSingleTargetPreference;
    F32 fAOENumTargetMultiplier;
} AIConfigAOEUsage;
typedef struct AIConfig {
    char*                name;
    char**                baseConfigNames;
    struct AIConfig**    baseConfigs;
    
    AIConfigSetting**    settings;
    AIConfigTargetPref**    targetPref;
    AIConfigPowerConfig**    powerConfigs;

    AIConfigAOEUsage *AOEUsageConfig;
    struct {
        int                            initialized;
        AIConfigPowerAssignment**    list;
    } powerAssignment;
} AIConfig;

typedef struct AIConfigPowerGroup {
    char*        name;
    int            bit;
} AIConfigPowerGroup;

typedef struct AIAllConfigs {
    AIConfig**                configs;
    AIConfigPowerGroup**    powerGroups;
} AIAllConfigs;

extern AIAllConfigs aiAllConfigs;

// Dynamically assigned power group variables.

extern int    AI_POWERGROUP_BASIC_ATTACK;
extern int    AI_POWERGROUP_FLY_EFFECT;
extern int    AI_POWERGROUP_RESURRECT;
extern int    AI_POWERGROUP_BUFF;
extern int    AI_POWERGROUP_DEBUFF;
extern int    AI_POWERGROUP_ROOT;
extern int    AI_POWERGROUP_VICTOR;
extern int    AI_POWERGROUP_POSTDEATH;
extern int    AI_POWERGROUP_HEAL_SELF;
extern int    AI_POWERGROUP_TOGGLEHARM;
extern int    AI_POWERGROUP_TOGGLEAOEBUFF;
extern int    AI_POWERGROUP_HEAL_ALLY;
extern int    AI_POWERGROUP_SUMMON;
extern int    AI_POWERGROUP_KAMIKAZE;
extern int    AI_POWERGROUP_PLANT;
extern int    AI_POWERGROUP_TELEPORT;
extern int    AI_POWERGROUP_EARLYBATTLE;
extern int    AI_POWERGROUP_MIDBATTLE;
extern int    AI_POWERGROUP_ENDBATTLE;
extern int    AI_POWERGROUP_FORCEFIELD;
extern int    AI_POWERGROUP_HELLIONFIRE;
extern int    AI_POWERGROUP_PET;
extern int    AI_POWERGROUP_SECONDARY_TARGET;

extern int    AI_POWERGROUP_STAGES;

#define AI_BUFF_FLAGS (AI_POWERGROUP_BUFF | AI_POWERGROUP_HEAL_SELF | AI_POWERGROUP_HEAL_ALLY | AI_POWERGROUP_TOGGLEAOEBUFF)
#define AI_DEBUFF_FLAGS (AI_POWERGROUP_DEBUFF | AI_POWERGROUP_TOGGLEHARM | AI_POWERGROUP_ROOT)

void*            aiActivityDataAlloc(Entity* e, U32 size);
void            aiActivityDataFree(Entity* e, void* data);
const char*        aiGetActivityName(AIActivity activity);

#define AI_ALLOC_ACTIVITY_DATA(e, ai, type) ai->activityData = aiActivityDataAlloc(e, sizeof(type))
#define AI_FREE_ACTIVITY_DATA(e, ai) {aiActivityDataFree(e, ai->activityData); \
    ai->activityData = NULL;}

//----------------------------------------------------------------
// entaiNPC.c stuff.
//----------------------------------------------------------------

typedef struct AINPCActivityDataThankHero {
    int                state;
    Entity*            hero;
    Entity*            door;
    Vec3            dest;
    U32                storedTime;
    char            thankStringHasEmotes;
} AINPCActivityDataThankHero;


typedef struct AINPCActivityDataHostage {
    int                state;
    Entity*            whoScaredMe;

    struct {
        U32            beginState;
        U32            scared;
        U32            checkedProximity;
    } time;
} AINPCActivityDataHostage;

void aiActivityThankHero(Entity* e, AIVars* ai);

//----------------------------------------------------------------
// entaiInstance.c stuff.
//----------------------------------------------------------------

void aiReInitPowerAssignments(Entity* e, AIVars* ai);

//----------------------------------------------------------------
// entaiPriority.c stuff.
//----------------------------------------------------------------
void aiPriorityRestartList(AIPriorityManager* manager);

//----------------------------------------------------------------
// entaiCritterActivityWander.c stuff.
//----------------------------------------------------------------
void aiCritterActivityPatrol(Entity* e, AIVars* ai);

#endif

