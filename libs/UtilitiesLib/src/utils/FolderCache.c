/***************************************************************************
*     Copyright (c) 2003-2006, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
* 
***************************************************************************/

#include "utilitieslib/utils/FolderCache.h"
#include <string.h>
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/components/genericlist.h"
#include "utilitieslib/utils/DirMonitor.h"
#include <sys/stat.h>
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/fileutil.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/winfiletime.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/components/StringTable.h"
#include "utilitieslib/utils/fileWatch.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/StringCache.h"
#include "utilitieslib/utils/fileWatch.h"
#include "utilitieslib/components/SharedMemory.h"

static void FolderCacheAddPig(FolderCache *fc, PigFile *pigfile, int virtual_location);

typedef struct FolderCacheCallbackInfo{
    struct FolderCacheCallbackInfo *next;
    struct FolderCacheCallbackInfo *prev;
    union{
        FolderCacheCallback callback;
        FolderCacheCallbackEx callbackEx;
    };
    FolderCache* fc;
    FolderNode* node;
    int when;
    int isEx;
    char filespec[MAX_PATH];
} FolderCacheCallbackInfo;

typedef struct PigChangeCallbackMapping {
    FolderCache *fc;
    int virtual_location;
} PigChangeCallbackMapping;

static void fcCallbackDeleted(void *data, const char* path);
static void fcCallbackUpdated(void *data, const char* path,
	U32 filesize, U32 timestamp);
static void fcCallbackNew(void *data, const char* path,
	U32 filesize, U32 timestamp);

int folder_cache_debug=0;
int folder_cache_update_enable=1;
int folder_cache_manual_callback_check=0;

static int folder_cache_dirMonTimeout = 0;
static int folder_cache_noDisableOnBufferOverruns = 0;

static FolderCacheExclusion folder_cache_exclusion = FOLDER_CACHE_ALL_FOLDERS;
static char *folder_cache_exclusion_folder=NULL;


static const char* storeHashKeyStringLocal(const char* pcStringToCopy)
{
    return allocAddString(pcStringToCopy);
}


void FolderCacheExclude(FolderCacheExclusion mode, const char *folder) {
    folder_cache_exclusion = mode;
    SAFE_FREE(folder_cache_exclusion_folder);
    switch(mode) {
    xcase FOLDER_CACHE_ALL_FOLDERS:
        assert(folder==NULL);
    xcase FOLDER_CACHE_ONLY_FOLDER:
    case FOLDER_CACHE_EXCLUDE_FOLDER:
        folder_cache_exclusion_folder = strdup(folder);
    xdefault:
        assert(false);
    }
}

void FolderCacheSetFullScanCreatedFolders(FolderCache* fc, int enabled){
    fc->scanCreatedFolders = enabled ? 1 : 0;
}


static DWORD threadid=0;

FolderCache *FolderCacheCreate() {
    FolderCache *fc = calloc(sizeof(FolderCache),1);

    if (!threadid) {
        threadid = GetCurrentThreadId();
    }

    eaCreate(&fc->gamedatadirs);
    eaCreate(&fc->piggs);
    InitializeCriticalSectionAndSpinCount(&fc->critSecAllFiles, 4000);

    return fc;
}

void FolderCacheDestroy(FolderCache *fc)
{
    if(!fc){
        return;
    }
    
    EnterCriticalSection(&fc->critSecAllFiles);

    while(FolderCache_GetNumGDDs(fc)){
        dirMonRemoveDirectory(fc->gamedatadirs[0]);
        free(fc->gamedatadirs[0]);
        eaRemove(&fc->gamedatadirs, 0);
    }

    FolderNodeDestroy(fc, fc->root);
    fc->root = fc->root_tail = NULL;

    stashTableDestroy(fc->htAllFiles);
    fc->htAllFiles=0;

    eaClearEx(&fc->piggs, NULL);
    eaClearEx(&fc->gamedatadirs, NULL);

    eaDestroy(&fc->piggs);
    eaDestroy(&fc->gamedatadirs);
    LeaveCriticalSection(&fc->critSecAllFiles);
    DeleteCriticalSection(&fc->critSecAllFiles);
    
    free(fc);
}

void FolderCacheHashTableAdd(FolderCache* fc, const char* name, FolderNode* node){
    if(fc->htAllFiles){
        stashAddPointer(fc->htAllFiles, name, node, true);
        node->in_hash_table = 1;

        #if 0
        // Check parents.

        {
            FolderNode* cur;
            for(cur = node; cur; cur = cur->parent){
                assert(cur->in_hash_table);
            }
        }
        #endif
    }
}

FolderNode* FolderCacheHashTableRemove(FolderCache* fc, const char* name){
    FolderNode* node = NULL;
    
    if(fc->htAllFiles){
        stashRemovePointer(fc->htAllFiles, name, &node);
        
        if(node){
            node->in_hash_table = 0;
        }
    }
    
    return node;
}

void folderCacheVerifyIntegrity(FolderCache *fc) {
    FolderNode *walk;
    if (fc->root) {
        assert(fc->root_tail!=NULL);
    } else {
        assert(fc->root_tail==NULL);
    }
    walk = fc->root;
    while (walk) {
        FolderNodeVerifyIntegrity(walk, NULL);
        walk = walk->next;
    }
}

static int count_file=0;
static int count_folder=0;

static char *banned_overrides[] = {
    // These are not allowed to be overridden
    "/scenes",
    "/ent_types",
    "/object_library",
    "/geobin",
};
static int banned_overrides_len[ARRAY_SIZE(banned_overrides)];
static bool ban_all_overrides = false;

void FolderCacheDisallowOverrides(void)
{
    ban_all_overrides = true;
}

