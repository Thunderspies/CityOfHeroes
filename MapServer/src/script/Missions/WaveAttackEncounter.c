// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"
#include <utilitieslib/utils/timing.h>

//TO DO waypoints?
//TO DO waypoints?
//Pick a random selection of minions to say something
static void MinionSays( TEAM team, STRING says )
{
    int numEnts;
    ENTITY minion;

    if( team && says )
    {
        numEnts = NumEntitiesInTeam(team);
        minion = GetEntityFromTeam(team, RandomNumber(1, numEnts) );
        if( numEnts )
            Say( minion, says, 0 );
    }
}

static void UpdateUI()
{
    int i;

    if ( VarGetNumber( "TimeToProtect" ) > 0 )
    {
        // check UI
        for (i = 0; i < NumEntitiesInTeam(ALL_PLAYERS); i++)
        {    
            ENTITY targetEnt = GetEntityFromTeam(ALL_PLAYERS, i + 1 );
            if (DistanceBetweenLocations(EncounterPos(MyEncounter(), 1), PointFromEntity(targetEnt)) > VarGetFraction( "TimerRadius"))
            {
                ScriptUIHide("Collection:WaveAttackUI", targetEnt);
            } else {
                ScriptUIShow("Collection:WaveAttackUI", targetEnt);
            }
        }
    }
}

static void WaveAttackShutdownScript(NUMBER state)
{
    SetEncounterComplete( MyEncounter(), state);
    EndScript();
}


void checkForSuccessOrFailure()
{
    if( VarGetNumber( "TimeToProtect" ) == 0 && VarGetNumber( "WaveNumber" ) >= VarGetNumber( "NumberOfWaves" ) && NumEntitiesInTeam("WaveAttackers") == 0)
    {
        SET_STATE( "EndWaveAtacks" );
    }
    else if ( VarGetNumber( "TimeToProtect" ) > 0  && TimerElapsed("Clock") ) 
    {
        SET_STATE( "EndWaveAtacks" );
    }
    else if (StringEqual(VarGet( "ObjectiveActorName" ), "All")) 
    {
        if( NumEntitiesInTeam( MyEncounter()) < 1 )
        {
            // everything has been destroyed
            WaveAttackShutdownScript(false);
        }
    } 
    else if (!StringEqual(VarGet( "ObjectiveActorName" ), "")) 
    {
        if( 0.0 >= GetHealth( VarGet( "ObjectiveEntity" ) ) )
        {
            WaveAttackShutdownScript(false);
        }
    }
}

void UpdateAmbushers(NUMBER chase)
{
    char *correctBehavior = chase ? "AttackTarget" : "MoveToPos";
    NUMBER i, n, livingPlayers = 0;

    n = NumEntitiesInTeam("Ambushers");
    for(i = 1; i <= n; i++)
    {
        ENTITY ambusher = GetEntityFromTeam("Ambushers", i);

        if(CurAIBehaviorOverridenByCombat(ambusher) || CurAIBehaviorMatchesName(ambusher, correctBehavior))
        {
            // This entity is assumed (because it's top behavior looks right to be following the script), put him on the current ambushers
            SwitchScriptTeam(ambusher, "AwaitingOrders", "CurrentlyAmbushing");
        }
        else
        {
            // This entity is not chasing a target, mark it as awaiting orders
            SwitchScriptTeam(ambusher, "CurrentlyAmbushing", "AwaitingOrders");
        }
    }

    n = NumEntitiesInTeam(ALL_PLAYERS);
    for(i = 1; i <= n; i++)
    {
        ENTITY player = GetEntityFromTeam(ALL_PLAYERS, i);
        if(GetHealth(player) > 0)
            livingPlayers++;
    }

    // Give a chase target to anything still awaiting orders
    if(NumEntitiesInTeam("AwaitingOrders") > 0)
    {
        if(chase)
        {
            if(livingPlayers > 0)
            {
                SetAIAttackTarget("AwaitingOrders", ALL_PLAYERS, LOW_PRIORITY);
                SwitchScriptTeam("AwaitingOrders", "AwaitingOrders", "CurrentlyAmbushing");
            }
        }
        else
        {
            if (StringEqual(VarGet( "ObjectiveActorName" ), "")) 
            {
                if(livingPlayers > 0)
                {
                    SetAIMoveTargetTeam("AwaitingOrders", ALL_PLAYERS, LOW_PRIORITY, 0, 1, 0.0f);
                    SwitchScriptTeam("AwaitingOrders", "AwaitingOrders", "CurrentlyAmbushing");
                }
            } 
            else 
            {
                if (VarIsEmpty( "ObjectiveGotoOverride" ))
                    SetAIMoveTarget("AwaitingOrders", VarGet( "ObjectiveEntity" ), LOW_PRIORITY, 0, 1, 0.0f);
                else
                    SetAIMoveTarget("AwaitingOrders", VarGet( "ObjectiveGotoOverride" ), LOW_PRIORITY, 0, 1, 0.0f);
                SwitchScriptTeam("AwaitingOrders", "AwaitingOrders", "CurrentlyAmbushing");
            }
        }
        
    }
}


