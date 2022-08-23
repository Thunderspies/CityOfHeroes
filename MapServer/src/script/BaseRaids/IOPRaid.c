// ZONE SCRIPT
//
// Runs the main IOP SG base raid

#include "script/scriptutil.h"
#include <utilitieslib/utils/timing.h>
#include "bases/bases.h"
#include "bases/basesystems.h"
#include "bases/basedata.h"
#include <utilitieslib/components/earray.h>
#include "gameComm/VillainDef.h"
#include "gameSys/sgraid.h"
#include <utilitieslib/utils/error.h>
#include "ai/entai.h"
#include "entity/entity.h"
#include "entity/entserver.h"
#include "ai/entaiScript.h"
#include "script/scriptengine.h"
#include "gameComm/dooranimcommon.h"
#include "svr/svr_player.h"
#include "gameSys/raidmapserver.h"
#include "entity/character_target.h"
#include "gameSys/sgraid_V2.h"

#define INITIAL_WAIT_DELAY    1.0        /* time in minutes for everyone to connect*/
#define WRAPUP_DELAY        1.0        /* time in minutes at end */

#define    FLAG_IN_BASE        0
#define    FLAG_BEING_CARRIED    1
#define    FLAG_ON_GROUND        2

#define    GLOWIE_NAME_LEN        24
#define    GLOWIE_POS_LEN        256

// TODO Fix these when we get the artwork.
#define IOP_RAID_SAFE_FLAGICON        "sp_icon_pillbox_hero"
#define IOP_RAID_LOOSE_FLAGICON        "sp_icon_pillbox_neutral"
#define IOP_RAID_STOLEN_FLAGICON    "sp_icon_pillbox_Villain"

static GLOWIEDEF BlueTeamIOP[MAX_SG_IOP];
static GLOWIEDEF RedTeamIOP[MAX_SG_IOP];
static GLOWIEDEF BlueTeamFlag;
static GLOWIEDEF RedTeamFlag;
static char BlueTeamIOPGlowie[MAX_SG_IOP][GLOWIE_NAME_LEN];
static char RedTeamIOPGlowie[MAX_SG_IOP][GLOWIE_NAME_LEN];
static char BlueTeamFlagGlowie[GLOWIE_NAME_LEN];
static char RedTeamFlagGlowie[GLOWIE_NAME_LEN];
//static char *BlueTeamIOPPos[MAX_SG_IOP];
//static char *RedTeamIOPPos[MAX_SG_IOP];

#define    WonTextWonColor            0x00ff00ff
#define    WonTextLostColor        0xff0000ff
#define    DrawTextColor            0xff8000ff
#define    TakenTextColor            0xff0000ff
#define    YouTookTextColor        0x00ff00ff
#define DroppedTextColor        0xff8000ff
#define TheyDroppedTextColor    0xffff00ff
#define CapturedTextColor        0xff0000ff
#define YouCapturedTextColor    0x00ff00ff
#define ReturnedTextColor        0x00ff00ff
#define TheyReturnedTextColor    0xff8000ff

static NUMBER StringToHex(STRING str)
{
    NUMBER n;

    if (!str || !str[0] || sscanf(str, "%x", &n) != 1)
    {
        return 0;
    }
    return n;
}

static void activateBlueGlowies()
{
    RemoveAllBehaviors("BlueGlowies");
    AddAIBehavior("BlueGlowies", "AnimClearBit(UP)");
    RemoveAllBehaviors("BlueGlowies");
    AddAIBehavior("BlueGlowies", "AnimBit(CAGE,DOWN)");
    printf("activateBlueGlowies\n");
}

static void activateRedGlowies()
{
    RemoveAllBehaviors("RedGlowies");
    AddAIBehavior("RedGlowies", "AnimClearBit(UP)");
    RemoveAllBehaviors("RedGlowies");
    AddAIBehavior("RedGlowies", "AnimBit(CAGE,DOWN)");
    printf("activateRedGlowies\n");
}

static void deactivateBlueGlowies()
{
    RemoveAllBehaviors("BlueGlowies");
    AddAIBehavior("BlueGlowies", "AnimClearBit(DOWN)");
    RemoveAllBehaviors("BlueGlowies");
    AddAIBehavior("BlueGlowies", "AnimBit(CAGE,UP)");
    printf("deactivateBlueGlowies\n");
}

static void deactivateRedGlowies()
{
    RemoveAllBehaviors("RedGlowies");
    AddAIBehavior("RedGlowies", "AnimClearBit(DOWN)");
    RemoveAllBehaviors("RedGlowies");
    AddAIBehavior("RedGlowies", "AnimBit(CAGE,UP)");
    printf("deactivateRedGlowies\n");
}

