/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef ENTITY_H__
#define ENTITY_H__

#include <utilitieslib/stdtypes.h>
#include "entity/gametypes.h"
#include "seq/TriggeredMove.h"
#include "entity_enum.h"
#include "entity/entityRef.h"
#include "group/groupgrid.h"
#include "chatdefs.h"

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct NetFx NetFx;
typedef struct Supergroup Supergroup;
typedef struct Teamup Teamup;
typedef struct League League;
typedef struct TaskForce TaskForce;
typedef struct LevelingPact LevelingPact;
typedef U32 NwEmissaryDataGuid; // typedef'd in NwWrapper.h

typedef enum SayCondition
{
    SAYAFTERTIMEELAPSES = 0, //Say the next thing in the chat queue after the time parameter has elapsed
    SAYAFTERARRIVAL        = 1, //Say the next thnig in the chat queue after you arrive at the location specified in the goto command
} SayCondition;

typedef struct QueuedChat
{
    char * string;
    F32    timeToSay;
    int sayCondition;
    int privateMsg;
} QueuedChat;

typedef struct OnClickCondition
{
    OnClickConditionType type;
    char *condition;
    char *say;
    char *reward;
} OnClickCondition;

// This includes the non-struct parts for client and server.
// I moved it before the entity declaration in case there are datatypes
// which need to be defined get included before then.
#define ENT_INCLUDE_PART 3

#if SERVER
    #include "entity/entIncludeServer.h"
#endif

#if CLIENT
    #include "entity/entIncludeClient.h"
#endif

#undef ENT_INCLUDE_PART

extern const float BASE_PLAYER_SIDEWAYS_SPEED;  //as percentage of forward speed
extern const float BASE_PLAYER_VERTICAL_SPEED;    //feet per tick
extern const float BASE_PLAYER_FORWARDS_SPEED;  //feet per tick
extern const float BASE_PLAYER_BACKWARDS_SPEED;    //as percentage of forward speed

typedef struct Character        Character;
typedef struct ClientLink       ClientLink;
typedef struct DefTracker      DefTracker;
typedef struct Friend            Friend;
typedef struct WdwBase          WdwBase;
typedef struct cCostume            cCostume;
typedef struct Costume            Costume;
typedef struct PowerCustomizationList PowerCustomizationList;
typedef struct CustomNPCCostume    CustomNPCCostume;
typedef struct Power            Power;
typedef struct PowerInfo        PowerInfo;
typedef struct VillainDef        VillainDef;
typedef struct EmailInfo        EmailInfo;
typedef struct SeqInst            SeqInst;
typedef struct StaticMapInfo    StaticMapInfo;
typedef struct Packet            Packet;
typedef struct Ragdoll            Ragdoll;
typedef struct HitLocation        HitLocation;
typedef struct PNPCDef            PNPCDef;
typedef struct PCC_Critter        PCC_Critter;

typedef struct DamageTracker    DamageTracker;
typedef struct SupergroupStats    SupergroupStats;

typedef struct MissionInventory    MissionInventory;

// entity components
typedef struct EntPlayer        EntPlayer;

// entai structures.
typedef struct ScoreBoard ScoreBoard;
typedef struct AIVars AIVars;
typedef struct Grid Grid;



#define MAX_ATTACK_POWERS ( 50 )
//extern Grid ent_grid;

//TO DO, I really should make all these dynamic
//#define MAX_NETFX               30 //Max number of net fx server can track on an entity at once (also max client can hear about in one frame)
//#define MAX_CLIENT_FXTRACKERS    30 //Max number of net fx client can track in an entity at once
#define MAX_TRIGGEREDMOVES      20


#define MAX_S2C_CMDS_PER_FRAME  ( 500 )

enum
{
    DOOR_MSG_DOOR_OPENING,
    DOOR_MSG_OK,
    DOOR_MSG_FAIL,
    DOOR_MSG_ASK,
};

enum
{
    MOVETYPE_WALK    = 1,
    MOVETYPE_FLY    = 2,
    MOVETYPE_NOCOLL = 4,
    MOVETYPE_WIRE    = 8,
    MOVETYPE_JUMPPACK = 16,
};

typedef int BITFIELD32;

typedef struct EntState    //lf stuff info
{
    BITFIELD32  mode;
#if SERVER
    float       timer[ TOTAL_ENT_MODES ];
    BITFIELD32  last_sent_mode;
#endif
}EntState;

