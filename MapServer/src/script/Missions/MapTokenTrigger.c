// MISSION SCRIPT
// This script will set a map token when the mission is completed
//
#include "script/scriptutil.h"


void MapTokenTrigger(void)
{  
    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
        }
        END_ENTRY

        if( MissionIsComplete() )
        {
            SET_STATE( "MissionOver" );
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    STATE( "MissionOver" ) ////////////////////////////////////////////////////////
    {
        MapSetDataToken(VarGet("TokenName"), VarGetNumber("TokenValue"));

        EndScript();
    }

    END_STATES 
}


void MapTokenTriggerInit()
{
    SetScriptName( "MapTokenTrigger" );
    SetScriptFunc( MapTokenTrigger );
    SetScriptType( SCRIPT_MISSION );        
    
    SetScriptVar( "TokenName",                                "",            0 );
    SetScriptVar( "TokenValue",                                "",            0 );

}
