/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/utils/textparser.h>
#include "entity/character_eval.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/eval.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/components/bitfield.h>
#include <utilitieslib/utils/timing.h>

#include "pl_stats.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "gameComm/VillainDef.h"
#include "gameComm/sendToClient.h"
#include "entity/entGameActions.h"
#include "dbcomm/dbcomm.h"
#include "Reward.h"
#include "language/langServerUtil.h"
#include "language/commonLangUtil.h"
#include "auth/authUserData.h"
#include "gameSys/arenamap.h"
#include <utilitieslib/utils/structDefines.h>

#include "entity/origins.h"
#include "entity/classes.h"
#include "entity/character_base.h"
#include "entity/character_level.h"

#include "player/badges.h"
#include "badges_server.h"

#include <utilitieslib/language/MessageStore.h>
#include "TeamReward.h"
#include "container/team.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "BadgeStats.h"
#include "badges_load.h"
#include "entity/SgrpBadges.h"
#include "entity/SgrpStats.h"
#include <utilitieslib/components/StashTable.h>
#include "cmdparse/cmdserver.h"
#include <utilitieslib/language/AppLocale.h>
#include "plaque.h" // badge reward popups
#include "comm_game.h"
#include "player/pophelp.h"
#include "dbcomm/logcomm.h"

static EvalContext *s_pBadgeEval;
static bool s_bSystemDirty = false;
static char g_badge_localize_ret[1000];

/**********************************************************************func*
 * Percent
 *
 */
static void BadgeMillionPercent(EvalContext *pcontext)
{
    float rhs = (float)eval_IntPop(pcontext);
    float lhs = (float)eval_IntPop(pcontext);

    eval_IntPush(pcontext, ((lhs*1000000.0f)/rhs));
}


/**********************************************************************func*
 * badge_EvalInit
 *
 * Note: keep in sync with badges_load.c:LoadBadgeStatUsage.
 */
void badge_EvalInit(void)
{
    // Create the evaluation context used by everyone.    
    if(verify( s_pBadgeEval == NULL ))
    {
        s_pBadgeEval = eval_Create();
        
        eval_SetBlame(s_pBadgeEval, "defs/badges.def");
        
        // Add all standard entity and supergroup eval funcs
        chareval_AddDefaultFuncs(s_pBadgeEval);
        
        // Add badge-specific funcs
        eval_ReRegisterFunc(s_pBadgeEval, "%",              BadgeMillionPercent, 1, 1);
        eval_RegisterFunc(s_pBadgeEval,   "int>",           chareval_EntityBadgeStatFetch, 1, 1);
    }
}



/**********************************************************************func*
 * badge_StatSet
 *
 */
bool badge_StatSet(Entity *e, const char *pchStat, int iVal)
{
    int idx;
    if(e && e->pl && stashFindInt(g_hashBadgeStatNames, pchStat, &idx))
    {
        if(idx<=g_iMaxBadgeStatIdx)
        {
            e->pl->aiBadgeStats[idx] = iVal;
            MarkModifiedBadges(g_hashBadgeStatUsage, e->pl->aiBadges, pchStat);
            return true;
        }
    }
    return false;
}


/**********************************************************************func*
 * s_AddSafe
 * small helper specifically for badge_StatAdd
 */
static void s_AddSafe(int *v, int dv)
{
    if( verify( v ))
    {
        int i = *v + dv;
        if(i>=0)
        {
            *v = i;
        }
        else
        {
            // Overflow, just clamp to max
            *v = 0x7fffffff;
        }
    }
}

extern bool badge_IsFixupGroupStat(const char *pchStat);

static char *s_fixupGroupStat(Entity *e, const char *pchStat)
{    
    static char *eStat = NULL;
    char *res = NULL;
    
    if(badge_IsFixupGroupStat(pchStat))
    {
        if(e->teamup)
        {
            switch ( e->teamup->members.count )
            {
            case 1:
                estrPrintf(&eStat,"%s.group1",pchStat);
                break;
            case 2:
                estrPrintf(&eStat,"%s.group2",pchStat);
                break;
            case 3:
            case 4:
                estrPrintf(&eStat,"%s.group3_4",pchStat);
                break;
            case 5:
            case 6:
            case 7:
            case 8:
                estrPrintf(&eStat,"%s.group5_8",pchStat);
                break;                    
            default:
                Errorf("invalid switch value.");
                estrPrintCharString(&eStat, pchStat);
                break;
            };
            STATIC_INFUNC_ASSERT(MAX_TEAM_MEMBERS == 8);
            res = eStat;
        }
    }
    return res;
}
/**********************************************************************func*
 * badge_StatAdd
 * NOTE: keep in sync with badge_StatAddSgrpSpecial 
 */
