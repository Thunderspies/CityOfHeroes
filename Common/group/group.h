#ifndef _GROUP_H
#define _GROUP_H

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/earray.h>
#include "render/texEnums.h"

typedef struct GroupDef GroupDef;
typedef struct GroupFileEntry GroupFileEntry;
typedef struct TrickNode TrickNode;
typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct MemoryPoolImp *MemoryPool;
typedef struct GridIdxList GridIdxList;
typedef struct Model Model;
typedef struct Grid Grid;
typedef struct BasicTexture BasicTexture;
typedef struct GroupFile GroupFile;
typedef struct Base Base;
typedef struct SoundScript SoundScript;

//Each GroupDef gets one of these 
typedef struct GroupEnt
{
    Mat4Ptr        mat;
    GroupDef    *def;
#if CLIENT
    U32            flags_cache : 8;
    U32            has_dynamic_parent_cache : 1;
    F32            vis_dist_scaled_cache;    // view_scale * def->vis_dist * def->lod_scale
    F32            vis_def_radius_cache;    // def->radius
    int            recursive_count_cache;
    F32            radius;                // def->radius + def->shadow_dist
    Vec3        mid_cache;            //cached def->mid * mat
#endif
} GroupEnt;

#if 0
typedef struct
{
    GroupDef    *expanded;
    int            count;
    GroupEnt    *entries;
    F32            radius;
    U32            someflags : 1;
    
} MinDef;
#endif

typedef struct DefTexSwap {
    char * tex_name;                //name of the original texture
    char * replace_name;            //name of the texture that replaces it
    int composite;                    //1 iff this is a composite texture
#if CLIENT
    union {
        struct { //following two are used for basic textures
            BasicTexture *tex_bind;        //ID of the original texture
            BasicTexture *replace_bind;    //ID of the texture that replaces it
        };
        struct { //following two are used for composite textures
            TexBind *comp_tex_bind;                //ID of the original texture
            TexBind *comp_replace_bind;            //ID of the texture that replaces it
        };
    };
#endif
} DefTexSwap;

DefTexSwap* createDefTexSwap(const char *tex_name, const char *replace_name, int composite);
void deleteDefTexSwap(DefTexSwap * dts);
DefTexSwap * copyDefTexSwap(DefTexSwap * dts);

typedef struct DefTexSwapNode DefTexSwapNode;
typedef struct DefTexSwapNode {
    int index;
    DefTexSwap ** swaps;
} DefTexSwapNode;

#define GroupBoundsMembers \
    Vec3 min;\
    Vec3 max;\
    Vec3 mid;\
    F32 vis_dist;\
    F32 radius;\
    F32 lod_scale;\
    F32 shadow_dist;\
    int        recursive_count; /* total number of groups under this one (children + children's children etc.)*/ \
    U32        breakable : 1;\
    U32        child_breakable : 1;\
    U32        is_tray : 1;    /* "VisTray" model trick (or "VisOutside" model trick for mysterious reasons)*/\
    U32        child_tray : 1;\
    U32        has_ambient : 1;\
    U32        child_ambient: 1;\
    U32        has_light : 1;\
    U32        child_light : 1;\
    U32        has_cubemap : 1;\
    U32        child_cubemap : 1;\
    U32        has_fog : 1;\
    U32        child_has_fog : 1;\
    U32        has_volume : 1;\
    U32        child_volume : 1;\
    U32        has_beacon : 1;\
    U32        child_beacon : 1;\
    U32        open_tracker : 1;\
    U32        has_properties : 1;\
    U32        child_properties : 1;\
    U32        no_fog_clip : 1;\
    U32        child_no_fog_clip : 1;\
    U32        is_autogroup : 1;\
    U32        child_volume_trigger : 1;\
    U32        no_coll : 1; \
    U32        do_welding : 1; \
    U32        editor_visible_only : 1; /* set to 1 if the group AND all its children are editor visible only */
    

typedef struct GroupBounds {
    GroupBoundsMembers
} GroupBounds;


