/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <assert.h>
#include <stdio.h>
#include <utilitieslib/utils/wininclude.h>
#include "sql/sqlinclude.h"

#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/timing.h>
#include "player/stats_base.h" // Sorry about this
#include "container_sql.h"
#include "dbrelay.h"      // For SendToServers
#include "clientcomm.h"   // For getAttribString
#include "dbserver/servercfg.h"    // server_cfg.no_stats

#define STAT_MAX_TOPS 10
//#define DUMP_TOPS // Define to dump HTML of initial tops at startup

typedef struct StatCols
{
    char *pchName;
    int iType;
    int iSize;
    int iCat;
    int iPeriod;
} StatCols;

typedef struct StatTops
{
    DBStatResult aTop[kStat_Count][kStatPeriod_Count];
} StatTops;

static StashTable s_hashTops = 0;
static U32 s_uLastUpdateTime = 0;

static StatCols cols[] =
{
#define STATS_LASTACTIVE_COL 0
     { "Ents.LastActive"    , SQL_C_TYPE_TIMESTAMP, 16, -1, -1 }, // MAKE SURE YOU SET STATS_LASTACTIVE_COL TO THIS INDEX
#define STATS_DBID_COL 1
     { "Stats.ContainerId"  , SQL_C_LONG,            4, -1, -1 }, // MAKE SURE YOU SET STATS_DBID_COL TO THIS INDEX
#define STATS_NAME_COL 2
     { "Stats.Name"         , SQL_C_LONG,            4, -1, -1 }, // MAKE SURE YOU SET STATS_NAME_COL TO THIS INDEX
#define STATS_HASH_BASE 3

    // The items above must be first in this array.

     { "General_Today"      , SQL_C_LONG,            4, kStat_General,   kStatPeriod_Today     },
     { "General_Yesterday"  , SQL_C_LONG,            4, kStat_General,   kStatPeriod_Yesterday },
     { "General_ThisMonth"  , SQL_C_LONG,            4, kStat_General,   kStatPeriod_ThisMonth },
     { "General_LastMonth"  , SQL_C_LONG,            4, kStat_General,   kStatPeriod_LastMonth },
     { "Kills_Today"        , SQL_C_LONG,            4, kStat_Kills,     kStatPeriod_Today     },
     { "Kills_Yesterday"    , SQL_C_LONG,            4, kStat_Kills,     kStatPeriod_Yesterday },
     { "Kills_ThisMonth"    , SQL_C_LONG,            4, kStat_Kills,     kStatPeriod_ThisMonth },
     { "Kills_LastMonth"    , SQL_C_LONG,            4, kStat_Kills,     kStatPeriod_LastMonth },
     { "Deaths_Today"       , SQL_C_LONG,            4, kStat_Deaths,    kStatPeriod_Today     },
     { "Deaths_Yesterday"   , SQL_C_LONG,            4, kStat_Deaths,    kStatPeriod_Yesterday },
     { "Deaths_ThisMonth"   , SQL_C_LONG,            4, kStat_Deaths,    kStatPeriod_ThisMonth },
     { "Deaths_LastMonth"   , SQL_C_LONG,            4, kStat_Deaths,    kStatPeriod_LastMonth },
     { "Time_Today"         , SQL_C_LONG,            4, kStat_Time,      kStatPeriod_Today     },
     { "Time_Yesterday"     , SQL_C_LONG,            4, kStat_Time,      kStatPeriod_Yesterday },
     { "Time_ThisMonth"     , SQL_C_LONG,            4, kStat_Time,      kStatPeriod_ThisMonth },
     { "Time_LastMonth"     , SQL_C_LONG,            4, kStat_Time,      kStatPeriod_LastMonth },
     { "XP_Today"           , SQL_C_LONG,            4, kStat_XP,        kStatPeriod_Today     },
     { "XP_Yesterday"       , SQL_C_LONG,            4, kStat_XP,        kStatPeriod_Yesterday },
     { "XP_ThisMonth"       , SQL_C_LONG,            4, kStat_XP,        kStatPeriod_ThisMonth },
     { "XP_LastMonth"       , SQL_C_LONG,            4, kStat_XP,        kStatPeriod_LastMonth },
     { "Influence_Today"    , SQL_C_LONG,            4, kStat_Influence, kStatPeriod_Today     },
     { "Influence_Yesterday", SQL_C_LONG,            4, kStat_Influence, kStatPeriod_Yesterday },
     { "Influence_ThisMonth", SQL_C_LONG,            4, kStat_Influence, kStatPeriod_ThisMonth },
     { "Influence_LastMonth", SQL_C_LONG,            4, kStat_Influence, kStatPeriod_LastMonth },
     { "Wisdom_Today"       , SQL_C_LONG,            4, kStat_Wisdom,    kStatPeriod_Today     },
     { "Wisdom_Yesterday"   , SQL_C_LONG,            4, kStat_Wisdom,    kStatPeriod_Yesterday },
     { "Wisdom_ThisMonth"   , SQL_C_LONG,            4, kStat_Wisdom,    kStatPeriod_ThisMonth },
     { "Wisdom_LastMonth"   , SQL_C_LONG,            4, kStat_Wisdom,    kStatPeriod_LastMonth },
     { "Architect_XP_Today"              , SQL_C_LONG,            4, kStat_ArchitectXP,        kStatPeriod_Today     },
     { "Architect_XP_Yesterday"       , SQL_C_LONG,            4, kStat_ArchitectXP,        kStatPeriod_Yesterday },
     { "Architect_XP_ThisMonth"       , SQL_C_LONG,            4, kStat_ArchitectXP,        kStatPeriod_ThisMonth },
     { "Architect_XP_LastMonth"       , SQL_C_LONG,            4, kStat_ArchitectXP,        kStatPeriod_LastMonth },
     { "Architect_Influence_Today"    , SQL_C_LONG,            4, kStat_ArchitectInfluence, kStatPeriod_Today     },
     { "Architect_Influence_Yesterday", SQL_C_LONG,            4, kStat_ArchitectInfluence, kStatPeriod_Yesterday },
     { "Architect_Influence_ThisMonth", SQL_C_LONG,            4, kStat_ArchitectInfluence, kStatPeriod_ThisMonth },
     { "Architect_Influence_LastMonth", SQL_C_LONG,            4, kStat_ArchitectInfluence, kStatPeriod_LastMonth },

};

