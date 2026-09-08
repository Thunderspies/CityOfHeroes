#include "group/groupgrid.h"
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include "group/group.h"
#include "group/grouptrack.h"
#include "gridcoll/gridcoll.h"
#include "seq/anim.h"
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/utils.h>
#include "group/grouputil.h"
#include "group/groupfileload.h"
#include "gridcoll/gridfind.h"
#include "bases/bases.h"

#if CLIENT
#include <utilitieslib/utils/timing.h>
#include "graphics/groupdraw.h"
#include "graphics/vistraygrid.h"
#endif

extern GroupInfo    *group_ptr;
extern Grid            obj_grid;

static void collGridAdd(Grid *grid,void *ptr,Mat4 pmat,Vec3 pmin,Vec3 pmax,F32 radius,DefTracker *tracker,int is_group)
{
    Vec3        min,max,dv;
    F32            boxvol,radvol;
    GroupDef    *def = tracker->def;
    int            j;
    
    if(    !def ||
        def->model && !def->count && !*(int*)&def->model->grid.size ||
        def->pre_alloc)
    {
        return;
    }
    
    groupGetBoundsTransformed(pmin, pmax, pmat, min, max);
    
    for(j=0;j<3;j++)
    {
        if(max[j] - min[j] < def->radius * 2)
        {
            break;
        }
    }
    if(j == 3)
    {
        // The transformed bounding box is LARGER than the diameter in all dimensions, so use the radius.
        
        Vec3 mi;
        mulVecMat4(def->mid,pmat,mi);
        for(j=0;j<3;j++)
        {
            max[j] = mi[j] + def->radius;
            min[j] = mi[j] - def->radius;
        }
    }
    
    subVec3(max,min,dv);
    boxvol = dv[0] * dv[1] * dv[2];
    radvol = radius * 2;
    radvol = radvol * radvol * radvol;
    
    gridAdd(grid,ptr,min,max,is_group,&tracker->grids_used);
}

void collGridAddGroup(Grid *grid,DefTracker *ref,void *tracker)
{
    GroupDef    *def;

    def = ref->def;
    collGridAdd(grid,ref,ref->mat,def->min,def->max,def->radius,tracker,1);
}

void groupGridActivate(DefTracker *parent_tracker,int group_id,Grid *grid)
{
    GroupEnt    *ent;
    DefTracker    *tracker;
    GroupDef    *def;
    int            i;

    // I commented this out again because apparently it's causing a lot of
    // game crashes. Not sure exactly why that is so, but we need a fix ASAP.
//    trackerClose(parent_tracker);
    trackerOpen(parent_tracker);
    def = parent_tracker->def;

    if (!def || (def->file && def->pre_alloc))
        assert(0);

    if (def->model || def->has_volume)
    {
        collGridAdd(grid,parent_tracker,parent_tracker->mat,def->min,def->max,def->radius,parent_tracker,0);
    }
    assert(def->count == parent_tracker->count);
    if (!def->has_volume)
    {
        for(i=0;i<def->count;i++)
        {
            ent = &def->entries[i];
            tracker = &parent_tracker->entries[i];
            if (ent->def)
            {
                tracker->def = ent->def;
                collGridAdd(grid,tracker,tracker->mat,ent->def->min,ent->def->max,tracker->def->radius,tracker,1);
            }
        }
    }
}

DefTracker *createCollTracker(const char * anmfilename, GroupDef * worldGroupPtr, const Mat4 mat)
{
    GroupDef    *def = 0;
    DefTracker    *tracker = 0;
    Model    *model = 0;

    //IF I'm supposed to be using the GEO_COLLSION file in the default graphics file.
    if( anmfilename )
    {
#ifdef CLIENT
        model = modelFind("GEO_Collision",anmfilename,LOAD_NOW,GEO_INIT_FOR_DRAWING | GEO_USED_BY_WORLD);
#else
        model = modelFind("GEO_Collision",anmfilename,LOAD_NOW,GEO_DONT_INIT_FOR_DRAWING | GEO_USED_BY_WORLD);
#endif
        if (model)
        {
            def = calloc(sizeof(*def),1);
            def->model = model;
        }
    }
    else if( worldGroupPtr ) //if you are supposed to be using the worldGroup.
    {
        def = worldGroupPtr;
    }

    if( def )
    {
        //TO DO do I need to do this for the worldGroupPtr
        //assert(!def->is_tray || def->model); //Why this assert?
        groupSetBounds(def);
        groupSetVisBounds(def);
        def->init = 1;

        tracker = calloc(sizeof(*tracker),1);
        tracker->def = def;
        copyMat4(mat,tracker->mat);
        groupRefActivate(tracker);
    }
    return tracker;

}

