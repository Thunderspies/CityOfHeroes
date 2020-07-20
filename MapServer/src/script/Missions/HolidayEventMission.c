// MISSION SCRIPT
//
///////////////////////////////////////////////////////////////////////////////////
// Rebuild of HolidayEventZone script for use on mission maps via the TUT

#include "script/scriptutil.h"
#include "script/scriptengine.h"
#include "gridcoll/gridcoll.h"
#include "generator/encounterprivate.h"
#include <utilitieslib/utils/file.h>
#include "script/ZoneEvents/holidayevent.h"
#include <utilitieslib/utils/SuperAssert.h>
#include "dbcomm/dbcontainer.h"

#define    LORD_WINTERS_TEAM                "LordWintersTeam"
#define    WINTER_LORDS_TEAM                "WinterLordsTeam"
#define    WINTER_LORD                        "WinterHorde_Winter_Guardian"
#define    LORD_WINTER_INTANGIBLE            "Event.Snowbeast_Lord_Winter.One_with_Winter"

#define    CRYSTAL_COUNT                    6
#define    STANDOFF_DIST                    100.0f

#define    PATH_CRITTER_TARGET_DISTANCE    15.0f

static void HolidayEventZoneSpawnLordWinterAndMinions()
{
    ENCOUNTERGROUP group;
    ENTITY lordWinter;

    SetAIPriorityList(LORD_WINTERS_TEAM, "DestroyMe");
    SetAIPriorityList(WINTER_LORDS_TEAM, "DestroyMe");
    SetAIPriorityList(ALL_CRITTERS, "DestroyMe");

    group = FindEncounterGroupByRadius(StringAdd("marker:", VarGet("SpawnLocation")),
                                            0, 50, VarGet("SpawnGroup"), 0, 0);  
    if (StringEqual(group, "location:none"))
    {
        // If you come here because this assert tripped, find me, and then make sure I talk to Paul Gunn.
        // This assert trips appens if the mapserver can't figure where to place Lord Winter
        assertmsg(0, "Can't find EncounterGroup to spawn Lord Winter.");
    }
    Spawn(1, LORD_WINTERS_TEAM, "UseCanSpawns", group, VarGet("SpawnGroup"), 0, 0);
    if (NumEntitiesInTeam(LORD_WINTERS_TEAM) != 1)
    {
        // If you come here because this assert tripped, find me, and then make sure I talk to Paul Gunn.
        // This assert trips if more than one critter spawns from Lord Winter's layout
        assertmsg(0, "Lord Winter has unexpected accomplices.");
    }
    lordWinter = GetEntityFromTeam(LORD_WINTERS_TEAM, 1);
    SetAINeverChooseThisPowerUnlessITellYouTo(lordWinter, LORD_WINTER_INTANGIBLE);
    VarSet("LordWinter", lordWinter);
    VarSet("TetherPoint", EntityPos(lordWinter));
    VarSetFraction("LastHealth", 1.0f);
    VarSetNumber("Seen75", 0);
    VarSetNumber("Seen50", 0);
    VarSetNumber("Seen25", 0);
    VarSetFraction("Target75", RandomFraction(0.75, 0.80));
    VarSetFraction("Target50", RandomFraction(0.50, 0.55));
    VarSetFraction("Target25", RandomFraction(0.25, 0.30));
    VarSetNumber("WLState", 0);
    VarSetNumber("LordWinterLeashing", 0);

    EncounterForceSpawnAll();
}