/**********************************************************************func*
 * CalcStatColIdx
 *
 */
static _inline int CalcStatColIdx(char *pchName)
{
    // This is part of a really cheesy hand-made hash.
    // If you change the shape of the list above, you'll need to change this too.
    // The first character of the name and the second character following the
    //   undescore (_) are used to hash the string directly into an index.
    static int s_aiColHasher[] =
    {
        3, // a - LastMonth
        0, // b
        0, // c
        2, // d - Deaths
        1, // e - Yesterday
        0, // f
        0, // g - General
        2, // h - ThisMonth
        5, // i - Influence
        0, // j
        1, // k - Kills
        0, // l
        0, // m
        0, // n
        0, // o - Today
        0, // p
        0, // q
        0, // r
        0, // s
        3, // t - Time
        0, // u
        0, // v
        6, // w - Wisdom
        4, // x - XP
        0, // y
        0, // z
    };

    // The first character of the name and the second character following the
    //   undescore (_) are used to hash the string directly into an index.
    int iCat = pchName[0]-'A';
    char *pch = strchr(pchName, '_');
    if(pch)
    {
        int i;
        int iPer = pch[2]-'a';

        if(iCat>=0 && iCat<ARRAY_SIZE(s_aiColHasher) && iPer>=0 && iPer<ARRAY_SIZE(s_aiColHasher))
        {
            i = s_aiColHasher[iCat]*4+s_aiColHasher[iPer]+STATS_HASH_BASE;
            if (i<ARRAY_SIZE(cols))
                return i;
        }
    }
    return 0;
}

/**********************************************************************func*
 * GetStatColFromName
 *
 */
static _inline StatCols *GetStatColFromName(char *pchName)
{
    int iCol = CalcStatColIdx(pchName);

    if(stricmp(cols[iCol].pchName, pchName)==0)
    {
        return &cols[iCol];
    }
    else
    {
        // OK, the hash didn't work. Step through and try to find it by hand.
        //   This should never happen if the person who modified this code
        //   did everything they were supposed to.
        for(iCol=0; iCol<ARRAY_SIZE(cols); iCol++)
        {
            if(stricmp(cols[iCol].pchName, pchName)==0)
            {
                return &cols[iCol];
            }
        }
    }
    
    return NULL;
}

