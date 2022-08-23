// LOCATION SCRIPT

// This is a location script for the Bloody Bay Collect The Ore Game
//

#include "script/scriptutil.h"
#include "ScriptUI.h"

#define COVBLOODYBAYORETOKEN_NUM    6
char *CoVBloodyBayOreTokens[] =
{
    "BloodyBayOre1",
    "BloodyBayOre2",
    "BloodyBayOre3",
    "BloodyBayOre4",
    "BloodyBayOre5",
    "BloodyBayOre6",
};

typedef struct {
    char *name;
    char *token;
} CoVBloodyBayMeteorTypes;

CoVBloodyBayMeteorTypes CoVBloodyBayMeteors[] =
{
    {"Meteors_Meteor1", "BloodyBayOre1" },
    {"Meteors_Meteor2", "BloodyBayOre2" },
    {"Meteors_Meteor3", "BloodyBayOre3" },
    {"Meteors_Meteor4", "BloodyBayOre4" },
    {"Meteors_Meteor5", "BloodyBayOre5" },
    {"Meteors_Meteor6", "BloodyBayOre6" },
};

enum {
    COV_BLOODYBAY_PAGE_HELLO = 1,
    COV_BLOODYBAY_PAGE_ACCEPTMISSION,
    COV_BLOODYBAY_PAGE_LEAVE,
};


int CoVBloodyBayOreCheck(ENTITY player, NUMBER remove)
{
    int i; 
    int count = COVBLOODYBAYORETOKEN_NUM;
    int salvage = EntityCountSalvage(player, VarGet("ProxySalvage"));

    for (i = 0; i < COVBLOODYBAYORETOKEN_NUM; i++)
    {
        if (EntityHasRewardToken(player, CoVBloodyBayOreTokens[i]))
        {
            count--;
        }
    }

    if (count)
    {
        if (remove && (count <= salvage))
            EntityGrantSalvage(player, VarGet("ProxySalvage"), -count);

        return (count <= salvage);
    } else {
        return true;
    }
}

void CoVBloodyBayOreGameStop(ENTITY player)
{
    /////////////////////////////////////////////////////////////////////////////
    // STOP GAME
    /////////////////////////////////////////////////////////////////////////////

    // remove the ore and the sample
    EntityRemoveRewardToken(player, "BloodyBayOre1");
    EntityRemoveRewardToken(player, "BloodyBayOre2");
    EntityRemoveRewardToken(player, "BloodyBayOre3");
    EntityRemoveRewardToken(player, "BloodyBayOre4");
    EntityRemoveRewardToken(player, "BloodyBayOre5");
    EntityRemoveRewardToken(player, "BloodyBayOre6");
    EntityRemoveRewardToken(player, "BloodyBayOreSample");
    EntityRemoveTempPower(player, "SilentPowers", "V_Ore_Exposure");
    EntityRemoveTempPower(player, "SilentPowers", "V_Sample_Exposure");

    // remove extractor & token
    EntityRemoveRewardToken(player, VarGet("OreExtractorRewardToken"));
    EntityRemoveTempPower(player, "Temporary_Powers", "V_Ore_Extractor");
}

// called when the player leaves the map
int CoVBloodyBayOreGameOnLeaveMap(ENTITY player)
{
    CoVBloodyBayOreGameStop(player);

    //Remove gang affiliation
    SetAIGang(player, 0);

    // remove waypoints
    ClearWaypoint(player, VarGetNumber("HeroContact"));
    ClearWaypoint(player, VarGetNumber("VillainContact"));

    return 0;
}

// called when the player enters the map
int CoVBloodyBayOreGameOnEnterMap(ENTITY player)
{
    // Set waypoints correctly
    if (IsEntityHero(player)) {
        SetWaypoint(player, VarGetNumber("HeroContact"));
    } else {
        SetWaypoint(player, VarGetNumber("VillainContact"));
    }

    // clean up anything bad they might have
    CoVBloodyBayOreGameStop(player);

    return 0;
}


