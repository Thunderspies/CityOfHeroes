#include "group/group.h"
#include <stdlib.h>
#include <string.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/error.h>
#include "ctype.h"
#include <utilitieslib/utils/mathutil.h>
#include <stdio.h>
#include <utilitieslib/utils/file.h>
#include "seq/anim.h"
#include "seq/tricks.h"
#include <utilitieslib/utils/utils.h>
#include "seq/gfxtree.h"
#include "group/grouptrack.h"
#include <utilitieslib/components/StashTable.h>
#include "group/groupProperties.h"
#include <utilitieslib/assert/assert.h>
#include "group/groupfilelib.h"
#include "entity/motion.h"
#include "gridcoll/grid.h"
#include "group/groupgrid.h"
#include <utilitieslib/network/crypt.h>
#include "group/groupdyn.h"
#include "NovodeX/NwWrapper.h"
#include "group/groupnovodex.h"
#if SERVER
#include "group/groupjournal.h"
#include "group/groupdbmodify.h"
#include "group/groupdynsend.h"
#include "group/groupjournal.h"
#endif
#include <utilitieslib/utils/strings_opt.h>
#include "group/grouputil.h"
#include "group/groupfileload.h"
#include "bases/bases.h"
#if CLIENT
#include "cmdparse/cmdgame.h"
#include "edit/edit_select.h"
#include "render/tex.h"
#include "graphics/vistray.h"
#include "NovodeX/NwWrapper.h"
#include "graphics/camera.h"
#include "edit/edit_cmd.h"
#include "graphics/gfxLoadScreens.h"
#include "bases/baseedit.h"
#include "graphics/sun.h"
#endif
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/components/EString.h>

GroupInfo            group_info,*group_ptr;
static StashTable s_texNames;

char *lastslash(char *s)
{
    int i = (int)strlen(s);

    for(;i>=0;i--)
    {
        if (s[i] == '/' || s[i] == '\\')
            return s + i;
    }
    return 0;
}

int groupRefShouldAutoGroup(DefTracker *ref)
{
    int iResult = 0;
    if (!ref || !ref->def)
        return 0;

    if ( ref->def->properties && stashFindInt(ref->def->properties, "AutoGroup", &iResult))
        return iResult;

    return (stricmp(ref->def->name, "grp_Geometry")==0);
}

// "maps/City_Zones/Trial_04_01/Trial_04_01.txt" -> "maps/City_Zones/Trial_04_01"
// "object_library/Streets_Ruined/Ruined_Cars/Ruined_cars.txt" -> "Streets_Ruined/Ruined_Cars"
void groupBaseName(const char *fname,char *basename)
{
    char    *s,buf[1000];

    if (!fname)
    {
        basename[0]=0;
        return;
    }
    strcpy(buf,fname);
    s = lastslash(buf);
    if (s)
        *s = 0;

    s = strstr(buf,"object_library");
    if (s)
        s += strlen("object_library/");
    else
        s = strstri(buf,"maps");
        
    if (!s)
        s = buf; // This happens with noname.txt, and that is all?

    strcpy(basename,s);
}

Model * groupModelFind(const char *name)
{
    GroupFileEntry    *gf;
    Model        *model;
    char            geoname[MAX_PATH];
    const char* cs;

    PERFINFO_AUTO_START("groupModelFind",1);

    cs = strrchr(name,'/');
    if (!cs++)
        cs = name;
    gf = objectLibraryEntryFromObj(cs);
    if (!gf)
    {
        PERFINFO_AUTO_STOP_CHECKED("groupModelFind");
        return 0;
    }
    changeFileExt(gf->name,".geo",geoname);
#if CLIENT
    model = modelFind(cs,geoname,LOAD_NOW, GEO_INIT_FOR_DRAWING | GEO_USED_BY_WORLD);
#else
    // Note: I changed the LOAD_NOW below to LOAD_HEADER, which basically means
    // load the geo_data stubs, but don't create the actual geo_data
    // since it's only used for collision triangles on the server side,
    // these are created when they are needed, which may be never if the ctris are
    // in shared memory
    model = modelFind(cs,geoname,LOAD_HEADER, GEO_DONT_INIT_FOR_DRAWING | GEO_USED_BY_WORLD);
#endif
    PERFINFO_AUTO_STOP_CHECKED("groupModelFind");
    return model;
}

int defContainCount(GroupDef *container,GroupDef *def)
{
    int        total=0,i;

    if (!container)
        return 0;
    if (container == def)
        return 1;
    for(i=0;i<container->count;i++)
        total += defContainCount(container->entries[i].def,def);
    return total;
}

int groupDefRefCount(GroupDef *def)
{
int        i,total=0;

    for(i=0;i<group_ptr->ref_count;i++)
    {
        if(group_ptr->refs[i])
        {
            total += defContainCount(group_ptr->refs[i]->def,def);
        }
    }
    return total;
}

GroupDef *groupDefFind(const char *name)
{
    const char        *s;
    GroupDef    *def;

    PERFINFO_AUTO_START("groupDefFind",1);

    s = strrchr(name,'/');
    if (!s++)
        s = name;
    stashFindPointer( group_ptr->groupDefNameHashTable,s, &def );

    PERFINFO_AUTO_STOP_CHECKED("groupDefFind");
    return def;
}

GroupDef *groupDefFindWithLoad(const char *objname)
{
    GroupDef    *def;
    char        *path;
    char        path_txt[MAX_PATH];

    def = groupDefFind(objname);
    if (def)
        return def;
    path = objectLibraryPathFromObj(objname);
    if (!path)
        return 0;
    changeFileExt(path,".txt",path_txt);
    groupLoadFull(path_txt);
    def = groupDefFind(objname);
    return def;
}

int isFxTypeName(const char *name)
{
    int        len;

    if (!name)
        return 0;
    len = (int)strlen(name);
    if (len >= 3 && stricmp(name + len - 3,".fx")==0)
        return 1;
    return 0;
}

static void updateVisBoundsFromChild(GroupDef * group, int i, F32 *highrad, F32 *maxrad, F32 *maxvis, F32 *maxsound) 
{
    Vec3 dv;
    F32 r, d;
    GroupEnt *ent = &group->entries[i];
    if (ent->def==NULL)
        return;

    if (ent->def->model && !(ent->def->model->flags & OBJ_FULLBRIGHT) && !(ent->def->model->trick && (ent->def->model->trick->flags1 & TRICK_ADDITIVE)) && !ent->def->lod_near && ent->def->model->radius > *highrad)
    {
        *highrad = ent->def->model->radius;
#if CLIENT
        group->child_high_lod_idx = i;
#endif
    }

    mulVecMat4(ent->def->mid,ent->mat,dv);
    subVec3(dv,group->mid,dv);
    r = lengthVec3(dv) + ent->def->radius + ent->def->shadow_dist;
    if (r > *maxrad)
        *maxrad = r;
    d = lengthVec3(dv) + (ent->def->vis_dist * ent->def->lod_scale);
    if (d > *maxvis)
        *maxvis = d;
    if (ent->def->has_ambient || ent->def->child_ambient)
        group->open_tracker = group->child_ambient = 1;
    if (ent->def->has_beacon || ent->def->child_beacon)
        group->child_beacon = 1;
    if (ent->def->has_light || ent->def->child_light)
        group->child_light = 1;
    if (ent->def->has_cubemap || ent->def->child_cubemap)
        group->child_cubemap = 1;
    if (ent->def->has_volume || ent->def->child_volume)
        group->child_volume= 1;
    if (ent->def->has_fog || ent->def->child_has_fog)
        group->child_has_fog = 1;
    if (ent->def->no_fog_clip || ent->def->child_no_fog_clip)
        group->child_no_fog_clip = 1;
    if (ent->def->has_properties || ent->def->child_properties)
        group->child_properties = 1;
    if (((ent->def->parent_fade || ent->def->child_parent_fade)) && !ent->def->lod_fadenode)
        group->child_parent_fade = 1;
    if (ent->def->is_tray)
        group->open_tracker = 1;
    if (ent->def->open_tracker)
        group->open_tracker = 1;
    if (ent->def->is_tray || ent->def->child_tray)
        group->child_tray = 1;
    if (ent->def->volume_trigger || ent->def->child_volume_trigger)
        group->child_volume_trigger = 1;

    if (group->editor_visible_only && !ent->def->editor_visible_only)
        group->editor_visible_only = 0;

    if (ent->def->sound_radius)
    {
        r = lengthVec3(dv) + ent->def->sound_radius;
        if (r > *maxsound)
            *maxsound = r;
    }
    if (ent->def->key_light && group->has_light)
        group->key_light = 1;
    if (ent->def->breakable || ent->def->child_breakable)
        group->child_breakable = group->open_tracker = 1;

    group->recursive_count += ent->def->recursive_count;
}