/**********************************************************************func*
 * FixupPeriod
 *
 */
static _inline int FixupPeriod(int iPer, U32 uNow, U32 uStartTodaySecs, 
    U32 uStartYesterdaySecs, U32 uStartThisMonthSecs,
    U32 uStartLastMonthSecs)
{
    if(uNow < uStartTodaySecs)
    {
        // If it's before today, then the "yesterday" data is useless.
        if(iPer==kStatPeriod_Yesterday)
            return -1;

        if(iPer==kStatPeriod_Today)
        {
            if(uNow >= uStartYesterdaySecs)
            {
                // Saved yesterday.
                // "Today" is actually yesterday
                return kStatPeriod_Yesterday;
            }
            else
            {
                // Older than yesterday
                // "Today" is too old
                return -1;
            }
        }

        // Only dealing with months from here on.

        if(uNow < uStartThisMonthSecs)
        {
            // If it's before this month, then "last month" is outdated.
            if(iPer==kStatPeriod_LastMonth)
                return -1;

            if(iPer==kStatPeriod_ThisMonth)
            {
                if(uNow >= uStartLastMonthSecs)
                {
                    // "This month" is actually "last month"
                    return kStatPeriod_LastMonth;
                }
                else
                {
                    // Older than last month
                    // "This month" is out of date.
                    return -1;
                }
            }
        }
    }

    return iPer;
}

/**********************************************************************func*
 * GetStartTimes
 *
 */
static void GetStartTimes(U32 uNow, U32 *puStartTodaySecs, U32 *puStartYesterdaySecs, 
    U32 *puStartMonthSecs, U32 *puStartLastMonthSecs)
{
    struct tm now;

    // See if the stats need to be rotated
    timerMakeTimeStructFromSecondsSince2000(uNow, &now);
    now.tm_hour = 0;
    now.tm_min = 0;
    now.tm_sec = 0;
    *puStartTodaySecs = timerGetSecondsSince2000FromTimeStruct(&now);

    *puStartYesterdaySecs = (*puStartTodaySecs) - 24*60*60;

    now.tm_mday = 1;
    *puStartMonthSecs = timerGetSecondsSince2000FromTimeStruct(&now);

    now.tm_mon--;
    if(now.tm_mon<0)
    {
        now.tm_mon = 11;
        now.tm_year--;
    }
    *puStartLastMonthSecs = timerGetSecondsSince2000FromTimeStruct(&now);
}

/**********************************************************************func*
 * TopsInit
 *
 */
static void TopsInit(void)
{
    if(s_hashTops==0)
    {
        s_hashTops = stashTableCreateInt(4);
    }
}

/**********************************************************************func*
 * GetTopsForAttrib
 *
 */
static StatTops *GetTopsForAttrib(int iName)
{
    StatTops *ptops;

    if(!stashIntFindPointer(s_hashTops, iName, &ptops))
    {
        // This top doesn't exist yet. Make it.
        ptops = calloc(sizeof(StatTops), 1);
        stashIntAddPointer(s_hashTops, iName, ptops, false);
    }

    return ptops;
}

/**********************************************************************func*
 * PackageStats
 *
 */
static void PackageStats(StuffBuff *psb)
{
    int i;

    // Stats are currently broadcast in a more-or-less universally
    // parsable form. If the size of these updates gets too large,
    // then it's pretty easy to strip out all the formatting junk and
    // write a special parser for it. (I estimate that it would be 75% of the
    // formatted size unless we went to a binary format.)

    StashTableIterator iter;
    StashElement elem;

    // The comnmand being sent to all the servers.

    stashGetIterator(s_hashTops, &iter);
    while(stashGetNextElement(&iter, &elem))
    {
        int col;

        StatTops *ptops = (StatTops *)stashElementGetPointer(elem);
        int iName = stashElementGetIntKey(elem);

        for(col=0; col<ARRAY_SIZE(cols); col++)
        {
            if(cols[col].iCat>=0)
            {
                DBStatResult *pres = &ptops->aTop[cols[col].iCat][cols[col].iPeriod];

                // eaiSize is safe to give NULL to (returns 0)
                addStringToStuffBuff(psb, "<set n=\"%s\" c=%d p=%d t=%d>",
                    getAttribString(iName), cols[col].iCat, cols[col].iPeriod,
                    eaiSize(&pres->pIDs));

                for(i=0; i<eaiSize(&pres->pIDs); i++)
                {
                    addStringToStuffBuff(psb, "<i i=%d e=%d v=%d>",
                        i, pres->pIDs[i], pres->piValues[i]);
                }
                addStringToStuffBuff(psb, "</>\n");
            }
        }
    }
}