bool badge_StatAddNoFixup(Entity *e, const char *pchStat, int iAdd)
{
    int idx;
    int res = false;
    if( devassert( e )
        && e->pl )
    {        
        // handle player
        if(stashFindInt(g_hashBadgeStatNames, pchStat, &idx))
        {
            if(idx<=g_iMaxBadgeStatIdx)
            {
                s_AddSafe( &e->pl->aiBadgeStats[idx], iAdd );
                MarkModifiedBadges(g_hashBadgeStatUsage, e->pl->aiBadges, pchStat);
                res = true;
            }
        }        

        // forward to supergroup
        if( e->pl->supergroup_mode )
        {
            int iAddFinal = iAdd;
            char tmp[512] = "sg.";
            Strncatt( tmp, pchStat );

            // ab: default behavior for these stats as getting added at the same amount
            // is okay.
//              // check special
//             if(strStartsWith(pchStat,"kills"))
//             {
//                 Errorf("kill statistics should call badge_StatAddSgrpSpecial");
//             }
//             else
            {
                // apply
                res |= sgrpstats_StatAdj( e->supergroup_id, tmp, iAddFinal );
            }
        }        
    }
    return res;
}

bool badge_StatAdd(Entity *e, const char *pchStat, int iAdd)
{
    const char *fixup;
    bool res;

    if( g_ArchitectTaskForce ) 
        return 0;
    
    res = badge_StatAddNoFixup(e,pchStat,iAdd);
    fixup = s_fixupGroupStat(e,pchStat);
    if(fixup)
        badge_StatAddNoFixup(e,fixup,iAdd);
    return res;
}

bool badge_StatAddArchitectAllowed(Entity *e, const char *pchStat, int iAdd, int iAllowCheat)
{
    char *fixup = 0;
    bool res;

    // Some stats have to be incremented even if the 'cheats' were used. For
    // example the badge total, because the badges will still award.
    if( g_ArchitectCheated && !iAllowCheat )
        return 0;

    res = badge_StatAddNoFixup(e,pchStat,iAdd);
    fixup = s_fixupGroupStat(e,pchStat);
    if(fixup)
        badge_StatAddNoFixup(e,fixup,iAdd);
        
    return res;
}


/**********************************************************************func*
 * badge_StatAddSgrpSpecial 
 * a way to add a different value to a supergroup than a player
 * NOTE: keep in sync with badge_StatAdd
 */
bool badge_StatAddSgrpSpecialNoFixup(Entity *e, const char *pchStat, int iAdd, int iAddSgrp)
{
    int idx;
    int res = false;
    if( devassert( e )
        && e->pl && pchStat )
    {        
        // handle player
        if(stashFindInt(g_hashBadgeStatNames, pchStat, &idx))
        {
            if(idx<=g_iMaxBadgeStatIdx)
            {
                s_AddSafe( &e->pl->aiBadgeStats[idx], iAdd );
                MarkModifiedBadges(g_hashBadgeStatUsage, e->pl->aiBadges, pchStat);
                res = true;
            }
        }

        // forward to supergroup
        if( e->pl->supergroup_mode )
        {
            char tmp[512] = "sg.";
            Strncatt( tmp, pchStat );

            // --------------------
            // apply

            res |= sgrpstats_StatAdj( e->supergroup_id, tmp, iAddSgrp );
        }
    }
    return res;
}

bool badge_StatAddSgrpSpecial(Entity *e, const char *pchStat, int iAdd, int iAddSgrp)
{
    char *fixup;
    bool res;
    if(g_ArchitectTaskForce)
        return 0;

    res = badge_StatAddSgrpSpecialNoFixup(e,pchStat,iAdd,iAddSgrp);
    fixup = s_fixupGroupStat(e,pchStat);
    if(fixup)
        badge_StatAddSgrpSpecialNoFixup(e,fixup,iAdd,iAddSgrp);
    return res;
}


/**********************************************************************func*
 * badge_StatGet
 *
 */
int badge_StatGet(Entity *e, const char *pchStat)
{
    int idx;
    if(e && e->pl && stashFindInt(g_hashBadgeStatNames, pchStat, &idx))
    {
        if(idx<=g_iMaxBadgeStatIdx)
        {
            return e->pl->aiBadgeStats[idx];
        }
    }

    return 0;
}

/**********************************************************************func*
 * badge_StatGetIdx
 *
 */
int badge_StatGetIdx(const char *pchStat)
{
    int idx;
    if(stashFindInt(g_hashBadgeStatNames, pchStat, &idx))
    {
        if(idx<=g_iMaxBadgeStatIdx)
        {
            return idx;
        }
    }

    return -1;
}

/**********************************************************************func*
* badge_RecordGhostTrapped
*
*/
void badge_RecordGhostTrapped(Entity *e)
{
    badge_StatAdd(e, "GhostsTrapped", 1);
}

/**********************************************************************func*
 * badge_RecordKill
 *
 */
