#ifndef _ANIM_H
#define _ANIM_H

extern unsigned background_loader_threadID;

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/gridpoly.h>
#include "gridcoll/ctri.h"
#ifdef GETANIMATION
#define TexLoadHow int
#else
#include "render/texEnums.h"
#endif
#include "render/thread/rt_state.h"
#include <utilitieslib/components/GenericMesh.h>
#include "render/thread/rt_model_cache.h"
#include "seq/bones.h"

#define MAX_OBJBONES 15 //mm
#define MAX_SUBOBJS 10 //mm
#define MAX_ALTPIVOTS 15

//parameters to animload
typedef enum
{
    LOAD_BACKGROUND    = 1 << 0,
    LOAD_NOW        = 1 << 1,
    LOAD_HEADER        = 1 << 2, //used by svr sequencer where you don't need animations tracks
    LOAD_RELOAD        = 1 << 3, //used by on the fly reloading
    LOAD_FASTLOAD    = 1 << 4, //used by getvrml for onetime geo info print
} GeoLoadType;

//flags on an gld's data state
typedef enum
{
    NOT_LOADED        = 1 << 0,
    LOADING            = 1 << 1,
    LOADED            = 1 << 2,
} GeoLoadState;

//what is this anim being used for?
typedef enum
{
    GEO_DONT_INIT_FOR_DRAWING    = 1 << 0, //just called from geoload
    GEO_INIT_FOR_DRAWING        = 1 << 1, //called from modelload
    GEO_USED_BY_WORLD            = 1 << 2,
    GEO_USED_BY_GFXTREE            = 1 << 3,
    GEO_GETVRML_FASTLOAD        = 1 << 4,
    GEO_USE_MASK                = GEO_USED_BY_WORLD | GEO_USED_BY_GFXTREE,
} GeoUseType;


typedef struct Portal
{
    Vec3    pos, min, max;
    Vec3    normal;
} Portal;

typedef struct ModelExtra
{
    int        portal_count;
    Portal    portals[8];
} ModelExtra;

typedef struct PackNames
{
    char    **strings;
    int        count;
} PackNames;

typedef F32 Weights[2]; 
typedef S16 Matidxs[2]; 

typedef struct PackData
{
    int        packsize;
    U32        unpacksize;
    U8        *data;
} PackData;

//All the skinning info for this model.
typedef struct BoneInfo 
{
    int            numbones;                    //Only has boneinfo if numbones
    BoneId        bone_ID[MAX_OBJBONES];        //My geometry needs these bones
    Weights        *weights;                    //per vert: list of weights
    Matidxs        *matidxs;                    //per vert: list of mat associated with each weight
} BoneInfo;
STATIC_ASSERT(sizeof(BoneId)==4)    // this is necessary because BoneInfo is used for loading from disk


typedef struct AltPivotInfo
{
    int        altpivotcount;
    Mat4    altpivot[MAX_ALTPIVOTS];
} AltPivotInfo;


typedef struct GeoLoadData GeoLoadData;
typedef struct VBO VBO;
typedef struct TexBind TexBind;
typedef struct PerformanceInfo PerformanceInfo;
typedef struct ModelLODInfo ModelLODInfo;
typedef struct TrickNode TrickNode;
typedef struct SharedHeapHandle SharedHeapHandle;


#if CLIENT
typedef struct LodModel
{
    F32 tri_percent, error;
    struct Model *model;
} LodModel;
#endif

