/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/components/earray.h>
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/error.h>
#include "dbcomm/logcomm.h"

#include "entity/entity.h"
#include <utilitieslib/utils/utils.h>  // For StuffBuff
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/character_inventory.h"
#include "Reward.h"
#include "entity/entserver.h"
#include "container/team.h"   // For teamMembers() and related data structures
#include <utilitieslib/utils/timing.h>
#include "entity/entityRef.h"
#include "entity/entGameActions.h"
#include "dbcomm/dbcomm.h"
#include "gameComm/VillainDef.h"
#include "storyarc/storyarcinterface.h"
#include "player/badges_server.h"
#include "entity/entPlayer.h"
#include "entity/character_combat.h"

#include "player/badges.h"
#include "entity/costume.h"
#include "entity/power_customization.h"
#include "gameData/costume_data.h"
#include "TeamReward.h"
#include "entity/powers.h"
#include "entity/entVarUpdate.h"
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/utils/mathutil.h>
#include "script/scriptengine.h"
#include "entity/buddy_server.h"
#include "bases/DetailRecipe.h"
#include "entity/Concept.h"
#include "entity/Proficiency.h"
#include "entity/SgrpBadges.h"
#include "entity/Supergroup.h"
#include "entity/SgrpServer.h"
#include "gameSys/raidmapserver.h"
#include "storyarc/task.h"
#include "storyarc/playerCreatedStoryarcServer.h"
#include "storyarc/mission.h"
#include "svr/svr_base.h"
#include "comm_game.h"
#include <utilitieslib/network/netio.h>
#include "account/AccountInventory.h"
#include "account/AccountData.h"
#include "auth/authUserData.h"
#include "svr/svr_player.h"

#define TAB "  "

#define XP_CAP_LEVELS 4

typedef enum Reason
{
    kBecauseFalse = 0,
    kBecauseDead,
    kBecauseTooFar,
    kBecausePending,
} Reason;

typedef struct MemberInfo
{
    Entity *e;
    F32 fDamage;
    F32 fDistanceSquared;
    Reason eGetsNothing;
} MemberInfo;

// changed from SQR(200) due to use of character_WithinDist to account for large capsule ents
#define DIST_FOR_KILL_CREDIT 200.0

#define DIST_FOR_NO_DAMAGE_REWARD SQR(300)

//-----------------------------------------------------------------------------
// Team Reward Bonus
//-----------------------------------------------------------------------------
typedef struct
{
    float *pfAbove;
    float *pfBelow;
} LevelBonus;
static LevelBonus s_levelBonus;

typedef struct
{
    int teamSize;
    float experienceBoost;
} TeamReward;
static TeamReward** s_teamRewards;

typedef struct
{
    TeamReward** teamRewards;
} TeamRewardList;
static TeamRewardList s_teamRewardList;

typedef struct
{
    float *pfPrestigePercentage;
    float *pfInfluencePercentage;
} SupergroupMods;
static SupergroupMods s_SupergroupMods;


static TokenizerParseInfo parseTeamReward[] =
{
    {   "TeamSize",     TOK_STRUCTPARAM | TOK_INT(TeamReward, teamSize, 0)  },
    {   "ExpBoost",     TOK_STRUCTPARAM | TOK_F32(TeamReward, experienceBoost, 0)},
    {   "\n",           TOK_END,      0 },
    {   "", 0, 0 }
};

static TokenizerParseInfo parseTeamRewardList[] =
{
    {   "TeamReward",   TOK_STRUCT(TeamRewardList, teamRewards, parseTeamReward) },
    {   "", 0, 0 }
};

