/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <memory.h>
#include <string.h>

#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/file.h>

#include "seq/seqstate.h"
#include "seq/seq.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/utils/error.h>
#include "player/badges.h"
#include "entity/Concept.h"
#include "entity/salvage.h"
#include "entity/Proficiency.h"
#include "bases/DetailRecipe.h"
#include "auction/Auction.h"
#include "account/AccountCatalog.h"
#include "gameComm/VillainDef.h"
#include <utilitieslib/components/StashTable.h>
#include "player/pophelp.h"

#define BOOST_PARSE_INFO_DEFINITIONS // to get the ParseInfos for reading
#include "entity/boost.h"

#define POWERS_PARSE_INFO_DEFINITIONS // to get the ParseInfos for reading
#include "entity/powers.h"

#if SERVER
#define COMBAT_MOD_PARSE_INFO_DEFINITIONS // to get the ParseInfos for reading
#include "entity/combat_mod.h"
#include "cmdparse/cmdserver.h"
#include "gameSys/DayJob.h"
#include "entity/character_karma.h"
#include "gameSys/EndGameRaid.h"
#elif CLIENT
#include "cmdparse/cmdgame.h"
#include "UI/uiTextLink.h"
#include "gameData/arenastruct.h"
#include "UI/uiSupercostume.h"
#endif

#define CLASSES_PARSE_INFO_DEFINITIONS // to get the ParseInfos for reading
#include "entity/classes.h"

#define ORIGINS_PARSE_INFO_DEFINITIONS // to get the ParseInfos for reading
#include "entity/origins.h"

#define CHARACTER_BASE_PARSE_INFO_DEFINITIONS // to get the ParseInfos for reading
#include "entity/character_base.h"

#define SCHEDULES_PARSE_INFO_DEFINITIONS  // to get the ParseInfos for reading
#define EXPERIENCE_TABLE_PARSE_INFO_DEFINITIONS // to get the ParseInfos for reading
#include "entity/character_level.h"

#define ATTRIB_NAMES_PARSE_INFO_DEFINITIONS // to get the ParseInfos for reading
#include "entity/attrib_names.h"

#define BOOSTSET_PARSE_INFO_DEFINITIONS // to get the ParseInfos for reading
#include "entity/boostset.h"

#if SERVER
#define DAMAGEDECAY_PARSE_INFO_DEFINITIONS // to get the ParseInfos for reading
#include "entity/entGameActions.h"
#endif

#if SERVER
#define CHAT_EMOTE_PARSE_INFO_DEFINITIONS // to get the ParseInfos for reading
#include "cmdparse/chat_emote.h"
#include "gameSys/petarena.h"
#include "storyarc/contactdef.h"
#include "entity/incarnate_server.h"
#endif

#include "load_def.h"
#include "entity/LoadDefCommon.h"
#include "entity/character_eval.h"
#include <utilitieslib/utils/eval.h>
#include "entity/PCC_Critter.h"
#if SERVER
#include "entity/character_combat_eval.h"
#include "Reward.h"
#include "gameComm/NewFeatures.h"
#endif

#include "entity/attrib_description.h"
#include "entity/character_inventory.h"
#include <utilitieslib/components/EString.h>
#include "bases/basedata.h"
#include "storyarc/pnpcCommon.h"

DefineContext *g_pParsePowerDefines = NULL;
STATIC_DEFINE_WRAPPER(ParsePowerDefines, g_pParsePowerDefines);

#if SERVER
DefineContext *g_pParseIncarnateDefines = NULL;
STATIC_DEFINE_WRAPPER(ParseIncarnateDefines, g_pParseIncarnateDefines);
#endif

TokenizerParseInfo ParseVisionPhaseNamesTable[]=
{
    { "VisionPhaseName",    TOK_STRINGARRAY(VisionPhaseNames, visionPhases)},
    { "", 0, 0 }
};

TokenizerParseInfo ParsePCC_CritterRewardMods[]=
{
    { "{",                TOK_START,  0 },
    { "DifficultyMod",    TOK_F32ARRAY(PCC_CritterRewardMods, fDifficulty)},
    { "MissingRankPenalty",    TOK_F32ARRAY(PCC_CritterRewardMods, fMissingRankPenalty)},
    { "CustomPowerNumberMod",    TOK_F32ARRAY(PCC_CritterRewardMods, fPowerNum)},
    { "ArchitectAmbushScaling",        TOK_F32(PCC_CritterRewardMods, fAmbushScaling, 1.0f)},
    { "}",                TOK_END,    0 },
    { "", 0, 0 }
};


