// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

void DefeatListObjectiveComplete()
{
    SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGet("Objective"));

    if (!VarIsEmpty("Reward"))
        EntityGrantReward(MissionOwner(), VarGet("Reward"));
}

void DefeatListOnEntityDefeat(ENTITY player, ENTITY victor)
{
    // check for desired type of critter
    if (IsEntityCritter(player))
    {
        STRING name = GetVillainDefName(player);
        int i, count = VarGetArrayCount("DefeatList");
        ENCOUNTERGROUP group = "first"; 
        int found = false;

        if (!VarIsEmpty("DefeatVillainGroup"))
        {
            if (GetVillainGroupIDFromName(VarGet("DefeatVillainGroup")) == GetVillainGroupID(player))
                found = true;
        }

        for (i = 0; (i < count && !found); i++) 
        {
            if (StringEqual(name, VarGetArrayElement("DefeatList", i)))
            {
                found = true;
            }
        }

        if (found)
        {
            int totalCount = VarGetNumber("TotalKillCount") + 1;

            if (totalCount >= VarGetNumber("ObjectiveCount"))
            {
                DefeatListObjectiveComplete();
            } else {
                if (!StringEmpty(VarGet("DisplayText")))
                {
                    STRING desc;
                    desc = StringAdd(NumberToString(VarGetNumber("ObjectiveCount") - totalCount), " ");
                    desc = StringAdd(desc, StringLocalize(VarGet("DisplayText")));

                    SetNamedObjectiveString(VarGet("Objective"), desc, desc);
                } 
                else if (!StringEmpty(VarGet("DisplayTextWithVar")))
                {
                    STRING desc = StringLocalizeWithReplacements(VarGet("DisplayTextWithVar"), 1,
                                            "num", NumberToString(VarGetNumber("ObjectiveCount") - totalCount));
                    SetNamedObjectiveString(VarGet("Objective"), desc, desc);
                }

            }
            VarSetNumber("TotalKillCount", totalCount);
        }
    }
}



void DefeatList(void)
{  
    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
                // count number of deaths
                OnEntityDefeated ( DefeatListOnEntityDefeat );
                if (!StringEmpty(VarGet("DisplayText")))
                {
                    STRING desc;
                    desc = StringAdd(NumberToString(VarGetNumber("ObjectiveCount")), " ");
                    desc = StringAdd(desc, StringLocalize(VarGet("DisplayText")));

                    CreateMissionObjective(VarGet("Objective"), desc, desc);
                    //SetNamedObjectiveString(VarGet("Objective"), desc, desc);
                } 
                else if (!StringEmpty(VarGet("DisplayTextWithVar")))
                {
                    STRING desc = StringLocalizeWithReplacements(VarGet("DisplayTextWithVar"), 1,
                                    "num", NumberToString(VarGetNumber("ObjectiveCount")));
                    CreateMissionObjective(VarGet("Objective"), desc, desc);
                    //SetNamedObjectiveString(VarGet("Objective"), desc, desc);
                }
        }
        END_ENTRY

        // spawn stuff
        if( MissionIsComplete() )
        {
            SET_STATE( "MissionOver" );
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    STATE( "MissionOver" ) ////////////////////////////////////////////////////////
    {
        EndScript();
    }

    END_STATES 

    ON_SIGNAL("CompleteObjective")
    { 
        DefeatListObjectiveComplete();
    }
    END_SIGNAL


    ON_STOPSIGNAL
        // nothing really to do...
    END_SIGNAL

}


void DefeatListInit()
{
    SetScriptName( "DefeatList" );
    SetScriptFunc( DefeatList );
    SetScriptType( SCRIPT_MISSION );        

    SetScriptSignal( "Objective", "CompleteObjective" );        

    SetScriptVar( "DefeatList",            "",                            SP_OPTIONAL );
    SetScriptVar( "ObjectiveCount",        "",                            0 );
    SetScriptVar( "Objective",            "",                            0 );
    SetScriptVar( "Reward",                "",                            SP_OPTIONAL );
    SetScriptVar( "DisplayText",        "",                            SP_OPTIONAL );
    SetScriptVar( "DisplayTextWithVar",    "",                            SP_OPTIONAL );
    SetScriptVar( "DefeatVillainGroup",    "",                            SP_OPTIONAL );

}
