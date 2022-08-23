#include "group/group.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/error.h>
#include "group/groupfileload.h"
#include "group/groupProperties.h"
#include <string.h>
#include <utilitieslib/utils/utils.h>
#include "group/groupfileloadutil.h"
#include <utilitieslib/utils/fileutil.h>
#include "gridcoll/gridcoll.h"
#include "group/groupfilelib.h"
#include <utilitieslib/components/earray.h>
#include "group/grouputil.h"
#include "group/grouptrack.h"
#include "seq/tricks.h"
#include <utilitieslib/utils/timing.h>
#include "gridcoll/gridcache.h"
#include "group/groupgrid.h"
#include "group/groupdyn.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include "seq/anim.h"
#include <float.h>
#include "NovodeX/NwWrapper.h"
#include <utilitieslib/utils/strings_opt.h>
#include "gridcoll/gridfind.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/StashTable.h>
#include "seq/AutoLOD.h"
#include <utilitieslib/utils/structInternals.h>



#ifdef CLIENT
#include "cmdparse/cmdgame.h"
#include "graphics/groupdraw.h"
#include "bases/basedraw.h"
#include "render/tex.h"
#include "graphics/vistray.h"
#include "group/groupnetrecv.h"
#include "graphics/imageCapture.h"
#define SERVER_OR_GAMESTATE game_state

#endif

#ifdef SERVER
#define SERVER_OR_GAMESTATE server_state
#include "cmdparse/cmdserver.h"  //Temp
#include "group/groupdb_util.h"
#include "group/groupfilesave.h"
#include "storyarc/missiongeoCommon.h"
#endif


// change this whenever map bins need to be reprocessed from scratch, parsetable changes are handled automatically
#define GROUP_DEPENDENCY_VERSION 1

// stash of def names that are or will be loaded at some point during this loading process
StashTable futureDefs;    


typedef enum
{
    BIN_LOAD_NORMALLY=0,
    BIN_REBUILD_MAP=1,
    BIN_REBUILD_OBJLIB=2,
} BinForceType;

static int g_force_load_full;

typedef struct
{
    int        tex_unit;
    char    *tex_name;
} ReplaceTexLoad;

typedef struct
{
    char    *tex_swap_name;
    char    *rep_swap_name;
    int        composite;
} TexSwapLoad;

typedef struct
{
    U8        rgbs[2][4];
} TintColorLoad;

typedef struct
{
    U8        rgb[4];
} AmbientLoad;

typedef struct
{
    U8        rgb[4];
    F32        radius;
    U32        negative;
} OmniLoad;

typedef struct 
{
    int genSize;
    int captureSize;
    F32 blur;
    F32 time;
} CubemapLoad;

typedef struct
{
    F32 test;
    Vec3    scale;
} VolumeLoad;

typedef struct
{
    F32        radius,neardist,fardist,speed;
    U8        rgbs[2][4];
} FogLoad;

typedef struct
{
    char    *name;
    F32        radius;
} BeaconLoad;

typedef struct
{
    char    *name;
    F32        volume;
    F32        radius;
    F32        ramp;
    U32        exclude;
} SoundLoad;

typedef struct
{
    char    *name;
    char    *value;
    int        type;
} PropertyLoad;

typedef struct
{
    char    *name;
    Vec3    pos;
    Vec3    pyr;
    U32        flags;
} GroupLoad;

typedef struct
{
    F32        lod_far,lod_farfade,lod_scale;
} LodLoad;

typedef struct
{
    char            *name;
    char            *obj_name;
    char            *type;
    char            *sound_script;
    GroupLoad        **groups;
    PropertyLoad    **properties;
    TintColorLoad    **tintcolor;
    SoundLoad        **sound;
    ReplaceTexLoad    **replace_tex;
    TexSwapLoad        **tex_swap;
    OmniLoad        **omni;
    CubemapLoad        **cubemap;
    VolumeLoad        **volume;
    BeaconLoad        **beacon;
    FogLoad            **fog;
    AmbientLoad        **ambient;
    LodLoad            **lod;
    U32                flags;
    F32                alpha;
} DefLoad;

typedef struct
{
    char    *name;
    Vec3    pos;
    Vec3    pyr;
} RefLoad;

typedef struct
{
    int        version;
    DefLoad    **defs;
    RefLoad    **refs;
    char    *scenefile;
    char    *loadscreen;
    char    ** importFiles;
} GroupFileLoad;

enum
{
    DEF_UNGROUPABLE = 1<<0,
    DEF_FADENODE = 1<<1,
    DEF_FADENODE2 = 1<<2,
    DEF_NOFOGCLIP = 1<<3,
    DEF_NOCOLL = 1<<4,
    DEF_NOOCCLUSION = 1<<5,
};

enum
{
    GROUP_DISABLEPLANARREFLECTIONS    = 1<<0,
};


static StaticDefineInt sound_flags[] = {
    DEFINE_INT
    { "Exclude",            1 },
    DEFINE_END
};

static StaticDefineInt omnilight_flags[] = {
    DEFINE_INT
    { "Negative",            1 },
    DEFINE_END
};

static StaticDefineInt def_flags[] = {
    DEFINE_INT
    { "Ungroupable",        DEF_UNGROUPABLE },
    { "FadeNode",            DEF_FADENODE },
    { "FadeNode2",            DEF_FADENODE2 },
    { "NoFogClip",            DEF_NOFOGCLIP },
    { "NoColl",                DEF_NOCOLL },
    { "NoOcclusion",        DEF_NOOCCLUSION },
    DEFINE_END
};

static StaticDefineInt group_flags[] = {
    DEFINE_INT
    { "DisablePlanarReflections",        GROUP_DISABLEPLANARREFLECTIONS },
    DEFINE_END
};

