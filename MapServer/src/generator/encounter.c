// encounter.c - random encounter system handling.  spawns villians, etc from
// encounter spawn points

#include <time.h>
#include <utilitieslib/assert/assert.h>
#include <float.h>
#include <utilitieslib/components/StringCache.h>
#include "encounter.h"
#include "encounterprivate.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/scriptvars.h>
#include <utilitieslib/utils/memcheck.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/ArrayOld.h>
#include "group/group.h"
#include "group/groupProperties.h"
#include "group/grouputil.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/utils.h>
#include "entity/entity.h"
#include "utils/position.h"
#include "entgenCommon.h"
#include "entgen.h"
#include "ai/entai.h"
#include "ai/entaiprivate.h"
#include "ai/entaiLog.h"
#include "entity/entserver.h"
#include "varutils.h"

#include "gameComm/svr_chat.h"
#include "storyarc/storyarcutil.h"
#include "storyarc/storyarcprivate.h"
#include "svr/svr_base.h"
#include "gameComm/sendtoclient.h"
#include "gridcoll/gridcoll.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>
#include "container/team.h"
#include "gameComm/villaindef.h"
#include "entity/character_base.h"
#include "dbcomm/dbcomm.h"
#include "entity/character_level.h"
#include "comm_game.h"
#include "storyarc/storyarcInterface.h"
#include "storyarc/storyarcprivate.h"
#include "storyarc/task.h"
#include <utilitieslib/utils/timing.h>
#include "gameComm/npcnames.h"
#include "gamecomm/NpcServer.h"
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/components/MemoryPool.h>
#include "player/pmotion.h"
#include "reward.h"
#include "dbghelper.h"
#include "beacon/beaconPath.h"
#include "TeamReward.h"
#include "svr/svr_player.h"
#include "dbcomm/staticMapInfo.h"
#include <utilitieslib/version/AppVersion.h>
#include "ai/entaiPriority.h"
#include "seq/seq.h"
#include "gameSys/cutScene.h"
#include "cmdparse/cmdserver.h"
#include "gridcoll/gridfind.h"
#include <utilitieslib/components/StringTable.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>
#include "group/groupgrid.h"
#include "player/badges_server.h"
#include "ai/entaivars.h"
#include "ailib/aiBehaviorPublic.h"
#include "storyarc/dialogdef.h"
#include "entity/costume.h"
#include "entity/CustomVillainGroup.h"
#include "entity/PCC_Critter.h"
#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/playerCreatedStoryarcServer.h"
#include "gameData/BodyPart.h"
#include "dbcomm/logcomm.h"
#include "storyarc/pnpcCommon.h" // for g_exclusiveVisionPhaseNames
#include "player/pophelp.h"
#include "entity/character_animfx.h"

//TO DO where should I include this?
int ScriptGetEncounter(EncounterGroup* group, Entity* player);
static void EncounterActiveInfoDestroy(EncounterGroup* group);

#include <utilitieslib/utils/strings_opt.h>

static Array        g_encountergroups    = {0, 0, 0};


int g_bSendCritterLocations = LOCATION_SEND_NONE;

// these numbers can be tweaked to adjust the number of spawns in a zone
int g_encounterSpawnRadius = 300;        // base radius for setting off spawns
int g_encounterPlayerRadius = 200;        // we won't create a spawn inside this radius
int g_encounterVillainRadius = 125;        // base radius to determine distance between spawns
int g_encounterInactiveRadius = 100;    // when the encounter will go inactive and say something
int g_encounterAdjustProb = 0;            // adjustment to normal spawn probability

// panic system
int    g_panicSystemOn = 1;
int g_panicLevel = 0;
F32 g_panicThresholdLow = EG_PANIC_THRESHOLD_LOW;
F32 g_panicThresholdHigh = EG_PANIC_THRESHOLD_HIGH;
F32 g_panicMinPlayers = EG_PANIC_MIN_PLAYERS;

// other global numbers
int g_encounterDebugOn = 0;
int g_encounterAlwaysSpawn = 0;
int g_encounterForceTeam = 0;
int g_encounterForceIgnoreGroup = -1;
int g_encounterAllowProcessing = 1;
int g_encounterNumActive = 0;
int g_encounterNumWithSpawns = 0;
int g_encounterTotalSpawned = 0;
int g_encounterTotalActive = 0;
int g_encounterTotalCompleted = 0;
int g_encounterMinCritters = 0;
int g_encounterMaxCritters = 0;
int g_encounterCurCritters = 0;
int g_encounterMinAutostartTime = 0;
int g_encounterScriptControlsSpawning = 0;

Mat4 g_nullMat4 = { 0 };
Vec3 g_nullVec3 = { 0 };

static int s_teamSizeOverride = 0;
static int s_mobLevelOverride = 0;
static int s_respectManualSpawn = 0;

static void EncounterCalculateAutostart(EncounterGroup* group);
static void LoadNictusOptions(void);
static void StopEncounterScripts(EncounterGroup* group);
static int HasKheldianTeamMember(Entity* e);
static void EncounterDebug(char* str, ...);
static void AddNictusHunters(EncounterGroup* group);
static void StartEncounterScripts(EncounterGroup* group);
static void StartCutScenes( EncounterGroup * group );
static void EncounterCalcPanic();


// *********************************************************************************
//  Encounter mode
// *********************************************************************************

typedef enum {
    ENCOUNTERMODE_CITY,
    ENCOUNTERMODE_MISSION,
} EncounterMode;

EncounterMode g_encounterMode = ENCOUNTERMODE_CITY;

// The encounters on a map are either in city or mission map mode.  The same spawns
// and geometry can be used for both now, but there are differences in how they behave:
#define EM_IGNOREGROUPS            (g_encounterMode == ENCOUNTERMODE_CITY)
#define EM_USEVILLAINRADIUS        (g_encounterMode == ENCOUNTERMODE_CITY)
#define EM_RESPAWN                (g_encounterMode == ENCOUNTERMODE_CITY)
#define EM_USEPANICSYSTEM        (g_encounterMode == ENCOUNTERMODE_CITY)
#define EM_ALWAYSSPAWN            (g_encounterMode == ENCOUNTERMODE_MISSION)
#define EM_IGNOREEVENTS            (g_encounterMode == ENCOUNTERMODE_MISSION)

int EncounterInMissionMode(void) // extern function
{
    return g_encounterMode == ENCOUNTERMODE_MISSION;
}

void EncounterSetMissionMode(int on)
{
    g_encounterMode = on? ENCOUNTERMODE_MISSION: ENCOUNTERMODE_CITY;
}

// *********************************************************************************
//  Memory pools
// *********************************************************************************
MP_DEFINE(EncounterGroup);
MP_DEFINE(EncounterPoint);
MP_DEFINE(EncounterActiveInfo);
MP_DEFINE(EncounterEntityInfo);

static EncounterGroup* EncounterGroupAlloc()
{
    MP_CREATE(EncounterGroup, 100);
    return MP_ALLOC(EncounterGroup);
}
static void EncounterGroupFree(EncounterGroup* group)
{
    MP_FREE(EncounterGroup, group);
}

static EncounterPoint* EncounterPointAlloc()
{
    MP_CREATE(EncounterPoint, 100);
    return MP_ALLOC(EncounterPoint);
}
static void EncounterPointFree(EncounterPoint* point)
{
    MP_FREE(EncounterPoint, point);
}

static EncounterActiveInfo* EncounterActiveInfoAlloc()
{
    MP_CREATE(EncounterActiveInfo, 100);
    return MP_ALLOC(EncounterActiveInfo);
}
static void EncounterActiveInfoFree(EncounterActiveInfo* active)
{
    ScriptVarsTableClear(&active->vartable);    // required because we have clients that copy vars
    MP_FREE(EncounterActiveInfo, active);
}

static EncounterEntityInfo* EncounterEntityInfoAlloc()
{
    MP_CREATE(EncounterEntityInfo, 100);
    return MP_ALLOC(EncounterEntityInfo);
}
static void EncounterEntityInfoFree(EncounterEntityInfo* entinfo)
{
    MP_FREE(EncounterEntityInfo, entinfo);
}

// *********************************************************************************
//  Spawndefs
// *********************************************************************************

StaticDefineInt ParseActorConColor[] =
{
    DEFINE_INT
    { "Red",            CONCOLOR_RED },
    { "Yellow",            CONCOLOR_YELLOW },
    DEFINE_END
};

StaticDefineInt ParseActorType[] =
{
    DEFINE_INT
    { "eVillain",        ACTOR_VILLAIN },
    { "eNPC",            ACTOR_NPC },
    DEFINE_END
};

StaticDefineInt ParseActorFlags[] =
{
    DEFINE_INT
    { "AlwaysCon",            ACTOR_ALWAYSCON },
    { "SeeThroughWalls",    ACTOR_SEETHROUGHWALLS },
    { "Invisible",            ACTOR_INVISIBLE },
    DEFINE_END
};

TokenizerParseInfo ParseActor[] = {
    { "{",                TOK_START,        0},
    { "Type",            TOK_INT(Actor,type,    ACTOR_VILLAIN),    ParseActorType }, // DEPRECATED
    { "Number",            TOK_INT(Actor,number,        1) },
    { "HeroesRequired",    TOK_REDUNDANTNAME | TOK_INT(Actor,minheroesrequired, 0) },
    { "MinimumHeroesRequired",    TOK_INT(Actor,minheroesrequired, 0) },
    { "MaximumHeroesRequired",    TOK_INT(Actor,maxheroesrequired, 0) },
    { "ExactHeroesRequired",    TOK_INT(Actor,exactheroesrequired, 0) },
    { "ActorName",        TOK_NO_TRANSLATE | TOK_STRING(Actor,actorname, 0) },
    { "Name",            TOK_STRING(Actor,displayname, 0) },
    { "DisplayInfo",    TOK_STRING(Actor,displayinfo, 0) },
    { "DisplayGroup",    TOK_STRING(Actor,displayGroup, 0) },
    { "Location",        TOK_INTARRAY(Actor,locations) },
    { "ShoutRange",        TOK_NO_TRANSLATE | TOK_STRING(Actor,shoutrange, 0) },
    { "ShoutChance",    TOK_NO_TRANSLATE | TOK_STRING(Actor,shoutchance, 0) },
    { "WalkRange",        TOK_NO_TRANSLATE | TOK_STRING(Actor,wanderrange, 0) },
    { "NotRequired",    TOK_NO_TRANSLATE | TOK_STRING(Actor,notrequired, 0) },
    { "NoGroundSnap",    TOK_INT(Actor,nogroundsnap, 0) },
    { "Ally",            TOK_NO_TRANSLATE | TOK_STRING(Actor,ally, 0) },
    { "Gang",            TOK_NO_TRANSLATE | TOK_STRING(Actor,gang, 0) },
    { "ClassOverride",    TOK_NO_TRANSLATE | TOK_STRING(Actor,villainclass, 0) },
    { "NoUnroll",        TOK_INT(Actor,nounroll, 0) },
    { "SucceedOnDeath",    TOK_INT(Actor,succeedondeath, 0) },
    { "FailOnDeath",    TOK_INT(Actor,failondeath, 0) }, // DEPRECATED
    { "CustomCritterIdx",TOK_INT(Actor,customCritterIdx, 0) },
    { "npcDefOverride", TOK_INT(Actor,npcDefOverride, 0) },
    { "Flags",            TOK_FLAGS(Actor,flags, 0),                            ParseActorFlags },
    { "Reward",            TOK_STRUCT(Actor,reward, ParseStoryReward) },
    { "ConColor",        TOK_INT(Actor, conColor, CONCOLOR_RED), ParseActorConColor },
    { "RewardScaleOverridePct", TOK_INT(Actor, rewardScaleOverridePct, -1) },
    { "VisionPhases", TOK_STRINGARRAY(Actor, bitfieldVisionPhaseRawStrings) },
    { "DONTUSETHISJUSTINITIALIZE", TOK_INT(Actor, bitfieldVisionPhases, 1) },    // is there a better way to initialize?
    { "ExclusiveVisionPhase", TOK_STRING(Actor, exclusiveVisionPhaseRawString, 0) },
    { "DONTUSETHISEITHERINITIALIZE", TOK_INT(Actor, exclusiveVisionPhase, 0) },    // is there a better way to initialize?

    { "Model",            TOK_NO_TRANSLATE | TOK_STRING(Actor,model, 0) },
    { "VillainLevelMod",TOK_INT(Actor,villainlevelmod, 0) },
    { "Villain",        TOK_NO_TRANSLATE | TOK_STRING(Actor,villain, 0) },
    { "VillainGroup",    TOK_NO_TRANSLATE | TOK_STRING(Actor,villaingroup, 0) },
    { "VillainType",    TOK_NO_TRANSLATE | TOK_STRING(Actor,villainrank, 0) },

    { "AI_Group",        TOK_INT(Actor,ai_group, -1) },
    { "AI_InActive",    TOK_NO_TRANSLATE | TOK_STRING(Actor,ai_states[ACT_WORKING], 0) },
    { "AI_Active",        TOK_NO_TRANSLATE | TOK_STRING(Actor,ai_states[ACT_ACTIVE], 0) },
    { "AI_Alerted",        TOK_NO_TRANSLATE | TOK_STRING(Actor,ai_states[ACT_ALERTED], 0) },
    { "AI_Completion",    TOK_NO_TRANSLATE | TOK_STRING(Actor,ai_states[ACT_COMPLETED], 0) },
    { "Dialog_InActive",    TOK_STRING(Actor,dialog_states[ACT_INACTIVE], 0) },
    { "Dialog_Active",        TOK_STRING(Actor,dialog_states[ACT_ACTIVE], 0) },
    { "Dialog_Alerted",        TOK_STRING(Actor,dialog_states[ACT_ALERTED], 0) },
    { "Dialog_Completion",    TOK_STRING(Actor,dialog_states[ACT_COMPLETED], 0) },
    { "Dialog_ThankHero",    TOK_STRING(Actor,dialog_states[ACT_THANK], 0) },
    { "ActorReward",    TOK_REDUNDANTNAME | TOK_STRUCT(Actor,reward,ParseStoryReward) },
    { "}",                TOK_END,            0},
    { "", 0, 0 }
};

StaticDefineInt ParseSpawnDefFlags[] =
{
    DEFINE_INT
    { "AllowAddingActors",    SPAWNDEF_ALLOWADDS },
    { "IgnoreRadius",        SPAWNDEF_IGNORE_RADIUS },
    { "AutoStart",            SPAWNDEF_AUTOSTART },
    { "MissionRespawn",        SPAWNDEF_MISSIONRESPAWN },
    { "CloneGroup",            SPAWNDEF_CLONEGROUP },
    { "NeighborhoodDefined",    SPAWNDEF_NEIGHBORHOODDEFINED },
    DEFINE_END
};

// The Ally Group the encounter was designed for
StaticDefineInt ParseEncounterAlliance[] =
{
    DEFINE_INT
    {"Default",                ENC_UNINIT},
    {"Hero",                ENC_FOR_HERO},
    {"Villain",                ENC_FOR_VILLAIN},
    DEFINE_END
};

StaticDefineInt ParseMissionTeamOverride[] =
{
    DEFINE_INT
    {"None",            MO_NONE},
    {"Player",            MO_PLAYER},
    {"Mission",            MO_MISSION},
    DEFINE_END
};

TokenizerParseInfo ParseSpawnDef[] = {
    { "{",                            TOK_START,        0},
    { "",                            TOK_CURRENTFILE(SpawnDef,fileName) },
    { "LogicalName",                TOK_NO_TRANSLATE | TOK_STRING(SpawnDef,logicalName, 0) },
    { "EncounterSpawn",                TOK_NO_TRANSLATE | TOK_STRING(SpawnDef,spawntype, 0) },
    { "Dialog",                        TOK_FILENAME(SpawnDef,dialogfile, 0) },
    { "Deprecated",                    TOK_BOOLFLAG(SpawnDef,deprecated, 0) },
    { "SpawnRadius",                TOK_INT(SpawnDef,spawnradius, -1) },
    { "VillainMinLevel",            TOK_INT(SpawnDef,villainminlevel, 0) },
    { "VillainMaxLevel",            TOK_INT(SpawnDef,villainmaxlevel, 0) },
    { "MinTeamSize",                TOK_INT(SpawnDef,minteamsize, 1) },
    { "MaxTeamSize",                TOK_INT(SpawnDef,maxteamsize, MAX_TEAM_MEMBERS) },
    { "RespawnTimer",                TOK_INT(SpawnDef,respawnTimer, -1) },
    { "Flags",                        TOK_FLAGS(SpawnDef,flags,0), ParseSpawnDefFlags }, 

    { "Actor",                        TOK_STRUCT(SpawnDef,actors,    ParseActor) },
    { "Script",                        TOK_STRUCT(SpawnDef,scripts, ParseScriptDef) },
    { "DialogDef",                    TOK_STRUCT(SpawnDef, dialogDefs, ParseDialogDef) },

    { "EncounterComplete",            TOK_STRUCT(SpawnDef,encounterComplete,ParseStoryReward) },

    // mission map only
    { "MissionPlacement",            TOK_INT(SpawnDef,missionplace,    MISSION_ANY),        ParseMissionPlaceEnum },
    { "MissionCount",                TOK_INT(SpawnDef,missioncount,    1) },
    { "MissionUncounted",            TOK_BOOLFLAG(SpawnDef,missionuncounted, 0) },
    { "CreateOnObjectiveComplete",    TOK_NO_TRANSLATE | TOK_STRINGARRAY(SpawnDef,createOnObjectiveComplete) },
    { "ActivateOnObjectiveComplete",TOK_NO_TRANSLATE | TOK_STRINGARRAY(SpawnDef,activateOnObjectiveComplete) },
    { "Objective",                    TOK_STRUCT(SpawnDef,objective,ParseMissionObjective) },
    { "PlayerCreatedDetailType",    TOK_INT(SpawnDef, playerCreatedDetailType, 0) },
    //Cut Scene 
    { "CutScene",                    TOK_STRUCT(SpawnDef,cutSceneDefs, ParseCutScene) },

    { "EncounterAlliance",            TOK_INT(SpawnDef,encounterAllyGroup,    ENC_UNINIT), ParseEncounterAlliance },
    { "MissionTeamOverride",        TOK_INT(SpawnDef, override,    0), ParseMissionTeamOverride},

    { "MissionEncounter",            TOK_INT(SpawnDef,oldfield, 0) },        // DEPRECATED - was used to determine how spawndef was verified
    { "CVGIndex",                    TOK_INT(SpawnDef,CVGIdx,0)    },
    { "CVGIndex2",                    TOK_INT(SpawnDef,CVGIdx2, 0) },
    { "}",                            TOK_END,            0},
    SCRIPTVARS_STD_PARSE(SpawnDef)
    { "", 0, 0 }
};

TokenizerParseInfo ParseSpawnDefList[] = {
    { "SpawnDef",        TOK_STRUCT(SpawnDefList, spawndefs,    ParseSpawnDef) },
    { "", 0, 0 }
};

// dialog files
TokenizerParseInfo ParseDialogFile[] = {
    { "{",                TOK_START,        0},
    { "",                TOK_CURRENTFILE(DialogFile,name) },
    { "}",                TOK_END,            0},
    SCRIPTVARS_STD_PARSE(DialogFile)
    { "", 0, 0 }
};

TokenizerParseInfo ParseDialogFileList[] = {
    { "Dialog",            TOK_STRUCT(DialogFileList,dialogfiles,ParseDialogFile) },
    { "", 0, 0 }
};

SHARED_MEMORY SpawnDefList g_spawndeflist;
SHARED_MEMORY DialogFileList g_dialogfilelist;
extern Array hashStackSpawnAreaDefs;    // entry to villain .dta system

void DoGroupZDrops(EncounterGroup* group);
void DoZDrops(EncounterPoint *point);

static int CorrectSpawnRadius(int spawnradius)
{
    if (spawnradius > 0) return spawnradius;
    return EG_MAX_SPAWN_RADIUS;
}

static int CorrectVillainRadius(int villainradius)
{
    if (villainradius > 0) return villainradius;
    return EG_VILLAIN_RADIUS;
}

// *********************************************************************************
//  Running scripts
// *********************************************************************************

static int VerifySpawnDefScripts(const SpawnDef* spawndef)
{
    int i, n, ret = 1;

    // Verify that saUtilPreprocessScripts() did its job
    devassert(saUtilVerifyCleanFileName(spawndef->fileName));
    devassert(saUtilVerifyCleanFileName(spawndef->dialogfile));

    n = eaSize(&spawndef->scripts);
    for (i = 0; i < n; i++)
    {
        const char* scriptName = spawndef->scripts[i]->scriptname;
        ScriptFunction* function = ScriptFindFunction(scriptName);
        if (!function || function->type != SCRIPT_ENCOUNTER)
        {
            ErrorFilenamef(spawndef->fileName, "Spawndef %s called for script that doesn't exist or is wrong type: %s",
                spawndef->fileName, scriptName);
            ret = 0;
        }

        devassert(saUtilVerifyCleanFileName(spawndef->scripts[i]->filename));
    }
    return ret;
}

// lots of checks on consistency of spawndef file, assert on error
int VerifySpawnDef(const SpawnDef* spawndef, int inmission)
{
    int j, k;
    int ret = 1;

    if (!VerifySpawnDefScripts(spawndef)) ret = 0;

    if (!inmission && spawndef->objective)
    {
        ErrorFilenamef(spawndef->fileName, "VerifySpawnDef: objective specified for non-mission spawndef %s\n",
            spawndef->fileName);
        ret = 0;
    }

    for (j = 0; j < eaSize(&spawndef->actors); j++)
    {
        const Actor* actor = spawndef->actors[j];

        // verify that we have a correct encounter point for each ent
        if (actor->number > eaiSize(&actor->locations))
        {
            ErrorFilenamef(spawndef->fileName, "VerifySpawnDef: not enough locations given for an actor in %s named %s\n",
                spawndef->fileName, actor->displayname);
            ret = 0;
        }
        for (k = 0; k < eaiSize(&actor->locations); k++)
        {
            if (actor->locations[k] < 0 || actor->locations[k] >= EG_MAX_ENCOUNTER_POSITIONS)
            {
                ErrorFilenamef(spawndef->fileName, "VerifySpawnDef: invalid location %i for an actor in %s named %s\n",
                    actor->locations[k], spawndef->fileName, actor->displayname);
                ret = 0;
            }
        }
    } // for actor
    
    if(spawndef->dialogDefs)
    {
        DialogDefs_Verify(spawndef->fileName, spawndef->dialogDefs);
    }

    return ret;
}

static bool VerifyAllSpawnDefs(TokenizerParseInfo tpi[], void* structptr)
{
    int i;
    bool spawndefsok = true;

    saUtilPreprocessScripts(tpi, structptr);

    for (i = 0; i < eaSize(&g_spawndeflist.spawndefs); i++) {
        spawndefsok &= VerifySpawnDef(g_spawndeflist.spawndefs[i], 0);
    }
    return spawndefsok;
}

// we require an exact filename (passed through saUtilCleanFilename)
static ScriptVarsScope* DialogScope(const char* filename)
{
    int i, n;
    n = eaSize(&g_dialogfilelist.dialogfiles);
    for (i = 0; i < n; i++)
    {
        if (strcmp(g_dialogfilelist.dialogfiles[i]->name, filename)) continue;
        return &g_dialogfilelist.dialogfiles[i]->vs;
    }
    return NULL;
}



static void AttachDialogScopes(SpawnDef* spawndef)
{
    if (spawndef->dialogfile)
    {
        spawndef->vs.higherscope = DialogScope(spawndef->dialogfile);
        if (!spawndef->vs.higherscope)
        {
            ErrorFilenamef(spawndef->fileName, "Encounter dialog file %s missing or incorrect", spawndef->dialogfile);
        }
    }
}

void SpawnDef_ParseVisionPhaseNames(SpawnDef *spawn)
{
    int j;

    for (j = eaSize(&spawn->actors) - 1; j >= 0; j--)
    {
        Actor *a = cpp_const_cast(Actor*)(spawn->actors[j]);

        if (a && a->bitfieldVisionPhaseRawStrings && eaSize(&a->bitfieldVisionPhaseRawStrings))
        {
            int actorPhaseNameIndex, globalPhaseNameIndex;
            char *visionPhaseName;

            for (actorPhaseNameIndex = eaSize(&a->bitfieldVisionPhaseRawStrings) - 1; actorPhaseNameIndex >= 0; actorPhaseNameIndex--)
            {
                globalPhaseNameIndex = 0;
                visionPhaseName = a->bitfieldVisionPhaseRawStrings[actorPhaseNameIndex];

                while (globalPhaseNameIndex < eaSize(&g_visionPhaseNames.visionPhases))
                {
                    if (!stricmp(visionPhaseName, g_visionPhaseNames.visionPhases[globalPhaseNameIndex]))
                    {
                        break;
                    }
                    globalPhaseNameIndex++;
                }

                if (globalPhaseNameIndex == eaSize(&g_visionPhaseNames.visionPhases))
                {
                    ErrorFilenamef(spawn->fileName, "Vision Phase '%s' not found in visionPhases.def!", visionPhaseName);
                }
                else if (globalPhaseNameIndex >= 0 && globalPhaseNameIndex < 32)
                {
                    if (globalPhaseNameIndex == 0)
                    {
                        // NoPrime
                        a->bitfieldVisionPhases &= 0xfffffffe;
                    }
                    else
                    {
                        a->bitfieldVisionPhases |= 1 << globalPhaseNameIndex;
                    }
                }
            }
        }

        if (a && a->exclusiveVisionPhaseRawString)
        {
            int phaseNameIndex = 0;

            while (phaseNameIndex < eaSize(&g_exclusiveVisionPhaseNames.visionPhases))
            {
                if (!stricmp(a->exclusiveVisionPhaseRawString, g_exclusiveVisionPhaseNames.visionPhases[phaseNameIndex]))
                {
                    break;
                }
                phaseNameIndex++;
            }

            if (phaseNameIndex == eaSize(&g_exclusiveVisionPhaseNames.visionPhases))
            {
                ErrorFilenamef(spawn->fileName, "Exclusive Vision Phase '%s' not found in visionPhasesExclusive.def!", a->exclusiveVisionPhaseRawString);
            }
            else
            {
                a->exclusiveVisionPhase = phaseNameIndex;
            }
        }
    }
}

// set up the spawndefs to fail over to dialog vars scope
static bool LoadSpawnDefListPostprocess(TokenizerParseInfo tpi[], SpawnDefList* slist, bool shared_memory)
{
    int i, n = eaSize(&slist->spawndefs);
    for (i = 0; i < n; i++)
    {
        SpawnDef *spawn = (SpawnDef*)slist->spawndefs[i];
        AttachDialogScopes(spawn);
        SpawnDef_ParseVisionPhaseNames(spawn);
    }

    return true;
}

static int SpawnDefReattachDialog(DialogFile* dialog)
{
    int i, n = eaSize(&g_spawndeflist.spawndefs);
    for (i = 0; i < n; i++)
    {
        SpawnDef* spawndef = (SpawnDef*)g_spawndeflist.spawndefs[i];
        if (spawndef->dialogfile && !stricmp(dialog->name, spawndef->dialogfile))
            AttachDialogScopes(spawndef);
    }
    return 1;
}

static StashTable    g_spawndefbyfilename = 0;

static void AddToSpawnDefHash(const SpawnDef* spawndef)
{
    stashAddPointerConst(g_spawndefbyfilename, spawndef->fileName, spawndef, false);
}

static void CreateSpawnDefHash(void)
{
    int i;
    int size = eaSize(&g_spawndeflist.spawndefs);
    if (g_spawndefbyfilename) 
        stashTableDestroy(g_spawndefbyfilename);

    // Note that this hash table now uses a shallow copy of the keyname,
    // so we need to be sure the pointer passed in is stored somewhere
    g_spawndefbyfilename = stashTableCreateWithStringKeys(size, StashDefault);
    for (i = 0; i < size; i++)
        AddToSpawnDefHash(g_spawndeflist.spawndefs[i]);
}

static void RemoveFromSpawnDefHash(const char* filename)
{
    char buf[MAX_PATH];
    saUtilCleanFileName(buf, (char*)filename);
    if (stashFindPointerReturnPointer(g_spawndefbyfilename, buf))
        stashRemovePointer(g_spawndefbyfilename, buf, NULL);
}

// returns NULL on error
const SpawnDef* SpawnDefFromFilename(const char* filename)
{
    char buf[MAX_PATH];
    saUtilCleanFileName(buf, filename);
    return stashFindPointerReturnPointer(g_spawndefbyfilename, buf);
}

// Finds any active encounters that are currently using the spawndef and restarts them
static void EncounterGroupRefreshSpawndef(SpawnDef* spawndef)
{
    int i;
    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];
        if (group->active && group->active->spawndef && group->active->spawndef->fileName && !stricmp(spawndef->fileName, group->active->spawndef->fileName))
        {
            if (EncounterNewSpawnDef(group, spawndef, NULL))
                EncounterSpawn(group);
        }
    }
}

// Search for a recently reloaded contact def that still needs to be preprocessed
// Different search because the def has not yet been processed
static SpawnDef* SpawnDefFindUnprocessed(const char* relpath)
{
    char cleanedName[MAX_PATH];
    int i, n = eaSize(&g_spawndeflist.spawndefs);
    strcpy(cleanedName, strstri((char*)relpath, SCRIPTS_DIR));
    forwardSlashes(_strupr(cleanedName));
    for (i = 0; i < n; i++)
        if (!stricmp(cleanedName, g_spawndeflist.spawndefs[i]->fileName))
            return (SpawnDef*)g_spawndeflist.spawndefs[i];
    return NULL;
}

