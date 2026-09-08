#include "cmdparse/cmdgame.h"
#include "graphics/vistray.h"
#include "group/group.h"
#include "graphics/groupdraw.h"
#include "graphics/groupdrawinline.h"
#include "group/groupgrid.h"
#include "graphics/camera.h"
#include <utilitieslib/components/StashTable.h>
#include "gridcoll/gridfind.h"
#include "seq/anim.h"
#include "gridcoll/grid.h"
#include <utilitieslib/utils/utils.h>
#include "group/grouptrack.h"
#include "graphics/gfxwindow.h"
#include "graphics/zOcclusion.h"
#include "group/grouputil.h"
#include "graphics/gfxLoadScreens.h"
#include <utilitieslib/utils/error.h>
#include "graphics/vistraygrid.h"
#include "render/renderprim.h"
#include "graphics/gfx.h"

// The max depth in the tree a tray can be located.  1 = root level (ref)
#define MAX_TRAY_DEPTH 1

typedef struct VisPortal
{
    struct VisTray *tray;
    Vec3 pos, min, max;
    F32 radius;
    U32 draw_id; // used to prevent exploring a portal more than once
} VisPortal;

typedef struct VisTray
{
    DefTracker    *def;            // the DefTracker

    DefTracker    **details;        // detail groups
    int            *detail_uids;

    U32            draw_id;        // used to prevent drawing a tray more than once

    int            portal_count;    // number of connected trays
    VisPortal    *portals;        // the connected trays

    int            is_open;        // a tray can exist but not be opened if a brother has linked to it

    int            uid;

} VisTray;

MP_DEFINE(VisTray);

static StashTable vistray_trayhash = 0;
static VisTray            **vistray_trayarray = 0;

static DefTracker        **vistray_visibleTrays;
static int                vistray_visibleTrayCount, vistray_visibleTrayMax;

static DefTracker        **vistray_outsideTrays;
static int                vistray_outsideTrayCount, vistray_outsideTrayMax;

////////////////////////////////////////////////////////////////////
// Helpers
////////////////////////////////////////////////////////////////////

static INLINEDBG void vistrayInit(void)
{
    if (vistray_trayarray)
        return;

    eaCreate(&vistray_trayarray);
    vistray_trayhash = stashTableCreateAddress(100);
}

static INLINEDBG VisPortal *allocPortals(int count)
{
    VisPortal *portals;
    if (!count)
        return 0;
    portals = calloc(count, sizeof(VisPortal));
    return portals;
}

static INLINEDBG void setupPortal(VisPortal *portal1, VisPortal *portal2, VisTray *tray1, VisTray *tray2, Portal *model_portal, Mat4 transform, Vec3 portal_pos)
{
    portal1->tray = tray2;
    groupGetBoundsTransformed(model_portal->min, model_portal->max, transform, portal1->min, portal1->max);
    copyVec3(portal_pos, portal1->pos);
    portal1->radius = distance3(portal1->min, portal1->max) * 0.5f;

    portal2->tray = tray1;
    copyVec3(portal1->min, portal2->min);
    copyVec3(portal1->max, portal2->max);
    copyVec3(portal_pos, portal2->pos);
    portal2->radius = portal1->radius;
}

static INLINEDBG void clearPortal(VisPortal *portal)
{
    portal->tray = 0;
}

static INLINEDBG void freePortals(VisTray *tray)
{
    if (tray->portals)
    {
        int i, j;
        for (i = 0; i < tray->portal_count; i++)
        {
            VisTray *brother = tray->portals[i].tray;
            if (!brother || !brother->portals)
                continue;

            for (j = 0; j < brother->portal_count; j++)
            {
                if (brother->portals[j].tray == tray)
                {
                    clearPortal(&(brother->portals[j]));
                    brother->is_open = 0;
                }
            }
        }

        free(tray->portals);
    }

    tray->portals = 0;
    tray->portal_count = 0;
    tray->is_open = 0;
}