static TokenizerParseInfo parse_sound[] = {
    { "",                TOK_STRUCTPARAM | TOK_STRING(SoundLoad,name, 0)},
    { "",                TOK_STRUCTPARAM | TOK_F32(SoundLoad,volume, 0)},
    { "",                TOK_STRUCTPARAM | TOK_F32(SoundLoad,radius, 0)},
    { "",                TOK_STRUCTPARAM | TOK_F32(SoundLoad,ramp, 0)},
    { "",                TOK_STRUCTPARAM | TOK_FLAGS(SoundLoad,exclude,0),sound_flags},
    { "\n",                TOK_END,            },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_replace_tex[] = {
    { "",                TOK_STRUCTPARAM | TOK_INT(ReplaceTexLoad,tex_unit, 0)},
    { "",                TOK_STRUCTPARAM | TOK_STRING(ReplaceTexLoad,tex_name, 0)},
    { "\n",                TOK_END,            },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_tex_swap[] = {
    { "",                TOK_STRUCTPARAM | TOK_STRING(TexSwapLoad,tex_swap_name, 0)},
    { "",                TOK_STRUCTPARAM | TOK_STRING(TexSwapLoad,rep_swap_name, 0)},
    { "",                TOK_STRUCTPARAM    | TOK_INT(TexSwapLoad,composite, 0)},
    { "\n",                TOK_END,            },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_beacon[] = {
    { "",                TOK_STRUCTPARAM | TOK_STRING(BeaconLoad,name, 0)},
    { "",                TOK_STRUCTPARAM | TOK_F32(BeaconLoad,radius, 0)},
    { "\n",                TOK_END,            },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_group[] = {
    { "",                TOK_STRUCTPARAM | TOK_STRING(GroupLoad,name, 0)},
    { "Pos",            TOK_VEC3(GroupLoad,pos)},
    { "PYR",            TOK_PYRDEGREES(GroupLoad,pyr)},
    { "Rot",            TOK_REDUNDANTNAME|TOK_VEC3(GroupLoad,pyr)},
    { "Flags",            TOK_FLAGS(GroupLoad,flags,0),group_flags },
    { "End",            TOK_END,            },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_tintcolor[] = {
    { "",        TOK_STRUCTPARAM | TOK_FIXED_ARRAY | TOK_U8_X, offsetof(TintColorLoad,rgbs[0]), 3},
    { "",        TOK_STRUCTPARAM | TOK_FIXED_ARRAY | TOK_U8_X, offsetof(TintColorLoad,rgbs[1]), 3},
    { "\n",                TOK_END,            },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_ambient[] = {
    { "",        TOK_STRUCTPARAM | TOK_FIXED_ARRAY | TOK_U8_X, offsetof(AmbientLoad,rgb),  3},
    { "\n",                TOK_END,            },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_fog[] = {
    { "",    TOK_STRUCTPARAM | TOK_F32(FogLoad,radius, 0)},
    { "",    TOK_STRUCTPARAM | TOK_F32(FogLoad,neardist, 0)},
    { "",    TOK_STRUCTPARAM | TOK_F32(FogLoad,fardist, 0)},
    { "",    TOK_STRUCTPARAM | TOK_FIXED_ARRAY | TOK_U8_X, offsetof(FogLoad,rgbs[0]), 3},
    { "",    TOK_STRUCTPARAM | TOK_FIXED_ARRAY | TOK_U8_X, offsetof(FogLoad,rgbs[1]), 3},
    { "",    TOK_STRUCTPARAM | TOK_F32(FogLoad,speed, 1.0)},
    { "\n",    TOK_END    },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_omni[] = {
    { "",        TOK_STRUCTPARAM | TOK_FIXED_ARRAY | TOK_U8_X, offsetof(OmniLoad,rgb),    3},
    { "",                TOK_STRUCTPARAM | TOK_F32(OmniLoad,radius, 0)},
    { "",                TOK_STRUCTPARAM | TOK_FLAGS(OmniLoad,negative,0),omnilight_flags},
    { "\n",                TOK_END,            },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_cubemap[] = {
    { "",                TOK_STRUCTPARAM | TOK_INT(CubemapLoad,genSize,256)},
    { "",                TOK_STRUCTPARAM | TOK_INT(CubemapLoad,captureSize,1024)},
    { "",                TOK_STRUCTPARAM | TOK_F32(CubemapLoad,blur,0)},
    { "",                TOK_STRUCTPARAM | TOK_F32(CubemapLoad,time,12)},
    { "\n",                TOK_END,            },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_volume[] = {
    { "",                TOK_STRUCTPARAM | TOK_F32(VolumeLoad,scale[0],0)},
    { "",                TOK_STRUCTPARAM | TOK_F32(VolumeLoad,scale[1],0)},
    { "",                TOK_STRUCTPARAM | TOK_F32(VolumeLoad,scale[2],0)},
    { "\n",                TOK_END,            },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_property[] = {
    { "",                TOK_STRUCTPARAM | TOK_STRING(PropertyLoad,name, 0)},
    { "",                TOK_STRUCTPARAM | TOK_STRING(PropertyLoad,value, 0)},
    { "",                TOK_STRUCTPARAM | TOK_INT(PropertyLoad,type, 0)},
    { "\n",                TOK_END },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_lod[] = {
    { "Far",        TOK_F32(LodLoad,lod_far, 0) },
    { "FarFade",    TOK_F32(LodLoad,lod_farfade, 0) },
    { "Scale",        TOK_F32(LodLoad,lod_scale, 0) },
    { "End",        TOK_END,    },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_def[] = {
    { "",                TOK_STRUCTPARAM | TOK_STRING(DefLoad,name, 0)},
    { "Group",            TOK_STRUCT(DefLoad,groups,        parse_group) },
    { "Property",        TOK_STRUCT(DefLoad,properties,    parse_property) },
    { "TintColor",        TOK_STRUCT(DefLoad,tintcolor,    parse_tintcolor) },
    { "Ambient",        TOK_STRUCT(DefLoad,ambient,        parse_ambient) },
    { "Omni",            TOK_STRUCT(DefLoad,omni,        parse_omni) },
    { "Cubemap",        TOK_STRUCT(DefLoad,cubemap,        parse_cubemap) },
    { "Volume",            TOK_STRUCT(DefLoad,volume,        parse_volume) },
    { "Sound",            TOK_STRUCT(DefLoad,sound,        parse_sound) },
    { "ReplaceTex",        TOK_STRUCT(DefLoad,replace_tex,    parse_replace_tex) },
    { "Beacon",            TOK_STRUCT(DefLoad,beacon,        parse_beacon) },
    { "Fog",            TOK_STRUCT(DefLoad,fog,            parse_fog) },
    { "Lod",            TOK_STRUCT(DefLoad,lod,            parse_lod) },
    { "Type",            TOK_STRING(DefLoad,type, 0) },
    { "Flags",            TOK_FLAGS(DefLoad,flags,0),def_flags },
    { "Alpha",            TOK_F32(DefLoad,alpha, 0) },
    { "Obj",            TOK_STRING(DefLoad,obj_name, 0) },
    { "TexSwap",        TOK_STRUCT(DefLoad,tex_swap,    parse_tex_swap) },
    { "SoundScript",    TOK_STRING(DefLoad,sound_script, 0) },
    { "End",            TOK_END,            0},
    { "DefEnd",            TOK_END,            0},
    { "", 0, 0 }
};

static TokenizerParseInfo parse_ref[] = {
    { "",                TOK_STRUCTPARAM | TOK_STRING(RefLoad,name, 0)},
    { "Pos",            TOK_VEC3(RefLoad,pos)},
    { "PYR",            TOK_PYRDEGREES(RefLoad,pyr)},
    { "Rot",            TOK_REDUNDANTNAME|TOK_VEC3(RefLoad,pyr)},
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

static TokenizerParseInfo parse_group_list[] = {
    { "Version",        TOK_INT(GroupFileLoad,version, 0) },
    { "Scenefile",        TOK_STRING(GroupFileLoad,scenefile, 0) },
    { "Loadscreen",        TOK_STRING(GroupFileLoad,loadscreen, 0) },
    { "Def",            TOK_STRUCT(GroupFileLoad,defs,parse_def) },
    { "RootMod",        TOK_REDUNDANTNAME | TOK_STRUCT(GroupFileLoad,defs,parse_def) },
    { "Ref",            TOK_STRUCT(GroupFileLoad,refs,parse_ref) },
    { "Import",            TOK_STRINGARRAY(GroupFileLoad,importFiles) },
    { "", 0, 0 }
};

typedef struct GroupFileDep
{
    char *fname;
    int timestamp;
} GroupFileDep;

typedef struct GroupFileDepInfo
{
    int version;
    U32 parse_crc;
    GroupFileDep **fileDeps;
} GroupFileDepInfo;

static GroupFileDepInfo *file_dep_info = 0;
static U32 parse_crc = 0;

static TokenizerParseInfo parse_file_dep[] = {
    { "{",                TOK_START,        0 },
    { "Filename",        TOK_STRING(GroupFileDep,fname, 0) },
    { "Timestamp",        TOK_INT(GroupFileDep,timestamp, 0) },
    { "}",                TOK_END,            0 },
    { "", 0, 0 }
};

static TokenizerParseInfo parse_file_deps[] = {
    { "Version",        TOK_INT(GroupFileDepInfo, version, 0) },
    { "ParseCRC",        TOK_INT(GroupFileDepInfo, parse_crc, 0) },
    { "Dependency",        TOK_STRUCT(GroupFileDepInfo, fileDeps, parse_file_dep) },
    { "", 0, 0 }
};

static void setParseCRC(void)
{
    if(!parse_crc)
        parse_crc = ParseTableCRC(parse_group_list, NULL);
}

static void startDependencies(void)
{
    assert(!file_dep_info);
    setParseCRC();

    file_dep_info = malloc(sizeof(GroupFileDepInfo));
    ZeroStruct(file_dep_info);
    file_dep_info->version = GROUP_DEPENDENCY_VERSION;
    file_dep_info->parse_crc = parse_crc;
    eaCreate(&file_dep_info->fileDeps);
}

static INLINEDBG void addDependency(const char *filename)
{
    GroupFileDep *dep;
    if (!file_dep_info)
        return;

    dep = ParserAllocStruct(sizeof(GroupFileDep));
    dep->fname = ParserAllocString(filename);
    dep->timestamp = fileLastChanged(filename);

    eaPush(&file_dep_info->fileDeps, dep);
}

static void addDependencyCallback(char* path, __time32_t date)
{
    addDependency(path);
}


static void writeDependencies(char *srcFilename)
{
    char buf[MAX_PATH];

    if (!file_dep_info)
        return;
    
    fileLocateWrite(srcFilename, buf);
    mkdirtree(buf);
    changeFileExt(srcFilename, ".dep", buf);
    ParserWriteTextFile(buf, parse_file_deps, file_dep_info, 0, 0);
    ParserDestroyStruct(parse_file_deps, file_dep_info);

    SAFE_FREE(file_dep_info);
}

static int dependenciesUpToDate(const char *srcFilename)
{
    char buf[MAX_PATH];
    int len, i;
    char *data;
    GroupFileDepInfo read_dep={0};

    changeFileExt(srcFilename, ".dep", buf);
    data = fileAlloc(buf, &len);
    if (!data || !len)
        return 0;
    if (!ParserReadText(data, len, parse_file_deps, &read_dep))
    {
        SAFE_FREE(data);
        return 0;
    }

    SAFE_FREE(data);

    setParseCRC();
    if(    read_dep.version != GROUP_DEPENDENCY_VERSION ||
        read_dep.parse_crc != parse_crc )
    {
        ParserDestroyStruct(parse_file_deps, &read_dep);
        return 0;
    }

    for (i = 0; i < eaSize(&read_dep.fileDeps); i++)
    {
        if (fileLastChanged(read_dep.fileDeps[i]->fname) != read_dep.fileDeps[i]->timestamp)
        {
            ParserDestroyStruct(parse_file_deps, &read_dep);
            return 0;
        }
    }

    ParserDestroyStruct(parse_file_deps, &read_dep);
    return 1;
}

static int bitSet(U32 flag,U32 bit)
{
    if (flag & bit)
        return 1;
    return 0;
}

void groupSetTrickFlags(GroupDef *group)
{
    TrickInfo    *trick;
    Model    *model;
    U32            flags;

    PERFINFO_AUTO_START("groupSetTrickFlags",1);

    group->do_welding        = !!groupDefFindProperty(group, "DoWelding");

    model = group->model;
    if (!model) {
        // Clear flags that might trickle up
        group->key_light = 0;
        PERFINFO_AUTO_STOP_CHECKED("groupSetTrickFlags");
        return;
    }

    trick = model->trick?model->trick->info:NULL;

    // LOD info can come from the model, trick, and elsewhere, only reset it if it changed on the trick

    group->lod_near        = trick?trick->lod_near:0;
    group->lod_nearfade    = trick?trick->lod_nearfade:0;
    group->lod_far        = trick?trick->lod_far:0;
    group->lod_farfade    = trick?trick->lod_farfade:0;
    group->lod_scale    = trick?trick->lod_scale:0;
    group->shadow_dist    = trick?trick->shadow_dist:0;

#if CLIENT
    eaDestroy(&group->auto_lod_models);
    if (model->lod_info)
    {
        ModelLODInfo *lod_info = model->lod_info;
        int i=0;
        int numLODs = eaSize(&lod_info->lods);

        group->lod_near        = lod_info->lods[0]->lod_near;
        group->lod_nearfade = lod_info->lods[0]->lod_nearfade;
        group->lod_far        = lod_info->lods[numLODs-1]->lod_far;
        group->lod_farfade    = lod_info->lods[numLODs-1]->lod_farfade;

        for (i = 0; i < numLODs; i++)
        {
            Model *lodmodel=0;
            if (lod_info->lods[i]->lod_modelname)
            {
                lodmodel = modelFind(lod_info->lods[i]->lod_modelname, lod_info->lods[i]->lod_filename, LOAD_NOW, GEO_INIT_FOR_DRAWING | GEO_USED_BY_WORLD);
            }
            else if (lod_info->lods[i]->max_error)
            {
                int need_shrink = lod_info->lods[i]->lod_nearfade > 0 || (i > 0 && lod_info->lods[i-1]->lod_farfade > 0);
                extern int gLODReloadCount, gLODReloadTriCount;
                assert(model->pack.reductions.unpacksize);
                gLODReloadCount++;
                gLODReloadTriCount += model->tri_count;
                lodmodel = modelFindLOD(model, lod_info->lods[i]->max_error, (lod_info->lods[i]->flags & LOD_ERROR_TRICOUNT)?TRICOUNT_RMETHOD:ERROR_RMETHOD, need_shrink?(i * 0.1f):0);
            }
            else
            {
                lodmodel = model;
            }

            if(lodmodel)//if(devassert(lodmodel))
            {
                eaPush(&group->auto_lod_models, lodmodel);
            }
            else
            {
                // TODO: Print an error on the console!
            }
        }

        modelDoneMakingLODs(model);
    }
#endif

    flags = trick?trick->group_flags:0;
    group->parent_fade        = bitSet(flags, GROUP_PARENT_FADE);
    group->volume_trigger    = bitSet(flags, GROUP_VOLUME_TRIGGER);
    group->water_volume        = bitSet(flags, GROUP_WATER_VOLUME);
    group->lava_volume        = bitSet(flags, GROUP_LAVA_VOLUME);
    group->sewerwater_volume = bitSet(flags, GROUP_SEWERWATER_VOLUME);
    group->redwater_volume    = bitSet(flags, GROUP_REDWATER_VOLUME);
    group->door_volume        = bitSet(flags, GROUP_DOOR_VOLUME);
    group->materialVolume    = bitSet(flags, GROUP_MATERIAL_VOLUME);
    group->outside            = bitSet(flags, GROUP_VIS_OUTSIDE) || groupDefFindProperty(group, "VisOutside");
    group->outside_only        = bitSet(flags, GROUP_VIS_OUTSIDE_ONLY) || groupDefFindProperty(group, "VisOutsideOnly");
    group->sound_occluder    = bitSet(flags, GROUP_SOUND_OCCLUDER) || groupDefFindProperty(group, "SoundOccluder");
    group->vis_blocker        = bitSet(flags, GROUP_VIS_BLOCKER);
    group->vis_angleblocker = bitSet(flags, GROUP_VIS_ANGLEBLOCKER);
    group->vis_window        = bitSet(flags, GROUP_VIS_WINDOW);
    group->vis_doorframe    = bitSet(flags, GROUP_VIS_DOORFRAME);
    group->key_light        = bitSet(flags, GROUP_KEY_LIGHT);
    group->is_tray            = group->vis_blocker || group->outside || bitSet(flags, GROUP_VIS_TRAY) || groupDefFindProperty(group, "VisTray");
    group->stackable_floor    = bitSet(flags, GROUP_STACKABLE_FLOOR);
    group->stackable_wall    = bitSet(flags, GROUP_STACKABLE_WALL);
    group->stackable_ceiling= bitSet(flags, GROUP_STACKABLE_CEILING);

    //materialVolume tag is implied by any of the other material volume flags
    if( group->water_volume        || 
        group->sewerwater_volume|| 
        group->redwater_volume    ||
        group->lava_volume        ||
        group->door_volume        ||
        group->materialVolume    )
        group->materialVolume = 1;
    else
        group->materialVolume = 0;

    if (group->lod_near || group->lod_far || group->lod_nearfade || group->lod_farfade || group->lod_scale) {
        group->lod_fromtrick = 1;
        group->lod_autogen = 0;
    } else 
        group->lod_fromtrick = 0;

    groupSetLodValues(group); // Reapply any LOD values that were attached to the def at load time

    PERFINFO_AUTO_STOP_CHECKED("groupSetTrickFlags");
}


static void resetDef(GroupDef *def, GroupResetWhat what)
{
    if (what & RESET_BOUNDS)
        groupSetBounds(def);
    groupSetVisBounds(def);
}

static GroupResetWhat g_reset_what=0;

static int resetAll_cb(GroupDefTraverser *def_trav)
{
    resetDef(def_trav->def, g_reset_what);
#if CLIENT
    if (def_trav->ent)
        def_trav->ent->flags_cache &=~CACHEING_IS_DONE;
#endif
    return 1;
}

static int resetOnTrackers_cb(DefTracker *tracker)
{
    // should just close all deftrackers?
#if CLIENT
    vistrayClosePortals(tracker);
    if (tracker->light_grid)
        lightGridDel(tracker);
#endif
    if (tracker->entries)
        return 1;
    return 0;
}

static DefTracker **aux_trackers = 0;
void groupAddAuxTracker(DefTracker *tracker)
{
    if (!aux_trackers)
        eaCreate(&aux_trackers);
    eaPush(&aux_trackers, tracker);
}

void groupRemoveAuxTracker(DefTracker *tracker)
{
    eaFindAndRemove(&aux_trackers, tracker);
}

static void groupClearAuxTrackerDefsHelper(DefTracker* trackers)
{
    int i;
    trackers->def = 0;

    if (trackers->entries)
    {
        for (i = 0; i < trackers->count; i++)
        {
            groupClearAuxTrackerDefsHelper(&(trackers->entries[i]));
        }
    }
}

void groupClearAuxTrackerDefs(void)
{
    int i;
    for (i = 0; i < eaSize(&aux_trackers); i++)
    {
        if (aux_trackers[i])
        {
            groupClearAuxTrackerDefsHelper(aux_trackers[i]);
        }
    }
}

// Used in both trick and geo reload
void groupResetAll(GroupResetWhat what)
{
    GroupDefTraverser traverser={0};
    traverser.bottomUp = 1;
    g_reset_what = what;
#if CLIENT
    if ((what & RESET_TRICKS) || (what & RESET_LODINFOS))
        modelFreeAllLODs(!!(what & RESET_LODINFOS));
#endif
    groupProcessDefTracker(&group_info, resetOnTrackers_cb);
    if (what & RESET_TRICKS)
    {
        int i, j;
        for (i = 0; i < group_info.file_count; i++)
            for (j = 0; j < group_info.files[i]->count; j++)
                if(group_info.files[i]->defs[j])
                    groupSetTrickFlags(group_info.files[i]->defs[j]);
    }
    groupProcessDefExBegin(&traverser, &group_info, resetAll_cb);
    gridCacheReset();
#if CLIENT
    {
        int i;
        groupDrawCacheInvalidate();

        for (i=0; i<group_info.ref_count; i++) 
            colorTrackerDel(group_info.refs[i]);
        for (i=0; i<eaSize(&aux_trackers); i++)
            colorTrackerDel(aux_trackers[i]);
        baseHackeryFreeCachedRgbs();
    }
#endif
    if (what & RESET_BOUNDS) {
        groupGridRecreate();
    }
}

void groupResetOne(DefTracker *tracker, GroupResetWhat what)
{
#if CLIENT
    Model *model = (tracker && tracker->def && tracker->def->model) ? tracker->def->model : 0;

    if (!tracker)
        return;

    if (model && ((what&RESET_TRICKS) || (what&RESET_LODINFOS)))
        modelFreeLODs(model, !!(what&RESET_LODINFOS));
#endif

    if (tracker->def && (what&RESET_TRICKS))
        groupSetTrickFlags(tracker->def);

    while (tracker)
    {
        resetOnTrackers_cb(tracker);
        if (tracker->def)
        {
            resetDef(tracker->def, what);
#if CLIENT
            if (tracker->parent && tracker->parent->def)
            {
                int i;
                for (i = 0; i < tracker->parent->def->count; i++)
                    tracker->parent->def->entries[i].flags_cache &=~CACHEING_IS_DONE;
            }
#endif
        }
        tracker = tracker->parent;
    }
    gridCacheReset();
#if CLIENT
    {
        int i;
        groupDrawCacheInvalidate();

        for (i=0; i<group_info.ref_count; i++) 
            colorTrackerDel(group_info.refs[i]);
        for (i=0; i<eaSize(&aux_trackers); i++)
            colorTrackerDel(aux_trackers[i]);
        baseHackeryFreeCachedRgbs();
    }
#endif
    if (what & RESET_BOUNDS) {
        groupGridRecreate();
    }
}

static void wcollLoad(const char *dir)
{
    char    buf[1000],buf2[1000], *s;
    const char *cs;

    cs = strchr(dir,'/');
    if (!cs++)
        cs = dir;
#if CLIENT
    geoLoad(dir, LOAD_NOW, GEO_INIT_FOR_DRAWING | GEO_USED_BY_WORLD);
#endif
    strcpy(buf,dir);
    s = strrchr(buf,'.');
    if (s)
        *s = 0;
    sprintf(buf2,"%s.txt",buf);
    groupLoad(buf2);
}


int groupLoadRequiredLibsForDef(GroupDef *def)
{
    GroupFileEntry        *gf;

    if (!def || !def->in_use)
        return 0;

    //OPT: gf = groupGetFileEntryPtr(def->name);
    if (!def->gf) { // Cache GroupFileEntry in GroupDef
        gf = objectLibraryEntryFromObj(def->name);
        def->gf = gf;
        if (!def->gf)
            def->gf=(void*)-1;
    } else {
        gf = def->gf;
    }

    if (!gf || gf==(void*)-1)
        return 0;
    if (!gf->loaded)
    {
        gf->loaded = 1;
        #if SERVER
        wcollLoad(gf->name);
        #else
        {
            extern int    map_full_load;
            int            i;

            if (map_full_load)
                wcollLoad(gf->name);
            else
            {
                geoLoad(gf->name, LOAD_NOW, GEO_INIT_FOR_DRAWING | GEO_USED_BY_WORLD);
                for(i=0;i<def->count;i++)
                {
                    groupLoadRequiredLibsForDef(def->entries[i].def);
                }
            }
        }
        #endif
    }
    return 1;
}

int groupFileLoadFromName(const char *name)
{
    GroupDef        *def;
    GroupFileEntry    *gf;
    static int depth=0;

    PERFINFO_AUTO_START("groupFileLoadFromName", 1);

        if (depth==0)
        {
            if (futureDefs)
                stashTableDestroy(futureDefs);
            futureDefs = stashTableCreateWithStringKeys(1000, StashDeepCopyKeys);
        }
        depth++;

        gf = objectLibraryEntryFromObj(name);
        if (!gf)
        {
            PERFINFO_AUTO_STOP_CHECKED("groupFileLoadFromName");
            depth--;
            return 0;
        }
        def = groupDefFind(name);
        if (gf->loaded)
        {
            if (!def || !def->file)
            {
                PERFINFO_AUTO_STOP_CHECKED("groupFileLoadFromName");
                depth--;
                return 0;
            }
            def->file->force_reload=1;
            def->pre_alloc = 0;
            PERFINFO_AUTO_STOP_CHECKED("groupFileLoadFromName");
            depth--;
            return 1;
        }
        if(def)
            def->pre_alloc = 0;
        gf->loaded = 1; // gotta be before the load to avoid recursion
        wcollLoad(gf->name);
        groupLoadRequiredLibsForDef(def);

        depth--;

    PERFINFO_AUTO_STOP_CHECKED("groupFileLoadFromName");
    return 1;
}

static void addProperties(PropertyLoad **props,GroupDef *def)
{
    int            i;
    PropertyEnt    *prop;
    int iSize = eaSize(&props);

    if (!iSize)
        return;

    PERFINFO_AUTO_START("addProperties",1);

    for(i=0;i<iSize;i++)
    {
        if (!props[i]->name || !props[i]->value)
        {
            #if SERVER
                Errorf(    "Error on map load: null prop name or value (%s,%s) in %s/%s",
                        props[i]->name,
                        props[i]->value,
                        def->file->fullname,
                        def->name);
            #endif
            
            continue;
        }
 
        prop = createPropertyEnt(); 

        removeLeadingAndFollowingSpaces(props[i]->name);
        removeLeadingAndFollowingSpacesUnlessAllSpaces(props[i]->value);

        strcpy(prop->name_str, props[i]->name);
        estrPrintCharString(&prop->value_str, props[i]->value);
        prop->type = props[i]->type;

        if(!def->properties)
        {
            def->properties = stashTableCreateWithStringKeys(16,StashDeepCopyKeys);
            def->has_properties = 1;
        }
        stashAddPointer(def->properties, prop->name_str, prop, false);
    }

    groupSetTrickFlags(def);

    PERFINFO_AUTO_STOP_CHECKED("addProperties");
}

static void addFlags(DefLoad *defload,GroupDef *def)
{
    if (defload->flags & DEF_UNGROUPABLE)
        def->lib_ungroupable = 1;
    if (defload->flags & DEF_FADENODE)
        def->lod_fadenode = 1;
    if (defload->flags & DEF_FADENODE2)
        def->lod_fadenode = 2;
    if (defload->flags & DEF_NOFOGCLIP)
        def->no_fog_clip = 1;
    if (defload->flags & DEF_NOCOLL)
        def->no_coll = 1;
    if (defload->flags & DEF_NOOCCLUSION)
        def->no_occlusion = 1;
}

#if SERVER
#include "cmdparse/cmdserver.h"
#include "group/groupdbmodify.h"
#endif

GroupDef *groupDefFuture(const char *name)
{
    GroupDef    *def;
    GroupFile    *file;
    StashElement    result;

    if (!(file=group_info.pre_alloc))
    {
        file = group_info.pre_alloc = calloc(sizeof(*file),1);
        file->pre_alloc = 1;
    }
    def = groupDefNew(file);

    stashAddPointerAndGetElement(group_info.groupDefNameHashTable, name, def, false, &result);
    def->name = stashElementGetStringKey(result);
    objectLibraryGetBounds(name,def);
    return def;
}

//Convert the "Group" from the map files to the GroupEnt+GroupDef of the scenegraph
static void addGroups( DefLoad *defload, GroupDef *def, void *namelist, const char *fname ) //fname for debug only
{
    int        i, count;
    char    newname[400];
    GroupEnt * entry;
    GroupLoad * groupLoad;

    PERFINFO_AUTO_START("addGroups",1);

    count = eaSize(&defload->groups);
    if (!count)
    {
        PERFINFO_AUTO_STOP_CHECKED("addGroups");
        return;
    }

    def->entries = calloc( sizeof(GroupEnt), count );
    def->count = 0;

    for ( i = 0 ; i < count ; i++ )
    {
        groupLoad    = defload->groups[i]; //Source
        entry        = &def->entries[def->count]; //Target
        
        groupRename( namelist, groupLoad->name, newname, RENAME_ENTRY );

        //Find the def; if it hasn't been loaded yet, load it.
        entry->def = groupDefFind( newname );
        if ( !entry->def )
        {
            if (group_info.load_full || g_force_load_full)
            {
                groupFileLoadFromName(newname);
                entry->def = groupDefFind(newname);
                if (!entry->def)
                {
                    if (!stashFindPointer(futureDefs,newname,NULL) && stricmp(groupLoad->name,"object_library/Omni/NAVCMBT")!=0 && stricmp(groupLoad->name,"object_library/Omni/_NAVCMBT")!=0)
                        ErrorFilenamef( fname, "Group %s\ncan't find member %s\n", def->name, groupLoad->name );
                    entry->def = groupDefFuture(newname);
                }
            }
            else
                entry->def = groupDefFuture(newname);
        }

        if ( entry->def )
        {
            def->count++;
            entry->mat = mpAlloc(group_info.mat_mempool);
            copyVec3(groupLoad->pos,((Mat4Ptr)entry->mat)[3]);
            createMat3YPR((Mat4Ptr)entry->mat,groupLoad->pyr);
            def->disable_planar_reflections = (groupLoad->flags & GROUP_DISABLEPLANARREFLECTIONS) ? 1 : 0;
        }
        else
        {
            if (!stashFindPointer(futureDefs,newname,NULL) && stricmp(groupLoad->name,"object_library/Omni/NAVCMBT")!=0 && stricmp(groupLoad->name,"object_library/Omni/_NAVCMBT")!=0)
                ErrorFilenamef( fname, "Group %s\ncan't find member %s\n", def->name, groupLoad->name );
            #if SERVER
            if (server_state.prune)
            {
                int t = group_info.loading;
                group_info.loading = 0;
                checkoutDefFile(def);
                group_info.loading = t;
            }
            #endif
        }
    }

    PERFINFO_AUTO_STOP_CHECKED("addGroups");
}

static void addFileIdx(GroupDef *def)
{
    #if SERVER
    {
    extern        int map_full_load;

        if (map_full_load)
            def->file_idx = -1;
    }
    #endif
}

static void addReplaceTex(ReplaceTexLoad **replaces,GroupDef *def)
{
    int        i;

    for(i=eaSize(&replaces)-1;i>=0;i--)
    {
        def->has_tex |= 1 << replaces[i]->tex_unit;
        groupAllocString(&def->tex_names[replaces[i]->tex_unit]);
        strcpy(def->tex_names[replaces[i]->tex_unit],replaces[i]->tex_name);
    }
}

static void addAlpha(DefLoad *defload, GroupDef *def)
{
    def->groupdef_alpha = defload->alpha;
    if (defload->alpha)
        def->has_alpha = 1;
}

static void addTexSwap(TexSwapLoad **texSwaps,GroupDef *def)
{
    int i;
    for(i = 0; i < eaSize(&texSwaps); i++)
        if(texSwaps[i]->tex_swap_name && texSwaps[i]->rep_swap_name)
            eaPush(&def->def_tex_swaps, createDefTexSwap(texSwaps[i]->tex_swap_name, texSwaps[i]->rep_swap_name, texSwaps[i]->composite));
}

void groupSetLodValues(GroupDef *def) // TODO: move this to grouputil or something?
{
    // lod_near/lod_nearfade comes *only* from trick files
    if (def->lod_fromfile) {
        if (!def->lod_fromtrick)
        {
            def->lod_far        = def->from_file.lod_far;
            def->lod_farfade    = def->from_file.lod_farfade;
            def->lod_autogen    = 0;
        }
        def->lod_scale        = def->from_file.lod_scale;
    }
    if (!def->lod_scale)
        def->lod_scale = 1.f;
    if (!def->lod_far && def->model) {
        // auto-gen values, even if thre is an lod trick (trick on "graffitti" has an lod_farfade but wants the lod_far to be auto-gen'd)
        F32 maxrad;
        Vec3 dv;
        Model *model = def->model;
        subVec3(model->max,model->min,dv);
        maxrad = lengthVec3(dv) * 0.5f + def->shadow_dist;
        def->lod_far = 10 * (maxrad + 10);
        def->lod_autogen = 1;
    }
    if (!def->lod_farfade && def->model && !def->lod_fromtrick) {
        // Only auto-gen lod_farfade if there is no lod trick (trick on doors have a lod_far, but want an lod_farfade of 0)
        def->lod_farfade = def->lod_far * 0.25;
        def->lod_autogen = 1;
    }
}

static void addLod(LodLoad **lodp,GroupDef *def)
{
    LodLoad    *lod;

    if (!lodp) {
        def->lod_fromfile = 0;
        def->from_file.lod_scale = 0; // Clear this just in case
        return;
    }
    lod = lodp[0];
    // Save original values for Trick reloading
    def->from_file.lod_far        = lod->lod_far;
    def->from_file.lod_farfade    = lod->lod_farfade;
    def->from_file.lod_scale    = lod->lod_scale;
    def->lod_fromfile = 1;
    groupSetLodValues(def);
}

static void addTintColor(TintColorLoad **tint,GroupDef *def)
{
    if (!eaSize(&tint))
    {
        return;
    }
    memcpy(def->color,tint[0]->rgbs,8);
    def->has_tint_color = 1;
}

static void addAmbient(AmbientLoad **amb,GroupDef *def)
{
    if (!eaSize(&amb))
    {
        return;
    }
    memcpy(def->color,amb[0]->rgb,4);
    def->has_ambient = 1;
}

static void addFog(FogLoad **fog,GroupDef *def)
{
    if (!eaSize(&fog))
        return;
    memcpy(def->color,fog[0]->rgbs,8);
    def->fog_radius = fog[0]->radius;
    def->fog_near = fog[0]->neardist;
    def->fog_far = fog[0]->fardist;
    def->fog_speed = fog[0]->speed ? fog[0]->speed : 1.0f;
    def->has_fog = 1;
}

static void addBeacon(BeaconLoad **beacon,GroupDef *def)
{
    if (!eaSize(&beacon))
        return;

    groupAllocString(&def->beacon_name);
    strcpy(def->beacon_name,beacon[0]->name);
    def->beacon_radius = beacon[0]->radius;
    def->has_beacon = 1;
}

static void addSound(SoundLoad **sound,GroupDef *def)
{
    int        ivol;

    if (!eaSize(&sound))
        return;
    groupAllocString(&def->sound_name);
    strcpy(def->sound_name,sound[0]->name);
    ivol = 128 * sound[0]->volume;
    def->sound_vol = MIN(ivol,255);
    def->sound_radius = sound[0]->radius;
    def->has_sound = 1;
    def->sound_ramp = sound[0]->ramp;
    if (sound[0]->exclude)
        def->sound_exclude = 1;
}

static void addSoundScript(DefLoad *defload,GroupDef *def)
{
    if (!defload->sound_script)
        return;
    groupAllocString(&def->sound_script_filename);
    strcpy(def->sound_script_filename,defload->sound_script);
}

static void addOmniLight(OmniLoad **omni,GroupDef *def)
{
    if (!eaSize(&omni))
        return;

    memcpy(def->color,omni[0]->rgb,4);
    def->light_radius = omni[0]->radius;
    if (omni[0]->negative)
        def->color[0][3] = 1;
    def->has_light = 1;
}

static void addCubemap(CubemapLoad **cubemap,GroupDef *def)
{
    if (!eaSize(&cubemap))
        return;

    def->cubemapGenSize = cubemap[0]->genSize;
    def->cubemapCaptureSize = cubemap[0]->captureSize;
    def->cubemapBlurAngle = cubemap[0]->blur;
    def->cubemapCaptureTime = cubemap[0]->time;
    def->has_cubemap = 1;
}

static void addVolume(VolumeLoad **volume,GroupDef *def)
{
    if (!eaSize(&volume))
        return;
    copyVec3(volume[0]->scale,def->box_scale);
    def->has_volume = 1;
}

static void addType(DefLoad *defload,GroupDef *def)
{
    if (!defload->type)
        return;
    groupAllocString(&def->type_name);
    strcpy(def->type_name,defload->type);
}

static GroupDef* addDef(GroupFile *file, DefLoad *defload, void * namelist, const char * fname )
{
    GroupDef    *def;
    char        newname[400];
    int            i,move;

    PERFINFO_AUTO_START("addDef",1);

    if ( 0 == eaSize(&defload->groups) && !defload->obj_name) //Ignore degenerate Group
    {
        PERFINFO_AUTO_STOP_CHECKED("addDef");
        return NULL;
    }

    groupRename(namelist,defload->name,newname,RENAME_DEF); //TO DO: make sure this has zero side effects
    def = groupDefFind(newname);
    if (!def)
        def = groupDefNew(file);
    else if (def->file != file)
        groupAssignToFile(def,file);
    //addModel
    if (defload->obj_name)
    {
        def->model = groupModelFind(defload->obj_name);
        if (!def->model)
            Errorf("Cant find root geo %s",defload->obj_name);
        groupSetTrickFlags(def);
    }
    groupDefName(def,file->fullname,newname); //TO DO how exactly this work?

    addGroups( defload, def, namelist, fname ); //fname for debug only

    //remove NULL entries and shift everything over, this keeps things in order,
    //not sure if that is necessary or not
    move=0;
    for (i=0;i<def->count;i++)
    {
        while (i+move<def->count && def->entries[i+move].def==NULL)
            move++;
        if (move>0 && i+move<def->count)
            def->entries[i] = def->entries[i+move];
    }
    def->count-=move;

    if (!def->count && !def->model) //Ignore degenerate Group
    {
        groupDefFree(def,1);
        PERFINFO_AUTO_STOP_CHECKED("addDef");
        return NULL;
    }
    addFileIdx(def); //TO DO ??
    addProperties(defload->properties,def);
    addFlags(defload,def);
    addLod(defload->lod,def);
    addReplaceTex(defload->replace_tex,def);
    addTexSwap(defload->tex_swap,def);
    addTintColor(defload->tintcolor,def);
    addAmbient(defload->ambient,def);
    addFog(defload->fog,def);
    addBeacon(defload->beacon,def);
    addSound(defload->sound,def);
    addSoundScript(defload,def);
    addOmniLight(defload->omni,def);
    addCubemap(defload->cubemap,def);
    addVolume(defload->volume,def);
    addType(defload,def);
    addAlpha(defload,def);

    if (strnicmp(def->name, "grpa", 4)==0)
        def->is_autogroup = 1;

    groupSetBounds(def);
    groupSetVisBounds(def);
#if CLIENT
    setClientFlags(def, fname);
#endif
    def->init = 1;
    def->saved = 1;

    PERFINFO_AUTO_STOP_CHECKED("addDef");
    return def;
}

GroupDef *loadDef(const char *newname)
{
    GroupDef    *def;

    def = groupDefFind(newname);
    if (!def || def->pre_alloc)
    {
        groupFileLoadFromName(newname);
        def = groupDefFind(newname);
        if (!def)
            return 0;
    }
    if (!def->child_valid)
    {
        int            i;
        
        for(i=0;i<def->count;i++)
            loadDef(def->entries[i].def->name);
    }
    def->child_valid = 1;
    return def;
}

static DefTracker * addRef(RefLoad *refload,void *namelist)
{
    GroupDef    *def;
    DefTracker    *ref;
    char        newname[400];

    groupRename(namelist,refload->name,newname,RENAME_REF);
#if CLIENT
    def = groupDefFind(newname);
    if (!def)
        def = groupDefFuture(newname);
#else
    #if 0
    def = loadDef(newname);
    if (!def)
    {
        Errorf("Missing reference: %s\n",newname);
        return 0;
    }
    #else
    def = groupDefFind(newname);
    if (!def)
    {
        groupFileLoadFromName(newname);
        def = groupDefFind(newname);
        if (!def)
        {
            Errorf("Missing reference: %s\n",newname);
            def = groupDefFuture(newname);
        }
    }
    #endif
#endif

    ref = groupRefNew();
    ref->def = def;
    copyVec3(refload->pos,ref->mat[3]);
    createMat3YPR(ref->mat,refload->pyr);
    groupRefActivate(ref);
    return ref;
}

static int groupLoadRaw(const char *fname,GroupFileLoad *group_file)
{
    TokenizerHandle tok;
    int                fileisgood;

    if ( !fileExists( fname ) )
        return 0;

    tok = TokenizerCreate(fname);
    if (!tok)
        return 0;
    fileisgood = TokenizerParseList(tok, parse_group_list, group_file, TokenizerErrorCallback);
    TokenizerDestroy(tok);
    if (!fileisgood)
        return 0;

    return 1;
}

static int groupLoadRawMem(const char *mem,GroupFileLoad *group_file)
{
    TokenizerHandle tok;
    int                fileisgood;

    if ( !mem )
        return 0;

    PERFINFO_AUTO_START("groupLoadRawMem",1);

    tok = TokenizerCreateString(mem, -1);
    if (!tok)
    {
        PERFINFO_AUTO_STOP_CHECKED("groupLoadRawMem");
        return 0;
    }
    fileisgood = TokenizerParseList(tok, parse_group_list, group_file, TokenizerErrorCallback);
    TokenizerDestroy(tok);

    PERFINFO_AUTO_STOP_CHECKED("groupLoadRawMem");

    if (!fileisgood)
        return 0;

    return 1;
}

static int isFileVersion2(const char *fname)
{
    char    buf[100];
    FILE    *file;

    if( !fileExists(fname) )//sure an nonexistent file is version 2
        return 1;
    file = fileOpen(fname,"rt");
    if (!file)
        return 0;
    fread(buf,sizeof(buf),1,file);
    fclose(file);
    if (strnicmp(buf,"Version 2",9)!=0)
        return 0;
    return 1;
}

static char *getLibRootName(const char *fname,char *rootname)
{
    char    *s;

    if (!fname)
        return 0;
    strcpy(rootname,fname);
    s = strrchr(rootname,'.');
    if (s && strstri(rootname,"object_library"))
    {
        strcpy(s,".rootnames");
        return rootname;
    }
    return 0;
}

static void convertFile(const char *fname,char *newfname)
{
    char        *mem,*s,*args[100];
    int            i,count,in_group=0,in_lod=0,enddefref;
    FILE        *file;

    s = mem  = fileAlloc(fname,0);
    file = fileOpen(newfname,"wt");
    fprintf(file,"Version 2\n");
    for(;;)
    {
        count=tokenize_line(s,args,&s);
        if (!s)
            break;
        enddefref = 0;
        if (!count)
        {
            fprintf(file,"\n");
            continue;
        }
        if (stricmp(args[0],"DefEnd")==0 || stricmp(args[0],"RefEnd")==0)
        {
            args[0] = "End";
            enddefref = 1;
        }
        if (in_group)
        {
            if (!(stricmp(args[0],"Pos")==0 || stricmp(args[0],"Rot")==0))
            {
                fprintf(file,"\tEnd\n");
                in_group = 0;
            }
        }
        if (stricmp(args[0],"LodFar")==0 || stricmp(args[0],"LodFarFade")==0
            || stricmp(args[0],"LodNear")==0 || stricmp(args[0],"LodNearScale")==0
            || stricmp(args[0],"LodScale")==0)
        {
            if (!in_lod)
            {
                fprintf(file," Lod\n");
            }
            in_lod++;
            args[0] = &args[0][3];
        }
        else
        {
            if (in_lod)
            {
                in_lod = 0;
                fprintf(file," End\n");
            }
        }
        if (stricmp(args[0],"Fadenode")==0 || stricmp(args[0],"Ungroupable")==0)
        {
            fprintf(file,"\tFlags ");
            enddefref = 1;
        }
        if (stricmp(args[0],"Group")==0)
            in_group++;
        if (!(enddefref || stricmp(args[0],"Def")==0 || stricmp(args[0],"Ref")==0 || stricmp(args[0],"RootMod")==0))
            fprintf(file,"\t");
        if (in_group>1 || in_lod>1)
            fprintf(file,"\t");
        if (stricmp(args[0],"negative")==0)
            fseek(file,-3,1);
        for(i=0;i<count;i++)
        {
            fprintf(file,"%s ",args[i]);
        }
        fprintf(file,"\n");
    }
    fclose(file);
}

static char *makeBinName(const char *fname,char *buf)
{
    const char    *cs;
    char *s;

    if (strstriConst(fname, "BACKUPS")) {
        // *Never* load .bin files when loading a backup!
        sprintf(buf, "");
        return NULL;
    }
    cs = strstriConst(fname,"object_library");
    if (!cs)
        cs = strstriConst(fname,"maps");
    if (!cs)
        return 0;
    sprintf(buf,"geobin/%s",cs);
    //strcpy(buf,fname);
    s = strrchr(buf,'.');
    if (!s)
        s = buf + strlen(buf);
    strcpy(s,".bin");
    return buf;
}



static void groupWriteGroupAndRootnameToBin( GroupFileLoad * group_file, const char * fname, char * librootname )
{
    //FileList filelist;
    char    fullbinpath[1000];
    char    bin_name[1000];

    PERFINFO_AUTO_START("groupWriteGroupAnRootNameToBin",1);

    if (!makeBinName(fname,bin_name))
    {
        PERFINFO_AUTO_STOP_CHECKED("groupWriteGroupAnRootNameToBin");
        return;
    }

    //FileListCreate(&filelist);

    FileListInsert(&g_parselist, fname, 0);
    if ( fileExists( librootname ) )
        FileListInsert(&g_parselist, librootname, 0);

    sprintf(fullbinpath,"%s/%s", fileDataDir(), bin_name);
    forwardSlashes(fullbinpath);
    mkdirtree(fullbinpath);
    ParserWriteBinaryFile(bin_name, parse_group_list, group_file, &g_parselist, NULL, 0, 0);
    //FileListDestroy(&filelist);

    PERFINFO_AUTO_STOP_CHECKED("groupWriteGroupAnRootNameToBin");
}

///*
static GroupFile *groupLoadInternal(const char *fname, BinForceType reprocessBins, GroupFile *file)
{
    char fnamebuf[MAX_PATH];
    GroupFileLoad    group_file = {0};        //Root parse structure to parse into
    char        *librootname,buf[1000]; //If this is the object_library, and there is a .rootname file, include it 
    int            loadedFromBin = 0;
    char *layer;
    char * * oldptr1=0;
    char * * oldptr2=0;
    char * oldval1=0;
    char * oldval2=0;

    PERFINFO_AUTO_START("groupLoadInternal",1);

    strcpy(fnamebuf, fname);
    fname = forwardSlashes(fnamebuf);
    layer = strstri(fnamebuf, "_layer_");
    addDependency(fname);

    {
        GroupFileEntry *gf = objectLibraryEntryFromFname(fname);
        if (gf)
            gf->loaded = 1;
    }

    group_info.loading++;
    librootname = getLibRootName(fname,buf); //Convert to .rootnames

    //Load file from the bin if the bin is up to date with the regular .txt and the .rootnames file for object library
    if (fname && (reprocessBins == BIN_LOAD_NORMALLY))
    {
        SimpleBufHandle binfile = 0;
        char bin_name[1000];
        if (makeBinName(fname,bin_name))
        {
            if( fileExists( bin_name ) )
                binfile = ParserIsPersistNewer(NULL, NULL, bin_name, parse_group_list, NULL);
            if ( binfile ) 
                loadedFromBin = ParserReadBinaryFile(binfile, bin_name, parse_group_list, &group_file, NULL, NULL) ;
#ifndef FINAL
            else if(!isDevelopmentMode()) {
                printf("Failed to load group file: %s\n", bin_name);
            }
#endif
        }
    }

    //If you didn't get it from the bin, get it from the .txt file (and .rootname file if it's object_library)
    //DEV mode only.  Write back to bin. TO DO: why write the file to the bin right now? Will includes affect this?
    //(Non version 2 stuff will just throw parser errors now, I'm pretty sure I converted them all.)
    if ( !loadedFromBin )
    {
        FileListDestroy(&g_parselist); //cleans out g_parselist. I don't know why it's not cleaned out already...
        groupLoadRaw( librootname, &group_file ); //only if it exists. (Duplicates in .txt and ,rootnames are concatenated 
        groupLoadRaw( fname, &group_file );
        if ( reprocessBins != BIN_REBUILD_MAP && isDevelopmentMode())
            groupWriteGroupAndRootnameToBin( &group_file, fname, librootname );
        if (file_dep_info) { // Add include files to depency file
            FileListForEach(&g_parselist, addDependencyCallback);
        } else { // The dep file will now be out of date so delete it
            char tmp[1000];
            if(makeBinName(fname,tmp))
            {
                changeFileExt(tmp, ".dep", tmp);
                fileLocateWrite(tmp, tmp);
                fileForceRemove(tmp);
            }
        }
        FileListDestroy(&g_parselist);
    }

    //Major Hack while I figure out how to handle the editing of Individual layers
    if(!librootname && layer)
    {
        group_info.grp_idcnt += (U32)((strlen(fname) * strlen(fname)));
        if( strstriConst( fname, "Geometry" ) )
            group_info.grp_idcnt += 20101; //Make sure the range is outside the possible range of toher people editting the map
    }
    //End Hack

    //If you are really loading the game, convert parsed file to game format
    if ( reprocessBins != BIN_REBUILD_OBJLIB )
    {
        int i, count = 0;
        void *namelist;
        GroupDef *lastDef = NULL;
        DefTracker * newRef = 0; //temp for Layer Convertion

        //Load parsed group_file into game's group_info
        if(group_file.scenefile)
        {
            assertmsg(!librootname, "How did a library piece get a scene file?"); //library pieces should never have scenefiles...
            strcpy(group_info.scenefile,group_file.scenefile);
        }

        if(group_file.loadscreen)
        {
            assertmsg(!librootname, "Library piece with loadscreen");
            strncpy(group_info.loadscreen,group_file.loadscreen, MAX_PATH);
        }

        //Rename the root ref and the def it refers to with the file name
        namelist = groupRenameCreate(fname); //Notice we're using .txt version not .rootnames to name

        if (!file)
        {
            file = groupFileNew();
            groupFileSetName(file,fname);
        }

        count = eaSize(&group_file.defs);

        if (futureDefs)
        {
            for(i=0;i<count;i++)
            {
                stashAddPointer(futureDefs,group_file.defs[i]->name,group_file.defs[i]->name,0);
            }
        }

        for(i=0;i<count;i++)
            lastDef = addDef( file, group_file.defs[i], namelist, fname ); //fname for debug only        GroupDef *lastDef;

        if(layer && count)
        {
            int shared;
            char *s;
            assert(lastDef && stashFindPointer(lastDef->properties, "Layer", NULL)); // the root is saved last

            *layer = '\0';
            if( !(s = strstri(fnamebuf, "maps/")) )
                s = fnamebuf;
            shared = strstri(file->fullname, s) == NULL; // s includes from maps/ onward, but not _layer_ onward
            *layer = '_';

            if(shared) // imported from another map
            {
                PropertyEnt *prop = createPropertyEnt();
                strcpy(prop->name_str, "SharedFrom");
                estrPrintCharString(&prop->value_str, s); // s includes from maps/ onward, including _layer_ onward
                prop->type = String;
                assert(stashAddPointer(lastDef->properties, prop->name_str, prop, false));
            }
            else
                stashRemovePointer(lastDef->properties, "SharedFrom", NULL);
        }

        count = eaSize(&group_file.refs);

        for(i=0;i<count;i++)
        {
            newRef = addRef( group_file.refs[i], namelist ); //Could use fname for debug, too
        }

        groupRenameFree(namelist); //TO DO why this hash table??
    }

    /*if( !strstri( fname, "object_library" ) ) 
    {
        int ix; 
        for( ix = 0; ix < group_info.def_count ; ix++ )
        {
            if ( !(group_info.defs[ix] && group_info.defs[ix]->in_use ) )
                continue;
            if( strstri( group_info.defs[ix]->name, "grpa" ) )
                printf( "!!!!!!!!!!!" );
        }
        printf("");
    }*/

    /////// Layer autogrouping
    if ( !SERVER_OR_GAMESTATE.doNotAutoGroup)
    {
        int i;
        DefTracker * ref;

        if( !strstriConst( fname, "object_library" ) )
        {
            for( i=0 ; i < group_info.ref_count ; i++ )
            {
                ref = group_info.refs[i];

                //If this is a layer and has changed, try to check it out.
                if (groupRefShouldAutoGroup(ref))
                {
                    if( ref->def->count > 1)
                    {
                        PERFINFO_AUTO_START("autoGroup", 1);
                            PERFINFO_AUTO_START("autoGroupSelection", 1);
                                autoGroupSelection( ref );
                            PERFINFO_AUTO_STOP_START("groupRecalcAllBounds", 1);
                                groupRecalcAllBounds();
                            PERFINFO_AUTO_STOP();
                        PERFINFO_AUTO_STOP();
                    }    
                }
            }
        }
    }
    ///////// End Layer Autogrouping

    //Load up any imported files
    {
        int i, j, count = eaSize(&group_file.importFiles);
        for(i = 0; i < count; i++)
        {
            //Sanity check that you aren't importing the same file twice.
            for(j = 0; j < i; j++)
            {
                if(group_file.importFiles[j][0] && 0 == stricmp(group_file.importFiles[i], group_file.importFiles[j]))
                {
                    if(isDevelopmentMode())
                        Errorf( "The map %s is trying to import %s twice.  I'm assuming that's not what you want to be doing and ignoring it, but you should probably check with a programmer to figure out what happened.", fname, group_file.importFiles[i]);
                    group_file.importFiles[i][0] = 0;
                    break;
                }
            }
            //End Sanity Check

            if(group_file.importFiles[i][0])
                groupLoadInternal(group_file.importFiles[i], reprocessBins, file);
        }
    }
    ParserDestroyStruct(parse_group_list, &group_file);
    
    group_info.loading--;
    
    PERFINFO_AUTO_STOP();

    return file;
}
//*/


GroupFile *groupLoadTo(const char *fname,GroupFile *file)
{
    GroupFile* returnFile = groupLoadInternal(fname,BIN_LOAD_NORMALLY,file);
    
    return returnFile;
}

static void groupFileLoadAllDefs(GroupFile* file)
{
    int i;
    
    if(!file)
    {
        return;
    }
    
    for(i=0;i<file->count;i++)
    {
        if(file->defs[i])
        {
//            printf("LOADING: %s\n", file->defs[i]->name);
        
            loadDef(file->defs[i]->name);
        }
    }
}

static void groupLoadAllRefDefs()
{
    int i;
    
    for(i = 0; i < group_info.ref_count; i++)
    {
        DefTracker* ref = group_info.refs[i];
        
        if(ref && ref->def)
        {
            loadDef(ref->def->name);
        }
    }
}

GroupFile *groupLoadFull(const char *fname)
{
    extern void groupFileRecalcBounds(GroupFile *file);
    GroupFile    *file;

    PERFINFO_AUTO_START("groupLoadFull",1);

    file = groupFileFromName(fname);
    if (!file)
    {
        PERFINFO_AUTO_STOP();
        return 0;
    }
    groupFileLoadAllDefs(file);
    groupLoadAllRefDefs();
    groupFileRecalcBounds(file);
    objectLibraryWriteBoundsFileIfDifferent(file);
    PERFINFO_AUTO_STOP();
    return file;
}

#ifdef SERVER

static        StashTable htProcessed=0;

static FileScanAction makeObjLibBin(char* dir, struct _finddata32_t* data)
{
    int            len;
    char        fname[1000],*s;

    PERFINFO_AUTO_START("makeObjLibBin",1);
    if (0==htProcessed) {
        htProcessed = stashTableCreateWithStringKeys(256, StashDeepCopyKeys);
    }

    len = (int)strlen(data->name);
    if(!strstr(dir, "/_") && !(data->name[0] == '_'))
    {
        if (strEndsWith(data->name, ".txt") || strEndsWith(data->name, ".rootnames"))
        {
            sprintf(fname,"%s/%s",dir,data->name);
            s = strrchr(fname,'.');
            strcpy(s,".txt");
            if (0==stashFindPointerReturnPointer(htProcessed, fname))
            {
                GroupFile    *file;
                printf("binning %s\n",fname);
                groupLoadInternal(fname,BIN_REBUILD_OBJLIB,0);
                file = groupLoadFull(fname);
                if (file)
                    objectLibraryWriteBoundsFile(file);
                if (group_info.file_count > 1000)
                    groupReset();
                stashAddInt(htProcessed, fname, 1,false);
            }
        }
    }
    PERFINFO_AUTO_STOP();
    return FSA_EXPLORE_DIRECTORY;
}

static FileScanAction makeMapStat(char* dir, struct _finddata32_t* data)
{
    int            len;
    char        fname[1000],*s;

    len = (int)strlen(data->name);
    if(!strstr(dir, "/_") && !(data->name[0] == '_'))
    {
        if (strEndsWith(data->name, ".txt") && !strstri(data->name, "_layer_") && !strstri(data->name, "city name guide") && !strstri(data->name, "in this dir") )
        {
            sprintf(fname,"%s/%s",dir,data->name);
            s = strrchr(fname,'.');
            strcpy(s,".txt");
            groupLoadInternal(fname,BIN_LOAD_NORMALLY,0);
            if(server_state.map_stats)
                MissionMapStats_MakeFile( fname );
            if(server_state.map_minimaps)
                minimap_saveHeader(fname);
            groupReset();
        }
    }
    return FSA_EXPLORE_DIRECTORY;
}

void groupBinCurrent(const char *outname)
{
	char *memsave=0;
	GroupFileLoad mem_group_file = {0};

	printf("binning map %s\n", outname);

	estrCreate(&memsave);
	groupSaveToMem(&memsave);

	groupLoadRawMem(memsave, &mem_group_file);
	groupWriteGroupAndRootnameToBin(&mem_group_file, outname, 0);

	ParserDestroyStruct(parse_group_list, &mem_group_file);
	estrDestroy(&memsave);
}

static FileScanAction makeMapBin(char* dir, struct _finddata32_t* data)
{
    int            len;
    char        bin_name[1000];
    char        fname[1000],*s;

    PERFINFO_AUTO_START("makeMapBin",1);
    if (0==htProcessed) {
        htProcessed = stashTableCreateWithStringKeys(256, StashDeepCopyKeys);
    }

    len = (int)strlen(data->name);
    if(!strstr(dir, "/_") && !(data->name[0] == '_'))
    {
        if (strEndsWith(data->name, ".txt") && !strstri(data->name, "_layer_") && !strstri(data->name, "city name guide") && !strstri(data->name, "in this dir") )
        {
            sprintf(fname,"%s/%s",dir,data->name);
            s = strrchr(fname,'.');
            strcpy(s,".txt");
            if (0==stashFindPointerReturnPointer(htProcessed, fname))
            {
                makeBinName(fname,bin_name);
                if (!fileExists(bin_name) || !dependenciesUpToDate(bin_name))
                {
                    char *memsave=0;
                    GroupFileLoad mem_group_file = {0};

                    printf("binning map %s\n",fname);
                    startDependencies();
                    groupLoadInternal(fname,BIN_REBUILD_MAP,0);
                    writeDependencies(bin_name);


                    estrCreate(&memsave);
                    groupSaveToMem(&memsave);
                    groupReset();

                    groupLoadRawMem(memsave, &mem_group_file);
                    groupWriteGroupAndRootnameToBin(&mem_group_file, fname, 0);

                    ParserDestroyStruct(parse_group_list, &mem_group_file);
                    estrDestroy(&memsave);
                }

                stashAddInt(htProcessed, fname, 1,false);
            }
        }
    }
    PERFINFO_AUTO_STOP();
    return FSA_EXPLORE_DIRECTORY;
}

static FileScanAction removeOldBin(char* dir, struct _finddata32_t* data)
{
    int            len;
    char        buf[1000];
    char        fname[1000],*s;

    PERFINFO_AUTO_START("removeOldBin",1);
    if (0==htProcessed) {
        htProcessed = stashTableCreateWithStringKeys(256, StashDeepCopyKeys);
    }

    len = (int)strlen(data->name);
    if(!strstr(dir, "/_") && !(data->name[0] == '_'))
    {
        if(strEndsWith(data->name, ".bin"))
        {
            char *dir2 = strstr(dir, "geobin/");
            if (dir2)
                dir2 += 7;
            else
                dir2 = dir;
            sprintf(fname,"%s/%s",dir2,data->name);
            s = strrchr(fname,'.');
            strcpy(s,".txt");
            if (0==stashFindPointerReturnPointer(htProcessed, fname))
            {
                sprintf(fname,"%s/%s",dir,data->name);
                printf("removing bin %s\n", fname);
                fileLocateWrite(fname, buf);
                fileForceRemove(buf);
                s = strrchr(buf,'.');
                strcpy(s,".dep");
                fileForceRemove(buf);
                strcpy(s,".bounds");
                fileForceRemove(buf);
            }
        }
    }
    PERFINFO_AUTO_STOP();
    return FSA_EXPLORE_DIRECTORY;
}


#endif

void groupLoadMakeAllMapStats(void)
{
#ifdef SERVER
    loadstart_printf("Binning maps..\n");
    groupReset();
    fileScanAllDataDirs("maps", makeMapStat);
    loadend_printf("\n");
#endif
}

static FileScanAction makeMapImage(char* dir, struct _finddata32_t* data)
{
#ifdef CLIENT    
    int            len;

    len = strlen(data->name);
    if(!strstr(dir, "/_") && !(data->name[0] == '_'))
    {
        if (strEndsWith(data->name, ".txt") && !strstri(data->name, "_audio")
            && !strstri(data->name, "mapstats") && !strstri(data->name, "_layer_") 
            && !strstri(data->name, "autosave") && !strstri(data->name, "clientsave")
            && !strstri(data->name, "city name guide") && !strstri(data->name, "in this dir") )
        {
            ImageCapture_WriteMapImages(data->name, dir, 1);
        }
    }
#endif
    return FSA_EXPLORE_DIRECTORY;
}

void groupLoadMakeAllMapImages(char *directory)
{
#ifdef CLIENT
    if(!directory || strlen(directory) ==0)
        directory = "maps";
    loadstart_printf("Binning map images..\n");
    //Testing a subset: "maps/Missions/5th_Column"
    //real (all): "maps"
    game_state.making_map_images = 1;
    fileScanAllDataDirs(directory, makeMapImage);
    game_state.making_map_images = 0;

    loadend_printf("\n");
#endif
}

void groupLoadMakeAllBin(int force_level)
{
#ifdef SERVER
    PERFINFO_AUTO_START("groupLoadMakeAllBin",1);
    loadstart_printf("Binning object library..\n");
    fileScanAllDataDirs("object_library", makeObjLibBin);
    loadend_printf("\n");
    loadstart_printf("Binning maps..\n");
    groupReset();
    fileScanAllDataDirs("maps", makeMapBin);
    loadend_printf("\n");
    loadstart_printf("Removing old bins..\n");
    groupReset();
    fileScanAllDataDirs("geobin", removeOldBin);
    loadend_printf("\n");
    PERFINFO_AUTO_STOP();
#endif
}

void groupLoadIfPreload(GroupDef *def)
{
    if (def && def->file && (def->pre_alloc || def->file == group_info.pre_alloc))
        groupFileLoadFromName(def->name);
}


GroupFile *groupLoad(const char *fname)
{
    GroupFile    *file = groupLoadInternal(fname,BIN_LOAD_NORMALLY,0);
    return file;
}

GroupFile *groupLoadFromMem(const char *map_data)
{
    char    *fakename = "c:/temp/tempmap.txt";
    FILE    *tempfile;
    GroupFile    *file;

    tempfile = fileOpen(fakename,"wb");
    fwrite(map_data,strlen(map_data),1,tempfile);
    fclose(tempfile);
    file = groupLoad("c:/temp/tempmap.txt");
    unlink(fakename);
    return file;
}