void groupSetVisBounds(GroupDef *group)
{
    int            i;
    Vec3        dv;
    F32            maxsound = 0,maxrad = 0,maxvis = 0;
    F32            highrad = -1;

    PERFINFO_AUTO_START("groupSetVisBounds",1);

    if (!group)
    {
        PERFINFO_AUTO_STOP();
        return;
    }
#if CLIENT
    if (group->pre_alloc)
    {
        PERFINFO_AUTO_STOP();
        return;
    }
#endif

    group->do_welding        = !!groupDefFindProperty(group, "DoWelding");

    // Clear flags that this function will set
    group->child_ambient = 0;
    group->child_beacon = 0;
    group->child_breakable = 0;
    group->child_has_fog = 0;
    group->child_volume = 0;
    group->child_no_fog_clip = 0;
    group->child_light = 0;
    group->child_parent_fade = 0;
    group->child_properties = 0;
    group->child_volume_trigger = 0;
    group->breakable = 0;
    group->child_tray = 0;
    group->recursive_count = group->count;

#if CLIENT
    group->child_high_lod_idx = -1;
#endif

    group->editor_visible_only = !group->model || group->has_fog || group->has_sound || 
        group->has_light || group->has_volume || group->has_cubemap;

    if (!group->editor_visible_only && group->model && group->model->trick)
        group->editor_visible_only = (group->model->trick->flags1 & TRICK_EDITORVISIBLE) || (group->model->trick->flags1 & TRICK_HIDE);
    if (group->has_fx)
        group->editor_visible_only = 0; // fx emitters need to be traversed so the fx will be drawn

    if (!group->lod_scale)
        group->lod_scale = 1.f;
    if (group->model)
    {
        Model *model = group->model;
        subVec3(model->max,model->min,dv);
        maxrad = lengthVec3(dv) * 0.5f + group->shadow_dist;
        if (!group->lod_far)
            groupSetLodValues(group);
        maxvis = group->lod_far + group->lod_farfade;
    }
    maxsound = group->sound_radius;

    if (group->properties)
    {
        if (stashFindPointer(group->properties,"Health",NULL))
            group->breakable = 1;
    }

    for(i=0;i<group->count;i++)
        updateVisBoundsFromChild(group, i, &highrad, &maxrad, &maxvis, &maxsound);

    if (isFxTypeName(group->type_name))
        group->has_fx = group->open_tracker = 1;
    group->vis_dist    = maxvis;
    if (!group->shadow_dist)
        group->shadow_dist = maxrad - group->radius;
    if (group->shadow_dist < 0)
        group->shadow_dist = 0;
    //printf("rold: %f   rnew: %f\n",group->radius,maxrad);
    //group->radius    = maxrad;
    group->sound_radius = maxsound;
    if (group->has_volume)
    {
        group->vis_dist    = 3000;
    }

    PERFINFO_AUTO_STOP();
}

F32        rad_count,box_count;

void groupGetBoundsTransformed(const Vec3 min, const Vec3 max, const Mat4 pmat, Vec3 minOut, Vec3 maxOut)
{
    #define MAXVAL 8e16
    
    int x, y, z;
    Vec3 extents[2];
    
    setVec3(minOut,MAXVAL,MAXVAL,MAXVAL);
    setVec3(maxOut,-MAXVAL,-MAXVAL,-MAXVAL);
    copyVec3(min,extents[0]);
    copyVec3(max,extents[1]);
    for(x=0;x<2;x++)
    {
        for(y=0;y<2;y++)
        {
            for(z=0;z<2;z++)
            {
                Vec3 pos;
                Vec3 tpos;
                int i;
                
                pos[0] = extents[x][0];
                pos[1] = extents[y][1];
                pos[2] = extents[z][2];
                mulVecMat4(pos,pmat,tpos);
                for(i=0;i<3;i++)
                {
                    minOut[i] = MIN(tpos[i],minOut[i]);
                    maxOut[i] = MAX(tpos[i],maxOut[i]);
                }
            }
        }
    }
}

void groupGetBoundsMinMax(GroupDef *def,const Mat4 mat,Vec3 min,Vec3 max)
{
    int        j;
    Vec3    minCheck,maxCheck;

    if (def==NULL)
        return;
    groupGetBoundsTransformed(def->min, def->max, mat, minCheck, maxCheck);

    // Check if the bounding box is smaller than the diameter.
    for(j=0;j<3;j++)
    {
        if(maxCheck[j] - minCheck[j] < def->radius*2)
            break;
    }
    if(j == 3)
    {
        // The bounding box is LARGER than the diameter in all dimensions, so use the radius.
        Vec3 mi;
        mulVecMat4(def->mid,mat,mi);
        for(j=0;j<3;j++)
        {
            max[j] = MAX(max[j],mi[j] + def->radius);
            min[j] = MIN(min[j],mi[j] - def->radius);
        }
    }
    else
    {
        for(j=0;j<3;j++)
        {
            max[j] = MAX(max[j],maxCheck[j]);
            min[j] = MIN(min[j],minCheck[j]);
        }
    }
}

void groupGetBoundsMinMaxNoVolumes(GroupDef *def,const Mat4 mat,Vec3 min,Vec3 max)
{
    int i;
    Model    *model;
    TrickNode    *trick;
    if (def->volume_trigger)
        return;

    model = def->model;
    if (model && !strEndsWith(model->name,"__PARTICLE"))
    {
        trick = model->trick;
        if(!(trick && trick->flags1 & TRICK_NOCOLL))
            groupGetBoundsMinMax(def,mat,min,max);
    }

    for (i=def->count-1; i >= 0; i--)
    {
        Mat4    child_mat;

        mulMat4(mat,def->entries[i].mat,child_mat);
        groupGetBoundsMinMaxNoVolumes(def->entries[i].def,child_mat,min,max);
    }
}

// returns 1 if it looks like objects were sent in non bottom-up order
int groupSetBounds(GroupDef *group)
{
    int            i,set=0;
    GroupEnt    *ent;
    Vec3        min = {8e8,8e8,8e8},max = {-8e8,-8e8,-8e8},mid,dv;
    F32            r,maxrad=0,ap_len=0;
    Model    *model;
    
    PERFINFO_AUTO_START("groupSetBounds",1);

    if (!group)
    {
        PERFINFO_AUTO_STOP();
        return 0;
    }
#if CLIENT
    if (group->pre_alloc)
    {
        PERFINFO_AUTO_STOP();
        return 1;
    }
#endif
    model = group->model;
    if (model)
    {
        copyVec3(model->min,min);
        copyVec3(model->max,max);
        subVec3(max,min,dv);
        ap_len = lengthVec3(dv) * 0.5;
        set = 1;
    }
    for(i=0;i<group->count;i++)
    {
        ent = &group->entries[i];
        if (ent->def==NULL)
            continue;
        groupGetBoundsMinMax(ent->def,ent->mat,min,max);
        set = 1;
    }
    if (group->has_volume)
    {
        set = 1;
        scaleVec3(group->box_scale, 0.5f, max);
        scaleVec3(max, -1, min);
        min[1] = 0;
        max[1] = group->box_scale[1];
    }
    if (!set)
    {
        zeroVec3(min);
        zeroVec3(max);
    }
    subVec3(max,min,mid);
    group->radius = lengthVec3(mid) * 0.5f;
    scaleVec3(mid,0.5f,mid);
    addVec3(mid,min,mid);
    copyVec3(max,group->max);
    copyVec3(min,group->min);
    copyVec3(mid,group->mid);

    if (!group->has_volume)
    {
        for(i=0;i<group->count;i++)
        {
            ent = &group->entries[i];
            if (ent->def==NULL)
                continue;
            mulVecMat4(ent->def->mid,ent->mat,dv);
            subVec3(dv,group->mid,dv);
            r = lengthVec3(dv) + ent->def->radius;
            if (r > maxrad)
                maxrad = r;
        }
    }
    if (maxrad)
        group->radius = maxrad;

    {
        Vec3    dv;
        F32        boxlen;

        subVec3(group->max,group->min,dv);
        boxlen = lengthVec3(dv) / 2;
        rad_count += group->radius;
        box_count += boxlen;
    }
    group->radius = MAX(group->radius,ap_len);
    if (!group->radius && group->count)
    {
        PERFINFO_AUTO_STOP();
        return 1;
    }
    PERFINFO_AUTO_STOP();
    return group->bounds_valid;
}

static void calcBoundsRecurse(GroupDef *def)
{
    int        i;

    if (!def || def->bounds_valid) {
        return;
    }
    if (def->pre_alloc) // Must have data loaded here!
        groupFileLoadFromName(def->name);
    for(i=0;i<def->count;i++)
    {
        #if CLIENT
        def->entries[i].flags_cache = 0;
        #endif
        calcBoundsRecurse(def->entries[i].def);
    }
    groupSetBounds(def);
    groupSetVisBounds(def);
    def->bounds_valid = 1;
    assert(!def->count || def->recursive_count);
}

