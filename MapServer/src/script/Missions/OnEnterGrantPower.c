// MISSION/ZONE SCRIPT
// This script will give a power to a player when they enter the map and meet the requirements
//
#include "script/scriptutil.h"
#include "script/scripthook/ScriptHookReward.h"

// called when the player enters the map
int OnEnterGrantPowerTest(ENTITY player)
{
    if (EvalEntityRequires(player, VarGet("Requires"), Script_GetCurrentBlame()) && !VarIsEmpty("Power"))
    {    
        EntityGrantPower(player, VarGet("Power"));
    }
    return 0;
}

// called when the player exits the map
int OnExitRemovePower(ENTITY player)
{
    if(!VarIsEmpty("Power"))
        EntityRemovePower(player, VarGet("Power"));

    return 0;
}
    
void OnEnterGrantPower(void)
{  
    INITIAL_STATE 
    {
        ON_ENTRY
        {
            OnPlayerEnteringMap( OnEnterGrantPowerTest );
            OnPlayerExitingMap( OnExitRemovePower );
        }
        END_ENTRY

    }

    END_STATES 
}

void OnEnterGrantPowerMissionInit()
{
    SetScriptName( "OnEnterGrantPowerMission" );
    SetScriptFunc( OnEnterGrantPower );
    SetScriptType( SCRIPT_MISSION );        
    
    SetScriptVar( "Requires",                                "1",            SP_OPTIONAL );
    SetScriptVar( "Power",                                    "",                0 );
}

void OnEnterGrantPowerZoneInit()
{
    SetScriptName( "OnEnterGrantPowerZone" );
    SetScriptFunc( OnEnterGrantPower );
    SetScriptType( SCRIPT_ZONE );        
    
    SetScriptVar( "Requires",                                "1",            SP_OPTIONAL );
    SetScriptVar( "Power",                                    "",                0 );
}