typedef struct Model
{
    // frequently used data keep in same cache block
    U32                flags;
    F32                radius;
#if CLIENT
    VBO                *vbo;
#endif
    S16                id;            //I am this bone 
    U8                loadstate;
    BoneInfo        *boneinfo; //if I am skinned, everything about that
    TrickNode        *trick;
    int                vert_count;
    int                tri_count;
    int                reflection_quad_count;
    GeoLoadData        *gld;

    int                tex_count;    //number of tex_idxs and blend_modes (sum of all tex_idx->counts == tri_count)
    TexID            *tex_idx;        //array of (textures + number of tris that have it)
#if CLIENT
    BlendModeType    common_blend_mode; // If all blend modes on sub-objects are the same
    BlendModeType    *blend_modes;    // blend mode for each texture
    TexBind            **tex_binds;
#endif

#if CLIENT | GETVRML
    ModelLODInfo    *lod_info;
#endif

    // collision
    PolyGrid        grid;
    CTri            *ctris;
    int                ctriflags_setonall; // Flags that are set on all tris (for optimizing redundant sets)
    int                ctriflags_setonsome; // Flags that are set on some (for optomizing redundant clears)
    int                *tags;
    F32                grid_size_orig; // Original grid size when loaded from file, for trick reloading

#if CLIENT
    // unpacked data, for z-occlusion, radiosity, and LODing
    struct
    {
        int            *tris;
        Vec3        *verts;
        Vec3        *norms;
        Vec2        *sts;
        Vec2        *sts3;
        GMeshReductions *reductions;
        Vec3        *reflection_quads;    // For planar reflections
    } unpack;

    Model            *srcmodel;            // only valid for lod models
    LodModel        **lod_models;        // only valid for non-lod models that have lods
    F32                autolod_dists[3];    // distances to place 75% tris, 50% tris, and 25% tris lods for automatic mode
    F32                lod_minerror, lod_maxerror;
    F32                lod_mintripercent, lod_maxtripercent;
#endif

    // Less frequently used data
    char            *name;
    int                namelen;
    int                namelen_notrick;
    char            *filename;
    AltPivotInfo    *api;    // if I have alternate pivot points defined for fx, everything about that
    ModelExtra        *extra;    // Portals
    Vec3            scale;    // hardly used at all, but dont remove, jeremy scaling files
    Vec3            min,max;

    struct
    {
        PackData        tris;
        PackData        verts;
        PackData        norms;
        PackData        sts;
        PackData        sts3;        // for lightmaps
        PackData        weights;
        PackData        matidxs;
        PackData        grid;
        PackData        reductions;
        PackData        reflection_quads;    // For planar reflections
    } pack;

    SharedHeapHandle*    pSharedHeapHandle;
    /*
    SharedHeapHandle*    pSharedHeapNxStreamHandle;
    char*            nxShapeStream;
    int                nxShapeStreamSize;
    */
#ifdef MODEL_PERF_TIMERS
    PerformanceInfo *perfInfo;
#endif
} Model;

// This structure is read verbatim off of disk, don't move fields around
typedef struct ModelHeader
{
    char        name[124];            //vrml file name?
    Model        *model_data;        // pointer to chunk of data models* are stored in (normally models[0], but munged on reload)
                                    // Used to be unused gld backpointer, but still in .geo files
    F32            length;                //of animation track, if any, in case the sequencer doesn't specify, kinda hack.
    Model        **models;
    int            model_count;
} ModelHeader;

typedef struct GeoLoadData
{
    struct GeoLoadData * next; //for the background loader only now, and maybe the unloader later?
    struct GeoLoadData * prev;
    ModelHeader    modelheader;
    char        name[192];
    PackNames    texnames;
    int            headersize;
    int            datasize;
    int            loadstate;
    F32            lasttimeused;
    int            type;
    void        *file;
    TexLoadHow    tex_load_style; // from main or bg thread?
    GeoUseType    geo_use_type;
    void        *header_data;
    void        *geo_data;
    int            data_offset;
    int            file_format_version;
#if CLIENT | GETVRML
    ModelLODInfo *lod_infos;
#endif
} GeoLoadData;


typedef enum
{
    PACK_F32,
    PACK_U32,
    PACK_U16,
} PackType;

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
extern StashTable glds_ht;

// Generated by mkproto
Model * modelFind( const char *name, const char * filename, int load_type, int use_type );
int uncompressDeltas(void *dst,U8 *src,int stride,int count,PackType pack_type);
void geoUnpackDeltas(PackData *pack,void *data,int stride,int count,int type,char *modelname,char *filename);
void geoUnpack(PackData *pack,void *data,char *modelname,char *filename);
#ifndef GETVRMLxx
void modelFreeCtris(Model *model);
int modelCreateCtris(Model *model);
void modelSetCtriFlags(Model *model,int base,int count,U32 set_flag,U32 clear_flag);
#endif
void initBackgroundLoader(void);
// void backgroundLoaderSetThreadPriority(U32 priority);
void geoCheckThreadLoader(void);
void forceGeoLoaderToComplete(void);
void geoSetExistenceErrorReporting(int report_errors);
GeoLoadData * geoLoad(const char * name_old, GeoLoadType load_type, GeoUseType use_type);
void geoLoadData(GeoLoadData* gld);
void modelFreeCache(Model *model);
void modelFreeAllGeoData(void);
void modelListFree(GeoLoadData *gld);
void modelFreeAllCache(GeoUseType unuse_type);
void modelResetAllFlags(bool freeModels);
void modelInitTrickNodeFromName(char *name,Model *model);
void modelPrintFileInfo(char *fileName,GeoUseType use_type);
void geoLoadResetNonExistent(void);
PolyCell *polyCellUnpack(Model* model,PolyCell *cell,void *base_offset);
PolyCell *polyCellPack(Model* model, PolyCell *cell, void *base_offset, PolyCell* dest, void* dest_base_offset);
void geoSortModels(GeoLoadData* gld);

