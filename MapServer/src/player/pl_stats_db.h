/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef PL_STATS_DB_H__
#define PL_STATS_DB_H__

#include <utilitieslib/stdtypes.h>

#ifndef STATS_BASE_H__
#include "stats_base.h"
#endif

typedef struct Entity Entity;
typedef struct StuffBuff StuffBuff;
typedef struct StructDesc StructDesc;

#define MAX_DB_STATS 1024

typedef struct DBStat
{
    const char *pchItem;
    int iValues[kStat_Count][kStatPeriod_Count];
} DBStat;

typedef struct DBStats
{
    DBStat stats[MAX_DB_STATS];
} DBStats;

void packageEntStats(Entity *e, StuffBuff *psb, StructDesc *desc, U32 uLastUpdateTime);
void unpackEntStats(Entity *e, DBStats *pdbstats);
void StuffStatNames(StuffBuff *psb);
    // These are used by containerloadsave for database storage.


#endif /* #ifndef PL_STATS_DB_H__ */

/* End of File */

