// MISSION SCRIPT
// This is an encounter script that runs the Striga Island Giant Robot room and the coubtdown when the mission is over
//
#include "script/scriptutil.h"
#include <stdio.h>

void StrigaBossFightAndEscape(void)
{  
    INITIAL_STATE     
//////////////////////////////////////////////////////////////////////////
    ON_ENTRY  //////////////////////////////////////////////////////////////////////////
    {
    ENCOUNTERGROUP bossGroup;

    //Looking for the boss...
    bossGroup = FindEncounterGroup("EncounterGroup:BossSpawn", 0, 0, 0);
    VarSet( "BossGroup", bossGroup );
    SET_STATE( "WaitForBossToSpawn" );
    }
    END_ENTRY

////////////////////////////////////////////////////////////////////////////////////////
    STATE("WaitForBossToSpawn") ////////////////////////////////////////////////////////

    ENTITY boss;
    boss = ActorFromEncounterPos(VarGet( "BossGroup"), 1);

    //When you find him, start monitoring him
    if( !StringEqual( TEAM_NONE, boss ) )
    {
        VarSet( "Boss", boss ); 
        SET_STATE("MonitorBoss");
    }

////////////////////////////////////////////////////////////////////////////////////////
    STATE("MonitorBoss") ////////////////////////////////////////////////////////

    if( GetHealth( VarGet("Boss") ) < VarGetFraction( "BossFleeHealth" ) )
    {
        //Make Boss Run Away
        SetAIMoveTarget( VarGet("Boss"), "EncounterGroup:BossSpawn", MEDIUM_PRIORITY, 0, 1, 0.0f );
        SET_STATE( "BossFleeing" );
    }

////////////////////////////////////////////////////////////////////////////////////////
    STATE("BossFleeing") ////////////////////////////////////////////////////////
    
    if( !EntityExists( VarGet("Boss") ) || GetHealth( VarGet("Boss") ) <= 0 )
    {
        ENTITY badthing;
        ENTITY badthing2;
        //ENCOUNTERGROUP smokegroup;
        //Trigger Count Down
        badthing = CreateVillain("BadThing", VarGet("BadThingVillainType"), VarGetNumber("BadThingVillainLevel"), "Self Destruct Blast", "marker:badthing");
        SetAIPriorityList( badthing, "PL_BEGOODANDSTILL" );
        VarSet( "BadThing", badthing );
        
        badthing2 = CreateVillain("BadThing", VarGet("BadThingVillainType"), VarGetNumber("BadThingVillainLevel"), "Self Destruct Blast", "marker:badthing2");
        SetAIPriorityList( badthing2, "PL_BEGOODANDSTILL" );
        VarSet( "BadThing2", badthing2 );

        //TO DO Alarm sound, start a count down 
        if( !StringEqual( "None", VarGet("SmokeVillainType") ) )
        {
            CreateVillain("Smoke", VarGet("SmokeVillainType"), 30, "", "marker:smoke1");
            CreateVillain("Smoke", VarGet("SmokeVillainType"), 30, "", "marker:smoke2");
            CreateVillain("Smoke", VarGet("SmokeVillainType"), 30, "", "marker:smoke3");
            CreateVillain("Smoke", VarGet("SmokeVillainType"), 30, "", "marker:smoke4");
            CreateVillain("Smoke", VarGet("SmokeVillainType"), 30, "", "marker:smoke5");
        }

        //smokegroup = FindEncounterGroup("EncounterGroup:SmokeMachine", 0, 0, 0);
        //SetAIPriorityList( smokegroup, VarGet( "SmokeTurnOnPL" ) );
        {
            NUMBER waypoint = CreateWaypoint("marker:escapedoor", VarGet("WayPointText"), NULL, NULL, true, false, -1.0f);
            SetWaypoint(ALL_PLAYERS, waypoint);
        }
        TimerSet( "TimeBomb", (float)VarGetFraction( "TimeBombTime" ) );
        TimerSet( "FirstMessage", (float)VarGetFraction( "TimeBombTime" ) - (20.0f/60.0f) ); //20 seconds
        TimerSet( "SecondMessage", (float)VarGetFraction( "TimeBombTime" ) - (10.0f/60.0f) ); //10 seconds
        BroadcastAttentionMessage( VarGet("YouMustGetOutMessage"), 0, 0 );
        ShowTimer((float)VarGetFraction( "TimeBombTime" ), 0, 0 );
        VarSetNumber( "SecondsRemaining", (NUMBER)((float)VarGetFraction( "TimeBombTime" ) * 60.0 ));
        SET_STATE( "Escape" );
    }

////////////////////////////////////////////////////////////////////////////////////////
    STATE("Escape") ////////////////////////////////////////////////////////

    if( TimerElapsed( "FirstMessage" ) )
    { 
        TimerSet( "FirstMessage", 100000.0f ); //20 seconds
        BroadcastAttentionMessage( VarGet("20SecMessage"), 0, 0 );
    }
    if( TimerElapsed( "SecondMessage" ) )
    {
        TimerSet( "SecondMessage", 100000.0f ); //10 seconds
        BroadcastAttentionMessage( VarGet("10SecMessage"), 0, 0 );
    }
    if( TimerElapsed( "TimeBomb" ) )
    {
        //Boom
        SetHealth( VarGet( "BadThing" ), 0.0f, 0 ); 
        SetHealth( VarGet( "BadThing2" ), 0.0f, 0 ); 
    }

    END_STATES
}