void badge_RecordKill(Entity *e, Entity *eVictim, F32 fPercent)
{
    static char *estr = NULL;
    int iAddSgrp = fPercent*32.f;

    if(!estr)
        estrCreate(&estr);

    if(g_ArchitectTaskForce)
    {
        if( g_ArchitectTaskForce->architect_flags&ARCHITECT_TEST && !g_ArchitectCheated )
            badge_StatAddArchitectAllowed(e, "Architect.TestKill", 1, 0);
        else
        {
            if( eVictim && eVictim->custom_critter )
                badge_StatAddArchitectAllowed(e, "Architect.CustomKills", 1, 0);
            badge_StatAddArchitectAllowed(e, "Architect.Kill", 1, 0);
        }
    }

    badge_StatAddSgrpSpecial(e, "kills", 1, iAddSgrp);

    if(eVictim)
    {
        if(ENTTYPE(eVictim)==ENTTYPE_PLAYER)
        {
            // pvp and arena kills don't get multiplied out for supergroups.
            if(OnArenaMap())
            {
                badge_StatAdd(e, "kills.arena", 1);
                badge_StatAdd(eVictim, "deaths.arena", 1);
            }
            else
            {
                badge_StatAdd(e, "kills.pvp",1);
                badge_StatAdd(eVictim, "deaths.pvp",1);
            }
        }
        else if(eVictim->villainDef)
        {
            const char *str;

            if(!(eVictim->villainDef->flags & VILLAINDEF_NOGENERICBADGESTAT))
                badge_StatAddSgrpSpecial(e, "kills.npc", 1, iAddSgrp);

            if(!(eVictim->villainDef->flags & VILLAINDEF_NONAMEBADGESTAT))
            {
                estrPrintCharString(&estr, "kills.");
                estrConcatCharString(&estr, eVictim->villainDef->name);
                badge_StatAddSgrpSpecial(e, estr, 1, iAddSgrp);
            }

            if(!(eVictim->villainDef->flags & VILLAINDEF_NOGROUPBADGESTAT))
            {
                estrPrintCharString(&estr, "kills.");
                estrConcatCharString(&estr, villainGroupGetName(eVictim->villainDef->group));
                badge_StatAddSgrpSpecial(e, estr, 1, iAddSgrp);
            }

            if(!(eVictim->villainDef->flags & VILLAINDEF_NORANKBADGESTAT))
            {
                str = StaticDefineIntRevLookup(ParseVillainRankEnum, eVictim->villainRank);
                if(str)
                {
                    estrPrintCharString(&estr, "kills.");
                    estrConcatCharString(&estr, villainGroupGetName(eVictim->villainDef->group));
                    estrConcatChar(&estr, '.');
                    estrConcatCharString(&estr, str);
                    badge_StatAddSgrpSpecial(e, estr, 1, iAddSgrp);
                    
                    estrPrintCharString(&estr, "kills.");
                    estrConcatCharString(&estr, str);
                    badge_StatAddSgrpSpecial(e, estr, 1, iAddSgrp);
                }
            }

            if(eVictim->villainDef->customBadgeStat)
                badge_StatAddSgrpSpecial(e, eVictim->villainDef->customBadgeStat, 1, iAddSgrp);
        }
    }
}

/**********************************************************************func*
 * badge_RecordDefeat
 *
 */
void badge_RecordDefeat(Entity *e, Entity *eVictor)
{
    static char *estr = NULL;

    if(!estr)
        estrCreate(&estr);

    badge_StatAdd(e, "defeats", 1);

    if(g_ArchitectTaskForce && g_ArchitectTaskForce->architect_flags&ARCHITECT_TEST && !g_ArchitectCheated )
        badge_Award(e, "ArchitectDefeatedinTest", 1 );

    if(eVictor)
    {
        if(ENTTYPE(eVictor)==ENTTYPE_PLAYER)
        {
            badge_StatAdd(e, OnArenaMap()?"defeats.arena":"defeats.pvp", 1);
        }
        else if(eVictor->villainDef)
        {
            estrPrintCharString(&estr, "defeats.");
            estrConcatCharString(&estr, eVictor->villainDef->name);
            badge_StatAdd(e, estr, 1);

            estrPrintCharString(&estr, "defeats.");
            estrConcatCharString(&estr, villainGroupGetName(eVictor->villainDef->group));
            badge_StatAdd(e, estr, 1);
        }
    }
}

/**********************************************************************func*
 * badge_RecordPlaque
 *
 */
void badge_RecordPlaque(Entity *e, const char *pchName)
{
    static char *estr = NULL;

    if(!estr)
        estrCreate(&estr);

    badge_StatAdd(e, "visits", 1);

    if(pchName && *pchName)
    {
        estrPrintCharString(&estr, "visits.");
        estrConcatCharString(&estr, pchName);
        badge_StatAdd(e, estr, 1);
    }
}

/**********************************************************************func*
 * badge_RecordTime
 * test: PvP time, PvE time
 */
