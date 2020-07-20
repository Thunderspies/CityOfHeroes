// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

void KickAllFromMap(void)
{  
    INITIAL_STATE 
    {
        // If this encounter is active
        if (CheckMissionObjectiveExpression(VarGet("ActivateOnObjectiveComplete")))
        {
            NUMBER index;
            NUMBER count = NumEntitiesInTeam(ALL_PLAYERS);

            for (index = count - 1; index >= 0; index--)
            {
                ScriptKickFromMap(GetEntityFromTeam(ALL_PLAYERS, index + 1));
            }

            EndScript();
        }
    }

    END_STATES
}


void KickAllFromMapInit()
{
    SetScriptName("KickAllFromMap");
    SetScriptFunc(KickAllFromMap);
    SetScriptType(SCRIPT_MISSION);        

    SetScriptVar("ActivateOnObjectiveComplete",        "",            0);
}
