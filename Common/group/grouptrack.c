#include <utilitieslib/stdtypes.h>
#include "group/grouptrack.h"
#include <stdlib.h>
#include "group/group.h"
#include "group/groupgrid.h"
#include <utilitieslib/utils/mathutil.h>
#include <memory.h>
#include <utilitieslib/utils/utils.h>
#include "string.h"
#include <utilitieslib/utils/timing.h>
#ifdef CLIENT
#include "edit/edit_select.h"
#include "render/model_cache.h"
#include "graphics/vistray.h"
#include "NovodeX/NwWrapper.h"
#include "edit/edit_net.h"
#include "graphics/camera.h"
#include "bases/basedraw.h"
#endif
#include "group/groupfileload.h"
#include <utilitieslib/assert/assert.h>

// Trackers that must be updated after the edit cache finishes moving them.
static DefTracker **frozenTrackers;

int tracker_count;

#if CLIENT
TrickNode *allocTrackerTrick(DefTracker *tracker)
{
    if (!tracker->tricks) {
        tracker->tricks = calloc(sizeof(TrickNode),1);
    }
    return tracker->tricks;
}

void colorTrackerDel(DefTracker *tracker)
{
    int        i;

    if (tracker->src_rgbs)
    {
        for (i = 0; i < eaSize(&tracker->src_rgbs); i++)
            modelFreeRgbBuffer(tracker->src_rgbs[i]);
        eaDestroy(&tracker->src_rgbs);
        tracker->src_rgbs = 0;
    }

    for(i=0;i<tracker->count;i++)
        colorTrackerDel(&tracker->entries[i]);

    if (tracker->def && tracker->def->is_tray)
        vistrayDetailColorTrackerDel(tracker);
}

void lightGridDel(DefTracker *tracker)
{
DefTracker    *t;

    for(t = tracker;t;t = t->parent)
    {
        if (t->light_grid)
            break;
    }
    if (!t || !t->light_grid)
        return;
    gridFreeAll(t->light_grid);
    destroyGrid(t->light_grid);
    t->light_grid = 0;
}

//deletes the src_rgbs on all trackers so they can get
//redone
void colorTrackerDeleteAll()
{
    int i;
    for (i=0;i<group_info.ref_count;i++)
    {
        colorTrackerDel(group_info.refs[i]);
    }

    baseHackeryFreeCachedRgbs();
}

#endif

int trackerPack(DefTracker *tracker, int *idxs, char **names, int alloc_names)
{
    int i, depth;
    DefTracker *t = tracker;

    if (!tracker)
        return -1;

    for(depth=0;tracker->parent;tracker=tracker->parent)
        depth++;
    assert(depth <= 100); // hardcoded arrays abound

    tracker = t;
    for(i = depth-1; i >= 0; --i)
    {
        idxs[i] = tracker - tracker->parent->entries;
        if(names)
            names[i] = alloc_names ? strdup(tracker->def->name) : tracker->def->name;
        tracker = tracker->parent;
    }
    return depth;
}

DefTracker *trackerUnpack(DefTracker *tracker, int depth, int *idxs, char **names, int free_names)
{
    int i;

    if(depth == -1)
        return NULL;

    if(!tracker || !tracker->def)
    {
        assertmsg(!names || !names[0], "Trackers out of sync or bad ref");
        return NULL;
    }

    for(i=0;i<depth;i++)
    {
        trackerOpen(tracker);
        if(!INRANGE0(idxs[i], tracker->count))
        {
            assertmsg(!names || !names[i], "Trackers out of sync or bad index");
            return NULL;
        }
        tracker = &tracker->entries[idxs[i]];

        assertmsg(!names || !names[i] ||// the does not exist OR
            stricmp(tracker->def->name, names[i])==0,// || // the name is a match
            "Trackers out of sync");
        if(names && free_names)
            SAFE_FREE(names[i]);
    }
    trackerOpen(tracker);
    return tracker;
}

void trackerSetMid(DefTracker *tracker)
{
    GroupDef *def;
    Vec3    dv,mid;

    def = tracker->def;
    subVec3(def->max,def->min,dv);
    scaleVec3(dv,0.5,dv);
    tracker->radius = lengthVec3(dv); // * node->scale
    addVec3(dv,def->min,mid);
    mulVecMat4(mid,tracker->mat,tracker->mid);
}

int max_tracker_count;

static int trkr_ent_count,trkr_count,trkr_ent_hist[100];

void trackerStatsRecur(DefTracker *tracker)
{
int        t,i;

    if (!tracker)
        return;

    trkr_count++;
    t = 1;//tracker->grid_count;
    trkr_ent_count += t;
    if (t >= ARRAY_SIZE(trkr_ent_hist))
        t = ARRAY_SIZE(trkr_ent_hist)-1;
    trkr_ent_hist[t]++;
    for(i=0;i<tracker->count;i++)
    {
        trackerStatsRecur(&tracker->entries[i]);
    }
}