static INLINEDBG int portalMatch(Vec3 pos, DefTracker *tracker, F32 *dist)
{
    Vec3        currp,dv;
    ModelExtra    *extra;
    int            i,idx=-1;
    F32            len,maxlen=0.2;

    modelCreateCtris(tracker->def->model);
    extra = tracker->def->model->extra;
    for(i=0;extra && i<extra->portal_count;i++)
    {
        mulVecMat4(extra->portals[i].pos, tracker->mat, currp);
        subVec3(pos, currp, dv);
        len = lengthVec3(dv);
        if (len < maxlen)
        {
            maxlen = len;
            idx = i;
        }
    }
    *dist = maxlen;
    return idx;
}

static INLINEDBG void freeTrayDetails(VisTray *tray)
{
    int i, size = eaSize(&tray->details);
    for (i = 0; i < size; i++)
    {
        tray->details[i]->dyn_parent_tray = 0;
        trackerClose(tray->details[i]);
    }

    eaDestroy(&tray->details);
    eaiDestroy(&tray->detail_uids);
}

static void freeTray(VisTray *tray)
{
    freePortals(tray);
    freeTrayDetails(tray);
    MP_FREE(VisTray, tray);
}

static INLINEDBG VisTray *getTray(DefTracker *tracker)
{
    VisTray* pReturn;
    vistrayInit();

    if (stashAddressFindPointer(vistray_trayhash, tracker, &pReturn))
        return pReturn;
    return NULL;
}

static INLINEDBG VisTray *addTray(DefTracker *tracker)
{
    VisTray *tray;
    
    MP_CREATE(VisTray, 32);
    tray = MP_ALLOC(VisTray);
    tray->def = tracker;
    tray->uid = -1;

    vistrayInit();

    eaPush(&vistray_trayarray, tray);
    stashAddressAddPointer(vistray_trayhash, tracker, tray, false);

    return tray;
}

static INLINEDBG VisTray *getOrAddTray(DefTracker *tracker)
{
    VisTray *tray = getTray(tracker);
    if (tray)
        return tray;

    return addTray(tracker);
}

static INLINEDBG DefTracker *findClosestBrother(Vec3 pos, DefTracker *tracker, int *idxOut)
{
    DefTracker        *nodes[1000], *brother, *minbrother;
    F32                mindist, dist;
    extern Grid        obj_grid;
    int                i, count, idx, minidx=-1;

    count = gridFindObjectsInSphere(&obj_grid, nodes, 1000, pos, 10);

    mindist = 8e8;
    minbrother = 0;
    for(i = 0; i < count; i++)
    {
        if (nodes[i] == tracker || !nodes[i]->def->is_tray)
            continue;

        brother = nodes[i];
        idx = portalMatch(pos, brother, &dist);
        if (idx >= 0 && dist < mindist)
        {
            minbrother = brother;
            mindist = dist;
            minidx = idx;
        }
    }

    *idxOut = minidx;
    return minbrother;
}

static INLINEDBG void vistrayLinkDetail(VisTray *parent_tray, DefTracker *parent, DefTracker *detail)
{
    int    i, size;

    // set the parent of the detail
    detail->dyn_parent_tray = parent_tray;

    // check for duplicate linking
    size = eaSize(&parent_tray->details);
    for (i = 0; i < size; i++)
    {
        if (parent_tray->details[i] == detail)
            return;
    }

    // add the detail
    if (!parent_tray->details)
    {
        eaCreate(&parent_tray->details);
        eaiCreate(&parent_tray->detail_uids);
    }
    eaPush(&parent_tray->details, detail);
    eaiPush(&parent_tray->detail_uids, -1);

    if (detail->def->has_light || detail->def->child_light)
    {
        colorTrackerDel(parent);
        lightGridDel(parent);
    }
}