TokenizerParseInfo ParsePowerSetConversion[]=
{
    { "",   TOK_STRUCTPARAM|TOK_STRING(PowerSetConversion, pchPlayerPowerSet, 0 ),  0 },
    { "",   TOK_STRUCTPARAM|TOK_STRINGARRAY(PowerSetConversion, ppchPowerSets ),  0 },
    { "\n",   TOK_STRUCTPARAM|TOK_END,  0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParsePowerSetConversionTable[]=
{
    { "PowerSet",   TOK_STRUCT(PowerSetConversionTable, ppConversions,  ParsePowerSetConversion),  0 },
    { "", 0, 0 }
};

static DefineIntList s_PowerEnums[] =
{
    // BasePower ////////////////////////////////////////////////////////
//    { "kUnknown",            0  },

    // Special Attributes
    { "kTranslucency",       kSpecialAttrib_Translucency     },
    { "kEntCreate",          kSpecialAttrib_EntCreate        },
    { "kClearDamagers",      kSpecialAttrib_ClearDamagers    },
    { "kSilentKill",         kSpecialAttrib_SilentKill       },
    { "kXPDebtProtection",   kSpecialAttrib_XPDebtProtection },
    { "kSetMode",            kSpecialAttrib_SetMode          },
    { "kSetCostume",         kSpecialAttrib_SetCostume       },
    { "kGlide",              kSpecialAttrib_Glide            },
    { "kNull",               kSpecialAttrib_Null             },
    { "kEvade",              kSpecialAttrib_Null             },
    { "kAvoid",              kSpecialAttrib_Avoid            },
    { "kReward",             kSpecialAttrib_Reward           },
    { "kXPDebt",             kSpecialAttrib_XPDebt           },
    { "kDropToggles",        kSpecialAttrib_DropToggles      },
    { "kGrantPower",         kSpecialAttrib_GrantPower       },
    { "kGrantBoostedPower",  kSpecialAttrib_GrantBoostedPower},
    { "kRevokePower",        kSpecialAttrib_RevokePower      },
    { "kUnsetMode",          kSpecialAttrib_UnsetMode        },
    { "kGlobalChanceMod",    kSpecialAttrib_GlobalChanceMod  },
    { "kPowerChanceMod",     kSpecialAttrib_PowerChanceMod   },
    { "kViewAttrib",         kSpecialAttrib_ViewAttrib         },
    { "kRewardSource",         kSpecialAttrib_RewardSource     },
    { "kRewardSourceTeam",     kSpecialAttrib_RewardSourceTeam },
    { "kClearFog",             kSpecialAttrib_ClearFog         },
    { "kPhase",                 kSpecialAttrib_CombatPhase         },
    { "kCombatPhase",         kSpecialAttrib_CombatPhase         },
    { "kCombatModShift",     kSpecialAttrib_CombatModShift     },
    { "kRechargePower",         kSpecialAttrib_RechargePower     },
    { "kVisionPhase",         kSpecialAttrib_VisionPhase         },
    { "kNinjaRun",             kSpecialAttrib_NinjaRun         },
    { "kWalk",                 kSpecialAttrib_Walk             },
    { "kBeastRun",             kSpecialAttrib_BeastRun         },
    { "kSteamJump",             kSpecialAttrib_SteamJump         },
    { "kDesignerStatus",     kSpecialAttrib_DesignerStatus         },
    { "kHoverBoard",         kSpecialAttrib_HoverBoard         },
    { "kExclusiveVisionPhase",    kSpecialAttrib_ExclusiveVisionPhase    },
    { "kSetSZEValue",         kSpecialAttrib_SetSZEValue         },
    { "kAddBehavior",         kSpecialAttrib_AddBehavior         },
    { "kPowerRedirect",         kSpecialAttrib_PowerRedirect    },
    { "kMagicCarpet",         kSpecialAttrib_MagicCarpet         },
    { "kTokenAdd",             kSpecialAttrib_TokenAdd         },
    { "kTokenSet",             kSpecialAttrib_TokenSet         },
    { "kTokenClear",         kSpecialAttrib_TokenClear         },
    { "kLuaExec",             kSpecialAttrib_LuaExec             },
    { "kForceMove",                kSpecialAttrib_ForceMove            },
    { "kParkourRun",            kSpecialAttrib_ParkourRun            },
    { 0, 0 }
};
#if SERVER
TokenizerParseInfo ParseZoneEventKarmaVar[]=
{
    { "{",   TOK_START,  0 },
    { "Amount",   TOK_INT(ZoneEventKarmaVarTable, karmaVar[CKV_AMOUNT], 0 ),  0 },
    { "Lifespan",   TOK_INT(ZoneEventKarmaVarTable, karmaVar[CKV_LIFESPAN], 0 ),  0 },
    { "StackLimit",   TOK_INT(ZoneEventKarmaVarTable, karmaVar[CKV_STACK_LIMIT], 0 ),  0 },
    { "}",   TOK_END,  0 },
    { "", 0, 0 }
};
TokenizerParseInfo ParseZoneEventKarmaClassMod[]=
{
    { "{",   TOK_START,  0 },
    { "ClassName",   TOK_STRING(ZoneEventKarmaClassMod, pchClassName, 0 )},
    { "ClassMod",   TOK_F32(ZoneEventKarmaClassMod, classMod, 1.0f )},
    { "}",   TOK_END,  0 },
    { "", 0, 0 }
};
TokenizerParseInfo ParseZoneEventKarmaTable[]=
{
    { "PowerClick",   TOK_EMBEDDEDSTRUCT(ZoneEventKarmaTable, karmaReasonTable[CKR_POWER_CLICK], ParseZoneEventKarmaVar),  0 },
    { "BuffPowerClick",   TOK_EMBEDDEDSTRUCT(ZoneEventKarmaTable, karmaReasonTable[CKR_BUFF_POWER_CLICK], ParseZoneEventKarmaVar),  0 },
    { "TeamObjComplete",   TOK_EMBEDDEDSTRUCT(ZoneEventKarmaTable, karmaReasonTable[CKR_TEAM_OBJ_COMPLETE], ParseZoneEventKarmaVar),  0 },
    { "AllPlayObjComplete",   TOK_EMBEDDEDSTRUCT(ZoneEventKarmaTable, karmaReasonTable[CKR_ALL_OBJ_COMPLETE], ParseZoneEventKarmaVar),  0 },
    { "ClassModifier",    TOK_STRUCT(ZoneEventKarmaTable, ppClassMod, ParseZoneEventKarmaClassMod), 0 },
    { "PowerLifeCap",   TOK_INT(ZoneEventKarmaTable, powerLifespanCap, 0),  0 },    
    { "PowerLifeConst",   TOK_INT(ZoneEventKarmaTable, powerLifespanConst, 0),  0 },    
    { "PowerStackMod",   TOK_INT(ZoneEventKarmaTable, powerStackModifier, 0),  0 },    
    { "PowerBubbleDur",     TOK_INT(ZoneEventKarmaTable, powerBubbleDur, 0),  0 },    
    { "PowerBubbleRad",     TOK_INT(ZoneEventKarmaTable, powerBubbleRad, 0),  0 },    
    { "KarmaPardonDuration",        TOK_INT(ZoneEventKarmaTable, pardonDuration, 0), 0 },
    { "KarmaPardonGrace",        TOK_INT(ZoneEventKarmaTable, pardonGrace, 0), 0 },
    { "ActiveDuration",        TOK_INT(ZoneEventKarmaTable, activeDuration, 0), 0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseDesignerContactTip[]=
{
    { "",   TOK_STRUCTPARAM|TOK_STRING(DesignerContactTip, rewardTokenName, 0 ) },
    { "",   TOK_STRUCTPARAM|TOK_STRING(DesignerContactTip, tokenTypeStr, 0 ) },
    { "",   TOK_STRUCTPARAM|TOK_INT(DesignerContactTip, tipLimit, 0 ) },
    { "",    TOK_STRUCTPARAM|TOK_BOOL(DesignerContactTip, revokeOnAlignmentSwitch, 0), ModBoolEnum},
    { "\n",   TOK_STRUCTPARAM|TOK_END,  0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseDesignerContactTipTypes[]=
{
    { "DesignerTipType",   TOK_STRUCT(DesignerContactTipTypes, designerTips,  ParseDesignerContactTip),  0 },
    { "", 0, 0 }
};
TokenizerParseInfo ParseIncarnateSlotInfo[] =
{
    { "{",   TOK_START,  0 },
    { "SlotUnlocked",   TOK_INT(IncarnateSlotInfo, slotUnlocked, -1),  ParseIncarnateSlotNames},
    { "BadgeStat",        TOK_STRING(IncarnateSlotInfo,badgeStat, 0)},
    { "ClientMessage",    TOK_STRING(IncarnateSlotInfo,clientMessage, 0)},
    { "}",   TOK_END,  0 },
    { "", 0, 0 }
};
TokenizerParseInfo ParseIncarnateExpInfo[]=
{
    { "{",   TOK_START,  0 },
    { "ExpType",         TOK_STRING(IncarnateExpInfo, incarnateXPType, 0)},
    { "IncarnateSlotInfo",   TOK_STRUCT(IncarnateExpInfo, incarnateSlotInfo, ParseIncarnateSlotInfo)},
    { "}",   TOK_END,  0 },
    { "", 0, 0 }
};
TokenizerParseInfo ParseIncarnateExpMods[]=
{
    { "{",   TOK_START,  0 },
    { "Modifiers",   TOK_F32ARRAY(IncarnateModifier, incarnateExpModifier)},
    { "}",   TOK_END,  0 },
    { "", 0, 0 }
};
TokenizerParseInfo ParseIncarnateMods[]=
{
    { "IncarnateSlots",   TOK_STRUCT(IncarnateMods, incarnateExpList,  ParseIncarnateExpInfo),  0 },
    { "OriginTechMods",   TOK_EMBEDDEDSTRUCT(IncarnateMods, incarnateModifierList[kProfOriginType_Tech], ParseIncarnateExpMods),  0 },
    { "OriginScienceMods",   TOK_EMBEDDEDSTRUCT(IncarnateMods, incarnateModifierList[kProfOriginType_Science], ParseIncarnateExpMods),  0 },
    { "OriginMutantMods",   TOK_EMBEDDEDSTRUCT(IncarnateMods, incarnateModifierList[kProfOriginType_Mutant], ParseIncarnateExpMods),  0 },
    { "OriginMagicMods",   TOK_EMBEDDEDSTRUCT(IncarnateMods, incarnateModifierList[kProfOriginType_Magic], ParseIncarnateExpMods),  0 },
    { "OriginNaturalMods",   TOK_EMBEDDEDSTRUCT(IncarnateMods, incarnateModifierList[kProfOriginType_Natural], ParseIncarnateExpMods),  0 },
    { "", 0, 0 }
};
TokenizerParseInfo ParseEndGameRaidKickMods[]=
{
    { "LastKickTime_Secs",            TOK_INT(EndGameRaidKickMods, lastKickTimeSeconds, 0),  0 },
    { "LastPlayerKickTime_Mins",    TOK_INT(EndGameRaidKickMods, lastPlayerKickTimeMins, 0),  0 },
    { "MinRaidTime_Mins",            TOK_INT(EndGameRaidKickMods, minRaidTimeMins, 0),  0 },
    { "KickDuration_Secs",            TOK_INT(EndGameRaidKickMods, kickDurationSeconds, 0),  0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseMARTYLevelMods[]=
{
    {    ".",        TOK_STRUCTPARAM | TOK_INT(MARTYLevelMods, level, 0), 0 },
    {    ".",        TOK_STRUCTPARAM | TOK_INT(MARTYLevelMods, expGain1Min, 0), 0 },
    {    ".",        TOK_STRUCTPARAM | TOK_INT(MARTYLevelMods, expGain5Min, 0), 0 },
    {    ".",        TOK_STRUCTPARAM | TOK_INT(MARTYLevelMods, expGain15Min, 0), 0 },
    {    ".",        TOK_STRUCTPARAM | TOK_INT(MARTYLevelMods, expGain30Min, 0), 0 },
    {    ".",        TOK_STRUCTPARAM | TOK_INT(MARTYLevelMods, expGain60Min, 0), 0 },
    {    ".",        TOK_STRUCTPARAM | TOK_INT(MARTYLevelMods, expGain120Min, 0), 0 },
    { "\n",                        TOK_END, 0                                                        },
    { "", 0, 0 }
};
TokenizerParseInfo ParseMARTYExpMods[]=
{
    { "{",   TOK_START,  0 },
    { "ExpType",        TOK_INT(MARTYExpMods, expType, 0),  ParseMARTYExperienceTypes },
    { "Action",            TOK_INT(MARTYExpMods, action, 0),  ParseMARTYActions },
    { "Level",            TOK_STRUCT(MARTYExpMods, perLevelThrottles, ParseMARTYLevelMods)},
    { "LogMessage",            TOK_STRING(MARTYExpMods, logAppendMsg, 0), 0},
    { "}",   TOK_END,  0 },
    { "", 0, 0 }
};
TokenizerParseInfo ParseMARTYLevelupMods[]=
{
    {    ".",        TOK_STRUCTPARAM | TOK_INT(MARTYLevelupShifts, level, 0), 0 },
    {    ".",        TOK_STRUCTPARAM | TOK_F32(MARTYLevelupShifts, mod, 0), 0 },
    { "\n",                        TOK_END, 0                                                        },
    { "", 0, 0 }
};

TokenizerParseInfo ParseMARTYMods[]=
{
    {    "MARTYMod",            TOK_STRUCT(MARTYMods, perActionType, ParseMARTYExpMods)    },
    {    "MARTYLevelupMod",    TOK_STRUCT(MARTYMods, levelupShifts, ParseMARTYLevelupMods)    },
    { "", 0, 0 }
};
#endif
/**********************************************************************func*
 * AddSequencerDefines
 *
 */
static void AddSequencerDefines(DefineContext *defines)
{
    char *achSeqBits[] =
    {
        "NONE",
        "CLAW",
        "KUNGFU",
        "GUN",
        "BLADE",
        "BLUNT",
        "HANDGUN",
        "COMBAT",
        "WEAPON",
        "SPEED",
        "TELEPORT",
        "CARRY",
        "CLUB",
        NULL
    };

    int i;
    char ach[256];
    char val[20];

    for( i = 0 ; i < g_MaxUsedStates ; i++ )
    {
        if( stateBits[i].name )
        {
            strcpy(ach, "ANIM_");
            strcat(ach, stateBits[i].name);

            sprintf(val, "%d", stateBits[i].bitNum);

            DefineAdd(defines, ach, val);
        }
    }

    // CrAzY SpOoNhEaD SEQBITS stuff which are just synonyms for the ANIM
    // stuff. WTF.
    for(i=1; achSeqBits[i]!=NULL; i++)
    {
        int ival;
        ival = seqGetStateNumberFromName(achSeqBits[i]);
        assert(ival!=0);

        strcpy(ach, "SEQBITS_");
        strcat(ach, achSeqBits[i]);
        sprintf(val, "%d", ival);

        DefineAdd(defines, ach, val);
    }
}

/**********************************************************************func*
 * AddAttributeDefines
 *
 */
static void AddAttributeDefines(DefineContext *defines)
{
    int i;
    char ach[256];
    char val[20];

    for(i=0; ParseCharacterAttributes[i].name!=NULL; i++)
    {
        if(TOK_GET_TYPE(ParseCharacterAttributes[i].type) == TOK_F32_X)
        {
            strcpy(ach, "k");
            strcat(ach, ParseCharacterAttributes[i].name);
            sprintf(val, "%d", ParseCharacterAttributes[i].storeoffset);

            DefineAdd(defines, ach, val);
        }
    }
}




/**********************************************************************func*
 * AddAttribNames
 *
 */
static void AddAttribNames(DefineContext *defines)
{
    int i;
    int iOriginBase;
    int iSize = eaSize(&g_AttribNames.ppDamage);

    for(i=0; i<iSize; i++)
    {
        int j;
        char ach[256];
        char val[256];

        strcpy(ach, g_AttribNames.ppDamage[i]->pchName);
        sprintf(val, "DamageType%02d", i);
        DefineAdd(defines, ach, val);

        // TODO: This isn't efficient, but it's only called once at the start
        for(j=0; ParseCharacterAttributes[j].name!=NULL; j++)
        {
            if(stricmp(ParseCharacterAttributes[j].name, val)==0)
            {
                strcpy(ach, "k");
                strcat(ach, g_AttribNames.ppDamage[i]->pchName);
                sprintf(val, "%d", ParseCharacterAttributes[j].storeoffset);

                DefineAdd(defines, ach, val);
                break;
            }
        }
    }

    iSize = eaSize(&g_AttribNames.ppElusivity);
    for(i=0; i<iSize; i++)
    {
        int j;
        char ach[256];
        char val[256];

        strcpy(ach, g_AttribNames.ppElusivity[i]->pchName);
        sprintf(val, "Elusivity%02d", i);
        DefineAdd(defines, ach, val);

        // TODO: This isn't efficient, but it's only called once at the start
        for(j=0; ParseCharacterAttributes[j].name!=NULL; j++)
        {
            if(stricmp(ParseCharacterAttributes[j].name, val)==0)
            {
                strcpy(ach, "k");
                strcat(ach, g_AttribNames.ppElusivity[i]->pchName);
                sprintf(val, "%d", ParseCharacterAttributes[j].storeoffset);

                DefineAdd(defines, ach, val);
                break;
            }
        }
    }

    iSize = eaSize(&g_AttribNames.ppDefense);
    for(i=0; i<iSize; i++)
    {
        int j;
        char ach[256];
        char val[256];

        strcpy(ach, g_AttribNames.ppDefense[i]->pchName);
        sprintf(val, "DefenseType%02d", i);
        DefineAdd(defines, ach, val);

        // TODO: This isn't efficient, but it's only called once at the start
        for(j=0; ParseCharacterAttributes[j].name!=NULL; j++)
        {
            if(stricmp(ParseCharacterAttributes[j].name, val)==0)
            {
                strcpy(ach, "k");
                strcat(ach, g_AttribNames.ppDefense[i]->pchName);
                sprintf(val, "%d", ParseCharacterAttributes[j].storeoffset);

                DefineAdd(defines, ach, val);
                break;
            }
        }
    }

    iSize = eaSize(&g_CharacterOrigins.ppOrigins);
    for(i=0; i<iSize; i++)
    {
        char ach[256];
        char val[256];

        strcpy(ach, "k");
        strcat(ach, g_CharacterOrigins.ppOrigins[i]->pchName);
        strcat(ach, "_Boost");
        sprintf(val, "%d", i);
        DefineAdd(defines, ach, val);
    }
    iOriginBase = i;

    iSize = eaSize(&g_AttribNames.ppBoost);
    for(i=0; i<iSize; i++)
    {
        char ach[256];
        char val[256];

        strcpy(ach, "k");
        strcat(ach, g_AttribNames.ppBoost[i]->pchName);
        sprintf(val, "%d", i+iOriginBase);
        DefineAdd(defines, ach, val);
    }

    iSize = eaSize(&g_AttribNames.ppGroup);
    for(i=0; i<iSize; i++)
    {
        char ach[256];
        char val[256];

        strcpy(ach, "k");
        strcat(ach, g_AttribNames.ppGroup[i]->pchName);
        sprintf(val, "%d", i);
        DefineAdd(defines, ach, val);
    }

    iSize = eaSize(&g_AttribNames.ppMode);
    for(i=0; i<iSize; i++)
    {
        char ach[256];
        char val[256];

        strcpy(ach, "k");
        strcat(ach, g_AttribNames.ppMode[i]->pchName);
        sprintf(val, "%d", i);
        DefineAdd(defines, ach, val);
    }

    iSize = eaSize(&g_AttribNames.ppStackKey);
    for(i=0; i<iSize; i++)
    {
        char ach[256];
        char val[256];

        strcpy(ach, "k");
        strcat(ach, g_AttribNames.ppStackKey[i]->pchName);
        sprintf(val, "%d", i);
        DefineAdd(defines, ach, val);
    }
}

#if SERVER
static void AddIncarnateMods(DefineContext *defines)
{
    int i, iSize;

    iSize = eaSize(&g_IncarnateMods.incarnateExpList);
    for(i=0; i<iSize; i++)
    {
        char ach[256];
        char val[256];
        int len = MIN((int)strlen(g_IncarnateMods.incarnateExpList[i]->incarnateXPType), 255);

        // doesn't add the k in front on purpose, to support existing data
        strncpy(ach, g_IncarnateMods.incarnateExpList[i]->incarnateXPType, len + 1);
        sprintf(val, "%d", i);
        DefineAdd(defines, ach, val);
    }
}
#endif


/**********************************************************************func*
* load_SalvageDictionary
*
// -AB: created :2005 Feb 15 02:11 PM
*/
static void load_SalvageDictionary( SHARED_MEMORY_PARAM SalvageDictionary *p, char *pchFilename, bool bNewAttribs )
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER 
    int flags = PARSER_SERVERONLY | (bNewAttribs?PARSER_FORCEREBUILD:0);
#else
    int flags = (bNewAttribs?PARSER_FORCEREBUILD:0);
#endif

    // load the file directly into shared mem.
    if (salvage_GetParseInfo())
    {
        ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
            salvage_GetParseInfo(),p,sizeof(*p),NULL,NULL,NULL,NULL,salvage_FinalProcess);
    }
}


//------------------------------------------------------------
//  loads the concepts
//----------------------------------------------------------
static void load_ConceptDictionary( SHARED_MEMORY_PARAM ConceptDictionary *p, char *pchFilename, bool bNewAttribs )
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER 
    int flags = PARSER_SERVERONLY | (bNewAttribs?PARSER_FORCEREBUILD:0);
#else
    int flags = (bNewAttribs?PARSER_FORCEREBUILD:0);
#endif

    // load the file directly into shared mem.
    if (conceptdef_GetParseInfo())
    {
        ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
            conceptdef_GetParseInfo(),p,sizeof(*p),NULL,NULL,NULL,NULL,conceptdict_FinalProcess);
    }
}


static void load_ProficiencyDictionary( SHARED_MEMORY_PARAM ProficiencyDictionary *p, char *pchFilename, bool bNewAttribs )
{
    const char *pchBinFilename;
#if SERVER 
    int flags = PARSER_SERVERONLY | (bNewAttribs?PARSER_FORCEREBUILD:0);
#else
    int flags = (bNewAttribs?PARSER_FORCEREBUILD:0);
#endif
    
    // --------------------
    // load the unique ids
    TokenizerParseInfo *attribParseInfo = attribfiledict_GetParseInfo();

    if( attribParseInfo )
    {
        // cast away const here. this is the only place you should have to do it.
        AttribFileDict *pdict = (AttribFileDict *)proficiency_GetAttribs();
        
        if( pdict )
        {
            const char *fname = (const char*)proficiency_GetIdmapFilename();
            const char *pchBinFilename = MakeBinFilename(fname);
            if( fileExists(pchBinFilename) || fileExists(fname) )
            {
                ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,fname,pchBinFilename,flags,
                    attribParseInfo,pdict,sizeof(*pdict),NULL,NULL,NULL,NULL,attribfiledict_FinalProcess);
            }
        }
    }

    // --------------------
    // load the file directly into shared mem.

    pchBinFilename = MakeBinFilename(pchFilename);
    if (proficiency_GetParseInfo() && (fileExists(pchBinFilename) || fileExists(pchFilename) ) )
    {
        ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
            proficiency_GetParseInfo(),p,sizeof(*p),NULL,NULL,NULL,NULL,proficiency_FinalProcess);

    }

}

#if SERVER 
static bool salvageTracked_FinalProcess(TokenizerParseInfo pti[], SalvageTrackedByEntList *salvageTrackedList, bool shared_memory)
{
    int i;
    for (i = 0; i < eaSize(&salvageTrackedList->ppTrackedSalvage); ++i)
    {
        SalvageTrackedByEnt *salvageItem = salvageTrackedList->ppTrackedSalvage[i];
        assert(!salvageItem->item);
        salvageItem->item = salvage_GetItem(salvageItem->salvageName);
    }
    return true;
}

static void load_SalvageTrackedList(SHARED_MEMORY_PARAM SalvageTrackedByEntList *p, char *pchFilename, bool bNewAttribs )
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
    int flags = PARSER_SERVERONLY | (bNewAttribs?PARSER_FORCEREBUILD:0);

    // load the file directly into shared mem.
    if (salvageTrackedByEntParseInfo())
    {
        ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
            salvageTrackedByEntParseInfo(),p,sizeof(*p),NULL,NULL,0,NULL,salvageTracked_FinalProcess);
    }
}
#endif

