// ZONE SCRIPT
//

#include "script/scriptutil.h"
#include "script/scriptengine.h"
#include <utilitieslib/utils/file.h>
#include "gameComm/svr_chat.h"
#include "HolidayEvent.h"

static GLOWIEDEF HolidayEventPresent = NULL;
static GLOWIEDEF HolidayEventSpecialPresent = NULL;
static int *HolidayEventLocationUsed = NULL;
static int *HolidayEventContacts = NULL;
static StashTable HolidayEventPresentEnts;
static int presentMapMarker;

// Tunable value.  Minimum distance a player has to be from a sigil before the UI appears
#define UI_APPEAR_RADIUS            200.0f

// Tunable value.  Maximum distance a player has to be from a sigil before the UI disappears
#define UI_DISAPPEAR_RADIUS            250.0f

static void chatSendDebugPrintf(char *format, ...)
{
    va_list ap;
    char buf[16384];

    if (isDevelopmentMode())
    {
        va_start(ap, format);
        vsnprintf(buf, 16384, format, ap);
        buf[16383] = 0;
        va_end(ap);

        chatSendDebug(buf);
    }
}

// Only run in city zones that have the markers we expect, or pocket D (map 83)
static int CorrectCityZone()
{
    return (LocationExists(VarGetArrayElement("Markers", 0)) || GetBaseMapID() == 83);
}

static void ValidateGlowieDef()
{
    if (HolidayEventPresent == NULL)
    {
        // create present glowie
        HolidayEventPresent = GlowieCreateDef(StringLocalize(VarGet("PresentName")), VarGet("PresentModel"), 
                                                VarGet("PresentInteract"), VarGet("PresentInterrupt"), 
                                                VarGet("PresentComplete"), VarGet("PresentInteractBar"), 
                                                VarGetNumber("PresentInteractTime"));
        GlowieAddEffect(HolidayEventPresent, kGlowieEffectCompletion, "Explosions/Confetti/XmasConfettiExplosion.fx");
    }
    if (HolidayEventSpecialPresent == NULL)
    {
        // create special present glowie
        HolidayEventSpecialPresent = GlowieCreateDef(StringLocalize(VarGet("PortalPresentName")), VarGet("PortalPresentModel"), 
                                                NULL, NULL, NULL, NULL, 0);
    }
}


static NUMBER GetRespawnTime()
{
    return SecondsSince2000() + (RandomNumber(VarGetNumber("PresentRespawnMin"), VarGetNumber("PresentRespawnMax")) * 60);
}

static void SpawnContacts()
{
    ENCOUNTERGROUP group; 
    int count, i;
    STRING loc, spawn, def;

    if (!VarGetNumber("ContactsSpawned")) 
    {
        count = VarGetArrayCount("ContactList");

        for (i = 0; i < count; i++)
        {
            loc = VarGetArrayElement("ContactList", i);
            spawn = VarGetArrayElement("ContactSpawnList", i);
            def = VarGetArrayElement("ContactDefList", i);

            // spawn contact hero one
            group = FindEncounterGroupByRadius( 
                loc,
                0, 
                100, 
                "HolidayContact", 
                1, 
                 0 );  

            if (stricmp(group, "location:none") != 0) {
                Spawn( 1, "Contact", spawn, group, "HolidayContact", 1, 0 );  

                // add on click handler
                OnScriptedContactInteractFromContactDef(def, ActorFromEncounterPos(group,1));
                ScriptRegisterNewContact(ScriptGetContactHandle(def), loc);
            }
        }

        VarSetNumber("ContactsSpawned", 1);
    }
}

