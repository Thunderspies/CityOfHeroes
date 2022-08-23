/***************************************************************************
 *     Copyright (c) 2004-2006 Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
 
#include "gameComm/wdwbase.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiWindows.h"
#include "UI/uiComboBox.h"
#include "uiSlider.h"
#include "UI/uiScrollBar.h"
#include "UI/uiContextMenu.h"
#include "UI/uiChatUtil.h"
#include "UI/uiEditText.h"
#include "UI/uiInput.h"
#include "UI/uiReticle.h"
#include "UI/uiCursor.h"
#include "UI/uiFocus.h"
#include "UI/uiArena.h"
#include "UIEdit.h"
#include "UI/uiGame.h"
#include "UI/uiInfo.h"
#include "uiHelp.h"
#include "UI/uiNet.h"
#include "graphics/textureatlas.h"

#include "graphics/ttFontUtil.h"
#include "graphics/truetype/ttFontDraw.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "gameData/arenastruct.h"
#include "player/player.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/earray.h>
#include "win/input.h"
#include "limits.h"
#include <utilitieslib/utils/utils.h>
#include "entity/entVarUpdate.h"
#include "entity/character_level.h"
#include <utilitieslib/utils/timing.h>
#include "sound/sound.h"
#include "win/win_init.h"
#include "UI/uiFx.h"
#include "UI/uiToolTip.h"
#include "arena/arenagame.h"
#include "formatter/smf_main.h"
#include "uiSMFView.h"
#include "entity/entity.h"
#include "UI/sprite/sprite_base.h"
#include "clientcomm/clientcommreceive.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "auth/authUserData.h"
#include "clientcomm/dbclient.h"
#include <utilitieslib/language/MessageStoreUtil.h>

//--------------------------------------------------------------------------------------------
// GLOBALS, STRUCTS, DEFINES /////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------

extern ArenaMaps g_arenamaplist;

typedef struct UIArenaParticipant
{
    int    id;
    ComboBox team;
    ComboBox Archetype;
} UIArenaParticipant;

UIArenaParticipant ** uiArenaParticipant;

ArenaEvent gArenaDetail;
ArenaEvent gArenaLast;

ComboBox comboTeamStyle;
ComboBox comboTeamType;
ComboBox comboTeamCount;
SMFBlock *labelPlayerCount;
SMFBlock *textPlayerCount;
ComboBox comboVictoryType;
SMFBlock *labelVictoryValue;
SMFBlock *textVictoryValue;
ComboBox comboTournamentType;
ComboBox comboWeight;
ComboBox comboMap;
ComboBox comboInspiration; // on options window

static int creator_update;
static int full_player_update;
static ArenaParticipant *player_update;

static bool arena_detail_init = false;
static bool arena_settings_init = false;

#define ARENA_SETTING_HT    104
#define ARENA_CHAT_HT        100
#define ARENA_CHAT_WD        450

#define ICON_COL_WD    25
#define NAME_COL_WD    220
#define TEAM_COL_WD 90
#define LVL_COL_WD    80
#define ARCH_COL_WD    175 
#define KICK_COL_WD    50
#define PLAYER_HT    25

ComboSetting eventTeamStyles[] = 
{
    {ARENA_FREEFORM,        "FreeformString",        "FreeformString",        },
    {ARENA_STAR,            "5TARString",            "5TARString",            },
    {ARENA_STAR_VILLAIN,    "Villain5TARString",    "Villain5TARString",    },
    {ARENA_STAR_VERSUS,        "Versus5TARString",        "Versus5TARString",        },
    {ARENA_7STAR,            "7TARString",            "7TARString",            },
    {ARENA_7STAR_VILLAIN,    "Villain7TARString",    "Villain7TARString",    },
    {ARENA_7STAR_VERSUS,    "Versus7TARString",        "Versus7TARString",        },
};

ComboSetting eventTeamTypes[] =
{
    {ARENA_SINGLE,            "FreeForAll",        "FreeForAll"        },
    {ARENA_TEAM,            "ArenaTeam",        "ArenaTeam"            },
    {ARENA_SUPERGROUP,        "ArenaSupergroup",    "ArenaSupergroup"    },
};

ComboSetting eventVictoryTypes[] = 
{
    {ARENA_TIMED,            "ArenaTimed",        "ArenaTimed"        },            
    {ARENA_TEAMSTOCK,        "ArenaTeamStock",    "ArenaTeamStock"    },
    {ARENA_LASTMAN,            "LastManStanding",    "LastManStanding"    },
    {ARENA_KILLS,            "ArenaKills",        "ArenaKills"        },
    {ARENA_STOCK,            "ArenaStock",        "ArenaStock"        },
};

typedef struct ArenaVictoryValueDetails
{
    ArenaVictoryType victoryType;
    char *valueLabel;
    bool editable;
    int minimumValue;
    int maximumValue;
} ArenaVictoryValueDetails;

// Needs to be in the same order as eventVictoryTypes[]!
ArenaVictoryValueDetails eventVictoryValueLabels[] = 
{
    {ARENA_TIMED,            "ArenaTimedValueLabel",    true,    5,    30    },
    {ARENA_TEAMSTOCK,        "ArenaStockValueLabel",    true,    1,    15    },
    {ARENA_LASTMAN,            "",                        false,    0,    0    },
    {ARENA_KILLS,            "ArenaKillsValueLabel",    true,    1,    10    },
    {ARENA_STOCK,            "ArenaStockValueLabel",    true,    1,    10    },
};

ComboSetting eventTournamentTypes[] =
{
    {ARENA_TOURNAMENT_NONE,            "ArenaSingleMatchString",    "ArenaSingleMatchString"    },
    {ARENA_TOURNAMENT_SWISSDRAW,    "SwissDrawString",            "SwissDrawString"            },
};

ComboSetting eventWeight[] =
{
    {ARENA_WEIGHT_ANY,    "AnyLevel",                "AnyLevel"                },
    {ARENA_WEIGHT_0,    "StrawWeight",            "StrawWeightTitle"        },
    {ARENA_WEIGHT_5,    "FlyWeight",            "FlyWeightTitle"        },
    {ARENA_WEIGHT_10,    "BantamWeight",            "BantamWeightTitle"        },
    {ARENA_WEIGHT_15,    "FeatherWeight",        "FeatherWeightTitle"    },
    {ARENA_WEIGHT_20,    "LightWeight",            "LightWeightTitle"        }, 
    {ARENA_WEIGHT_25,    "WelterWeight",            "WelterWeightTitle"        }, 
    {ARENA_WEIGHT_30,    "MiddleWeight",            "MiddleWeightTitle"        }, 
    {ARENA_WEIGHT_35,    "CruiserWeight",        "CruiserWeightTitle"    }, 
    {ARENA_WEIGHT_40,    "HeavyWeight",            "HeavyWeightTitle"        }, 
    {ARENA_WEIGHT_45,    "SuperHeavyWeight",        "SuperHeavyWeightTitle"    }, 
};

ComboSetting eventTeamCounts[] = 
{
    {2,            "2Teams",        "2Teams"        },
    {3,            "3Teams",        "3Teams"        },
    {4,            "4Teams",        "4Teams"        },
    {5,            "5Teams",        "5Teams"        },
    {6,            "6Teams",        "6Teams"        },
    {7,            "7Teams",        "7Teams"        },
    {8,            "8Teams",        "8Teams"        },
};

ComboSetting eventInspirations[] =
{
    {ARENA_NO_INSPIRATIONS,                "ArenaNoInspirations",                "ArenaNoInspirations"                },
    {ARENA_SMALL_INSPIRATIONS,            "ArenaSmallInspirations",            "ArenaSmallInspirations"            },
    {ARENA_SMALL_MEDIUM_INSPIRATIONS,    "ArenaSmallMediumInspirations",        "ArenaSmallMediumInspirations"    },
    {ARENA_NORMAL_INSPIRATIONS,            "ArenaNormalInspirations",            "ArenaNormalInspirations"            },
    {ARENA_ALL_INSPIRATIONS,            "ArenaAllInspirations",                "ArenaAllInspirations"                },
};

ComboSetting participantTeam[] =
{
    {0, "SoloString",    "SoloString"    },
    {1, "RedString",    "RedString"        },
    {2, "BlueString",     "BlueString"    },
    {3, "GreenString",     "GreenString"    },
    {4, "BlackString",    "BlackString"    }, 
    {5, "WhiteString",    "WhiteString"    }, 
    {6, "YellowString",    "YellowString"    }, 
    {7, "OrangeString",    "OrangeString"    }, 
    {8, "PurpleString",    "PurpleString"    }, 
};

static int teamColors[] = { 0, 0xff000044, 0x0000ff44, 0x00ff0044, 0x00000044, 
                        0xffffff44, 0xffff0044, 0xff880044, 0xff00ff44 }; 

const char * getTeamString(int n) {
    int teamStringIndex = n;
    if (n > 0)
    {
        //    this is accounting for the 0 index not meant to be used for the team setting
        //    so if team is > participantTeam (not counting the solo), it mods properly
        teamStringIndex = ((n-1) % (ARRAY_SIZE(participantTeam)-1)) + 1;
    }
    return textStd(participantTeam[teamStringIndex].text);
}

int getTeamColor(int n) {
    int teamIndex = n;
    if (n > 0)
    {
        //    this is accounting for the 0 index not meant to be used for the team setting
        //    so if team is > teamColors (not counting the solo), it mods properly
        teamIndex = ((n-1) % (ARRAY_SIZE(teamColors)-1)) + 1;
    }
    return teamColors[teamIndex];
}

ComboSetting participantClass[] =
{
    {0,        "AnyClass",                    "AnyClass"                                    },
    {1,        "Class_Blaster",            "Class_Blaster",            "Class_Blaster"        },
    {2,        "Class_Controller",            "Class_Controller",            "Class_Controller"    }, 
    {3,        "Class_Defender",            "Class_Defender",            "Class_Defender"    }, 
    {4,        "Class_Scrapper",            "Class_Scrapper",            "Class_Scrapper"    }, 
    {5,        "Class_Tanker",                "Class_Tanker",                "Class_Tanker"        }, 
    {6,        "Class_Warshade",            "Class_Warshade",            "Class_Warshade"    }, 
    {7,        "Class_Peacebringer",        "Class_Peacebringer",        "Class_Peacebringer"},
    {8,        "Class_Brute",                "Class_Brute",                "Class_Brute"        },
    {9,        "Class_Corruptor",            "Class_Corruptor",            "Class_Corruptor"    },
    {10,    "Class_Dominator",            "Class_Dominator",            "Class_Dominator"    },
    {11,    "Class_MasterMind",            "Class_MasterMind",            "Class_MasterMind"    },
    {12,    "Class_Stalker",            "Class_Stalker",            "Class_Stalker"        },
    {13,    "Class_Arachnos_Widow",        "Class_Arachnos_Widow",        "Class_Arachnos_Widow"        },
    {14,    "Class_Arachnos_Soldier",    "Class_Arachnos_Soldier",    "Class_Arachnos_Soldier"        },
};

ComboCheckboxElement **teamChoices;
ComboCheckboxElement **classChoices;

ToolTipParent arenaSettingsParent;
ToolTipParent arenaPlayerListParent;
ToolTipParent arenaChatParent;
ToolTipParent arenaButtonsParent;

extern int g_display_exit_button;

char * arenaGetWeightName( int weight )
{
    return eventWeight[weight].text;
}

char * arenaGetVictoryTypeName( int victoryType )
{
    return eventVictoryTypes[victoryType].text;
}

//--------------------------------------------------------------------------------------------
// ARENA INIT ////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------

#define DEFAULT_ARENA_MAXPLAYER    4

static void arena_initDetails()
{
    gArenaDetail.teamStyle = ARENA_FREEFORM;
    gArenaDetail.teamType = ARENA_SINGLE;
    gArenaDetail.weightClass = ARENA_WEIGHT_ANY;
    gArenaDetail.victoryType = ARENA_TIMED;
    gArenaDetail.victoryValue = 10; // 10 minute default
    gArenaDetail.tournamentType = ARENA_TOURNAMENT_NONE;
    gArenaDetail.verify_sanctioned = false;
    gArenaDetail.inviteonly = false;
    gArenaDetail.no_observe = false;
    gArenaDetail.numsides = 1; // = numplayers for ARENA_SINGLE
    gArenaDetail.maxplayers = DEFAULT_ARENA_MAXPLAYER;
    gArenaDetail.tactical_start = false;
    gArenaDetail.no_end = false;
    gArenaDetail.no_travel = false;
    gArenaDetail.no_stealth = false;
    gArenaDetail.no_travel_suppression = false;
    gArenaDetail.no_diminishing_returns = false;
    gArenaDetail.no_heal_decay = false;
    gArenaDetail.inspiration_setting = ARENA_NORMAL_INSPIRATIONS;
    gArenaDetail.enable_temp_powers = false;
}

static void arena_initTextEntryFields()
{
    int creator = ( playerPtr()->db_id == gArenaDetail.creatorid );

    if (!textVictoryValue)
    {
        textVictoryValue = smfBlock_Create();
        smf_SetFlags(textVictoryValue, (creator ? SMFEditMode_ReadWrite : SMFEditMode_ReadOnly), SMFLineBreakMode_SingleLine, SMFInputMode_NonnegativeIntegers, 999,
            SMFScrollMode_ExternalOnly, SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, 0);
    }
    if (!labelVictoryValue)
    {
        labelVictoryValue = smfBlock_Create();
        smf_SetFlags(labelVictoryValue, SMFEditMode_ReadOnly, SMFLineBreakMode_SingleLine, 0, 0,
            SMFScrollMode_ExternalOnly, 0, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Right, 0, 0, 0);
        smf_SetRawText(labelVictoryValue, "VictoryValue", false);
    }

    if (!textPlayerCount)
    {
        textPlayerCount = smfBlock_Create();
        smf_SetFlags(textPlayerCount, (creator ? SMFEditMode_ReadWrite : SMFEditMode_ReadOnly), SMFLineBreakMode_SingleLine, SMFInputMode_NonnegativeIntegers, 999,
            SMFScrollMode_ExternalOnly, SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, 0, 0, 0);
    }
    if (!labelPlayerCount)
    {
        labelPlayerCount = smfBlock_Create();
        smf_SetFlags(labelPlayerCount, SMFEditMode_ReadOnly, SMFLineBreakMode_SingleLine, 0, 0,
            SMFScrollMode_ExternalOnly, 0, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Right, 0, 0, 0);
        smf_SetRawText(labelPlayerCount, "PlayerCount", false);
    }
}

static void arena_initSettings()
{
    int i;

    comboboxTitle_init( &comboTeamType,            R10,        R10+62, 20, 155, 20, 400, WDW_ARENA_CREATE );
    comboboxTitle_init( &comboTeamStyle,        R10 + 480,    R10+62, 20, 155, 20, 400, WDW_ARENA_CREATE );
    comboboxTitle_init( &comboTeamCount,        R10 + 320,    R10+62, 20, 155, 20, 400, WDW_ARENA_CREATE );
    comboboxTitle_init( &comboVictoryType,        R10,        R10+32, 20, 155, 20, 400, WDW_ARENA_CREATE );
    comboboxTitle_init( &comboTournamentType,    R10 + 320,    R10+32, 20, 155, 20, 400, WDW_ARENA_CREATE );
    comboboxTitle_init( &comboWeight,            R10 + 160,    R10+2,  20, 155, 20, 400, WDW_ARENA_CREATE );
    comboboxTitle_init( &comboMap,                R10 + 320,    R10+2,  20, 155, 20, 400, WDW_ARENA_CREATE );
    comboboxTitle_init( &comboInspiration,        448,        R10+2,  20, 200, 20, 400, WDW_ARENA_OPTIONS );

    // add the elements to the main combo boxes
    for( i = 0; i < ARRAY_SIZE(eventTeamStyles); i++ )
        comboboxTitle_add( &comboTeamStyle, 0, NULL, eventTeamStyles[i].text, eventTeamStyles[i].title, eventTeamStyles[i].id, CLR_WHITE,0,0 );
    for( i = 0; i < ARRAY_SIZE(eventTeamTypes); i++ )
        comboboxTitle_add( &comboTeamType, 0, NULL, eventTeamTypes[i].text, eventTeamTypes[i].title, eventTeamTypes[i].id, CLR_WHITE,0 ,0);
    for( i = 0; i < ARRAY_SIZE(eventTeamCounts); i++ )
        comboboxTitle_add( &comboTeamCount, 0, NULL, eventTeamCounts[i].text, eventTeamCounts[i].title, eventTeamCounts[i].id, CLR_WHITE,0 ,0);
    for( i = 0; i < ARRAY_SIZE(eventVictoryTypes); i++ )
        comboboxTitle_add( &comboVictoryType, 0, NULL, eventVictoryTypes[i].text, eventVictoryTypes[i].title, eventVictoryTypes[i].id, CLR_WHITE,0,0 );
    for( i = 0; i < ARRAY_SIZE(eventTournamentTypes); i++ )
        comboboxTitle_add( &comboTournamentType, 0, NULL, eventTournamentTypes[i].text, eventTournamentTypes[i].title, eventTournamentTypes[i].id, CLR_WHITE,0,0 );
    for( i = 0; i < ARRAY_SIZE(eventWeight); i++ )
        comboboxTitle_add( &comboWeight, 0, NULL, eventWeight[i].text, eventWeight[i].title, eventWeight[i].id, CLR_WHITE,0,0 );
    for( i = 0; i < ARRAY_SIZE(eventInspirations); i++ )
        comboboxTitle_add( &comboInspiration, 0, NULL, eventInspirations[i].text, eventInspirations[i].title, eventInspirations[i].id, CLR_WHITE, 0, 0);

    comboboxTitle_add( &comboMap, 1, NULL, "RandomMapString", "RandomMapString", 9999, CLR_WHITE,0,0 );
    for( i = 0; i < eaSize(&g_arenamaplist.maps); i++ )
        comboboxTitle_add( &comboMap, 0, NULL, g_arenamaplist.maps[i]->displayName, g_arenamaplist.maps[i]->displayName, i, CLR_WHITE,0,0 );


    // create the element lists for the player comboboxs (of which there could be hundreds)
    for( i = 0; i < ARRAY_SIZE(participantTeam);   i++ )
        comboboxSharedElement_add( &teamChoices, uiGetArchetypeIconByName(participantTeam[i].icon), participantTeam[i].text,   participantTeam[i].title,   participantTeam[i].id, NULL );
    for( i = 0; i < ARRAY_SIZE(participantClass);  i++ )
        comboboxSharedElement_add( &classChoices, uiGetArchetypeIconByName(participantClass[i].icon), participantClass[i].text,  participantClass[i].title,  participantClass[i].id, NULL );

    // make sure this exists
    if( !gArenaDetail.participants )
        eaCreate(&gArenaDetail.participants);

    //tooltips
    addWindowToolTip( R10, R10,       150, 20, "arenaEventTip",    &arenaSettingsParent, MENU_GAME, WDW_ARENA_CREATE, 0 );
    addWindowToolTip( R10, R10+30, 150, 20, "arenaDurationTip",    &arenaSettingsParent, MENU_GAME, WDW_ARENA_CREATE, 0 );
    addWindowToolTip( R10, R10+60, 150, 20, "arenaLevelTip",    &arenaSettingsParent, MENU_GAME, WDW_ARENA_CREATE, 0 );
    addWindowToolTip( R10*2 + 150, R10+30, 150, 20, "arenaTeamTip", &arenaSettingsParent, MENU_GAME, WDW_ARENA_CREATE, 0 );

    arena_initTextEntryFields();
}

//--------------------------------------------------------------------------------------------
// PARTICIPANT MANAGEMENT ////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------

// find a UIparticipant
static UIArenaParticipant * uiArenaParticipantFind( int id )
{
    int i;
    for(i = eaSize( &uiArenaParticipant )-1; i >= 0; i--)
    {
        if( uiArenaParticipant[i]->id == id )
            return uiArenaParticipant[i];
    }

    return NULL;
}

// Find the index of a UIparticipant
static int uiArenaParticipantFindIdx( int id )
{
    int i;
    for(i = eaSize( &uiArenaParticipant )-1; i >= 0; i--)
    {
        if( uiArenaParticipant[i]->id == id )
            return i;
    }

    return -1;
}

static void uiArenaParticipantModify( int id, const char * arch, int team );
static void uiArenaParticipantRemove( int id );

static void uiArenaParticipantAdd( int id, const char * arch, int team )
{
    
    UIArenaParticipant *uiap = uiArenaParticipantFind( id );
    int i;

    if( uiap )
        uiArenaParticipantModify( id, arch, team );
    else
    {
        uiap = calloc( 1, sizeof(UIArenaParticipant) );

        comboboxTitle_init( &uiap->Archetype,    0, 0, 0, ARCH_COL_WD, 0, 300, WDW_ARENA_CREATE );
        comboboxTitle_init( &uiap->team,        0, 0, 0, TEAM_COL_WD, 0, 300, WDW_ARENA_CREATE );

          if( arch )
        {
            for( i = 0; i < ARRAY_SIZE(arenaArchetype); i++ )
            {
                if( stricmp( arenaArchetype[i], arch ) == 0 )
                    uiap->Archetype.currChoice    = i+1;
            }
        }
        else
            uiap->Archetype.currChoice = 0;

        uiap->team.currChoice        = team;

        uiap->id = id;

        eaPush( &uiArenaParticipant, uiap );
    }
}

static void uiArenaParticipantModify( int id, const char *arch, int team )
{
    UIArenaParticipant *apui =  uiArenaParticipantFind( id );

    if( apui ) 
    {
        int i;
        
        if( arch )
        {
            for( i = 0; i < ARRAY_SIZE(arenaArchetype); i++ )
            {
                if( stricmp( arenaArchetype[i], arch ) == 0 )
                    apui->Archetype.currChoice    = i+1;
            }
        }
        else
            apui->Archetype.currChoice    = 0;

        apui->team.currChoice        = team;
    }
    else
        uiArenaParticipantAdd( id, arch, team );
}

static void uiArenaParticipantRemove( int id )
{
    int idx =  uiArenaParticipantFindIdx( id );

    if( idx >= 0 )
    {
        UIArenaParticipant * uiap = eaRemove( &uiArenaParticipant, idx );
        if( uiap )
        {
            comboboxClear(&uiap->team);
            comboboxClear(&uiap->Archetype);
            free( uiap );
        }
    }
}

// Applies the event settings to all UI data
static void arenaUpdateParticipantUI(void)
{
    int i, size;

    if( uiArenaParticipant )
    {
        eaDestroyEx(&uiArenaParticipant, NULL );
    }

    eaCreate(&uiArenaParticipant);

    size = eaSize(&gArenaDetail.participants);
    for( i = 0; i < size; i++ )
    {
        ArenaParticipant * ap = gArenaDetail.participants[i];

         if( ap->dbid )
             uiArenaParticipantModify( i, ap->archetype, ap->side );
        else
//            uiArenaParticipantModify( i, ap->desired_archetype, ap->desired_team ? ap->desired_team : ap->side);
            uiArenaParticipantModify( i, ap->desired_archetype, ap->desired_team);
    }
}

// Applies UI data to the event data
static void arenaUpdateParticipants(void)
{
    int i;
    for( i = eaSize(&gArenaDetail.participants)-1; i >= 0; i-- )
    {
        ArenaParticipant * ap = gArenaDetail.participants[i];
        
        if( ap->dbid )
        {
            ap->side = uiArenaParticipant[i]->team.currChoice;
        }
        else
        {
            if( uiArenaParticipant[i]->Archetype.currChoice )
                ap->desired_archetype = arenaArchetype[uiArenaParticipant[i]->Archetype.currChoice-1];
            else
                ap->desired_archetype = NULL;
            ap->desired_team = uiArenaParticipant[i]->team.currChoice;
            ap->id = i;
        }
    }
}

//--------------------------------------------------------------------------------------------
// UPDATE FUNCTIONS //////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------

// Applies All of the Event the UI.  Done after event is received from server
void arenaApplyEventToUI(void)
{
    int i, sanctioned;
    int creator = (playerPtr()->db_id == gArenaDetail.creatorid);
    char temp[12];
    
    if (!arena_settings_init)
    {
        // Can't legitimately touch any of the settings information until this has been called.
        arena_initSettings();
        arena_settings_init = true;
    }

    // update the player lists
    arenaUpdateParticipantUI();

    comboTeamStyle.currChoice = gArenaDetail.teamStyle;

    comboTeamType.currChoice = gArenaDetail.teamType;

    comboTeamCount.currChoice = (gArenaDetail.teamType != ARENA_SINGLE) ? gArenaDetail.numsides - 2 : 0;

    comboVictoryType.currChoice = gArenaDetail.victoryType;

    comboTournamentType.currChoice = gArenaDetail.tournamentType;

    comboWeight.currChoice = gArenaDetail.weightClass;

    sanctioned = gArenaDetail.verify_sanctioned;

    // team combobox for every player
    for (i = eaSize(&teamChoices) - 1; i >= 0; i--)
    {
        if (gArenaDetail.teamType != ARENA_SINGLE)
        {
            if (i && i <= comboTeamCount.currChoice + 2)
                teamChoices[i]->flags &= ~CCE_UNSELECTABLE;
            else
                teamChoices[i]->flags |= CCE_UNSELECTABLE;
        }
        else if (i)
            teamChoices[i]->flags |= CCE_UNSELECTABLE;
        else
            teamChoices[i]->flags &= ~CCE_UNSELECTABLE;
    }

    //Level ranges
    if( gArenaDetail.use_gladiators ) 
    {
        comboWeight.currChoice = ARENA_WEIGHT_ANY;
        comboWeight.unselectable = true; 
    }

    // victory value
    smf_SetRawText(labelVictoryValue, textStd(eventVictoryValueLabels[comboVictoryType.currChoice].valueLabel), false);

    if (!creator || gArenaDetail.lastupdate_cmd == CLIENTINP_ARENA_REQ_CREATE) // I seem to get a lot of race conditions while typing in the text field...
    {
        if (gArenaDetail.victoryValue || *textVictoryValue->rawString)
        {
            itoa(gArenaDetail.victoryValue, temp, 10);
            if (stricmp(textVictoryValue->rawString, temp))
            {
                smf_SetRawText(textVictoryValue, temp, false);
            }
        }
    }

    textVictoryValue->editMode = SMFEditMode_ReadOnly;
    textPlayerCount->editMode = SMFEditMode_ReadOnly;

    if (creator && !sanctioned && !gArenaDetail.leader_ready)
    {
        textPlayerCount->editMode = SMFEditMode_ReadWrite;

        if (eventVictoryValueLabels[comboVictoryType.currChoice].editable)
        {
            textVictoryValue->editMode = SMFEditMode_ReadWrite;
        }
    }

    // player count
    if (gArenaDetail.teamType != ARENA_SINGLE)
        smf_SetRawText(labelPlayerCount, textStd("PlayersPerTeam"), false);
    else
        smf_SetRawText(labelPlayerCount, textStd("MaxPlayers"), false);

    if (!creator || gArenaDetail.lastupdate_cmd == CLIENTINP_ARENA_REQ_CREATE)
    {
        int displayedPlayerCount;

        if (gArenaDetail.teamType != ARENA_SINGLE)
        {
            // Better to show wrong value of 0 instead of crashing due to divide by zero
            // gArenaDetail.numsides probably only has a value of 0 for a short time.
            if (gArenaDetail.numsides > 0)
            {
                displayedPlayerCount = gArenaDetail.maxplayers / gArenaDetail.numsides;
            }
            else
            {
                displayedPlayerCount = 0;
            }
        }
        else
        {
            displayedPlayerCount = gArenaDetail.maxplayers;
        }

        itoa(displayedPlayerCount, temp, 10);
        if (stricmp(textPlayerCount->rawString, temp))
        {
            smf_SetRawText(textPlayerCount, temp, false);
        }
    }


    // maps
    if (comboMap.elements)
    {
        combobox_setChoiceCur(&comboMap, gArenaDetail.mapid);
    }

    // inspiration setting
    comboInspiration.currChoice = gArenaDetail.inspiration_setting;
}

// Applies any changed made in the UI to the event.  Done prior to sending update
static void arenaApplyUIToEvent(void)
{
    // event
    gArenaDetail.teamStyle = comboTeamStyle.currChoice;
    gArenaDetail.teamType = comboTeamType.currChoice;
    gArenaDetail.weightClass = comboWeight.currChoice;
    gArenaDetail.mapid = comboMap.elements[comboMap.currChoice]->id;
    gArenaDetail.victoryType = comboVictoryType.currChoice;
    gArenaDetail.victoryValue = atoi(textVictoryValue->outputString);
    gArenaDetail.tournamentType = comboTournamentType.currChoice;
    gArenaDetail.inspiration_setting = comboInspiration.currChoice;

    if (gArenaDetail.teamType == ARENA_SINGLE)
    {
        gArenaDetail.teamStyle = ARENA_FREEFORM;
        gArenaDetail.maxplayers = atoi(textPlayerCount->outputString);
    }
    else
    {
        gArenaDetail.numsides = comboTeamCount.currChoice + 2;
        gArenaDetail.maxplayers = atoi(textPlayerCount->outputString) * gArenaDetail.numsides;
    }
    // the other variables are updated already

    // arena settings
    if( gArenaDetail.verify_sanctioned )
    {
        gArenaDetail.no_travel_suppression    = false;
        gArenaDetail.tactical_start            = false;
        gArenaDetail.no_end                    = false;
        gArenaDetail.no_diminishing_returns    = false;
        gArenaDetail.no_heal_decay            = false;
        gArenaDetail.no_pool                = false;
        gArenaDetail.no_travel                = false;
        gArenaDetail.inspiration_setting    = ARENA_NORMAL_INSPIRATIONS;
        gArenaDetail.enable_temp_powers        = false;
        gArenaDetail.no_stealth                = false;
    }

    if( gArenaDetail.use_gladiators )
    {
        gArenaDetail.no_travel_suppression    = false;
        gArenaDetail.tactical_start            = false;
        gArenaDetail.no_end                    = false;
        gArenaDetail.no_diminishing_returns    = false;
        gArenaDetail.no_heal_decay            = false;
        gArenaDetail.no_pool                = false;
        gArenaDetail.no_travel                = false;
        gArenaDetail.inspiration_setting    = ARENA_NORMAL_INSPIRATIONS;
        gArenaDetail.enable_temp_powers        = false;
        gArenaDetail.no_stealth                = false;
        gArenaDetail.weightClass            = ARENA_WEIGHT_ANY;
    }
}


//--------------------------------------------------------------------------------------------
// Arena Settings ////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------

#define MAX_SOLO_PLAYERS 64
#define MIN_PLAYERS 2

static void drawArenaSettings(void)
{
    float x, y, z, wd, ht, sc;
    int i = 0, color, bcolor;
    CBox box;

    int creator = ( playerPtr()->db_id == gArenaDetail.creatorid );

    window_getDims( WDW_ARENA_CREATE, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor );

     font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );

    // team type combo box
    if (playerPtr()->supergroup_id)
        comboTeamType.elements[ARENA_SUPERGROUP]->flags &= ~CCE_UNSELECTABLE;
    else
        comboTeamType.elements[ARENA_SUPERGROUP]->flags |= CCE_UNSELECTABLE;

    comboTeamType.unselectable = !creator || gArenaDetail.leader_ready;
    combobox_display( &comboTeamType );

    if (gArenaDetail.teamType != ARENA_SINGLE)
    {
        // team style combo box
        comboTeamStyle.unselectable = !creator || gArenaDetail.leader_ready;
        combobox_display( &comboTeamStyle );

        // team count combo box
        comboTeamCount.unselectable = !creator || gArenaDetail.leader_ready;
        combobox_display( &comboTeamCount );
    }

    // victory type combo box
    comboVictoryType.unselectable = !creator || gArenaDetail.sanctioned || gArenaDetail.leader_ready;
    combobox_display( &comboVictoryType );

    // tournament type combo box
    comboTournamentType.unselectable = !creator || gArenaDetail.leader_ready;
    combobox_display( &comboTournamentType );
 
    // map combo box
    comboMap.unselectable = !creator || gArenaDetail.leader_ready;
    combobox_display( &comboMap );

    // weight combo box
    comboWeight.unselectable = !creator || gArenaDetail.leader_ready || gArenaDetail.use_gladiators;
    combobox_display( &comboWeight );
    
    // see if any of the comboboxes was changed
      creator_update |= comboTeamStyle.changed | comboVictoryType.changed | 
                        comboTeamType.changed | comboTeamCount.changed | 
                        comboWeight.changed | comboMap.changed | comboTournamentType.changed;
 
    // options button
    if (D_MOUSEHIT == drawStdButton(x + 570*sc, y + 23*sc, z, 160*sc, 24*sc, color, "ArenaOptionsString", 1.f, 0))
    {
        window_setMode(WDW_ARENA_OPTIONS, WINDOW_GROWING);
        window_setMode(WDW_ARENA_CREATE, WINDOW_SHRINKING);
    }

    // victory value text entry field
    smf_Display(labelVictoryValue, x + wd / 2 - 145 * sc, y + 45 * sc, z, 90, 0, 0, 0, &gTextAttr_Chat, 0);
    if (creator && eventVictoryValueLabels[comboVictoryType.currChoice].editable && !gArenaDetail.verify_sanctioned)
    {
        BuildCBox(&box, x + wd / 2 - 50 * sc - R10, y + 42 * sc, 25 + 2 * R10, 24);
        if (mouseClickHit(&box, MS_LEFT) && !smfBlock_HasFocus(textVictoryValue))
        {
            smf_SelectAllText(textVictoryValue);
        }
        if (creator && !gArenaDetail.leader_ready)
        {
            drawFrame(PIX3, R10, x + wd / 2 - 40 * sc - R10, y + 42 * sc, z, 25 + 2 * R10, 24, sc, color, 0x000000ff);
            if (smfBlock_HasFocus(textVictoryValue))
            {
                drawFrame(PIX3, R10, x + wd / 2 - 40 * sc - R10, y + 42 * sc, z, 25 + 2 * R10, 24, sc, 0, 0x00ff003f);
            }
        }
    }
    if (eventVictoryValueLabels[comboVictoryType.currChoice].editable)
    {
        smf_Display(textVictoryValue, x + wd / 2 - 40 * sc, y + 45 * sc, z, 25, 0, 0, 0, &gTextAttr_Chat, 0);
    }
    creator_update |= textVictoryValue->editedThisFrame;

    // player count text entry field
    smf_Display(labelPlayerCount, x + wd / 2 - 145 * sc, y + 75 * sc, z, 90, 0, 0, 0, &gTextAttr_Chat, 0);
    if (creator)
    {
        BuildCBox(&box, x + wd / 2 - 50 * sc - R10, y + 72 * sc, 25 + 2 * R10, 24);
        if (mouseClickHit(&box, MS_LEFT) && !smfBlock_HasFocus(textPlayerCount))
        {
            smf_SelectAllText(textPlayerCount);
        }
        if (creator && !gArenaDetail.leader_ready)
        {
            drawFrame(PIX3, R10, x + wd / 2 - 40 * sc - R10, y + 72 * sc, z, 25 + 2 * R10, 24, sc, color, 0x000000ff);
            if (smfBlock_HasFocus(textPlayerCount))
            {
                drawFrame(PIX3, R10, x + wd / 2 - 40 * sc - R10, y + 72 * sc, z, 25 + 2 * R10, 24, sc, 0, 0x00ff003f);
            }
        }
    }
    smf_Display(textPlayerCount, x + wd / 2 - 40 * sc, y + 75 * sc, z, 25, 0, 0, 0, &gTextAttr_Chat, 0);
    creator_update |= textPlayerCount->editedThisFrame;

    // Gladiator checkbox
    creator_update |= drawSmallCheckbox(WDW_ARENA_CREATE, 497*sc, 53*sc, "GladiatorOptionString", &gArenaDetail.use_gladiators, NULL, creator && !gArenaDetail.leader_ready, NULL, NULL);
}

//--------------------------------------------------------------------------------------------
// Arena Player List /////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------

// This is what everyone sees for an active player
static void drawActivePlayer( float wx, float wy, float x, float y, float z, float sc, int color, ArenaParticipant *ap, UIArenaParticipant *uiap )
{
     ComboBox * teamCombo = &uiap->team;
    ComboBox * archCombo = &uiap->Archetype;
     Entity * e = playerPtr(), *player = entFromDbId(ap->dbid);
     int effective_level = ap->level, click_info = false;
    int info_color = 0xffffff88;
    AtlasTex *star = atlasLoadTexture( "MissionPicker_icon_star.tga" );
    AtlasTex *round = atlasLoadTexture("MissionPicker_icon_round.tga");
    AtlasTex *info = atlasLoadTexture("MissionPicker_icon_info_overlay.tga");

    // leader star or info icon
      if( player )
    {
        click_info = true;
        info_color = CLR_WHITE;
    }
    if( gArenaDetail.creatorid == ap->dbid )
    {
        float scale = ((float)ICON_COL_WD/star->width)*sc;
         if( D_MOUSEHIT == drawGenericButton( star, wx+x, wy+y, z, scale, info_color, info_color, click_info ) )
        {
            info_Entity( player, INFO_TAB_ARENA );
        }
    }
    else
    {
         float scale = ((float)ICON_COL_WD/round->width)*sc;
        if( D_MOUSEHIT == drawGenericButton( round, wx+x, wy+y, z, scale, color, color, click_info ) )
        {
            info_Entity( player, INFO_TAB_ARENA );
        }
        display_sprite( info, wx+x, wy+y, z, scale, scale, info_color );
    }

    x+=ICON_COL_WD*sc;

    font( &game_12 );
    font_color(CLR_WHITE, CLR_WHITE);

    //Name
       cprntEx( wx+x+R10*sc, wy+y+17*sc, z, sc, sc, NO_MSPRINT, ap->name );
    x += NAME_COL_WD*sc;
 
    //Team
       combobox_setLoc( teamCombo, x/sc, y/sc, 0 );
      teamCombo->elements = teamChoices;
    teamCombo->unselectable = (    gArenaDetail.teamType == ARENA_SINGLE || // only solo choice
                                gArenaDetail.teamType == ARENA_SUPERGROUP ||  // supergroup rumble
                                comboTeamStyle.elements[comboTeamStyle.currChoice]->id == ARENA_STAR || // hero star event
                                comboTeamStyle.elements[comboTeamStyle.currChoice]->id == ARENA_STAR_VILLAIN || // villain star event
                                comboTeamStyle.elements[comboTeamStyle.currChoice]->id == ARENA_STAR_VERSUS || // H vs V star event
                                comboTeamStyle.elements[comboTeamStyle.currChoice]->id == ARENA_7STAR || // hero 7star event
                                comboTeamStyle.elements[comboTeamStyle.currChoice]->id == ARENA_7STAR_VILLAIN || // villain 7star event
                                comboTeamStyle.elements[comboTeamStyle.currChoice]->id == ARENA_7STAR_VERSUS || // H vs V 7star event
                                (e->db_id != gArenaDetail.creatorid && e->db_id != ap->dbid) || //no access
                                gArenaDetail.leader_ready); // leader is ready
    combobox_display(teamCombo);
    x += TEAM_COL_WD*sc;

    font( &game_12 );
      if( teamCombo->changed )
    {
        player_update = ap;
        ap->side = teamCombo->currChoice;
    }

    //Level
    if( comboWeight.currChoice )
    {
        if (effective_level < g_weightRanges[comboWeight.currChoice].minLevel)
            font_color( CLR_RED, CLR_RED );
        if( effective_level > g_weightRanges[comboWeight.currChoice].maxLevel )
        {
            font_color( CLR_WHITE, CLR_BLUE );
            effective_level = g_weightRanges[comboWeight.currChoice].maxLevel;
        }
    }
     cprnt( wx+x+LVL_COL_WD*sc/2, wy+y+17*sc, z, sc, sc, "%i", effective_level+1 );
     x += (LVL_COL_WD)*sc;

     font_color(CLR_WHITE,CLR_WHITE);

    //Arch
    combobox_setLoc( archCombo, x/sc, y/sc, 0 );
    archCombo->elements = classChoices;
    archCombo->unselectable = true;
    combobox_display(archCombo);
    x += ARCH_COL_WD*sc;

    //Kick Button
     if( gArenaDetail.creatorid == e->db_id && ap->dbid != e->db_id  )
    {
             if( D_MOUSEHIT == drawStdButton( wx+x + KICK_COL_WD*sc/2, wy+y+PLAYER_HT*sc/2, z, (KICK_COL_WD-10)*sc, (PLAYER_HT-5)*sc, color, "KickString", .9f, 0 ) )
        {
            arenaSendPlayerDrop( &gArenaDetail, ap->dbid );
        }
    }
}

static void drawInactivePlayer( float wx, float wy, float x, float y, float z, float sc, ArenaParticipant *ap, UIArenaParticipant * uiap, int idx )
{
    ComboBox * teamCombo    = &uiap->team;
    ComboBox * archCombo    = &uiap->Archetype;
     Entity * e = playerPtr();

    x+=ICON_COL_WD*sc;

    //playerChoice
     font(&game_12);
    font_color(CLR_WHITE,CLR_WHITE);
    prnt( wx+x+R10*sc, wy+y+17*sc, z, sc, sc, "Empty" );
    x += NAME_COL_WD*sc;

    if( comboTeamStyle.elements[comboTeamStyle.currChoice]->id == ARENA_STAR ||
        comboTeamStyle.elements[comboTeamStyle.currChoice]->id == ARENA_STAR_VILLAIN ||
        comboTeamStyle.elements[comboTeamStyle.currChoice]->id == ARENA_STAR_VERSUS ||
        comboTeamStyle.elements[comboTeamStyle.currChoice]->id == ARENA_7STAR ||
        comboTeamStyle.elements[comboTeamStyle.currChoice]->id == ARENA_7STAR_VILLAIN ||
        comboTeamStyle.elements[comboTeamStyle.currChoice]->id == ARENA_7STAR_VERSUS ||
        e->db_id != gArenaDetail.creatorid || gArenaDetail.leader_ready )
    {
        teamCombo->unselectable = true;
        archCombo->unselectable = true;
    }
    else
    {
        teamCombo->unselectable = false;
        archCombo->unselectable = false;
    }

    //TeamChoice
     combobox_setLoc( teamCombo, x/sc, y/sc, 0 ); 
    teamCombo->elements = teamChoices;
    teamCombo->unselectable |= (gArenaDetail.teamType == ARENA_SINGLE || gArenaDetail.teamType == ARENA_SUPERGROUP || gArenaDetail.leader_ready);
    combobox_display(teamCombo);
    x += TEAM_COL_WD*sc;

    //LevelChoice
     if( comboWeight.currChoice )
        cprnt( wx+x+LVL_COL_WD*sc/2, wy+y+17*sc, z, sc, sc, "%i-%i", 
            g_weightRanges[comboWeight.currChoice].minLevel+1, 
            g_weightRanges[comboWeight.currChoice].maxLevel+1);
    else
        cprnt( wx+x+LVL_COL_WD*sc/2, wy+y+17*sc, z, sc, sc, "AnyLevel" );
    x += LVL_COL_WD*sc;

    //ArchChoice

    if( gArenaDetail.use_gladiators ) 
    {
        archCombo->currChoice = 0; // 0 == Any Class
        archCombo->unselectable = true;
    }


     combobox_setLoc( archCombo, x/sc, y/sc, 0 );
    archCombo->elements = classChoices;
    combobox_display(archCombo);

    if( (teamCombo->changed || archCombo->changed) )
    {
        player_update = ap;
        player_update->id = idx;
        player_update->desired_team = teamCombo->currChoice;
        if( archCombo->currChoice )
            player_update->desired_archetype = arenaArchetype[archCombo->currChoice-1];
        else
            player_update->desired_archetype = NULL;
    }
}

static void drawPlayerList( float offset )
{
    float tipx = R10, x, y, z, wd, ht, sc;
    int color, bcolor, i, j, count=0;
    Entity * e = playerPtr();
    CBox box;

       window_getDims( WDW_ARENA_CREATE, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor );

    addWindowToolTip( tipx, ARENA_SETTING_HT*sc, ICON_COL_WD*sc, ht - (ARENA_SETTING_HT+ARENA_CHAT_HT)*sc, "arenaIconTip", &arenaPlayerListParent, MENU_GAME, WDW_ARENA_CREATE, 0 );
    tipx += ICON_COL_WD;
    addWindowToolTip( tipx, ARENA_SETTING_HT*sc, NAME_COL_WD*sc, ht - (ARENA_SETTING_HT+ARENA_CHAT_HT)*sc, "arenaNameTip", &arenaPlayerListParent, MENU_GAME, WDW_ARENA_CREATE, 0 );
    tipx += NAME_COL_WD;
    addWindowToolTip( tipx, ARENA_SETTING_HT*sc, TEAM_COL_WD*sc, ht - (ARENA_SETTING_HT+ARENA_CHAT_HT)*sc, "arenaSideTip", &arenaPlayerListParent, MENU_GAME, WDW_ARENA_CREATE, 0 );
    tipx += TEAM_COL_WD;
    addWindowToolTip( tipx, ARENA_SETTING_HT*sc, LVL_COL_WD*sc, ht - (ARENA_SETTING_HT+ARENA_CHAT_HT)*sc, "arenaLvlTip", &arenaPlayerListParent, MENU_GAME, WDW_ARENA_CREATE, 0 );
    tipx += LVL_COL_WD;
    addWindowToolTip( tipx, ARENA_SETTING_HT*sc, ARCH_COL_WD*sc, ht - (ARENA_SETTING_HT+ARENA_CHAT_HT)*sc, "arenaArchTip", &arenaPlayerListParent, MENU_GAME, WDW_ARENA_CREATE, 0 );
    tipx += ARCH_COL_WD;

    // Now everyone else sorted by team
       for( i = 0; i <= gArenaDetail.numsides; i++ )
    {
        int prev_count = count;
        // for every player
          for( j = 0; j < eaSize(&gArenaDetail.participants); j++ )
        {
             if( !gArenaDetail.numsides || uiArenaParticipant[j]->team.currChoice == i )
            {
                   if( gArenaDetail.participants[j] && gArenaDetail.participants[j]->dbid )
                    drawActivePlayer( x, y, R10*sc, (ARENA_SETTING_HT+R10+PLAYER_HT*count)*sc - offset, z, sc, color, gArenaDetail.participants[j], uiArenaParticipant[j] );
                else
                    drawInactivePlayer( x, y, R10*sc, (ARENA_SETTING_HT+R10+PLAYER_HT*count)*sc - offset, z, sc, gArenaDetail.participants[j], uiArenaParticipant[j], j );
                count++;
            }
        }

        if( i > 0)
        {
               BuildCBox( &box, x, y - offset + (ARENA_SETTING_HT+R10+PLAYER_HT*prev_count)*sc, wd, PLAYER_HT*(count-prev_count)*sc  );
            drawBox( &box, z, 0, getTeamColor(i) );
        }
    }
}

//--------------------------------------------------------------------------------------------
// Arena Chat ////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------
chatQueue arenaChatQ;
static int reformatText;
UIEdit *arenaChat;
UIEdit *passwordChat;

#define CHAT_ENTRY_HT 25

void addArenaChatMsg( char * txt, int type )
{
    ChatLine * pLine = ChatLine_Create();
    pLine->pchText = strdup( txt ); 
    if( arenaChatQ.size >= 1000 )
    {
        ChatLine *pLine = eaRemove( &arenaChatQ.ppLines, 0 );
        ChatLine_Destroy(pLine);
    }

    eaPush( &arenaChatQ.ppLines, pLine );
    arenaChatQ.size = eaSize( &arenaChatQ.ppLines );
    reformatText = 1;
}

static void clearArenaChat()
{
    if( arenaChatQ.ppLines )
    {
        eaClearEx(&arenaChatQ.ppLines, ChatLine_Destroy);
        arenaChatQ.size = 0;
    }
}

static void arenaInputHandler(UIEdit* edit)
{
    KeyInput* input;

    // Handle keyboard input.
    input = inpGetKeyBuf();

     while(input )
    {
        if(input->type == KIT_EditKey)
        {
            switch(input->key)
            {
            case INP_ESCAPE:
                {
                    if( edit->contentCharacterCount > 0 )
                    {
                        uiEditReturnFocus(edit);
                    }
                }break;
            case INP_NUMPADENTER: /* fall through */
            case INP_RETURN:
                {
                    char buf[2000];
 
                    if( uiEditGetUTF8Text(edit) )
                    {
                        sprintf(buf, "arena_local [%s]: %s", playerPtr()->name, uiEditGetUTF8Text(edit));
                        cmdParse(buf);
                    }
                    uiEditClear(edit);
                    uiEditReturnFocus(edit);
                }break;
            default:
                uiEditDefaultKeyboardHandler(edit, input);
                break;
            }
        }
        else
            uiEditDefaultKeyboardHandler(edit, input);
        inpGetNextKey(&input);
    }
}

