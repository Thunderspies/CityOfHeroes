//////////////////////////////////////////////////////////////////////////
//
// MISSION SCRIPT
// Encounter Script
//
//
//
//////////////////////////////////////////////////////////////////////////
//
// Tether
// 
// Tether causes a spawn to reset if it leaves it's spawn area.
// if tetherRadius is exceeded, the boss teleports back to it's initial spot,
// and resets it's health to full.
// it may optionally say something (tether_dialog)
// it may optionally set an objective (tether_objective)
//

#include "script/scriptutil.h"
#include "script/scriptengine.h"
#include "entity/entity.h"
#include "storyarc/mission.h"
#include "script/Scripthook/ScriptHookInternal.h"

static char TETHER_RADIUS[] = "TETHER_RADIUS";
static char TETHER_DIALOG[] = "TETHER_DIALOG";
static char TETHER_OBJECTIVE[] = "TETHER_OBJECTIVE";
static char TETHER_AI[] = "TETHER_AI";

static char TETHER_BOSS[] = "TETHER_BOSS";
static char TETHER_LOC[] = "TETHER_LOC";

void Tether(void)
{  
    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY
        {
            ENTITY boss;

            boss = ActorFromEncounterActorName(MyEncounter(), VarGet( "Actor" ) );

            if( !boss || StringEqual(boss, TEAM_NONE))
                EndScript();

            VarSet(TETHER_LOC, EntityPos(boss));
            VarSet(TETHER_BOSS, boss );
        }
        END_ENTRY

        {
            ENTITY boss;
            FRACTION distance;
            const char *obj;
            const char *dialog;
            LOCATION loc;
            LOCATION tether_loc;
            FRACTION radius;
            const char *tether_ai;

            boss = VarGet( TETHER_BOSS );
            loc = EntityPos(boss);
            tether_loc = VarGet( TETHER_LOC );

            distance = DistanceBetweenLocations(loc, tether_loc);
            radius = VarGetFraction(TETHER_RADIUS);
            if (radius < 0.1)
                radius = 250.0;

            if (distance > radius)
            {
                SetPosition(boss, tether_loc);
                SetHealth( boss, 1.0, 0 );

                tether_ai = VarGet(TETHER_AI);
                if (tether_ai[0])
                    AddAIBehavior(boss, tether_ai);

                dialog = VarGet(TETHER_DIALOG);
                if (dialog[0])
                    Say( boss, dialog, 2 );

                obj = VarGet(TETHER_OBJECTIVE);
                if (obj[0])
                    SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, obj);
            }
        }
    }
    END_STATES 
}

void TetherInit()
{
    SetScriptName( "Tether" );     
    SetScriptFunc( Tether );
    SetScriptType( SCRIPT_ENCOUNTER );        

    SetScriptVar( "Actor",                "",            0 );

    SetScriptVar( TETHER_RADIUS,        "250.0",    SP_OPTIONAL ); 
    SetScriptVar( TETHER_DIALOG,        "",            SP_MULTIVALUE | SP_OPTIONAL ); 
    SetScriptVar( TETHER_OBJECTIVE,        "",            SP_OPTIONAL ); 
    //AI defaults to stand still for 12 seconds to prevent looping.  Better would be to drop aggro...
    SetScriptVar( TETHER_AI,            "Combat(DoNotMove,Timer(12))",    SP_OPTIONAL ); 
}