void groupRecalcAllBounds()
{
    int        i,k;

    for(k=0;k<group_ptr->file_count;k++)
    {
        GroupFile    *file = group_ptr->files[k];

        for(i=0;i<file->count;i++)
        {
            if (file->defs[i])
                file->defs[i]->bounds_valid = 0;
        }
    }
    for(k=0;k<group_ptr->file_count;k++)
    {
        GroupFile    *file = group_ptr->files[k];

        for(i=0;i<file->count;i++)
            calcBoundsRecurse(file->defs[i]);
    }
}


void groupFileRecalcBounds(GroupFile *file)
{
    int        i,k;

    for(k=0;k<group_ptr->file_count;k++)
    {
        GroupFile    *file = group_ptr->files[k];

        for(i=0;i<file->count;i++)
            file->defs[i]->bounds_valid = 0;
    }
    for(i=0;i<file->count;i++)
        calcBoundsRecurse(file->defs[i]);
}


void groupDefRecenter(GroupDef *def)
{
    int            i;
    GroupEnt    *ent;

    for(i=0;i<def->count;i++)
    {
        ent = &def->entries[i];
        subVec3(ent->mat[3],def->mid,ent->mat[3]);
    }
    subVec3(def->min,def->mid,def->min);
    subVec3(def->max,def->mid,def->max);
    zeroVec3(def->mid);
}



DefTracker *groupRefNew()
{
    int        i;

    for(;;)
    {
        for(i=0;i<group_ptr->ref_max;i++)
        {
            if (!group_ptr->refs[i] || !group_ptr->refs[i]->def)
            {
                if (group_ptr->refs[i])
                    free(group_ptr->refs[i]);
                group_ptr->refs[i] = calloc(sizeof(DefTracker),1);
                group_ptr->refs[i]->id = i;
                groupRefModify(group_ptr->refs[i]);
                if (i >= group_ptr->ref_count)
                    group_ptr->ref_count = i+1;
                return group_ptr->refs[i];
            }
        }
        dynArrayAddStruct(&group_ptr->refs,&group_ptr->ref_count,&group_ptr->ref_max);
    }
}

DefTracker *groupRefPtr(int id)
{
    DefTracker    *ref,**ref_ptr;

    ref_ptr = dynArrayFitStructs(&group_ptr->refs,&group_ptr->ref_max,id);
    ref = *ref_ptr;
    if (!ref)
    {
        ref = group_ptr->refs[id] = calloc(sizeof(*ref),1);
    }
    if (id >= group_ptr->ref_count)
        group_ptr->ref_count = id+1;
    ref->id = id;
    return ref;
}

void groupRefDel(DefTracker *ref)
{
    if (!ref->def)
        return;
    trackerClose(ref);
    groupRefModify(ref);
    groupRefGridDel(ref);
    ref->def = 0;
}

void groupFileSetName(GroupFile *file,const char *fullname)
{
    char    basename[256];

    SAFE_FREE(file->fullname);
    SAFE_FREE(file->basename);
    file->fullname = forwardSlashes(strdup(fullname));
    groupBaseName(fullname,basename);
    file->basename = forwardSlashes(strdup(basename));
    
#if CLIENT
    {
        int        i;

        for(i=0;i<group_info.svr_file_count;i++)
        {
            if (stricmp(fullname,group_info.svr_file_names[i])==0)
                file->svr_idx = i;
        }
    }
#endif
}

GroupFile *groupFileNew()
{
    GroupFile    *file;

    file = calloc(sizeof(*file),1);
    file->idx = group_ptr->file_count;
    dynArrayAddp(&group_ptr->files,&group_ptr->file_count,&group_ptr->file_max,file);
    return file;
}

GroupFile *groupFileFind(const char *fname)
{
    int        i;
    const char    *cs;

    if (!fname)
    {
        if (group_info.file_count && !group_info.files[0]->fullname)
            return group_info.files[0];
        return 0;
    }
     cs = strstriConst(fname,"object_library");
    if (cs)
        fname = cs;
    for(i=0;i<group_info.file_count;i++)
    {
        if (group_info.files[i]->fullname==NULL)
            continue;
        if (stricmp(group_info.files[i]->fullname,fname)==0)
            return group_info.files[i];
    }
    return 0;
}

GroupDef *groupDefNew(GroupFile *file)
{
    GroupDef    *def;

    if (!file)
    {
        if (!group_ptr->files || !group_ptr->files[0])
            groupFileNew();
        file = group_ptr->files[0];
    }
    if (!group_ptr->def_mempool)
    {
        group_ptr->def_mempool = createMemoryPool();
        initMemoryPool(group_ptr->def_mempool,sizeof(GroupDef),1024);
    }
    def = mpAlloc(group_ptr->def_mempool);
    def->id = file->count;
    def->file = file;
    if (!file->pre_alloc)
    {
        file->defs = realloc(file->defs,++file->count * sizeof(file->defs[0]));
        file->defs[def->id] = def;
        if (group_ptr == &group_info)
            groupDefModify(def);
    }
    def->in_use = 1;    
    def->pre_alloc = file->pre_alloc;
    def->try_to_weld = 1;

    return def;
}

GroupDef *groupDefPtrFromFuture(GroupFile *file,int id,GroupDef *future_def)
{
    GroupDef    *def;

    if (id >= file->count)
    {
        file->defs = realloc(file->defs,sizeof(file->defs[0]) * (id+1));
        ZeroStructs(&file->defs[file->count],1+id-file->count);
        file->count = id+1;
    }
    if (!file->defs[id])
    {
        if (future_def)
            file->defs[id] = future_def;
        else
            file->defs[id] = mpAlloc(group_ptr->def_mempool);
    }
    def = file->defs[id];
    def->id = id;
    def->file = file;
    return def;
}

GroupDef *groupDefPtr(GroupFile *file,int id)
{
    return groupDefPtrFromFuture(file,id,0);
}

//gets 
char *groupDefGetFullName(GroupDef *def,char *buf,size_t bufsize)
{
    char    *dir;

    if (!def->in_use)
        return 0;
    dir = def->file->basename;
    if (dir && dir[0]) {
        STR_COMBINE_BEGIN_S(buf,bufsize);
        STR_COMBINE_CAT(dir);
        STR_COMBINE_CAT("/");
        STR_COMBINE_CAT(def->name);
        STR_COMBINE_END();
        //sprintf(buf,"%s/%s",def->dir,def->name);
    } else if (def->name)
        strcpy(buf,def->name);
    else
        buf[0] = 0;
    return buf;
}

void groupAssignToFile(GroupDef *def,GroupFile *file)
{
    PERFINFO_AUTO_START("groupAssignToFile",1);
        def->id = file->count;
        def->file = file;
        file->defs = realloc(file->defs,++file->count * sizeof(file->defs[0]));
        file->defs[def->id] = def;
        groupDefModify(def);
    PERFINFO_AUTO_STOP_CHECKED("groupAssignToFile");
}

void groupSwapDefs(GroupDef *a,GroupDef *b)
{
    GroupFile    *a_file;
    int            a_id;

    a->file->defs[a->id] = b;
    b->file->defs[b->id] = a;

    a_id = a->id;
    a->id = b->id;
    b->id = a_id;

    a_file = a->file;
    a->file = b->file;
    b->file = a_file;
}

GroupFile *groupFileFromName(const char *fname)
{
    GroupFile    *file;

    file = groupFileFind(fname);
    if (!file)
        file = groupLoad(fname);
    if (!file)
    {
        file = groupFileNew();
        groupFileSetName(file,fname);
    }
    return file;
}

void groupDefName(GroupDef *def,char *pathname,const char *objname)
{
    StashElement    result;

    PERFINFO_AUTO_START("groupDefName",1);

    if (!pathname)
        pathname = group_ptr->files[0]->fullname;
    assert(!strchr(objname,'/') && !strchr(objname,'\\'));
        
    //Add this def to the possible names
    if (!stashAddPointerAndGetElement(group_ptr->groupDefNameHashTable, objname, def, false, &result))
    {
        stashFindElement(group_ptr->groupDefNameHashTable,objname, &result);
        stashElementSetPointer(result,def);
    }
    if (result)
    {
        GroupFile    *newfile;

        def->name = stashElementGetStringKey(result);
        newfile = groupFileFromName(pathname);
        if (def->file && def->file != newfile)
        {
            if (def->pre_alloc)
                groupAssignToFile(def,newfile);
            else
            {
                GroupDef    *newdef;

                newdef = groupDefNew(newfile);
                newdef->in_use = 0;
                groupSwapDefs(newdef,def);
            }
        }
        objectLibraryAddName(def); // keep editor up to date on latest lib additions
    }

    PERFINFO_AUTO_STOP();
}

void groupDefFreeNovodex(GroupDef* def)
{
#if NOVODEX

    if ( !def->nxShape )
        return;

    // Free the NovodeX stuff.
    if ( def->nxActorRefCount > 0 )
    {
//        destroyActorSporeByGroupDef(def);
    }
    assert(!def->nxActorRefCount);
    nwFreeMeshShape(def->nxShape);
    def->nxShape = NULL;

#endif
}

