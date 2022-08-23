// encounterPrivate.h - structure definitions for
// encounters

#ifndef __ENCOUNTERPRIVATE_H
#define __ENCOUNTERPRIVATE_H

#include "encounter.h"
#include "storyarc/storyarcutil.h"
#include <utilitieslib/utils/scriptvars.h>
#include "entity/entityRef.h"
#include "svr/svr_base.h"
#include "gameComm/VillainDef.h"
#include "script/scriptengine.h"
#include "script/scriptconstants.h"
#include "patrol.h"
#include "gameSys/cutScene.h"
#include "gridcoll/megaGrid.h"
#include "script/scriptengine.h"
#include "storyarc/dialogdef.h"

typedef struct GridIdxList GridIdxList;

extern int g_encounterPlayerRadius;        // base radius for setting off spawns
extern int g_encounterVillainRadius;    // base radius to determine distance between spawns
extern int g_encounterInactiveRadius;    // when the encounter will go inactive and say something
extern int g_encounterAdjustProb;        // adjustment to normal spawn probability

// the radii for encounters are no longer constant
#define EG_MAX_SPAWN_RADIUS            (g_encounterSpawnRadius)        // encounters can override with a smaller number
#define EG_AWAKE_RADIUS                (g_encounterSpawnRadius + 50)    // when we awake and start checking for spawning 
#define EG_STORYARC_RADIUS            (g_encounterSpawnRadius + 70)    // distance we'll look for a player to attach the encounter to
#define EG_INACTIVE_RADIUS            (g_encounterInactiveRadius)        // distance that we'll go inactive and say dialog
#define EG_VILLAIN_RADIUS            (g_encounterVillainRadius)        // default distance from another villain required around an auto-grouped encounter
#define EG_PLAYER_RADIUS            (g_encounterPlayerRadius)        // we won't create an encounter inside this radius

#define EG_PLAYER_VELPREDICT        6                                // how many timesteps of velocity to add to a player's position
                                                                    // - used to make encounter spawning radii a bit predictive

// panic threshold numbers - defaults
#define EG_PANIC_THRESHOLD_LOW        (2.0)
#define EG_PANIC_THRESHOLD_HIGH        (EG_PANIC_THRESHOLD_LOW + 0.25)
#define EG_PANIC_MIN_PLAYERS        100

// some limitations on behaviors of encounter groups
#define EG_DEFAULT_SPAWNPROB        100                    // HACK for missions: changing to 100% for now, probability encounter group will go off
#define EG_MAX_ENCOUNTER_POSITIONS    50                    // positions for an encounter can only be numbered 0-49
#define EG_DEFAULT_WANDER_RANGE        50                    // default radius that a critter will wander in
#define EG_LEVEL_PUSH_PROB            80                    // % chance that a encounter will be made at the instigator's level
#define EG_TEAMSIZE_PUSH_PROB        80                    // % chance that an encouner will be made at the instigator's team size
#define EG_MAX_ENCOUNTER_CAMERAS    10

#define NUM_PCC_OVERRIDE_POWERS        3
typedef struct StoryReward StoryReward;

typedef enum EncounterAlliance {
    ENC_UNINIT,
    ENC_FOR_VILLAIN,
    ENC_FOR_HERO,
} EncounterAlliance;

typedef enum MissionTeamOverride
{
    MO_NONE,            // Don't override the team
    MO_PLAYER,            // Override the team with the player's allyID
    MO_MISSION,            // Override the team with the mission's allyID
} MissionTeamOverride;

typedef enum ActorType {
    ACTOR_VILLAIN,
    ACTOR_NPC,
} ActorType;
// MAK - these fields mean less and less.  
//    - use TeamEvil to decide if they are a villain or a victim - not this
//  - this field determines whether they are a critter or npc when created by model
//  - this field determines whether mission asked to do a hostage rename

