#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/FolderCache.h>

#include "seq/AutoLOD.h"
#include "seq/anim.h"
#include "seq/tricks.h"
#include "group/groupfileload.h"

#if CLIENT
#include "clientError.h"
#include "group/groupfilelib.h"
#endif


typedef struct LODInfos
{
    ModelLODInfo **infos;
} LODInfos;


static StaticDefineInt autolod_flags[] = {
    DEFINE_INT
    { "ErrorTriCount",        LOD_ERROR_TRICOUNT },
    { "UseFallbackMaterial",LOD_USEFALLBACKMATERIAL },
    DEFINE_END
};

TokenizerParseInfo parse_auto_lod[] = {
    { "AllowedError",    TOK_F32(AutoLOD,max_error, 0)            },
    { "LodNearFade",    TOK_F32(AutoLOD,lod_nearfade, 0)        },
    { "LodNear",        TOK_F32(AutoLOD,lod_near, 0)            },
    { "LodFar",            TOK_F32(AutoLOD,lod_far, 0)            },
    { "LodFarFade",        TOK_F32(AutoLOD,lod_farfade, 0)        },
    { "LodFlags",        TOK_FLAGS(AutoLOD,flags,0),autolod_flags   },
    { "ModelName",        TOK_STRING(AutoLOD,lod_modelname, 0)        },
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

TokenizerParseInfo parse_model_lodinfo[] = {
    { "",                TOK_STRUCTPARAM | TOK_STRING(ModelLODInfo,modelname, 0)},
    { "Filename",        TOK_CURRENTFILE(ModelLODInfo,parsed_filename) },
    { "AutoLOD",        TOK_STRUCT(ModelLODInfo,lods,parse_auto_lod) },
    { "ForceAutomatic",    TOK_BOOL(ModelLODInfo,force_auto, 0) },
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

TokenizerParseInfo parse_lods[] = {
    { "LODInfo",        TOK_STRUCT(LODInfos,infos,parse_model_lodinfo) },
    { "", 0, 0 }
};

AutoLOD *allocAutoLOD(void)
{
    return ParserAllocStruct(sizeof(AutoLOD));
}

AutoLOD *dupAutoLOD(AutoLOD *lod)
{
    AutoLOD *new_lod = allocAutoLOD();
    CopyStructs(new_lod, lod, 1);
    new_lod->lod_modelname = lod->lod_modelname?ParserAllocString(lod->lod_modelname):0;
    new_lod->lod_filename = lod->lod_filename?ParserAllocString(lod->lod_filename):0;
    return new_lod;
}

void freeAutoLOD(void* lodData)
{
    AutoLOD * lod = (AutoLOD *)lodData;
    if (lod->lod_modelname)
        ParserFreeString(lod->lod_modelname);
    lod->lod_modelname = 0;

    if (lod->lod_filename)
        ParserFreeString(lod->lod_filename);
    lod->lod_filename = 0;

    ParserFreeStruct(lod);
}

void freeModelLODInfoData(ModelLODInfo *info)
{
    if (!info)
        return;
    if (info->modelname)
        ParserFreeString(info->modelname);
    if (info->parsed_filename)
        ParserFreeString(info->parsed_filename);
    eaDestroyEx(&info->lods, freeAutoLOD);
    ZeroStruct(info);
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static LODInfos all_lodinfos={0};
static StashTable lod_hash=0;
static char **lodinfo_reloads=0;

void lodinfoClearLODsFromTricks(void)
{
    char buf[MAX_PATH], *s;
    int i;
    
    for (i = 0; i < eaSize(&all_lodinfos.infos); i++)
    {
        ModelLODInfo *lod_info = all_lodinfos.infos[i];
        if (lod_info->bits.is_from_trick || (lod_info->bits.is_automatic && !lod_info->force_auto))
        {
            eaRemove(&all_lodinfos.infos, i);
            i--;

            assert(fileLocateWrite(lod_info->parsed_filename, buf));
            ParserFreeString(lod_info->parsed_filename);
            lod_info->parsed_filename = ParserAllocString(buf);
            sprintf(buf, "%s/%s", lod_info->parsed_filename, lod_info->modelname);
            s = strstri(buf, "__");
            if (s)
                *s = 0;

            assert(stashRemovePointer(lod_hash, buf, NULL));

            freeModelLODInfoData(lod_info);
            ParserFreeStruct(lod_info);
        }
    }
}

static void reloadLODInfoProcessor(const char *relpath, int when)
{
    eaPush(&lodinfo_reloads, strdup(relpath));
}

static LodReloadCallback lodReloadCallback = 0;

void lodinfoSetReloadCallback(LodReloadCallback callback)
{
    lodReloadCallback = callback;
}

static void fillInLODFilenames(ModelLODInfo *lodinfo)
{
#if CLIENT
    int i;
    for (i = 0; i < eaSize(&lodinfo->lods); i++)
    {
        AutoLOD *lod = lodinfo->lods[i];
        lod->modelname_specified = !!lod->lod_modelname;
        if (!lod->lod_filename && lod->lod_modelname)
        {
            lod->lod_filename = objectLibraryPathFromObj(lod->lod_modelname);
            if (lod->lod_filename)
                lod->lod_filename = ParserAllocString(lod->lod_filename);
        }
    }
#endif
}

static void reloadLODInfo(const char *relpath)
{
    char fullpath[MAX_PATH], buf[MAX_PATH], *s;
    LODInfos temp_infos={0};
    int i;

    sprintf(fullpath, "%s/%s", fileDataDir(), relpath);

    fileWaitForExclusiveAccess(fullpath);
    //errorLogFileIsBeingReloaded(relpath);

    PERFINFO_AUTO_START("reloadLODInfo",1);

    for (i = 0; i < eaSize(&all_lodinfos.infos); i++)
    {
        ModelLODInfo *lodinfo = all_lodinfos.infos[i];
        if ((!lodinfo->has_bits || (lodinfo->bits.is_automatic && lodinfo->force_auto)) && stricmp(fullpath, lodinfo->parsed_filename)==0)
        {
            lodinfo->removed = 1;
        }
    }

    if (!ParserLoadFiles(NULL, (char *)relpath, NULL, PARSER_OPTIONALFLAG|PARSER_EXACTFILE|PARSER_EMPTYOK, parse_lods, &temp_infos, 0, NULL, NULL))
    {
        ErrorFilenamef(relpath, "Error reloading LOD info file: %s\n", relpath);
#if CLIENT
        status_printf("Error reloading LOD info file: %s\n.", relpath);
#endif
    }
    else
    {
        for (i = 0; i < eaSize(&temp_infos.infos); i++)
        {
            ModelLODInfo *old_info, *lodinfo = temp_infos.infos[i];
            assert(fileLocateWrite(lodinfo->parsed_filename, buf));
            ParserFreeString(lodinfo->parsed_filename);
            lodinfo->parsed_filename = ParserAllocString(buf);
            s = strstri(lodinfo->modelname, "__");
            if (s)
                *s = 0;
            sprintf(buf, "%s/%s", lodinfo->parsed_filename, lodinfo->modelname);
            fillInLODFilenames(lodinfo);
            if (stashFindPointer(lod_hash, buf, &old_info))
            {
                eaDestroyEx(&old_info->lods, freeAutoLOD);
                if (lodinfo->lods)
                {
                    eaCopy(&old_info->lods, &lodinfo->lods);
                    eaDestroy(&lodinfo->lods);
                }
                old_info->removed = 0;
                old_info->has_bits = lodinfo->has_bits;
                old_info->force_auto = lodinfo->force_auto;
                freeModelLODInfoData(lodinfo);
            }
            else
            {
                lodinfo->removed = 0;
                stashAddPointer(lod_hash, buf, lodinfo, false);
                eaPush(&all_lodinfos.infos, lodinfo);
            }
        }

        eaDestroy(&temp_infos.infos);

        if (lodReloadCallback)
            lodReloadCallback(relpath);
    }

    PERFINFO_AUTO_STOP();
}

static int num_lod_reloads = 0;
void checkLODInfoReload(void)
{
    int need_reload=0;
    char *s;
    while (s = eaPop(&lodinfo_reloads)) {
        if (!eaSize(&lodinfo_reloads) || stricmp(s, lodinfo_reloads[eaSize(&lodinfo_reloads)-1])!=0) {
            // Not the same as the next one
            reloadLODInfo(s);
            need_reload = 1;
        }
        free(s);
    }

    if (need_reload)
    {
        num_lod_reloads++;
        lodinfoReloadPostProcess();
#if CLIENT
        status_printf("LOD infos reloaded.");
#endif
    }
}

int getNumLODReloads(void)
{
    return num_lod_reloads;
}

void lodinfoLoad(void)
{
    if (!ParserLoadFiles("lods", ".txt", "lods.bin", PARSER_OPTIONALFLAG|PARSER_EMPTYOK, parse_lods, &all_lodinfos, 0, NULL, NULL))
    {
        // nothing to do here
    }

    lodinfoLoadPostProcess();

    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "lods/*.txt", reloadLODInfoProcessor);
}

void lodinfoLoadPostProcess(void)
{
    int i;
    char buf[MAX_PATH], *s;

    lod_hash = stashTableCreateWithStringKeys(4096,StashDeepCopyKeys);
    
    for (i = 0; i < eaSize(&all_lodinfos.infos); i++)
    {
        ModelLODInfo *lodinfo = all_lodinfos.infos[i];
        fillInLODFilenames(lodinfo);
        assert(fileLocateWrite(lodinfo->parsed_filename, buf));
        ParserFreeString(lodinfo->parsed_filename);
        lodinfo->parsed_filename = ParserAllocString(buf);
        s = strstri(lodinfo->modelname, "__");
        if (s)
            *s = 0;
        snprintf(buf, sizeof(buf), "%s/%s", lodinfo->parsed_filename, lodinfo->modelname);
        if (!stashAddPointer(lod_hash, buf, lodinfo, false))
        {
            char author1[1000],author2[1000];
            ModelLODInfo *dup_lodinfo = 0;

            stashFindPointer(lod_hash, buf, &dup_lodinfo );
            Errorf("Duplicate LOD info: %s\n1st %s (checked in by: %s)\n2nd %s (checked in by: %s)\n\n",
                lodinfo->modelname, lodinfo->parsed_filename, author1, dup_lodinfo->parsed_filename, author2);
            errorLogFileHasError(lodinfo->parsed_filename);
            errorLogFileHasError(dup_lodinfo->parsed_filename);
        }
    }
}

void lodinfoReloadPostProcess(void)
{
#if CLIENT | SERVER
    PERFINFO_AUTO_START("groupResetAllTrickFlags",1);
        groupResetAll(RESET_TRICKS|RESET_LODINFOS);
    PERFINFO_AUTO_STOP();
#endif
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


int getAutoLodNum(char *mname)
{
    char *s, *p;
    int lodNum = 0;

    s = strstr(mname,"__");

    if (s)
        *s = 0;

    p = strrchr(mname, '_');

    if (p && strncmp(p+1, "LOD", 3)==0)
        lodNum = atoi(p+4);

    if (s)
        *s = '_';

    return lodNum;
}

char *getAutoLodModelName(char *modelname, int lod_num)
{
    static char buf[MAX_PATH];
    char *s;

    if (!modelname || !modelname[0])
        return 0;

    if (lod_num == 0)
        return modelname;

    s = strstr(modelname,"__");

    if (s)
        *s = 0;

    if (modelname[0] == '_')
        sprintf(buf, "%s_LOD%d%s%s", modelname, lod_num, s?"__":"", s?s+1:"");
    else
        sprintf(buf, "_%s_LOD%d%s%s", modelname, lod_num, s?"__":"", s?s+1:"");

    if (s)
        *s = '_';

    return buf;
}

char *getLODFileName(const char *geo_fname)
{
    static char dirbuf[MAX_PATH], *s;

    s = strstri((char *)geo_fname, "/data/");
    if (s)
        s += 6;
    else
        s = (char *)geo_fname;
    strcpy(dirbuf, "lods/");
    strcat(dirbuf, s);
    s = strstri(dirbuf, ".geo");
    assert(s);
    *s = 0;
    strcat(dirbuf, ".txt");

    return dirbuf;
}

#define HIGH_POLY_CUTOFF2 2000
#define LOW_LOD_CUTOFF 200
#define DIST_OFFSET 125

void lodinfoFillInAutoData(ModelLODInfo *lod_info, char *modelname, char *geoname, float maxrad, int src_tri_count, F32 lod_dists[3])
{
    int d = 0;
    int tri_count = src_tri_count;
    F32 tricount_error = 0.75f;
    int lod_fromtrick = 0;
    F32 lod_far = 0, lod_farfade = 0, lod_near = 0, lod_nearfade = 0, shadow_dist = 0;
    TrickInfo *trick = 0;

    assert(lod_info);
    assert(!eaSize(&lod_info->lods));

    // get LOD info from trick
    trickSetExistenceErrorReporting(0);
    trick = trickFromObjectName(modelname, "");
    trickSetExistenceErrorReporting(1);
    if (trick)
    {
        shadow_dist = trick->shadow_dist;
        lod_near = trick->lod_near;
        lod_nearfade = trick->lod_nearfade;
        lod_far = trick->lod_far;
        lod_farfade = trick->lod_farfade;
    }

    lod_fromtrick = lod_near || lod_far || lod_nearfade || lod_farfade;

    if (!lod_far)
        lod_far = 10.f * (maxrad  * 0.5f + shadow_dist + 10.f);

    if (!lod_farfade && !lod_fromtrick)
        lod_farfade = lod_far * 0.25f;

    // automatic LOD values
    lod_info->bits.is_automatic = 1;

    eaPush(&lod_info->lods, allocAutoLOD());
    lod_info->lods[0]->flags = LOD_ERROR_TRICOUNT;
    lod_info->lods[0]->max_error = 0;
    lod_info->lods[0]->lod_near = lod_near;
    lod_info->lods[0]->lod_nearfade = lod_nearfade;
    lod_info->lods[0]->lod_far = lod_far;
    lod_info->lods[0]->lod_farfade = lod_farfade;

    while (lod_dists && tricount_error > 0 && tri_count > LOW_LOD_CUTOFF)
    {
        F32 err_dist = lod_dists[d];
        AutoLOD *prev_lod = lod_info->lods[eaSize(&lod_info->lods)-1];

        tri_count = tricount_error * src_tri_count;

        // check if the reduction is worth it
        if (err_dist > (prev_lod->lod_far - DIST_OFFSET) || err_dist < 0)
            break;

        if (err_dist > (prev_lod->lod_near + DIST_OFFSET) && (tricount_error <= 0.5f || tri_count <= HIGH_POLY_CUTOFF2))
        {
            AutoLOD *lod = allocAutoLOD();
            eaPush(&lod_info->lods, lod);

            lod->flags = LOD_ERROR_TRICOUNT | LOD_USEFALLBACKMATERIAL;
            lod->max_error = 100 * (1 - tricount_error);

            lod->lod_far = prev_lod->lod_far;
            lod->lod_farfade = prev_lod->lod_farfade;
            prev_lod->lod_far = lod->lod_near = err_dist;

            if (maxrad > 200)
            {
                prev_lod->lod_farfade = 25;
                lod->lod_nearfade = 25;
            }
            else if (d == 0)
            {
                prev_lod->lod_farfade = 5;
                lod->lod_nearfade = 5;
            }
            else
            {
                prev_lod->lod_farfade = 0;
                lod->lod_nearfade = 0;
            }
        }

        tricount_error -= 0.25f;
        d++;
    }
}

ModelLODInfo *lodinfoFromObjectName(ModelLODInfo *default_lod_info, char *modelname, char *geoname, float maxrad, int is_legacy, int src_tri_count, F32 lod_dists[3])
{
    TrickInfo *trick = 0;
    int lod_fromtrick = 0;
    int is_tray = 0, automatic_ok = 0;
    intptr_t no_lod = is_legacy;
    ModelLODInfo *lod_info = 0;
    F32 lod_far = 0, lod_farfade = 0, lod_near = 0, lod_nearfade = 0, shadow_dist = 0;
    char buf[1024], *s;
    char outputname[MAX_PATH];

    assert(fileLocateWrite(getLODFileName(geoname), outputname));
    sprintf(buf, "%s/%s", outputname, modelname);
    s = strstri(buf, "__");
    if (s)
        *s = 0;

    if (stashFindPointer(lod_hash, buf, &lod_info) && !lod_info->removed)
    {
        if (!(lod_info->force_auto && !lod_info->lods))
            return lod_info;
    }

    if (lod_info && lod_info->removed)
    {
        freeModelLODInfoData(lod_info);
        lod_info->modelname = ParserAllocString(modelname);
        lod_info->parsed_filename = ParserAllocString(outputname);
    }
    else if (!lod_info)
    {
        lod_info = ParserAllocStruct(sizeof(*lod_info));
        lod_info->modelname = ParserAllocString(modelname);
        lod_info->parsed_filename = ParserAllocString(outputname);
    }

    // get LOD info from trick
    trickSetExistenceErrorReporting(0);
    trick = trickFromObjectName(modelname, "");
    trickSetExistenceErrorReporting(1);
    if (trick)
    {
        shadow_dist = trick->shadow_dist;
        lod_near = trick->lod_near;
        lod_nearfade = trick->lod_nearfade;
        lod_far = trick->lod_far;
        lod_farfade = trick->lod_farfade;

        is_tray = !!(trick->group_flags & (GROUP_VIS_TRAY | GROUP_VIS_OUTSIDE));
    }

    if (default_lod_info && default_lod_info->lods)
    {
        int num = eaSize(&default_lod_info->lods);

        lod_near = default_lod_info->lods[0]->lod_near;
        lod_nearfade = default_lod_info->lods[0]->lod_nearfade;
        lod_far = default_lod_info->lods[num-1]->lod_far;
        lod_farfade = default_lod_info->lods[num-1]->lod_farfade;
    }

    lod_fromtrick = lod_near || lod_far || lod_nearfade || lod_farfade;

    if (!lod_far)
        lod_far = 10.f * (maxrad  * 0.5f + shadow_dist + 10.f);

    if (!lod_farfade && !lod_fromtrick)
        lod_farfade = lod_far * 0.25f;

    // capes can't be LODed
    if (!no_lod)
    {
        char basename[MAX_PATH];
        char * bs, * fs, * s;

        fs = strrchr(geoname, '/'); 
        bs = strrchr(geoname, '\\');
        s = fs > bs ? fs : bs;
        if(s)
            strcpy(basename, s + 1);
        else
            strcpy(basename, geoname);
        s = strrchr(basename,'.'); 
        if (s)
            *s = 0;
        no_lod = (intptr_t)strstri(basename, "_cape");
    }

    automatic_ok = !no_lod && !is_tray && lod_near==0 && !lod_fromtrick;

    if (!lod_info->force_auto && !no_lod && trick && trick->auto_lod)
    {
        int done_far = 0;
        int i, num = eaSize(&trick->auto_lod);

        lod_info->bits.is_from_trick = 1;

        for (i = 0; i < num; i++)
        {
            eaPush(&lod_info->lods, dupAutoLOD(trick->auto_lod[i]));
            if (!done_far)
            {
                if (!lod_info->lods[i]->lod_far)
                {
                    lod_info->lods[i]->lod_far = lod_far;
                    done_far = 1;
                }
                if (!lod_info->lods[i]->lod_farfade)
                {
                    lod_info->lods[i]->lod_farfade = lod_farfade;
                    done_far = 1;
                }
            }
        }
    }
    else if (!lod_info->force_auto && !no_lod && default_lod_info && default_lod_info->lods)
    {
        int i, num = eaSize(&default_lod_info->lods);

        lod_info->bits.is_from_default = 1;

        for (i = 0; i < num; i++)
            eaPush(&lod_info->lods, dupAutoLOD(default_lod_info->lods[i]));

        lod_info->lods[0]->lod_near = (lod_near<default_lod_info->lods[0]->lod_far)?lod_near:default_lod_info->lods[0]->lod_near;
        lod_info->lods[0]->lod_nearfade = lod_nearfade;
        lod_info->lods[num-1]->lod_far = lod_far;
        lod_info->lods[num-1]->lod_farfade = lod_farfade;
    }
    else if (automatic_ok || lod_info->force_auto)
    {
        lodinfoFillInAutoData(lod_info, modelname, geoname, maxrad, src_tri_count, lod_dists);
    }
    else
    {
        lod_info->bits.is_no_lod = 1;
        eaPush(&lod_info->lods, allocAutoLOD());
        lod_info->lods[0]->flags = is_legacy?LOD_LEGACY:0;
        lod_info->lods[0]->max_error = 0;
        lod_info->lods[0]->lod_near = lod_near;
        lod_info->lods[0]->lod_nearfade = lod_nearfade;
        lod_info->lods[0]->lod_far = lod_far;
        lod_info->lods[0]->lod_farfade = lod_farfade;
    }

    if (!lod_info->force_auto)
    {
        stashAddPointer(lod_hash, buf, lod_info, false);
        eaPush(&all_lodinfos.infos, lod_info);
    }

    return lod_info;
}

void writeLODInfo(ModelLODInfo *info, char *geoname)
{
    char outputname[MAX_PATH];
    char *s;
    int i;

    assertmsg(info->modelname, "Invalid LOD info passed to writeLODInfo.");

    s = strstri(info->modelname, "__");
    if (s)
        *s = 0;

    if (fileLocateWrite(getLODFileName(geoname), outputname))
    {
        int info_pushed=0;
        LODInfos infos={0};
        mEArrayHandle lods = 0;
        info->parsed_filename = ParserAllocString(outputname);
        for (i = 0; i < eaSize(&all_lodinfos.infos); i++)
        {
            ModelLODInfo *lodinfo = all_lodinfos.infos[i];
            if ((!lodinfo->has_bits || (lodinfo->bits.is_automatic && lodinfo->force_auto)) && !lodinfo->removed && stricmp(info->parsed_filename, lodinfo->parsed_filename)==0)
            {
                if (stricmp(info->modelname, lodinfo->modelname)!=0)
                {
                    eaPush(&infos.infos, lodinfo);
                    eaPush(&lods, lodinfo->lods);
                    if (lodinfo->force_auto)
                        lodinfo->lods = 0;
                }
                else if (!info_pushed)
                {
                    eaPush(&infos.infos, info);
                    eaPush(&lods, info->lods);
                    if (info->force_auto)
                        info->lods = 0;
                    info_pushed = 1;
                }
            }
        }
        if (!info_pushed)
        {
            eaPush(&infos.infos, info);
            eaPush(&lods, info->lods);
            if (info->force_auto)
                info->lods = 0;
        }

        // write the lod file and add to source control if necessary
        mkdirtree(info->parsed_filename);
        ParserWriteTextFile(info->parsed_filename, parse_lods, &infos, 0, 0);

        for (i = 0; i < eaSize(&infos.infos); i++)
            infos.infos[i]->lods = lods[i];
        eaDestroy(&infos.infos);
        eaDestroy(&lods);
    }
    else
    {
        Errorf("Unable to write LOD info for geo: %s", geoname);
    }
}