int CoVBloodyBayOreGameOnConverterContactClick(ENTITY player, ENTITY target, NUMBER link, ContactResponse* response)
{
    NUMBER interactOk = 0;

    switch (link)
    {
    case COV_BLOODYBAY_PAGE_HELLO:    // HELLO

        //Find the FireBase I am inside, determine if it is on the same team as the player
        {
            ENCOUNTERGROUP group;
            ENTITY firebaseBrain = 0;

            group = FindEncounter( EntityPos(target), 0, 100, 0, 0, NULL, "FireBase", SEF_OCCUPIED_ONLY | SEF_SORTED_BY_DISTANCE );
            firebaseBrain = ActorFromEncounterPos( group, VarGetNumber( "PositionOfFirebaseBrain"  ) );

            if( StringEqual(firebaseBrain,"ENT:NONE") )//If you aren't in an active firebase, just process it (Debug, not legal)
            {
                strcat(response->dialog, "Debug: I'm not in an active firebase, I'll interact with anybody");
                interactOk = 1;
            }
            else
            {
                if ( VarGetNumber("CanControlFirebase") )
                {
                    if( SameTeam( firebaseBrain, player ) )
                        interactOk = 1;
                } else {
                    if( GetAIAlly( firebaseBrain ) == kAllyID_Hero ) 
                        interactOk = 1;
                }
            }
        }

        //Next check the 
        if( !interactOk )
        {
            strcat(response->dialog, StringLocalizeWithVars(VarGet("OreConverterSpeechYouDontOwnFirebase"), player));
        }
        else
        {
            // check to see if player is playing the game
            if (EntityHasRewardToken(player, VarGet("OreExtractorRewardToken")))
            {
                // check for ore!
                if (CoVBloodyBayOreCheck(player, false))
                {
                    // give credit for use
                    GiveBadgeCredit(player, kScriptBadgeTypeBBOreConvert);

                    // remove any salvage used as proxy
                    CoVBloodyBayOreCheck(player, true);

                    // remove the ore and give the sample
                    EntityRemoveRewardToken(player, "BloodyBayOre1");
                    EntityRemoveRewardToken(player, "BloodyBayOre2");
                    EntityRemoveRewardToken(player, "BloodyBayOre3");
                    EntityRemoveRewardToken(player, "BloodyBayOre4");
                    EntityRemoveRewardToken(player, "BloodyBayOre5");
                    EntityRemoveRewardToken(player, "BloodyBayOre6");
                    EntityRemoveTempPower(player, "SilentPowers", "V_Ore_Exposure");
                    EntityGrantReward(player, "BloodyBayOreSample");
                    EntityGrantReward(player, VarGet("OreSampleReward"));

                    ScriptUIHide("OreCollectUI", player);
                    ScriptUIShow("SampleUI", player);

                    // find out if player is hero or villain
                    if (IsEntityHero(player)) {
                        strcat(response->dialog, StringLocalizeWithVars(VarGet("OreConverterSpeechCompleteHero"), player));
                    } else {
                        strcat(response->dialog, StringLocalizeWithVars(VarGet("OreConverterSpeechCompleteVillain"), player));
                    }

                } else {
                    // find out if player is hero or villain
                    if (IsEntityHero(player)) {
                        strcat(response->dialog, StringLocalizeWithVars(VarGet("OreConverterSpeechNoOreHero"), player));
                    } else {
                        strcat(response->dialog, StringLocalizeWithVars(VarGet("OreConverterSpeechNoOreVillain"), player));
                    }
                }
            } else {
                // find out if player is hero or villain
                if (IsEntityHero(player)) {
                    strcat(response->dialog, StringLocalizeWithVars(VarGet("OreConverterSpeechNoGameHero"), player));
                } else {
                    strcat(response->dialog, StringLocalizeWithVars(VarGet("OreConverterSpeechNoGameVillain"), player));
                }
            }
        }
        AddResponse(response, StringLocalizeWithVars(VarGet("ScientistExitText"), player), COV_BLOODYBAY_PAGE_LEAVE);
        return 1;
        break;
    case COV_BLOODYBAY_PAGE_LEAVE:
    default:
        return 0;
    }
}


