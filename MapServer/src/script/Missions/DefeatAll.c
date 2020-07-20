// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

void DefeatAllObjectiveComplete()
{
    SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGet("Objective"));

    if (!VarIsEmpty("Reward"))
        EntityGrantReward(MissionOwner(), VarGet("Reward"));
}

int DefeatAllIsTrackedCritter(STRING name)
{
    int i, defeatCount = VarGetArrayCount("DefeatList");

    if (!name)
        return false;

    for (i = 0; i < defeatCount; i++) 
    {
        if (StringEqual(name, VarGetArrayElement("DefeatList", i)))
        {
            return true;
        }
    }
    return false;
}

void DefeatAllOnEntityDefeat(ENTITY player, ENTITY victor)
{
    // check for desired type of critter
    if (IsEntityCritter(player) && DefeatAllIsTrackedCritter(GetVillainDefName(player)))
    {
        int i, critterCount;
        int isClear = VarGetNumber("IsClear");

        critterCount = NumEntitiesInTeam(ALL_CRITTERS);
        for (i = critterCount; i >= 1; i--)
        {
            if (DefeatAllIsTrackedCritter(GetVillainDefName(GetEntityFromTeam(ALL_CRITTERS, i))))
            {
                VarSetNumber("IsClear", 0);
                return;
            }
        }

        VarSetNumber("IsClear", isClear + 1);
    }
}



void DefeatAll(void)
{  
    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
                // count number of deaths
                OnEntityDefeated ( DefeatAllOnEntityDefeat );
                if (!StringEmpty(VarGet("DisplayText")))
                {
                    CreateMissionObjective(VarGet("Objective"), StringLocalize(VarGet("DisplayText")), StringLocalize(VarGet("DisplayText")));
                    //SetNamedObjectiveString(VarGet("Objective"), StringLocalize(VarGet("DisplayText")), StringLocalize(VarGet("DisplayText")));
                } 
                VarSetNumber("IsClear", 0);
        }
        END_ENTRY

        if (VarGetNumber("IsClear") > 0)
        {
            int i, critterCount;
            int isClear = VarGetNumber("IsClear");

            critterCount = NumEntitiesInTeam(ALL_CRITTERS);
            for (i = critterCount; i >= 1; i--)
            {
                if (DefeatAllIsTrackedCritter(GetVillainDefName(GetEntityFromTeam(ALL_CRITTERS, i))))
                {
                    VarSetNumber("IsClear", 0);
                    return;
                }
            }
            isClear++;

            if (isClear >= VarGetNumber("PassCount"))
            {
                DefeatAllObjectiveComplete();
                SET_STATE( "MissionOver" );
            } else {
                VarSetNumber("IsClear", isClear);
            }
        }

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
        DefeatAllObjectiveComplete();
        SET_STATE( "MissionOver" );
    }
    END_SIGNAL


    ON_STOPSIGNAL
        // nothing really to do...
    END_SIGNAL

}


void DefeatAllInit()
{
    SetScriptName( "DefeatAll" );
    SetScriptFunc( DefeatAll );
    SetScriptType( SCRIPT_MISSION );        

    SetScriptSignal( "Objective",        "CompleteObjective" );        

    SetScriptVar( "DefeatList",            "",                            SP_OPTIONAL );
    SetScriptVar( "PassCount",            "10",                        0 );
    SetScriptVar( "Objective",            "",                            0 );
    SetScriptVar( "Reward",                "",                            SP_OPTIONAL );
    SetScriptVar( "DisplayText",        "",                            SP_OPTIONAL );

}