// references to Crystals in this are now misnomers.  They were originally the points where the
// ice shards were to spawn, but are now just being used to provide locations for the winter lord to spawn
static void HolidayEventZoneSpawnWinterLord()
{
    NUMBER i;
    NUMBER j;
    NUMBER n;
    STRING locStr;
    ENTITY currentPlayer;
    Vec3 playerPos;
    FRACTION playerDistSQ;
    FRACTION bestDistSQ;
    NUMBER bestIndex;
    ENTITY winterLord;
    Vec3 crystalPos[CRYSTAL_COUNT];
    FRACTION distancesSQ[CRYSTAL_COUNT];

    // Firstly assemble the locations of the six crystal markers
    for (j = 0; j < CRYSTAL_COUNT; j++)
    {
        locStr = StringAdd("marker:Crystal_0", NumberToString(j + 1));
        if (GetPointFromLocation(locStr, crystalPos[j]) == 0)
        {
            distancesSQ[j] = -1;
        }
        else
        {
            distancesSQ[j] = 1e12;
        }
    }
    // Then iterate over all players figuring the shortest distance between all crsytal markers and all players
    n = NumEntitiesInTeam(ALL_PLAYERS_READYORNOT);
    for (i = 1; i <= n; i++)
    {
        currentPlayer = GetEntityFromTeam(ALL_PLAYERS_READYORNOT, i);
        if (!StringEqual(currentPlayer, TEAM_NONE) && GetPointFromLocation(currentPlayer, playerPos) != 0)
        {
            for (j = 0; j < CRYSTAL_COUNT; j++)
            {
                playerDistSQ = distance3Squared(playerPos, crystalPos[j]);
                if (playerDistSQ < distancesSQ[j])
                {
                    distancesSQ[j] = playerDistSQ;
                }
            }
        }
    }

    // Now find which crystal marker has the longest shortest distance
    bestDistSQ = -2;
    bestIndex = 0;
    for (j = 0; j < CRYSTAL_COUNT; j++)
    {
        if (distancesSQ[j] > bestDistSQ)
        {
            bestDistSQ = distancesSQ[j];
            bestIndex = j;
        }
    }

    // KK, by the time we reach here, bestIndex contains the index of the crystal marker that is furthest from all players.  Use that location to spawn holmie,
    // and then point him at big daddy
    locStr = StringAdd("marker:Crystal_0", NumberToString(bestIndex + 1));

    winterLord = CreateVillain(WINTER_LORDS_TEAM, WINTER_LORD, 0, NULL, locStr);
    if (StringEqual(winterLord, TEAM_NONE))
    {
        VarSet("WinterLord", NULL);
        VarSetFraction("HoldHealth", -1.0f);
    }
    else
    {
        VarSet("WinterLord", winterLord);
        AddAIBehavior(winterLord, "Combat");
        SetAIMoveTarget(winterLord, VarGet("LordWinter"), MEDIUM_PRIORITY, 0, 1, PATH_CRITTER_TARGET_DISTANCE);
        VarSetNumber("WatchDist", 1);
    }
}

static void HolidayZoneEventStartFoggyBit(ENTITY lordWinter, FRACTION health, STRING seenwhat, STRING message)
{
    VarSetNumber("WLState", 1);
    TimerSet("WLTick", 22.0f / 60.0f);
    ScriptSkyFileSetFade(VarGetNumber("FoggySky"), 0, 20.0f);
    VarSetFraction("HoldHealth", health);
    VarSetNumber(seenwhat, 1);
    VarSet("SayText", message);
    VarSetNumber("IsFoggy", 1);
    SetAIPriorityList(lordWinter, "DoNothing");
    EntityActivatePower(lordWinter, NULL, LORD_WINTER_INTANGIBLE);
}


static void HolidayEventMissionEnd()
{
        if (VarGetNumber("IsFoggy"))
        {
            ScriptSkyFileSetFade(0, VarGetNumber("FoggySky"), 10.0f);
            VarSetNumber("IsFoggy", 0);
        }
        ScriptUIHide("Collection:LWRealmUI", ALL_PLAYERS_READYORNOT);
        SetAIPriorityList(LORD_WINTERS_TEAM, "DestroyMe");
        SetAIPriorityList(WINTER_LORDS_TEAM, "DestroyMe");
        SetAIPriorityList(ALL_CRITTERS, "DestroyMe");
        
        if(VarGetNumber("EventSuccess"))
            SetTrialStatus(trialStatus_Success);
        else
            SetTrialStatus(trialStatus_Failure);

        SendTrialStatus(ALL_PLAYERS_READYORNOT);
}

static void HolidayEventZoneOnEntityDefeat(ENTITY player, ENTITY victor)
{
    ENTITY lordWinter;

    if(TimerElapsed("BootOutTime"))
        return;

    lordWinter = VarGet("LordWinter");
    if (StringEqual(player, lordWinter))
    {
        VarSet("LordWinter", NULL);
        SendPlayerAttentionMessageWithColor(ALL_PLAYERS, VarGet("SuccessMessage"), 0x00ff00ff, kFloaterStyle_Attention);
        VarSetNumber("EventSuccess", 1);
        
        SET_STATE("Shutdown");
    }
    if (StringEqual(player, VarGet("WinterLord")))
    {
        VarSet("WinterLord", NULL);
        VarSetNumber("WatchDist", 0);
        VarSetFraction("HoldHealth", -1.0f);
        EntityDeactivateTogglePower(lordWinter, LORD_WINTER_INTANGIBLE);
        SetAIPriorityList(lordWinter, "Combat");
    }
}

