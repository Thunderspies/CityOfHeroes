// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

//TO DO waypoints?

//Var the bad guy encounter
//Var the number to be found
//
void CompetitiveSearch(void)
{  
    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            //1. Spawn some bad guys at the other end of the map
            //TO DO figure out levels once nad for all
            //Spawn( 1, "SearchingEnemies", VarGet("SearchingEnemiesEncounter"), 
            //2. Set the bad guys wander and with a PL_WANDERANDMEETOBJECTIVE

            //3. Tell bad guys that they like to kill objectives
            AddAIBehaviorPermFlag(VarGet("Rescuee"), "AggroNonAggroTargets");
            //3. Tell players the baddies are looking for the same stuff they are

            //4. Start counting number of objectives the baddies have found 
        }
        END_ENTRY

        //How do we make baddies find, and use (destroy?) objectives, 
        //How do we count them?

        //if( remainingObjectives < MissionObjectivesNeeded )
        {
            //Give message that the bad guys found too many objectives
            SetMissionComplete( 0 ); //you failed the mission, the 
            //Villains all run out of the map
            EndScript();
        }
    }
    END_STATES 
}