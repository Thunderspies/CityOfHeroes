// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"


void PvPMission(void)
{  
    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
        }
        END_ENTRY

        if( MissionIsComplete() )
        {
            SET_STATE( "MissionOver" );
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    STATE( "MissionOver" ) ////////////////////////////////////////////////////////
    {
        if (MissionIsSuccessful())
        {
            STRING varname, name;
            int count;

            varname = StringAdd(VarGet("Map"), VarGet("Type"));
            varname = StringAdd(varname, VarGet("Side"));
            varname = StringAdd(varname, VarGet("Direction"));

            //////////////////////////////////////////
            // update variables

            // add player's name
            count = 0;
            while (count < 5) {
                name = StringAdd(varname, "Name");
                name = StringAdd(name, NumberToString(count));
                if (MapHasDataToken(name)) {
                    count++;
                } else {
                    MapSetDataToken(name, MissionOwnerDBID());
                    break;
                }
            }

            // increment count
            if (MapHasDataToken(varname)) {
                count = MapGetDataToken(varname) + 1;
            } else {
                count = 1;
            }
            MapSetDataToken(varname, count); 

        }
        EndScript();
    }

    END_STATES 
}


void PvPMissionInit()
{
    SetScriptName( "PvPMission" );
    SetScriptFunc( PvPMission );
    SetScriptType( SCRIPT_MISSION );        
    
    SetScriptVar( "Type",                "Dam",                        0 );
    SetScriptVar( "Side",                "H",                        0 );
    SetScriptVar( "Direction",            "B",                        0 );
    SetScriptVar( "Map",                "None",                        0 );

    //    SetScriptVar( "WaveShout",                "<shout>OOOOOO",            SP_MULTIVALUE | SP_OPTIONAL );
}