static bool load_BoostSetDictionary_DataProcess(ParseTable pti[], BoostSetDictionary *bdict)
{
    boostset_Validate(bdict, "defs/boostsets/boostsets.def");
    return 1;
}

static SharedMemoryHandle *s_powers_handle = NULL;
static bool load_BoostSetDictionary_FinalProcess(ParseTable pti[], BoostSetDictionary *bdict, bool shared_memory)
{
    bool ret;

    if (s_powers_handle)
        sharedMemoryLock(s_powers_handle);

    ret = boostset_Finalize(bdict, shared_memory, "defs/boostsets/boostsets.def", true);

    if (s_powers_handle)
        sharedMemoryUnlock(s_powers_handle);

    return ret;
}

/**********************************************************************func*
* load_BoostSetDictionary
*
*/
static void load_BoostSetDictionary(SHARED_MEMORY_PARAM BoostSetDictionary *p, char *pchFilename, SharedMemoryHandle *powers_handle, bool bNewAttribs)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER 
    int flags = PARSER_SERVERONLY | (bNewAttribs?PARSER_FORCEREBUILD:0);
#else
    int flags = (bNewAttribs?PARSER_FORCEREBUILD:0);
#endif

    s_powers_handle = powers_handle;

    // load the file directly into shared mem.
    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseBoostSetDictionary,p,sizeof(*p),NULL,NULL,NULL,load_BoostSetDictionary_DataProcess,load_BoostSetDictionary_FinalProcess);

    s_powers_handle = NULL;
}

