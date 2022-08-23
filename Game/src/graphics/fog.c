#include <utilitieslib/stdtypes.h>
#include "graphics/fog.h"
#include "group/grouputil.h"
#include <utilitieslib/components/earray.h>
#include "graphics/light.h"
#include "graphics/camera.h"
#include "graphics/sun.h"
#include <utilitieslib/utils/Color.h>
#include <utilitieslib/utils/utils.h>
#include "graphics/font.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/error.h>
#include "player/player.h"
#include "entity/entity.h"
#include "group/group.h"

#define NEAR_RATIO 0.2f
// #define NEAR_PERCENT 20
// #define NEAR_MUL ((100.f + NEAR_PERCENT)/100)
// #define NEAR_PERCENT_INV (100.f/NEAR_PERCENT)
typedef struct FogNodeEntry
{
    F32        fog_near, fog_far;
    F32        fog_radius;
    F32        fog_radius_squared;
    F32        fog_radius_near_mul;
    F32        fog_radius_near_squared;
    F32        fog_speed;
    Color    color[2];
    Vec3    pos;
    U32        in_tray:1;
} FogNodeEntry;

static FogNodeEntry **fogNodes;
static bool needsRefresh=true;

MP_DEFINE(FogNodeEntry);
FogNodeEntry *createFogNodeEntry(void) {
    MP_CREATE(FogNodeEntry, 8);
    return MP_ALLOC(FogNodeEntry);
}
void destroyFogNodeEntry(FogNodeEntry *entry) {
    MP_FREE(FogNodeEntry, entry);
}

static int fogFind_cb(GroupDefTraverser *def_trav)
{
    if (groupHidden(def_trav->def))
        return 0;

    if (def_trav->def->has_fog)
    {
        FogNodeEntry *entry = createFogNodeEntry();
        GroupDef *def = def_trav->def;
        getDefColorU8(def, 0, entry->color[0].rgba);
        getDefColorU8(def, 1, entry->color[1].rgba);
        entry->fog_far = def->fog_far;
        entry->fog_near = def->fog_near;
        entry->fog_radius = def->fog_radius;
        entry->fog_radius_squared = entry->fog_radius*entry->fog_radius;
//        entry->fog_radius_near_squared = entry->fog_radius*entry->fog_radius*NEAR_MUL*NEAR_MUL; // +10%
        entry->fog_radius_near_mul = 1 + NEAR_RATIO/def->fog_speed;
        entry->fog_radius_near_squared = entry->fog_radius_squared*entry->fog_radius_near_mul*entry->fog_radius_near_mul;
        entry->fog_speed = def->fog_speed;
        copyVec3(def_trav->mat[3], entry->pos);
        entry->in_tray = def_trav->isInTray;
        eaPush(&fogNodes, entry);
        return 1;
    }
    if (!def_trav->def->child_has_fog)
        return 0;
    return 1;
}

void fogGather(void)
{
    GroupDefTraverser traverser={0};

    eaClearEx(&fogNodes, destroyFogNodeEntry);

    groupProcessDefExBegin(&traverser, &group_info, fogFind_cb);
    needsRefresh=true;
    skyInvalidateFogVals();
}

#define SET(name, index, entry, distVal) \
    nodes##name[index] = entry; \
    dist##name[index] = distVal;
#define COPY(name, indexDst, indexSrc) SET(name, indexDst, nodes##name[indexSrc], dist##name[indexSrc])
#define LERP(v0, v1, w) ((v0)*(w)+(v1)*(1-(w)))

int fog_num_nodes_in, fog_num_nodes_near;