// Reloads the latest version of a def, finds it, then processes
// MAKE SURE to do all the pre/post/etc stuff here that gets done during regular loading
static void SpawnDefReloadCallback(const char* relpath, int when)
{
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);

    RemoveFromSpawnDefHash(relpath);

    if (ParserReloadFile(relpath, ParseSpawnDefList, sizeof(SpawnDefList), (void*)&g_spawndeflist, NULL, NULL))
    {
        SpawnDef* spawndef = SpawnDefFindUnprocessed(relpath);
        saUtilPreprocessScripts(ParseSpawnDef, spawndef);
        VerifySpawnDef(spawndef, 0);
        AttachDialogScopes(spawndef);
        EncounterGroupRefreshSpawndef(spawndef);
        AddToSpawnDefHash(spawndef);
        SpawnDef_ParseVisionPhaseNames(spawndef);
    }
    else
    {
        Errorf("Error reloading spawndef: %s\n", relpath);
    }
}

// Search for a recently reloaded dialog def that still needs to be preprocessed
// Different search because the def has not yet been processed
static DialogFile* DialogFileFindUnprocessed(const char* relpath)
{
    char cleanedName[MAX_PATH];
    int i, n = eaSize(&g_dialogfilelist.dialogfiles);
    strcpy(cleanedName, strstri((char*)relpath, SCRIPTS_DIR));
    forwardSlashes(_strupr(cleanedName));
    for (i = 0; i < n; i++)
        if (!stricmp(cleanedName, g_dialogfilelist.dialogfiles[i]->name))
            return g_dialogfilelist.dialogfiles[i];
    return NULL;
}

// Reloads the latest version of a def, finds it, then processes
// MAKE SURE to do all the pre/post/etc stuff here that gets done during regular loading
static void SpawnDefDialogReloadCallback(const char* relpath, int when)
{
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    if (ParserReloadFile(relpath, ParseDialogFileList, sizeof(DialogFileList), (void*)&g_dialogfilelist, NULL, NULL))
    {
        DialogFile* dialog = DialogFileFindUnprocessed(relpath);
        saUtilPreprocessScripts(ParseDialogFile, dialog);
        SpawnDefReattachDialog(dialog);
    }
    else
    {
        Errorf("Error reloading dialog: %s\n", relpath);
    }
}

static void SpawnDefSetupCallbacks()
{
    if (!isDevelopmentMode())
        return;
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "scripts.loc/*.dialog2", SpawnDefDialogReloadCallback);
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "scripts.loc/*.spawndef", SpawnDefReloadCallback);
}

// called once on startup
void EncounterPreload(void)
{
    int flags = PARSER_SERVERONLY;
    if (g_spawndeflist.spawndefs) // Reload!
        flags |= PARSER_FORCEREBUILD;
    ParserUnload(ParseDialogFileList, &g_dialogfilelist, sizeof(g_dialogfilelist));
    ParserUnload(ParseSpawnDefList, &g_spawndeflist, sizeof(g_spawndeflist));

    loadstart_printf("Loading spawn definitions.. ");
    if (!ParserLoadFilesShared("SM_dialog.bin", SCRIPTS_DIR, ".dialog2", "dialog.bin",
        flags, ParseDialogFileList, &g_dialogfilelist, sizeof(g_dialogfilelist), NULL, NULL, saUtilPreprocessScripts, NULL, NULL))
    {
        printf("EncounterPreload: Error loading encounter dialog files\n");
    }
    if (!ParserLoadFilesShared("SM_spawndefs.bin", SCRIPTS_DIR, ".spawndef", "spawndefs.bin",
        flags, ParseSpawnDefList, &g_spawndeflist, sizeof(g_spawndeflist), NULL, NULL, VerifyAllSpawnDefs, NULL, LoadSpawnDefListPostprocess))
    {
        printf("EncounterPreload: Error loading spawn definitions\n");
    }
    CreateSpawnDefHash();
    LoadNictusOptions();
    MissionEncounterVerify(); // let mission system verify referenced spawndefs
    SpawnDefSetupCallbacks();
    loadend_printf("%i spawn definitions", eaSize(&g_spawndeflist.spawndefs));
}

const SpawnDef* EncounterGetMyResponsibleSpawnDef(Entity* ent)
{
    if(!ent || !ent->encounterInfo)
        return NULL;

    if(OnMissionMap())
        return ent->encounterInfo->parentGroup->missionspawn;
    else
        return ent->encounterInfo->parentGroup->active->spawndef;
}

// *********************************************************************************
//  Encounter group debug strings
// *********************************************************************************
// strings that are used in EncounterGroups.  only make one duplicate of them, they aren't
// destroyed individually.  They can all be thrown away with GroupStringClearAll

cStashTable    g_groupstringhashes = 0;
StringTable g_groupstrings        = 0;

static void GroupStringInit()
{
    if (g_groupstrings) return;
    g_groupstrings = createStringTable();
    initStringTable(g_groupstrings, 10000);
    g_groupstringhashes = stashTableCreateWithStringKeys(100, StashDeepCopyKeys | StashCaseSensitive );
}

static const char* GroupStringAdd(const char* src)
{
    char* str;

    if (!g_groupstrings) GroupStringInit();
    stashFindPointerConst( g_groupstringhashes, src, &str );
    if (str) return str;
    str = strTableAddString(g_groupstrings, src);
    stashAddPointerConst(g_groupstringhashes, src, str, false);
    return str;
}

static void GroupStringClearAll()
{
    if (!g_groupstrings) return;
    destroyStringTable(g_groupstrings);
    g_groupstrings = 0;
    stashTableDestroyConst(g_groupstringhashes);
    g_groupstringhashes = 0;
}

// *********************************************************************************
//  Encounter grid
// *********************************************************************************

Grid        g_encountergrid        = {0};

#define EG_GRID_DXYZ    125
#define EG_GRID_NODETYPE  3
static void EncounterGridAdd(EncounterGroup* group)
{
    int index;
    Vec3 dv = { EG_GRID_DXYZ, EG_GRID_DXYZ, EG_GRID_DXYZ };
    Vec3 min, max;

    subVec3(group->mat[3],dv,min);
    addVec3(group->mat[3],dv,max);
    gridAdd(&g_encountergrid, group, min, max, EG_GRID_NODETYPE, &group->grid_idx);

    // Add to the mega-grid as well
    index = g_encountergroups.size - 1;    // Index in array of the group
    if (group->encounterGridNode)
        destroyMegaGridNode(group->encounterGridNode);
    group->encounterGridNode = createMegaGridNode((void*)(intptr_t)index, EG_MAX_SPAWN_RADIUS, 1);
    mgUpdate(3, group->encounterGridNode, group->mat[3]);
}

static void EncounterGridClear()
{
    int i;
    gridFreeAll(&g_encountergrid);
    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];
        mgRemove(group->encounterGridNode);
        destroyMegaGridNode(group->encounterGridNode);
    }
}

// Don't need this right now - see comments about "commenting out" code in EncountersWithinDistance(...)
//static int __cdecl testPointer(const void** left, const void** right)
//{
//    if (*left < *right) return -1;
//    if (*right < *left) return 1;
//    return 0;
//}

// Faster Search using the mega array, only finds nodes within a max distance of EG_MAX_SPAWN_RADIUS
// Added this to help with faster spawn checking, uses the new grid instead of the old
// DO NOT use this for distances greater than EG_MAX_SPAWN_RADIUS
// If you want to use this for larger distances, create another megagrid
static int EncountersWithinDistanceMegaGrid(Vec3 pos, F32 distance, EncounterGroup** result)
{
    EncounterGroup* group;
    int* indices[ENT_FIND_LIMIT];
    EncounterGroup* lastgroup = NULL;
    int gridCount;
    int count = 0;
    int i;
    F32 d2 = distance * distance;

    // use grid to narrow search
    i = distance - EG_GRID_DXYZ;
    if (i < 0) i = 0;
    gridCount = mgGetNodesInRange(3, pos, (void **)indices, 0);

    // verify distance
    count = 0;
    for (i = 0; i < gridCount; i++)
    {
        group = g_encountergroups.storage[(int)(intptr_t)indices[i]];
        if (distance3Squared(group->mat[3], pos) <= d2)
        {
            result[count++] = group;
        }
    }
    return count;
}

// A better solution to this whole mess is to simply skip use of the grid system if the radius handed to EncountersWithinDistance(...) is
// greater than 5000.  In a case like that, we're going to hit everything anyway, which is the underlying intention in the first place.
// Therefore, we'd just run through g_encountergroups, doing the "Verify distance" step as we go and hand the results back to the caller.

static EncounterGroup *CBgroups[ENT_FIND_LIMIT];
static int CBgroupcount;
static char flagArray[ENT_FIND_LIMIT];

// This routine, and more importantly, the call that uses it, is not thread safe.  Since there's no way to get additional
// data in here, we can't get the above three data items in from local variables in EncountersWithinDistance(...).  Two
// possible solutions exist:
// 1. Enclose most all of EncountersWithinDistance(...) in a critical section
// 2. Declare the above three variables as thread local
static int gridFindCB(void *param)
{
    EncounterGroup *group;

    group = (EncounterGroup *) param;
    devassertmsg(CBgroupcount < ENT_FIND_LIMIT, "Go find David G. and tell him ENT_FIND_LIMIT is too small");
    if (flagArray[group->ordinal] == 0)
    {
        CBgroups[CBgroupcount++] = group;
        flagArray[group->ordinal] = 1;
    }
    return 0;
}

// Uses the original grid to check for encounters within a distance of a position
static int EncountersWithinDistance(const Vec3 pos, F32 distance, EncounterGroup** result)
{
    // For now, I'm just going to comment out the old code.  That way we have a fast fallback if something goes catastrophically wrong with the new.
    //EncounterGroup *groups[ENT_FIND_LIMIT];
    EncounterGroup *lastgroup = NULL;
    //int gridCount;
    //int origGridCount;
    int count = 0;
    int i;
    F32 d2 = distance * distance;
    Vec3 ncpos; // solve const correctness

    copyVec3(pos, ncpos);
    // use grid to narrow search
    i = distance - EG_GRID_DXYZ;
    if (i < 0)
        i = 0;
    //gridCount = gridFindObjectsInSphere(&g_encountergrid, (void **)groups, ARRAY_SIZE(groups), pos, i);

    CBgroupcount = 0;
    devassertmsg(g_encountergroups.size < ENT_FIND_LIMIT, "Go find David G. and tell him ENT_FIND_LIMIT is too small");
    memset(flagArray, 0, ENT_FIND_LIMIT);
    // See comment above at gridFindCB(...) rergarding thread safety (or lack thereof) for this function call.
    gridFindObjectsInSphereCB(&g_encountergrid, gridFindCB, ncpos, i);

    // eliminate dupes
    //qsort(groups, gridCount, sizeof(EncounterGroup*), testPointer);
    //count = 0;
    //lastgroup = NULL;
    //origGridCount = gridCount;
    //for (i = 0; i < gridCount; i++)
    //{
    //    if (lastgroup != groups[i])
    //    {
    //        lastgroup = groups[i];
    //        groups[count++] = groups[i];
    //    }
    //}
    //gridCount = count;

    //qsort(CBgroups, CBgroupcount, sizeof(EncounterGroup*), testPointer);

    //if (origGridCount < ENT_FIND_LIMIT)
    //{
    //    if (gridCount != CBgroupcount)
    //    {
    //        log_printf("encounterGroupSearch", "EncounterGroupSearch - count error: %d != %d\n", gridCount, CBgroupcount);
    //    }
    //    else
    //    {
    //        int j = 0;

    //        for (i = 0; i < gridCount; i++)
    //        {
    //            if (groups[i] != CBgroups[i])
    //            {
    //                if (++j < 10)
    //                {
    //                    log_printf("encounterGroupSearch", "EncounterGroupSearch - compare error: %d != %d\n", groups[i], CBgroups[i]);
    //                }
    //            }
    //        }
    //        if (j > 0)
    //        {
    //            log_printf("encounterGroupSearch", "EncounterGroupSearch - %d mismatches\n", j);
    //        }
    //    }
    //}

    // verify distance
    //count = 0;
    //for (i = 0; i < gridCount; i++)
    //{
    //    if (distance3Squared(groups[i]->mat[3], pos) <= d2)
    //    {
    //        groups[count++] = groups[i];
    //    }
    //}
    //gridCount = count;

    // verify distance
    count = 0;
    for (i = 0; i < CBgroupcount; i++)
    {
        if (distance3Squared(CBgroups[i]->mat[3], pos) <= d2)
        {
            CBgroups[count++] = CBgroups[i];
        }
    }
    // Don't need this
    //CBgroupcount = count;

    // copy result
    if (result)
    {
        for (i = 0; i < count; i++)
            result[i] = CBgroups[i];
    }

    return count;
}

// returns 1 if any encounters are active within given circle
static int EncountersRunningInRadius(EncounterGroup *group, Vec3 pos, F32 distance)
{
    EncounterGroup* groups[ENT_FIND_LIMIT];
    int count, i;
    int found = 0;

    count = EncountersWithinDistance(pos, distance, groups);
    for (i = 0; i < count; i++)
    {
        if (EGSTATE_IS_RUNNING(groups[i]->state) && groups[i]->active 
            && !groups[i]->ignoreradius && !(groups[i]->active->spawndef->flags & SPAWNDEF_IGNORE_RADIUS)
            && groups[i] != group)
        {
            found = 1;
            break;
        }
    }
    return found;
}

// *********************************************************************************
//  Encounter per-map coverage information
// *********************************************************************************

// we record if a layout or a villain group appears
static StashTable    g_encounterlayouts = 0;
static StashTable    g_encountervillains = 0;
static StashTable    g_villainsbyspawndef = 0;    // memo: villain group that is produced by this spawndef

static void InitCoverageHashTables(void)
{
    if (g_encounterlayouts) stashTableDestroy(g_encounterlayouts);
    if (g_encountervillains) stashTableDestroy(g_encountervillains);
    if (g_villainsbyspawndef) stashTableDestroy(g_villainsbyspawndef);
    g_encounterlayouts = stashTableCreateWithStringKeys(10, StashDeepCopyKeys);
    g_encountervillains = stashTableCreateWithStringKeys(10, StashDeepCopyKeys);
    g_villainsbyspawndef = stashTableCreateWithStringKeys(50, StashDeepCopyKeys);
}

//Accurate now
static void AddVillainsFromSpawnDef(const SpawnDef* spawndef)
{
    int i, n;
    ScriptVarsTable defvars = {0};

    //If you haven't already done this spawndef
    if ( stashFindElement(g_villainsbyspawndef, spawndef->fileName, NULL) )
        return;

    ScriptVarsTablePushScope(&defvars, &spawndef->vs);
    ScriptVarsTableSetSeed(&defvars, (unsigned int)time(NULL));

    //Add every villain group represented in this spawndef to g_encountervillains, the list of all villains in CanSpawns in this Map
    n = eaSize(&spawndef->actors);
    for (i = 0; i < n; i++)
    {
        const Actor * actor = spawndef->actors[i];
        const char* villainGroup = 0;

        if (actor->villain)
        {
            const char* villain = ScriptVarsTableLookup(&defvars, actor->villain);
            const VillainDef* def = villainFindByName(villain);

            if (def)
                villainGroup = villainGroupGetName(def->group);
            else if(villain && strstr(villain, DOPPEL_NAME))
                villainGroup = DOPPEL_GROUP_NAME;
            else
                ErrorFilenamef(saUtilCurrentMapFile(), "Error: %s Villain doesn't exist ( actor %i of spawnDef %s)\n", actor->villain, i, spawndef->fileName );
        }
        else if (actor->villaingroup)
        {
            villainGroup = ScriptVarsTableLookup(&defvars, actor->villaingroup);
        }

        if ( villainGroup && !stashFindElement( g_encountervillains, villainGroup, NULL) ) 
            stashAddInt(g_encountervillains, villainGroup, 1, false);
    }

    //Never check this spawndef again
    stashAddInt(g_villainsbyspawndef, spawndef->fileName, 1, false);
}

static void VerifyEncounterCoverage(void)
{
    const char** requiredLayouts;
    const char** requiredVillains;
    int i, n = 0;

    requiredLayouts = MapSpecEncounterLayouts(saUtilCurrentMapFile());
    if (requiredLayouts)
        n = eaSize(&requiredLayouts);
    for (i = 0; i < n; i++)
    {
        if (!stashFindPointerReturnPointer(g_encounterlayouts, requiredLayouts[i]))
        {
            ErrorFilenamef(saUtilCurrentMapFile(), "Map required to have %s encounter layouts, but does not", requiredLayouts[i]);
        }
    }

    n = 0;
    requiredVillains = MapSpecVillainGroups(saUtilCurrentMapFile());
    if (requiredVillains)
        n = eaSize(&requiredVillains);
    for (i = 0; i < n; i++)
    {
        if (!stashFindPointerReturnPointer(g_encountervillains, requiredVillains[i]))
        {
            ErrorFilenamef(saUtilCurrentMapFile(), "Map required to have %s villain group, but does not", requiredVillains[i]);
        }
    }
}


static ClientLink* pec_client = NULL;
static FILE* pec_file = NULL;

static int PEC_PrintHashKey(StashElement el)
{
    conPrintf(pec_client, "  %s\n", stashElementGetStringKey(el));
    if (pec_file)
    {
        char buf[200];
        sprintf(buf, "%s, ", stashElementGetStringKey(el));
        fwrite(buf, 1, strlen(buf), pec_file);
    }
    return 1;
}

// print the list of layouts and villains
void DebugPrintEncounterCoverage(ClientLink* client)
{
    // layouts
    pec_client = client;
    if (isDevelopmentMode())
    {
        pec_file = fopen("c:\\encountercoverage.txt", "wb");
        fwrite("EncounterLayouts ", 1, strlen("EncounterLayouts "), pec_file);
    }
    conPrintf(client, "Encounter layouts on this map:\n");
    stashForEachElement(g_encounterlayouts, PEC_PrintHashKey);

    // villains
    if (pec_file)
        fwrite("\r\nVillainGroups ", 1, strlen("\r\nVillainGroups "), pec_file);
    conPrintf(client, "Villain groups on this map:\n");
    stashForEachElement(g_encountervillains, PEC_PrintHashKey);

    if (pec_file)
    {
        fwrite("\r\n", 1, strlen("\r\n"), pec_file);
        fclose(pec_file);
        pec_file = 0;
    }
}

// *********************************************************************************
//  Encounter groups and geometry loading
// *********************************************************************************

int EncounterGetMatchingPoint(EncounterGroup* group, const char* spawntype)
{
    EncounterPoint* point;
    int* resultlist;
    int i, result;

    // iterate through children looking for those with matching type
    eaiCreate(&resultlist);
    for (i = 0; i < group->children.size; i++)
    {
        point = group->children.storage[i];
        if (!stricmp(point->name, spawntype))
        {
            eaiPush(&resultlist, i);
        }
    }

    // randomly select one
    result = -1;
    if (eaiSize(&resultlist))
        result = resultlist[rand() % eaiSize(&resultlist)];
    eaiDestroy(&resultlist);
    return result;
}

// coordinate groupLoader and spawnLoader
static EncounterGroup g_groupProperties = {0};
static EncounterGroup *g_groupInst = 0;
static EncounterPoint *g_pointInst = 0;
static int g_honorGroup = 0;
static int g_ignoreRadiusForMe = 0; //I ignore radius, but others near me don't ignore me

// utils to deal with encounter group properties
static void EncounterGroupClear(EncounterGroup* group)
{
    memset(group, 0, sizeof(EncounterGroup));
}

static void EncounterGroupCopy(EncounterGroup* to, EncounterGroup* from)
{
    memcpy(to, from, sizeof(EncounterGroup));
}

static void EncounterGroupAdd(EncounterGroup* group)
{
    // Use the index into g_encountergroups as an ordinal for this group.
    group->ordinal = g_encountergroups.size;
    arrayPushBack(&g_encountergroups, group);
    EncounterGridAdd(group);
}

// Prevent double deletion of encounter points
static void EncounterClonePoints(EncounterGroup* src, EncounterGroup* dest)
{
    int i;
    memset(&dest->children, 0, sizeof(Array));
    arrayCopy(&src->children, &dest->children, NULL);
    for (i = 0; i < src->children.size; i++)
    {
        dest->children.storage[i] = EncounterPointAlloc();
        memcpy(dest->children.storage[i], src->children.storage[i], sizeof(EncounterPoint));
    }
}

// Does a lot of stuff from EncounterCleanup()
// except doesn't complete objective and doesn't free active, sets pointer to NULL instead
// also doesn't call EncounterCalculateAutostart(), because that appears to be there for a restart, and this isn't a restart
EncounterGroup *EncounterGroupClone(EncounterGroup* group)
{
    EncounterGroup *retval = EncounterGroupAlloc();
    EncounterGroupCopy(retval, group);
    EncounterClonePoints(group, retval);
    retval->objectiveInfo = NULL;        // prevents clone from being able to complete original's objectives
    retval->encounterGridNode = NULL;    // prevents deletion of original grid node
    retval->active = NULL;                // clear this out, a new one should get built when the clone is spawned into
    retval->state = EG_ASLEEP;
    retval->noautoclean = 0;
    EncounterGroupAdd(retval);
    return retval;
}

// better to set the group mat to the position of the first child,
//     sometimes the center of the mat is not related to actual to any
//   reasonable position
static void EncounterGroupSetMat(EncounterGroup* group, Mat4 mat)
{
    copyMat4(mat, group->mat);
    group->intray = groupFindInside(group->mat[3], FINDINSIDE_TRAYONLY,0,0)? 1: 0;
}

// don't pay attention to how EncounterSpawn geom is grouped into EncounterGroup
// - this causes encounters to try and maintain a clear radius around themselves,
// and makes each EncounterSpawn an independent group
static int IgnoreEncounterGrouping()
{
    if (g_encounterForceIgnoreGroup >= 0) return g_encounterForceIgnoreGroup;
    if (!EM_IGNOREGROUPS) return 0;
    return !g_honorGroup;
}

static int UseVillainRadius()
{
    return EM_USEVILLAINRADIUS && !g_ignoreRadiusForMe;
}

// callback used by EncounterFindInfo
const SpawnDef ***g_findinfo_infos;
static int FindInfoIterate(StashElement element)
{
    PropertyEnt* property;
    const SpawnDef* spawndef;

    // only look at elements beginning with "CanSpawn"
    if (strnicmp("CanSpawn", stashElementGetStringKey(element), 8/*strlen("CanSpawn")*/)) 
        return 1;
    property = stashElementGetPointer(element);

    // look for a matching encounter to add to array
    spawndef = SpawnDefFromFilename(property->value_str);

    if (!spawndef)
    {
        ErrorFilenamef(saUtilCurrentMapFile(), "FindInfoIterate: can't find spawndef %s specified in EncounterGroup %s\n", property->value_str, g_pointInst->geoname);
    }
    else
    {
        AddVillainsFromSpawnDef( spawndef );
        eaPushConst(g_findinfo_infos, spawndef);
    }
    return 1;    // continue iterating
}

// add any encounters matching CanSpawn properties to array
static int EncounterFindInfo(const SpawnDef ***infos, StashTable properties)
{
    // iterate over properties
    g_findinfo_infos = infos;
    stashForEachElement(properties, FindInfoIterate);
    return eaSize(infos);
}

static Vec3 up = {0.0, 1.0, 0.0};
static int positionLoader(GroupDefTraverser* traverser)
{
    EncounterPoint* parent = g_pointInst;
    PropertyEnt* positionProp;
    int position;

    // find the group
    if (!traverser->def->properties) {
        if (!traverser->def->child_properties)    // if none of our children have props, don't go further
            return 0;
        return 1;
    }
    stashFindPointer( traverser->def->properties, "EncounterPosition", &positionProp );

    // if we have a position
    if (positionProp)
    {
        Mat3 mat;
        Vec3 pyr;

        // get the right id
        position = atoi(positionProp->value_str);
        if (position < 0 || position >= EG_MAX_ENCOUNTER_POSITIONS)
        {
            ErrorFilenamef(saUtilCurrentMapFile(), "EncounterPositionLoader: invalid position at %f, %f, %f\n",
                traverser->mat[3][0], traverser->mat[3][1], traverser->mat[3][2]);
            return 0;
        }

        // place in parent spawn info
        assert(parent);
        copyVec3(traverser->mat[3], parent->positions[position]);

        // MS: Turn around the generator yaw 180 degrees because all the generator geometries are BACK-FUCKING-WARDS!!!
        copyMat3(traverser->mat, mat);
        scaleVec3(mat[0], -1, mat[0]);
        scaleVec3(mat[2], -1, mat[2]);
        getMat3YPR(mat, pyr);
        copyVec3(pyr, parent->pyrpositions[position]);
        if (dotVec3(mat[1], up) > 0.80)
            parent->allownpcs = 1;

        //Stealth Camera Options for Encounter Positions
        {
            PropertyEnt* extraProp;
            F32 arc = 0;
            F32 speed = 0;
            eEPFlags flags;

            stashFindPointer( traverser->def->properties, "StealthCameraMovement", &extraProp );
            if( extraProp && 0 == stricmp( extraProp->value_str, "Arc" ) )
                flags = STEALTH_CAMERA_ARC;
            else if( extraProp && 0 == stricmp( extraProp->value_str, "Frozen" ) )
                flags = STEALTH_CAMERA_FROZEN;
            else if( extraProp && 0 == stricmp( extraProp->value_str, "ReverseRotate" ) )
                flags = STEALTH_CAMERA_REVERSEROTATE;
            else if( extraProp && 0 == stricmp( extraProp->value_str, "ReverseArc" ) )
                flags = STEALTH_CAMERA_REVERSEARC;
            else //if( extraProp && 0 == stricmp( extraProp->value_str, "Rotate" ) //Default, so no check needed
                flags = STEALTH_CAMERA_ROTATE;

            stashFindPointer( traverser->def->properties, "StealthCameraArc", &extraProp );
            if( extraProp )
                arc = atof(extraProp->value_str);
            else
                arc = 90;  //Default 90 degrees;

            stashFindPointer( traverser->def->properties, "StealthCameraSpeed", &extraProp );
            if( extraProp )
                speed = atof(extraProp->value_str);
            else
                speed = 30.0;  //Degrees per second;

            parent->epFlags[position] = flags;
            parent->stealthCameraArcs[position] = arc;
            parent->stealthCameraSpeeds[position] = speed;
        }

        // Validate the spawn point.
        if(server_state.validate_spawn_points)
        {
            if(server_state.validate_spawn_points & 2 || position != 40)
            {
                if(!beaconGetClosestCombatBeaconVerifier(traverser->mat[3]))
                {
                    Vec3 pos;

                    EncounterDoZDrop(traverser->mat[3], pos);

                    if(!beaconGetClosestCombatBeaconVerifier(pos))
                    {
                        // Add to the bad spawn list.

                        Vec3* newPos = dynArrayAddStruct(    &server_state.bad_spawns,
                            &server_state.bad_spawn_count,
                            &server_state.bad_spawn_max_count);

                        copyVec3(traverser->mat[3], *newPos);
                    }
                }
            }
        }
        return 0; // stop traversing
    }

    if (!traverser->def->child_properties)    // if none of our children have props, don't go further
        return 0;
    return 1; // traverse children
}

// adds the EncounterPoint to an encounter group
//   independentGroup == 1: point gets it's own group derived from g_groupProperties
//   independentGroup == 0: point gets added to the shared g_groupInst group
static void AddEncounterPoint(EncounterPoint* point, int independentGroup, int useVillainRadius, int missiongroup)
{
    DoZDrops(point);
    if (independentGroup)
    {
        EncounterGroup* group = EncounterGroupAlloc();
        EncounterGroupCopy(group, &g_groupProperties);

        group->geoname = GroupStringAdd(point->geoname);

        EncounterGroupSetMat(group, point->mat);
        EncounterGroupAdd(group);
        group->usevillainradius = useVillainRadius;
        arrayPushBack(&group->children, point);
        group->missiongroup = missiongroup;

        MissionProcessEncounterGroup(group);
    }
    else
    {
        if (!g_groupInst)
        {
            g_groupInst = EncounterGroupAlloc();
            EncounterGroupCopy(g_groupInst, &g_groupProperties);
            EncounterGroupSetMat(g_groupInst, point->mat);
            EncounterGroupAdd(g_groupInst);
        }
        g_groupInst->usevillainradius = useVillainRadius;
        arrayPushBack(&g_groupInst->children, point);
        if (point->autospawn) g_groupInst->hasautospawn = 1;
        if (missiongroup) g_groupInst->missiongroup = 1;
    }
}