/**********************************************************************func*
* load_ConversionSets
*
*/
static void load_ConversionSets(SHARED_MEMORY_PARAM ConversionSets *p, char *pchFilename, bool bNewAttribs)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER 
    int flags = PARSER_SERVERONLY | (bNewAttribs?PARSER_FORCEREBUILD:0);
#else
    int flags = (bNewAttribs?PARSER_FORCEREBUILD:0);
#endif

    // load the file directly into shared mem.
    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
                            ParseConversionSet,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}


/**********************************************************************func*
* load_PowerDiectionary_Callback
*
*/
static void load_PowerDictionary_Callback(SharedMemoryHandle *powers_handle, bool bNewAttribs)
{
    load_BoostSetDictionary(&g_BoostSetDict, "defs/boostsets/boostsets.def", powers_handle, bNewAttribs);

    load_ConversionSets(&g_ConversionSets, "defs/boostsets/conversionsets.def", bNewAttribs);
}

/**********************************************************************func*
* load_CharacterClasses_DataProcess
*
*/
static bool load_CharacterClasses_DataProcess(TokenizerParseInfo pti[], CharacterClasses *p)
{
    classes_Repack(p, &g_PowerDictionary);
    return 1;
}

/**********************************************************************func*
* load_CharacterClasses_FinalProcess
*
*/
static bool load_CharacterClasses_FinalProcess(TokenizerParseInfo pti[], CharacterClasses *p, bool shared_memory)
{
    return classes_Finalize(p, shared_memory);
}

