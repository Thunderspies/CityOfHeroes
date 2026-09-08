#include "group/group.h"
#include "groupdbmodify.h"
#include "groupdb_util.h"
#include "groupnetdb.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/utils.h>
#include <string.h>
#include "group/groupfilelib.h"
#include "dbcomm/dbcomm.h"
#include <utilitieslib/utils/file.h>
#include "seq/anim.h"
#include "group/groupgrid.h"
#include "groupnetsend.h"
#include "group/grouptrack.h"
#include "group/grouputil.h"
#include "groupjournal.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/StashTable.h>

GroupDef *protect_parent;
//I think this is only for checking out library pieces...

int checkoutMapFile(const char *fname)
{
    return 1;
}

int checkoutDefFileByName(const char *dir,const char *name)
{
    GroupFileEntry    *gf;
//    char            *s,libname[1000],libpath[1000];

    if (!groupInLibSub(name))
        return 0;
    gf = objectLibraryEntryFromObj(name);
    if (!gf)
    {
        char    *s2,buf[1000];

        s2 = strrchr(dir,'/');
        if (!s2)
            return 0;
        sprintf(buf,"%s%s.geo",dir,s2);
        gf = objectLibraryEntryFromFname(buf);
        if (!gf)
            return 0;
    }
    if (gf->checked_out)
        return 1;
    if (checkoutMapFile(dir))
        gf->checked_out = 1;
    return 1;    
/*
    strcpy(libname,gf->name);
    s = strrchr(libname,'.');
    if (!s)
        return 0;
    strcpy(s,".txt");
    sprintf(libpath, "%s/%s", fileDataDir(), libname);
    return 1;
    */
}

int checkoutDefFile(GroupDef *def)
{
    if (group_info.loading || !def->name)
        return 0;

    if (!groupInLib(def))
        return 0;  //The map file now gets checked out in worldHandleMsg, based on the layer changed // checkoutMapFile(db_state.map_name);
    return checkoutDefFileByName(def->file->fullname,def->name);
}

GroupDef *groupPartFind(DefTracker *ref,int depth,int *idxs,PartInfo *part_info)
{
    int            i;
    GroupDef    *def;
    Mat4        m;
    GroupDef    *parent = 0;

    parent = def = ref->def;
    if (!def)
        return 0;
    copyMat4(ref->mat,part_info->mat);
    for(i=0;i<depth;i++)
    {
        parent = def;
        if (def->entries && idxs[i] < def->count)
        {
            mulMat4(part_info->mat,def->entries[idxs[i]].mat,m);
            copyMat4(m,part_info->mat);
            def = def->entries[idxs[i]].def;
        }
    }
    part_info->def = def;
    return parent;
}


extern DefTracker * insertUniqueEntry(DefTracker * tracker);
extern DefTracker * getGreatestParent(DefTracker * tracker);

/********************************************************************************************/
// new stuff

void groupdbDirtyTracker(DefTracker * tracker) {
    extern int world_modified;
    int depth=0;
    PERFINFO_AUTO_START("groupdbDirtyTracker", 1);
    group_info.ref_mod_time++;

    while (tracker) {
        tracker->dirty=1;
        tracker->mod_time=group_info.ref_mod_time;                // update the modtimes so we know if
        tracker->def->mod_time=group_info.ref_mod_time;            // a client will need to be updated
        tracker->def->file->mod_time=group_info.ref_mod_time;    // on a tracker, def, or file
        if (!group_info.loading)
            tracker->def->file->unsaved=1;    
        tracker->def->saved=0;
        if (depth==0 && tracker->parent)                        // if it was passed in at the root level
            checkoutDefFile(tracker->parent->def);                // then we don't want to check it out
        tracker=tracker->parent;                                // because the def itself didn't change
        depth++;
    }

    world_modified=1;
    PERFINFO_AUTO_STOP();
}

static bool defOrDescendentMustBeUnique(GroupDef *def)
{
    return def->has_cubemap || def->child_cubemap;
}

void groupdbInstanceNormalExplode(DefTracker *tracker, cStashTable newnames);

