/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <stdio.h>

#include "entity/entity.h"
#include "entity/entserver.h" // for entGetName
#include "entity/powers.h"
#include "gameComm/VillainDef.h"
#include "generator/encounterPrivate.h"
#include <utilitieslib/utils/strings_opt.h>


/**********************************************************************func*
 * dbg_BasePowerStr
 *
 */
char *dbg_BasePowerStr(const BasePower *ppow)
{
    static char s_ach[5][256];
    static int iIdx = 0;
    char *pch;

    if(!ppow)
        return "";

    iIdx++;
    if(iIdx>=5)
        iIdx=0;

    switch(ppow->eType)
    {
        case kPowerType_Auto:
            pch = "Auto";
            break;
        case kPowerType_Boost:
            pch = "Boost";
            break;
        case kPowerType_Click:
            pch = "Click";
            break;
        case kPowerType_Inspiration:
            pch = "Inspiration";
            break;
        case kPowerType_Toggle:
            pch = "Toggle";
            break;
        default:
            pch = "Invalid";
            break;
    }

//    sprintf(s_ach, "%s %s.%s.%s", pch, // JE: sprintf is slow, I hates it
//        ppow->psetParent->pcatParent->pchName,
//        ppow->psetParent->pchName,
//        ppow->pchName);
    STR_COMBINE_BEGIN(s_ach[iIdx])
        STR_COMBINE_CAT(pch);
        STR_COMBINE_CAT_C(' ');
        STR_COMBINE_CAT(ppow->pchFullName);
    STR_COMBINE_END();

    return s_ach[iIdx];
}

/**********************************************************************func*
 * dbg_LocationStr
 *
 */
char *dbg_LocationStr(const Vec3 vec)
{
    static char s_ach[256];
    sprintf(s_ach, "(%.3f, %.3f, %.3f)", vec[0], vec[1], vec[2]);

    return s_ach;
}

/**********************************************************************func*
 * dbg_FillNameStr
 *
 */
char *dbg_FillNameStr(char *pchDest, size_t iDestSize, const Entity *e)
{
    const char *pch = entGetName(e);

    if(!pch)
    {
        pch = "(no name)";
    }

    if(e && ENTTYPE(e)==ENTTYPE_CRITTER)
    {
        //sprintf(pchDest, "%s:%u",pch, e->id); // JE: sprintf is slow, I hates it
        STR_COMBINE_BEGIN_S(pchDest, iDestSize)
            STR_COMBINE_CAT(pch);
            STR_COMBINE_CAT_C(':');
            STR_COMBINE_CAT_D(e->id);
        STR_COMBINE_END();
    }
    else
    {
        STR_COMBINE_BEGIN_S(pchDest, iDestSize)
            STR_COMBINE_CAT(pch);
            if(e)
            {
                STR_COMBINE_CAT_C(':');
                STR_COMBINE_CAT(e->auth_name);
            }
        STR_COMBINE_END();
    }

    return pchDest;
}

/**********************************************************************func*
* dbg_NameStr
*
*/
char *dbg_NameStr(const Entity *e)
{
    static char s_ach[256];
    return dbg_FillNameStr(SAFESTR(s_ach), e);
}

char *dbg_EncounterGroupStr(const EncounterGroup * group)
{
    static char s_ach[256];
    sprintf(s_ach, "Group %s (%0.2f, %0.2f, %0.2f)", group->groupname, group->mat[3][0], group->mat[3][1], group->mat[3][2]);
    return s_ach;
}



/* End of File */