static int spawnLoader(GroupDefTraverser* traverser)
{
    EncounterPoint* pointInst;
    PropertyEnt* spawnProp;
    PropertyEnt* autospawnProp;
    int missiongroup = 0;

    // find the spawn point
    if (!traverser->def->properties) {
        if (!traverser->def->child_properties)    // if none of our children have props, don't go further
            return 0;
        return 1;
    }
    stashFindPointer( traverser->def->properties, "EncounterSpawn", &spawnProp );
    stashFindPointer( traverser->def->properties, "AutoSpawn", &autospawnProp );

    // if we have a point
    if (spawnProp)
    {
        // initialize our spawn point
        pointInst = EncounterPointAlloc();
        copyMat4(traverser->mat, pointInst->mat);
        pointInst->name = GroupStringAdd(spawnProp->value_str);
        pointInst->geoname = GroupStringAdd(traverser->def->name); // debug only

        // verify that hostage encounters are always in their own group
        if (g_groupProperties.personobjective)
        {
            if (pointInst->name && stricmp(pointInst->name, "PersonObjective"))
            {
                ErrorFilenamef(saUtilCurrentMapFile(), "Hostage group %s contains a %s spawn",
                    g_groupProperties.geoname, pointInst->name);
            }
        }
        else
        {
            if (pointInst->name && !stricmp(pointInst->name, "PersonObjective"))
            {
                ErrorFilenamef(saUtilCurrentMapFile(), "Hostage spawn inside a normal encounter group %s",
                    g_groupProperties.geoname);
            }
        }

        // see if we are a mission only point (special processing)
        if (pointInst->name && !stricmp(pointInst->name, "Mission"))
            missiongroup = 1;

        // record we have this layout available
        stashAddPointer(g_encounterlayouts, pointInst->name, pointInst, false);

        // get the corresponding static info, missiongroup points don't even look at the screwed up info        
        g_pointInst = pointInst;
        pointInst->infos = NULL;
        if (!missiongroup) {
            eaCreateConst(&pointInst->infos);
            EncounterFindInfo(&pointInst->infos, traverser->def->properties);
        }

        // other properties
        if (stashFindPointerReturnPointer(traverser->def->properties, "SquadRenumber"))
        {
            pointInst->squad_renumber = 1;
        }
        if (autospawnProp)
        {
            pointInst->autospawn = 1;
        }

        // load up the children (spawn positions)
        groupProcessDefExContinue(traverser, positionLoader);
        pointInst->patrol = LoadPatrolRoute(traverser, NULL);

        // add to the last encounter group
        AddEncounterPoint(pointInst,
            g_groupProperties.personobjective || (!autospawnProp && IgnoreEncounterGrouping()),
            !autospawnProp && UseVillainRadius(),
            missiongroup);

        return 0; // stop traversing
    }
    if (!traverser->def->child_properties)    // if none of our children have props, don't go further
        return 0;
    return 1; // traverse children
}

// load the encounter groups
static int groupLoader(GroupDefTraverser* traverser)
{
    PropertyEnt* generatorProp;
    PropertyEnt* otherProp;

    // find the group
    if (!traverser->def->properties) {
        if (!traverser->def->child_properties)    // if none of our children have props, don't go further
            return 0;
        return 1;
    }
    stashFindPointer( traverser->def->properties, "EncounterGroup", &generatorProp );
    stashFindPointer( traverser->def->properties, "CutSceneCamera", &otherProp );

    if(otherProp)
    {
        cutSceneAddMapCamera( traverser->mat, otherProp->value_str );
        otherProp = 0;
    }

    // if we have a group
    if (generatorProp)
    {
        // init properties to zero
        g_groupInst = 0;
        g_honorGroup = 0;
        g_ignoreRadiusForMe = 0;
        EncounterGroupClear(&g_groupProperties);
        g_groupProperties.state = EG_ASLEEP;
        g_groupProperties.geoname = GroupStringAdd(traverser->def->name);
        copyMat4(traverser->mat, g_groupProperties.mat);

        // spawn probability
        stashFindPointer( traverser->def->properties, "SpawnProbability", &otherProp );
        if (otherProp) g_groupProperties.spawnprob = atoi(otherProp->value_str);
        else g_groupProperties.spawnprob = EG_DEFAULT_SPAWNPROB;

        // manual spawning
        stashFindPointer( traverser->def->properties, "ManualSpawn", &otherProp );
        if (otherProp)
        {
            g_groupProperties.manualspawn = 1;
            g_honorGroup = 1;
            g_ignoreRadiusForMe = 1;
        }

        // forces spawning
        stashFindPointer( traverser->def->properties, "ForcedSpawn", &otherProp );
        if (otherProp)
        {
            g_groupProperties.forcedspawn = 1;
        }

        // honor grouping
        stashFindPointer( traverser->def->properties, "HonorGrouping", &otherProp );
        if (otherProp)
        {
            g_honorGroup = 1;
            g_ignoreRadiusForMe = 1;
        }

        // volume triggered grouping
        stashFindPointer( traverser->def->properties, "volumeTriggered", &otherProp );
        if (otherProp)
        {
            g_groupProperties.volumeTriggered = 1;
        }

        // honor grouping
        stashFindPointer( traverser->def->properties, "IgnoreRadius", &otherProp );
        if (otherProp)
        {
            g_groupProperties.ignoreradius = 1;
        }

        // dont subdivide
        stashFindPointer( traverser->def->properties, "DontSubdivide", &otherProp );
        if (otherProp)
            g_honorGroup = 1;

        // group name
        stashFindPointer( traverser->def->properties, "GroupName", &otherProp );
        if (otherProp) g_groupProperties.groupname = GroupStringAdd(otherProp->value_str);

        // single use
        stashFindPointer( traverser->def->properties, "SingleUse", &otherProp );
        if (otherProp) g_groupProperties.singleuse = 1;

        // villain radius
        stashFindPointer( traverser->def->properties, "VillainRadius", &otherProp );
        if (otherProp) g_groupProperties.villainradius = atoi(otherProp->value_str);
        else g_groupProperties.villainradius = -1;

        // time restrictions
        stashFindPointer( traverser->def->properties, "EncounterEnableStart", &otherProp );
        if (otherProp) g_groupProperties.time_start = atof(otherProp->value_str);
        else g_groupProperties.time_start = 0.0;
        stashFindPointer( traverser->def->properties, "EncounterEnableDuration", &otherProp );
        if (otherProp) g_groupProperties.time_duration = atof(otherProp->value_str);
        else g_groupProperties.time_duration = 24.0;    // all day

        // autospawn markers
        stashFindPointer( traverser->def->properties, "EncounterAutospawnStart", &otherProp );
        if (otherProp)
        {
            g_groupProperties.autostart_begin = atof(otherProp->value_str);
            g_honorGroup = 1;
            g_ignoreRadiusForMe = 1;
        }
        else g_groupProperties.autostart_begin = 0.0;

        stashFindPointer( traverser->def->properties, "EncounterAutospawnEnd", &otherProp );
        if (otherProp)
        {
            g_groupProperties.autostart_end = atof(otherProp->value_str);
            g_honorGroup = 1;
            g_ignoreRadiusForMe = 1;
        }
        else g_groupProperties.autostart_end = 0.0;
        EncounterCalculateAutostart(&g_groupProperties);

        if (OnMissionMap())
        {
            RoomInfo* roominfo;
            roominfo = RoomGetInfoEncounter(&g_groupProperties);

            if (!g_groupProperties.missionspawn && roominfo == g_rooms[0]) // not in a tray
            {
                g_groupProperties.conquered = 1;
            }
        }

        // mission marker
        stashFindPointer( traverser->def->properties, "PersonObjective", &otherProp );
        if (otherProp) g_groupProperties.personobjective = 1;

        // load all EncounterSpawn children
        groupProcessDefExContinue(traverser, spawnLoader);

        // if the parent group was created (by having an EncounterSpawn loaded under it)
        if (g_groupInst)
        {
            // allow the mission system to look at the groups as they are loaded
            MissionProcessEncounterGroup(g_groupInst);
        }
        else // I didn't have any children
        {
            if (!IgnoreEncounterGrouping() &&
                !g_groupProperties.personobjective &&
                !g_groupProperties.missiongroup) // if we aren't ignoring groups, I should have had children
            {
                ErrorFilenamef(saUtilCurrentMapFile(), "EncounterGroupLoader: empty encounter group \"%s\" at %f, %f, %f\n",
                    traverser->def->name, g_groupProperties.mat[3][0], g_groupProperties.mat[3][1], g_groupProperties.mat[3][2]);
            }
        }
        return 0; // stop traversing, EncounterGroups as children should be ignored
    }
    if (!traverser->def->child_properties)    // if none of our children have props, don't go further
        return 0;
    return 1; // traverse children
}

void EncounterUnload(void)
{
    int i, j;

    // clean up in case we were init'ed again
    EncounterForceReset(NULL, NULL);
    EncounterGridClear();
    GroupStringClearAll();
    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];
        if (group)
        {
            for (j = 0; j < group->children.size; j++)
            {
                EncounterPointFree(group->children.storage[j]);
            }
            if (group->objectiveInfo) 
            {
                eaFindAndRemove(&objectiveInfoList, group->objectiveInfo);
                MissionObjectiveInfoDestroy(group->objectiveInfo);
            }
            EncounterActiveInfoDestroy(group);
            for (j = eaSize(&g_runningScripts) - 1; j >= 0; j--)
            {
                if (g_runningScripts[j]->encounter == group)
                {
                    g_runningScripts[j]->encounter = NULL;
                }
            }
            EncounterGroupFree(group);
        }
    }
    clearArray(&g_encountergroups);
}

// EncounterLoad is called when we have a map.  It traces through
// the map looking for groups marked as EncounterGroup's (groups
// of spawn definition map objects).  It allocates an EncounterGroup
// structure for each of these found, and places them in the global
// g_encountergroups variable.  It loads information about the
// contained spawn points for each group.  The EncounterGroup
// struct keeps track of the state of this encounter group and
// current information for what it contains if it has gone off.
void EncounterLoad(void)
{
    StaticMapInfo* mapinfo;
    GroupDefTraverser traverser = {0};

    server_state.bad_spawn_count = 0;

    InitCoverageHashTables();
    EncounterUnload();
    LoadPatrolRoutes();
    loadstart_printf("Loading encounter groups.. ");
    groupProcessDefExBegin(&traverser, &group_info, groupLoader);

    // get critter limits from static map info
    mapinfo = staticMapGetBaseInfo(db_state.map_id);
    if (mapinfo)
    {
        g_encounterMinCritters = mapinfo->minCritters;
        g_encounterMaxCritters = mapinfo->maxCritters;
    }
    VerifyEncounterCoverage();
    if (EncounterInMissionMode()) VerifyMissionEncounterCoverage();

    loadend_printf("%i found", g_encountergroups.size);
}

// *********************************************************************************
//  Encounter state transitions
// *********************************************************************************

// An encounter is an enemy if it is not of the group it is flagged for
static INLINEDBG int EncounterIsEnemy(Entity* e)
{
    //returns false if its on the GANG_OF_PLAYERS_WHEN_IN_COED_TEAMUP gang
    if ((server_state.allowHeroAndVillainPlayerTypesToTeamUp || server_state.allowPrimalAndPraetoriansToTeamUp) && e->pchar && e->pchar->iGangID == GANG_OF_PLAYERS_WHEN_IN_COED_TEAMUP)
        return false;

    if( g_ArchitectTaskForce ) // this should probably be not just for the architect, but I dont want to change the whole mission system
    {
        if( e->pchar && e->pchar->iGangID > 1 )
            return 1;
    }

    // First line just check to make sure the pointers are good
    // Second Line returns true if its built for Good and the entity is not good
    // Third Line returns true if its built for Villains and the entity is not a villain
    return e->pchar && e->encounterInfo && e->encounterInfo->parentGroup && e->encounterInfo->parentGroup->active &&
        (((e->pchar->iAllyID != kAllyID_Good)&&(e->encounterInfo->parentGroup->active->allyGroup==ENC_FOR_HERO)) ||
        ((e->pchar->iAllyID != kAllyID_Villain)&&(e->encounterInfo->parentGroup->active->allyGroup==ENC_FOR_VILLAIN)));
}

// when an actor is defeated or rescued
static void ActorRewardApply(Entity* rescuer, Entity* actor)
{
    EncounterGroup* group;
    StoryReward* reward;
    RewardAccumulator* accumulator;
    const Actor* a;

    // see if we have an actor and a reward
    if (!actor || !actor->encounterInfo || actor->encounterInfo->rewardgiven || !actor->encounterInfo->parentGroup ||
        !rescuer || rescuer->db_id <= 0) return;
    group = actor->encounterInfo->parentGroup;
    if (group->cleaning) return;
    if (actor->encounterInfo->actorID >= eaSize(&group->active->spawndef->actors) || actor->encounterInfo->actorID < 0)
        return; // not an actor

    a = group->active->spawndef->actors[actor->encounterInfo->actorID];
    if (!a->reward || !eaSize(&a->reward)) return;
    reward = a->reward[0];
    actor->encounterInfo->rewardgiven = 1;

    // let story arc take care of it if possible
    if (StoryArcEncounterReward(group, rescuer, reward, actor)) return;

    // otherwise, issue the fields we can, and only if the rescuer is online
    if (!rescuer || rescuer->db_id <= 0) return;

    // dialog
    if (reward->rewardDialog)
    {
        const char* dialog;

        ScriptVarsTablePushVarEx(&group->active->vartable, "Rescuer", (char*)rescuer, 'E', 0);
        dialog = saUtilTableAndEntityLocalize(reward->rewardDialog, &group->active->vartable, rescuer);
        saUtilEntSays(actor, !EncounterIsEnemy(actor), dialog);
        ScriptVarsTablePopVar(&group->active->vartable);
    }

    // random fame
    if (reward->famestring)
    {
        ScriptVarsTablePushVarEx(&group->active->vartable, "Rescuer", (char*)rescuer, 'E', 0);
        RandomFameAdd(rescuer, saUtilTableAndEntityLocalize(reward->famestring, &group->active->vartable, rescuer));
        ScriptVarsTablePopVar(&group->active->vartable);
    }

    // badge stat
    if (reward->badgeStat)
        badge_StatAdd(rescuer, reward->badgeStat, 1);

    // bonus time
    if (reward->bonusTime)
        StoryRewardBonusTime(reward->bonusTime);

    // float text
    if (reward->floatText)
        StoryRewardFloatText(rescuer, saUtilTableAndEntityLocalize(reward->floatText, &group->active->vartable, rescuer));

    // rewards
    accumulator = rewardGetAccumulator();
    rewardSetsAward(reward->rewardSets, accumulator, rescuer, group->active->villaingroup, adjustRewardLevelForSidekick(rescuer,group->active->villainlevel-1)+1, NULL, 0, NULL);
    rewardApplyLogWrapper(accumulator, rescuer, true, !villainWillGiveItems(group->active->villainlevel, character_GetCombatLevelExtern(rescuer->pchar)), REWARDSOURCE_ENCOUNTER);

    // table-driven rewards
    if( eaSize(&reward->primaryReward))
    {
        // In missions, apply the special story reward wrapper
        if (OnMissionMap())
            rewardStoryApplyToDbID(rescuer->db_id, reward->primaryReward, group->active->villaingroup, group->active->villainlevel, MissionGetLevelAdjust(), &g_activemission->vars, REWARDSOURCE_ENCOUNTER);
        else
            rewardFindDefAndApplyToDbID(rescuer->db_id, reward->primaryReward, group->active->villaingroup, group->active->villainlevel, 0, REWARDSOURCE_ENCOUNTER);
    }
}

static void EncounterCompleteRewardApply(EncounterGroup *group, Entity* player)
{
    RewardAccumulator* accumulator;
    StoryInfo *info;
    int iLockedStoryInfo, taskIndex, i;
    const StoryReward *reward;
    ScriptVarsTable vars = {0};

    if (!player || player->db_id <= 0) return;
    if (group->cleaning) return;
    if (!eaSize(&group->active->spawndef->encounterComplete)) return;

    if (StoryArcInfoIsLocked(player))
    {
        info = StoryInfoFromPlayer(player);
        iLockedStoryInfo = false;
    }
    else
    {
        info = StoryArcLockInfo(player);
        iLockedStoryInfo = true;
    }

    reward = group->active->spawndef->encounterComplete[0];

    // dialog
    if (reward->rewardDialog)
    {
        const char *rewardstr = saUtilTableAndEntityLocalize(reward->rewardDialog, &vars, player);
        if (rewardstr)
            sendInfoBox(player, INFO_REWARD, "%s", rewardstr);
    }

    // random fame
    if (reward->famestring)
    {
        ScriptVarsTablePushVarEx(&group->active->vartable, "Rescuer", (char*)player, 'E', 0);
        RandomFameAdd(player, saUtilTableAndEntityLocalize(reward->famestring, &group->active->vartable, player));
        ScriptVarsTablePopVar(&group->active->vartable);
    }

    // badge stat
    if (reward->badgeStat)
        badge_StatAdd(player, reward->badgeStat, 1);

    // bonus time
    if (reward->bonusTime)
        StoryRewardBonusTime(reward->bonusTime);

    // float text
    if (reward->floatText)
        StoryRewardFloatText(player, saUtilTableAndEntityLocalize(reward->floatText, &group->active->vartable, player));

    // add and remove tokens
    StoryRewardHandleTokens(player, reward->addTokens, reward->addTokensToAll, reward->removeTokens, reward->removeTokensFromAll);

    for (i = eaSize(&reward->popHelp) - 1; i >= 0; i--)
        triggerPopHelpEventHappenedByTag(player, reward->popHelp[i]);

    if (info)
    {
        for (i = eaSize(&reward->abandonContacts) - 1; i >= 0; i--)
        {
            for (taskIndex = eaSize(&info->tasks) - 1; taskIndex >= 0; taskIndex--)
            {
                StoryContactInfo *contactInfo = TaskGetContact(info, info->tasks[taskIndex]);
                if (contactInfo && contactInfo->contact->def)
                {
                    const char *contactName = strrchr(contactInfo->contact->def->filename, '/');
                    if (contactName)
                    {
                        contactName++;
                        if (!strnicmp(contactName, reward->abandonContacts[i], strlen(reward->abandonContacts[i])))
                        {
                            TaskAbandon(player, info, contactInfo, info->tasks[taskIndex], 0);
                        }
                    }
                }
            }
        }
    }

    StoryRewardHandleSounds(player, reward);

    if (reward->rewardPlayFXOnPlayer)
    {
        character_PlayFX(player->pchar, player->pchar, reward->rewardPlayFXOnPlayer, colorPairNone, 0, 0, PLAYFX_NO_TINT);
    }

    // rewards
    accumulator = rewardGetAccumulator();
    rewardSetsAward(reward->rewardSets, accumulator, player, group->active->villaingroup, adjustRewardLevelForSidekick(player,group->active->villainlevel-1)+1, NULL, 0, NULL);
    rewardApplyLogWrapper(accumulator, player, true, !villainWillGiveItems(group->active->villainlevel, character_GetCombatLevelExtern(player->pchar)), REWARDSOURCE_ENCOUNTER);

    // table-driven rewards
    if( eaSize(&reward->primaryReward))
    {
        // In missions, apply the special story reward wrapper
        if (OnMissionMap())
            rewardStoryApplyToDbID(player->db_id, reward->primaryReward, group->active->villaingroup, group->active->villainlevel, MissionGetLevelAdjust(), &g_activemission->vars, REWARDSOURCE_ENCOUNTER);
        else
            rewardFindDefAndApplyToDbID(player->db_id, reward->primaryReward, group->active->villaingroup, group->active->villainlevel, 0, REWARDSOURCE_ENCOUNTER);
    }

    if (iLockedStoryInfo)
    {
        StoryArcUnlockInfo(player);
    }
}

// change the state of a single entity
// ignores change if entity is already in that state
static void entityTransition(EncounterGroup* group, Entity* ent, ActorState tostate)
{
    const Actor* a = NULL;
    const char* dialog;

    // check entry conditions
    if (!group || !group->active) 
        return;
    if (!ent || !ent->encounterInfo) 
        return;
    if (ent->encounterInfo->state == tostate) 
        return; // already in correct state

    // get actor
    if (ent->encounterInfo->actorID >= 0 && ent->encounterInfo->actorID < eaSize(&group->active->spawndef->actors))
        a = group->active->spawndef->actors[ent->encounterInfo->actorID];
    else
        return; // not an actor

    // make sure we don't jump state changes
    if (ent->encounterInfo->state < ACT_WORKING        && tostate > ACT_INACTIVE) 
        entityTransition(group, ent, ACT_WORKING);
    if (ent->encounterInfo->state < ACT_INACTIVE    && tostate > ACT_INACTIVE) 
        entityTransition(group, ent, ACT_INACTIVE);
    if (ent->encounterInfo->state < ACT_ACTIVE        && tostate > ACT_ACTIVE) 
        entityTransition(group, ent, ACT_ACTIVE);
    if (ent->encounterInfo->state < ACT_ALERTED        && tostate > ACT_ALERTED) 
        entityTransition(group, ent, ACT_ALERTED);
    if (ent->encounterInfo->state < ACT_COMPLETED    && tostate > ACT_COMPLETED) 
        entityTransition(group, ent, ACT_COMPLETED);

    // make state change
    ent->encounterInfo->state = tostate;

    // make ai transition, set priority list and any additional flags
    if (a->ai_states[tostate])
    {
        const char* ai;

        ai = ScriptVarsTableLookup(&group->active->vartable, a->ai_states[tostate]);
        if (ai)
        {
            AI_LOG(AI_LOG_ENCOUNTER, (ent, "Encounter changing ai to %s (on %i)\n", ai, tostate));
            aiBehaviorAddString(ent, ENTAI(ent)->base, ai);
        }
    }

    // do dialog
    if (ent->encounterInfo->actorSubID == 0)
    {
        Entity* rescuer = NULL;

        // do rescuer variable
        if (tostate == ACT_THANK)
            rescuer = EncounterWhoSavedMe(ent);
        else
            rescuer = EncounterNearestPlayer(ent); // just find the closest player

        if (rescuer) 
            ScriptVarsTablePushVarEx(&group->active->vartable, "Rescuer", (char*)rescuer, 'E', 0);

        dialog = saUtilTableAndEntityLocalize(a->dialog_states[tostate], &group->active->vartable, rescuer);
        dialog = MissionAdjustDialog(dialog, group, ent, a, tostate, &group->active->vartable);
        if (dialog) 
            saUtilEntSays(ent, !EncounterIsEnemy(ent), dialog);
        if (rescuer) 
            ScriptVarsTablePopVar(&group->active->vartable);
    }
}

// change the state of an entire actor group
static void actorGroupTransition(EncounterGroup* group, int groupnum, ActorState tostate)
{
    int i, n;

    if (!group || !group->active) return;
    if (groupnum < 0) return;

    // iterate through each entity in encounter
    n = eaSize(&group->active->ents);
    for (i = 0; i < n; i++)
    {
        const Actor* a = NULL;
        Entity* ent = group->active->ents[i];

        // check entity and actor
        if (!ent || !ent->encounterInfo)
        {
            assert(0 && "Encounter group internal error");
            continue;
        }
        if (ent->encounterInfo->actorID >= 0 && ent->encounterInfo->actorID < eaSize(&group->active->spawndef->actors))
        {
            a = group->active->spawndef->actors[ent->encounterInfo->actorID];
            if (a->ai_group == groupnum)
            {
                entityTransition(group, ent, tostate);
            } // correct ai group
        }
    } // each entity in encounter
}

// change every actor in encounter to <tostate>, saying dialog and changing ai as appropriate
static void encounterTransition(EncounterGroup* group, ActorState tostate)
{
    int i, n;

    // iterate through each entity in encounter
    if (!group || !group->active) return;
    n = eaSize(&group->active->ents);
    for (i = 0; i < n; i++)
    {
        entityTransition(group, group->active->ents[i], tostate);
    }
}

// returns the encounter position this actor spawned from
int EncounterPosFromActor(Entity* ent)
{
    const Actor* a;
    EncounterGroup* group;

    if (!ent || !ent->encounterInfo) return 0;
    group = ent->encounterInfo->parentGroup;
    if (!group || !group->active) return 0;
    if (ent->encounterInfo->actorID >= 0 && ent->encounterInfo->actorID < eaSize(&group->active->spawndef->actors))
    {
        a = group->active->spawndef->actors[ent->encounterInfo->actorID];
        if (ent->encounterInfo->actorSubID >= 0 && ent->encounterInfo->actorSubID < eaiSize(&a->locations))
        {
            return a->locations[ent->encounterInfo->actorSubID];
        }
    }
    return 0;
}

// returns the actorName of the actor definition in the spawnDef used to spawn this entity 
char * EncounterActorNameFromActor(Entity* ent)
{
    const Actor* a;
    EncounterGroup* group;

    if (!ent || !ent->encounterInfo) return 0;
    group = ent->encounterInfo->parentGroup;
    if (!group || !group->active) return 0;
    if (ent->encounterInfo->actorID >= 0 && ent->encounterInfo->actorID < eaSize(&group->active->spawndef->actors))
    {
        a = group->active->spawndef->actors[ent->encounterInfo->actorID];
        return a->actorname;
    }
    return 0;
}

// Given an encounter, returns first actor with that name (Maybe it needs to return all of them)
Entity * EncounterActorFromEncounterActorName(EncounterGroup* group, const char * actorName )
{
    int i, n;

    if( !group || !group->active || !actorName )
        return 0;

    n = eaSize(&group->active->ents);
    for (i = 0; i < n; i++)
    {
        char * myActorName = EncounterActorNameFromActor( group->active->ents[i] );
        if( myActorName && 0 == stricmp( myActorName, actorName ) )
            return  group->active->ents[i];
    }
    return 0;
}

//Gets everything from the map about this encounter position
int EncounterGetPositionInfo( EncounterGroup* group, int encounterPosition, Vec3 pyr, Vec3 pos, Vec3 zpos, int * EPFlags, F32 * stealthCameraArc, F32 * stealthCameraSpeed )
{
    EncounterPoint* point;
    int child = 0;

    //Check and prep
    if (!group || encounterPosition < 0 || encounterPosition >= EG_MAX_ENCOUNTER_POSITIONS) 
        return 0;
    if (group->active) 
    {
        child = group->active->point;
        if (child < 0 || child >= group->children.size) 
            return 0;
    }

    point = group->children.storage[child];
    DoZDrops(point);


    //Copy Info
    if( pyr )
        copyVec3(point->pyrpositions[encounterPosition], pyr);
    if( pos )
        copyVec3(point->positions[encounterPosition], pos);
    if( zpos )
        copyVec3(point->zpositions[encounterPosition], zpos);
    if( EPFlags )
        *EPFlags = point->epFlags[encounterPosition];
    if( stealthCameraArc )
        *stealthCameraArc = point->stealthCameraArcs[encounterPosition];
    if( stealthCameraSpeed )
        *stealthCameraSpeed = point->stealthCameraSpeeds[encounterPosition];

    return 1;

}


// returns the mat of a given encounter position, returns 0 on failure
//  - always gives ground-snapped, unrolled positions
//  - if the group isn't active, it gets the positions from the first encounter spawn
int EncounterMatFromPos(EncounterGroup* group, int pos, Mat4 result)
{
    EncounterPoint* point;
    int child = 0;
    Vec3 pyr;

    if (!group || pos < 0 || pos >= EG_MAX_ENCOUNTER_POSITIONS) return 0;
    if (group->active) 
    {
        child = group->active->point;
        if (child < 0 || child >= group->children.size) 
            return 0;
    }

    point = group->children.storage[child];

    DoZDrops(point);
    copyVec3(point->pyrpositions[pos], pyr);
    pyr[0] = 0;
    pyr[2] = 0;
    createMat3YPR(result, pyr);
    copyVec3(point->zpositions[pos], result[3]);
    return 1;
}

// returns the mat of a given encounter position from the specified layout, returns 0 on failure
//  - always gives ground-snapped, unrolled positions
int EncounterMatFromLayoutPos(EncounterGroup* group, char* layout, int pos, Mat4 result)
{
    EncounterPoint* point;
    int child = 0;
    Vec3 pyr;

    if (!group || !layout || pos < 0 || pos >= EG_MAX_ENCOUNTER_POSITIONS) return 0;

    child = EncounterGetMatchingPoint(group, layout);

    if (child < 0) return 0;

    point = group->children.storage[child];

    DoZDrops(point);
    copyVec3(point->pyrpositions[pos], pyr);
    pyr[0] = 0;
    pyr[2] = 0;
    createMat3YPR(result, pyr);
    copyVec3(point->zpositions[pos], result[3]);
    return 1;
}

// returns a position defined by the encounter group but not
// occupied by an actor - only valid for active groups. -1 on error
static int EncounterFindFreePos(EncounterGroup* group)
{
    EncounterPoint* point;
    int trypos, i, n, ok;
    if (!group->active) return -1;

    i = group->active->point;
    if (i < 0 || i >= group->children.size) return -1;
    point = group->children.storage[i];

    for (trypos = 0; trypos < EG_MAX_ENCOUNTER_POSITIONS; trypos++)
    {
        if (memcmp(point->positions[trypos], g_nullVec3, sizeof(Vec3)) == 0)
            continue; // point isn't on map

        // any actors attached to this encounter at that point?
        ok = 1;
        n = eaSize(&group->active->ents);
        for (i = 0; i < n; i++)
        {
            EncounterEntityInfo* info = group->active->ents[i]->encounterInfo;
            if (info && info->actorID >= 0 && info->actorID < eaSize(&group->active->spawndef->actors))
            {
                const Actor* actor = group->active->spawndef->actors[info->actorID];
                if (info->actorSubID >= 0 && info->actorSubID < eaiSize(&actor->locations))
                {
                    if (trypos == actor->locations[info->actorSubID])
                    {
                        ok = 0;
                        break;
                    }
                }
            }
        }

        if (ok) return trypos;
    }
    return -1;
}

// *********************************************************************************
//  Auto-start spawns
// *********************************************************************************

static void EncounterCalculateAutostart(EncounterGroup* group)
{
    group->autostart_time = 0;
    if (group->autostart_begin != 0.0 || group->autostart_end != 0.0)
    {
        F32 range, time, r;
        U32 seconds;

        // range check
        if (group->autostart_begin < 0.0)
            group->autostart_begin = 0.0;
        if (group->autostart_end < group->autostart_begin)
            group->autostart_end = group->autostart_begin;

        // pick a random time
        range = group->autostart_end - group->autostart_begin;
        time = group->autostart_begin;
        r = rule30Float();
        if (r < 0.0) r *= -1;
        time += r * range;

        // figure out dbSeconds
        seconds = (U32)(time * 3600);
        if (g_encounterMinAutostartTime && (U32)g_encounterMinAutostartTime < seconds)
            seconds = g_encounterMinAutostartTime;
        group->autostart_time = dbSecondsSince2000() + seconds;
    }
    else if (group->active && group->active->spawndef && (group->active->spawndef->flags & SPAWNDEF_AUTOSTART) &&
        !group->conquered) 
    {
        group->autostart_time = 1;
    }
}