static void FolderCacheAddFolderToNode(    FolderCache* fc,
                                        FolderNode **node,
                                        FolderNode **tail,
                                        FolderNode *parent,
                                        const char *basepath,
                                        int virtual_location)
{
    char                        buffer[1024];
    FolderNode*                    node2;
    int                            i;
    int                            strLength;
    WIN32_FIND_DATAA             wfd;
    U32                            handle;

    if (!fileIsUsingDevData()) {
        static bool banned_overrides_inited=false;
        
        if (ban_all_overrides)
            return;
        if (!banned_overrides_inited) {
            for (i=0; i<ARRAY_SIZE(banned_overrides); i++) 
                banned_overrides_len[i] = (int)strlen(banned_overrides[i]);
            banned_overrides_inited=true;
        }

        strLength = (int)strlen(basepath);
        for (i=0; i<ARRAY_SIZE(banned_overrides); i++) {
            if(banned_overrides_len[i] > strLength)
                continue;
            if(stricmp(basepath + strLength - banned_overrides_len[i], banned_overrides[i]) == 0)
                return;
        }
    }

    strcpy(buffer, basepath);
    strcat(buffer, "/*");

    if (!fwFindFirstFile(&handle, buffer, &wfd)) {
        // no files found (this is not a valid filespec/path)
        // do nothing
        return;
    }
    do {
        const char* fileName = wfd.cFileName;
        U32            fileSize;
        __time32_t        utc_time;
        int            created;
        
        if(fileName[0] == '.')
            continue;
            
        fileSize = wfd.nFileSizeLow;
        
        _FileTimeToUnixTime(&wfd.ftLastWriteTime, &utc_time, FALSE);
        
        if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            // folder
            switch (folder_cache_exclusion) {
                xcase FOLDER_CACHE_EXCLUDE_FOLDER:
                    if (folder_cache_exclusion_folder && stricmp(fileName, folder_cache_exclusion_folder)==0) {
                        continue; // skip it!
                    }
                xcase FOLDER_CACHE_ONLY_FOLDER:
                    if (parent==NULL && folder_cache_exclusion_folder && stricmp(fileName, folder_cache_exclusion_folder)!=0) {
                        continue; // skip it if it doesn't match and this is in the root
                    }
                xcase FOLDER_CACHE_ALL_FOLDERS:
                    // Everything is good
                xdefault:
                    assert(false);
            }
            // add the folder
            STR_COMBINE_SS(buffer, fileName, "/");
            node2 = FolderNodeAdd(node, tail, parent, buffer, utc_time, fileSize, virtual_location, -1, &created);
            node2->is_dir = true; // in case it has no contents
            assert(node2->parent == parent);
            // Callback if created.
            if(created && fc->nodeCreateCallback){
                fc->nodeCreateCallback(fc, node2);
            }
            count_folder++;
        } else {
            // normal file
            node2 = FolderNodeAdd(node, tail, parent, fileName, utc_time, fileSize, virtual_location, -1, &created);
            
            if(node2){
                node2->writeable = !(wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY);
                node2->hidden = wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ? 1 : 0;
                node2->system = wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ? 1 : 0;
            
                // Callback if created.
                if(created && fc->nodeCreateCallback){
                    fc->nodeCreateCallback(fc, node2);
                }
            }
            
            count_file++;
        }

    } while (fwFindNextFile(handle, &wfd)!=0);
    
    fwFindClose(handle);

    for(node2 = *node; node2; node2 = node2->next){
        if(node2->is_dir){
            // Add the contents of the folder.
            STR_COMBINE_SSS(buffer, basepath, "/", node2->name);
            FolderCacheAddFolderToNode(fc, &node2->contents, &node2->contents_tail, node2, buffer, virtual_location);
        }
    }
}

static int FolderCacheBuildHashTableOp(const char *dir, FolderNode *node, void *userdata) {
    FolderCache *fc = (FolderCache*)userdata;
    StashElement elem;

    // Prevent calling storeHashKeyStringLocal unless necessary.
    
    if ( !stashFindElement(fc->htAllFiles, dir, &elem) )
    {
        const char* pcPath = storeHashKeyStringLocal(dir);
        FolderCacheHashTableAdd(fc, pcPath, node);
    }
    else
    {
        stashElementSetPointer(elem, node);
    }
    return 1;
}

void FolderCacheBuildHashTable(FolderCache *fc, FolderNode *node, char *pathsofar) {
    EnterCriticalSection(&fc->critSecAllFiles);
    if (!fc->htAllFiles)
    {
        fc->htAllFiles = stashTableCreateWithStringKeys(10000, StashDefault);
    }
    FolderNodeRecurseEx(node, FolderCacheBuildHashTableOp, (void*)fc, pathsofar);
    LeaveCriticalSection(&fc->critSecAllFiles);
}


typedef struct DirectoryUpdateContext {
    FolderCache *fc;
    int virtual_location;
} DirectoryUpdateContext;

void FolderCacheAddFolder(FolderCache *fc, const char *basepath, int virtual_location) {
    DirectoryUpdateContext *context=NULL;
    int timer;
    int update, discard;

    if (FolderCache_GetFromVirtualLocation(fc, virtual_location))
    {
        assert(!"Two folders with the same virtual_location!");
        return;
    }

    // Add to virtual_location list
    FolderCache_SetFromVirtualLocation(fc, virtual_location, strdup(basepath));

    if (FolderCacheGetMode()==FOLDER_CACHE_MODE_PIGS_ONLY
        || FolderCacheGetMode()==FOLDER_CACHE_MODE_FILESYSTEM_ONLY) {
        return;
    }

    // Add notification hook
    dirMonSetFlags(DIR_MON_NO_TIMESTAMPS);
    context = malloc(sizeof(*context));
    context->fc = fc;
    context->virtual_location = virtual_location;
    if (!dirMonAddDirectory(basepath, true, (void*)context)) {
        verbose_printf("Error adding dirMon!  Dynamic reloading may not fully function.\n");
    }
    
    if (FolderCacheGetMode()!=FOLDER_CACHE_MODE_DEVELOPMENT_DYNAMIC) {

        timer = timerAlloc();
        timerStart(timer);
        count_file=0;
        count_folder=0;

        // Scan HD
        
        FolderCacheAddFolderToNode(fc, &fc->root, &fc->root_tail, NULL, basepath, virtual_location);

        // Re-Fill hashtable by parsing the tree
        //if (fc->htAllFiles)
        // It's important that this table be built, so that when the piggs are loaded in prod mode 
        // they make shallow copies of they keynames into shared memory, and don't make extra local
        // deep copies of every string
        
        FolderCacheBuildHashTable(fc, fc->root, "");

        FolderNodeGetCounts(&discard, &update);
        if (folder_cache_debug) {
            printf("FOLDER: %25s: Added %5d files and %5d folders with %5d updates and %5d discards in %1.3gs\n",
                basepath, count_file, count_folder, update, discard, timerElapsed(timer));
        }
        timerFree(timer);
    }
}

// returns the virtual location or -1 if not found
int FolderCacheFindFolder(FolderCache *fc, const char *basepath )
{
    char** ppFolder = fc->gamedatadirs;
    while ( *ppFolder )
    {
        ppFolder++;
    }
    return -1;
}