void badge_RecordTime(Entity *e, int iTime)
{
    static char *estr = NULL;
    char *tmp;
    bool handled = false;
    
    if(!e)
        return;
    
    if(!estr)
        estrCreate(&estr);

    // ----------------------------------------
    // time in map

    estrPrintCharString(&estr, "time.");

    // get supergroup base info
    if( (tmp=strstri(db_state.map_name, "supergroupid=")) )
    {
        int idSg = 0;
        if(1==sscanf(tmp, "supergroupid=%i",&idSg) && idSg )
        {
            if( idSg == e->supergroup_id )
                estrConcatStaticCharArray(&estr,"base.your");
            else
                estrConcatStaticCharArray(&estr,"base.other");
            badge_StatAdd(e, estr, iTime);
            handled = true;
        }
    }

    if(!handled)
    {
        if(*db_state.map_name=='/' || *db_state.map_name=='\\')
            estrConcatCharString(&estr, db_state.map_name+1);
        else
            estrConcatCharString(&estr, db_state.map_name);
        badge_StatAdd(e, estr, iTime);
    }

    // ----------------------------------------
    // time grouped/solo/pvp
    
    badge_StatAdd(e,"time",iTime);
    if(e->pchar && character_IsPvPActive(e->pchar))
        badge_StatAdd(e,"time.pvp",iTime);
        
    if(!server_state.levelEditor)
        addToRewardToken(e, "TimePlayedAsCurrentAlignment", iTime);
}

/**********************************************************************func*
 * badge_RecordLevelChange
 *
 */
void badge_RecordLevelChange(Entity *e)
{
    if(e && e->pl)
    {
        MarkModifiedBadges(g_hashBadgeStatUsage, e->pl->aiBadges, "*char");
    }
}


/**********************************************************************func*
 * badge_OwnsBadge
 *
 */
bool badge_OwnsBadge(Entity *e, const char *pchBadge)
{
    if(e && e->pl)
    {
        int iIdx;
        if(badge_GetIdxFromName(pchBadge, &iIdx))
        {
            if(iIdx<=g_BadgeDefs.idxMax)
            {
                return BitFieldGet(e->pl->aiBadgesOwned, BADGE_ENT_BITFIELD_SIZE, iIdx);
            }
        }
    }
    return false;
}

static void addBadgeToRecentList(EntPlayer *player, int iIdx, U32 timeAwarded)
{
    U32 count;

    for (count = (BADGE_ENT_RECENT_BADGES - 1); count > 0; count--)
    {
        player->recentBadges[count].idx = player->recentBadges[count - 1].idx;
        player->recentBadges[count].timeAwarded = player->recentBadges[count - 1].timeAwarded;
    }

    player->recentBadges[0].idx = iIdx;
    player->recentBadges[0].timeAwarded = timeAwarded;
}

/**********************************************************************func*
 * badge_Award
 *
 */