//Different things can set untargetable and invincible
//if you add more, be sure to add bits to untargetable and invincible in entity.h
#define UNTARGETABLE_GENERAL        (1 << 0)  //Set by ai config, PLs, or scripts
#define UNTARGETABLE_DBFLAG            (1 << 1)  //Permanent DB flag is set on this guy
#define UNTARGETABLE_TRANSFERING    (1 << 2)  //In the process of a door or map transfer

#define INVINCIBLE_GENERAL            (1 << 0)  //Set by ai config, PLs, or scripts
#define INVINCIBLE_DBFLAG            (1 << 1)  //Permanent DB flag is set on this guy
#define INVINCIBLE_TRANSFERING        (1 << 2)  //In the process of a door or map transfer

// these are for Entity::move_bits and Entity::next_move_bits, a hack for getting special SeqState bits down to the client
#define    NEXTMOVEBIT_RIGHTHAND        (1<<0)
#define    NEXTMOVEBIT_LEFTHAND        (1<<1)
#define    NEXTMOVEBIT_TWOHAND            (1<<2)
#define    NEXTMOVEBIT_DUALHAND        (1<<3)
#define NEXTMOVEBIT_EPICRIGHTHAND    (1<<4)
#define    NEXTMOVEBIT_SHIELD            (1<<5)
#define    NEXTMOVEBIT_HOVER            (1<<6)
#define    NEXTMOVEBIT_AMMO            (1<<7)
#define    NEXTMOVEBIT_INCENDIARYROUNDS    (1<<8)
#define    NEXTMOVEBIT_CRYOROUNDS            (1<<9)
#define    NEXTMOVEBIT_CHEMICALROUNDS        (1<<10)
#define    NEXTMOVEBIT_COUNT                11

typedef struct InterpPosition
{
    char            offsets[3];
    unsigned char    y_used        : 1;
    unsigned char    xz_used        : 1;
} InterpPosition;

// a structure to hold all of the entities strings
typedef struct EntStrings
{
    char playerDescription[1024];  // fluff
    char motto[128];               // fluff
} EntStrings;

// in some cases we want to use an existing entity to host a new
// costume for some manipulation. This structure is used to store
// the current costume information (e.g. owned, mutable, etc) so that
// it can be detached for the substitution and then later restored.
// Could be used for a 'costume' stack on the entity but does not appear to be necessary
typedef struct EntCostumeRestoreBlock
{
    const cCostume* costume;
    Costume*        owned_costume;    // should either be NULL or equal to  costume
    U32                costume_is_mutable    : 1;
    U32                costume_is_owned    : 1;
} EntCostumeRestoreBlock;

//-------------------------------------------------------------------------------------------------------------
//typedef struct Entity Entity;

typedef struct MotionState MotionState;



