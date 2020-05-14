
#include "script/scriptutil.h"

extern int LeadToPlaceOnLeaveMap(ENTITY player);
extern int LeadToPlaceOnEnterMap(ENTITY player);

// Documentation at: http://code:8081/display/coh/Multi-Lead+To+Place+Encounter+Script

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

void ScriptMultiLeadToPlaceSetUpVars(NUMBER idx)
{
    if (VarGetArrayCount("PlaceToTakeRescueeList") > 0)
    {
        STRING element = VarGetArrayElement("PlaceToTakeRescueeList", idx);
        if (!StringEmpty(element) && !StringEqual(element, "none"))
            VarSet( "PlaceToTakeRescuee", element);
    }
    
    if (VarGetArrayCount("DeliveryObjectiveList") > 0)
    {
        STRING element = VarGetArrayElement("DeliveryObjectiveList", idx);
        if (!StringEmpty(element) && !StringEqual(element, "none"))
            VarSet( "DeliveryObjective", element);
    }

    if (VarGetArrayCount("PL_OnArrivalList") > 0)
    {
        STRING element = VarGetArrayElement("PL_OnArrivalList", idx);
        if (!StringEmpty(element) && !StringEqual(element, "none"))
            VarSet( "PL_OnArrival", element);
    }

    if (VarGetArrayCount("SayOnArrivalList") > 0)
    {
        STRING element = VarGetArrayElement("SayOnArrivalList", idx);
        if (!StringEmpty(element) && !StringEqual(element, "none"))
            VarSet( "SayOnArrival", element);
    }

    if (VarGetArrayCount("WayPointTextList") > 0)
    {
        STRING element = VarGetArrayElement("WayPointTextList", idx);
        if (!StringEmpty(element) && !StringEqual(element, "none"))
            VarSet( "WayPointText", element);
    }

    if (VarGetArrayCount("LeadToObjectiveTextList") > 0)
    {
        STRING element = VarGetArrayElement("LeadToObjectiveTextList", idx);
        if (!StringEmpty(element) && !StringEqual(element, "none"))
            VarSet( "LeadToObjectiveText", element);
    }

    if (VarGetArrayCount("LeadToObjectiveTextSingularList") > 0)
    {
        STRING element = VarGetArrayElement("LeadToObjectiveTextSingularList", idx);
        if (!StringEmpty(element) && !StringEqual(element, "none"))
            VarSet( "LeadToObjectiveTextSingular", element);
    }
}

int ScriptMultiLeadToPlaceCheckForSkip()
{
    int i;
    int currentStep = VarGetNumber("CurrentStep");
    int skipToStep = -1;

    if (VarGetArrayCount("PlaceToTakeRescueeList") - 1 > currentStep) {
        for(i = currentStep; i < VarGetArrayCount("PlaceToTakeRescueeList"); i++) {
            STRING element = VarGetArrayElement("SkipToOnObjectiveCompleteList", i);
            if (!StringEmpty(element) && !StringEqual(element, "none")) {
                if(CheckMissionObjectiveExpression(element)) skipToStep = i;
            }
        }
    }

    return skipToStep;
}

