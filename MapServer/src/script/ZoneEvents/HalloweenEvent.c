/*\
 *
 *    HalloweenEvent.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    The Halloween is a shard event.  The HalloweenEvent() script
 *  function handles spawning some extra bad guys in the city zones
 *  and turning on and off the event.  Working with the script are 
 *  some C functions to handle the Trick-Or-Treat behavior of city
 *  doors during the event.
 *
 */

#include "script/scriptutil.h"
#include "entity/entity.h"
#include "gameSys/DayJob.h"

static int        g_trickortreatdelay = 60 * 10;        // time between door openings
static int        g_trickortreatplayerdelay = 60;        // time between door openings
static int        g_treatprob = 80;
static int        g_minLevel = 0;
static int        g_maxLevel = 0;
static STRING    g_treatReward = NULL;

void TrickOrTreatMode(int set);
void DoorAnimClearTrickOrTreat(void);

// look for an invasion marker that doesn't have a spirit already nearby
static STRING FindFreeSpiritLocation(void)
{
    NUMBER i, n, start, firsttime;
    n = VarGetArrayCount("MarkersList");

    // start at random point and look circularly
    firsttime = 1;
    start = RandomNumber(0, n-1);
    for (i = start; ; i = (i + 1) % n)
    {
        // check and see if any spirits are nearby
        if (LocationExists(VarGetArrayElement("MarkersList", i)) && 
            NumEntitiesInArea(VarGetArrayElement("MarkersList", i), "Spirits") == 0)
        {
            return VarGetArrayElement("MarkersList", i);
        }

        // make sure we don't loop forever
        if (i == start && !firsttime) 
            return LOCATION_NONE;

        firsttime = 0;
    }
}

static void SpawnSpirits(void)
{
    int i;
    int count = NumEntitiesInTeam("Spirits"); 

    for (i = 1; i <= count; i++)
    {
        ENTITY e = GetEntityFromTeam("Spirits", i);

        if (!StringEqual(GetVillainDefName(e), "Event_Pumpkinator"))
            return;
    }

    {
        STRING location;
        STRING group;
        STRING spawnTable = VarGet("SpawnType");
        int count = VarGetArrayCount(spawnTable); 
        STRING spawndef = VarGetArrayElement(spawnTable, RandomNumber(0, count - 1));

        if (!StringEmpty(spawndef)) 
        {
            location = FindFreeSpiritLocation();
            if (LocationExists(location))
            {
                group = FindEncounterGroup(location, VarGet("SpawnLayout"), 0, 0);
                if (LocationExists(group))
                {
                    Kill("Spirits", false);
                    DebugString(StringAdd("Spawning spirit at ", location));
                    CloseEncounter(group); // if it's open, just get rid of anyone there
                    ReserveEncounter(group); // don't spawn anything else there until I'm done
                    Spawn(1, "Spirits", spawndef, group,  VarGet("SpawnLayout"), g_maxLevel, 0);
//                    DetachSpawn("Spirits");
                }
            }
        }
    }
}

static void SpawnCollector(STRING spawn, STRING layout, STRING marker)
{
    ENCOUNTERGROUP group = "first"; 

    // spawn contact hero one
    group = FindEncounterGroupByRadius( 
        marker,
        0, 
        300, 
        layout, 
        1, 
         0 );  

    if (stricmp(group, "location:none") != 0) {
        Spawn( 1, "Collectors", spawn, group, layout, 1, 0 );  
    }
}


static void ClearDoors(void)
{
    DoorAnimClearTrickOrTreat();
}