typedef struct Entity
{
    char*                    namePtr;            // points to name which is now at the end of the struct

    // Index of entity in the entities array. 
    // This value on the server corresponds to Entity::svr_idx on client.
    // This value on the client is different than Entity::svr_idx.
    int                     owner;              

    U8                      move_type;          // swim, walk, fly, etc

    #define ENT_INCLUDE_PART 4

    #if SERVER
        #include "entity/entIncludeServer.h"
    #endif

    #undef ENT_INCLUDE_PART

    EntityRef                erOwner;            // The entity (never a pet) which is ultimately responsible for creating this entity. If non-zero then this entity is a "pet".
    int                        teamup_id;            // dbserver's id for my teamup
    int                        teamlist_uptodate;
    int                        league_id;            // dbserver's id for my league
    int                        leaguelist_uptodate;

    SeqInst*                 seq;                // Pointer to anim. info for this entity.
    MotionState*            motion;
    EntPlayer*                pl;                    // player structure for all player entities

    EntState                state;

    Mat4                    fork;                // cur position, orientation.  Do not move.
    F32                     timestep;           // timestep for this server calc
    int                        randSeed;            // Initializing graphics on the client uses several random numbers, for size variation for example, this makes sure all clients chooses the same number
    U32                     id;                 // unique identifier for this ent
    int                     db_id;              // unique dbserver identifier for this ent (if player)
    EntityRef                erCreator;          // The entity (which might be a pet) which created this entity. If non-zero then this entity is a "pet".
    int                        supergroup_id;        // dbserver's id for my supergroup
    int                        taskforce_id;        // dbserver's id for my taskforce
    int                        raid_id;
    int                        levelingpact_id;
    U32                        auth_id;            // login userid of account holder
    char                    auth_name[32];      // login handle of account holder
    int                     static_map_id;
    int                     map_id;
    int                     gurney_map_id;           //last map with hero gurney 
    int                        villain_gurney_map_id; //last map with villain gurney (track both, because in theory you could change sides?)
    int                        mission_gurney_map_id; //for missions with custom gurneys (like prisons)
    U32                     total_time;         // total amount of time entity has been online for all logins
    U32                     on_since;           // time entity logged in
    U32                     last_time;          // time entity was last saved
    U32                     last_xp;            // time entity last received xp
    U32                     last_levelingpact_time;    // last total time the entity has received from its levelingpact.
    U32                        last_login;            // time entity last logged into game - no persisted
    U32                     login_count;        // total number of logins for entity
    AccessLevel                access_level;       // Settable only by dbserver/sql. used to control access to customer service / debug features
    U32                     chat_ban_expire;    // date when ban on chat will expire
    U32                        auction_ban_expire;    // date when ban on auction commands will expire
    DbFlag                  db_flags;
    char                    spawn_target[64];    // name of spawn location on target map

    Supergroup                *supergroup;
    SupergroupStats            *superstat;            // Supergroup stat for this entity
    SupergroupStats            **sgStats;            // Supergroup stats for entities in the group.
    Teamup                    *teamup;
    TaskForce                *taskforce;
    League                    *league;
    LevelingPact            *levelingpact;
    MissionInventory        *mission_inventory;

    // Shared network stuff
    int                     move_idx;           // remember last non-obvious seq move idx, update if it changes
    float                    move_change_timer;    // on server, nothing.  on client, time until move change.
    float                    timeMoveChanged;    //time move changed on sever;
    U8                        net_move_change_time;

    TriggeredMove            triggered_moves[MAX_TRIGGEREDMOVES]; //structure to send/recv triggered moves
    int                        triggered_move_count;  //cleared on server when it sends packet and cleared on client when the packet is handled

    int                        set_triggered_moves;//flag to sequencer to check for new triggered moves//server only

    int                     lastticksent;        //woomer temp var
    F32                        lastTimeEntSent;    //in 1/30ths of a second
    U32                     pkt_id;             // remembers last pkt id sent/received for this ent
    Vec3                    net_pos;            // last pos sent/received
    int                     net_ipos[3];        // compressed integer position of ent

    NetFx  *                 netfx_list;         // new fx to play.
    int                     netfx_count;        // how many new fx to play
    int                        netfx_max;            // for dynArrayAdd
    int                        netfx_effective;    // how many netfx need to get sent this ent update
    int                     fade;               //server's //mm fade

    // tracks state of player entities - logging out, or network disconnect timeout
    F32                        logout_timer;
    int                        logout_login;

    int                        npcIndex;
    int                        costumeIndex;
    int                        costumePriority;  // used for stacking costume powers - does not need to be sent to client
    int                        costumeNameIndex; // for reverting costume changes on critters
    char                    *costumeWorldGroup;    // gives the world group that this entity looks like.

    F32                        xlucency;

    DefTracker*                coll_tracker;

    int                        ignoredAuthId[MAX_IGNORES];   // the ignore list for the client

    Friend*                    friends;
    int                        friendCount;

    VolumeList                volumeList;
    DefTracker *            roomImIn;

    Ragdoll*                ragdoll;
    int*                    ragdollCompressed;
    F32                        ragdollFramesLeft;
    F32                        maxRagdollFramesLeft;

    const cCostume*            costume;
    PowerCustomizationList *    powerCust;
    Character*                pchar;

    char*                    favoriteWeaponAnimList;      // Used for fx replacements for seqTriggeredfx
    char*                    aiAnimList;                     // Used for fx replacements for seqTriggeredfx

        // The basic info which makes up the character's attributes.
        // Includes their XP, level, power points, class, origin, powers
        //   and so on.
        // This is the basic structure needed for combat to take place.
    EntStrings*                strings;   // holder for entity strings

    PowerInfo*                powerInfo;

    Gender                    gender;
    Gender                    name_gender;    // germans claim they need this in case a woman wants to be called 'spiderman'

    char *                    specialPowerDisplayName;  //if you are a pet and have a special power, what is it? Mirrored only on owner's client
    int                        specialPowerState; //When you use your special power

    char *                    petName; //If you're a pet, the name your master gave you
    int                        petVisibility;

    int                        idDetail;        // For bases: the detail id that this entity was spawned for

    U32            favoriteWeaponAnimListState[STATE_ARRAY_SIZE];
    U32            aiAnimListState[STATE_ARRAY_SIZE];

    Costume*            ownedCostume;        // This will be destroyed along with the entity.
    PowerCustomizationList *ownedPowerCustomizationList;
    CustomNPCCostume*    customNPC_costume;
    char*                CVG_overrideName;        // (OPTIONAL) override name for player created custom villain groups.
    F32                    fRewardScale;            // used by custom critters only for now, but could be re-appropriated
    F32                    fRewardScaleOverride;    // If this is between 0 and 1, use it instead of the villain def value 
                                                //  to calculate reward scale.

    int bitfieldVisionPhases; 
                    // bitfield that designates which VisionPhases you are in.  
                    // Is mapserver-specific and should not be persisted.
                    // To be in the same vision phase, you must pass both this and exclusive.

    int bitfieldVisionPhasesLastFrame;
    int bitfieldVisionPhasesDefault;
    float fMagnitudeOfBestVisionPhase;

    int exclusiveVisionPhase;
                    // functions similarly to bitfieldVisionPhases,
                    // except that you're only in one of this kind at a time.
                    // To be in the same vision phase, you must pass both this and bitfield.

    int exclusiveVisionPhaseLastFrame;
    int exclusiveVisionPhaseDefault;

    int shouldIConYellow; // mildly blatant hack to ensure Passive/Defensive mobs con yellow instead of red

    /* End common members ******************************************************/

    // MS: This is totally insane, but it will let you change or add server or client variables
    //     without recompiling the other one.

    #define ENT_INCLUDE_PART 1

    #if SERVER
        #include "entity/entIncludeServer.h"
    #endif

    #if CLIENT
        #include "entity/entIncludeClient.h"
    #endif

    #undef ENT_INCLUDE_PART
    const char *            description;
    char                    name[MAX_NAME_LEN];    // Moved back here to not hinder our performance... use autoexp.dat

    // Bit flags stuck at the end.
    U32                        checked_coll_tracker    : 1;
    U32                        logout_bad_connection    : 1;
    U32                        noDrawOnClient            : 1; // Doesn't draw entity on the client if set on the server.
    U32                        contactOrPnpc            : 1;
    U32                        alwaysCon                : 1;
    U32                        seeThroughWalls            : 1;
    U32                        aiAnimListUpdated        : 1;    
    U32                        commandablePet            : 1; // pet made commandable via def, scripting event or arena 
    U32                        petDismissable            : 1;
    U32                        contactArchitect        : 1; // 
    U32                        custom_critter            : 1;
    U32                        petByScript                : 1;
    U32                        showOnMap                : 1; // Rescued hostages show on the minimap.  Other things might.
    U32                        notSelectable            : 1; // Make this noselect on the client if set on the server.
    U32                        doppelganger            : 1;
    U32                        costume_is_mutable        : 1;
} Entity;