// state of an actor in an encounter
typedef enum ActorState {
    ACT_WORKING,        // immediately after spawning
    ACT_INACTIVE,        // once a hero comes within hearing distance
    ACT_ACTIVE,            // at least one actor has seen a hero
    ACT_ALERTED,        // this particular actor has seen the hero
    ACT_COMPLETED,        // all villains killed, or encounter completed another way
    ACT_SUCCEEDED,        // all the villains killed, or key villain killed
    ACT_FAILED,            // key npc died
    ACT_THANK,            // actor has found hero to thank

    ACT_NUMSTATES
} ActorState;

#define    ACTOR_ALWAYSCON            (1 << 0)    // turn on alwaysCon bit
#define    ACTOR_SEETHROUGHWALLS    (1 << 1)    // turn on seeThroughWalls bit
#define    ACTOR_INVISIBLE            (1 << 2)    // create with invisible bit set

typedef enum ConColor
{
    CONCOLOR_RED,
    CONCOLOR_YELLOW
} ConColor;

typedef struct Actor
{
    // most fields are variable
    ActorType    type;        // DEPRECATED - use priority lists to change teams if required
    int        number;            // not variable
    int        minheroesrequired;    // not variable
    int        maxheroesrequired;    // not variable
    int        exactheroesrequired;// not variable
    int        nogroundsnap;    // not variable
    int        nounroll;        // not variable
    int        customCritterIdx;// bit to indicate player creation
    int        customNPCCostumeIdx;
    char*    ally;            // variable, Hero / Villain / Monster 
    char*    gang;            // variable
    char*    displayname;    // str
    char*    displayinfo;    // str
    char*    displayGroup;    // str
    char*    actorname;        // str name used in XL to refer to this guy
    int*    locations;        // not variable
    char*    shoutrange;        // int
    char*    shoutchance;    // int
    char*    wanderrange;    // int
    char*    notrequired;    // int
    char*    model;            // str, (costume) used only for NPC's and villain costume override
    char*    villain;        // str
    char*    villaingroup;    // str
    char*    villainrank;    // str
    char*    villainclass;    // str
    int        villainlevelmod;// not variable
    int        ai_group;        // not variable
    int        succeedondeath; // not variable
    int        failondeath;    // not variable, DEPRECATED - any actor on team good will fail encounter
    int        flags;            // not variable, ACTOR_Xxx bits
    int        npcDefOverride;    // in case you want to specify the costume.
    char*    ai_states[ACT_NUMSTATES];    // str's
    char*    dialog_states[ACT_NUMSTATES];    // str's
    //char*    textparams[MAX_TEXT_PARAMS];    // str
    StoryReward** reward;
    ConColor conColor;
    int        rewardScaleOverridePct;
    char**    bitfieldVisionPhaseRawStrings;
    int        bitfieldVisionPhases;
    char*    exclusiveVisionPhaseRawString;
    int        exclusiveVisionPhase;
} Actor;

#define    SPAWNDEF_ALLOWADDS        (1 << 0)    // allow adding generic actors (Nictus hunters, etc.)
#define    SPAWNDEF_IGNORE_RADIUS    (1 << 1)    // ignore radius checks and be ignored in radius checks
#define    SPAWNDEF_AUTOSTART        (1 << 2)    // Start this encounter as soon as the map is loaded.
#define    SPAWNDEF_MISSIONRESPAWN    (1 << 3)    // Respawn this encounter on mission maps
#define    SPAWNDEF_CLONEGROUP        (1 << 4)    // Clone this encounter when allocated on mission maps
#define    SPAWNDEF_NEIGHBORHOODDEFINED    (1 << 5)    // try to obtain min/max level and teamsize from the neighborhood this spawn is placed in