static void initArenaChat(void)
{
    if(!arenaChat)
    {
        arenaChat = uiEditCreateSimple(NULL, 255, &chatFont, CLR_WHITE, arenaInputHandler);
        uiEditSetClicks(arenaChat, "type2", "mouseover");

        uiEditDisallowReturns(arenaChat, 1);    // used to filter carriage returns on paste ops

        // This will have the edit box scroll horizontally.
        uiEditSetMinWidth(arenaChat, 9000);

        addArenaChatMsg( textStd("arenaEventChatIntro"), INFO_ARENA);
    }
}

static int drawArenaChat( float x, float y, float z, float wd, float ht, float sc )
{
    //pane/scrollbar
    int i, iFontHeight, retval = 0;

    static ScrollBar sb = { 0 };
    static float docHt = 0;
     float chatHeight = ht - CHAT_ENTRY_HT*sc;
    float curr_ht = 0;
    CBox box;

    initArenaChat();

    font(&chatFont);
    iFontHeight = ttGetFontHeight(font_grp, sc, sc);


    // Reformat all the text if new messages have been added
    docHt = 0;
    for(i=eaSize(&arenaChatQ.ppLines)-1; i>=0; i--)
    {
        if( reformatText )
        {
            SMFBlock *pBlock = arenaChatQ.ppLines[i]->pBlock;
            char * pchText = arenaChatQ.ppLines[i]->pchText;
            smf_SetFlags(pBlock, SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespaceWithInverseTabbing, 0, 0, 
                SMFScrollMode_ExternalOnly, SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, SMAlignment_Left, "arenaChat", 0, 0);
            smf_SetRawText(pBlock, pchText, false);
            arenaChatQ.ppLines[i]->ht = smf_ParseAndFormat(pBlock, pchText, 0, 0, 0, wd-(R10+PIX3)*2, -1, false, 1, 1, &gTextAttr_Chat, 0 ); 

            //Move the scroll bar to the bottom if some new text was added.
            if( !sb.grabbed )
                sb.offset = INT_MAX; // force to bottom
        }
            
        sb.offset += smf_GetInternalScrollDiff(arenaChatQ.ppLines[i]->pBlock);

        docHt += arenaChatQ.ppLines[i]->ht;
    }
    reformatText = 0;

    BuildCBox( &box, x, y+PIX3*sc, wd, chatHeight - PIX3*sc );
       doScrollBar( &sb, chatHeight - 4*PIX3*sc, ((docHt))+(PIX3*sc), x+wd, y + 4*PIX3*sc, z+2, &box, 0 );

    gTextAttr_Chat.piScale = (int *)((int)(sc*SMF_FONT_SCALE));
    for(i=0; i<eaSize(&arenaChatQ.ppLines); i++)
    {
        SMFBlock *pBlock = arenaChatQ.ppLines[i]->pBlock;
        char *pchText = arenaChatQ.ppLines[i]->pchText;
        smf_SetScissorsBox(pBlock, x, y+PIX3*sc, wd, chatHeight - PIX3*sc);
        curr_ht += smf_Display(pBlock, x + (R10+PIX3)*sc, y + curr_ht + iFontHeight - sb.offset, z+1, wd-(R10+PIX3)*2, -1, 
                                        0, 0, &gTextAttr_Chat, 0); 
    }

    //chat entry box
    // ok now we can do the happy text, start out with the edit text panel
    if(arenaChat)
    {
        UIBox chatEditBounds;
        CBox box;

           uiBoxDefine(&chatEditBounds, x+str_wd(font_grp, sc, sc, "EventChat")+(R10+PIX3*2)*sc, y+ht-(CHAT_ENTRY_HT-PIX3)*sc, wd - (R10+PIX3*2)*sc-str_wd(font_grp, sc, sc, "EventChat"), (CHAT_ENTRY_HT-2*PIX3)*sc);
        BuildCBox( &box, x+PIX3*2*sc, y+ht-(CHAT_ENTRY_HT-PIX3)*sc, wd - PIX3*2*sc, (CHAT_ENTRY_HT-2*PIX3)*sc);
 
        if(arenaChat->inEditMode)
               drawFrame( PIX3, R10, x, y + ht - (CHAT_ENTRY_HT)*sc , z + 1, wd, CHAT_ENTRY_HT*sc, sc, 0, 0x00ff003f );

        if( mouseCollision(&box) )
        {
            if( mouseDown( MS_LEFT ))
            {
                uiEditTakeFocus(arenaChat);
            }
        }
        else if ( mouseDown( MS_LEFT ) ) 
        {
            char buf[2000];

             if( uiEditGetUTF8Text(arenaChat) )
            {
                 sprintf(buf, "arena_local [%s]:%s", playerPtr()->name, uiEditGetUTF8Text(arenaChat));
                cmdParse(buf);
            }
             uiEditClear(arenaChat);
            uiEditReturnFocus(arenaChat);
        }


          uiEditSetBounds(arenaChat, chatEditBounds);
        uiEditSetFontScale(arenaChat, sc);
        arenaChat->z = z+1;
        uiEditProcess(arenaChat);
        prnt( x + (PIX3+R10)*sc, y+ht-(2*PIX3+1)*sc, z+2, sc, sc, "EventChat" );
    }

    return retval;
}