int FolderCacheAddAllPigs(FolderCache *fc) {
    int i;
    if (FolderCacheGetMode() == FOLDER_CACHE_MODE_FILESYSTEM_ONLY) {
        return 0;
    }
    if (-1 == PigSetGetNumPigs()) {
        return 1; //TODO return an error
    }

    for (i=0; i<PigSetGetNumPigs(); i++)  {
        FolderCacheAddPig(fc, PigSetGetPigFile(i), PIG_INDEX_TO_VIRTUAL_LOCATION(i));
    }
    return 0;
}

// Scan for new pigs that are downloaded during gameplay.  Only call after FolderCacheAddAllPigs has been called at startup.
//    Return the number of new piggs loaded.
int FolderCacheAddNewPigs(FolderCache *fc) {
    int i, numPigs, oldNumPigs;
    if (FolderCacheGetMode() == FOLDER_CACHE_MODE_FILESYSTEM_ONLY) {
        return 0;
    }

    assert(PigSetInited());
    oldNumPigs = PigSetGetNumPigs();
    if( !PigSetCheckForNew() )
        return 0; // no new piggs found

    numPigs = PigSetGetNumPigs();
    assert(numPigs >= oldNumPigs);
    for (i=oldNumPigs; i<numPigs; i++)  {
        FolderCacheAddPig(fc, PigSetGetPigFile(i), PIG_INDEX_TO_VIRTUAL_LOCATION(i));
    }
    return numPigs - oldNumPigs;
}

static void FolderCacheAddPig(FolderCache *fc, PigFile *pigfile, int virtual_location)
{
    U32 i;
    int skipped=0;
    int update, discard;
    char temp[MAX_PATH];
    int timer = timerAlloc();
    bool addToHashtable=!!fc->htAllFiles;
    U32 num_files;
    PigChangeCallbackMapping *callback_data;
    timerStart(timer);

    if (FolderCache_GetFromVirtualLocation(fc, virtual_location))
    {
        assert(!"Two pigs with the same virtual_location!");
        timerFree(timer);
        return;
    }

    // Add to virtual_location list
    STR_COMBINE_SS(temp, PigFileGetArchiveFileName(pigfile), ":");
//    assert(temp[1]==':' || temp[0]=='.');
    assert(fileIsAbsolutePath(temp));
    FolderCache_SetFromVirtualLocation(fc, virtual_location, strdup(temp));

    if (addToHashtable)
        EnterCriticalSection(&fc->critSecAllFiles);

    PigFileLock(pigfile); // Make sure no one changes it while we're adding the information but before we've set up our callbacks!

    num_files = PigFileGetNumFiles(pigfile);
    for (i=0; i<num_files; i++)
    {
        const char *relpath = PigFileGetFileName(pigfile, i);
        FolderNode *node;
        int created;

        if (!relpath || PigFileIsSpecialFile(pigfile, i)) {
            skipped++;
            continue;
        }

        if (FolderCacheGetMode()==FOLDER_CACHE_MODE_DEVELOPMENT) {
            if (NULL==FolderCacheQuery(fc, relpath)) {
                //printf("File %s not found in File system, but is in Pig, NOT adding\n", relpath);
                skipped++;
                continue;
            }
        }
        //printf("%s\n", relpath);
        node = FolderNodeAdd(&fc->root, &fc->root_tail, NULL, relpath,
            PigFileGetFileTimestamp(pigfile, i),
            PigFileGetFileSize(pigfile, i), virtual_location, i, &created);

        if(node){
            if (addToHashtable) {
                FolderCacheHashTableAdd(fc, relpath, node);
            }
            
            // Callback if created.
            if(created && fc->nodeCreateCallback){
                fc->nodeCreateCallback(fc, node);
            }
        }
    }
    // Fill hashtable by parsing the tree
    //FolderCacheBuildHashTable(fc, fc->root, "");

    callback_data = calloc(sizeof(*callback_data), 1);
    callback_data->fc = folder_cache;
    callback_data->virtual_location = virtual_location;

    if (!PigFileSetCallbacks(pigfile, callback_data, fcCallbackDeleted, fcCallbackNew, fcCallbackUpdated))
        free(callback_data);
    
    PigFileUnlock(pigfile);

    if (addToHashtable)
        LeaveCriticalSection(&fc->critSecAllFiles);

    FolderNodeGetCounts(&discard, &update);
    if (folder_cache_debug) {
        printf("PIG [%22s]: Added %5d files (%5d skipped) with %5d updates and %5d discards in %1.3gs\n",
            PigFileGetArchiveFileName(pigfile), PigFileGetNumFiles(pigfile) - skipped, skipped, update, discard, timerElapsed(timer));
    }
    timerFree(timer);
}


void printFilenameCallback(const char *relpath, int when) { // example/test callback
    printf("FolderCacheCallback on : %s\n", relpath);
}


static FolderCacheCallbackInfo *fccallbacks=NULL;
static FolderCacheCallbackInfo *queued_callbacks=NULL;

void FolderCacheSetCallback(int when, const char *filespec, FolderCacheCallback callback) {
    FolderCacheCallbackInfo * newitem;
    
    if( sharedMemoryGetMode() == SMM_ENABLED && !(when & FOLDER_CACHE_CALLBACK_CAN_USE_SHARED_MEM) ) {
        // @todo these printf make a mess of load_printf console output, if we want to log them maybe group log lines all together at the end.
        // it might make more sense to log what the servers will actually allow to be reloaded on the fly
        // printf("FolderCacheSetCallback filespec disabled for shared memory: \"%s\"\n", filespec);
        return;
    }

    writeConsole(OUTPUT_DEBUG, "\tSet folder cache callback for %s", filespec);
    assert(when!=0);
    newitem = listAddNewMember(&fccallbacks, sizeof(FolderCacheCallbackInfo));
    strcpy(fccallbacks->filespec, filespec);
    fccallbacks->callback = callback;
    fccallbacks->when = when;
}

void FolderCacheSetCallbackEx(int when, const char *filespec, FolderCacheCallbackEx callback) {
    FolderCacheCallbackInfo * newitem;

    if( sharedMemoryGetMode() == SMM_ENABLED && !(when & FOLDER_CACHE_CALLBACK_CAN_USE_SHARED_MEM) ) {
        // @todo these printf make a mess of load_printf console output, if we want to log them maybe group log lines all together at the end.
        // it might make more sense to log what the servers will actually allow to be reloaded on the fly
        // printf("FolderCacheSetCallbackEx: Ignoring request because shared memory is enabled.  Folder: \"%s\"\n", filespec);
        return;
    }

    assert(when!=0);
    newitem = listAddNewMember(&fccallbacks, sizeof(FolderCacheCallbackInfo));
    strcpy(fccallbacks->filespec, filespec);
    fccallbacks->callbackEx = callback;
    fccallbacks->when = when;
    fccallbacks->isEx = 1;
}