int CoVBloodyBayOreGameOnScientistContactClick(ENTITY player, ENTITY target, NUMBER link, ContactResponse* response)
{
    // headshot
    sprintf(response->dialog, getContactHeadshotScriptSMF(target, player));

    switch (link)
    {
    case COV_BLOODYBAY_PAGE_HELLO:    // HELLO
        // check to see if player is playing the game 
        if (EntityHasRewardToken(player, "BloodyBayOreSample"))
        {
            CoVBloodyBayOreGameStop(player);
            ScriptUIHide("SampleUI", player);

            // find out if player is hero or villain
            if (IsEntityHero(player)) {
                strcat(response->dialog, StringLocalizeWithVars( VarGet("CompleteSpeechHero"), player));
                EntityGrantReward(player, VarGet("FinalRewardHero"));
            } else {
                strcat(response->dialog, StringLocalizeWithVars(VarGet("CompleteSpeechVillain"), player));
                EntityGrantReward(player, VarGet("FinalRewardVillain")); 
            }
        } else if (EntityHasRewardToken(player, VarGet("OreExtractorRewardToken"))) {
            if (IsEntityHero(player)) {
                strcat(response->dialog, StringLocalizeWithVars(VarGet("NoSampleSpeechHero"), player));
            } else {
                strcat(response->dialog, StringLocalizeWithVars(VarGet("NoSampleSpeechVillain"), player));
            }
        } else {
            if (IsEntityHero(player)) {
                strcat(response->dialog, StringLocalizeWithVars(VarGet("ScientistPlayGameHero"), player));
                AddResponse(response, StringLocalizeWithVars(VarGet("ScientistPlayGameAcceptHero"), player), COV_BLOODYBAY_PAGE_ACCEPTMISSION);
                AddResponse(response, StringLocalizeWithVars(VarGet("ScientistPlayGameDeclineHero"), player), COV_BLOODYBAY_PAGE_LEAVE);
            } else {
                strcat(response->dialog, StringLocalizeWithVars(VarGet("ScientistPlayGameVillain"), player));
                AddResponse(response, StringLocalizeWithVars(VarGet("ScientistPlayGameAcceptVillain"), player), COV_BLOODYBAY_PAGE_ACCEPTMISSION);
                AddResponse(response, StringLocalizeWithVars(VarGet("ScientistPlayGameDeclineVillain"), player), COV_BLOODYBAY_PAGE_LEAVE);
            }
        }
        AddResponse(response, StringLocalizeWithVars(VarGet("ScientistExitText"), player), COV_BLOODYBAY_PAGE_LEAVE);
        return 1;
        break;
    case COV_BLOODYBAY_PAGE_ACCEPTMISSION:
        /////////////////////////////////////////////////////////////////////////////
        // START GAME
        /////////////////////////////////////////////////////////////////////////////
        // give game playing token
        if (IsEntityHero(player)) {
            strcat(response->dialog, StringLocalizeWithVars(VarGet("OreExtractorSpeechHero"), player));
        } else {
            strcat(response->dialog, StringLocalizeWithVars(VarGet("OreExtractorSpeechVillain"), player));
        }
        EntityGrantReward(player, VarGet("OreExtractorRewardToken"));
        EntityGrantReward(player, VarGet("OreExtractorReward"));
        ScriptUIShow("OreCollectUI", player);
        AddResponse(response, StringLocalizeWithVars(VarGet("ScientistExitText"), player), COV_BLOODYBAY_PAGE_LEAVE);
        return 1;
        break;
    case COV_BLOODYBAY_PAGE_LEAVE:
    default:
        return 0;
    }
}

int CoVBloodyBayOreGameOnScientistVillainContactClick(ENTITY player, ENTITY target, NUMBER link, ContactResponse* response)
{
    if (IsEntityVillain(player)) {
        return CoVBloodyBayOreGameOnScientistContactClick( player, target, link, response);
    } else {
        if (TimerElapsed("VillainContactTalk")) {
            Say(target, VarGet("VillainGoAway"), 0);
            TimerSet("VillainContactTalk", 0.25f);
        }
        return 0;
    }
}

int CoVBloodyBayOreGameOnScientistHeroContactClick(ENTITY player, ENTITY target, NUMBER link, ContactResponse* response)
{
    if (IsEntityHero(player)) {  
        return CoVBloodyBayOreGameOnScientistContactClick( player, target, link, response);
    } else {
        if (TimerElapsed("HeroContactTalk")) {
            Say(target, VarGet("HeroGoAway"), 0);
            TimerSet("HeroContactTalk", 0.25f);
        }
        return 0;
    }
}