void HalloweenEvent(void)
{
    int i, count;
    STRING mapName;

    INITIAL_STATE

        ON_ENTRY 
            TrickOrTreatMode(1);
            SetDayNightCycle(DAYNIGHT_ALWAYSNIGHT);
            if (!MapHasDataToken("ZombieEventToken"))
                MapSetDataToken("ZombieEventToken", 1);
            if (!MapHasDataToken("DeadlyEventToken"))
                MapSetDataToken("DeadlyEventToken", 1);


            count = VarGetArrayCount("SpawnLevelMapList");
            mapName = GetShortMapName();
            for (i = 0; i < count; i++) {
                if (StringEqual(mapName, VarGetArrayElement("SpawnLevelMapList", i)))
                {
                    g_minLevel = StringToNumber(VarGetArrayElement("SpawnLevelMinList", i));
                    VarSetNumber("SpawnLevelMin", g_minLevel);
                    g_maxLevel = StringToNumber(VarGetArrayElement("SpawnLevelMaxList", i));
                    VarSetNumber("SpawnLevelMax", g_maxLevel);
                    if (StringEqual(VarGetArrayElement("SpawnTypelist", i), "Low"))
                    {
                        VarSet("SpawnType", "LowSpawnDefList");
                    } else {
                        VarSet("SpawnType", "HighSpawnDefList");
                    }
                    break;
                }
            }
            if (VarGetNumber("SpawnLevelMin") == 0 || VarGetNumber("SpawnLevelMax") == 0)
            {        
                TrickOrTreatMode(0);
                SetDayNightCycle(DAYNIGHT_NORMAL);

                EndScript();
            }
            
// Removed these since they are now that they are permenantly in the world
//            SpawnCollector(VarGet("VillainCollectorSpawn"), VarGet("VillainCollectorSpawnLayout"), VarGet("VillainCollectorSpawnMarker"));
//            SpawnCollector(VarGet("HeroCollectorSpawn"), VarGet("HeroCollectorSpawnLayout"), VarGet("HeroCollectorSpawnMarker"));

            g_treatReward = VarGet("TreatReward");
            g_treatprob = VarGetNumber("OddsOfTreat");
            g_trickortreatplayerdelay = VarGetNumber("PlayerTime");
            g_trickortreatdelay = VarGetNumber("DoorTime");

        END_ENTRY

        DoEvery("SpawnSpirits", 2.00, SpawnSpirits);

        DoEvery("DoorClean", g_trickortreatdelay, ClearDoors);

        if (!MapHasDataToken("ZombieEventToken"))
            MapSetDataToken("ZombieEventToken", 1);
        if (!MapHasDataToken("DeadlyEventToken"))
            MapSetDataToken("DeadlyEventToken", 1);

    END_STATES

    // Pumpkin King signals
    ON_SIGNAL("King1")
    VarSetNumber("MaxSpirits", 1);
    END_SIGNAL
        ON_SIGNAL("King2")
        VarSetNumber("MaxSpirits", 2);
    END_SIGNAL
        ON_SIGNAL("King3")
        VarSetNumber("MaxSpirits", 3);
    END_SIGNAL
        ON_SIGNAL("King4")
        VarSetNumber("MaxSpirits", 4);
    END_SIGNAL
        ON_SIGNAL("King6")
        VarSetNumber("MaxSpirits", 6);
    END_SIGNAL
        ON_SIGNAL("King8")
        VarSetNumber("MaxSpirits", 8);
    END_SIGNAL
    ON_SIGNAL("King10")
        VarSetNumber("MaxSpirits", 10);
    END_SIGNAL

    // trick or treat settings
    ON_SIGNAL("TT5s")
    g_trickortreatdelay = 5;
    END_SIGNAL
    ON_SIGNAL("TT15s")
        g_trickortreatdelay = 15;
    END_SIGNAL
    ON_SIGNAL("TT30s")
        g_trickortreatdelay = 30;
    END_SIGNAL
    ON_SIGNAL("TT45s")
        g_trickortreatdelay = 45;
    END_SIGNAL
    ON_SIGNAL("TT60s")
        g_trickortreatdelay = 60;
    END_SIGNAL
    ON_SIGNAL("TT75s")
        g_trickortreatdelay = 75;
    END_SIGNAL
    ON_SIGNAL("TT90s")
        g_trickortreatdelay = 90;
    END_SIGNAL

    // treat percentage
    ON_SIGNAL("Treat20%")
        g_treatprob = 20;
    END_SIGNAL
    ON_SIGNAL("Treat30%")
        g_treatprob = 30;
    END_SIGNAL
    ON_SIGNAL("Treat40%")
        g_treatprob = 40;
    END_SIGNAL
    ON_SIGNAL("Treat50%")
        g_treatprob = 50;
    END_SIGNAL
    ON_SIGNAL("Treat60%")
        g_treatprob = 60;
    END_SIGNAL
    ON_SIGNAL("Treat70%")
        g_treatprob = 70;
    END_SIGNAL
    ON_SIGNAL("Treat80%")
        g_treatprob = 80;
    END_SIGNAL
    ON_SIGNAL("Treat90%")
        g_treatprob = 90;
    END_SIGNAL
    ON_SIGNAL("Kill")
        Kill("Spirits", false);
    END_SIGNAL


    ON_SIGNAL("End")
        VarSetNumber("MaxSpirits", 0);
    END_SIGNAL

    ON_STOPSIGNAL
        Kill("Spirits",0);
        TrickOrTreatMode(0);
        SetDayNightCycle(DAYNIGHT_NORMAL);
        if (MapHasDataToken("ZombieEventToken"))
            MapRemoveDataToken("ZombieEventToken");
        if (MapHasDataToken("DeadlyEventToken"))
            MapRemoveDataToken("DeadlyEventToken");
    END_SIGNAL
}