void ScriptMultiLeadToPlace(void)
{  
    int currentStep;
    int skipToStep;

    INITIAL_STATE 
    {
        ON_ENTRY
        {
            //load up initial LeadToPlace vars
            ScriptMultiLeadToPlaceSetUpVars( 0 );
            VarSetNumber("CurrentStep", 0);

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

        skipToStep = ScriptMultiLeadToPlaceCheckForSkip();
        if(skipToStep > VarGetNumber("CurrentStep"))
        {
            ScriptMultiLeadToPlaceSetUpVars(skipToStep);
            VarSetNumber( "CurrentStep", skipToStep);
        }

        //If I'm alive and everybody else is dead, make with the delivery part of the script
        else if( PlayersNearby() && NumRescueValidEntitiesInTeam( MyEncounter(), VarGet( "Rescuee" ) ) == 0 && GetHealth( VarGet( "Rescuee" ) ) > 0.0 ) 
        {
            AddAIBehaviorPermFlag(VarGet("Rescuee"), "DoNotDrawAggro(0)");
            SetChatObjective( VarGet("Rescuee"), VarGet( "PlaceToTakeRescuee" ) ); //if the place is an entity (door, objective or person, set it )
            SET_STATE("RescuedState");
            SetShowOnMap( VarGet("Rescuee"), true );

            //Set the RescueObjective if there is one
            if (!StringEqual( VarGet( "RescueObjective" ), "None" ) && !StringEqual( VarGet( "RescueObjective" ), "" ) )
            {
                SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGet( "RescueObjective" ));
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

        if( VarGet( "PlaceToTakeRescuee" ) && !StringEqual( VarGet( "PlaceToTakeRescuee" ), LOCATION_NONE ) && !StringEqual( VarGet( "PlaceToTakeRescuee" ), "none" ) )
        {
            if( StringEqual( VarGet( "PlaceToTakeRescuee" ), "CurrentLocation" ) || EntityInArea( VarGet( "Rescuee" ), VarGet( "PlaceToTakeRescuee" ) ) )
                SET_STATE( "RescueAdvance" );
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

        skipToStep = ScriptMultiLeadToPlaceCheckForSkip();
        if(skipToStep > VarGetNumber("CurrentStep"))
        {
            DestroyWaypoint( VarGetNumber("WayPoint") );
            ScriptMultiLeadToPlaceSetUpVars(skipToStep);
            VarSetNumber( "CurrentStep", skipToStep);
            SET_STATE( "RescuedState" );
        }
    }

    STATE( "RescueAdvance" )
    {
        //Kill this step's waypoint
        DestroyWaypoint( VarGetNumber("WayPoint") );

        //Set this step's objective to succeeded
        if (!StringEqual( VarGet( "DeliveryObjective" ), "None" ) && !StringEqual( VarGet( "DeliveryObjective" ), "" ) )
        {
            SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGet( "DeliveryObjective" ));
        }

        //Do arrival behaviors/dialog
        SetChatObjective( VarGet( "Rescuee" ), VarGet( "PlaceToTakeRescuee" ) ); //if the place is an entity (door, objective or person, set it )
        SetAIPriorityList( VarGet( "Rescuee" ), VarGet( "PL_OnArrival" ) );
        if( GetHealth( VarGet( "Rescuee" ) ) > 0.0 )
            Say( VarGet( "Rescuee" ), VarGet( "SayOnArrival" ), 2 );

        if(VarGetArrayCount("PlaceToTakeRescueeList") - 1 > VarGetNumber( "CurrentStep" ))
        {
            //set up the next stage and go again
            currentStep = VarGetNumber( "CurrentStep" ) + 1;
            ScriptMultiLeadToPlaceSetUpVars(currentStep);
            VarSetNumber( "CurrentStep", currentStep);
            SET_STATE( "RescuedState" );
        }
        else
        {
            //no more steps, complete this script
            SET_STATE( "RescueComplete" );
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    STATE( "RescueComplete" ) ////////////////////////////////////////////////////////
    {
        SetFollowers(TEAM_NONE, VarGet( "Rescuee" ) ); //Don't be anyone's follower anymore
        SetShowOnMap( VarGet("Rescuee"), false );

        SetEncounterComplete( MyEncounter(), 1 );
        if( !VarIsTrue( "Betrayed") && ObjectiveIsComplete( VarGet( "BetrayalObjectiveComplete") ) ) 
        {
            VarSet("Betrayed", "true");
            SET_STATE( "RescueComplete" );
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

void MultiLeadToPlaceInit()
{
    SetScriptName( "MultiLeadToPlace" );
    SetScriptFunc( ScriptMultiLeadToPlace );
    SetScriptType( SCRIPT_ENCOUNTER );        

    //singular vars
    SetScriptVar( "PositionOfRescuee",                0,                    0 );
    SetScriptVar( "RescueObjective",                0,                    SP_OPTIONAL );
    SetScriptVar( "EndScriptObjective",                0,                    SP_OPTIONAL );

    SetScriptVar( "PL_OnRescue",                    0,                    0 ); 
    SetScriptVar( "SayOnRescue",                    0,                    SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "PL_OnRecapture",                    0,                    0 ); 
    SetScriptVar( "SayOnRecapture",                    0,                    SP_MULTIVALUE | SP_OPTIONAL ); 
    SetScriptVar( "PL_OnReRescue",                    0,                    0 ); 
    SetScriptVar( "SayOnReRescue",                    0,                    SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "PL_OnStranded",                    0,                    0 );
    SetScriptVar( "SayOnStranded",                    0,                    SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "SayOnClickAfterRecapture",        0,                    SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "SayOnClickAfterRescue",            0,                    SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "ControlRadius",                    "200",                0 ); //radius leader must be in the follow 
    SetScriptVar( "LookForLeaderInRadius",            "30",                0 ); //radius to look for a new leader
    SetScriptVar( "RescueObjectiveText",            NULL,                SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "RescueObjectiveTextSingular",    NULL,                SP_MULTIVALUE | SP_OPTIONAL );

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

    //array vars
    SetScriptVar( "PlaceToTakeRescueeList",            0,                    0 );
    SetScriptVar( "PL_OnArrivalList",                0,                    0 ); 
    SetScriptVar( "SayOnArrivalList",                0,                    SP_OPTIONAL );
    SetScriptVar( "DeliveryObjectiveList",            0,                    SP_OPTIONAL );
    SetScriptVar( "WayPointTextList",                0,                    SP_OPTIONAL );
    SetScriptVar( "LeadToObjectiveTextList",        NULL,                SP_OPTIONAL );
    SetScriptVar( "LeadToObjectiveTextSingularList",NULL,                SP_OPTIONAL );
    SetScriptVar( "SkipToOnObjectiveCompleteList",    NULL,                SP_OPTIONAL );

    SetScriptVar( "NoRecapture",                    0,                    SP_OPTIONAL );
    SetScriptVar( "SeeStealth",                        0,                    SP_OPTIONAL );

    SetScriptSignal( "End Event", "EndScript" );
}

