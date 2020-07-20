// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

void SendText(void)
{  
    INITIAL_STATE 
    {
        // If this encounter is active
        if (CheckMissionObjectiveExpression(VarGet("ActivateOnObjectiveComplete")))
        {
            NUMBER count = NumEntitiesInTeam(ALL_PLAYERS);
            STRING chatMessage = VarGet("ChatMessage");
            STRING floaterMessage = VarGet("FloaterMessage");

            if (stricmp(chatMessage, ""))
            {
                ScriptSendChatMessage(ALL_PLAYERS, chatMessage);
            }

            if (stricmp(floaterMessage, ""))
            {
                ScriptServeFloater(ALL_PLAYERS, floaterMessage);
            }

            EndScript();
        }
    }

    END_STATES
}


void SendTextInit()
{
    SetScriptName("SendText");
    SetScriptFunc(SendText);
    SetScriptType(SCRIPT_MISSION);        

    SetScriptVar("ActivateOnObjectiveComplete",        "",            0);
    SetScriptVar("ChatMessage",                        "",            SP_OPTIONAL);
    SetScriptVar("FloaterMessage",                    "",            SP_OPTIONAL);
}