//--------------------------------------------------------------------------------------------
// Arena Buttons /////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------

static void drawArenaButtons( float x, float y, float z, float sc, float wd, float ht, int color, float winht )
{
    int i, dbid = playerPtr()->db_id;
    ArenaParticipant * ap = 0, *creator = 0;
    char* readyStr = NULL;
    float scale;
    AtlasTex * round;
    bool displayGoButton = true;

    // find the creator and player participants
      for( i = eaSize( &gArenaDetail.participants)-1; i >= 0; i-- )
    {
        if( gArenaDetail.participants[i]->dbid == dbid )
            ap = gArenaDetail.participants[i];
        if( gArenaDetail.participants[i]->dbid == gArenaDetail.creatorid )
            creator = gArenaDetail.participants[i];
    }

    if( gArenaDetail.creatorid == dbid )
    {
        creator_update |= drawSmallCheckbox( WDW_ARENA_CREATE, (ARENA_CHAT_WD+R10)*sc + 100*sc, (y-winht) + ht - 90*sc, "SanctionedString", &gArenaDetail.verify_sanctioned,    NULL, creator && !gArenaDetail.leader_ready, "arenaSanctionTip", &arenaSettingsParent );
           creator_update |= drawSmallCheckbox( WDW_ARENA_CREATE, (ARENA_CHAT_WD+R10)*sc,            (y-winht) + ht - 90*sc, "InviteOnly",        &gArenaDetail.inviteonly,            NULL, creator && !gArenaDetail.leader_ready, NULL, NULL );
    }

    addToolTipEx( x+wd/2 - 65*sc, y+ht - 42*sc, 130*sc, 30*sc, "arenaQuitTip",        &arenaButtonsParent, MENU_GAME, WDW_ARENA_CREATE, 0 );

    // cancel
     if( D_MOUSEHIT == drawStdButton( x + wd/2 - 15*sc, y + ht - 27*sc, z, 100*sc, 30*sc, CLR_RED, "QuitString", 1.f, 0 ) )
    {
        arenaSendPlayerDrop(&gArenaDetail, dbid);
        window_setMode(WDW_ARENA_CREATE, WINDOW_SHRINKING);
        window_setMode(WDW_ARENA_OPTIONS, WINDOW_SHRINKING);
    }

    // choose the ready string
    if (window_getMode(WDW_ARENA_OPTIONS) == WINDOW_DISPLAYING)
    {
        // Guests in the room get this, not just the host/creator!
        if (gArenaDetail.creatorid != dbid)
        {
            readyStr = textStd("ArenaReturnToRoomString");
            addToolTipEx( x+wd/2 - 65*sc, y+ht - 80*sc, 130*sc, 30*sc, "arenaReturnToRoomTip",        &arenaButtonsParent, MENU_GAME, WDW_ARENA_OPTIONS, 0 );
        }
        else
        {
            readyStr = textStd("ArenaAcceptOptionsString");
            addToolTipEx( x+wd/2 - 65*sc, y+ht - 80*sc, 130*sc, 30*sc, "arenaAcceptOptionsTip",        &arenaButtonsParent, MENU_GAME, WDW_ARENA_OPTIONS, 0 );
        }
    }
    else if (gArenaDetail.creatorid != dbid)
    {
        displayGoButton = false;
    }
     else if( gArenaDetail.start_time )
    {
        int seconds = gArenaDetail.start_time-timerSecondsSince2000WithDelta();
        int minutes = MAX(0,seconds/60);
        seconds = MAX( 0, seconds - minutes*60);

        readyStr = textStd("CancelPlusTime", minutes, seconds);
        clearToolTipEx( "arenaListEventTip" );
        addToolTipEx( x+wd/2 - 65*sc, y+ht - 80*sc, 130*sc, 30*sc, "arenaStartTip",        &arenaButtonsParent, MENU_GAME, WDW_ARENA_CREATE, 0 );
    }
    else if( gArenaDetail.listed || gArenaDetail.inviteonly )
    {
        readyStr = textStd("StartEvent");
        clearToolTipEx( "arenaListEventTip" );
        addToolTipEx( x+wd/2 - 65*sc, y+ht - 80*sc, 130*sc, 30*sc, "arenaStartTip",        &arenaButtonsParent, MENU_GAME, WDW_ARENA_CREATE, 0 );
    }
    else
    {
        readyStr = textStd("ListEvent");
        clearToolTipEx( "arenaStartTip" );
        addToolTipEx( x+wd/2 - 65*sc, y+ht - 80*sc, 130*sc, 30*sc, "arenaListEventTip", &arenaButtonsParent, MENU_GAME, WDW_ARENA_CREATE, 0 );
    }

    if( ap )
    {
        if (displayGoButton)
        {
            int victoryValue = atoi(textVictoryValue->outputString);
            int victoryValueIsLegit = !eventVictoryValueLabels[comboVictoryType.currChoice].editable || (victoryValue >= eventVictoryValueLabels[comboVictoryType.currChoice].minimumValue && victoryValue <= eventVictoryValueLabels[comboVictoryType.currChoice].maximumValue) || gArenaDetail.verify_sanctioned;
            int grey = window_getMode(WDW_ARENA_OPTIONS) != WINDOW_DISPLAYING && 
                            ((gArenaDetail.inviteonly || gArenaDetail.listed) && 
                             (gArenaDetail.cannot_start || 
                              (gArenaDetail.verify_sanctioned && !gArenaDetail.sanctioned)) || 
                              (!victoryValueIsLegit && gArenaDetail.listed));
            //go button
            if( D_MOUSEHIT == drawStdButton( x + wd/2, y + ht - 65*sc, z, 130*sc, 30*sc, CLR_GREEN, readyStr, 1.f, grey ) )
            {
                   creator_update = true;

                if (window_getMode(WDW_ARENA_OPTIONS) == WINDOW_DISPLAYING)
                {
                    window_setMode(WDW_ARENA_OPTIONS, WINDOW_SHRINKING);
                    window_setMode(WDW_ARENA_CREATE, WINDOW_GROWING);
                }
                else if (!gArenaDetail.listed)
                {
                    gArenaDetail.listed = true;
                }
                else if (gArenaDetail.leader_ready)
                {
                    gArenaDetail.leader_ready = false;
                }
                else
                {
                    gArenaDetail.leader_ready = true;
                    gArenaDetail.inviteonly = true;
                }
            }
        }
        else
        {
            font(&game_18);

            if( gArenaDetail.start_time )
            {
                int seconds = gArenaDetail.start_time-timerSecondsSince2000WithDelta();
                int minutes = MAX( 0, seconds/60);
                float col_pct = 0;
                int f_color = CLR_WHITE;
                static float timer = 0;
                static int last_second = 0;

                seconds = MAX( 0, seconds - minutes*60);
                timer += TIMESTEP/(2*PI);
                col_pct = (sinf(timer)+1)/2;

                if (seconds != last_second && seconds < 10 && minutes == 0)
                {
                    sndPlay( "2beep", SOUND_GAME );
                }
                last_second = seconds;

                interp_rgba( col_pct, CLR_WHITE, CLR_GREEN, &f_color );
                font_color( f_color, f_color );

                cprntEx( x + wd/2, y + ht - 70*sc, z, 1.f, 1.f, CENTER_X|CENTER_Y, "%i:%02i", minutes, seconds );
            }
            else
            {
                font_color( 0xffffff88, 0xffffff88 );
                cprntEx( x + wd/2, y + ht - 70*sc, z, .8f, .8f, CENTER_X|CENTER_Y, "NegotiatingStr"  );
            }
        }
    }

    font_color(CLR_WHITE, CLR_WHITE);
    round = atlasLoadTexture("MissionPicker_icon_round.tga");
    scale = ((float)30/round->width)*sc;
    cprntEx( x+wd+15*sc - (52)*sc, y+ht+15*sc - (42)*sc, z+1, 1.f, 1.f, CENTER_X|CENTER_Y, "?" );
    if( D_MOUSEHIT == drawGenericButton( round, x+wd - (52)*sc, y+ht - (42)*sc, z, scale, color, color, true ) )
    {
        helpSet(9);
    }
}

