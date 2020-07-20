// MISSION SCRIPT
// Encounter Script
//
#include "script/scriptutil.h"
#include "storyarc/missionobjective.h"
#include <utilitieslib/assert/assert.h>

// Called when the player enters the map - set their gurney mode as appropriate
static int MissionTeleportOnEnterMap(ENTITY player)
{
    NUMBER glowieActivated;
    STRING gurneyMode;
    STRING myTeleportHelperName;
    ENTITY myTeleportHelper;
    LOCATION aboveTheClouds = "coord:0.0,5000.0,0.0";    // 0, 5,000, 0 - place this 5000 feet above the origin

    glowieActivated = VarGetNumber("GlowieActivated");
    gurneyMode = glowieActivated ? VarGet("GurneyModeActive") : VarGet("GurneyModeInit");
    VarSet(EntVar(player, "GurneyMode"), gurneyMode);
    SetScriptTeam(player, "Mapteam");    

    // Check and see if I actually have a teleport helper yet
    myTeleportHelper = VarGet(EntVar(player, "TeleportHelper"));
    if (StringEqual(myTeleportHelper, ""))
    {
        // If not ...
        // Compose my port helper entity name from my name
        myTeleportHelperName = StringAdd("Teleporthelper_", EntityName(player));
        // Make my helper
        myTeleportHelper = CreateVillain("TeleportHelpers", "Objects_Script_Teleporter", 50, myTeleportHelperName, aboveTheClouds);
        // and save his name.  This is technically wrong, I should call EntityName.  The problem is that there does not appear to be an inverse
        // of this function, i.e. something that takes a STRING entityName and converts it back to an ENTITY.  If this becomes a major issue,
        // A fallback plan would be to craft something ourselves that searches the entire TeleportHelpers team, looking for a match.
        VarSet(EntVar(player, "TeleportHelper"), myTeleportHelper);
    }

    return 0;
}

// Called when something is defeated.  We hook this to deterct player death and 
// override the map prison gurney if needed.
static void MissionTeleportOnEntityDefeat(ENTITY player, ENTITY victor)
{
    STRING gurneyMode;
    NUMBER overridePrisonGurney;

    if (IsEntityPlayer(player))
    {
        gurneyMode = VarGet(EntVar(player, "GurneyMode"));
        overridePrisonGurney = stricmp(gurneyMode, "Hospital") == 0;
        OverridePrisonGurney(player, overridePrisonGurney);
    }
}

// Called when the player teleports due to the mission port power going off
static int MissionTeleportOnActivateTeleport(ENTITY player)
{
    LOCATION loc;

    loc = VarGet(EntVar(player, "TeleportTarget"));
    TeleportToLocation(player, loc);
    return 0;
}

// Called when the player initiates the glowie interaction.  It checks to see if 
// all members of the team are on the mission map.
static int MissionTeleportGlowieInteract(ENTITY player, ENTITY target)
{
    if (VarGetNumber("GlowieActivated"))
    {
        return false;
    }
    else if (AreAllTeamMembersPresent(player))
    {
        return false;
    }
    // Should I use SendPlayerSystemMessage(...); ?
    SendPlayerDialog(player, VarGet("CantActivate"));
    return true;
}
// Called when the player has successfully completed the glowie interact timer.
// First time ever it ports evertyone to the target.
static int MissionTeleportGlowieComplete(ENTITY player, ENTITY glowie)
{
    STRING target;
    STRING gurneyMode;
    STRING objectiveName;
    ENTITY currPlayer;
    ENTITY helper;
    STRING teleportPower = "Objects.Script_Teleport.Script_Teleport";
    int teamSize;
    int i;

    target =  VarGet("Target");
    gurneyMode = VarGet("GurneyModeUsed");

    if (VarGetNumber("GlowieActivated"))
    {
        // Second and subsequent times, just port the player themselves, using the port power
        VarSet(EntVar(player, "TeleportTarget"), target);
        VarSet(EntVar(player, "GurneyMode"), gurneyMode);
        helper = VarGet(EntVar(player, "TeleportHelper"));
        EntityActivatePower(helper, player, teleportPower);
    }
    else if (AreAllTeamMembersPresent(player))
    {
        // Everyone's here ...
        teamSize = NumEntitiesInTeam("MapTeam");
        for (i = 0; i < teamSize; i++)
        {
            currPlayer = GetEntityFromTeam("MapTeam", i + 1);    // +1 since GetEntityFromTeam() works 1 based
            // get the target and set the player's gurney mode
            VarSet(EntVar(currPlayer, "TeleportTarget"), target);
            VarSet(EntVar(currPlayer, "GurneyMode"), gurneyMode);
            // but only port if the teleport method says to do so.  This allows a value different from the default
            // "Power" (e.g. "CutScene") to override and let the cutscene or some other agent, do the heavy lifting.
            if (StringEqual(VarGet("TeleportMethod"), "Power"))
            {
                helper = VarGet(EntVar(currPlayer, "TeleportHelper"));
                EntityActivatePower(helper, currPlayer, teleportPower);
            }
        }
        VarSetNumber("GlowieActivated", 1);
        // Trigger an objective if requested.
        objectiveName = VarGet("ObjectiveName");
        if (objectiveName && objectiveName[0])
        {
            SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, objectiveName);
        }
    }
    return true;
}

