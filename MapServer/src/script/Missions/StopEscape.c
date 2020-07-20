// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

//TO DO waypoints?

void StopEscapeUpdateStatus()
{
    STRING status;
    char buf[256];
    int fled = VarGetNumber("Fled");
    int lastfled = VarGetNumber("LastFled");
    int remain = NumEntitiesInTeam(ALL_CRITTERS);

    if (VarGetNumber( "NumToSpawn" ) > 0) {
        if (fled != lastfled)
        {
            if (fled == 1) {
                SetMissionStatusWithCount(fled, VarGet("StatusMessageSingular"));
            } else {
                SetMissionStatusWithCount(fled, VarGet("StatusMessage"));
            }
            VarSetNumber("LastFled", fled);
        }
    } else {
        sprintf(buf, "%d ", fled);
        if (fled == 1) {
            status = StringAdd(buf, StringLocalize(VarGet("StatusMessageSingular")));
        } else {
            status = StringAdd(buf, StringLocalize(VarGet("StatusMessage")));
        }
        status = StringAdd(status, ", ");
        sprintf(buf, "%d ", remain);
        status = StringAdd(status, buf);
        if (remain == 1) {
            status = StringAdd(status, StringLocalize(VarGet("RemainMessageSingular")));
        } else {
            status = StringAdd(status, StringLocalize(VarGet("RemainMessage")));
        }

        SetMissionStatus(status);
    }

}

int StopEscapeOnEnterMap(ENTITY player)
{
    SetWaypoint(player, VarGetNumber("Waypoint"));
    return 0;
}

void StopEscape(void)
{  
    ENCOUNTERGROUP group; 
    int i;

    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            VarSetNumber("Waypoint", CreateWaypoint(VarGet("DoorLocation"), VarGet("WaypointText"), NULL, NULL, true, false, -1.0f));
            SetWaypoint(ALL_PLAYERS, VarGetNumber("Waypoint"));
            StopEscapeUpdateStatus();
            OnPlayerEnteringMap( StopEscapeOnEnterMap );
            VarSetNumber("LastFled", -1);
        }
        END_ENTRY
        

        // find group and send them to the door
        if( DoEvery("WaitTime", VarGetFraction("WaitTime"), 0 ) ) 
        {

             group = FindEncounter(LOCATION_NONE, 0, 0, 0, 0, "Mission", 0, SEF_OCCUPIED_ONLY);
            if( StringEqual( group, LOCATION_NONE ) )
            {
                // out of original groups
                VarSetNumber("OutOfSpawns", 1);

                // check to see if we should spawn more critters
                if (VarGetNumber( "Spawned" ) < VarGetNumber( "NumToSpawn" )) {
                    // Spawn more critters using distant arounds
                    group = FindEncounter( 
                        VarGet("DoorLocation"),                                //Relative to this spot
                        20,                                //No Nearer than this
                        600,                            //No Farther than this
                        0,                                //Inside this AREA
                        0,                                //Assigned this MissionEncounter (the LogicalName)
                        "Mission",                        //Layout (Many Named Demon. Called "EncounterSpawn" in Spawndef files and in Maps, "Layouts" in mission specs and scripts and often throughout the code, "Spawntype" in SpawnDef struct, "EncounterLayouts" in mapspec)
                        0,                                //Group Name
                        SEF_UNOCCUPIED_ONLY | SEF_HAS_PATH_TO_LOC | SEF_SORTED_BY_NEARBY_PLAYERS //Flags
                        );

                    if (!StringEqual(group, "location:none")) {
                        Spawn( 1, "NewSpawn", VarGet("AdditionalSpawns"), group, "Mission", MissionLevel(), 0 );  

                        AddAIBehavior(group, "Combat");
                        SetAIMoveTarget( group, VarGet("DoorLocation"), MEDIUM_PRIORITY, 0, 0, 0.0f );
                        VarSetNumber( "Spawned", VarGetNumber( "Spawned") + NumEntitiesInTeam( "NewSpawn" ) );
                        SwitchScriptTeam(group, "NewSpawn", "Running");
                        DetachSpawn( group );
                        VarSetNumber( "Running", NumEntitiesInTeam( "Running" ) );
                    }
                }
            } else { 

                // tell group to move to door
                SetScriptTeam(group, "Running");
                AddAIBehavior(group, "Combat");
                SetAIMoveTarget( group, VarGet("DoorLocation"), MEDIUM_PRIORITY, 0, 0, 0.0f );
                VarSetNumber( "Running", NumEntitiesInTeam( "Running" ) );
                DetachSpawn(group);
            }
        }


        // Check to see if anyone made it to the door
        if( DoEvery("ArrivalCheckTime", 0.05, 0 ) ) 
        {
            // If there are any running critters that made it to the location, move them out the door
            int running = NumEntitiesInTeam( "Running" );

            for( i = 1 ; i <= running ; i++ )
            {
                ENTITY e = GetEntityFromTeam("Running", i);
                if (GetAIReachedObjective(e))
                {
                    MarkAIReachedObjective(e);
                    AddAIBehavior(e, "RunIntoDoor");
                    SwitchScriptTeam(e, "Running", "");

                    running = NumEntitiesInTeam( "Running" );
                    i--;

                    // increment count of number that have escaped
                    VarSetNumber( "Fled", VarGetNumber( "Fled" ) + 1 );
                }
            }
            VarSetNumber( "Running", NumEntitiesInTeam( "Running" ) );

            // Check to see if too many escaped
            if (VarGetNumber( "Fled" ) >= VarGetNumber( "EscapeLimit" ) )
            {
                ClearWaypoint(ALL_PLAYERS, VarGetNumber("Waypoint"));
                DestroyWaypoint(VarGetNumber("Waypoint"));
                SetMissionComplete(0);
                SetMissionStatus("");
                EndScript();
            }

            // check to see if they got them all - either we've spawned all we need or 
            //        we've gotten all of the spawns on the map
            if ((VarGetNumber("NumToSpawn") == 0 && VarGetNumber("OutOfSpawns") == 1) ||
                ((VarGetNumber("NumToSpawn") > 0) && (VarGetNumber( "Spawned" ) >= VarGetNumber( "NumToSpawn" )) &&
                (NumEntitiesInTeam("Running") <= 0))) 
            {
                ClearWaypoint(ALL_PLAYERS, VarGetNumber("Waypoint"));
                DestroyWaypoint(VarGetNumber("Waypoint"));
                SetMissionComplete(1);
                SetMissionStatus("");
                EndScript();
            }

            // update status messages
            StopEscapeUpdateStatus();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    STATE( "EndScript" ) ////////////////////////////////////////////////////////
    {
        ClearWaypoint(ALL_PLAYERS, VarGetNumber("Waypoint"));
        DestroyWaypoint(VarGetNumber("Waypoint"));
        EndScript();
    }

    END_STATES 
}