/**********************************************************************func*
 * DbgDumpTops
 *
 */
static void DbgDumpTops(void)
{
#ifdef DUMP_TOPS
    int col;
    StashTableIterator iter;
    StashElement elem;
    FILE *fp = fopen("statagg.html", "w");

    fprintf(fp, "%d<br>", timerSecondsSince2000());

    fprintf(fp, "<table>");
    fprintf(fp, "<tr>");

    fprintf(fp, "<td><b>Name</b></td>");
    for(col=STATS_NAME_COL+1 ;col<ARRAY_SIZE(cols); col++)
    {
        fprintf(fp, "<td><b>%s</b></td>", cols[col].pchName);
    }
    fprintf(fp, "</tr>");


    stashGetIterator(s_hashTops, &iter);
    while((stashGetNextElement(&iter, &elem))!=NULL)
    {
        StatTops *ptops = (StatTops *)stashElementGetPointer(elem);

        fprintf(fp, "<tr>");

        fprintf(fp, "<td align=right>");
        fprintf(fp, "%s", getAttribString(stashElementGetIntKey(elem)));
        fprintf(fp, "</td>");

        for(col=0; col<ARRAY_SIZE(cols); col++)
        {
            if(cols[col].iCat>=0)
            {
                DBStatResult *pres = &ptops->aTop[cols[col].iCat][cols[col].iPeriod];

                fprintf(fp, "<td align=right>");
                // eaiSize is safe to give NULL to (returns 0)
                if(eaiSize(&pres->pIDs)==0)
                {
                    fprintf(fp, "<i>none</i>");
                }
                else
                {
                    int i;
                    for(i=0; i<eaiSize(&pres->pIDs); i++)
                    {
                        fprintf(fp, "%d - %d<br>", pres->pIDs[i], pres->piValues[i]);
                    }
                }
                fprintf(fp, "</td>");
            }
        }
        fprintf(fp, "</tr>\n");
    }

    fprintf(fp, "</table>");
    fprintf(fp, "</font>");

    fclose(fp);
#endif
}

/**********************************************************************func*
 * RotateStats
 *
 */
