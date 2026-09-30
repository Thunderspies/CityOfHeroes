/***************************************************************************
*     Copyright (c) 2005-2006, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
*/

#include "seq/modelReload.h"
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FileSystem.h>
#include "seq/anim.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/memlog.h>
#include <utilitieslib/utils/error.h>
#include "gridcoll/gridcache.h"
#include "seq/tricks.h"
#include "group/groupfileload.h"
#include "group/groupfilelib.h"
#include <fcntl.h>
#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/components/SharedHeap.h>
#include <utilitieslib/components/StashTable.h>
#include "seq/AutoLOD.h"

#if CLIENT
#include "seq/gfxtree.h"
#include "graphics/seqgraphics.h"
#include "graphics/groupdraw.h"
#include "graphics/FX/fxgeo.h"
#include "clientError.h"
#endif

#ifdef SERVER 
#include "cmdparse/cmdserver.h"
#endif

MemLog modelReloadLog = {0}; 

static bool modelReload_inited=false;
static bool modelReload_needsReload=false;
static int modelReload_numReloads=0;

typedef enum ModelReloadType {
    MRT_OBJECTLIBRARY,
    MRT_PLAYERLIBRARY,
    MRT_PLAYERANIM,
} ModelReloadType;

typedef struct ModelReloadRequest {
    char file[MAX_PATH];
    ModelReloadType type;
} ModelReloadRequest;

static ModelReloadRequest **modelReload_requests=NULL;

static bool g_trick_changed=false, g_reloaded=false;

int getNumModelReloads(void)
{
    return modelReload_numReloads;
}

static void addRequest(const char *file, ModelReloadType type)
{
    ModelReloadRequest *req = calloc(sizeof(ModelReloadRequest), 1);
    Strncpyt(req->file, file);
    req->type = type;
    eaPush(&modelReload_requests, req);
    modelReload_needsReload = 1;
}

static void __cdecl swap (void *a, void *b, size_t width)
{
    char tmp;
    char *ca=a, *cb=b;
    while ( width-- ) {
        tmp = *ca;
        *ca++ = *cb;
        *cb++ = tmp;
    }
}

// Needs to copy all data, copy all pointers to static data, swap pointers to dynamic data (so it gets freed)
static void reloadModel(Model *dstModel, Model *srcModel)
{
    swap(dstModel, srcModel, sizeof(Model));
}

static void verifyGeoLoadData(GeoLoadData *gld)
{
    int i;

    assert(gld->modelheader.model_count == eaSize(&gld->modelheader.models));
    for (i=0; i<gld->modelheader.model_count; i++) {
        Model *model = gld->modelheader.models[i];
        assert(model);
        assert(model->name);
        assert(strlen(model->name)<255);
        assert(model->gld == gld);
    }
}

static bool objectNamesMatch(const char *name1, const char *name2)
{
    for (; *name1&&*name2&&*name1==*name2; name1++, name2++);
    if (*name1==*name2)
        return true;
    if (!*name1 && name2[0]=='_' && name2[1]=='_') {
        g_trick_changed = true;
        return true;
    }
    if (!*name2 && name1[0]=='_' && name1[1]=='_') {
        g_trick_changed = true;
        return true;
    }
    return false;
}