// add details from a tree to trays
static void vistrayAddDetailsToTrays(DefTracker *detail, int recurseDepth)
{
    DefTracker *tray_tracker;
    int recurse = 1;

    loadUpdate("Tray Detailing", 100);

    // we don't want to add details that are trays
    if (!detail || !detail->def || detail->def->is_tray)
        return;

    if (!trayGridCheck(detail->mid, detail->def->radius))
        return;

    if (detail->def->is_autogroup)
    {
        // recurse without testing or incrementing the depth
        int i;
        trackerOpen(detail);
        for (i = 0; i < detail->count; i++)
            vistrayAddDetailsToTrays(&(detail->entries[i]), recurseDepth);
        return;
    }

    loadUpdate("Tray Detailing", 1000);

    // we don't want to make the parent of a tray a detail either
    if (!detail->def->child_tray)
    {
        // find the tray containing the detail
        tray_tracker = groupFindInside(detail->mat[3],FINDINSIDE_TRAYONLY,0,0);
        if (!tray_tracker)
        {
            // hack for objects placed on floors
            Vec3 offsetpos;
            copyVec3(detail->mat[3], offsetpos);
            offsetpos[1] += 0.001;
            tray_tracker = groupFindInside(offsetpos,FINDINSIDE_TRAYONLY,0,0);
        }

        // the outdoor mission tray is special: it is for mission purposes, not for drawing
        if (tray_tracker && tray_tracker->def && ((tray_tracker->parent && tray_tracker->parent->def && tray_tracker->parent->def->has_volume) || (tray_tracker->def->model && strstri(tray_tracker->def->model->name, "_outdoor_mission_tray"))))
            tray_tracker = 0;

        if (tray_tracker)
        {
            VisTray *tray = getOrAddTray(tray_tracker);
            if (tray)
            {
                vistrayLinkDetail(tray, tray_tracker, detail);
                //printf("%s inside %s\n", detail->def->name, tray_tracker->def->name);
            }
            recurse = 0;
        }
    }

    if (recurse && detail->def->count && ((++recurseDepth) <= MAX_TRAY_DEPTH))
    {
        // recursive step
        int i;
        trackerOpen(detail);
        for (i = 0; i < detail->count; i++)
            vistrayAddDetailsToTrays(&(detail->entries[i]), recurseDepth);
    }
}

// free a specific detail from a specific tray
static INLINEDBG void vistrayFreeTrayDetail(DefTracker *detail, VisTray *tray)
{
    int i, size = eaSize(&tray->details);

    for (i = 0; i < size; i++)
    {
        if (tray->details[i] == detail)
        {
            detail->dyn_parent_tray = 0;
            eaRemove(&tray->details, i);
            eaiRemove(&tray->detail_uids, i);
            trackerClose(detail);
            size--;
            i--;
        }
    }
}

// free all details from a specific tree from their parent trays
static void vistrayFreeDetailsFromTrays(DefTracker *detail, int recurseDepth)
{
    if (!detail)
        return;

    if (detail->def && detail->def->is_autogroup)
    {
        // recurse without incrementing the depth
        int i;
        if (detail->entries)
        {
            for (i = 0; i < detail->count; i++)
                vistrayFreeDetailsFromTrays(&(detail->entries[i]), recurseDepth);
        }
        return;
    }

    if (detail->dyn_parent_tray)
    {
        vistrayFreeTrayDetail(detail, detail->dyn_parent_tray);
    }
    else if ((++recurseDepth) <= MAX_TRAY_DEPTH)
    {
        // recursive step
        int i;
        if (detail->entries)
        {
            for (i = 0; i < detail->count; i++)
                vistrayFreeDetailsFromTrays(&(detail->entries[i]), recurseDepth);
        }
    }
}




////////////////////////////////////////////////////////////////////
// Drawing
////////////////////////////////////////////////////////////////////