static void RotateStats(void)
{
    U32 uStartTodaySecs;
    U32 uStartYesterdaySecs;
    U32 uStartMonthSecs;
    U32 uStartLastMonthSecs;
    U32 uNow;

    // See if the stats need to be rotated
    uNow = timerSecondsSince2000();
    GetStartTimes(uNow, &uStartTodaySecs, &uStartYesterdaySecs, 
        &uStartMonthSecs, &uStartLastMonthSecs);

    if(s_uLastUpdateTime < uStartTodaySecs)
    {
        StashTableIterator iter;
        StashElement elem;

        stashGetIterator(s_hashTops, &iter);

        while (stashGetNextElement(&iter, &elem))
        {
            int iCat;
            StatTops *ptops = (StatTops *)stashElementGetPointer(elem);

            if(s_uLastUpdateTime >= uStartYesterdaySecs)
            {
                // Today stats are for yesterday
                // Yesterday stats are garbage
                // Month and last month stats are good.

                // Move today to yesterday
                for(iCat=0; iCat<kStat_Count; iCat++)
                {
                    DBStatResult resTmp = ptops->aTop[iCat][kStatPeriod_Yesterday];
                    ptops->aTop[iCat][kStatPeriod_Yesterday] = ptops->aTop[iCat][kStatPeriod_Today];
                    ptops->aTop[iCat][kStatPeriod_Today] = resTmp;

                    // eaiSetSize fails safely if NULL
                    eaiSetSize(&ptops->aTop[iCat][kStatPeriod_Today].piValues, 0);
                    eaiSetSize(&ptops->aTop[iCat][kStatPeriod_Today].pIDs, 0);
                }
            }
            else
            {
                // Reset yesterday and today
                // eaiSetSize fails safely if NULL
                for(iCat=0; iCat<kStat_Count; iCat++)
                {
                    eaiSetSize(&ptops->aTop[iCat][kStatPeriod_Today].piValues, 0);
                    eaiSetSize(&ptops->aTop[iCat][kStatPeriod_Today].pIDs, 0);
                    eaiSetSize(&ptops->aTop[iCat][kStatPeriod_Yesterday].piValues, 0);
                    eaiSetSize(&ptops->aTop[iCat][kStatPeriod_Yesterday].pIDs, 0);
                }
            }

            if(s_uLastUpdateTime < uStartMonthSecs)
            {
                if(s_uLastUpdateTime >= uStartLastMonthSecs)
                {
                    // Month stats are for last month.
                    // Last month stats are garbage

                    // Move month to last month
                    for(iCat=0; iCat<kStat_Count; iCat++)
                    {
                        DBStatResult resTmp = ptops->aTop[iCat][kStatPeriod_LastMonth];
                        ptops->aTop[iCat][kStatPeriod_LastMonth] = ptops->aTop[iCat][kStatPeriod_ThisMonth];
                        ptops->aTop[iCat][kStatPeriod_ThisMonth] = resTmp;

                        eaiSetSize(&ptops->aTop[iCat][kStatPeriod_ThisMonth].piValues, 0);
                        eaiSetSize(&ptops->aTop[iCat][kStatPeriod_ThisMonth].pIDs, 0);
                    }
                }
                else
                {
                    // Reset them all
                    for(iCat=0; iCat<kStat_Count; iCat++)
                    {
                        eaiSetSize(&ptops->aTop[iCat][kStatPeriod_ThisMonth].piValues, 0);
                        eaiSetSize(&ptops->aTop[iCat][kStatPeriod_ThisMonth].pIDs, 0);
                        eaiSetSize(&ptops->aTop[iCat][kStatPeriod_LastMonth].piValues, 0);
                        eaiSetSize(&ptops->aTop[iCat][kStatPeriod_LastMonth].pIDs, 0);
                    }
                }
            }
        }
    }

    s_uLastUpdateTime = uNow;
    DbgDumpTops();
}

/**********************************************************************func*
 * MergeIntoTops
 *
 */
static void MergeIntoTops(U32 uName, StatCategory eCat, StatPeriod ePer, int iDbid, int iVal)
{
    StatTops *ptops;
    int iCnt;
    int iIdx;
    int iIdxRem = -1;
    int i;

    assert(ePer>=0 && eCat>=0);

    // Don't bother with zeros.
    if(iVal<=0)
        return;

    ptops = GetTopsForAttrib(uName);

    if(!ptops->aTop[eCat][ePer].piValues)
    {
        eaiCreate(&ptops->aTop[eCat][ePer].pIDs);
        eaiSetCapacity(&ptops->aTop[eCat][ePer].pIDs, STAT_MAX_TOPS+1);

        eaiCreate(&ptops->aTop[eCat][ePer].piValues);
        eaiSetCapacity(&ptops->aTop[eCat][ePer].piValues, STAT_MAX_TOPS+1);
    }

    // Do a quick check to see if this value can even be in the top 10.
    iCnt = eaiSize(&ptops->aTop[eCat][ePer].piValues);
    if(iCnt==STAT_MAX_TOPS)
    {
        int iMin = ptops->aTop[eCat][ePer].piValues[iCnt-1];
        if(iVal<iMin)
            return;
    }

    // Find out where this value belongs.
    // Also find out if they're in the list already.
    iIdx = iCnt;
    for(i=0; i<iCnt; i++)
    {
        if(iDbid==ptops->aTop[eCat][ePer].pIDs[i])
        {
            if (iIdxRem >= 0)
            {
                // we have already found this id?  this means the name is in here more than once.
                // remove the newfound one
                eaiRemove(&ptops->aTop[eCat][ePer].pIDs, i);
                eaiRemove(&ptops->aTop[eCat][ePer].piValues, i);
                i--;
                iCnt--;
            }
            else
            {
                iIdxRem = i;
            }
        }

        // get only the lowest index (higher in the table) placing
        if(iIdx == iCnt && iVal>=ptops->aTop[eCat][ePer].piValues[i])
        {
            iIdx = i;
        }
    }

    // This person is already in the tops. Update their entry.
    if(iIdxRem>=0)
    {
        if(iIdxRem==iIdx)
        {
            // They're in the same spot, just update the value.
            ptops->aTop[eCat][ePer].piValues[iIdx] = iVal;
            return;
        }
        else
        {
            // They've moved. We'll remove the entry, the code further below
            //   will put a new one in the right place.
            eaiRemove(&ptops->aTop[eCat][ePer].pIDs, iIdxRem);
            eaiRemove(&ptops->aTop[eCat][ePer].piValues, iIdxRem);

            iCnt--;

            // iIdx should always be better than iIdxRem (and hence at
            //   a lower index), but better safe than sorry, right?
            if(iIdxRem < iIdx)
                iIdx--;
        }
    }

    if(iIdx<STAT_MAX_TOPS)
    {
        eaiInsert(&ptops->aTop[eCat][ePer].pIDs, iDbid, iIdx);
        eaiInsert(&ptops->aTop[eCat][ePer].piValues, iVal, iIdx);

        // Truncate off any extras
        if(iCnt==STAT_MAX_TOPS)
        {
            eaiSetSize(&ptops->aTop[eCat][ePer].pIDs, STAT_MAX_TOPS);
            eaiSetSize(&ptops->aTop[eCat][ePer].piValues, STAT_MAX_TOPS);
        }
    }
}