void deleteDefTexSwap(DefTexSwap * dts);
static void deleteDefTexSwapCallback(void* arg0)
{
    deleteDefTexSwap((DefTexSwap *)arg0);
}

void groupDefFree(GroupDef *def,int check_tree)
{
    int            i;
    const char    *name;
    char        defname[256];
    GroupFile    *file;

    PERFINFO_AUTO_START("groupDefFree",1);
    if (check_tree)
    {
        groupDefGetFullName(def,SAFESTR(defname));
        groupDefModify(def);
    }
    if (def->entries )
    {
        int j;
        for( j = 0 ; j < def->count ; j++ )
        {
            if (def->entries[j].mat) { // This only goes bad with memory corruption?
                mpFree(group_info.mat_mempool,def->entries[j].mat);
                def->entries[j].mat = NULL;
            }
        }
    }
    if (def->name && def->name[0] && groupDefRefCount(def)==0)
        if (group_ptr->groupDefNameHashTable)
            stashRemovePointer(group_ptr->groupDefNameHashTable,def->name, NULL);
    SAFE_FREE(def->entries);
    def->count = 0;
    def->in_use = 0;
    def->saved = 0;
    name = def->name;
    file = def->file;

    if (check_tree)
    {
        int            j,k;
        GroupFile    *file;
        GroupDef    *curr;

        for(k=0;k<group_ptr->file_count;k++)
        {
            file = group_ptr->files[k];
            for(i=0;i<file->count;i++)
            {
                curr = file->defs[i];
                if (!curr->in_use)
                    continue;
                for(j=0;j<curr->count;j++)
                {
                    if (curr->entries[j].def == def)
                    {
                        log_printf("group.log","%s:\nDeleted reference %s\n",curr->name,defname);
                        Errorf("%s:\nDeleted reference %s\n",curr->name,defname);
                        curr->count--;
                        //MW GROUPENT
                        mpFree(group_info.mat_mempool,curr->entries[j].mat);
                        curr->entries[j].mat = NULL;
                        CopyStructs(curr->entries + j, curr->entries+j+1, curr->count-j);
                        j--;
                    }
                }
            }
        }
    }
#if CLIENT
    // Currently, only the base texture can be created as a "dynamic" texture
    if(def->tex_binds.base)
        texRemoveRef(def->tex_binds.base);
#endif
    
    if(def->properties)
        groupDefDeleteProperties(def);

    eaDestroyEx(&def->def_tex_swaps, deleteDefTexSwapCallback);

#if CLIENT
    if (def->auto_lod_models)
        eaDestroy(&def->auto_lod_models);

    if (def->welded_models)
        eaDestroy(&def->welded_models);

    if (def->unwelded_children)
        eaiDestroy(&def->unwelded_children);
#endif

    if ( def->nxShape )
        groupDefFreeNovodex(def);

    i = def->id;
    ZeroStruct(def);
    def->name = name;
    def->file = file;
    def->id = i;

    PERFINFO_AUTO_STOP_CHECKED("groupDefFree");
}

DefTexSwap* createDefTexSwap(const char *tex_name, const char *replace_name, int composite)
{
    DefTexSwap *dts;
    if(!tex_name || !replace_name)
        return NULL;
    dts = malloc(sizeof(DefTexSwap));
    dts->tex_name = strdup(tex_name);
    dts->replace_name = strdup(replace_name);
    dts->composite = composite;
#if CLIENT
    if (dts->composite)
    {
        dts->comp_tex_bind = texFindComposite(dts->tex_name);
        dts->comp_replace_bind = texFindComposite(dts->replace_name);

        if (dts->comp_tex_bind && dts->comp_replace_bind && dts->comp_tex_bind->texopt && dts->comp_replace_bind->texopt)
        {
            // Make sure we are not replacing a cubemap texture with a non-cubemap texture or vice-versa
            if ( (dts->comp_tex_bind->texopt->flags & TEXOPT_CUBEMAP) && !(dts->comp_replace_bind->texopt->flags & TEXOPT_CUBEMAP) )
            {
                Errorf("TexSwap Error: Cannot swap a cubemap texture '%s' with a non-cubemap texture '%s'\n", tex_name, replace_name);
                deleteDefTexSwap(dts);
                dts = NULL;
            }
            else if (!(dts->comp_tex_bind->texopt->flags & TEXOPT_CUBEMAP) && (dts->comp_replace_bind->texopt->flags & TEXOPT_CUBEMAP) )
            {
                Errorf("TexSwap Error: Cannot swap a non-cubemap texture '%s' with a cubemap texture '%s'\n", tex_name, replace_name);
                deleteDefTexSwap(dts);
                dts = NULL;
            }
        }
    }
    else
    {
        dts->tex_bind = texFind(dts->tex_name);
        dts->replace_bind = texFind(dts->replace_name);

        if (dts->tex_bind && dts->replace_bind)
        {
            // Make sure we are not replacing a cubemap texture with a non-cubemap texture or vice-versa
            if ( (dts->tex_bind->texopt_flags & TEXOPT_CUBEMAP) && !(dts->replace_bind->texopt_flags & TEXOPT_CUBEMAP) )
            {
                Errorf("TexSwap Error: Cannot swap a cubemap texture '%s' with a non-cubemap texture '%s'\n", tex_name, replace_name);
                deleteDefTexSwap(dts);
                dts = NULL;
            }
            else if (!(dts->tex_bind->texopt_flags & TEXOPT_CUBEMAP) && (dts->replace_bind->texopt_flags & TEXOPT_CUBEMAP) )
            {
                Errorf("TexSwap Error: Cannot swap a non-cubemap texture '%s' with a cubemap texture '%s'\n", tex_name, replace_name);
                deleteDefTexSwap(dts);
                dts = NULL;
            }
            else if (dts->tex_bind->target != dts->replace_bind->target)
            {
                Errorf("TexSwap Error: OpenGL bind target for target texture '%s' (0x%04X) does not match swap texture '%s' (0x%04X)\n", tex_name, dts->tex_bind->target, replace_name, dts->replace_bind->target);
                deleteDefTexSwap(dts);
                dts = NULL;
            }
        }
    }
#endif
    return dts;
}

void deleteDefTexSwap(DefTexSwap * dts) {
    assert(dts);
    free(dts->tex_name);
    free(dts->replace_name);
    free(dts);
}

//copies a DefTexSwap, allocating new strings and whatnot
DefTexSwap * copyDefTexSwap(DefTexSwap * dts) {
    DefTexSwap * ret;
    if (dts==NULL)
        return NULL;
    ret=(DefTexSwap *)malloc(sizeof(DefTexSwap));
    ret->tex_name=strdup(dts->tex_name);
    ret->replace_name=strdup(dts->replace_name);
    ret->composite=dts->composite;
#if CLIENT
    if (dts->composite) {
        ret->comp_tex_bind=dts->comp_tex_bind;
        ret->comp_replace_bind=dts->comp_replace_bind;
    } else {
        ret->tex_bind=dts->tex_bind;
        ret->replace_bind=dts->replace_bind;
    }
#endif
    return ret;
}

//this function copies the data from one def to another, and also makes unique copies of any data
//hanging off src that would get deleted when groupDefFree is called
//does not updated any of the heigherarchy at all
void copyGroupDef(GroupDef * def,GroupDef * src) {
    *def=*src;
    def->nxShape = NULL;
    def->nxActorRefCount = 0;
    if (def->has_properties)
        def->properties=copyPropertyStashTable(src->properties, 0);
    {
        int i;
        def->def_tex_swaps = NULL; // shallow copied above, deep copy instead
        for(i = 0; i < eaSize(&src->def_tex_swaps); i++)
            eaPush(&def->def_tex_swaps, copyDefTexSwap(src->def_tex_swaps[i]));
    }
    if (def->count)
    {
        int j;
        def->entries = malloc(def->count * sizeof(GroupEnt));
        memcpy(def->entries,src->entries,def->count * sizeof(GroupEnt));
        //MW GROUPENT
        for( j = 0 ; j < def->count ; j++ )
        {
            def->entries[j].mat = mpAlloc(group_info.mat_mempool);
            copyMat4( src->entries[j].mat, def->entries[j].mat );
        }
    }
    else
        def->entries = 0;
}
// this is used by the undo journal to free groupDefs copied with copyGroupDef
// it should not touch the world at all
void uncopyGroupDef(GroupDef *def)
{
    int i;
    if(def->has_properties)
        stashTableDestroy(def->properties);
    eaDestroyEx(&def->def_tex_swaps, deleteDefTexSwapCallback);
    for(i = 0; i < def->count; i++)
        mpFree(group_info.mat_mempool, def->entries[i].mat);
    SAFE_FREE(def->entries);
}

static char *getTexName(const char *fileName)
{
    static char *ext = ".texture";
    static int ext_len = 8; // strlen(ext);
    static char texName[MAX_PATH];
    int len = (int)strlen(fileName);
    int i;

    if (len<ext_len || strnicmp(fileName + len - ext_len, ext, ext_len)!=0) // not a .texture file
        return NULL;

    strcpy_s(texName, sizeof(texName), fileName);
    len -= ext_len;
    texName[len] = '\0';

    for (i = 0; i < len; ++i)
        texName[i] = tolower(texName[i]);

    return texName;
}