static TokenizerParseInfo ParseLevelBonus[] =
{
    { "{",          TOK_START,  0 },
    { "BonusAbove", TOK_F32ARRAY(LevelBonus, pfAbove) },
    { "BonusBelow", TOK_F32ARRAY(LevelBonus, pfBelow) },
    { "}",          TOK_END,    0 },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseSupergroupMods[] =
{
    { "{",          TOK_START,  0 },
    { "Prestige",   TOK_F32ARRAY(SupergroupMods, pfPrestigePercentage) },
    { "Influence",  TOK_F32ARRAY(SupergroupMods, pfInfluencePercentage) },
    { "}",          TOK_END,    0 },
    { "", 0, 0 }
};

#define TEAM_REWARD_FILE "defs/Reward/team_xp_boost.def"
#define TEAM_WEIGHTS_FILE "defs/Reward/team_xp_weights.def"
#define LEVEL_BONUS_FILE "defs/Reward/level_bonus.def"
#define SG_MODS_FILE "defs/Reward/supergroup_mods.def"

/**********************************************************************func*
 * teamRewardVerify
 *
 */
static bool teamRewardVerify(TokenizerParseInfo tpi[], void* structptr)
{
    int i;
    int ret = true;

    // Make sure all TeamReward entries have unique team sizes.
    for(i = eaSize(&s_teamRewards)-1; i >= 0; i--)
    {
        TeamReward* ref = s_teamRewards[i];
        int j;
        int iSize = eaSize(&s_teamRewards);
        for(j = i+1; j < iSize; j++)
        {
            TeamReward* target = s_teamRewards[j];
            if(ref->teamSize == target->teamSize)
            {
                ErrorFilenamef(TEAM_REWARD_FILE, "Team reward entry %i and %i have the same group size", i, j);
                ret = false;
            }
        }
    }
    return ret;
}

/**********************************************************************func*
 * teamRewardReadDefFiles
 *
 */
void teamRewardReadDefFiles(void)
{
    if(s_teamRewards)
    {
        s_teamRewardList.teamRewards = s_teamRewards;
        ParserDestroyStruct(parseTeamRewardList, &s_teamRewardList);
        s_teamRewardList.teamRewards = NULL;
    }
    if(s_levelBonus.pfAbove)
    {
        eafDestroy(&s_levelBonus.pfAbove);
        eafDestroy(&s_levelBonus.pfBelow);
    }

    s_teamRewardList.teamRewards = NULL;
    ParserLoadFiles(NULL, TEAM_REWARD_FILE,  "TeamReward.bin", PARSER_SERVERONLY, parseTeamRewardList, &s_teamRewardList, NULL, NULL, teamRewardVerify);
    s_teamRewards = s_teamRewardList.teamRewards;

    ParserLoadFiles(NULL, LEVEL_BONUS_FILE, "LevelBonus.bin", PARSER_SERVERONLY, ParseLevelBonus, &s_levelBonus, NULL, NULL, NULL);
    ParserLoadFiles(NULL, SG_MODS_FILE, "SupergroupMods.bin", PARSER_SERVERONLY, ParseSupergroupMods, &s_SupergroupMods, NULL, NULL, NULL);
}

/**********************************************************************func*
 * teamFindReward
 *
 */
static TeamReward* teamFindReward(int teamSize)
{
    int i;
    for(i = eaSize(&s_teamRewards)-1; i >= 0; i--)
    {
        TeamReward* reward = s_teamRewards[i];
        if(reward->teamSize == teamSize)
        {
            return reward;
        }
    }

    return NULL;
}

/**********************************************************************func*
 * teamGetExpBonus
 *
 */
static float teamGetExpBonus(int teamSize)
{
    TeamReward* reward = teamFindReward(teamSize);
    if(reward)
        return reward->experienceBoost;
    else
        return 1.0;
}

/**********************************************************************func*
 * rewardGetSupergroupMods
 *
 */
void rewardGetSupergroupMods(Entity *e, float *pfPrestige, float *pfInf)
{
    float fPrestige = 1.0f;
    float fInf = 1.0f;

    if(e && e->pl && e->pl->supergroup_mode)
    {
        int iIdx;
        int iSize;

        int iLevel = character_CalcExperienceLevel(e->pchar);

        iIdx = iLevel;
        iSize = eafSize(&s_SupergroupMods.pfPrestigePercentage);
        if(iIdx >= iSize)
            iIdx = iSize-1;
        fPrestige = s_SupergroupMods.pfPrestigePercentage[iIdx];

        iIdx = iLevel;
        iSize = eafSize(&s_SupergroupMods.pfInfluencePercentage);
        if(iIdx >= iSize)
            iIdx = iSize-1;
        fInf = s_SupergroupMods.pfInfluencePercentage[iIdx];
    }

    if(pfPrestige)
        *pfPrestige = fPrestige;

    if(pfInf)
        *pfInf = fInf;
}

/**********************************************************************func*
 * GetLevelBonus
 *
 */
float GetLevelBonus(int iCharacterLevel, int iVillainLevel)
{
    float fBonus = 1.0f;
    int iDiff = iCharacterLevel-iVillainLevel;
    float **ppfBonus = &s_levelBonus.pfAbove;

    if(iDiff<0)
    {
        ppfBonus = &s_levelBonus.pfBelow;
        iDiff = -iDiff;
    }

    if(*ppfBonus)
    {
        if(iDiff >= eafSize(ppfBonus))
        {
            iDiff = eafSize(ppfBonus)-1;
        }

        fBonus = (*ppfBonus)[iDiff];
    }

    return fBonus;
}


//-----------------------------------------------------------------------------
// Reward splitting
//-----------------------------------------------------------------------------
static MemoryPool mempoolTeamDamageTrackers = 0;
static TeamDamageTracker** s_ppTeams = NULL;
static TeamDamageTracker** s_ppLeagues = NULL;
StuffBuff s_sbRewardLog = {0};

/**********************************************************************func*
 * teamDamageTrackerCreate
 *
 */
TeamDamageTracker* teamDamageTrackerCreate()
{
    if(!mempoolTeamDamageTrackers)
    {
        mempoolTeamDamageTrackers = createMemoryPool();
        initMemoryPool(mempoolTeamDamageTrackers, sizeof(TeamDamageTracker), 16);
    }
    return mpAlloc(mempoolTeamDamageTrackers); // mpAlloc clears memory to 0
}

/**********************************************************************func*
 * teamDamageTrackerDestroy
 *
 */
void teamDamageTrackerDestroy(TeamDamageTracker* t)
{
    mpFree(mempoolTeamDamageTrackers, t);
}

/**********************************************************************func*
 * teamDamageTrackerFindTeam
 *
 */
TeamDamageTracker* teamDamageTrackerFindTeam(TeamDamageTracker** t, int teamID)
{
    int i;
    int size = eaSize(&t);
    for(i = 0; i < size; i++)
    {
        if(t[i]->idGroup == teamID)
            return t[i];
    }
    return NULL;
}

// once you are 3 levels away from villain, you don't get items.
// NOTE: the params may or may not be 1-based
int villainWillGiveItems(int villainLevel, int playerLevel)
{
    if (playerLevel >= villainLevel + 3)
        return 0;
    return 1;
}


/**********************************************************************func*
 * giveRewardToken
 *
 */
static void s_notifyRewardToken(Entity *e, RewardToken *rewardToken, int activePlayer)
{
    // notify systems
    badge_RecordRewardToken(e);
    if (e->pchar)
        character_GrantAutoIssuePowers(e->pchar);
    TokenCountTasksModify(e, rewardToken->reward);

    // notify client
    START_PACKET(pak, e, SERVER_SEND_TOKEN);
    pktSendString(pak, rewardToken->reward);
    pktSendBitsAuto(pak, rewardToken->val);
    pktSendBitsAuto(pak, activePlayer);
    END_PACKET;
}

RewardToken * giveRewardToken( Entity * e, const char * rewardTokenName )
{
    return addToRewardToken(e, rewardTokenName, 1);
}

RewardToken * modifyRewardToken( Entity * e, const char * rewardTokenName, int val, bool updateTime, bool setValue )
{
    RewardToken * rewardToken = 0;
    if( e && e->pl && rewardTokenName )
    {
        RewardTokenType type = reward_GetTokenType( rewardTokenName );
        
        switch ( type )
        {
            xcase kRewardTokenType_Player:
            {
                int i = rewardtoken_AwardEx(&e->pl->rewardTokens, rewardTokenName, 0, updateTime);
                if( i >= 0 )
                {
                    rewardToken = e->pl->rewardTokens[i];
                    rewardToken->val = setValue ? val : rewardToken->val + val;
                    s_notifyRewardToken(e, rewardToken, 0);
                }
            }

            xcase kRewardTokenType_ActivePlayer:
            {
                int i = rewardtoken_AwardEx(&e->pl->activePlayerRewardTokens, rewardTokenName, 0, updateTime);
                if (i >= 0)
                {
                    rewardToken = e->pl->activePlayerRewardTokens[i];
                    rewardToken->val = setValue ? val : rewardToken->val + val;
                    s_notifyRewardToken(e, rewardToken, 1);

                    if (!e->teamup_id)
                    {
                        e->activePlayerRevision = -1;
                    }
                }

                if (e->teamup && e->db_id == e->teamup->activePlayerDbid
                    && teamLock(e, CONTAINER_TEAMUPS))
                {
                    i = rewardtoken_AwardEx(&e->teamup->activePlayerRewardTokens, rewardTokenName, 0, updateTime);
                    if (i >= 0)
                    {
                        rewardToken = e->teamup->activePlayerRewardTokens[i];
                        rewardToken->val = setValue ? val : rewardToken->val + val;

                        e->teamup->activePlayerRevision++;
                    }
                    teamUpdateUnlock(e, CONTAINER_TEAMUPS);
                }
            }

            xcase kRewardTokenType_Sgrp: // Supergroup reward tokens don't use val?
                if( e->supergroup )
                {
                    int i = entity_AddSupergroupRewardToken( e, rewardTokenName );
                    if( i >= 0 )
                        rewardToken = e->supergroup->rewardTokens[i];
                }

            xdefault:
                verify(0 && "invalid switch value.");
                LOG( LOG_ERROR, LOG_LEVEL_IMPORTANT, 0, "ERROR: " __FUNCTION__ ": ent %s(%d), invalid type from reward token name %s, %d", e->name, e->db_id, rewardTokenName ? rewardTokenName : "<NULL>", type );
        };
    }
    else if(e && e->pchar && (ENTTYPE(e) == ENTTYPE_CRITTER) && rewardTokenName)
    {
        RewardTokenType type = reward_GetTokenType( rewardTokenName );
        if(type != kRewardTokenType_Player)
        {
            Errorf("Attempted to give non-existent, ActivePlayer or SG reward token %s to critter %s", rewardTokenName ? rewardTokenName : "<NULL>", e->name ? e->name : "<NULL>");
        }
        else
        {
            int i = rewardtoken_AwardEx(&e->pchar->critterRewardTokens, rewardTokenName, 0, updateTime);
            if( i >= 0)
            {
                rewardToken = e->pchar->critterRewardTokens[i];
                rewardToken->val = setValue ? val : rewardToken->val + val;
            }
        }
    }
    else
    {
        char *entname = e ? e->name : "<NULL ent>";
        int db_id = e ? e->db_id : 0;

        LOG( LOG_ERROR, LOG_LEVEL_IMPORTANT, 0, "ERROR: " __FUNCTION__ ": invalid params. ent %s(%d), %s", entname, db_id, rewardTokenName ? rewardTokenName : "<NULL>" );
    }

    return rewardToken;
}

RewardToken * addToRewardToken( Entity * e, const char * rewardTokenName, int val )
{
    return modifyRewardToken( e, rewardTokenName, val, true, false );
}

RewardToken * setRewardToken( Entity * e, const char * rewardTokenName, int val )
{
    return modifyRewardToken( e, rewardTokenName, val, true, true );
}

// Only removes player reward tokens, not supergroup reward tokens
void removeRewardToken( Entity * e, const char * rewardTokenName )
{
    int count, i;
    if( e && e->pl && rewardTokenName )
    {
        count = eaSize(&e->pl->rewardTokens);
        for (i = count - 1; i >= 0; i--)
        {
            // checking for bad token names
            if (e->pl->rewardTokens[i]->reward == NULL)
            {
                rewardtoken_Destroy(e->pl->rewardTokens[i]);
                eaRemove(&e->pl->rewardTokens, i );
            } else {
                if( 0 == stricmp( e->pl->rewardTokens[i]->reward, rewardTokenName ) )
                {
                    rewardtoken_Destroy(e->pl->rewardTokens[i]);
                    eaRemove(&e->pl->rewardTokens, i );
                    TokenCountTasksModify(e, rewardTokenName);
                    break;
                }
            }
        }

        count = eaSize(&e->pl->activePlayerRewardTokens);
        for (i = count - 1; i >= 0; i--)
        {
            // checking for bad token names
            if (e->pl->activePlayerRewardTokens[i]->reward == NULL)
            {
                rewardtoken_Destroy(e->pl->activePlayerRewardTokens[i]);
                eaRemove(&e->pl->activePlayerRewardTokens, i );
            } else {
                if( 0 == stricmp( e->pl->activePlayerRewardTokens[i]->reward, rewardTokenName ) )
                {
                    rewardtoken_Destroy(e->pl->activePlayerRewardTokens[i]);
                    eaRemove(&e->pl->activePlayerRewardTokens, i );
                    TokenCountTasksModify(e, rewardTokenName);

                    if (!e->teamup_id)
                    {
                        e->activePlayerRevision = -1;
                    }
                    break;
                }
            }
        }

        if (e->teamup && e->db_id == e->teamup->activePlayerDbid
            && teamLock(e, CONTAINER_TEAMUPS))
        {
            count = eaSize(&e->teamup->activePlayerRewardTokens);
            for (i = count - 1; i >= 0; i--)
            {
                // checking for bad token names
                if (e->teamup->activePlayerRewardTokens[i]->reward == NULL)
                {
                    rewardtoken_Destroy(e->teamup->activePlayerRewardTokens[i]);
                    eaRemove(&e->teamup->activePlayerRewardTokens, i );
                } else {
                    if( 0 == stricmp( e->teamup->activePlayerRewardTokens[i]->reward, rewardTokenName ) )
                    {
                        rewardtoken_Destroy(e->teamup->activePlayerRewardTokens[i]);
                        eaRemove(&e->teamup->activePlayerRewardTokens, i );
                        TokenCountTasksModify(e, rewardTokenName);

                        e->teamup->activePlayerRevision++;
                        break;
                    }
                }
            }

            teamUpdateUnlock(e, CONTAINER_TEAMUPS);
        }
    }
    else if(e && e->pchar && (ENTTYPE(e) == ENTTYPE_CRITTER) && rewardTokenName)
    {
        count = eaSize(&e->pchar->critterRewardTokens);
        for (i = count - 1; i >= 0; i--)
        {
            // checking for bad token names
            if (e->pchar->critterRewardTokens[i]->reward == NULL)
            {
                rewardtoken_Destroy(e->pchar->critterRewardTokens[i]);
                eaRemove(&e->pchar->critterRewardTokens, i );
            } else {
                if( 0 == stricmp( e->pchar->critterRewardTokens[i]->reward, rewardTokenName ) )
                {
                    rewardtoken_Destroy(e->pchar->critterRewardTokens[i]);
                    eaRemove(&e->pchar->critterRewardTokens, i );
                    break;
                }
            }
        }
    }
}

void removeAllActivePlayerRewardTokens(Entity *e)
{
    int i, count;

    if (!e || !e->pl)
        return;

    count = eaSize(&e->pl->activePlayerRewardTokens);
    for (i = count - 1; i >= 0; i--)
    {
        rewardtoken_Destroy(e->pl->activePlayerRewardTokens[i]);
        eaRemove(&e->pl->activePlayerRewardTokens, i );

        if (!e->teamup_id)
        {
            e->activePlayerRevision = -1;
        }
    }

    if (e->teamup && e->db_id == e->teamup->activePlayerDbid
        && teamLock(e, CONTAINER_TEAMUPS))
    {
        count = eaSize(&e->teamup->activePlayerRewardTokens);
        for (i = count - 1; i >= 0; i--)
        {
            rewardtoken_Destroy(e->teamup->activePlayerRewardTokens[i]);
            eaRemove(&e->teamup->activePlayerRewardTokens, i );

            e->teamup->activePlayerRevision++;
        }

        teamUpdateUnlock(e, CONTAINER_TEAMUPS);
    }
}

//------------------------------------------------------------
//  helper for common op for checking if character is powered-up
//----------------------------------------------------------
int character_CalcEffectiveCombatLevel(Character *pchar)
{
    if(pchar->buddy.uTimeLastMentored+60>dbSecondsSince2000())
        return pchar->buddy.iCombatLevelLastMentored;
    else
        return pchar->iCombatLevel;
}
static int s_awardSpecialMeritToken(Entity *e, const char *pchTokenName, int duration)
{
    RewardToken *pToken = getRewardToken( e, pchTokenName );
    if (pToken) 
    {
        if ((int)(timerSecondsSince2000() - pToken->timer) > duration)
        {
            pToken->val = 1;
            pToken->timer = timerSecondsSince2000();
        } else {
            pToken->val = pToken->val + 1;
        }
        return pToken->val;
    } else {
        // create new token if it doesn't exist
        pToken = giveRewardToken( e, pchTokenName );
        if (pToken) // make sure that the named reward token exists
        {
            pToken->val = 1;
            pToken->timer = timerSecondsSince2000();
            return pToken->val;
        }
    }
    return 0;
}

#define NORMAL_TF_TOKEN_DURATION    (18*SECONDS_PER_HOUR)
//------------------------------------------------------------
//  Helper for the special rewards in the game.
//----------------------------------------------------------
static bool s_rewardSpecial(Entity *e, const char *pchRewardTable, RewardLevel *rl, char **lootList)
{
    if(!s_sbRewardLog.buff)
        initStuffBuff(&s_sbRewardLog, 1024);

    if(pchRewardTable[0] == '/') // execute a slash command
    {
        ClientLink clientLink = {0};
        int forcedClient = 0;
        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d  %2d %2d | Command: %s\n",
            e->name, e->db_id, rl->iEffLevel+1, rl->iExpLevel+1,
            pchRewardTable);
        if (!e->client)
        {
            // Need to have a client with a controlled entity in order to run serverParseClient
            svrClientLinkInitialize(&clientLink, NULL);
            clientLink.entity = e;
            e->client = &clientLink;
            forcedClient = true;
        }

        serverParseClientEx(pchRewardTable+1, e->client, ACCESS_INTERNAL);
        rewardAppendToLootList(lootList, "Slash %s", pchRewardTable+1);

        if (forcedClient)
        {
            clientLink.entity = NULL;
            e->client = NULL;
            svrClientLinkDeinitialize(&clientLink);
        }
        return true;
    }

    // try special respec token reward
    if( playerGiveRespecToken(e, pchRewardTable) )
    {
        sendInfoBox(e, INFO_REWARD, "RespecReward" );
        serveFloater(e, e, "FloatRespec", 0.0f, kFloaterStyle_Attention, 0);
        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d  %2d %2d | Respec: %s\n",
                             e->name, e->db_id, rl->iEffLevel+1,
                             rl->iExpLevel+1,
                             pchRewardTable);
        return true;
    }

    // special cape reward
    if( stricmp( pchRewardTable, specialRewardNames[SPECIALREWARDNAME_UNLOCKCAPES] ) == 0 )
    {
        sendInfoBox(e, INFO_REWARD, "CapeReward" );
        serveFloater(e, e, "FloatCape", 0.0f, kFloaterStyle_Attention, 0);
        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d  %2d %2d | Unlock: %s\n",
                             e->name, e->db_id, rl->iEffLevel+1,
                             rl->iExpLevel+1,
                             pchRewardTable);

        costume_Award(e,"Back_Regular_Cape", true, 0);
        return true;
    }

    if( stricmp( pchRewardTable, specialRewardNames[SPECIALREWARDNAME_UNLOCKGLOWIE] ) == 0 )
    {
        sendInfoBox(e, INFO_REWARD, "GlowReward" );
        serveFloater(e, e, "FloatGlow", 0.0f, kFloaterStyle_Attention, 0);
        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d  %2d %2d | Unlock: %s\n",
                             e->name, e->db_id, rl->iEffLevel+1,
                             rl->iExpLevel+1,
                             pchRewardTable);

        costume_Award(e, "Auras_Common", true, 0);
        return true;
    }

    if( stricmp( pchRewardTable, specialRewardNames[SPECIALREWARDNAME_FREECOSTUMECHANGE] ) == 0 )
    {
        sendInfoBox(e, INFO_REWARD, "FreeCostumeChange" );
        serveFloater(e, e, "FreeCostumeChange", 0.0f, kFloaterStyle_Attention, 0);
        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d  %2d %2d | Unlock: %s\n",
            e->name, e->db_id, rl->iEffLevel+1,
            rl->iExpLevel+1,
            pchRewardTable);

        e->pl->freeTailorSessions++;
        e->send_costume = true;
        return true;
    }

    if( stricmp( pchRewardTable, specialRewardNames[SPECIALREWARDNAME_COSTUMESLOT] ) == 0 )
    {
        if (e->pl->num_costume_slots < costume_get_max_earned_slots(e)) 
        {
            sendInfoBox(e, INFO_REWARD, "FloatNewCostumeSlot" );
            serveFloater(e, e, "FloatNewCostumeSlot", 0.0f, kFloaterStyle_Attention, 0);
            addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d  %2d %2d | Unlock: %s\n",
                e->name, e->db_id, rl->iEffLevel+1,
                rl->iExpLevel+1,
                pchRewardTable);

            e->pl->num_costume_slots++;
            costume_destroy( e->pl->costume[costume_get_num_slots(e)-1]  );
            e->pl->costume[costume_get_num_slots(e)-1] = NULL; // destroy does not set to NULL.  NULL is checked in costume_new_slot()
            e->pl->costume[costume_get_num_slots(e)-1] = costume_new_slot( e );
            powerCustList_destroy( e->pl->powerCustomizationLists[costume_get_num_slots(e)-1] );
            e->pl->powerCustomizationLists[costume_get_num_slots(e)-1] = powerCustList_new_slot( e );
            e->pl->num_costumes_stored = costume_get_num_slots(e);
            costumeFillPowersetDefaults(e);
            e->send_costume = true;
            e->send_all_costume = true;
            e->updated_powerCust = true;
            e->send_all_powerCust = true;
            return true;
        }
    }

    if( stricmp( pchRewardTable, specialRewardNames[SPECIALREWARDNAME_STORYARCMERIT] ) == 0 ||
        stricmp( pchRewardTable, specialRewardNames[SPECIALREWARDNAME_STORYARCMERITFAILED] ) == 0)
    {
        char *pStoryArc = rewardVarGet("StoryArcName");

        if (!pStoryArc) 
        {
            Errorf("Error attempting to grant StoryArcMerits in location other than TaskSuccess, TaskFail, ReturnSuccess or ReturnFail.\n");
        } 
        else if ( e && e->pl && e->pchar )
        { 
            char *arcname = rewardVarGet("StoryArcName");
            char *successString = rewardVarGet("Success");
            int success = (stricmp( pchRewardTable, specialRewardNames[SPECIALREWARDNAME_STORYARCMERIT] ) == 0);
            int meritCount = reward_FindMeritCount(arcname, success, 0);
            const char *pchTokenName = reward_FindMeritTokenName(arcname, 0);
            const char *pchBonusTokenName = reward_FindMeritTokenName(arcname, 1);
            const SalvageItem *si = salvage_GetItem( "S_MeritReward" );
            float fMeritMultiplier = 1.0f;

            // check for mission repeat
            if (pchTokenName != NULL)
            {
                int numTimesRun = s_awardSpecialMeritToken(e, pchTokenName, NORMAL_TF_TOKEN_DURATION);
                fMeritMultiplier = numTimesRun ? (1.f/numTimesRun) : 0.f;

            }

            meritCount = (int) (meritCount * fMeritMultiplier);

            if (si != NULL && (meritCount > 0) && AccountCanEarnMerits(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
            {
                serveFloater(e, e, si->pchDisplayDropMsg, 0.0f, kFloaterStyle_Attention, 0);
                sendInfoBox(e, INFO_REWARD, "RewardMeritsYouReceived", meritCount);
                character_AdjustSalvage( e->pchar, si, meritCount, "storyreward", false );
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Mer Auth: %s  Count: %d  StoryArc: %s", e->auth_name, meritCount, arcname);
                badge_StatAdd(e,"drops.merit_rewards", meritCount);
            }

            if (pchBonusTokenName && success)
            {
                if (reward_FindInWeeklyTFTokenList(arcname))
                {
                    unsigned int epochStartTime = weeklyTF_GetEpochStartTime();
                    int numTimesRun = s_awardSpecialMeritToken(e, pchBonusTokenName, timerSecondsSince2000()-epochStartTime);
                    if (numTimesRun == 1)
                    {
                        const char *bonusOncePerEpochTable = reward_FindBonusTableName(arcname, 0);    //    only award out for the first time run
                        int bonusMeritCount = reward_FindMeritCount(arcname, success, 1);

                        if (AccountCanEarnMerits(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
                        {
                            serveFloater(e, e, si->pchDisplayDropMsg, 0.0f, kFloaterStyle_Attention, 0);
                            sendInfoBox(e, INFO_REWARD, "BonusRewardMeritsYouReceived", bonusMeritCount);
                            character_AdjustSalvage( e->pchar, si, bonusMeritCount, "storyreward", false );
                            LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Mer Bonus Auth: %s  Count: %d  StoryArc: %s", e->auth_name, bonusMeritCount, arcname);
                            badge_StatAdd(e,"drops.merit_rewards", bonusMeritCount);
                        }

                        if (bonusOncePerEpochTable && e->pchar)
                        {
                            RewardLevel rl = character_CalcRewardLevel( e->pchar );
                            rewardTeamMember(e, bonusOncePerEpochTable, 1, character_CalcCombatLevel(e->pchar), 1, 1, 1,
                                1, 0, 1, 1, rl, REWARDSOURCE_TFWEEKLY, 0, 0, NULL);
                        }
                    }
                    else if (numTimesRun > 1)
                    {
                        const char *bonusReplayTable = reward_FindBonusTableName(arcname, 1);    //    as long as this is defined and it is the bonus TF
                        //    design wants this tabled to be handed out
                        if (bonusReplayTable && e->pchar)
                        {
                            RewardLevel rl = character_CalcRewardLevel( e->pchar );
                            rewardTeamMember(e, bonusReplayTable, 1, character_CalcCombatLevel(e->pchar), 1, 1, 1,
                                1, 0, 1, 1, rl, REWARDSOURCE_TFWEEKLY, 0, 0, NULL);
                        }
                    }
                }
            }
        }
        return true;
    }

    if( stricmp(pchRewardTable, specialRewardNames[SPECIALREWARDNAME_DISABLEHALFHEALTH]) == 0 )
    {
        if( e && e->pchar )
        {
            e->db_flags &= ~DBFLAG_HALF_MAX_HEALTH;
            e->pchar->bHalfMaxHealth = 0;
            LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Special Disabled half max health flag");
        }

        return true;
    }

    if( stricmp(pchRewardTable, specialRewardNames[SPECIALREWARDNAME_FINISHEDPRAETORIANTUTORIAL]) == 0 )
    {
        if( e && e->pl )
        {
            AccountStoreBuyProduct(e->auth_id, SKU("inttutor"), 1);

            if( e->pl->praetorianProgress == kPraetorianProgress_Tutorial )
            {
                entSetPraetorianProgress( e, kPraetorianProgress_Praetoria );

                // The token tells the new-player code to auto-issue a mission
                // so there's a smooth connection between the tutorial and
                // Praetoria at large.
                rewardtoken_Award(&e->pl->rewardTokens, "CharacterDoneWithPTutorial", 1);

                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Special Moved from Praetorian Tutorial to Praetoria poper");
            }
        }

        return true;
    }

    if( stricmp(pchRewardTable, specialRewardNames[SPECIALREWARDNAME_FINISHEDPRAETORIA]) == 0 )
    {
        if( e && e->pl )
        {
            if( e->pl->praetorianProgress == kPraetorianProgress_Praetoria )
            {
                int hero = 0;
                int villain = 0;

                if( getRewardToken( e, "PraetorianTravelChoseHero" ) )
                    hero = 1;
                if( getRewardToken( e, "PraetorianTravelChoseVillain" ) )
                    villain = 1;

                if( hero && !villain )
                {
                    entSetPraetorianProgress( e, kPraetorianProgress_TravelHero );
                    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Special Travelling from Praetoria to Paragon City" );
                }
                else if( villain && !hero )
                {
                    entSetPraetorianProgress( e, kPraetorianProgress_TravelVillain );
                    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Special Travelling from Praetoria to Paragon City" );
                }
                else if( hero && villain )
                {
                    Errorf( "Leaving Praetoria but both tokens are set!" );
                    entSetPraetorianProgress( e, kPraetorianProgress_TravelHero );
                    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Special Travelling from Praetoria to Paragon City - default for having both tokens" );
                }
                else
                {
                    Errorf( "Leaving Praetorian but neither token is set!" );
                    entSetPraetorianProgress( e, kPraetorianProgress_TravelHero );
                    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Special Travelling from Praetoria to Paragon City - default for having neither token" );
                }

                // Once you've been marked as travelling you don't need either
                // token. Your destination is fixed by your travel type.
                removeRewardToken( e, "PraetorianTravelChoseHero" );
                removeRewardToken( e, "PraetorianTravelChoseVillain" );
            }
        }

        return true;
    }

    if(!strnicmp(pchRewardTable,"RecipeBonusSlots",16))
    {
        int count = atoi(pchRewardTable+16);
        sendInfoBox(e, INFO_REWARD, "RecipeBonusSlots", count);
        serveFloater(e, e, "FloatRecipeBonusSlots", 0.0f, kFloaterStyle_Attention, 0);
        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d  %2d %2d | RecipeBonusSlots: %d\n",
            e->name, e->db_id, rl->iEffLevel+1,    rl->iExpLevel+1, count);
        e->pchar->recipeInvBonusSlots += count;
        character_UpdateInvSizes(e->pchar);
        return true;
    }
    if(!strnicmp(pchRewardTable,"SalvageBonusSlots",17))
    {
        int count = atoi(pchRewardTable+17);
        sendInfoBox(e, INFO_REWARD, "SalvageBonusSlots", count);
        serveFloater(e, e, "FloatSalvageBonusSlots", 0.0f, kFloaterStyle_Attention, 0);
        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d  %2d %2d | SalvageBonusSlots: %d\n",
            e->name, e->db_id, rl->iEffLevel+1,    rl->iExpLevel+1, count);
        e->pchar->salvageInvBonusSlots += count;
        character_UpdateInvSizes(e->pchar);
        return true;
    }
    if(!strnicmp(pchRewardTable,"AuctionBonusSlots",17))
    {
        int count = atoi(pchRewardTable+17);
        sendInfoBox(e, INFO_REWARD, "AuctionBonusSlots", count);
        serveFloater(e, e, "FloatAuctionBonusSlots", 0.0f, kFloaterStyle_Attention, 0);
        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d  %2d %2d | AuctionBonusSlots: %d\n",
            e->name, e->db_id, rl->iEffLevel+1,    rl->iExpLevel+1, count);
        e->pchar->auctionInvBonusSlots += count;
        character_UpdateInvSizes(e->pchar);
        return true;
    }

    if( badge_FindName(pchRewardTable) )
    {
        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d  %2d %2d | Badge: %s\n",
                             e->name, e->db_id, rl->iEffLevel+1,
                             rl->iExpLevel+1,
                             pchRewardTable);

        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Bdg %s", pchRewardTable);
        badge_Award(e, pchRewardTable, 1);
        return true;
    }

    if( sgrpbadge_GetIdxFromName(pchRewardTable, NULL) )
    {
        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d  %2d %2d | SgrpBadge: %s\n",
                             e->name, e->db_id, rl->iEffLevel+1,
                             rl->iExpLevel+1,
                             pchRewardTable);

        badge_Award(e, pchRewardTable, 1);
        return true;
    }


    if( costume_isReward(pchRewardTable) )
    {
        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %d  %2d %2d | Costume: %s\n",
                             e->name, e->db_id, rl->iEffLevel+1,
                             rl->iExpLevel+1,
                             pchRewardTable);

        costume_Award(e, pchRewardTable, true, 0);
        return true;
    }


    //Permanent Rewards are just strings (and a value if you want that stick to the character)
    //(In the RewardToken struct you get back, you can change the value field and it will automatically
    //be saved and permanently stored with the token.  )
    if( rewardIsPermanentRewardToken( pchRewardTable ) )
    {
        giveRewardToken( e, pchRewardTable );
        return true;
    }

    if( rewardIsRandomItemOfPower( pchRewardTable ) )
    {
        if( e->supergroup )
        {
            ItemOfPowerGrantNew( e, e->supergroup_id );
        }
        return true;
    }


    return false;
}