void reloadGeoLoadData(GeoLoadData *gld)
{
    bool orphaned = false;
    int i, j;
    GeoLoadData *gld_new;
    bool removeAll = false;
    ModelHeader *header, *header_new;
    Model *temp;

    gld_new = geoLoad(gld->name, LOAD_RELOAD, gld->type);

    if (!gld_new)
        removeAll = true;

    header = &gld->modelheader;
    header_new = removeAll?NULL:&gld_new->modelheader;
    memlog_printf(&modelReloadLog, "Reloading GeoLoadData %s/%s", gld->name, header->name);
    assert(removeAll || stricmp(header->name, header_new->name)==0);

    // Go through header, update ones found in header_new, delete ones not found in header_new
    for( i=header->model_count-1; i >=0; i-- )
    {
        bool foundit=false;
        Model *model = header->models[i];

        modelFreeCtris(model); // Will be auto-generated

#if CLIENT
        assert(!model->lod_models);
#endif

        // Look for model in header_new
        for (j=0; !removeAll && j<header_new->model_count && !foundit; j++) {
            Model *model_new = header_new->models[j];
            if (objectNamesMatch(model->name, model_new->name)) {
                foundit=true;
                memlog_printf(&modelReloadLog, "  Model %s found (old: %p new: %p)", model->name, model, model_new);
                reloadModel(model, model_new);
            }
        }
        if (!foundit) {
            // Remove it! (but don't free it, as things might still be pointing to it)
            //model->gld = NULL; // Can't do this, things may need to dereference this

#if CLIENT    // not as clean as i'd like... -GG
            if(gld->lod_infos)
                freeModelLODInfoData(&gld->lod_infos[i]);
            gld->modelheader.models[i]->lod_info = 0;
#endif
            eaRemove(&header->models, i);
            header->model_count--;
            orphaned = true;
            memlog_printf(&modelReloadLog, "  Model %s NOT found (old: %p)", model->name, model);
            printf("    Model %s has been deleted (leaking some data)\n", model->name);
        }
        assert(header->model_count == eaSize(&header->models));
    }

    // Go through header_new, add ones not found in header
    
    if (!removeAll)
    {
        for (i=header_new->model_count-1; i>=0; i--)
        {
            Model *model_new = header_new->models[i];
            bool foundit=false;


            for (j=0; j<header->model_count && !foundit; j++) 
            {
                Model *model = header->models[j];
                if (objectNamesMatch(model_new->name, model->name))
                    foundit=true;
            }
            if (!foundit) {
                // Add it
                Model *modelCopy = calloc(sizeof(Model),1);
                reloadModel(modelCopy, model_new);
                eaPush(&header->models, modelCopy);
                header->model_count++;
                memlog_printf(&modelReloadLog, "  Model %s is NEW (new: %p loaded: %p)", modelCopy->name, modelCopy, model_new);
                printf("    Model %s has been added\n", modelCopy->name);
            }
        }
        assert(header_new->model_count == header->model_count);
    } else {
        assert(0 == header->model_count);
    }
    assert(header->model_count == eaSize(&header->models));

    // At this point, we want to keep all of the pointers/structures in gld->headers[]->models,
    //  because they are what the rest of the code is pointing to, but we want to keep the
    //  anim_list_new base structure, because it has our texbinds, texnames, objnames, header data, etc

    // Free anim_list_new's models, it's all been copied over to the first loaded one
    // Then setup this guy so his models point at the first one
    // Also clear the first one's models so they don't get freed
    if (!removeAll)
    {
        for( j=0 ; j < gld_new->modelheader.model_count; j++ )
        {
            // Swap, so they get freed later
            temp = gld_new->modelheader.models[j];
            gld_new->modelheader.models[j] = gld->modelheader.models[j];
            gld->modelheader.models[j] = temp;
            // Patch up pointers
            gld_new->modelheader.models[j]->gld = gld_new;

        }
        temp = gld_new->modelheader.model_data;
        gld_new->modelheader.model_data = gld->modelheader.model_data;
        gld->modelheader.model_data = temp;
    }

    // Remove the old one from the hash table
    if (removeAll) {
        stashRemovePointer(glds_ht, gld->name, NULL);
    }
    if (!removeAll) {
        StashElement element;
        // Add the correct one
        if (stashFindElement(glds_ht, gld_new->name, &element)) {
            // Already in there, just update
            stashElementSetPointer(element, gld_new);
        } else {
            // New
            stashAddPointer(glds_ht, gld_new->name, gld_new, false);
        }
        geoSortModels(gld_new);
        verifyGeoLoadData(gld_new);
    }
    if(!orphaned)
    {
        modelListFree(gld);
    }
    else
    {
#if CLIENT
        for(i = 0; i < gld->modelheader.model_count; i++)
        {
            if(gld->lod_infos)
                freeModelLODInfoData(&gld->lod_infos[i]);
            gld->modelheader.models[i]->lod_info = 0;
        }
#endif
    }
}
#ifdef SERVER
#include "group/groupnetdb.h"
#endif
static int reloadGeoLoadDatas(char *relpath)
{
    StashElement element;
    StashTableIterator it;
    bool foundit=false;

    // Turn off shared memory, reloading breaks it
    sharedMemorySetMode(SMM_DISABLED);
    sharedHeapTurnOff("Anims reloading, thus turning off shared heap\n");

    geoLoadResetNonExistent();

    forwardSlashes(relpath);
    while (relpath[0]=='/') relpath++;

    if (strEndsWith(relpath, ".geolm"))
        relpath[strlen(relpath)-2] = 0;

    if (glds_ht) {
        stashGetIterator(glds_ht, &it);
        while(stashGetNextElement(&it, &element) && !foundit)
        {
            GeoLoadData*    gld;

            gld = stashElementGetPointer(element);
            if (stricmp(gld->name, relpath)==0) {
                foundit = true;
                reloadGeoLoadData(gld);
            }
        }
    }
    if (!foundit) {
        printf("  File was not reloaded because it did not appear to previously have been loaded\n");
        return 0;
    } else {
        return 1;
    }
}