/**********************************************************************func*
 * load_CharacterClasses
 *
 */
static void load_CharacterClasses(SHARED_MEMORY_PARAM CharacterClasses *p, char *pchFilename, bool bNewAttribs)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER 
    int flags = PARSER_SERVERONLY | (bNewAttribs?PARSER_FORCEREBUILD:0);
#else
    int flags = (bNewAttribs?PARSER_FORCEREBUILD:0);
#endif

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseCharacterClasses,p,sizeof(*p),g_pParsePowerDefines,NULL,NULL,load_CharacterClasses_DataProcess,load_CharacterClasses_FinalProcess);
}


/**********************************************************************func*
 * load_CharacterOrigins
 *
 */
static void load_CharacterOrigins(SHARED_MEMORY_PARAM CharacterOrigins *p, char *pchFilename, bool bNewAttribs)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER 
    int flags = PARSER_SERVERONLY | (bNewAttribs?PARSER_FORCEREBUILD:0);
#else
    int flags = (bNewAttribs?PARSER_FORCEREBUILD:0);
#endif

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseCharacterOrigins,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}

/**********************************************************************func*
 * load_Schedules
 *
 */
static void load_Schedules(SHARED_MEMORY_PARAM Schedules *p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER 
    int flags = PARSER_SERVERONLY;
#else
    int flags = 0;
#endif
    
    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseSchedules,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}

/**********************************************************************func*
* load_AttribDescriptions
*
*/
static void load_AttribDescriptions(SHARED_MEMORY_PARAM AttribCategoryList *p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if CLIENT
    // clients receive attribute updates from the server
    ParserLoadFiles(NULL,pchFilename,pchBinFilename,0,ParseAttribCategoryList,cpp_const_cast(void*)(p),NULL, NULL,attribDescriptionPreprocess);
#elif SERVER
    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,0,ParseAttribCategoryList,p, sizeof(*p), NULL, NULL, attribDescriptionPreprocess, NULL, NULL);
#endif
    attribDescriptionCreateStash();
    attribNameSetAll();
}

/**********************************************************************func*
 * load_ExperienceTables
 *
 */
static void load_ExperienceTables(SHARED_MEMORY_PARAM ExperienceTables *p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER 
    int flags = PARSER_SERVERONLY;
#else
    int flags = 0;
#endif

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseExperienceTables,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}

static void load_CustomCritterMods(SHARED_MEMORY_PARAM PCC_CritterRewardMods *p, char * pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER 
    int flags = PARSER_SERVERONLY;
#else
    int flags = 0;
#endif
    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename), NULL, pchFilename, pchBinFilename, flags, ParsePCC_CritterRewardMods, p, sizeof(*p), NULL, NULL, NULL, NULL, NULL);

    if( eafSize(&p->fDifficulty) != PCC_DIFF_COUNT-1)
        Errorf( pchFilename, "Incorrect Number of Difficulty Mods" );
    if( eafSize(&p->fPowerNum) >= MAX_CRITTER_POWERS-1)
        Errorf( pchFilename, "Incorrect Number of Custom Power Mods" );
    if( eafSize(&p->fDifficulty) != 3 )
        Errorf( pchFilename, "Incorrect Number of Missing Rank Mods" );
    if (p->fAmbushScaling > 1.0f || p->fAmbushScaling < 0.0f)
        Errorf( pchFilename, "Experience scaling is exponential or negative." );
}

static bool load_PowerConversionTable_FinalProcess(ParseTable pti[], PowerSetConversionTable *p, bool shared_memory)
{
    bool ret = true;
    int i;

    assert(!p->st_PowerConversion);
    p->st_PowerConversion = stashTableCreateWithStringKeys(stashOptimalSize(eaSize(&p->ppConversions)), stashShared(shared_memory));

    for( i = 0; i < eaSize(&p->ppConversions); i++ )
    {
        if (!stashAddPointerConst(p->st_PowerConversion, p->ppConversions[i]->pchPlayerPowerSet, p->ppConversions[i], 0))
        {
            assertmsgf(0, "duplicate power conversion %s", p->ppConversions[i]->pchPlayerPowerSet);
            ret = false;
        }
    }

    return ret;
}

static void load_PowerConversionTable(SHARED_MEMORY_PARAM PowerSetConversionTable *p, char * pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename), NULL, pchFilename, pchBinFilename, PARSER_SERVERONLY, ParsePowerSetConversionTable, p, sizeof(*p), NULL, NULL, NULL, NULL, load_PowerConversionTable_FinalProcess);
}


/**********************************************************************func*
 * load_BoostChances
 *
 */
static void load_BoostChances(SHARED_MEMORY_PARAM float **p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER 
    int flags = PARSER_SERVERONLY;
#else
    int flags = 0;
#endif

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseBoostChanceTable,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}

/**********************************************************************func*
 * load_BoostEffectiveness
 *
 */
static void load_BoostEffectiveness(SHARED_MEMORY_PARAM float **p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER 
    int flags = PARSER_SERVERONLY;
#else
    int flags = 0;
#endif

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseBoostEffectivenessTable,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}

/**********************************************************************func*
 * load_ExemplarHandicaps
 *
 */
static void load_ExemplarHandicaps(SHARED_MEMORY_PARAM ExemplarHandicaps *p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER 
    int flags = PARSER_SERVERONLY;
#else
    int flags = 0;
#endif
    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseBoostExemplarTable,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}

/**********************************************************************func*
 * load_AttribNames
 *
 */
static void load_AttribNames(SHARED_MEMORY_PARAM AttribNames *p, char *pchFilename, bool* bNewAttribs)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);

#if SERVER 
    int flags = PARSER_SERVERONLY;
#else
    int flags = 0;
#endif

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseAttribNames,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);

    g_offHealAttrib = attrib_Offset("Heal");
    if(g_offHealAttrib < 0)
    {
        Errorf("Can't find Heal attrib.");
    }
}

static bool load_DiminishingReturns_FinalProcess(ParseTable pti[], void *p, bool shared_memory)
{
    if(!CreateDimReturnsData())
        ErrorFilenamef("defs/dim_returns.def", "Powers", 4, "No defaults for diminishing returns.\n");
    return 1;
}

/**********************************************************************func*
 * load_DiminishingReturns
 *
 */
