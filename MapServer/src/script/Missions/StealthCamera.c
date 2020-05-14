// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

void StealthCamera(void)
{  
    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            TimerSet("Wait", 0.02); //Nasty
        }
        END_ENTRY

        if ( TimerElapsed("Wait") )
        {
            STRING buf = "CameraScan( ";

            if( !VarIsEmpty("ScanPattern") ) //Unused
                buf = StringAdd( buf, VarGet( "ScanPattern" ) ); //Unused
            else
                buf = StringAdd( buf, "UpAndDown" ); //Unused

            buf = StringAdd( buf, " )" );

            AddAIBehavior( MyEncounter(), buf ); //TO DO Mayhem (Or other anim)

            SET_STATE( "SCANNING" )
        }
    }

    STATE( "SCANNING" )
    {
        if( EncounterState( MyEncounter() ) == ENCOUNTER_ACTIVE ||
            EncounterState( MyEncounter() ) == ENCOUNTER_COMPLETED )
        {
            if( !VarIsEmpty("ObjectiveSetFailedByAlert")  )
                SetMissionObjectiveComplete( OBJECTIVE_SUCCESS,  VarGet("ObjectiveSetFailedByAlert") ); //TO DO switch back when Vince gets fail in
                //SetMissionObjectiveComplete( OBJECTIVE_FAILED,  VarGet("ObjectiveSetFailedByAlert") );

            if( !VarIsEmpty("FloatTextByAlert")  )
                SendPlayerAttentionMessage( ALL_PLAYERS, VarGet("FloatTextByAlert" ) ); 

            if( !VarIsEmpty("EncounterSpawnedByAlert")  )
            {
                ENCOUNTERGROUP group;
                //TO DO: find right layout by looking in the group
                group = FindEncounter( MyEncounter(), 0, 100, 0, 0, "StealthPopups", 0, SEF_UNOCCUPIED_ONLY | SEF_SORTED_BY_DISTANCE );
                if( !StringEqual( group, LOCATION_NONE ) )
                {
                    Spawn( 1, "StealthPopups", VarGet("EncounterSpawnedByAlert"), group, "StealthPopups", 0, 0 );
                    SetEncounterActive( group );
                }
            }
                    
            EndScript();
        }
    }
    END_STATES 
}

void StealthCameraInit()
{
    SetScriptName( "StealthCamera" );
    SetScriptFunc( StealthCamera );
    SetScriptType( SCRIPT_ENCOUNTER );        

    SetScriptVar( "ObjectiveSetFailedByAlert",    "",        SP_OPTIONAL );
    SetScriptVar( "FloatTextByAlert",            "",        SP_OPTIONAL );
    SetScriptVar( "EncounterSpawnedByAlert",    "",        SP_OPTIONAL );
    SetScriptVar( "ScanPattern",                "",        SP_OPTIONAL ); //Unused

    SetScriptSignal( "End Event", "EndScript" );
}
