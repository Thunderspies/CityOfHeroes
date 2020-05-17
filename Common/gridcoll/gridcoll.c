#include <stdio.h>
#include <utilitieslib/utils/mathutil.h>
#include "gridcoll/grid.h"
#include "gridcoll/ctri.h"
#include "gridcoll/gridcoll.h"
#include "seq/anim.h"
#include "group/group.h"
#include <stdlib.h>
#include "gridcoll/gridcache.h"
#include <utilitieslib/assert/assert.h>
#include "gridcollobj.h"
#include "gridcollperftest.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include "group/groupgrid.h"
#include "group/grouptrack.h"

DefTracker **curr_obj_list;
int        curr_obj_count,curr_obj_max,obj_nocache_count;

static int INLINEDBG gridClipLineToBox(Vec3 box,F32 r,Vec3 start,Vec3 end,Vec3 lna,Vec3 lnb)
{
    Vec3    dv;
    F32        fst = 0,fet = 1;
    int        i;

    for(i=0;i<3;i++)
    {
        F32        st,et,inv_di,maxbi,minbi;

        maxbi = box[i] + r;
        minbi = box[i] - r;
        if (start[i] < end[i])
        {
            if (start[i] > maxbi || end[i] < minbi)
                return 0;
            //st = (start[i] < minB[i])? (minB[i] - start[i]) / di: 0;
            //et = (end[i] > maxB[i])? (maxB[i] - start[i]) / di: 1;
            st = minbi - start[i];
            if (st > 0)
            {
                inv_di = 1.f / (end[i] - start[i]);
                st *= inv_di;
                if (end[i] > maxbi)
                {
                    et = (maxbi - start[i]) * inv_di;
                }
                else
                {
                    et = 1;
                }
            }
            else
            {
                st = 0;

                if (end[i] > maxbi)
                {
                    et = (maxbi - start[i]) / (end[i] - start[i]);
                }
                else
                {
                    et = 1;
                }
            }
        }
        else
        {
            if (end[i] > maxbi || start[i] < minbi)
                return 0;
            //st = (start[i] > maxB[i])? (maxB[i] - start[i]) / di: 0;
            //et = (end[i] < minB[i])? (minB[i] - start[i]) / di: 1;
            st = maxbi - start[i];
            if (st < 0)
            {
                inv_di = 1.f / (end[i] - start[i]);
                st *= inv_di;
                if (end[i] < minbi)
                {
                    et = (minbi - start[i]) * inv_di;
                }
                else
                {
                    et = 1;
                }
            }
            else
            {
                st = 0;

                if (end[i] < minbi)
                {
                    et = (minbi - start[i]) / (end[i] - start[i]);
                }
                else
                {
                    et = 1;
                }
            }
        }
        if (st > fst)    fst = st;
        if (et < fet)    fet = et;
        if (fet < fst)
            return 0;
    }
    subVec3(end,start,dv);
    scaleAddVec3(dv,fst,start,lna);
    scaleAddVec3(dv,fet,start,lnb);
    return 1;
}

static void findNearestVertex(CollInfo *coll)
{
    int            i,idx=0;
    Model    *model;
    F32            d,dist = 8e16,*v;
    DefTracker    *node;
    Vec3        dv,pos,tverts[3];
    Mat4        inv_mat;

    node = coll->node;
    model = node->def->model;
    expandCtriVerts(&model->ctris[coll->tri_idx],tverts);
    invertMat4Copy(node->mat,inv_mat);

    mulVecMat4(coll->mat[3],inv_mat,pos);
    for(i=0;i<3;i++)
    {
        v = tverts[i];
        subVec3(pos,v,dv);
        d = lengthVec3(dv);
        if (d < dist)
        {
            dist = d;
            idx = i;
        }
    }
    mulVecMat4(tverts[idx],node->mat,coll->mat[3]);
}