//////////////////////////////////////////////////////////////////////////////////////
// Sets up contact for players who don't have it!
static int HolidayEventZoneOnEnterMap(ENTITY player)
{
    ScriptUIShow("Collection:LWRealmUI", player);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// Called when a player leaves the map - clear any minimap waypoints that might be set
//
static int HolidayEventZoneOnLeaveMap(ENTITY player)
{
    ScriptUIHide("Collection:LWRealmUI", player);

    return 0;
}

void HolidayEventMission(void)
{
    ENTITY lordWinter;
    ENTITY winterLord;
    FRACTION health;
    FRACTION lastHealth;
    FRACTION holdHealth;
    NUMBER WLstate;
    FRACTION target75;
    FRACTION target50;
    FRACTION target25;
    LOCATION actualLoc;
    LOCATION tetherLoc;
    FRACTION distance;
    FRACTION radius;

    //////////////////////////////////////////////////////////////////////////////////////
    // S E T U P
    //////////////////////////////////////////////////////////////////////////////////////
    INITIAL_STATE

        ON_ENTRY
        {    
            NUMBER bootTime;
            
            OnEntityDefeated(HolidayEventZoneOnEntityDefeat);
            OnPlayerEnteringMap(HolidayEventZoneOnEnterMap);
            OnPlayerExitingMap(HolidayEventZoneOnLeaveMap);

            bootTime = (FRACTION)SecondsSince2000() + (VarGetFraction("TimeLimit") * 60.0f);
            VarSetNumber("BootSeconds", bootTime);

            HolidayEventZoneSpawnLordWinterAndMinions();
            VarSetNumber("FoggySky", SkyFileGetByName("Foggy_Winter06.txt"));

            ScriptUITitle("TitleWidget", "TitleText", "TitleHint");
            ScriptUITimer("TimerWidget", VarGet("BootSeconds"), VarGet("TimeTillBootout"), "");
            ScriptUICreateCollection("LWRealmUI", 2, "TitleWidget", "TimerWidget");
            
            TimerSet("BootOutTime", VarGetFraction("TimeLimit"));

            ScriptUIShow("Collection:LWRealmUI", ALL_PLAYERS_READYORNOT);
        }
        END_ENTRY

        SET_STATE("InProgress");

    //////////////////////////////////////////////////////////////////////////////////////
    // I N   P R O G R E S S
    //////////////////////////////////////////////////////////////////////////////////////

    STATE("InProgress") //////////////////////////////////////////////////////////////////
        ON_ENTRY 
        {
        }
        END_ENTRY

        if(TimerElapsed("BootOutTime"))
        {
            SendPlayerAttentionMessageWithColor(ALL_PLAYERS, VarGet("FailureMessage"), 0xff0000ff, kFloaterStyle_Attention);

            SET_STATE("Shutdown");
        }

        lordWinter = VarGet("LordWinter"); 
        if (!StringEmpty(lordWinter))
        {
            health = GetHealth(lordWinter);
            lastHealth = VarGetFraction("LastHealth");

            target75 = VarGetFraction("Target75");
            target50 = VarGetFraction("Target50");
            target25 = VarGetFraction("Target25");

            if (lastHealth > target75 && health <= target75 && VarGetNumber("Seen75") == 0)
            {
                HolidayZoneEventStartFoggyBit(lordWinter, health, "Seen75", "GuardianTextOne");
            }
            if (lastHealth > target50 && health <= target50 && VarGetNumber("Seen50") == 0)
            {
                HolidayZoneEventStartFoggyBit(lordWinter, health, "Seen50", "GuardianTextTwo");
            }
            if (lastHealth > target25 && health <= target25 && VarGetNumber("Seen25") == 0)
            {
                HolidayZoneEventStartFoggyBit(lordWinter, health, "Seen25", "GuardianTextThree");
            }
                holdHealth = VarGetFraction("HoldHealth");
            if (holdHealth > 0.0f)
            {
                health = holdHealth;
                SetHealth(lordWinter, holdHealth, 0);
            }

            WLstate = VarGetNumber("WLState");
            if (WLstate && TimerElapsed("WLTick"))
            {
                if (WLstate == 1)
                {
                    if (WLstate == 1)
                    {
                        HolidayEventZoneSpawnWinterLord();
                        Say(lordWinter, VarGet(VarGet("SayText")), 2);
                        VarSetNumber("WLState", 2);
                        TimerSet("WLTick", 3.0f / 60.0f);
                    }
                    else
                    {
                        ScriptSkyFileSetFade(0, VarGetNumber("FoggySky"), 30.0f);
                        VarSetNumber("WLState", 0);
                        VarSetNumber("IsFoggy", 0);
                        TimerRemove("WLTick");
                    }
                }
                else
                {
                    ScriptSkyFileSetFade(0, VarGetNumber("FoggySky"), 30.0f);
                    VarSetNumber("WLState", 0);
                    VarSetNumber("IsFoggy", 0);
                    TimerRemove("WLTick");
                }
            }

            if (VarGetNumber("WatchDist"))
            {
                winterLord = VarGet("WinterLord");
                if (StringEmpty(winterLord))
                {
                    VarSetNumber("WatchDist", 0);
                }
                else if (DistanceBetweenEntities(winterLord, lordWinter) < STANDOFF_DIST)
                {
                    SetAIPriorityList(winterLord, "Combat");
                    VarSetNumber("WatchDist", 0);
                }
            }

            if (DoEvery("TetherTimer", 10.0f / 60.0f, NULL))
            {
                actualLoc = EntityPos(lordWinter);
                tetherLoc = VarGet("TetherPoint");
                distance = DistanceBetweenLocations(actualLoc, tetherLoc);
                if (VarGetNumber("LordWinterLeashing"))
                {
                    if (distance < PATH_CRITTER_TARGET_DISTANCE * 3)
                    {
                        SetAIPriorityList(lordWinter, "Combat");
                        VarSetNumber("LordWinterLeashing", 0);
                    }
                }
                else
                {
                    radius = VarGetFraction("TetherRadius");
                    if (radius < 0.1)
                    {
                        radius = 250.0;
                    }

                    if (distance > radius)
                    {
                        SetAIMoveTarget(lordWinter, tetherLoc, HIGH_PRIORITY, 0, 1, PATH_CRITTER_TARGET_DISTANCE);
                        VarSetNumber("LordWinterLeashing", 1);
                    }
                }
            }

            VarSetFraction("LastHealth", health);
        }
        else
        {
            if (VarGetNumber("IsFoggy"))
            {
                ScriptSkyFileSetFade(0, VarGetNumber("FoggySky"), 30.0f);
                VarSetNumber("IsFoggy", 0);
            }
        }
    //////////////////////////////////////////////////////////////////////////////////////
    // S H U T D O W N
    //////////////////////////////////////////////////////////////////////////////////////
    STATE("Shutdown") ////////////////////////////////////////////////////////
        HolidayEventMissionEnd();
    END_STATES

    ON_SIGNAL("End")
        SET_STATE("Shutdown");
    END_SIGNAL

    ON_STOPSIGNAL
        HolidayEventMissionEnd();
    END_SIGNAL
}

void HolidayEventMissionInit()
{
    SetScriptName("HolidayEventMission");
    SetScriptFunc(HolidayEventMission);
    SetScriptType(SCRIPT_MISSION);

    SetScriptSignal("End", "End");

    SetScriptVar("SpawnLocation",            NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("SpawnGroup",                NULL,        SP_DONTDISPLAYDEBUG);

    SetScriptVar("CrystalSpawns",            NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("CrystalList",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("SpawnFX",                    NULL,        SP_DONTDISPLAYDEBUG);

    SetScriptVar("GuardianTextOne",            NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("GuardianTextTwo",            NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("GuardianTextThree",        NULL,        SP_DONTDISPLAYDEBUG);

    SetScriptVar("TitleText",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("TitleHint",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("TimeTillBootout",            NULL,        SP_DONTDISPLAYDEBUG);

    SetScriptVar("SuccessMessage",            NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("FailureMessage",            NULL,        SP_DONTDISPLAYDEBUG);

    SetScriptVar("TimeLimit",                "15",        SP_DONTDISPLAYDEBUG);

    SetScriptVar("TetherRadius",            "250.0",    SP_DONTDISPLAYDEBUG | SP_OPTIONAL); 
}
