#include "group/groupdyn.h"
#include "group/group.h"
#include "group/grouputil.h"
#include <utilitieslib/utils/utils.h>
#include "group/grouptrack.h"
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/mathutil.h>
#include "group/groupfileload.h"

DynGroup        *dyn_groups = NULL;
int                dyn_group_count = 0,dyn_group_max = 0;
DynGroupStatus    *dyn_group_status = NULL;

static int findBreakable(DefTracker *tracker)
{
    GroupDef    *def = tracker->def;
    tracker->dyn_group=0;
    if (!def)
        return 0;

    if (def->breakable || def->child_breakable)
        groupLoadIfPreload(def);
    if (def->breakable)
    {
        DynGroup    *dyn;

        dyn = dynArrayAddStruct(&dyn_groups,&dyn_group_count,&dyn_group_max);
        dyn->tracker = tracker;
        tracker->dyn_group = dyn_group_count;
    }
    return def->child_breakable;
}

void groupDynBuildBreakableList()
{
    int        i;

    dyn_group_count = 0;
#ifndef TEST_CLIENT
    groupProcessDefTracker(&group_info,findBreakable);
#endif
    dyn_group_status = realloc(dyn_group_status,dyn_group_count * sizeof(DynGroupStatus));
    memset(dyn_group_status,0,dyn_group_count * sizeof(DynGroupStatus));
    for(i=0;i<dyn_group_count;i++)
        dyn_group_status[i].hp = 100;
}

void groupDynUpdateStatus(int idx)
{
#if 0
    // this was originally for destroyable objects, hopefully we will have them some decade
    if (!dyn_group_status[idx].hp)
    {
        dyn_groups[idx].tracker->invisible = 1;
        dyn_groups[idx].tracker->no_collide = 1;
    }
    else
    {
        dyn_groups[idx].tracker->invisible = 0;
        dyn_groups[idx].tracker->no_collide = 0;
    }
#endif
}

DynGroup *groupDynFromWorldId(int world_id)
{
    int        idx = world_id-1;

    assert(idx >= 0 && idx < dyn_group_count);
    return &dyn_groups[idx];
}

void groupDynGetMat(int world_id,Mat4 mat)
{
    DynGroup    *dyn = groupDynFromWorldId(world_id);

    if (dyn && dyn->tracker)
        copyMat4(dyn->tracker->mat,mat);
}

DynGroupStatus groupDynGetStatus(int world_id)
{
    int        idx = world_id-1;

    assert(idx >= 0 && idx < dyn_group_count);
    return dyn_group_status[idx];
}