//------------------------------------------------------------
// effectively reduces the reward level of enemies defeated by
// weaker players who are mentored to a higher level. if the
// enemy is one level greater than the the boosted level, the
// level will be one above the actual level.
//
//  Two parts:
//  - If the charater was recently mentored, boost their combat level to that
//  - if their combat level doesn't equal their xp combat level, subtract
//    their effective level from what they should have and use that to adjust the level.
//
// ex: level = 19, xpLevel = 10, iCombatLevel = 20 (mentored)
//     a character fighting a more powerful enemy with the help of a mentor
//   iEffLevel = 19 + 10 - 20 = 9
//----------------------------------------------------------
static int s_calcRewardLevel(RewardLevel const *rl, int iLevel)
{
    int iEffLevel = iLevel;

    if( verify( rl ))
    {
        if(rl->iExpLevel != rl->iEffLevel )
        {
            iEffLevel = rl->iExpLevel + iLevel - rl->iEffLevel;
            iEffLevel = MIN(MAX(0, iEffLevel),MAX_PLAYER_SECURITY_LEVEL+3);
        }
    }

    return iEffLevel;
}

//------------------------------------------------------------
//  deprecated, don't use this
//----------------------------------------------------------
int adjustRewardLevelForSidekick(Entity* e, int iLevel)
{
    RewardLevel rl = character_CalcRewardLevel( e->pchar );
    return s_calcRewardLevel( &rl, iLevel );
}