void groupRefGridDelEx(DefTracker *ref,Grid *grid)
{
    int        i;

    gridClearAndFreeIdxList(grid,&ref->grids_used);
    if (ref->entries)
    {
        for(i=0;i<ref->count;i++)
            groupRefGridDelEx(&ref->entries[i],grid);
        free(ref->entries);
        ref->entries = 0;
        ref->count = 0;
    }
}

void groupRefGridDel(DefTracker *ref)
{
    groupRefGridDelEx(ref,&obj_grid);
}

void groupGridDelEx(DefTracker *ref,Grid *grid)
{
    int        i;

    gridClearAndFreeIdxList(grid,&ref->grids_used);
    if (ref->entries)
    {
        for(i=0;i<ref->count;i++)
            groupGridDelEx(&ref->entries[i],grid);
    }
}

void groupGridDel(DefTracker *ref)
{
    groupGridDelEx(ref,&obj_grid);

#if CLIENT
    trayGridDel(ref);
#endif
}

void groupUnpackGrid(Grid *grid,DefTracker *ref)
{
    groupGridDelEx(ref,grid);
    groupGridActivate(ref,0,grid);
}

void groupRefActivate(DefTracker *ref)
{
    if (!ref->def || ref->grids_used)
        return;
    groupLoadIfPreload(ref->def);
    trackerSetMid(ref);
    collGridAddGroup(&obj_grid,ref,ref);

#if CLIENT
    trayGridRefActivate(ref);
#endif
}

DefTracker *groupFindOnLine(Vec3 start,Vec3 end,Mat4 mat,bool* hitWorld,int use_notselectable)
{
    CollInfo    coll;
    DefTracker    *tracker;

    coll.node_callback = 0;
    if (!collGrid(0,start,end,&coll,0,(use_notselectable?COLL_NOTSELECTABLE:0)|COLL_DISTFROMSTART|COLL_PLAYERSELECT))
    {
        *hitWorld = false;
        return 0;
    }

    *hitWorld = true;

    copyVec3(coll.mat[3],end);

    if(mat)
        copyMat4(coll.mat, mat);

    tracker = coll.node;
    if (!tracker || !tracker->def)
        return 0; 
    for(;tracker;tracker = tracker->parent)
    {
        if (tracker->def->properties || tracker->def->breakable || tracker->ent_id) //breakable is redundant, since it's only set by "Health" property...
            break;
    }

    return tracker;
}

FindInsideOpts glob_find_type;
VolumeList * glob_volumeList;  //assumed to be cleared except last frame info which is assumed correct
int glob_volumesChanged;
int glob_neighborhoodVolumeIdx;
int glob_materialVolumeIdx;
int glob_indoorVolumeIdx;