static int EncounterHasAutostart(EncounterGroup* group)
{
    return group->autostart_time != 0;
}

static int EncounterNeedAutostart(EncounterGroup* group)
{
    return (group->autostart_time != 0 && dbSecondsSince2000() > group->autostart_time);
}

// *********************************************************************************
//  Hard limits on critters
// *********************************************************************************

static void EncounterCountCritters()
{
    int i;

    g_encounterCurCritters = 0;
    for(i = 0; i < entities_max; i++)
    {
        Entity* e = validEntFromId(i);
        if (e && ENTTYPE(e) == ENTTYPE_CRITTER)
            g_encounterCurCritters++;
    }
}

static int TooFewCritters(int subtract)
{
    return g_encounterMinCritters && (g_encounterCurCritters - subtract) < g_encounterMinCritters;
}

static int TooManyCritters()
{
    return g_encounterMaxCritters && g_encounterCurCritters > g_encounterMaxCritters;
}

// *********************************************************************************
//  Polling functions
// *********************************************************************************

void EncounterDoZDrop(Vec3 pos_in, Vec3 pos_out)
{
    CollInfo info;
    Vec3 endPos;
    Vec3 startPos;

    copyVec3(pos_in, startPos);
    copyVec3(startPos, endPos);

    // Go from 5 feet above point to 100 feet below.
    startPos[1] += 5;
    endPos[1] -= 100;

    // If there's a collision, then move the spawn point to there.
    if(collGrid(NULL, startPos, endPos, &info, 0, COLL_NOTSELECTABLE|COLL_DISTFROMSTART))
    {
        copyVec3(info.mat[3], pos_out);
    }
    else
    {
        copyVec3(pos_in, pos_out);
    }
}

static void DoZDrops(EncounterPoint *point)
{
    int i;

    if (point->havezdropped) return;
    for (i = 0; i < EG_MAX_ENCOUNTER_POSITIONS; i++)
    {
        if (memcmp(point->positions[i], g_nullVec3, sizeof(Vec3)) == 0)
            continue;

        EncounterDoZDrop(point->positions[i], point->zpositions[i]);
    }
    point->havezdropped = 1;
}

static void DoGroupZDrops(EncounterGroup* group)
{
    int i;

    if (group->havezdropped) return;
    for (i = 0; i < group->children.size; i++)
    {
        EncounterPoint* point = (EncounterPoint*)group->children.storage[i];
        DoZDrops(point);
    }
    group->havezdropped = 1;
}

// we actually just move the original encounter position mat4's around
// depending on where the player entered.  Works because we always know we should
// move around 1-5, 6-10, 11-15, and the EncounterPoint can't be referenced by
// more than one group.  We make the judgement of how to move around based on
// the points 1, 6, and 11.
static void SquadRenumber(EncounterPoint *point, Vec3 player_pos)
{
    EncounterPoint tempPoint;        // buffer to copy positions;
    float distances[3];                // original 1, 6, 11 distances
    int sourceOffset[3];            // ordered copying offsets
    int i, tempi;
    float tempf;

    memcpy(&tempPoint, point, sizeof(EncounterPoint));
    distances[0] = distance3Squared(player_pos, point->positions[1]);
    distances[1] = distance3Squared(player_pos, point->positions[6]);
    distances[2] = distance3Squared(player_pos, point->positions[11]);

    sourceOffset[0] = 1;
    sourceOffset[1] = 6;
    sourceOffset[2] = 11;

    for (i = 0; i < 2; i++)            // too lazy to write a qsort functor
    {
        if (distances[0] > distances[1])
        {
            tempf = distances[1];            tempi = sourceOffset[1];
            distances[1] = distances[0];    sourceOffset[1] = sourceOffset[0];
            distances[0] = tempf;            sourceOffset[0] = tempi;
        }
        if (distances[1] > distances[2])
        {
            tempf = distances[2];            tempi = sourceOffset[2];
            distances[2] = distances[1];    sourceOffset[2] = sourceOffset[1];
            distances[1] = tempf;            sourceOffset[1] = tempi;
        }
    }

    memcpy(point->positions[1], tempPoint.positions[sourceOffset[0]], sizeof(Vec3) * 5);
    memcpy(point->positions[6], tempPoint.positions[sourceOffset[1]], sizeof(Vec3) * 5);
    memcpy(point->positions[11], tempPoint.positions[sourceOffset[2]], sizeof(Vec3) * 5);

    memcpy(point->zpositions[1], tempPoint.zpositions[sourceOffset[0]], sizeof(Vec3) * 5);
    memcpy(point->zpositions[6], tempPoint.zpositions[sourceOffset[1]], sizeof(Vec3) * 5);
    memcpy(point->zpositions[11], tempPoint.zpositions[sourceOffset[2]], sizeof(Vec3) * 5);

    memcpy(point->pyrpositions[1], tempPoint.pyrpositions[sourceOffset[0]], sizeof(Vec3) * 5);
    memcpy(point->pyrpositions[6], tempPoint.pyrpositions[sourceOffset[1]], sizeof(Vec3) * 5);
    memcpy(point->pyrpositions[11], tempPoint.pyrpositions[sourceOffset[2]], sizeof(Vec3) * 5);

    //These should never be in a renumbered squad, but I move them anyway in the interest of completeness
    //(Its pretty clear it's time to make encounter positions structures)
    memcpy(&point->epFlags[1], &tempPoint.epFlags[sourceOffset[0]], sizeof(int) * 5);
    memcpy(&point->epFlags[6], &tempPoint.epFlags[sourceOffset[1]], sizeof(int) * 5);
    memcpy(&point->epFlags[11], &tempPoint.epFlags[sourceOffset[2]], sizeof(int) * 5);

    memcpy(&point->stealthCameraArcs[1], &tempPoint.stealthCameraArcs[sourceOffset[0]], sizeof(F32) * 5);
    memcpy(&point->stealthCameraArcs[6], &tempPoint.stealthCameraArcs[sourceOffset[1]], sizeof(F32) * 5);
    memcpy(&point->stealthCameraArcs[11], &tempPoint.stealthCameraArcs[sourceOffset[2]], sizeof(F32) * 5);

    memcpy(&point->stealthCameraSpeeds[1], &tempPoint.stealthCameraSpeeds[sourceOffset[0]], sizeof(F32) * 5);
    memcpy(&point->stealthCameraSpeeds[6], &tempPoint.stealthCameraSpeeds[sourceOffset[1]], sizeof(F32) * 5);
    memcpy(&point->stealthCameraSpeeds[11], &tempPoint.stealthCameraSpeeds[sourceOffset[2]], sizeof(F32) * 5);
}

// we want a certain probability of getting an ideal result, otherwise we make
// sure to have a non-ideal result
static int EncounterRangeSelection(int min, int max, int ideal, int ideal_prob)
{
    int choices[MAX_VILLAIN_LEVEL + 1];
    int numchoices, i;

    max = CLAMP(max, 1, MAX_VILLAIN_LEVEL);
    min = CLAMP(min, 1, MAX_VILLAIN_LEVEL);

    if (ideal < min || ideal > max) ideal = -1; // ignore ideal if outside normal range
    if (ideal != -1 && (rand() % 100 <= ideal_prob))
    {
        return ideal;
    }
    else // need a random result that is NOT ideal
    {
        numchoices = 0;
        for (i = min; i <= max; i++)
        {
            if (i == ideal) continue;

            choices[numchoices++] = i;

        }
        if (numchoices == 0)
            return min;
        else
            return choices[rand() % numchoices];
    }
}

// standard method for choosing level and size of an encounter in the city,
// group should have an active record to be filled in
static void EncounterSizeLevelSelection(EncounterGroup* group, Entity* instigator)
{
    int villainMinLevel = group->active->spawndef->villainminlevel;
    int villainMaxLevel = group->active->spawndef->villainmaxlevel;
    int minTeamSize = group->active->spawndef->minteamsize;
    int maxTeamSize = group->active->spawndef->maxteamsize;

    if(group->active->spawndef->flags & SPAWNDEF_NEIGHBORHOODDEFINED)
        pmotionGetNeighborhoodFromPos(group->mat[3], NULL, NULL, &villainMinLevel, &villainMaxLevel, &minTeamSize, &maxTeamSize);

    if (s_mobLevelOverride != 0)
    {
        group->active->villainlevel = s_mobLevelOverride + (rand() & 1);
    }
    // if no instigator, everything's random.  (doesn't happen often)
    else if (!instigator)
    {
        int diff = MAX(villainMaxLevel - villainMinLevel, 0);
        group->active->villainlevel = villainMinLevel + (diff? (rand() % diff): 0);

        diff = MAX(maxTeamSize - minTeamSize, 0);
        group->active->numheroes = minTeamSize + (diff? (rand() % diff): 0);
    }
    else 
    {
        group->active->villainlevel = EncounterRangeSelection(
            villainMinLevel, villainMaxLevel,
            character_GetCombatLevelExtern(instigator->pchar), EG_LEVEL_PUSH_PROB);
        group->active->numheroes = EncounterRangeSelection(
            minTeamSize, maxTeamSize,
            group->active->numheroes, EG_TEAMSIZE_PUSH_PROB);
    }
    if (g_encounterForceTeam > 0) 
        group->active->numheroes = g_encounterForceTeam;    // DEBUG hack
}

// verify the active info and any actors attached to this encounter are destroyed
static void EncounterActiveInfoDestroy(EncounterGroup* group)
{
    int i;

    if (group->active)
    {
        StopEncounterScripts(group);

        group->cleaning = 1; // don't do the normal entity destruction processing
        for (i = eaSize(&group->active->ents)-1; i >= 0; i--)
        {
            Entity* ent = group->active->ents[i];
            EncounterEntDetach(ent);
            entFree(ent);
        }
        group->cleaning = 0;

        dbLog("Encounter:Destroyed", NULL, "%s", dbg_EncounterGroupStr(group));
        eaDestroy(&group->active->ents);
        eaiDestroy(&group->active->scriptids);
        eaiDestroy(&group->active->whoKilledMyActors);
        EncounterActiveInfoFree(group->active);
        group->active = NULL;
    }
}

//externed because if everything works properly, I'm going to move the code that uses this into 
//mission.c and make an accessor function
extern MissionInfo* MissionInitInfo(Entity* player);

// do random selections and story arc lookups to set up the active field in group
//  - spawndef is optional
int EncounterNewSpawnDef(EncounterGroup* group, const SpawnDef* spawndef, const char* spawntype)
{
    EncounterPoint* point = NULL;
    Entity* players[ENT_FIND_LIMIT];
    int count, sel, i;
    int replaced = 0;
    Vec3 player_pos;
    Entity* instigator = NULL;    // player who started the encounter

    if (!spawntype && spawndef)
        spawntype = spawndef->spawntype;

    copyVec3(group->mat[3], player_pos); // sane default

    // set up the active record
    EncounterActiveInfoDestroy(group);
    group->active = EncounterActiveInfoAlloc();

    // find a player nearby
    count = saUtilPlayerWithinDistance(group->mat, EG_STORYARC_RADIUS, players, EG_PLAYER_VELPREDICT);
    if (s_teamSizeOverride)
    {
        group->active->numheroes = s_teamSizeOverride;
    }
    else if (count)
    {
        sel = rand() % count;
        group->active->numheroes = count;

        // look through all players nearby for the largest team
        for (i = 0; i < count; i++)
        {
            TeamMembers* team = teamMembers(players[i], CONTAINER_TEAMUPS);
            if (team && team->count > group->active->numheroes)
                group->active->numheroes = team->count;
        }

        instigator = players[sel];
        copyVec3(ENTPOS(players[sel]), player_pos);
    }
    else
    {
        // just behave sanely if we can't find a player nearby any more
        group->active->numheroes = 1;
    }

    // set up default fields before passing on to mission, storyarcs, etc.
    group->cleaning = 0;
    group->active->point = rand() % group->children.size;
    point = group->children.storage[group->active->point];
    group->active->seed = (unsigned int)time(NULL);
    eaCreate(&group->active->ents);
    group->active->villaincount = 0;
    group->active->whokilledme = 0;
    // flag the allyGroup before it gets to the missions so we make sure not to overwrite 
    group->active->allyGroup = ENC_UNINIT;
    if (instigator)
    {
        group->active->instigator = erGetRef(instigator);
        group->active->kheldian = HasKheldianTeamMember(instigator);
    }

    // let everyone else get a crack at it
    if (spawndef)
    {
        //TO DO functionize
        if (spawntype)
        { // spawntype might be a variable
            ScriptVarsTable vars = {0};
            ScriptVarsTablePushScope(&vars, &spawndef->vs);
            ScriptVarsTableSetSeed(&vars, group->active->seed);
            spawntype = ScriptVarsTableLookup(&vars, spawntype);
            group->active->point = EncounterGetMatchingPoint(group, spawntype);
        }

        if (group->active->point == -1)
        {
            group->state = EG_OFF;
        }
        else
        {
            replaced = 1;
            group->active->spawndef = spawndef;
            ScriptVarsTablePushScope(&group->active->vartable, &spawndef->vs);
            ScriptVarsTableSetSeed(&group->active->vartable, group->active->seed);
            group->active->mustspawn = 1;
        }

        if (replaced)
        {

            EncounterSizeLevelSelection(group, NULL);

            //TO DO functionify
            if( OnMissionMap() )
            {
                MissionInfo * info = MissionInitInfo(NULL); //just g_activemission
                if( group->active->spawndef && info )
                {
                    ScriptVarsTableClear(&group->active->vartable);
                    // If info->ownerDbid is zero, we're using the placeholder mission for end game raids, which means the var scopes are not valid
                    if (info->ownerDbid)
                    {
                        // calls ScriptVarsTableClear in EncounterActiveInfoDestroy. potential mismatch
                        saBuildScriptVarsTable(&group->active->vartable, instigator, info->ownerDbid, info->contact, NULL, info->task, NULL, NULL, NULL, info->missionGroup, info->seed, true);
                    }
                    ScriptVarsTablePushScope(&group->active->vartable, &group->active->spawndef->vs);

                    // Flag the encounter the same as the mission if spawndef does not specify
                    group->active->allyGroup = group->active->spawndef->encounterAllyGroup;
                    if(group->active->allyGroup == ENC_UNINIT)
                        group->active->allyGroup = info->missionAllyGroup;

                    // decide if we are doing boss reduction
                    if (MissionNumHeroes() == 1 && !info->difficulty.dontReduceBoss && 
                        (!info->taskforceId || info->flashback || info->scalableTf))
                    {
                        group->active->bossreduction = 1;
                    }
                }
            }
        } 
        else 
            Errorf("Unable to find location for specified spawndef (%s)", spawntype);
    }
    if (!replaced)
        replaced = MissionSetupEncounter(group, instigator);
    if (!replaced)
        replaced = StoryArcGetEncounter(group, instigator); // try getting an encounter from a story arc
    if (!replaced)
        replaced = TaskGetEncounter(group, instigator); // try getting an encounter from an active task
    if (!replaced)
        replaced = ScriptGetEncounter(group, instigator); // try getting an encounter from an active script
    if (!replaced && eaSize(&point->infos)) // CANSPAWN stuff only do it ourselves if we have some spawndefs
    {
        //TO DO functionize
        // set up spawndef and string localization fields
        sel = rand() % eaSize(&point->infos);
        group->active->spawndef = point->infos[sel];

        if( OnMissionMap() )
        {
            MissionInfo * info = MissionInitInfo(NULL); //just g_activemission
            if( group->active->spawndef && info )
            {
                ScriptVarsTableClear(&group->active->vartable);
                // If info->ownerDbid is zero, we're using the placeholder mission for end game raids, which means the var scopes are not valid
                if (info->ownerDbid)
                {
                    // calls ScriptVarsTableClear in EncounterActiveInfoDestroy. potential mismatch
                    saBuildScriptVarsTable(&group->active->vartable, instigator, info->ownerDbid, info->contact, NULL, info->task, NULL, NULL, NULL, info->missionGroup, info->seed, true);
                }
                ScriptVarsTablePushScope(&group->active->vartable, &group->active->spawndef->vs);

                // decide if we are doing boss reduction
                if (MissionNumHeroes() == 1 && !info->difficulty.dontReduceBoss &&                     
                    (!info->taskforceId || info->flashback || info->scalableTf))
                {
                    group->active->bossreduction = 1;
                }
            }
        }

        group->active->iscanspawn = 1; // This group can from a CANSPAWN
        group->active->villaingroup = VG_NONE; // no default group for a normal encounter
        EncounterSizeLevelSelection(group, instigator);    // set numheroes, villainlevel
        if (OnMissionMap()) 
            MissionSetEncounterLevel(group, instigator); // let mission override
        replaced = 1;
    }


    // otherwise, we just can't get an encounter here
    if (!replaced)
    {
        group->state = EG_OFF;
    }

    if (group->state == EG_OFF)
    {
        group->conquered = 1;
        EncounterActiveInfoDestroy(group);
        EncounterDebug("Encounter didn't go off");
        return 0;
    }
    dbLog("Encounter:Created", NULL, "%s", dbg_EncounterGroupStr(group));

    // make sure this stuff is set up correctly by mission and story arc clients
    assert(group->active->spawndef);
    assert(group->active->point >= 0 && group->active->point < group->children.size);
    point = group->children.storage[group->active->point];
    assert(point);
    group->active->patrol = point->patrol;
    if (!group->active->vartable.numscopes)
    {
        ScriptVarsTablePushScope(&group->active->vartable, &group->active->spawndef->vs);
        ScriptVarsTableSetSeed(&group->active->vartable, group->active->seed);
    }
    if (group->active->spawndef->missionuncounted)
    {
        group->uncounted = 1;
    }

    // final stuff before we're ready
    if (point->squad_renumber)
    {
        SquadRenumber(point, player_pos);
    }

    EncounterDebug("Encounter deciding how to spawn - %s:%s, (team:%i level:%i)", group->active->spawndef->logicalName,group->active->spawndef->fileName, group->active->numheroes, group->active->villainlevel);
    group->state = EG_AWAKE;

    // If allyGroup is still ENC_UNINIT, the mission did not assign it and we check the spawndef
    if(group->active->allyGroup == ENC_UNINIT)
        group->active->allyGroup = group->active->spawndef->encounterAllyGroup;
    // If it is still EC_UNINIT it means it was not specified by design and should default to hero
    if(group->active->allyGroup == ENC_UNINIT)
        group->active->allyGroup = ENC_FOR_HERO;
    return 1;
}

Pair* pickPercentageWeightedPair(Array* pairs);

// if the entity is not friends with the existing ai team, start a new one.
// this should work the vast majority of the time, but could cause two separate
// cliques that should be the same ai team when an enemy is added in the midst of friends.
// the existing functionality can probably have the same effect at times, anyway.
static void EncounterAIGroupTeamAdd(Entity *ent, Entity ***pTeamInfo, int ai_group)
{
    if(ai_group < 0 || !pTeamInfo)
        return;

    if(eaSize(pTeamInfo) <= ai_group)
        eaSetSize(pTeamInfo, ai_group + 1);

    if(!(*pTeamInfo)[ai_group] || !aiIsMyFriend(ent, (*pTeamInfo)[ai_group]))
        (*pTeamInfo)[ai_group] = ent;
    else
        aiTeamJoin(&ENTAI((*pTeamInfo)[ai_group])->teamMemberInfo,
        &ENTAI(ent)->teamMemberInfo);
}
#define CVG_SPAWN_LEVEL_LEEWAY 3
static void GetMatchingVillainsFromCVG( int spawnRank, int level, CompressedCVG *cvg, CVG_ExistingVillain ***matchingEVs, 
                                       PCC_Critter ***matchingPCCs, PlayerCreatedStoryArc *pArc, int ignoreLevelRestrictions, int bestMaxLevel)
{
    int k;
    for (k = 0; k < eaSize(&cvg->existingVillainList); ++k)
    {
        const VillainDef *def = villainFindByName(cvg->existingVillainList[k]->pchName);
        if (!def)
        {
            int match = 0;
            switch (spawnRank)
            {
            case VR_SMALL:
            case VR_MINION:
                if (stricmp(cvg->existingVillainList[k]->costumeIdx, "RandomMinion") == 0)
                {
                    match = 1;
                }
                break;
            case VR_LIEUTENANT:
            case VR_SNIPER:
                if (stricmp(cvg->existingVillainList[k]->costumeIdx, "RandomLt") == 0)
                {
                    match = 1;
                }
                break;
            case VR_BOSS:
                if (stricmp(cvg->existingVillainList[k]->costumeIdx, "RandomBoss") == 0)
                {
                    match = 1;
                }
                break;
            }
            if (match)
            {
                MMVillainGroup *mmGroup;
                mmGroup = playerCreatedStoryArc_GetVillainGroup( cvg->existingVillainList[k]->pchName, 
                    cvg->existingVillainList[k]->setNum );
                if (mmGroup)
                {
                    if (ignoreLevelRestrictions)
                    {
                        if (mmGroup->maxLevel > (MIN(level,(bestMaxLevel - CVG_SPAWN_LEVEL_LEEWAY))))
                        {
                            eaPush(matchingEVs, cvg->existingVillainList[k]);
                        }
                    }
                    else if ((level <= mmGroup->maxLevel) && (level >= mmGroup->minGroupLevel))
                    {
                        eaPush(matchingEVs, cvg->existingVillainList[k]);
                    }
                }
            }
        }
        else
        {
            int match = 0;
            MMVillainGroup *mmGroup = playerCreatedStoryArc_GetVillainGroup(villainGroupGetName(def->group), cvg->existingVillainList[k]->setNum);
            if (StringArrayFind(cvg->dontAutoSpawnEVs, cvg->existingVillainList[k]->pchName) == -1)
            {
                if ( def->rank == VR_PET )
                {
                    const char * pchClass = def->characterClassName;
                    if( strstriConst(pchClass, "Arch") || strstriConst(pchClass, "Monster") )
                        match = (VR_ARCHVILLAIN == spawnRank);
                    else if( strstriConst( pchClass, "Boss") )
                        match = (VR_BOSS == spawnRank);
                    else if( strstriConst( pchClass, "Lt_") )
                        match = (VR_LIEUTENANT == spawnRank);
                    else
                        match = (VR_MINION == spawnRank);
                }
                else
                {
                    match = def->rank == spawnRank;
                }
            }
            if ( match && def->levels)
            {
                if (ignoreLevelRestrictions)
                {
                    if ( def->levels[eaSize(&def->levels)-1]->level > (MIN(level,(bestMaxLevel-CVG_SPAWN_LEVEL_LEEWAY)) ) )
                    {
                        eaPush(matchingEVs, cvg->existingVillainList[k]);
                    }
                }
                else if ((level <= (MIN((def->levels[eaSize(&def->levels)-1]->level), mmGroup->maxLevel) )    )        && 
                        (level >= (MAX(def->levels[0]->level, mmGroup->minLevel))))
                {
                    eaPush(matchingEVs, cvg->existingVillainList[k]);
                }
            } 
        }
    }
    for (k = 0; k < eaiSize(&cvg->customCritterIdx); ++k)
    {
        PCC_Critter *pcc;
        int rankMatch;
        pcc = pArc->ppCustomCritters[cvg->customCritterIdx[k]-1];
        if (pcc && (StringArrayFind(cvg->dontAutoSpawnPCCs, pcc->referenceFile ) == -1))
        {
            rankMatch = 0;
            switch (spawnRank)
            {
            case VR_MINION:
                rankMatch = (pcc->rankIdx == PCC_RANK_MINION) ? 1 : 0;
                break;
            case VR_LIEUTENANT:        //    intentional fallthrough
            case VR_SNIPER:
                rankMatch = (pcc->rankIdx == PCC_RANK_LT) ? 1 : 0;
                break;
            case VR_BOSS:
                rankMatch = (pcc->rankIdx == PCC_RANK_BOSS) ? 1 : 0;
                break;
            case VR_ARCHVILLAIN:
            case VR_ARCHVILLAIN2:
                rankMatch = ( (pcc->rankIdx == PCC_RANK_ELITE_BOSS) || (pcc->rankIdx == PCC_RANK_ARCH_VILLAIN) ) ? 1 : 0;
            }
            if (rankMatch)
            {
                eaPush(matchingPCCs, pcc);
            }
        }
    }
}


static int cvgNumMissingRanksAtThisLevel( int level, CompressedCVG *cvg, PlayerCreatedStoryArc *pArc)
{
    int bNoMinion = 1, bNoLt = 1, bNoBoss = 1;
    int k;

    for (k = 0; k < eaSize(&cvg->existingVillainList); ++k)
    {
        CVG_ExistingVillain *ev = cvg->existingVillainList[k];
        const VillainDef *def = villainFindByName(ev->pchName);
        if (!def)
        {
            //    the map isn't necessarily going to spawn this villain, but at least we found the closest level 
            //    villain that can be spawned at this level (for the level check)
            MMVillainGroup *mmGroup = playerCreatedStoryArc_GetVillainGroup( ev->pchName, ev->setNum );
            if (mmGroup && ( level <= mmGroup->maxLevel ))
            {
                //    If we found a def, that means there is a villain that can be spawned
                VillainGroupEnum vgroup = StaticDefineIntGetInt(ParseVillainGroupEnum, ev->pchName);
                if ((stricmp(ev->costumeIdx, "RandomMinion") == 0 ) && villainFindByCriteria(vgroup, VR_MINION, level, 0, 0, VE_MA))
                    bNoMinion = 0;
                else if ((stricmp(ev->costumeIdx, "RandomLt") == 0) && villainFindByCriteria(vgroup, VR_LIEUTENANT, level, 0, 0, VE_MA))
                    bNoLt = 0;
                else if ((stricmp(ev->costumeIdx, "RandomBoss") == 0) && villainFindByCriteria(vgroup, VR_BOSS, level, 0, 0, VE_MA))
                    bNoBoss = 0;
            }
        }
        else
        {
            if ( level <= def->levels[eaSize(&def->levels)-1]->level )
            {
                if (StringArrayFind(cvg->dontAutoSpawnEVs, ev->pchName) == -1)
                {
                    switch( def->rank )
                    {
                    case VR_PET:
                        {
                            const char * pchClass = def->characterClassName;

                            if( strstriConst( pchClass, "Boss") )
                                bNoBoss = 0;
                            else if( strstriConst( pchClass, "Lt_") )
                                bNoLt = 0;
                            else
                                bNoMinion = 0;
                        }break;
                    case VR_BOSS: bNoBoss = 0; break;
                    case VR_LIEUTENANT: // fallthrough
                    case VR_SNIPER:    bNoLt = 0; break;
                    case VR_MINION:    bNoMinion = 0; break;
                    default: break;
                    }
                }
            }
        }
    }

    for (k = 0; k < eaiSize(&cvg->customCritterIdx); ++k)
    {
        PCC_Critter *pcc;
        pcc = pArc->ppCustomCritters[cvg->customCritterIdx[k]-1];
        if (pcc && (StringArrayFind(cvg->dontAutoSpawnPCCs, pcc->referenceFile ) == -1))
        {
            switch( pcc->rankIdx )
            {
                case PCC_RANK_ELITE_BOSS: // fallthrough                    
                case PCC_RANK_BOSS: bNoBoss = 0; break;
                case PCC_RANK_LT:    bNoLt = 0; break;
                case PCC_RANK_MINION:    bNoMinion = 0; break;
                default: break;
            }
        }
    }

    return bNoMinion + bNoLt + bNoBoss;
}


//    Custom villain groups do not change, so the categories (rank) and the category sizes shouldn't change either
//    i.e. CVG A will always have 10 minions, 5 lt's, 1 boss
//    i.e. CVG B will always have 3 minions, 10 lt's, 3 bosses
static int getCVGVillain( int matchCount, int spawnRank, CompressedCVG *cvg )
{
    int idx;
    switch ( spawnRank )
    {
    case VR_SMALL:
        idx = 0;
        break;
    case VR_MINION:
        idx = 1;
        break;
    case VR_LIEUTENANT:
        idx = 2;
        break;
    case VR_SNIPER:
        idx = 3;
        break;
    case VR_BOSS:
        idx = 4;
        break;
    case VR_ARCHVILLAIN:
        idx = 5;
        break;
    case VR_ARCHVILLAIN2:
        idx = 6;
        break;
    default:
        idx = 7;
        break;
    }
    cvg->spawnCount[idx] = ( ( cvg->spawnCount[idx] + 1) % matchCount );
    return cvg->spawnCount[idx];
}
static int CVG_getMatchingVillainList(int originalRank, int level, CompressedCVG *cvg, CVG_ExistingVillain ***matchingEVs, 
                                      PCC_Critter ***matchingPCCs, PlayerCreatedStoryArc *pArc, int ignoreLevelRestrictions, int bestMaxLevel)
{
    int spawnRank = originalRank;
    //    find down spawn matches
    while ( spawnRank >= VR_SMALL )
    {
        GetMatchingVillainsFromCVG(spawnRank, level, cvg, matchingEVs, matchingPCCs, pArc, ignoreLevelRestrictions, bestMaxLevel);
        if (eaSize(matchingEVs) + (eaSize(matchingPCCs)) > 0)
        {
            break;
        }
        else
        {
            switch (spawnRank)
            {
            case VR_SMALL:
                spawnRank = VR_NONE;
                break;
            case VR_MINION:
                spawnRank = VR_SMALL;
                break;
            case VR_LIEUTENANT:
                spawnRank = VR_MINION;
                break;
            case VR_SNIPER:
                spawnRank = VR_LIEUTENANT;
                break;
            case VR_BOSS:
                spawnRank = VR_SNIPER;
                break;
            case VR_ARCHVILLAIN:
                spawnRank = VR_BOSS;
                break;
            case VR_ARCHVILLAIN2:
                spawnRank = VR_ARCHVILLAIN;
                break;
            default:
                spawnRank = VR_BOSS;
                break;
            }
        }
    }

    if ((eaSize(matchingEVs) + eaSize(matchingPCCs)) > 0 )
        return spawnRank;

    //    spawn up
    spawnRank = originalRank;
    while ( spawnRank <= VR_ARCHVILLAIN2 )
    {
        GetMatchingVillainsFromCVG(spawnRank, level, cvg, matchingEVs, matchingPCCs, pArc, ignoreLevelRestrictions, bestMaxLevel);
        if ((eaSize(matchingEVs) + eaSize(matchingPCCs)) > 0 )
        {
            break;
        }
        else
        {
            switch (spawnRank)
            {
            case VR_SMALL:
                spawnRank = VR_MINION;
                break;
            case VR_MINION:
                spawnRank = VR_LIEUTENANT;
                break;
            case VR_LIEUTENANT:
                spawnRank = VR_SNIPER;
                break;
            case VR_SNIPER:
                spawnRank = VR_BOSS;
                break;
            case VR_BOSS:
                spawnRank = VR_ARCHVILLAIN;
                break;
            case VR_ARCHVILLAIN:
                spawnRank = VR_ARCHVILLAIN2;
                break;
            case VR_ARCHVILLAIN2:
                spawnRank = VR_BIGMONSTER;
                break;
            default:
                spawnRank = VR_SMALL;
                break;
            }
        }
    }
    return spawnRank;
}