static void SetBlueFlagState(int state)
{
    char *statestr = NULL;

    switch (state)
    {
    default:
        devassert(0);
    xcase FLAG_IN_BASE:
        VarSet("BlueBlueIcon", IOP_RAID_SAFE_FLAGICON);
        VarSet("RedBlueIcon", IOP_RAID_STOLEN_FLAGICON);
        statestr = "FLAG_IN_BASE";
    xcase FLAG_BEING_CARRIED:
        VarSet("BlueBlueIcon", IOP_RAID_STOLEN_FLAGICON);
        VarSet("RedBlueIcon", IOP_RAID_SAFE_FLAGICON);
        statestr = "FLAG_BEING_CARRIED";
    xcase FLAG_ON_GROUND:
        VarSet("BlueBlueIcon", IOP_RAID_LOOSE_FLAGICON);
        VarSet("RedBlueIcon", IOP_RAID_LOOSE_FLAGICON);
        statestr = "FLAG_ON_GROUND";
    }
    VarSetNumber("BlueFlagState", state);
    printf("Blue flag new state: %s\n", statestr);
}

static void SetRedFlagState(int state)
{
    char *statestr = NULL;

    switch (state)
    {
    default:
        devassert(0);
    xcase FLAG_IN_BASE:
        VarSet("RedRedIcon", IOP_RAID_SAFE_FLAGICON);
        VarSet("BlueRedIcon", IOP_RAID_STOLEN_FLAGICON);
        statestr = "FLAG_IN_BASE";
    xcase FLAG_BEING_CARRIED:
        VarSet("RedRedIcon", IOP_RAID_STOLEN_FLAGICON);
        VarSet("BlueRedIcon", IOP_RAID_SAFE_FLAGICON);
        statestr = "FLAG_BEING_CARRIED";
    xcase FLAG_ON_GROUND:
        VarSet("RedRedIcon", IOP_RAID_LOOSE_FLAGICON);
        VarSet("BlueRedIcon", IOP_RAID_LOOSE_FLAGICON);
        statestr = "FLAG_ON_GROUND";
    }
    VarSetNumber("RedFlagState", state);
    printf("Red flag new state: %s\n", statestr);
}

// IOP glowie interact routines

// Initial interact route, called when player first clicks.  It's broken into two main sections. The first one is when the 
// player is clicking on their own teams IOP, the second is clicking on an enemy IOP
static void IOPRaidIOPInteract(ENTITY player, char *teamName)
{
    int i;
    int teamSize;
    int blueCaptures;
    int redCaptures;

    if (GetPlayerRaidID(player) == VarGetNumber(teamName))
    {
        // Clicking on your own IOP - successful capture
        if (strcmp(teamName, "BlueRaidID") == 0)
        {
            // Blue team capture
            // Note that [Color]FlagCarrier use the Color to specify the color of the player, not the flag
            if (!StringEqual(player, VarGet("BlueFlagCarrier")))
            {
                return;
            }
            devassert(VarGetNumber("RedFlagState") == FLAG_BEING_CARRIED);
            VarSet("BlueFlagCarrier", "");
            blueCaptures = VarGetNumber("BlueCaptures") + 1;
            VarSetNumber("BlueCaptures", blueCaptures);
            VarSet("BlueTeamUICaptureCount", NumberToString(blueCaptures));
            SendPlayerAttentionMessageWithColor("BlueTeam", VarGet("YouCapturedText"), YouCapturedTextColor, kFloaterStyle_Attention);
            SendPlayerAttentionMessageWithColor("RedTeam", VarGet("CapturedText"), CapturedTextColor, kFloaterStyle_Attention);
            EntityRemoveTempPower(player, "SilentPowers", "V_Ore_Exposure");
        }
        else
        {
            // Red team capture
            if (!StringEqual(player, VarGet("RedFlagCarrier")))
            {
                return;
            }
            devassert(VarGetNumber("BlueFlagState") == FLAG_BEING_CARRIED);
            VarSet("RedFlagCarrier", "");
            redCaptures = VarGetNumber("RedCaptures") + 1;
            VarSetNumber("RedCaptures", redCaptures);
            VarSet("RedTeamUICaptureCount", NumberToString(redCaptures));
            SendPlayerAttentionMessageWithColor("RedTeam", VarGet("YouCapturedText"), YouCapturedTextColor, kFloaterStyle_Attention);
            SendPlayerAttentionMessageWithColor("BlueTeam", VarGet("CapturedText"), CapturedTextColor, kFloaterStyle_Attention);
            EntityRemoveTempPower(player, "SilentPowers", "V_Ore_Exposure");
        }
        printf("Capture - reset\n");
        // By the time we get here, we know for certain that someone got a capture.
        // Turn everything back on
        activateBlueGlowies();
        activateRedGlowies();
        // Flag that flags are in bases
        SetBlueFlagState(FLAG_IN_BASE);
        SetRedFlagState(FLAG_IN_BASE);
        // Adjust this line to grant badge credit for a capture.
        teamName = strcmp(teamName, "BlueRaidID") == 0 ? "BlueTeam" : "RedTeam";
        teamSize = NumEntitiesInTeam(teamName);
        for (i = 0; i < teamSize; i++)
        {
            player = GetEntityFromTeam(teamName, i + 1);
            // Places we'll need to hit to make this work:
            // 1. enum ScriptBadgeType, line 216, script.h
            // 2. void GiveBadgeCredit( ENTITY player, ScriptBadgeType type ), line 1462, scripthook.c
            // 3. New routine in badges_server.h, see line 144 for an example
            //GiveBadgeCredit(player, kScriptBadgeTypeIOPCapture);
        }
    }
    else
    {
        printf("Theft - setup\n");
        // Opposing player trying to take the flag
        if (strcmp(teamName, "BlueRaidID") == 0)
        {
            // red player stealing blue flag
            devassert(VarGetNumber("BlueFlagState") == FLAG_IN_BASE);
            SetBlueFlagState(FLAG_BEING_CARRIED);
            // Note that [Color]FlagCarrier use the Color to specify the color of the player, not the flag
            VarSet("RedFlagCarrier", player);
            deactivateBlueGlowies();
            SendPlayerAttentionMessageWithColor("RedTeam", VarGet("YouTookText"), YouTookTextColor, kFloaterStyle_Attention);
            SendPlayerAttentionMessageWithColor("BlueTeam", VarGet("TakenText"), TakenTextColor, kFloaterStyle_Attention);
            EntityGrantTempPower(player, "SilentPowers", "V_Ore_Exposure");
        }
        else
        {
            // blue player stealing red flag
            devassert(VarGetNumber("RedFlagState") == FLAG_IN_BASE);
            SetRedFlagState(FLAG_BEING_CARRIED);
            // Note that [Color]FlagCarrier use the Color to specify the color of the player, not the flag
            VarSet("BlueFlagCarrier", player);
            deactivateRedGlowies();
            SendPlayerAttentionMessageWithColor("BlueTeam", VarGet("YouTookText"), YouTookTextColor, kFloaterStyle_Attention);
            SendPlayerAttentionMessageWithColor("RedTeam", VarGet("TakenText"), TakenTextColor, kFloaterStyle_Attention);
            EntityGrantTempPower(player, "SilentPowers", "V_Ore_Exposure");
        }
    }
}