int findInsideTest(void* trackerData, int backside)
{
    DefTracker * tracker = (DefTracker *)trackerData;
    int        ok=0;
    FindInsideOpts find_type = glob_find_type;

    if (find_type==FINDINSIDE_TRAYONLY && tracker->def->is_tray )// && !tracker->def->outside) //Removing this might break something, but we needed to, and can't remember why the check is here anyway
    {
        if (!backside) //Tray's polys face inward, other volume's polys face outward 
            return 1;
    }
    else if (find_type==FINDINSIDE_TRAYONLY && tracker->def->has_volume && tracker->def->entries && tracker->def->entries[0].def && tracker->def->entries[0].def->is_tray)
    {
        if (backside)
            return 1;
    }
    else if (find_type==FINDINSIDE_VOLUMETRIGGER && (tracker->def->volume_trigger || tracker->def->has_volume))
    {
        if (backside) //Normal volume's polys face outward 
        {
            //Add the Volume tracker you found to a Volume List, because now you can be inside more than one volume at once! (should always have a volumeList if you get here)
            if( glob_volumeList)
            {
                bool shouldskip=false;
                Volume * newVolume;
                int i, alreadyHit = 0;

                //This loop is because of a bug in the collision code that sometimes finds the same collision multiple times
                //(might be fixed now)
                for( i = 0 ; i < glob_volumeList->volumeCount ; i++ )
                {
                    if( glob_volumeList->volumes[i].volumeTracker == tracker)
                        alreadyHit = 1;
                }

#                if CLIENT
                    if (!alreadyHit) {
                        PERFINFO_AUTO_START("trackerIsHiddenFromBurningBuilding", 1);
                            // Check to see if this volume is hidden by the burning buildings tech
                            shouldskip = trackerIsHiddenFromBurningBuilding(tracker, NULL);
                        PERFINFO_AUTO_STOP();
                    }
#                endif

                if( !alreadyHit && !shouldskip ) 
                {
                    DefTracker * materialVolumeFoundByProperty = 0;
                    int goodMaterialVolumeAlreadyFound = 0;

                    newVolume = dynArrayAdd(&glob_volumeList->volumes,sizeof(glob_volumeList->volumes[0]),&glob_volumeList->volumeCount,&glob_volumeList->volumeCountMax,1);
                    newVolume->volumeTracker = tracker;

                    //Flag if anything has changed to speed up finding if you've entered a new volume for Shannon's badge callback
                    if( glob_volumeList->volumeCount > glob_volumeList->volumeCountLastFrame )
                        glob_volumesChanged = 1;
                    else if( newVolume->volumeTracker != glob_volumeList->volumesLastFrame[glob_volumeList->volumeCount-1].volumeTracker )
                        glob_volumesChanged = 1;
        
                    //////////////Bookkeeping for the Volume List

                    //Search up the tracker tree for volumePropertiesTracker if there is one -- this will be where the properties associated with this volume will be
                    //If you find a different volume, quit; any properties above that will be for it, not you.
                    // (Also look for neighborhood tag)
                    newVolume->volumePropertiesTracker = 0; 
                    {
                        DefTracker    * myTracker;
                        for( myTracker = tracker ; myTracker ; myTracker = myTracker->parent)
                        {
                            if( myTracker != tracker && myTracker->def->volume_trigger ) 
                                break;

                            //Do we really need the NamedVolume string compare
                            if( myTracker->def->properties )
                            {
                                if( groupDefFindPropertyValue(myTracker->def,"NamedVolume") )
                                    newVolume->volumePropertiesTracker = myTracker;

                                //If the properties are neighborhood, this is your neighborhood volume so catch that.
                                if( groupDefFindPropertyValue(myTracker->def,"NHoodName") )
                                {
                                    glob_neighborhoodVolumeIdx = i; //hold off on actually applying pointer since they can be realloced
                                    newVolume->volumePropertiesTracker = myTracker;
                                }

                                if( groupDefFindPropertyValue(myTracker->def,"SonicFencing") )
                                {
                                    glob_neighborhoodVolumeIdx = i; //hold off on actually applying pointer since they can be realloced
                                    newVolume->volumePropertiesTracker = myTracker;
                                }

                                if( groupDefFindPropertyValue(myTracker->def,"IndoorLighting") )
                                {
                                    glob_indoorVolumeIdx = i;
                                    newVolume->volumePropertiesTracker = myTracker;
                                }

                                if( groupDefFindPropertyValue(myTracker->def,"MaterialVolume") )
                                {
                                    if( glob_materialVolumeIdx == -1 ||
                                        !groupDefFindPropertyValue(myTracker->def,"MaterialPriority") ||
                                        !groupDefFindPropertyValue(glob_volumeList->volumes[glob_materialVolumeIdx].volumeTracker->def, "MaterialPriority") )
                                    {
                                        materialVolumeFoundByProperty = myTracker;
                                    }
                                    else
                                    {
                                        int currentPropertyValue = atoi(groupDefFindPropertyValue(myTracker->def,"MaterialPriority"));
                                        int previousPropertyValue = atoi(groupDefFindPropertyValue(glob_volumeList->volumes[glob_materialVolumeIdx].volumeTracker->def, "MaterialPriority"));
                                        if( currentPropertyValue > previousPropertyValue )
                                        {
                                            materialVolumeFoundByProperty = myTracker;
                                        }
                                        else
                                        {
                                            goodMaterialVolumeAlreadyFound = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    //If this is you materialVolume, record that
                    //Note, prefer map set material volumes over trick set material volumes.  I don't know 
                    //If this is right
                    if ( materialVolumeFoundByProperty )
                        glob_materialVolumeIdx = i;
                    else if( !goodMaterialVolumeAlreadyFound && tracker->def->materialVolume ) //TO DO make this the only tag to check
                        glob_materialVolumeIdx = i;
                    //////////////End Bookkeeping for the Volume List

                    return 1;
                }
            }
        }
    }
    return 0;
}

extern Volume glob_dummyEmptyVolume;
VolumeList sharedVolumeList;    

DefTracker *groupFindInside(const Vec3 pos,FindInsideOpts find_type, VolumeList * volumeList, int * didVolumeListChangePtr )
{
    Vec3        start,end;
    int            ret;
    CollInfo    coll;
    U32            flags=COLL_NOTSELECTABLE | COLL_FINDINSIDE | COLL_BOTHSIDES | COLL_DISTFROMSTART | COLL_PORTAL;

    copyVec3(pos,start);
    copyVec3(pos,end);
    end[1] += 1;        // Bug bruce before you monkey with this

    if( volumeList )
    {
        volumeList->volumeCount = 0; //Reset volume struct 
        volumeList->materialVolume = &glob_dummyEmptyVolume;
        volumeList->neighborhoodVolume = &glob_dummyEmptyVolume;
        glob_materialVolumeIdx = -1;
        glob_neighborhoodVolumeIdx = -1;
        glob_indoorVolumeIdx = -1;
    }

    glob_find_type = find_type; //poor man's parameters to findInside callback.
    glob_volumesChanged = 0;
    glob_volumeList = volumeList;        // "" 
    coll.node_callback = findInsideTest;

    if (find_type == FINDINSIDE_ANYOBJ)
        flags |= COLL_FINDINSIDE_ANY;
    ret = collGrid(0,start,end,&coll,0,flags);
    glob_find_type = 0;

    //Optimization thing
    if( didVolumeListChangePtr )
    {
        assert( volumeList ); //you gotta have a volumeList, or why do you have a didVolumeListChangePtr
        if( volumeList->volumeCount != volumeList->volumeCountLastFrame )
            glob_volumesChanged = 1;
        *didVolumeListChangePtr = glob_volumesChanged;
    }
    //End optimization thin

    //Since dynArrayAlloc can realloc, I have ot wait to here to bind the materialVolume and the neighborhoodvolume
    if( volumeList )
    {
        if(    glob_materialVolumeIdx >= 0 )
            glob_volumeList->materialVolume = &volumeList->volumes[glob_materialVolumeIdx];
        if(    glob_neighborhoodVolumeIdx >= 0 )
            glob_volumeList->neighborhoodVolume = &volumeList->volumes[glob_neighborhoodVolumeIdx];
        if( glob_indoorVolumeIdx >= 0 )
            glob_volumeList->indoorVolume = &volumeList->volumes[glob_indoorVolumeIdx];
        else
            glob_volumeList->indoorVolume = 0;
    }

    //Debugging volumelist, temp
    {
        int i, h = 0;
        if(volumeList && volumeList->neighborhoodVolume->volumeTracker)
        {
            for(i=0;i<volumeList->volumeCount;i++)
            {
                if(volumeList->neighborhoodVolume==&volumeList->volumes[i])
                    h=1;
            }
            assert(h);
        }
    }
    //End debug

    if (ret)
        return coll.node;
    return 0;
}

// Find nearby door volume, yanked some code from groupFindInside
int groupFindDoorVolumeWithinRadius(const Vec3 pos, int radius)
{
    static VolumeList volumeList;
    Vec3        start,end;
    CollInfo    coll;

    copyVec3(pos,start);
    copyVec3(pos,end);
    end[1] += 1;        // Bug bruce before you monkey with this

    volumeList.volumeCount = 0; //Reset volume struct 
    volumeList.materialVolume = &glob_dummyEmptyVolume;
    volumeList.neighborhoodVolume = &glob_dummyEmptyVolume;
    glob_materialVolumeIdx = -1;
    glob_neighborhoodVolumeIdx = -1;
    glob_indoorVolumeIdx = -1;
    glob_find_type = FINDINSIDE_VOLUMETRIGGER;
    glob_volumesChanged = 0;
    glob_volumeList = &volumeList;
    coll.node_callback = findInsideTest;
    collGrid(0,start,end,&coll,radius,COLL_NOTSELECTABLE|COLL_FINDINSIDE|COLL_BOTHSIDES|COLL_DISTFROMSTART|COLL_PORTAL);
    glob_find_type = 0;

    if(    glob_materialVolumeIdx >= 0 )
        glob_volumeList->materialVolume = &volumeList.volumes[glob_materialVolumeIdx];
    if(    glob_neighborhoodVolumeIdx >= 0 )
        glob_volumeList->neighborhoodVolume = &volumeList.volumes[glob_neighborhoodVolumeIdx];
    if( glob_indoorVolumeIdx >= 0 )
        glob_volumeList->indoorVolume = &volumeList.volumes[glob_indoorVolumeIdx];
    else
        glob_volumeList->indoorVolume = 0;
    return (volumeList.materialVolume && volumeList.materialVolume->volumeTracker && volumeList.materialVolume->volumeTracker->def && volumeList.materialVolume->volumeTracker->def->door_volume);
}

void groupGridRecreate(void)
{
    int i;

    if (!group_ptr)
        return;

    // Remove everything
    for(i=0;i<group_ptr->ref_max;i++)
    {
        if (group_ptr->refs[i] && group_ptr->refs[i]->def)
        {
            groupGridDel(group_ptr->refs[i]);
        }
    }

//    gridFreeAll(&obj_grid);

    // Re-add
    for(i=0;i<group_ptr->ref_max;i++)
    {
        if (group_ptr->refs[i] && group_ptr->refs[i]->def)
        {
            groupRefActivate(group_ptr->refs[i]);
        }
    }

}

DefTracker *groupFindLightTracker(Vec3 pos)
{
    static VolumeList volumeList = {0};
    DefTracker *tracker;

    // find inside tray
    tracker = groupFindInside( pos, FINDINSIDE_TRAYONLY,0,0 ); //find the group you are inside right now

#ifdef CLIENT
    // find inside indoor lighting volume
    if (!tracker || !(tracker->def && tracker->def->has_ambient))
    {
        groupFindInside(pos,FINDINSIDE_VOLUMETRIGGER, &volumeList, 0 );
        if (volumeList.indoorVolume)
        {
            DefTracker *newtracker = volumeList.indoorVolume->volumePropertiesTracker;
            while (newtracker && !newtracker->light_grid)
                newtracker = newtracker->parent;
            if (newtracker)
                tracker = newtracker;
        }
    }
#endif

    // find inside base room
    if (!tracker && g_base.ref)
    {
        BaseRoom *room = baseRoomLocFromPos(&g_base, pos, NULL);
        if (room)
        {
            int i;
            for (i = 0; i < g_base.ref->count; i++)
            {
                DefTracker *newtracker = &g_base.ref->entries[i];
                if (newtracker && newtracker->def == room->def)
                    tracker = newtracker;
            }
        }
    }

    return tracker;
}

#if SERVER
typedef struct candidateVolume
{
    float xmin, xmax;
    float ymin, ymax;
    float zmin, zmax;
    int haspriority;
} candidateVolume;

static candidateVolume **s_volumes = NULL;

typedef struct overlapPoint
{
    float x;
    float y;
    float z;
} overlapPoint;

static overlapPoint **s_points = NULL;

static int curPointIndex;

static candidateVolume *cvolume_Create()
{
    candidateVolume *volume = malloc(sizeof(candidateVolume));
    assert(volume);
    memset(volume, 0, sizeof(candidateVolume));
    return volume;
}

static void cvolume_Destroy(candidateVolume *v)
{
    free(v);
}

static overlapPoint *opoint_Create()
{
    overlapPoint *point = malloc(sizeof(overlapPoint));
    assert(point);
    memset(point, 0, sizeof(overlapPoint));
    return point;
}

static void opoint_Destroy(overlapPoint *p)
{
    free(p);
}

static int cvolume_Handler(GroupDefTraverser* traverser)
{
    candidateVolume *volume;

    // We are only interested in nodes with properties.
    if(traverser->def->properties == NULL)
    {
        // We're only interested in subtrees with properties.
        if(!traverser->def->child_properties)
        {
            return 0;
        }

        return 1;
    }

    // Is this a MaterialVolume?
    if (groupDefFindPropertyValue(traverser->def, "MaterialVolume") == NULL)
    {
        return 1;
    }

    volume = cvolume_Create();
    volume->xmin = traverser->def->min[0] + traverser->mat[3][0];
    volume->ymin = traverser->def->min[1] + traverser->mat[3][1];
    volume->zmin = traverser->def->min[2] + traverser->mat[3][2];
    volume->xmax = traverser->def->max[0] + traverser->mat[3][0];
    volume->ymax = traverser->def->max[1] + traverser->mat[3][1];
    volume->zmax = traverser->def->max[2] + traverser->mat[3][2];
    volume->haspriority = groupDefFindPropertyValue(traverser->def, "MaterialPriority") != NULL;

    eaPush(&s_volumes, volume);

    return 1;
}

int isBetween(float lower, float upper, float candidate)
{
    return lower <= candidate && candidate <= upper;
}

// volumes overlap in a given dimension if either volume has either of it's bound planes for that dimension
// between the two bound planes of the other.
// There's probably a faster way to check this using sgn(upper2 - lower1) and sgn(upper1 - lower2).
// I just can't be bothered to work it out.  sgn(x) is defined as x ? x / abs(x) : 0.
static int overlapDimension(float lower1, float upper1, float lower2, float upper2)
{
    return isBetween(lower1, upper1, lower2) || isBetween(lower1, upper1, upper2) ||
        isBetween(lower2, upper2, lower1) || isBetween(lower2, upper2, upper1);
}

static float findOverlapCenter(float lower1, float upper1, float lower2, float upper2)
{
    float lower = MAX(lower1, lower2);
    float upper = MIN(upper1, upper2);
    return (lower + upper) / 2.0f;
}

static void scanForOverlappingVolumes()
{
    int i;
    int j;
    candidateVolume *volume1;
    candidateVolume *volume2;
    overlapPoint *point;

    for (i = eaSize(&s_volumes) - 1; i >= 1; i--)
    {
        volume1 = s_volumes[i];
        for (j = i - 1; j >= 0; j--)
        {
            // Volumes intersect if and only if they overlap in all three dimensions.  We also ignore intersections
            // if both volumes have priorities assigned.
            volume2 = s_volumes[j];
            if (overlapDimension(volume1->xmin, volume1->xmax, volume2->xmin, volume2->xmax) &&
                overlapDimension(volume1->ymin, volume1->ymax, volume2->ymin, volume2->ymax) &&
                overlapDimension(volume1->zmin, volume1->zmax, volume2->zmin, volume2->zmax) &&
                (!volume1->haspriority || !volume2->haspriority))
            {
                // set the point itself to the center of the "overlap box"
                point = opoint_Create();
                point->x = findOverlapCenter(volume1->xmin, volume1->xmax, volume2->xmin, volume2->xmax);
                point->y = findOverlapCenter(volume1->ymin, volume1->ymax, volume2->ymin, volume2->ymax);
                point->z = findOverlapCenter(volume1->zmin, volume1->zmax, volume2->zmin, volume2->zmax);
                eaPush(&s_points, point);
            }
        }
    }
}

static void cvolume_DestroyAdapter(void* arg0)
{
    cvolume_Destroy((candidateVolume *)arg0);
}

static void opoint_DestroyAdapter(void* arg0)
{
    opoint_Destroy((overlapPoint *)arg0);
}

void overlappingVolume_Load()
{
    GroupDefTraverser traverser = {0};
    curPointIndex = 0;
    if (s_volumes)
    {
        eaClearEx(&s_volumes, cvolume_DestroyAdapter);
        eaSetSize(&s_volumes, 0);
    }
    else
    {
        eaCreate(&s_volumes);
    }
    if (s_points)
    {
        eaClearEx(&s_points, opoint_DestroyAdapter);
        eaSetSize(&s_points, 0);
    }
    else
    {
        eaCreate(&s_points);
    }
    // Scan the map hierarchy looking for MaterialVolumes
    groupProcessDefExBegin(&traverser, &group_info, cvolume_Handler);
    // If we find any
    if (eaSize(&s_volumes))
    {
        // See if any of them overlap
        scanForOverlappingVolumes();
    }
}

int nextOverlapPoint(Vec3 point, int *current, int *count)
{
    // No points, all done
    if (eaSize(&s_points) == 0)
    {
        return 0;
    }
    // Sanity check curIndex
    if (curPointIndex < 0)
    {
        curPointIndex = 0;
    }
    if (curPointIndex >= eaSize(&s_points))
    {
        curPointIndex = eaSize(&s_points) - 1;
    }
    // return the point ...
    point[0] = s_points[curPointIndex]->x;
    point[1] = s_points[curPointIndex]->y;
    point[2] = s_points[curPointIndex]->z;
    if (current)
    {
        // ... Current index ...
        *current = curPointIndex + 1;
    }
    if (count)
    {
        // .. and count
        *count = eaSize(&s_points);
    }
    // Make currebnt index wrap.  This behavior is slightly different from the bad spawn code.  If the
    // design department complain, switch this to a simple ++curPointIndex, remove the - 1 from the conditional
    // clause in the second sanity check test, and then after the sanity check if (curPointIndex == eaSize(&s_points))
    // evaluates true, return 0.
    if (++curPointIndex >= eaSize(&s_points))
    {
        curPointIndex = 0;
    }
    return 1;
}
#endif