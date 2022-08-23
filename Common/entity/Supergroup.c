/***************************************************************************
 *     Copyright (c) 2005-2005, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include <utilitieslib/utils/eval.h>
#include "entity/Supergroup.h"
#include "entity/RewardToken.h"
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/earray.h>
#include "entity/teamup.h"
#include <utilitieslib/components/bitfield.h>
#include <utilitieslib/components/StashTable.h>
#include "bases/basedata.h"

#if SERVER || STATSERVER
#include "dbcomm/dbcomm.h"
#include "comm_backend.h"
#include "storyarc/storyinfo.h"
#include "container/containerSupergroup.h"
#endif

#include <utilitieslib/utils/timing.h>

sgroupPermissionName sgroupPermissions[] =
{    
    {SG_PERM_ALLIANCECHAT,    "AllianceChatStr",1},
    {SG_PERM_DESCRIPTION,    "DescriptStr",    1},
    {SG_PERM_MOTTO,            "MottoStr",        1},
    {SG_PERM_PROMOTE,        "PromoteStr",    1},
    {SG_PERM_DEMOTE,        "DemoteStr",    1},
    {SG_PERM_KICK,            "KickStr",        1},
    {SG_PERM_INVITE,        "InviteStr",    1},
    {SG_PERM_TITHING,        "TithingStr",    0},
    {SG_PERM_RAID_SCHEDULE,    "RaidScheduleStr",1},
    {SG_PERM_MOTD,            "MOTDStr",        1},
    {SG_PERM_ALLIANCE,        "AllianceStr",    1},
    {SG_PERM_AUTODEMOTE,    "AutoDemoteStr",1},
    {SG_PERM_BASE_EDIT,        "BaseArchStr",    1},
    {SG_PERM_RANKS,            "RanksStr",        1},
    {SG_PERM_PAYRENT,        "PermissionPayRentStr",        1},
    {SG_PERM_GET_SGMISSION,    "SGMissionStr",    0},
    {SG_PERM_CAN_SET_BASEENTRY_PERMISSION, "SGPermissionCanSetBaseEntryPermissions", 1},
    {SG_PERM_ARENA,            "SGPermArenaStr",1},
    {SG_PERM_COSTUME,        "SGEditCostume", 1},
    {SG_PERM_BASESTORAGE_ALLOWED_TO_TAKE_FROM,    "SgPermissionBaseStorageCanTakeFrom", 0},
    {SG_PERM_BASESTORAGE_ALLOWED_TO_ADD_TO,        "SgPermissionBaseStorageCanAddTo", 0},
    {SG_PERM_RAID_POINTS_ALLOWED_TO_SPEND,        "SgPermissionAllowedToSpendRaidPoints", 1},
};
STATIC_ASSERT(ARRAY_SIZE(sgroupPermissions) == SG_PERM_COUNT);

// initialized in sgroup_hasPermission().
U32 s_darksiderSupergroupPermissionMask[(SG_PERM_COUNT+31) / 32];

void sgroup_verifyPermissionTable()
{
    int i;
    
    for(i = 0; i < SG_PERM_COUNT; i++)
    {
        devassertmsg(sgroupPermissions[i].permission == i, "sgroupPermissions must be in the same order as the enum");
    }
}

int sgroup_hasPermissionByName( Entity* e, const char *permission)
{
    int i;

    for(i = 0; i < SG_PERM_COUNT; i++)
    {
        if (stricmp(permission, sgroupPermissions[i].name) == 0)
            return sgroup_hasPermission(e, i);
    }
    return false;
}

int sgroup_hasPermission( Entity* e, sgroupPermissionEnum permission)
{
    int i;
    Supergroup* sg;
    int returnMe = 0;
    static int darksiderInited = false;

    // Added NULL check on e->supergroup.  This should never happen in normal use, but might happen immediately
    // after a mapmove
    if(!e || !e->supergroup_id || e->supergroup == NULL )
        return 0;

    sg = e->supergroup;

    if (e->access_level)
        return 1; //Access level can always do everything

    devassertmsg(permission == SG_PERM_ANYONE || sgroupPermissions[permission].used, "Trying to check a non-used supergroup permission: %s", sgroupPermissions[permission].name);

    if(permission == SG_PERM_ANYONE)
        return 1;

    for (i = eaSize(&sg->memberranks)-1; i >= 0; --i)
    {
        if (sg->memberranks[i]->dbid == e->db_id)
        {
            if (sg->memberranks[i]->rank >= NUM_SG_RANKS - 2)
            {
                returnMe = 1;
            }
            else
            {
                returnMe = TSTB(sg->rankList[sg->memberranks[i]->rank].permissionInt, permission );
            }
            break;
        }
    }

    if (returnMe && sgroup_isPlayerDark(e))
    {
        if (!darksiderInited)
        {
            SETB(s_darksiderSupergroupPermissionMask, SG_PERM_ALLIANCECHAT);
            SETB(s_darksiderSupergroupPermissionMask, SG_PERM_PAYRENT);

            darksiderInited = true;
        }

        returnMe = TSTB(s_darksiderSupergroupPermissionMask, permission);
    }

    return returnMe;
}

int sgroup_hasStoragePermission( Entity* e, U32 permissions, int amount)
{
    int i;
    int rank = 0;
    Supergroup* sg;

    // Added NULL check on e->supergroup.  This should never happen in normal use, but might happen immediately
    // after a mapmove
    if (!e || !e->supergroup_id || e->supergroup == NULL)
        return 0;

    sg = e->supergroup;

    if (e->access_level)
    {
        return 1; //Access level can always do everything
    }

    // Find my rank ...
    for(i = eaSize(&sg->memberranks) - 1; i >= 0; --i)
    {
        if(sg->memberranks[i]->dbid == e->db_id)
        {
            rank = sg->memberranks[i]->rank;
            break;
        }
    }
    // Sanity check ...
    if (rank < 0)
    {
        rank = 0;
    }
    else if (rank >= NUM_SG_RANKS_WITH_VARIABLE_PERMISSIONS)
    {
        return 1;    // supreme leader and above can always do it
    }

    // Shift the appropriate permission bit to the correct spot
    permissions >>= rank;
    if (amount < 0)
    {
        permissions >>= 16;    // "Take" permissions are in the upper 16 bit word ...
    }
    return permissions & 1;
}

int sgroup_rankHasPermission( Entity* e, int rank, sgroupPermissionEnum permission)
{
    // Added NULL check on e->supergroup.  This should never happen in normal use, but might happen immediately
    // after a mapmove
    if(!e || !e->supergroup || e->supergroup == NULL)
        return 0;

    if(permission == SG_PERM_ANYONE)
        return 1;

    return TSTB(e->supergroup->rankList[rank].permissionInt, permission);
}

bool sgrpbadge_GetIdxFromName(const char *pch, int *pi)
{
#ifdef RAIDSERVER
    return false;
#else
    BadgeDef *pdef;
    if(stashFindInt(g_SgroupBadges.hashByName, pch, (int *)&pdef))
    {
        if( pi )
        {
            *pi = pdef->iIdx;
        }
        return true;
    }

    return false;
#endif
}

void sgroup_setDefaultPermissions(Supergroup* sg)
{
    int i;

    // Member thru Leader
      for( i = 0; i < NUM_SG_RANKS_WITH_VARIABLE_PERMISSIONS; i++ )
    {
        memset(sg->rankList[i].permissionInt, 0, sizeof(sg->rankList[i].permissionInt));
    }
    // SuperLeader & Hidden GM
    for (i = NUM_SG_RANKS_WITH_VARIABLE_PERMISSIONS; i < NUM_SG_RANKS; i++)
    {
        memset(sg->rankList[i].permissionInt, 0xff, sizeof(sg->rankList[i].permissionInt));
    }

    for(i = 0; i < NUM_SG_RANKS_WITH_VARIABLE_PERMISSIONS; i++)
    {
        SETB(sg->rankList[i].permissionInt, SG_PERM_ALLIANCECHAT);
        //SETB(sg->rankList[i].permissionInt, SG_PERM_BASESTORAGE_ALLOWED_TO_ADD_TO);
        //SETB(sg->rankList[i].permissionInt, SG_PERM_BASESTORAGE_ALLOWED_TO_TAKE_FROM);
    }
    for(i = 1; i < NUM_SG_RANKS_WITH_VARIABLE_PERMISSIONS; i++)
    {
        SETB(sg->rankList[i].permissionInt, SG_PERM_MOTTO);
        SETB(sg->rankList[i].permissionInt, SG_PERM_DESCRIPTION);
    }
    for(i = 2; i < NUM_SG_RANKS_WITH_VARIABLE_PERMISSIONS; i++)
    {
        SETB(sg->rankList[i].permissionInt, SG_PERM_PROMOTE);
        SETB(sg->rankList[i].permissionInt, SG_PERM_DEMOTE);
        SETB(sg->rankList[i].permissionInt, SG_PERM_KICK);
        SETB(sg->rankList[i].permissionInt, SG_PERM_INVITE);
    }
    for(i = 3; i < NUM_SG_RANKS_WITH_VARIABLE_PERMISSIONS; i++)
    {
        SETB(sg->rankList[i].permissionInt, SG_PERM_RAID_SCHEDULE);
//        SETB(sg->rankList[i].permissionInt, SG_PERM_TITHING);
        SETB(sg->rankList[i].permissionInt, SG_PERM_MOTD);
        SETB(sg->rankList[i].permissionInt, SG_PERM_COSTUME);
        SETB(sg->rankList[i].permissionInt, SG_PERM_RAID_POINTS_ALLOWED_TO_SPEND);
    }
}

MP_DEFINE(Supergroup);

// @rev ab: moved here for statserver reasons
Supergroup *createSupergroup(void)
{
    Supergroup * sg;
    MP_CREATE(Supergroup, 4);
    sg = MP_ALLOC(Supergroup);
    memset(sg, 0, sizeof(Supergroup));
    return sg;
}

void destroySupergroup(Supergroup *supergroup)
{
    if (!supergroup)
        return;
    clearSupergroup(supergroup);
    MP_FREE(Supergroup, supergroup);
}

void clearSupergroup(Supergroup *supergroup)
{
    if (!supergroup)
        return;
    destroyTeamMembersContents(&supergroup->members);
    eaDestroyEx(&supergroup->memberranks, destroySupergroupMemberInfo);
    eaDestroyEx(&supergroup->rewardTokens, rewardtoken_Destroy);

#if SERVER || STATSERVER
    eaDestroyEx(&supergroup->specialDetails, DestroySpecialDetail);
    if (supergroup->activetask) storyTaskInfoDestroy(supergroup->activetask);
#endif

#if GAME
    TaskStatusDestroy(supergroup->sgMission);
    compass_ClearAllMatchingId(SG_LOCATION);
#endif

    memset( supergroup, 0, sizeof(Supergroup) );
}

MP_DEFINE(SupergroupStats);
SupergroupStats *createSupergroupStats(void)
{
    MP_CREATE(SupergroupStats,4);
    return MP_ALLOC(SupergroupStats);
}
void destroySupergroupStats(SupergroupStats *stats)
{
    MP_FREE(SupergroupStats, stats);
}

MP_DEFINE(SupergroupMemberInfo);
SupergroupMemberInfo* createSupergroupMemberInfo(void)
{
    MP_CREATE(SupergroupMemberInfo, 10);
    return MP_ALLOC(SupergroupMemberInfo);
}

void destroySupergroupMemberInfo(SupergroupMemberInfo* member)
{
    MP_FREE(SupergroupMemberInfo, member);
}

//----------------------------------------
//  RecipeInvItems
//----------------------------------------


MP_DEFINE(RecipeInvItem);

/**********************************************************************func*
 * CreateRecipeInvItem
 *
 */