void CoVBloodyBayOreGameOnPK(ENTITY player, ENTITY victor)
{
    int count, start;
    int i, j;
    ENTITY killer;

    // get list of all players on killing team
    
    ENTITY team = GetPlayerTeamFromPlayer(victor);
    count = NumEntitiesInTeam(team);

    // randomly order list
    start = RandomNumber(1, count);

    // for all ore widgets
    for (i = 0; i < COVBLOODYBAYORETOKEN_NUM; i++)
    {
        EntityRemoveTempPower(player, "SilentPowers", "V_Ore_Exposure");
        if (EntityHasRewardToken(player, CoVBloodyBayOreTokens[i]))
        {
            EntityRemoveRewardToken(player, CoVBloodyBayOreTokens[i]);

            // go through player team list and see if they have the one we've got
            for (j = 1; j <= count; j++) {
                start++;
                if (start > count)
                    start = 1;

                killer = GetEntityFromTeam(team, start);

                if (EntityHasRewardToken(killer, VarGet("OreExtractorRewardToken"))&&
                    !EntityHasRewardToken(killer, CoVBloodyBayOreTokens[i]) && 
                    DistanceBetweenEntities(killer, player) < 600.0f )
                {
                    // if they don't, give it to them
                    EntityGrantReward(killer, CoVBloodyBayOreTokens[i]);
                    EntityGrantReward(killer, "V_OreExposure");

                    break;
                }
            }
        }
    }


    // check for sample
    if (EntityHasRewardToken(player, "BloodyBayOreSample"))
    {
        EntityRemoveTempPower(player, "SilentPowers", "V_Sample_Exposure");
        EntityRemoveRewardToken(player, "BloodyBayOreSample");
        ScriptUIHide("SampleUI", player);
        ScriptUIShow("OreCollectUI", player);

        // go through player team list and see if they have the one we've got
        for (j = 1; j <= count; j++) {
            start++;
            if (start > count)
                start = 1;

            killer = GetEntityFromTeam(team, start);

            if (EntityHasRewardToken(killer, VarGet("OreExtractorRewardToken"))&&
                !EntityHasRewardToken(killer, VarGet("OreExtractorRewardToken")) && 
                DistanceBetweenEntities(killer, player) < 600.0f )
            {
                // remove old stuff first
                EntityRemoveRewardToken(player, "BloodyBayOre1");
                EntityRemoveRewardToken(player, "BloodyBayOre2");
                EntityRemoveRewardToken(player, "BloodyBayOre3");
                EntityRemoveRewardToken(player, "BloodyBayOre4");
                EntityRemoveRewardToken(player, "BloodyBayOre5");
                EntityRemoveRewardToken(player, "BloodyBayOre6");
                ScriptUIHide("OreCollectUI", player);

                // if they don't, give it to them
                ScriptUIShow("SampleUI", player);
                EntityGrantReward(killer, VarGet("OreSampleReward"));
                EntityGrantReward(killer, "BloodyBayOreSample");
                break;
            }
        }
    }

}

void CoVBloodyBayOreGameOnEntityDefeat(ENTITY player, ENTITY victor)
{
    int i;

    // don't need to bother with this if it isn't a player
    if (IsEntityPlayer(victor))
    {
        // check for PK
        if (IsEntityPlayer(player))
        {
            CoVBloodyBayOreGameOnPK(player, victor);
        } else {
            // check for meteors
            if (IsEntityCritter(player))
            {
                STRING name = GetVillainDefName(player);
                for (i = (ARRAY_SIZE(CoVBloodyBayMeteors)-1); i >= 0; --i)
                {
                    if (StringEqual(CoVBloodyBayMeteors[i].name, name))
                    {
                        EntityGrantReward(victor, "V_OreExposure");
                        EntityGrantReward(victor, CoVBloodyBayMeteors[i].token);
                        break;
                    }
                }
            }
        }
    }

}