static INLINEDBG int sphereNearLine(DefTracker *tracker,CollInfo *coll)
{
F32        d,rad,t;
Vec3    dv,dv_p1,dv_ln,line_pos;

    // First do a quick sphere-sphere test. Since a lot of the lines are short this discards a bunch
    rad = tracker->radius + coll->radius;
    subVec3(tracker->mid,coll->start,dv_p1);
    t = coll->line_len + rad;
    if (lengthVec3Squared(dv_p1) > t*t)
        return 0;

    // Get dot product of point and line seg
    // Is the sphere off either end of the line?
    d = dotVec3(dv_p1,coll->dir);
    if (d < -rad || d > coll->line_len + rad)
        return 0;

    // Get point on line closest to given point
    scaleVec3(coll->dir,d,dv_ln);
    addVec3(dv_ln,coll->start,line_pos);

    // How far apart are they?
    subVec3(line_pos,tracker->mid,dv);
    d = lengthVec3Squared(dv);

    return d < rad*rad;
}


static int checkGridCellObjs(GridCell *cell,CollInfo *coll)
{
    int            i,tag,group_opened;
    U32            t;
    static        int rndo;
    void        *node;
    DefTracker    *tracker;
    GridEnts    *ents;

    tag = coll->grid->tag;
    for(;;)
    {
        group_opened = 0;
        for(ents=cell->entries;ents;ents=ents->next)
        {
            for(i=0;i<ARRAY_SIZE(ents->entries);i++)
            {
                t = (U32)ents->entries[i];
                if (!t)
                    continue;
                node = (void*)(uintptr_t)(t & ~1);
                tracker = node;

                if (!(coll->flags & COLL_NOTTRACKERS))
                {
                    if (t & 1)
                    {
                        group_opened = 1;
                        groupUnpackGrid(coll->grid,node);
                        break;
                    }
                    if (tracker->coll_time == coll->grid->tag)
                        continue;
                    tracker->coll_time = coll->grid->tag;
                }
#if 0
                if (!(coll->flags & COLL_EDITONLY) && !(coll->flags & COLL_NOTTRACKERS))
                {
                    Model *model;

                    assert(tracker->def->model);
                    model = tracker->def->model;
                    if (model && (model->tricks->flags & TRICK_NOCOLL))
                        continue;
                }
#endif
                if ((coll->flags & COLL_NODECALLBACK) && !coll->node_callback(tracker,coll->tri_state.backside))
                    continue;

                if (coll->flags & COLL_NOTTRACKERS)
                    continue;

                if (coll->flags & COLL_FINDINSIDE)
                {
#if CLIENT
                    if (tracker->dyn_parent_tray)
                        continue;
#endif
                    if (!tracker->def || 
                        (!tracker->def->volume_trigger 
                        && !tracker->def->has_volume 
                        && !tracker->def->is_tray))
                        continue;
                }

                if (!sphereNearLine(tracker,coll))
                    continue;
#if 0
                if (collObj(tracker,0,coll))
                    return 1;
#else
                dynArrayAddp(&curr_obj_list,&curr_obj_count,&curr_obj_max,tracker);
#endif
            }
        }
        if (!group_opened)
            break;
    }
    return 0;
}

DefTracker *last_coll_obj = 0;
static int gridCheckObjs(CollInfo *coll,DefTracker **curr_obj_list,int curr_obj_count)
{
    int            i;

    for(i=0;i<curr_obj_count;i++)
    {
        if (collObj(curr_obj_list[i],0,coll))
        {
            last_coll_obj = curr_obj_list[i];
            return 1;
        }
    }
    return 0;
}

static int gridCheckObjsSphereCheck(CollInfo *coll,DefTracker **curr_obj_list,int curr_obj_count)
{
    int            i;
    DefTracker    *tracker;

    for(i=0;i<curr_obj_count;i++)
    {
        tracker = curr_obj_list[i];
        if (!sphereNearLine(tracker,coll))
            continue;
        if (collObj(tracker,0,coll))
            return 1;
    }
    return 0;
}

