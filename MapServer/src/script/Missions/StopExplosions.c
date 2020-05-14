
#include "script/scriptutil.h"


void StopExplosions()
{
    INITIAL_STATE   
     {    
        //When the Key is found, start the timer
        if( ObjectiveIsComplete( VarGet( "ObjectiveThatStartsTimer" ) ) )
        {
            SET_STATE("StartExplosionTimer");
        }
    }

    ////////////////////////////////////////////////////////////////////
    STATE("StartExplosionTimer")
    {
        ON_ENTRY  
        {
            TimerSet( "ExplosionTimer", (float)VarGetFraction( "TimeBeforeExplosions" ) ); //20 seconds
            BroadcastAttentionMessage( VarGet("FloatTextForTimer"), 0, 0 );
        }
        END_ENTRY

        //TO DO add X second left floaters

        //When the Timer is up, explode
        if( TimerElapsed( "ExplosionTimer" ) )
        { 
            int i, count, message = false;

            //Get all the objectives that explode and set them failed
            count = VarGetArrayCount("ObjectivesThatExplodeList");
            for( i = 0 ; i < count ; i++ )
            {
                if (!ObjectiveIsComplete(VarGetArrayElement("ObjectivesThatExplodeList", i))) {
                    SetMissionObjectiveComplete( OBJECTIVE_FAILED, VarGetArrayElement("ObjectivesThatExplodeList", i) );
                    message = true;
                }
            }

            if (message)
                BroadcastAttentionMessage( VarGet("FloatTextForTimeUp"), 0, 0 );

            SET_STATE( "Done" );
        }
    }

    STATE( "Done" ) ////////////////////////////////////////////////////////
    {
        EndScript();
    }

    END_STATES
}

void StopExplosionsInit()
{
    SetScriptName( "StopExplosions" );
    SetScriptFunc( StopExplosions );
    SetScriptType( SCRIPT_MISSION );        

    SetScriptVar( "ObjectiveThatStartsTimer",    "",            0 );
    SetScriptVar( "TimeBeforeExplosions",        "",            0 );
    SetScriptVar( "FloatTextForTimer",            "",            0 );  
    SetScriptVar( "ObjectivesThatExplodeList",    "",            0 );  
    SetScriptVar( "FloatTextForTimeUp",            "",            0 );  
}