typedef struct CappedInfo
{
    bool XP;
    bool Wisdom;
    bool Inf;
    bool Prestige;
} CappedInfo;

//------------------------------------------------------------
//  Helper for making sure the xp,wisdom,etc. haven't gotten
// out of control from the multipliers.
//----------------------------------------------------------
static bool s_CapRewards(RewardAccumulator *accum, Entity *e, VillainGroupEnum  eVillainGroup, const char *pchRewardTable, int iRewardLevel, F32 fRewardScale, F32 fTeamPercent, F32 fPercent, F32 fBonus, RewardLevel *rl, int containerType, char **lootList)
{
    float fInfluenceMod;
    float fPrestigeMod;
    CappedInfo res = {0};
    RewardAccumulator accumCap = {0};
    rewardaccumulator_Init(&accumCap);

    if(!s_sbRewardLog.buff)
        initStuffBuff(&s_sbRewardLog, 1024);

    // Cap influence and XP to at most XP_CAP_LEVELS levels above the player XP level.
    // As a failsafe, don't let them get more XP than they need for a whole level.
    if( verify( accum && rl ))
    {
        F32 xpscale_inv = server_state.xpscale ? 1.f/server_state.xpscale : 1.f;
        int iExpLevel = min(rl->iExpLevel+1, MAX_PLAYER_SECURITY_LEVEL-1);
        float fMaxBonus = GetLevelBonus(0, XP_CAP_LEVELS);
        int avgMaxXp = (g_ExperienceTables.aTables.piRequired[iExpLevel]-g_ExperienceTables.aTables.piRequired[iExpLevel-1])/2;
        int iMaxXP = xpscale_inv*avgMaxXp;
        const RewardDef *pDef = rewardFind(pchRewardTable, iExpLevel+XP_CAP_LEVELS+1); // reward levels are 1-based

        if(!pDef)
        {
            addStringToStuffBuff(&s_sbRewardLog, "***Error: Unable to find reward table %s[%d]!\n", pchRewardTable, iExpLevel+1);
            // This is fatal for giving out the reward.
            return false; // BREAK IN FLOW
        }
        rewardDefAward(pDef, &accumCap, e, eVillainGroup, NULL, containerType, lootList); // NULL for no datamining
        accumCap.experience *= fMaxBonus;
        accumCap.wisdom *= fMaxBonus;
        accumCap.influence *= fMaxBonus;
        accumCap.prestige *= fMaxBonus;
 
        if(accumCap.experience > iMaxXP)
        {
            float fReduction = (float)iMaxXP/(float)accumCap.experience;
            accumCap.experience = iMaxXP;
            accumCap.wisdom = accumCap.wisdom*fReduction;
            accumCap.influence = accumCap.influence*fReduction;
            accumCap.prestige = accumCap.prestige*fReduction;
        }

        // Cap the reward.
        if(accum->experience > accumCap.experience)
        {
            accum->experience = accumCap.experience;
            res.XP = true;
        }
        if(accum->wisdom > accumCap.wisdom)
        {
            accum->wisdom = accumCap.wisdom;
            res.Wisdom = true;
        }
        if(accum->influence > accumCap.influence)
        {
            accum->influence = accumCap.influence;
            res.Inf = true;
        }
        if(accum->prestige > accumCap.prestige)
        {
            accum->prestige = accumCap.prestige;
            res.Prestige = true;
        }

        rewardGetSupergroupMods(e, &fPrestigeMod, &fInfluenceMod);
        fInfluenceMod += e->pchar->attrCur.fInfluenceGain;
        fPrestigeMod += e->pchar->attrCur.fPrestigeGain;

        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d  %2d %2d | %10.10s  %2d %3.2f |  %3.2f  %3.2f  %3.2f  %3.2f       |  %3.2f  %3.2f | %6d%s%6d%s%6d%s%6d%s",
                            e->name, e->db_id, rl->iEffLevel+1,
                            rl->iExpLevel+1,
                            pchRewardTable, iRewardLevel+1, fRewardScale,
                            fTeamPercent, fPercent, fBonus, fPercent*fBonus*fRewardScale,
                            fInfluenceMod, fPrestigeMod,
                            accum->experience, res.XP ? "*" : " ",
                            (int)(accum->influence*fInfluenceMod), res.Inf ? "*" : " ",
                            accum->wisdom, res.Wisdom ? "*" : " ",
                            (int)(accum->prestige*fPrestigeMod), res.Prestige ? "*" : " ");
    }

    // --------------------
    // finally

    rewardaccumulator_Cleanup( &accumCap, true );

    return true; // success
}