// Wrapper routines for the above that set the clicked on raid ID correctly
// Because gameplay requires "insta-clickies", we always return true from these.  IOPRaidIOPInteract(ENTITY, char *) does all the heavy lifting
static int IOPRaidBlueTeamIOPInteract(ENTITY player, ENTITY target)
{
    if (StringEqual(GET_STATE, "Play"))
    {
        IOPRaidIOPInteract(player, "BlueRaidID");
    }
    return 1;
}

static int IOPRaidRedTeamIOPInteract(ENTITY player, ENTITY target)
{
    if (StringEqual(GET_STATE, "Play"))
    {
        IOPRaidIOPInteract(player, "RedRaidID");
    }
    return 1;
}

// Empty routines - all the action happens in the initial click routine
static int IOPRaidBlueTeamIOPComplete(ENTITY player, ENTITY target)
{
    return 1;
}

static int IOPRaidRedTeamIOPComplete(ENTITY player, ENTITY target)
{
    return 1;
}


// IOP glowie interact routines

static void IOPRaidFlagInteract(ENTITY player, char *teamName)
{
    if (GetPlayerRaidID(player) == VarGetNumber(teamName))
    {
        printf("Loose pickup - return\n");
        // Clicking on your own loose flag
        if (strcmp(teamName, "BlueRaidID") == 0)
        {
            devassert(VarGetNumber("BlueFlagState") == FLAG_ON_GROUND);
            SetBlueFlagState(FLAG_IN_BASE);
            activateBlueGlowies();
            GlowieRemove(BlueTeamFlagGlowie);
            SendPlayerAttentionMessageWithColor("BlueTeam", VarGet("ReturnedText"), ReturnedTextColor, kFloaterStyle_Attention);
            SendPlayerAttentionMessageWithColor("RedTeam", VarGet("TheyReturnedText"), TheyReturnedTextColor, kFloaterStyle_Attention);
        }
        else
        {
            devassert(VarGetNumber("RedFlagState") == FLAG_ON_GROUND);
            SetRedFlagState(FLAG_IN_BASE);
            activateRedGlowies();
            GlowieRemove(RedTeamFlagGlowie);
            SendPlayerAttentionMessageWithColor("RedTeam", VarGet("ReturnedText"), ReturnedTextColor, kFloaterStyle_Attention);
            SendPlayerAttentionMessageWithColor("BlueTeam", VarGet("TheyReturnedText"), TheyReturnedTextColor, kFloaterStyle_Attention);
        }
    }
    else
    {
        printf("Loose pickup - capture\n");
        // Clicking on the other teams loose flag
        if (strcmp(teamName, "BlueRaidID") == 0)
        {
            devassert(VarGetNumber("BlueFlagState") == FLAG_ON_GROUND);
            SetBlueFlagState(FLAG_BEING_CARRIED);
            VarSet("RedFlagCarrier", player);
            GlowieRemove(BlueTeamFlagGlowie);
            SendPlayerAttentionMessageWithColor("RedTeam", VarGet("YouTookText"), YouTookTextColor, kFloaterStyle_Attention);
            SendPlayerAttentionMessageWithColor("BlueTeam", VarGet("TakenText"), TakenTextColor, kFloaterStyle_Attention);
            EntityGrantTempPower(player, "SilentPowers", "V_Ore_Exposure");
        }
        else
        {
            devassert(VarGetNumber("RedFlagState") == FLAG_ON_GROUND);
            SetRedFlagState(FLAG_BEING_CARRIED);
            VarSet("BlueFlagCarrier", player);
            GlowieRemove(RedTeamFlagGlowie);
            SendPlayerAttentionMessageWithColor("BlueTeam", VarGet("YouTookText"), YouTookTextColor, kFloaterStyle_Attention);
            SendPlayerAttentionMessageWithColor("RedTeam", VarGet("TakenText"), TakenTextColor, kFloaterStyle_Attention);
            EntityGrantTempPower(player, "SilentPowers", "V_Ore_Exposure");
        }
    }
}