typedef struct SpawnDef
{
    const char* logicalName;     // task included SpawnDefs w/ same filename can have different logicalNames
    const char* fileName;            // filename
    const char* spawntype;        // variable!, a string used to match with encounter (only for storyarcs and tasks)
    const char* dialogfile;        // filename of a dialog file to attach to vars
    bool deprecated;
    int spawnradius;
    int villainminlevel;
    int villainmaxlevel; 
    int minteamsize;
    int maxteamsize;
    int respawnTimer;
    int playerCreatedDetailType;
    U32 flags;

    const Actor** actors;
    const ScriptDef** scripts;
    const DialogDef **dialogDefs;

    const StoryReward **encounterComplete;

    const CutSceneDef ** cutSceneDefs;

    // fields for spawns within missions
    MissionPlaceEnum missionplace;    // where do I get placed within a mission (front, middle, back)
    int missioncount;                // how many times do I appear within a mission
    bool missionuncounted;            // this spawndef doesn't count toward DefeatAllXxx
    const MissionObjective** objective;    // optional objective spec for this encounter

    const char** createOnObjectiveComplete; //Create me when this Objective is completed, and not any other time
    const char** activateOnObjectiveComplete; //GO active (and create if necessary) when this objective is complete, and no other time

    EncounterAlliance encounterAllyGroup;    // Encounter is designed for this alliance group
    
    MissionTeamOverride    override;            // Override the team with the player's team or mission's team    

    int oldfield;                // not used any more

    int    CVGIdx;
    int CVGIdx2;
    SCRIPTVARS_STD_FIELD
} SpawnDef;

extern TokenizerParseInfo ParseSpawnDef[];

typedef struct SpawnDefList
{
    const SpawnDef** spawndefs;
} SpawnDefList;

// dialog files
typedef struct DialogFile
{
    char* name;                // filename
    SCRIPTVARS_STD_FIELD
} DialogFile;

typedef struct DialogFileList
{
    DialogFile** dialogfiles;
} DialogFileList;

// state of the encounter group
typedef enum EGState {
    EG_ASLEEP    = ENCOUNTER_ASLEEP,        // not near any players, not polled
    EG_AWAKE    = ENCOUNTER_AWAKE,        // players getting close, polled for EP_WORKING change
    EG_WORKING    = ENCOUNTER_WORKING,    // creatures spawned
    EG_INACTIVE = ENCOUNTER_INACTIVE,    // someone has heard encounter
    EG_ACTIVE    = ENCOUNTER_ACTIVE,        // creatures have been alerted
    EG_COMPLETED= ENCOUNTER_COMPLETED,    // we are logically completed, still have NPC's with stuff to do
    EG_OFF        = ENCOUNTER_OFF,        // went off, either didn't spawn anything, or I'm closed after completion
} EGState;

#define EGSTATE_IS_RUNNING(x) (((x) >= EG_WORKING) && ((x) <= EG_COMPLETED))

// an encounter point on the running map
typedef struct EncounterPoint {
    const SpawnDef ** infos;    // corresponding static infos for this point
    const char* name;        // the logical name for this encounter point (for matching with story arcs)
    const char* geoname;    // the name of the associated geometry object - for debugging only
    Mat4 mat;                // position 
    Vec3 positions[EG_MAX_ENCOUNTER_POSITIONS];        // positions of child nodes
    Vec3 zpositions[EG_MAX_ENCOUNTER_POSITIONS];    // z-dropped version of positions array
    Vec3 pyrpositions[EG_MAX_ENCOUNTER_POSITIONS];    // orientations of positions
    eEPFlags epFlags[EG_MAX_ENCOUNTER_POSITIONS];    
    F32     stealthCameraArcs[EG_MAX_ENCOUNTER_POSITIONS];    
    F32 stealthCameraSpeeds[EG_MAX_ENCOUNTER_POSITIONS];
    PatrolRoute* patrol;    // optional route attached to point

    //Mat4 cutSceneCameraMats[EG_MAX_ENCOUNTER_CAMERAS+1];            //Optional camera position for cutscenes //TO DO dynArray

    unsigned int squad_renumber    : 1;    // renumber points 1-5, 6-10, 11-15 in distance when encounter goes off
    unsigned int autospawn        : 1;    // point marked as AutoSpawn, doesn't require a villain radius    
    unsigned int havezdropped    : 1;    // have I calculated zpositions yet?
    unsigned int allownpcs        : 1;    // Should I allow random npcs to spawn here?
} EncounterPoint;

