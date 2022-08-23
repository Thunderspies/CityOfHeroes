/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>

#include <utilitieslib/components/earray.h>

#include "container/dbcontainerpack.h"
#include "gameComm/sendToClient.h" // conprintf

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "cmdparse/cmdserver.h"
#include "entity/character_base.h"
#include "gameComm/VillainDef.h"
#include "gameSys/arenamap.h"

#include "stats_base.h"
#include "pl_stats.h"
#include "pl_stats_db.h"
#include "pl_stats_internal.h"

#include "language/langServerUtil.h"
#include <utilitieslib/utils/timing.h>
#include "storyarc/mission.h"
#include "player/badges_server.h"
#include "entity/character_level.h"
#include "cmdparse/cmdstatserver.h"
#include "entity/teamCommon.h"
#include "dbcomm/logcomm.h"

/**********************************************************************func*
 * packageEntStats
 *
 */
void packageEntStats(Entity *e, StuffBuff *psb, StructDesc *desc, U32 uLastUpdateTime)
{
    DBStats dbstats;
    int i, j, k;
    int iCnt = 0;

    if(server_state.nostats)
        return;

    stat_UpdatePeriod(e, uLastUpdateTime);

    memset(&dbstats, 0, sizeof(DBStats));
    for(i=stat_GetStatCount()-1; i>=0; i--)
    {
        bool bHasStat = false;

        for(j=0; j<kStat_Count; j++)
        {
            for(k=0; k<kStatPeriod_Count; k++)
            {
                if(e->pl->stats[k].cats[j][i]!=0)
                {
                    const char *pch = stat_GetStatName(i);
                    if(pch!=NULL)
                    {
                        if(!bHasStat)
                        {
                            int t;
                            int c;
                            bHasStat = true;

                            dbstats.stats[iCnt].pchItem = pch;

                            // Clear out all the other columns
                            for(c=0; c<kStat_Count; c++)
                            {
                                for(t=0; t<kStatPeriod_Count; t++)
                                {
                                    dbstats.stats[iCnt].iValues[c][t] = 0;
                                }
                            }
                        }

                        dbstats.stats[iCnt].iValues[j][k] = e->pl->stats[k].cats[j][i];
                    }
                }
            }

        }

        if(bHasStat)
        {
            iCnt++;

            if(iCnt>=(MAX_DB_STATS-1))
            {
                assertmsg(0, "Not enough stat slots!");
                // Bail from the loop
                i = stat_GetStatCount();
            }
        }

    }

    // Evil hack
    desc->lineDescs[0].size = iCnt;
    for(i=0;desc->lineDescs[i].type;i++)
    {
        structToLine(psb,(char*)&dbstats,0,desc,&desc->lineDescs[i],0);
    }
    // This is undoing an evil hack
    desc->lineDescs[0].size = MAX_DB_STATS;
}

/**********************************************************************func*
 * unpackEntStats
 *
 */
void unpackEntStats(Entity *e, DBStats *pdbstats)
{
    int i = 0;

    while(pdbstats->stats[i].pchItem!=NULL)
    {
        int c;

        for(c=0; c<kStat_Count; c++)
        {
            int k;
            for(k=0; k<kStatPeriod_Count; k++)
            {
                stat_SetWithPeriod(e->pl,
                    k,
                    c,
                    pdbstats->stats[i].pchItem,
                    pdbstats->stats[i].iValues[c][k]);
            }
        }

        i++;
    }

    stat_UpdatePeriod(e, e->last_time);
}

/**********************************************************************func*
 * stat_Init
 *
 */
void stat_Init(EntPlayer *pl)
{
    int i, j;

    for(i=0; i<kStatPeriod_Count; i++)
    {
        for(j=0; j<kStat_Count; j++)
        {
            if(pl->stats[i].cats[j]==NULL)
            {
                eaiCreate(&pl->stats[i].cats[j]);
            }
            else
            {
                eaiSetSize(&pl->stats[i].cats[j], 0);
            }

            eaiSetSize(&pl->stats[i].cats[j], stat_GetStatCount());
        }
    }
}