static bool actorSpawnRequirementsMet(const Actor* a, EncounterGroup* group) 
{
    if( g_activemission ) 
    {
        int numheroes = MAX( g_activemission->difficulty.teamSize, MissionNumHeroes());

        if (group->active->spawndef->minteamsize && group->active->spawndef->maxteamsize)
            numheroes = CLAMP(numheroes, group->active->spawndef->minteamsize, group->active->spawndef->maxteamsize);

        if (a->minheroesrequired && a->minheroesrequired > numheroes ) 
            return false;
        if (a->maxheroesrequired && a->maxheroesrequired < numheroes ) 
            return false;
        if (a->exactheroesrequired && a->exactheroesrequired !=  numheroes ) 
            return false;
    }
    else
    {
        if (a->minheroesrequired && a->minheroesrequired > group->active->numheroes ) 
            return false;
        if (a->maxheroesrequired && a->maxheroesrequired < group->active->numheroes) 
            return false;
        if (a->exactheroesrequired && a->exactheroesrequired != group->active->numheroes ) 
            return false;
    }

    return true;
}
static Entity * s_forceRandomSpawn(int vgroup, int level, EncounterGroup * group, int exclusion, const char* villainclass)
{
    Entity *ent = NULL;
    int k;
    int randomSpawnRanks[5] = { VR_BOSS, VR_PET, VR_LIEUTENANT, VR_SNIPER, VR_MINION };
    int randomStart = rand();
    //    try the reasonable ranks first
    for (k = 0; (k < ARRAY_SIZE(randomSpawnRanks)) && (!ent); k++)
    {
        ent = villainCreateByCriteria(vgroup, randomSpawnRanks[(k+randomStart)%ARRAY_SIZE(randomSpawnRanks)], level, 0, group->active->bossreduction, exclusion, villainclass, 0, NULL);
    }
    if (!ent)
    {
        //    then try the more exotic ones
        int moreSpawnRanks[3] = { VR_ARCHVILLAIN2, VR_ARCHVILLAIN, VR_SMALL };
        for (k = 0; (k < ARRAY_SIZE(moreSpawnRanks)) && (!ent); k++)
        {
            ent = villainCreateByCriteria(vgroup, moreSpawnRanks[(k+randomStart)%ARRAY_SIZE(moreSpawnRanks)], level, 0, group->active->bossreduction, exclusion, villainclass, 0, NULL);
        }
    }
    return ent;
}
static void SpawnEncounterActor( EncounterGroup * group, VillainSpawnCount* spawnCounts, Entity ***pTeamInfo, int i, int *newSpawnGroup )
{
    int j;
    EncounterPoint* point;
    const SpawnDef* info = group->active->spawndef;
    const Actor* a = info->actors[i];

    point = group->children.storage[group->active->point];

    if (!actorSpawnRequirementsMet(a, group))
        return;

    // otherwise, create the actor
    for (j = 0; j < a->number; j++)
    {
        Pair* pair;
        char* NPCName;
        Entity* ent = NULL;
        GeneratedEntDef* entdef = NULL;
        int loc = 0;
        EntType ent_type = ENTTYPE_NOT_SPECIFIED;
        SpawnAreaDef* saDef = hashStackSpawnAreaDefs.storage[0];
        int level = group->active->villainlevel + a->villainlevelmod;
        char *model, *displayname, *displayGroup;
        const char *displayinfo, *villain, *villaingroup, *villainrank, *ally, *gang, *villainclass;
        int shoutrange, shoutchance, wanderrange;
        int notrequired;
        int playerCreatedGroupIndex;

        // random variables
        model            = (char*)ScriptVarsTableLookup(&group->active->vartable, a->model);
        displayname        = (char*)ScriptVarsTableLookup(&group->active->vartable, a->displayname);
        displayinfo        = ScriptVarsTableLookup(&group->active->vartable, a->displayinfo);
        displayGroup    = (char*)ScriptVarsTableLookup(&group->active->vartable, a->displayGroup);
        villain            = ScriptVarsTableLookup(&group->active->vartable, a->villain);
        villaingroup    = ScriptVarsTableLookup(&group->active->vartable, a->villaingroup);
        //we do this here and not in the vars table so we can use "random" and don't 
        //have to confuse the designers with "randomvillaingroup" or something like that
        //if no villaingroup is explicitly defined, try to use the mission default
        if ((villaingroup && stricmp("random",villaingroup)==0) || (!villaingroup || !villaingroup[0]))
        {
            if (g_activemission)
                villaingroup=villainGroupGetName(g_activemission->missionGroup);
            else if (group->active->taskvillaingroup)
                villaingroup=villainGroupGetName(group->active->taskvillaingroup);
        }
        villainrank        = ScriptVarsTableLookup(&group->active->vartable, a->villainrank);
        villainclass    = ScriptVarsTableLookup(&group->active->vartable, a->villainclass);
        ally            = ScriptVarsTableLookup(&group->active->vartable, a->ally);
        gang            = ScriptVarsTableLookup(&group->active->vartable, a->gang);
        shoutrange        = ScriptVarsTableLookupInt(&group->active->vartable, a->shoutrange);
        shoutchance        = ScriptVarsTableLookupInt(&group->active->vartable, a->shoutchance);
        wanderrange        = ScriptVarsTableLookupInt(&group->active->vartable, a->wanderrange);
        notrequired        = ScriptVarsTableLookupInt(&group->active->vartable, a->notrequired);

        if (displayname) 
        {
            //    copy the localized name into local storage for displayname
            strdup_alloca(displayname, saUtilTableLocalize(displayname, &group->active->vartable));
        }

        if (displayinfo)
        {
            const char *loc = saUtilTableLocalize(displayinfo, &group->active->vartable);

            // check to see if there is anything actually present - most of the time there is not, and we don't need to allocate
            if(loc && loc[0])
            {
                //    allocates the localized string onto the string pool
                //    sets the display info to the localized string on the string pool
                displayinfo = allocAddString(loc);
            }
            else
            {
                displayinfo = NULL;
            }
        }

        if (displayGroup)
        {
            //    copy the localized name into local storage
            strdup_alloca(displayGroup, saUtilTableLocalize(displayGroup, &group->active->vartable));
        }

        // let the mission system rename hostages
        if (model && model[0])
            MissionHostageRename(group, &displayname, &model);

        // get the location
        if (j < eaiSize(&a->locations)) 
        {
            loc = a->locations[j];
        }
        else
        {
            ErrorFilenamef(info->fileName, "EncounterSpawn: trying to access beyond end of locations\n");
            // this error should be caught during preprocessing anyway
        }

        if (memcmp(point->positions[loc], g_nullVec3, sizeof(Vec3)) == 0)
        {
            ErrorFilenamef(info->fileName, "EncounterSpawn: Encounter Point %i missing for entity in %s. Group name %s, encounter spawn name %s.\n",
                loc, info->fileName, group->geoname, point->geoname);
            continue; // try next entity
        }


        playerCreatedGroupIndex = 0;
        if (info && (info->CVGIdx || info->CVGIdx2) )
        {
            //    special case handling for rumbles =/
            if (a && a->villaingroup && ( stricmp(a->villaingroup, "VILLAIN_GROUP_2") == 0 ) )
            {
                //    if server is spawning 2nd villain group, set the CVGIndex to CVGIdx2
                //    if this is non-zero, it will spawn from the CVG2
                //    if this is zero, it will spawn from the enemy group that is supposed to spawn (normal operations)
                playerCreatedGroupIndex = info->CVGIdx2;
            }
            else
            {
                //    if there is no villain group or 
                //    if spawn is not a villain group 2    (i.e. villain group 1, or spawn that is not a rumble (boss fight) )
                playerCreatedGroupIndex = info->CVGIdx;
            }
        }
        else
        {
            //    if the spawn has a group, spawn that group
            VillainGroupEnum vgroup = StaticDefineIntGetInt(ParseVillainGroupEnum, villaingroup);
            if (vgroup <= VG_NONE && g_activemission && g_activemission->def && g_activemission->def->CVGIdx )
            {
                //    if the spawn doesnt explicitly have a group, but the mission has a cvg group, 
                //    spawn the cvg group.
                playerCreatedGroupIndex = g_activemission->def->CVGIdx;
            }
        }

        // try to create the entity somehow
        level = CLAMP(level, 1, MAX_VILLAIN_LEVEL);
        if( villain && strstr(villain, DOPPEL_NAME) )
        {
            Mat4 pos;
            VillainRank rank = StaticDefineIntGetInt(ParseVillainRankEnum, villainrank);

            createMat3YPR(pos, point->pyrpositions[loc]);
            ent = villainCreateDoppelganger( villain + strlen(DOPPEL_NAME), displayname, villaingroup, rank, villainclass, level, group->active->bossreduction, pos, 0, 0, 0);
            if (!ent)
            {
                ErrorFilenamef(info->fileName, "EncounterSpawn: couldn't make doppelganger %s in script %s\n", a->villain, info->fileName);
            }
        }
        else if (a->customCritterIdx)
        {
            int isAlly = 0;
            PCC_Critter *pcc;
            PlayerCreatedStoryArc *pArc;
            pArc = playerCreatedStoryArc_GetPlayerCreatedStoryArc(g_ArchitectTaskForce_dbid);
            pcc = pArc->ppCustomCritters[a->customCritterIdx-1];
            playerCreatedStoryArc_PCCSetCritterCostume( pcc, pArc);
            if (gang && ( stricmp("1", gang) == 0) )
            {
                isAlly = 1;
            }
            ent = villainCreatePCC( displayname, NULL, pcc,level, spawnCounts, group->active->bossreduction, isAlly);
            if (!ent)
            {
                ErrorFilenamef(info->fileName, "EncounterSpawn: couldn't find villain %s in script %s\n", a->villain, info->fileName);
            }
        }
        else if (villain && villain[0]) // by villain def first
        {
            if(a->npcDefOverride)
            {
                const NPCDef *npcDef = npcFindByIdx(a->npcDefOverride);
                const VillainDef* def = villainFindByName(villain);
                if(def)
                    ent = villainCreate(def, level, spawnCounts, group->active->bossreduction, villainclass, npcDef->name, 0, NULL);
            }
            else if(model && model[0]) // if we specified both villain and model, that means we wanted a villain with a costume override of model
            {
                const VillainDef* def = villainFindByName(villain);
                if(def)
                    ent = villainCreate(def, level, spawnCounts, group->active->bossreduction, villainclass, model, 0, NULL);
            }
            else
                ent = villainCreateByName(villain, level, spawnCounts, group->active->bossreduction, villainclass, 0, NULL);
            if ( a->customNPCCostumeIdx )
            {
                PlayerCreatedStoryArc *pArc;
                //    create a match list based on rank and level
                pArc = playerCreatedStoryArc_GetPlayerCreatedStoryArc(g_ArchitectTaskForce_dbid);
                if (a->customNPCCostumeIdx && EAINRANGE(a->customNPCCostumeIdx-1, pArc->ppNPCCostumes) )
                {
                    if (!ent->customNPC_costume)
                    {
                        ent->customNPC_costume = StructAllocRaw(sizeof(CustomNPCCostume));
                    }
                    StructCopy(ParseCustomNPCCostume, pArc->ppNPCCostumes[a->customNPCCostumeIdx-1], ent->customNPC_costume, 0,0);
                }
            }

            if (!ent)
            {
                ErrorFilenamef(info->fileName, "EncounterSpawn: couldn't find villain %s in script %s\n", villain, info->fileName);
            } 
            else if(level!=character_GetCombatLevelExtern(ent->pchar))
            {
                dbLogBug("\n(Internal)\nServer Ver:%s\nEncounterSpawn: villain %s "
                    "has no level %d, using %d\n"
                    "Group name %s, encounter spawn name %s\n"
                    "Use this to go to the spawn:\nloadmap %s\nsetpos %.2f %.2f %.2f\n\n"
                    "In script %s\nThis villain was created by name.\n@@END\n\n\n\n\n\n\n\n\n",
                    getExecutablePatchVersion("CoH Server"), villain,
                    level, character_GetCombatLevelExtern(ent->pchar),
                    group->geoname, point->geoname,
                    saUtilCurrentMapFile(),
                    point->positions[loc][0], point->positions[loc][1], point->positions[loc][2],
                    info->fileName);
            }
        }
        else if (model && model[0])    // by overriding model
        {
            // Try a lookup through the old style spawning system.
            // This is only done so that we can refer to a collection of costumes through
            // a single name.  Used for NPC costumes only.
            //        This means that the old spawning definitions takes precedence over the
            //        new npc costume definition.
            stashFindPointer( saDef->generatedEntDefNameHash, model, &entdef );
            if(entdef)
            {
                pair = pickPercentageWeightedPair(&entdef->NPCTypeName);
                NPCName = pair->value1;
            }
            else
            {
                // If the old style spawning system doesn't know what the model is,
                // try using npcCreate() directly.
                NPCName = model;
            }

            ent_type = ENTTYPE_NPC;
            ent = npcCreate(NPCName, ent_type);
            if (ent)
            {
                if (ent->pchar) {
                    character_SetLevel(ent->pchar, level - 1);
                }
            }
            else
            {
                ErrorFilenamef(info->fileName, "EncounterSpawn: couldn't find model %s in script %s\n", model, info->fileName);
            }
        }
        else if ( playerCreatedGroupIndex && !(displayname) )
        {
            CompressedCVG *cvg;
            CVG_ExistingVillain **matchingEVs;
            PCC_Critter**matchingPCCs;
            int selectedVillain;
            int testingCustomVillains = 1;
            int spawnRank;
            int originalRank;
            int missingRanks;
            int bestMaxLevel = 0;
            int k;
            PlayerCreatedStoryArc *pArc;
            //    create a match list based on rank and level
            pArc = playerCreatedStoryArc_GetPlayerCreatedStoryArc(g_ArchitectTaskForce_dbid);
            cvg = pArc->ppCustomVGs[playerCreatedGroupIndex-1];
            originalRank = StaticDefineIntGetInt(ParseVillainRankEnum, villainrank);
            eaCreate(&matchingEVs);
            eaCreate(&matchingPCCs);

            spawnRank = CVG_getMatchingVillainList(originalRank, level, cvg, &matchingEVs, &matchingPCCs, pArc, 0, 0 );
            missingRanks = cvgNumMissingRanksAtThisLevel( level, cvg, pArc );

            for (k = 0; k < eaSize(&cvg->existingVillainList); ++k)
            {
                const VillainDef *def = villainFindByName(cvg->existingVillainList[k]->pchName);
                if (def)
                {
                    if (def->levels && ( bestMaxLevel < def->levels[eaSize(&def->levels)-1]->level) )
                        bestMaxLevel = def->levels[eaSize(&def->levels)-1]->level;
                }
                else
                {
                    MMVillainGroup *mmGroup = playerCreatedStoryArc_GetVillainGroup( cvg->existingVillainList[k]->pchName, cvg->existingVillainList[k]->setNum );
                    if (mmGroup && (bestMaxLevel < mmGroup->maxLevel))
                    {
                        bestMaxLevel = mmGroup->maxLevel;
                    }
                }
            }

            //    Server couldn't find anything on the first attempt. Try ignoring the level restrictions now.
            if ( ( eaSize(&matchingEVs) + eaSize(&matchingPCCs) ) == 0  || spawnRank != originalRank )
            {                
                //    If we are going to ignore level restrictions
                //    And we are going to spawn critters under their normal range
                //    Try to only spawn the ones that are closest to the level cap
                spawnRank = CVG_getMatchingVillainList(originalRank, level, cvg, &matchingEVs, &matchingPCCs, pArc, 1, bestMaxLevel );
            }

            if ( (eaSize(&matchingEVs) + eaSize(&matchingPCCs)) > 0 )
            {
                if ( newSpawnGroup && (*newSpawnGroup) )
                {
                    int itr;
                    for (itr = 0; itr < 8; itr++)
                    {
                        cvg->spawnCount[itr] = rand();
                    }
                    (*newSpawnGroup) = 0;
                }
                selectedVillain = getCVGVillain( eaSize(&matchingEVs) + eaSize(&matchingPCCs) , spawnRank, cvg );
                if (selectedVillain < (eaSize(&matchingPCCs)) )
                {
                    PCC_Critter *pcc;
                    int isAlly = 0;
                    pcc = matchingPCCs[selectedVillain];
                    playerCreatedStoryArc_PCCSetCritterCostume( pcc, pArc);
                    if (gang && ( stricmp("1", gang) == 0) )
                    {
                        isAlly = 1;
                    }
                    ent = villainCreatePCC( NULL, cvg->displayName, pcc, level,spawnCounts, group->active->bossreduction, isAlly );
                    if (pcc->description)
                    {
                        displayinfo = pcc->description;
                    }
                    else
                    {
                        displayinfo = NULL;
                    }
                }
                else
                {
                    CVG_ExistingVillain *ev = matchingEVs[selectedVillain-(eaSize(&matchingPCCs))];
                    if (stricmp(ev->costumeIdx, "RandomBoss") == 0)
                    {
                        VillainGroupEnum vgroup = StaticDefineIntGetInt(ParseVillainGroupEnum, ev->pchName);
                        ent = villainCreateByCriteria(vgroup, VR_BOSS, level, spawnCounts, group->active->bossreduction, VE_MA, villainclass, 0, NULL);
                    }
                    else if (stricmp(ev->costumeIdx, "RandomLt") == 0)
                    {
                        VillainGroupEnum vgroup = StaticDefineIntGetInt(ParseVillainGroupEnum, ev->pchName);
                        ent = villainCreateByCriteria(vgroup, VR_LIEUTENANT, level, spawnCounts, group->active->bossreduction, VE_MA, villainclass, 0, NULL);
                    }
                    else if (stricmp(ev->costumeIdx, "RandomMinion") == 0)
                    {
                        VillainGroupEnum vgroup = StaticDefineIntGetInt(ParseVillainGroupEnum, ev->pchName);
                        ent = villainCreateByCriteria(vgroup, VR_MINION, level, spawnCounts, group->active->bossreduction, VE_MA, villainclass, 0, NULL);
                    }
                    else
                    {
                        ent = villainCreateCustomNPC(ev, level, spawnCounts, group->active->bossreduction, villainclass);
                        if (ev->description)
                        {
                            displayinfo = ev->description;
                        }
                    }
                    if (!ent)
                    {
                        ErrorFilenamef(info->fileName, "EncounterSpawn: couldn't find villain %s in script %s\n", 
                            ev->pchName, info->fileName);
                    } 
                    else if(level!=character_GetCombatLevelExtern(ent->pchar))
                    {
                        dbLogBug( "\n(Internal)\nServer Ver:%s\nEncounterSpawn: villain %s "
                            "has no level %d, using %d\n"
                            "Group name %s, encounter spawn name %s\n"
                            "Use this to go to the spawn:\nloadmap %s\nsetpos %.2f %.2f %.2f\n\n"
                            "In script %s\nThis villain was created by name.\n@@END\n\n\n\n\n\n\n\n\n",
                            getExecutablePatchVersion("CoH Server"), ev->pchName,
                            level, character_GetCombatLevelExtern(ent->pchar),
                            group->geoname, point->geoname,
                            saUtilCurrentMapFile(),
                            point->positions[loc][0], point->positions[loc][1], point->positions[loc][2],
                            info->fileName);
                    }
                    if (ent)
                    {
                        if (cvg->displayName)
                        {
                            if (ent->CVG_overrideName) StructFreeString(ent->CVG_overrideName);
                            ent->CVG_overrideName = StructAllocString(cvg->displayName);    //    does this need to be duped?
                        }
                    }
                }

                if( ent && missingRanks )
                {
                    ent->fRewardScale *= g_PCCRewardMods.fMissingRankPenalty[MIN(3,missingRanks)];
                }
                eaDestroy(&matchingPCCs);
                eaDestroy(&matchingEVs);
            }
            else
            {
                ent = NULL;
            }
        }
        else
        {
            VillainGroupEnum vgroup = StaticDefineIntGetInt(ParseVillainGroupEnum, villaingroup);
            VillainRank rank = StaticDefineIntGetInt(ParseVillainRankEnum, villainrank);
            int exclusion;
            exclusion = (group->active->allyGroup == ENC_FOR_VILLAIN)?VE_COV:VE_COH;
            if ( OnMissionMap() && g_ArchitectTaskForce)
                exclusion = VE_MA;
            // verify
            if (vgroup <= VG_NONE && group->active->villaingroup > 0)
                vgroup = group->active->villaingroup;    // can be defaulted by mission
            if ((vgroup > VG_NONE) && (rank > VR_NONE))
            {

                ent = villainCreateByCriteria(vgroup, rank, level, spawnCounts, group->active->bossreduction, exclusion , villainclass, 0, NULL);
                //    if we couldn't find someone, and we really wanted to spawn someone (as indicated by this person being named...)
                //    force something to spawn
                if (!ent && displayname)
                {
                    ent = s_forceRandomSpawn(vgroup, level, group, exclusion, villainclass);
                }
            }
            else if (vgroup > VG_NONE) // try some valid ranks
            {
                ent = s_forceRandomSpawn(vgroup, level, group, exclusion, villainclass);
            }
            else
            {
                if (villaingroup)
                    Errorf("Attempting to spawn a villaingroup (%s) that couldn't be found.", villaingroup);
                else
                    Errorf("Attempting to spawn from a NULL villain group.");
            }

            if (!ent)
            {
                if (villaingroup)
                {
                    ErrorFilenamef(info->fileName, "EncounterSpawn: couldn't find villain (%s,%s) in script %s\n", villaingroup, villainrank, info->fileName);
                }
                else
                {
                    ErrorFilenamef(info->fileName, "EncounterSpawn: couldn't find villain (NULL villaingroup,%s) in script %s\n", villainrank, info->fileName);
                }
            }
            else if(level!=character_GetCombatLevelExtern(ent->pchar))
            {
                dbLogBug( "\n(Internal)\nServer Ver:%s\nEncounterSpawn: villain (%s,%s=>%s) "
                    "has no level %d, using %d\n"
                    "Group name %s, encounter spawn name %s\n"
                    "Use this to go to the spawn:\nloadmap %s\nsetpos %.2f %.2f %.2f\n\n"
                    "In script %s\nThis encounter was spawned using group and rank criteria.\n@@END\n\n\n\n\n\n\n\n\n",
                    getExecutablePatchVersion("CoH Server"), villaingroup, villainrank, ent->villainDef->name,
                    level, character_GetCombatLevelExtern(ent->pchar),
                    group->geoname, point->geoname,
                    saUtilCurrentMapFile(),
                    point->positions[loc][0], point->positions[loc][1], point->positions[loc][2],
                    info->fileName);
            }
        }

        // we successfully created the entity
        if (ent)
        {
            Vec3 pyr;
            Mat4 pos;

            if (model)
                EncounterDebug("Using model to create a %s, named %s", model, displayname);
            else if (villain)
                EncounterDebug("Creating a specific villain %s at level %i, named %s", villain, level, displayname);
            else
                EncounterDebug("Creating a generic villain of type %s, rank %s, level %i, named %s", villaingroup, villainrank, level, displayname);

            // ground snap the entity if we are supposed to
            copyVec3(point->pyrpositions[loc], pyr);
            if (!a->nounroll)
            {
                pyr[0] = 0;
                pyr[2] = 0;
            }
            createMat3YPR(pos, pyr);
            if (a->nogroundsnap)
                copyVec3(point->positions[loc], pos[3]);
            else
                copyVec3(point->zpositions[loc], pos[3]);

            // set the entity position and orientation
            entUpdateMat4Instantaneous(ent, pos);

            // If inside a room, assume we aren't normally distance fading
            ent->fade = !group->intray;

            // random flags
            if (a->flags & ACTOR_ALWAYSCON)
                ent->alwaysCon = 1;
            if (a->flags & ACTOR_SEETHROUGHWALLS)
                ent->seeThroughWalls = 1;
            if (a->flags & ACTOR_INVISIBLE)
                SET_ENTHIDE(ent) = 1;

            if (displayname && displayname[0])
                strcpy(ent->name, displayname);

            if (displayinfo && displayinfo[0])
            {
                //    This is a pointer assign because the string is never freed off the global pool
                ent->description = displayinfo;
            }
            if (displayGroup && displayGroup[0])
            {
                if (ent->CVG_overrideName) StructFreeString(ent->CVG_overrideName);
                ent->CVG_overrideName = StructAllocString(displayGroup);
            }
            // attach entity pointers to encounter group
            ent->encounterInfo = EncounterEntityInfoAlloc();
            ent->encounterInfo->parentGroup = group;
            ent->encounterInfo->actorID = i;
            ent->encounterInfo->actorSubID = j;
            ent->encounterInfo->state = -1;    // set to invalid state, so that entry into ACT_INACTIVE is made
            ent->encounterInfo->notrequired = notrequired;
            copyVec3(point->positions[loc], ent->encounterInfo->spawnLoc);
            if (wanderrange) ent->encounterInfo->wanderRange = wanderrange;
            else ent->encounterInfo->wanderRange = EG_DEFAULT_WANDER_RANGE;

            eaPush(&group->active->ents, ent);

            // If the optional params include an alliance or a gang, override the actor's natural team
            if (ally)
            {
                if ( 0 == stricmp( ally, "Hero" ) )
                    entSetOnTeam( ent, kAllyID_Good );
                else if ( 0 == stricmp( ally, "Villain" ) )
                    entSetOnTeam( ent, kAllyID_Foul );
                else if ( 0 == stricmp( ally, "Villain2" ) )
                    entSetOnTeam( ent, kAllyID_Villain2 );
                else if ( 0 == stricmp( ally, "Monster" ) )
                    entSetOnTeam( ent, kAllyID_Evil );
            }

            // Do the mission overrides if necessary
            if (g_activemission && group->active->iscanspawn && (info->override != MO_NONE))
            {
                if (info->override == MO_MISSION)
                    entSetOnTeamMission(ent);
                else if (info->override == MO_PLAYER)
                    entSetOnTeamPlayer(ent);
            }

            if ( gang )
            {
                int gangId = 0;
                if ( 0 == stricmp( gang, "WithPlayers" ) )
                {
                    if( OnMissionMap() )
                        gangId = GANG_OF_PLAYERS_WHEN_IN_COED_TEAMUP;
                }
                else
                {
                    gangId = entGangIDFromGangName( gang );
                }
                entSetOnGang( ent, gangId );
            }
            else if( OnPlayerCreatedMissionMap() )
            {
                entSetOnGang( ent, 2 );
            }
            aiInit(ent, NULL);

            if (shoutrange)
                aiSetShoutRadius(ent, shoutrange);    // otherwise, default for entity
            if (shoutchance) 
                aiSetShoutChance(ent, shoutchance); // otherwise, default for entity
            EncounterAIGroupTeamAdd(ent, pTeamInfo, a->ai_group);

            EncounterEntUpdateState(ent, 0);
            {
                const MapSpec* spec = MapSpecFromMapId(db_state.base_map_id);
                if (spec && spec->globalAIBehaviors)
                {
                    aiBehaviorAddString(ent, ENTAI(ent)->base, spec->globalAIBehaviors);
                }
            }
            ent->shouldIConYellow = (a->conColor == CONCOLOR_YELLOW);

            ent->fRewardScaleOverride = 0.01 * a->rewardScaleOverridePct;

            ent->bitfieldVisionPhasesDefault = a->bitfieldVisionPhases;
            ent->exclusiveVisionPhaseDefault = a->exclusiveVisionPhase;
        } // have entity
    } // each entity for this actor
}

void EncounterSpawn(EncounterGroup* group)
{
    const SpawnDef* info;
    int i, n;
    EncounterPoint * point;
    VillainSpawnCount spawnCounts = {0};
    Entity **teamInfo = NULL;
    int newSpawn = 1;
    if (g_encounterScriptControlsSpawning)
    {
        // abort and clean up
        EncounterCleanup(group);
        return;
    }

    PERFINFO_AUTO_START("EncounterSpawn", 1);
    assert(group->active);
    info = group->active->spawndef;
    point = group->children.storage[group->active->point];

    DoZDrops(point);    // get the group-snap positions
    EncounterDebug("Encounter spawning at %0.2f,%0.2f,%0.2f",
        point->mat[3][0], point->mat[3][1], point->mat[3][2]);

    // iterate over all actors and place them
    group->active->villaincount = 0;
    n = eaSize(&info->actors);
    for (i = 0; i < n; i++)
    {
        SpawnEncounterActor( group, &spawnCounts, &teamInfo, i, &newSpawn );
    }

    if (eaSize(&group->active->ents))
    {
        dbLog("Encounter:Spawn", NULL, "%s", dbg_EncounterGroupStr(group));
        g_encounterNumWithSpawns++; g_encounterTotalSpawned++;
    }

    // do nictus adds if applicable
    AddNictusHunters(group);

    // finally transition actors into first state
    group->state = EG_WORKING;
    StartEncounterScripts(group);                // need to be sure the scripts have a chance to grab completion control
    encounterTransition(group, ACT_WORKING);
    EncounterCountCritters();                    // need to get this updated before we spawn the next encounter
    StartCutScenes(group);

    eaDestroy(&teamInfo);
    PERFINFO_AUTO_STOP();
}