RecipeInvItem *CreateRecipeInvItem(void)
{
    MP_CREATE(RecipeInvItem, 64);
    return MP_ALLOC(RecipeInvItem);
}

/**********************************************************************func*
 * DestroyRecipeInvItem
 *
 */
void DestroyRecipeInvItem(RecipeInvItem *p)
{
    MP_FREE(RecipeInvItem, p);
}

/**********************************************************************func*
 * AddRecipeInvItem
 *
 */
static RecipeInvItem *AddRecipeInvItem(Supergroup *supergroup, DetailRecipe const *pDetailRecipe, int iCount)
{
    int i;
    RecipeInvItem *pNew = NULL;
    int iSize = eaSize(&supergroup->invRecipes);

    for(i=0; i<iSize; i++)
    {
        if(supergroup->invRecipes[i] == NULL)
        {
            pNew = supergroup->invRecipes[i] = CreateRecipeInvItem();
            break;
        }
        else if(supergroup->invRecipes[i]->recipe == NULL)
        {
            pNew = supergroup->invRecipes[i];
            break;
        }
    }
    if(i==iSize)
    {
        pNew = CreateRecipeInvItem();
        eaPush(&supergroup->invRecipes, pNew);
    }

    pNew->recipe = pDetailRecipe;
    pNew->count = iCount;

    return pNew;
}