static void addTexName(const char *fileName) 
{
    char *texName = getTexName(fileName);

    if (texName)
        stashAddInt(s_texNames, texName, 1, false);
}

static void removeTexName(const char *fileName)
{
    char *texName = getTexName(fileName);

    if (texName)
        stashRemoveInt(s_texNames, texName, NULL);
}

static FileScanAction texNamesProcessor(char *dir, struct _finddata32_t *data)
{
    if (data->name[0]=='_') 
        return FSA_NO_EXPLORE_DIRECTORY;

    if (!(data->attrib & _A_SUBDIR))
        addTexName(data->name);
    
    return FSA_EXPLORE_DIRECTORY;
}

static void texNamesUpdate(const char *relpath, int when)
{
    const char *fileName;

    if (strstr(relpath, "/_"))
        return;
    
    if (dirExists(relpath))
        return; // This should only happen if there's a folder with named something.texture!

    fileName = getFileNameConst(relpath);

    if (fileExists(relpath))
        addTexName(fileName);
    else
        removeTexName(fileName);
}

static void initializeTexNames()
{
    s_texNames = stashTableCreateWithStringKeys(1000, StashDeepCopyKeys);
    fileScanAllDataDirs("texture_library", texNamesProcessor);
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE_AND_DELETE, "texture_library/*.texture", texNamesUpdate);
}

// Minimaps can be implicitly attached to defs by name. If the group changes its name the attachment
//  must be made explicit. Though this should only be called on the mapserver, this function
//  requires access to texture data. Since this should only happen on a development system with
//  both a local client and mapserver, this requirement shouldn't be a problem.
static void addMinimapPropertyIfNecessary(GroupDef *def, const GroupDef *orig)
{
    const char *origName;
    char texName[MAX_PATH];
    char *texNamePtr = texName;

    assert(def);
    assert(orig);

    if (!orig->is_tray || !orig->name ||
        stashFindPointer(def->properties, "MinimapTextureName", NULL))
    {
        return;
    }

    origName = orig->name;

    if (origName[0] == '_')
        ++origName;

    if (origName[0] == '\0')
        return;

    while (*origName != '\0')
        *texNamePtr++ = tolower(*origName++);

    *texNamePtr = '\0';

    if (!s_texNames)
        initializeTexNames();

    if (stashFindElement(s_texNames, texName, NULL))
    {
        PropertyEnt* prop = createPropertyEnt();
        strcpy(prop->name_str, "MinimapTextureName");
        estrPrintCharString(&prop->value_str, texName);
        prop->type = String;
    
        if(!def->properties)
        {
            def->properties = stashTableCreateWithStringKeys(16,StashDeepCopyKeys);
            def->has_properties = 1;
        }

        stashAddPointer(def->properties, prop->name_str, prop, false);
    }
}


GroupDef *groupDefDup(GroupDef *orig,GroupFile *file)
{
    GroupDef    *def;
    char        defname[256];
    int            id, i;
    char *        namePrefix;

    def = groupDefNew(file);
    id = def->id;
    file = def->file;
    *def = *orig;
    def->id = id;
    def->file = file;    
    def->nxShape = NULL;
    def->nxActorRefCount = 0;
    if (def->count)
        def->entries = malloc(def->count * sizeof(GroupEnt));
    else
        def->entries = 0;

    memcpy(def->entries,orig->entries,def->count * sizeof(GroupEnt));
    //MW GROUPENT
    for( i = 0 ; i < def->count ; i++ )
    {
        def->entries[i].mat = mpAlloc(group_info.mat_mempool);
        copyMat4( orig->entries[i].mat, def->entries[i].mat );
    }
    def->obj_lib = 0;

    if (def->has_sound)
    {
        namePrefix = "grpsound"; 
        if (orig->sound_name[0])
        {
            def->sound_name=NULL;
            groupAllocString(&def->sound_name);
            strcpy(def->sound_name,orig->sound_name);
        }
        if (orig->sound_script_filename && orig->sound_script_filename[0])
        {
            def->sound_script_filename=NULL;
            groupAllocString(&def->sound_script_filename);
            strcpy(def->sound_script_filename,orig->sound_script_filename);
        }
    } else if (def->has_light)
        namePrefix = "grplite";
    else if (def->has_cubemap)
        namePrefix = "grpcubemap";
    else if (def->has_fog)
        namePrefix = "grpfog";
    else if (def->has_volume)
        namePrefix = "grpvolume";
    else
        namePrefix = "grp";
    groupMakeName(defname,namePrefix,orig->name);
    groupDefName(def,def->file->fullname,defname);
    groupDefModify(def);

    if (def->has_properties)
        def->properties = copyPropertyStashTable(orig->properties, 0);

    def->def_tex_swaps = NULL; // shallow copied above, deep copy instead
    for(i = 0; i < eaSize(&orig->def_tex_swaps); i++)
        eaPush(&def->def_tex_swaps, copyDefTexSwap(orig->def_tex_swaps[i]));

    addMinimapPropertyIfNecessary(def, orig);
    return def;
}



DefTracker *groupRefFind(int group_id)
{
    return INRANGE0(group_id, group_ptr->ref_count) ? group_ptr->refs[group_id] : NULL;
}

int groupRefFindID(DefTracker *ref)
{
    return trackerRoot(ref)->id;
}

void groupRefMid(int group_id,Vec3 mid)
{
    addVec3(group_ptr->refs[group_id]->mat[3],group_ptr->refs[group_id]->def->mid,mid);
}

void groupBounds(int group_id,Vec3 min,Vec3 max)
{
    copyVec3(group_ptr->refs[group_id]->def->min,min);
    copyVec3(group_ptr->refs[group_id]->def->max,max);
}

char * firstCharAfterPrefix( char * name, char * prefix )
{
    char pathedPrefix[128];
    char * s;

    if( !prefix || !name )
        return 0;

    //If the string starts with the prefix, find the end
    if( 0 == stricmp( name, prefix ) )
        return name + strlen(prefix);

    //If the string is a path, look for '/prefix'. (Other parts of the group code do it this way)
    sprintf( pathedPrefix, "/%s", prefix ); 
    s = strstri( name, pathedPrefix );
    if( s )
        return s + strlen(pathedPrefix);

    //if not a path, just compare to the front
    if( !s && 0 == strnicmp( name, prefix, strlen( prefix ) ) )
        return name + strlen(prefix);

    return 0;
}

void extractGivenName( char * givenName, const char * oldName )
{
    const char * s = 0;

    if( !oldName || !oldName[0] )
    {
        givenName[0] = '\0';
        return;
    }

    if(!s) s = firstCharAfterPrefix( (char*)oldName, "grpsound"  );
    if(!s) s = firstCharAfterPrefix( (char*)oldName, "grplite"   );
    if(!s) s = firstCharAfterPrefix( (char*)oldName, "grpcubemap");
    if(!s) s = firstCharAfterPrefix( (char*)oldName, "grpvolume" );
    if(!s) s = firstCharAfterPrefix( (char*)oldName, "grpfog"    );
    if(!s) s = firstCharAfterPrefix( (char*)oldName, "grpa"      );
    if(!s) s = firstCharAfterPrefix( (char*)oldName, "grp"       );

    if( s && s[0] == '_' )
        strcpy( givenName, s+1 );
    else
        givenName[0] = '\0';

    //TO DO Maybe remove suffix numbers it might have from collisions?
    //TO DO make sure nobody names the group grp...
}



void groupMakeName(char *name, const char * grpxxx, const char * oldName )
{
    int idx = 0;
    char    namebuf[128];
    char    givenName[128];

    extractGivenName( givenName, oldName );

    //If someone gave a name to this group, use it instead of a number
    if( givenName[0] ) 
    {
        sprintf( namebuf, "%s_%s", grpxxx, givenName );
        while( groupDefFind(namebuf) )  //Maybe print a warning that this name exists already
            sprintf( namebuf,"%s_%s%d",grpxxx,givenName,++idx );
    }
    else //Otherwise just give it a random name.
    {
        for(;;)
        {
            sprintf(namebuf,"%s%d",grpxxx,++group_ptr->grp_idcnt);
            if (!groupDefFind(namebuf))
                break;
        }
    }
    strcpy(name,namebuf);
}

int groupRefAtomic(GroupDef *def)
{
    return !def->count;
}

int groupPolyCount(GroupDef *def)
{
    GroupEnt    *ent;
    int            i,count=0;

    if (def->model)
    {
#if CLIENT
        Model *model = def->model;

        if (!(model->trick && model->trick->flags1 & TRICK_EDITORVISIBLE))
            count += model->tri_count;
#endif
    }
    for(i=0;i<def->count;i++)
    {
        ent = &def->entries[i];
        count += groupPolyCount(ent->def);
    }
    return count;
}