void logRewards(RewardAccumulator *accum, bool bHandicapped, bool bExemplar, bool bGiveItems )
{
#define LOG_REWARD_STATUS(WHICH) bHandicapped?" (h)":(bNotGettingExemplar?" (e)":((accum->WHICH[i]->set_flags & RSF_EVERYONE)?" (f)":""))

    if(!s_sbRewardLog.buff)
        initStuffBuff(&s_sbRewardLog, 1024);

    if( verify( accum ))
    {
        bool bGotSome = false;
        if(bHandicapped)
        {
            addStringToStuffBuff(&s_sbRewardLog, "(handicapped)");
        }
        else if(bExemplar)
        {
            addStringToStuffBuff(&s_sbRewardLog, "(exemplar)");
        }


        {
            int i;

            // --------------------
            // powers

            for(i = eaSize(&accum->powers)-1; i >= 0; i--)
            {
                if(bGiveItems || accum->powers[i]->set_flags & RSF_EVERYONE)
                {
                    bool bNotGettingExemplar = false;

                    const BasePower *ppow = powerdict_GetBasePowerByName(&g_PowerDictionary, accum->powers[i]->powerNameRef->powerCategory, accum->powers[i]->powerNameRef->powerSet, accum->powers[i]->powerNameRef->power);

                    if(bExemplar)
                    {
                        if(!ppow || ppow->eType==kPowerType_Boost)
                        {
                            bNotGettingExemplar = true;
                        }
                    }

                    if(ppow && ppow->eType==kPowerType_Boost)
                    {
                        int level = accum->powers[i]->powerNameRef->level;
                        if(!accum->powers[i]->powerNameRef->fixedlevel)
                            level+=accum->powers[i]->level;

                        addStringToStuffBuff(&s_sbRewardLog, " %s.%s.%s.%d%s, ",
                            accum->powers[i]->powerNameRef->powerCategory,accum->powers[i]->powerNameRef->powerSet,accum->powers[i]->powerNameRef->power,
                            level,
                            LOG_REWARD_STATUS(powers));
                    }
                    else
                    {
                        addStringToStuffBuff(&s_sbRewardLog, " %s.%s.%s%s, ",
                            accum->powers[i]->powerNameRef->powerCategory,accum->powers[i]->powerNameRef->powerSet,accum->powers[i]->powerNameRef->power,
                            LOG_REWARD_STATUS(powers));
                    }
                    bGotSome = true;
                }
            }

            // --------------------
            // salvages

            for(i = eaSize(&accum->salvages)-1; i >= 0; i--)
            {
                if(bGiveItems || accum->salvages[i]->set_flags & RSF_EVERYONE)
                {
                    bool bNotGettingExemplar = false;
                    if(bExemplar)
                    {
                        // does exemplar exclude this?
                    }

                    addStringToStuffBuff(&s_sbRewardLog, " %s%s, ",
                                         accum->salvages[i]->item->pchName,
                                         LOG_REWARD_STATUS(salvages));
                    bGotSome = true;
                }
            }

            // --------------------
            // recipes

            for(i = eaSize(&accum->detailRecipes)-1; i >= 0; i--)
            {
                if((bGiveItems || accum->detailRecipes[i]->set_flags & RSF_EVERYONE) &&
                    accum->detailRecipes[i]->item)
                {
                    bool bNotGettingExemplar = false;
                    if(bExemplar)
                    {
                        // does exemplar exclude this?
                    }

                    addStringToStuffBuff(&s_sbRewardLog, " %s%s, ",
                                         accum->detailRecipes[i]->item->pchName,
                                         LOG_REWARD_STATUS(detailRecipes));
                    bGotSome = true;
                }
            }

            // --------------------
            // concepts

            for(i = eaSize(&accum->concepts)-1; i >= 0; i--)
            {
                if(bGiveItems || accum->concepts[i]->set_flags & RSF_EVERYONE)
                {
                    bool bNotGettingExemplar = false;
                    if(bExemplar)
                    {
                        // does exemplar exclude this?
                    }

                    addStringToStuffBuff(&s_sbRewardLog, " %s%s, ",
                                         accum->concepts[i]->item->name,
                                         LOG_REWARD_STATUS(concepts));
                    bGotSome = true;
                }
            }

            // --------------------
            // proficiencies

            for(i = eaSize(&accum->proficiencies)-1; i >= 0; i--)
            {
                if(bGiveItems || accum->proficiencies[i]->set_flags & RSF_EVERYONE)
                {
                    bool bNotGettingExemplar = false;
                    if(bExemplar)
                    {
                        // does exemplar exclude this?
                    }

                    addStringToStuffBuff(&s_sbRewardLog, " %s%s, ",
                                         accum->proficiencies[i]->item->name,
                                         LOG_REWARD_STATUS(proficiencies));
                    bGotSome = true;
                }
            }
        }

        // check if no rewards logged, and log
        if(!bGotSome)
        {
            addStringToStuffBuff(&s_sbRewardLog, " none");
        }

        addStringToStuffBuff(&s_sbRewardLog, "\n");
    }
}


//------------------------------------------------------------
//  Helper for calculating the levels for a given system
//----------------------------------------------------------
RewardLevel character_CalcRewardLevel(Character *pchar)
{
    RewardLevel res = {0};
    res.type = kPowerSystem_Powers;
    res.iExpLevel = character_CalcExperienceLevel(pchar);
    res.iEffLevel = character_CalcEffectiveCombatLevel(pchar);
    return res;
}

/**********************************************************************func*
 * rewardTeamMember
 *
 */
