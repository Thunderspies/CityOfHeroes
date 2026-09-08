/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "entity/SgrpBasePermissions.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/StashTable.h>


bool sgrpbaseentrypermission_Valid( SgrpBaseEntryPermission perm )
{
    int max = (1<<kSgrpBaseEntryPermission_Count) - 1;
    return (perm >= 0 && perm <= max);
}

//----------------------------------------
//  Get the menu message for the base entry permissions
//----------------------------------------
char *sgrpbaseentrypermission_ToMenuMsg(SgrpBaseEntryPermission e )
{
    static char *s_strs[] = 
        {
            "SgBaseEntryPermissionNone",
            "SgBaseEntryPermissionCoalition",
            "SgBaseEntryPermissionLeaderTeammates",
        };
    STATIC_INFUNC_ASSERT( ARRAY_SIZE( s_strs ) == kSgrpBaseEntryPermission_Count );
    return AINRANGE( e, s_strs ) ? s_strs[e] : NULL;
}
