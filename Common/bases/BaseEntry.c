/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "BaseEntry.h"
#include "entity/SgrpBasePermissions.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/StashTable.h>
#include "entity/Supergroup.h"

BaseAccess sgrp_BaseAccessFromSgrp(Supergroup *sg, SgrpBaseEntryPermission bep)
{
    BaseAccess res = kBaseAccess_None;
	if (sg)
    {
        if( sg->entryPermission & (1<<bep) )
        {
            res = kBaseAccess_Allowed;
        }
        else
        {
            res = kBaseAccess_PermissionDenied;
        } 
    }
    return res;
}

char *baseaccess_ToStr(BaseAccess s)
{
    char *strs[] = {
        "kBaseAccess_None",
        "kBaseAccess_Allowed",
        "kBaseAccess_PermissionDenied",
        "kBaseAccess_RentOwed",
        "kBaseAccess_RaidScheduled",
        "kBaseAccess_Count",
    };
    return AINRANGE( s, strs ) ? strs[s] : "invalid enum";
}