/**********************************************************************func*
 * stat_Free
 *
 */
void stat_Free(EntPlayer *pl)
{
    int i, j;

    for(i=0; i<kStatPeriod_Count; i++)
    {
        for(j=0; j<kStat_Count; j++)
        {
            eaiDestroy(&pl->stats[i].cats[j]);
        }
    }
}

/**********************************************************************func*
 * stat_GetByIdx
 *
 */
int stat_GetByIdx(EntPlayer *pl, int per, int cat, int idx)
{
    if(idx<0 || per<0 || cat<0) return 0;
    return pl->stats[per].cats[cat][idx];
}

/**********************************************************************func*
 * stat_SetByIdx
 *
 */
void stat_SetByIdx(EntPlayer *pl, int per, int cat, int idx, int i)
{
    if(idx<0 || per<0 || cat<0) return;
    pl->stats[per].cats[cat][idx] = i;
}

/**********************************************************************func*
 * stat_IncByIdx
 *
 */
void stat_IncByIdx(EntPlayer *pl, int per, int cat, int idx)
{
    if(idx<0 || per<0 || cat<0) return;
    pl->stats[per].cats[cat][idx]++;
}

/**********************************************************************func*
 * stat_DecByIdx
 *
 */
void stat_DecByIdx(EntPlayer *pl, int per, int cat, int idx)
{
    if(idx<0 || per<0 || cat<0) return;
    pl->stats[per].cats[cat][idx]--;
}

/**********************************************************************func*
 * stat_AddByIdx
 *
 */
void stat_AddByIdx(EntPlayer *pl, int per, int cat, int idx, int i)
{
    if(idx<0 || per<0 || cat<0) return;
    pl->stats[per].cats[cat][idx] += i;
}

/**********************************************************************func*
 * stat_AddKill
 *
 */
void stat_AddKill(EntPlayer *pl, Entity *victim)
{
    if(pl==NULL)
        return;

    if(victim!=NULL)
    {
        if(ENTTYPE(victim)==ENTTYPE_PLAYER)
        {
            
            stat_Inc(pl, kStat_Kills, OnArenaMap()?"arena":"pvp");
        }
        else if(victim->villainDef!=NULL)
        {
            stat_Inc(pl, kStat_Kills, villainGroupGetName(victim->villainDef->group));

#ifdef STAT_TRACK_VILLAIN_TYPE
            stat_Inc(pl, kStat_Kills, victim->villainDef->name);
#endif
#ifdef STAT_TRACK_LEVEL
            stat_Inc(pl, kStat_Kills, stat_LevelStatName(victim->pchar->iLevel));
#endif
        }
    }

    stat_Inc(pl, kStat_Kills, stat_ZoneStatName());
    stat_Inc(pl, kStat_Kills, "overall");
}

/**********************************************************************func*
 * stat_AddDeath
 *
 */
void stat_AddDeath(EntPlayer *pl, Entity *victor)
{
    if(pl==NULL)
        return;

    if(victor!=NULL)
    {
        if(ENTTYPE(victor)==ENTTYPE_PLAYER)
        {
            stat_Inc(pl, kStat_Deaths, OnArenaMap()?"arena":"pvp");
        }
        else if(victor->villainDef!=NULL)
        {
            stat_Inc(pl, kStat_Deaths, villainGroupGetName(victor->villainDef->group));

#ifdef STAT_TRACK_VILLAIN_TYPE
            stat_Inc(pl, kStat_Deaths, victor->villainDef->name);
#endif
#ifdef STAT_TRACK_LEVEL
            stat_Inc(pl, kStat_Deaths, stat_LevelStatName(victor->pchar->iLevel));
#endif
        }
    }

    stat_Inc(pl, kStat_Deaths, stat_ZoneStatName());
    stat_Inc(pl, kStat_Deaths, "overall");
}

/**********************************************************************func*
 * stat_AddTimeInZone
 *
 */
void stat_AddTimeInZone(EntPlayer *pl, int secs)
{
    if(pl==NULL)
        return;

    stat_Add(pl, kStat_Time, stat_ZoneStatName(), secs);
    stat_Add(pl, kStat_Time, "overall", secs);
}