static int gridSplitPlanes(CollInfo *coll,GridCell *cell,Vec3 lnap,Vec3 lnbp,Vec3 ppos,F32 size)
{
    Vec3        pos,lna,lnb;
    F32            ta,tb,r;
    int            i,nohit=0;
    U8            mask;
    static        U8    masks[3] = {XLO,YLO,ZLO};

    if (!cell)
        return 0;

    if (!gridClipLineToBox(ppos,coll->radius + size,lnap,lnbp,lna,lnb))
        return 0;
    if (cell->entries && checkGridCellObjs(cell,coll))
        return 1;

    if (!cell->children)
        return 0;

    mask = cell->filled;
    size *= 0.5;
    r = coll->radius;
    for(i=0;i<3;i++)
    {
        ta = lna[i] - ppos[i];
        tb = lnb[i] - ppos[i];

        if (!(ta <= r || tb <= r)) // neg side
            mask &= ~masks[i];

        if (!(ta >= -r || tb >= -r)) // pos side
            mask &= masks[i];
    }
    if (!mask)
        return 0;

    if (mask & YLO)
    {
        pos[1] = ppos[1] - size;
        if (mask & XLO)
        {
            pos[0] = ppos[0] - size;
            if (mask & ZLO)
            {
                pos[2] = ppos[2] - size;
                if (gridSplitPlanes(coll,cell->children[0],lna,lnb,pos,size))
                    return 1;
            }
            if (mask & ~ZLO)
            {
                pos[2] = ppos[2] + size;
                if (gridSplitPlanes(coll,cell->children[2],lna,lnb,pos,size))
                    return 1;
            }
        }
        if (mask & ~XLO)
        {
            pos[0] = ppos[0] + size;
            if (mask & ZLO)
            {
                pos[2] = ppos[2] - size;
                if (gridSplitPlanes(coll,cell->children[1],lna,lnb,pos,size))
                    return 1;
            }
            if (mask & ~ZLO)
            {
                pos[2] = ppos[2] + size;
                if (gridSplitPlanes(coll,cell->children[3],lna,lnb,pos,size))
                    return 1;
            }
        }
    }
    if (mask & ~YLO)
    {
        pos[1] = ppos[1] + size;
        if (mask & XLO)
        {
            pos[0] = ppos[0] - size;
            if (mask & ZLO)
            {
                pos[2] = ppos[2] - size;
                if (gridSplitPlanes(coll,cell->children[4],lna,lnb,pos,size))
                    return 1;
            }
            if (mask & ~ZLO)
            {
                pos[2] = ppos[2] + size;
                if (gridSplitPlanes(coll,cell->children[6],lna,lnb,pos,size))
                    return 1;
            }
        }
        if (mask & ~XLO)
        {
            pos[0] = ppos[0] + size;
            if (mask & ZLO)
            {
                pos[2] = ppos[2] - size;
                if (gridSplitPlanes(coll,cell->children[5],lna,lnb,pos,size))
                    return 1;
            }
            if (mask & ~ZLO)
            {
                pos[2] = ppos[2] + size;
                if (gridSplitPlanes(coll,cell->children[7],lna,lnb,pos,size))
                    return 1;
            }
        }
    }
    return 0;
}

int reuse_obj_count,full_search_count;

static void expandAllTrackers(DefTracker* tracker)
{
    int i;
    
    if(!tracker)
        return;

    trackerOpen(tracker);

    dynArrayAddp(&curr_obj_list,&curr_obj_count,&curr_obj_max,tracker);

    for(i = 0; i < tracker->count; i++)
        expandAllTrackers(&tracker->entries[i]);
}

