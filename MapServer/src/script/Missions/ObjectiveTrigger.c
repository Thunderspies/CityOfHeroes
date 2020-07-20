// MISSION SCRIPT
// Checks a list of mission objectives and plays a specified music sound file when they are completed
//
#include "script/scriptutil.h"
#include "storyarc/mission.h"

void ObjectiveTrigger(void)
{  
    int i, count;

    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            // only on mission maps
            if (!OnMissionMap())
                EndScript();
        }
        END_ENTRY
    }


    count = VarGetArrayCount("ObjectiveList");

    // run through all objectives and see if we should play the corresponding sound
    for (i = 0; i < count; i++)
    {    
        if (VarIsEmpty(StringAdd("Complete", NumberToString(i))) && 
            ObjectiveIsComplete(VarGetArrayElement("ObjectiveList", i)))
        {
            // play sound if there is one
            if (!VarIsEmpty("SoundList") && i < VarGetArrayCount("SoundList"))
            {
                STRING soundName = VarGetArrayElement("SoundList", i);
                SendPlayerSound(ALL_PLAYERS, soundName, SOUND_MUSIC, 1.0f);
                mission_StoreCurrentServerMusic(soundName, 1.0f);
            }

            // mark complete
            VarSet(StringAdd("Complete", NumberToString(i)), "done");
        }
    }

    END_STATES 
}

void ObjectiveTriggerInit()
{
    SetScriptName( "ObjectiveTrigger" );
    SetScriptFunc( ObjectiveTrigger );
    SetScriptType( SCRIPT_MISSION );        

    SetScriptVar( "ObjectiveList",                "",                    0);
    SetScriptVar( "SoundList",                    "",                    0);
}