static void drawTray(VisTray *tray, VisType vis, DrawParams *draw, Mat4 parent_mat, int draw_connected)
{
    DefTracker *tracker;
    GroupDef *group;
    int i;

    tracker = tray->def;
    group = tracker->def;

    if (tray->uid < 0)
        tray->uid = trackerGetUID(tracker);

    if (group->outside || group->outside_only)
        group_draw.see_outside = 1;

    // See COH-17001 and COH-17181
    // Don't draw a tray twice, but check it's portals if draw_connected is true
    if (tray->draw_id != group_draw.draw_id)
    {
        GroupEnt ent = {0};

        // draw the tray's group hiererchy
        makeAGroupEnt(&ent, group, tracker->mat, draw->view_scale, 0);
        if (gfx_state.renderPass >= RENDERPASS_SHADOW_CASCADE1 && gfx_state.renderPass <= RENDERPASS_SHADOW_CASCADE_LAST)
        {
            if (!drawDefInternal_shadowmap(&ent, parent_mat, tracker, vis, draw, tray->uid) && vis != VIS_EYE)
                return;
        }
        else
        {
            if (!drawDefInternal(&ent, parent_mat, tracker, vis, draw, tray->uid) && vis != VIS_EYE)
                return;
        }

        // add to the list of drawn trays
        dynArrayAddp(&vistray_visibleTrays, &vistray_visibleTrayCount, &vistray_visibleTrayMax, tracker);

        // mark as drawn
        tray->draw_id = group_draw.draw_id;
    }

    // At this point, handleDefTracker() called from drawDef() called from drawDefInternal() should have opened the portals for this tray.

    // draw connected trays
    if (draw_connected && tray->portals)
    {
        VisPortal *portal;

        //If this tray is a visblocker, and you aren't in this tray, you don't need to draw any connectors
        if (vis != VIS_EYE && group->vis_blocker)
            return;

        for(i = 0; i < tray->portal_count; i++)
        {
            int draw_grandchildren = 1;

            portal = &tray->portals[i];

            if (!portal->tray || (portal->draw_id == group_draw.draw_id))
                continue;

            portal->draw_id = group_draw.draw_id;

            {
                int clip;
                Vec3 pos;
                mulVecMat4(portal->pos, parent_mat, pos);

                if (!(clip = gfxSphereVisible(pos, portal->radius)))
                    continue;

#if 1
                if (game_state.zocclusion)
                {
                    if (vis != VIS_EYE && !zoTest(portal->min, portal->max, parent_mat, 1, portal->tray->uid, (portal->tray->def && portal->tray->def->def)?portal->tray->def->def->recursive_count:0))
                        draw_grandchildren = 0;
                }
                else
#endif
                {
                    if (vis != VIS_EYE && !checkBoxFrustum(clip, portal->min, portal->max, parent_mat))
                        draw_grandchildren = 0;
                }

            }

            if (game_state.show_portals)
            {
                Mat4 matModelToWorld;
                mulMat4(cam_info.inv_viewmat, parent_mat, matModelToWorld);
                drawBox3D(portal->min, portal->max, matModelToWorld, 0xFFFF00FF, 2);
            }

            drawTray(portal->tray, VIS_CONNECTOR, draw, parent_mat, draw_grandchildren);
        }
    }
}


////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////

void vistrayDrawDetails(DefTracker *tracker_parent, DrawParams *draw, Mat4 parent_mat, int vis)
{
    VisTray *tray;
    int i, size, uid;
    DefTracker *detail;
    GroupEnt ent = {0};
    TrickNode *trick = draw->tricks;

    //if (!tracker_parent || !tracker_parent->def || !tracker_parent->def->is_tray)
    //    return;

    tray = getTray(tracker_parent);
    if (!tray)
        return;

    if (vis != VIS_DRAWALL)
        vis = VIS_NORMAL;

    draw->tricks = 0;

    // draw details
    size = eaSize(&tray->details);
    for (i = 0; i < size; i++)
    {
        detail = tray->details[i];
        uid = tray->detail_uids[i];
        if (uid < 0)
            uid = tray->detail_uids[i] = trackerGetUID(detail);
        makeAGroupEnt(&ent, detail->def, detail->mat, draw->view_scale, 1);
        drawDefInternal(&ent, cam_info.viewmat, detail, vis, draw, tray->detail_uids[i]);
    }

    draw->tricks = trick;
}

