
#include "group/group.h"
#include "group/groupdyn.h"
#include "groupdynsend.h"
#include <utilitieslib/utils/utils.h>
#include <zlib.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/timing.h>

static int    *net_modified_groups,net_modified_count,net_modified_max;
static int    dyn_rescan_breakable;

int groupDynGetHp(int world_id)
{
    int        idx = world_id-1;

    assert(idx >= 0 && idx < dyn_group_count);
    return dyn_group_status[idx].hp;
}

void groupDynSetHpAndRepair(int world_id,int hp,int repair)
{
    int        idx = world_id-1,*iptr;

    assert(idx >= 0 && idx < dyn_group_count);

    if (hp == dyn_group_status[idx].hp && repair == dyn_group_status[idx].repair)
        return;

    dyn_group_status[idx].hp = hp;
    dyn_group_status[idx].repair = repair;
    groupDynUpdateStatus(idx);

    iptr = dynArrayAddStruct(    &net_modified_groups,
                                &net_modified_count,
                                &net_modified_max);
    *iptr = idx;
}

void groupDynResetSends()
{
    net_modified_count = 0;
}

int groupDynSend(Packet *pak,int full_update)
{
    if ( full_update || dyn_rescan_breakable )
    {
        pktSendBitsPack(pak,1, GROUP_DYN_FULL_UPDATE);
        pktSendZipped( pak, dyn_group_count * sizeof(DynGroupStatus), dyn_group_status);
        return 1;
    }
    else
    {
        int        i,idx;

        pktSendBitsPack(pak,1,net_modified_count);
        for(i=0;i<net_modified_count;i++)
        {
            idx = net_modified_groups[i];
            pktSendBitsPack(pak,16,idx);
            pktSendBits(pak,GRPDYN_HP_BITS,dyn_group_status[idx].hp);
            pktSendBits(pak,GRPDYN_REPAIR_BITS,dyn_group_status[idx].repair);
        }
        return net_modified_count != 0;
    }
}

// name = null means we don't care about the name, just the dist
// dist = -1 means we don't care about the dist, just the name
// return value of 0 means nothing found
int groupDynFind(const char *name,const Vec3 pos,F32 dist)
{
    int        i,idx=-1;
    F32        d2;

    if (dist < 0)
        dist = 10000000;
    dist = SQR(dist);
    for(i=0;i<dyn_group_count;i++)
    {
        if (name && stricmp(dyn_groups[i].tracker->def->name,name)!=0)
            continue;
        d2 = distance3Squared(dyn_groups[i].tracker->mid,pos);    // should this be pivot or mid?
        if (d2 > dist)
            continue;
        dist = d2;
        idx = i;
    }
    return idx+1;
}

void groupDynForceRescan()
{
    dyn_rescan_breakable = 1;
}

void groupDynFinishUpdate()
{
    dyn_rescan_breakable = 0;
}