void WaveAttackEncounter(void)
{  
    //Lines 
    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            VarSetNumber( "WaveNumber", 0 );
            ScriptControlsCompletion( MyEncounter() );
        
            //Get ObjectiveEntity: guy the Wave Attack is attacking
            if (StringEqual(VarGet( "ObjectiveActorName" ), "")) 
            {

            } 
            else if (StringEqual(VarGet( "ObjectiveActorName" ), "All")) 
            {
                //Pick the first actor as the Target for the Wave Attack to bear down on.
                ENTITY objective = GetEntityFromTeam( MyEncounter(), 1 );
                VarSet( "ObjectiveEntity", objective );
            } 
            else 
            {
                ENTITY objective = ActorFromEncounterActorName( MyEncounter(), VarGet( "ObjectiveActorName" ) );
                if( !objective )
                    EndScript();
                VarSet( "ObjectiveEntity", objective );
            }
        }
        END_ENTRY

        //When this Encounter becomes Active, Start the Wave Attacks. 
        if( EncounterState(MyEncounter()) == ENCOUNTER_ACTIVE ) 
        {
            SET_STATE( "WaveAttacking" );

            //If this Encounter must be protected for a certain amount of time.
            if ( VarGetNumber( "TimeToProtect" ) > 0 )
            { 
                // set timer
                TimerSet( "Clock", VarGetFraction( "TimeToProtect" ));
                VarSetNumber("WaveAttackEnd", VarGetNumber("TimeToProtect")*60 +timerSecondsSince2000());

                // create UI for timer and display
                ScriptUITitle("WaveTitle", "TimerName", "TimerInfo");
                ScriptUITimer("WaveTimer", "WaveAttackEnd", "TimerText", "");
                ScriptUICreateCollection("WaveAttackUI", 2, "WaveTitle", "WaveTimer");
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////
    STATE("WaveAttacking") ////////////////////////////////////////////////////////
    {
        ON_ENTRY
        {
            int flags = SEF_UNOCCUPIED_ONLY | SEF_HAS_PATH_TO_LOC;
            char spawnTemp[512];
            STRING spawn;
            LOCATION target = LOCATION_NONE;
            ENCOUNTERGROUP group; 
            int waveNumber;
            int failed = false;
            STRING layout;

            waveNumber = 1 + VarGetNumber( "WaveNumber" );
            VarSetNumber( "WaveNumber", waveNumber );
                
            // If you are given a particular spawn, use it, otherwise, get a Generic Mission Spawn
            // can be forms:
            // SpawnDefWaveX missionencounter:SomeMissionEncounterName
            // SpawnDefWaveX scripts/spawndefs/taskincluded/MissionMap/Something.spawndef

            sprintf(spawnTemp, "%s%d", "SpawnDefWave", VarGetNumber( "WaveNumber" ));
            
            if (!StringEqual( VarGet(spawnTemp), "") ) 
                spawn = VarGet(spawnTemp); 
            else                     
                spawn = GetGenericMissionSpawn();

            //If the spawn has a particular layout it needs, always use that one, otherwise get the one the Script asked for
            layout = GetSpawnLayout(spawn);
            if (StringEqual( layout, "") ) 
                layout = VarGet("FarSpawnLayout");

            //If the wave attack has no ObjectiveEntity, pick a random player and target him 
            if (StringEqual(VarGet( "ObjectiveActorName" ), "")) 
            {
                NUMBER n;
                n = NumEntitiesInTeam(ALL_PLAYERS);
                if( n )
                {
                    target = GetEntityFromTeam(ALL_PLAYERS, RandomNumber(1, n) );
                }
            } 
            else 
            {
                if (VarIsEmpty( "ObjectiveGotoOverride" ) )
                {
                    target = VarGet( "ObjectiveEntity" );
                } else {
                    target = VarGet( "ObjectiveGotoOverride" );
                }
            }

            if (VarGetNumber("FlyingOk"))
                flags |= SEF_HAS_FLIGHT_PATH_TO_LOC;

            //Try to get a spot based on the layout given
            group = FindEncounter( 
                target,                                        //Relative to this spot
                VarGetNumber( "InnerDoughnut" ), 
                VarGetNumber( "OuterDoughnut" ), 
                0,                                            //Inside this AREA
                0,                                            //Assigned this MissionEncounter (the LogicalName)
                layout,                                        //Layout (Many Named Demon. Called "EncounterSpawn" in Spawndef files and in Maps, "Layouts" in mission specs and scripts and often throughout the code, "Spawntype" in SpawnDef struct, "EncounterLayouts" in mapspec)
                0,                                            //Group Name
                flags                                        //Flags
                );

            if( !StringEqual( group, LOCATION_NONE ) )
            {
                // If you did find a good spot, Spawn Em.
                group = CloneEncounter(group);
                //printf("WaveAttackEncounter() is spawning (#1)...\n");
                Spawn( 1, "WaveAttackers", spawn, group, layout, MissionLevel(), 0 );  
            }
            else // look for occupied groups we can clone
            {   
                group = FindEncounter(
                    target, 
                    VarGetNumber( "InnerDoughnut" ), 
                    VarGetNumber( "OuterDoughnut" ), 
                    0, 
                    0, 
                    layout, 
                    0, 
                    SEF_OCCUPIED_ONLY | SEF_HAS_PATH_TO_LOC );

                if( !StringEqual( group, LOCATION_NONE ) ) 
                { 
                    // yay we found one, clone the encounter and spawn our boys
                    group = CloneEncounter(group);
                    //printf("WaveAttackEncounter() is spawning() (#2)...\n");
                    Spawn( 1, "WaveAttackers", spawn, group, layout, MissionLevel(), 0 );  
                }
                else
                {    
                    // failed to find anything in range occupied or not, next we should either try infinite range
                    // or spawn directly on top the player, but vince doesn't like these ideas

                    failed = true;
                    VarSetNumber("TimesIFailedToGetFarSpawn", VarGetNumber( "TimesIFailedToGetFarSpawn" ) + 1 );//debug

                    //If I didn't get anything, or Guarantee Waves is set, and I didn't get exactly what I wanted, set the wave back
                    if ( (VarGetNumber("GuaranteeWaves") || failed) )
                    {   // roll back wave count since this didn't get the wave asked for
                        waveNumber = VarGetNumber( "WaveNumber" ) - 1;
                        VarSetNumber( "WaveNumber", waveNumber );
                    }
                }
            } 
    
            // If we did get a group, either spawned, or grabbed from already spawned mission encounter
            if( !failed ) 
            { 
                SetEncounterInactive(group);
                AddAIBehavior(group, "Combat");
                if (VarGetNumber("ChasePlayers"))
                {
                    AddAIBehaviorPermFlag(group, "UseEnterable(1)");
                }
                SetScriptTeam(group, "Ambushers");
                
                MinionSays( group, VarGet("WaveShout" ) );
                if (VarGetNumber("DetachSpawns"))
                    DetachSpawn("WaveAttackers");
            }
        }
        END_ENTRY

        UpdateUI(); 

        if ( VarGetNumber("WaitUntilPreviousWaveDefeated") == 1 ) {
            VarSetNumber("EntsRemaining", NumEntitiesInTeam("WaveAttackers"));
            if( NumEntitiesInTeam("WaveAttackers") == 0 )
            {
                SET_STATE( "LullState" );
            }
            else
                UpdateAmbushers(VarGetNumber("ChasePlayers"));
        } else {
            if( VarGetNumber( "TimeToProtect" )!=0 || VarGetNumber( "WaveNumber" ) < VarGetNumber( "NumberOfWaves" ) ) //Once we've finished spawning waves, never lull
            {
                SET_STATE( "LullState" );
            }
            else
                UpdateAmbushers(VarGetNumber("ChasePlayers"));
        }

        checkForSuccessOrFailure();
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    STATE( "LullState" ) ////////////////////////////////////////////////////////
    { 

        ON_ENTRY
        TimerSet( "Lull", (float)VarGetFraction( "LullTime" ) );
        END_ENTRY

        UpdateAmbushers(VarGetNumber("ChasePlayers"));
        UpdateUI();

        if( TimerElapsed( "Lull" ) )
        {
            SET_STATE( "WaveAttacking" );
        }

        checkForSuccessOrFailure();
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    STATE( "EndWaveAtacks" ) ////////////////////////////////////////////////////////
    {
        //Waves are over, if the encounter is also cleared, you are done
        //TO DO does encounter still handle good guy failure
        if (StringEqual(VarGet( "ObjectiveActorName" ), "All")) {
            if( NumEntitiesInTeam( MyEncounter()) < 1 )
            {
                // everything has been destroyed
                WaveAttackShutdownScript(false);
            } else {
                WaveAttackShutdownScript(true);
            }
        } else if (!StringEqual(VarGet( "ObjectiveActorName" ), "")) {
            if( 0.0 >= GetHealth( VarGet( "ObjectiveEntity" ) ) )
            {
                WaveAttackShutdownScript(false);
            }
            else if( NumEntitiesInTeam( MyEncounter()) == 1 ) //only objective is left
            {
                WaveAttackShutdownScript(true);
            }
        } else {
            WaveAttackShutdownScript(true);
        }
    }

    END_STATES 
}


void WaveAttacksEncounterInit()
{
    SetScriptName( "WaveAttackEncounter" );
    SetScriptFunc( WaveAttackEncounter );
    SetScriptType( SCRIPT_ENCOUNTER );        

    SetScriptVar( "ObjectiveActorName",                "",                    0);
    SetScriptVar( "ObjectiveGotoOverride",            "",                    0);

    SetScriptVar( "NumberOfWaves",                    "3",                0);
    SetScriptVar( "TimeToProtect",                    "",                    0 );
    SetScriptVar( "ChasePlayers",                    "0",                0 );
    SetScriptVar( "InnerDoughnut",                    "40",                0 );
    SetScriptVar( "OuterDoughnut",                    "300",                0 );
    SetScriptVar( "FarSpawnLayout",                    "Mission",            0 );
    SetScriptVar( "LullTime",                        "0.1",                0 );
    
    SetScriptVar( "SpawnDefWave1",                    "",                    0  );
    SetScriptVar( "SpawnDefWave2",                    "",                    0  );
    SetScriptVar( "SpawnDefWave3",                    "",                    0  );
    SetScriptVar( "SpawnDefWave4",                    "",                    0  );
    SetScriptVar( "SpawnDefWave5",                    "",                    0  );
    SetScriptVar( "SpawnDefWave6",                    "",                    0  );
    SetScriptVar( "SpawnDefWave7",                    "",                    0  );
    SetScriptVar( "SpawnDefWave8",                    "",                    0  );
    SetScriptVar( "SpawnDefWave9",                    "",                    0  );
    SetScriptVar( "SpawnDefWave10",                    "",                    0  );

    SetScriptVar( "FlyingOk",                        "0",                0  );
    SetScriptVar( "DetachSpawns",                    "1",                0  );
    SetScriptVar( "WaitUntilPreviousWaveDefeated",    "1",                0  );
    SetScriptVar( "GuaranteeWaves",                    "0",                0  );

    SetScriptVar( "TimerName",                        "",                    0  );
    SetScriptVar( "TimerInfo",                        "",                    0  );
    SetScriptVar( "TimerText",                        "",                    0  );
    SetScriptVar( "TimerRadius",                    "100",                0  );
    SetScriptVar( "WaveShout",                0,            SP_MULTIVALUE | SP_OPTIONAL );
}