void vistrayDraw(DrawParams *draw, Mat4 parent_mat)
{
    DefTracker *curr;
    VisTray *tray;

    PERFINFO_AUTO_START("vistrayDraw", 1);

    curr = groupFindInside(cam_info.cammat[3],FINDINSIDE_TRAYONLY,0,0);
    cam_info.last_tray = groupFindLightTracker(cam_info.cammat[3]);

    if (!curr)
    {
        PERFINFO_AUTO_STOP();
        return;
    }

    tray = getOrAddTray(curr);
    if (!tray || (tray->draw_id == group_draw.draw_id))
    {
        PERFINFO_AUTO_STOP();
        return;
    }

    // Initialize to inside only.  Will be set if a drawn tray can see outside.
    group_draw.see_outside = 0;
    draw->tray_child = true;

    // draw
    drawTray(tray, VIS_EYE, draw, parent_mat, 1);
    draw->tray_child = false;

    PERFINFO_AUTO_STOP();
}

static void freeTrayAdapter(void* arg0)
{
    freeTray((VisTray *)arg0);
}

void vistrayReset(void)
{
    vistrayInit();

    eaClearEx(&vistray_trayarray, freeTrayAdapter);
    stashTableClear(vistray_trayhash);
}

void vistrayOpenPortals(DefTracker *tracker)
{
    VisTray            *tray;
    Model            *model;
    Portal            *modelPortals;

    Vec3            pos;
    DefTracker        *brother;
    VisTray            *br_tray;
    ModelExtra        *br_extra;
    int                i, idx;

    if (!tracker || !tracker->def || !tracker->def->is_tray)
        return;

    tray = getOrAddTray(tracker);
    model = tracker->def->model;

    if (!model || !model->extra) // no exits
        return;

    if (tray->is_open)
        return;

    modelPortals = &(model->extra->portals[0]);

    tray->portal_count = model->extra->portal_count;
    if (!tray->portals) // could've been opened by brother
        tray->portals = allocPortals(tray->portal_count);

    tray->is_open = 1;

    // find brother trays and connect to them
    for(i = 0; i < tray->portal_count; i++)
    {
        if (tray->portals[i].tray)
            continue;

        mulVecMat4(modelPortals[i].pos, tracker->mat, pos);

        brother = findClosestBrother(pos, tracker, &idx);
        if (brother && idx >= 0)
        {
            br_tray = getOrAddTray(brother);
            br_extra = brother->def->model->extra;

            if (!br_tray->portals)
            {
                br_tray->portal_count = br_extra->portal_count;
                br_tray->portals = allocPortals(br_tray->portal_count);
            }

            if (br_tray->portals && !br_tray->portals[idx].tray)
                setupPortal(&tray->portals[i], &br_tray->portals[idx], tray, br_tray, &modelPortals[i], tracker->mat, pos);
        }
    }
}

void vistrayClosePortals(DefTracker *tracker)
{
    VisTray *tray;

    if (!tracker || !tracker->def || !tracker->def->is_tray)
        return;

    tray = getTray(tracker);

    if (!tray)
        return;

    freePortals(tray);
}

VisTrayInfo * vistrayGetInfo(DefTracker *tracker)
{
    static VisTrayInfo trayinfo;
    VisTray *tray = getTray(tracker);

    if (!tray || !tray->is_open)
        return 0;

    trayinfo.portal_count = tray->portal_count;
    trayinfo.detail_count = eaSize(&tray->details);

    return &trayinfo;
}