/**********************************************************************func*
 * RemoveRecipeInvItem
 *
 */
static void RemoveRecipeInvItem(Supergroup *supergroup, DetailRecipe const *pDetailRecipe)
{
    int i;
    for(i=0; i<eaSize(&supergroup->invRecipes); i++)
    {
        if(supergroup->invRecipes[i] != NULL
            && supergroup->invRecipes[i]->recipe == pDetailRecipe)
        {
            DestroyRecipeInvItem(supergroup->invRecipes[i]);
            supergroup->invRecipes[i] = NULL;
        }
    }
}

/**********************************************************************func*
 * sgrp_FindRecipeInvItem
 *
 */
RecipeInvItem *sgrp_FindRecipeInvItem(Supergroup *sg, DetailRecipe const *recipe)
{
    int i;
    for(i=eaSize(&sg->invRecipes)-1; i>=0; i--)
    {
        if(sg->invRecipes[i]->recipe == recipe)
            return sg->invRecipes[i];
    }

    return NULL;
}


/**********************************************************************func*
 * sgrp_SetRecipeNoLock
 *
 * Forcibly sets the recipe inventory item to the given values. This can
 * be used to re-limit an unlimited recipe, for example, or set the number
 * of recipes to an exact value.
 *
 * If the supergroup doesn't yet have the recipe, it is added.
 *
 */
