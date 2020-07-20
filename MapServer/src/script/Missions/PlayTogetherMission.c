// PlayTogetherMission is now deprecated!  All mission maps have this functionality by default!
// Except for Spring Fling, which still has a super blatant hack in the update function of this script...

#include "script/scriptutil.h"

void PlayTogetherMission(void)
{
    INITIAL_STATE
    {
        ON_ENTRY 
        {
            //Crazy Hack to make the Spring Fling mission work without retrofitting tons of ai code into U10. 
            //Should be removed and set correctly when Raoul makes NotAttackedByJustAnyone is a regular ai flag
            ENCOUNTERGROUP group = FindEncounter( 0, 0, 0, 0, "GuardedObject", 0, 0, 0 );
            if( !StringEqual( group, LOCATION_NONE ) )
            {
                VarSet( "MyGuardedObject", group );
                SET_STATE( "DoCrazyHack" );
            }
            //End hack
        }
        END_ENTRY
    }

    STATE( "DoCrazyHack")
    {
        //Hack cont. As soon as it's spawn and then from here on out, set this flag
        ENTITY ent = ActorFromEncounterPos( VarGet("MyGuardedObject"), 6 );
        if( !StringEqual( ent, TEAM_NONE ) )
            AddAIBehaviorPermFlag(ent, "DoNotDrawAggro");
        //End hack
    }
    END_STATES


    ON_SIGNAL("End")
        EndScript();
    END_SIGNAL

    ON_STOPSIGNAL
        EndScript();
    END_SIGNAL
}

void PlayTogetherMissionInit()
{
    SetScriptName( "PlayTogetherMission" );
    SetScriptFunc( PlayTogetherMission );
//    SetScriptFuncToRunOnInitialization( PlayTogetherMissionRunOnInitialization );
    SetScriptType( SCRIPT_MISSION );            

    SetScriptVar( "WhoCanTeam", "Factions", SP_OPTIONAL | SP_DONTDISPLAYDEBUG );
}