//Any actors who are dead are respawned
//(Note this works by actor, not entity, and since mulitple entities can refer to one actor, if any of them still exist, the actor 
//is considered to still exist.  This is OK, since 99.9% of the time, it's one actor to one entity)
void EncounterRespawnActors( EncounterGroup * group )
{
    int n, i;
    int actorStillAlive;
    VillainSpawnCount spawnCounts = {0};
    Entity **teamInfo = NULL;

    if( !group || !group->active )
        return;

    n = eaSize(&group->active->spawndef->actors);
    for (i = 0; i < n; i++)
    {
        int j;
        int size = eaSize( &group->active->ents );
        actorStillAlive = 0;
        //Look through all the living entities to see if this actor is still alive

        for( j = 0 ; j < size ; j++ )
        {
            Entity * ent = group->active->ents[j];
            if( ent->encounterInfo->actorID == i )
            {
                actorStillAlive = 1;
                EncounterAIGroupTeamAdd(ent,&teamInfo,group->active->spawndef->actors[i]->ai_group);
            }
        }

        if( !actorStillAlive )
        {
            SpawnEncounterActor( group, &spawnCounts, &teamInfo, i, NULL );
        }
    }

    //Bring everyone in the group up to speed
    if( group->state == EG_INACTIVE )
        encounterTransition(group, ACT_INACTIVE );
    else if (group->state == EG_ACTIVE )
        encounterTransition(group, ACT_ACTIVE );

    //Since I don't really know which entities I just spawned, just update all of em
    for( i = 0 ; i < eaSize( &group->active->ents ) ; i++ )
    {
        EncounterEntUpdateState( group->active->ents[i], 0 );
    }

    eaDestroy(&teamInfo);
}

// Takes an encounter group and actor location and finds and respawns that individual actor
// Returns 1 if it successfully respawned the actor
int EncounterRespawnActorByLocation(EncounterGroup* group, int actorPos)
{
    VillainSpawnCount spawnCounts = {0};
    if (group && group->active && group->active->spawndef)
    {
        int actorIndex, numActors = eaSize(&group->active->spawndef->actors);
        for (actorIndex = 0; actorIndex < numActors; actorIndex++)
        {
            const Actor* currActor = group->active->spawndef->actors[actorIndex];
            if (eaiSize(&currActor->locations) && currActor->locations[0] && currActor->locations[0] == actorPos)
            {
                int newestActor;
                int i, numEnts = eaSize(&group->active->ents);

                // Find the actor in the list of ents and kill him
                for (i = 0; i < numEnts; i++)
                {
                    Entity* currEnt = group->active->ents[i];
                    if (currEnt->encounterInfo->actorID == actorIndex)
                    {
                        entFree(currEnt);
                        break;
                    }
                }

                // Respawn the actor now and setup his info
                if (group->active)
                {
                    SpawnEncounterActor(group, &spawnCounts, NULL, actorIndex, NULL);
                    newestActor = eaSize(&group->active->ents) - 1;

                    // Double check just in case the spawn failed for whatever reason
                    if (newestActor >= 0 && group->active->ents[newestActor]->encounterInfo->actorID == actorIndex)
                    {
                        entityTransition(group, group->active->ents[newestActor], group->state);
                        EncounterEntUpdateState(group->active->ents[newestActor], 0);
                        return 1;
                    }
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    return 0;
}

// shut down all ents, and clean up the encounter
void EncounterCleanup(EncounterGroup* group)
{
    EGState wasState;

    if(!group)
        return;

    if (group->state != EG_OFF) EncounterDebug("Encounter cleaning up");

    wasState = group->state;
    group->state = EG_ASLEEP;
    group->noautoclean = 0;

    PERFINFO_AUTO_START("EncounterCleanup", 1);
    EncounterCalculateAutostart(group);
    if (!group->active)    
    {
        PERFINFO_AUTO_STOP();
        return; // ok
    }
    StoryArcEncounterComplete(group, 0);
    EncounterActiveInfoDestroy(group);
    PERFINFO_AUTO_STOP();
}

static void MissionEncounterSendLocations(Entity *player)
{
    int i, incomplete=0;
    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];
        if (!group->conquered && !group->uncounted && group->state != ENCOUNTER_OFF && group->state != ENCOUNTER_INACTIVE ) 
            incomplete++;
    }

    START_PACKET(pak, player, SERVER_SEND_CRITTERLOCATIONS );
    pktSendBitsAuto(pak, db_state.map_id);
    pktSendBitsAuto(pak, incomplete);

    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];
        if (!group->conquered && !group->uncounted && group->state != ENCOUNTER_OFF && group->state != ENCOUNTER_INACTIVE) 
            pktSendVec3(pak, group->mat[3] );    
    }

    END_PACKET;
    player->sentCritterLocations = 1;
}


// call often, spawns new encounters, merger of the old IdlePoll and CheckAll
// uses a modulo method to look at odd encounters each tick
static void EncounterIdlePoll(int startindex, int increment)
{
    int i, j;
    int encounterCount;
    int spawnradius;
    unsigned int roll;
    EncounterGroup* groups[ENT_FIND_LIMIT];
    Mat4 mat;
    Vec3 vel;
    F32 inactiveSquared;

    // Calculate Once for distance check
    inactiveSquared = EG_INACTIVE_RADIUS * EG_INACTIVE_RADIUS;

    // Check all players to see if they are near encounters
    for(i = startindex; i < player_ents->count; i += increment)
    {

        if( OnMissionMap() )
        {
            if( g_bSendItemLocations == LOCATION_SEND_NEW || g_bSendItemLocations == LOCATION_SEND_NEW_SENT || 
                (g_bSendItemLocations == LOCATION_SENT && !player_ents->ents[i]->sentItemLocations) )
            {
                MissionObjectiveSendLocations(player_ents->ents[i]);
                g_bSendItemLocations = LOCATION_SEND_NEW_SENT;
            }
            if( g_bSendCritterLocations == LOCATION_SEND_NEW || g_bSendCritterLocations == LOCATION_SEND_NEW_SENT ||
                (g_bSendCritterLocations == LOCATION_SENT && !player_ents->ents[i]->sentCritterLocations) )
            {
                MissionEncounterSendLocations(player_ents->ents[i]);
                g_bSendCritterLocations = LOCATION_SEND_NEW_SENT;
            }
        }

        if (!ENTINFO(player_ents->ents[i]).has_processed)
            continue;

        if (player_ents->ents[i]->doNotTriggerSpawns)
            continue;

        copyVec3(ENTPOS(player_ents->ents[i]), mat[3]);
        scaleVec3(player_ents->ents[i]->motion->vel, EG_PLAYER_VELPREDICT, vel);
        addVec3(mat[3], vel, mat[3]);

        encounterCount = EncountersWithinDistanceMegaGrid(mat[3], EG_MAX_SPAWN_RADIUS, groups);

        for(j = 0; j < encounterCount; j++)
        {
            EncounterGroup * group = groups[j];

            if (group->manualspawn || (group->singleuse && group->conquered) || EncounterHasAutostart(group)) 
                continue;

            //If proximity doesn't trigger me, and I'm waiting for a sign
            if( group->active && group->active->spawndef && group->active->spawndef->createOnObjectiveComplete )
                continue;

//             if (group->active && group->active->spawndef)
//                 group->active->spawndef->respawnTimer = 5; // hack for testing

            if (group->state == EG_ASLEEP
                || (group->state == EG_COMPLETED && group->active && group->active->spawndef
                    && group->active->spawndef->respawnTimer != -1))
            {
                F32 distSq;

                if (group->state == EG_COMPLETED)
                {
                    U32 now = timerSecondsSince2000();
                    if (now >= group->respawnTime)
                    {
                        group->respawnTime += group->active->spawndef->respawnTimer; // in case we need to try again
                    }
                    else
                    {
                        continue;
                    }
                }

                if(TooManyCritters())
                {
                    group->state = EG_OFF;
                    continue;
                }

                if(!group->active)
                    EncounterNewSpawnDef(group, NULL, NULL);

                if(!group->active)
                    continue;

                //If proximity doesn't trigger me, and I'm waiting for a sign
                if( group->active && group->active->spawndef && group->active->spawndef->createOnObjectiveComplete )
                    continue;

                spawnradius = CorrectSpawnRadius(group->active->spawndef->spawnradius);

                distSq = spawnradius * spawnradius;

                if (distance3Squared(mat[3], group->mat[3]) <= distSq)
                {
                    F32 servertime = server_visible_state.time;
                    int timeok = 0;
                    int villainDistOk = 1;
                    int spawnprob;

                    // probability roll
                    roll = 1 + (rand() % 100);

                    // check time
                    timeok = servertime >= group->time_start && servertime < group->time_start + group->time_duration;
                    timeok = timeok || (servertime < group->time_start + group->time_duration - 24.0);

                    // check villain distances
                    if (group->usevillainradius && group->villainradius && !group->hasautospawn && !group->ignoreradius && !(group->active->spawndef->flags & SPAWNDEF_IGNORE_RADIUS)) // need check
                    {
                        int villainradius = CorrectVillainRadius(group->villainradius);
                        if (EncountersRunningInRadius(group, group->mat[3], villainradius))
                        {
                            villainDistOk = 0;
                            EncounterDebug("Other encounter too close");
                        }
                    }

                    spawnprob = group->spawnprob;
                    if (spawnprob)
                        spawnprob = CLAMP(group->spawnprob + g_encounterAdjustProb, 1, 100);
                    if (!group->active->spawndef->deprecated && (group->active->mustspawn || g_encounterAlwaysSpawn || (roll <= (U32)spawnprob && timeok && villainDistOk)))
                    {        
                        //If I'm triggered by someone hitting my volume, wait for that
                        if( group->volumeTriggered )
                            group->active->waitingForVolumeTrigger = 1;
                        else
                        {
                            if (group->state == EG_COMPLETED)
                            {
                                EncounterCleanup(group);
                                EncounterNewSpawnDef(group, NULL, NULL);
                            }
                            EncounterSpawn(group);
                        }
                    }
                    else if (group->state != EG_COMPLETED)
                    {
                        // don't turn respawners off, let them try again later
                        group->state = EG_OFF;
                        group->conquered = 1;
                        EncounterDebug("Encounter turning off");
                    }
                }
            }
            if (group->state == EG_WORKING)
            {
                assert(group->active);
                if (distance3Squared(mat[3], group->mat[3]) <= inactiveSquared)
                {
                    group->state = EG_INACTIVE;
                    encounterTransition(group, ACT_INACTIVE);
                }
            }
        }
    }

    if( g_bSendItemLocations == LOCATION_SEND_NEW_SENT )
        g_bSendItemLocations = LOCATION_SENT;
    if( g_bSendCritterLocations == LOCATION_SEND_NEW_SENT )
        g_bSendCritterLocations = LOCATION_SENT;
}

// Starts all the auto-spawn encounters
static void EncounterStartAuto(int startindex, int increment)
{
    int i;

    // just check points that are running to see if they should clean up
    for (i = startindex; i < g_encountergroups.size; i += increment)
    {
        EncounterGroup* group = g_encountergroups.storage[i];

        // Handle Auto Start Spawns that don't need a radius check
        if ((!EM_IGNOREEVENTS || (group->missionspawn && group->missionspawn->flags & SPAWNDEF_MISSIONRESPAWN)) &&  
            group->state == EG_OFF && 
            (EncounterHasAutostart(group) || TooFewCritters(0)))
        {
            EncounterCleanup(group); // autostart groups can clean up themselves once turned off
        }
        else if (group->state == EG_ASLEEP && (EncounterNeedAutostart(group) || TooFewCritters(0)))
        {
            F32 servertime = server_visible_state.time;
            int timeok = 0;
            timeok = servertime >= group->time_start && servertime < group->time_start + group->time_duration;
            timeok = timeok || (servertime < group->time_start + group->time_duration - 24.0);
            if (timeok)
            {
                EncounterNewSpawnDef(group, NULL, NULL);
                if (group->active && !group->active->spawndef->deprecated && !(group->missionspawn && group->missionspawn->createOnObjectiveComplete) )
                    EncounterSpawn(group);
            }
        }
    }
}

// call every 5 mins or so to look for encounter points to clean up
// uses a modulo method to look at odd encounters each tick
static void EncounterCheckCleanup(int startindex, int increment)
{
    int i;

    if (!EM_RESPAWN) return;

    // just check points that are running to see if they should clean up
    for (i = startindex; i < g_encountergroups.size; i += increment)
    {
        int playernear = 0;
        int e, n;
        EncounterGroup* group = g_encountergroups.storage[i];

        if (group->manualspawn || group->noautoclean || group->singleuse || group->hasautospawn ) 
            continue;
        //MW per mark I added hasautospawn to the things that prevent autoclean up based on player proximity

        if (group->state == EG_WORKING || group->state == EG_INACTIVE ||
            group->state == EG_ACTIVE || group->state == EG_COMPLETED ||
            group->state == EG_OFF)
        {
            // we may be off without every having gone active
            if (group->state == EG_OFF && !group->active)
            {
                // check to see if any player is too close too group
                if (!saUtilPlayerWithinDistance(group->mat, EG_PLAYER_RADIUS, NULL, EG_PLAYER_VELPREDICT))
                    group->state = EG_ASLEEP;
                continue;
            }

            // check if cleaning up the encounter would bring us below min critters
            n = eaSize(&group->active->ents);
            if (TooFewCritters(n + 5)) break; // avoid cycling much

            // otherwise, we must be active to be here
            assert(group->active);

            // check to see if any player is within the spawn radius of any entity
            for (e = 0; e < n; e++)
            {
                if (saUtilPlayerWithinDistance(ENTMAT(group->active->ents[e]), EG_MAX_SPAWN_RADIUS, NULL, EG_PLAYER_VELPREDICT))
                {
                    playernear = 1;
                    break;
                }
            }

            // also check to see if player is close to the center of the encounter (may not be any ents left)
            if (!playernear && saUtilPlayerWithinDistance(group->mat, EG_MAX_SPAWN_RADIUS, NULL, EG_PLAYER_VELPREDICT))
                playernear = 1;

            if (!playernear) 
                EncounterCleanup(group);
        }
    }
}

static int processtick = 0;

// EncounterProcess is called in the main map loop.
// It calls EncounterCheckCleanup, EncounterStartAuto,
// and EncounterIdlePoll at varying frequencies.
// EncounterIdlePoll is called pretty often, it checks
// encounters that are within the spawn radius.
// It decides when to set off an encounter.
// EncounterStartAuto is called less frequently and checks
// all the autospawns. EncounterCheckCleanup isn't called
// often at all. It checks encounters that have already
// gone off to see if they can be cleaned up.
void EncounterProcess(void)
{
    if (!g_encounterAllowProcessing || server_state.validate_spawn_points) return;
    processtick++;

    if (!(processtick % 60))
    {
        EncounterCountCritters();
        EncounterCalcPanic();
    }

    PERFINFO_AUTO_START("EncounterCheckCleanup", 1);
    EncounterCheckCleanup(processtick % 3000, 3000);    // 0 - 1.5 mins
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("EncounterStartAuto", 1);
    EncounterStartAuto(processtick % 150, 150);            // 0 - 5 secs
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("EncounterIdlePoll", 1);
    EncounterIdlePoll(processtick % 10, 10);            // 0 - 0.3 secs
    PERFINFO_AUTO_STOP();
}

int EncounterAllowProcessing(int on)
{
    int was = g_encounterAllowProcessing;
    if (on >= 0) g_encounterAllowProcessing = on;
    return was;
}

// *********************************************************************************
//  Encounter panic system
// *********************************************************************************

typedef struct PanicAdjustment {
    int playerradius;
    int villainradius;
    int adjustprob;
} PanicAdjustment;

PanicAdjustment panicTable[] =
{
    // adjust spawn probability first
    { 200, 125, 0 },
    { 200, 125, 10 },
    { 200, 125, 20 },    // most encounters at 80% prob
    { 200, 125, 100 },    // we don't want to ratchet through 30-90

    // then adjust the distance between spawns
    { 200, 125, 100 },
    { 200, 120, 100 },
    { 200, 115, 100 },
    { 200, 110, 100 },
    { 200, 105, 100 },
    { 200, 100, 100 },
    { 200, 95, 100 },
    { 200, 90, 100 },
    { 200, 85, 100 },
    { 200, 80, 100 },
    { 200, 75, 100 },
    { 200, 70, 100 },
    { 200, 65, 100 },
    { 200, 60, 100 },
    { 200, 55, 100 },
    { 200, 50, 100 },

    // finally, we have to adjust the spawn distance :-(
    { 195, 50, 100 },
    { 190, 50, 100 },
    { 185, 50, 100 },
    { 180, 50, 100 },
    { 175, 50, 100 },
    { 170, 50, 100 },
    { 165, 50, 100 },
    { 160, 50, 100 },
    { 155, 50, 100 },
    { 150, 50, 100 },
    { 145, 50, 100 },
    { 140, 50, 100 },
    { 135, 50, 100 },
    { 130, 50, 100 },
    { 125, 50, 100 },
    { 120, 50, 100 },
    { 115, 50, 100 },
    { 110, 50, 100 },
    { 105, 50, 100 },
    { 100, 50, 100 },

    // below here, I think we're just killing gameplay for very little incremental benefit
};

// we adjust the panic level up or town one notch at a time, based on ratio
// of active encounters to spawned encounters
static void EncounterCalcPanic()
{
    // preconditions to panic at all
    if (!g_panicSystemOn || (player_count < g_panicMinPlayers))
    {
        g_panicLevel = 0;
        g_encounterPlayerRadius = panicTable[0].playerradius;
        g_encounterVillainRadius = panicTable[0].villainradius;
        g_encounterAdjustProb = panicTable[0].adjustprob;
        return;
    }

    // bump up or down
    if (!g_encounterNumActive)
    {
        g_panicLevel++;
    }
    else
    {
        F32 activeRatio = (float)g_encounterNumWithSpawns / (float)g_encounterNumActive;
        if (activeRatio < g_panicThresholdLow)
            g_panicLevel++;
        else if (activeRatio > g_panicThresholdHigh)
            g_panicLevel--;
    }
    g_panicLevel = CLAMP(g_panicLevel, 0, ARRAY_SIZE(panicTable)-1);

    // adjust
    g_encounterPlayerRadius = panicTable[g_panicLevel].playerradius;
    g_encounterVillainRadius = panicTable[g_panicLevel].villainradius;
    g_encounterAdjustProb = panicTable[g_panicLevel].adjustprob;
}

// *********************************************************************************
//  Encounter group names & handles
// *********************************************************************************

// layout will optionally narrow the selection, if multiple selections are possible,
// a random result is returned
EncounterGroup* EncounterGroupByName(const char* groupname, const char* layout, int inactiveonly)
{
    int* grouplist;
    int sel, i;
    EncounterGroup* result = NULL;

    eaiCreate(&grouplist);
    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];
        if (!group->groupname) continue;
        if (stricmp(group->groupname, groupname)) continue;
        if (inactiveonly &&
            (group->state == EG_WORKING || group->state == EG_INACTIVE ||
            group->state == EG_ACTIVE || group->state == EG_COMPLETED)) continue;
        if (layout &&
            EncounterGetMatchingPoint(group, layout) == -1) continue;
        eaiPush(&grouplist, i);
    }
    if (eaiSize(&grouplist))
    {
        sel = rule30Rand() % eaiSize(&grouplist);
        result = g_encountergroups.storage[grouplist[sel]];
    }
    eaiDestroy(&grouplist);
    return result;
}




// Returns a list of all EncounterGroups of a given description 
static int EncounterGroupByNameAll(char* groupname, const char* layout, int flags, EncounterGroup * groupsFound[] )
{
#define MAX_FOUND 256
    int foundCount = 0, i;

    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];
        if (!group->groupname) 
            continue;
        if (stricmp(group->groupname, groupname)) 
            continue;
        if ( (flags & INACTIVE_ONLY) && 
            (group->state == EG_WORKING || group->state == EG_INACTIVE ||    
            group->state == EG_ACTIVE || group->state == EG_COMPLETED) ) 
            continue;
        if ( (flags & ACTIVE_ONLY) &&
            (group->state != EG_WORKING && group->state != EG_INACTIVE &&    
            group->state != EG_ACTIVE && group->state != EG_COMPLETED) ) 
            continue;
        if (layout && 
            EncounterGetMatchingPoint(group, layout) == -1) 
            continue;

        if( foundCount < MAX_FOUND )
            groupsFound[ foundCount++ ] = group;
        else
            Errorf( "Too many encounter groups of type %s found on the map.  Why are you asking for so many", groupname ); 
        //TO DO : be a bit more elegant and use dynarray.

    }

    return foundCount;
}

typedef struct GroupListSp
{
    int idx;   //index into groups list
    int count; //number of players near this group
} GroupListSp;
//
int sortGroupListSp(GroupListSp *a,GroupListSp *b)
{
    if( a->count == b->count )
        return( a->idx > b->idx );
    return( a->count > b->count );
}


// layout will optionally narrow the selection, if multiple selections are possible,
// a random result is returned
EncounterGroup* EncounterGroupByRadius(Vec3 pos, int radius, const char* layout, int inactiveonly, int nearbyPlayers, int innerRadius)
{
    int* grouplist;
    EncounterGroup* result = NULL;
    EncounterGroup* groups[ENT_FIND_LIMIT];
    int count, i, sel;

    count = EncountersWithinDistance(pos, radius, groups);
    eaiCreate(&grouplist);
    for (i = 0; i < count; i++)
    {
        EncounterGroup* group = groups[i];
        if (innerRadius &&
            distance3Squared(groups[i]->mat[3], pos) <= SQR( innerRadius )) //dont get groups closer than this ( if you are going for a doughnut of spawns )
            continue;
        if (inactiveonly &&
            (group->state == EG_WORKING || group->state == EG_INACTIVE ||
            group->state == EG_ACTIVE || group->state == EG_COMPLETED)) 
            continue;
        if (layout && EncounterGetMatchingPoint(group, layout) == -1) 
            continue;

        { //TO DO ???
            int j, foundAlready = 0;
            for( j = 0 ; j < eaiSize(&grouplist) ; j++ )
            {
                if( group == groups[grouplist[j]] )
                    foundAlready = 1;
            }
            if( foundAlready )
                continue;
        }
        eaiPush(&grouplist, i);
    }
    if (eaiSize(&grouplist))
    {
        int groupListSize = eaiSize(&grouplist);

        if( nearbyPlayers == NEARBY_PLAYERS_OK )
        {
            sel = rule30Rand() % groupListSize;
        }
        if( nearbyPlayers == FEWEST_NEARBY_PLAYERS )
        {
            int i;
            int lowestPlayerCount;
            int numberOfGroupsWithLowestPlayerCount;
            GroupListSp nearbyPlayerCountList[ENT_FIND_LIMIT];

            for (i = 0; i < groupListSize; i++)
            {
                nearbyPlayerCountList[i].idx   = i;
                nearbyPlayerCountList[i].count = saUtilPlayerWithinDistance( groups[grouplist[i]]->mat, EG_INACTIVE_RADIUS, NULL, EG_PLAYER_VELPREDICT);
            }
            qsort( nearbyPlayerCountList, groupListSize, sizeof(GroupListSp), (int (*) (const void *, const void *))sortGroupListSp );

            lowestPlayerCount =  nearbyPlayerCountList[0].count;
            numberOfGroupsWithLowestPlayerCount = 0;
            for( i = 0 ; i < groupListSize; i++ )
            {
                if( nearbyPlayerCountList[i].count == lowestPlayerCount )
                    numberOfGroupsWithLowestPlayerCount++;
                else
                    break;
            }
            sel = nearbyPlayerCountList[rule30Rand() % numberOfGroupsWithLowestPlayerCount].idx;

        }

        result = groups[grouplist[sel]];
    }
    eaiDestroy(&grouplist);
    return result;
}

// for debug purposes
void outputEncounterGroups()
{
    int encounterIndex;

    printf("g_encountergroups.size == %d\n", g_encountergroups.size);
    for (encounterIndex = 0; encounterIndex < g_encountergroups.size; encounterIndex++)
    {
        EncounterGroup *group = g_encountergroups.storage[encounterIndex];
        printf("group[%d]->geoname == %s\n", encounterIndex, group->geoname);
        printf("group[%d]->objectiveInfo == %s\n", encounterIndex, group->objectiveInfo ? group->objectiveInfo->name : 0);
    }
}

ScriptVarsTable* EncounterVarsFromEntity( Entity * e )
{
    if(SAFE_MEMBER3(e, encounterInfo, parentGroup, active))
        return &e->encounterInfo->parentGroup->active->vartable;
    else
        return NULL;
}

//TO DO : make the above functions go through this function

//Gets all ancounter groups with the given parameters
// layout will optionally narrow the selection, if multiple selections are possible,
// results must be as big as ENT_FIND_LIMIT
//TO DO Errar Int makes function more complex than needed, just make pointer list
int FindEncounterGroups( EncounterGroup *results[], 
                        Vec3 center,            //Center point to start search from
                        F32 innerRadius,        //No nearer than this
                        F32 outerRadius,        //No farther than this
                        const char * area,        //Inside this area
                        const char * layout,    //Layout required in the EncounterGroup
                        const char * groupname,    //Name of the encounter group ( Property GroupName )
                        SEFindEGFlags flags,
                        const char * missionEncounterName )   //Logical name of the mission encounter spawndef attached to this group 
{
    int* grouplist;
    EncounterGroup* groups[ENT_FIND_LIMIT];
    int count, i;
    int groupListSize = 0;

    //Gather nearby groups, or just get 'em all
    if( center && outerRadius )
        count = EncountersWithinDistance(center, outerRadius, groups);
    else
        count = g_encountergroups.size;

    //Figure out which ones you'd like
    eaiCreate(&grouplist);
    for (i = 0; i < count; i++)
    {
        EncounterGroup* group;

        if( center && outerRadius )
            group = groups[i];
        else
            group = g_encountergroups.storage[i];

        if ( center && innerRadius &&
            distance3Squared(groups[i]->mat[3], center) <= SQR( innerRadius )) //dont get groups closer than this ( if you are going for a doughnut of spawns )
            continue;

        if ((group->state == EG_WORKING || group->state == EG_INACTIVE ||
            group->state == EG_ACTIVE))
        {
            if ( SEF_UNOCCUPIED_ONLY & flags )
                continue;
        } else {
            if ( SEF_OCCUPIED_ONLY & flags )
                continue;
        }

        if (layout && EncounterGetMatchingPoint(group, layout) == -1) 
            continue;
        if( groupname ) 
        {
            if( !group->groupname )
                continue;
            if( 0 != stricmp( groupname, group->groupname ) )
                continue;
        }

        if( missionEncounterName )
        {
            if(!group->missionspawn )
                continue;
            if( !group->missionspawn->logicalName )
                continue;
            if( 0 != stricmp( missionEncounterName, group->missionspawn->logicalName ) )
                continue;
        }

        if (SEF_GENERIC_ONLY & flags)
        {
            if (group && group->missionspawn)
                continue;
        }

        { //TO DO why am I getting repeats?
            int j, foundAlready = 0;
            for( j = 0 ; j < eaiSize(&grouplist) ; j++ )
            {
                if( center && outerRadius )
                {
                    if( group == groups[grouplist[j]] )
                        foundAlready = 1;
                }
                else
                {
                    if( group == g_encountergroups.storage[grouplist[j]] )
                        foundAlready = 1;
                }
            }
            if( foundAlready )
                continue;
        }

        if( area && 0 != stricmp( area, LOCATION_NONE ) )
        {
            if( !PointInArea( group->mat[3], area, 0 ) )
                continue;
        }

        //Only do these checks here if we need the whole list checked (If we only need one decent result, wait on this check till later because it's expensive )
        if( flags & SEF_FULL_LIST )
        {
            //////Here do the copy and paste of the code you avoided up there for performance reasons:
            if( center && (flags & (SEF_HAS_PATH_TO_LOC | SEF_HAS_FLIGHT_PATH_TO_LOC)) )
            {
                int j;
                bool pathFound = 0;
                EncounterPoint* point = group->children.storage[0];
                //find some spawn location in the encounter group
                for ( j = 0 ; j < EG_MAX_ENCOUNTER_POSITIONS; j++ )
                {
                    if ( !vec3IsZero(point->zpositions[j]) )
                    {
                        if( beaconPathExists( point->zpositions[j], center, (flags & SEF_HAS_FLIGHT_PATH_TO_LOC), 20.0, 1 ) )
                            pathFound = 1;
                        break;
                    }
                }
                if( !pathFound )
                    continue;
            }
            /////End copy and paste
        }

        eaiPush(&grouplist, i);
    }

    groupListSize = eaiSize(&grouplist); 

    //Sort if needed
    if (groupListSize && (flags & SEF_SORTED_BY_NEARBY_PLAYERS) )
    {
        int i;
        GroupListSp nearbyPlayerCountList[ENT_FIND_LIMIT];

        for (i = 0; i < groupListSize; i++)
        {
            EncounterGroup* group;

            if( center && outerRadius )
                group = groups[grouplist[i]];
            else
                group = g_encountergroups.storage[grouplist[i]];

            nearbyPlayerCountList[i].idx   = i;
            nearbyPlayerCountList[i].count = saUtilPlayerWithinDistance( group->mat, EG_INACTIVE_RADIUS, NULL, EG_PLAYER_VELPREDICT);

        }
        qsort( nearbyPlayerCountList, groupListSize, sizeof(GroupListSp), (int (*) (const void *, const void *))sortGroupListSp );
    }



    //Write to final list
    if(flags & SEF_FULL_LIST)
    {
        groupListSize = MIN( groupListSize, ENT_FIND_LIMIT );

        for( i = 0 ; i < groupListSize ; i++ )
        {
            if( center && outerRadius )
                results[i] = groups[grouplist[i]];
            else
                results[i] = g_encountergroups.storage[grouplist[i]];
        }
    }
    else
    {
        EncounterGroup * group = 0;
        int found = 0;
        for( i = 0 ; i < groupListSize ; i++ )
        {
            if( center && outerRadius )
                group = groups[grouplist[i]];
            else
                group = g_encountergroups.storage[grouplist[i]];

            //////Here do the copy and paste of the code you avoided up there for performance reasons:
            //If I spawned guys here, would they be able to get to the objective?
            if( center && (flags & (SEF_HAS_PATH_TO_LOC | SEF_HAS_FLIGHT_PATH_TO_LOC)) )
            {
                int j;
                bool pathFound = 0;
                EncounterPoint* point = group->children.storage[0];
                //find some spawn location in the encounter group
                for ( j = 0 ; j < EG_MAX_ENCOUNTER_POSITIONS; j++ )
                {
                    if ( !vec3IsZero(point->zpositions[j]) )
                    {
                        if( beaconPathExists( point->zpositions[j], center, (flags & SEF_HAS_FLIGHT_PATH_TO_LOC), 20.0, 1 ) )
                            pathFound = 1;
                        break;
                    }
                }
                if( !pathFound )
                    continue;
            }
            //TO DO area search?

            /////End copy and paste
            results[0] = group;
            groupListSize = 1;
            found = 1;
        }
        if (!found)
            groupListSize = 0;
    }

    eaiDestroy(&grouplist);
    return groupListSize;
}