typedef struct GroupDef
{
    GroupBoundsMembers

    int            count;
    GroupEnt    *entries;
    const char    *name;
    char        *type_name;
    U32            mod_time;
    U32        pre_alloc : 1;
    U32        vis_window : 1;
    U32        saved : 1;
    U32        save_temp : 1;
    U32        open : 1;
    U32        volume_trigger : 1;            //"VolumeTrigger" modle trick, must be true for any other volume flags to matter
    U32        water_volume : 1;            //"WaterVolume" model trick
    U32        lava_volume : 1;            //"LavaVolume" model trick    
    U32        sewerwater_volume : 1;        //"SewerWaterVolume" model trick    
    U32        redwater_volume : 1;        //"RedWaterVolume" model trick    
    U32        door_volume : 1;            //"DoorVolume" model trick
    U32        materialVolume : 1;            //"MaterialVolume" model trick : anthing that's a material for powers, but not a liquid
    U32        obj_lib : 1;
    U32        lod_autogen : 1;
    U32        in_use : 2;
    U32        init : 1;
    U32        has_sound : 1;

    U32        sound_exclude : 1;
    U32        lod_fromtrick : 1;
    U32        lod_fromfile : 1;
    U32        vis_blocker : 1; 
    U32        vis_angleblocker : 1;
    U32        vis_doorframe : 1;
    U32        outside : 1;
    U32        outside_only : 1;
    U32        sound_occluder : 1;
    U32        has_tint_color : 1;
    U32        has_tex : 2;
    U32        has_alpha : 1;
    U32        has_fx : 1;
    U32        lib_ungroupable : 1;
    U32        parent_fade : 1;                    // I'm looking for a parent that is a fadenode
    U32        child_parent_fade : 1;                // I have a child with the parent_fade flag
    U32        lod_fadenode : 2;                    // I am a fadenode
    U32        key_light : 1;
    U32        no_beacon_ground_connections : 1;    // Flag for beacon processing.
    U32        child_valid : 1;                    // all children of this node are loaded and drawable
    U32        bounds_valid : 1;                    // used so recursive bounds calculator doesn't have to redo work
    U32        stackable_floor : 1;                // stuff can be stacked on this group in the base editor
    U32        stackable_wall : 1;                    // stuff can be stacked on this group in the base editor
    U32        stackable_ceiling : 1;                // stuff can be stacked on this group in the base editor
    U32        try_to_weld : 1;
    U32        deleteMe : 1;
    U32        deleteMyChild : 1;
    U32        hideForHeadshot : 1;                            //Will not draw this node if drawing a headshot.
    U32        hideForGameplay : 1;                // Can be toggled by the mapserver for gameplay purposes.
                                                // Drawn by default in the editor.
    U32        disable_planar_reflections : 1;        // Ignore reflection_quads
                                                // These two flags are not combined into a single "I'm a zowie" flag since they are used by completely separate
                                                // systems.  Keping them separate provides painlessly fgor the option of turning them on and off individually
    U32        pulse_glow : 1;                        // Flag that I should have a pulsating glow
    U32        play_objective_sound : 1;            // Flag that I should play the objective loop "glowie hum"

    U32        no_occlusion : 1;                    // Do not add this def's model to the occlusion buffer

    int        id;                                    // index into defs[] array

    struct {
        F32            lod_near,lod_far;
        F32            lod_nearfade,lod_farfade;
    };
    struct {
        F32            lod_far;
        F32            lod_farfade;
        F32            lod_scale;
    } from_file;

    Model     *    model;

    Model     ** auto_lod_models;
    Model     **    welded_models;
    int         *  welded_models_duids; // delta UIDs for finding MiniTrackers
    int         *    unwelded_children;

    int            child_high_lod_idx;

    U16            file_idx;            // only valid if def was read from file

    F32            light_radius;

    U8            sound_vol;
    F32            sound_radius;
    F32            sound_ramp;
    char        *sound_name;

    U8            color[2][4];
    F32            fog_near,fog_far;
    F32            fog_radius;
    F32            fog_speed;
    F32            groupdef_alpha;

    char        *beacon_name;
    F32            beacon_radius;
    
    char        *tex_names[2];
    TextureOverride tex_binds;

    DefTexSwap    ** def_tex_swaps;

    StashTable    properties;
    U32            access_time;
    GroupFileEntry *gf;        // used in groupFileLoad.c to cache the results of groupGetFileEntryPtr(def->name);
    GroupFile    *file;
    
    void*        nxShape;
    int            nxActorRefCount;

    char * sound_script_filename;
    SoundScript ** sound_scripts;
    Vec3        box_scale;    // for variable sized collision volumes

    int            cubemapGenSize;
    int            cubemapCaptureSize;
    F32            cubemapBlurAngle;
    F32            cubemapCaptureTime;
} GroupDef;