// Wrapper routines for the above that set the clicked on raid ID correctly
static int IOPRaidBlueTeamFlagInteract(ENTITY player, ENTITY target)
{
    if (StringEqual(GET_STATE, "Play"))
    {
        IOPRaidFlagInteract(player, "BlueRaidID");
    }
    return 1;
}

static int IOPRaidRedTeamFlagInteract(ENTITY player, ENTITY target)
{
    if (StringEqual(GET_STATE, "Play"))
    {
        IOPRaidFlagInteract(player, "RedRaidID");
    }
    return 1;
}

// Empty routines - all the action happens in the initial click routine
static int IOPRaidBlueTeamFlagComplete(ENTITY player, ENTITY target)
{
    return 1;
}

static int IOPRaidRedTeamFlagComplete(ENTITY player, ENTITY target)
{
    return 1;
}

// Create the glowie defs and glowies for the IOPs.
static void CreateIOPs()
{
    unsigned int i;
    char *model;
    ENTITY glowie;
    char buff[256];
    Mat4 mat;

    for (i = 0; i < MAX_SG_IOP; i++)
    {
        if (sgRaidGetBaseRaidIOPData(0, i, &model, mat))
        {
            devassert(strnicmp(model, "Base_", 5) == 0);
            devassert(strlen(model) < 200);
            sprintf(buff, "ItemOfPower_%s", &model[5]);

            BlueTeamIOP[i] = GlowieCreateDef(StringLocalize(VarGet("IOPName")), buff, // "Xmas_Gift",
                                                VarGet("IOPInteract"), VarGet("IOPInterrupt"), 
                                                VarGet("IOPComplete"), VarGet("IOPInteractBar"), 0);

            sprintf(buff, "matrix:%f,%f,%f/%f,%f,%f/%f,%f,%f/%f,%f,%f", mat[0][0], mat[0][1], mat[0][2], 
                                                                        mat[1][0], mat[1][1], mat[1][2],
                                                                        mat[2][0], mat[2][1], mat[2][2],
                                                                        mat[3][0], mat[3][1], mat[3][2]);
            // Hush.  We cheat and copy the string returned from GlowiePlace into a static buffer.  Saving the returned pointer
            // doesn't work, because this string doesn't appear to have quite the persistence I'd like.
            glowie = GlowiePlace(BlueTeamIOP[i], buff, true, IOPRaidBlueTeamIOPInteract, IOPRaidBlueTeamIOPComplete);
            strncpy(BlueTeamIOPGlowie[i], glowie, GLOWIE_NAME_LEN);
            BlueTeamIOPGlowie[i][GLOWIE_NAME_LEN - 1] = 0;
            // If this ever trips, it means we're no longer using the ent:%p format for entdefs.  As in a pointer takes > 16 characters to print.
            // Follow the breadcrumbs from GlowiePlace, via char *EntityNameFromEnt(e), you'll probably wind up in char *erGetRefString(Entity* ent) over in entityRef.c.
            // You need a big enough buffer for what erGetRefString returns, plus whatever else EntityNameFromEnt adds on.  24 (current value) should hold us for 32 and 64
            // bit versions.
            devassert(strlen(BlueTeamIOPGlowie[i]) < GLOWIE_NAME_LEN - 1);
            SetScriptTeam(glowie, "BlueGlowies");    
        }
        else
        {
            BlueTeamIOP[i] = NULL;
            BlueTeamIOPGlowie[i][0] = 0;
        }
        if (sgRaidGetBaseRaidIOPData(1, i, &model, mat))
        {
            devassert(strnicmp(model, "Base_", 5) == 0);
            devassert(strlen(model) < 200);
            sprintf(buff, "ItemOfPower_%s", &model[5]);

            RedTeamIOP[i] = GlowieCreateDef(StringLocalize(VarGet("IOPName")), buff, // "Xmas_Gift",
                                                VarGet("IOPInteract"), VarGet("IOPInterrupt"), 
                                                VarGet("IOPComplete"), VarGet("IOPInteractBar"), 0);

            sprintf(buff, "matrix:%f,%f,%f/%f,%f,%f/%f,%f,%f/%f,%f,%f", mat[0][0], mat[0][1], mat[0][2], 
                                                                        mat[1][0], mat[1][1], mat[1][2],
                                                                        mat[2][0], mat[2][1], mat[2][2],
                                                                        mat[3][0], mat[3][1], mat[3][2]);
            glowie = GlowiePlace(RedTeamIOP[i], buff, true, IOPRaidRedTeamIOPInteract, IOPRaidRedTeamIOPComplete);
            strncpy(RedTeamIOPGlowie[i], glowie, GLOWIE_NAME_LEN);
            RedTeamIOPGlowie[i][GLOWIE_NAME_LEN - 1] = 0;
            devassert(strlen(RedTeamIOPGlowie[i]) < GLOWIE_NAME_LEN - 1);
            SetScriptTeam(glowie, "RedGlowies");    
        }
        else
        {
            RedTeamIOP[i] = NULL;
            RedTeamIOPGlowie[i][0] = 0;
        }
    }
    BlueTeamFlag = GlowieCreateDef(StringLocalize(VarGet("DroppedIOPName")), "Xmas_Gift", // buf,
                                                VarGet("DroppedIOPInteract"), VarGet("DroppedIOPInterrupt"), 
                                                VarGet("DroppedIOPComplete"), VarGet("DroppedIOPInteractBar"), 0);
    BlueTeamFlagGlowie[0] = 0;
    RedTeamFlag = GlowieCreateDef(StringLocalize(VarGet("DroppedIOPName")), "Xmas_Gift", // buf,
                                                VarGet("DroppedIOPInteract"), VarGet("DroppedIOPInterrupt"), 
                                                VarGet("DroppedIOPComplete"), VarGet("DroppedIOPInteractBar"), 0);
    RedTeamFlagGlowie[0] = 0;
    // Because GLOWIEDEF's are ref counted, and tear themselves apart when the last GLOWIE created from them is removed,
    // we create a couple of instances of these guys way off in left field.  That keeps the DEF's valid when the "in base"
    // instances go away.
    GlowiePlace(BlueTeamFlag, "coord:100000.0,1000.0,100000.0", true, IOPRaidBlueTeamFlagComplete, IOPRaidBlueTeamFlagComplete);
    GlowiePlace(RedTeamFlag, "coord:100000.0,1000.0,100100.0", true, IOPRaidRedTeamFlagComplete, IOPRaidRedTeamFlagComplete);
}

