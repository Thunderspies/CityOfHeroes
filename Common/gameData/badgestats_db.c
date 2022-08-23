#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/components/EString.h>
#include "player/badges.h"
#include "badgestats_db.h"
#include "container/dbcontainerpack.h"

// for the offset to the array
// I don't believe this is actually used, but structToLineTemplate could want it
#include "sgrpstatsstruct.h"
#ifndef SERVER
#define SERVER
#endif
#include "entity/entPlayer.h"

typedef struct badgestat_column_desc
{
    const char *attribute_file;
    const char *table;
    bool number_first_row;
    const char *column_prefix;
    bool pad;
    int column_start;
    int first_row_max; // should never be more than 999
    int column_max; // used to prevent column padding from going too high
    U32 array_offset;

} badgestat_column_desc;

badgestat_column_desc s_column_desc[BADGESTATSTYPE_MAXTYPES] =
{
    // BADGESTATSTYPE_ENTITY
    {    "server/db/templates/badgestats.attribute",
        "Badges",            false,    "c",    true,
        1,    861,    BADGE_ENT_MAX_STATS-1,
        offsetof(EntPlayer, aiBadgeStats)    },
    // BADGESTATSTYPE_SUPERGROUP
    {    "server/db/templates/supergroup_badgestats.attribute",
        "SgrpBadgeStats",    false,    "id",    false,
        0,    999,    BADGE_SG_MAX_STATS-1,
        offsetof(stat_SgrpStats,badgeStats.aiStats)    },
//    // BADGESTATSTYPE_DEFAULT
//    //    no longer used, but this is probably a good starting point for new entries
//    //    more '0' padding would probably be nice though
//    {    "",
//        "",                    true,    "stat",    true,
//        0,    999,    999    }
};

static char* badgestats_ColumnString_Internal(BadgeStatsType type, int idx, int table_idx)
{
    static char *estr = NULL;
    estrPrintCharString(&estr, s_column_desc[type].table);
    if(idx > s_column_desc[type].first_row_max || s_column_desc[type].number_first_row)
    {
        // normalize the index to a first row of 1000
        estrConcatf(&estr, "%02d", (idx - s_column_desc[type].first_row_max + 999)/1000);
    }
    estrConcatf(&estr, "[%d].%s", table_idx, s_column_desc[type].column_prefix);
    estrConcatf(&estr, (s_column_desc[type].pad ? "%03d" : "%d"), idx);
    return estr;
}

char* badgestats_ColumnString(BadgeStatsType type, int idx)
{
    return badgestats_ColumnString_Internal(type,idx,0);
}

void badgestats_Subtable(BadgeStatsType type, StuffBuff *psb)
{
    int i;
    LineDesc stats[] = {{ PACKTYPE_INT, SIZE_INT32, NULL, { 0, 0, "MadeBy", "badgestat_Subtable"}, }, {0}};
    int max_column = fileLineCount(s_column_desc[type].attribute_file);

    // pad out a few columns in case the dbserver is older than its clients (for devs)
    max_column += 16;
    if(max_column > s_column_desc[type].column_max)
        max_column = s_column_desc[type].column_max;

    for(i = s_column_desc[type].column_start; i <= max_column; i++)
    {
        stats[0].name = badgestats_ColumnString_Internal(type,i,1);
        stats[0].indirection[0].offset = s_column_desc[type].array_offset + i*sizeof(U32);
        structToLineTemplate(psb,0,stats,1);
    }
}

int badgestats_GetIndex(BadgeStatsType type, char *table, char *field)
{
    int len_table_desc = (int)strlen(s_column_desc[type].table); // Store these instead of calculating?
    int len_field_desc = (int)strlen(s_column_desc[type].column_prefix);

    if(!strnicmp(table,s_column_desc[type].table,len_table_desc) &&
       !strnicmp(field,s_column_desc[type].column_prefix,len_field_desc))
        return atoi(field+len_field_desc);

    return -1;
}