// ZONE SCRIPT
//

// Used for testing new features of the script system

#include "script/scriptutil.h"

void SetLevelMissionRunOnInitialization(void)
{
    if (!VarIsEmpty("AbsoluteCombatLevel"))
        SetScriptCombatLevel(VarGetNumber("AbsoluteCombatLevel"));
    if (!VarIsEmpty("MinCombatLevel"))
        SetScriptMinCombatLevel(VarGetNumber("MinCombatLevel"));
    if (!VarIsEmpty("MaxCombatLevel"))
        SetScriptMaxCombatLevel(VarGetNumber("MaxCombatLevel"));
}

void SetLevelMission(void)
{
    INITIAL_STATE
    {
        ON_ENTRY 
        {
            // Nothing to do.... really.
        }
        END_ENTRY
    }

    END_STATES


    ON_SIGNAL("End")
        EndScript();
    END_SIGNAL

    ON_STOPSIGNAL
        EndScript();
    END_SIGNAL
}

void SetLevelMissionInit()
{
    SetScriptName( "SetLevelMission" );
    SetScriptFunc( SetLevelMission );
    SetScriptFuncToRunOnInitialization( SetLevelMissionRunOnInitialization );
    SetScriptVar( "AbsoluteCombatLevel",                                "",                    SP_OPTIONAL );
    SetScriptVar( "MinCombatLevel",                                        "",                    SP_OPTIONAL );
    SetScriptVar( "MaxCombatLevel",                                        "",                    SP_OPTIONAL );

    SetScriptType( SCRIPT_MISSION );            

}