typedef struct EncounterActiveInfo {
    const SpawnDef*        spawndef;        // attached spawn definition
    ScriptVarsTable        vartable;        // vars for this def
    unsigned int        seed;            // generated seed for vars
    int                    point;            // what EncounterPoint was selected
    int                    villaincount;    // how many villains are left in scenario
    int                    numheroes;        // how many heroes this spawn was generated for
    Entity**            ents;            // EArray of entities that belong to this group
    int*                scriptids;        // list of scripts that I'm running
    EntityRef            whokilledme;    // hero who killed the last villian

    int*            whoKilledMyActors;    // heroes who killed any villain in encounter
    // only used if spawndef->encounterComplete is not NULL
    // must be 32-bit for EArray, received from 32-bit TeamMembers::ids array.

    EntityRef            instigator;        // hero to started the encounter
    int                    villainlevel;    // what level villains should be generated at (modified by VillainModLevel)
    int                    villainbaselevel;//what level villains were before notoriety and pacing adjustments
    VillainGroupEnum    villaingroup;    // what group villains belong to (default)
    int                    numactiveactors;// how many of my actors are active
    PatrolRoute*        patrol;            // optional route I got from my EncounterPoint
    unsigned int        mustspawn    : 1;// don't do probability rolls, this spawn must go off (story arc or task)
    unsigned int        taskcomplete: 1;// the corresponding task has been completed
    unsigned int        didactivate : 1;// did the encounter ever activate?
    unsigned int        succeeded    : 1;// did the encounter logically succeed?
    unsigned int        failed        : 1;// did the encounter logically fail?
    unsigned int        kheldian    : 1;// was the encounter started with a Keldean team?
    unsigned int        bossreduction: 1;// should we reduce bosses to lieutenants?
    unsigned int        iscanspawn    : 1;// this encounter was created by a canspawn
    unsigned int        waitingForVolumeTrigger : 1;

    EncounterAlliance    allyGroup;        // Encounter is designed for this alliance group
    
    // story arc fields - used if the encounter was replaced by a storyarc task
    EntityRef            player;            // player who started the encounter
    int                    taskcontext;    // task identifier
    int                    tasksubhandle;    // ..
    int                    taskvillaingroup; // Villain Group for Random Villain Group Tasks(Newspaper)

} EncounterActiveInfo;

typedef struct MissionObjectiveInfo MissionObjectiveInfo;

typedef struct EncounterGroup {
    Mat4 mat;                // position
    GridIdxList* grid_idx;    // for encounter grid
    Array children;            // EncounterPoint's belonging to this group
    EGState    state;            // current state of group
    EncounterActiveInfo* active; // active stuff set up when we are awake and ready to go off
    const char* groupname;    // (optional) proper name of group for debug commands
    const char* geoname;    // the name of the encounter geometry object - for debugging only
    int villainradius;        // the radius required around encounter clear of villains (-1 for EG_VILLAIN_RADIUS)
    F32 time_start;            // what time of day is it possible to start
    F32 time_duration;        // how long during the day is it possible to start
    F32 autostart_begin;    // the group can spawn itself between _begin and _end hours from turning off (if either non-zero)
    F32 autostart_end;
    U32 autostart_time;        // the ABS_TIME the group will automatically spawn
    int spawnOccupiedBy;
    int ordinal;
    const SpawnDef* missionspawn;    // (for use by mission system)
    U32 respawnTime;

    unsigned int cleaning    : 1;    // (internal) only set if I'm cleaning up ents (don't signal events, etc.)
    unsigned int manualspawn: 1;    // no auto spawn, no auto cleanup - used by scripts
    unsigned int forcedspawn: 1;    // spawns automatically once at the beginning
    unsigned int noautoclean: 1;    // no auto cleanup, cleared after encounter closes - used by scripts
    unsigned int singleuse    : 1;    // spawn only once, no auto cleanup
    unsigned int conquered    : 1;    // has this spawn group been completed at least once?
    unsigned int spawnprob    : 7;    // 0-100, probability that this point will go off
    unsigned int hasautospawn    : 1; // this group contains an AutoSpawn.  It won't do a villain radius check
    unsigned int havezdropped    : 1; // has the entire group been z-dropped yet?
    unsigned int usevillainradius : 1;  // I look for nearby groups before deciding to spawn
    unsigned int ignoreradius : 1;        // I don't look for nearby groups before deciding to spawn *and* others don't look at me when deciding to spawn (overrides useVillainRadius)
    unsigned int intray        : 1;    // is this group inside a tray?  (really, is my first child inside a tray)
    unsigned int personobjective: 1; // this group is a person objective for a mission (hostage encounter)
    unsigned int missiongroup : 1;    // this has a spawn marked "Mission" (will get spawn from missionspawn list)
    unsigned int uncounted    : 1;    // this group not counted toward mission DefeatAllXxx goals
    unsigned int scriptControlsCompletion : 1; //event scripts toggles this on and off
    unsigned int scriptHasNotHadTick : 1; // true if encounter has a script but it hasn't had a single tick yet
    unsigned int volumeTriggered  : 1;

    int cutSceneHandle;  //handle to the cut scene it is currently running os it knows to no do other things

    MissionObjectiveInfo* objectiveInfo;

    MegaGridNode*            encounterGridNode;  // For fast checks

} EncounterGroup;