// Handle a message
static int MissionTeleportHandleMessage(STRING message)
{
    int teamSize;
    int i;
    ENTITY currPlayer;

    if (StringEqual(message, "MovePlayers"))
    {
        teamSize = NumEntitiesInTeam("MapTeam");
        for (i = 0; i < teamSize; i++)
        {
            currPlayer = GetEntityFromTeam("MapTeam", i + 1);    // +1 since GetEntityFromTeam() works 1 based
            // We have to teleport people one at a time, since this relies on an entity variable
            MissionTeleportOnActivateTeleport(currPlayer);
        }
    }
    else if (StringEqual(message, "DestroyPets"))
    {
        ScriptDestroyAllPets("MapTeam");
    }
    return true;
}

void MissionTeleportGlowie(void)
{  
    ENTITY glowie;
    GLOWIEDEF teleportGlowie;
    STRING glowieMarker;
    STRING glowieRequires;
    STRING teleportTarget;

    INITIAL_STATE 
    {

        ON_ENTRY
        {
            VarSetNumber("GlowieActivated", 0);
            OnPlayerEnteringMap(MissionTeleportOnEnterMap);
            OnEntityDefeated(MissionTeleportOnEntityDefeat);
            OnPlayerMissionPort(MissionTeleportOnActivateTeleport);
            OnScriptMessage(MissionTeleportHandleMessage);

            teleportGlowie = GlowieCreateDef(StringLocalize(VarGet("ControlName")), 
                                            VarGet("ControlModel"), 
                                            VarGet("ControlInteract"), VarGet("ControlInterrupt"), 
                                            VarGet("ControlComplete"), VarGet("ControlInteractBar"), 
                                            VarGetNumber("ControlInteractTime"));
            GlowieSetDescriptions(teleportGlowie, NULL, NULL);
            glowieRequires = VarGet("ControlRequires");
            if (!StringEmpty(glowieRequires))
            {
                GlowieSetActivateRequires(teleportGlowie, glowieRequires);
            }
            glowieMarker = StringAdd("marker:", VarGet("TeleportGlowie"));
            teleportTarget = StringAdd("marker:", VarGet("TeleportTarget"));

            glowie = GlowiePlace(teleportGlowie, glowieMarker, true, MissionTeleportGlowieInteract, MissionTeleportGlowieComplete);
            SetScriptTeam(glowie, "glowies");
            VarSet("Target", teleportTarget);
        }
        END_ENTRY

        {
        }
    }
    END_STATES 
}

void MissionTeleportGlowieInit()
{
    SetScriptName( "MissionTeleportGlowie" );     
    SetScriptFunc( MissionTeleportGlowie );
    SetScriptType( SCRIPT_MISSION );        

    SetScriptVar( "TeleportGlowie",                    "",            0 ); 
    SetScriptVar( "TeleportTarget",                    "",            0 ); 
    SetScriptVar( "TeleportMethod",                    "Power",    SP_OPTIONAL ); 
    SetScriptVar( "GurneyModeInit",                    "Hospital",    SP_OPTIONAL ); 
    SetScriptVar( "GurneyModeActive",                "Prison",    SP_OPTIONAL ); 
    SetScriptVar( "GurneyModeUsed",                    "Prison",    SP_OPTIONAL ); 
    SetScriptVar( "ObjectiveName",                    "",            SP_OPTIONAL ); 

    SetScriptVar( "ControlName",                    NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "ControlModel",                    NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "ControlInteract",                NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "ControlInterrupt",                NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "ControlComplete",                NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "ControlInteractBar",                NULL,        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "ControlInteractTime",            NULL,        SP_DONTDISPLAYDEBUG );

    SetScriptVar( "ControlRequires",                NULL,        SP_DONTDISPLAYDEBUG | SP_OPTIONAL );

    SetScriptVar( "GlowieActivated",                NULL,        SP_OPTIONAL );
}