void vistrayDetailTrays(void)
{
    int    i, j, start_depth=1;

    writeConsole(OUTPUT_DEBUG, "Detailing trays");
    for (i = 0; i < group_info.ref_count; i++)
    {
        DefTracker *ref = group_info.refs[i];
        if (ref->def && groupDefFindProperty(ref->def, "Layer"))
        {
            trackerOpen(ref);
            for (j = 0; j < ref->count; j++)
            {
                vistrayAddDetailsToTrays(&ref->entries[j], start_depth);
            }
        }
        else
        {
            vistrayAddDetailsToTrays(ref, start_depth);
        }
    }
    writeConsole(OUTPUT_INFO, "Detailed trays");
}

void vistrayFreeTrayDetails(DefTracker *tray_tracker)
{
    VisTray *tray;

    if (!tray_tracker || !tray_tracker->def || !tray_tracker->def->is_tray)
        return;

    tray = getTray(tray_tracker);
    if (!tray)
        return;

    freeTrayDetails(tray);
}

void vistrayAddDetail(DefTracker *detail)
{
    int j;
    if (detail->def && groupDefFindProperty(detail->def, "Layer"))
    {
        trackerOpen(detail);
        for (j = 0; j < detail->count; j++)
        {
            vistrayAddDetailsToTrays(&detail->entries[j], 1);
        }
    }
    else
    {
        vistrayAddDetailsToTrays(detail, 1);
    }
}

void vistrayFreeDetail(DefTracker *detail)
{
    int j;
    if (detail->def && groupDefFindProperty(detail->def, "Layer"))
    {
        trackerOpen(detail);
        for (j = 0; j < detail->count; j++)
        {
            vistrayFreeDetailsFromTrays(&detail->entries[j], 1);
        }
    }
    else
    {
        vistrayFreeDetailsFromTrays(detail, 1);
    }
}

void vistrayDetailColorTrackerDel(DefTracker *tray_tracker)
{
    VisTray *tray;
    int i, size;

    if (!tray_tracker || !tray_tracker->def || !tray_tracker->def->is_tray)
        return;

    tray = getTray(tray_tracker);
    if (!tray)
        return;

    size = eaSize(&tray->details);
    for (i = 0; i < size; i++)
        colorTrackerDel(tray->details[i]);
}

void lightsToGrid(DefTracker *tracker,Grid *light_grid);

void vistrayDetailLightsToGrid(DefTracker *tray_tracker, Grid *light_grid)
{
    VisTray *tray;
    int i, size;

    if (!tray_tracker || !tray_tracker->def || !tray_tracker->def->is_tray)
        return;

    tray = getTray(tray_tracker);
    if (!tray)
        return;

    size = eaSize(&tray->details);
    for (i = 0; i < size; i++)
        lightsToGrid(tray->details[i],light_grid);
}

static int cmpPtrQSort(const void* ptr1Data, const void* ptr2Data)
{
    void ** ptr1 = (void **)ptr1Data;
    void ** ptr2 = (void **)ptr2Data;
    if(*ptr1 < *ptr2)
        return -1;
    if(*ptr1 == *ptr2)
        return 0;
    return 1;
}

void vistrayClearVisibleList(void)
{
    memset(vistray_visibleTrays, 0, sizeof(vistray_visibleTrays[0]) * vistray_visibleTrayCount );
    vistray_visibleTrayCount = 0;
}

void vistraySortVisibleList(void)
{
    qsort(vistray_visibleTrays, vistray_visibleTrayCount, sizeof(vistray_visibleTrays[0]), cmpPtrQSort );
}

int vistrayIsVisible(DefTracker *tray_tracker)
{
    if (tray_tracker)
    {
        // check if the tray is in the visible list
        if (bsearch(&tray_tracker, vistray_visibleTrays, vistray_visibleTrayCount, sizeof(DefTracker*), cmpPtrQSort ))
            return 1;

        return 0;
    }

    // If this is outside, and you can't see outside, it is not visible
    return group_draw.see_outside;
}

void vistrayClearOutsideTrayList(void)
{
    memset(vistray_outsideTrays, 0, sizeof(vistray_outsideTrays[0]) * vistray_outsideTrayCount);
    vistray_outsideTrayCount = 0;
}

