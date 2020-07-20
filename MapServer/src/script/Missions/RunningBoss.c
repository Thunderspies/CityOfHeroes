// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

// 
int RunningBossDeathCallback(ENTITY entity)
{
    //IF this is the running boss dying, record why the encounter died and complete it
    if( entity && StringEqual( entity, VarGet( "RunningBoss" ) ) )
    {
        SetEncounterComplete( MyEncounter(), EntityFledMap( entity ) ? 0 : 1 ); 
    }
    return 1;
}

void RunningBoss(void)
{  
    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            ENTITY boss; 

            //Has the script been given any meaningful parameters?
            if( 0 == VarGetFraction("RunWhenHealthIsBelow") &&
                0 == VarGetNumber("RunWhenTeamSizeIsBelow") )
            {
                EndScript();
                return;  //return because I don't want any of the other stuff even tried, especially the fled Check
            }
            
            //Does the boss actually exist?
            boss = ActorFromEncounterActorName(MyEncounter(), VarGet( "Actor" ) );

            if( !boss )
            {
                EndScript();
                return;  //return because I don't want any of the other stuff even tried, especially the fled Check
            }

            SayOnKillHero(boss, VarGet("BOSS_KILLEDHERO_DIALOG") );

            VarSet( "RunningBoss", boss );

            ScriptControlsCompletion( MyEncounter() );

            OnEntityFreed(RunningBossDeathCallback);
        }
        END_ENTRY

        {
            ENTITY boss = VarGet( "RunningBoss" );
        
            if( VarGetFraction("RunWhenHealthIsBelow") >= GetHealth( boss ) && VarGetFraction("RunWhenHealthIsBelow") )
            {
                Say( boss, VarGet( "BOSS_RUNAWAY_DIALOG" ), 2 ); 
                SET_STATE( "BossRuns" );
            }
            // The -1 is so boss does not count himself
            if( (NumEntitiesInTeam( MyEncounter() ) - 1) < VarGetNumber( "RunWhenTeamSizeIsBelow" ) && VarGetNumber( "RunWhenTeamSizeIsBelow" ) )
            {
                Say( boss, VarGet( "BOSS_RUNAWAY_DIALOG" ), 2 ); 
                SET_STATE( "BossRuns" );
            }
        }
    }

    STATE( "BossRuns" )
    {
        ON_ENTRY
            SetAIPriorityList( VarGet( "RunningBoss" ), "PL_FleeToNearestDoor" );
        END_ENTRY

        if( 0 ) //Defend myself if needed
        {
            SET_STATE( "BossDefends" );
        }

    }

    STATE( "BossDefends" )
    {
        ON_ENTRY
            SetAIPriorityList( VarGet( "RunningBoss" ), "PL_COMBAT" );
        END_ENTRY

        if( 0 ) //Defend myself if needed
        {
            SET_STATE( "BossRuns" );
        }
    }
    END_STATES 
    
    //Done on every tick
    //Encounter fails if the boss got away
    //Encounter succeeds if he is killed for any other reason
    if( GetHealth( VarGet( "RunningBoss" ) ) <= 0.0 )
    {
        EndScript();
    }
}