static void IOPRaidOnEntityDefeat(ENTITY player, ENTITY victor)
{
    LOCATION flagPos;
    ENTITY glowie;

    if (IsEntityPlayer(player))
    {
        // Player killed another player - check for a flag drop
        // CreateVillainNearEntity - study this for placement if current code becomes a problem
        flagPos = PointFromEntity(player);
        if (StringEqual(player, VarGet("RedFlagCarrier")))
        {
            glowie = GlowiePlace(BlueTeamFlag, flagPos, true, IOPRaidBlueTeamFlagInteract, IOPRaidBlueTeamFlagComplete);
            strncpy(BlueTeamFlagGlowie, glowie, GLOWIE_NAME_LEN);
            BlueTeamFlagGlowie[GLOWIE_NAME_LEN - 1] = 0;
            devassert(strlen(BlueTeamFlagGlowie) < GLOWIE_NAME_LEN - 1);
            devassert(VarGetNumber("BlueFlagState") == FLAG_BEING_CARRIED);
            SetBlueFlagState(FLAG_ON_GROUND);
            VarSet("RedFlagCarrier", "");
            SendPlayerAttentionMessageWithColor("BlueTeam", VarGet("DroppedText"), DroppedTextColor, kFloaterStyle_Attention);
            SendPlayerAttentionMessageWithColor("RedTeam", VarGet("TheyDroppedText"), TheyDroppedTextColor, kFloaterStyle_Attention);
            EntityRemoveTempPower(player, "SilentPowers", "V_Ore_Exposure");
        }
        // CreateVillainNearEntity - study this for placement
        if (StringEqual(player, VarGet("BlueFlagCarrier")))
        {
            glowie = GlowiePlace(RedTeamFlag, flagPos, true, IOPRaidRedTeamFlagInteract, IOPRaidRedTeamFlagComplete);
            strncpy(RedTeamFlagGlowie, glowie, GLOWIE_NAME_LEN);
            RedTeamFlagGlowie[GLOWIE_NAME_LEN - 1] = 0;
            devassert(strlen(RedTeamFlagGlowie) < GLOWIE_NAME_LEN - 1);
            devassert(VarGetNumber("RedFlagState") == FLAG_BEING_CARRIED);
            SetRedFlagState(FLAG_ON_GROUND);
            VarSet("BlueFlagCarrier", "");
            SendPlayerAttentionMessageWithColor("RedTeam", VarGet("DroppedText"), DroppedTextColor, kFloaterStyle_Attention);
            SendPlayerAttentionMessageWithColor("BlueTeam", VarGet("TheyDroppedText"), TheyDroppedTextColor, kFloaterStyle_Attention);
            EntityRemoveTempPower(player, "SilentPowers", "V_Ore_Exposure");
        }
    }
}

