// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

// check objective list
int SafeGuardVandalsCheckObjectiveListAreAllComplete(STRING varlist)
{
    int i, count;

    count = VarGetArrayCount(varlist);

    for (i = 0; i < count; i++)
    {
        if (!ObjectiveIsComplete(VarGetArrayElement(varlist, i)))
            return false;
    }
    return true;
}

// called when the player enters the map
int SafeGuardVandalsOnEnterMap(ENTITY player)
{

    if( !VarIsEmpty( "VandalWayPoint" ) )
    {
        // remove waypoints
        ClearWaypoint(player, VarGetNumber("VandalWayPoint"));

        // add correct waypoints
        SetWaypoint(player, VarGetNumber("VandalWayPoint"));
    }
    return 0;
}


// called when the player leaves the map
int SafeGuardVandalsOnLeaveMap(ENTITY player)
{
    // remove waypoints
    if( !VarIsEmpty( "VandalWayPoint" ) )
    {
        ClearWaypoint(player, VarGetNumber("VandalWayPoint"));
    }
    return 0;
}

void SafeGuardVandals()
{
    //TO DO lame to use a static, but the variables are such a pain.
    //On reflection, I think we should have a way to attach random hunks of data to a script.  A script struct.
    static int objectives[100];

    INITIAL_STATE   
     {    
        ON_ENTRY  
        {
            int i;

            //Set the timer
            F32 timeToVandals = (float)VarGetFraction( "TimeToFirstWave" );

            TimerSet( "TimeToVandals", timeToVandals );
            VarSetNumber( "FirstWave", 0 );

            //Randomize Objectives by doing a bunch of swaps
            {
                int count = VarGetArrayCount("ObjectivesThatLaunchWavesList");
                for( i = 0 ; i < count ; i++ )
                    objectives[i] = i;
                for( i = 0 ; i < count ; i++ )
                {
                    int a = rand()%count;
                    int b = rand()%count;
                    int t = objectives[ a ];
                    objectives[ a ] = objectives[ b ];
                    objectives[ b ] = t;
                }
                
            }
            VarSetNumber( "ObjectiveWaveNumber", 0 );

            OnPlayerExitingMap( SafeGuardVandalsOnLeaveMap );
            OnPlayerEnteringMap( SafeGuardVandalsOnEnterMap );

        }
        END_ENTRY

        if( SafeGuardVandalsCheckObjectiveListAreAllComplete("TriggerObjectiveList"))
        {
            SET_STATE("VandalsAttacking");
        }

    }
    ////////////////////////////////////////////////////////////////////
    STATE("VandalsAttacking")
    {

        //While there are still un smashed objectives, keep sending waves after them
        if( TimerElapsed( "TimeToVandals" ) )
        {
            int objNum;

            if (VarGetNumber("FirstWave") == 0) 
            {
                if( VarIsEmpty( "VandalWayPoint" ) )
                {
                    NUMBER waypoint = CreateWaypoint( VarGet("FirstWaveWaypointLocation"), 
                                                        VarGet("WaveAttackWaypointTooltip"), "map_enticon_mission_a", 
                                                        "map_enticon_mission_b", false, true, -1.0f);
                    SetWaypoint(ALL_PLAYERS, waypoint);
                    VarSetNumber( "VandalWayPoint", waypoint );
                }

                BroadcastAttentionMessage( VarGet("FloatMessageWhenVandalWaveLaunches"), 0, 0 );
                SetMissionObjectiveComplete( OBJECTIVE_SUCCESS, VarGet("FirstWaveObjective") );
                VarSetNumber("FirstWave", 1);

            } else {
                objNum = VarGetNumber( "ObjectiveWaveNumber" );

                if( objNum < VarGetArrayCount("ObjectivesThatLaunchWavesList") )
                {
                    BroadcastAttentionMessage( VarGet("FloatMessageWhenVandalWaveLaunches"), 0, 0 );

                    SetMissionObjectiveComplete( OBJECTIVE_SUCCESS, VarGetArrayElement("ObjectivesThatLaunchWavesList", objectives[ objNum ] ) );

                    //Set the Waypoint
                    UpdateWaypoint( VarGetNumber("VandalWayPoint"), VarGetArrayElement("WaveAttackWaypointLocation", objectives[ objNum ] ), 0, 0, 0);

                    //Increment the Objective to head for next
                    VarSetNumber( "ObjectiveWaveNumber", objNum+1 );
                }
            }

            //Reset the timer
            {
                F32 timeToVandals = (float)VarGetFraction( "BaseTimeBetweenVandalWaves" );
                F32 variance = (float)VarGetFraction( "TimePlusOrMinusBetweenVandalWaves" );

                timeToVandals += RandomNumber( -1*variance, variance );

                TimerSet( "TimeToVandals", timeToVandals );
            }
        }
    }

    END_STATES
}


void SafeGuardVandalsInit()
{
    SetScriptName( "SafeGuardVandals" );
    SetScriptFunc( SafeGuardVandals );
    SetScriptType( SCRIPT_MISSION );        
    
    SetScriptVar( "TriggerObjectiveList",                    "",            0 );
    SetScriptVar( "BaseTimeBetweenVandalWaves",                "",            0 );
    SetScriptVar( "TimePlusOrMinusBetweenVandalWaves",        "",            0 );
    SetScriptVar( "TimeToFirstWave",                        "",            0 );
    SetScriptVar( "FirstWaveObjective",                        "",            0 );
    SetScriptVar( "FirstWaveWaypointLocation",                "",            0 );
    SetScriptVar( "FloatMessageWhenVandalWaveLaunches",        "",            0 );
    SetScriptVar( "ObjectivesThatLaunchWavesList",            "",            0 );
    SetScriptVar( "WaveAttackWaypointLocation",                "",            0 );
    SetScriptVar( "WaveAttackWaypointTooltip",                "",            0 );
}