void HalloweenEventInit()
{
    SetScriptName( "HalloweenEvent" );
    SetScriptFunc( HalloweenEvent );
    SetScriptType( SCRIPT_ZONE );        

    SetScriptVar( "MarkersList",                            NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "SpawnLayout",                            NULL,        SP_DONTDISPLAYDEBUG );

    SetScriptVar( "SpawnLevelMapList",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "SpawnLevelMinList",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "SpawnLevelMaxList",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "SpawnTypeList",                            NULL,        SP_DONTDISPLAYDEBUG );

    SetScriptVar( "HighSpawnDefList",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "LowSpawnDefList",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "OddsOfTreat",                            NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "TreatReward",                            NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "DoorTime",                                NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PlayerTime",                                NULL,        SP_DONTDISPLAYDEBUG );

    /*  These are now permenant
    SetScriptVar( "VillainCollectorSpawn",                    NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "VillainCollectorSpawnLayout",            NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "VillainCollectorSpawnMarker",            NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "HeroCollectorSpawn",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "HeroCollectorSpawnLayout",                NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "HeroCollectorSpawnMarker",                NULL,        SP_DONTDISPLAYDEBUG );
    */

//    SetScriptVar( "ControlName",                            NULL,        SP_DONTDISPLAYDEBUG );
//    SetScriptVar( "LullTime",                                "0.1",        SP_OPTIONAL );

    SetScriptSignal( "End Event", "End" );        
}


// *********************************************************************************
//  Direct functions - the following are NOT script functions
// *********************************************************************************

#include "HalloweenEvent.h"
#include "entity/entGameActions.h"
#include "Reward.h"
#include "ai/entai.h"
#include "ai/entaiScript.h"
#include "ai/entaiprivate.h"
#include "language/langServerUtil.h"
#include "entity/character_level.h"
#include "entity/entserver.h"
#include "gameSys/dooranim.h"
#include "entity/entPlayer.h"
#include "storyarc/storyarcutil.h"
#include "dbcomm/dbcomm.h"
#include "group/groupnetsend.h"

int g_trickortreat = 0;

ENTITY EntityNameFromEnt(Entity* e);

void TrickOrTreatMode(int set) { g_trickortreat = set; }

typedef struct TrickVillain
{
    int prob;
    float value;
    char* villain;
} TrickVillain;

TrickVillain trickVillains[] = {
    { 15,    0.34,    "Event_Witch_Minion",        },
    { 20,    0.34,    "Event_Zombie_Rifle",        },
    { 25,    0.34,    "Event_Zombie_Club",        },

    { 30,    0.34,    "Event_Zombie_Tomahawk",    },
    { 45,    0.34,    "Event_Jack_O_Lantern",        },

    { 55,    0.75,    "Event_Witch_Lt",            },
    { 65,    0.75,    "Event_Werewolf_Lt",        },
    { 75,    0.75,    "Event_Vampire_Lt",            },
    { 90,    0.75,    "Event_Spirits_Lieutenant",    },

    { 95,    1.5,    "Event_Vampire_Boss",        },
    { 101,    1.5,    "Event_Witch_Boss",            },
};

/*
typedef struct TrickLevelRange
{
    char* cityzone;
    int minlevel;
    int maxlevel;
} TrickLevelRange;

TrickLevelRange trickLevelRanges[] = {
    { "City_01_01",        1,        10 },
    { "City_01_03",        1,        10 },
    { "City_01_02",        5,        15 },
    { "Hazard_01_01",    5,        15 },
    { "Hazard_01_02",    5,        15 },
    { "Hazard_01_03",    5,        15 },

    { "City_02_01",        10,        20 },
    { "City_02_02",        10,        20 },
    { "City_02_03",        10,        20 },
    { "Hazard_02_01",    10,        20 },
    { "Trial_02_01",    10,        20 },

    { "City_03_01",        20,        30 },
    { "City_03_02",        20,        30 },
    { "Hazard_03_01",    20,        30 },
    { "Hazard_03_02",    20,        30 },
    { "Trial_03_01",    20,        30 },

    { "City_04_01",        30,        40 },
    { "City_04_02",        30,        40 },
    { "Hazard_04_01",    30,        40 },
    { "Trial_04_01",    30,        40 },
    { "Trial_04_02",    30,        40 },

    { "City_05_01",        40,        50 },
    { 0, 0, 0 },
};
*/

// called by door code to see if there is a trick or treat.
// this function can still work without an actual DoorEntry if it
// can find a nearby door entity.
int TrickOrTreat(Entity* e, Vec3 pos)
{
    char mapname[MAX_PATH];
    DoorAnimPoint* exitPoint = NULL;
    int treat, villain, i, level;
    int treatGiven = false;

    if (!g_trickortreat) 
        return 0;

    // to be safe for now, I'm only going to work with doors that have
    // animation points..
    exitPoint = DoorAnimFindPoint(pos, DoorAnimPointExit, MAX_DOORANIM_BEACON_DIST);
    if (!exitPoint) 
        return 0;

    // if too soon..
    if (DoorAnimFindTrickOrTreat(exitPoint, e))
        return 0;
//    if (DoorAnimGetLastTrickOrTreat(exitPoint) + g_trickortreatdelay > dbSecondsSince2000()) 
//        return 0;

    if (!e->pl) 
        return 0;
    if (e->pl->lastTrickOrTreat + g_trickortreatplayerdelay > dbSecondsSince2000()) 
        return 0;

    // if outside level range for zone
    saUtilFilePrefix(mapname, world_name);
    level = character_GetCombatLevelExtern(e->pchar);
    if (level < g_minLevel)
        return 0;
    if (level > g_maxLevel)
        return 0;

    // Check we're not inside a dayjob volume - this disables ToT'ing in several
    // places we don't want it: MA buildings, Arenas, etc.
    if (dayjob_IsPlayerInAnyDayjobVolume(e))
        return 0;

    // all set
    treat = rule30Rand() % 100;
    if (treat < g_treatprob) // treat
    {
        treatGiven = rewardFindDefAndApplyToEnt(e, (const char**)eaFromPointerUnsafe(g_treatReward), 0, level, false, REWARDSOURCE_EVENT, NULL);
        if (treatGiven)
            serveFloater(e, e, "TreatFloat", 0.0, kFloaterStyle_Attention, 0);
    }

    if (!treatGiven)
    {
        // trick!
        int teamCount = NumEntitiesInTeam(GetPlayerTeamFromPlayer(EntityNameFromEnt(e)));
        float count = 0.0f;
        float diffFactor = 1.0f;
        int diffLevel = 0;

        if (GetTeamSizeOverride())
            teamCount = GetTeamSizeOverride();

        if (teamCount > 6)
        {
            diffLevel = 3;
            diffFactor = 3.0f;
        } else if (teamCount > 4) {
            diffLevel = 2;
            diffFactor = 2.5f;
        } else if (teamCount > 2) {
            diffLevel = 1;
            diffFactor = 1.75f;
        }


        serveFloater(e, e, "TrickFloat", 0.0, kFloaterStyle_Attention, 0); 
        while (count < ((float) teamCount))
        {
            villain = rule30Rand() % 100;
            for (i = 0; ; i++)
            {
                if (villain < trickVillains[i].prob)
                {
                    Entity* v = villainCreateByName(trickVillains[i].villain, level + diffLevel, NULL, false, NULL, 0, NULL);
                    if (!v) 
                        continue;

                    v->bitfieldVisionPhasesDefault = e->bitfieldVisionPhases;
                    v->exclusiveVisionPhaseDefault = e->exclusiveVisionPhase;
                    entUpdatePosInstantaneous(v, pos);
                    aiInit(v, NULL);
                    AddAIBehavior(EntityNameFromEnt(v), "FeelingBothered(100), RunOutOfDoor, Combat(NeverForgetTargets, TargetInstigator, EndCondition(TimeSinceIAttacked > 60)), RunIntoDoor(Timer(20)), DestroyMe");
                    aiSetAttackTarget(v, e, NULL, "HalloweenAttackTarget", 0, 1);
                    count += (trickVillains[i].value * diffFactor);
                    break;
                }
            }
        }
    }
    DoorAnimAddTrickOrTreat(exitPoint, e);
//    DoorAnimSetLastTrickOrTreat(exitPoint, dbSecondsSince2000());
    e->pl->lastTrickOrTreat = dbSecondsSince2000();

    return 1;
}