// Much like the "on glowie click" routines, this returns zero if the teleport should happen
// one if not.
static int IOPRaidOnPlayerTeleport(ENTITY player)
{
    // Uncomment this if we decide that teleportatioon drops the flag
    //IOPRaidOnEntityDefeat(player, NULL);
    return 0;
}

// Remove "I'm carrying an IOP" buffs from players, destroy dropped flags, and turn IOPs
// back on in bases.
// TODO better cleanup of the UI
static void CleanupIOPs()
{
    ENTITY player;
    
    player = VarGet("BlueFlagCarrier");
    if (player[0])
    {
        EntityRemoveTempPower(player, "SilentPowers", "V_Ore_Exposure");
        VarSet("BlueFlagCarrier", "");
    }
    player = VarGet("RedFlagCarrier");
    if (player[0])
    {
        EntityRemoveTempPower(player, "SilentPowers", "V_Ore_Exposure");
        VarSet("RedFlagCarrier", "");
    }
    if (VarGetNumber("BlueFlagState") == FLAG_ON_GROUND)
    {
        GlowieRemove(BlueTeamFlagGlowie);
    }
    if (VarGetNumber("RedFlagState") == FLAG_ON_GROUND)
    {
        GlowieRemove(RedTeamFlagGlowie);
    }
    printf("CleanupIOPs\n");
    activateBlueGlowies();
    activateRedGlowies();
}

// Called when the player enters the map - set their gang ID to their raid ID
int IOPRaidOnEnterMap(ENTITY player)
{
    int raidID = GetPlayerRaidID(player);
    SetAIGang(player, raidID);

    if (raidID == VarGetNumber("BlueRaidID"))
    {
        SetScriptTeam(player, "BlueTeam");    
        ScriptUIShow("Collection:BlueTeamUI", player);
    }
    else if (raidID == VarGetNumber("RedRaidID"))
    {
        SetScriptTeam(player, "RedTeam");    
        ScriptUIShow("Collection:RedTeamUI", player);
    }
    else
    {
        devassertmsg(0, "Intruder detected on map");
    }
    if (StringEqual(GET_STATE, "WaitForPlayers"))
    {
        // Freeze the player - totally inactive
    }
    return 0;
}

// Called when the player leaves the map.  Remove the "I'm carrying an IOP" buff from the player.
// Drop the IOP at their location if they were carrying, reset their gang ID to zero, and remove them from
// whatever team they're in
int IOPRaidOnLeaveMap(ENTITY player)
{
    SetAIGang(player, 0);
    // "defeat" the player so the flag drops
    IOPRaidOnEntityDefeat(player, NULL);
    return 0;
}