typedef struct EntityInfo
{
    EntType                    type;                        // player, critter, etc

    #define ENT_INCLUDE_PART 2

    #if SERVER
        #include "entity/entIncludeServer.h"
    #endif

    #if CLIENT
        #include "entity/entIncludeClient.h"
    #endif

    #undef ENT_INCLUDE_PART

    U32                        hide                    : 2;// Don't send me to any clients.

    U32                        send_on_odd_send        : 1;// I only get sent every other net update.
} EntityInfo;

// The arrays of Entity pointers and other big arrays of crap.

C_DECLARATIONS_BEGIN
extern Entity*                    entities[];
extern U8                        entity_state[];
extern int                        entities_max;
extern EntityInfo                entinfo[];
C_DECLARATIONS_END

#define SET_ENTINFO_BY_ID(id)    (entinfo[id])
#define SET_ENTINFO(e)            SET_ENTINFO_BY_ID((e)->owner)

#define ENTINFO_BY_ID(id)        (*(const EntityInfo *)&entinfo[id])
#define ENTINFO(e)                ENTINFO_BY_ID((e)->owner)

#define ENTTYPE_BY_ID(id)        (ENTINFO_BY_ID(id).type)
#define ENTTYPE(e)                (ENTINFO(e).type)
#define SET_ENTTYPE(e)            (SET_ENTINFO(e).type)
#define SET_ENTTYPE_BY_ID(id)    (SET_ENTINFO_BY_ID(id).type)

