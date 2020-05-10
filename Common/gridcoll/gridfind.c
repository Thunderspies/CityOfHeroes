#include "gridcoll/grid.h"
#include "gridcoll/gridfind.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/assert/assert.h>

#include "group/groupgrid.h" // FIXME!!!

Grid obj_grid;

typedef struct
{
    void    *node[2];
    int        tag[2];
} TagEnt;

TagEnt tag_cache[128];

int high_count;

#define XLO 0x55
#define YLO 0x0f
#define ZLO 0x33

typedef struct
{
    Vec3    point;
    F32        radius;
    int        tag;
    GridEntryCallback    *callback;
    void    **list;
    int        count;
    int        max_count;
    Grid    *grid;
} GridEntryState;

static void processGridEntries(GridCell *cell,GridEntryState *state)
{
    int            i,t,tag,group_opened;
    static        int rndo = 0;
    void        *node;
    GridEnts    *ents;
    static        U32    *tags = NULL;
    static        int tag_max = 0;

    tag = state->tag;
    do
    {
        group_opened = 0;
        for(ents=cell->entries;ents;ents=ents->next)
        {
            for(i=0;i<ARRAY_SIZE(ents->entries);i++)
            {
                node = (void*)(uintptr_t)ents->entries[i];
                if (!node)
                    continue;

                if (((uintptr_t)node) & 1)
                {
                    group_opened = 1;
                    groupUnpackGrid(state->grid,(DefTracker*)(((uintptr_t) node) & ~1));
                    break;
                }

                t = ((uintptr_t)node) >> 1;
                if (t < 100000)
                {
                    if (t >= tag_max)
                    {
                        assert(t < 100000);
                        tag_max = 2*t;
                        tags = realloc(tags,tag_max * sizeof(U32));
                    }
                    if (tags[t] == tag)
                        continue;
                    tags[t] = tag;
                }
                else
                {
                    TagEnt        *tagp;
                    tagp = &tag_cache[( (((uintptr_t)node) >> 3) & (ARRAY_SIZE(tag_cache)-1) )];
                    if ( (tagp->node[0] == node && tagp->tag[0] == tag)
                        || (tagp->node[1] == node && tagp->tag[1] == tag) )
                        continue;
                    rndo = (~rndo) & 1;
                    tagp->node[rndo] = node;
                    tagp->tag[rndo] = tag;
                }
                if (state->list)
                {
                    if (state->count <= state->max_count-1)
                        state->list[state->count++] = node;
                }
                else
                    state->callback(node);
            }
            if (group_opened)
                break;
        }
    }
    while (group_opened);
}

static int gridSplitPlanes(GridCell *cell,Vec3 ppos,F32 size,GridEntryState *state)
{
    Vec3        pos;
    F32            t,r,*point;
    int            i;
    U8            mask;
    static        U8    masks[3] = {XLO,YLO,ZLO};

    if (!cell)
        return 0;

    r = state->radius;
    t = r + size;
    point = state->point;

    for(i=0;i<3;i++)
    {
        if (point[i] < ppos[i] - t)
            return 0;
        if (point[i] > ppos[i] + t)
            return 0;
    }

    processGridEntries(cell,state);

    if (!cell->children)
        return 0;

    mask = 0xff;
    size *= 0.5;
    for(i=0;i<3;i++)
    {
        t = point[i] - ppos[i];

        if (!(t <= r)) // neg side
            mask &= ~masks[i];

        if (!(t >= -r)) // pos side
            mask &= masks[i];
    }
    if (!mask)
        return 0;
    for(i=0;i<8;i++)
    {
        if ((1<<i) & mask)
        {
            copyVec3(ppos,pos);
            if (i & 1)
                pos[0] += size;
            else
                pos[0] -= size;

            if (i & 4)
                pos[1] += size;
            else
                pos[1] -= size;

            if (i & 2)
                pos[2] += size;
            else
                pos[2] -= size;

            if (cell->children[i] && gridSplitPlanes(cell->children[i],pos,size,state))
                return 1;
        }
    }
    return 0;
}

void gridFindObjectsInSphereCB(Grid *grid,GridEntryCallback *callback,Vec3 point,F32 radius)
{
    Vec3            pos;
    F32                size;
    GridEntryState    state;

    size = grid->size * 0.5f;
    setVec3(pos,size,size,size);
    subVec3(point,grid->pos,state.point);
    state.radius = radius;
    state.list = 0;
    state.callback = callback;
    state.tag = ++grid->tag;
    state.grid = grid;
    gridSplitPlanes(grid->cell,pos,size,&state);
}

int gridFindObjectsInSphere(Grid *grid,void **list,int max_count,const Vec3 point,F32 radius)
{
    Vec3            pos;
    F32                size;
    GridEntryState    state;

    size = grid->size * 0.5f;
    setVec3(pos,size,size,size);
    subVec3(point,grid->pos,state.point);
    state.radius = radius;
    state.callback = 0;
    state.list = list;
    state.count = 0;
    state.max_count = max_count;
    state.tag = ++grid->tag;
    state.grid = grid;
    gridSplitPlanes(grid->cell,pos,size,&state);
    return state.count;
}

GridCell *gridFindCell(Grid *grid,Vec3 pos,void **obj_list,int *count_ptr,int max_count)
{
    GridCell        *cell,*child;
    int                bit_count,bit,i,idx[3],oct_bits[3],count,bad_mask;
    Vec3            dv;
    GridEntryState    state;

    state.callback = 0;
    state.list = obj_list;
    state.count = 0;
    state.max_count = max_count;
    state.tag = ++grid->tag;
    state.grid = grid;

    subVec3(pos,grid->pos,dv);
    qtruncVec3(dv,oct_bits);
    bit_count = grid->num_bits;
    bad_mask = ~((1 << bit_count) - 1);
    if ((oct_bits[0] |  oct_bits[1] | oct_bits[2]) & bad_mask)
        return 0; // not in grid

    cell = grid->cell;
    count = 0;
    for(bit = bit_count-1;bit >= 0;bit--)
    {
        processGridEntries(cell,&state);
        if (!cell->children)
            break;
        for(i=0;i<3;i++)
        {
            idx[i] = (oct_bits[i] >> bit) & (NUMCELLS-1);
        }
        child = cell->children[(idx[1] << (NUMCELLBITS*2)) + (idx[2] << NUMCELLBITS) + idx[0]];
        if (!child)
            break;
        cell = child;
    }
    if (count_ptr)
        *count_ptr    = state.count;
    return cell;
}