void IOPRaid()
{
    int blueCaptures;
    int redCaptures;
    int blueRaidID;
    int redRaidID;

    INITIAL_STATE
        ON_ENTRY 
            VarSetNumber("BlueSGID", sgRaidGetBaseRaidSGID(0));
            VarSetNumber("RedSGID", sgRaidGetBaseRaidSGID(1));
            VarSet("BlueSGName", sgRaidGetBaseRaidSGName(0));
            VarSet("RedSGName", sgRaidGetBaseRaidSGName(1));
            blueRaidID = sgRaidGetBaseRaidRaidID(0);
            redRaidID = sgRaidGetBaseRaidRaidID(1);
            VarSetNumber("BlueRaidID", blueRaidID);
            VarSetNumber("RedRaidID", redRaidID);
            VarSetNumber("BlueCaptures", 0);
            VarSetNumber("RedCaptures", 0);
            VarSet("BlueFlagCarrier", "");
            VarSet("RedFlagCarrier", "");
            VarSet("AlwaysOn", "1");

            // Set up the UI
            VarSet("BlueTeamUICaptureTitle", StringAdd(/*VarGet("BlueSGName")*/ "Blue" , StringAdd(" ", StringLocalize(VarGet("CaptureText")))));
            VarSet("RedTeamUICaptureTitle", StringAdd(/*VarGet("RedSGName")*/ "Red" , StringAdd(" ", StringLocalize(VarGet("CaptureText")))));
            VarSet("BlueTeamUICaptureCount", "0");
            VarSet("RedTeamUICaptureCount", "0");
            //VarSet("BlueTeamUIFlagTitle", StringAdd(VarGet("BlueSGName"), StringAdd(" ", StringLocalize(VarGet("FlagText")))));
            //VarSet("RedTeamUIFlagTitle", StringAdd(VarGet("RedSGName"), StringAdd(" ", StringLocalize(VarGet("FlagText")))));

            ScriptUIList("UIBlueTeamCaptures", "", "BlueTeamUICaptureTitle", "ffffffff", "BlueTeamUICaptureCount", "ffffffff");
            ScriptUIList("UIRedTeamCaptures", "", "RedTeamUICaptureTitle", "ffffffff", "RedTeamUICaptureCount", "ffffffff");

            ScriptUICollectItems("UIBlueTeamCollection", 2, "AlwaysOn", "", /*VarGet("BlueTeamUIFlagTitle"),*/ "BlueBlueIcon",
                                                            "AlwaysOn", "", /*VarGet("RedTeamUIFlagTitle"),*/ "BlueRedIcon");

            ScriptUICollectItems("UIRedTeamCollection", 2,  "AlwaysOn", "", /*VarGet("RedTeamUIFlagTitle"),*/ "RedRedIcon",
                                                            "AlwaysOn", "", /*VarGet("BlueTeamUIFlagTitle"),*/ "RedBlueIcon");
            ScriptUITimer("UIGameTimer", "TimeLeft", "TimeRemaining", "");

            SetBlueFlagState(FLAG_IN_BASE);
            SetRedFlagState(FLAG_IN_BASE);

            ScriptUICreateCollection("BlueTeamUI", 4, "UIBlueTeamCollection", "UIGameTimer", "UIBlueTeamCaptures", "UIRedTeamCaptures");
            ScriptUICreateCollection("RedTeamUI", 4, "UIRedTeamCollection", "UIGameTimer", "UIRedTeamCaptures", "UIBlueTeamCaptures");

            CreateIOPs();
            OnPlayerEnteringMap(IOPRaidOnEnterMap);
            OnPlayerExitingMap(IOPRaidOnLeaveMap);
            OnEntityDefeated(IOPRaidOnEntityDefeat);
            OnPlayerTeleporting(IOPRaidOnPlayerTeleport);
            //OnPlayerEntersVolume();
        END_ENTRY

        // Need to change state immediately, so I can test if we're in this state
        SET_STATE("WaitForPlayers");

    STATE("WaitForPlayers")
        ON_ENTRY 
            TimerSet("Clock", INITIAL_WAIT_DELAY); // change this for maximum time we'll wait for everyone
            VarSetNumber("TimeLeft", (int) (INITIAL_WAIT_DELAY * 60.0f) + timerSecondsSince2000());
        END_ENTRY
        if (1 || TimerElapsed("Clock") /* || AllPlayersOnMap() */ )
        {
            // Unfreeze All Players
            SET_STATE("BuffDelay");
        }

    STATE("BuffDelay")
        ON_ENTRY 
            TimerSet("Clock", 0.25f); // This needs to be a parmeter - buff delay
            VarSetNumber("TimeLeft", (int) (0.25f * 60.0f) + timerSecondsSince2000());
        END_ENTRY

        if (TimerElapsed("Clock"))
        {
            SET_STATE("Play");
        }

    STATE("Play")
        ON_ENTRY
            TimerSet("Clock", 10.0f); // This needs to be a parmeter - game duration
            VarSetNumber("TimeLeft", 600 + timerSecondsSince2000());
        END_ENTRY

        if (TimerElapsed("Clock"))
        {
            blueCaptures = VarGetNumber("BlueCaptures");
            redCaptures = VarGetNumber("RedCaptures");
            if (blueCaptures > redCaptures)
            {
                SET_STATE("BlueTeamWin");
            }
            else if (redCaptures > blueCaptures)
            {
                SET_STATE("RedTeamWin");
            }
            else
            {
                SET_STATE("Draw");
            }
        }

    STATE("BlueTeamWin")
        SendPlayerAttentionMessageWithColor("BlueTeam", StringAdd(VarGet("BlueSGName"), StringAdd(" ", StringLocalize(VarGet("WonText")))), WonTextWonColor, kFloaterStyle_Attention);
        SendPlayerAttentionMessageWithColor("RedTeam", StringAdd(VarGet("BlueSGName"), StringAdd(" ", StringLocalize(VarGet("WonText")))), WonTextLostColor, kFloaterStyle_Attention);
        //EntityAddRaidPoints("BlueTeam", WinningPoints, 1);
        //EntityAddRaidPoints("RedTeam", LosingPoints, 1);
        SET_STATE("WrapupTime");

    STATE("RedTeamWin")
        SendPlayerAttentionMessageWithColor("RedTeam", StringAdd(VarGet("RedSGName"), StringAdd(" ", StringLocalize(VarGet("WonText")))), WonTextWonColor, kFloaterStyle_Attention);
        SendPlayerAttentionMessageWithColor("BlueTeam", StringAdd(VarGet("RedSGName"), StringAdd(" ", StringLocalize(VarGet("WonText")))), WonTextLostColor, kFloaterStyle_Attention);
        //EntityAddRaidPoints("RedTeam", WinningPoints, 1);
        //EntityAddRaidPoints("BlueTeam", LosingPoints, 1);
        SET_STATE("WrapupTime");
        

    STATE("Draw")
        SendPlayerAttentionMessageWithColor(ALL_PLAYERS, VarGet("DrawText"), DrawTextColor, kFloaterStyle_Attention);
        SET_STATE("WrapupTime");

    STATE("WrapupTime")
        ON_ENTRY
            TimerSet("Clock", WRAPUP_DELAY);
            VarSetNumber("TimeLeft", (int) (WRAPUP_DELAY * 60.0f) + timerSecondsSince2000());
            CleanupIOPs();
        END_ENTRY

        if (TimerElapsed("Clock"))
        {
            EndScript();
        }
    END_STATES

    ON_STOPSIGNAL
        CleanupIOPs();
        EndScript();
    END_SIGNAL
}

