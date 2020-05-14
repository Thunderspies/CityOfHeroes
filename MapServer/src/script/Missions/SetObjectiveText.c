// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"
#include <utilitieslib/utils/SuperAssert.h>

// called when the player leaves the map
int SetObjectiveTextOnLeaveMap(ENTITY player)
{
    ClearAllWaypoints(player); // clean up anything that might have been missed
    return 1;
}

// called when the player enters the map (restablish waypoints) 
int SetObjectiveTextOnEnterMap(ENTITY player)
{
    NUMBER waypointIndex;
    NUMBER waypointCount = VarGetArrayCount("WaypointIndices");
        
    if( waypointCount > 0 )
    {
        for (waypointIndex = 0; waypointIndex < waypointCount; waypointIndex++)
        {
            SetWaypoint(ALL_PLAYERS, VarGetArrayElementNumber("WaypointIndices", waypointIndex));
        }
    }
    return 0;
}

void SetObjectiveText(void)
{  
    INITIAL_STATE 
    {
        ON_ENTRY
        {
            // only on mission maps
            if (!OnMissionMap())
                EndScript();
        }
        END_ENTRY

        // If the objective is active or wasn't supplied
        if ( (0 == stricmp("",VarGet("ActivateOnObjectiveComplete"))) || CheckMissionObjectiveExpression(VarGet("ActivateOnObjectiveComplete")))
        {
            STRING objectiveUIText = VarGet("ObjectiveUIText");
            STRING objectiveName = VarGet("ObjectiveName");
            NUMBER waypointIndex;
            NUMBER waypointCount = VarGetArrayCount("Waypoints");

            SET_STATE("Active");
            
            if (stricmp(objectiveUIText, ""))
            {
                CreateMissionObjective(objectiveName, objectiveUIText, objectiveUIText);
            }

            for (waypointIndex = 0; waypointIndex < waypointCount; waypointIndex++)
            {
                NUMBER waypointId = CreateWaypoint(VarGetArrayElement("Waypoints", waypointIndex), VarGetArrayElement("WaypointNames", waypointIndex), NULL, NULL, true, false, -1.0f); // mimics LeadToPlace.c::110
                SetWaypoint(ALL_PLAYERS, waypointId);
                VarSetArrayElementNumber("WaypointIndices", waypointIndex, waypointId);
            }

            OnPlayerExitingMap( SetObjectiveTextOnLeaveMap );
            OnPlayerEnteringMap( SetObjectiveTextOnEnterMap );
        }
    }

    STATE("Active")
    {
        NUMBER waypointIndex;
        NUMBER waypointCount = VarGetArrayCount("WaypointIndices");

        if (CheckMissionObjectiveExpression(VarGet("DeactivateOnObjectiveComplete")))
        {
            SET_STATE("Complete");
        }

        // update waypoints
        if( waypointCount > 0 )
        {
            if( DoEvery( "WaypointUpdateTimer", 0.1f, NULL ) )
            {
                for (waypointIndex = 0; waypointIndex < waypointCount; waypointIndex++)
                {
                    SetWaypoint(ALL_PLAYERS, VarGetArrayElementNumber("WaypointIndices", waypointIndex));
                }
            }
        }
    }

    STATE("Complete")
    {
        STRING objectiveName = VarGet("ObjectiveName");
        NUMBER waypointIndex;
        NUMBER waypointCount = VarGetArrayCount("WaypointIndices");

        SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, objectiveName);

        for (waypointIndex = 0; waypointIndex < waypointCount; waypointIndex++)
        {
            DestroyWaypoint(VarGetArrayElementNumber("WaypointIndices", waypointIndex));
        }

        EndScript();
    }

    END_STATES

    ON_STOPSIGNAL
    {

    }
    END_SIGNAL
}


void SetObjectiveTextInit()
{
    SetScriptName("SetObjectiveText");
    SetScriptFunc(SetObjectiveText);
    SetScriptType(SCRIPT_MISSION);        

    SetScriptVar("ActivateOnObjectiveComplete",        "",            SP_OPTIONAL);
    SetScriptVar("ObjectiveName",                    "",            0);
    SetScriptVar("DeactivateOnObjectiveComplete",    "0",        SP_OPTIONAL);
    SetScriptVar("ObjectiveUIText",                    "",            SP_OPTIONAL);
    SetScriptVar("Waypoints",                        0,            SP_MULTIVALUE | SP_OPTIONAL);
    SetScriptVar("WaypointNames",                    0,            SP_MULTIVALUE | SP_OPTIONAL);
}
