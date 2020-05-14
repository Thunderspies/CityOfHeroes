// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

// Some helper functions to make accessing the list vars easier
// LH: I'd like to change this at some point, make the ScriptUI handle this functionality
static char* DefeatListUIName(int index)
{
    static char buf[32];
    sprintf(buf, "DefeatList%i", index);
    return buf;
}
static char* DefeatListEnemyName(int index)
{
    static char buf[32];
    sprintf(buf, "EnemyName%i", index);
    return buf;
}
static char* DefeatListEnemyColor(int index)
{
    static char buf[32];
    sprintf(buf, "EnemyColor%i", index);
    return buf;
}
static char* DefeatListValName(int index)
{
    static char buf[32];
    sprintf(buf, "ValName%i", index);
    return buf;
}
static char* DefeatListValColor(int index)
{
    static char buf[32];
    sprintf(buf, "ValColor%i", index);
    return buf;
}

void DefeatCounterSetComplete(NUMBER idx)
{
    STRING reward = VarGetArrayElement("RewardList", idx);
    STRING rewardFloat = VarGetArrayElement("RewardFloatList", idx);
    int numLeft = VarGetNumber("NumToKill") - 1;

    // give any rewards
    if (!StringEmpty(reward) &&    !StringEqual("None", reward))
        MissionGrantReward(reward);

    // send any floats
    if (!StringEmpty(rewardFloat) && !StringEqual("None", rewardFloat))
        SendPlayerAttentionMessage(ALL_PLAYERS, rewardFloat);

    // clean up UI
    if (VarGetArrayElement("RemoveOnCompleteList", idx) != NULL && StringToNumber(VarGetArrayElement("RemoveOnCompleteList", idx)))
        ScriptUIHide(DefeatListUIName(idx), ALL_PLAYERS);

    // complete any objectives
    if (VarGetArrayElement("ObjectiveList", idx) != NULL && !StringEqual("", VarGetArrayElement("ObjectiveList", idx)))
        SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGetArrayElement("ObjectiveList", idx));

    // Are we done?
    VarSetNumber("NumToKill", numLeft);
}



NUMBER DefeatCounterCountVillainDefs(STRING def)
{
    int i, count = 0;
    for (i = 0; i < NumEntitiesInTeam(ALL_CRITTERS); i++)
    {    
        ENTITY targetEnt = GetEntityFromTeam(ALL_CRITTERS, i + 1 );

        if (StringEqual(def, GetVillainDefName(targetEnt)))
            count++;
    }
    return count;
}


void DefeatCounterUpdateUI()
{
    int i, count = VarGetArrayCount("TargetList");

    if( !VarIsEmpty("DoNotShowUI") && VarIsEmpty("ShowObjectives"))
        return;

    if (VarGetNumber("UpdateUI") == 0)
        return;

    VarSetNumber("UpdateUI", 0);

    for (i = 0; i < count; i++) 
    {
        char objective[50];
        int objectiveComplete;
        int groupSize;
        int DefeatCount = StringToNumber(VarGetArrayElement("DefeatCountList", i));
        int RemainCount = StringToNumber(VarGetArrayElement("RemainCountList", i));
        int DefeatCountSize = 0;
        int RemainCountSize = 0;

        // checking for overrides for count based on group size when on mission maps  
        if (OnMissionMap())
        {
            groupSize = MissionNumHeroes();
            sprintf(objective, "DefeatCountList%d", groupSize);
            DefeatCountSize = StringToNumber(VarGetArrayElement(objective, i));
            if (DefeatCountSize)
                DefeatCount = DefeatCountSize;
            sprintf(objective, "RemainCountList%d", groupSize);
            RemainCountSize = StringToNumber(VarGetArrayElement(objective, i));
            if (RemainCountSize)
                RemainCount = RemainCountSize;
        }

            
        if (DefeatCount > 0 && RemainCount >= 0) {
            printf( "DefeatCounter:Target has both Defeat and Remain counter" );
            EndScript();
        }

        if (StringEqual(VarGetArrayElement("DisplayTextList", i), "None"))
            continue;

        sprintf(objective, "Objective%d", i);
        objectiveComplete = VarGetNumber(objective);

        if (DefeatCount > 0)
        {
            char killCountName[50];
            sprintf(killCountName, "Killcount%d", i);
            VarSetNumber(DefeatListValName(i), DefeatCount - VarGetNumber(killCountName));
            VarSet(DefeatListEnemyColor(i), objectiveComplete?"999999ff":"ffffffff");
            VarSet(DefeatListValColor(i), objectiveComplete?"999999ff":"ffffffff");
        }
        else if (RemainCount >= 0)
        {
            VarSet(DefeatListEnemyColor(i), objectiveComplete?"999999ff":"ffffffff");
            VarSet(DefeatListValColor(i), objectiveComplete?"999999ff":"ffffffff");
            VarSetNumber(DefeatListValName(i), DefeatCounterCountVillainDefs(VarGetArrayElement("TargetList", i)));
        }

        if (!VarIsEmpty("ShowObjectives") && !objectiveComplete)
        {
            if (!StringEmpty(VarGetArrayElement("ObjectiveList", i)) && 
                !StringEqual("None", VarGetArrayElement("ObjectiveList", i))) 
            {
                STRING desc = NULL;

                if (DefeatCount > 0) 
                {
                    char killCountName[50];
                    sprintf(killCountName, "Killcount%d", i);
                    desc = StringAdd(NumberToString(DefeatCount - VarGetNumber(killCountName)), " ");
                } 
                else if (RemainCount >= 0)
                {
                    desc = StringAdd(NumberToString(DefeatCounterCountVillainDefs(VarGetArrayElement("TargetList", i))), " ");
                }
                desc = StringAdd(desc, StringLocalize(VarGetArrayElement("DisplayTextList", i)));
                SetNamedObjectiveString(VarGetArrayElement("ObjectiveList", i), desc, desc);
            }
        }
    }
}