#define ENTHIDE_BY_ID(id)        (ENTINFO_BY_ID(id).hide)
#define ENTHIDE(e)                (ENTINFO(e).hide)
#define SET_ENTHIDE(e)            (SET_ENTINFO(e).hide)
#define SET_ENTHIDE_BY_ID(id)    (SET_ENTINFO_BY_ID(id).hide)

#define ENTMAT(e)                ((const Vec3*)(e)->fork)

//
// No more sleeping entities.
//
// entity_state has the following states:
//
// No bits set, not in use.
// bit 0, in use
// bit 1, sleeping
// bit 2, in the process of being freed.
//

#define ENTITY_IN_USE           ( 1 )
#define ENTITY_SLEEPING         ( 2 )
#define ENTITY_BEING_FREED        ( 4 )

#define GRIDENTIDX(x) ((((int)x)>>2)-1)
#define GRIDENTPTR(x) (entities[GRIDENTIDX(x)])

int entInUse(int id);
Entity * entAllocCommon();
void entFreeCommon( Entity * ent );
void entInitNetVars(Entity* e);
void entSetSeq(Entity* e, SeqInst* seq);
void entSyncControl(Entity *e);
bool entUpdatePosInterpolated(Entity *e,const Vec3 pos);
bool entUpdatePosInstantaneous(Entity *e,const Vec3 pos);
bool entUpdateMat4Interpolated(Entity *e,const Mat4 mat);
bool entUpdateMat4Instantaneous(Entity *e,const Mat4 mat);
bool entSetMat3(Entity *e,const Mat3 mat);
Entity *entFromDbIdEvenSleeping(int db_id);
void entTrackDbId(Entity *e);
void entUntrackDbId(Entity *e);
Entity * entFromDbId(int db_id);
int ent_close( const float max_dist, const Vec3 p1, const Vec3 p2 );
Entity * getChosenEnt( int target );
int ent_AdjInfluence(Entity *e, int iInfluence, char *stat);
int ent_SetInfluence(Entity *e, int iInfluence);
BOOL entIsTrial(Entity *e);
int getEntAlignment(Entity *e);
S64 getEntMaxInf(Entity *e);
int ent_canAddInfluence( Entity *e, int iInfluence );

typedef struct AccountInventorySet AccountInventorySet;
AccountInventorySet* ent_GetProductInventory( Entity* e );    // returns NULL if either e or e->pl is NULL

// Costume handlers
const cCostume* entGetConstCostume(Entity* e);    // returns the current entity const immutable costume

Costume*        entGetMutableCostume(Entity* e); // gets a mutable version of the current entity costume
Costume*        entGetMutableOrNewCostume(Entity* e); // if costume is mutable return that, otherwise start with a brand new mutable costume for entity and return that
Costume*        entUseCopyOfCurrentCostumeAndOwn(Entity* e);    // force a clone of current const costume, and own it

Costume*        entUseCopyOfCostume(Entity* e, Costume* aCostume); // give entity a local owned copy of the supplied costume, returns ptr to mutable owned costume
Costume*        entUseCopyOfConstCostume(Entity* e, const cCostume* aConstCostume);

void            entSetMutableCostumeUnowned(Entity* e, Costume* mutableCostume);
//void            entSetMutableCostumeOwned(Entity* e, Costume* mutableCostume);
void            entSetImmutableCostumeUnowned(Entity* e, const cCostume* immutableCostume);

// used when we want to save current costume state, swap in another costume for a bit, and then restore the original state
void            entDetachCostume(Entity* e, EntCostumeRestoreBlock* restore_block );
void            entRestoreDetachedCostume(Entity* e, EntCostumeRestoreBlock* restore_block);

void            entReleaseCostume(Entity* e); // release current costume and destroy of owned, etc

void            entCostumeShare(Entity* e, Entity* e_to_share_from ); // use the costume information from another entity (unowned share)

#if CLIENT
void            entSetCostumeDirect(Entity* e,  Costume* aCostume);            // set e->costume directly, does not make copies or adjust ownedCostume
Costume*        entSetCostumeAsUnownedCopy(Entity* e,  Costume* aCostume);
#endif

// Going Rogue system constants.  (Is there a better file for these?)
static int maxAlignmentPoints_Hero = 10;
static int maxAlignmentPoints_Vigilante = 10;
static int maxAlignmentPoints_Villain = 10;
static int maxAlignmentPoints_Rogue = 10;

#endif