static void checkForCallbacks(FolderCache* fc, FolderNode* node, int whathappened, const char *filename) {
    FolderCacheCallbackInfo *walk;
    FolderCacheCallbackInfo *newitem;
    if(FolderCacheMatchesIgnorePrefixAnywhere(filename))
        return;
    for (walk = fccallbacks; walk; walk = walk->next) {
        if ((whathappened & walk->when) && simpleMatch(walk->filespec, filename)) {
            newitem = listAddNewMember(&queued_callbacks, sizeof(FolderCacheCallbackInfo));
            if(walk->isEx){
                newitem->fc = fc;
                newitem->node = node;
            }
            newitem->callback = walk->callback;
            strcpy(newitem->filespec, filename);
            newitem->when = whathappened;
        }
    }
}

static void doAllCallbacks(void) {
    while (queued_callbacks) {
        FolderCacheCallbackInfo *walk = queued_callbacks;
        bool doit=true;
        // If either this is the last element, or it's not the same filename as the next element
        // (to prevent multiple callbacks on a single file)
        if (walk->next) {
            FolderCacheCallbackInfo *w2 = walk->next;
            while (w2) {
                if (stricmp(walk->filespec, w2->filespec)==0 && walk->callback==w2->callback) {
                    doit=false;
                    break;
                }
                w2 = w2->next;
            }
        }
        if (doit) {
            ErrorfResetCounts();
            if(walk->fc){
                walk->callbackEx(walk->fc, walk->node, walk->filespec, walk->when);
            }else{
                walk->callback(walk->filespec, walk->when);
            }
        }
        listFreeMember(walk, &queued_callbacks);
    }
}

void FolderCacheSetManualCallbackMode(int on) // Call this if you're calling FolderCacheDoCallbacks each tick
{
    folder_cache_manual_callback_check = on;
}

void FolderCacheDoCallbacks(void)
{
    folder_cache_manual_callback_check = 1;
    FolderCacheQuery(NULL, NULL);
    doAllCallbacks();
}

static void handleDeleteInternal(FolderCache *fc, FolderNode *node, int virtual_location) {
    // TODO: make this do the following:
    //  If it's a folder deleted, call handleDelete on all entries off of the folder that match
    //     the virtual location
    //  If it's a file, and the node matches the virtual_location, scan the file system sources
    //     to see if it still exists in another one of the sources

    // Remove from tree
    EnterCriticalSection(&fc->critSecAllFiles);
    FolderNodeDeleteFromTree(fc, node);
    LeaveCriticalSection(&fc->critSecAllFiles);
}

static void FolderCacheRemoveNonexistent(FolderCache* fc, const char* relativeDir){
    WIN32_FIND_DATAA wfd;
    U32                handle;
    FolderNode*        dirNode = FolderNodeFind(fc->root, relativeDir);
    FolderNode*        curNode;
    S32                good;
    char            buffer[MAX_PATH];
    
    // Find the first node in the dir.
    
    if(dirNode){
        dirNode = dirNode->contents;
    }
    else if(!relativeDir || !relativeDir[0]){
        dirNode = fc->root;
    }
    else{
        //printf("Can't find dir: %s/%s\n", fc->gamedatadirs[0], relativeDir);
        return;
    }
    
    // Mark all nodes for deletion.
    
    for(curNode = dirNode; curNode; curNode = curNode->next){
        curNode->delete_if_absent = 1;
    }
    
    STR_COMBINE_SSSS(buffer, fc->gamedatadirs[0], "/", relativeDir, "/*");

    for(good = fwFindFirstFile(&handle, buffer, &wfd); good; good = fwFindNextFile(handle, &wfd)){
        const char* fileName = wfd.cFileName;
        FolderNode* node;
        
        node = FolderNodeFind(dirNode, fileName);
        
        if(node){
            node->delete_if_absent = 0;
        }
    }
    
    for(curNode = dirNode; curNode;){
        FolderNode* next = curNode->next;
        
        if(curNode->delete_if_absent){
            STR_COMBINE_SSS(buffer, relativeDir, relativeDir[0] ? "/" : "", curNode->name);

            //printf("Deleting absent node: %s/%s (deleted using short file name?)\n", fc->gamedatadirs[0], buffer);

            handleDeleteInternal(fc, curNode, 0);
        }

        curNode = next;
    }

    fwFindClose(handle);
}

static void handleDelete(FolderCache *fc, int virtual_location, const char *relpath_const, int log)
{
    char relpath[MAX_PATH];
    FolderNode *node;
    Strncpyt(relpath, relpath_const);
    forwardSlashes(relpath);
    node = FolderNodeFind(fc->root, relpath);
    // File has been deleted
    if (node==NULL) {
        if(strchr(relpath, '~')){
            // Deleted using short filename, so no way to know for sure which file it was... rescanning parent.
            
            if(    FolderCache_GetNumGDDs(fc) == 1 &&
                FolderCache_GetNumPiggs(fc) == 0)
            {
                char buffer[1000];
                char* lastSlash;

                strcpy(buffer, relpath_const);
                
                forwardSlashes(buffer);
                
                lastSlash = strrchr(buffer, '/');
                
                if(lastSlash){
                    *lastSlash = 0;
                }else{
                    buffer[0] = 0;
                }

                FolderCacheRemoveNonexistent(fc, buffer);
            }
        }
        else if (folder_cache_debug>=2 && log) {
            // Happens when things are too quick for us to catch, just let it go by
            printfColor(COLOR_RED, "(created and deleted)\n");
        }
    } else {
        if (!node->contents ||
            FolderCache_GetNumGDDs(fc) == 1 &&
            FolderCache_GetNumPiggs(fc) == 0)
        {
            // Only delete it IF:
            //   a. It's an empty tree in all gamedatadirs (and thus has no "contents"), OR...
            //   b. There's only one gamedatadir, OR...
            //   c. It's a single file.

            bool needToCheck=false;
            if (folder_cache_debug>=2 && log) {
                printfColor(COLOR_RED|COLOR_BRIGHT, "deleting\n");
            }
            if (node && !node->is_dir)
                needToCheck = true;
            handleDeleteInternal(fc, node, virtual_location);
            if (needToCheck)
                checkForCallbacks(fc, NULL, FOLDER_CACHE_CALLBACK_DELETE, relpath);
        }
    }
    if (fc->htAllFiles) {
        EnterCriticalSection(&fc->critSecAllFiles);
        FolderCacheHashTableRemove(fc, relpath);
        LeaveCriticalSection(&fc->critSecAllFiles);
    }
}

