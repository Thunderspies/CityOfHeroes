#ifndef _AUTOLOD_H_
#define _AUTOLOD_H_

#include <utilitieslib/utils/textparser_types.h>
#include <utilitieslib/stdtypes.h>


typedef struct GMeshReductions GMeshReductions;


extern TokenizerParseInfo parse_lodinfo[];
extern TokenizerParseInfo parse_auto_lod[];


enum
{
    //    LOD_IGNORE_SEAMS = 1 << 0,
    LOD_ERROR_TRICOUNT = 1 << 1,
    //    LOD_ERROR_CONTRACTIONS = 1 << 2,
    //    LOD_PLACEMENT_ENDPOINTS = 1 << 3,
    //    LOD_GREY_TEX = 1 << 4,
    //    LOD_IGNORE_NORMAL_SEAMS = 1 << 5,
    LOD_LEGACY = 1 << 6,
    LOD_USEFALLBACKMATERIAL = 1 << 7,
    // ADD ONLY AFTER HERE
};

typedef struct AutoLOD
{
    float        max_error;
    F32            lod_near, lod_far, lod_nearfade, lod_farfade;
    int            flags;
    U32            modelname_specified : 1;
    char        *lod_modelname, *lod_filename;
} AutoLOD;

typedef struct ModelLODInfo
{
    union
    {
        struct 
        {
            U32 is_automatic : 1;
            U32 is_from_trick : 1;
            U32 is_from_default : 1;
            U32 is_no_lod : 1;
        } bits;

        U32 has_bits;
    };
    bool force_auto;
    bool removed;
    char *modelname;
    char *parsed_filename; // which file this is parsed from
    AutoLOD **lods;
} ModelLODInfo;


typedef void (*LodReloadCallback)(const char *path);

void lodinfoLoad(void);
void checkLODInfoReload(void);
void lodinfoLoadPostProcess(void);
void lodinfoReloadPostProcess(void);
void lodinfoClearLODsFromTricks(void);
int getNumLODReloads(void);
void lodinfoSetReloadCallback(LodReloadCallback callback);

void writeLODInfo(ModelLODInfo *info, char *geoname);

int getAutoLodNum(char *mname);
char *getLODFileName(const char *geo_fname);
char *getAutoLodModelName(char *modelname, int lod_num);
AutoLOD *allocAutoLOD(void);
AutoLOD *dupAutoLOD(AutoLOD *lod);
void freeModelLODInfoData(ModelLODInfo *info);

ModelLODInfo *lodinfoFromObjectName(ModelLODInfo *default_lod_info, char *modelname, char *geoname, float maxrad, int is_legacy, int src_tri_count, F32 lod_dists[3]);

void lodinfoFillInAutoData(ModelLODInfo *lod_info, char *modelname, char *geoname, float maxrad, int src_tri_count, F32 lod_dists[3]);


#endif //_AUTOLOD_H_

