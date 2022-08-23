// ZONE SCRIPT
//
///////////////////////////////////////////////////////////////////////////////////
// This is currently set up to use a time based system.  Each zone remains a target
// for four minutes.

// Notes - Crystal locations are Crystal_01 thru Crystal_06

#include "script/scriptutil.h"
#include "script/scriptengine.h"
#include "gridcoll/gridcoll.h"
#include "generator/encounterPrivate.h"
#include <utilitieslib/utils/file.h>
#include "HolidayEvent.h"
#include <utilitieslib/utils/SuperAssert.h>
#include "dbcomm/dbcontainer.h"

#define    LORD_WINTERS_TEAM                "LordWintersTeam"
#define    WINTER_LORDS_TEAM                "WinterLordsTeam"
#define    WINTER_LORD                        "WinterHorde_Winter_Guardian"
#define    LORD_WINTER_INTANGIBLE            "Event.Snowbeast_Lord_Winter.One_with_Winter"

#define    CRYSTAL_COUNT                    6
#define    STANDOFF_DIST                    100.0f

#define    PATH_CRITTER_TARGET_DISTANCE    15.0f

#define BOOT_TIME_TOKEN_NAME_SIZE        64
#define    BOOT_TIME_TOKEN_NAME_TEMPLATE    "HolidayEventBootToken%d"

// Get the locally adjusted heartbeat value, in minutes.
static NUMBER HolidayEventZoneGetHeartBeat()
{
    NUMBER heartbeat;
    
    heartbeat = MapGetDataToken(HEARTBEAT_TOKEN);
    VarSetNumber("GlobalHeartBeat", heartbeat);
    return (heartbeat + WINTER_EVENT_CYCLE_DURATION - VarGetNumber("ZoneIndex") * WINTER_EVENT_OFFSET) % WINTER_EVENT_CYCLE_DURATION;
}

// Set the base heartbeat
static void HolidayEventZoneSetHeartBeat()
{
    NUMBER heartbeat;
    NUMBER targetZone;
    NUMBER entryTime;
    NUMBER lastEntryTime;
    NUMBER secondsSinceLastEntryTime;
    char bootTimeTokenName[BOOT_TIME_TOKEN_NAME_SIZE];

    // Base heartbeat used to synchronize the events in the 5 snowy zones.
    // This is (minutes since 2000) % (the duration of a complete cycle)
    heartbeat = (SecondsSince2000() / SECONDS_PER_MINUTE) % WINTER_EVENT_CYCLE_DURATION;

    // And save it away
    MapSetDataToken(HEARTBEAT_TOKEN, heartbeat);
    VarSetNumber("HeartBeat", heartbeat);

    // Set the player ticket cookie.  This is used to allow croatoa zones to determine how
    // recently the player actually clicked on a present.  The base cookie is simply minutes
    // since 2000.
    MapSetDataToken(PLAYER_COOKIE_TOKEN, SecondsSince2000() / SECONDS_PER_MINUTE);

    // Now the shenanigens to set the target zone and entry time.

    // Use divide then multiply to remove the remainder portion, these two lines leave lastEntryTime
    // holding the SecondsSince2000() value at which the most recent zone had it's heartbeat start at zero
    lastEntryTime = SecondsSince2000() / (SECONDS_PER_MINUTE * WINTER_EVENT_OFFSET);
    lastEntryTime = lastEntryTime * (SECONDS_PER_MINUTE * WINTER_EVENT_OFFSET);

    // Now figure how long ago that was
    secondsSinceLastEntryTime = SecondsSince2000() - lastEntryTime;
    // A zone becomes the current target six minutes before it's internal heartbeat becomes zero.
    // Adding 6 is the correct thing to do, at time zero, (heartbeat + 6) / WINTER_EVENT_OFFSET
    // will evaluate to 1, which is correct.  zone 1 will be the next one that becomes active
    targetZone = ((heartbeat + 6) / WINTER_EVENT_OFFSET) % WINTER_EVENT_ZONECOUNT;

    // We only do anything if we're over two minutes in, and target zone points to the wrong place
    if (secondsSinceLastEntryTime > 2 * SECONDS_PER_MINUTE && targetZone != VarGetNumber("TargetZone"))
    {
        // Now figure the actual SecondsSince2000() value at which players should be allowed to enter.
        // The target zone is actually two further along in the cycle, so if we add twice the offset to the current
        // we'll have the actual time at which our target zone has it's zero heartbeat.
        entryTime = lastEntryTime + 2 * (SECONDS_PER_MINUTE * WINTER_EVENT_OFFSET);
        // There is a little slop factor in here, zones can lag as much as 15 seconds behind the actuial heartbeat
        // This is not, however, an issue, since the zone has completed it's reset one minute before zero time,
        // which means there's a 45 second safety margin

        // Set the target token and the entrytime token
        MapSetDataToken(ZONE_TARGET_TOKEN, targetZone);
        MapSetDataToken(ZONE_ENTRYTIME_TOKEN, entryTime);

        snprintf(bootTimeTokenName, BOOT_TIME_TOKEN_NAME_SIZE, BOOT_TIME_TOKEN_NAME_TEMPLATE, targetZone);
        bootTimeTokenName[BOOT_TIME_TOKEN_NAME_SIZE - 1] = 0;
        MapSetDataToken(bootTimeTokenName, entryTime + (WINTER_EVENT_CYCLE_DURATION - 3) * 60);
        
        VarSetNumber("TargetZone", targetZone);
    }
}

