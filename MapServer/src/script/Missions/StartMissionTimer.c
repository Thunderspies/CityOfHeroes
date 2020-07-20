// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"


void StartMissionTimer(void)
{  
    int i, count;
    int complete = true;

    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
        }
        END_ENTRY

        // check for objectives completed
        count = VarGetArrayCount("TriggerObjectiveList");
        for (i = 0; i < count; i++)
        {
            if (!ObjectiveIsComplete(VarGetArrayElement("TriggerObjectiveList", i)))
            {    
                complete = false;
                break;
            }
        }
        if (complete) 
        {
            SetMissionFailingTimer(VarGetNumber("Time") * 60);
            EndScript();
        }

    }

    END_STATES 

    ON_SIGNAL("Trigger")
        SetMissionFailingTimer(VarGetNumber("Time") * 60); 
        EndScript();
    END_SIGNAL

}


void StartMissionTimerInit()
{
    SetScriptName( "StartMissionTimer" );
    SetScriptFunc( StartMissionTimer );
    SetScriptType( SCRIPT_MISSION );        
    
    SetScriptVar( "Time",                    "",                            0 );
    SetScriptVar( "TriggerObjectiveList",    "",                            0 );

    SetScriptSignal( "Trigger", "Trigger" );        

}
