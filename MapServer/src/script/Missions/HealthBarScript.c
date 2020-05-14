// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

#define MAX_HEALTH_DISPLAY 10
int HealthBarListOnLeaveMap(ENTITY player)
{
    ScriptUIHide("Collection:HealthBarList", player);
    return 1;
}
void HealthBarListScript()
{
    INITIAL_STATE 
    {
        ON_ENTRY
        {
            int i;
            int count = 0;
            for (i = 0; i < MAX_HEALTH_DISPLAY; ++i)
            {
                STRING currentStr = StringAdd("HealthBarEncounter", NumberToString(i));
                STRING enc = VarGet(currentStr);
                if (stricmp(enc, ""))        //    if there is anything defined here
                {
                    ENCOUNTERGROUP group = FindEncounter(0, 0, 0, 0, enc, 0, 0, 0);
                    VarSet(StringAdd(currentStr, "group"), group);
                    count++;
                }
                else
                {
                    break;
                }
            }
            VarSetNumber("DefinedEntCount", count);
            OnPlayerExitingMap( HealthBarListOnLeaveMap );
        }
        END_ENTRY
        {
            int i;
            int num = VarGetNumber("DefinedEntCount");
            int activeCount = 0;

            //    find all existing ents
            for (i = 0; i < num; ++i)
            {
                STRING enc = StringAdd("HealthBarEncounter", NumberToString(i));
                ENCOUNTERGROUP group = VarGet(StringAdd(enc, "group"));
                ENTITY ent = ActorFromEncounterPos(group, 1);
                if (EntityExists(ent))
                {
                    VarSet(StringAdd("HealthBarEntity", NumberToString(i)), ent);
                    activeCount++;
                }
                else
                {
                    VarSet(StringAdd("HealthBarEntity", NumberToString(i)), "");
                }
            }
            if (activeCount)
            {
                if (activeCount != VarGetNumber("LastActiveCount"))
                {
                    //    if new ents
                    //    clean out old ui and create new one
                    const char **widgetList = NULL;
                    ScriptUICleanup();
                    ScriptUITitle("TitleWidget", VarGet("HealthBarListTitle"), "");
                    ScriptUIList("ListWidget", "", VarGet("HealthBarListTitle"), "ffffffff", " ", "ffffffff");
                    eaPushConst(&widgetList, "TitleWidget");
                    eaPushConst(&widgetList, "ListWidget");
                    
                    for (i = 0; i < num; ++i)
                    {
                        STRING entStr = StringAdd("HealthBarEntity", NumberToString(i));
                        ENTITY ent = VarGet(entStr);
                        if (stricmp(ent, ""))
                        {
                            ScriptUIMeterEx(entStr, GetDisplayName(ent), GetDisplayName(ent), StringAdd(entStr, "Health"), "100", "Blue", "", "");
                            eaPushConst(&widgetList, entStr);
                        }
                    }
                    VarSetNumber("LastActiveCount", activeCount);
                    ScriptUICreateCollectionEx("HealthBarList", eaSize(&widgetList), widgetList);
                    eaDestroyConst(&widgetList);
                }

                //    show the ui
                ScriptUIShow("Collection:HealthBarList", ALL_PLAYERS);
                ScriptUISendDetachCommand(ALL_PLAYERS, 1);

                //    this has to be after the "show" command
                //    this sets the vars to dirty, which allows them to update on the client
                for (i = 0; i < num; ++i)
                {
                    STRING entStr = StringAdd("HealthBarEntity", NumberToString(i));
                    ENTITY ent = VarGet(entStr);
                    if (stricmp(ent, ""))
                    {
                        VarSetNumber(StringAdd(entStr, "Health"), GetHealth(ent) * 100);
                    }
                }
            }
            else
            {
                //    if nothing active, hide the ui
                ScriptUIHide("Collection:HealthBarList", ALL_PLAYERS);
            }
        }
    }
    END_STATES
}

void HealthBarListScriptInit()
{
    int i;
    SetScriptName( "HealthBarListScript" );     
    SetScriptFunc( HealthBarListScript );
    SetScriptType( SCRIPT_MISSION );

    SetScriptVar( "HealthBarListTitle",                    0,        0 );
    for (i = 0; i < MAX_HEALTH_DISPLAY; ++i)
    {
        SetScriptVar( StringAdd("HealthBarEncounter", NumberToString(i)),    0,        SP_OPTIONAL );
    }
}
#undef MAX_HEALTH_DISPLAY