void DefeatCounterOnEntityDefeat(ENTITY player, ENTITY victor)
{
    // check for desired type of critter
    if (IsEntityCritter(player))
    {
        STRING name = GetVillainDefName(player);
        int i, count = VarGetArrayCount("TargetList");
        char objective[50];

        for (i = 0; i < count; i++) 
        {
            if (StringEqual(name, VarGetArrayElement("TargetList", i)))
            {
                int DefeatCount = StringToNumber(VarGetArrayElement("DefeatCountList", i));
                int RemainCount = StringToNumber(VarGetArrayElement("RemainCountList", i));
                int groupSize;
                int DefeatCountSize = 0;
                int RemainCountSize = 0;

                // checking for overrides for count based on group size when on mission maps  
                if (OnMissionMap()) 
                {
                    groupSize = MissionNumHeroes();
                    sprintf(objective, "DefeatCountList%d", groupSize);
                    DefeatCountSize = StringToNumber(VarGetArrayElement(objective, i));
                    if (DefeatCountSize)
                        DefeatCount = DefeatCountSize;
                    sprintf(objective, "RemainCountList%d", groupSize);
                    RemainCountSize = StringToNumber(VarGetArrayElement(objective, i));
                    if (RemainCountSize)
                        RemainCount = RemainCountSize;
                }

                sprintf(objective, "Objective%d", i);

                if (VarGetNumber(objective) == 0) 
                {
                    if (DefeatCount > 0 )
                    {
                        int killCount;
                        char killCountName[50];

                        sprintf(killCountName, "Killcount%d", i);
                        killCount = VarGetNumber(killCountName) + 1;
                        VarSetNumber(killCountName, killCount);
                        if (killCount >= DefeatCount)
                        {
                            DefeatCounterSetComplete(i);
                            VarSetNumber(objective, 1); // Can only complete each objective set once
                        }
                    } else if (RemainCount >= 0) {
                        int remainCount = DefeatCounterCountVillainDefs(VarGetArrayElement("TargetList", i));

                        if (remainCount <= RemainCount)
                        {
                            DefeatCounterSetComplete(i);
                            VarSetNumber(objective, 1); // Can only complete each objective set once
                        }
                    }
                    // force UI Update
                    VarSetNumber("UpdateUI", 1);
                }
            }
        }
    }
}