void trackerStats()
{
int        i,blocks1=0,blocks3=0,blocks7=0,blocks15=0,blocks31=0,count=0;

    trkr_ent_count=trkr_count = 0;
    memset(trkr_ent_hist,0,sizeof(trkr_ent_hist));
    for(i=0;i<group_info.ref_count;i++)
        trackerStatsRecur(group_info.refs[i]);

    printf("trackers %d, entries %d  avg %f\n",trkr_count,trkr_ent_count,(F32)trkr_ent_count / (F32)trkr_count);
    for(i=0;i<ARRAY_SIZE(trkr_ent_hist);i++)
    {
        printf("%d ",trkr_ent_hist[i]);
        blocks1 += trkr_ent_hist[i] * ((i + 0) / 1);
        blocks3 += trkr_ent_hist[i] * ((i + 2) / 3);
        blocks7 += trkr_ent_hist[i] * ((i + 6) / 7);
        blocks15 += trkr_ent_hist[i] * ((i + 14) / 15);
        blocks31 += trkr_ent_hist[i] * ((i + 30) / 31);
        count += trkr_ent_hist[i] * i;
    }
    printf("\n");
    printf("ent bytes %d %d\n",trkr_ent_count * 4,count * 4);
    printf("blocks1   %d\n",blocks1 * 8);
    printf("blocks3   %d\n",blocks3 * 16);
    printf("blocks7   %d\n",blocks7 * 32);
    printf("blocks15  %d\n",blocks15 * 64);
    printf("blocks31  %d\n",blocks31 * 128);
}

void trackerOpen(DefTracker *tracker)
{
    int            i;
    DefTracker    *ent;

    if (tracker->entries)
        return;
    PERFINFO_AUTO_START("trackerOpen", 1);
    if (tracker->parent)
        groupLoadIfPreload(tracker->parent->def);
    groupLoadIfPreload(tracker->def);
    tracker->count = tracker->def->count;

    tracker_count += tracker->count;

    tracker->entries = calloc(tracker->count, sizeof(DefTracker));
    for(i=0;i<tracker->count;i++)
    {
        groupLoadIfPreload(tracker->def->entries[i].def);
        ent = &tracker->entries[i];
        ent->id = -1;
        ent->def = tracker->def->entries[i].def;
        if(ent->def) // can be null during deletion
        {
            if (ent->def->is_tray)
                group_info.tray_opened = 1;
            ent->def_mod_time = ent->def->mod_time;
            ent->no_coll = tracker->def->no_coll || tracker->no_coll;

            ent->entries = 0;
            ent->parent = tracker;
#if CLIENT
            ent->src_rgbs = 0;
            ent->tricks = 0;
#endif
            mulMat4(tracker->mat,tracker->def->entries[i].mat,ent->mat);
            trackerSetMid(ent);
        }
        else
        {
#if CLIENT
            assertmsg(0, "NULL GroupDef entry!");
#else
            extern g_deleting_trackers;
            assertmsg(g_deleting_trackers, "NULL GroupDef entry!");
#endif
        }

    }
    PERFINFO_AUTO_STOP();
}

void trackerOpenEdit(DefTracker *tracker)
{
    trackerOpen(tracker);
    tracker->edit = 1;
}

#if CLIENT
#include "edit/edit_select.h"
#include "graphics/FX/fx.h"

void trackerCloseEdit(DefTracker *tracker)
{
int        i;

    if (!tracker)
        return;
    tracker->edit = 0;
    tracker->inherit = 0;
    editRefSelect(tracker,0);

    for(i=0;i<tracker->count;i++)
        trackerCloseEdit(&tracker->entries[i]);
}

#endif

void trackerClose(DefTracker *tracker)
{
    int        i;

    if (!tracker)
    {
        return;
    }

    PERFINFO_AUTO_START("trackerClose", 1);

    groupGridDel(tracker);
#if CLIENT
    if (tracker->dyn_parent_tray)
        vistrayNotifyDetailClosed(tracker);
    trackerUnselect(tracker);
    colorTrackerDel(tracker);
    lightGridDel(tracker);
    if (tracker->def && tracker->def->is_tray)
    {
        vistrayClosePortals(tracker);
        vistrayFreeTrayDetails(tracker);
    }
    tracker->fx_id = fxDelete( tracker->fx_id, SOFT_KILL );
    SAFE_FREE(tracker->tricks);
#endif
    for(i=0;i<tracker->count;i++)
    {
#if CLIENT
        if (&tracker->entries[i] == sel.group_parent_def)
        {
            sel.group_parent_def = 0;
            sel.group_parent_refid = 0;
        }
        if (&tracker->entries[i] == sel.activeLayer )
        {
            sel.activeLayer = 0;
            sel.activeLayer_refid = 0;
        }
        if (&tracker->entries[i] == cam_info.last_tray )
        {
            cam_info.last_tray = NULL; // Fix crash while editting?
        }
#endif
        tracker_count--;
        trackerClose(&tracker->entries[i]);
    }
    //memChunkAdd(tracker->entries);
    SAFE_FREE(tracker->entries);
    tracker->count = 0;
    
    tracker->edit = 0;
    PERFINFO_AUTO_STOP();
}