/**********************************************************************func*
 * stat_ReadTable
 *
 */
void stat_ReadTable(void)
{
    StuffBuff sb;
    HSTMT stmt;
    char *pchBuff;
    SQLLEN *plenIndicator;
    char pchLastMonth[80];
    U32 uStartTodaySecs;
    U32 uStartYesterdaySecs;
    U32 uStartThisMonthSecs;
    U32 uStartLastMonthSecs;

    int iLen;
    int idx;
    int retcode;
    int col;
    int count;

    char * nolock;

    TODO(); // Code cleanup needed to improve clarity for future audits

    loadstart_printf("Reading stats...");
    TopsInit();

    if(server_cfg.no_stats)
    {
        // Don't process stats
        return;
    }

    initStuffBuff(&sb,200);

    s_uLastUpdateTime = timerSecondsSince2000();
    GetStartTimes(s_uLastUpdateTime, &uStartTodaySecs, &uStartYesterdaySecs, 
        &uStartThisMonthSecs, &uStartLastMonthSecs);

    // A buffer ODBC wants.
    plenIndicator = calloc(ARRAY_SIZE(cols), sizeof(SQLLEN));

    // Make up the SQL statement to collect all the rows we care about.
    // Also figure out how many bytes we need to store a row.
    addStringToStuffBuff(&sb, "SELECT ");
    for(iLen=0, col=0 ;col<ARRAY_SIZE(cols); col++)
    {
        if(col>0)
        {
            addStringToStuffBuff(&sb, ", ");
        }
        addStringToStuffBuff(&sb, "%s", cols[col].pchName);

        iLen += cols[col].iSize;
    }

    switch (gDatabaseProvider) {
        xcase DBPROV_MSSQL:
            nolock = "WITH (NOLOCK)";
        xdefault:
            nolock = "";
    };

    addStringToStuffBuff(&sb, " FROM dbo.Stats INNER JOIN dbo.Ents %s ON Stats.ContainerID=Ents.ContainerID WHERE Ents.LastActive>='%s';",
        nolock, timerMakeDateStringFromSecondsSince2000(pchLastMonth, uStartLastMonthSecs));        

    // Allocate the row buffer.
    pchBuff = calloc(iLen, 1);

    stmt = sqlConnStmtAlloc(SQLCONN_FOREGROUND);

    // Bind all the columns to their spot in the row buffer.
    for(idx=0, col=0; col<ARRAY_SIZE(cols); col++)
    {
        retcode = SQLBindCol(stmt, col+1,
            cols[col].iType,
            pchBuff+idx,
            cols[col].iSize,
            plenIndicator+col);

        idx+=cols[col].iSize;
    }

    retcode = sqlConnStmtExecDirect(stmt, sb.buff, SQL_NTS, SQLCONN_FOREGROUND, false);
    if(!SQL_SUCCEEDED(retcode))
    {
        freeStuffBuff(&sb);
        return;
    }

    for(count=0;;count++)
    {
        int iName = -1;
        int iDbid = -1;
        int uLastActive = 0;

        memset(pchBuff, 0, iLen);
        retcode = _sqlConnStmtFetch(stmt, SQLCONN_FOREGROUND);
        if (SQL_SHOW_ERROR(retcode))
            sqlConnStmtPrintError(stmt, sb.buff);

        if (!SQL_SUCCEEDED(retcode))
            break;

        for(idx=0, col=0 ;col<ARRAY_SIZE(cols); col++)
        {
            if(col==STATS_NAME_COL)
            {
                iName = *(int *)(pchBuff+idx);
            }
            else if(col==STATS_DBID_COL)
            {
                iDbid = *(int *)(pchBuff+idx);
            }
            else if(col==STATS_LASTACTIVE_COL)
            {
                uLastActive = timerGetSecondsSince2000FromSQLTimestamp((SQL_TIMESTAMP_STRUCT *)(pchBuff+idx));
            }
            else if(iName>0 && cols[col].iCat>=0)
            {
                int iVal = *(int *)(pchBuff+idx);
                if(plenIndicator[col]>=0 && iVal>0)
                {
                    int iPer = FixupPeriod(cols[col].iPeriod, uLastActive, uStartTodaySecs, uStartYesterdaySecs, uStartThisMonthSecs, uStartLastMonthSecs);
                    if(iPer>=0)
                        MergeIntoTops(iName, cols[col].iCat, iPer, iDbid, iVal-1);
                }
            }

            idx += cols[col].iSize;
        }

    }

    sqlConnStmtFree(stmt);

    free(pchBuff);
    free(plenIndicator);
    freeStuffBuff(&sb);

    DbgDumpTops();
    loadend_printf("");
}