// just get the index of the encounter group, not valid if the encounters
// are reloaded
int EncounterGetHandle(EncounterGroup* group)
{
    int i;
    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* g = (EncounterGroup*)g_encountergroups.storage[i];
        if (g == group) return i+1;
    }
    return 0;
}

EncounterGroup* EncounterGroupFromHandle(int handle)
{
    if (handle < 1 || handle > g_encountergroups.size) return NULL;
    return g_encountergroups.storage[handle-1];
}



//TO DO right now all cut scenes start when the encounter starts
static void StartCutScenes( EncounterGroup * group )
{
    int h,i, n;
    CutSceneParams csp;

    n = eaSize(&group->active->spawndef->cutSceneDefs);

    for (h = 0; h < n; h++)
    {
        memset(&csp, 0, sizeof( CutSceneParams ) );

        //Cut Scene to play
        csp.cutSceneDef = group->active->spawndef->cutSceneDefs[h];

        //Give the cutScene all the actors
        //TO DO naming is there actor name, while isn't flexible enough?
        for (i = 0; i < eaSize(&group->active->ents); i++) 
        {
            char * name;
            csp.cast[csp.castCount].entRef = erGetRef(group->active->ents[i]);
            name = EncounterActorNameFromActor(group->active->ents[i]);
            if( name )
                strncpy( csp.cast[csp.castCount].role, name, MAX_ROLE_STR_LEN-1 );

            csp.castCount++;

            assert( csp.castCount < MAX_ACTORS ); //cheesy //TO DO make this Earray 
        }


        //If a camera mat point has been defined in the group (with property CutSceneCamera)
        //Use that point.  //TO DO dynamic?
        {
            EncounterPoint * point;
            int pidx = group->active->point;
            if (pidx < 0 || pidx >= group->children.size)
                assert(0);
            point = group->children.storage[pidx];
            if( point)
            {
                int i;
                for (i = 0; i <= MAX_CAMERA_MATS && i <= EG_MAX_ENCOUNTER_POSITIONS ; i++) 
                {
                    Vec3 pyr;
                    copyVec3( point->pyrpositions[i], pyr );
                    pyr[0] = -pyr[0]; //Camera points the wrong way
                    pyr[1] = addAngle( pyr[1], RAD(180) ); //Camera points the wrong way
                    pyr[2] = 0; //camera never rolls
                    createMat3YPR( csp.cameraMats[i], pyr );
                    copyVec3( point->positions[i], csp.cameraMats[i][3] );
                    //copyMat4( csp.cameraMats[i], point->cutSceneCameraMats[i] );
                }
            }
        }

        //TO DO make this a param
        csp.freezeServerAndMoveCameras = 1;

        group->cutSceneHandle = startCutScene( &csp );
    }
}

// Debug Command, runs through all encounters and starts the cutscene
void ShowCutScene(Entity* player)
{
    int i;

    if (!OnMissionMap())
        return;

    // Reinitialize the mission
    MissionLoad(1, db_state.map_id, db_state.map_name);
    MissionForceUninit();
    MissionForceReinit(player);

    // Search the encounter groups for a cutscene and run it if it finds one
    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];
        if (group->missionspawn && eaSize(&group->missionspawn->cutSceneDefs))
        {
            EncounterNewSpawnDef(group, NULL, NULL);
            EncounterSpawn(group);
        }
    }
}

static void StartEncounterScripts(EncounterGroup* group)
{
    int i, n;

    group->scriptControlsCompletion = false;

    n = eaSize(&group->active->spawndef->scripts);
    for (i = 0; i < n; i++)
    {
        const ScriptDef* script = group->active->spawndef->scripts[i];
        if (!group->active->scriptids) 
            eaiCreate(&group->active->scriptids);
        ScriptVarsTablePushScope(&group->active->vartable, &script->vs);
        eaiPush(&group->active->scriptids,
            ScriptBegin(script->scriptname, &group->active->vartable, SCRIPT_ENCOUNTER, group, group->active->spawndef->fileName, group->active->spawndef->fileName));
        ScriptVarsTablePopScope(&group->active->vartable);
    }

    if (n > 0)
        group->scriptHasNotHadTick = true;
}

// For debugging, add an script to an active encounter
U32 StartEncounterScriptFromDef(const ScriptDef *script, EncounterGroup* group)
{
    U32 scriptId = 0;
    if(!script || !group || !group->active || !group->active->spawndef)
    {
        Errorf("Invalid parameters or encounter not active in StartEncounterScriptFromDef");
        return scriptId;
    }

    ScriptVarsTablePushScope(&group->active->vartable, &script->vs);
    scriptId = ScriptBegin(script->scriptname, &group->active->vartable, SCRIPT_ENCOUNTER, group, "StartEncounterScriptFromDef", group->active->spawndef->fileName);
    if(scriptId > 0)
        eaiPush(&group->active->scriptids, scriptId);
    else
        Errorf("Error starting script %s on encounter %s", script->scriptname, group->groupname ? group->groupname : "Unknown");
    ScriptVarsTablePopScope(&group->active->vartable);

    return scriptId;
}

// For debugging, add an script to an active encounter
U32 StartEncounterScriptFromFunction(const char *scriptFunction, EncounterGroup* group)
{
    U32 scriptId = 0;
    if(!scriptFunction || !group || !group->active || !group->active->spawndef)
    {
        Errorf("Invalid parameters or encounter not active in StartEncounterScriptFromFunction");
        return scriptId;
    }

    scriptId = ScriptBegin(scriptFunction, &group->active->vartable, SCRIPT_ENCOUNTER, group, "StartEncounterScriptFromFunction", group->active->spawndef->fileName);
    if(scriptId > 0)
        eaiPush(&group->active->scriptids, scriptId);
    else
        Errorf("Error starting script %s on encounter %s", scriptFunction, group->groupname ? group->groupname : "Unknown");
    return scriptId;
}

static void StopEncounterScripts(EncounterGroup* group)
{
    int i, n;

    n = eaiSize(&group->active->scriptids);
    for (i = n-1; i >= 0; i--)
    {
        ScriptEnd(group->active->scriptids[i], 0);
    }
    eaiSetSize(&group->active->scriptids, 0);
}

void EncounterNotifyScriptStopping(EncounterGroup* group, int scriptid)
{
    int i, n;

    if (!group || !group->active)
    {
        // ok, since the script system expects this now when an encounter closes a script
        //        Errorf("Encounter told that a script is stopping, but it isn't bound to an encounter");
        return;
    }
    n = eaiSize(&group->active->scriptids);
    for (i = n-1; i >= 0; i--)
    {
        if (group->active->scriptids[i] == scriptid)
        {
            eaiRemove(&group->active->scriptids, i);
            return;
        }
    }
}

// *********************************************************************************
//  Debug stuff
// *********************************************************************************

// this is no longer a debug only command, it may be used to shut down
// encounters if the server goes into a panic
void EncounterForceReset(ClientLink* client, char* groupname)
{
    int i;

    // no groupname means perform operation globally
    if (!groupname || !groupname[0] || !stricmp(groupname, "all"))
    {
        // clean up every group and put asleep
        for (i = 0; i < g_encountergroups.size; i++)
        {
            EncounterGroup* group = g_encountergroups.storage[i];

            if (group->state == EG_WORKING || group->state == EG_INACTIVE ||
                group->state == EG_ACTIVE || group->state == EG_COMPLETED ||
                group->state == EG_OFF)
            {
                EncounterCleanup(group);
            }
            if (OnMissionMap())
            {
                RoomInfo* roominfo;
                roominfo = RoomGetInfoEncounter(group);

                if (!group->missionspawn && roominfo == g_rooms[0]) // not in a tray
                {
                    group->conquered = 1;
                }
            }
        }
        processtick = 10000; // force a clean
    }
    else // have a specific groupname
    {
        EncounterGroup* group = EncounterGroupByName(groupname, NULL, 0);
        if (!group)
        {
            conPrintf(client, "ERROR: Could not find group %s", groupname);
        }
        else
        {
            if (group->state == EG_WORKING || group->state == EG_INACTIVE ||
                group->state == EG_ACTIVE || group->state == EG_COMPLETED ||
                group->state == EG_OFF)
            {
                EncounterCleanup(group);
            }
        }
        if (OnMissionMap())
        {
            RoomInfo* roominfo;
            roominfo = RoomGetInfoEncounter(group);

            if (!group->missionspawn && roominfo == g_rooms[0]) // not in a tray
            {
                group->conquered = 1;
            }
        }
    }
}

void EncounterForceSpawnGroup(ClientLink* client, EncounterGroup* group)
{
    if (group == NULL || (s_respectManualSpawn && group->manualspawn))
    {
        return;
    }

    if (group->state == EG_WORKING || group->state == EG_INACTIVE ||
        group->state == EG_ACTIVE || group->state == EG_COMPLETED)
    {
        conPrintf(client, "ERROR: encounter group %s currently active", group->groupname);
    }
    else
    {
        if (group->state == EG_OFF) EncounterCleanup(group);
        if (EncounterNewSpawnDef(group, NULL, NULL))
            EncounterSpawn(group);
    }
}

void EncounterForceSpawn(ClientLink* client, char* groupname)
{
    EncounterGroup * groupsFound[MAX_FOUND];
    int count, i;

    count = EncounterGroupByNameAll( groupname, 0, 0, groupsFound );

    if (!count)
    {
        conPrintf(client, "ERROR: Could not find group %s", groupname);
    }
    else
    {
        for( i = 0 ; i < count ; i++ )
        {
            EncounterForceSpawnGroup(client, groupsFound[i]);
        }
    }
}

// Force spawns the closest encounter to the player
void EncounterForceSpawnClosest(ClientLink* client)
{
    EncounterGroup* closest = NULL;
    F32 minDist;
    int i;

    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];
        int dist = distance3Squared(ENTPOS(client->entity), group->mat[3]);

        if (!closest || (dist < minDist))
        {
            closest = group;
            minDist = dist;
        }
    }

    if (closest)
        EncounterForceSpawnGroup(client, closest);
    else
        conPrintf(client, "ERROR: Could not find any groups");
}

// Force spawns all encounters in the mission
void EncounterForceSpawnAll()
{
    int i;
    for (i = 0; i < g_encountergroups.size; i++)
        EncounterForceSpawnGroup(NULL, g_encountergroups.storage[i]);
}

// DEBUG ONLY - dirty way to reload all encounter scripts and reparse map
// LH: Changed to work when on missions properly
void EncounterForceReload(Entity* player)
{
    if (g_activemission)
    {
        MissionLoad(1,  db_state.map_id, db_state.map_name);
        MissionForceUninit();
        MissionForceReinit(player);
    }
    else
    {
        EncounterLoad();
    }
}

// DEBUG ONLY - causes state info to be broadcast
void EncounterDebugInfo(int on)
{
    g_encounterDebugOn = on;
}

int EncounterGetDebugInfo()
{
    return g_encounterDebugOn;
}

static void EncounterDebug(char* str, ...)
{
    if (g_encounterDebugOn)
    {
        char buf[2000];
        va_list va;
        va_start(va, str);
        vsprintf(buf, str, va);
        chatSendDebug(buf);
        va_end(va);
    }
}

void EncounterForceSpawnAlways(int on)
{
    g_encounterAlwaysSpawn = on;
}

int EncounterGetSpawnAlways()
{
    return g_encounterAlwaysSpawn;
}

// DEBUG ONLY - force spawn points to pretend team-ups are of size X (0 to disable)
void EncounterForceTeamSize(int size)
{
    g_encounterForceTeam = size;
}

// DEBUG ONLY - change the default behavior for ignoring EncounterGroups
void EncounterForceIgnoreGroup(int on)
{
    if (on == 0 || on == 1) g_encounterForceIgnoreGroup = on;
    else g_encounterForceIgnoreGroup = -1; // use default
}

void EncounterMemUsage(ClientLink* client)
{
    int numPoints = 0;
    int numActive = 0;
    int numEnts = 0;
    int numStrings, stringSize;
    int i;

    // add up per-group info
    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];
        numPoints += group->children.size;
        if (group->active)
        {
            numActive++;
            numEnts += eaSize(&group->active->ents);
        }
    }

    numStrings = stashGetValidElementCount(g_groupstringhashes);
    stringSize = (int)strTableMemUsage(g_groupstrings);

    conPrintf(client, "EncounterGroup %i instances, %i size, %i total", g_encountergroups.size, sizeof(EncounterGroup),
        g_encountergroups.size * sizeof(EncounterGroup));
    conPrintf(client, "EncounterPoint %i instances, %i size, %i total", numPoints, sizeof(EncounterPoint), numPoints * sizeof(EncounterPoint));
    conPrintf(client, "EncounterActiveInfo %i instances, %i size, %i total", numActive, sizeof(EncounterActiveInfo), numActive * sizeof(EncounterActiveInfo));
    conPrintf(client, "EncounterEntityInfo %i instances, %i size, %i total", numEnts, sizeof(EncounterEntityInfo), numEnts * sizeof(EncounterEntityInfo));
    conPrintf(client, "Debug strings %i instances, %i total", numStrings, stringSize);
}

// just show active number of encounters
void EncounterDebugStats(ClientLink* client)
{
    conPrintf(client, "Encounters with critters: %i", g_encounterNumWithSpawns);
    conPrintf(client, "Encounters that are active: %i", g_encounterNumActive);
    //conPrintf(client, "Encounters that are active (by infos): %i", mpGetAllocatedCount(MP_NAME(EncounterActiveInfo)));
    conPrintf(client, "Total encounters active: %i", g_encounterTotalActive);
    conPrintf(client, "Total encounters defeated: %i", g_encounterTotalCompleted);
    conPrintf(client, "Defeat percentage: %2.2f",
        g_encounterTotalActive? (float)g_encounterTotalCompleted / (float)g_encounterTotalActive: 0.0);
}

void EncounterGetStats(int* playercount, int* running, int* active, F32* success, F32* playerratio, F32* activeratio, int* panic)
{
    *playercount = player_count;
    *running = g_encounterNumWithSpawns;
    *active = g_encounterNumActive;
    if (g_encounterTotalActive)
        *success = (float)g_encounterTotalCompleted / (float)g_encounterTotalActive;
    else
        *success = 0.0;
    if (player_count)
        *playerratio = (float)g_encounterNumWithSpawns / (float)player_count;
    else
        *playerratio = 0.0;
    if (g_encounterNumActive)
        *activeratio = (float)g_encounterNumWithSpawns / (float)g_encounterNumActive;
    else
        *activeratio = 0.0;
    *panic = g_panicLevel;
}

void EncounterSetTweakNumbers(int playerradius, int villainradius, int adjustprob)
{
    if (playerradius == 0 && villainradius == 0 && adjustprob == 0)
        g_panicSystemOn = 1;
    else
        g_panicSystemOn = 0;

    if (playerradius > 0) g_encounterPlayerRadius = playerradius;
    else g_encounterPlayerRadius = 250;
    if (villainradius > 0) g_encounterVillainRadius = villainradius;
    else g_encounterVillainRadius = 125;
    g_encounterAdjustProb = adjustprob;        // 0 is default
}

void EncounterGetTweakNumbers(int *playerradius, int* villainradius, int* adjustprob, int* playerdefault, int* villaindefault, int* adjustdefault)
{
    *playerradius = g_encounterPlayerRadius;
    *villainradius = g_encounterVillainRadius;
    *adjustprob = g_encounterAdjustProb;
    *playerdefault = panicTable[0].playerradius;
    *villaindefault = panicTable[0].villainradius;
    *adjustdefault = panicTable[0].adjustprob;
}

void EncounterSetPanicThresholds(F32 low, F32 high, int minplayers)
{
    g_panicThresholdLow = low;
    g_panicThresholdHigh = high;
    g_panicMinPlayers = minplayers;
}

void EncounterSetCritterLimits(int low, int high)
{
    g_encounterMinCritters = low;
    g_encounterMaxCritters = high;
}

void EncounterGetPanicThresholds(F32* low, F32* high, int* minplayers)
{
    *low = g_panicThresholdLow;
    *high = g_panicThresholdHigh;
    *minplayers = g_panicMinPlayers;
}

void EncounterGetCritterNumbers(int *curcritters, int *mincritters, int *belowmin, int *maxcritters, int* abovemax)
{
    *curcritters = g_encounterCurCritters;
    *mincritters = g_encounterMinCritters;
    *maxcritters = g_encounterMaxCritters;
    *belowmin = TooFewCritters(0);
    *abovemax = TooManyCritters();
}

#define MAX_ENCOUNTERBEACONS_SENT        200

Vec3 ctp_pos = {0};
static int __cdecl closestToPlayer(const EncounterGroup** lhs, const EncounterGroup** rhs)
{
    F32 distl, distr;
    distl = distance3Squared(((EncounterGroup*)(*lhs))->mat[3], ctp_pos);
    distr = distance3Squared(((EncounterGroup*)(*rhs))->mat[3], ctp_pos);
    if (distl < distr) return -1;
    if (distl > distr) return 1;
    return 0;
}

void EncounterSendBeacons(Packet* pak, Entity* player)
{
    EncounterGroup* groups[ENT_FIND_LIMIT];
    int i, speed, found;
    U32 color;

    // we have to do a graduated radius thing here because the zones vary
    // so much in density and we hit the ENT_FIND_LIMIT
    for (i = 500; i <= 2000; i += 500)
    {
        found = EncountersWithinDistance(ENTPOS(player), i, groups);
        if (found >= MAX_ENCOUNTERBEACONS_SENT) break;
    }
    if (player) copyVec3(ENTPOS(player), ctp_pos);
    qsort(groups, found, sizeof(EncounterGroup*), closestToPlayer);

    // now just send the closest ones
    if (found > MAX_ENCOUNTERBEACONS_SENT) found = MAX_ENCOUNTERBEACONS_SENT;
    pktSendBitsPack(pak, 1, found);
    for (i = 0; i < found; i++)
    {
        pktSendF32(pak, groups[i]->mat[3][0]);
        pktSendF32(pak, groups[i]->mat[3][1]);
        pktSendF32(pak, groups[i]->mat[3][2]);
        switch (groups[i]->state)
        {
        case EG_ASLEEP:        color = 0xff555555;    speed = 10;        break;
        case EG_AWAKE:        color = 0xffffffff;    speed = 30;        break;
        case EG_WORKING:    color = 0xff00aa00; speed = 20;        break;
        case EG_INACTIVE:    color = 0xffffff00;    speed = 25;        break;
        case EG_ACTIVE:        color = 0xffff7700;    speed = 30;        break;
        case EG_COMPLETED:    color = 0xffff0000; speed = 0;        break;
        case EG_OFF:        color = 0xff0000ff;    speed = 0;        break;
        }
        pktSendBits(pak, 32, color);
        pktSendBitsPack(pak, 1, speed);
        if (!groups[i]->active || !groups[i]->active->spawndef || groups[i]->state == EG_OFF)
            pktSendBits(pak, 1, 0);
        else
        {
            pktSendBits(pak, 1, 1);
            pktSendString(pak, saUtilDirtyFileName(groups[i]->active->spawndef->fileName));
        }            

        if (groups[i]->autostart_time != 0)
        {
            pktSendBits(pak, 1, 1);
            pktSendBits(pak, 32, groups[i]->autostart_time);
        } else {
            pktSendBits(pak, 1, 0);
        }
    }
}

void EncounterSetMinAutostart(ClientLink* client, int min)
{
    int i;

    g_encounterMinAutostartTime = min;

    if (min) // if not turning off
    {
        U32 now = dbSecondsSince2000();

        for (i = 0; i < g_encountergroups.size; i++)
        {
            EncounterGroup* group = g_encountergroups.storage[i];
            if (group->autostart_time > now + min)
                group->autostart_time = now + min;
        }
        conPrintf(client, "Autostart spawns will spawn every %i seconds", min);
    }
    else
        conPrintf(client, "Autostart spawns will act as normal");
}

// *********************************************************************************
// *********************************************************************************

void EncounterSetScriptControlsSpawning(int flag)
{
    g_encounterScriptControlsSpawning = flag;
}

int EncounterGetScriptControlsSpawning()
{
    return g_encounterScriptControlsSpawning;
}

// *********************************************************************************
//  Hooks for other systems to signal encounters
// *********************************************************************************
// EncounterAISignal, EncounterRemoveEnt, EncounterPlayerCreate are
// called on various conditions and can advance the encounter in different
// ways.  Encounters progress from WORKING to ALERTED to COMPLETED to OFF.
// Dialog, etc. is cued at the appropriate time.

static void SetActorAlert(Entity* ent, int alert)
{
    if (!ent || !ent->encounterInfo) return;
    if (ent->encounterInfo->alert == alert) return;
    ent->encounterInfo->alert = alert;
    EncounterEntUpdateState(ent, 0);
}


static void EncounterSetActive( EncounterGroup * group )
{
    // force the inactive transition if need be
    if (group && group->state == EG_WORKING)
    {
        group->state = EG_INACTIVE;
        encounterTransition(group, ACT_INACTIVE);
    }

    // now do the alerted transition
    if (group && group->state == EG_INACTIVE)
    {
        group->state = EG_ACTIVE;
        EncounterDebug("Encounter started %s",SAFE_MEMBER3(group,active,spawndef,logicalName));
        encounterTransition(group, ACT_ACTIVE);
    }
}

// I noticed a player and am being alerted
static void SignalAlerted( Entity* ent )
{
    EncounterGroup* group;

    // if the encounter group isn't alerted yet, make everyone active
    if (!ent || !ent->encounterInfo) return;

    group = ent->encounterInfo->parentGroup;
    assert(group->active);

    if( group->active->spawndef->activateOnObjectiveComplete )
    {
        //Signal alerted by proximity doesn't actually alert me, I'm waiting for the magic signal
        return;
    }

    SetActorAlert(ent, 1);

    EncounterSetActive( group );

    // now just make this entity alerted
    if (group && group->state == EG_ACTIVE)
    {
        const Actor* a = NULL;

        // find my AI
        if (ent->encounterInfo->actorID >= 0 && ent->encounterInfo->actorID < eaSize(&group->active->spawndef->actors))
            a = group->active->spawndef->actors[ent->encounterInfo->actorID];

        // if this entity belongs to a group, alert entire group
        if (a && a->ai_group >= 0)
        {
            actorGroupTransition(group, a->ai_group, ACT_ALERTED);  // this takes care of me too
        }
        else
        {
            entityTransition(group, ent, ACT_ALERTED);
        }
    }
}

// I'm an NPC, and I'm thanking the player
static void SignalThankHero(Entity* ent)
{
    EncounterGroup* group;

    // if we're in correct state
    if (!ent || !ent->encounterInfo) return;
    group = ent->encounterInfo->parentGroup;
    assert(group->active);
    entityTransition(group, ent, ACT_THANK);
    ActorRewardApply(EncounterWhoSavedMe(ent), ent);
}

// alerts from entities back to me
void EncounterAISignal(Entity* ent, int what)
{
    PERFINFO_AUTO_START("EncounterAISignal", 1);
    switch (what)
    {
    case ENCOUNTER_ALERTED:
        SignalAlerted(ent);
        break;
    case ENCOUNTER_THANKHERO:
        SignalThankHero(ent);
        break;
    case ENCOUNTER_NOTALERTED:
        SetActorAlert(ent, 0);
        break;
    default:
        FatalErrorf("EncounterAISignal called with invalid arg (%i)\n", what);
        break;
    }
    PERFINFO_AUTO_STOP();
}

void EncounterAllGroupsCompleteCheckThreshold(void)
{
    int i;
    int count = 0, incomplete = 0;

    for (i = 0; i < g_encountergroups.size; i++) 
    {
        EncounterGroup* group = g_encountergroups.storage[i];
        if(!group->uncounted && !group->conquered ) 
                incomplete++;
    }

     if( g_activemission && g_activemission->def && g_activemission->def->showCritterThreshold >= incomplete )
    {
        MissionObjectiveCheckThreshold();                    //    trigger the objective check so that if there is only 1 obj, it will notify the player
        g_bSendCritterLocations = LOCATION_SEND_NEW;
    }

}

// mark myself as complete and set off any entities that
// still have work to do
void EncounterComplete(EncounterGroup* group, int success)
{
    int wasCompleted = 0;
    int playerIndex;
    if (!group) 
        return;
    if( group->scriptControlsCompletion || group->scriptHasNotHadTick )
        return;
    assert(group->active);

//     if (group->active && group->active->spawndef)
//         group->active->spawndef->respawnTimer = 5; // hack for testing

    // mark encounter completed
    wasCompleted = group->state == EG_COMPLETED;
    group->state = EG_COMPLETED;
    if (group->active && group->active->spawndef && group->active->spawndef->respawnTimer != -1)
    {
        U32 now = timerSecondsSince2000();
        group->respawnTime = now + group->active->spawndef->respawnTimer;
    }
    else
    {
        group->respawnTime = 0;
    }
    group->conquered = 1;
    EncounterDebug("Encounter complete");

    if (group->active->didactivate && !wasCompleted)
    {
        dbLog("Encounter:Completed", NULL, "%s", dbg_EncounterGroupStr(group));
        g_encounterTotalCompleted++;
    }

    // logical success and failure
    if (success && group->active->succeeded)
        return;
    if (!success && group->active->failed)
        return;

    if (success)
        group->active->succeeded = 1;
    else
        group->active->failed = 1;

    if (!wasCompleted)
    {
        // this stuff only called once.
        encounterTransition(group, ACT_COMPLETED);
        if (success)
            encounterTransition(group, ACT_SUCCEEDED);
        else
            encounterTransition(group, ACT_FAILED);
        StoryArcEncounterComplete(group, success);
    }

    for (playerIndex = eaiSize(&group->active->whoKilledMyActors) - 1; playerIndex >= 0; playerIndex--)
    {
        EncounterCompleteRewardApply(group, entFromDbId(group->active->whoKilledMyActors[playerIndex]));
    }
    
    EncounterAllGroupsCompleteCheckThreshold(); // this will check to see if the encounter threshold has been hit

    // mission knows how to deal with both success & failure bits
    MissionEncounterComplete(group, success);
}

// mark myself as off and make sure all my entities are cleaned up
void EncounterClose(EncounterGroup* group)
{
    EGState wasState;

    if (!group) return;
    assert(group->active);
    wasState = group->state;

    //I'm not allowed to kill myself, even if I'm out of guys, because I have a script I'm running
    //that needs at least another tick
    if( group->scriptControlsCompletion )
        return;

    group->state = EG_OFF;
    group->conquered = 1;

    EncounterActiveInfoDestroy(group);
    EncounterDebug("Encounter closed");
}

int EncounterAllGroupsComplete(void)
{
    int i;
    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];
        if (!group->conquered && !group->uncounted) 
            return 0;
    }

    return 1;
}



// special function for an NPC that wants to know what hero completed the encounter
Entity* EncounterWhoSavedMe(Entity* ent)
{
    Entity* heroes[ENT_FIND_LIMIT];
    EncounterGroup* group;
    int herocount;

    // get the player who killed the last enemy
    if (!ent) 
        return NULL;
    if (!ent->encounterInfo) 
        goto WhoSavedMe_GetNearest;
    group = ent->encounterInfo->parentGroup;
    if (!group) 
        goto WhoSavedMe_GetNearest;
    if (group->active && group->active->whokilledme)
    {
        Entity* result = erGetEnt(group->active->whokilledme);
        if (result && ENTTYPE(result) == ENTTYPE_PLAYER)
        {
            return result;
        }
    }

    // otherwise, try and look for the nearest hero instead
WhoSavedMe_GetNearest:
    herocount = saUtilPlayerWithinDistance(ENTMAT(ent), SA_SAY_DISTANCE, heroes, 0);
    if (herocount) // find closest
    {
        int best, e;
        float bestdist;

        best = 0;
        bestdist = distance3Squared(ENTPOS(ent), ENTPOS(heroes[0]));
        for (e = 1; e < herocount; e++)
        {
            float dist = distance3Squared(ENTPOS(ent), ENTPOS(heroes[e]));
            if (dist < bestdist)
            {
                best = e;
                bestdist = dist;
            }
        }
        return heroes[best];
    }
    else return NULL;    // couldn't find anybody nearby
}