enum 
{
    NEARBY_PLAYERS_OK = 0,
    FEWEST_NEARBY_PLAYERS = 1,
};

// info attached to entities generated by encounter groups
typedef struct EncounterEntityInfo {
    EncounterGroup*            parentGroup;        // what group do I belong to?
    int                        actorID;            // what actor do I represent? (-1 if none)
    int                        actorSubID;            // what specific entity am I in this actor subset?
    Vec3                    spawnLoc;            // what location was I spawned at?
    int                        wanderRange;        // how far can I wander?
    ActorState                state;                // what state am I in?
    unsigned int            rewardgiven : 1;    // have I given my reward?
    unsigned int            alert : 1;            // am I currently alert? (straight from AI)
    unsigned int            villaincounted : 1; // am I counted by active->villaincount?
    unsigned int            activated : 1;        // am I counted by active->numactiveactors?
    unsigned int            dead : 1;            // does the encounter know I'm dead yet?
    unsigned int            notrequired : 1;    // don't count me for encounter completion
} EncounterEntityInfo;

void EncounterCleanup(EncounterGroup* group);

const SpawnDef* SpawnDefFromFilename(const char* filename);    // returns NULL on error
int VerifySpawnDef(const SpawnDef* spawndef, int inmission); // returns 0 on error
void SpawnDef_ParseVisionPhaseNames(SpawnDef *spawn);

extern int g_encounterForceTeam;        // a debug override - mission system can look at this too

typedef enum egStatusFlags
{
    INACTIVE_ONLY    = 1 << 0,
    ACTIVE_ONLY        = 1 << 1,

} egStatusFlags;


// for scripting
EncounterGroup* EncounterGroupByName(const char* groupname, const char* layout, int inactiveonly); // layout is optional
EncounterGroup* EncounterGroupByRadius(Vec3 pos, int radius, const char* layout, int inactiveonly, int nearbyPlayers, int innerRadius);
//TO DO fold above three into this
int FindEncounterGroups( EncounterGroup *results[], 
                        Vec3 center,            //Center point to start search from
                        F32 innerRadius,        //No nearer than this
                        F32 outerRadius,        //No farther than this
                        const char * area,        //Inside this area (as defined in scripts.h)
                        const char * layout,    //Layout required in the EncounterGroup
                        const char * groupname,    //Name of the encounter group ( Property GroupName )
                        SEFindEGFlags flags,
                        const char * missionEncounterName);

