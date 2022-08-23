/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef SGRPSTATSSTRUCT_H
#define SGRPSTATSSTRUCT_H

#include <utilitieslib/stdtypes.h>
#include "container/container_store.h"
#include "entity/Supergroup.h"

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;

typedef struct stat_SgrpStats
{
    int db_id;
    int id;                // the container ID: CONTAINER_SGRPSTATS
     char name[SG_TITLE_LEN];
    int idSgrp;
    SgrpBadgeStats badgeStats; // only ever one in earray

    // ----------
    // not persisted
    bool dirty;
} stat_SgrpStats;

stat_SgrpStats* stat_SgrpStats_Create  (int id, int idSgrp, char *name);
void            stat_SgrpStats_Destroy (stat_SgrpStats *pStats);
void            stat_SgrpStats_Unpack  (stat_SgrpStats *pStats, char *buff);
char*            stat_SgrpStats_Package (stat_SgrpStats *pStats);
char*            stat_SgrpStats_Template();

#endif //SGRPSTATSSTRUCT_H