Entity *EncounterNearestPlayer(Entity* ent)
{
    Entity* heroes[ENT_FIND_LIMIT];
    int herocount;

    // get the player who killed the last enemy
    if (!ent) 
        return NULL;

    herocount = saUtilPlayerWithinDistance(ENTMAT(ent), EG_INACTIVE_RADIUS + 50, heroes, 0);
    if (herocount) // find closest
    {
        int best, e;
        float bestdist;

        best = 0;
        bestdist = distance3Squared(ENTPOS(ent), ENTPOS(heroes[0]));
        for (e = 1; e < herocount; e++)
        {
            float dist = distance3Squared(ENTPOS(ent), ENTPOS(heroes[e]));
            if (dist < bestdist)
            {
                best = e;
                bestdist = dist;
            }
        }
        return heroes[best];
    }
    else 
    {
        int i;
        for(i=1;i<entities_max;i++)
        {
            if( (entity_state[i]&ENTITY_IN_USE)  && ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER ) 
            {
                return entities[i];
            }
        }
        return NULL;// couldn't find anybody at all
    }
}

Entity *EncounterNearestPlayerByPos(Mat4 pos)
{
    Entity* heroes[ENT_FIND_LIMIT];
    int herocount;

    herocount = saUtilPlayerWithinDistance(pos, EG_INACTIVE_RADIUS + 50, heroes, 0);
    if (herocount) // find closest
    {
        int best, e;
        float bestdist;

        best = 0;
        bestdist = distance3Squared(pos[3], ENTPOS(heroes[0]));
        for (e = 1; e < herocount; e++)
        {
            float dist = distance3Squared(pos[3], ENTPOS(heroes[e]));
            if (dist < bestdist)
            {
                best = e;
                bestdist = dist;
            }
        }
        return heroes[best];
    }
    else 
    {
        int i;
        for(i=1;i<entities_max;i++)
        {
            if( (entity_state[i]&ENTITY_IN_USE)  && ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER ) 
            {
                return entities[i];
            }
        }
        return NULL;// couldn't find anybody at all
    }
}

PatrolRoute* EncounterPatrolRoute(Entity* actor)
{
    EncounterGroup* group;

    if (!actor || !actor->encounterInfo) 
        return NULL;
    group = actor->encounterInfo->parentGroup;
    if (!group || !group->active || !group->active->patrol) 
        return NULL;
    return group->active->patrol;
}

// find out who instigated the encounter that spawned this actor
Entity* EncounterWhoInstigated(Entity* actor)
{
    EncounterGroup* group;

    if (!actor || !actor->encounterInfo) 
        return NULL;
    group = actor->encounterInfo->parentGroup;
    if (!group || !group->active || !group->active->instigator) 
        return NULL;
    return erGetEnt(group->active->instigator);
}

// find the wander range for a particular entity, returns values in pos and range.
// if NOT in an encounter and therefore not restricted, 0 is returned in range
void EncounterGetWanderRange(Entity* ent, Vec3 *pos, int *range)
{
    if(range)
        *range = 0;

    if (!ent->encounterInfo) return;// don't belong to an encounter group
    if(range)
        *range = ent->encounterInfo->wanderRange;
    if(pos)
        copyVec3(ent->encounterInfo->spawnLoc, *pos);
}

void EncounterAddWhoKilledMyActor(Entity *victim, int victor)
{
    EncounterGroup* group;

    if (!victim->encounterInfo) 
        return;

    group = victim->encounterInfo->parentGroup;

    // don't bother tracking this information if it isn't going to be used
    if (!group || !group->active || !group->active->spawndef
        || !eaSize(&group->active->spawndef->encounterComplete)) 
        return;

    eaiPushUnique(&group->active->whoKilledMyActors, victor);
}

// look at success and failure conditions
static void EncounterEntDied(Entity* ent)
{
    const Actor* a = NULL;
    EncounterEntityInfo* info;
    EncounterGroup* group;

    if (!ent->encounterInfo || !ent->encounterInfo->parentGroup) 
        return;
    info = ent->encounterInfo; 
    group = ent->encounterInfo->parentGroup;

    // find actor
    if (info->actorID >= 0 && info->actorID < eaSize(&group->active->spawndef->actors))
    {
        a = group->active->spawndef->actors[info->actorID];
    }

    // rewards
    if (ent->pchar && ent->pchar->erLastDamage)
    {
        if (EncounterIsEnemy(ent))
            group->active->whokilledme = ent->pchar->erLastDamage;
        ActorRewardApply(erGetEnt(ent->pchar->erLastDamage), ent);
    }

    if( a && ( a->succeedondeath || a->failondeath ) )
    {
        EncounterComplete(group, a->succeedondeath ? 1 : 0 );
    }
    else if( !EncounterIsEnemy(ent) ) //If not otherwise specified, the death of a good guy in an encounter fails the encounter
    {
        EncounterComplete(group, 0);
    }
}

// should be updated if team changes, death state changes, or alerted state changes
// updates active villain count and active actor count as necessary
void EncounterEntUpdateState(Entity* ent, int deleting)
{
    int shouldCount = 0;
    int shouldActivate = 0;
    int actorRequired = 1;
    EncounterEntityInfo* info;
    EncounterGroup* group;

    assert(ent);

    if (!ent->encounterInfo || !ent->encounterInfo->parentGroup) 
        return;

    info = ent->encounterInfo;
    group = ent->encounterInfo->parentGroup;
    assert(group->active);

    if (entMode(ent, ENT_DEAD))
        ent->encounterInfo->alert = 0; // reset the ai variable on death
    if (!deleting && EncounterIsEnemy(ent) && !entMode(ent, ENT_DEAD) && !info->notrequired )
        shouldCount = 1;
    // count rank pet things in architect missions
    if( !g_ArchitectTaskForce && ent->villainDef && ent->villainDef->rank == VR_PET )    
        shouldCount = 0;
    if (!deleting && shouldCount && ent->encounterInfo->alert)
        shouldActivate = 1;

    if (shouldCount && !ent->encounterInfo->villaincounted)
        group->active->villaincount++;
    else if (!shouldCount && ent->encounterInfo->villaincounted)
        group->active->villaincount--;
    ent->encounterInfo->villaincounted = shouldCount;

    if (entMode(ent, ENT_DEAD))
    {
        if (!ent->encounterInfo->dead)
        {
            EncounterEntDied(ent); // after updating villain count
            info->actorID = -1;    // no longer considered the same actor if I resurrect
        }
        ent->encounterInfo->dead = 1;
    }
    else
    {
        ent->encounterInfo->dead = 0;
    }

    //If you are out of bad guys to fight, succeed the encounter
    if (group->state != EG_AWAKE 
        && !group->active->succeeded
        && group->active->villaincount == 0 
        && !group->cleaning ) 
    {
        EncounterComplete(group, 1);
    }

    if (shouldActivate && !ent->encounterInfo->activated)
    {
        if (group->active->numactiveactors == 0)
        {
            dbLog("Encounter:Active", NULL, "%s", dbg_EncounterGroupStr(ent->encounterInfo->parentGroup));
            group->active->didactivate = 1;
            g_encounterNumActive++;
            g_encounterTotalActive++;
        }
        group->active->numactiveactors++;
    }
    else if (!shouldActivate && ent->encounterInfo->activated)
    {
        if (group->active->numactiveactors == 1)
        {
            dbLog("Encounter:Inactive", NULL, "%s", dbg_EncounterGroupStr(ent->encounterInfo->parentGroup));
            g_encounterNumActive--;
        }
        group->active->numactiveactors--;
    }
    ent->encounterInfo->activated = shouldActivate;
}

// entity can be attached in EncounterSpawn as well, if here this entity is not an actor
void EncounterEntAttach(EncounterGroup* group, Entity* ent)
{
    if (!ent) 
        return;
    if (!group->active) 
        return;

    if (ent->encounterInfo)
        EncounterEntDetach(ent);
    ent->encounterInfo = EncounterEntityInfoAlloc();
    ent->encounterInfo->parentGroup = group;
    copyVec3(group->mat[3], ent->encounterInfo->spawnLoc);
    ent->encounterInfo->wanderRange = EG_DEFAULT_WANDER_RANGE;
    ent->encounterInfo->actorID = -1;
    eaPush(&group->active->ents, ent);
    EncounterDebug("Adding entity to encounter");
    EncounterEntUpdateState(ent, 0);
}

void EncounterEntDetach(Entity* ent)
{
    int i;
    Actor* a = NULL;
    EncounterGroup* group;
    EncounterEntityInfo* entinfo;

    if (!ent)
        return;
    if (!ent->encounterInfo) 
        return;
    entinfo = ent->encounterInfo;
    group = entinfo->parentGroup;
    assert(group->active);

    // remove
    ent->encounterInfo->actorID = -1; // This prevents the close of the encounter from modifying this entity
    EncounterEntUpdateState(ent, 1);
    ent->encounterInfo = NULL;
    EncounterEntityInfoFree(entinfo);
    i = eaFind(&group->active->ents, ent);
    if (i >= 0)
    {
        eaRemove(&group->active->ents, i);
        EncounterDebug("Detaching entity %s from encounter", SAFE_MEMBER(ent,name));
    }

    // decide whether encounter is done
    if (eaSize(&group->active->ents) == 0 && (!group->active->spawndef || group->active->spawndef->respawnTimer == -1))
    {
        dbLog("Encounter:Despawn", NULL, "%s", dbg_EncounterGroupStr(group));
        g_encounterNumWithSpawns--;
        if (!group->cleaning) 
            EncounterClose(group);
    }
}

static void EncounterDetachAllEntities(EncounterGroup* group)
{
    int i, n;

    if (!group->active) return;

    n = eaSize(&group->active->ents);
    for (i = n-1; i >= 0; i--)
    {
        EncounterEntDetach(group->active->ents[i]);
    }
}

void EncounterEntAddPet(Entity* parent, Entity* pet)
{
    EncounterGroup* group;

    if (!parent->encounterInfo) return;
    group = parent->encounterInfo->parentGroup;
    assert(group->active);
    EncounterEntAttach(group, pet);
}

// check for any nearby spawn points, and turn them off
void EncounterPlayerCreate(Entity* ent)
{
    int i;

    if (!EM_RESPAWN) return;

    // turn off points that haven't gone off yet
    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];

        if (!group->manualspawn && (group->state == EG_AWAKE || group->state == EG_ASLEEP))
        {
            if (posWithinDistance(ENTMAT(ent), group->mat, EG_PLAYER_RADIUS))
            {
                EncounterDebug("Encounter turned off (player blinked in)");
                group->state = EG_OFF;
                group->conquered = 1;
            }
        }
    }
}

// *********************************************************************************
//  Mission NPC's
// *********************************************************************************

// get a random location from any of the encounter groups,
// returns g_numMat4 on error
static void EncounterGetRandomLocation(Mat4* pos)
{
    EncounterGroup* group;
    EncounterPoint* point;
    int groupnum, pointnum, startp, p;

    // choose a random group
    if (!g_encountergroups.size) { copyMat4(g_nullMat4, *pos); return; }
    groupnum = rand() % g_encountergroups.size;
    group = g_encountergroups.storage[groupnum];

    // choose an encounter point
    if (!group->children.size) { copyMat4(g_nullMat4, *pos); return; }
    pointnum = rand() % group->children.size;
    point = group->children.storage[pointnum];

    // choose a position, can't use an empty position
    startp = p = rand() % EG_MAX_ENCOUNTER_POSITIONS;
    if (memcmp(point->positions[p], g_nullVec3, sizeof(Vec3)) && point->allownpcs)
    {
        createMat3YPR(*pos, point->pyrpositions[p]);
        copyVec3(point->positions[p], (*pos)[3]);
        return;
    }
    else for (    p = (p+1) % EG_MAX_ENCOUNTER_POSITIONS;
    p != startp;
    p = (p+1) % EG_MAX_ENCOUNTER_POSITIONS)
    {
        if (memcmp(point->positions[p], g_nullVec3, sizeof(Vec3)) && point->allownpcs)
        {
            createMat3YPR(*pos, point->pyrpositions[p]);
            copyVec3(point->positions[p], (*pos)[3]);
            return;
        }
    }

    // if we got to here, there were no actual positions in the encounter point..
    copyMat4(g_nullMat4, *pos);
}

static void SpawnMissionNpc(Mat4 pos)
{
    SpawnAreaDef* saDef = hashStackSpawnAreaDefs.storage[0];
    GeneratedEntDef* entdef = NULL;
    Pair* pair;
    CollInfo info;
    Vec3 endPos;
    Vec3 startPos;
    Entity* ent;
    char* model = (rand() % 2)? "MissionMaleNPC": "MissionFemaleNPC";

    // create the ent
    stashFindPointer( saDef->generatedEntDefNameHash, model, &entdef );
    if (entdef)
    {
        pair = pickPercentageWeightedPair(&entdef->NPCTypeName);
        model = pair->value1;
    }
    ent = npcCreate(model, ENTTYPE_NPC);
    if (!ent) 
        return; // error handling?

    // group snap the position
    copyVec3(pos[3], startPos);
    copyVec3(startPos, endPos);
    startPos[1] += 5; // 5 feet above
    endPos[1] -= 100; // to 100 feet below

    // If there's a collision, then move the spawn point to there.
    if(collGrid(NULL, startPos, endPos, &info, 0, COLL_NOTSELECTABLE|COLL_DISTFROMSTART))
    {
        copyVec3(info.mat[3], pos[3]);
    }

    entUpdateMat4Instantaneous(ent, pos);

    ent->fade = 1;
    SET_ENTTYPE(ent) = ENTTYPE_NPC;
    if( ent->seq && ent->seq->type->hasRandomName )
        npcAssignName(ent);
    aiInit(ent, NULL);
    aiPriorityQueuePriorityList(ent, "PL_MissionScared", 0, NULL);
}

// spawn <count> random NPCs who will just run around the map,
// they can be spawned from any encounter position on the map.
// The map doesn't need to be a mission map
void EncounterSpawnMissionNPCs(int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        // find a random position
        Mat4 pos;
        EncounterGetRandomLocation(&pos);
        if (memcmp(pos, g_nullMat4, sizeof(Mat4)) == 0)
            continue;

        // spawn a random NPC there
        SpawnMissionNpc(pos);
    }
}

// *********************************************************************************
//  Neighborhood spawn list
// *********************************************************************************

typedef struct NeighborhoodSpawn
{
    char neighborhood[500];
    StashTable spawns;
} NeighborhoodSpawn;

static StashTable g_neighborhoodspawnlist; // of NeighborhoodSpawn's
static ClientLink* g_neighborhoodlistclient;
FILE* g_neighborhoodout;

static void NeighborhoodPrint(char* format, ...)
{
    char buf[1000];
    va_list arg;
    va_start(arg, format);
    vsprintf(buf, format, arg);
    va_end(arg);

    conPrintf(g_neighborhoodlistclient, buf);
    fwrite(buf, 1, strlen(buf), g_neighborhoodout);
}

static void AddNeighborhoodSpawn(const char* neighborhood, const char* spawndef)
{
    NeighborhoodSpawn* neigh;
    int count;

    stashFindPointer( g_neighborhoodspawnlist, neighborhood, &neigh );
    if (!neigh)
    {
        neigh = calloc(sizeof(NeighborhoodSpawn), 1);
        strncpy(neigh->neighborhood, neighborhood, 500);
        neigh->neighborhood[499] = 0;
        neigh->spawns = stashTableCreateWithStringKeys(10, StashDeepCopyKeys);
        stashAddPointer(g_neighborhoodspawnlist, neighborhood, neigh, false);
    }

    if (!stashFindInt(neigh->spawns, spawndef, &count)) count = 0;
    count++;
    stashAddInt(neigh->spawns, spawndef, count,true);
}

static int ForEachSpawn(StashElement el)
{
    const char* spawn = stashElementGetStringKey(el);
    int times = (int)(intptr_t)stashElementGetPointer(el);
    NeighborhoodPrint("   %s (%i)\n", spawn, times);
    return 1;
}

static int ForEachNeighborhoodSpawn(StashElement el)
{
    NeighborhoodSpawn* neigh = stashElementGetPointer(el);
    NeighborhoodPrint("%s:\n", neigh->neighborhood);
    stashForEachElement(neigh->spawns, ForEachSpawn);
    return 1;
}

static int DestroyNeighborhoodSpawn(StashElement el)
{
    NeighborhoodSpawn* neigh = stashElementGetPointer(el);
    stashTableDestroy(neigh->spawns);
    free(neigh);
    return 1;
}

// cycle through all spawndefs and print a list of neighborhoods they belong to
void EncounterNeighborhoodList(ClientLink* client)
{
    int i, j, k;

    g_neighborhoodspawnlist = stashTableCreateWithStringKeys(10, StashDeepCopyKeys);
    g_neighborhoodlistclient = client;
    g_neighborhoodout = fopen("c:\\encounterneighborhood.txt", "w");

    // iterate through every spawndef and record
    for (i = 0; i < g_encountergroups.size; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];
        for (j = 0; j < group->children.size; j++)
        {
            EncounterPoint* point = group->children.storage[j];
            for (k = 0; k < eaSize(&point->infos); k++)
            {
                const SpawnDef* spawn = point->infos[k];
                char* neighborhood = 0;

                if (!pmotionGetNeighborhoodFromPos(group->mat[3], &neighborhood, NULL, NULL, NULL, NULL, NULL) || !neighborhood)
                    neighborhood = "(none)";
                AddNeighborhoodSpawn(neighborhood, spawn->fileName);
            }
        }
    }

    // go through neighboorhoods and print details
    NeighborhoodPrint("SPAWNDEFS IN EACH NEIGHBORHOOD\n");
    stashForEachElement(g_neighborhoodspawnlist, ForEachNeighborhoodSpawn);

    // destroy temp info
    stashForEachElement(g_neighborhoodspawnlist, DestroyNeighborhoodSpawn);
    stashTableDestroy(g_neighborhoodspawnlist);
    g_neighborhoodspawnlist = 0;
    g_neighborhoodlistclient = 0;
    fclose(g_neighborhoodout);
    g_neighborhoodout = 0;
}

// *********************************************************************************
//  Nictus hunter adds to spawns
// *********************************************************************************

TokenizerParseInfo ParseRankSelect[] = {
    { "{",                TOK_START,    0     },
    { "",                TOK_STRUCTPARAM | TOK_INT(RankSelect,teamsize, 0)     },
    { "BossChance",        TOK_F32(RankSelect,boss, 0)                    },
    { "LieutenantChance", TOK_F32(RankSelect,lieutenant, 0)                },
    { "}",                TOK_END,        0    },
    { "", 0, 0 }
};
TokenizerParseInfo ParseNictusOptions[] = {
    { "MissionAdd",        TOK_F32(NictusOptions,chanceMissionAdd, 0)     },
    { "ZoneAdd",        TOK_F32(NictusOptions,chanceZoneAdd, 0)         },
    { "NictusGroup",    TOK_F32(NictusOptions,chanceNictusGroup, 0)     },
    { "TeamSize",        TOK_STRUCT(NictusOptions,rankByTeamSize, ParseRankSelect)     },
    { "", 0, 0 }
};

NictusOptions g_nictusOptions = {0};
char* g_nictusOptionsFile = "defs/NictusHunterOptions.def";

static bool VerifyNictusOptions(TokenizerParseInfo pti[], void* structptr)
{
    int i;

    if (eaSize(&g_nictusOptions.rankByTeamSize) != MAX_TEAM_MEMBERS)
    {
        ErrorFilenamef(g_nictusOptionsFile, "Nictus options file does not define options for all team sizes (%i)", MAX_TEAM_MEMBERS);
        return 0;
    }

    // just verify we have all the team sizes, in order
    for (i = 0; i < MAX_TEAM_MEMBERS; i++)
    {
        if (g_nictusOptions.rankByTeamSize[i]->teamsize != i+1)
        {
            ErrorFilenamef(g_nictusOptionsFile, "%ith entry in team size list specifies size %i", i+1, 
                g_nictusOptions.rankByTeamSize[i]->teamsize);
            return 0;
        }
    }
    return true;
}

static void LoadNictusOptions(void)
{
    if (g_nictusOptions.rankByTeamSize)
        ParserDestroyStruct(ParseNictusOptions, &g_nictusOptions);
    ParserLoadFiles(NULL, g_nictusOptionsFile, "NictusHunterOptions.bin",    PARSER_SERVERONLY,
        ParseNictusOptions, &g_nictusOptions, NULL, NULL, VerifyNictusOptions);
}

int IsKheldian(Entity* e)
{
    if (!e->pchar) return 0;
    return (!stricmp(e->pchar->pclass->pchName, "Class_Peacebringer") ||
        !stricmp(e->pchar->pclass->pchName, "Class_Warshade"));
}

static int HasKheldianTeamMember(Entity* e)
{
    if (IsKheldian(e))
        return 1;
    if (e->teamup)
        return e->teamup->kheldianCount? 1: 0;
    return 0;
}

static void AddNictusHunters(EncounterGroup* group)
{
    Entity* e;
    VillainRank rank = VR_MINION;
    int t, numt;
    F32 r;
    Mat4 mat;

    // preliminary conditions
    if( g_ArchitectTaskForce )
        return;
    if (!group->active || !group->active->kheldian) 
        return;
    if (!(group->active->spawndef->flags & SPAWNDEF_ALLOWADDS)) 
        return;
    if (OnMissionMap())
    {
        if (rule30FloatPct() >= g_nictusOptions.chanceMissionAdd) 
            return;
    }
    else
    {
        if (rule30FloatPct() >= g_nictusOptions.chanceZoneAdd) 
            return;
    }

    // select rank, percentages change according to team size
    numt = eaSize(&g_nictusOptions.rankByTeamSize);
    t = group->active->numheroes - 1;
    CLAMP(t, 0, numt-1);
    if (t < numt) // could fail if we have no rank data
    {
        r = rule30FloatPct();
        if (r < g_nictusOptions.rankByTeamSize[t]->boss)
            rank = VR_BOSS;
        else 
        {
            r -= g_nictusOptions.rankByTeamSize[t]->boss;
            if (r < g_nictusOptions.rankByTeamSize[t]->lieutenant)
                rank = VR_LIEUTENANT;
        }
    }

    // make sure we have a free space
    if (!EncounterMatFromPos(group, EncounterFindFreePos(group), mat)) return;

    // go ahead and create and attach
    e = villainCreateNictusHunter(group->active->villaingroup, rank, group->active->villainlevel, group->active->bossreduction);
    if (!e) return;
    entUpdateMat4Instantaneous(e, mat);
    EncounterEntAttach(group, e);
    aiInit(e, NULL);
}

// *********************************************************************************
//  Nictus hunter adds to spawns
// *********************************************************************************

// returns whether ambush is ok, resets time if it is
int TeamCanHaveAmbush(Entity* player)        
{
    U32 curtime = dbSecondsSince2000();
    if (!player->teamup) 
        return 1;
    if (curtime - player->teamup->lastambushtime >= TEAM_AMBUSH_DELAY)
    {
        if (teamLock(player, CONTAINER_TEAMUPS))
        {
            player->teamup->lastambushtime = curtime;
            teamUpdateUnlock(player, CONTAINER_TEAMUPS);
        }
        return 1;
    }
    return 0;
}


//For the Scripts
// wtf is this? it's the same as EncounterSetActive
void setEncounterActive( EncounterGroup * group )
{
    // force the inactive transition if need be
    if (group && group->state == EG_WORKING)
    {
        group->state = EG_INACTIVE;
        encounterTransition(group, ACT_INACTIVE);
    }

    // now do the alerted transition
    if (group && group->state == EG_INACTIVE)
    {
        group->state = EG_ACTIVE;
        EncounterDebug("Encounter started");
        encounterTransition(group, ACT_ACTIVE);
    }
}

void setEncounterInactive( EncounterGroup * group )
{
    // force the inactive transition if need be
    if (group && group->state == EG_WORKING)
    {
        group->state = EG_INACTIVE;
        encounterTransition(group, ACT_INACTIVE);
    }
}


void encounterApplySetsOnReactorBits( Entity * e )
{
    EncounterGroup * group = 0;
    const char ** IAmA = 0; //An EArray of params
    int i, n;

    if( !e || !e->seq || !e->seq->animation.move->setsOnReactorStr )
        return;

    //See if I'm in an encoutner
    if( e && e->encounterInfo && e->encounterInfo->parentGroup )
        group = e->encounterInfo->parentGroup;

    if( !group || !group->active )
        return;

    //Look in my PL to see if I have an IAmA
    IAmA = aiGetIAmA( e );

    if( !IAmA )
        return;

    // iterate through each entity in encounter
    n = eaSize(&group->active->ents);

    for (i = 0; i < n; i++)
    {
        Entity* reactorEnt = group->active->ents[i];
        const char ** IReactTo = 0;
        int j, k, l;

        if( reactorEnt == e ) //Don't set this bit on me
            continue;

        IReactTo = aiGetIReactTo( reactorEnt );

        if( !IReactTo )
            continue;

        //Compare React list to IAmA list, if any matches, then set the bit on ractor
        //If you react to me, flash my reaction bits on to you
        //I think 99% of time there will just be one of each
        for(j = 0; j < eaSize(&IAmA); j++)
        {
            for(k = 0; k < eaSize(&IReactTo); k++)
            {
                if( 0 == stricmp( IAmA[j], IReactTo[k] ) )
                {
                    for( l = 0 ; l < eaSize( &e->seq->animation.move->setsOnReactorStr ) ; l++ )
                    {
                        seqSetStateByName( reactorEnt->seq->state, 1, e->seq->animation.move->setsOnReactorStr[l] );
                    }
                }
            }
        }

    }
}


void EncounterNotifyInterestedEncountersThatObjectiveIsComplete( char * objectiveName, Entity * completer )
{
    int i;
    int count = g_encountergroups.size;
    for (i = 0; i < count; i++)
    {
        EncounterGroup* group = g_encountergroups.storage[i];

        // check for encounter that might be interested that hasn't been prepped
        if (!group->active && !group->conquered && group->missionspawn && 
            (group->missionspawn->activateOnObjectiveComplete || group->missionspawn->createOnObjectiveComplete))
        {
            if (group->state == EG_OFF) 
                EncounterCleanup(group);

            if (EncounterNewSpawnDef(group, group->missionspawn, NULL))
            {

                // level and size are optional
                if (OnMissionMap()) {
                    group->active->villainlevel = g_activemission->missionLevel;
                }

                if (OnMissionMap())
                {
                    group->active->numheroes = MAX( g_activemission->difficulty.teamSize, MissionNumHeroes() );
                }

                if (OnMissionMap()) 
                {
                    group->active->villaingroup = g_activemission->missionGroup;
                }
            }
        }

        if( group->active && !group->conquered && group->active->spawndef )
        {
            const SpawnDef * spawndef = group->active->spawndef;

            //TO DO check to avoid repeat triggers?
            //TO DO what should repeat rules be?

            //If this encounter is created or activated on this objective's completion, and it doesn't exist yet, create it
            if( (spawndef->createOnObjectiveComplete && MissionObjectiveExpressionEval(spawndef->createOnObjectiveComplete)) || 
                (spawndef->activateOnObjectiveComplete && MissionObjectiveExpressionEval(spawndef->activateOnObjectiveComplete) ) )
            {
                //TO DO are these all the rules for this?
                if (group->state == EG_ASLEEP || group->state == EG_OFF || group->state == ENCOUNTER_AWAKE)
                {
                    if (group->state == EG_OFF) 
                        EncounterCleanup(group);
                    EncounterNewSpawnDef(group, NULL, NULL);
                    EncounterSpawn(group);
                    if (completer) 
                        ScriptVarsTablePushVarEx(&group->active->vartable, "Rescuer", (char*)completer, 'E', 0);
                    if (spawndef->createOnObjectiveComplete && group->objectiveInfo)
                        MissionWaypointCreate(group->objectiveInfo);
                }
            }

            //If this encounter is activated on this objective's completion, activate it
            if(spawndef->activateOnObjectiveComplete && MissionObjectiveExpressionEval( spawndef->activateOnObjectiveComplete ) )
            {
                EncounterSetActive( group );
                if (group->objectiveInfo)
                    MissionWaypointCreate(group->objectiveInfo);
            }
        }
    }
}

void notifyEncounterItsVolumeHasBeenEntered( DefTracker * volumeTracker )
{
    EncounterGroup * group = 0;
    DefTracker    * myTracker;
    EncounterGroup *results[ENT_FIND_LIMIT];
    int count = 0;

    //TO DO this should be faster or moved elsewhere
    for( myTracker = volumeTracker ; myTracker ; myTracker = myTracker->parent)
    {
        if( groupDefFindPropertyValue(myTracker->def,"EncounterGroup") )
        {
            //Get list of encounter groups that fulfill spec
            count = FindEncounterGroups( results, 
                myTracker->mat[3],                //Center point to start search from
                0,        //No nearer than this
                0.3,    //No farther than this
                0,        //inside this area
                0,        //Layout required in the EncounterGroup
                0,        //Name of the encounter group ( Property GroupName )
                0,
                0 );    //missionEncounterName

            if( count )
            {
                group = results[0];
                break;
            }
        }
    }

    if( group )
    {
        if (group->active && group->state == EG_AWAKE && group->active->waitingForVolumeTrigger )
        {
            group->active->waitingForVolumeTrigger = 0;
            EncounterSpawn(group); 
        }
    }
}

void EncounterOverrideTeamSize(int size)
{
    s_teamSizeOverride = size;
}

void EncounterOverrideMobLevel(int level)
{
    s_mobLevelOverride = level;
}

void EncounterRespectManualSpawn(int flag)
{
    s_respectManualSpawn = flag;
}
