/***************************************************************************
 *     Copyright (c) 2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include "entity/TaskforceParams.h"
#include <utilitieslib/utils/SuperAssert.h>
#include "entity/teamCommon.h"
#include "entity/entity.h"
#include <utilitieslib/utils/eval.h>

#if defined(SERVER)
#include "container/team.h"
#include "entity/entGameActions.h"
#include <utilitieslib/utils/timing.h>
#include "player/badges_server.h"
#include "dbcomm/dbcomm.h"
#include <utilitieslib/utils/structDefines.h>
#include "entity/character_base.h"
#include "entity/powers.h"
#include "entity/PowerInfo.h"
#include <utilitieslib/components/earray.h>
#include "entity/character_combat.h"
#include "storyarc/taskforce.h"
#include "dbcomm/dbcontainer.h"
#include "container/containerbroadcast.h"
#include "gameComm/VillainDef.h"
#include "ai/entai.h"
#include <utilitieslib/utils/textparser.h>
#include "entity/LoadDefCommon.h"
#include "storyarc/storyarcprivate.h"
#include "comm_game.h"
#include "gameData/miningaccumulator.h"
#include "dbcomm/logcomm.h"
#include "entity/character_level.h"

#define TIME_LIMIT_DEF_FILENAME        "defs/TFTimeLimits.def"
#endif

typedef struct SLLevelMapping
{
    TFParamSL stature;
    int minLevel;
    int maxLevel;
} SLLevelMapping;

static SLLevelMapping SLMappings[] =
{
    TFPARAM_SL1,    1,    5,
    TFPARAM_SL2,    6,    10,
    TFPARAM_SL25,    11,    14,
    TFPARAM_SL3,    15,    19,
    TFPARAM_SL4,    20,    24,
    TFPARAM_SL5,    25,    29,
    TFPARAM_SL6,    30,    34,
    TFPARAM_SL7,    35,    39,
    TFPARAM_SL8,    40,    45,
    TFPARAM_SL9,    46,    50
};

#if defined(SERVER)
typedef struct StoryArcTimeLimit
{
    StoryArc* arcDef;
    char* fileName;
    U32 bronze;
    U32 silver;
    U32 gold;
} StoryArcTimeLimit;

typedef struct StoryArcTimeLimitList
{
    StoryArcTimeLimit** limits;
} StoryArcTimeLimitList;

static ParseTable ParseStoryArcTimeLimits[] =
{
    { "{", TOK_START, 0 },
    { "Filename", TOK_STRING(StoryArcTimeLimit, fileName, 0) },
    { "Gold", TOK_INT(StoryArcTimeLimit, gold, 0) },
    { "Silver", TOK_INT(StoryArcTimeLimit, silver, 0) },
    { "Bronze", TOK_INT(StoryArcTimeLimit, bronze, 0) },
    { "}", TOK_END, 0 },
    { 0 }
};

extern const StoryArcList g_storyarclist;

static StoryArcTimeLimitList StoryArcTimeLimits;

static ParseTable ParseStoryArcTimeLimit[] =
{
    { "StoryArc", TOK_STRUCT(StoryArcTimeLimitList, limits, ParseStoryArcTimeLimits), 0 },
    { 0 }
};

static StaticDefineInt MaxDefeats[] =
{
    DEFINE_INT
    // This means we don't care about the setting, just that the challenge
    // was enabled.
    { "ANY",    TFDEATHS_INFINITE            },

    // These mean the challenge was enabled and the specific number of
    // defeats was set initially.
    { "NONE",    TFDEATHS_NONE                },
    { "1_ONLY",    TFDEATHS_ONE                },
    { "3_ONLY",    TFDEATHS_THREE                },
    { "5_ONLY",    TFDEATHS_FIVE                },
    { "1_EACH",    TFDEATHS_ONE_PER_MEMBER        },
    { "3_EACH",    TFDEATHS_THREE_PER_MEMBER    },
    { "5_EACH", TFDEATHS_FIVE_PER_MEMBER    },
    DEFINE_END
};

static StaticDefineInt TimeLimits[] =
{
    DEFINE_INT
    // We don't care about the value, just that the challenge was enabled.
    { "ANY",    TFTIMELIMITS_NONE        },

    // For these the initial time has to match as well.
    { "GOLD",    TFTIMELIMITS_GOLD        },
    { "SILVER",    TFTIMELIMITS_SILVER        },
    { "BRONZE",    TFTIMELIMITS_BRONZE        },
    DEFINE_END
};

static StaticDefineInt DisabledPowers[] =
{
    DEFINE_INT
    // We don't care about the value, just that the challenge was enabled.
    { "ANY",        TFPOWERS_ALL            },
    { "ONLY_AT",    TFPOWERS_ONLY_AT        },
    { "NO_TEMPS",    TFPOWERS_NO_TEMP        },
    { "NO_EPIC",    TFPOWERS_NO_EPIC_POOL    },
    { "NO_TRAVEL",    TFPOWERS_NO_TRAVEL        },
    DEFINE_END
};
#endif

#if defined(SERVER)
static void FixupTimeLimits(StoryArcTimeLimit* limit)
{
    int count;
    char* limit_leaf_name;
    char* limit_ext_pos;
    int limit_len;
    char* arc_leaf_name;

    devassert(limit);

    if (limit)
    {
        if (!limit->fileName)
        {
            // We need a filename to match against the story arc otherwise
            // there's nothing we can do.
            return;
        }

        limit_leaf_name = strrchr(limit->fileName, '\\');

        if (!limit_leaf_name)
        {
            limit_leaf_name = strrchr(limit->fileName, '/');
        }

        if (limit_leaf_name)
        {
            // Skip the separator, because there might not be one on the
            // story arc name so we can't have it in the comparison.
            limit_leaf_name++;
        }
        else
        {
            // There was no separator, start from the beginning.
            limit_leaf_name = limit->fileName;
        }

        limit_ext_pos = strrchr(limit_leaf_name, '.');

        if (limit_ext_pos)
        {
            limit_len = (limit_ext_pos - limit_leaf_name);
        }
        else
        {
            limit_len = (int)strlen(limit_leaf_name);
        }

        // Match filename to story arc pointer in list
        for (count = 0; count < eaSize(&g_storyarclist.storyarcs); count++)
        {
            arc_leaf_name =
                strrchr(g_storyarclist.storyarcs[count]->filename, '\\');

            if (!arc_leaf_name)
            {
                arc_leaf_name =
                    strrchr(g_storyarclist.storyarcs[count]->filename, '/');
            }

            if (arc_leaf_name)
            {
                // Skip any separator - the time limit data might not have
                // used one.
                arc_leaf_name++;
            }
            else
            {
                // There was no separator so we start at the beginning.
                arc_leaf_name = g_storyarclist.storyarcs[count]->filename;
            }

            if (strnicmp(limit_leaf_name, arc_leaf_name, limit_len) == 0)
            {
                limit->arcDef = g_storyarclist.storyarcs[count];
                break;
            }
        }

        // We can fill in the other values if we have at least one given
        // the relationship that each medal is half the time of the next.
        if (limit->bronze || limit->silver || limit->gold)
        {
            if (!limit->bronze)
            {
                if (!limit->silver)
                {
                    limit->silver = limit->gold * 2;
                }

                limit->bronze = limit->silver * 2;
            }

            if (!limit->silver)
            {
                if (limit->gold)
                {
                    limit->silver = limit->gold * 2;
                }
                else
                {
                    limit->silver = MAX((limit->bronze / 2), 1);
                    limit->gold = MAX((limit->silver / 2), 1);
                }
            }

            if (!limit->gold)
            {
                limit->gold = MAX((limit->silver / 2), 1);
            }

            // Let the .def file values be entered in minutes and then we
            // turn them into seconds for use internally.
            limit->bronze *= 60;
            limit->silver *= 60;
            limit->gold *= 60;
        }
        else
        {
            // We need at least ONE of the values to make something
            // sensible.
            ErrorFilenamef(TIME_LIMIT_DEF_FILENAME, "Story arc %s needs at least one time limit!", limit_leaf_name);
        }
    }
}

static int TimeLimits_CompareArcPointers(const void* firstData, const void* secondData)
{
    const StoryArcTimeLimit** first = (const StoryArcTimeLimit**)firstData;
    const StoryArcTimeLimit** second = (const StoryArcTimeLimit**)secondData;
    int retval;

    devassert(first);
    devassert(*first);
    devassert(second);
    devassert(*second);

    retval = 0;

    if (first && *first && second && *second)
    {
        retval = (*first)->arcDef - (*second)->arcDef;
    }

    return retval;
}

static int TimeLimits_SearchArcPointers(const void* arcData, const void* limitData)
{
    const StoryArc** arc = (const StoryArc**)arcData;
    const StoryArcTimeLimit** limit = (const StoryArcTimeLimit**)limitData;
    int retval;

    devassert(arc);
    devassert(limit);
    devassert(*limit);

    retval = 0;

    if (arc && limit && *limit)
    {
        retval = *arc - (*limit)->arcDef;
    }

    return retval;
}

static bool TaskForceEvalMaxDefeatsHelper(Entity* player, const char* rhs)
{
    bool retval;
    int defeats;

    retval = false;

    if (TaskForceIsParameterEnabled(player, TFPARAM_DEATHS) &&
        !TaskForceCheckFailureBit(player, TFPARAM_DEATHS))
    {
        defeats = StaticDefineIntGetInt(MaxDefeats, rhs);

        switch (defeats)
        {
            case TFDEATHS_INFINITE:
                // This is the 'any' result so we don't need to check the
                // starting lives value.
                retval = true;
                break;

            case TFDEATHS_NONE:
            case TFDEATHS_ONE:
            case TFDEATHS_THREE:
            case TFDEATHS_FIVE:
            case TFDEATHS_ONE_PER_MEMBER:
            case TFDEATHS_THREE_PER_MEMBER:
            case TFDEATHS_FIVE_PER_MEMBER:
                if (player->taskforce->parameters[TFPARAM_DEATHS_SETUP - 1] == defeats)
                {
                    retval = true;
                }

                break;

            case -1:
            default:
                // No suitable parameter to interrogate.
                break;
        }
    }

    return retval;
}

static bool TaskForceEvalTimeLimitHelper(Entity* player, const char* rhs)
{
    bool retval;
    int limit;
    const StoryArc* arc;
    U32 arc_limit;

    retval = false;

    if (TaskForceIsParameterEnabled(player, TFPARAM_TIME_LIMIT) &&
        !TaskForceCheckFailureBit(player, TFPARAM_TIME_LIMIT))
    {
        limit = StaticDefineIntGetInt(TimeLimits, rhs);

        if (limit == TFTIMELIMITS_NONE)
        {
            // We just need to have been enabled for the challenge, any
            // time limit works.
            retval = true;
        }
        else if (limit != -1)
        {
            // The current story arc for the TF is always stored as the
            // first (and only) arc of the story info.
            arc = player->taskforce->storyInfo->storyArcs[0]->def;
            arc_limit = TaskForceGetStoryArcTimeLimit(arc, limit);

            // We must match the specific time limit.
            if (player->taskforce->parameters[TFPARAM_TIME_LIMIT_SETUP - 1] == arc_limit)
            {
                retval = true;
            }
        }
    }

    return retval;
}

static bool TaskForceEvalDisabledPowersHelper(Entity* player, const char* rhs)
{
    bool retval;
    int powers;

    retval = false;

    if (TaskForceIsParameterEnabled(player, TFPARAM_DISABLE_POWERS) &&
        !TaskForceCheckFailureBit(player, TFPARAM_DISABLE_POWERS))
    {
        powers = StaticDefineIntGetInt(DisabledPowers, rhs);

        switch (powers)
        {
            case TFPOWERS_ALL:
                // This is the 'any' result so we're always good (we
                // checked that the challenge was enabled already).
                retval = true;
                break;

            case TFPOWERS_NO_TEMP:
            case TFPOWERS_NO_TRAVEL:
            case TFPOWERS_NO_EPIC_POOL:
            case TFPOWERS_ONLY_AT:
                // Check that the challenge was set to the appropriate
                // value for the query.
                if (player->taskforce->parameters[TFPARAM_DISABLE_POWERS - 1] == powers)
                {
                    retval = true;
                }

            case -1:
            default:
                // No suitable parameter to interrogate.
                break;
        }
    }

    return retval;
}

static bool TaskForceSetParameter(TaskForce *tf, TaskForceParameter param,
    U32 paramVal)
{
    bool retval = false;

    devassert(tf);
    devassert(param != TFPARAM_INVALID);
    devassert(param < MAX_TASKFORCE_PARAMETERS);

    if (tf && param > TFPARAM_INVALID && param < MAX_TASKFORCE_PARAMETERS)
    {
        // Enum is in the range 1-32 but we need to adjust that to do the
        // bitfield math. We also check if the bit was already set so we can
        // alert the caller.
        if (!(tf->parametersSet & (1 << (param - 1))))
        {
            retval = true;
        }

        tf->parametersSet |= (1 << (param - 1));
        tf->parameters[param - 1] = paramVal;
    }

    return retval;
}

static bool TaskForceClearParameter(TaskForce *tf, TaskForceParameter param)
{
    bool retval = false;

    devassert(tf);
    devassert(param != TFPARAM_INVALID);
    devassert(param < MAX_TASKFORCE_PARAMETERS);

    if (tf && param > TFPARAM_INVALID && param < MAX_TASKFORCE_PARAMETERS)
    {
        if (tf->parametersSet & (1 << (param - 1)))
        {
            retval = true;
        }

        tf->parametersSet &= ~(1 << (param - 1));
        tf->parameters[param - 1] = 0;
    }

    return retval;
}

static bool TaskForceSetFailureBit(TaskForce* tf, TaskForceParameter param)
{
    bool retval;

    devassert(tf);
    devassert(param > TFPARAM_INVALID && param < (TFPARAM_FAILURE_BITS - 1));

    retval = false;

    if (tf && param > TFPARAM_INVALID && param < (TFPARAM_FAILURE_BITS - 1))
    {
        // Do the check so we can report whether the bit was already set.
        if (!(tf->parameters[TFPARAM_FAILURE_BITS - 1] & (1 << (param - 1))))
        {
            tf->parameters[TFPARAM_FAILURE_BITS - 1] |=
                (1 << (param - 1));

            retval = true;
        }
    }

    return retval;
}

static bool TaskForceUpdateParameter(TaskForce* tf,
    TaskForceParameter param, U32 newVal)
{
    bool retval;

    devassert(tf);

    retval = false;

    if (tf)
    {
        if (tf->parametersSet & (1 << (param - 1)))
        {
            tf->parameters[param - 1] = newVal;
            retval = true;
        }
    }

    return retval;
}

static bool TaskForceIsChallengeComplete(TaskForce* tf,
     TaskForceParameter param)
{
    bool retval;
    U32 bits;

    devassert(tf);

    retval = false;

    if (tf)
    {
        if (tf->parametersSet & (1 << (param - 1)))
        {
            bits = tf->parameters[TFPARAM_FAILURE_BITS - 1];

            if (!(bits & (1 << (param - 1))))
            {
                retval = true;
            }
        }
    }

    return retval;
}

static bool TaskForceApplyTempPower(Entity* player, char* power_name)
{
    bool retval;
    const BasePower* pow_base;
    PowerSet* pow_set;
    Power* pow;
    PowerRef* ppowRef;

    devassert(player);
    devassert(power_name);

    retval = false;

    if (player && player->pchar && power_name)
    {
        pow_base = powerdict_GetBasePowerByName(&g_PowerDictionary,
            "Temporary_Powers", "Temporary_Powers", power_name);

        if (pow_base)
        {
            pow_set = character_BuyPowerSet(player->pchar,
                pow_base->psetParent);
            pow = character_BuyPower(player->pchar, pow_set, pow_base, 0);

            if (pow)
            {
                ppowRef = powerRef_CreateFromPower(player->pchar->iCurBuild, pow);

                if (ppowRef)
                {
                    eaPush(&(player->powerDefChange), ppowRef);
                    character_ActivatePowerOnCharacter(player->pchar,
                        player->pchar, pow);
                    if (pow_base->bShowBuffIcon)
                        player->team_buff_update = true;

                    retval = true;
                }
            }
        }
    }

    return retval;
}
static void TaskForceChalkCompletionTimeStat(char* arc_leafname,
    U32 member_count, U32 time_minutes)
{
    char namebuf[100];
    char totalbuf[100];
    char countbuf[100];

    //devassert(arc_leafname);
    devassert(member_count > 0 && member_count <= 8);

    if (arc_leafname && member_count > 0 && member_count <= 8)
    {
        sprintf(namebuf, "TFChallenge_Arc.%s", arc_leafname);

        if (member_count == 1)
        {
            strcpy_s(totalbuf, 100, "1_avg");
            strcpy_s(countbuf, 100, "1_count");
        }
        else if (member_count >= 2 && member_count <= 4)
        {
            strcpy_s(totalbuf, 100, "234_avg");
            strcpy_s(countbuf, 100, "234_count");
        }
        else if (member_count >= 5)
        {
            strcpy_s(totalbuf, 100, "5678_avg");
            strcpy_s(countbuf, 100, "5678_count");
        }

        MiningAccumulatorAdd(namebuf, totalbuf, time_minutes);
        MiningAccumulatorAdd(namebuf, countbuf, 1);
    }
}
#endif

static void TaskForceEvalMaxDefeats(EvalContext* context)
{
    Entity* e;
    const char* rhs;
    int result;

    devassert(context);

    if (context)
    {
        result = 0;
        rhs = eval_StringPop(context);

        if (eval_FetchInt(context, "entity", (int*)(&e)) &&
            e && e->pl)
        {
#if defined(SERVER)
            if (e->taskforce_id && e->taskforce &&
                TaskForceEvalMaxDefeatsHelper(e, rhs))
            {
                result = 1;
            }
#endif
        }

        eval_IntPush(context, result);
    }
}

static void TaskForceEvalTimeLimit(EvalContext* context)
{
    Entity* e;
    const char* rhs;
    int result;

    devassert(context);

    if (context)
    {
        result = 0;
        rhs = eval_StringPop(context);

        if (eval_FetchInt(context, "entity", (int*)(&e)) &&
            e && e->pl)
        {
#if defined(SERVER)
            if (e->taskforce_id && e->taskforce &&
                TaskForceEvalTimeLimitHelper(e, rhs))
            {
                result = 1;
            }
#endif
        }

        eval_IntPush(context, result);
    }
}

static void TaskForceEvalNoEnhancements(EvalContext* context)
{
    Entity* e;
    int result;

    devassert(context);

    if (context)
    {
        result = 0;

        if (eval_FetchInt(context, "entity", (int*)(&e)) &&
            e && e->pl)
        {
#if defined(SERVER)
            if (e->taskforce_id && e->taskforce &&
                TaskForceIsChallengeComplete(e->taskforce,
                    TFPARAM_DISABLE_ENHANCEMENTS))
            {
                result = 1;
            }
#endif
        }

        eval_IntPush(context, result);
    }
}

static void TaskForceEvalDisabledPowers(EvalContext* context)
{
    Entity* e;
    const char* rhs;
    int result;

    devassert(context);

    if (context)
    {
        result = 0;
        rhs = eval_StringPop(context);

        if (eval_FetchInt(context, "entity", (int*)(&e)) &&
            e && e->pl)
        {
#if defined(SERVER)
            if (e->taskforce_id && e->taskforce &&
                TaskForceEvalDisabledPowersHelper(e, rhs))
            {
                result = 1;
            }
#endif
        }

        eval_IntPush(context, result);
    }
}

static void TaskForceEvalBuffEnemies(EvalContext* context)
{
    Entity* e;
    int result;

    devassert(context);

    if (context)
    {
        result = 0;

        if (eval_FetchInt(context, "entity", (int*)(&e)) &&
            e && e->pl)
        {
#if defined(SERVER)
            if (e->taskforce_id && e->taskforce &&
                TaskForceIsChallengeComplete(e->taskforce,
                    TFPARAM_BUFF_ENEMIES))
            {
                result = 1;
            }
#endif
        }

        eval_IntPush(context, result);
    }
}

static void TaskForceEvalDebuffPlayers(EvalContext* context)
{
    Entity* e;
    int result;

    devassert(context);

    if (context)
    {
        result = 0;

        if (eval_FetchInt(context, "entity", (int*)(&e)) &&
            e && e->pl)
        {
#if defined(SERVER)
            if (e->taskforce_id && e->taskforce &&
                TaskForceIsChallengeComplete(e->taskforce,
                    TFPARAM_DEBUFF_PLAYERS))
            {
                result = 1;
            }
#endif
        }

        eval_IntPush(context, result);
    }
}

static void TaskForceEvalNoInspirations(EvalContext* context)
{
    Entity* e;
    int result;

    devassert(context);

    if (context)
    {
        result = 0;

        if (eval_FetchInt(context, "entity", (int*)(&e)) &&
            e && e->pl)
        {
#if defined(SERVER)
            if (e->taskforce_id && e->taskforce &&
                TaskForceIsChallengeComplete(e->taskforce,
                    TFPARAM_NO_INSPIRATIONS))
            {
                result = 1;
            }
#endif
        }

        eval_IntPush(context, result);
    }
}

bool TaskForceIsParameterEnabled(Entity* player, TaskForceParameter param)
{
    bool retval = false;

    devassert(player);
    devassert(param > TFPARAM_INVALID && param < MAX_TASKFORCE_PARAMETERS);

    if (player && param > TFPARAM_INVALID && param < MAX_TASKFORCE_PARAMETERS)
    {
        if (player->taskforce)
        {
            // 'Invalid' is 0 so we subtract 1 to get the bit index in
            // the bitfield.
            if (player->taskforce->parametersSet & (1 << (param - 1)))
            {
                retval = true;
            }
        }
    }

    return retval;
}

U32 TaskForceGetParameter(Entity* player, TaskForceParameter param)
{
    U32 retval = 0;

    if (TaskForceIsParameterEnabled(player, param))
    {
        retval = player->taskforce->parameters[param - 1];
    }

    return retval;
}

void TaskForceEvalAddDefaultFuncs(EvalContext* context)
{
    if (context)
    {
        eval_RegisterFunc(context, "TFChallenge.maxDefeats", TaskForceEvalMaxDefeats, 1, 1);
        eval_RegisterFunc(context, "TFChallenge.timeLimit", TaskForceEvalTimeLimit, 1, 1);
        eval_RegisterFunc(context, "TFChallenge.noEnhancements", TaskForceEvalNoEnhancements, 0, 1);
        eval_RegisterFunc(context, "TFChallenge.disabledPowers", TaskForceEvalDisabledPowers, 1, 1);
        eval_RegisterFunc(context, "TFChallenge.buffedEnemies", TaskForceEvalBuffEnemies, 0, 1);
        eval_RegisterFunc(context, "TFChallenge.debuffedGroup", TaskForceEvalDebuffPlayers, 0, 1);
        eval_RegisterFunc(context, "TFChallenge.noInspirations", TaskForceEvalNoInspirations, 0, 1);
    }
}

TFParamSL TaskForceConvertRangeToStature(int min_level, int max_level)
{
    TFParamSL retval;
    int count;
    bool done;

    devassert(min_level >= 1 && min_level <= max_level && max_level <= 50);

    retval = TFPARAM_SL1;
    done = false;

    if (min_level >= 1 && min_level <= max_level && max_level <= 50)
    {
        // We give precedence to the minimum level as defining the stature
        // level because that would be the earliest the player could
        // do the content, but if there isn't one, then the maximum level
        // has to do. We can't put all that stuff at SL1 and the cutoff
        // point makes a reasonable proxy.
        if (min_level > 1)
        {
            // Find the first bucket high enough to meet the minimum.
            count = 0;

            while (!done && count < ARRAY_SIZE_CHECKED(SLMappings))
            {
                if (SLMappings[count].maxLevel >= min_level)
                {
                    retval = SLMappings[count].stature;
                    done = true;
                }

                count++;
            }
        }
        else
        {
            // First the first bucket low enough to meet the maximum.
            count = ARRAY_SIZE_CHECKED(SLMappings) - 1;

            while (!done && count >= 0)
            {
                if (SLMappings[count].minLevel <= max_level)
                {
                    retval = SLMappings[count].stature;
                    done = true;
                }

                count--;
            }
        }
    }

    return retval;
}

#if defined(SERVER)
void TaskForceLoadTimeLimits(void)
{
    const char* bin_filename;
    int count;

    bin_filename = MakeBinFilename(TIME_LIMIT_DEF_FILENAME);

    printf("Loading TF time limits\n");

    ParserLoadFiles(NULL, TIME_LIMIT_DEF_FILENAME, bin_filename,
        PARSER_SERVERONLY, ParseStoryArcTimeLimit, &StoryArcTimeLimits,
        NULL, NULL, NULL);

    // Attach story arc definition pointers to the time limits and fill
    // out any missing limits if all three were not specified for a given
    // arc.
    for (count = 0; count < eaSize(&StoryArcTimeLimits.limits); count++)
    {
        FixupTimeLimits(StoryArcTimeLimits.limits[count]);
    }

    // Sort the limits by story arc to make future lookup quicker.
    eaQSort(StoryArcTimeLimits.limits, TimeLimits_CompareArcPointers);
}

U32 TaskForceGetStoryArcTimeLimit(const StoryArc *arc,
    TFParamTimeLimits limit)
{
    U32 retval;
    U32 bronze_time;
    U32 silver_time;
    U32 gold_time;

    retval = 0;

    TaskForceFindTimeLimits(arc, &bronze_time, &silver_time, &gold_time);

    switch(limit)
    {
        case TFTIMELIMITS_NONE:
            retval = 0;
            break;

        case TFTIMELIMITS_BRONZE:
            retval = bronze_time;
            break;

        case TFTIMELIMITS_SILVER:
            retval = silver_time;
            break;

        case TFTIMELIMITS_GOLD:
        default:
            retval = gold_time;
            break;
    }

    return retval;
}

bool TaskForceFindTimeLimits(const StoryArc* arc, U32* bronze_dest,
    U32* silver_dest, U32* gold_dest)
{
    bool retval;
    StoryArcTimeLimit** limit;

    retval = false;

    if (bronze_dest)
    {
        *bronze_dest = TFPARAM_TIMELIMIT_BRONZE_DEFAULT;
    }

    if (silver_dest)
    {
        *silver_dest = TFPARAM_TIMELIMIT_SILVER_DEFAULT;
    }

    if (gold_dest)
    {
        *gold_dest = TFPARAM_TIMELIMIT_GOLD_DEFAULT;
    }

    if (arc)
    {
        limit = eaBSearch(StoryArcTimeLimits.limits,
            TimeLimits_SearchArcPointers, arc);

        if (limit && *limit)
        {
            if (bronze_dest)
            {
                *bronze_dest = (*limit)->bronze;
            }

            if (silver_dest)
            {
                *silver_dest = (*limit)->silver;
            }

            if (gold_dest)
            {
                *gold_dest = (*limit)->gold;
            }

            retval = true;
        }
    }

    return retval;
}

void TaskForceSendTimeLimitsToClient(Entity* player, const StoryArc* arc)
{
    U32 bronze;
    U32 silver;
    U32 gold;

    TaskForceFindTimeLimits(arc, &bronze, &silver, &gold);

    START_PACKET(pak, player, SERVER_TASKFORCE_TIME_LIMITS);

    pktSendBitsAuto(pak, bronze);
    pktSendBitsAuto(pak, silver);
    pktSendBitsAuto(pak, gold);

    END_PACKET
}

bool TaskForceInitParameters(Entity* player, TFParamDeaths max_deaths,
    U32 time_limit, bool time_counter, bool debuff,
    TFParamPowers disable_powers, bool disable_enhancements, bool buff,
    bool disable_inspirations)
{
    bool retval;
    int count;
    U32 deaths;
    Entity* member;

    devassert(player);

    retval = false;

    if (player)
    {
        if (max_deaths != TFDEATHS_INFINITE)
        {
            U32 lives;

            TaskForceSetParameter(player->taskforce, TFPARAM_DEATHS_SETUP,
                max_deaths);

            // Number of deaths remaining. Dying at zero fails the challenge.
            switch (max_deaths)
            {
                case TFDEATHS_NONE:
                    lives = 0;
                    break;

                case TFDEATHS_ONE:
                    lives = 1;
                    break;

                case TFDEATHS_THREE:
                    lives = 3;
                    break;

                case TFDEATHS_FIVE:
                    lives = 5;
                    break;

                case TFDEATHS_ONE_PER_MEMBER:
                    lives = player->teamup->members.count;
                    break;

                case TFDEATHS_THREE_PER_MEMBER:
                    lives = (player->teamup->members.count * 3);
                    break;

                case TFDEATHS_FIVE_PER_MEMBER:
                    lives = (player->teamup->members.count * 5);
                    break;

                default:
                    dbLog("TaskForceCreate", player, "Invalid max deaths value %d", max_deaths);

                    // Force to a single life just to make it coherent.
                    lives = 1;
                    TaskForceSetParameter(player->taskforce,
                        TFPARAM_DEATHS_SETUP, TFDEATHS_NONE);
                    break;
            }

            TaskForceSetParameter(player->taskforce,
                TFPARAM_DEATHS_MAX_VALUE, lives);

            deaths = 0;

            // Count up how many team members are already dead when we start
            // the taskforce and credit those deaths to the total so that they
            // can't cheat by hauling dead bodies around for vengeance bait.
            if (player->teamup)
            {
                for (count = 0; count < player->teamup->members.count; count++)
                {
                    member = entFromDbId(player->teamup->members.ids[count]);

                    if (member->pchar->attrCur.fHitPoints == 0.0f)
                    {
                        deaths++;
                    }
                }
            }

            TaskForceSetParameter(player->taskforce, TFPARAM_DEATHS, deaths);

            if (deaths > lives)
            {
                TaskForceSetFailureBit(player->taskforce, TFPARAM_DEATHS);
            }

            retval = true;
        }

        if (time_limit)
        {
            // This is an ending timestamp - they must finish before this
            // time.
            TaskForceSetParameter(player->taskforce, TFPARAM_TIME_LIMIT,
                timerSecondsSince2000() + time_limit);

            // We keep the length of time rather than the absolute value for
            // easier comparison when doing rewards etc.
            TaskForceSetParameter(player->taskforce,
                TFPARAM_TIME_LIMIT_SETUP, time_limit);

            retval = true;
        }

        if (time_counter)
        {
            U32 curr_time = timerSecondsSince2000();

            // Time counts forward from now. When the taskforce succeeds we
            // update the time counter field and the difference between them
            // is the total time taken for the TF.
            TaskForceSetParameter(player->taskforce, TFPARAM_START_TIME,
                curr_time);
            TaskForceSetParameter(player->taskforce, TFPARAM_TIME_COUNTER,
                curr_time);

            retval = true;
        }

        if (debuff)
        {
            TaskForceSetParameter(player->taskforce, TFPARAM_DEBUFF_PLAYERS,
                1);

            retval = true;
        }

        if (disable_powers != TFPOWERS_ALL)
        {
            switch (disable_powers)
            {
                case TFPOWERS_NO_TEMP:
                case TFPOWERS_NO_TRAVEL:
                case TFPOWERS_NO_EPIC_POOL:
                case TFPOWERS_ONLY_AT:
                    TaskForceSetParameter(player->taskforce,
                        TFPARAM_DISABLE_POWERS, disable_powers);
                    break;

                default:
                    dbLog("TaskForceCreate", player,
                        "Invalid disable powers value %d", disable_powers);
                    break;
            }

            retval = true;
        }

        if (disable_enhancements)
        {
            // This is all or nothing, the value doesn't matter currently.
            TaskForceSetParameter(player->taskforce,
                TFPARAM_DISABLE_ENHANCEMENTS, 1);
        }

        if (disable_inspirations)
        {
            // This is also all or nothing.
            TaskForceSetParameter(player->taskforce,
                TFPARAM_NO_INSPIRATIONS, 1);
        }

        if (buff)
        {
            TaskForceSetParameter(player->taskforce, TFPARAM_BUFF_ENEMIES,
                1);

            retval = true;
        }

        // Send the parameters to the client now that they've been configured.
        player->tf_params_update = TRUE;

        LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Taskforce:Parameters MaxDeaths: %s, TimeLimit: %i, Debuff:%s, DisablePowers:%s, DisableEnhancements:%s, DisableInspirations:%s, BuffEnemies:%s", StaticDefineIntRevLookup( MaxDefeats, max_deaths), time_limit, debuff ? "yes":"no", StaticDefineIntRevLookup( DisabledPowers, disable_powers),disable_enhancements? "yes":"no", disable_inspirations ? "yes":"no", buff ? "yes":"no");
    }

    return retval;
}

bool TaskForceCheckFailureBit(Entity* player, TaskForceParameter param)
{
    bool retval;
    U32 bits;

    devassert(player);
    devassert(player->taskforce);
    devassert(param > TFPARAM_INVALID && param <= TFPARAM_MAX_USABLE_PARAMETER);

    retval = false;

    if (player && player->taskforce && param > TFPARAM_INVALID &&
        param <= TFPARAM_MAX_USABLE_PARAMETER)
    {
        bits = player->taskforce->parameters[TFPARAM_FAILURE_BITS - 1];

        if (bits & (1 << (param - 1)))
        {
            retval = true;
        }
    }

    return retval;
}

bool TaskForceChalkMemberDeath(Entity* player)
{
    bool retval;
    U32 deaths;

    devassert(player);
    devassert(player->taskforce);

    retval = false;

    // Don't do anything unless taskforce challenges can be in progress.
    if (player && player->taskforce &&
        !TaskForceIsParameterEnabled(player, TFPARAM_SUCCESS_BITS))
    {
        teamLock(player, CONTAINER_TASKFORCES);

        // We always count the deaths so we can show them on the summary.
        deaths = TaskForceGetParameter(player, TFPARAM_DEATHS);

        // The challenge can only fail if it was turned on originally.
        if (TaskForceIsParameterEnabled(player, TFPARAM_DEATHS))
        {
            // Did we just use up our last remaining life?
            if (deaths ==
                TaskForceGetParameter(player, TFPARAM_DEATHS_MAX_VALUE))
            {
                TaskForceSetFailureBit(player->taskforce, TFPARAM_DEATHS);

                // Make sure everybody knows something was just failed.
                player->tf_challenge_failed_msg = TRUE;
            }

            retval = true;
        }

        // We always tally deaths even if the challenge is not enabled,
        // so the normal update function cannot suffice.
        player->taskforce->parameters[TFPARAM_DEATHS - 1] = deaths + 1;

        teamUpdateUnlock(player, CONTAINER_TASKFORCES);
    }

    return retval;
}

bool TaskForceCheckTimeout(Entity* player)
{
    bool retval;
    U32 time_limit;

    devassert(player);
    devassert(player->taskforce);

    retval = false;

    // Don't do anything unless taskforce challenges can be in progress.
    if (player && player->taskforce &&
        !TaskForceIsParameterEnabled(player, TFPARAM_SUCCESS_BITS))
    {
        if (TaskForceIsParameterEnabled(player, TFPARAM_TIME_LIMIT))
        {
            time_limit = TaskForceGetParameter(player,
                TFPARAM_TIME_LIMIT);

            // Must be longer than the time limit, but not already have
            // wiped out the timer and thus triggered the failure.
            if (timerSecondsSince2000() > time_limit && time_limit > 0)
            {
                teamLock(player, CONTAINER_TASKFORCES);

                // Must re-get the value to make sure we aren't subject
                // to a race condition.
                time_limit = TaskForceGetParameter(player,
                    TFPARAM_TIME_LIMIT);

                if (time_limit > 0)
                {
                    TaskForceSetFailureBit(player->taskforce,
                        TFPARAM_TIME_LIMIT);
                    TaskForceUpdateParameter(player->taskforce,
                        TFPARAM_TIME_LIMIT, 0);
                }

                teamUpdateUnlock(player, CONTAINER_TASKFORCES);

                // Make sure everybody knows something was just failed.
                player->tf_challenge_failed_msg = TRUE;
            }
        }
    }

    return retval;
}

bool TaskForceShowChallengeFailedMsg(Entity* player)
{
    bool retval;

    devassert(player);

    retval = false;

    if (player && player->taskforce_id && player->taskforce)
    {
        serveFloater(player, player, "FloatTFChallengeFailed", 0.0f,
            kFloaterStyle_Attention, 0xdd0000ff);

        retval = true;
    }

    return retval;
}

bool TaskForceShowChallengeSummary(Entity* player)
{
    bool retval;

    devassert(player);

    retval = false;

    if (player && player->taskforce_id && player->taskforce)
    {
        TaskForceCompleteStatus(player);

        retval = true;
    }

    return retval;
}

bool TaskForceSetEffects(Entity* player, bool disable_enhancements,
    TFParamPowers disable_powers, bool disable_inspirations)
{
    bool retval;

    devassert(player);

    retval = false;

    if (player)
    {
        if (disable_enhancements)
        {
            // Shut off the enhancements and make sure the powers system
            // updates to reflect this.
            character_DisableBoosts(player->pchar);
            TaskForceApplyTempPower(player, "Disable_SetBonus");

            retval = true;
        }

        if (disable_inspirations)
        {
            character_DisableInspirations(player->pchar);
        }

        switch(disable_powers)
        {
            case TFPOWERS_NO_TEMP:
                TaskForceApplyTempPower(player, "Disable_Temp_TF");

                retval = true;
                break;

            case TFPOWERS_NO_TRAVEL:
                TaskForceApplyTempPower(player, "Disable_Travel_TF");

                retval = true;
                break;

            case TFPOWERS_NO_EPIC_POOL:
                TaskForceApplyTempPower(player, "Disable_Epic_TF");

                retval = true;
                break;

            case TFPOWERS_ONLY_AT:
                TaskForceApplyTempPower(player, "Disable_Temp_TF");
                TaskForceApplyTempPower(player, "Disable_Pool_TF");

                retval = true;
                break;

            case TFPOWERS_ALL:
                // There's nothing to shut off but it is a valid value
                // so we should consider it a "correct" use of the function.
                retval = true;
                break;

            default:
                break;
        }
    }

    return retval;
}

bool TaskForceInitDebuffs(Entity* player)
{
    bool retval;

    devassert(player);

    retval = false;

    if (player &&
        TaskForceIsParameterEnabled(player, TFPARAM_DEBUFF_PLAYERS))
    {
        TaskForceApplyTempPower(player, "Challenge_Debuff");
        retval = true;
    }

    return retval;
}

bool TaskForceInitMap(Entity* player)
{
    bool retval;
    Entity* debuffer;

    devassert(player);

    retval = false;

    if (player && TaskForceIsParameterEnabled(player, TFPARAM_BUFF_ENEMIES))
    {
        debuffer = villainCreateByName("Objects_Challenge_Buffer", 50,
            NULL, false, NULL, 0, NULL);

        if (debuffer)
        {
            SET_ENTTYPE(debuffer) = ENTTYPE_CRITTER;
            aiInit(debuffer, NULL);
            debuffer->fade = 1;

            retval = true;
        }
    }

    return retval;
}

bool TaskForceResumeEffects(Entity* player)
{
    bool retval;
    bool enhancements;
    TFParamPowers powers;
    bool inspirations;

    devassert(player);

    retval = false;

    if (player)
    {
        enhancements = false;
        powers = TFPOWERS_ALL;
        inspirations = false;

        if (TaskForceIsParameterEnabled(player,
            TFPARAM_DISABLE_ENHANCEMENTS))
        {
            enhancements = true;
        }

        if (TaskForceIsParameterEnabled(player,
            TFPARAM_NO_INSPIRATIONS))
        {
            inspirations = true;
        }

        if (TaskForceIsParameterEnabled(player, TFPARAM_DISABLE_POWERS))
        {
            powers = TaskForceGetParameter(player, TFPARAM_DISABLE_POWERS);
        }

        retval = TaskForceSetEffects(player, enhancements, powers,
            inspirations);
    }

    return retval;
}

void TaskForce_HandleExit(Entity* player)
{
    // Turn enhancements back on if we shut them off for the duration of
    // the taskforce/flashback.
    if (TaskForceIsParameterEnabled(player, TFPARAM_DISABLE_ENHANCEMENTS))
    {
        character_EnableBoosts(player->pchar);
    }

    if (TaskForceIsParameterEnabled(player, TFPARAM_NO_INSPIRATIONS))
    {
        character_EnableInspirations(player->pchar);
    }
}

bool TaskForceCheckBroadcasts(Entity* player)
{
    bool retval;

    devassert(player);
    devassert(player->taskforce);

    retval = false;

    if (player && player->taskforce && !player->taskforce->architect_flags)
    {
        if (player->tf_challenge_failed_msg)
        {
            dbBroadcastMsg(CONTAINER_TASKFORCES, &player->taskforce_id,
                INFO_SVR_COM, 0, 1, DBMSG_TFCHALLENGE_FAILED);

            player->tf_challenge_failed_msg = FALSE;
            retval = true;
        }
    }

    return retval;
}

bool TaskForceCheckCompletionParameters(Entity* player,
    char* arc_leafname, bool succeeded)
{
    bool retval;
    U32 count;

    devassert(player);
    devassert(player->taskforce);

    retval = false;

    // We shouldn't double-set the success bits and we'd better have valid
    // data to work with.
    if (player && player->taskforce &&
        !TaskForceIsParameterEnabled(player, TFPARAM_SUCCESS_BITS))
    {
        player->taskforce->parameters[TFPARAM_SUCCESS_BITS - 1] = 0;

        // We don't want to check the special parameters, they aren't
        // things that are challenges by themselves so they should never
        // be failed or succeeded per se.
        for (count = 0; count < TFPARAM_MAX_USABLE_PARAMETER; count++)
        {
            // Everything that's turned on and hasn't been failed yet
            // should be marked as successful.
            if (player->taskforce->parametersSet & (1 << count) &&
                !(player->taskforce->parameters[TFPARAM_FAILURE_BITS - 1] & (1 << count)))
            {
                if (succeeded)
                {
                    player->taskforce->parameters[TFPARAM_SUCCESS_BITS - 1] |=
                        (1 << count);
                }
                else
                {
                    player->taskforce->parameters[TFPARAM_FAILURE_BITS - 1] |=
                        (1 << count);
                }
            }
        }

        // Marking the parameter as 'in use' is how we know the challenges
        // are over and how we stop the success check happening multiple
        // times if we get called again.
        player->taskforce->parametersSet |=
            (1 << (TFPARAM_SUCCESS_BITS - 1));

        // Record how long it actually took them to run the TF.
        TaskForceUpdateParameter(player->taskforce, TFPARAM_TIME_COUNTER,
            timerSecondsSince2000() -
                TaskForceGetParameter(player, TFPARAM_START_TIME));

        // The accumulated time is given in minutes so that we don't roll
        // it over too quickly (it'll take 16 million at 2 hours each to
        // do it for a given bucket).
        TaskForceChalkCompletionTimeStat(arc_leafname,
            player->taskforce->members.count,
            TaskForceGetParameter(player, TFPARAM_TIME_COUNTER) / 60);

        // may want to use JSON log line here instead
        LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, 
            "Taskforce:Complete Name: %s, CompleteTime: %d, TaskforceID: %d, TaskforceMemberCount: %d, NumDeaths: %d, Class: %s, CharacterLevel: %d, CombatLevel: %d", 
                arc_leafname, 
                TaskForceGetParameter(player, TFPARAM_TIME_COUNTER) / 60, 
                player->taskforce_id,
                player->taskforce->members.count,
                TaskForceIsParameterEnabled(player, TFPARAM_DEATHS) ? TaskForceGetParameter(player, TFPARAM_DEATHS_MAX_VALUE) : 0,
                player->pchar->pclass->pchName,
                character_GetExperienceLevelExtern(player->pchar),
                character_GetCombatLevelExtern(player->pchar));
        if (player->taskforce_id && player->taskforce && player->pl->taskforce_mode == TASKFORCE_ARCHITECT)
        {
            LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Taskforce:Architect Player completed taskforce. Taskforce ID: %d Arc ID: %d", player->taskforce_id, player->taskforce->architect_id);
        }

        // Alert everyone of the completion of the challenges.
        dbBroadcastMsg(CONTAINER_TASKFORCES, &player->taskforce_id,
            INFO_SVR_COM, 0, 1, DBMSG_TFCHALLENGE_SHOWSUMMARY);
    }

    return retval;
}
#endif