static void handleUpdate(FolderCache *fc, int virtual_location, int file_index, const char *relpath_const, U32 filesize, __time32_t timestamp, U32 attrib, int isCreate, int log)
{
    char relpath[MAX_PATH];
    FolderNode *node;
    Strncpyt(relpath, relpath_const);
    node = FolderNodeFind(fc->root, relpath);
    // File has changed or is new
    if (folder_cache_debug>=2 && log) {
        if (node==NULL) {
            printfColor(COLOR_GREEN | COLOR_BRIGHT, "adding\n");
        } else {
            printfColor(COLOR_BLUE | COLOR_BRIGHT, "updating\n");
        }
    }

    if(node){
        node->writeable = !(attrib & _A_RDONLY);
        node->hidden = attrib & _A_HIDDEN ? 1 : 0;
        node->system = attrib & _A_SYSTEM ? 1 : 0;
    }

    if (node && fileDatesEqual(node->timestamp, timestamp, true) && node->size == filesize) {
        node->timestamp = timestamp;
    } else {
        int created;

        node = FolderNodeAdd(&fc->root, &fc->root_tail, NULL, relpath, timestamp, filesize, virtual_location, file_index, &created);

        if (node) { // Sometimes it doesn't do an update, like if it is just responding to an access time modification
            node->is_dir = attrib & _A_SUBDIR ? 1 : 0;
            // Update attributes after update/create
            node->writeable = !(attrib & _A_RDONLY);
            node->hidden = attrib & _A_HIDDEN ? 1 : 0;
            node->system = attrib & _A_SYSTEM ? 1 : 0;

            // Callback if created.
            if(created && fc->nodeCreateCallback){
                fc->nodeCreateCallback(fc, node);
            }

            forwardSlashes(relpath);
            EnterCriticalSection(&fc->critSecAllFiles);
            // fpe removed 1/20/11, firing in getanimation2 if run without -prescan -- assert(fc->htAllFiles);
            if (fc->htAllFiles)
            {
                const char* pcPath = storeHashKeyStringLocal(relpath);
                FolderCacheHashTableAdd(fc, pcPath, node);
            }
            //assert(hashFindElement(fc->allfiles, relpath));
            LeaveCriticalSection(&fc->critSecAllFiles);

            if(    created &&
                fc->scanCreatedFolders &&
                node->is_dir &&
                FolderCache_GetNumGDDs(fc) == 1 &&
                FolderCache_GetNumPiggs(fc) == 0)
            {
                char buffer[MAX_PATH];

                STR_COMBINE_SSS(buffer, fc->gamedatadirs[0], "/", relpath);

                FolderCacheAddFolderToNode(    fc,
                    &node->contents,
                    &node->contents_tail,
                    node,
                    buffer,
                    0);

                FolderCacheBuildHashTable(fc, node->contents, relpath);
            }
        }
        if (node && !node->is_dir)
            checkForCallbacks(fc, node, FOLDER_CACHE_CALLBACK_UPDATE, relpath);
    }
}

int FolderCacheSetDirMonTimeout(int timeout){
    int old = folder_cache_dirMonTimeout;
    
    folder_cache_dirMonTimeout = timeout;
    
    return old;
}

void FolderCacheSetNoDisableOnBufferOverruns(int value){
    folder_cache_noDisableOnBufferOverruns = value;
}

static int FolderCacheUpdate(DirChangeInfo** bufferOverrunOut)
{
    DirChangeInfo *dci;
    static volatile long is_doing_update = 0;
    long test;
    bool hog_file_changed=false;
    DirChangeInfo* bufferOverrun;

    if (!folder_cache_update_enable) {
        return 0;
    }

    if (GetCurrentThreadId()!=threadid) {
        return 0;
    }

    test = InterlockedIncrement(&is_doing_update);
    if (test>=2) { // Someone else is in this function
        InterlockedDecrement(&is_doing_update);
        return 1; // No updates while in a callback!
    }

    // Check for fs changes
    while(dci = dirMonCheckDirs(folder_cache_dirMonTimeout, &bufferOverrun)) {
        DirectoryUpdateContext *context = (DirectoryUpdateContext*)dci->userData;
        char fullpath[MAX_PATH], long_path_name[MAX_PATH];
        char *relpath;
        struct _finddata32_t fd;
        int fdHandle;
        int log=1;

        if (folder_cache_debug>=2) {
            if (strnicmp(dci->filename + strlen(dci->filename) - 4, ".log", 4)==0 ||
                strstri(dci->filename, "WINDOWS\\system")!=0 ||
                strstri(dci->filename, "Temp\\vc7")!=0 ||
                strnicmp(dci->filename, "RECYCLER", 8)==0) {
                    log=0;
                }
            if (log) {
                consoleSetFGColor(COLOR_RED | COLOR_BLUE | COLOR_GREEN);
                printf("%s/%s: ", dci->dirname, dci->filename);
            }
        }
        // process it

        assert(stricmp(FolderCache_GetFromVirtualLocation(context->fc, context->virtual_location), dci->dirname)==0);
        // Make full path
        STR_COMBINE_SSS(fullpath, dci->dirname, "/", dci->filename);
        makeLongPathName(fullpath, long_path_name);
        strcpy(fullpath, long_path_name);
        // Make relative path from the LFN
        relpath = fullpath + strlen(dci->dirname);
        while (*relpath=='/')
            relpath++;

        // Check if it's a hog file
        if (strEndsWith(dci->filename, ".hogg"))
            hog_file_changed = true;
            
        // Force a delete, to catch those damn short filename thingies.
        if(dci->isDelete){
            handleDelete(context->fc, context->virtual_location, dci->filename, log);
        }
        
        // Find existing node
        fdHandle = _findfirst32(fullpath, &fd);
        if (fdHandle==-1) {
            // Only re-delete this if the name is different from the one above.
            if(!dci->isDelete || stricmp(dci->filename, relpath)){
                // Deleted!
                handleDelete(context->fc, context->virtual_location, relpath, log);
            }
        } else {
            U32 timestamp = statTimeToUTC(fd.time_write);
            U32 filesize = fd.size;
            
            _findclose(fdHandle);
            
            handleUpdate(context->fc, context->virtual_location, -1, relpath,
                filesize, timestamp, fd.attrib, dci->isCreate, log);
        }

        if(bufferOverrun)
        {
            printf("ERROR reading directory changes! ");
            if(fileIsUsingDevData() && !folder_cache_noDisableOnBufferOverruns)
            {
                printf("Disabling directory cache!\n");
                FolderCacheSetMode(FOLDER_CACHE_MODE_FILESYSTEM_ONLY);
            }
            else
            {
                printf("Rebuilding directory cache!\n");
                // FIXME: restart dirmon
                // FIXME: scan folders and fake updates
            }
            InterlockedDecrement(&is_doing_update);
            return 1;
        }
    }

    // Check for hog modifications
    if (hog_file_changed && PigSetInited()) {
        int i;
        for (i=0; i<PigSetGetNumPigs(); i++) {
            PigFileCheckForModifications(PigSetGetPigFile(i));
        }
    }
    if (!folder_cache_manual_callback_check) {
        doAllCallbacks();
    }
    InterlockedDecrement(&is_doing_update);
    return 0;
}