void vistrayAddOutsideTray(DefTracker *tray_tracker)
{
    if (tray_tracker)
        dynArrayAddp(&vistray_outsideTrays, &vistray_outsideTrayCount, &vistray_outsideTrayMax, tray_tracker);
}

void vistrayDrawOutsideTrays(DrawParams *draw, Mat4 parent_mat)
{
    int i;
    draw->tray_child = true;
    for (i = 0; i < vistray_outsideTrayCount; i++)
    {
        VisTray *tray;
        tray = getOrAddTray(vistray_outsideTrays[i]);
        if (!tray || (tray->draw_id == group_draw.draw_id))
            continue;

        // draw
        drawTray(tray, VIS_CONNECTOR, draw, parent_mat, 1);
    }
    draw->tray_child = false;
}

void vistraySetUid(DefTracker *tray_tracker, int uid)
{
    VisTray *tray;

    if (!tray_tracker || !tray_tracker->def || !tray_tracker->def->is_tray)
        return;

    tray = getTray(tray_tracker);
    if (!tray)
        return;

    tray->uid = uid;
}

void vistraySetDetailUid(DefTracker *detail, int uid)
{
    VisTray *tray;
    int i, size;

    if (!detail || !detail->dyn_parent_tray)
        return;

    tray = detail->dyn_parent_tray;

    size = eaSize(&tray->details);
    for (i = 0; i < size; i++)
    {
        if (tray->details[i] == detail)
        {
            tray->detail_uids[i] = uid;
            return;
        }
    }
}

void vistrayNotifyDetailClosed(DefTracker *detail)
{
    if (!detail || !detail->dyn_parent_tray)
        return;
    vistrayFreeTrayDetail(detail, detail->dyn_parent_tray);
}


void vistrayResetUIDs(void)
{
    int i, j;
    for (i=0; i<eaSize(&vistray_trayarray); i++) {
        VisTray *tray = vistray_trayarray[i];
        tray->uid = -1;
        for (j=0; j<eaiSize(&tray->detail_uids); j++) {
            tray->detail_uids[j] = -1;
        }
    }
}

static bool areTraysConnected(DefTracker* trayDef0, DefTracker* trayDef1)
{
    VisTray** liveTrays = NULL;
    VisTray** visitedTrays = NULL;
    bool result = false;
    VisTray* goal = getOrAddTray(trayDef1);

    eaPush(&visitedTrays, getOrAddTray(trayDef0));
    eaPush(&liveTrays, getOrAddTray(trayDef0));

    while (eaSize(&liveTrays) > 0)
    {
        VisTray* currentTray = eaPop(&liveTrays);
        int i;

        for(i = 0; i < currentTray->portal_count; i++)
        {
            VisPortal *portal = &currentTray->portals[i];

            if (!portal->tray)
                continue;

            if (portal->tray == goal)
            {
                result = true;
                goto CLEANUP;
            }

            if (eaFind(&visitedTrays, portal->tray) >= 0)
                continue;

            eaPush(&visitedTrays, portal->tray);
            eaPush(&liveTrays, portal->tray);
        }
    }

CLEANUP:
    eaDestroy(&liveTrays);
    eaDestroy(&visitedTrays);
    return result;
}

bool vistrayIsSoundReachable(const Vec3 p0, const Vec3 p1)
{
    DefTracker* trayDef0 = groupFindInside(p0, FINDINSIDE_TRAYONLY, 0, 0);
    DefTracker* trayDef1 = groupFindInside(p1, FINDINSIDE_TRAYONLY, 0, 0);

    if (trayDef0 == trayDef1)
        return true;

    if (trayDef0 == NULL)
        return !trayDef1->def->sound_occluder;

    if (trayDef1 == NULL)
        return !trayDef0->def->sound_occluder;

    if (!trayDef0->def->sound_occluder && !trayDef1->def->sound_occluder)
        return true;

    return areTraysConnected(trayDef0, trayDef1);
}