#if CLIENT
typedef struct
{
    GroupDef    *def;
    U8            hide;
    U8            freeze;
    U8            dirtyFreeze;
    Mat4        absolutePosition;    //
    U8            edit;
    U8            selected;
    U8            setParent;
    U8            activeLayer;
    int            idxs[100];
    int            depth;
} EditCache;

static EditCache    *edit_caches;
static int            edit_cache_count,edit_cache_max;

void editCacheClear()
{
    eaDestroy(&frozenTrackers);
}

void editCacheUpkeep()
{
}

static void clearTags(DefTracker * tracker)
{
    int i;
    if (tracker==NULL)
        return;
    tracker->tagged=0;
    for (i=0;i<tracker->count;i++)
        clearTags(&tracker->entries[i]);
}

static void tagTracker(DefTracker * tracker)
{
    if (tracker==NULL)
        return;
    if (tracker->tagged)
        return;
    tracker->tagged=1;
    tagTracker(tracker->parent);
}

static int needsCaching(DefTracker * tracker)
{
    return (    (sel_count==1 && sel_list[0].def_tracker == tracker) ||
                (sel.group_parent_def == tracker) ||
                (sel.activeLayer == tracker) ||
                tracker->edit || 
                (tracker->tricks && (tracker->tricks->flags1 & TRICK_HIDE))
            );
}
//that's right, given a tracker, this function finds any descendants that need caching and
//tags their entire lineage
static void recurseAndTagCachedLineage(DefTracker * tracker)
{
    int i;
    if (tracker==NULL)
        return;
    for (i=0;i<tracker->count;i++)
        recurseAndTagCachedLineage(&tracker->entries[i]);
    if (!tracker->tagged && needsCaching(tracker))
        tagTracker(tracker);
}

static void cacheEditInfo(DefTracker *tracker)
{
    EditCache    *ec;
    int            selected=0, activeLayer = 0, setParent = 0;

    if (!tracker)
        return;

    if ( sel_count==1 && sel_list[0].def_tracker == tracker )
        selected = 1;

    //Maybe this will work??
    if ( sel.group_parent_def == tracker )
        setParent = 1;

    if ( sel.activeLayer == tracker )
        activeLayer = 1;

//    if (!needsCaching(tracker))
//        return;

    ec = dynArrayAdd(&edit_caches,sizeof(EditCache),&edit_cache_count,&edit_cache_max,1);

    memset(ec,0,sizeof(*ec));
    ec->depth = trackerPack(tracker, ec->idxs, NULL, 0);
    if (tracker->tricks && tracker->tricks->flags1 & TRICK_HIDE)
        ec->hide = 1;
    if (tracker->edit)
        ec->edit = 1;
    if (tracker->frozen)
    {
        Mat4 buf;
        DefTracker * t;
        copyMat4(unitmat,ec->absolutePosition);
        t=tracker;
        while (t!=NULL) {
            mulMat4(t->mat,ec->absolutePosition,buf);
            copyMat4(buf,ec->absolutePosition);
            t=t->parent;
        }
        ec->freeze=1;
    }
    ec->selected = selected;
    ec->activeLayer = activeLayer;
    ec->setParent = setParent;

    ec->def = tracker->def;
//    for(i=0;i<tracker->count;i++)
//        cacheEditInfo(&tracker->entries[i]);
}

static void cacheTaggedTrackers(DefTracker * tracker)
{
    int i;
    if (tracker==NULL)
        return;
    if (!tracker->tagged)
        return;
    tracker->tagged=0;
    cacheEditInfo(tracker);
    for (i=0;i<tracker->count;i++)
        cacheTaggedTrackers(&tracker->entries[i]);
}

static EditCache *cacheEditFind(DefTracker *tracker)
{
    EditCache    look,*ec;
    int            i;

    if (!tracker)
        return 0;
    look.depth = trackerPack(tracker, look.idxs, NULL, 0);
    for(i=0;i<edit_cache_count;i++)
    {
        ec = &edit_caches[i];

        if (ec->depth == look.depth && memcmp(ec->idxs,look.idxs,ec->depth * sizeof(ec->idxs[0]))==0)
            return ec;
    }
    return 0;
}