void CoVBloodyBayOreGame(void)
{   
    ENCOUNTERGROUP group = "first"; 

    INITIAL_STATE      
//////////////////////////////////////////////////////////////////////////
    ON_ENTRY  //////////////////////////////////////////////////////////////////////////
    {
        // setup timers for contact
        TimerSet("HeroContactTalk", 0.25f);
        TimerSet("VillainContactTalk", 0.25f);

        // create UI
        ScriptUITitle(ONENTER("TitleUI"), "EventName", "EventInfo");
        ScriptUICollectItems("OreCollectUI", 6,    "Token:BloodyBayOre1", "Ore1Info", "sp_icon_meteor_01.tga",
                                                "Token:BloodyBayOre2", "Ore2Info", "sp_icon_meteor_02.tga",
                                                "Token:BloodyBayOre3", "Ore3Info", "sp_icon_meteor_03.tga",
                                                "Token:BloodyBayOre4", "Ore4Info", "sp_icon_meteor_04.tga",
                                                "Token:BloodyBayOre5", "Ore5Info", "sp_icon_meteor_05.tga",
                                                "Token:BloodyBayOre6", "Ore6Info", "sp_icon_meteor_06.tga");

        // create second UI
        ScriptUICollectItems("SampleUI", 1, "Token:BloodyBayOreSample", "SampleInfo", "sp_icon_meteor_full.tga");

        // spawn NPCs
        group = FindEncounterGroupByRadius( 
            MyLocation(),
            0, 
            9999, 
            "NPC_Villain_Scientist", 
            1, 
             0 );  
        if (stricmp(group, "location:none") != 0) {
            Spawn( 1, "Scientists", "scripts.loc/SpawnDefs/V_PvP_02_01/NPC_OreNPCs_D0_V1.spawndef", group, "NPC_Villain_Scientist", VarGetNumber("SpawnLevel"), 0 );  

            // add on click handler
            OnScriptedContactInteract(CoVBloodyBayOreGameOnScientistVillainContactClick, ActorFromEncounterPos(group, 1));
//            OnPlayerInteract(CoVBloodyBayOreGameOnScientistClick, ActorFromEncounterPos(group, 1));

            // playing with map markers
            VarSetNumber("VillainContact", CreateWaypoint(EncounterPos(group, 1), VarGet("ScientistVillainMapName"), "map_enticon_contact", NULL, false, false, -1.0f));

        }

        group = FindEncounterGroupByRadius( 
            MyLocation(),
            0, 
            9999, 
            "NPC_Hero_Scientist", 
            1, 
             0 );  
        if (stricmp(group, "location:none") != 0) {
            Spawn( 1, "Scientists", "scripts.loc/SpawnDefs/V_PvP_02_01/NPC_OreNPCs_D0_V0.spawndef", group, "NPC_Hero_Scientist", VarGetNumber("SpawnLevel"), 0 );  

            // add on click handler
            OnScriptedContactInteract(CoVBloodyBayOreGameOnScientistHeroContactClick, ActorFromEncounterPos(group, 1));
//            OnPlayerInteract(CoVBloodyBayOreGameOnScientistClick, ActorFromEncounterPos(group, 1));

            VarSetNumber("HeroContact", CreateWaypoint(EncounterPos(group, 1), VarGet("ScientistHeroMapName"), "map_enticon_contact", NULL, false, false, -1.0f));

        }

        // spawn Converters
        group = FindEncounterGroupByRadius( 
            MyLocation(),
            0, 
            9999, 
            "Ore_Converter", 
            1, 
             0 );  

        while (stricmp(group, "location:none") != 0)
        {
            Spawn( 1, "Converters", "scripts.loc/SpawnDefs/V_PvP_02_01/NPC_OreNPCs_D0_V2.spawndef", group, "Ore_Converter", VarGetNumber("SpawnLevel"), 0 );  

            OnScriptedContactInteract(CoVBloodyBayOreGameOnConverterContactClick, ActorFromEncounterPos(group, 1));

            group = FindEncounterGroupByRadius( 
                MyLocation(),
                0, 
                9999, 
                "Ore_Converter", 
                1, 
                 0 );  

        } 

        //////////////////////////////////////////////////////////////////////////
        ///Script Hooks
        // setup whatever to remove ore and stuff when player leaves map
        OnPlayerExitingMap( CoVBloodyBayOreGameOnLeaveMap );

        OnPlayerEnteringMap( CoVBloodyBayOreGameOnEnterMap );
            
//        OnPlayerJoinsTeam( CoVBloodyBayOreGameOnJoinTeam );

//        OnPlayerLeavesTeam(    CoVBloodyBayOreGameOnLeaveTeam );

        OnEntityDefeated ( CoVBloodyBayOreGameOnEntityDefeat );

    }
    END_ENTRY

//    RunBloodyBayTick();

////////////////////////////////////////////////////////////////////////////////////////
    STATE("Cleanup") ////////////////////////////////////////////////////////
    

    EndScript();

     
    END_STATES

    ON_SIGNAL("Win")
    {
        int i, count = NumEntitiesInTeam(ALL_PLAYERS);

        for (i = 1; i <= count; i++)
        {
            ENTITY player = GetEntityFromTeam(ALL_PLAYERS, i);

            if (IsEntityHero(player)) {
                EntityGrantReward(player, VarGet("FinalRewardHero"));
            } else {
                EntityGrantReward(player, VarGet("FinalRewardVillain")); 
            }
        }
    }
    END_SIGNAL

    ON_SIGNAL("EndScript")
        SET_STATE( "Cleanup" );
    END_SIGNAL


}