static char **reloadGetRootnames(const char *fname)
{
    char    *mem,*str,*args[100];
    int        count;
    char **ret=NULL;

    str = mem = fileAlloc(fname,0);
    while(str)
    {
        count = tokenize_line(str,args,&str);
        if (count == 2 && stricmp(args[0],"Def")==0 && args[1][strlen(args[1])-1] != '&')
        {
            eaPush(&ret, strdup(args[1]));
        }
    }
    free(mem);
    return ret;
}

static void reloadObjectLibraryNames(const char *relpath)
{
    char filename[MAX_PATH];
    char **newnames;

    if (strstr(relpath, "/_"))
    {
        log_printf("getvrml.log","Left from reloadObjectLibraryNames early because %s contains \"/_\"",relpath);
        return;
    }
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);

    strcpy(filename, relpath);

    // Load list of names in .rootnames file
    newnames = reloadGetRootnames(relpath);
    // Search current list and update removals/additions
    objectLibraryProcessNameChanges(relpath, newnames);

    // Cleanup
    eaDestroyEx(&newnames, NULL);
}

static void reloadObjectLibrary(const char *relpath)
{
    char filename[MAX_PATH], *s;
    strcpy(filename, relpath);

    printf("  %s\n", relpath);
    g_trick_changed = false;
    g_reloaded = reloadGeoLoadDatas(filename) || g_reloaded;
    s = strrchr(filename, '.');
    strcpy(s, ".rootnames");
    log_printf("getvrml.log","Reloading file %s (%s)",filename,relpath);
    reloadObjectLibraryNames(filename);
}

static void reloadPlayerLibraryAnim(const char *relpath)
{
    char filename[MAX_PATH];
    strcpy(filename, relpath);

    // This is reloaded elsewhere
//    printf("detected player library anim change on %s (not reloaded)\n", relpath);
}

static void reloadPlayerLibraryGeo(const char *relpath)
{
    int reloaded;
    char filename[MAX_PATH];
    strcpy(filename, relpath);

    printf("  %s:\n", relpath);
    g_trick_changed=false;
    reloaded = reloadGeoLoadDatas(filename);
    if (g_trick_changed) {
#if CLIENT
        gfxTreeResetTrickFlags( gfx_tree_root );
        animCalculateSortFlags( gfx_tree_root );
#endif
    }
    if (reloaded) {
#if CLIENT
        fxGeoResetAllCapes();
#endif
    }
}

static void reloadPlayerLibraryGeoCallback(const char *relpath, int when)
{
    if (strstr(relpath, "/_")) return;
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    addRequest(relpath, MRT_PLAYERLIBRARY);
}

static void reloadPlayerLibraryGeoCallbackFileChanged(const FileChange *change, void *user)
{
	(void)user;
	reloadPlayerLibraryGeoCallback(change->entry.path, change->kind);
}
static void reloadPlayerLibraryAnimCallback(const char *relpath, int when)
{
    if (strstr(relpath, "/_")) return;
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    addRequest(relpath, MRT_PLAYERANIM);
}

static void reloadPlayerLibraryAnimCallbackFileChanged(const FileChange *change, void *user)
{
	(void)user;
	reloadPlayerLibraryAnimCallback(change->entry.path, change->kind);
}
static void reloadObjectLibraryCallback(const char *relpath, int when)
{
    if (strstr(relpath, "/_")) return;
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    addRequest(relpath, MRT_OBJECTLIBRARY);
}

