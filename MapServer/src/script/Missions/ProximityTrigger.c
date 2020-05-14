// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

//TO DO waypoints?

void ProximityTrigger(void)
{  
    int i;

    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            ScriptControlsCompletion( MyEncounter() );
        }
        END_ENTRY
    }

    // check to see if players are nearby

    // turn off UI
    for (i = 0; i < NumEntitiesInTeam(ALL_PLAYERS); i++)
    {    
        ENTITY targetEnt = GetEntityFromTeam(ALL_PLAYERS, i + 1 );

        if (DistanceBetweenLocations(EncounterPos(MyEncounter(), 1), PointFromEntity(targetEnt)) < VarGetFraction( "TriggerRadius"))
        {
            SetEncounterComplete( MyEncounter(), 1);
            EndScript();
        }
    }

    END_STATES 
}

void ProximityTriggerInit()
{
    SetScriptName( "ProximityTrigger" );
    SetScriptFunc( ProximityTrigger );
    SetScriptType( SCRIPT_ENCOUNTER );        

    SetScriptVar( "TriggerRadius",                "100.0",                    0);
}