static void load_DiminishingReturns(SHARED_MEMORY_PARAM DimReturnList *p, char *pchFilename, bool bNewAttribs)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER 
    int flags = PARSER_SERVERONLY | (bNewAttribs?PARSER_FORCEREBUILD:0);
#else
    int flags = (bNewAttribs?PARSER_FORCEREBUILD:0);
#endif

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseDimReturnList,p,sizeof(*p),NULL,NULL,NULL,NULL,load_DiminishingReturns_FinalProcess);
}

static void load_VisionPhaseNames(SHARED_MEMORY_PARAM VisionPhaseNames *visionPhaseNames, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
#if SERVER
    int flags = PARSER_SERVERONLY;
#else
    int flags = 0;
#endif

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseVisionPhaseNamesTable,visionPhaseNames,sizeof(*visionPhaseNames),NULL,NULL,NULL,NULL,NULL);
}

#if SERVER
/**********************************************************************func*
 * load_CombatMods
 *
 */
static void load_CombatMods(SHARED_MEMORY_PARAM CombatModsTable *p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
    int flags = PARSER_SERVERONLY;

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseCombatModsTable,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}

/**********************************************************************func*
* load_InventorySizes
*
*/
static void load_InventorySizes(SHARED_MEMORY_PARAM InventorySizes *p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
    int flags = PARSER_SERVERONLY;

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseInventoryDefinitions,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);

    if(eaiSize(&p->auctionSizes.piAmountAtLevel) == 0)
        ErrorFilenamef( pchFilename, "Default auction inventory sizes not found\n");

    if(eaiSize(&p->recipeSizes.piAmountAtLevel) <= 0)
        ErrorFilenamef( pchFilename, "Default recipe inventory sizes not found\n");

    if(eaiSize(&p->salvageSizes.piAmountAtLevel) <= 0)
        ErrorFilenamef( pchFilename, "Default salvage inventory sizes not found\n");

}

static bool load_InventoryLoyaltyBonusSizes_FinalProcess(ParseTable pti[], InventoryLoyaltyBonusSizes *p, bool shared_memory)
{
    bool ret = true;
    int i;

    assert(!p->st_LoyaltyBonus);
    p->st_LoyaltyBonus = stashTableCreateWithStringKeys(stashOptimalSize(eaSize(&p->bonusList)), stashShared(shared_memory));

    for( i = 0; i < eaSize(&p->bonusList); i++ )
    {
        if (!stashAddPointerConst(p->st_LoyaltyBonus, p->bonusList[i]->name, p->bonusList[i], 0))
        {
            assertmsgf(0, "duplicate inventory loyalty bonus name %s", p->bonusList[i]->name);
            ret = false;
        }
    }

    return ret;
}

/**********************************************************************func*
* load_InventoryLoyatyBonusSizes
*
*/
static void load_InventoryLoyaltyBonusSizes(SHARED_MEMORY_PARAM InventoryLoyaltyBonusSizes *p, const char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
    int flags = PARSER_SERVERONLY;

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseInventoryLoyaltyBonusDefinitions,p,sizeof(*p),NULL,NULL,NULL,NULL,load_InventoryLoyaltyBonusSizes_FinalProcess);

    if(eaSize(&p->bonusList) == 0)
        ErrorFilenamef( pchFilename, "Loyalty definitions not found\n");
}

/**********************************************************************func*
* load_ZoneEventKarmaMods
*
*/
static void load_ZoneEventKarmaMods(SHARED_MEMORY_PARAM ZoneEventKarmaTable *p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
    int flags = PARSER_SERVERONLY;

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseZoneEventKarmaTable,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}

/**********************************************************************func*
* load_DesignerContactTipTypes
*
*/
static void load_DesignerContactTipTypes(SHARED_MEMORY_PARAM DesignerContactTipTypes *p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
    int flags = PARSER_SERVERONLY;
    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseDesignerContactTipTypes,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}

/**********************************************************************func*
* load_IncarnateMods
*
*/
static void load_IncarnateMods(SHARED_MEMORY_PARAM IncarnateMods *p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
    int flags = PARSER_SERVERONLY;
    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseIncarnateMods,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}
static void load_EndGameRaidKickMods(EndGameRaidKickMods *p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
    int flags = PARSER_SERVERONLY;
    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseEndGameRaidKickMods,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}

static void load_MARTYMods(MARTYMods *p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
    int flags = PARSER_SERVERONLY;
    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseMARTYMods,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);

    validateMARTYMods(p, pchFilename);
}
/**********************************************************************func*
 * load_EmoteAnims
 *
 */
static void load_EmoteAnims(SHARED_MEMORY_PARAM EmoteAnims *p, char *pchFilename, bool bNewAttribs)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
    int flags = PARSER_SERVERONLY | (bNewAttribs?PARSER_FORCEREBUILD:0);
    EvalContext *context = chareval_Context();

    // ARM NOTE: Goddammit David Goodenough, this was a terrible idea.
    extern void CCEmoteCatch(EvalContext *pcontext);
    eval_RegisterFunc(context, "ccemote?", CCEmoteCatch, 0, 1);

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseEmoteAnims,p,sizeof(*p),NULL,NULL,NULL,NULL,EmoteAnimFinalProcess);

    eval_UnregisterFunc(context, "ccemote?");
}

/**********************************************************************func*
 * load_DamageDecayConfig
 *
 */
static void load_DamageDecayConfig(SHARED_MEMORY_PARAM DamageDecayConfig *p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);
    int flags = PARSER_SERVERONLY;

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,flags,
        ParseDamageDecayConfig,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}

#endif

/**********************************************************************func*
* load_AuctionConfig
*
*/
static void load_AuctionConfig(SHARED_MEMORY_PARAM AuctionConfig *p, char *pchFilename)
{
    const char *pchBinFilename = MakeBinFilename(pchFilename);

    ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,pchFilename,pchBinFilename,0,
        ParseAuctionConfig,p,sizeof(*p),NULL,NULL,NULL,NULL,NULL);
}

/**********************************************************************func*
* load_AccountProducts
*
*/
static void load_AccountProducts(void)
{
#ifdef CLIENT
    accountCatalogInit();
#elif SERVER
    accountCatalogGenerateServerBin();
#endif
}

/**********************************************************************func*
 * load_AllDefs
 *
 */