bool rewardTeamMember(Entity *e,      // Team member to reward
    const char *pchRewardTable,       // Reward table to fetch reward from
    float fRewardScale,               // Scale XP and Inf by this much (used to tweak villains)
    int iLevel,                       // Level in table
    bool isVictimLevel,                  // True means iLevel is Victim's level, needs to be calculated
                                      // False means iLevel is the final level and shouldn't be calculated
    float fPercent,                   // The portion of the entire reward to be given
    float fSizeBonusPercent,          // Size Bonus
    float fTeamPercent,               // The portion of the team part of the reward (used for logging only)
    VillainGroupEnum  eVillainGroup,  // The villain def of the defeated villain
    bool bGiveItems,                  // True if powers as well as xp and inf are given.
    bool bIgnoreHandicap,             // True to ignore level handicapping.
    RewardLevel rl,                   // the reward level, take by value because needs to be altered
    RewardSource source,              // Source of the reward, for logging and special handling
    int containerType,                   // type of group (team or league)
    int levelOffset,                  // Offset - only used when per critter ignore combat mods level shifting is going on
    char **lootList)                      // Estring list of rewards given - used by open salvage
{
    RewardAccumulator accum = {0};
    float fBonus;
    const RewardDef *pDef;
    int iCritterLevel = iLevel;
    int iRewardLevel = isVictimLevel ? s_calcRewardLevel( &rl, iLevel ) : iLevel;
    bool bHandicapped = !bIgnoreHandicap && !villainWillGiveItems(iLevel+rl.iVillainConAdjust, rl.iEffLevel);
    bool bExemplar = !bIgnoreHandicap && character_IsRewardedAsExemplar(e->pchar);
    bool bRewardHadLoot=0;
    TeamRewardDebug * pDebug;
    float fTempScale;
    int i;
    
    assert(source < REWARDSOURCE_COUNT);

    if( source == REWARDSOURCE_DEBUG )
    {
        pDebug = getRewardDebug(e->db_id);
        pDebug->kill_count++;
        if( bGiveItems )
            pDebug->item_count++;
    }

    if(!s_sbRewardLog.buff)
        initStuffBuff(&s_sbRewardLog, 1024);

    if(bIgnoreHandicap)
    {
        // When ignoring handicap, you get the reward based on your
        //   experience level, not the villain's level.
        // DGNOTE 8/2/2010 - we also add the ignorecombatmods shift here, since that will have bumped the effective combat
        // level of the critter from the player's perspective
        iLevel = iRewardLevel = rl.iEffLevel = rl.iExpLevel + levelOffset;
    }

    // --------------------
    // handle invalid ent or pchar

    if(!e || !e->pchar)
    {
        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9s  %2d %2d | %10.10s  %2d %3.2f |  %3.2f  %3.2f  %3.2s  %3.2f | (invalid entity)",
                "*unknown*", "?", rl.iEffLevel+1,
                rl.iExpLevel+1,
                pchRewardTable, iRewardLevel+1, 0.f,
                fTeamPercent, fPercent, "?", fPercent*fRewardScale);
        return false;
    }

    // --------------------
    // handle special rewards that fall outside the normal rewards system

    if( s_rewardSpecial(e, pchRewardTable, &rl, lootList) )
    {
        return true; // BREAK IN FLOW
    }

    rewardaccumulator_Init(&accum);

    if(!strnicmp(pchRewardTable,"IS.",3)) // check for raw itemset and award directly
    {
        rewardItemSetAccumByName(pchRewardTable + 3, &accum, e, iRewardLevel + 1, 0, kItemSetTarget_Player, lootList);
    }
    else // regular reward def flow
    {
        // find the reward, levels are 1-based
        pDef = rewardFind(pchRewardTable, iRewardLevel+1);

        if(!pDef) // check for NULL def, but okay if critter def is NULL
        {
            addStringToStuffBuff(&s_sbRewardLog, "***Error: Unable to find reward table %s[%d]!\n", pchRewardTable, iRewardLevel+1);
            // This is fatal for giving out the reward.

            return false;
        }

        // ----------------------------------------
        // accum the rewards
        

        // accumulate the reward
        rewardDefAward(pDef, &accum, e, eVillainGroup, pchRewardTable, containerType, lootList);

        // Only award salvage if the player would normally get rewards
        if (!bHandicapped && fTeamPercent > 0.0f)
        {
            // --------------------
            // award invention salvage by critter level, not calc'd level

            const RewardDef *pCritterDef;
            char *estrCritterTable;
            estrStackCreate(&estrCritterTable, 256);

            estrPrintCharString(&estrCritterTable, pchRewardTable);
            estrConcatStaticCharArray(&estrCritterTable, "_Salvage");
            pCritterDef = rewardFind(estrCritterTable, iCritterLevel+1);

            if(pCritterDef)
            {
                rewardDefAward(pCritterDef, &accum, e, eVillainGroup, estrCritterTable, containerType, lootList);
            }

            estrDestroy(&estrCritterTable);
        }

        // calc the bonus
        // note: assuming same bonus for all system types
        fBonus = bIgnoreHandicap ? 1.0f : GetLevelBonus( rl.iEffLevel, iLevel );



        // Figure out how much of it is mine and give me my bonus.
        fTempScale = fPercent * fSizeBonusPercent * fTeamPercent * fBonus * fRewardScale;
        accum.experience = ceil(accum.experience * fTempScale);
        accum.wisdom = ceil(accum.wisdom * fTempScale);
        accum.influence = ceil(accum.influence * fTempScale);
        for (i = 0; i < eaSize(&accum.incarnatePoints); i++)
        {
            accum.incarnatePoints[i]->count = ceil(accum.incarnatePoints[i]->count * fTempScale);
        }

        // No team size bonus, and always give level bonus for prestige
        if(e->pl->supergroup_mode)
        {
            accum.prestige = ceil(accum.prestige*fPercent*fTeamPercent*fBonus*fRewardScale);
        }
        else
        {
            accum.prestige = 0;
        }

        if (!accum.bIgnoreRewardCaps)
        {
            // cap the result, just in case.
            if( !s_CapRewards(&accum, e, eVillainGroup, pchRewardTable, iRewardLevel, fRewardScale, fTeamPercent, fPercent*fSizeBonusPercent*fTeamPercent, fBonus, &rl, containerType, lootList) )
            {
                // if this fails, return
                return false;
            }
        }
    }

    // --------------------
    // Logging

    logRewards(&accum, bHandicapped, bExemplar, bGiveItems);

    // --------------------
    // apply rewards
    // Need to do this after the logging since they might go up a level.

    if( g_ArchitectTaskForce && bGiveItems && !bHandicapped && source != REWARDSOURCE_DEVCHOICE && !rewardSourceAllowedArchitect(source)
        && fTeamPercent>0 && containerType == CONTAINER_TEAMUPS)
    {
        bool throttled = false;
        if (server_state.MARTY_enabled)
        {
            if (e->pl->MARTY_rewardData[MARTY_ExperienceArchitect].expThrottled || e->pl->MARTY_rewardData[MARTY_ExperienceCombined].expThrottled)
                throttled = true;
        }
        
        if(!throttled)
            playerCreatedStoryArc_CalcReward( e, iLevel, pchRewardTable, fRewardScale*fSizeBonusPercent*fPercent );
    }

    bRewardHadLoot |= rewardApply(&accum, e, bGiveItems, bHandicapped, source, lootList);

    // ARM NOTE: This appears to be unused, so I'm pulling it.  Revert if I'm wrong.
    // recursive call for rewardTables within reward sets
//    if(accum.rewardTable && eaSize(&accum.rewardTable))
//    {
//        bRewardHadLoot |= rewardFindDefAndApplyToEntShortLog(e, accum.rewardTable, eVillainGroup, iLevel+1, false, rl, bIgnoreHandicap, source, lootList);
//    }

    // --------------------
    // finally, cleanup

    rewardaccumulator_Cleanup( &accum, true );

    return bRewardHadLoot;
}

static F32 calcRewardScale(const VillainDef *pVillainDef, Entity *eVictim) 
{
    F32 result =  pVillainDef?pVillainDef->rewardScale:1.0f;
    if (eVictim->fRewardScaleOverride >= 0.0f && eVictim->fRewardScaleOverride <= 1.0f)
        result = eVictim->fRewardScaleOverride;
    if( g_ArchitectTaskForce && result > 1.f )
        result = 1.f;
    if( g_ArchitectTaskForce )
        result *= eVictim->fRewardScale;
    return result;
}

/**********************************************************************func*
 * rewardGroup
 *
 */
void rewardGroup(Entity *e,    // Any member of the team (needed to get teamup)
    Entity *eVictim,          // The critter which was defeated
    int group_id,              // ID of team
    const char *pchRewardTable,     // Reward table to fetch reward from
    float fPercent,           // Portion of reward to be given to team
    bool bGetItems,           // true if this team gets items from the reward
    PowerSystem eSystem,      // what system this applies to
    int idxRotor,             // which rotor to use for round-robin
    U32 flags,
    int containerType)                // logging flags
{
    int i;
    int iLuckyMember;
    float fRewardBonus;
    int iCntMembers;
    int iLevelHigest = -1;
    int iCntMembersOnline = 0;
    int iCntUselessMembersOnline = 0;
    int iGroupSize = 0;
    int *piMembers;
    int dbidLeader = 0;
    int *piRotor = NULL;
    MemberInfo aMemberInfo[MAX_LEAGUE_MEMBERS];
    int iLevel = eVictim->pchar->iCombatLevel;
    const VillainDef *pVillainDef = eVictim->villainDef;
    F32 fRewardScale;
    char *containerName = NULL;
    F32 fTotalDamage=0;
    F32 fTotalDamageByPendingMembers=0;
    bool doNotIncrementRotor = false;

    if(!e)
        return;

    if(!s_sbRewardLog.buff)
        initStuffBuff(&s_sbRewardLog, 1024);

    fRewardScale = calcRewardScale(pVillainDef, eVictim);

    if (fRewardScale <= 0)
        return;

    switch (containerType)
    {
    case CONTAINER_TEAMUPS:
        containerName = "Team";
        break;
    case CONTAINER_LEAGUES:
        containerName = "League";
        break;
    default:
        devassert(0);    //    undefined type
        return;
    }
    if(group_id>0)
    {
        TeamMembers *ptm = teamMembers(e, containerType);
        if (containerType == CONTAINER_LEAGUES)
        {
            if(!ptm)
            {
                ptm = teamMembers(e, CONTAINER_TEAMUPS);
            }
        }

        if(!ptm)
            return;

        piMembers = ptm->ids;
        iCntMembers = ptm->count;
        dbidLeader = ptm->leader;
    }
    else
    {
        piMembers = &e->db_id;
        iCntMembers = 1;
    }

    // Get the highest level of everyone on the team (who is on this map).
    // Only members of the team who are on the map split the reward.
    // creates these things:
    // - aMemberInfo
    for(i=0; i<iCntMembers; i++)
    {
        aMemberInfo[iCntMembersOnline].e = entFromDbId(piMembers[i]);

        if(aMemberInfo[iCntMembersOnline].e)
        {
            int j;
            int iEffLevel = (pVillainDef && pVillainDef->ignoreCombatMods) ? 1
                : character_CalcRewardLevel(aMemberInfo[iCntMembersOnline].e->pchar).iEffLevel;

            iLevelHigest = MAX(iEffLevel, iLevelHigest);

            aMemberInfo[iCntMembersOnline].fDamage = 0;
            aMemberInfo[iCntMembersOnline].eGetsNothing = 0;
            aMemberInfo[iCntMembersOnline].fDistanceSquared =
            distance3Squared(ENTPOS(aMemberInfo[iCntMembersOnline].e), ENTPOS(eVictim));

            // I wish I didn't have to loop over this again...
            for(j=eaSize(&eVictim->who_damaged_me)-1; j>=0; j--)
            {
                DamageTracker *dt = eVictim->who_damaged_me[j];
                if(piMembers[i] == erGetDbId(dt->ref))
                {
                    aMemberInfo[iCntMembersOnline].fDamage = dt->damage;
                    fTotalDamage += dt->damage;
                    break;
                }
            }

            if( character_IsPending( aMemberInfo[iCntMembersOnline].e->pchar) )
            {
                aMemberInfo[iCntMembersOnline].eGetsNothing = kBecausePending;
                iCntUselessMembersOnline++;
                if( aMemberInfo[iCntMembersOnline].fDamage > 0 )
                {
                    if( iEffLevel > iLevel )
                    {
                        addStringToStuffBuff(&s_sbRewardLog, "%s (Id %i) gets no reward because pending member (%s %id) dealt damage at a level (%i) above the critter (%i) ", 
                            containerName, group_id, aMemberInfo[iCntMembersOnline].e->name, aMemberInfo[iCntMembersOnline].e->db_id, iEffLevel, iLevel );
                        return; 
                    }
                    fTotalDamageByPendingMembers += aMemberInfo[iCntMembersOnline].fDamage;
                }
            }
            else if(db_state.is_static_map && !aMemberInfo[iCntMembersOnline].fDamage>0 )
            {
                if(!entAlive(aMemberInfo[iCntMembersOnline].e)
                    && 60<=(dbSecondsSince2000()-aMemberInfo[iCntMembersOnline].e->pchar->aulEventTimes[kPowerEvent_Defeated]))
                {
                    aMemberInfo[iCntMembersOnline].eGetsNothing = kBecauseDead;
                    iCntUselessMembersOnline++;
                }
                else if(aMemberInfo[iCntMembersOnline].fDistanceSquared>DIST_FOR_NO_DAMAGE_REWARD)
                {
                    aMemberInfo[iCntMembersOnline].eGetsNothing = kBecauseTooFar;
                    iCntUselessMembersOnline++;
                }
            }

            // The leader holds the rotor. If the leader isn't online, then
            // someone else will start up a rotor until they aren't online
            // or the leader is, etc.
            if(piMembers[i]==dbidLeader || piRotor==NULL)
            {
                TeamMembers *ptm = teamMembers(aMemberInfo[iCntMembersOnline].e, containerType);

                if (containerType == CONTAINER_LEAGUES && !ptm)
                {
                    ptm = teamMembers(aMemberInfo[iCntMembersOnline].e, CONTAINER_TEAMUPS);
                    
                    // If we're reusing the rotor from the Teamup, then the rotor will already have been
                    // incremented from the previous call and double incrementing will wind up skipping players
                    doNotIncrementRotor = true;
                }                
    
                if(ptm)
                {
                    piRotor = &ptm->rotors[idxRotor];

                    // If the rotor is empty, seed it with something arbitrary
                    // and varies between the rotors. These keeps the various 
                    // rotors from being in lock-step.
                    if(*piRotor==0)
                    {
                        *piRotor = pchRewardTable[0]+idxRotor;
                    }
                }
            }
            iCntMembersOnline++;
        }
    }

    // If no one on the team is around, give up.
    if(iCntMembersOnline==0 || iCntUselessMembersOnline == iCntMembersOnline || fTotalDamageByPendingMembers >= fTotalDamage )
        return;

    // If any pending members damage mob, only a fractional amount of reward is given
    if( ((fTotalDamage-fTotalDamageByPendingMembers)/fTotalDamage) < .75f )
        bGetItems = 0;

    fPercent *=  (fTotalDamage-fTotalDamageByPendingMembers)/fTotalDamage;
    fRewardBonus = teamGetExpBonus(iCntMembersOnline-iCntUselessMembersOnline);
    iGroupSize = iCntMembersOnline;

    if(flags & LOG_GROUP_INFO)
    {
        int marginSpace = (int)(20 - (strlen(containerName) + 1));    //    +1 for blank space
        addStringToStuffBuff(&s_sbRewardLog, "%s %*.*s %9d        | cnt:%2d   on:%2d      |        %3.2f  %3.2f        %3.2f |             |  -------------------------------\n",
            containerName, marginSpace, marginSpace, "-------------------------", group_id, iCntMembers, iCntMembersOnline, fPercent, fRewardBonus, fPercent*fRewardBonus );
    }

    // This is the member who is going to get the item rewards
    iLuckyMember = 0; 
    if( bGetItems && piRotor)
    {
        iLuckyMember = (*piRotor)%iCntMembersOnline;
        if(!doNotIncrementRotor)
            (*piRotor)++;
    }

    for(i=0; i<iCntMembersOnline; i++)
    {
        if(aMemberInfo[i].eGetsNothing == kBecauseDead)
        {
            addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d        |                     |                              |             | %6d %6d %6d %6d (dead too long)\n",
                aMemberInfo[i].e->name, aMemberInfo[i].e->db_id, 0, 0, 0, 0);
        }
        else if(aMemberInfo[i].eGetsNothing == kBecauseTooFar)
        {
            addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d        |                     |                              |             | %6d %6d %6d %6d (no dmg, too far: %.2f)\n",
                aMemberInfo[i].e->name, aMemberInfo[i].e->db_id, 0, 0, 0, 0, sqrt(aMemberInfo[i].fDistanceSquared));
        }
        else if(aMemberInfo[i].eGetsNothing == kBecausePending)
        {
            addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d        |                     |                              |             | %6d %6d %6d %6d (pending level change)\n",
                aMemberInfo[i].e->name, aMemberInfo[i].e->db_id, 0, 0, 0, 0 );
        }
        else
        {
            bool bGiveItems = (iLuckyMember==0 && bGetItems);
            bool bIgnoreHandicap = false;
            int levelOffset = 0;
            RewardLevel rl = character_CalcRewardLevel( aMemberInfo[i].e->pchar );
            rl.iVillainConAdjust = villainRankConningAdjustment(eVictim->villainRank);

            if (pVillainDef && pVillainDef->ignoreCombatMods)
            {
                bIgnoreHandicap = true;
            }
            else if (eVictim && eVictim->pchar && eVictim->pchar->bIgnoreCombatMods)
            {
                bIgnoreHandicap = true;
                levelOffset = eVictim->pchar->ignoreCombatModLevelShift;
            }

            rewardTeamMember(aMemberInfo[i].e,
                pchRewardTable,
                fRewardScale,
                iLevel,
                true,
                fPercent,
                fRewardBonus,
                (1.f/(float)iCntMembersOnline),
                pVillainDef?pVillainDef->group:VG_NONE,
                bGiveItems,
                bIgnoreHandicap,
                rl,
                (flags&LOG_GROUP_DEBUG_INFO)?REWARDSOURCE_DEBUG:REWARDSOURCE_DROP,
                containerType,
                levelOffset,
                NULL);

        }
        iLuckyMember--;
    }
}


