/*
 *
 *    turnstileCommon.c - Copyright 2010 NC Soft
 *        All Rights Reserved
 *        Confidential property of NC Soft
 *
 *    Turnstile system for matching players for large raid content
 *
 */

#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/assert/assert.h>
#include "entity/teamCommon.h"
#include "TurnstileServerCommon.h"
#include <math.h>
#if SERVER
#include "entity/character_eval.h"
#endif

static StaticDefineInt parse_TUT_MissionType[] =
{
    DEFINE_INT
    {    "Contact",    TUT_MissionType_Contact },
    {    "Instance",    TUT_MissionType_Instance },
    DEFINE_END
};

static TokenizerParseInfo ParseDBServer[] =
{
    { ".",                        TOK_STRUCTPARAM | TOK_STRING(DBServerCfg, serverName, 0),        },
    { ".",                        TOK_STRUCTPARAM | TOK_STRING(DBServerCfg, serverPublicName, 0), },
    { ".",                        TOK_STRUCTPARAM | TOK_INT(DBServerCfg, serverPort, 0),            },
    { ".",                        TOK_STRUCTPARAM | TOK_STRING(DBServerCfg, shardName, 0)            },
    { ".",                        TOK_STRUCTPARAM | TOK_INT(DBServerCfg, shardID, 0)                },
    { ".",                        TOK_STRUCTPARAM | TOK_INT(DBServerCfg, authShardID, 0)            },
    { "\n",                        TOK_END, 0                                                        },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseATDesc[] =
{
    { ".",                        TOK_STRUCTPARAM | TOK_STRING(ATDesc, className, 0),                },
    { ".",                        TOK_STRUCTPARAM | TOK_INT(ATDesc, dps, 0)                        },
    { ".",                        TOK_STRUCTPARAM | TOK_INT(ATDesc, tank, 0)                        },
    { ".",                        TOK_STRUCTPARAM | TOK_INT(ATDesc, buff, 0)                        },
    { ".",                        TOK_STRUCTPARAM | TOK_INT(ATDesc, melee, 0)                        },
    { "\n",                        TOK_END, 0                                                        },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseMission[] =
{
    { "{",                        TOK_START, 0                                                    },
    { "",                        TOK_STRUCTPARAM | TOK_STRING(TurnstileMission, name, 0)            },
    { "Category",                TOK_INT(TurnstileMission, category, 0), ParseTurnstileCategory    },
    { "MinimumPlayers",            TOK_INT(TurnstileMission, preformedMinimumPlayers, 0)                    },
    { "PickupMinPlayers",        TOK_INT(TurnstileMission, pickupMinimumPlayers, 0)                    },
    { "MaximumPlayers",            TOK_INT(TurnstileMission, maximumPlayers, 0)                    },
    { "StartDelay",                TOK_INT(TurnstileMission, startDelay, 0)                        },
    { "LevelShift",                TOK_INT(TurnstileMission, levelShift, 0)                        },
    { "ForceMissionLevel",        TOK_INT(TurnstileMission, forceMissionLevel, 0)                    },
    { "MinimumTank",            TOK_INT(TurnstileMission, minimumTank, 0)                        },
    { "MinimumBuff",            TOK_INT(TurnstileMission, minimumBuff, 0)                        },
    { "MinimumDPS",                TOK_INT(TurnstileMission, minimumDPS, 0)                        },
    { "MinimumMelee",            TOK_INT(TurnstileMission, minimumMelee, 0)                        },
    { "MinimumRanged",            TOK_INT(TurnstileMission, minimumRanged, 0)                        },
    { "Requires",                TOK_STRING(TurnstileMission, required, 0)                        },
    { "HideIf",                    TOK_STRING(TurnstileMission, hideIf, 0)                            },
    { "Type",                    TOK_INT(TurnstileMission, type, TUT_MissionType_Instance),    parse_TUT_MissionType        },
    { "MapName",                TOK_STRINGARRAY(TurnstileMission, mapName)                        },
    { "Script",                    TOK_STRING(TurnstileMission, script, 0)                            },
    { "Pos",                    TOK_VEC3(TurnstileMission, pos)                                    },
    { "Location",                TOK_STRING(TurnstileMission, location, 0)                        },
    { "Radius",                    TOK_F32(TurnstileMission, radius, 5)                            },
    { "LinkTo",                    TOK_STRINGARRAY(TurnstileMission, linkToStr)                    },
    { "AutoQueueMyLinks",        TOK_BOOLFLAG(TurnstileMission, autoQueueMyLinks, 0)                },
    { "PlayerGang",                TOK_INT(TurnstileMission, playerGang, 1)                        },
    { "}",                        TOK_END, 0                                                        },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseTurnstileCfg[] =
{
    { "dbserver",                TOK_STRUCT(TurnstileConfigCfg, dbservers, ParseDBServer)        },
    { "FastEMA",                TOK_INT(TurnstileConfigCfg, fastEMA, 0)                            },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseTurnstileDef[] =
{
    { "ATDesc",                    TOK_STRUCT(TurnstileConfigDef, atDesc, ParseATDesc)                },
    { "Mission",                TOK_STRUCT(TurnstileConfigDef, missions, ParseMission)            },
    { "", 0, 0 }
};

TurnstileConfigCfg turnstileConfigCfg;
TurnstileConfigDef turnstileConfigDef;

// DGNOTE - I've set this somewhat arbitrarily to 1000 players 
#define        EMA_FACTOR        0.9993070930        // Gives a half life of 1000 players.  Other values to try are:
//            EMA_FACTOR        0.9862327045        // Gives a half life of 50 players.
//            EMA_FACTOR        0.9930924954        // Gives a half life of 100 players.
//            EMA_FACTOR        0.9972312514        // Gives a half life of 250 players.
//            EMA_FACTOR        0.9986146661        // Gives a half life of 500 players.
//            EMA_FACTOR        0.9993070930        // Gives a half life of 1000 players.
//            EMA_FACTOR        0.9997227796        // Gives a half life of 2500 players.
//            EMA_FACTOR        0.9998613802        // Gives a half life of 5000 players.
//            EMA_FACTOR        0.9999306877        // Gives a half life of 10000 players.
// General rule for these is that EMA_FACTOR is (0.5 ^ (1 / numplayers))
#define        EMA_FACTOR_10    0.9330329915        // DEBUG value - half life of 10 players

// DGNOTE - TODO
// Rather than use a hard coded EMA factor, dynamically change it.  This would do two things.  Until the use count (number of
// players, *NOT* number of calls) has exceeded 10,000, keep the ema factor at something in the region of numplayers / 10, with a
// minimum of 100.  That'll tend to make it adjust fairly quickly.
// Once we reach steady state (i.e. over 10,000 players, keep the ema factor to the number of players that have used it in the last hour.
// This is probably best done by simply running a count that resets every 15 minutes, and whenever it resets, tweak the ema factor.

// Compute the EMA factor for the given half life, i.e. the "ith" root of 1/2
static double EMAFactor(int i)
{
    return pow(0.5, 1.0 / (double) i);
}

// Update the EMA average wait time, assuming that numPlayers players are entering an event after waiting seconds seconds
float updateAverageWaitTime(float oldTime, int numPlayers, int seconds)
{
    // Do all the number crunch at double precision
    double factor;
    double keepFraction;                        // This is the fraction of the current EMA we'll keep
    double replaceFraction;                        // This is the fraction we'll replace, i.e. 1 - keep
    double ema;                                    // Final result

    factor = turnstileConfigCfg.fastEMA ? EMA_FACTOR_10 : EMA_FACTOR;
    keepFraction = pow(factor, (double) numPlayers);
    replaceFraction = 1.0 - keepFraction;
    ema = (double) oldTime * keepFraction + (double) seconds * replaceFraction;
    return (float) ema;
}

static void s_matchUpLinks(TurnstileMission *mission)
{
    int i;
    if (mission->autoQueueMyLinks)
    {
        assertmsg(eaSize(&mission->linkToStr), "Error: Can't autoQueue since mission has no links");
    }
    for (i = 0; i < eaSize(&mission->linkToStr); ++i)
    {
        //    match up the names here
        int j;
        for (j = 0; j < eaSize(&turnstileConfigDef.missions); ++j)
        {
            if (stricmp(mission->linkToStr[i], turnstileConfigDef.missions[j]->name) == 0)
            {
                assertmsg(!eaSize(&turnstileConfigDef.missions[j]->linkToStr), "Error: Can't link to a mission that is a master link");
                assertmsg(mission->missionID != j, "Error: Can't link to yourself");
                assertmsg(mission->category == turnstileConfigDef.missions[j]->category, "Error: Category must be the same");
                assertmsg(mission->type == turnstileConfigDef.missions[j]->type, "Error: Type must be the same");
                if (mission->missionID != j && !eaSize(&turnstileConfigDef.missions[j]->linkToStr))
                {
                    turnstileConfigDef.missions[j]->isLink = mission->autoQueueMyLinks ? 2 : 1;
                    eaiPush(&mission->linkTo, j);
                }                
                break;
            }
        }
        assertmsg(j < eaSize(&turnstileConfigDef.missions), "Error: Can't link missions properly. Mission not found");
    }
}

bool TurnstileDefPostProcess(ParseTable pti[], TurnstileConfigDef *configDef)
{
#if SERVER
    int i;

    for (i = 0; i < eaSize(&configDef->missions); i++)
    {
        TurnstileMission *mission = configDef->missions[i];

        if (mission->hideIf)
        {
            chareval_requiresValidate(mission->hideIf, "defs/turnstile_server.def");
        }
        if (mission->required)
        {
            chareval_requiresValidate(mission->required, "defs/turnstile_server.def");
        }
    }
#endif
    return true;
}

// Slurp in the config files.
int turnstileParseTurnstileConfig(int loadFlags)
{
    int i;
    char buf[MAX_PATH];

    StructInit(&turnstileConfigCfg, sizeof(turnstileConfigCfg), ParseTurnstileCfg);
    StructInit(&turnstileConfigDef, sizeof(turnstileConfigDef), ParseTurnstileDef);

    if (loadFlags & TURNSTILE_LOAD_CONFIG)
    {
        // Only read turnstile_server.cfg on the turnstile server itself
        if (!fileLocateRead("server/db/turnstile_server.cfg", buf))
        {
            printf("\nError: can't locate server/db/turnstile_server.cfg\n");
            return 0;
        }
        if (!ParserLoadFiles(NULL, buf, NULL, 0, ParseTurnstileCfg, &turnstileConfigCfg, NULL, NULL, NULL))
        {
            return 0;
        }
    }
    if (loadFlags & TURNSTILE_LOAD_DEF)
    {
        if (!ParserLoadFiles(NULL, "defs/turnstile_server.def", "turnstile_server.bin", PARSER_SERVERONLY, ParseTurnstileDef, &turnstileConfigDef, NULL, NULL, (ParserLoadPreProcessFunc)TurnstileDefPostProcess))
        {
            return 0;
        }
        for (i = eaSize(&turnstileConfigDef.missions) - 1; i >= 0; i--)
        {
            // Init avg wait time to 5 mins, it'll adjust over time
            turnstileConfigDef.missions[i]->avgWait = INITIAL_WAIT_TIME;
            // Limit the level shift to -1 thru +4
            if (turnstileConfigDef.missions[i]->levelShift < -1)
            {
                turnstileConfigDef.missions[i]->levelShift = -1;
            }
            else if (turnstileConfigDef.missions[i]->levelShift > 4)
            {
                turnstileConfigDef.missions[i]->levelShift = 4;
            }
            turnstileConfigDef.missions[i]->missionID = i;
            if (!turnstileConfigDef.missions[i]->pickupMinimumPlayers)
            {
                turnstileConfigDef.missions[i]->pickupMinimumPlayers = turnstileConfigDef.missions[i]->preformedMinimumPlayers;
            }
            assert(turnstileConfigDef.missions[i]->preformedMinimumPlayers <= turnstileConfigDef.missions[i]->pickupMinimumPlayers);
            assert(turnstileConfigDef.missions[i]->pickupMinimumPlayers <= turnstileConfigDef.missions[i]->maximumPlayers);
            s_matchUpLinks(turnstileConfigDef.missions[i]);
        }
        turnstileConfigCfg.overallAverage = INITIAL_WAIT_TIME;
    }
    return 1;
}

// If something goes wrong during readin we delete the missions.  This *does* potentially lead to a slightly
// inconsistant state in the event that there's an error reading the DBserver list.  In that case the turnstile
// server config read will fail while the mapserver will succeed.  On the other hand the turnstile server is
// going to go south, and make a very loud noise doing so, therefore someone should fix things up.
void turnstileDestroyConfig()
{
    StructClear(ParseTurnstileCfg, &turnstileConfigCfg);
    StructClear(ParseTurnstileDef, &turnstileConfigDef);
}

// Search for shard config data either by name or number
DBServerCfg *turnstileFindShardByName(char *shardName)
{
    int i;

    for (i = eaSize(&turnstileConfigCfg.dbservers) - 1; i >= 0; i--)
    {
        if (stricmp(turnstileConfigCfg.dbservers[i]->shardName, shardName) == 0)
        {
            return turnstileConfigCfg.dbservers[i];
        }
    }
    return NULL;
}

DBServerCfg *turnstileFindShardByNumber(int shardID)
{
    int i;

    for (i = eaSize(&turnstileConfigCfg.dbservers) - 1; i >= 0; i--)
    {
        if (turnstileConfigCfg.dbservers[i]->shardID == shardID)
        {
            return turnstileConfigCfg.dbservers[i];
        }
    }
    return NULL;
}