void load_AllDefs(void)
{
    bool bNewAttribs = 0; // make sure to change svr_init.c:loadConfigFiles and game.c:game_loadData as well

#if SERVER
    allRewardNamesInit();
#endif

    // Must be done before making the ParsePowerDefines.
    writeConsole(OUTPUT_DEBUG, "Loading attribute names");
    load_AttribNames(&g_AttribNames, "defs/attrib_names.def", &bNewAttribs);
    writeConsole(OUTPUT_INFO, "Loaded attribute names");

    load_VisionPhaseNames(&g_visionPhaseNames, "defs/visionPhases.def");
    if (eaSize(&g_visionPhaseNames.visionPhases) > 32)
    {
        ErrorFilenamef("defs/visionPhases.def", "Too many vision phases in definition file!");
    }

    writeConsole(OUTPUT_DEBUG, "Loading vision phase names");
    load_VisionPhaseNames(&g_exclusiveVisionPhaseNames, "defs/visionPhasesExclusive.def");
    // if we get near the four billion mark on exclusive vision phase names, we'll have to check here...
    writeConsole(OUTPUT_INFO, "Loaded vision phase names");

    // Sequencer state bits might be changed by Steve now
    bNewAttribs = bNewAttribs || seqStateBitsAreUpdated();

    writeConsole(OUTPUT_DEBUG, "Loading hero origins");
    load_CharacterOrigins(&g_CharacterOrigins, "defs/origins.def", bNewAttribs);
    writeConsole(OUTPUT_INFO, "Loaded hero origins");

#ifndef TEST_CLIENT
    writeConsole(OUTPUT_DEBUG, "Loading villain origins");
    load_CharacterOrigins(&g_VillainOrigins, "defs/villain_origins.def", bNewAttribs);
    writeConsole(OUTPUT_INFO, "Loaded villain origins");
#endif // TEST_CLIENT
    
    // NULL means use the static local version in badges.c. (backwards compatible)
    writeConsole(OUTPUT_DEBUG, "Loading badges");
    load_Badges(&g_BadgeDefs,
        "defs/badges.def", 0, "server/db/templates/badges.attribute",
        BADGE_ENT_MAX_BADGES);
    writeConsole(OUTPUT_INFO, "Loaded badges");

#ifndef TEST_CLIENT
    writeConsole(OUTPUT_DEBUG, "Loading supergroup badges and pophelp");
    load_Badges(&g_SgroupBadges,
        "defs/supergroup_badges.def", 0, "server/db/templates/supergroup_badges.attribute",
        BADGE_SG_MAX_BADGES);
    loadPopHelp("defs/PopHelp.def", "server/db/templates/pophelp.attribute");
    writeConsole(OUTPUT_INFO, "Loaded supergroup badges and pophelp");
#endif

#if SERVER
    loadstart_printf("loading combat mods..");
    load_CombatMods(&g_CombatModsPlayer, "defs/combat_mods.def");
    load_CombatMods(&g_CombatModsVillain, "defs/combat_mods_villain.def");
    loadend_printf("done");

    loadstart_printf("loading damagedecay..");
    load_DamageDecayConfig(&g_DamageDecayConfig, "defs/damagedecay.def");
    loadend_printf("done");
    
    loadstart_printf("loading inventory limits..");
    load_InventorySizes(&g_InventorySizeDefs, "defs/inventory_sizes.def");
    load_InventoryLoyaltyBonusSizes(&g_InventoryLoyaltyBonusSizeDefs, "defs/inventory_tier_bonus.def");
    loadend_printf("done");

    loadstart_printf("loading day jobs..");
    dayjobdetail_Load(); 
    loadend_printf("done");

    loadstart_printf("loading zone event karma mods..");
    load_ZoneEventKarmaMods(&g_ZoneEventKarmaTable, "defs/zone_event_karma.def");
    loadend_printf("done");    

    loadstart_printf("loading designer contact tips..");
    load_DesignerContactTipTypes(&g_DesignerContactTipTypes, "defs/designer_contact_tip_types.def");
    loadend_printf("done");    

    loadstart_printf("loading incarnate mods..");
    load_IncarnateMods(&g_IncarnateMods, "defs/incarnate.def");
    loadend_printf("done");    

     loadstart_printf("loading votekick specs..");
     load_EndGameRaidKickMods(&g_EndGameRaidKickMods, "defs/tut_votekick.def");
     loadend_printf("done");    

    loadstart_printf("loading MARTY level mods..");
    load_MARTYMods(&g_MARTYMods, "defs/MARTY_exp_mods.def");
    loadend_printf("done");    

    loadstart_printf("loading New Features List..");
    load_NewFeatures(&g_NewFeatureList, "defs/NewFeatures/NewFeatures.def");
    loadend_printf("done");
#endif

    writeConsole(OUTPUT_DEBUG, "Loading miscellaneous");
    load_CustomCritterMods(&g_PCCRewardMods,"defs/CustomCritterRewardMods.def");
    load_PowerConversionTable(&g_PowerSetConversionTable, "defs/PowersetConversion.def");
    load_AuctionConfig(&g_AuctionConfig, "defs/auctionconfig.def");
    load_AccountProducts();
    load_BoostChances(&g_pfBoostCombineChances, "defs/combine_chances.def");
    load_BoostChances(&g_pfBoostSameSetCombineChances, "defs/combine_same_set_chances.def");
    load_BoostEffectiveness(&g_pfBoostEffectivenessAbove, "defs/boost_effect_above.def");
    load_BoostEffectiveness(&g_pfBoostEffectivenessBelow, "defs/boost_effect_below.def");
    load_BoostChances(&g_pfBoostCombineBoosterChances, "defs/combine_booster_chances.def");
    load_BoostEffectiveness(&g_pfBoostEffectivenessBoosters, "defs/boost_effect_boosters.def");
    load_ExemplarHandicaps(&g_ExemplarHandicaps, "defs/exemplar_handicaps.def");
    load_ExperienceTables(&g_ExperienceTables, "defs/experience.def");
    load_Schedules(&g_Schedules, "defs/schedules.def");
    load_AttribDescriptions(&g_AttribCategoryList, "defs/attrib_descriptions.def");
    writeConsole(OUTPUT_INFO, "Loaded miscellaneous");
    
    /**********************************************************************/

    writeConsole(OUTPUT_DEBUG, "Adding defines");
    if(g_pParsePowerDefines!=NULL)
    {
        DefineDestroy(g_pParsePowerDefines);
    }
    g_pParsePowerDefines = DefineCreateFromIntList(s_PowerEnums);
    AddSequencerDefines(g_pParsePowerDefines);
    AddAttributeDefines(g_pParsePowerDefines);
    AddAttribNames(g_pParsePowerDefines);

#if SERVER
    if (g_pParseIncarnateDefines)
    {
        DefineDestroy(g_pParseIncarnateDefines);
    }
    g_pParseIncarnateDefines = DefineCreate();
    AddIncarnateMods(g_pParseIncarnateDefines);
#endif
    writeConsole(OUTPUT_INFO, "Added defines");
    
    /**********************************************************************/
    // Everything from here on needs ParsePowerDefines.

    writeConsole(OUTPUT_DEBUG, "Loading power dictionary and boost sets");
    load_DiminishingReturns( &g_DimReturnList, "defs/dim_returns.def", bNewAttribs ); // Must be before load_PowerDictionary
    load_PowerDictionary(&g_PowerDictionary, "defs/powers/", bNewAttribs, load_PowerDictionary_Callback);
    writeConsole(OUTPUT_INFO, "Loaded power dictionary and boost sets");
    
#if SERVER
    load_EmoteAnims(&g_EmoteAnims, "defs/emotes.def", bNewAttribs);
    LoadPetBattleCreatureInfos(); //"defs/petBattleCreatureInfo.def"
#endif

    writeConsole(OUTPUT_DEBUG, "Loading hero classes");
    load_CharacterClasses(&g_CharacterClasses, "defs/classes.def", bNewAttribs);
    writeConsole(OUTPUT_INFO, "Loaded hero classes");
    
    // must be done before bases and inventory
    writeConsole(OUTPUT_DEBUG, "Loaded inventory attribute files");
    load_InventoryAttribFiles( bNewAttribs ); 
    writeConsole(OUTPUT_INFO, "Loaded inventory attribute files");

    // Must be done before salvage is loaded and after powers
    writeConsole(OUTPUT_DEBUG, "Loading base data");
    basedata_Load(); 
    writeConsole(OUTPUT_INFO, "Loaded base data");

    writeConsole(OUTPUT_DEBUG, "Loading villain classes");
    load_CharacterClasses(&g_VillainClasses, "defs/villain_classes.def", bNewAttribs);
    writeConsole(OUTPUT_INFO, "Loaded villain classes");

    writeConsole(OUTPUT_DEBUG, "Loading salvage");
    load_SalvageDictionary( &g_SalvageDict, "defs/Invention/Salvage.salvage", bNewAttribs );
    load_ProficiencyDictionary( &g_ProficiencyDict, "defs/Invention/Proficiencies.proficiency", bNewAttribs );
#if SERVER
    load_SalvageTrackedList( &g_SalvageTrackedList, "defs/TrackedSalvage.def", bNewAttribs );
#endif
    writeConsole(OUTPUT_INFO, "Loaded salvage");

    writeConsole(OUTPUT_DEBUG, "Loading recipes");
    detailrecipes_Load(); // Must be done after salvage and details are loaded
    writeConsole(OUTPUT_INFO, "Loaded recipes");

#if CLIENT
    //writeConsole(OUTPUT_DEBUG, "Loading text links");
    init_TextLinkStash();
    writeConsole(OUTPUT_INFO, "Loaded text links");

    writeConsole(OUTPUT_DEBUG, "Loading arena maps list");
    arenaLoadMaps();
    writeConsole(OUTPUT_INFO, "Loaded arena maps list");

#ifndef TEST_CLIENT
    writeConsole(OUTPUT_DEBUG, "Loading costume weapon stance list");
    load_CostumeWeaponStancePairList();
    writeConsole(OUTPUT_INFO, "Loaded costume weapon stance list");
#endif
#endif
}