void DefeatCounter(void)
{  
    int i, count = VarGetArrayCount("TargetList");
    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            // only on mission maps
            if (!OnMissionMap())
                EndScript();


            //////////////////////////////////////////////////////////////////////////
            ///Script Hooks

            // count number of deaths
            OnEntityDefeated ( DefeatCounterOnEntityDefeat );

            //////////////////////////////////////////////////////////////////////////
            // setup UI
            if( VarIsEmpty("DoNotShowUI") )
            {
                ScriptUITitle(ONENTER("TitleUI"), "DisplayTitle", "DisplayTooltip");
                // Create a List for each enemy in the target list
                // Take a look at this later because it could be done better i think
                for (i = 0; i < count; i++)
                {
                    ScriptUIList(ONENTER(DefeatListUIName(i)), "DisplayTooltip", DefeatListEnemyName(i), DefeatListEnemyColor(i), DefeatListValName(i), DefeatListValColor(i));
                    VarSet(DefeatListEnemyName(i), VarGetArrayElement("DisplayTextList", i));
                }
            }
            VarSetNumber("NumToKill", count);

            // setting up objectives if they want the kill counts to be displayed as mission objectives
            if (!VarIsEmpty("ShowObjectives"))
            {
                for (i = 0; i < count; i++)
                {
                    if (!StringEmpty(VarGetArrayElement("ObjectiveList", i)) && 
                        !StringEqual("None", VarGetArrayElement("ObjectiveList", i))) 
                    {
                        CreateMissionObjective(VarGetArrayElement("ObjectiveList", i), NULL, NULL);
                    }
                }
            }

            VarSetNumber("UpdateUI", 1);
            DefeatCounterUpdateUI();
        }
        END_ENTRY

        if (DoEvery("CountingStart", 0.25f, NULL))
        {
            SET_STATE("Counting");
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    STATE( "Counting" ) ////////////////////////////////////////////////////////
    {

        // force an update every so often to catch any player triggered spawns
        if (DoEvery("CountingStart", 0.1f, NULL))
        {
             VarSetNumber("UpdateUI", 1);
        }

        DefeatCounterUpdateUI();

        // cleanup when mission is over
        if( (MissionIsComplete() && !VarGetNumber("DoNotCompleteOnMissionEnd")) || !VarGetNumber("NumToKill"))
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
}


void DefeatCounterInit()
{
    SetScriptName( "DefeatCounter" );
    SetScriptFunc( DefeatCounter );
    SetScriptType( SCRIPT_MISSION );        
    
    SetScriptVar( "TargetList",                    "",                            0 );
    SetScriptVar( "DefeatCountList",            "",                            SP_OPTIONAL );
    SetScriptVar( "DefeatCountList1",            "",                            SP_OPTIONAL );
    SetScriptVar( "DefeatCountList2",            "",                            SP_OPTIONAL );
    SetScriptVar( "DefeatCountList3",            "",                            SP_OPTIONAL );
    SetScriptVar( "DefeatCountList4",            "",                            SP_OPTIONAL );
    SetScriptVar( "DefeatCountList5",            "",                            SP_OPTIONAL );
    SetScriptVar( "DefeatCountList6",            "",                            SP_OPTIONAL );
    SetScriptVar( "DefeatCountList7",            "",                            SP_OPTIONAL );
    SetScriptVar( "DefeatCountList8",            "",                            SP_OPTIONAL );
    SetScriptVar( "RemainCountList",            "",                            SP_OPTIONAL );
    SetScriptVar( "RemainCountList1",            "",                            SP_OPTIONAL );
    SetScriptVar( "RemainCountList2",            "",                            SP_OPTIONAL );
    SetScriptVar( "RemainCountList3",            "",                            SP_OPTIONAL );
    SetScriptVar( "RemainCountList4",            "",                            SP_OPTIONAL );
    SetScriptVar( "RemainCountList5",            "",                            SP_OPTIONAL );
    SetScriptVar( "RemainCountList6",            "",                            SP_OPTIONAL );
    SetScriptVar( "RemainCountList7",            "",                            SP_OPTIONAL );
    SetScriptVar( "RemainCountList8",            "",                            SP_OPTIONAL );
    SetScriptVar( "RewardList",                    "",                            SP_OPTIONAL );
    SetScriptVar( "RewardFloatList",            "",                            SP_OPTIONAL );
    SetScriptVar( "ObjectiveList",                "",                            SP_OPTIONAL );
    SetScriptVar( "RemoveOnCompleteList",        "",                            SP_OPTIONAL );
    SetScriptVar( "DisplayKillCountList",        "",                            SP_OPTIONAL ); //Unused
    SetScriptVar( "DisplayRemainCountList",        "",                            SP_OPTIONAL ); //Unused
    SetScriptVar( "DisplayTextList",            "",                            SP_OPTIONAL );
    SetScriptVar( "DoNotShowUI",                "",                            SP_OPTIONAL );
    SetScriptVar( "DoNotCompleteOnMissionEnd",    0,                            SP_OPTIONAL );
    SetScriptVar( "ShowObjectives",                "",                            SP_OPTIONAL );
    SetScriptVar( "DisplayTitle",                "",                            SP_OPTIONAL );
    SetScriptVar( "DisplayTooltip",                "",                            SP_OPTIONAL );

}
