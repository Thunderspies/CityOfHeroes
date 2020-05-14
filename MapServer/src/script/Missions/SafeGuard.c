// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

// check objective list
int SafeGuardCheckObjectiveListAreAllComplete(STRING varlist)
{
    int i, count;

    count = VarGetArrayCount(varlist);

    for (i = 0; i < count; i++)
    {
        if (!ObjectiveIsComplete(VarGetArrayElement(varlist, i)))
            return false;
    }
    return true;
}

//If a dead guy is a robber who got away, fail the encounter
int FleeingRobberDeathCallback(ENTITY deadGuy )
{
    if( EntityFledMap( deadGuy ) )
    {
        int i;
        int count = NumEntitiesInTeam( "Robbers" );

        for( i = 1 ; i <= count ; i++ )
        {
            ENTITY robber = GetEntityFromTeam("Robbers", i);

            if( StringEqual( robber, deadGuy ) )
            {
                SetEncounterComplete( VarGet("FleeingEncounter"), 0 );        //Failure
                SetEncounterComplete( VarGet("FleeingEncounterBoss"), 0 );    //Failure

                if (!VarIsEmpty("ObjectiveSetFailedWhenBankRobbersEscape"))
                    SetMissionObjectiveComplete(false, VarGet("ObjectiveSetFailedWhenBankRobbersEscape"));
            }
        }
    }
    return 1;
}

void SafeGuard()
{
    INITIAL_STATE   
     {    
        ON_ENTRY  
        {
            F32 timeToRobbery = (float)VarGetFraction( "BaseTimeBeforeBankRobbery" );
            F32 variance = (float)VarGetFraction( "TimePlusOrMinusBeforeBankRobbery" );

            timeToRobbery += RandomNumber( -1*variance, variance );

            TimerSet( "TimeToBankRobbery", timeToRobbery );
            TimerSet( "TimeToNearBankRobbery", timeToRobbery - VarGetFraction("RobberyWarningTime"));
            VarSetNumber("WarningSent", 0);

            OnEntityFreed( FleeingRobberDeathCallback ); //To see if a robber got away

            SendPlayerAttentionMessageWithColor( ALL_PLAYERS, VarGet("FloatMessageWhenWarning"), 0xff0000ff, kFloaterStyle_Attention );
        }
        END_ENTRY

 
        //When the Bank Robbery goes off
        if( TimerElapsed( "TimeToBankRobbery" ) || SafeGuardCheckObjectiveListAreAllComplete("TriggerObjectiveList"))
        {
            SET_STATE("BankRobbery");
        }
    }

    ////////////////////////////////////////////////////////////////////
    STATE("BankRobbery")
    {
        ON_ENTRY  
        {
            ENCOUNTERGROUP vaultRobbers;
            ENCOUNTERGROUP group; 

            //This should trigger the two mission encounters that don't go for the vault
            SetMissionObjectiveComplete( OBJECTIVE_SUCCESS, VarGet("ObjectiveSetCompleteWhenTimerExpires") );
            
            //The guys that go for the vault are controlled by the script, the other guys are just Spawn on objective complete
            group = FindEncounter( 
                LOC_ORIGIN,                                                            //Relative to this spot
                0,                                                                    //No Nearer than this
                0,                                                                    //No Farther than this (0 = all of them)
                0,                                                                    //Inside this AREA
                0,                                                                    //Assigned this MissionEncounter (the LogicalName)
                GetSpawnLayout(VarGet("EncounterThatGoesForTheVaultThenFlees")),    //Layout (Many Named Demon. Called "EncounterSpawn" in Spawndef files and in Maps, "Layouts" in mission specs and scripts and often throughout the code, "Spawntype" in SpawnDef struct, "EncounterLayouts" in mapspec)
                0,                                                                    //Group Name
                SEF_UNOCCUPIED_ONLY | SEF_SORTED_BY_NEARBY_PLAYERS //Flags
                );

            VarSet("FleeingEncounter", group);
            vaultRobbers = Spawn( 1, "Robbers", VarGet("EncounterThatGoesForTheVaultThenFlees"), group, 0, 0, 0 );

            //Set the robbers to run for the Vault and destroy it handled by patrolling

            //The boss is also controlled by the script, since they will need to run
            group = FindEncounter( 
                LOC_ORIGIN,                                                                //Relative to this spot
                0,                                                                        //No Nearer than this
                0,                                                                        //No Farther than this (0 = all of them)
                0,                                                                        //Inside this AREA
                0,                                                                        //Assigned this MissionEncounter (the LogicalName)
                GetSpawnLayout(VarGet("EncounterBossThatWaitsForTheVaultThenFlees")),    //Layout (Many Named Demon. Called "EncounterSpawn" in Spawndef files and in Maps, "Layouts" in mission specs and scripts and often throughout the code, "Spawntype" in SpawnDef struct, "EncounterLayouts" in mapspec)
                0,                                                                        //Group Name
                SEF_UNOCCUPIED_ONLY | SEF_SORTED_BY_NEARBY_PLAYERS                        //Flags
                );

            VarSet("FleeingEncounterBoss", group);
            vaultRobbers = Spawn( 1, "Robbers", VarGet("EncounterBossThatWaitsForTheVaultThenFlees"), group, 0, 0, 0 );


            SendPlayerAttentionMessageWithColor( ALL_PLAYERS, VarGet("FloatMessageWhenTimerExpires"), 0xff0000ff, kFloaterStyle_Attention );

            SetMissionStatus( VarGet("NewMissionMessageWhenTimerExpires") );
        }
        END_ENTRY

        //Now wait for the Vault to get destroyed.  When it does, instruct the Vault destroyers to
        //head for the exit.  
        //TO DO maybe get the vault and check for existance
        if( ObjectiveIsComplete( VarGet( "ObjectiveThatCausesBankRobbersToFlee" ) ) )
        {
            if( NumEntitiesInTeam("Robbers") > 0 )
            {
                SET_STATE("TheGetAway");
            }
            else
            {
                SET_STATE("TheJigIsUp");
            }
        }
    }

    ////////////////////////////////////////////////////////////////////
    STATE("TheGetAway")
    {
        ON_ENTRY  
        {
            LOCATION exit = LOCATION_NONE;
            STRING bhvr = 0;
            STRING loc = VarGet( "DoorBankRobbersFleeTo" );
    
            bhvr = StringAdd( bhvr, "MoveToPos(\"");
            bhvr = StringAdd( bhvr, loc);
            bhvr = StringAdd( bhvr, "\",CombatOverRide(Aggressive),UseEnterable), RunIntoDoor" );

            AddAIBehavior( "Robbers", bhvr );

            BroadcastAttentionMessage( VarGet("FloatMessageWhenBankRobbersFlee"), 0, 0 );

            SetMissionStatus( VarGet("NewMissionMessageWhenRobbersFlee") );

            {
                NUMBER waypoint = CreateWaypoint(VarGet("DoorWaypointLocation"), VarGet("DoorWaypointTooltip"), 
                                                    NULL, NULL, true, false, -1.0f);
                SetWaypoint(ALL_PLAYERS, waypoint);
                VarSetNumber( "FleeingWayPoint", waypoint );
            }
        }
        END_ENTRY

        //The script shouldn't need to do anything else...FleeingRobberDeathCallback should handle escapees
    }

    ////////////////////////////////////////////////////////////////////
    STATE("TheJigIsUp")
    {
        //Just waiting for the mission to end -- no reason to end the script, and it might be useful to keep open for debugging
    }

    END_STATES
}