typedef struct DefTracker
{
    Mat4        mat;                // absolute location
    Vec3        mid;                // physical midpoint

    GroupDef    *def;
    struct
    {
        U32            open            : 1;
        U32            inherit            : 1;// all my children should inherit my tricks - (ie; force wireframe on all children)
        U32            edit            : 1;// used by game editor to control if this tracker's children are selectable
        U32            frozen            : 1;// used by editor, the tracker cannot be selected if this flag is set
        U32            tagged            : 1;// used in grouptrack.c to tag trackers that will need to use the editcache
        U32            invisible        : 1;
        U32            no_collide        : 1;// not selectable, even in editor
        U32            selected        : 1;
        U32            no_coll            : 1;// groupdef or parent groupdef has no_coll set, make it editor selectable only
        U32            dirty            : 1;// has undergone an edit operation
        U32            deleteMe        : 1;// flagged for deletion due to an edit operation
        U32            deleteMyChild    : 1;// some descendant has deleteMe set
        U32            justPasted        : 1;// just pasted, so move to empty spot in parent's entries if possible
    };

    struct DefTracker    *parent;
    struct DefTracker    *entries;    // list of children
    int            count;                // number of children

    U32            def_mod_time;        // synchronizes last time this tracker was updated to its groupdef
    F32            radius;                // physical radius

    U32            mod_time;            // used for root trackers on server for journaling
#ifdef DEBUG_UIDS_GOING_BAD
    U32            cached_uid;
    U32            cached_uid_version;
#endif

    GridIdxList    *grids_used;        // list of open collision table
    int            coll_time;
    int            id;                    // index in refs[] array
    int            dyn_group;            // index+1 into dynGroups array
    int            ent_id;                // Entity this corresponds to (used to be stored on groupdef, which makes no sense and is wrong)

#if CLIENT
    struct VisTray        *dyn_parent_tray;    // if dynamic linked, this is the tray I'm visually linked to

    TrickNode            *tricks;
    U8                    **src_rgbs;        // cached rgbs for individually lit geometry
    Grid                *light_grid;
    F32                    light_luminance; // Average luminance of all lights in this tray (for tone mapping)
    int                    fx_id;
    U32                    burning_buildings_flags;
    U32                    burning_buildings_fade_start_abs_time;
    int                    lod_override;    // lod+1 to display.  set to zero to disable override.

    GridIdxList            *traygrid_idxs;    // tray grid indexes used
#endif

} DefTracker;

typedef struct TrackerHandle
{
    int ref_id;        // which root level object this is under
    int depth;        // number of valid entries in idxs
    int idxs[100];    // list of child indices from the root level you'd follow to get to this tracker
    char *names[100]; // def names for each level. this can be removed (if necessary) once the editor is more stable.
} TrackerHandle;

typedef struct GroupFile
{
    U32                mod_time;
    GroupDef        **defs;
    GroupFileEntry    *lib;
    char            *basename;
    char            *fullname;
    int                count;
    int                idx;
    Base            *base;        // this GroupFile is auto generated from a base
    U32                cache : 1;
    U32                unsaved : 1;
    U32                obj_lib : 1;
    U32                pre_alloc : 1;
    U32                modified_on_disk : 1;
    U32                force_reload : 1;
    U32                dirty : 1;    // an edit operation has been performed on a def from this file
    U32                bounds_checksum;
#if CLIENT
    int                svr_idx;
#endif
} GroupFile;

typedef struct GroupInfo
{
    GroupFile    **files;
    GroupFile    *pre_alloc;
    int            file_count,file_max;

    DefTracker    **refs;
    int            ref_count,ref_max;

    int            ref_mod_time;
    int            def_access_time;
    U32            lastSavedTime;
    U32            completed_map_load_time;

    int            *ref_idxs,*def_idxs;    //only used for journaling
    char        scenefile[MAX_PATH];
    char        loadscreen[MAX_PATH];

    StashTable    groupDefNameHashTable;    //All GroupDef Names.  Each GroupDef name in a map is unique
    MemoryPool    string_mempool;            //GroupDefs use this for their strings (GROUPDEF_STRING_LENGTH)
    MemoryPool    def_mempool;            //GroupDefs alloced into this
    MemoryPool    mat_mempool;            //GroupEnts get their mats from this pool 

    U32            load_full : 1;        // SERVER sets this to 1, client defaults to partial loading - only load what's drawn / collided
    U32            tray_opened : 1;    // tray deftracker was opened since last groupNetRecv - may need to rescan objects not in trays and see if they are now
    U32            grp_idcnt;            // used by groupMakeName, reset when a new map is loaded
    int            loading;            // currently loading a group file from disk (disables some defMod stuff), might make more sense as an on/off bit, but uses a counter so...
#if CLIENT
    char        (*svr_file_names)[256];
    int            svr_file_count;
#endif
} GroupInfo;