TeamRewardDebug **g_ppTeamRewardDebug;

TeamRewardDebug * getRewardDebug( int id )
{
    int i;
    TeamRewardDebug * newStruct;

    for( i = eaSize(&g_ppTeamRewardDebug)-1; i>=0; i-- )
    {
        if( id == g_ppTeamRewardDebug[i]->member_id )
            return g_ppTeamRewardDebug[i];
    }

    newStruct = calloc(1, sizeof(TeamRewardDebug));
    newStruct->member_id = id;
    eaPush(&g_ppTeamRewardDebug, newStruct);
    return newStruct;
}

static void addToStaticGroupList(int containerType,DamageTracker *dt, int **soloPlayers)
{
    TeamDamageTracker***pGroupList;
    TeamDamageTracker* ptt = NULL;
    Entity *e=erGetEnt(dt->ref);
    int soloPlayer = 0;
    int idToMatch;
    switch (containerType)
    {
    case CONTAINER_TEAMUPS:
        pGroupList = &s_ppTeams;
        idToMatch = e ? e->teamup_id : 0;
        break;
    case CONTAINER_LEAGUES:
        pGroupList = &s_ppLeagues;
        if (e && e->league_id)
            idToMatch = e->league_id;
        else
            return;
        break;
    default:
        devassert(0);    //    undefined
        return;
    }

    // If the player is still on the map, update the teamup id. If
    // they aren't, then we'll used what was cached earlier and hope
    // that it works out.
    if(e)
    {
        dt->group_id = idToMatch;
    }

    if(!dt->group_id)
    {
        // If this entity does not belong to any teams, it is on its own
        // team.
        // TODO: I made up a team ID here which is the -db_id. If
        //   team_ids and db_ids are mutually exclusive anyway, then I
        //   should just use the db_id straight. The -db_id isn't
        //   guaranteed to be different than the team_id (i.e. if the
        //   team_id is very large.
        dt->group_id = -erGetDbId(dt->ref);
        if (soloPlayers)    eaiPush(soloPlayers, dt->group_id);
        soloPlayer = 1;
        if (dt->group_id > 0)
        {
            //    if teamup_id is positive, it will think that this is a team, not a player
            addStringToStuffBuff(&s_sbRewardLog, "ERROR::-db_id %d is positive which may not properly dispense a reward", erGetDbId(dt->ref));
        }
    }
    else
    {
        if (dt->group_id < 0)
        {
            //    if teamup_id is negative, it will think that this is a player, not a teamup_id
            addStringToStuffBuff(&s_sbRewardLog, "ERROR::teamup_id %d is negative which may not properly dispense a reward", dt->group_id);
        }
    }

    ptt = teamDamageTrackerFindTeam(*pGroupList, dt->group_id);

    //    Trying to track down bug
    if (ptt)
    {
        if (soloPlayer)
        {
            //    solo player and a team tracker already exists for that db_id
            addStringToStuffBuff(&s_sbRewardLog, "ERROR::teamDamageTracker collision between teamup_id %d and solo player -db_id", dt->group_id);
        }
        else if (soloPlayers && (eaiFind(soloPlayers, dt->group_id) != -1))
        {
            //    team tracker inserted, but it is already being used by a solo player
            addStringToStuffBuff(&s_sbRewardLog, "ERROR::teamDamageTracker collision between teamup_id %d and solo player -db_id", dt->group_id);
        }
    }

    if(ptt==NULL)
    {
        ptt = teamDamageTrackerCreate();
        ptt->idGroup = dt->group_id;
        eaPush(pGroupList, ptt);
    }

    // We need at least one person from the team still on the server
    // so we can look at the team list and so on.
    if(ptt->erMember==0 && e!=NULL)
    {
        ptt->erMember=dt->ref;
    }

    ptt->fDamage += dt->damage;
    ptt->iCntMembers++;
}