void sgrp_SetRecipeNoLock(Supergroup *sg, DetailRecipe const *recipe, int count, bool isUnlimited)
{
    RecipeInvItem *item = sgrp_FindRecipeInvItem(sg, recipe);
    if(item)
    {
        if(isUnlimited)
            item->count = -1;
        else
        {
            item->count = count;
            if(item->count <= 0)
            {
                RemoveRecipeInvItem(sg, recipe);
            }
        }
    }
    else
    {
        if(count>0)
            AddRecipeInvItem(sg, recipe, isUnlimited ? -1 : count);
    }
}

/**********************************************************************func*
 * sgrp_AdjRecipeNoLock
 *
 * Modifies an existing recipe inventory item (or adds one if it doesn't
 * exist yet). A negative number can be given as the count to decrease the
 * number of those recipes a supergroup has. If the number drops to zero,
 * it is removed from the list of recipes the SG has.
 *
 * If the adjustment sets isUnlimited to true, then the recipe is set to
 * never run out. 
 *
 * Adding or deducting from an unlimited recipe has no effect. If a recipe is 
 * unlimited, setting isUnlimited to false has no effect. Making an
 * unlimited recipe limited again can be done with sgrp_SetRecipeNoLock.
 *
 */
void sgrp_AdjRecipeNoLock(Supergroup *sg, DetailRecipe const *recipe, int count, bool isUnlimited)
{
    RecipeInvItem *item = sgrp_FindRecipeInvItem(sg, recipe);
    if(item)
    {
        if(item->count != -1)
        {
            if(isUnlimited)
                item->count = -1;
            else
            {
                item->count += count;
                if(item->count <= 0)
                {
                    RemoveRecipeInvItem(sg, recipe);
                }
            }
        }
    }
    else
    {
        if(count>0)
            AddRecipeInvItem(sg, recipe, isUnlimited ? -1 : count);
    }
}

// *********************************************************************************
// loaded sgrps
// *********************************************************************************

#define SECS_CACHEDSG_VALID (15*60)

typedef struct CachedSg
{    
    Supergroup *sg;
    U32 secsLastUpdated;
} CachedSg;