/**********************************************************************func*
 * ReloadClasses
 *
 */
static void ReloadClasses(CharacterClasses *pDest, CharacterClasses *pSrc)
{
    TokenizerParseInfo DestroyOldClassInfo[] =
    {
        { "AttribBase",    TOK_STRUCT(CharacterClass, ppattrBase, ParseCharacterAttributes) },
        { "AttribMin",     TOK_STRUCT(CharacterClass, ppattrMin, ParseCharacterAttributes) },
        { "StrengthMin",   TOK_STRUCT(CharacterClass, ppattrStrengthMin, ParseCharacterAttributes) },
        { "ResistanceMin", TOK_STRUCT(CharacterClass, ppattrResistanceMin, ParseCharacterAttributes )},
        { "ModTable",      TOK_STRUCT(CharacterClass, ppNamedTables,          ParseNamedTable)          },
        { "", 0, 0 }
    };
    extern TokenizerParseInfo ParseCharacterAttributesTable[];
    extern TokenizerParseInfo ParseNamedTable[];

    int i;
    int iSize = eaSize(&pSrc->ppClasses);
    for(i=0; i<iSize; i++)
    {
        const CharacterClass *pNew = pSrc->ppClasses[i];
        CharacterClass *pOld = (CharacterClass*)pDest->ppClasses[i];

        if(stricmp(pNew->pchName, pOld->pchName)!=0)
        {
            printf("Unmatched class name %s.\n", pNew->pchName);
            continue;
        }

        //free(pOld->pchName);
        //free(pOld->pchDisplayName);
        //free(pOld->pchDisplayHelp);
        //free(pOld->pchDisplayShortHelp);
        //free(pOld->pattrMax);
        //free(pOld->pattrMaxMax);
        //free(pOld->pattrStrengthMax);
        //free(pOld->pattrResistanceMax);

        //if (!isSharedMemory(pOld))
        //    ParserDestroyStruct(DestroyOldClassInfo, pOld);

        pOld->pchName             = pNew->pchName;
        pOld->pchDisplayName      = pNew->pchDisplayName;
        pOld->pchDisplayHelp      = pNew->pchDisplayHelp;
        pOld->pchDisplayShortHelp = pNew->pchDisplayShortHelp;
        pOld->pattrMax            = pNew->pattrMax;
        pOld->pattrMaxMax         = pNew->pattrMaxMax;
        pOld->pattrStrengthMax    = pNew->pattrStrengthMax;
        pOld->pattrResistanceMax  = pNew->pattrResistanceMax;
        pOld->ppattrBase          = pNew->ppattrBase;
        pOld->ppattrMin           = pNew->ppattrMin;
        pOld->ppattrStrengthMin   = pNew->ppattrStrengthMin;
        pOld->ppattrResistanceMin = pNew->ppattrResistanceMin;
        pOld->ppNamedTables       = pNew->ppNamedTables;
        pOld->namedStash          = pNew->namedStash;
    }
}

/**********************************************************************func*
 * ReloadBoostSets
 *
 */
static void ReloadBoostSets(BoostSetDictionary *pDest, const BoostSetDictionary *pSrc)
{
    int i;
    int iSize = eaSize(&pSrc->ppBoostSets);
    for(i=0; i<iSize; i++)
    {
        const BoostSet *pNew = pSrc->ppBoostSets[i];
        BoostSet *pOld = (BoostSet*)pDest->ppBoostSets[i];

        if(stricmp(pNew->pchName, pOld->pchName)!=0)
        {
            printf("Unmatched class name %s.\n", pNew->pchName);
            continue;
        }

        pOld->pchName             = pNew->pchName;
        pOld->pchDisplayName      = pNew->pchDisplayName;
        pOld->ppPowers            = pNew->ppPowers;
        pOld->ppBoostLists        = pNew->ppBoostLists;
        pOld->ppBonuses           = pNew->ppBonuses;
    }

    boostset_DestroyDictHashes(pDest);
    boostset_Finalize(pDest, false, NULL, false);
}

/**********************************************************************func*
 * load_AllDefsReload
 *
 */
void load_AllDefsReload(void)
{
    PowerDictionary NewPowerDictionary = {0};    
    CharacterClasses NewCharacterClasses = {0};
    CharacterClasses NewVillainClasses = {0};
    BoostSetDictionary NewBoostSetDict = {0};
//    CharacterOrigins NewCharacterOrigins;
//    CharacterOrigins NewVillainOrigins;

    ErrorfResetCounts();

    // release stuff in shared, assume it's old and the next process doesn't want our old data!
    sharedMemoryUnshare((void*)g_PowerDictionary.ppPowerCategories);
    sharedMemoryUnshare((void*)g_CharacterClasses.ppClasses);
    sharedMemoryUnshare((void*)g_VillainClasses.ppClasses);
    sharedMemoryUnshare((void*)g_BoostSetDict.ppBoostSets);

    // disable shared memory
    sharedMemorySetMode(SMM_DISABLED);

    if(g_pParsePowerDefines!=NULL)
    {
        DefineDestroy(g_pParsePowerDefines);
    }
    g_pParsePowerDefines = DefineCreateFromIntList(s_PowerEnums);
    AddSequencerDefines(g_pParsePowerDefines);
    AddAttributeDefines(g_pParsePowerDefines);
    AddAttribNames(g_pParsePowerDefines);

#if SERVER
    if (g_pParseIncarnateDefines)
    {
        DefineDestroy(g_pParseIncarnateDefines);
    }
    g_pParseIncarnateDefines = DefineCreate();
    AddIncarnateMods(g_pParseIncarnateDefines);
#endif

    // Load up the new powers, classes, and origin info.
    load_PowerDictionary(&NewPowerDictionary, "defs/powers/", 1, NULL); // force reload    
    load_CharacterClasses(&NewCharacterClasses, "defs/classes.def", 1); // force reload
//    load_CharacterOrigins(&NewCharacterOrigins, "defs/origins.def");

    load_CharacterClasses(&NewVillainClasses, "defs/villain_classes.def", 1); // force reload
//    load_CharacterOrigins(&NewVillainOrigins, "defs/villain_origins.def");

    // Now reload what we can
    //done in load_CharacterClasses: classeNewFinalize(&NewCharacterClasses, &g_PowerDictionary);
    //done in load_CharacterClasses: classeNewFinalize(&NewVillainClasses, &g_PowerDictionary);

    ReloadPowers((PowerDictionary*)&g_PowerDictionary, &NewPowerDictionary);
    ReloadClasses((CharacterClasses*)&g_CharacterClasses, &NewCharacterClasses);
//    ReloadOrigins(&g_CharacterOrigins, &NewCharacterOrigins);

    ReloadClasses((CharacterClasses*)&g_VillainClasses, &NewVillainClasses);
//    ReloadOrigins(&g_VillainOrigins, &NewVillainOrigins);

    load_BoostSetDictionary(&NewBoostSetDict, "defs/test.boostset", NULL, 1);
    ReloadBoostSets((BoostSetDictionary*)&g_BoostSetDict, &NewBoostSetDict);
}

/* End of File */