int groupObjectCount(GroupDef *def)
{
    GroupEnt    *ent;
    int            i,count=0;

    if (def->model)
    {
#if CLIENT
        Model *model = def->model;

        if (!(model->trick && model->trick->flags1 & TRICK_EDITORVISIBLE))
            count += model->tex_count;
#endif
    }
    for(i=0;i<def->count;i++)
    {
        ent = &def->entries[i];
        count += groupObjectCount(ent->def);
    }
    return count;
}

void groupInfoInit(GroupInfo *gi)
{
    U32        last_mod_time = gi->ref_mod_time;

    destroyMemoryPool(gi->mat_mempool);
    destroyMemoryPool(gi->def_mempool);
    SAFE_FREE(gi->pre_alloc);
    SAFE_FREE(gi->refs);
    
    if ( gi->groupDefNameHashTable ) stashTableDestroy(gi->groupDefNameHashTable);
    gi->groupDefNameHashTable = NULL;

#if CLIENT
    SAFE_FREE(gi->svr_file_names);
#endif

    ZeroStruct(gi);
    gi->ref_mod_time = last_mod_time;
#if SERVER
    gi->load_full = 1;
#endif
    gi->groupDefNameHashTable = stashTableCreateWithStringKeys(2,StashDeepCopyKeys);
    gi->mat_mempool = createMemoryPool();
    initMemoryPool(gi->mat_mempool,sizeof(Mat4),1024);
    gi->def_mempool = createMemoryPool();
    initMemoryPool(gi->def_mempool,sizeof(GroupDef),1024);
}

void groupInfoDefault(GroupInfo *gi)
{
    group_ptr = gi;
}

void groupFileFree(GroupFile *file)
{
    int        i;

    for(i=0;i<file->count;i++)
    {
        GroupDef    *def;

        def = file->defs[i];
        if (def)
        {
            if (def->in_use)
            {
                groupDefFree(def, 0);
            }
            else
            {
                assert(!def->nxShape);
            }
            mpFree(group_ptr->def_mempool,def);
        }
    }
    SAFE_FREE(file->basename);
    SAFE_FREE(file->fullname);
    SAFE_FREE(file->defs);
    ZeroStruct(file);
}

void groupFreeAll(GroupInfo *gi)
{
    int        i;

    groupInfoDefault(gi);
    for(i=0;i<gi->file_count;i++)
    {
        groupFileFree(gi->files[i]);
        SAFE_FREE(gi->files[i]);
    }
    groupInfoDefault(&group_info);
    for(i=0;i<gi->ref_count;i++)
    {
        SAFE_FREE(gi->refs[i]);
    }
    SAFE_FREE(gi->refs);
    SAFE_FREE(gi->files);

    if (gi->groupDefNameHashTable) stashTableDestroy(gi->groupDefNameHashTable);
    destroyMemoryPool(gi->string_mempool);

    ZeroStruct(gi);
}

void groupInit()
{
    groupInfoInit(&group_info);
    groupInfoDefault(&group_info);
}

#if SERVER
#include "cmdparse/cmdserver.h"
#endif

void groupLoadLibs()
{
    objectLibraryLoadNames(0);
}

//Is this the name of a library piece??
//It is if it's an extended path and not the "maps" directory
//or if it's a simple name and doesn't have "grp"
int groupInLibSub(const char *name)
{
    if (!strrchr(name,'/'))
    {
        if (strnicmp(name,"grp",3)==0)
            return 0;
    }
    else
    {
        if (strnicmp(name,"maps",4)==0)
            return 0;
    }
    return 1;
}

int groupInLib(GroupDef *def)
{
    if (!def || !def->in_use || !def->name)
        return 0;
    if (strnicmp(def->name,"grp",3)==0)
        return 0;
    if (def->file->fullname && strnicmp(def->file->fullname,"object_library",strlen("object_library"))==0)
        return 1;
    return 0;
}

int groupIsPrivate(GroupDef *def)
{
    if (groupInLib(def) && def->name[strlen(def->name)-1] == '&')
        return 1;
    return 0;
}

int groupIsPublic(GroupDef *def)
{
    if (groupInLib(def) && !groupIsPrivate(def))
        return 1;
    return 0;
}


void groupRefModify(DefTracker *ref)
{
    if (!ref)
        return;
#if SERVER
//    journalRef(ref);
#endif
    ref->mod_time = ++group_ptr->ref_mod_time;
}

void groupDefModify(GroupDef *def)
{
#if SERVER
    if (!def->file || !def->file->base)
    {
//journalDef(def);
        checkoutDefFile(def);
    }
#endif
    def->mod_time = ++group_ptr->ref_mod_time;
    def->file->mod_time = def->mod_time;
    if (!group_ptr->loading)
        def->file->unsaved = 1;
    def->saved = 0;
}

char *groupAllocString(char **memp)
{
    if (*memp)
        return *memp;
    if (!group_ptr->string_mempool)
    {
        group_ptr->string_mempool = createMemoryPool();
        initMemoryPool( group_ptr->string_mempool, GROUPDEF_STRING_LENGTH, 32) ;
    }
    *memp = mpAlloc(group_ptr->string_mempool);
    return *memp;
}

void groupFreeString(char **memp)
{
    if (*memp)
        mpFree(group_ptr->string_mempool,*memp);
    *memp = 0;
}

//This function should be called whenever the tracker pointers might have changed.
//If you have anything that carries tracker pointers, here is were you'll need to reset them
void groupTrackersHaveBeenReset()
{
    static nextGroupDefVersion = 0;
    //Global that says old pointers to defs are now bad
#ifdef CLIENT
    game_state.groupDefVersion = ++nextGroupDefVersion; //old pointers to defs are now bad.
    //edit_state.last_selected_tracker = NULL; should do this here?
    mouseover_tracker = NULL;
    cam_info.last_tray = NULL;
    edit_state.textureSwapTracker = NULL;
    sunUnsetSkyFadeOverride();
    vistrayResetUIDs();
    groupClearAuxTrackerDefs();
#else
    server_state.groupDefVersion = ++nextGroupDefVersion;
#endif
    entMotionClearAllWorldGroupPtr();
     groupDynBuildBreakableList();
}

void groupReset()
{
    extern Grid obj_grid;
    int        i;

#if CLIENT
    vistrayReset();
#endif

#if NOVODEX
    nx_state.gameInPhysicsPossibleState = 0;
    if ( nwEnabled() )
    {
        writeConsole(OUTPUT_DEBUG, "Deinitializing PhysX");
        if ( nwDeinitializeNovodex() )
        {
            writeConsole(OUTPUT_INFO, "Deinitialized PhysX");
        }
        else
        {
            writeConsole(OUTPUT_ERROR, "Failed to deinitialize PhysX");
        }
    }
#endif

#if SERVER
    journalReset();
#endif

    for(i=0;i<group_info.ref_count;i++)
    {
        groupRefDel(group_info.refs[i]);
        SAFE_FREE(group_ptr->refs[i]);
    }

    
    entMotionFreeCollAll();
    gridFreeAll(&obj_grid);
    objectLibraryClearTags();
    for(i=0;i<group_info.file_count;i++)
    {
        groupFileFree(group_info.files[i]);
        SAFE_FREE(group_ptr->files[i]);
    }

    groupTrackersHaveBeenReset();
    
    SAFE_FREE(group_info.refs);
    SAFE_FREE(group_info.files);

    if (group_info.groupDefNameHashTable)
        stashTableDestroy(group_info.groupDefNameHashTable);
    group_info.groupDefNameHashTable = NULL;
    
    if (group_info.string_mempool)
        destroyMemoryPool(group_info.string_mempool);
    group_info.string_mempool = NULL;

    groupInit();
    group_info.file_count = 0;
    group_info.ref_count = 0;
    
    ZeroArray(group_info.scenefile);
    ZeroArray(group_info.loadscreen);


    groupCleanUp();
    group_info.lastSavedTime = ++group_info.ref_mod_time;
    //TO DO reset ref_mod_time and def_mod_time??
#if CLIENT
    clearSetParentAndActiveLayer();
#endif
}

// Cleans up anything in shared memory that we don't want hanging around when we exit
// or reload a map
void groupCleanUp( )
{
    modelFreeAllCache(GEO_USED_BY_WORLD);

    // TODO: Is there a "correct" spot to put this?
    g_base.def = NULL;
    g_base.ref = NULL;
    g_base.plot_def = NULL;
    g_base.sound_def = NULL;

#if CLIENT
    baseedit_InvalidateGroupDefs();
#endif
}

#if CLIENT
int groupHidden(GroupDef *def)
{
    assert(def);

    if ((def->volume_trigger || def->has_volume) && edit_state.hideCollisionVolumes)
        return 1;

    if (def->hideForGameplay && !editMode())
        return 1;

    return 0;
}

int groupTrackerHidden(DefTracker *tracker)
{
    return (tracker->tricks && (tracker->tricks->flags1 & TRICK_HIDE));
}

#endif

 
#ifdef SERVER
extern int        world_modified;
#endif