int collGrid(Grid *grid,const Vec3 start,const Vec3 end,CollInfo *coll,F32 radius,int flags)
{
    Vec3        pos,lna,lnb;
    F32            size;
    extern        Grid    obj_grid;
    CollInfo    *collp = 0;
    CollParams    params;
    int            re_use_objs,set_obj_cache;

    extern FindInsideOpts glob_find_type;
    flags |= COLL_NORMALTRI;
    if (!grid)
        grid = &obj_grid;

    if (!curr_obj_list)
        curr_obj_list = calloc(256, sizeof(curr_obj_list[0]));

    copyVec3(start,params.start);
    copyVec3(end,params.end);
    params.radius = radius;
    params.flags = flags;
    params.find_type = glob_find_type;

    if(!(flags & COLL_USE_TRACKERS))
    {
        coll->trackers = NULL;
        coll->tracker_count = 0;
    }
    else
    {
        int i;
        curr_obj_count = 0;
        for(i = 0; i < coll->tracker_count; i++)
        {
            expandAllTrackers(coll->trackers[i]);
        }
    }

    if (grid == &obj_grid && !(flags & (COLL_GATHERTRIS|COLL_FINDINSIDE|COLL_NODECALLBACK|COLL_TRICALLBACK)) &&
        !coll->tracker_count)
    {
        if (gridCacheFind(&params,coll))
            return coll->sqdist < GRID_MAXDIST;
    }

    full_search_count++;
    coll->sqdist    = GRID_MAXDIST;
    coll->ctri        = 0;
    coll->surfTex    = 0;
    coll->radius    = radius;
    coll->flags        = flags;
    copyVec3(start,coll->start);
    copyVec3(end,coll->end);
    coll->grid        = grid;
    coll->valid_id    = grid->valid_id;
    coll->coll_count= 0;

    if(!coll->tracker_count)
    {
        re_use_objs = objCacheFind(start,end,radius,flags,curr_obj_list,&curr_obj_count);
        if (!re_use_objs)
            set_obj_cache = objCacheFitBlock(coll->start,coll->end,&coll->radius);
        else
            reuse_obj_count++;
    }
    else
    {
        re_use_objs = 0;
        set_obj_cache = 0;
    }

    setTriCollState(coll);
    grid->tag++;
    size = grid->size * 0.5;
    setVec3(pos,size,size,size);
    subVec3(start,grid->pos,lna);
    subVec3(coll->end,grid->pos,lnb);
    coll->grid_cache = 0;
    if (!re_use_objs)
    {
        if(!coll->tracker_count)
        {
            curr_obj_count=0;
            gridSplitPlanes(coll,grid->cell,lna,lnb,pos,size);
        }

        if (set_obj_cache)
        {
            objCacheSet(coll->start,coll->end,flags,curr_obj_list,curr_obj_count);
            coll->radius = radius;
            copyVec3(start,coll->start);
            copyVec3(end,coll->end);
            setTriCollState(coll);
            gridCheckObjsSphereCheck(coll,curr_obj_list,curr_obj_count);
        }
        else
        {
            gridCheckObjs(coll,curr_obj_list,curr_obj_count);
            obj_nocache_count++;
        }
    }
    else
        gridCheckObjsSphereCheck(coll,curr_obj_list,curr_obj_count);

    if (coll->sqdist < GRID_MAXDIST)
    {
        DefTracker    *tracker = coll->node;
        Mat3        m;

        if (!coll->ctri) // was just looking for nearest obj
        {
            copyMat4(tracker->mat,coll->mat);
            return 1;
        }
        normScaleToMat(coll->ctri->norm,coll->ctri->scale,m);
        mulMat3(tracker->mat,m,coll->mat);
        if (flags & COLL_NEARESTVERTEX)
            findNearestVertex(coll);

        if(!coll->tracker_count)
        {
            saveCollRec(start,end,radius,flags,1,coll->mat[3]);
            gridCacheSet(&params,coll);
        }
        if (collp)
            *collp = *coll;
        return 1;
    }
    if(!coll->tracker_count)
    {
        saveCollRec(start,end,radius,flags,0,zerovec3);
        gridCacheSet(&params,coll);
    }

    if (collp)
        *collp = *coll;
    return 0;
}
#if 0
void gridcolltest()
{
    Vec3 ptA = {-1784.0000, 4.5000000, -1912.0000};
    Vec3 ptB = {-1736.0000, 4.5000000, -1864.0000};
    CollInfo    info;
    extern void addDebugLine( Vec3 va, Vec3 vb, U8 r, U8 b, U8 g, U8 a, F32 width );

    return;
     if(!collGrid(NULL, ptA, ptB, &info, 1.8, COLL_NOTSELECTABLE | COLL_HITANY | COLL_ENTBLOCKER | COLL_BOTHSIDES)) {
        printf("miss\n"); }
    else
        printf("hit\n");
    addDebugLine( ptA, ptB, 255,255,255,255, 1 );
}
#endif