F32 fogUpdate(F32 scene_fog_near, F32 scene_fog_far, U8 scene_color[3], bool forceFogColor, bool doNotOverrideFogNodes, FogVals *fog_vals)
{
    int numFogNodes;
    Vec3 pos;
    int i, j;
    static F32 fog_near, fog_far, fog_speed = 1.0f;
    static Color color[2];
    static int fog_nodes_found=false;
    FogNodeEntry *nodesIn[3]={0};
    F32    distIn[3]={9e9,9e9};
    FogNodeEntry *nodesNear[2]={0};
    F32    distNear[2]={-9e9,-9e9};
    static Vec3 lastPos;
    static int lastInTray=0;
    int inTray = cam_info.last_tray && cam_info.last_tray->def && cam_info.last_tray->def->is_tray;

    // find nearby nodes
    copyVec3(cam_info.cammat[3], pos);
    // Only gather new nodes if we've moved at least some
    if (distance3Squared(lastPos, pos) > 0.01 || inTray != lastInTray) {
        needsRefresh = true;
    }

    if (game_state.info && (fog_num_nodes_in || fog_num_nodes_near)) { // yeah, this will be one frame delayed
        xyprintf(5,21, "fogNodes: in: %d  near: %d", fog_num_nodes_in, fog_num_nodes_near);
    }

    if (!needsRefresh) {
        if (fog_nodes_found) {
            skySetIndoorFogVals(0,fog_near,fog_far,forceFogColor?scene_color:color[0].rgba, fog_vals);
            skySetIndoorFogVals(1,fog_near,fog_far,forceFogColor?scene_color:color[1].rgba, fog_vals);
        }
        return fog_speed;
    }

    fog_num_nodes_in=fog_num_nodes_near=0;
    numFogNodes = eaSize(&fogNodes);
    for (i=0; i<numFogNodes; i++) {
        F32 distSquared;
        FogNodeEntry *entry = fogNodes[i];
        if (entry->in_tray != inTray) // Don't do this node if the fog is in a tray and I'm not!
            continue;
        distSquared = distance3Squared(pos, entry->pos);
        if (distSquared < entry->fog_radius_squared) {
            fog_num_nodes_in++;
            if (distSquared < distIn[0]) {
                COPY(In, 2, 1);
                COPY(In, 1, 0);
                SET(In, 0, entry, distSquared);
            } else if (distSquared < distIn[1]) {
                COPY(In, 2, 1);
                SET(In, 1, entry, distSquared);
            } else if (distSquared < distIn[2]) {
                SET(In, 2, entry, distSquared);
            }
        } else if (!nodesIn[0] && distSquared < entry->fog_radius_near_squared) {
            F32 weight;
            fog_num_nodes_near++;
            // Calc weighted distance
            weight = sqrt(distSquared) / entry->fog_radius;
            //assert(weight >= 1.0  && weight <=NEAR_MUL); // Weight should be between 1.0 and 1.1, add a litte range for rounding errors
//            weight = NEAR_PERCENT_INV*(NEAR_MUL - weight);
            weight = entry->fog_speed*(1.0f/NEAR_RATIO)*(entry->fog_radius_near_mul - weight);
            weight = CLAMP(weight, 0.001, 1.0);
            if (weight > distNear[0]) {
                COPY(Near, 1, 0);
                SET(Near, 0, entry, weight);
            } else if (distSquared > distNear[1]) {
                SET(Near, 1, entry, weight);
            }
        }
    }
    needsRefresh = false;
    copyVec3(pos, lastPos);

    // Calc effective fog
    if (nodesIn[0]) {
        F32 weight_total=0;
        Vec3 color0={0}, color1={0};
        fog_near = 0;
        fog_far = 0;
        fog_speed = 0;
        // Inside at least one node
        for (i=0; i<ARRAY_SIZE(nodesIn); i++) {
            if (nodesIn[i]) {
                F32 weight = nodesIn[i]->fog_radius - sqrt(distIn[i]);
                if (!nodesIn[1]) // If we have only one node, use a weight of 1 to stop floating point errors
                    weight = 1;
                fog_near += nodesIn[i]->fog_near*weight;
                fog_far += nodesIn[i]->fog_far*weight;
                fog_speed += nodesIn[i]->fog_speed*weight;
                for (j=0; j<3; j++) {
                    color0[j] += nodesIn[i]->color[0].rgba[j]*weight;
                    color1[j] += nodesIn[i]->color[1].rgba[j]*weight;
                }
                weight_total += weight;
            }
        }
        fog_near/=weight_total;
        fog_far/=weight_total;
        fog_speed/=weight_total;
        for (j=0; j<3; j++) {
            color[0].rgba[j] = color0[j]/weight_total;
            color[1].rgba[j] = color1[j]/weight_total;
        }
        fog_nodes_found = true;
    } else if (nodesNear[0]) {
        // Not in any, but near at least one
        if (!nodesNear[1]) {
            // Just one
            // lerp(nodesNear[0], default)
            fog_near = LERP(nodesNear[0]->fog_near, scene_fog_near, distNear[0]);
            fog_far = LERP(nodesNear[0]->fog_far, scene_fog_far, distNear[0]);
            fog_speed = LERP(nodesNear[0]->fog_speed, 1.0, distNear[0]);
            for (i=0; i<4; i++) {
                color[0].rgba[i] = LERP(nodesNear[0]->color[0].rgba[i], scene_color[i], distNear[0]);
                color[1].rgba[i] = LERP(nodesNear[0]->color[1].rgba[i], scene_color[i], distNear[0]);
            }
        } else {
            // Near two
            // lerp(nodesNear[0], nodesNear[1])
            F32 w0 = distNear[0] / (distNear[0] + distNear[1]);
            fog_near = LERP(nodesNear[0]->fog_near, nodesNear[1]->fog_near, w0);
            fog_far = LERP(nodesNear[0]->fog_far, nodesNear[1]->fog_far, w0);
            fog_speed = LERP(nodesNear[0]->fog_speed, nodesNear[1]->fog_speed, w0);
            for (i=0; i<4; i++) {
                color[0].rgba[i] = LERP(nodesNear[0]->color[0].rgba[i], nodesNear[1]->color[0].rgba[i], w0);
                color[1].rgba[i] = LERP(nodesNear[0]->color[1].rgba[i], nodesNear[1]->color[1].rgba[i], w0);
            }
            // lerp(result, default)
            w0 = MIN(distNear[0] + distNear[1], 1.0);
            fog_near = LERP(fog_near, scene_fog_near, w0);
            fog_far = LERP(fog_far, scene_fog_far, w0);
            fog_speed = LERP(fog_speed, 1.0, w0);
            for (i=0; i<4; i++) {
                color[0].rgba[i] = LERP(color[0].rgba[i], scene_color[i], w0);
                color[1].rgba[i] = LERP(color[1].rgba[i], scene_color[i], w0);
            }
        }
        fog_nodes_found = true;
    } else {
        // leave default values!
        fog_speed = 1.0f;
        fog_nodes_found=false;
    }

    if (inTray != lastInTray) // Snap fog values if moving in/out of a tray.  Better would be to detect any door movement
        skyInvalidateFogVals();
    lastInTray = inTray;

    if (fog_nodes_found) {
        if (!doNotOverrideFogNodes && fog_far > scene_fog_far) {
            // If the new fog_far is more than the scenes, assume the scene wants to bring in the fog
            F32 d = fog_far - scene_fog_far;
            fog_far = scene_fog_far;
            // Don't let it be any foggier than the scene fog if it wasn't to start with
            fog_near = MAX(MIN(scene_fog_near, fog_near), fog_near - d);
        }
        skySetIndoorFogVals(0,fog_near,fog_far,forceFogColor?scene_color:color[0].rgba, fog_vals);
        skySetIndoorFogVals(1,fog_near,fog_far,forceFogColor?scene_color:color[1].rgba, fog_vals);
    }

    return fog_speed;
}