#define PULSE_SPEED .1
static int arenaProblemsString( float x, float y, float z, float wd, float ht, float sc )
{
    static SMFView *s_pview;
    static float timer = 0;
    int victoryValue = atoi(textVictoryValue->outputString);
    int victoryValueIsLegit = !eventVictoryValueLabels[comboVictoryType.currChoice].editable || (victoryValue >= eventVictoryValueLabels[comboVictoryType.currChoice].minimumValue && victoryValue <= eventVictoryValueLabels[comboVictoryType.currChoice].maximumValue);

    char buf[1024] = {0};
    int i, text_ht = 15, green_val;

    timer += PULSE_SPEED*TIMESTEP;
       green_val = ((int)(((sinf(timer)+1)/2)*255))<<16;

       if( gArenaDetail.sanctioned ) // server gave us all clear
    {
        font_color( CLR_GREEN, CLR_GREEN );
        prnt( x + R10, y+ht-ARENA_CHAT_HT, z, 1.f, 1.f, "EventIsSanctioned" );
    }
    else if( gArenaDetail.cannot_start || !victoryValueIsLegit) // somethings wrong
    {

        if( gArenaDetail.verify_sanctioned )
            strcat( buf, textStd("UnsanctionedReasons") );
        else
            strcat( buf, textStd("EventCannotStart") );

        if (gArenaDetail.cannot_start)
        {
            for( i = 0; i < ARENA_NUM_PROBLEMS; i++) // cat together the reasons
            {
                if( gArenaDetail.eventproblems[i][0] )
                {
                    strcat( buf, textStd(gArenaDetail.eventproblems[i]) );
                    strcat( buf, ", ");
                }        
            }
        }
        if (!victoryValueIsLegit && !gArenaDetail.verify_sanctioned)
        {
            if (eventVictoryValueLabels[comboVictoryType.currChoice].victoryType == ARENA_TIMED)
            {
                strcat(buf, textStd("ArenaBadTimedVictoryValue"));
            }
            else if (eventVictoryValueLabels[comboVictoryType.currChoice].victoryType == ARENA_STOCK ||
                     eventVictoryValueLabels[comboVictoryType.currChoice].victoryType == ARENA_TEAMSTOCK)
            {
                strcat(buf, textStd("ArenaBadStockVictoryValue"));
            }
            else if (eventVictoryValueLabels[comboVictoryType.currChoice].victoryType == ARENA_KILLS)
            {
                strcat(buf, textStd("ArenaBadKillsVictoryValue"));
            }
            else
            {
                strcat(buf, textStd("ArenaBadVictoryValue"));
            }
            strcat(buf, ", ");
        }
        buf[strlen(buf)-2] = '.'; // replace comma with period on last one

        if(s_pview==NULL)
        {
            s_pview = smfview_Create(0);
        }

         gTextAttr_Arena.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
        gTextAttr_Arena.piColor = (int*)((int)(CLR_RED|green_val)); // cycle red to yellow
        smfview_SetText(s_pview, buf);
        smfview_SetAttribs(s_pview, &gTextAttr_Arena);
         smfview_SetLocation(s_pview, 0, 0, 0);
        text_ht = smfview_GetHeight(s_pview);
          smfview_SetBaseLocation(s_pview, x+(PIX3+R10)*sc,y+ht-(ARENA_CHAT_HT)*sc-text_ht, z, wd-2*(R10+PIX3), 0);
         smfview_SetSize(s_pview, wd-2*R10, text_ht);
        smfview_Draw(s_pview);
    }
    else
        return 0;

    return text_ht;
}

