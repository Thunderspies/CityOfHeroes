// MISSION SCRIPT
// This script will set a mission objective complete when a player enters a map and meets the requirement
//
#include "script/scriptutil.h"


// called when the player enters the map
int OnEnterObjectiveCompleteTest(ENTITY player)
{
    int count, i;

    count = VarGetArrayCount("RequiresList");
    for (i = 0; i < count; i++)
    {
        if (EvalEntityRequires(player, VarGetArrayElement("RequiresList", i), Script_GetCurrentBlame()))
        {    
            SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGetArrayElement("ObjectiveList", i));
        }
    }
    return 0;
}

void OnEnterObjectiveComplete(void)
{  
    if (!OnMissionMap())
    {
        return;
    }

    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            OnPlayerEnteringMap( OnEnterObjectiveCompleteTest );
        }
        END_ENTRY

    }

    END_STATES 
}


void OnEnterObjectiveCompleteInit()
{
    SetScriptName( "OnEnterObjectiveComplete" );
    SetScriptFunc( OnEnterObjectiveComplete );
    SetScriptType( SCRIPT_MISSION );        
    
    SetScriptVar( "RequiresList",                                "",            0 );
    SetScriptVar( "ObjectiveList",                                "",            0 );

}
