#include <utilitieslib/components/earray.h>
#include "automapServer.h"
#include "entity/entPlayer.h"
#include "dbcomm/dbcomm.h"
#include "svr/svr_base.h"
#include "comm_game.h"
#include "dbcomm/staticMapInfo.h"
#include "entity/entity.h"
#include "entity/teamCommon.h"

static U32    mission_vis_bits[(1024*1024+31)/32];
static int mission_highest_bit;

#define IS_STATIC_MAP (db_state.is_static_map)

VisitedMap *visitedMap_Create()
{
    return calloc(1, sizeof(VisitedMap));
}
// Returns a bitfield:
//    MVC_NEW_MAP if on a map for the first time,
//    MVC_MARKED 1 if a cell is marked (success)
//    0 on error
int mapperVisitCell(Entity *e,int cell_idx)
{
    int ret = 0;
    VisitedMap *visit_map=0;
    static VisitedMap mission_visit_map;

    if (!IS_STATIC_MAP)
    {
        if (cell_idx < 0 || cell_idx >= ARRAY_SIZE(mission_vis_bits)*32)
            return ret;
        if (!e->teamup || e->teamup->map_id!=db_state.map_id)
            return ret;
        if (!TSTB(mission_vis_bits,cell_idx))
        {
            ret |= MVC_MARKED;
            SETB(mission_vis_bits,cell_idx);
            if (cell_idx >= mission_highest_bit)
                mission_highest_bit = cell_idx+1;
        }
    }

    return ret;
}

void mapperSend(Entity *e, int clearFog)
{
    if (!IS_STATIC_MAP && !mission_vis_bits)
        return;

    START_PACKET( pak, e, SERVER_VISITED_MAP_CELLS );
    if (IS_STATIC_MAP)
    {
        StaticMapInfo *info = staticMapInfoFind(db_state.base_map_id);
        pktSendBitsPack(pak, 1, db_state.base_map_id);
        pktSendBits(pak, 1, (info && info->opaqueFogOfWar) ? 1 : 0);
        pktSendBits(pak, 1, clearFog ? 1 : 0);
    }
    else
    {
        pktSendBitsPack(pak, 1, 0);
        pktSendBitsPack(pak, 1, mission_highest_bit);
        if (mission_highest_bit)
        {
            int i;
            int max = (mission_highest_bit+31)/32;
            U32        *data = mission_vis_bits;
            for (i = 0; i < max; i++)
                pktSendBits(pak, 32, data[i]);
        }
    }
    END_PACKET
}


void mapperClearFogForCurrentMap(Entity *e)
{
    int i;
    int count = MAX_MAPVISIT_CELLS;

    if (!IS_STATIC_MAP)
    {
        count = ARRAY_SIZE(mission_vis_bits)*32;
    }

    for (i = 0; i < count; i++)
    {
        mapperVisitCell(e, i);
    }

    mapperSend(e, 1);
}