static void explodeChildren(DefTracker * tracker, bool force, cStashTable newnames ) 
{
    int i;
    bool journaled = false;

    for(i = 0; i < tracker->count; i++)
    {
        if (force || defOrDescendentMustBeUnique(tracker->entries[i].def))
        {
            if (!journaled)
            {
                journalDef(tracker->def);
                journaled = true;
            }

            groupdbInstanceNormalExplode(&tracker->entries[i], newnames);
        }
    }
}

void groupdbInstanceNormalExplode(DefTracker *tracker, cStashTable newnames)
{
    bool isLibSub = groupIsLibSub(tracker->def);

    if(isLibSub || defOrDescendentMustBeUnique(tracker->def))
    {
        const char *oldname = tracker->def->name;
        const char *newname;

        if(!stashFindPointerConst(newnames, oldname, &newname))
        {
            tracker->def = groupDefDup(tracker->def, NULL);
            newname = tracker->def->name;
            assert(stashAddPointerConst(newnames, oldname, newname, false));

            trackerOpen(tracker);
            explodeChildren(tracker, isLibSub, newnames);
        }
        else
        {
            // we've already duplicated this def, get the existing duplicate
            // we've already instanced its children
            // groupdbInstanceNormal() will do further instancing as necessary
            tracker->def = groupDefFindWithLoad(newname);
            trackerClose(tracker); // any instanced children are wrong, refresh
        }

        // modify in parent def. parent must exist. parent was journaled by the caller.
        tracker->parent->def->entries[tracker - tracker->parent->entries].def = tracker->def;
        groupdbDirtyTracker(tracker);
    }
    else // we've hit a group external to this lib piece, let it be
    {
        assert(groupInLib(tracker->def)); // somehow a map group got in a library piece...
    }
}

void groupdbInstanceNormal(DefTracker *tracker, int *idxs, int depth)
{
    int lib_piece = groupInLib(tracker->def);

    if(lib_piece || groupDefRefCount(tracker->def) > 1)
    {
        assert(!groupIsLibSub(tracker->def)); // this library piece should have already been exploded

        tracker->def = groupDefDup(tracker->def,NULL);
        if(tracker->parent)
        {
            journalDef(tracker->parent->def);
            tracker->parent->def->entries[tracker - tracker->parent->entries].def = tracker->def;
        }

        trackerOpen(tracker);
        if((lib_piece || tracker->def->child_cubemap) && tracker->count)
        {
            static cStashTable s_newnames;

            if(s_newnames)
                stashTableClearConst(s_newnames);
            else
                s_newnames = stashTableCreateWithStringKeys(0, StashDefault);

            explodeChildren(tracker, lib_piece, s_newnames);
        }
    }

    if(depth)
    {
        trackerOpen(tracker);
        assert(INRANGE0(*idxs, tracker->count)); // somehow the index became invalid?
        groupdbInstanceNormal(&tracker->entries[*idxs], idxs+1, depth-1);
    }

    groupdbDirtyTracker(tracker);
    if(!tracker->parent) // clean up the whole tree
        trackerClose(tracker);
}

void groupdbInstanceLibrary(TrackerHandle *handle)
{
    DefTracker *tracker = trackerFromHandle(handle);

    if((groupInLib(tracker->def) && strchr(tracker->def->name,'&')==NULL) || groupDefRefCount(tracker->def)>1)
    {
        journalDef(tracker->def);
        if(tracker->parent)
            journalDef(tracker->parent->def);

        while(tracker && !groupInLib(tracker->def))
            tracker = tracker->parent;
        if(!tracker)
            tracker = trackerFromHandle(handle);
        moveDefsToLib(tracker->def);
    }

    groupdbDirtyTracker(tracker);
    trackerClose(tracker);
}

