// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"

void ChattyBoss(void)
{  
    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY  //////////////////////////////////////////////////////////////////////////
        {
            ENTITY boss;
            
            if( 0 == stricmp( "", VarGet( "BOSS_1STBLOOD_DIALOG" ) ) &&
                0 == stricmp( "", VarGet( "BOSS_75HEALTH_DIALOG" ) ) &&
                0 == stricmp( "", VarGet( "BOSS_50HEALTH_DIALOG" ) ) &&
                0 == stricmp( "", VarGet( "BOSS_25HEALTH_DIALOG" ) ) &&
                0 == stricmp( "", VarGet( "BOSS_DEAD_DIALOG" ) ) &&
                0 == stricmp( "", VarGet( "BOSS_HELD_DIALOG" ) ) &&
                0 == stricmp( "", VarGet( "BOSS_FREED_DIALOG" ) ) &&
                0 == stricmp( "", VarGet( "BOSS_AFRAID_DIALOG" ) ) &&
                0 == stricmp( "", VarGet( "BOSS_75HEALTH_Objective" ) ) &&
                0 == stricmp( "", VarGet( "BOSS_50HEALTH_Objective" ) ) &&
                0 == stricmp( "", VarGet( "BOSS_25HEALTH_Objective" ) ) &&
                0 == stricmp( "", VarGet( "BOSS_DEAD_Objective" ) ) &&
                0 == stricmp( "", VarGet( "BOSS_KILLEDHERO_DIALOG" ) ) )
                EndScript();

            boss = ActorFromEncounterActorName(MyEncounter(), VarGet( "Actor" ) );

            if( !boss || StringEqual(boss, TEAM_NONE))
                EndScript();


            SayOnKillHero(boss, VarGet("BOSS_KILLEDHERO_DIALOG") );

            VarSet( "ChattyBoss", boss );
            VarSetFraction( "LastHealth", GetHealth( boss ) );
        }
        END_ENTRY

        {
            ENTITY boss = VarGet( "ChattyBoss" );
            FRACTION health = GetHealth( VarGet( "ChattyBoss" ) );
            FRACTION lastHealth = VarGetFraction( "LastHealth" );
 
            if (( lastHealth >= 1.00 && 1.00 > health ) && !VarGetNumber( "Done1stBlood" ) )
            {
                Say( boss, VarGet( "BOSS_1STBLOOD_DIALOG" ), 2 ); 
                VarSetNumber( "Done1stBlood", 1 ); 
            }
            if (( lastHealth > 0.75 && 0.75 >= health ) && !VarGetNumber( "Done75Health" ) )
            {
                Say( boss, VarGet( "BOSS_75HEALTH_DIALOG" ), 2 ); 
                VarSetNumber( "Done75Health", 1 ); 
                if (!StringEqual(VarGet("BOSS_75HEALTH_Objective"), ""))
                    SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGet("BOSS_75HEALTH_Objective"));
                if (VarIsTrue("BOSS_75HEALTH_FORCE_HEALTH"))
                    health = SetHealth( VarGet( "ChattyBoss" ), 0.75, 0);
            }
            if (( lastHealth > 0.50 && 0.50 >= health ) && !VarGetNumber( "Done50Health" ) )
            {
                Say( boss, VarGet( "BOSS_50HEALTH_DIALOG" ), 2 ); 
                VarSetNumber( "Done50Health", 1 ); 
                if (!StringEqual(VarGet("BOSS_50HEALTH_Objective"), ""))
                    SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGet("BOSS_50HEALTH_Objective"));
                if (VarIsTrue("BOSS_50HEALTH_FORCE_HEALTH"))
                    health = SetHealth( VarGet( "ChattyBoss" ), 0.50, 0);
            }
            if (( lastHealth > 0.25 && 0.25 >= health ) && !VarGetNumber( "Done25Health" ) )
            {
                Say( boss, VarGet( "BOSS_25HEALTH_DIALOG" ), 2 ); 
                VarSetNumber( "Done25Health", 1 ); 
                if (!StringEqual(VarGet("BOSS_25HEALTH_Objective"), ""))
                    SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGet("BOSS_25HEALTH_Objective"));
                if (VarIsTrue("BOSS_25HEALTH_FORCE_HEALTH"))
                    health = SetHealth( VarGet( "ChattyBoss" ), 0.25, 0);
            }
            if( lastHealth > 0.00 && 0.00 >= health )//&& !VarGetNumber( "DoneDead" ) )
            {
                Say( boss, VarGet( "BOSS_DEAD_DIALOG" ), 2 ); 
                VarSetNumber( "DoneDead", 1 ); 
                if (!StringEqual(VarGet("BOSS_DEAD_Objective"), ""))
                    SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, VarGet("BOSS_DEAD_Objective"));
            }
            
            //TO DO
            //!VarGet( "BOSS_HELD_DIALOG" ) &&
            //    !VarGet( "BOSS_FREED_DIALOG" ) &&
            //    !VarGet( "BOSS_AFRAID_DIALOG" ) &&
            //    !VarGet( "BOSS_KILLEDHERO_DIALOG" ) )

            VarSetFraction( "LastHealth", health );
        }
    }
    END_STATES 
}

void ChattyBossInit()
{
    SetScriptName( "ChattyBoss" );     
    SetScriptFunc( ChattyBoss );
    SetScriptType( SCRIPT_ENCOUNTER );        

    SetScriptVar( "Actor",                                "",        0 );
    SetScriptVar( "BOSS_75HEALTH_DIALOG",                "",        SP_MULTIVALUE | SP_OPTIONAL ); 
    SetScriptVar( "BOSS_75HEALTH_Objective",            "",        SP_OPTIONAL ); 
    SetScriptVar( "BOSS_50HEALTH_DIALOG",                "",        SP_MULTIVALUE | SP_OPTIONAL ); 
    SetScriptVar( "BOSS_50HEALTH_Objective",            "",        SP_OPTIONAL ); 
    SetScriptVar( "BOSS_25HEALTH_DIALOG",                "",        SP_MULTIVALUE | SP_OPTIONAL ); 
    SetScriptVar( "BOSS_25HEALTH_Objective",            "",        SP_OPTIONAL ); 
    SetScriptVar( "BOSS_DEAD_DIALOG",                    "",        SP_MULTIVALUE | SP_OPTIONAL ); 
    SetScriptVar( "BOSS_DEAD_Objective",                "",        SP_OPTIONAL ); 
    SetScriptVar( "BOSS_KILLEDHERO_DIALOG",                "",        SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "BOSS_1STBLOOD_DIALOG",                "",        SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "BOSS_75HEALTH_FORCE_HEALTH",            "",        SP_OPTIONAL );
    SetScriptVar( "BOSS_50HEALTH_FORCE_HEALTH",            "",        SP_OPTIONAL );
    SetScriptVar( "BOSS_25HEALTH_FORCE_HEALTH",            "",        SP_OPTIONAL );

//    SetScriptSignal( "Remove Leveler", "EndScript" );
}