bool badge_Award(Entity *e, const char *pchBadge, bool bNotify)
{
    bool retval = false;
    const BadgeDef *pdef = badge_GetBadgeByName(pchBadge);

    // Must be a known badge, and we can't give out contact intro badges
    // while the player's on a taskforce (and thus their original contact
    // list is supressed).
    if(pdef && pdef->iIdx>0 && (!pdef->ppchContacts || !e->taskforce))
    {
        if((e->pl->aiBadges[pdef->iIdx] & BADGE_FULL_OWNERSHIP) != BADGE_FULL_OWNERSHIP)
        {
            e->pl->aiBadges[pdef->iIdx] = BADGE_FULL_OWNERSHIP | BADGE_CHANGED_MASK;
        }

        if(!BitFieldGet(e->pl->aiBadgesOwned, BADGE_ENT_BITFIELD_SIZE, pdef->iIdx))
        {
            MarkModifiedBadges(g_hashBadgeStatUsage, e->pl->aiBadges, "*badge");

            BitFieldSet(e->pl->aiBadgesOwned, BADGE_ENT_BITFIELD_SIZE, pdef->iIdx, 1);

            // Don't push any sundry or invisible badges onto the most recent
            // list, only ones the player will get to see in some other
            // category.
            if(!pdef->bDoNotCount && pdef->eCategory != kBadgeType_None)
            {
                addBadgeToRecentList(e->pl, pdef->iIdx, timerSecondsSince2000());

                // We're giving the badge so we have to update the badge stat
                // no matter what otherwise it will be out of synch.
                if( strnicmp( pdef->pchName, "Architect", 9 ) == 0 )
                    badge_StatAddArchitectAllowed(e, "Architect.Badges", 1, 1 );
            }

            LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Bdg]:Rcv: %s", pdef->pchName);

            // Allow even internal badges to show custom float text. This is
            // so they can introduce contacts.
            if(bNotify && (!pdef->bDoNotCount || pdef->pchAwardText))
            {
                if(pdef->eCategory != kBadgeType_Internal)
                {
                    sendInfoBox(e, INFO_REWARD, "BadgeYouReceived", printLocalizedEnt(pdef->pchDisplayTitle[ENT_IS_VILLAIN(e)?1:0], e));
                }

                if(pdef->pchAwardText)
                {
                    U32 color = (pdef->rgbaAwardText[0] << 24) +
                        (pdef->rgbaAwardText[1] << 16) +
                        (pdef->rgbaAwardText[2] << 8) +
                        pdef->rgbaAwardText[3];

                    serveFloater(e, e, pdef->pchAwardText, 0.0f, kFloaterStyle_Attention, color);
                }
                else if(pdef->eCategory == kBadgeType_Gladiator)
                {
                    serveFloater(e, e, "FloatEarnedGladiatorBadge", 0.0f, kFloaterStyle_Attention, 0);
                }
                else if(pdef->eCategory == kBadgeType_Veteran)
                {
                    serveFloater(e, e, "FloatEarnedVeteranBadge", 0.0f, kFloaterStyle_Attention, 0);
                }
                else if(pdef->eCategory != kBadgeType_Internal)
                {
                    serveFloater(e, e, "FloatEarnedBadge", 0.0f, kFloaterStyle_Attention, 0);
                }
            }

            if(pdef->ppchReward!=NULL && eaSize(&pdef->ppchReward))
            {
                int iEffCombatLevel;
                // HACK: THis has some internal knowledge of the rewardTeamMember
                // function so that it never handicaps them.
                if(e->pchar->buddy.uTimeLastMentored+60>dbSecondsSince2000())
                {
                    iEffCombatLevel = e->pchar->buddy.iCombatLevelLastMentored;
                }
                else
                {
                    iEffCombatLevel = e->pchar->iCombatLevel;
                }

                // @todo -AB: This is deprecated. the assert maintains compatability :2005 Apr 12 02:13 PM
                verify(iEffCombatLevel == character_CalcEffectiveCombatLevel(e->pchar));

                rewardFindDefAndApplyToDbID(e->db_id, pdef->ppchReward, VG_NONE, character_CalcEffectiveCombatLevel(e->pchar)+1, 0, REWARDSOURCE_BADGE);
            }

            // Grant access to all the listed contacts as a benefit.
            if(pdef->ppchContacts!=NULL && eaSize(&pdef->ppchContacts))
            {
                int count, didILockIt = false;
                StoryInfo *info;

                if (!StoryArcInfoIsLocked())
                {
                    info = StoryArcLockInfo(e);
                    didILockIt = true;
                }
                else
                {
                    info = StoryInfoFromPlayer(e);
                }

                for(count = 0; count < eaSize(&pdef->ppchContacts); count++)
                {
                    ContactHandle chandle;

                    chandle = ContactGetHandle(pdef->ppchContacts[count]);

                    // Only add contacts known to the system but not the
                    // player, so their contact list stays valid with no
                    // duplicates.
                    if(chandle && !ContactGetInfo(info, chandle))
                    {
                        ContactAdd(e, info, chandle, "Granted by Badge");
                    }
                }

                if (didILockIt)
                {
                    StoryArcUnlockInfo(e);
                }
            }

            if(pdef->pchDisplayPopup)
            {
                ScriptVarsTable vars = {0};
                saBuildScriptVarsTable(&vars, e, e->db_id, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, false);
                msxPrintf(svrMenuMessages,SAFESTR(g_badge_localize_ret),
                    getCurrentLocale(),pdef->pchDisplayPopup,NULL,&vars,0);
                sendDialog(e,localizedPrintf(e,g_badge_localize_ret));
            }

            if (pdef->eCategory == kBadgeType_Tourism && getPopHelpStateByTag(e, "CODEPH_Found_Exploration_Badge") == PopHelpState_Untriggered)
            {
                triggerPopHelpEventHappenedByTag(e, "CODEPH_Found_Exploration_Badge");
            }

            // update powers since you may have earned one
            if (e->pchar) 
                character_GrantAutoIssuePowers(e->pchar);
        }

        retval = true;
    }

    return retval;
}


/**********************************************************************func*
 * badge_ButtonUse
 *
 */
bool badge_ButtonUse(Entity *e, int iIdx)
{
    const BadgeDef *pdef = badge_GetBadgeByIdx(iIdx);

    // Must be a valid badge which the character owns, which they haven't
    // already clicked on and which actually gives out a reward.
    if(pdef && iIdx>0 && BitFieldGet(e->pl->aiBadgesOwned, BADGE_ENT_BITFIELD_SIZE, iIdx) &&
        !BADGE_IS_REWARDING(e->pl->aiBadges[iIdx]) && pdef->ppchButtonReward!=NULL && eaSize(&pdef->ppchButtonReward))
    {
        // Marking the badge as 'reward in progress' stops it being used again
        // until it is updated, at which point it will either revoke or we'll
        // know it's meant to be used again and it's safe to do so.
        BADGE_SET_REWARD(e->pl->aiBadges[iIdx]);

        if( pdef->eCategory == kBadgeType_Veteran )
            serveFloater(e, e, "FloatClaimedVeteranBadge", 0.0f, kFloaterStyle_Attention, 0);
        else
            serveFloater(e, e, "FloatClaimedBadge", 0.0f, kFloaterStyle_Attention, 0);

        rewardFindDefAndApplyToDbID(e->db_id, pdef->ppchButtonReward, VG_NONE, character_CalcExperienceLevel(e->pchar)+1, 1, REWARDSOURCE_BADGE_BUTTON);

        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Bdg]:Used: %s", pdef->pchName);
        return true;
    }
    return false;
}