DefTracker* groupdbInstanceTracker(DefTracker *tracker, UpdateMode mode)
{
    TrackerHandle handle = {0};
    if(!tracker)
        return NULL;

    trackerHandleFillNoAllocNoCheck(tracker, &handle);

    switch(mode)
    {
        // normal mode means that instance anything that is a library pieces
        // or that isn't a library piece but that occurs more than once in a
        // map, this way we're only affecting the one thing we're working on
        xcase UM_NORMAL: // this may come through with noerrcheck
            groupdbInstanceNormal(groupRefFind(handle.ref_id), handle.idxs, handle.depth);

        // since we are changing the original version of everything, we just
        // modify the def and we're done, though we still journal everything
        xcase UM_ORIGINAL:

        // in this case we only modify the library pieces we are working in,
        // so when we instance things those things need to go in the library
        xcase UM_LIBRARY:
            groupdbInstanceLibrary(&handle);

        xdefault:
            assert(0);
    }
    STATIC_INFUNC_ASSERT(UM_COUNT == 3);

    return trackerFromHandle(&handle);
}

void groupdbReactivateDirtyRefs(void)
{
    int i;
    for(i = 0; i < group_info.ref_max; i++)
    {
        if(group_info.refs[i] && group_info.refs[i]->def && group_info.refs[i]->dirty)
        {
            PERFINFO_AUTO_START("groupdbReactivateRef", 1);
            groupRefGridDel(group_info.refs[i]);
            groupRefActivate(group_info.refs[i]);
            group_info.refs[i]->dirty=0;
            PERFINFO_AUTO_STOP();
        }
    }
}

//    parent is the GroupDef to add the child to
void groupdbAddToGroupOld(GroupDef * parent,GroupDef * child) {
    GroupEnt * ent;
    PERFINFO_AUTO_START("groupdbAddToGroup", 1);
    journalDef(parent);
    parent->entries=realloc(parent->entries,(parent->count+1) * sizeof(GroupEnt));
    ent=&parent->entries[parent->count++];
    memset(ent,0,sizeof(*ent));
    ent->def=child;
    ent->mat = mpAlloc(group_info.mat_mempool);
    copyMat4(unitmat,ent->mat);
    PERFINFO_AUTO_STOP();
}

void groupdbAddToGroup(GroupDef * parent,GroupDef * child,int index) {
    GroupEnt * ent;
    PERFINFO_AUTO_START("groupdbAddToGroup", 1);
    journalDef(parent);
    if (index<0 || index>=parent->count)
        index=parent->count;
    parent->entries=realloc(parent->entries,(parent->count+1) * sizeof(GroupEnt));
    memmove(&parent->entries[index+1],&parent->entries[index],(parent->count-index)*sizeof(GroupEnt));
    parent->count++;
    ent=&parent->entries[index];
    memset(ent,0,sizeof(*ent));
    ent->def=child;
    ent->mat = mpAlloc(group_info.mat_mempool);
    copyMat4(unitmat,ent->mat);
    PERFINFO_AUTO_STOP();
}

void groupdbMoveObject(DefTracker *tracker, const char *name, Mat4 mat)
{
    PERFINFO_AUTO_START("groupdbMoveObject", 1);
    if(tracker->parent)
    {
        DefTracker * parent=tracker->parent;
        Mat4 buf,rel;
        int index=tracker-tracker->parent->entries;
        journalDef(tracker->parent->def);
        invertMat4Copy(parent->mat,buf);
        mulMat4(buf,mat,rel);
        copyMat4(rel,parent->def->entries[index].mat);
        if(name[strlen(name)-1] != '&') // hack to make noerrorcheck play nice on subgroups
            parent->def->entries[index].def=groupDefFindWithLoad(name);
        trackerClose(parent);                    // close and open this tracker so it's children are correct
        trackerOpen(parent);                    // it will get fixed in the collision grid later
        groupdbDirtyTracker(parent);
    }
    else
    {
        copyMat4(mat,tracker->mat);
        if(name[strlen(name)-1] != '&') // hack to make noerrorcheck play nice on subgroups
            tracker->def=groupDefFindWithLoad(name);
        groupdbDirtyTracker(tracker);
    }
    PERFINFO_AUTO_STOP();
}

TrackerHandle ** groupdbDeletedTrackers;

void groupdbMarkAllDeletedTrackers() {
    int i;
    for (i=0;i<eaSize(&groupdbDeletedTrackers);i++) {
        DefTracker * tracker=trackerFromHandle(groupdbDeletedTrackers[i]);
        tracker->deleteMe=1;
    }
}