static void expand(DefTracker *tracker, DefTracker * expandedRefs[], int * exRefCnt ) 
{
    int        i;

    trackerOpen(tracker);
    for(i=0;i<tracker->count;i++)
    {
        if (tracker->entries[i].def && tracker->entries[i].def->is_autogroup)
        {
            expand(&tracker->entries[i], expandedRefs, exRefCnt );
        }
        else
        {
            DefTracker    *ref;

            ref = groupRefNew();
            copyMat4(tracker->entries[i].mat,ref->mat);
            ref->def = tracker->entries[i].def;

            if( expandedRefs )
            {
                assert( *exRefCnt < 100000 ); //clumsy, but hey
                expandedRefs[(*exRefCnt)++] = ref;
            }
        }
    }
}

void ungroupAll()
{
    int            i;

    for(i=0;i<group_info.ref_count;i++)
    {
        if (group_info.refs[i]->def && group_info.refs[i]->def->is_autogroup)
        {
            expand(group_info.refs[i], 0, 0);
            groupRefDel(group_info.refs[i]);
        }
    }
#ifdef SERVER
    world_modified = 1;
#endif
}

static DefTracker *makeGroup(DefTracker **trackers,int count, char * prefix)
{
    DefTracker    *ref;
    GroupDef    *def;
    char        defname[1000];
    int            i;
    Vec3        ref_pos;

    def = groupDefNew(0);
    def->count    = count;

    //TEMP HACK for layerconvert
    if( strstri( prefix, "grp_Geometry") )
        groupMakeName(defname,"grp","grp_Geometry");
    else
        groupMakeName(defname,prefix,0);

    groupDefName(def,0,defname);
    if (def->count)
        def->entries = calloc(def->count,sizeof(GroupEnt));
    else
        def->entries = 0;

    if (stricmp(prefix, "grpa")==0)
        def->is_autogroup = 1;

    copyVec3(trackers[0]->mat[3],ref_pos);
    for(i=0;i<count;i++)
    {
        def->entries[i].def = trackers[i]->def;
        //MW GROUPENT
        def->entries[i].mat = mpAlloc(group_info.mat_mempool);
        copyMat4(trackers[i]->mat,def->entries[i].mat);
        groupRefDel(trackers[i]);
    }
    groupSetBounds(def);
    groupDefRecenter(def);
    groupSetVisBounds(def);

    ref = groupRefNew();
    ref->def = def;
    copyMat3(unitmat,ref->mat);
    subVec3(ref_pos,def->entries[0].mat[3],ref->mat[3]);
    return ref;
}

static DefTracker *mergeGroups(Vec3 center, Vec3 dv_in, F32 maxdv, DefTracker **trackers_in, int count_in)
{
    int            i,j,idx,counts[8],count_out=0;
    DefTracker    **trackers[8],**trackers_out,*ref;
    F32            t;
    Vec3        pos, tracker_dv, dv;

    copyVec3(dv_in, dv);

    if (count_in < 5 || (dv[0] <= 8 && dv[1] <= 8 && dv[2] <= 8) || (count_in < 20 && (dv[0] <= 64 && dv[1] <= 64 && dv[2] <= 64)))
    {
        if (count_in == 1)
            return trackers_in[0];
        else
            return makeGroup(trackers_in,count_in, "grpa");
    }
    ZeroArray(counts);
    for(i=0;i<8;i++)
        trackers[i] = calloc(sizeof(DefTracker *),count_in);
    trackers_out = calloc(sizeof(DefTracker *),count_in);
    for(j=0;j<count_in;j++)
    {
        DefTracker * tracker;
        tracker = trackers_in[j];

        if (!tracker->def)
            continue;

        subVec3(tracker->def->max, tracker->def->min, tracker_dv);
        scaleVec3(tracker_dv, 0.5f, tracker_dv);

        if (tracker_dv[0] < maxdv && tracker_dv[1] < maxdv && tracker_dv[2] < maxdv)
        {
            Vec3 point;

            mulVecMat4( tracker->def->mid, tracker->mat, point );

            idx = 0;
            for(i=0;i<3;i++)
            {
                t = point[i] - center[i];

                if (t > 0) // pos side
                    idx |= 1 << i;
            }

            assert(idx < 8);
            trackers[idx][counts[idx]++] = tracker;
        }
        else
            trackers_out[count_out++] = tracker;

    }
    scaleVec3(dv, 0.5f, dv);
    maxdv *= 0.5f;
    for(i=0;i<8;i++)
    {
        copyVec3(center,pos);
        if (i & 1)
            pos[0] += dv[0];
        else
            pos[0] -= dv[0];

        if (i & 2)
            pos[1] += dv[1];
        else
            pos[1] -= dv[1];

        if (i & 4)
            pos[2] += dv[2];
        else
            pos[2] -= dv[2];

        if (counts[i])
            trackers_out[count_out++] = mergeGroups(pos, dv, maxdv, trackers[i], counts[i]);
        free(trackers[i]);
    }
    if (count_out == 1)
        ref =  trackers_out[0];
    else
        ref = makeGroup(trackers_out,count_out,"grpa");
    free(trackers_out);
#ifdef CLIENT
    loadUpdate("AutoGrouping", 50);
#endif
    return ref;
}

void groupAll()
{
#ifdef SERVER
    extern int        world_modified;
#endif
    int        i,j;
    F32        *pos,maxdv;
    Vec3    min,max,dv,center;
    DefTracker *ref;
#define MAXVAL 8e16

    loadstart_printf("\nGroup All Begin....");

    loadstart_printf("\n  First Step....");
    setVec3(min,MAXVAL,MAXVAL,MAXVAL);
    setVec3(max,-MAXVAL,-MAXVAL,-MAXVAL);
    for(i=0;i<group_info.ref_count;i++)
    {
        ref = group_info.refs[i];
        if (!ref->def)
            continue;
        pos = ref->mat[3];
        for(j=0;j<3;j++)
        {
            min[j] = MIN(min[j],ref->def->min[j]);
            max[j] = MAX(max[j],ref->def->max[j]);
        }
    }
    loadend_printf("done");
    loadstart_printf("\n  mergeGroups Step....");
    subVec3(max,min,dv);
    scaleVec3(dv,0.5,dv);
    addVec3(dv,min,center);
    maxdv = MAX(dv[0], dv[1]);
    maxdv = MAX(maxdv, dv[2]);
    mergeGroups(center,dv,maxdv,group_info.refs,group_info.ref_count);
#ifdef SERVER
    world_modified = 1;
#endif
    loadend_printf("done");
    loadend_printf("done");
}

//Doesn't really work right....
void unAutoGroupSelection( DefTracker * tracker )
{
//    DefTracker * ref;
//    DefTracker ** expandedRefs;
//    int exRefCnt = 0;
//
//    expandedRefs = calloc( sizeof(void*), 100000 );
//    if (tracker->def && isAutoGroup(tracker->def))
//    {
//        expand(tracker, expandedRefs, &exRefCnt);
//        groupRefDel(tracker);
//    }
//
//    ref = makeGroup(expandedRefs,exRefCnt);
//    //reNAME THE gROUP    
//    //groupSet( expandedRefs, exRefCnt );
//
//    free( expandedRefs );
}


static int canUngroupSafely(GroupDef *def)
{
    int i;

    if (def->model)
        return 0;

    if (def->has_properties || def->has_tint_color || def->has_tex || eaSize(&def->def_tex_swaps) || def->no_fog_clip || def->no_coll)
        return 0;

    // group bounds
    if (def->breakable || def->is_tray || def->has_ambient || def->has_light || def->has_cubemap || def->has_fog || def->has_beacon)
        return 0;

    if (def->vis_window || def->volume_trigger || def->water_volume || def->lava_volume || def->sewerwater_volume || def->redwater_volume || def->door_volume || def->materialVolume)
        return 0;
    
    if (def->has_sound || def->vis_blocker || def->vis_angleblocker || def->vis_doorframe || def->outside || def->has_alpha || def->has_fx || def->lod_fadenode == 2 || def->lod_scale != 1.f)
        return 0;

    if (def->lod_fadenode == 1)
    {
        for (i = 0; i < def->count; i++)
        {
            if (def->entries[i].def->parent_fade)
                return 0;
        }
    }

    // reject if library piece
    if (strnicmp(def->name, "grp", 3)!=0)
        return 0;

    return 1;
}

