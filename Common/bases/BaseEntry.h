/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef BASEENTRY_H
#define BASEENTRY_H

#include <utilitieslib/stdtypes.h>
#include "entity/SgrpBasePermissions.h"

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct Supergroup Supergroup;

typedef enum BaseAccess
{
    kBaseAccess_None,
    kBaseAccess_Allowed,
    kBaseAccess_PermissionDenied,
    kBaseAccess_RentOwed,
    kBaseAccess_RaidScheduled,
    kBaseAccess_Count
} BaseAccess;

BaseAccess sgrp_BaseAccessFromSgrp(Supergroup *sg, SgrpBaseEntryPermission bep);
char *baseaccess_ToStr(BaseAccess s);

#endif //BASEENTRY_H