void SafeGuardInit()
{
    SetScriptName( "SafeGuard" );
    SetScriptFunc( SafeGuard );
    SetScriptType( SCRIPT_MISSION );        
    
    SetScriptVar( "TriggerObjectiveList",                        "",            0 );
    SetScriptVar( "BaseTimeBeforeBankRobbery",                    "",            0 );
    SetScriptVar( "TimePlusOrMinusBeforeBankRobbery",            "",            0 );
    SetScriptVar( "RobberyWarningTime",                            "1",        0 );
    SetScriptVar( "ObjectiveSetCompleteWhenTimerExpires",        "",            0 );
    SetScriptVar( "FloatMessageWhenWarning",                    "",            0 );
    SetScriptVar( "FloatMessageWhenTimerExpires",                "",            0 );
    SetScriptVar( "NewMissionMessageWhenTimerExpires",            "",            0 );
    
    SetScriptVar( "EncounterThatGoesForTheVaultThenFlees",        "",            0 ); 
    SetScriptVar( "EncounterBossThatWaitsForTheVaultThenFlees",    "",            0 ); 
    //Mission encounter name of the guys that head for the vault, (need to be Script Controlled, not SpawnOnObjectiveComplete
    //because they need to be controlled more carefully by the script

    SetScriptVar( "ObjectiveThatCausesBankRobbersToFlee",        "",            0 );
    SetScriptVar( "FloatMessageWhenBankRobbersFlee",            "",            0 );
    SetScriptVar( "NewMissionMessageWhenRobbersFlee",            "",            0 );
    SetScriptVar( "DoorBankRobbersFleeTo",                        "",            0 );
    SetScriptVar( "DoorWaypointTooltip",                        "",            0 );
    SetScriptVar( "DoorWaypointLocation",                        "",            0 );

    SetScriptVar( "ObjectiveSetFailedWhenBankRobbersEscape",    "",            0 );
    SetScriptVar( "FloatMessageWhenBankRobbersEscape",            "",            0 );
}