#define GROUPDEF_STRING_LENGTH 128 //TO DO: check groupDef strings


extern GroupInfo    group_info;

void groupGetBoundsMinMax(GroupDef *def,const Mat4 mat,Vec3 min,Vec3 max);
void groupGetBoundsMinMaxNoVolumes(GroupDef *def,const Mat4 mat,Vec3 min,Vec3 max);
int groupRefShouldAutoGroup(DefTracker *ref);
void groupSwapDefs(GroupDef *a,GroupDef *b);
void groupAssignToFile(GroupDef *def,GroupFile *file);
void groupRecalcAllBounds();
void groupFileSetName(GroupFile *file,const char *fullname);
GroupFile *groupFileFind(const char *dir);
GroupFile *groupFileFromName(const char *dir);
char *lastslash(char *s);
void groupBaseName(const char *fname,char *basename);
Model * groupModelFind(const char *name);
int defContainCount(GroupDef *container,GroupDef *def);
int groupDefRefCount(GroupDef *def);
GroupDef *groupDefFind(const char *name);
GroupDef *groupDefFindWithLoad(const char *objname);
GroupDef *groupDefFindByID(int id);
int isFxTypeName(const char *name);
void groupSetVisBounds(GroupDef *group);
void groupGetBoundsTransformed(const Vec3 min, const Vec3 max, const Mat4 pmat, Vec3 minOut, Vec3 maxOut);
int groupSetBounds(GroupDef *group);
void groupDefRecenter(GroupDef *def);
DefTracker *groupRefNew();
DefTracker *groupRefPtr(int id);
void groupRefDel(DefTracker *ref);
GroupFile *groupFileNew();
GroupDef *groupDefNew(GroupFile *file);
GroupDef *groupDefPtr(GroupFile *file, int id);
char *groupDefGetFullName(GroupDef *def,char *buf,size_t bufsize);
void groupDefName(GroupDef *def,char *pathname,const char *objname);
void groupDefFreeNovodex(GroupDef* def);
void groupDefFree(GroupDef *def,int check_tree);
GroupDef *groupDefDup(GroupDef *orig,GroupFile *file);
void copyGroupDef(GroupDef *def,GroupDef *src);
void uncopyGroupDef(GroupDef *def);
DefTracker *groupRefFind(int group_id);
int groupRefFindID(DefTracker *ref);
void groupRefMid(int group_id,Vec3 mid);
void groupBounds(int group_id,Vec3 min,Vec3 max);
void groupMakeName(char *name,const char *base,const char * oldName);
int groupRefAtomic(GroupDef *def);
int groupPolyCount(GroupDef *def);
int groupObjectCount(GroupDef *def);
void groupInfoInit(GroupInfo *gi);
void groupInfoDefault(GroupInfo *gi);
void groupFreeAll(GroupInfo *gi);
void groupInit();
void groupTrackersHaveBeenReset();
void groupLoadLibs();
int groupInLibSub(const char *name);
int groupInLib(GroupDef *def);
void groupRefModify(DefTracker *ref);
void groupDefModify(GroupDef *def);
char *groupAllocString(char **memp);
void groupFreeString(char **memp);
void groupReset();
int groupIsPrivate(GroupDef *def);
int groupIsPublic(GroupDef *def);
void groupCleanUp(void);

void trackerHandleFill(DefTracker *tracker, TrackerHandle *handle);
void trackerHandleFillNoAllocNoCheck(DefTracker *tracker, TrackerHandle *handle); // does not need to be cleared, does no name checking
void trackerHandleClear(TrackerHandle *handle);
TrackerHandle * trackerHandleCreate(DefTracker * tracker);
void trackerHandleDestroy(TrackerHandle * handle);
DefTracker* trackerFromHandle(TrackerHandle *handle);
TrackerHandle * trackerHandleCopy(TrackerHandle * handle);
int trackerHandleComp(const TrackerHandle * a,const TrackerHandle *b);

#if CLIENT
int groupHidden(GroupDef *def);
int groupTrackerHidden(DefTracker *tracker);
#endif


int isAutoGroup(GroupDef *def);
void ungroupAll();
void groupAll();
void unAutoGroupSelection( DefTracker * tracker );
void autoGroupSelection( DefTracker * ref );
void fixUpConverted( void );//temp for me

void extractGivenName( char * givenName, const char * oldName );

U32 groupInfoCrcDefs();
U32 groupInfoCrcRefs();
U32 groupInfoRefCount();

void groupTreeGetBounds(Vec3 min, Vec3 max);

#endif