int EncounterNewSpawnDef(EncounterGroup* group, const SpawnDef* spawndef, const char* spawntype);
void EncounterEntDetach(Entity* ent);
void EncounterEntAttach(EncounterGroup* group, Entity* ent);
int EncounterGetHandle(EncounterGroup* group);    // handles become invalid if encounters are reloaded
EncounterGroup* EncounterGroupFromHandle(int handle);
void EncounterComplete(EncounterGroup* group, int success);
int EncounterPosFromActor(Entity* ent);
int EncounterMatFromPos(EncounterGroup* group, int pos, Mat4 result);
int EncounterMatFromLayoutPos(EncounterGroup* group, char* layout, int pos, Mat4 result);
int EncounterGetPositionInfo( EncounterGroup* group, int encounterPosition, Vec3 pyr, Vec3 pos, Vec3 zpos, int * EPFlags, F32 * stealthCameraArc, F32 * stealthCameraSpeed );
char * EncounterActorNameFromActor(Entity* ent);
Entity * EncounterActorFromEncounterActorName(EncounterGroup* group, const char * actorName );
void ShowCutScene(Entity* player);

//  Nictus hunter adds to spawns

// all these probabilities are 0.0 to 100.0
typedef struct RankSelect
{
    int        teamsize;                        // 1 .. n, this is verified on load to be in order
    F32        boss;                            // chance of getting a boss
    F32        lieutenant;                        // otherwise, chance of getting a lieutenant
} RankSelect;
typedef struct NictusOptions
{
    F32        chanceMissionAdd;                // chance an encounter will have an add
    F32        chanceZoneAdd;                    // .. in a zone
    F32        chanceNictusGroup;                // chance that we select from Nictus instead of correct VG
    RankSelect** rankByTeamSize;            // chances for bosses, etc by team size
} NictusOptions;
extern NictusOptions g_nictusOptions;

// don't allow too many ambushes for a team
#define TEAM_AMBUSH_DELAY        180            // seconds between ambushes a team can have
int TeamCanHaveAmbush(Entity* player);        // returns whether ambush is ok, resets time if it is

// DEBUG ONLY stuff
void EncounterDebugInfo(int on);        // turn debug broadcasts on or off
int EncounterGetDebugInfo();
void EncounterForceReload(Entity* player);// reload all scripts and spawn points from map
void EncounterForceTeamSize(int size);    // force spawn points to pretend team-ups are of size X (0 to disable)
void EncounterForceIgnoreGroup(int on); // force EncounterGroups to be ignored, -1 resets to default behavior
void EncounterForceSpawnGroup(ClientLink* client, EncounterGroup* group); // spawn entities in an encounter
void EncounterForceSpawn(ClientLink* client, char* groupname);    // spawn entities in an encounter
void EncounterForceSpawnClosest(ClientLink* client);
void EncounterForceSpawnAll();
void EncounterForceSpawnAlways(int on);// if set, all encounters will always spawn (don't check probability)
int EncounterGetSpawnAlways();// if set, all encounters will always spawn (don't check probability)
void EncounterMemUsage(ClientLink* client); 
void EncounterDebugStats(ClientLink* client);
void EncounterGetStats(int* playercount, int* running, int* active, F32* success, F32* playerratio, F32* activeratio, int* panic);
void EncounterNeighborhoodList(ClientLink* client);  // DEBUG - print a list of encounters broken down by neighborhood
void EncounterSetTweakNumbers(int playerradius, int villainradius, int adjustprob);
void EncounterGetTweakNumbers(int *playerradius, int* villainradius, int* adjustprob, int* playerdefault, int* villaindefault, int* adjustdefault);
void EncounterSetPanicThresholds(F32 low, F32 high, int minplayers);
void EncounterGetPanicThresholds(F32* low, F32* high, int* minplayers);
void EncounterSetCritterLimits(int low, int high);
void EncounterGetCritterNumbers(int *curcritters, int *mincritters, int *belowmin, int *maxcritters, int* abovemax);
void EncounterSendBeacons(Packet* pak, Entity* player);


#endif __ENCOUNTERPRIVATE_H