void trackerOpenNewer(DefTracker *tracker,U32 mod_time)
{
    int            i;
    EditCache    *ec;

    ec = cacheEditFind(tracker);
    if (!ec)
        return;
    
    if (ec->hide)
    {
        allocTrackerTrick(tracker)->flags1 |= TRICK_HIDE;
        allocTrackerTrick(tracker)->flags2 |= TRICK2_WIREFRAME;
    }

    if (ec->freeze)
    {
        Mat4 location,buf;
        DefTracker * t = tracker->parent;
        copyMat4(unitmat,location);
        while (t!=NULL) {
            mulMat4(t->mat,location,buf);
            copyMat4(buf,location);
            t=t->parent;
        }
        invertMat4Copy(location,buf);
        copyMat4(buf,location);
        mulMat4(location,ec->absolutePosition,buf);
        {
            int i,j;
            int flag=0;
            for (i=0;i<4;i++)
                for (j=0;j<3;j++)
                    if (fabs(buf[i][j]-tracker->mat[i][j])>1e-9)
                        flag=1;
            if (flag) 
            {
                copyMat4(buf,tracker->mat);
                eaPush(&frozenTrackers,tracker);
            }
        }
        tracker->frozen=1;
    }

    if (ec->selected)
        selAdd(tracker,trackerRootID(tracker),0,0);

    //Try to recover from edit changes by keeping the active layer
    if ( ec->setParent )
    {
        sel.group_parent_def   = tracker;
        sel.group_parent_refid = trackerRootID(tracker);
    }
    if ( ec->activeLayer )
    {
        sel.activeLayer            = tracker;
        sel.activeLayer_refid    = trackerRootID(tracker);
    }

    trackerOpenEdit(tracker);

    for(i=0;i<tracker->count;i++)
        trackerOpenNewer(&tracker->entries[i],mod_time);

    if (!ec->edit)
        trackerCloseEdit(tracker);
}

DefTracker *trackerUpdate(DefTracker *tracker,GroupDef *def,int force)
{
    int        mod_time;

    if (!tracker || !tracker->def)
        return 0;
    if (force || tracker->def->mod_time != tracker->def_mod_time)
    {
        mod_time = tracker->def_mod_time;
        edit_cache_count = 0;
        recurseAndTagCachedLineage(tracker);
        cacheTaggedTrackers(tracker);
//        cacheEditInfo(tracker);
        trackerClose(tracker);
        if (mod_time) {
            sel_count=0;
            updatePropertiesMenu = true;
            trackerOpenNewer(tracker,mod_time);
        }
        if (tracker->def->is_tray)
            group_info.tray_opened = 1;
    }
    else
    {
        int        i;

        for(i=0;i<tracker->count;i++)
        {
            mulMat4(tracker->mat,def->entries[i].mat,tracker->entries[i].mat);
            trackerSetMid(&tracker->entries[i]);
            trackerUpdate(&tracker->entries[i],def->entries[i].def,0);
        }
    }
    return tracker;
}
#endif

void trackerGetMat(DefTracker *tracker,DefTracker *ref,Mat4 mat)
{
    copyMat4(tracker->mat,mat);
}

int trackerGetUID(DefTracker *tracker)
{
    int ret=0;
    int i;
    GroupEnt *childGroupEnt;
    GroupDef *defParent;
    if (!tracker->parent) {
        // Root level
        for (i = 0; i < group_info.ref_count && group_info.refs[i] != tracker; i++)
        {
            if (group_info.refs[i]->def)
            {
                ret += 1 + group_info.refs[i]->def->recursive_count;
            }
        }
        assert(group_info.refs[i] == tracker);
        return ret;
    }
    defParent = tracker->parent->def;
    ret = 1; // parent
    for (childGroupEnt = defParent->entries,i=0; i < defParent->count && &(tracker->parent->entries[i])!=tracker; i++,childGroupEnt++) 
        ret += 1 + childGroupEnt->def->recursive_count;
    assert(&(tracker->parent->entries[i])==tracker);
    return ret + trackerGetUID(tracker->parent);
}

int trackerDepth(DefTracker *tracker)
{
int        depth;

    if (!tracker)
        return 0;

    for(depth=0;tracker->parent;tracker=tracker->parent)
        depth++;

    return depth;
}

DefTracker *trackerRoot(DefTracker *tracker)
{
    for(;tracker && tracker->parent;tracker = tracker->parent)
        ;
    return tracker;
}

int trackerRootID(DefTracker *tracker)
{
DefTracker    *ref;

    ref = trackerRoot(tracker);
    if (ref==NULL)
        return -1;
    return groupRefFindID(ref);
}