/**********************************************************************func*
 * badge_Revoke
 *
 */
bool badge_Revoke(Entity *e, const char *pchBadge)
{
    const BadgeDef *pdef = badge_GetBadgeByName(pchBadge);

    if(pdef && pdef->iIdx>0)
    {
        MarkModifiedBadges(g_hashBadgeStatUsage, e->pl->aiBadges, "*badge");

        BitFieldSet(e->pl->aiBadgesOwned, BADGE_ENT_BITFIELD_SIZE, pdef->iIdx, 0);

        e->pl->aiBadges[pdef->iIdx] = BADGE_CHANGED_MASK;

        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Bdg]:Revoke: %s", pdef->pchName);

        return true;
    }

    return false;
}

//------------------------------------------------------------
//  revoke a given badge. also marks the badges affected by badges.
//----------------------------------------------------------
bool badgestates_Revoke(SgrpBadges *bs, const BadgeDef *pdef, StashTable idxBadgesFromName )
{
    if(verify(bs && idxBadgesFromName && pdef && pdef->iIdx>0))
    {
        MarkModifiedBadges(idxBadgesFromName, bs->eaiStates, "*badge");

        BitFieldSet(bs->abitsOwned,ARRAY_SIZE( bs->abitsOwned ), pdef->iIdx, 0);

        bs->eaiStates[pdef->iIdx] = BADGE_CHANGED_MASK;

        LOG_OLD( "[Bdg]:Revoke: %s", pdef->pchName);

        return true;
    }

    return false;
}

/**********************************************************************func*
 * badge_Tick
 *
 */
void badge_Tick(Entity *e)
{
    badge_UpdateBadgesCollection(e);
}

/**********************************************************************func*
 * badge_UpdateBadgesCollection
 *
 */
void badge_UpdateBadgesCollection(Entity *e)
{
    int n = eaSize(&g_BadgeDefs.ppBadges);
    int i;

    if(n > BADGE_ENT_MAX_BADGES)
    {
        ErrorFilenamef("defs/badges.def", "Too many badges defined!");
        n = BADGE_ENT_MAX_BADGES;
    }

    if(s_bSystemDirty)
    {
        MarkModifiedBadges(g_hashBadgeStatUsage, e->pl->aiBadges, "*system");
    }

    for(i=0; i<n; i++)
    {
        const BadgeDef *badge = g_BadgeDefs.ppBadges[i];
        int iBadgeIdx = badge->iIdx;

        // We must have a valid badge and we can't award contact-intro
        // badges while the player is on a taskforce (and thus their contact
        // list is hidden).
        if(iBadgeIdx>0 && (!e->taskforce || !badge->ppchContacts))
        {
            int iBadge = e->pl->aiBadges[iBadgeIdx];

            if(!BADGE_IS_UPDATED(iBadge))
            {
                if(BADGE_IS_OWNED(iBadge))
                {
                    eval_StoreInt(s_pBadgeEval, "Entity", (int)(intptr_t)e);
                    eval_ClearStack(s_pBadgeEval);

                    if(badge->ppchRevoke
                       && eval_Evaluate(s_pBadgeEval, badge->ppchRevoke))
                    {
                        badge_Revoke(e, badge->pchName);
                    }
                }

                // Not an else since the last part might have changed OWNED.
                if(!BADGE_IS_OWNED(iBadge))
                {
                    eval_StoreInt(s_pBadgeEval, "Entity", (int)(intptr_t)e);
                    eval_ClearStack(s_pBadgeEval);

                    if(BitFieldGet(e->pl->aiBadgesOwned, BADGE_ENT_BITFIELD_SIZE, iBadgeIdx))
                    {
                        // i think this only occurs during login/zoning
                        // this fixes the case where someone zones before getting a badge_tick to revoke a badge 
                        // -Garth
                        badge_Award(e, badge->pchName, true);

                        if(badge->ppchRevoke
                            && eval_Evaluate(s_pBadgeEval, badge->ppchRevoke))
                        {
                            badge_Revoke(e, badge->pchName);
                        }
                    }
                    else if(eval_Evaluate(s_pBadgeEval, badge->ppchRequires))
                    {
                        badge_Award(e, badge->pchName, true);
                    }
                    else
                    {
                        if(eval_Evaluate(s_pBadgeEval, badge->ppchVisible))
                        {
                            BADGE_SET_VISIBLE(iBadge);

                            if(eval_Evaluate(s_pBadgeEval, badge->ppchKnown))
                            {
                                BADGE_SET_KNOWN(iBadge);
                            }

                            eval_Evaluate(s_pBadgeEval, badge->ppchMeter);
                            BADGE_CLEAR_COMPLETION(iBadge);

                            if(!eval_StackIsEmpty(s_pBadgeEval))
                            {
                                int val = eval_IntPop(s_pBadgeEval);
                                if(val>0)
                                {
                                    if(val>1000000)
                                        val=1000000;

                                    BADGE_SET_COMPLETION(iBadge, val);
                                }
                            }
                        }

                        if(e->pl->aiBadges[iBadgeIdx]!=iBadge)
                        {
                            e->pl->aiBadges[iBadgeIdx]=iBadge;
                            BADGE_SET_CHANGED(e->pl->aiBadges[iBadgeIdx]);
                        }
                    }
                } // end if !owned

                // 
                BADGE_SET_UPDATED(e->pl->aiBadges[iBadgeIdx]);

                // If the badge was marked as having given a reward it will
                // either have been revoked or it's allowed to give that
                // reward again now that we've updated it.
                BADGE_CLEAR_REWARD(e->pl->aiBadges[iBadgeIdx]);

            } // end if not updated yet
        } // end if valid badge
    } // end for each badge
}

