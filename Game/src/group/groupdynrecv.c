#include "group/groupdyn.h"
#include "group/groupdynrecv.h"
#include <utilitieslib/utils/utils.h>
#include <zlib.h>
#include "graphics/groupdraw.h"
#include <utilitieslib/network/netcomp.h>
#include "demo.h"


void groupDynReceive(Packet *pak)
{
    int        modified_count,dyn_group_count_from_server;

    modified_count = pktGetBitsPack(pak,1);
    if (modified_count == GROUP_DYN_FULL_UPDATE ) //-1
    {
        void    *data;
        int        i,byte_count;

        groupDynBuildBreakableList();
        data = pktGetZipped(pak,&byte_count);
        dyn_group_count_from_server = byte_count / sizeof(DynGroupStatus);
#ifndef TEST_CLIENT
        assert(dyn_group_count_from_server == dyn_group_count);
#endif
        dyn_group_status = realloc(dyn_group_status,byte_count);
        memcpy(dyn_group_status,data,byte_count);
        for(i=0;i<dyn_group_count;i++)
            groupDynUpdateStatus(i);
        free(data);
    }
    else if (modified_count)
    {
        int        i,idx;

        if (!devassert(dyn_group_status))
            groupDynBuildBreakableList();

        for(i=0;i<modified_count;i++)
        {
            idx = pktGetBitsPack(pak,16);
            dyn_group_status[idx].hp = pktGetBits(pak,GRPDYN_HP_BITS);
            dyn_group_status[idx].repair = pktGetBits(pak,GRPDYN_REPAIR_BITS);
            if (dyn_groups)
                groupDynUpdateStatus(idx);
        }
    }

    //Just record the entire char array on all updates because it's easier, and not too frequent
    if( (modified_count > 0 || modified_count == GROUP_DYN_FULL_UPDATE) && demoIsRecording() )
        demoRecordDynGroups();
}

#if 0
int groupDynId(DefTracker *tracker)
{
    return tracker->dyn_group;
}
#endif
