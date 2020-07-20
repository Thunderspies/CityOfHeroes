// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"
#include <utilitieslib/utils/SuperAssert.h>

static bool isTimedObjectiveCurrentlyActive = false; // shared among all running copies of TimedObjective

static char *timedObjectiveName = "TimedObjective12398757329839475"; // unlikely to be used as a real objective

static int s_timedObjective_bonusTime = 0;

void TimedObjectiveAddBonusTime(NUMBER bonusTime)
{
    s_timedObjective_bonusTime += bonusTime;
}

void TimedObjective(void)
{  
    INITIAL_STATE 
    {
        ON_ENTRY
        {
            if (!VarGetNumber("MaxTimer") && !VarGet("DeactivateOnObjectiveComplete"))
            {
                devassert(0 && "Must have either MaxTimer or DeactivateOnObjectiveComplete in a TimedObjective!");
            }

            if (VarGetArrayCount("Intervals") != VarGetArrayCount("ObjectiveCompleteOnIntervals"))
            {
                devassert(0 && "Intervals and ObjectiveCompleteOnIntervals must have the same length in a TimedObjective!");
            }

            if (VarGetArrayCount("Waypoints") != VarGetArrayCount("WaypointNames"))
            {
                devassert(0 && "Waypoints and WaypointNames must have the same length in a TimedObjective!");
            }
        }
        END_ENTRY

        // If this encounter is active
        if (!isTimedObjectiveCurrentlyActive &&
            CheckMissionObjectiveExpression(VarGet("ActivateOnObjectiveComplete")))
        {
            NUMBER maxTimer = VarGetNumber("MaxTimer");
            STRING objectiveUIText = VarGet("ObjectiveUIText");
            STRING timerType = VarGet("TimerType");
            NUMBER waypointIndex;
            NUMBER waypointCount = VarGetArrayCount("Waypoints");

            SET_STATE("Active");
            isTimedObjectiveCurrentlyActive = true;
            
            VarSetNumber("Timeout", SecondsSince2000() + maxTimer);
            s_timedObjective_bonusTime = 0;
            
            if (stricmp(objectiveUIText, ""))
            {
                CreateMissionObjective(timedObjectiveName, objectiveUIText, objectiveUIText);
            }

            if (!stricmp(timerType, "Countdown"))
            {
                SetMissionNonFailingTimer(maxTimer, -1);
            }
            else if (!stricmp(timerType, "Countup"))
            {
                SetMissionNonFailingTimer(maxTimer, 1);
            }

            for (waypointIndex = 0; waypointIndex < waypointCount; waypointIndex++)
            {
                NUMBER waypointId = CreateWaypoint(VarGetArrayElement("Waypoints", waypointIndex), VarGetArrayElement("WaypointNames", waypointIndex), NULL, NULL, true, false, -1.0f); // mimics LeadToPlace.c::110
                VarSetArrayElementNumber("WaypointIndices", waypointIndex, waypointId);
            }
        }
    }

    STATE("Active")
    {
        int timeRemaining = VarGetNumber("Timeout") - SecondsSince2000() + s_timedObjective_bonusTime;
        STRING timerType = VarGet("TimerType");
        int maxTimer = VarGetNumber("MaxTimer");
        bool isCountup = !stricmp(timerType, "Countup");

        if (isCountup && !maxTimer)
        {
            timeRemaining = -timeRemaining;
        }

        if (CheckMissionObjectiveExpression(VarGet("DeactivateOnObjectiveComplete")))
        {
            SET_STATE("Complete");
        }
        else
        {
            int intervalIndex;
            int intervalCount = VarGetArrayCount("Intervals");

            if (maxTimer && timeRemaining < 0)
            {
                SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGet("ObjectiveCompleteOnTimeout"));
                SET_STATE("Complete");                
            }

            for (intervalIndex = 0; intervalIndex < intervalCount; intervalIndex++)
            {
                int interval = VarGetArrayElementNumber("Intervals", intervalIndex);
                if ((isCountup ? (maxTimer ? (maxTimer - timeRemaining) > interval
                                           : timeRemaining > interval) 
                               : timeRemaining < interval)
                    && interval != -1)
                {
                    SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGetArrayElement("ObjectiveCompleteOnIntervals", intervalIndex));
                    VarSetArrayElementNumber("Intervals", intervalIndex, -1);
                }
            }
        }
    }

    STATE("Complete")
    {
        NUMBER waypointIndex;
        NUMBER waypointCount = VarGetArrayCount("Waypoints");
        STRING timerType = VarGet("TimerType");

        isTimedObjectiveCurrentlyActive = false;
        SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, timedObjectiveName);

        if (!stricmp(timerType, "Countdown") || (!stricmp(timerType, "Countup")))
        {
            ClearMissionTimer();
        }

        for (waypointIndex = 0; waypointIndex < waypointCount; waypointIndex++)
        {
            DestroyWaypoint(VarGetArrayElementNumber("WaypointIndices", waypointIndex));
        }

        EndScript();
    }

    END_STATES

    ON_STOPSIGNAL
    {
        isTimedObjectiveCurrentlyActive = false;
    }
    END_SIGNAL
}


void TimedObjectiveInit()
{
    SetScriptName("TimedObjective");
    SetScriptFunc(TimedObjective);
    SetScriptType(SCRIPT_MISSION);        

    SetScriptVar("ActivateOnObjectiveComplete",        "",            0);
    SetScriptVar("DeactivateOnObjectiveComplete",    "0",        SP_OPTIONAL);
    SetScriptVar("MaxTimer",                        "0",        SP_OPTIONAL);
    SetScriptVar("ObjectiveCompleteOnTimeout",        "",            SP_OPTIONAL);
    SetScriptVar("ObjectiveUIText",                    "",            SP_OPTIONAL);
    SetScriptVar("TimerType",                        "None",        SP_OPTIONAL);
    SetScriptVar("Intervals",                        0,            SP_MULTIVALUE | SP_OPTIONAL);
    SetScriptVar("ObjectiveCompleteOnIntervals",    0,            SP_MULTIVALUE | SP_OPTIONAL);
    SetScriptVar("Waypoints",                        0,            SP_MULTIVALUE | SP_OPTIONAL);
    SetScriptVar("WaypointNames",                    0,            SP_MULTIVALUE | SP_OPTIONAL);
}