/**********************************************************************func*
 * stat_AddDamageGiven
 *
 */
void stat_AddDamageGiven(EntPlayer *pl, int damage)
{
    if(pl==NULL)
        return;

    if(damage>=0)
    {
        stat_Add(pl, kStat_General, "damage_given", damage);
    }
    else
    {
        stat_Add(pl, kStat_General, "healing_given", -damage);
    }
}

/**********************************************************************func*
 * stat_AddDamageTaken
 *
 */
void stat_AddDamageTaken(EntPlayer *pl, int damage)
{
    if(pl==NULL)
        return;

    if(damage>=0)
    {
        stat_Add(pl, kStat_General, "damage_taken", damage);
    }
    else
    {
        stat_Add(pl, kStat_General, "healing_taken", -damage);
    }
}

//RACERS
/**********************************************************************func*
 * stat_AddDamageTaken
 *
 */
void stat_AddRaceTime(EntPlayer *pl, int time, const char* racetype)
{
    int invertedTime;

    if(pl==NULL)
        return;

    if( !racetype )
        return;

    if( -1 == stat_GetStatIndex(racetype) )
    {
        Errorf( "Bad Race Time Name %s", racetype );
        return;
    }

    if(time>=0)
    {
        invertedTime = 0xffffffff - time;
        stat_SetIfGreater( pl, kStat_General, racetype, invertedTime );
    }
}

/**********************************************************************func*
 * stat_AddXPReceived
 *
 */
int stat_TimeSinceXpReceived(Entity *e)
{
    int timeNow = SecondsSince2000();
    int dt = (e->last_xp)?MAX(timeNow - e->last_xp, 0):((timeNow-e->on_since)+e->total_time);
    e->last_xp = timeNow;
    return dt;
}

void stat_AddXPReceived(Entity *e, int iXP)
{
    if(e==NULL)
        return;
    {
        stat_Add(e->pl, kStat_XP, stat_ZoneStatName(), iXP);

        if( g_ArchitectTaskForce )
            stat_Add(e->pl, kStat_ArchitectXP, stat_ZoneStatName(), iXP);
    }
}

/**********************************************************************func*
 * stat_AddWisdomReceived
 *
 */
void stat_AddWisdomReceived(EntPlayer *pl, int iWisdom)
{
    if(pl==NULL)
        return;

    stat_Add(pl, kStat_Wisdom, stat_ZoneStatName(), iWisdom);
    stat_Add(pl, kStat_Wisdom, "overall", iWisdom);
}

/**********************************************************************func*
 * stat_AddInfluenceReceived
 *
 */
void stat_AddInfluenceReceived(EntPlayer *pl, int iInf)
{
    if(pl==NULL)
        return;

    stat_Add(pl, kStat_Influence, stat_ZoneStatName(), iInf);
    stat_Add(pl, kStat_Influence, "overall", iInf);


    if( g_ArchitectTaskForce )
        stat_Add(pl, kStat_ArchitectInfluence, stat_ZoneStatName(), iInf);

}

/**********************************************************************func*
 * stat_Dump
 *
 */
void stat_Dump(ClientLink *client, Entity *e)
{
    int i, j, k;

    conPrintf(client, "%s\n", localizedPrintf(e,"CurrentStats", e->name));

    for(k=0; k<kStatPeriod_Count; k++)
    {
        for(j=0; j<kStat_Count; j++)
        {
            for(i=eaiSize(&e->pl->stats[k].cats[j])-1; i>=0; i--)
            {
                if(e->pl->stats[k].cats[j][i]!=0)
                {
                    if(stat_GetStatName(i)!=NULL)
                    {
                        conPrintf(client, "%s %8.8d %7.7s.%s\n",
                            stat_GetPeriodName(k),
                            e->pl->stats[k].cats[j][i],
                            stat_GetCategoryName(j),
                            stat_GetStatName(i));
                    }
                }
            }
        }
    }
}


/**********************************************************************func*
 * stat_UpdatePeriod
 *
 */