/**********************************************************************func*
 * entity_SendBadgeUpdate
 *
 */
void entity_SendBadgeUpdate(Entity *e, Packet *pak, bool full_update)
{
    int i;
    int n = eaSize(&g_BadgeDefs.ppBadges);
    U32 *eaiStates = NULL;
    bool send_history = false;

    // Badge update
    //
    // 0: No badge
    // 1: badge update
    //    badge index
    //    1: owned
    //       button
    //    0: not owned
    //       visible
    //       known
    //       meter

    if(n > BADGE_ENT_MAX_BADGES)
    {
        ErrorFilenamef("defs/badges.def", "Too many badges defined!");
        n = BADGE_ENT_MAX_BADGES;
    }

    // Debug level (used to show unearned badges during development)
    pktSendBitsPack(pak, 4, e->pl->debugBadgeLevel);

    for(i=0; i<n; i++)
    {
        const BadgeDef *badge = g_BadgeDefs.ppBadges[i];
        if(badge->iIdx>0)
        {
            int iBadge = e->pl->aiBadges[badge->iIdx];
            if(BADGE_IS_CHANGED(iBadge)
               || full_update) // if its a full update, send it anyway.
            {
                BADGE_CLEAR_CHANGED(iBadge);

                // if their overall badge state changed the history must
                // have changed as well
                send_history = true;

                pktSendBits(pak, 1, 1);
                pktSendBitsPack(pak, 6, badge->iIdx);
                if(BADGE_IS_OWNED(iBadge))
                {
                    pktSendBits(pak, 1, 1);
                }
                else
                {
                    pktSendBits(pak, 1, 0);
                    pktSendBits(pak, 1, BADGE_IS_VISIBLE(iBadge)?1:0);

                    if(BADGE_IS_KNOWN(iBadge) || e->pl->debugBadgeLevel == 1 || e->pl->debugBadgeLevel == 2)
                    {
                        pktSendBits(pak, 1, 1);
                    }
                    else
                    {
                        pktSendBits(pak, 1, 0);
                    }

                    pktSendIfSetBits(pak, 20, BADGE_COMPLETION(iBadge));
                }

                pktSendBits(pak, 1, server_state.clickToSource);
                if (server_state.clickToSource)
                    pktSendString(pak, badge->filename);

                e->pl->aiBadges[badge->iIdx] = iBadge;

            }
        } // end if valid badge
    } // end for each badge
    pktSendBits(pak, 1, 0);

    // --------------------
    // recent badge history

    pktSendBits(pak, 1, send_history ? 1 : 0);

    if(send_history)
    {
        i = 0;
        n = 0;

        while(n < BADGE_ENT_RECENT_BADGES && e->pl->recentBadges[n].idx != 0)
            n++;

        pktSendBits(pak, 8, n);

        for(i=0; i<n; i++)
        {
            pktSendBits(pak, 16, e->pl->recentBadges[i].idx);
            pktSendBits(pak, 32, e->pl->recentBadges[i].timeAwarded);
        }
    }

    // --------------------
    // now supergroup

    if( e->supergroup_id )
    {
        eaiStates = sgrpbadge_eaiStatesFromId( e->db_id );
        pktSendBits(pak, 1, 1);
    }
    else
    {
        pktSendBits(pak, 1, 0 );
        return;
    }

    n = MIN(eaSize(&g_SgroupBadges.ppBadges), BADGE_SG_MAX_BADGES);

    for(i=0; i<n; i++)
    {
        const BadgeDef *sgBadge = g_SgroupBadges.ppBadges[i];
        if(sgBadge->iIdx>0)
        {
            
            int iBadge = eaiStates[sgBadge->iIdx];
            if(BADGE_IS_CHANGED(iBadge)
               || full_update ) // send it anyways if a full update
            {
                BADGE_CLEAR_CHANGED(iBadge);

                pktSendBits(pak, 1, 1);
                pktSendBitsPack(pak, 6, sgBadge->iIdx);
                if(BADGE_IS_OWNED(iBadge))
                {
                    pktSendBits(pak, 1, 1);
                }
                else
                {
                    pktSendBits(pak, 1, 0);
                    pktSendBits(pak, 1, BADGE_IS_VISIBLE(iBadge)?1:0);

                    if(BADGE_IS_KNOWN(iBadge) || e->pl->debugBadgeLevel == 1 || e->pl->debugBadgeLevel == 2)
                    {
                        pktSendBits(pak, 1, 1);
                    }
                    else
                    {
                        pktSendBits(pak, 1, 0);
                    }

                    pktSendIfSetBits(pak, 20, BADGE_COMPLETION(iBadge));
                }

                pktSendBits(pak, 1, server_state.clickToSource);
                if (server_state.clickToSource)
                    pktSendString(pak, sgBadge->filename);

                eaiStates[sgBadge->iIdx] = iBadge;

            } // end if !owned
        } // end if valid badge
    } // end for each badge
    pktSendBits(pak, 1, 0);

}