void IOPRaidInit()
{
    SetScriptName("IOPRaid");
    SetScriptFunc(IOPRaid);
    SetScriptType(SCRIPT_ZONE);

    SetScriptVar("BlueSGID",                    NULL,        SP_OPTIONAL);
    SetScriptVar("RedSGID",                        NULL,        SP_OPTIONAL);
    SetScriptVar("BlueSGName",                    NULL,        SP_OPTIONAL);
    SetScriptVar("RedSGName",                    NULL,        SP_OPTIONAL);
    SetScriptVar("BlueRaidID",                    NULL,        SP_OPTIONAL);
    SetScriptVar("RedRaidID",                    NULL,        SP_OPTIONAL);
    SetScriptVar("BlueFlagState",                NULL,        SP_OPTIONAL);
    SetScriptVar("RedFlagState",                NULL,        SP_OPTIONAL);
    SetScriptVar("BlueFlagCarrier",                NULL,        SP_OPTIONAL);
    SetScriptVar("RedFlagCarrier",                NULL,        SP_OPTIONAL);
    SetScriptVar("BlueCaptures",                NULL,        SP_OPTIONAL);
    SetScriptVar("RedCaptures",                    NULL,        SP_OPTIONAL);
    SetScriptVar("AlwaysOn",                    NULL,        SP_DONTDISPLAYDEBUG | SP_OPTIONAL);

    SetScriptVar("CapturesToWin",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("GameDuration",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("IOPName",                        NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("IOPInteract",                    NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("IOPInteractBar",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("IOPInterrupt",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("IOPComplete",                    NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("DroppedIOPName",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("DroppedIOPInteract",            NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("DroppedIOPInteractBar",        NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("DroppedIOPInterrupt",            NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("DroppedIOPComplete",            NULL,        SP_DONTDISPLAYDEBUG);

    SetScriptVar("BlueTeamUICaptureTitle",        NULL,        SP_DONTDISPLAYDEBUG | SP_OPTIONAL);
    SetScriptVar("RedTeamUICaptureTitle",        NULL,        SP_DONTDISPLAYDEBUG | SP_OPTIONAL);
    SetScriptVar("BlueTeamUICaptureCount",        NULL,        SP_DONTDISPLAYDEBUG | SP_OPTIONAL);
    SetScriptVar("RedTeamUICaptureCount",        NULL,        SP_DONTDISPLAYDEBUG | SP_OPTIONAL);
    SetScriptVar("CaptureText",                    NULL,        SP_DONTDISPLAYDEBUG);
    //SetScriptVar("BlueTeamUIFlagTitle",            NULL,        SP_DONTDISPLAYDEBUG | SP_OPTIONAL);
    //SetScriptVar("RedTeamUIFlagTitle",            NULL,        SP_DONTDISPLAYDEBUG | SP_OPTIONAL);
    SetScriptVar("FlagText",                    NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("WonText",                        NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("DrawText",                    NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("TakenText",                    NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("YouTookText",                    NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("DroppedText",                    NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("TheyDroppedText",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("CapturedText",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("YouCapturedText",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("ReturnedText",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("TheyReturnedText",            NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("TimeRemaining",                NULL,        SP_DONTDISPLAYDEBUG );
}