int arenaOptionsWindow(void)
{
    float x, y, z, wd, ht, sc;
    int color, bcolor, creator, creator_txt = 0, update = 0;

    // Do everything common windows are supposed to do.
    if ( !window_getDims( WDW_ARENA_OPTIONS, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ))
    {
        return 0;
    }

    creator = (gArenaDetail.creatorid == playerPtr()->db_id);

    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );                                                // window 
    drawFrame( PIX3, R10, x, y+ht-ARENA_CHAT_HT*sc, z, wd, ARENA_CHAT_HT*sc, sc, color, 0 );                // chat level
    drawFrame( PIX3, R10, x, y+ht-ARENA_CHAT_HT*sc, z, ARENA_CHAT_WD*sc, ARENA_CHAT_HT*sc, sc, color, 0    );    // chat box
    drawFrame( PIX3, R10, x, y+ht-CHAT_ENTRY_HT*sc, z, ARENA_CHAT_WD*sc, CHAT_ENTRY_HT*sc, sc, color, 0    );    // chat entry

    if( gArenaDetail.eventid == 0 )
    {
        return 0;
    }

    // draw setting checkboxes
    creator_update |= drawSmallCheckbox( WDW_ARENA_OPTIONS, 20*sc, 22*sc, "NoTravelSuppression",        &gArenaDetail.no_travel_suppression,    NULL, creator && !gArenaDetail.verify_sanctioned && !gArenaDetail.leader_ready && !gArenaDetail.use_gladiators,    "arenaNoTravelSuppressionTip",    &arenaSettingsParent );
    gArenaDetail.tactical_start = false; // was the first checkbox previously.
    creator_update |= drawSmallCheckbox( WDW_ARENA_OPTIONS, 20*sc, 38*sc, "InfiniteEndurance",            &gArenaDetail.no_end,                    NULL, creator && !gArenaDetail.verify_sanctioned && !gArenaDetail.leader_ready && !gArenaDetail.use_gladiators,    "arenaNoEndTip",                &arenaSettingsParent );
    creator_update |= drawSmallCheckbox( WDW_ARENA_OPTIONS, 20*sc, 54*sc, "NoDiminishingReturns",        &gArenaDetail.no_diminishing_returns,    NULL, creator && !gArenaDetail.verify_sanctioned && !gArenaDetail.leader_ready && !gArenaDetail.use_gladiators,    "arenaNoDiminishingReturnsTip",    &arenaSettingsParent );
    creator_update |= drawSmallCheckbox( WDW_ARENA_OPTIONS, 20*sc, 70*sc, "NoHealDecay",                &gArenaDetail.no_heal_decay,            NULL, creator && !gArenaDetail.verify_sanctioned && !gArenaDetail.leader_ready && !gArenaDetail.use_gladiators,    "arenaNoHealDecayTip",            &arenaSettingsParent );
    gArenaDetail.no_stealth = false; // was the third checkbox previously.
    creator_update |= drawSmallCheckbox( WDW_ARENA_OPTIONS, 20*sc, 86*sc, "NoPoolPowers",                &gArenaDetail.no_pool,                    NULL, creator && !gArenaDetail.verify_sanctioned && !gArenaDetail.leader_ready && !gArenaDetail.use_gladiators,    "arenaNoPoolTip",                &arenaSettingsParent );
    creator_update |= drawSmallCheckbox( WDW_ARENA_OPTIONS, 20*sc, 102*sc, "NoTravelPowers",            &gArenaDetail.no_travel,                NULL, creator && !gArenaDetail.verify_sanctioned && !gArenaDetail.leader_ready && !gArenaDetail.use_gladiators,    "arenaNoTravelTip",                &arenaSettingsParent );
    creator_update |= drawSmallCheckbox( WDW_ARENA_OPTIONS, 20*sc, 118*sc, "EnableTempPowers",            &gArenaDetail.enable_temp_powers,        NULL, creator && !gArenaDetail.verify_sanctioned && !gArenaDetail.leader_ready && !gArenaDetail.use_gladiators,    "arenaEnableTempPowersTip",        &arenaSettingsParent );
    creator_update |= drawSmallCheckbox( WDW_ARENA_OPTIONS, 20*sc, 134*sc, "NoObserver",                &gArenaDetail.no_observe,                NULL, creator && !gArenaDetail.leader_ready,                                                                    "noObserverTip",                &arenaSettingsParent );

    // draw inspiration combo box
    comboInspiration.unselectable = !creator || gArenaDetail.leader_ready;
    combobox_display( &comboInspiration );
    creator_update |= comboInspiration.changed;

    //drawArenaChat - only when window is all the way open so text wrapping isn't lame
    if (window_getMode(WDW_ARENA_OPTIONS) == WINDOW_DISPLAYING)
    {
        drawArenaChat( x, y+ht-(ARENA_CHAT_HT)*sc, z, ARENA_CHAT_WD*sc, ARENA_CHAT_HT*sc, sc );
    }

    //drawArenaButtons
    drawArenaButtons( x + ARENA_CHAT_WD*sc, y + ht - ARENA_CHAT_HT*sc, z, sc, wd-ARENA_CHAT_WD*sc, ARENA_CHAT_HT*sc, color, y );

    //notify server of any changes
    if( creator_update && creator ) //Send full arena update to the server
    {
        arenaApplyUIToEvent();
        arenaSendCreatorUpdate( &gArenaDetail );
        creator_update = 0;
    }
    else if( player_update ) // Send an update on a specific player (assumes only one player can be changed at a time manually
    {
        arenaSendPlayerUpdate( &gArenaDetail, player_update );
        player_update = NULL;
    }

    return 0;
}

