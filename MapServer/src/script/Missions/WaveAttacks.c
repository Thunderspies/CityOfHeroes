// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"


void WaveAttacks(void)
{  
    //if( game_state.debugScript )
    //{
    //    ENTITY targetEnt = 0;
    //    NUMBER n;
    //    n = NumEntitiesInTeam(ALL_PLAYERS);
    //    for( i = 0 ; i < n ; i++ )
    //    {
    //        ENITTY ent = GetEntityFromTeam("WaveAttackers", RandomNumber(1, n) );
    //        if( !StringEqual( ent, TEAM_NONE ) )
    //        {
    //            DrawLineToTarget( ent );
    //        }
    //    }
    //}
    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            VarSetNumber( "WaveNumber", 0 );

            //VarSet("WaveTargetLoc", "x"); //TO DO it's a location 
            //VarSet("WaveTarget", "x"); //TO DO it's a team
            //VarSetNumber( "WaveNumber", 0 );
        }
        END_ENTRY

        //If this encounter is active
        if( ObjectiveIsComplete( VarGet( "TriggerObjective" ) ) )
        {
            SET_STATE( "LaunchWaveAttack" );
        }
    }


    //////////////////////////////////////////////////////////////////////////////////////////
    //Try to launch a wave attack; if you can't, try again next tick
    STATE("LaunchWaveAttack") ////////////////////////////////////////////////////////
    {
        ENCOUNTERGROUP group; 
        int waveNumber;

        waveNumber = 1 + VarGetNumber( "WaveNumber" );

        if( waveNumber > VarGetNumber( "NumberOfWaves" ) )
        {
            SET_STATE( "EndWaveAtacks" );
        }
        else //Spawn an attack wave
        {
            //Look for a Random Player //TO DO a random player, or the player who spawned this encounter? or completed this objective?
            ENTITY targetEnt = 0;
            NUMBER n;
            n = NumEntitiesInTeam(ALL_PLAYERS);
            if( n )
                targetEnt = GetEntityFromTeam(ALL_PLAYERS, RandomNumber(1, n) );

            if( targetEnt )
            {
                //Look for an encounter near but not too near that player
                group = FindEncounter( 
                    targetEnt,
                    VarGetNumber( "InnerDoughnut" ), 
                    VarGetNumber( "OuterDoughnut" ),
                    0, 
                    0,
                    VarGet("FarSpawnLayout"),//"AROUND",  "AROUNDVANDALISM", 
                    0,
                    SEF_UNOCCUPIED_ONLY | SEF_SORTED_BY_NEARBY_PLAYERS| SEF_HAS_PATH_TO_LOC
                    ); 

                if( StringEqual( group, LOCATION_NONE ) )
                { 
                    // pull encounters that are active, if we can't find any that are empty
                    group = FindEncounter(
                        targetEnt, 
                        0, 
                        VarGetNumber( "OuterDoughnut" ), 
                        0, 0, VarGet("FarSpawnLayout"),
                        0, 
                        SEF_OCCUPIED_ONLY | SEF_HAS_PATH_TO_LOC);

                    VarSetNumber("TimesIFailedToGetFarSpawn", VarGetNumber( "TimesIFailedToGetFarSpawn" ) + 1 );
                    SetScriptTeam(group, "WaveAttackers");
                }
                else
                {
                    //If you found both, launch this wave attack
                    //printf("WaveAttacks() is spawning...\n");
                    group = CloneEncounter(group);
                    Spawn( 1, "WaveAttackers", GetGenericMissionSpawn(), group, VarGet("FarSpawnLayout"), MissionLevel(), 0 );   //TO DO make a var 
                }

                if( group )
                {
                    SetEncounterInactive(group);
                    AddAIBehavior(group, "Combat");
                    SetAIMoveTarget( group, targetEnt, LOW_PRIORITY, 0, 1, 0.0f );
                    MinionSays( group, VarGet("WaveShout" ) );
                    DetachSpawn( "WaveAttackers" );
                    VarSetNumber( "WaveNumber", waveNumber );
                    SET_STATE( "WaveAttackBearingDownOnTarget" );
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    /// A wave attack has spawned, now monitor it, and when it is destroyed, go into the wait for another wave
    STATE("WaveAttackBearingDownOnTarget")
    {
        if( NumEntitiesInTeam("WaveAttackers") == 0 )
        {
            SET_STATE( "LullState" );
        }
        else
        {
            //TO DO: if you've lost your attack target, look for another player to attack
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    // One wave has been destroyed, and soon another will be spawned
    STATE( "LullState" ) ////////////////////////////////////////////////////////
    { 
        ON_ENTRY
        TimerSet( "Lull", (float)VarGetFraction( "LullTime" ) );
        END_ENTRY

        if( TimerElapsed( "Lull" ) )
        {
            SET_STATE( "LaunchWaveAttack" );
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    STATE( "EndWaveAtacks" ) ////////////////////////////////////////////////////////
    {
        EndScript();
    }

    END_STATES 
}


void WaveAttacksInit()
{
    SetScriptName( "WaveAttacks" );
    SetScriptFunc( WaveAttacks );
    SetScriptType( SCRIPT_MISSION );        

    SetScriptVar( "NumberOfWaves",            "3",        0);
    SetScriptVar( "TriggerObjective",        0,            0 );
    SetScriptVar( "InnerDoughnut",            "100",        0 );
    SetScriptVar( "OuterDoughnut",            "600",        0 );
    SetScriptVar( "FarSpawnLayout",            "Mission",    0 );
    SetScriptVar( "LullTime",                "0.25",        0 );
    SetScriptVar( "WaveShout",                0,            SP_MULTIVALUE | SP_OPTIONAL );
}
