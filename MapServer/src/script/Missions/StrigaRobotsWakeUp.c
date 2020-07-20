// MISSION SCRIPT
// This is an encounter script that runs the Striga Island Robots Wake up and the Doors Lock
//
#include "script/scriptutil.h"

//#include "stdio.h"

void StrigaRobotsWakeUp(void)
{  
    INITIAL_STATE     

        //getchar();

        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        //Set up: Initially the doors are open, and waiting to get set to Locked
        UnlockDoor( "marker:InnerDoor1" );
        UnlockDoor( "marker:InnerDoor2" );
        UnlockDoor( "marker:OuterDoor" );
        OpenDoor( "marker:InnerDoor1" );
        OpenDoor( "marker:InnerDoor2" );
        OpenDoor( "marker:OuterDoor" );

        SET_STATE("RobotsAsleep");
        END_ENTRY  //////////////////////////////////////////////////////////////////////////

    STATE("RobotsAsleep") ////////////////////////////////////////////////////////
        //If either objective has been fulfilled, go to attack mode! 
        //(this is a little weird, I think it's likely we want just one)
        if( ObjectiveIsComplete( VarGet("InnerObjective") ) || ObjectiveIsComplete( VarGet("OuterObjective") ) )  
            SET_STATE("RobotsWakingUp");

    STATE("RobotsWakingUp") ///////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
        ENCOUNTERGROUP robotGroup;

        BroadcastAttentionMessage( VarGet("AmbushSprungMessage"), 0, 0 );

        CloseDoor( "marker:InnerDoor1" );
        CloseDoor( "marker:InnerDoor2" );
        CloseDoor( "marker:OuterDoor" );
        LockDoor( "marker:InnerDoor1", VarGet( "LockedDoorMessage" ) );
        LockDoor( "marker:InnerDoor2", VarGet( "LockedDoorMessage" ) );
        LockDoor( "marker:OuterDoor", VarGet( "LockedDoorMessage" ));

        //TO DO how many robot groups are there?  They are hard coded.  (Would be nice to be arbitrary)
        //Since "EncounterGroup:NAME" can't be treated as a teams, I convert them to "encounterid:##"
        //Then add the group members to the amalgamated team
        robotGroup = FindEncounterGroup("EncounterGroup:InnerRobots0", 0, 0, 0);
        VarSet( "InnerRobots0", robotGroup );
        SetScriptTeam( robotGroup, "AllInnerRobots" );
        robotGroup = FindEncounterGroup("EncounterGroup:InnerRobots1", 0, 0, 0);
        VarSet( "InnerRobots1", robotGroup );
        SetScriptTeam( robotGroup, "AllInnerRobots" );
        robotGroup = FindEncounterGroup("EncounterGroup:InnerRobots2", 0, 0, 0);
        VarSet( "InnerRobots2", robotGroup );
        SetScriptTeam( robotGroup, "AllInnerRobots" );
        robotGroup = FindEncounterGroup("EncounterGroup:InnerRobots3", 0, 0, 0);
        VarSet( "InnerRobots3", robotGroup );
        SetScriptTeam( robotGroup, "AllInnerRobots" );
        robotGroup = FindEncounterGroup("EncounterGroup:InnerRobots4", 0, 0, 0);
        VarSet( "InnerRobots4", robotGroup );
        SetScriptTeam( robotGroup, "AllInnerRobots" );
        robotGroup = FindEncounterGroup("EncounterGroup:InnerRobots5", 0, 0, 0);
        VarSet( "InnerRobots5", robotGroup );
        SetScriptTeam( robotGroup, "AllInnerRobots" );

        robotGroup = FindEncounterGroup("EncounterGroup:OuterRobots0", 0, 0, 0);
        VarSet( "OuterRobots0", robotGroup );
        SetScriptTeam( robotGroup, "AllOuterRobots" );
        robotGroup = FindEncounterGroup("EncounterGroup:OuterRobots1", 0, 0, 0);
        VarSet( "OuterRobots1", robotGroup );
        SetScriptTeam( robotGroup, "AllOuterRobots" );
        robotGroup = FindEncounterGroup("EncounterGroup:OuterRobots2", 0, 0, 0);
        VarSet( "OuterRobots2", robotGroup );
        SetScriptTeam( robotGroup, "AllOuterRobots" );
        robotGroup = FindEncounterGroup("EncounterGroup:OuterRobots3", 0, 0, 0);
        VarSet( "OuterRobots3", robotGroup );
        SetScriptTeam( robotGroup, "AllOuterRobots" );
        robotGroup = FindEncounterGroup("EncounterGroup:OuterRobots4", 0, 0, 0);
        VarSet( "OuterRobots4", robotGroup );
        SetScriptTeam( robotGroup, "AllOuterRobots" );
        robotGroup = FindEncounterGroup("EncounterGroup:OuterRobots5", 0, 0, 0);
        VarSet( "OuterRobots5", robotGroup );
        SetScriptTeam( robotGroup, "AllOuterRobots" );

        //Wake the first set of robots and set the timers for the remaining robots
        SetAIPriorityList( VarGet( "InnerRobots0" ), VarGet( "InnerRobotAttackPL" ) );
        SetAIPriorityList( VarGet( "OuterRobots0" ), VarGet( "OuterRobotAttackPL" ) );

        TimerSet( "Wake1", (float)VarGetFraction( "AwakenTimeOfRobotGroup1" ) );
        TimerSet( "Wake2", (float)VarGetFraction( "AwakenTimeOfRobotGroup2" ) );
        TimerSet( "Wake3", (float)VarGetFraction( "AwakenTimeOfRobotGroup3" ) );
        TimerSet( "Wake4", (float)VarGetFraction( "AwakenTimeOfRobotGroup4" ) );
        TimerSet( "Wake5", (float)VarGetFraction( "AwakenTimeOfRobotGroup5" ) );
        }
        END_ENTRY

        if( TimerElapsed( "Wake1" ) )
        {
            TimerSet("Wake1", 100000.0f); //hack till I make a timer clear
            SetAIPriorityList( VarGet( "InnerRobots1" ), VarGet( "InnerRobotAttackPL" ) );
            SetAIPriorityList( VarGet( "OuterRobots1" ), VarGet( "OuterRobotAttackPL" ) );
        }
        if( TimerElapsed( "Wake2" ) )
        {
            TimerSet("Wake2", 100000.0f); //hack till I make a timer clear
            SetAIPriorityList( VarGet( "InnerRobots2" ), VarGet( "InnerRobotAttackPL" ) );
            SetAIPriorityList( VarGet( "OuterRobots2" ), VarGet( "OuterRobotAttackPL" ) );
        }
        if( TimerElapsed( "Wake3" ) )
        {
            TimerSet("Wake3", 100000.0f); //hack till I make a timer clear
            SetAIPriorityList( VarGet( "InnerRobots3" ), VarGet( "InnerRobotAttackPL" ) );
            SetAIPriorityList( VarGet( "OuterRobots3" ), VarGet( "OuterRobotAttackPL" ) );
        }
        if( TimerElapsed( "Wake4" ) )
        {
            TimerSet("Wake4", 100000.0f); //hack till I make a timer clear
            SetAIPriorityList( VarGet( "InnerRobots4" ), VarGet( "InnerRobotAttackPL" ) );
            SetAIPriorityList( VarGet( "OuterRobots4" ), VarGet( "OuterRobotAttackPL" ) );
        }
        if( TimerElapsed( "Wake5" ) )
        {
            TimerSet("Wake5", 100000.0f); //hack till I make a timer clear
            SetAIPriorityList( VarGet( "InnerRobots5" ), VarGet( "InnerRobotAttackPL" ) );
            SetAIPriorityList( VarGet( "OuterRobots5" ), VarGet( "OuterRobotAttackPL" ) );
        }

        //If either of the two robot teams is destroyed -or-
        //If all players are gone from an area, unlock all doors
        if( DoEvery( "CheckToUnlock", 0.2f, 0 ) ) 
        {
            NUMBER unlock = 0;

            if(    0 == NumEntitiesInArea( "trigger:RobotRoom", ALL_PLAYERS) || 0 == NumEntitiesInArea( "trigger:InnerRobotRoom", ALL_PLAYERS) )
                unlock = 1;
            
            if( NumEntitiesInTeam( "AllInnerRobots" ) == 0 || NumEntitiesInTeam( "AllOuterRobots" ) == 0 )
                unlock = 1;

            if( unlock )
            {
                OpenDoor( "marker:InnerDoor1" );
                OpenDoor( "marker:InnerDoor2" );
                OpenDoor( "marker:OuterDoor" );
                UnlockDoor( "marker:InnerDoor1" );
                UnlockDoor( "marker:InnerDoor2" );
                UnlockDoor( "marker:OuterDoor" );
            }
        }

    END_STATES
}