static void fcCallbackDeleted(void *data, const char* path)
{
	PigChangeCallbackMapping *mapping = data;
    //printf("File deleted: %s\n", path);
    handleDelete(mapping->fc, mapping->virtual_location, path, 1);
}

static void fcCallbackUpdated(void *data, const char* path,
	U32 filesize, U32 timestamp)
{
	PigChangeCallbackMapping *mapping = data;
    int file_index=-1;
    PigFile *pig_file;
    pig_file = PigSetGetPigFile(VIRTUAL_LOCATION_TO_PIG_INDEX(mapping->virtual_location));
    file_index = PigFileFindIndexQuick(pig_file, path);
    assert(file_index != -1);
    //printf("File new/updated: %s  size:%d  timestamp:%d\n", path, filesize, timestamp);
    handleUpdate(mapping->fc, mapping->virtual_location, file_index, path, filesize, timestamp, 0, 0, 1);
}

static void fcCallbackNew(void *data, const char* path,
	U32 filesize, U32 timestamp)
{
	PigChangeCallbackMapping *mapping = data;
    fcCallbackUpdated(mapping, path, filesize, timestamp);
}

FolderNode *FolderCacheFileSystemOverride(FolderCache *fc, char *relpath)
{
    FolderNode *ret;
    int i;
    struct _stat32 sbuf;
    char filename[MAX_PATH];
    static StashTable htOverrideCache = 0;
    static CRITICAL_SECTION critsect; // Because the StashTable isn't thread-safe
    static int critsect_init = 0;
    int in_hash_table=0;

    if (!critsect_init) {
        InitializeCriticalSectionAndSpinCount(&critsect, 4000);
        critsect_init = 1;
    }
    EnterCriticalSection(&critsect);

    if (!htOverrideCache) {
        htOverrideCache = stashTableCreateWithStringKeys(1000, StashDeepCopyKeys);
    }

    ret = (FolderNode*)stashFindPointerReturnPointer(htOverrideCache, relpath);
    if (ret) {
        in_hash_table = 1;
    }
#ifndef _XBOX
    if (relpath[1]==':') { // actuall any absolute path
        if (_AltStat(relpath, &sbuf)!=-1) { //file exists
#else
    if (fileIsAbsolutePath(relpath)) { // actuall any absolute path
        if (fwStat(relpath,&sbuf) !=-1) { //file exists
#endif
            if (!in_hash_table)
                ret = FolderNodeCreate();
            strcpy(filename, relpath);
            ret->contents=NULL;
            ret->is_dir= (sbuf.st_mode & _S_IFDIR) ? 1: 0;
            ret->name = strdup(filename); // really passing back an absolute path
            ret->next = ret->parent = ret->prev = NULL;
            ret->size = sbuf.st_size;
            ret->timestamp = sbuf.st_mtime;
            ret->virtual_location = -99;
            if (!in_hash_table)
                stashAddPointer(htOverrideCache, relpath, ret, true);
            LeaveCriticalSection(&critsect);
            return ret;
        }
        LeaveCriticalSection(&critsect);
        return NULL;
    }

    // Just do a _stat or two, ignore the folder cache
    for (i=FolderCache_GetNumGDDs(fc)-1; i>=0; i--)
    {
        char *folder = FolderCache_GetFromVirtualLocation(fc, i);
        if (!folder) continue;
        STR_COMBINE_SSS(filename, folder, "/", relpath);
#ifndef _XBOX
        if (_AltStat(filename, &sbuf)!=-1) { //file exists
#else
        if (fwStat(filename,&sbuf) !=-1) { //file exists
#endif
            if (!in_hash_table)
                ret = FolderNodeCreate();
            strcpy(filename, relpath);
            ret->contents=NULL;
            ret->is_dir= (sbuf.st_mode & _S_IFDIR) ? 1: 0;
            if (!ret->name || stricmp(ret->name, filename)!=0) {
                size_t len = strlen(filename)+1;
                SAFE_FREE(ret->name);
                ret->name = malloc(len);
                strcpy_s(ret->name, len, filename);
            }
            ret->next = ret->parent = ret->prev = NULL;
            ret->size = sbuf.st_size;
            ret->timestamp = sbuf.st_mtime;
            ret->virtual_location = i;
            if (!in_hash_table)
                stashAddPointer(htOverrideCache, relpath, ret, true);
            LeaveCriticalSection(&critsect);
            return ret;
        }
    }
    LeaveCriticalSection(&critsect);
    return NULL;
}

static char *ignorePrefixArray[MAX_IGNORE_PREFIXES] = {0};
static int numIgnorePrefixes = 0;
const char* g_StdAdditionalFilePrefixes[] =
{
    "v_",
    "d_",
    ""
};
static const char* g_StdIgnorePrefixes[] =
{
    "d_",    // development only data
    ""
};

char ** FolderCacheGetIgnoredPrefixes(int *num)
{
    *num = numIgnorePrefixes;
    return ignorePrefixArray;
}

void FolderCacheIgnoreStdPrefixes(void)
{
    const char** prefix = g_StdIgnorePrefixes;
    while (**prefix)
    {
        FolderCacheAddIgnorePrefix(*prefix);
        prefix++;
    }
}

void FolderCacheAddIgnorePrefix(const char *prefix)
{
    assert(numIgnorePrefixes < MAX_IGNORE_PREFIXES);
    ignorePrefixArray[numIgnorePrefixes++] = strdup(prefix);
}

void FolderCacheRemoveIgnorePrefix(const char *prefix)
{
    int i,j;
    for (i = 0; i < numIgnorePrefixes; i++)
    {
        if (stricmp(ignorePrefixArray[i], prefix)==0)
        {
            numIgnorePrefixes--;
            SAFE_FREE(ignorePrefixArray[i]);
            for (j = i; j < numIgnorePrefixes; j++)
                ignorePrefixArray[j] = ignorePrefixArray[j+1];
            i--;
        }
    }
}

bool FolderCacheMatchesIgnorePrefixStart(const char *path)
{
    int prefixnum, prefixlen;
    char *prefix;

    for (prefixnum = 0; prefixnum < numIgnorePrefixes; prefixnum++)
    {
        prefix = ignorePrefixArray[prefixnum];
        if (prefix[0] == 0)
            continue;

        prefixlen = (int)strlen(prefix);

        if (strnicmp(path, prefix, prefixlen)==0)
        {
            return true;
        }
    }
    return false;
}

bool FolderCacheMatchesIgnorePrefixAnywhere(const char *path)
{
    int prefixnum, prefixlen;
    char *prefix;
    char prefixbuf[MAX_PATH];

    for (prefixnum = 0; prefixnum < numIgnorePrefixes; prefixnum++)
    {
        prefix = ignorePrefixArray[prefixnum];
        if (prefix[0] == 0)
            continue;

        prefixlen = (int)strlen(prefix);

        if (strnicmp(path, prefix, prefixlen)==0)
        {
            return true;
        }

        _strlwr_s(prefix, prefixlen+1);
        STR_COMBINE_SS(prefixbuf, "/", prefix);
        //sprintf(prefixbuf, "/%s", prefix);
        if (strstr(path, prefixbuf))
        {
            return true;
        }

        _strupr_s(prefix, prefixlen+1);
        STR_COMBINE_SS(prefixbuf, "/", prefix);
        //sprintf(prefixbuf, "/%s", prefix);
        if (strstr(path, prefixbuf))
        {
            return true;
        }
    }
    return false;
}

FolderNode *FolderCacheQuery(FolderCache *fc, const char *relpath)
{
    char path[MAX_PATH];
    FolderNode *node;
    int do_not_trust_piggs=0; // If we're in a callback that may have written to disk, we can't trust the piggs
    DirChangeInfo* bufferOverrun = NULL;
    
    // Build a hashtable if it doesn't exist yet
    if (fc && !fc->htAllFiles) {
        FolderCacheBuildHashTable(fc, fc->root, "");
    }

    PERFINFO_AUTO_START("FolderCacheUpdate", 1);
        do_not_trust_piggs = FolderCacheUpdate(&bufferOverrun);
    PERFINFO_AUTO_STOP();
    
    if(bufferOverrun){
        DirectoryUpdateContext* updateContext = bufferOverrun->userData;
        
        updateContext->fc->needsRescan = 1;
    }

    if (fc==NULL || relpath==NULL) return NULL;

    assert(*(U32*)relpath!=0xCCCCCCCC);

    // purdy up relpath
    strcpy(path, relpath);
    forwardSlashes(path);
    fixDoubleSlashes(path);

    // check for ignore prefixes (such as v_ and d_)
    if (FolderCacheMatchesIgnorePrefixAnywhere(path))
        return NULL;

    while (path[0]=='/') {
        strcpy(path, path+1);
    }
    if (FolderCacheGetMode() == FOLDER_CACHE_MODE_FILESYSTEM_ONLY) {
        PERFINFO_AUTO_START("FolderCacheFileSystemOverride", 1);
            node = FolderCacheFileSystemOverride(fc, path);
        PERFINFO_AUTO_STOP();
        
        return node;
    }
    assert(relpath[1]!=':');
    EnterCriticalSection(&fc->critSecAllFiles);
    if ( !stashFindPointer(fc->htAllFiles, path, &node))
    {
        node = NULL;
    }
    LeaveCriticalSection(&fc->critSecAllFiles);
    if (FolderCacheGetMode() == FOLDER_CACHE_MODE_DEVELOPMENT_DYNAMIC) {
        // If we're in dynamic mode, if it's not in the hashtable, it may exist somewhere on the disk...
        if (!node) {
            PERFINFO_AUTO_START("FolderCacheFileSystemOverride", 1);
                node = FolderCacheFileSystemOverride(fc, path);
            PERFINFO_AUTO_STOP();
            
            if (node) {
                const char* pcPath = storeHashKeyStringLocal(path);
                node->seen_in_fs = 1;
                EnterCriticalSection(&fc->critSecAllFiles);
                FolderCacheHashTableAdd(fc, pcPath, node);
                LeaveCriticalSection(&fc->critSecAllFiles);
            }
            return node;
        }
        if (node->virtual_location>=0 && !do_not_trust_piggs) {
            // It's in the HT, and it's marked as being from the filesystem, we must have queried this file once before
            return node;
        } else {
            FolderNode *node2;
            // It's at least in a .pig file, check the filesystem
            if (node->seen_in_fs && !do_not_trust_piggs) {
                return node;
            }
            
            PERFINFO_AUTO_START("FolderCacheFileSystemOverride", 1);
                node2 = FolderCacheFileSystemOverride(fc, path);
            PERFINFO_AUTO_STOP();
            
            if (!node2) {
                return NULL;
            }
            // In FS and Pigg, return the FS if it's different
            if (node->timestamp!=node2->timestamp) {
                const char* pcPath = storeHashKeyStringLocal(path);
                EnterCriticalSection(&fc->critSecAllFiles);
                FolderCacheHashTableAdd(fc, pcPath, node2);
                LeaveCriticalSection(&fc->critSecAllFiles);
                return node2;
            } else {
                // Timestamps are the same
                node->seen_in_fs = 1;
            }
            return node;
        }
    }
    return node;
}

void FolderCacheEnableCallbacks(int enable) { // Disables/enables checking for changes in the filesystem (turn these off when loading)
    folder_cache_update_enable = enable;
}

void FolderCacheChooseMode(void) { // Auto-chooses the appropriate mode, based on existence of ./piggs
    if (fileIsUsingDevData()) {
        // This mode reads data from a pig file, only if it's identical to the one on the filesystem
        FolderCacheSetMode(FOLDER_CACHE_MODE_DEVELOPMENT_DYNAMIC);
        if (pig_debug)
            OutputDebugStringA("Pig mode: development\n");
    } else {
        // This mode reads all data from pigs if available, but still allows filesystem overrides
        FolderCacheSetMode(FOLDER_CACHE_MODE_I_LIKE_PIGS);
        if (pig_debug)
            OutputDebugStringA("Pig mode: semi-production\n");
    }
}

static FolderCacheMode folder_cache_mode=FOLDER_CACHE_MODE_FILESYSTEM_ONLY; // Disabled by default
// Per-thread override stack
static struct {
    DWORD tid;
    FolderCacheMode override;
} fcm_overrides[8];
static int fcm_override_count=0;
static CRITICAL_SECTION fcmo_critsec;
static bool fcmo_critsec_inited=false;

void FolderCacheSetMode(FolderCacheMode mode)
{
    if (!fcmo_critsec_inited) {
        InitializeCriticalSectionAndSpinCount(&fcmo_critsec, 4000);
        fcmo_critsec_inited = true;
    }
    folder_cache_mode = mode;
}

FolderCacheMode FolderCacheGetMode(void)
{
    if (fcm_override_count)
    {
        int i;
        EnterCriticalSection(&fcmo_critsec);
        for (i=fcm_override_count-1; i>=0; i--) {
            if (fcm_overrides[i].tid == GetCurrentThreadId()) {
                LeaveCriticalSection(&fcmo_critsec);
                return fcm_overrides[i].override;
            }
        }
        LeaveCriticalSection(&fcmo_critsec);
    }
    return folder_cache_mode;
}

void FolderCacheModePush(FolderCacheMode newmode) // Another hack for dynamic mode
{
    if (folder_cache_mode==FOLDER_CACHE_MODE_DEVELOPMENT_DYNAMIC)
    {
        int index;
        EnterCriticalSection(&fcmo_critsec);
        index = fcm_override_count++;
        assertmsg(index<ARRAY_SIZE(fcm_overrides), "FolderCache stack overflow");
        fcm_overrides[index].tid = GetCurrentThreadId();
        fcm_overrides[index].override = newmode; // FOLDER_CACHE_MODE_DEVELOPMENT;
        LeaveCriticalSection(&fcmo_critsec);
    }
}

void FolderCacheModePop(void) // Another hack for dynamic mode
{
    if (folder_cache_mode==FOLDER_CACHE_MODE_DEVELOPMENT_DYNAMIC)
    {
        int i, j;
        EnterCriticalSection(&fcmo_critsec);
        for (i=fcm_override_count-1; i>=0; i--) {
            if (fcm_overrides[i].tid == GetCurrentThreadId()) {
                for (j=i; j<fcm_override_count-1; j++) {
                    fcm_overrides[j].tid = fcm_overrides[j+1].tid;
                    fcm_overrides[j].override = fcm_overrides[j+1].override;
                }
                fcm_override_count--;
                LeaveCriticalSection(&fcmo_critsec);
                return;
            }
        }
        assertmsg(0, "FolderCache Stack underflow");
        LeaveCriticalSection(&fcmo_critsec);
    }
}


char *FolderCacheGetRealPath(FolderCache *fc, FolderNode *node, char *buffer, size_t buffer_size)
{
    char temp[MAX_PATH];

    if (!node) return NULL;

    assert(buffer);

    strcpy_s(buffer, buffer_size, FolderCache_GetFromVirtualLocation(fc, node->virtual_location));
    if (buffer[strlen(buffer)-1]=='/') {
        buffer[strlen(buffer)-1]=0;
    }
    strcat_s(buffer, buffer_size, FolderNodeGetFullPath(node, temp));
    return buffer;
}

int FolderNodePigIndex(FolderNode *node) {
    if (node->virtual_location >=0) {
        return -1;
    } else {
        return -node->virtual_location - 1;
    }
}

void FolderCacheRequestTree(FolderCache *fc, const char *relpath) { // If we're in dynamic mode, this will load the tree!
    static StashTable ht_added_trees=0;
    char path[MAX_PATH], basepath[MAX_PATH];
    int i, num_dirs;
    FolderNode *node;

    if (FolderCacheGetMode() != FOLDER_CACHE_MODE_DEVELOPMENT_DYNAMIC) {
        return;
    }

    if (ht_added_trees==0) {
        ht_added_trees = stashTableCreateWithStringKeys(10000, StashDeepCopyKeys);
    }

    writeConsole(OUTPUT_DEBUG, "\t%s", relpath);
    while (relpath[0]=='/') relpath++;
    strcpy(path, relpath);
    relpath = path;

    if ( stashFindInt(ht_added_trees, relpath, NULL)) {
        return;
    }
    stashAddInt(ht_added_trees, relpath, 1, true);

    node = FolderNodeFind(fc->root, path);
    if (!node) {
        int created;
        
        // The node isn't there, add it!
        node = FolderNodeAdd(&fc->root, &fc->root_tail, NULL, path, 0, 0, 0, -1, &created);
        
        if(node){
            node->is_dir = 1;
            
            if(created && fc->nodeCreateCallback){
                fc->nodeCreateCallback(fc, node);
            }
        }
    }

    num_dirs = FolderCache_GetNumGDDs(fc);
    for (i=0; i<num_dirs; i++)
    {
        char *getpath;
        if (getpath = FolderCache_GetFromVirtualLocation(fc, i))
        {
            STR_COMBINE_SSS(basepath, getpath, "/", path);
            FolderCacheAddFolderToNode(fc, &node->contents, &node->contents_tail, node, basepath, i);
        }
    }
    if (fc->htAllFiles) {
        FolderCacheBuildHashTable(fc, node->contents, path);
    }
}

int FolderCache_GetNumGDDs(FolderCache *fc)
{
    return eaSize(&fc->gamedatadirs);
}

int FolderCache_GetNumPiggs(FolderCache *fc)
{
    return eaSize(&fc->piggs);
}

char *FolderCache_GetFromVirtualLocation(FolderCache *fc, int virtual_location)
{
    if (virtual_location < 0)
    {
        // pigg
        virtual_location = -virtual_location - 1;
        return (char *)eaGet(&fc->piggs, virtual_location);
    }

    // game data dir
    return (char *)eaGet(&fc->gamedatadirs, virtual_location);
}

void FolderCache_SetFromVirtualLocation(FolderCache *fc, int virtual_location, char *path)
{
    if (virtual_location < 0)
    {
        // pigg
        virtual_location = -virtual_location - 1;
        if (virtual_location >= eaSize(&fc->piggs))
            eaSetSize(&fc->piggs, virtual_location + 1);
        eaSet(&fc->piggs, path, virtual_location);
    }
    else
    {
        // game data dir
        if (virtual_location >= eaSize(&fc->gamedatadirs))
            eaSetSize(&fc->gamedatadirs, virtual_location + 1);
        eaSet(&fc->gamedatadirs, path, virtual_location);
    }
}