/**********************************************************************func*
 * stat_UpdateStatsForEnt
 *
 */
void stat_UpdateStatsForEnt(int dbid, char *pchEnt)
{
    extern AttributeList *var_attrs;

    char pch[1024];
    char *str;
    int iIdx = -1;
    int iName = -1;

    if(server_cfg.no_stats)
    {
        // Don't process stats
        return;
    }

    if((str=strstri(pchEnt, "Stats["))==NULL)
    {
        // No Stats to process.
        return;
    }

    for(;;)
    {
        if (!str[0])
            break;

        if(strnicmp(str, "Stats[", 6)==0)
        {
            char *s;
            char *e;
            int i = atoi(str+6);

            // Starting a new stat, reset everything.
            if(i>iIdx)
            {
                iIdx = i;
                iName = -1;
            }

            // Determine which column in the stat
            s = strchr(str,'.');
            if (!s)
                break;
            s++;

            // Name is special since it's used for all the stats of this
            // index. It should (and MUST) appear as the first column
            // for this index.
            if(strnicmp(s, "Name ", 5)==0)
            {
                s = strchr(s,'"');
                if (!s)
                    break;
                s++;
                e = strchr(s,'"');
                if(!e)
                    break;
                strncpy(pch, s, e-s);
                pch[e-s]=0;
                stashFindInt(var_attrs->hash_table, pch, &iName);
            }
            else
            {
                if(iName > 0 && iIdx>=0)
                {
                    int val;

                    e = strchr(s,' ');
                    if (!e)
                        break;
                    strncpy(pch, s, e-s);
                    pch[e-s]=0;

                    e++;
                    val = atoi(e);

                    if(val>1)
                    {
                        StatCols *pcol = GetStatColFromName(pch);
                        if(pcol && pcol->iCat>=0)
                        {
                            MergeIntoTops(iName, pcol->iCat, pcol->iPeriod, dbid, val-1);
                        }
                    }
                }
            }
        }

        str = strchr(str,'\n');
        if(!str)
            break;

        str++;
    }
}

/**********************************************************************func*
 * stat_Update
 *
 */
void stat_Update(void)
{
    static int timer=0;

    if(server_cfg.no_stats)
    {
        // Don't process stats
        return;
    }

    if(timer==0)
    {
        timer = timerAlloc();
    }
    else
    {
        if(timerElapsed(timer) < 10)
        {
            return;
        }
    }
    timerStart(timer);

    RotateStats();
}

/* End of File */