//    parent is the tracker that will inherit this new object as a child, if NULL the
//        object will go into the root level
//    defName is the name of the groupDef that will be put there
//    mat is the absolute matrix of the new object
//    this function returns a pointer to the new DefTracker, which is valid
DefTracker * groupdbPasteObject(DefTracker * parent,const char * defName,Mat4 mat, UpdateMode mode, bool isCopy) {
    GroupDef * newDef;
    DefTracker * ret;

    PERFINFO_AUTO_START("groupdbPasteObject", 1);

    if (defName==NULL) {                        // if we were passed a NULL defname, make a new one
        char groupName[256];
        newDef=groupDefNew(0);
        groupMakeName(groupName,"grp",0);
        groupDefName(newDef,0,groupName);
        newDef->name=strdup(groupName);
        newDef->count=0;                        // starts off with no entries, so make sure to add to it
        newDef->entries=NULL;
    }
    else
        newDef=groupDefFindWithLoad(defName);    // get the GroupDef we want to instance, load it if necessary
    assert(newDef);                                // something went wrong if we can't find the def
    if (parent==NULL) {                            // if parent is NULL, then we paste into the root level...
        parent=groupRefNew();                    // so make a new tracker for this
        parent->def=newDef;
        parent->dirty=1;
        copyMat4(mat,parent->mat);
        ret=parent;
    } else {
        Mat4 buf,rel;
        int index=parent->def->count;
        //groupdbMarkAllDeletedTrackers();
        //for (index=0;index<parent->count;index++)
        //    if (parent->entries[index].deleteMe)
        //        break;
        invertMat4Copy(parent->mat,buf);
        mulMat4(buf,mat,rel);
        groupdbAddToGroup(parent->def,newDef,index);    // add the new def to the parent's list of children

        copyMat4(rel,parent->def->entries[index].mat);
        trackerClose(parent);                    // close and open this tracker so it's children are correct
        trackerOpen(parent);                    // it will get fixed in the collision grid later
        ret=&parent->entries[parent->count-1];
        groupdbDirtyTracker(parent);
    }

    if (isCopy && defOrDescendentMustBeUnique(ret->def))
        ret = groupdbInstanceTracker(ret, mode);

    if (parent!=NULL)
        moveDefsToLib(parent->def);

    PERFINFO_AUTO_STOP();
    return ret;
}

void groupdbDeleteMyChild(DefTracker * tracker) {
    PERFINFO_AUTO_START("groupdbQueueDeleteObject", 1);

    if (tracker!=NULL) {
        tracker->deleteMyChild=1;
        groupdbDeleteMyChild(tracker->parent);
    }
    PERFINFO_AUTO_STOP();
}

void groupdbQueueDeleteObject(DefTracker * tracker) {
    PERFINFO_AUTO_START("groupdbQueueDeleteObject", 1);
    eaPush(&groupdbDeletedTrackers,trackerHandleCreate(tracker));
    if (tracker->parent)
        journalDef(tracker->parent->def);
    groupdbDirtyTracker(tracker);
    PERFINFO_AUTO_STOP();
}

void groupdbDeleteGroupDefEntry(GroupDef * def,int index) {
    GroupEnt    *entries;
    PERFINFO_AUTO_START("groupdbDeleteGroupDefEntry", 1);
    entries = malloc((def->count - 1) * sizeof(GroupEnt));
    memcpy(entries,def->entries,index * sizeof(GroupEnt));
    memcpy(&entries[index],&def->entries[index+1],(def->count - index - 1) * sizeof(GroupEnt));
    //MW GROUPENT HANDING OFF THE MATS, a little weird, but OK
    free(def->entries);
    def->entries = entries;
    def->count--;
    PERFINFO_AUTO_STOP();
}

int groupdbPruneGroupDef(GroupDef *def)
{
    int i;
    for(i = def->count-1; i >= 0; --i)
        if(!def->entries[i].def || groupdbPruneGroupDef(def->entries[i].def))
            groupdbDeleteGroupDefEntry(def,i); // marked for deletion, or no longer significant
    return !def->count && !def->model; // i have children or geometry, prune me
}

static int handleComp(const void* aData, const void* bData) {
    const TrackerHandle ** a = (const TrackerHandle **)aData;
    const TrackerHandle ** b = (const TrackerHandle **)bData;
    return -trackerHandleComp(*a,*b);
}