static void HolidayEventZoneGetBootTime()
{
    int bootTime;
    char bootTimeTokenName[BOOT_TIME_TOKEN_NAME_SIZE];

    snprintf(bootTimeTokenName, BOOT_TIME_TOKEN_NAME_SIZE, BOOT_TIME_TOKEN_NAME_TEMPLATE, VarGetNumber("ZoneIndex"));
    bootTimeTokenName[BOOT_TIME_TOKEN_NAME_SIZE - 1] = 0;

    bootTime = MapGetDataToken(bootTimeTokenName);
    if (bootTime <= 0)
    {
        bootTime = SecondsSince2000() + (WINTER_EVENT_CYCLE_DURATION - 3) * 60;
    }
    VarSetNumber("BootTimerUI", bootTime);
}


static void HolidayEventZoneSpawnLordWinterAndMinions()
{
    ENCOUNTERGROUP group;
    ENTITY lordWinter;

    Kill(LORD_WINTERS_TEAM, false);
    Kill(WINTER_LORDS_TEAM, false);
    Kill(ALL_CRITTERS, false);

    group = FindEncounterGroupByRadius(StringAdd("marker:", VarGet("SpawnLocation")),
                                            0, 50, VarGet("SpawnGroup"), 0, 0);  
    if (StringEqual(group, "location:none"))
    {
        // If you come here because this assert tripped, find me, and then make sure I talk to Paul Gunn.
        // This assert trips appens if the mapserver can't figure where to place Lord Winter
        assertmsg(0, "Can't find EncounterGroup to spawn Lord Winter.  Go find David G.");
    }
    Spawn(1, LORD_WINTERS_TEAM, "UseCanSpawns", group, VarGet("SpawnGroup"), 0, 0);
    if (NumEntitiesInTeam(LORD_WINTERS_TEAM) != 1)
    {
        // If you come here because this assert tripped, find me, and then make sure I talk to Paul Gunn.
        // This assert trips if more than one critter spawns from Lord Winter's layout
        assertmsg(0, "Lord Winter has unexpected accomplices.  Go find David G.");
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
    n = NumEntitiesInTeam(ALL_PLAYERS);
    for (i = 1; i <= n; i++)
    {
        currentPlayer = GetEntityFromTeam(ALL_PLAYERS, i);
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
        SetAIMoveTarget(winterLord, VarGet("LordWinter"), MEDIUM_PRIORITY, 0, 0, PATH_CRITTER_TARGET_DISTANCE);
        VarSetNumber("WatchDist", 1);
    }
}

#if 0
static void HolidayEventZoneSpawnCrystalCritters(NUMBER crittersToSpawn)
{
    NUMBER i;
    STRING critter;
    STRING spawnFX;
    NUMBER spawnCheckCount;
    STRING currentCritter;
    FRACTION xoff;
    FRACTION zoff;
    Vec3 centerPos;                        // Center of creation square
    Vec3 pos;                            // Actual creation position
    Vec3 topPos;                        // Straight up from pos, used in collision checking
    CollInfo coll;                        // Opaque data structure - used in collision checking
    char critterLoc[256];

    if (GetPointFromLocation(VarGet("CrystalLocation"), centerPos) == 0)
    {
        if (isDevelopmentMode())
        {
            assert(0 && "Bad crystal location in HolidayEventZoneSpawnCrystalCritters(...)");
        }
        return;
    }

    for (i = 0; i < crittersToSpawn; i++)
    {
        critter = VarGetArrayElement("CrystalSpawns", RandomNumber(0, VarGetArrayCount("CrystalSpawns") - 1));
        spawnFX = VarGet("SpawnFX");

        for (spawnCheckCount = 0; spawnCheckCount < 5; spawnCheckCount++)
        {
            // First things first.  Spread out around the spawn location, and then drop a probe to find the floor.  We scan +/- 20 ft
            // from the altitude of the spawn marker
            // The spread out code doubles the two offsets as long as they're both inside the minimum, that way they'll tend to spawn in
            // a ring around the crystal
            xoff = RandomFraction(-SPAWN_OUTER_RADIUS, SPAWN_OUTER_RADIUS);
            zoff = RandomFraction(-SPAWN_OUTER_RADIUS, SPAWN_OUTER_RADIUS);
            if (fabs(xoff) < 0.1f && fabs(zoff) < 0.1f)
            {
                // Deal with the edge case of being really really close in
                xoff = SPAWN_INNER_RADIUS + 1.0f;
            }
            while (fabs(xoff) < SPAWN_INNER_RADIUS && fabs(zoff) < SPAWN_INNER_RADIUS)
            {
                xoff = xoff + xoff;
                zoff = zoff + zoff;
            }

            pos[0] = centerPos[0] + xoff;
            pos[1] = centerPos[1] - 15.0f;
            pos[2] = centerPos[2] + zoff;

            topPos[0] = pos[0];
            topPos[1] = pos[1] + 30.0f;
            topPos[2] = pos[2];

            if (collGrid(NULL, topPos, pos, &coll, 1.0f, COLL_NOTSELECTABLE|COLL_DISTFROMSTART|COLL_CYLINDER|COLL_BOTHSIDES))
            {
                // found the floor.  Spawn him here
                pos[1] = coll.mat[3][1] + 0.1f;
                break;
            }
        }

        // If we actually did find a good spot, spawn the guy.
        if (spawnCheckCount < 5)
        {
            snprintf(critterLoc, sizeof(critterLoc), "coord:%f,%f,%f", pos[0], pos[1], pos[2]);
            critterLoc[sizeof(critterLoc) - 1] = 0;

            currentCritter = CreateVillain(ON_PATH_TEAM, critter, 0, NULL, critterLoc);

            SetAIStandoffDistance(currentCritter, PATH_CRITTER_TARGET_DISTANCE);
            // OK, this deserves some explanation.  Base priority is Combat, then we stack a moveto, and finally the items needed to make the
            // critters spawn in properly
            SetAIPriorityList(currentCritter, "Combat");
            SetAIMoveTarget(currentCritter, StringAdd("marker:", VarGet("SpawnLocation")), MEDIUM_PRIORITY, 0, 0, PATH_CRITTER_TARGET_DISTANCE);
            AddAIBehavior(currentCritter, CRITTER_SPAWN_PL);

            if (!StringEmpty(spawnFX))
            {
                ScriptPlayFXOnEntity(currentCritter, spawnFX);
            }
        }
    }
}
#endif

// Kick out players.  If forceboot is true, don't bother checking timestamps, just kick everyone.
// hardBoot is a flag that says we're shutting down the zone.
static void HolidayEventZoneBootPlayers(int hardBoot)
{
    int i;
    int n;
    int forceboot;
    int playerTicketTime;
    int validTicketTime;
    ENTITY player;
    STRING returnData;
    const char *tag;
    char *mapName;
    char *spawn_target;

    forceboot = VarGetNumber("ForceBootFlag");

    n = NumEntitiesInTeam(ALL_PLAYERS);
    for (i = n; i > 0; i--)
    {
        player = GetEntityFromTeam(ALL_PLAYERS, i);

        // Don't boot accesslevel characters, this allows CS to enter these zones via /mapmove without problems
        if (GetAccessLevel(player) > 0)
        {
            continue;
        }
        if (!forceboot && !hardBoot)
        {
            // This check determines if the player is allowed to be here based on when they came in.  Most of the
            // time it'll work, and they'll be skipped, and thus will stay in the zone.  The most likely cause of
            // this failing, and thus booting them is if a player logs out in Lord Winter's realm, and then logs
            // back in some time later.
            playerTicketTime = EntityGetRewardToken(player, PLAYER_TICKET_TOKEN);
            validTicketTime = VarGetNumber("TicketGoodTime");
            if (validTicketTime < playerTicketTime + 15)
            {
                continue;
            }
        }

        // hardBoot means the zone event is being terminated.  This *mandates* a kick out this instant, so to avoid the
        // race condition that lead to people being dropped outside the map in Steel etc., we set returnData to the empty
        // string, which forces an emergency return to Atlas / Mercy.  This should not be a problem since it actually
        // requires CS intervention  to cause this to happen.
        returnData = hardBoot ? "" : GetSpecialMapReturnData(player);

        // We've used the return value, so clear it.
        SetSpecialMapReturnData(player, NULL, NULL);
        // decompose into tag, mapname, and spawn_target.  If anything goes wrong, spawn_target is
        // going to be NULL.
        tag = returnData;
        mapName = strchr(tag, '=');
        if (mapName)
        {
            *mapName++ = 0;
            spawn_target = strchr(mapName, ';');
            if (spawn_target)
            {
                *spawn_target++ = 0;
            }
        }
        else
        {
            spawn_target = NULL;
        }

        // Check that the tag matches and we had a valid map and spawn target
        if (spawn_target == NULL || stricmp("WinterEvent", tag) != 0)
        {
            // Something wrong.  Use some standard values for safe places in Atlas, Mercy and Praetoria
            if (IsEntityPraetorianTutorial(player))
            {
                mapName = "P_City_00_05.txt";
            }
            else if (IsEntityPraetorian(player))
            {
                mapName = "P_City_00_02.txt";
            }
            else if (IsEntityVillain(player))
            {
                mapName = "V_City_01_01.txt";
            }
            else
            {
                mapName = "City_01_01.txt";
            }
            spawn_target = "NewPlayer";
        }
        // If the destination is multi-instanced, we may need to add a flag to this to
        // just unilaterally move them, doing the "select_best" black magic.  Otherwise
        // we run into the same problem that people used as an explioit to avoid getting
        // kicked from bases.
        ScriptEnterScriptDoor(player, mapName, spawn_target);
        ScriptSetSpecialReturnInProgress(player);
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

static void HolidayEventZoneOnEntityDefeat(ENTITY player, ENTITY victor)
{
    ENTITY lordWinter;

    lordWinter = VarGet("LordWinter");
    if (StringEqual(player, lordWinter))
    {
        VarSet("LordWinter", NULL);
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

static void HolidayEventZoneEnd()
{
    if (VarGetNumber("ControlTimer") == 0)
    {
        VarSetNumber("ForceBootFlag", 1);
        HolidayEventZoneBootPlayers(1);
        Kill(LORD_WINTERS_TEAM, false);
        Kill(WINTER_LORDS_TEAM, false);
        Kill(ALL_CRITTERS, false);
    }
}

void HolidayEventZone(void)
{
    NUMBER heartbeat;
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
            // ControlTimer will be set true on all instances of the chosen map.  So look at
            // the actual and base instance numbers and only set ReallyControlTimer if
            // they're the same.
            if (VarGetNumber("ControlTimer") && GetMapID() == GetBaseMapID())
            {
                VarSetNumber("ReallyControlTimer", 1);
                HolidayEventZoneSetHeartBeat();
            }
            else
            {
                VarSetNumber("ReallyControlTimer", 0);
            }
            if (VarGetNumber("ControlTimer") == 0)
            {
                OnEntityDefeated(HolidayEventZoneOnEntityDefeat);
                OnPlayerEnteringMap(HolidayEventZoneOnEnterMap);
                OnPlayerExitingMap(HolidayEventZoneOnLeaveMap);

                HolidayEventZoneSpawnLordWinterAndMinions();
                VarSetNumber("FoggySky", SkyFileGetByName("Foggy_Winter06.txt"));
                VarSetNumber("TicketGoodTime", MapGetDataToken(PLAYER_COOKIE_TOKEN));
                VarSetNumber("LastHeartBeat", -1);

                ScriptUITitle("TitleWidget", "TitleText", "TitleHint");
                ScriptUITimer("TimerWidget", "BootTimerUI", VarGet("TimeTillBootout"), "");
                ScriptUICreateCollection("LWRealmUI", 2, "TitleWidget", "TimerWidget");
                HolidayEventZoneGetBootTime();
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

        if (VarGetNumber("ReallyControlTimer") && DoEvery("HeartBeatTimer", 5.0f / 60.0f, NULL))
        {
            HolidayEventZoneSetHeartBeat();
        }

        if (VarGetNumber("ControlTimer") == 0)
        {
            if (DoEvery("HeartBeatCheck", 15.0f / 60.0f, NULL))
            {
                heartbeat = HolidayEventZoneGetHeartBeat();
                VarSetNumber("LocalHeartBeat", heartbeat);
                if (VarGetNumber("LastHeartBeat") != heartbeat)
                {
                    switch (heartbeat)
                    {
                    // Only a little to do at the start, most of the interesting stuff happens at the end
                    xcase 0:
                        HolidayEventZoneGetBootTime();

                    xcase WINTER_EVENT_CYCLE_DURATION - 4:
                        SendPlayerAttentionMessageWithColor(ALL_PLAYERS, VarGet("MessageOne"), 0x6070ffff, kFloaterStyle_Attention);
                        SendPlayerAttentionMessageWithColor(ALL_PLAYERS, VarGet("MessageTwo"), 0x6070ffff, kFloaterStyle_Attention);
                        SendPlayerAttentionMessageWithColor(ALL_PLAYERS, VarGet("MessageThree"), 0x6070ffff, kFloaterStyle_Attention);
                        
                    xcase WINTER_EVENT_CYCLE_DURATION - 2:
                        if (GetMapID() != GetBaseMapID())
                        {
                            // If this is not the base map, cause it to shut down.
                            dbRequestShutdown();
                        }
                        else
                        {
                            // Otherwise repopulate
                            HolidayEventZoneSpawnLordWinterAndMinions();
                        }

                    xcase WINTER_EVENT_CYCLE_DURATION - 1:
                        VarSetNumber("TicketGoodTime", MapGetDataToken(PLAYER_COOKIE_TOKEN));
                        VarSetNumber("ForceBootFlag", 0);
                        if (GetMapID() != GetBaseMapID())
                        {
                            // If this is not the base map, cause it to shut down.
                            dbRequestShutdown();
                        }
                    }
                    VarSetNumber("LastHeartBeat", heartbeat);
                }
            }

            // Check that we're in the window between 4 minutes and 1 minute from the end of the cycle, if so,
            // then check that current time is past zero time - 15, and that the boot flag isn't already set.
            // If all these conditions are met, set the force boot flag.
            // The minus - 15 seconds makes us set this flag 15 seconds early, however the synchronization in
            // The block immediately below makes sure we don't kick players too early.
            if (heartbeat >= WINTER_EVENT_CYCLE_DURATION - 4 && heartbeat < WINTER_EVENT_CYCLE_DURATION - 1 &&
                            SecondsSince2000() >= VarGetNumber("BootTimerUI") - 15 && VarGetNumber("ForceBootFlag") == 0)
            {
                VarSetNumber("ForceBootFlag", 1);
            }

            // If current time is past the earliest we can do a boot check ...
            if (SecondsSince2000() > VarGetNumber("EarliestNextBoot"))
            {
                // This counts from 0 to 29 every thirty seconds, but synchronized so it hits zero right when 
                // SecondsSince2000() == VarGetNumber("BootTimerUI").  The net result of this is that a zero
                // will happen when the "kick out timer" in Lord Winter's Realm reaches zero.  This is the
                // synchronization referred to just above.  I do the test for < 5 so there's a five second
                // window to execute the bootout.  The "EarliestNextBoot" logic prevents two calls during a
                // single 5 second window.  The net result of all this is that HolidayEventZoneBootPlayers(...)
                // cannot be called more often than once every 30 seconds.  This prevents the problem caused by
                // two calls in rapid succession, where the second one would overwrite and destroy the spawnTarget
                // value set by the first one, thus dropping players in incorrect locations.
                if ((SecondsSince2000() - VarGetNumber("BootTimerUI") + 3000000) % 30 < 5)
                {
                    HolidayEventZoneBootPlayers(0);
                    // Set it so we can't do a boot check any time in the next twenty seconds.  
                    VarSetNumber("EarliestNextBoot", SecondsSince2000() + 20);
                }
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
        }

    //////////////////////////////////////////////////////////////////////////////////////
    // S H U T D O W N
    //////////////////////////////////////////////////////////////////////////////////////
    STATE("Shutdown") ////////////////////////////////////////////////////////
        HolidayEventZoneEnd();
        EndScript();
    END_STATES

    ON_SIGNAL("End")
        SET_STATE("Shutdown");
    END_SIGNAL

    ON_STOPSIGNAL
        HolidayEventZoneEnd();
    END_SIGNAL
}

void HolidayEventZoneInit()
{
    SetScriptName("HolidayEventZone");
    SetScriptFunc(HolidayEventZone);
    SetScriptType(SCRIPT_ZONE);

    SetScriptSignal("End", "End");

    SetScriptVar("ZoneIndex",                NULL,        0);
    SetScriptVar("ControlTimer",            NULL,        0);

    SetScriptVar("SpawnLocation",            NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("SpawnGroup",                NULL,        SP_DONTDISPLAYDEBUG);

    SetScriptVar("CrystalSpawns",            NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("CrystalList",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("SpawnFX",                    NULL,        SP_DONTDISPLAYDEBUG);

    SetScriptVar("MessageOne",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("MessageTwo",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("MessageThree",            NULL,        SP_DONTDISPLAYDEBUG);

    SetScriptVar("GuardianTextOne",            NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("GuardianTextTwo",            NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("GuardianTextThree",        NULL,        SP_DONTDISPLAYDEBUG);

    SetScriptVar("TitleText",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("TitleHint",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("TimeTillBootout",            NULL,        SP_DONTDISPLAYDEBUG);

    SetScriptVar("TetherRadius",            "250.0",    SP_DONTDISPLAYDEBUG | SP_OPTIONAL); 
}