static void reloadObjectLibraryCallbackFileChanged(const FileChange *change, void *user)
{
	(void)user;
	reloadObjectLibraryCallback(change->entry.path, change->kind);
}


void modelInitReload(void)
{
    assert(!modelReload_inited);
    modelReload_inited = true;

    // Add callback for re-loading objects
    fileSystemSubscribe(NULL, "object_library/*.geo", (FILE_CHANGE_UPDATE | FILE_CHANGE_DELETE), reloadObjectLibraryCallbackFileChanged, NULL);
#ifdef CLIENT
    // fpe removed, no longer used FolderCacheSetCallback((FILE_CHANGE_UPDATE | FILE_CHANGE_DELETE), "object_library/*.geolm",reloadObjectLibraryCallback);
#endif
    fileSystemSubscribe(NULL, "player_library/*.geo", (FILE_CHANGE_UPDATE | FILE_CHANGE_DELETE), reloadPlayerLibraryGeoCallbackFileChanged, NULL);
    fileSystemSubscribe(NULL, "player_library/*.anim", (FILE_CHANGE_UPDATE | FILE_CHANGE_DELETE), reloadPlayerLibraryAnimCallbackFileChanged, NULL);
}

static FILE* getvrml_lock_handle=NULL;
void releaseGetVrmlLock(void)
{
    log_printf("getvrml.log","releaseGetvrmlLock");
    fclose(getvrml_lock_handle);
    getvrml_lock_handle = NULL;
}

int tryToLockGetVrml(void)
{
    char *lockfilename = "c:\\getvrml.lock";
    log_printf("getvrml.log","waitForGetvrmlToFinish");
    fileForceRemove(lockfilename);
    if ((getvrml_lock_handle = fopen(lockfilename, "wl")) == NULL)
        return 0;
    assert(getvrml_lock_handle > 0);
    fprintf(getvrml_lock_handle, "CityOfHeroes.exe\r\n");
    return 1;
}

void waitForGetVrmlLock(void)
{
    while (!tryToLockGetVrml())
        Sleep(100);
}

int isGetVrmlRunning(void)
{
    DWORD result;
    HANDLE hMutex;
    hMutex = CreateMutexA(NULL, 0, "Global\\CrypticGetVrml");
    result = WaitForSingleObject(hMutex, 0);
    if (!(result == WAIT_OBJECT_0 || result == WAIT_ABANDONED))
    {
        // mutex locked
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
        return 1;
    }

    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
    return 0;
}

void checkModelReload(void)
{
    ModelReloadRequest *req;
    ModelReloadRequest last_req={0};
    if (!modelReload_needsReload || !modelReload_inited)
        return;

    loadstart_printf("Geometry reload initiated...\n");
    g_reloaded = false;

#if CLIENT
    freeWeldedModels();
    modelFreeAllLODs(0);
#endif

    waitForGetVrmlLock();

    // Dispatch requests
    while (req = eaRemove(&modelReload_requests, 0)) {
        if (memcmp(&last_req, req, sizeof(last_req))==0) {
            // Don't do duplicates!
            log_printf("getvrml.log","Don't do duplicates!");
        } else {
            switch(req->type) {
            xcase MRT_OBJECTLIBRARY:
                log_printf("getvrml.log","Reloading Object Library...");
                reloadObjectLibrary(req->file);
            xcase MRT_PLAYERLIBRARY:
                log_printf("getvrml.log","Reloading Player Library...");
                reloadPlayerLibraryGeo(req->file);
            xcase MRT_PLAYERANIM:
                log_printf("getvrml.log","Reloading Library Anim...");
                reloadPlayerLibraryAnim(req->file);
            }
            last_req = *req;
#if CLIENT
            status_printf("Model reloaded: %s\n", req->file);
#endif
        }
        free(req);
    }
    releaseGetVrmlLock();
#ifdef SERVER
    // do it for the whole map
    //forAllTopLevelDefsUnpackCtris();
    modelFreeAllGeoData();
#endif

#if CLIENT
    modelFreeAllLODs(1);
#endif
    trickReloadPostProcess();
#if CLIENT
    if (g_reloaded)
        fxGeoResetAllCapes();
#endif
    loadend_printf("Done.");
    modelReload_needsReload = 0;
    modelReload_numReloads++;
}