//--------------------------------------------------------------------------------------------
// Arena Create Window Main //////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------

int arenaCreateWindow(void)
{
    float x, y, z, wd, ht, sc;
    int color, bcolor, creator, creator_txt = 0, update = 0;
    CBox box;

    static ScrollBar playerSb = { WDW_ARENA_CREATE };

    // display countdown for arena start
    if(arenaCountdown)
    {
        char countdown[10];
        int winwd, winht;
        unsigned long curTime;

        curTime = timerCpuTicks();
        if(timerSeconds(curTime - arenaCountdownMomentInitial) > (arenaCountdownTimerInitial - arenaCountdownTimerCurrent))
        {
            if (arenaCountdownTimerCurrent <= 0) {
                arenaCountdown = false;
            } else {
                if (arenaCountdownTimerCurrent % 10 == 0 && arenaCountdownTimerCurrent != ARENA_BUFF_TIMER)
                {
                    sprintf(countdown, "%d", arenaCountdownTimerCurrent);
                    windowClientSizeThisFrame( &winwd, &winht );
                    fadingTextEx( winwd/2, winht/2, 5000, 8.0, CLR_RED, CLR_RED, CENTER_X|CENTER_Y, &title_18, 30, 0, countdown, .1f );
                    sndPlay("BeepWarning", SOUND_GAME);
                }
                arenaCountdownTimerCurrent--;
            }
        }
    }
 
    playArenaStatsLoop();

    if (!arena_detail_init)
    {
        arena_initDetails();
        arena_detail_init = true;
    }

    // Do everything common windows are supposed to do.
       if ( !window_getDims( WDW_ARENA_CREATE, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ))
    {
        if (window_getMode(WDW_ARENA_OPTIONS) != WINDOW_DISPLAYING)
        {
            clearArenaChat();
            uiEditReturnFocus(arenaChat);
        }
        return 0;
    }

      if( !arena_settings_init ) // one time inits
    {
          arena_initSettings();
        arena_settings_init = true;
    }

    creator = (gArenaDetail.creatorid == playerPtr()->db_id);

    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );                                                // window 
      drawFrame( PIX3, R10, x, y+ARENA_SETTING_HT*sc, z, wd, ht-ARENA_SETTING_HT*sc, sc, color, 0 );            // settings
       drawFrame( PIX3, R10, x, y+ht-ARENA_CHAT_HT*sc, z, wd, ARENA_CHAT_HT*sc, sc, color, 0 );                // chat level
     drawFrame( PIX3, R10, x, y+ht-ARENA_CHAT_HT*sc, z, ARENA_CHAT_WD*sc, ARENA_CHAT_HT*sc, sc, color, 0    );    // chat box
     drawFrame( PIX3, R10, x, y+ht-CHAT_ENTRY_HT*sc, z, ARENA_CHAT_WD*sc, CHAT_ENTRY_HT*sc, sc, color, 0    );    // chat entry

    if( gArenaDetail.eventid == 0 )
        return 0;

    BuildCBox( &arenaSettingsParent.box, x, y, wd, ARENA_SETTING_HT*sc );
    BuildCBox( &arenaPlayerListParent.box, x, y + ARENA_SETTING_HT*sc, wd, ht - (ARENA_SETTING_HT+ARENA_CHAT_HT)*sc);
    BuildCBox( &arenaChatParent.box, x, y+ht-ARENA_CHAT_HT*sc, ARENA_CHAT_WD*sc, ARENA_CHAT_HT*sc ); 
     BuildCBox( &arenaButtonsParent.box, x + ARENA_CHAT_WD*sc, y+ht-ARENA_CHAT_HT*sc, wd-ARENA_CHAT_WD*sc, ARENA_CHAT_HT*sc );

       drawArenaSettings(); // Handles the creator settings at top of the window


      if( !gArenaDetail.creatorid || creator ) // Display the reasons the event is unsanctioned
        creator_txt = arenaProblemsString( x, y, z, wd, ht, sc );
    

    // Draw the list of players
    set_scissor(true);
      scissor_dims( x+PIX3*sc, y + (ARENA_SETTING_HT+PIX3+1)*sc, 5000, ht-(ARENA_SETTING_HT+ARENA_CHAT_HT+2*PIX3)*sc-creator_txt);
    BuildCBox( &box, x+PIX3*sc, y + (ARENA_SETTING_HT+PIX3+1)*sc, 5000, ht-(ARENA_SETTING_HT+ARENA_CHAT_HT+2*PIX3)*sc-creator_txt );
    drawPlayerList( playerSb.offset );
    set_scissor(false);
 

     doScrollBar( &playerSb, ht-(ARENA_SETTING_HT+ARENA_CHAT_HT)*sc-creator_txt, PLAYER_HT*(gArenaDetail.maxplayers+1)*sc, wd, ARENA_SETTING_HT*sc, z, &box, 0 );

    //drawArenaChat - only when window is all the way open so text wraping isn't lame
    if (window_getMode(WDW_ARENA_CREATE) == WINDOW_DISPLAYING)
    {
        drawArenaChat( x, y+ht-(ARENA_CHAT_HT)*sc, z, ARENA_CHAT_WD*sc, ARENA_CHAT_HT*sc, sc );
    }

    //drawArenaButtons
       drawArenaButtons( x + ARENA_CHAT_WD*sc, y + ht - ARENA_CHAT_HT*sc, z, sc, wd-ARENA_CHAT_WD*sc, ARENA_CHAT_HT*sc, color, y );

    //notify server of any changes
      if( creator_update && creator ) //Send full arena update to the server
    {
        arenaApplyUIToEvent();
        arenaSendCreatorUpdate( &gArenaDetail );
        creator_update = 0;
    }
    else if( player_update ) // Send an update on a specific player (assumes only one player can be changed at a time manually
    {
        arenaSendPlayerUpdate( &gArenaDetail, player_update );
        player_update = NULL;
    }

    return 0;
}

//--------------------------------------------------------------------------------------------
// Context Menus /////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------

int arenaCanInviteTarget(void *unused)
{
    Entity * e = playerPtr();
    int i;

    // Do you have a target and are you event creator?
    if( !current_target || !gArenaDetail.eventid || gArenaDetail.creatorid != e->db_id )
        return CM_HIDE;

    // is there room in the event?
    if( gArenaDetail.numplayers >= gArenaDetail.maxplayers )
        return CM_VISIBLE;

    // Is my target already in event?
    for( i = 0; i < gArenaDetail.numplayers; i++ )
    {
        if( current_target->db_id == gArenaDetail.participants[i]->dbid )
            return CM_VISIBLE;
    }
    
    return CM_AVAILABLE;
}

void arenaInviteTarget(void * unused)
{
    if( arenaCanInviteTarget(NULL) == CM_AVAILABLE )
    {
        // Note: This is hacked command until e->pl->arenaActiveEventId is really being set
        char buf[1024];
         sprintf(buf, "arenainvite \"%s\"", current_target->name);
        cmdParse(buf);
    }
}

/* End of File */