static void cachedsg_Update( CachedSg *csg, Supergroup *sg );

MP_DEFINE(CachedSg);
static CachedSg* cachedsg_Create( Supergroup *sg )
{
    CachedSg *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(CachedSg, 64);
    res = MP_ALLOC( CachedSg );
    if( verify( res ))
    {
        cachedsg_Update(res,sg);
    }
    // --------------------
    // finally, return

    return res;
}

static void cachedsg_Destroy( CachedSg *hItem )
{
    if(hItem)
    {
        destroySupergroup(hItem->sg);
        MP_FREE(CachedSg, hItem);
    }
}

static void cachedsg_Update( CachedSg *csg, Supergroup *sg )
{
    if(verify( csg ))
    {
        if( csg->sg != sg )
        {
            destroySupergroup(csg->sg);
            csg->sg = sg;
        }

        csg->secsLastUpdated = timerSecondsSince2000();
    }
}

static bool cachedsg_Valid( CachedSg *csg )
{
    U32 secs = timerSecondsSince2000();
    return csg && csg->secsLastUpdated + SECS_CACHEDSG_VALID > secs;
}

static StashTable s_cachedsgFromSgId = NULL;

void sgrp_TrackDbId(Supergroup *sg, int idSgrp)
{
    if( !s_cachedsgFromSgId )
    {
        s_cachedsgFromSgId = stashTableCreateInt(128);
    }
    
    if(verify( sg && idSgrp > 0 ))
    {
        CachedSg *cached;

        if( !stashIntFindPointer(s_cachedsgFromSgId, idSgrp, &cached) )
        {
            cached = cachedsg_Create( sg );
            stashIntAddPointer(s_cachedsgFromSgId, idSgrp, cached, false);
        }
        cachedsg_Update(cached, sg);
    }
}

//----------------------------------------
//  Boot the passed supergroup from the cache. Make sure nobody is
//  holding onto the supergroup before you call this
//----------------------------------------
void sgrp_UnTrackDbId(int idSgrp)
{
    CachedSg *csg;

    if(stashIntRemovePointer(s_cachedsgFromSgId, idSgrp, &csg))
    {
        cachedsg_Destroy(csg);
    }
}

//----------------------------------------
//  Find the supergroup with this id.
// Note: this is a cached supergroup, and isn't
// attached to any entity. changes to this must
// be propogated with the lock/unlock mechanism.
//----------------------------------------
Supergroup *sgrpFromDbId(int idSgrp)
{
    CachedSg *csg;
    Supergroup *sg = NULL;

    if( s_cachedsgFromSgId && stashIntFindPointer(s_cachedsgFromSgId, idSgrp, &csg)
        && cachedsg_Valid(csg) )
    {
        sg = csg->sg;
    }
    return sg;
}

#if SERVER
// Same as above but load it if it's not in the cache.
Supergroup *sgrpFromSgId(int idSgrp)
{
	Supergroup *sg = sgrpFromDbId(idSgrp);
	if (sg) return sg;
	dbSyncContainerRequest(CONTAINER_SUPERGROUPS, idSgrp, CONTAINER_CMD_TEMPLOAD, false);
	return sgrpFromDbId(idSgrp);
}
#endif

int sgrp_emptyMounts( Supergroup *sg )
{
    int i, count;
    int availableMounts;

    if (!sg)
        return 0;

    availableMounts    = sg->spacesForItemsOfPower;
    count = eaSize(&sg->specialDetails);
    for (i = 0; i < count; i++)
    {
        if (sg->specialDetails[i] && sg->specialDetails[i]->pDetail && 
            sg->specialDetails[i]->pDetail->eFunction == kDetailFunction_ItemOfPower)
            availableMounts--;
    }
    return availableMounts;
}

// Determine if this player is dark with respect to their supergroup
int sgroup_isPlayerDark(Entity *e)
{
    // Check we're in a supergroup, and NULL check e->pl.
    if (!e->supergroup_id || e->supergroup == NULL || e->pl == NULL)
    {
        // return false - this seems the most reasonable value in a case like this
        return 0;
    }

    return e->supergroup->playerType != e->pl->playerType;
}

/* End of File */