#if CLIENT
void modelDisplayOneTri(Model * model, Mat4 node, int idx);
Model *modelCopyForModification(const Model *basemodel, int tex_count);
void modelRebuildTextures(void);
void geoAddWeldedModel(Model *weldedModel);
void freeWeldedModels(void);
void modelFreeAllLODs(int reset_lodinfos);
void modelFreeLODs(Model *model, int reset_lodinfos);
Model *modelFindLOD(Model *srcmodel, F32 error, ReductionMethod method, F32 shrink_amount);
void modelDoneMakingLODs(Model *model);
#endif

static INLINEDBG void modelGetTris(U32 *tris, Model *model)
{
#if CLIENT
    if (model->unpack.tris)
        memcpy(tris,model->unpack.tris,model->tri_count*3*sizeof(U32));
    else
#endif
        geoUnpackDeltas(&model->pack.tris,tris,3,model->tri_count,PACK_U32,model->name,model->filename);
}

static INLINEDBG void modelGetVerts(Vec3 *verts, Model *model)
{
#if CLIENT
    if (model->unpack.verts)
        memcpy(verts,model->unpack.verts,model->vert_count*3*sizeof(F32));
    else
#endif
        geoUnpackDeltas(&model->pack.verts,verts,3,model->vert_count,PACK_F32,model->name,model->filename);
}

static INLINEDBG void modelGetNorms(Vec3 *norms, Model *model)
{
#if CLIENT
    if (model->unpack.norms)
        memcpy(norms,model->unpack.norms,model->vert_count*3*sizeof(F32));
    else
#endif
        geoUnpackDeltas(&model->pack.norms,norms,3,model->vert_count,PACK_F32,model->name,model->filename);
}

static INLINEDBG void modelGetSts(Vec2 *sts, Model *model)
{
#if CLIENT
    if (model->unpack.sts)
        memcpy(sts,model->unpack.sts,model->vert_count*2*sizeof(F32));
    else
#endif
        geoUnpackDeltas(&model->pack.sts,sts,2,model->vert_count,PACK_F32,model->name,model->filename);
}

static INLINEDBG void modelGetSts3(Vec2 *sts3, Model *model)
{
#if CLIENT
    if (model->unpack.sts3)
        memcpy(sts3,model->unpack.sts3,model->vert_count*2*sizeof(F32));
    else
#endif
        geoUnpackDeltas(&model->pack.sts3,sts3,2,model->vert_count,PACK_F32,model->name,model->filename);
}

static INLINEDBG void modelGetMatidxs(U8 *matidxs, Model *model)
{
    geoUnpack(&model->pack.matidxs,matidxs,model->name,model->filename);
}

static INLINEDBG void modelGetWeights(U8 *weights, Model *model)
{
    geoUnpack(&model->pack.weights,weights,model->name,model->filename);
}

#if CLIENT
    static INLINEDBG int modelHasTris(Model *model) { return model->pack.tris.unpacksize || model->unpack.tris; }
    static INLINEDBG int modelHasVerts(Model *model) { return model->pack.verts.unpacksize || model->unpack.verts; }
    static INLINEDBG int modelHasNorms(Model *model) { return model->pack.norms.unpacksize || model->unpack.norms; }
    static INLINEDBG int modelHasSts(Model *model) { return model->pack.sts.unpacksize || model->unpack.sts; }
    static INLINEDBG int modelHasSts3(Model *model) { return model->pack.sts3.unpacksize || model->unpack.sts3; }
#else
    static INLINEDBG int modelHasTris(Model *model) { return !!model->pack.tris.unpacksize; }
    static INLINEDBG int modelHasVerts(Model *model) { return !!model->pack.verts.unpacksize; }
    static INLINEDBG int modelHasNorms(Model *model) { return !!model->pack.norms.unpacksize; }
    static INLINEDBG int modelHasSts(Model *model) { return !!model->pack.sts.unpacksize; }
    static INLINEDBG int modelHasSts3(Model *model) { return !!model->pack.sts3.unpacksize; }
#endif

static INLINEDBG int modelHasMatidxs(Model *model) { return !!model->pack.matidxs.unpacksize; }
static INLINEDBG int modelHasWeights(Model *model) { return !!model->pack.weights.unpacksize; }

// End mkproto
#endif