void CoVBloodyBayOreGameInit()
{
    SetScriptName( "CoVBloodyBayOreGame" );
    SetScriptFunc( CoVBloodyBayOreGame );
    SetScriptType( SCRIPT_LOCATION );        

    SetScriptVar( "ScientistPlayGameHero",                NULL,        SP_MULTIVALUE );
    SetScriptVar( "ScientistPlayGameAcceptHero",        NULL,        SP_MULTIVALUE );
    SetScriptVar( "ScientistPlayGameDeclineHero",        NULL,        SP_MULTIVALUE );
    SetScriptVar( "ScientistPlayGameVillain",            NULL,        SP_MULTIVALUE );
    SetScriptVar( "ScientistPlayGameAcceptVillain",        NULL,        SP_MULTIVALUE );
    SetScriptVar( "ScientistPlayGameDeclineVillain",    NULL,        SP_MULTIVALUE );
    SetScriptVar( "ScientistExitText",                    NULL,        SP_MULTIVALUE );

    SetScriptVar( "OreExtractorSpeechVillain",            NULL,        SP_MULTIVALUE );
    SetScriptVar( "OreExtractorSpeechHero",                NULL,        SP_MULTIVALUE );
    SetScriptVar( "OreExtractorReward",                    NULL,        0 );
    SetScriptVar( "OreExtractorRewardToken",            NULL,        0 );
    SetScriptVar( "NoSampleSpeechVillain",                NULL,        SP_MULTIVALUE );

    SetScriptVar( "NoSampleSpeechHero",                    NULL,        SP_MULTIVALUE );
    SetScriptVar( "OreConverterSpeechYouDontOwnFirebase",NULL,        SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "OreConverterSpeechNoGameHero",        NULL,        SP_MULTIVALUE );
    SetScriptVar( "OreConverterSpeechNoGameVillain",    NULL,        SP_MULTIVALUE );
    SetScriptVar( "OreConverterSpeechNoOreHero",        NULL,        SP_MULTIVALUE );

    SetScriptVar( "OreConverterSpeechNoOreVillain",        NULL,        SP_MULTIVALUE );
    SetScriptVar( "OreConverterSpeechCompleteHero",        NULL,        SP_MULTIVALUE );
    SetScriptVar( "OreConverterSpeechCompleteVillain",    NULL,        SP_MULTIVALUE );
    SetScriptVar( "OreSampleReward",                    NULL,        0 );
    SetScriptVar( "CompleteSpeechVillain",                NULL,        SP_MULTIVALUE );

    SetScriptVar( "CompleteSpeechHero",                    NULL,        SP_MULTIVALUE );
    SetScriptVar( "FinalRewardHero",                    NULL,        0 );
    SetScriptVar( "FinalRewardVillain",                    NULL,        0 );
    SetScriptVar( "EventName",                            NULL,        0 );
    SetScriptVar( "EventInfo",                            NULL,        0 );

    SetScriptVar( "Ore1Info",                            NULL,        0 );
    SetScriptVar( "Ore2Info",                            NULL,        0 );
    SetScriptVar( "Ore3Info",                            NULL,        0 );
    SetScriptVar( "Ore4Info",                            NULL,        0 );
    SetScriptVar( "Ore5Info",                            NULL,        0 );
    SetScriptVar( "Ore6Info",                            NULL,        0 );
    SetScriptVar( "SampleInfo",                            NULL,        0 );
    SetScriptVar( "ScientistHeroMapName",                NULL,        0 );
    SetScriptVar( "ScientistVillainMapName",            NULL,        0 );

    SetScriptVar( "VillainGoAway",                        NULL,        0 );
    SetScriptVar( "HeroGoAway",                            NULL,        0 );

    SetScriptVar( "PositionOfFirebaseBrain",            "10",            0 );
    SetScriptVar( "CanControlFirebase",                    "1",            0 );

    SetScriptVar( "ProxySalvage",                        NULL,            0 );

    SetScriptSignal( "End Event", "EndScript" );        
    SetScriptSignal( "Win", "Win" );        
}
