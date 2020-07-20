// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

// called when the player leaves the map
int LeadToPlaceOnLeaveMap(ENTITY player)
{
    ClearAllWaypoints(player); // clean up anything that might have been missed
    return 1;
}

// called when the player enters the map (restablish waypoints) 
int LeadToPlaceOnEnterMap(ENTITY player)
{
    if( !VarIsEmpty( "WayPoint" ) )
        SetWaypoint(player, VarGetNumber("WayPoint"));
    return 0;
}

static int PlayersNearby()
{
    int count = 0;
    int i;
    STRING rescuee = VarGet("Rescuee");
    FRACTION radius = VarGetFraction("LookForLeaderInRadius");
    LOCATION loc = PointFromEntity(rescuee);
    for (i = 0; i < NumEntitiesInTeam(ALL_PLAYERS); i++)
    {    
        ENTITY targetEnt = GetEntityFromTeam(ALL_PLAYERS, i + 1 );
        if (DistanceBetweenLocations(loc, PointFromEntity(targetEnt)) <= radius)
        {
            count++;
        }
    }
    return count;
}

void LeadToPlace(void)
{  
    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            if (StringEqual( VarGet( "PlaceToTakeRescuee" ), "none" ))
                VarSet("PlaceToTakeRescuee" , LOCATION_NONE);

            VarSet( "Rescuee", ActorFromEncounterPos(MyEncounter(), VarGetNumber( "PositionOfRescuee" ) ) );
            //TO DO IF No rescuee here, then abort the script, succeed and throw an error
            if( !EntityExists( VarGet( "Rescuee" ) ) )
            {
                SET_STATE("END_SCRIPT");
            }
            else
            {
                if (!VarIsEmpty("AllyID"))
                    SetAIAlly(VarGet("Rescuee"), VarGetNumber("AllyID"));
                else
                    SetAITeamToMatchMissionPlayers( VarGet("Rescuee") );
                if (!VarIsEmpty("GangID"))
                    SetAIGang(VarGet("Rescuee"), VarGetNumber("GangID"));

                AddAIBehaviorPermFlag(VarGet("Rescuee"), "DoNotDrawAggro");
                AddAIBehaviorPermFlag(VarGet("Rescuee"), "FindTarget(0)");
                ScriptControlsCompletion( MyEncounter() );
                SET_STATE("WaitingToBeRescued");
            }

            //Set and remove the waypoints
            OnPlayerExitingMap( LeadToPlaceOnLeaveMap );
            OnPlayerEnteringMap( LeadToPlaceOnEnterMap );
        }
        END_ENTRY
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    STATE("WaitingToBeRescued") ////////////////////////////////////////////////////////
    {

        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            SetObjectiveString( VarGet("RescueObjectiveText"), VarGet("RescueObjectiveTextSingular"));
        } 
        END_ENTRY

        //I die, we're done
        if( !EntityExists( VarGet( "Rescuee" ) ) )
        {
            SetEncounterComplete(MyEncounter(), 0);
            SET_STATE( "EndScript" );
        }

        //If I'm alive and everybody else is dead, make with the delivery part of the script
        else if( PlayersNearby() && NumRescueValidEntitiesInTeam( MyEncounter(), VarGet( "Rescuee" ) ) == 0 && GetHealth( VarGet( "Rescuee" ) ) > 0.0 ) 
        {
//            SetMissionStatus(VarGet("StatusString"));
            AddAIBehaviorPermFlag(VarGet("Rescuee"), "DoNotDrawAggro(0)");
            SetChatObjective( VarGet("Rescuee"), VarGet( "PlaceToTakeRescuee" ) ); //if the place is an entity (door, objective or person, set it )
            SET_STATE("RescuedState");
            SetShowOnMap( VarGet("Rescuee"), true );

            //Complete the Encounter if you only have to Rescue and Not Deliver
            if (!StringEqual( VarGet( "RescueObjective" ), "None" ) && !StringEqual( VarGet( "RescueObjective" ), "" ) )
            {
                SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGet( "RescueObjective" ));
            }
            if( StringEqual( VarGet( "SucceedOnRescueOrDelivery" ), "Rescue" ) )
            {
                SetEncounterComplete(MyEncounter(), 1);
                VarSetNumber( "EncounterAlreadyCompleted", 1 );
            }
        }

    }

    ////////////////////////////////////////////////////////////////////////////////////////
    STATE("RescuedState") ////////////////////////////////////////////////////////
    {

        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            // Set Waypoint
            if( !StringEqual( VarGet( "PlaceToTakeRescuee" ), "CurrentLocation" ) && LocationExists( VarGet( "PlaceToTakeRescuee" ) ))
            {
                NUMBER waypoint = CreateWaypoint(VarGet( "PlaceToTakeRescuee" ), VarGet("WayPointText"), NULL, NULL, true, false, -1.0f);
                SetWaypoint(ALL_PLAYERS, waypoint);
                VarSetNumber( "WayPoint", waypoint );
            }

            SetEncounterActive(MyEncounter());
            AddAIBehaviorPermFlag(VarGet("Rescuee"), "FindTarget(1)");

            // reset the IDs
            if (!VarIsEmpty("AllyID"))
                SetAIAlly(VarGet("Rescuee"), VarGetNumber("AllyID"));
            if (!VarIsEmpty("GangID"))
                SetAIGang(VarGet("Rescuee"), VarGetNumber("GangID"));
        }
        END_ENTRY

        //Rescuee can be revived, so wait till he's gone to fail the objective
        if( !EntityExists( VarGet( "Rescuee" ) ) )
        {
            //If the delivery is what succeeds the encounter
            if( !VarGetNumber( "EncounterAlreadyCompleted" ) );
                SetEncounterComplete(MyEncounter(), 0);
            SET_STATE( "EndScript" );
        }

        // update waypoints
        if( !VarIsEmpty("WayPoint") )
        {
            if( DoEvery( "WaypointUpdateTimer", 0.1f, NULL ) )
            {
                SetWaypoint(ALL_PLAYERS, VarGetNumber("WayPoint"));
            }
        }

        //Look to see if you are at your final location (if the variable doesn't exist, instantly succeed
        //(added Door with name to point in area so Mission Return Works automatically and location:any)

        //TO DO : If No Rescue is needed, succeed the encounter, but don't clean it up
        if( VarGet( "PlaceToTakeRescuee" ) && !StringEqual( VarGet( "PlaceToTakeRescuee" ), LOCATION_NONE ) && !StringEqual( VarGet( "PlaceToTakeRescuee" ), "none" ) )
        {
            if( StringEqual( VarGet( "PlaceToTakeRescuee" ), "CurrentLocation" ) || EntityInArea( VarGet( "Rescuee" ), VarGet( "PlaceToTakeRescuee" ) ) )
                SET_STATE( "RescueComplete" );
        }

        //Launch Automatic wave attacks from time to time
        if( VarGetFraction( "AutoWaveAttackCount" ) && (VarGetNumber("AutoWaveAttacksDone" ) < VarGetNumber("AutoWaveAttackCount" ) ) )
        {
            if( DoEvery( "AutoWaveAttackTimer", VarGetFraction( "MinutesBetweenAutoWaveAttacks" ), NULL ) )
            {
                ENCOUNTERGROUP group;
                group = LaunchGenericWaveAttack( GetOwner( VarGet("Rescuee") ), NULL, NULL, VarGet("WaveShout") );

                if( !StringEqual( group, LOCATION_NONE ) )
                    VarSetNumber( "AutoWaveAttacksDone", VarGetNumber("AutoWaveAttacksDone" )+1 );
            }
        }

        if( !VarIsTrue( "Betrayed") &&  ObjectiveIsComplete( VarGet( "BetrayalObjectiveComplete") ) ) 
        {
            VarSet("Betrayed", "true");
            SET_STATE( "RescueComplete" );
        }
        //Otherwise follow your regular rules

        FollowerFollows( VarGet("Rescuee"), VarGetFraction( "ControlRadius" ), VarGetNumber( "NoRecapture" ), VarGetNumber( "SeeStealth" ) );

        SetObjectiveString( VarGet("LeadToObjectiveText"), VarGet("LeadToObjectiveTextSingular"));

    }


    ////////////////////////////////////////////////////////////////////////////////////////
    STATE( "RescueComplete" ) ////////////////////////////////////////////////////////
    {
        SetFollowers(TEAM_NONE, VarGet( "Rescuee" ) ); //Don't be anyone's follower anymore 
        SetShowOnMap( VarGet("Rescuee"), false );
        
//        RemoveAllBehaviors( VarGet( "Rescuee" ), true );

        //Another chance to aquire your objective
        SetChatObjective( VarGet( "Rescuee" ), VarGet( "PlaceToTakeRescuee" ) ); //if the place is an entity (door, objective or person, set it )
        SetAIPriorityList( VarGet( "Rescuee" ), VarGet( "PL_OnArrival" ) );
        if( GetHealth( VarGet( "Rescuee" ) ) > 0.0 )
            Say( VarGet( "Rescuee" ), VarGet( "SayOnArrival" ), 2 );

        //    If the delivery is what succeeds the encounter
        //    This happens regardless 
        if( !VarGetNumber( "EncounterAlreadyCompleted" ) )
        {
            SetEncounterComplete( MyEncounter(), 1 );
            if( !VarIsTrue( "Betrayed") &&  ObjectiveIsComplete( VarGet( "BetrayalObjectiveComplete") ) ) 
            {
                VarSet("Betrayed", "true");
                SET_STATE( "RescueComplete" );
            }
        }

        if (VarIsTrue("Betrayed"))
        {
            STRING betrayalBehavior = VarGet("PL_OnBetrayal");
            Say( VarGet( "Rescuee" ), VarGet( "SayOnBetrayal" ), 2 );
            SetAIGangPerm(VarGet("Rescuee"), VarGetNumber( "BetrayalGangId" ) );        
            SetAIAlly(VarGet("Rescuee"), kAllyID_None);
            SetAIPriorityList( VarGet( "Rescuee" ), betrayalBehavior ? betrayalBehavior : "PL_COMBAT");
        }


        if ( StringEqual("", VarGet("DelayTime"))) {
            SET_STATE( "EndScript" );
        } else {
            SET_STATE( "Delay" );
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    STATE( "Delay" ) ////////////////////////////////////////////////////////
    {

        if (DoEvery("DelayTimer", VarGetFraction("DelayTime"), NULL))
        {
            SET_STATE( "EndScript" );
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    STATE( "EndScript" ) ////////////////////////////////////////////////////////
    {

        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            if (!VarIsEmpty( "EndScriptObjective" ))
            {
                SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGet( "EndScriptObjective" ));
            }
        }
        END_ENTRY

        DestroyWaypoint( VarGetNumber("WayPoint") );
        if (NumEntitiesInTeam( MyEncounter() ) < 1)
            CloseMyEncounterAndEndScript();
    }


    END_STATES
}



void LeadToPlaceInit()
{
    SetScriptName( "LeadToPlace" );
    SetScriptFunc( LeadToPlace );
    SetScriptType( SCRIPT_ENCOUNTER );        

    SetScriptVar( "PositionOfRescuee",                0,                    0 );
    SetScriptVar( "RescueObjective",                0,                    SP_OPTIONAL );
    SetScriptVar( "EndScriptObjective",                0,                    SP_OPTIONAL );
    SetScriptVar( "PlaceToTakeRescuee",                0,                    0 ); 
    SetScriptVar( "SucceedOnRescueOrDelivery",        "Delivery",            0 );  //"Rescue" is the Other Option
    SetScriptVar( "PL_OnRescue",                    0,                    0 ); 
    SetScriptVar( "SayOnRescue",                    0,                    SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "PL_OnArrival",                    0,                    0 ); 
    SetScriptVar( "SayOnArrival",                    0,                    SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "PL_OnRecapture",                    0,                    0 ); 
    SetScriptVar( "SayOnRecapture",                    0,                    SP_MULTIVALUE | SP_OPTIONAL ); 
    SetScriptVar( "PL_OnReRescue",                    0,                    0 ); 
    SetScriptVar( "SayOnReRescue",                    0,                    SP_MULTIVALUE | SP_OPTIONAL ); 
    SetScriptVar( "WayPointText",                    0,                    SP_OPTIONAL); 
    SetScriptVar( "PL_OnStranded",                    0,                    0 );
    SetScriptVar( "SayOnStranded",                    0,                    SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "SayOnClickAfterRecapture",        0,                    SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "SayOnClickAfterRescue",            0,                    SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "ControlRadius",                    "200",                0 ); //radius leader must be in the follow 
    SetScriptVar( "LookForLeaderInRadius",            "30",                0 ); //radius to look for a new leader
    SetScriptVar( "StatusString",                    0,                    SP_MULTIVALUE | SP_OPTIONAL ); //Go in the Nav window under the active task string
    SetScriptVar( "RescueObjectiveText",            NULL,                SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "RescueObjectiveTextSingular",    NULL,                SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "LeadToObjectiveText",            NULL,                SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "LeadToObjectiveTextSingular",    NULL,                SP_MULTIVALUE | SP_OPTIONAL );

    SetScriptVar( "BetrayalObjectiveComplete",        NULL,                SP_OPTIONAL );
    SetScriptVar( "SayOnBetrayal",                    NULL,                SP_OPTIONAL );
    SetScriptVar( "BetrayalGangId",                    0,                    SP_OPTIONAL );
    SetScriptVar( "PL_OnBetrayal",                    0,                    SP_OPTIONAL );

    SetScriptVar( "DelayTime",                        0,                    SP_OPTIONAL); 
    SetScriptVar( "MinutesBetweenAutoWaveAttacks",    "0.5",                0 );
    SetScriptVar( "AutoWaveAttackCount",            "1",                0 );
    SetScriptVar( "WaveShout",                        0,                    SP_OPTIONAL );
    SetScriptVar( "GangID",                            0,                    SP_OPTIONAL ); 
    SetScriptVar( "AllyID",                            0,                    SP_OPTIONAL );

    SetScriptVar( "NoRecapture",                    0,                    SP_OPTIONAL );
    SetScriptVar( "SeeStealth",                        0,                    SP_OPTIONAL ); 
}