void dispenseRewards(Entity* eVictim, DeathBy eDeathBy, int debug)
{
    int i;
    float fTotalDamage = 0;
    const char *pchTable;
    const char * const * const * pppchRewards;
    int *soloPlayers = NULL;

    if(s_ppTeams)
        eaDestroyEx(&s_ppTeams, teamDamageTrackerDestroy);

    if (s_ppLeagues)
        eaDestroyEx(&s_ppLeagues, teamDamageTrackerDestroy);

    if(!s_sbRewardLog.buff)
        initStuffBuff(&s_sbRewardLog, 1024);

    {
        const char *pchVillainName;
        if(eVictim->villainDef)
            pchVillainName = eVictim->villainDef->name;
        else
            pchVillainName = entGetName(eVictim);
        addStringToStuffBuff(&s_sbRewardLog, "Villain Defeated: %s, level %d\n{\n", pchVillainName, eVictim->pchar->iCombatLevel+1);
    }

    // Produce a tally of the number of teams that worked to defeat the victim and
    // the amount of damage each team dealt.
    for(i=eaSize(&eVictim->who_damaged_me)-1; i>=0; i--)
    {
        DamageTracker *dt = eVictim->who_damaged_me[i];
        addToStaticGroupList(CONTAINER_TEAMUPS, dt, &soloPlayers);        //    if we find out anything about the hamidon bug, we can remove soloplayers
        addToStaticGroupList(CONTAINER_LEAGUES, dt, NULL);                //    we don't care about soloPlayer array for leagues
        fTotalDamage += dt->damage;
    }

    eaiDestroy(&soloPlayers);
    addStringToStuffBuff(&s_sbRewardLog, TAB "%.2f damage from %d players on %d teams\n", fTotalDamage, eaSize(&eVictim->who_damaged_me), eaSize(&s_ppTeams));
    addStringToStuffBuff(&s_sbRewardLog, TAB TAB "Breakdown: ");

    for(i=eaSize(&eVictim->who_damaged_me)-1; i>=0; i--)
    {
        const char *playerName;
        DamageTracker *dt = eVictim->who_damaged_me[i];
        Entity *e = erGetEnt(dt->ref);

        if(!e || !e->pchar)
        {
            playerName = "Unknown name";
        }
        else
        {
            playerName = entGetName(e);
        }

        addStringToStuffBuff(&s_sbRewardLog, "'%s'(%i), team %d, %.2f; ", playerName, erGetDbId(dt->ref), dt->group_id, dt->damage);
    }
    addStringToStuffBuff(&s_sbRewardLog, "\n");

    pchTable = rewardGetTableNameForRank(eVictim->villainRank, eVictim);
    pppchRewards = &eVictim->villainDef->additionalRewards;

    // Figure out what rewards will be dispensed.
    // Rewards include experience, influence, and inspirations.
    // Experience and influence will be shared by all who participated in combat.
    // Inspiration will be tailored to and received by a single random person.
    {
        // Find the team that dealt the most damage.
        TeamDamageTracker *ptt = (TeamDamageTracker *)damageTrackerFindMaxDmg((DamageTracker**)s_ppTeams);
        TeamDamageTracker *plt = (TeamDamageTracker *)damageTrackerFindMaxDmg((DamageTracker**)s_ppLeagues);
        int idLuckyTeam = ptt ? ptt->idGroup : 0;
        int idLuckyLeague = plt ? plt->idGroup : 0;
        int iCntRewards = eaSize(pppchRewards);

        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9.9s %3.3s %2.2s | %10.10s %3.3s %4.4s | %5.5s %5.5s %5.5s %5.5s %5.5s | %5.5s %5.5s | %6.6s %6.6s %6.6s %6.6s | %4.4s\n", "name", "dbid", "com", "xp", "table", "lvl", "scl", "%team", "%totl", "%bon", "%xp", "%finl", "inf%", "prs%", "xp", "inf", "wis", "pre", "loot");

        // reward lucky team in script
        if (idLuckyTeam != 0)
            ScriptTeamRewarded(eVictim, erGetEnt(ptt->erMember));

        for(i=eaSize(&s_ppTeams)-1; i>=0; i--)
        {
            Entity *e = erGetEnt(s_ppTeams[i]->erMember);

            if(e)
            {
                int j;
                // Each team receives rewards proportional to the amount of damage dealt.
                float fPercent = fTotalDamage ? s_ppTeams[i]->fDamage/fTotalDamage : 0.0;
                PowerSystem eSystem = kPowerSystem_Powers;

                if(pchTable)
                {
                    int flags = LOG_GROUP_INFO;
                    if( debug )
                        flags |= LOG_GROUP_DEBUG_INFO;
                    // First, do the rank reward
                    rewardGroup( e, eVictim, s_ppTeams[i]->idGroup, pchTable, fPercent, s_ppTeams[i]->idGroup==idLuckyTeam, eSystem, ROTOR_RANK, flags, CONTAINER_TEAMUPS);
                    if (!e->league_id)
                        rewardGroup( e, eVictim, s_ppTeams[i]->idGroup, pchTable, fPercent, s_ppTeams[i]->idGroup==idLuckyTeam, eSystem, ROTOR_RANK, flags, CONTAINER_LEAGUES);
                }

                // Now loop over the additional rewards specific for this villain.
                for(j=0; j<iCntRewards; j++)
                {
                    int flags = LOG_NO_GROUP_INFO;
                    addStringToStuffBuff(&s_sbRewardLog, "%20.20s                  | %s\n", "Additional --------------------", (*pppchRewards)[j]);
                    
                    if( debug )
                        flags |= LOG_GROUP_DEBUG_INFO;
                    rewardGroup( e, eVictim, s_ppTeams[i]->idGroup, (*pppchRewards)[j], fPercent, s_ppTeams[i]->idGroup==idLuckyTeam, eSystem, ROTOR_ADDITIONAL, flags, CONTAINER_TEAMUPS );
                    if (!e->league_id)
                        rewardGroup( e, eVictim, s_ppTeams[i]->idGroup, (*pppchRewards)[j], fPercent, s_ppTeams[i]->idGroup==idLuckyTeam, eSystem, ROTOR_ADDITIONAL, flags, CONTAINER_LEAGUES );
                }

                // If this team has done more than 10% of the damage, they
                // may get credit for a kill task
                if(fPercent >= 0.10f)
                {
                    if(e->teamup_id)
                    {
                        Entity *eNearbyTeammate = NULL;
                        // If there's an actual team see if anyone is close
                        // enough to the victim get the credit.
                        for(j=0; j<e->teamup->members.count; j++)
                        {
                            Entity *teammate = entFromDbId(e->teamup->members.ids[j]);
                            if(teammate
                                && character_WithinDist(teammate, eVictim, DIST_FOR_KILL_CREDIT))
                            {
                                eNearbyTeammate = teammate;
                                break;
                            }
                        }

                        // This updates for everyone on the team.
                        if(eNearbyTeammate)
                        {
                            for(j=0; j<e->teamup->members.count; j++)
                            {
                                Entity *teammate = entFromDbId(e->teamup->members.ids[j]);
                                if(teammate)
                                {
                                    badge_RecordKill(teammate, eVictim, fPercent);
                                    EncounterAddWhoKilledMyActor(eVictim, e->teamup->members.ids[j]);
                                }
                            }

                            //    If the player is on a TF that completes, it may destroy the TF/teamup
                            //    this makes the e->teamup pointer bad and not give the badge credit for the teammembers
                            KillTaskUpdateCount(eNearbyTeammate, eVictim);
                        }
                    }
                    else
                    {
                        // An army of one.
                        if(character_WithinDist(e, eVictim, DIST_FOR_KILL_CREDIT))
                        {
                            badge_RecordKill(e, eVictim, fPercent);
                            EncounterAddWhoKilledMyActor(eVictim, e->db_id);
                            KillTaskUpdateCount(e, eVictim);
                        }
                    }
                }
            }
        } // end for each team

        for (i=eaSize(&s_ppLeagues)-1; i>=0; --i)
        {
            Entity *e = erGetEnt(s_ppLeagues[i]->erMember);
            if(e)
            {
                int j;
                float fPercent = fTotalDamage ? s_ppLeagues[i]->fDamage/fTotalDamage : 0.0;
                PowerSystem eSystem = kPowerSystem_Powers;
                if(pchTable)
                {
                    int flags = LOG_GROUP_INFO;
                    if( debug )
                        flags |= LOG_GROUP_DEBUG_INFO;
                    rewardGroup( e, eVictim, s_ppLeagues[i]->idGroup, pchTable, fPercent, s_ppLeagues[i]->idGroup==idLuckyLeague, eSystem, ROTOR_RANK, flags, CONTAINER_LEAGUES);
                }
                if (iCntRewards)
                {
                    int flags = LOG_NO_GROUP_INFO;
                    if( debug )
                        flags |= LOG_GROUP_DEBUG_INFO;
                    for(j=0; j<iCntRewards; j++)
                    {
                        rewardGroup( e, eVictim, s_ppLeagues[i]->idGroup, (*pppchRewards)[j], fPercent, s_ppLeagues[i]->idGroup==idLuckyLeague, eSystem, ROTOR_ADDITIONAL, flags, CONTAINER_LEAGUES );
                    }
                }
                //    for now, that's it
                //    here would go stuff like adding badge credit or kill task credit
            }
        }
    }

    addStringToStuffBuff(&s_sbRewardLog, "\n}\n");
    LOG_ENT( eVictim, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "%s", s_sbRewardLog.buff);

    // clear out reward log.
    clearStuffBuff(&s_sbRewardLog);
}


void rewardDebugTarget( Entity *e, Entity *target, int iterations )
{
    int i;
    eaDestroyEx(&g_ppTeamRewardDebug, NULL);

    for( i = 0; i < iterations; i++ )
        dispenseRewards(target, kDeathBy_HitPoints, 1 ); 

      for( i = eaSize(&g_ppTeamRewardDebug)-1; i>=0; i-- )
    {
        Entity * rewardee = entFromDbId(g_ppTeamRewardDebug[i]->member_id);

        conPrintf( e->client, "Player %s was rewarded %i times and chosen for items %i times.\n", rewardee->name, g_ppTeamRewardDebug[i]->kill_count, g_ppTeamRewardDebug[i]->item_count );
        conPrintf( e->client, "-- %i debt payed off.  %i prestige awarded\n", g_ppTeamRewardDebug[i]->debt, g_ppTeamRewardDebug[i]->prestige);
        conPrintf( e->client, "-- %i influence ( %i per kill ) and %i experence (%i per kill )\n", g_ppTeamRewardDebug[i]->influence, g_ppTeamRewardDebug[i]->influence/g_ppTeamRewardDebug[i]->kill_count, g_ppTeamRewardDebug[i]->experience, g_ppTeamRewardDebug[i]->experience/g_ppTeamRewardDebug[i]->kill_count );
        conPrintf( e->client, "-- %i Inspirations Drops ( %.2f%% )\n", g_ppTeamRewardDebug[i]->inspirations, ((F32)g_ppTeamRewardDebug[i]->inspirations*100.f)/g_ppTeamRewardDebug[i]->item_count  );
        conPrintf( e->client, "-- %i Enhancement Drops ( %.2f%% )\n", g_ppTeamRewardDebug[i]->enhancement_cnt, ((F32)g_ppTeamRewardDebug[i]->enhancement_cnt*100.f)/g_ppTeamRewardDebug[i]->item_count  );
        conPrintf( e->client, "-- %i Salvage Drops ( %.2f%% )\n", g_ppTeamRewardDebug[i]->salvage_cnt, ((F32)g_ppTeamRewardDebug[i]->salvage_cnt*100.f)/g_ppTeamRewardDebug[i]->item_count );
        conPrintf( e->client, "-- %i Recipe Drops ( %.2f%% )\n", g_ppTeamRewardDebug[i]->recipe_cnt, ((F32)g_ppTeamRewardDebug[i]->recipe_cnt*100.f)/g_ppTeamRewardDebug[i]->item_count );
    }
}


//-----------------------------------------------------------------------------
// End Reward splitting
//-----------------------------------------------------------------------------

/* End of File */