static int ungroupSelection(GroupDef *parent, GroupDef *def, int idx)
{
    int i, count;
    GroupEnt *oldEntries;
    Mat4 defmat;

    if (!def->count)
        return 0;

    if (!canUngroupSafely(def))
        return 0;

    copyMat4(parent->entries[idx].mat, defmat);

    // recurse
    count = def->count;
    for (i = 0; i < count; )
    {
        if (ungroupSelection(def, def->entries[i].def, i))
            count--;
        else
            i++;
    }

    // add children to parent
    oldEntries = parent->entries;
    count = parent->count + def->count - 1;
    parent->entries = calloc(count, sizeof(parent->entries[0]));
    CopyStructs(parent->entries, oldEntries, idx);
    CopyStructs(parent->entries + idx, oldEntries + idx + 1, parent->count - idx - 1);
    mpFree(group_info.mat_mempool, oldEntries[idx].mat);
    oldEntries[idx].mat = NULL;
    free(oldEntries);

    for (i = 0; i < def->count; i++)
    {
        GroupEnt *dst = &parent->entries[parent->count - 1 + i];
        GroupEnt *src = &def->entries[i];

        dst->def = src->def;
        dst->mat = mpAlloc(group_info.mat_mempool);
        mulMat4(defmat, src->mat, dst->mat);
    }

    parent->count = count;

#ifdef CLIENT
    loadUpdate("AutoGrouping", 10);
#endif

    return 1;
}

static void groupLoadIfPreloadRecurse(GroupDef *def)
{
    int i;
    groupLoadIfPreload(def);
#ifdef CLIENT
    loadUpdate("AutoGrouping", 10);
#endif
    for (i = 0; i < def->count; i++)
        groupLoadIfPreloadRecurse(def->entries[i].def);
}

static void ungroupAllPossible(DefTracker * ref)
{
    int i, count;
    GroupDef *def = ref->def;

    if (!def)
        return;

    trackerClose(ref);
    groupRefModify(ref);
    groupRefGridDel(ref);
    groupLoadIfPreloadRecurse(ref->def);
    groupRecalcAllBounds();
#ifdef CLIENT
    loadUpdate("AutoGrouping", 100);
#endif
    count = def->count;
    for (i = 0; i < count; )
    {
        if (ungroupSelection(def, def->entries[i].def, i))
            count--;
        else
            i++;
    }

    groupRefActivate(ref);
    trackerOpen(ref);
}

//MW just hacked up groupall
void autoGroupSelection( DefTracker * ref)
{
#ifdef SERVER
    extern int    world_modified;
#endif
    int        i,j;
    F32        *pos,maxdv;
    Vec3    dv,center;
    DefTracker **  refList;
    DefTracker * returnedRef;
    U32 timeBeforeAutoGrouping;

    #define MAXVAL 8e16
    loadstart_printf("AutoGroup.. ");

    if (!ref->def)
        return;

    timeBeforeAutoGrouping = group_info.ref_mod_time;

    ungroupAllPossible(ref);

    loadupdate_printf("ungrouped to %d groups.. ", ref->count);

    if (1)
    {
        pos = ref->mat[3];

        subVec3(ref->def->max, ref->def->min, dv);
        scaleVec3(dv,0.5f,dv);
        addVec3(dv, ref->def->min, center);

        refList = calloc( sizeof( void* ), ref->count );
        for( i = 0 ; i < ref->count ; i++ )
        {
            refList[i] = &ref->entries[i];
        }

        maxdv = MAX(dv[0], dv[1]);
        maxdv = MAX(maxdv, dv[2]);
        returnedRef = mergeGroups(center,dv,maxdv,refList,ref->count);
        free(refList); //fix leak

        ///  Possible Hackery  ////////////////////////////////////

        //Place the newly formed autogrouped def under the geometry layer
        trackerClose(ref);
        groupRefModify(ref);
        groupRefGridDel(ref);
        copyMat4( returnedRef->mat, ref->mat );
        {
            GroupDef * def;
            def = ref->def; //This is the giant ungrouped list, free it
            for( j = 0 ; j < def->count ; j++ ) {
                mpFree( group_info.mat_mempool, def->entries[j].mat );
                def->entries[j].mat = NULL;
            }
            if(def->count)
                free(def->entries);

            //Then make it have one entry, the newly made autogroup def
            def->count = 1;
            def->entries = calloc(1, sizeof(GroupEnt));
            def->entries[0].def = returnedRef->def;
            def->entries[0].mat = mpAlloc(group_info.mat_mempool);
            copyMat4( unitmat, def->entries[0].mat );
            groupSetBounds(def);
        }
        groupRefActivate(ref);
        groupRefDel(returnedRef); //Then get rid of the temporary group you created
    }

    //Prevent the server from trying to send all these newly modified autogrouped defs to the client
    {
        GroupFile    *file = group_info.files[0];
        GroupDef    *def;

        for(i=0;i<file->count;i++)
        {
            def = file->defs[i];
            if( timeBeforeAutoGrouping < def->mod_time )
            {
                def->saved = 1;
                def->file_idx = -1; //TO DO is this crazy?
            }
        }
    }
    ////End possible hackery/////////////////////////////////

    loadend_printf("done");

#ifdef SERVER
    world_modified = 1;
#endif
}

U32 groupInfoCrcDefs()
{
    int i,k;
    cryptAdler32Init();
    for(k=0;k<group_info.file_count;k++)
    {
        GroupFile    *file = group_info.files[k];

        for (i=0; i<file->count; i++)
        {
            GroupDef    *def = file->defs[i];

            if (def && def->in_use && def->name)
                cryptAdler32Update(def->name, (int)strlen(def->name));
            else if (def)
                cryptAdler32Update("NAMELESS", 8);
        }
    }
    return cryptAdler32Final();
}

static void cryptAdler32UpdateString(const char *str)
{
    if (str)
        cryptAdler32Update(str, (int)strlen(str));
}

U32 groupInfoCrcRefs()
{
    int i;
    cryptAdler32Init();
    for (i=0; i<group_info.ref_count; i++)
    {
        GroupDef *def = group_info.refs[i]->def;
        if (def)
        {
            if (def->in_use && def->name)
                cryptAdler32UpdateString(def->name);
            else
                cryptAdler32UpdateString("NAMELESS");

            if (def->count && def->entries[0].def)
                cryptAdler32UpdateString(def->entries[0].def->name);
        }
    }
    return cryptAdler32Final();
}

U32 groupInfoRefCount()
{
    int i, count = 0;
    for (i=0; i<group_info.ref_count; i++)
    {
        if (group_info.refs[i]->def)
            count++;
    }

    return count;
}

void groupTreeGetBounds(Vec3 min, Vec3 max)
{
    int i;
    setVec3(min, 8e8, 8e8, 8e8);
    setVec3(max, -8e8, -8e8, -8e8);

    for (i = 0; i < group_info.ref_count; i++)
    {
        DefTracker    *ref;
        ref = group_info.refs[i];
        if (ref->def)
        {
            Vec3 defmin, defmax;
            mulBoundsAA(ref->def->min, ref->def->max, ref->mat, defmin, defmax);
            MIN1(min[0], defmin[0]);
            MIN1(min[1], defmin[1]);
            MIN1(min[2], defmin[2]);
            MAX1(max[0], defmax[0]);
            MAX1(max[1], defmax[1]);
            MAX1(max[2], defmax[2]);
        }
    }
}

void trackerHandleFill(DefTracker *tracker, TrackerHandle *handle)
{
    ZeroStruct(handle);
    handle->ref_id = trackerRootID(tracker);
    handle->depth = trackerPack(tracker, handle->idxs, handle->names, 1);
}

void trackerHandleFillNoAllocNoCheck(DefTracker *tracker, TrackerHandle *handle)
{
    ZeroStruct(handle);
    handle->ref_id = trackerRootID(tracker);
    handle->depth = trackerPack(tracker, handle->idxs, NULL, 0);
}

TrackerHandle * trackerHandleCreate(DefTracker * tracker)
{
    TrackerHandle *handle;
    
    if (tracker==NULL)
        return NULL;

    handle=calloc(1,sizeof(TrackerHandle));
    trackerHandleFill(tracker, handle);
    return handle;
}

void trackerHandleClear(TrackerHandle *handle)
{
    int i;
    handle->ref_id = -1;
    handle->depth = -1;
    for(i = 0; i < handle->depth; i++)
        SAFE_FREE(handle->names[i]);
}

void trackerHandleDestroy(TrackerHandle * handle)
{
    if(handle)
    {
        trackerHandleClear(handle);
        free(handle);
    }
}

DefTracker* trackerFromHandle(TrackerHandle *handle)
{
    return handle ? trackerUnpack(groupRefFind(handle->ref_id), handle->depth, handle->idxs, handle->names, 0) : NULL;
}

TrackerHandle * trackerHandleCopy(TrackerHandle * handle) {
    int i;
    TrackerHandle * ret=malloc(sizeof(TrackerHandle));
    memcpy(ret,handle,sizeof(TrackerHandle));
    for(i = 0; i < handle->depth; i++)
        ret->names[i] = strdup(handle->names[i]);
    return ret;
}

// you could sort with this, but the real point is that if they're equal it
// returns zero
int trackerHandleComp(const TrackerHandle * a,const TrackerHandle * b) {
    int i;
    if (a->ref_id!=b->ref_id)
        return a->ref_id-b->ref_id;
    for (i=0;i<a->depth && i<b->depth;i++)
        if (a->idxs[i]!=b->idxs[i])
            return a->idxs[i]-b->idxs[i];
    return a->depth-b->depth;
}