static int PlayersNearby(LOCATION loc)
{
    int i, found = false;
    int    groupNumber = NumEntitiesInTeam( ALL_PLAYERS );        

    for( i = 1 ; i <= groupNumber ; i++ )
    {
        ENTITY e = GetEntityFromTeam(ALL_PLAYERS, i); 
        if (DistanceBetweenLocations(e, loc) < VarGetFraction("ClosePlayerDist"))
        {
            found = true;
            break;
        }
    }
    return found;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Read the global zone index.  Very small, but kept as a separate routine in case we ever
// have to put any smarts in here
static void HolidayEventGetTargetZoneData()
{
    VarSetNumber("TargetZone", MapGetDataToken(ZONE_TARGET_TOKEN));
    VarSetNumber("EntryTime", MapGetDataToken(ZONE_ENTRYTIME_TOKEN));
}


/////////////////////////////////////////////////////////////////////////////////////////////
// A slight misnomer now.  Originally VarGet("BigGuyLocation") returned a "coord:x,y,z" location,
// thus this routine had to do a distance check.  It now holds the marker name where the big guy
// and the portal present will spawn, which means a StringEqual(...) call is sufficient.
static int SpecialPresentNearby(LOCATION loc)
{
    return VarGetNumber("PortalExclusion") != -1 && StringEqual(VarGet("BigGuyLocation"), loc);
}

int HolidayEventPresentComplete(ENTITY player, ENTITY target)
{
    int loc;
    ENCOUNTERGROUP group; 
    char spawnGroup[50] = "";
    int teamCount = NumEntitiesInTeam(GetPlayerTeamFromPlayer(player));

    // do present thing!
    if (RandomNumber(0, 100) < VarGetNumber("AmbushChance"))
    {
        // ambush
        group = FindEncounterGroupByRadius( 
            PointFromEntity(target),
            0, 
            30, 
            VarGet("SpawnLayout"), 
            1, 
            0 );  

        if (stricmp(group, "location:none") != 0) 
        {
            stashFindInt(HolidayEventPresentEnts, target, &loc);
            sprintf_s(spawnGroup, 50, "Ambush%d", loc);

            Spawn( 1, spawnGroup, VarGet("SpawnDef"), group, VarGet("SpawnLayout"), 
                        RandomNumber(VarGetNumber("SpawnLevelMin"), VarGetNumber("SpawnLevelMax")), teamCount );  

            SetScriptTeam(spawnGroup, "Ambush");
            SetAIPriorityList(spawnGroup, "Combat");
            // Unhitch these guys so that if we need to use this spawn to create the boss group, we can
            DetachSpawn(spawnGroup);
        }

        // you've been naughty
        SendPlayerAttentionMessage(player, StringLocalize(VarGet("PresentNaughty")));

    }
    else
    {
        // reward
        EntityGrantReward(player, VarGet("Reward"));

        // you've been nice
        SendPlayerAttentionMessage(player, StringLocalize(VarGet("PresentNice")));

    }

    // give credit
    GiveBadgeCredit(player, kScriptBadgeTypePresents);

    // remove glowie
    GlowieRemove(target);

    // remove from table
    if (stashRemoveInt(HolidayEventPresentEnts, target, &loc))
        HolidayEventLocationUsed[loc] = GetRespawnTime();    

    return true;
}

int HolidayEventSpecialPresentInteract(ENTITY player, ENTITY target)
{
    int zoneIndex;
    STRING zoneName;
    STRING spawnName;
    STRING fullMapName;
    STRING mapName;
    char returnCoords[96];
    Vec3 xpos;

    if (VarGetNumber("PortalState") != 2)
    {
        ScriptSendDialog(player, StringLocalize(VarGet("CantOpenThisYet")));
        return 1;
    }

    zoneIndex = VarGetNumber("TargetZone");
    
    zoneName = VarGetArrayElement("ZoneNameList", zoneIndex);
    spawnName = VarGet("SpawnTarget");

    fullMapName = GetMapName();
    if ((mapName = strrchr(fullMapName, '/')) != NULL || (mapName = strrchr(fullMapName, '\\')) != NULL)
    {
        mapName++;
    }
    else
    {
        mapName = fullMapName;
    }
    GetPointFromLocation(player, xpos);
    snprintf(returnCoords, 96, "%s;coord:%.1f,%.1f,%.1f", mapName, xpos[0], xpos[1], xpos[2]);

    // Re-enable this since the race condition in HolidayEventZone.c that caused return problems has
    // been fixed.
    // Save the return information that allows the player to come back here on exit from Lord
    // Winter's Realm
    SetSpecialMapReturnData(player, "WinterEvent", returnCoords);
    EntityGrantRewardToken(player, PLAYER_TICKET_TOKEN, MapGetDataToken(PLAYER_COOKIE_TOKEN));
    ScriptEnterScriptDoor(player, zoneName, spawnName);
    return 1;
}

void HolidayEventResetCritterCount(int panic)
{
    VarSetNumber("BigGuyDelayActive", 0);
    VarSet("BigGuyLocation", NULL),
    TimerRemove("BigGuyTimer");
    VarSetNumber("PortalExclusion", -1);
    VarSetNumber("PortalState", 0);
    TimerRemove("PortalTimer");
    VarSetNumber("ResetDelayActive", 0);
    TimerRemove("ResetTimer");
    VarSetNumber("CritterDefeatsNeeded", panic ? 10 : RandomNumber(VarGetNumber("MinimumCrittersForBigGuy"), VarGetNumber("MaximumCrittersForBigGuy")));
}

void HolidayEventPresentCheck(void)
{
    int i, count;
    char spawnGroup[50];
    ENTITY glowie;
    STRING loc;

    // no presents in pocket D
    if (GetBaseMapID() == 83)
        return;

    // clear this out, since any old presents might be deleted and freed
    HolidayEventPresent = NULL;

    // go through all of the event markers and check to see if we should create a present!
    count = VarGetArrayCount("Markers");

    for (i = 0; i < count; i++)
    {
        if (HolidayEventLocationUsed[i] > 0 && HolidayEventLocationUsed[i] < SecondsSince2000())
        {
            loc = VarGetArrayElement("Markers", i);
            // check if players are around
            if (!PlayersNearby(loc) && !SpecialPresentNearby(loc)) {
                ValidateGlowieDef();
                glowie = GlowiePlace(HolidayEventPresent, loc, false, NULL, HolidayEventPresentComplete);
                if (glowie) { // validation that a glowie was created
                    stashAddInt(HolidayEventPresentEnts, glowie, i, true);
                }
                HolidayEventLocationUsed[i] = 0;

                // cleanup any left over monsters
                sprintf_s(spawnGroup, 50, "Ambush%d", i);
                Kill(spawnGroup, false);

            } else {
                HolidayEventLocationUsed[i] = GetRespawnTime(); // reset time even if we couldn't place a new one
            }
        }
    }
}

void HolidayEventEnd(int freeThings)
{
    int i, count, results;
    StashTableIterator iter;
    StashElement elem;

    if (GetBaseMapID() != 83)
    {
        // clean up glowies
        stashGetIterator(HolidayEventPresentEnts, &iter);
        while (stashGetNextElement(&iter, &elem))
        {
            // remove glowie
            GlowieRemove((ENTITY) stashElementGetStringKey(elem));

            stashRemoveInt(HolidayEventPresentEnts, stashElementGetStringKey(elem), &results);
        }

        count = VarGetArrayCount("Markers");
        for (i = 0; i < count; i++)
        {
            HolidayEventLocationUsed[i] = GetRespawnTime();
        }
    }

    // clean up and remaining ambushes
    Kill("Ambush", false);

    // remove contacts (if they exist)
    Kill("Contact", false);
    VarSetNumber("ContactsSpawned", 0);

    // remove map tokens
    MapRemoveDataToken(VarGet("ContactMapToken"));
    MapRemoveDataToken(VarGet("ResortOpenToken"));

    // remove callbacks
    OnPlayerEnteringMap( NULL );
    OnEntityDefeated( NULL );

    if (freeThings)
    {
        // clean up arrays
        if (HolidayEventLocationUsed)
            eaiDestroy(&HolidayEventLocationUsed);
        if (HolidayEventContacts)
            eaiDestroy(&HolidayEventContacts);
        if (HolidayEventPresentEnts)
            stashTableDestroy(HolidayEventPresentEnts);
    }
}


//////////////////////////////////////////////////////////////////////////////////////
// E N T E R   C A L L B A C K
//////////////////////////////////////////////////////////////////////////////////////
// Sets up contact for players who don't have it!
int HolidayEventOnEnterMap(ENTITY player)
{
    int i, found = false;
    int count = VarGetArrayCount("ContactList");
    int index;

    // check to see if the player has any of the event contacts
    for (i = 0; i < count; i++)
    {
        if (HolidayEventContacts[i])
        {
            if (DoesPlayerHaveContact(player, HolidayEventContacts[i]))
            {
                found = true;
                break;
            }
        }
    }

    if (!found)
    {
        // only one contact this year!
        index = 0;
    
        // give contact
        ScriptGivePlayerNewContact(player, HolidayEventContacts[index]);

        // send popup
        SendPlayerDialog(player, StringLocalize(VarGetArrayElement("ContactDescList", index)));

    }

    if (presentMapMarker != 0)
    {
        SetWaypoint(player, presentMapMarker);
    }

    SetScriptTeam(player, "NoUITeam");

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// Called when a player leaves the map - clear any minimap waypoints that might be set
//
int HolidayEventOnLeaveMap(ENTITY player)
{
    // remove waypoints
    ClearAllWaypoints(player);

    LeaveAllScriptTeams(player);

    ScriptUIHide("Collection:PresentUI", player);

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////
// D E F E A T   C A L L B A C K
//////////////////////////////////////////////////////////////////////////////////////
// Called when anything defeats anything else.  This keeps track of critter counts needed
// to spawn the big guy, as well as actual defeats of the big guy
//
static void HolidayEventOnEntityDefeat(ENTITY player, ENTITY victor)
{
    NUMBER critterDefeatsNeeded;

//    ENTITY specialGlowie;
    NUMBER count;
    NUMBER bestLoc;
    STRING playerLoc;
    STRING markerLoc;
    FRACTION dist;
    FRACTION bestDist;
    int i;

    if (IsEntityOnScriptTeam(player, "Ambush"))
    {
        critterDefeatsNeeded = VarGetNumber("CritterDefeatsNeeded");
        if (critterDefeatsNeeded && VarGetNumber("BigGuyDelayActive") == 0)
        {
            VarSetNumber("CritterDefeatsNeeded", --critterDefeatsNeeded);
            if (critterDefeatsNeeded == 0)
            {
                // go through all of the event markers and check to see if we should create a present!
                count = VarGetArrayCount("Markers");
                bestDist = 1000000.0f;
                playerLoc = PointFromEntity(player);

                for (i = 0; i < count; i++)
                {
                    markerLoc = VarGetArrayElement("Markers", i);
                    dist = DistanceBetweenLocations(playerLoc, markerLoc);
                    if (dist < bestDist && HolidayEventLocationUsed[i] > 0)
                    {
                        bestDist = dist;
                        bestLoc = i;
                    }
                }

                if (bestDist < 1000000.0f)
                {
                    VarSetNumber("BigGuyDelayActive", 1);
                    VarSetNumber("PortalExclusion", bestLoc);
                    VarSet("BigGuyLocation", VarGetArrayElement("Markers", bestLoc));
                    TimerSet("BigGuyTimer", VarGetNumber("BigGuyDelay"));
                }
                else
                {
                    HolidayEventResetCritterCount(1);
                }
            }
        }
    }
    else if (StringEqual(player, VarGet("WinterLord")))
    {
        HolidayEventResetCritterCount(0);
    
        // Disabling this section of the Winter event for 2011 due to mapserver bandwidth considerations with the Heartbeat script
        /*
        ValidateGlowieDef();
        specialGlowie = GlowiePlace(HolidayEventSpecialPresent, VarGet("BigGuyLocation"), true, HolidayEventSpecialPresentInteract, NULL);
        GlowieSetState(specialGlowie);

        presentMapMarker = CreateWaypoint(VarGet("BigGuyLocation"), VarGet("PortalPresentName"), 
                                                    VarGet("PortalPresentIcon"), NULL, false, false, -1.0f);
        if (presentMapMarker > 0)
        {
            SetWaypoint(ALL_PLAYERS, presentMapMarker);
        }

        // If we want to announce in the appropriate event channel that a presnt is open, this is where we'd do so
        // If we want to send a zone wide floater, that would also happen here

        VarSet("SpecialGlowie", specialGlowie);
        VarSetNumber("PortalState", 1);
        VarSet("CurrentUI", "Collection:PresentUI");
        VarSetNumber("UIPhase", 0);
        // Read the current zone target once, and cache it.
        HolidayEventGetTargetZoneData();
        VarSetNumber("PresentTimerUI", VarGetNumber("EntryTime"));
        SendPlayerAttentionMessageWithColor(ALL_PLAYERS, VarGet("PresentFloater"), 0x6070ffff, kFloaterStyle_Attention);
        */
    }
}

void HolidayEvent(void)
{
    int i;
    int n;
    int count;
    STRING mapName;
    ENCOUNTERGROUP group; 
    ENTITY specialGlowie;
    ENTITY currentPlayer;
    NUMBER resetDelay;
    NUMBER portalState;
    NUMBER uiPhase;

    //////////////////////////////////////////////////////////////////////////////////////
    // S E T U P
    //////////////////////////////////////////////////////////////////////////////////////
    INITIAL_STATE

        ON_ENTRY 
            if (!CorrectCityZone()) {
                EndScript();
            } else {
                if (GetBaseMapID() != 83)
                {
                    // find min/max spawn level
                    count = VarGetArrayCount("SpawnLevelMapList");
                    mapName = GetShortMapName();
                    for (i = 0; i < count; i++) {
                        if (StringEqual(mapName, VarGetArrayElement("SpawnLevelMapList", i)))
                        {
                            VarSetNumber("SpawnLevelMin", StringToNumber(VarGetArrayElement("SpawnLevelMinList", i)));
                            VarSetNumber("SpawnLevelMax", StringToNumber(VarGetArrayElement("SpawnLevelMaxList", i)));
                            break;
                        }
                    }
                    if (VarGetNumber("SpawnLevelMin") == 0 || VarGetNumber("SpawnLevelMax") == 0)
                        EndScript();


                    // setup used list for glowies
                    count = VarGetArrayCount("Markers");
                    eaiCreate(&HolidayEventLocationUsed);
                    eaiSetSize(&HolidayEventLocationUsed, count);
                    for (i = 0; i < count; i++) {
                        HolidayEventLocationUsed[i] = GetRespawnTime();
                    }

                    // setup hash to track glowies in use for cleanup
                    HolidayEventPresentEnts = stashTableCreateWithStringKeys(count, StashDeepCopyKeys);
                }

                // setup contacts
                count = VarGetArrayCount("ContactList");
                eaiCreate(&HolidayEventContacts);
                eaiSetSize(&HolidayEventContacts, count);
                for (i = 0; i < count; i++) {
                    HolidayEventContacts[i] = ScriptGetContactHandle(VarGetArrayElement("ContactDefList", i));
                }
                presentMapMarker = 0;
            }

        END_ENTRY
        SET_STATE("PhaseThree");

    //////////////////////////////////////////////////////////////////////////////////////
    // P H A S E - O N E
    //////////////////////////////////////////////////////////////////////////////////////
    //        Open Rink
    //////////////////////////////////////////////////////////////////////////////////////

    STATE("PhaseOne") ////////////////////////////////////////////////////////
        ON_ENTRY 
        {
            // put in neutral state
            HolidayEventEnd(false);
            MapSetDataToken(VarGet("ResortOpenToken"), 1);
        }
        END_ENTRY

    //////////////////////////////////////////////////////////////////////////////////////
    // P H A S E - T W O
    //////////////////////////////////////////////////////////////////////////////////////
    //        Mission Contacts & Rink
    //////////////////////////////////////////////////////////////////////////////////////
    STATE("PhaseTwo") ////////////////////////////////////////////////////////
        ON_ENTRY 
        {
            MapSetDataToken(VarGet("ContactMapToken"), 1);
            MapSetDataToken(VarGet("ResortOpenToken"), 1);

            // setup mission contacts
            SpawnContacts();
            OnPlayerEnteringMap(HolidayEventOnEnterMap);
            OnPlayerExitingMap(HolidayEventOnLeaveMap);
        }
        END_ENTRY


    //////////////////////////////////////////////////////////////////////////////////////
    // P H A S E - T H R E E
    //////////////////////////////////////////////////////////////////////////////////////
    //        Presents, Mission Contacts & Rink
    //////////////////////////////////////////////////////////////////////////////////////
    STATE("PhaseThree") ////////////////////////////////////////////////////////
        ON_ENTRY 
        { 
            MapSetDataToken(VarGet("ContactMapToken"), 1);
            MapSetDataToken(VarGet("ResortOpenToken"), 1);

            // setup mission contacts
            SpawnContacts();
            // Set up hooks
            OnPlayerEnteringMap(HolidayEventOnEnterMap);
            OnPlayerExitingMap(HolidayEventOnLeaveMap);
            OnEntityDefeated(HolidayEventOnEntityDefeat);
            HolidayEventResetCritterCount(0);

            ScriptUITitle("TitleWidget", "TitleText", "TitleHint");
            ScriptUITimer("TimerWidget", "PresentTimerUI", VarGet("PresentDelay"), "");
            ScriptUICreateCollection("PresentUI", 2, "TitleWidget", "TimerWidget");
            ScriptUICreateCollection("PresentOnlyUI", 1, "TitleWidget");
        }
        END_ENTRY

        DoEvery("PresentCheck", VarGetFraction("PresentCheckTime"), HolidayEventPresentCheck);

        if (VarGetNumber("BigGuyDelayActive") && TimerElapsed("BigGuyTimer"))
        {
            // ambush
            group = FindEncounterGroupByRadius( 
                VarGet("BigGuyLocation"),
                0, 
                30, 
                VarGet("SpawnLayout"), 
                1, 
                0 );  

            if (stricmp(group, "location:none") != 0) 
            {
                int found;
                ENTITY winterLord;
                STRING critterClass;

                Spawn(1, "BossGroup", VarGet("SpawnDefHi"), group, VarGet("SpawnLayout"), 
                        RandomNumber(VarGetNumber("SpawnLevelMin"), VarGetNumber("SpawnLevelMax")), 8);

                SetAIPriorityList("BossGroup", "Combat");

                n = NumEntitiesInTeam("BossGroup");
                VarSet("WinterLord", NULL);
                found = 0;
                for (i = 1; i <= n; i++)
                {
                    winterLord = GetEntityFromTeam("BossGroup", i);
                    critterClass = GetVillainDefClassName(winterLord);
                    if (StringEqual(critterClass, "Class_Boss_Monster"))
                    {
                        VarSet("WinterLord", winterLord);
                        SendPlayerAttentionMessageWithColor(ALL_PLAYERS, VarGet("WinterLordFloater"), 0x6070ffff, kFloaterStyle_Attention);
                        found = 1;
                        break;
                    }
                }
                // In the extremely unlikely event that we don't find the winter lord, fail gracefully, and just set the counter to a small number
                if (!found)
                {
                    HolidayEventResetCritterCount(1);
                }
            }
            else
            {
                HolidayEventResetCritterCount(1);
            }
            VarSetNumber("BigGuyDelayActive", 0);
            TimerRemove("BigGuyTimer");
        }
        portalState = VarGetNumber("PortalState");
        if (portalState)
        {
            specialGlowie = VarGet("SpecialGlowie");
            uiPhase = VarGetNumber("UIPhase");
            if (uiPhase == 0)
            {
                n = NumEntitiesInTeam("NoUITeam");
                for (i = n; i > 0; i--)
                {
                    currentPlayer = GetEntityFromTeam("NoUITeam", i);
                    if (DistanceBetweenEntities(currentPlayer, specialGlowie) < UI_APPEAR_RADIUS)
                    {
                        SwitchScriptTeam(currentPlayer, "NoUITeam", "UITeam");
                        ScriptUIShow(VarGet("CurrentUI"), currentPlayer);
                    }
                }
                VarSetNumber("UIPhase", 1);
            }
            else if (uiPhase == 1)
            {
                VarSetNumber("UIPhase", 2);
            }
            else if (uiPhase == 2)
            {
                n = NumEntitiesInTeam("UITeam");
                for (i = n; i > 0; i--)
                {
                    currentPlayer = GetEntityFromTeam("UITeam", i);
                    if (DistanceBetweenEntities(currentPlayer, specialGlowie) > UI_DISAPPEAR_RADIUS)
                    {
                        SwitchScriptTeam(currentPlayer, "UITeam", "NoUITeam");
                        ScriptUIHide(VarGet("CurrentUI"), currentPlayer);
                    }
                }
                VarSetNumber("UIPhase", 3);
            }
            else
            {
                VarSetNumber("UIPhase", 0);
            }
            if (portalState == 1)
            {
                if (SecondsSince2000() >= VarGetNumber("EntryTime"))
                {
                    ScriptUIHide("Collection:PresentUI", "UITeam");
                    ScriptUIShow("Collection:PresentOnlyUI", "UITeam");
                    VarSetNumber("PortalState", 2);
                    VarSet("CurrentUI", "Collection:PresentOnlyUI");
                    TimerSet("PortalTimer", VarGetFraction("PortalPresentDuration"));
                }
            }
            else if (portalState == 2)
            {
                if (TimerElapsed("PortalTimer"))
                {
                    resetDelay = VarGetNumber("ResetDelay");
                    GlowieRemove(VarGet("SpecialGlowie"));
                    HolidayEventSpecialPresent = NULL;
                    // remove map marker
                    if (presentMapMarker != 0)
                    {
                        DestroyWaypoint(presentMapMarker);
                        presentMapMarker = 0;
                    }

                    if (resetDelay)
                    {
                        VarSet("ResetDelayActive", "1");
                        TimerSet("ResetTimer", (FRACTION) resetDelay);
                    }
                    else
                    {
                        HolidayEventResetCritterCount(0);
                    }
                    VarSetNumber("PortalExclusion", -1);
                    VarSetNumber("PortalState", 0);
                    TimerRemove("PortalTimer");
                    VarSet("CurrentUI", NULL);

                    ScriptUIHide("Collection:PresentOnlyUI", "UITeam");
                    SwitchScriptTeam(ALL_PLAYERS, "UITeam", "NoUITeam");
                }
            }
        }
        if (VarGetNumber("ResetDelayActive") && TimerElapsed("ResetTimer"))
        {
            HolidayEventResetCritterCount(0);
        }

    //////////////////////////////////////////////////////////////////////////////////////
    // S H U T D O W N
    //////////////////////////////////////////////////////////////////////////////////////
    STATE("Shutdown") ////////////////////////////////////////////////////////

        HolidayEventEnd(true);
        EndScript();
    END_STATES

    ON_SIGNAL("PhaseOne")
        SET_STATE("PhaseOne");
    END_SIGNAL

    ON_SIGNAL("PhaseTwo")
        SET_STATE("PhaseTwo");
    END_SIGNAL

    ON_SIGNAL("PhaseThree")
        SET_STATE("PhaseThree");
    END_SIGNAL
        

    ON_SIGNAL("End")
        SET_STATE("Shutdown");
    END_SIGNAL

    ON_STOPSIGNAL
        HolidayEventEnd(true);
    END_SIGNAL
}

void HolidayEventInit()
{
    SetScriptName( "HolidayEvent" );
    SetScriptFunc( HolidayEvent );
    SetScriptType( SCRIPT_ZONE );        

    SetScriptSignal( "End", "End" );        
    SetScriptSignal( "Phase One", "PhaseOne" );        
    SetScriptSignal( "Phase Two", "PhaseTwo" );        
    SetScriptSignal( "Phase Three", "PhaseThree" );        

    SetScriptVar( "Markers",                                "",            SP_DONTDISPLAYDEBUG );
    SetScriptVar( "SpawnLayout",                            "Around",    SP_DONTDISPLAYDEBUG );

    SetScriptVar( "SpawnLevelMapList",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "SpawnLevelMinList",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "SpawnLevelMaxList",                        NULL,        SP_DONTDISPLAYDEBUG );

    SetScriptVar( "ClosePlayerDist",                        "200",        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "AmbushChance",                            NULL,        0 );
    SetScriptVar( "GiantChance",                            NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "SpawnDef",                                NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "SpawnDefHi",                                NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "SpawnDefLo",                                NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "SpawnDefHiCutoff",                        NULL,        0 );
    SetScriptVar( "Reward",                                    NULL,        0 );

    SetScriptVar( "ContactList",                            NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "ContactSpawnList",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "ContactDefList",                            NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "ContactDescList",                        NULL,        SP_DONTDISPLAYDEBUG );

    SetScriptVar( "ContactMapToken",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "ResortOpenToken",                        NULL,        SP_DONTDISPLAYDEBUG );

    SetScriptVar( "PresentCheckTime",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PresentRespawnMin",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PresentRespawnMax",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PresentName",                            NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PresentModel",                            NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PresentInteract",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PresentInteractBar",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PresentInteractTime",                    NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PresentInterrupt",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PresentComplete",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PresentNaughty",                            NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PresentNice",                            NULL,        SP_DONTDISPLAYDEBUG );

    SetScriptVar( "MinimumCrittersForBigGuy",                "400",        SP_DONTDISPLAYDEBUG | SP_OPTIONAL );
    SetScriptVar( "MaximumCrittersForBigGuy",                "500",        SP_DONTDISPLAYDEBUG | SP_OPTIONAL );
    SetScriptVar( "BigGuyDelay",                            "1",        SP_DONTDISPLAYDEBUG | SP_OPTIONAL );
    SetScriptVar( "PortalPresentName",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PortalPresentModel",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PortalPresentIcon",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PortalPresentDuration",                    "4",        SP_DONTDISPLAYDEBUG | SP_OPTIONAL );
    SetScriptVar( "ResetDelay",                                "5",        SP_DONTDISPLAYDEBUG | SP_OPTIONAL );
    SetScriptVar( "CantOpenThisYet",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "WinterLordFloater",                        NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PresentFloater",                            NULL,        SP_DONTDISPLAYDEBUG );
    
    SetScriptVar( "TitleText",                                NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "TitleHint",                                NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "PresentDelay",                            NULL,        SP_DONTDISPLAYDEBUG );

    SetScriptVar( "ZoneNameList",                            NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "SpawnTarget",                            NULL,        SP_DONTDISPLAYDEBUG );
}