/**********************************************************************func*
 * badge_RecordRewardToken
 *
 */
void badge_RecordRewardToken(Entity *e)
{
    // this is a character modifying stat
    MarkModifiedBadges(g_hashBadgeStatUsage, e->pl->aiBadges, "*char");
}

/**********************************************************************func*
* badge_RecordAccountInventoryUpdate
*
*/
void badge_RecordAccountInventoryUpdate(Entity *e)
{
    // this is a character modifying stat
    if (e && e->pl)
        MarkModifiedBadges(g_hashBadgeStatUsage, e->pl->aiBadges, "*char");
}

/**********************************************************************func*
 * badge_RecordSouvenirClue
 *
 */
void badge_RecordSouvenirClue(Entity *e, const char *pchClue)
{
    MarkModifiedBadges(g_hashBadgeStatUsage, e->pl->aiBadges, "*souvenir");
}

/**********************************************************************func*
 * badge_RecordSystemChange
 *
 */
void badge_RecordSystemChange(void)
{
    // A system change affects all entities. Instead of looping over all the 
    //   entities, I decided to set a static to note that the *system badges
    //   needed updating. When the badge_tick for an entity happens and
    //   *system is marked as dirty, the badges are marked as modified there.
    // The global dirty flag is eventually cleared by a call to 
    //   badge_TicksComplete after all players have had a pass through 
    //   badge_tick.
    s_bSystemDirty = true;
}

/**********************************************************************func*
 * badge_ServerInit
 *
 */
void badge_ServerInit(void)
{
    static bool bInitDone = false;
    char *filename = NULL;
    if(bInitDone)
        return;

    // --------------------
    // init the player badge and stat

    filename = "server/db/templates/badgestats.attribute";
    g_hashBadgeStatNames = stashTableCreateWithStringKeys(128, StashDeepCopyKeys);
    g_iMaxBadgeStatIdx = LoadBadgeStatNames(filename, g_hashBadgeStatNames, NULL );

    if(g_iMaxBadgeStatIdx>=BADGE_ENT_MAX_STATS)
    {
        g_iMaxBadgeStatIdx=BADGE_ENT_MAX_STATS-1;
        ErrorFilenamef(filename, "Too many badge stats used!");
    }
    g_hashBadgeStatUsage = LoadBadgeStatUsage(g_hashBadgeStatUsage, &g_BadgeDefs, "defs/badges.def" );

    // --------------------
    // init the badges eval

    badge_EvalInit();

    // --------------------
    // finally

    bInitDone=true;
}


/**********************************************************************func*
 * badge_RecordMissionComplete
 *
 * record mission complete with optional name
 *
 */
void badge_RecordMissionComplete(Entity *e, const char *missionName, VillainGroupEnum vgrp)
{
    if(verify( e ))
    {        
        char *stat = NULL;
        const char *nameVgrp = villainGroupGetName(vgrp);
        
        estrCreate(&stat);

        // record the bare stat
        badge_StatAdd(e, "missions", 1);
        
        // missions.name
        if( missionName )
        {
            estrPrintf(&stat,"missions.%s", missionName);
            badge_StatAdd(e, stat, 1);
        }

        // missions.villaingroup
        if( nameVgrp )
        {
            estrPrintf(&stat,"missions.%s", nameVgrp);
            badge_StatAdd(e, stat, 1);
        }

        estrDestroy(&stat);
    }
}


/**********************************************************************func*
* badge_RecordInventionCreated
*
* record recipe usage (total, and by name.level)
*
*/
void badge_RecordInventionCreated(Entity *e, const char *recipename, bool dontGiveGenericCredit)
{
    if(verify(e))
    {
        char *stat = NULL;

        if (!dontGiveGenericCredit)
        {
            badge_StatAdd(e,"Invention.created",1);
        }

        if(recipename)
        {
            estrPrintf(&stat,"invented.%s",recipename);
            badge_StatAdd(e,stat,1);
        }

        estrDestroy(&stat);
    }
}


/**********************************************************************func*
 * badge_TicksComplete
 *
 */
void badge_TicksComplete(void)
{
    // See badge_RecordSystemChange for info.
    s_bSystemDirty = false;
}

void badgeMonitor_SendToClient(Entity *e)
{
    if (e && e->pl)
    {
        START_PACKET(pak, e, SERVER_UPDATE_BADGE_MONITOR_LIST);
        badgeMonitor_SendInfo(e, pak);
        END_PACKET
    }
}

/* End of File */