int g_deleting_trackers;
static void trackerHandleDestroyCallback(void* arg0)
{
    trackerHandleDestroy((TrackerHandle *)arg0);
}

void groupdbDeleteFlaggedTrackers(void)
{
    int i;
    PERFINFO_AUTO_START("groupdbDeleteFlaggedTrackers", 1);

    eaQSort(groupdbDeletedTrackers, handleComp);

    g_deleting_trackers = 1;
    for(i = 0; i < eaSize(&groupdbDeletedTrackers); i++)
    {
        DefTracker *tracker = trackerFromHandle(groupdbDeletedTrackers[i]);
        DefTracker *parent = tracker->parent;
        if(parent)
        {
            // mark the corresponding def for pruning, and invalidate this tracker
            int idx = tracker - parent->entries;
            assert(tracker->def == parent->def->entries[idx].def);
            parent->def->entries[idx].def = NULL;
//             groupdbDeleteGroupDefEntry(parent->def, idx);
            ZeroStruct(tracker);
            trackerClose(parent);
        }
        else
        {
            // this is a root tracker, get rid of it now
            trackerClose(tracker);
            groupRefGridDel(tracker);
            tracker->def = NULL;
        }
    }
    g_deleting_trackers = 0;

    eaDestroyEx(&groupdbDeletedTrackers, trackerHandleDestroyCallback);

    PERFINFO_AUTO_STOP();
}

void groupdbAttachObject(TrackerHandle * handle,DefTracker * parent, UpdateMode mode) {
    DefTracker * tracker;
    PERFINFO_AUTO_START("groupdbAttachObject", 1);
    tracker=trackerFromHandle(handle);
    groupdbPasteObject(parent,tracker->def->name,tracker->mat,mode, false);
    tracker=trackerFromHandle(handle);
    groupdbQueueDeleteObject(tracker);
    PERFINFO_AUTO_STOP();
}

void groupdbUngroupObject(TrackerHandle * handle, UpdateMode mode) {
    DefTracker * tracker;
    int i;
    PERFINFO_AUTO_START("groupdbUngroupObject", 1);
    tracker = trackerFromHandle(handle);
    if (tracker->parent)
        journalDef(tracker->parent->def);

    for (i=0;i<tracker->count;i++) {
        groupdbPasteObject(tracker->parent,tracker->entries[i].def->name,tracker->entries[i].mat,mode,false);
        tracker = trackerFromHandle(handle);
    }
    groupdbQueueDeleteObject(tracker);
    PERFINFO_AUTO_STOP();
}

// prep the rootDef by setting all of its defs to correspond with the trackers that are there
void groupdbEditTransactionBegin(const char *cmd)
{
    PERFINFO_AUTO_START("groupdbEditTransactionBegin", 1);
    journalBeginEntry(cmd);
    PERFINFO_AUTO_STOP();
}

// if anything was added at the root level we have to make sure the root trackers are 
// there, since things are handled slightly differently (though I don't understand
// how differently)
// eventually this should fuse with the EndEditTransaction function
void groupdbEditTransactionEnd() {
    int i;
    printf("groupdbEditTransactionEnd\n");
    PERFINFO_AUTO_START("groupdbEditTransactionEnd", 1);
    journalEndEntry();
    groupdbDeleteFlaggedTrackers();
    groupdbReactivateDirtyRefs();

    for (i=0;i<group_info.ref_count;i++)
    {
        PERFINFO_AUTO_START("groupdbPruneGroupDef", 1);
        if(group_info.refs[i]->def && groupdbPruneGroupDef(group_info.refs[i]->def))
        {
            trackerClose(group_info.refs[i]);
            groupRefGridDel(group_info.refs[i]);
            group_info.refs[i]->def = NULL;
        }
        PERFINFO_AUTO_STOP();
    }

    PERFINFO_AUTO_STOP();
}

bool groupdbHideForGameplay(GroupDef *def, int value) {
    if (!def)
        return false;

    value = value ? 1 : 0;
    if (def->hideForGameplay != value) {
        def->hideForGameplay = value;

        markDynamicDefChange(def);
    }

    return true;
}