void stat_UpdatePeriod(Entity *e, U32 uLastUpdateTime)
{
    U32 uStartTodaySecs;
    struct tm now;
    int iStatCnt = stat_GetStatCount();

    timerMakeTimeStructFromSecondsSince2000(timerSecondsSince2000(), &now);
    now.tm_hour = 0;
    now.tm_min = 0;
    now.tm_sec = 0;
    uStartTodaySecs = timerGetSecondsSince2000FromTimeStruct(&now);

    if(uLastUpdateTime < uStartTodaySecs)
    {
        int i;
        int *tmp[kStat_Count];
        U32 uStartYesterdaySecs;
        U32 uStartMonthSecs;
        U32 uStartLastMonthSecs;

        uStartYesterdaySecs = uStartTodaySecs - 24*60*60;

        now.tm_mday = 1;
        uStartMonthSecs = timerGetSecondsSince2000FromTimeStruct(&now);

        now.tm_mon--;
        if(now.tm_mon<0)
        {
            now.tm_mon = 11;
            now.tm_year--;
        }
        uStartLastMonthSecs = timerGetSecondsSince2000FromTimeStruct(&now);

        if(uLastUpdateTime >= uStartYesterdaySecs)
        {
            // Today stats are for yesterday
            // Yesterday stats are garbage
            // Month and last month stats are good.

            // Move today to yesterday
            memcpy(tmp, e->pl->stats[kStatPeriod_Yesterday].cats, sizeof(tmp));
            memcpy(e->pl->stats[kStatPeriod_Yesterday].cats, e->pl->stats[kStatPeriod_Today].cats, sizeof(tmp));
            memcpy(e->pl->stats[kStatPeriod_Today].cats, tmp, sizeof(tmp));

            for(i=0; i<kStat_Count; i++)
            {
                eaiSetSize(&e->pl->stats[kStatPeriod_Today].cats[i], 0);
                eaiSetSize(&e->pl->stats[kStatPeriod_Today].cats[i], iStatCnt);
            }
        }
        else
        {
            // Reset yesterday
            for(i=0; i<kStat_Count; i++)
            {
                eaiSetSize(&e->pl->stats[kStatPeriod_Today].cats[i], 0);
                eaiSetSize(&e->pl->stats[kStatPeriod_Today].cats[i], iStatCnt);
                eaiSetSize(&e->pl->stats[kStatPeriod_Yesterday].cats[i], 0);
                eaiSetSize(&e->pl->stats[kStatPeriod_Yesterday].cats[i], iStatCnt);
            }
        }

        if(uLastUpdateTime < uStartMonthSecs)
        {
            if(uLastUpdateTime >= uStartLastMonthSecs)
            {
                // Month stats are for last month.
                // Last month stats are garbage

                // Move month to last month
                memcpy(tmp, e->pl->stats[kStatPeriod_LastMonth].cats, sizeof(tmp));
                memcpy(e->pl->stats[kStatPeriod_LastMonth].cats, e->pl->stats[kStatPeriod_ThisMonth].cats, sizeof(tmp));
                memcpy(e->pl->stats[kStatPeriod_ThisMonth].cats, tmp, sizeof(tmp));

                // Reset today, yesterday, and month
                for(i=0; i<kStat_Count; i++)
                {
                    eaiSetSize(&e->pl->stats[kStatPeriod_ThisMonth].cats[i], 0);
                    eaiSetSize(&e->pl->stats[kStatPeriod_ThisMonth].cats[i], iStatCnt);
                }
            }
            else
            {
                // Reset them all
                for(i=0; i<kStat_Count; i++)
                {
                    eaiSetSize(&e->pl->stats[kStatPeriod_ThisMonth].cats[i], 0);
                    eaiSetSize(&e->pl->stats[kStatPeriod_ThisMonth].cats[i], iStatCnt);
                    eaiSetSize(&e->pl->stats[kStatPeriod_LastMonth].cats[i], 0);
                    eaiSetSize(&e->pl->stats[kStatPeriod_LastMonth].cats[i], iStatCnt);
                }
            }
        }
    }
}

/* End of File */
