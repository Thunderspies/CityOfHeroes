/***************************************************************************
*     Copyright (c) 2003-2006, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
***************************************************************************/
#include "utilitieslib/assert/assert.h"
#define WIN32_LEAN_AND_MEAN

#include "utilitieslib/utils/FolderCache.h"
#include <windows.h>
#include "utilitieslib/utils/piglib.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/components/genericlist.h"

#include "utilitieslib/utils/timing.h"
#include <direct.h>
#include <sys/types.h>
#include <sys/utime.h>
#include "utilitieslib/components/SharedMemory.h"
#include "utilitieslib/network/crypt.h"
#include "zlib/zlib.h"
#include "utilitieslib/utils/winfiletime.h"
#include "utilitieslib/utils/hoglib.h"
#include "utilitieslib/utils/MemoryMonitor.h"
#include "utilitieslib/components/StashTable.h"
#include "patchtest.h"
#include "heavyTest.h"
#include "utilitieslib/utils/mathutil.h"
#include <sys/stat.h>
#include "utilitieslib/utils/fileutil.h"
//#include "fileutil2.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/components/StringCache.h"
#include "utilitieslib/utils/threadedFileCopy.h"
#include "utilitieslib/utils/ListView.h"
#include "resource.h"
#include "utilitieslib/utils/winutil.h"
#include <commctrl.h>
#include "utilitieslib/utils/log.h"
#include "utilitieslib/utils/textparser.h"

#define loadstart_printf if (verbose) loadstart_printf
#define loadend_printf if (verbose) loadend_printf

typedef struct FileSpecNode FileSpecNode;
typedef struct FileSpecNode
{
    bool        bIsIncludes;
    StashTable    explicitList;
    StringArray wildcardedList;

    FileSpecNode *next;
} FileSpecNode;


static int pause=0;
extern void *extractFromFS(const char *name, U32 *count);  // in piglib.c

static PigFilePtr pig_file;
static PigFilePtr old_pig_file = NULL;
static char base_folder[MAX_PATH];
static int verbose=0;
static int debug_verbose=0;
static int overwrite=0;
static int incremental=1;
FileSpecNode *fileSpecs=NULL;
const char **currentIncludes=0;
static NewPigEntry *pig_entries;
static int pig_entry_count,pig_entry_max;
static int base_folder_set=0;
static int zip_compress;
static bool ignore_underscores=true;
static bool preserve_pathname=false;
static int make_pig_version = PIG_VERSION;

static int get_data=1;

enum {
    PIG_CREATE=1,
    PIG_LIST,
    PIG_EXTRACT,
    PIG_GENLIST,
    PIG_UPDATE,
    PIG_WLIST,
};

static FileSpecNode *CreateFileSpecNode(bool includes)
{
    FileSpecNode *newNode = malloc(sizeof(FileSpecNode));

    newNode->bIsIncludes = includes;

    newNode->explicitList = stashTableCreateWithStringKeys(100, StashDeepCopyKeys);

    eaCreateWithCapacity(&newNode->wildcardedList, 10);

    newNode->next = NULL;

    return newNode;
}

static void pak() {
    int c;
    //extern void hogThreadingStopTiming(void);
    //hogThreadingStopTiming();
    if (pause) {
        printf("Press any key to continue...\n");
        c = _getch();
    } else {
        printf("\n");
    }
}

static void checksum(U8 *data,int count,U32 *checksum)
{
    cryptMD5Update(data,count);
    cryptMD5Final(checksum);
}


static void closeOldPig()
{
    if(old_pig_file) {
        char oldpiggname[MAX_PATH];
        strncpy(oldpiggname, PigFileGetArchiveFileName(old_pig_file), MAX_PATH);
        PigFileDestroy(old_pig_file);
        fileForceRemove(oldpiggname);
    }
    old_pig_file = NULL;
}

extern PigErrLevel pig_err_level;
static int openOldPig(const char *filename)
{
    int ret = 0;
    int old_pig_err_level = pig_err_level;
    char oldfilename[MAX_PATH];
    sprintf_s(SAFESTR(oldfilename), "%s.bak", filename);

    if(!fileExists(oldfilename)) {
        printf("No old pigg '%s' exists, cannot incrementally update\n", oldfilename);
        return 1;
    }

    // open pigg that was renamed to .bak for comparison
    old_pig_file = PigFileAlloc();
    
    // disable assertions and try to read the old file
    pig_err_level = PIGERR_PRINTF;
    ret = PigFileRead(old_pig_file, oldfilename);
    pig_err_level = old_pig_err_level;

    if(ret != 0) {
        printf("Failed to open the old pigg '%s' for incremental mode\n", oldfilename);
        fileForceRemove(oldfilename);
        old_pig_file = NULL; // PigFileRead will imlicitly call PigFileDestroy
    }
    return ret;
}

static bool copyFromOldPig(NewPigEntry *entry, const char *filename)
{
    S32 index;
    U32 oldtime, oldsize;
    bool oldzipped;
    bool shouldbezipped;

    if(!old_pig_file) {
        return false;
    }

    index = PigFileFindIndexQuick(old_pig_file, entry->fname);
    if(index == -1) {
        if(verbose > 2) printf("not reusing %s (missing)\n", entry->fname);
        return false;
    }

    oldtime = PigFileGetFileTimestamp(old_pig_file, index);
    if(oldtime != entry->timestamp) {
        if(verbose > 2) printf("not reusing %s (time mismatch)\n", entry->fname);
        return false;
    }

    oldsize = PigFileGetFileSize(old_pig_file, index);
    if(oldsize != fileSize(filename)) {
        if(verbose > 2) printf("not reusing %s (size mismatch)\n", entry->fname);
        return false;
    }

    oldzipped = PigFileIsZipped(old_pig_file, index);
    shouldbezipped = !entry->dont_pack && (entry->must_pack || !doNotCompressMyExt(entry));
    if(oldzipped && shouldbezipped) {
        // just copy compressed data
        entry->data = PigFileExtractCompressed(old_pig_file, entry->fname, &entry->pack_size);
        assert(entry->data);
        assert(entry->pack_size);
        entry->size = oldsize;
        verify(PigFileGetChecksum(old_pig_file, index, entry->checksum));
        if(verbose>1) printf("%s (reused old compressed version)\n", entry->fname);
    } else if(!oldzipped && !shouldbezipped) {
        // just copy uncompressed data
        entry->data = PigFileExtract(old_pig_file, entry->fname, &entry->size);
        assert(entry->data);
        assert(entry->size == oldsize);
        verify(PigFileGetChecksum(old_pig_file, index, entry->checksum));
        if(verbose>1) printf("%s (reused old uncompressed version)\n", entry->fname);
    } else {
        // repack
        entry->data = PigFileExtract(old_pig_file, entry->fname, &entry->size);
        assert(entry->data);
        assert(entry->size == oldsize);
        pigChecksumAndPackEntry(entry);
        if(verbose) printf("%s (repacking)\n", entry->fname);
    }

    return true;
}

static bool testFileSpec(const char *filename, FileSpecNode *fileSpecNode, bool currentlyIncluded)
{
    int i;
    int dummyInt = 0;
    int num_filespecs;

    // If no file spec supplied, match everything
    if (!fileSpecNode)
        return true;

    if ((currentlyIncluded && !fileSpecNode->bIsIncludes) ||
        (!currentlyIncluded && fileSpecNode->bIsIncludes))
    {
        if (stashFindInt(fileSpecNode->explicitList, filename, &dummyInt))
        {
            currentlyIncluded = !currentlyIncluded;
            
            // Nothing else should match this explicit entry.
            // So, remove it so we don't have to check against it in the future
            stashRemoveInt(fileSpecNode->explicitList, filename, &dummyInt);
        }
        
        num_filespecs = eaSize(&fileSpecNode->wildcardedList);

        for (i=0; i<num_filespecs; i++)
        {
            if (match(fileSpecNode->wildcardedList[i], filename))
            {
                currentlyIncluded = !currentlyIncluded;
                break;
            }
        }
    }

    if (fileSpecNode->next)
    {
        return testFileSpec(filename, fileSpecNode->next, currentlyIncluded);
    }

    return currentlyIncluded;
}

static int checkFileSpec(const char *filename) { // returns 1 if it passes
    char temp[MAX_PATH];
    char *c;
    const char *c2;
    int ret = 0;

    for (c=temp, c2 = filename; *c2; c++, c2++)
    {
        *c = toupper(*c2);
        if (*c == '\\')
            *c = '/';
    }
    *c = 0;

    ret = testFileSpec(temp, fileSpecs, false);

    return ret;
}

FileScanAction pigInputProcessor(char* dir, struct _finddata32_t* data) {
    char filename[MAX_PATH];
    char *fn;
    NewPigEntry *entry;
    const char *ext;

    if (data->name[0]=='_' && ignore_underscores) {
        return FSA_NO_EXPLORE_DIRECTORY;
    }
    if (data->attrib & _A_SUBDIR) {
        return FSA_EXPLORE_DIRECTORY;
    }
    ext = strrchr(data->name, '.');
    if (ext) {
        if (stricmp(ext, ".bak")==0 || stricmp(ext, ".pigg")==0)
        {
            return FSA_EXPLORE_DIRECTORY;
        }
    }
    STR_COMBINE_SSS(filename, dir, "/", data->name);
    fn = filename + strlen(base_folder);
    if (*fn=='/') fn++;
    fn = strdup(fn);

    // check against filespec
    if (!currentIncludes)
    {
        eaCreateWithCapacityConst(&currentIncludes, 10);
    }

    if (!checkFileSpec(fn)) {
        return FSA_EXPLORE_DIRECTORY;
    }

    entry = dynArrayAdd(&pig_entries,sizeof(pig_entries[0]),&pig_entry_count,&pig_entry_max,1);
    forwardSlashes(filename);
    fixDoubleSlashes(filename);
    assert(strnicmp(filename, base_folder, strlen(base_folder))==0);
    entry->fname = fn;
    entry->dont_pack = !zip_compress;
#if 0
    {
        // Check that our conversions work
        U32 findfirstvalue = data->time_write;
        U32 pstvalue;
        U32 statvalue;
        U32 altstatvalue = fileLastChangedAltStat(filename);
        U32 utcvalue; // should be the same as findfirstvalue
        struct _stat32 statbuf;
        pststat(filename, &statbuf);
        pstvalue = statbuf.st_mtime;
        _stat32(filename, &statbuf);
        statvalue = statbuf.st_mtime;
        utcvalue = statTimeToUTC(statvalue);
        assert(statvalue == findfirstvalue);
        assert(altstatvalue == utcvalue);
        assert(pstTimeToUTC(pstvalue) == utcvalue);
        //printf("s:%10d g:%10d u:%10d %s\n", statvalue, pstvalue, utcvalue, filename);
    }
#endif
    if (make_pig_version == PIG_VERSION) {
        // Make sure I didn't break anything
        static U32 altstattime, utctime;
        altstattime = fileLastChangedAltStat(filename);
        utctime = statTimeToUTC(data->time_write);
        assert(utctime == altstattime);
        // Remove above lines after this is verified
    }
    entry->timestamp = statTimeToUTC(data->time_write);

    if (get_data) {
        bool got_old_data = false;

        if (incremental) {
            if(copyFromOldPig(entry, filename)) {
                got_old_data = true;
            }
        }

        if(!got_old_data) {
            U8 *newdata;
            if(verbose) printf("%s (updated)\n",fn);
            entry->data = extractFromFS(filename,&entry->size);
            pigChecksumAndPackEntry(entry);
            if (entry->pack_size) {
                // Attempt to defragment memory
                newdata = malloc(entry->pack_size);
                memcpy(newdata, entry->data, entry->pack_size);
                SAFE_FREE(entry->data);
                entry->data = newdata;
            }
        }
    } else {
        entry->data = NULL;
        entry->size = fileSize(filename);
    }
    assert(entry->size != -1);

    return FSA_EXPLORE_DIRECTORY;
}


void addFile(const char *path) {
    struct _finddata32_t c_file;
    intptr_t hFile;
    char local_base_folder[MAX_PATH];

    if( (hFile = _findfirst32( path, &c_file )) == -1L ) {
        printf( "No files matching '%s found!\n", path);
    } else {
        if (path[0]!='.' && path[1]!=':' && path[0]!='/') {
            // relative patch
            sprintf(local_base_folder, "./%s", path);
        } else {
            strcpy(local_base_folder, path);
        }
        forwardSlashes(local_base_folder);
        getDirectoryName(local_base_folder);
        if (base_folder_set) {
            assert(strnicmp(local_base_folder, base_folder, strlen(base_folder))==0);
        } else {
            strcpy(base_folder, local_base_folder);
        }
        do {
            pigInputProcessor(local_base_folder, &c_file);
        } while (_findnext32( hFile, &c_file ) == 0 );
        _findclose( hFile);
    }
}

bool buildIncludeList(const char *filename, FileSpecNode *currentFileSpecNode)
{
    char *s,*mem,*args[100];
    int count;
    char temp[MAX_PATH];
    // These characters are wildcards used by the match() function
    const char *wildcards = "?:*+[]";
    bool processingNotList = false;

    if (!currentIncludes)
    {
        eaCreateWithCapacityConst(&currentIncludes, 10);
    }

    if (StringArrayFind(currentIncludes, (char*) filename) != -1)
    {
        printf("Error: Circular include/exclude detected with filename '%s'\n", filename);
        return false;
    }

    eaPushConst(&currentIncludes, filename);

    strcpy(temp, filename);
    mem = extractFromFS(temp, 0);
    if (!mem) {
        sprintf(temp, "./%s", filename);
        mem = extractFromFS(temp, 0);
    }
    if (!mem) {
        printf("Error opening list file: '%s'\n", filename);
        eaPopConst(&currentIncludes);
        return false;
    }

    // Fill data 
    s = mem;
    while(s && (count=tokenize_line(s,args,&s))>=0)
    {
        char *c, *c2;
        char buf[2048] = "";
        int i;
        bool hasWildcards = false;

        // Ignore empty lines and lines which start with '#'
        if (count == 0 || args[0][0]=='#')
            continue;

        // Handle include blah blah blah.txt
        // and handle exclude blah blah blah.txt
        if (count > 1 && (strcmp(args[0], "include") == 0 || strcmp(args[0], "exclude") == 0))
        {
            int value = 0;

            buf[0] = 0;
            for (i=1; i<count; i++) {
                strcat(buf, args[i]);
                if (i!=count-1)
                    strcat(buf, " ");
            }

            // RECURSION
            if (strcmp(args[0], "include") == 0)
            {
                if (processingNotList)
                {
                    // Switch back to normal includes
                    FileSpecNode *newNode = CreateFileSpecNode(!currentFileSpecNode->bIsIncludes);
                    currentFileSpecNode->next = newNode;
                    currentFileSpecNode = newNode;

                    processingNotList = false;
                }

                // Add includes to the current list
                if (!buildIncludeList(buf, currentFileSpecNode))
                {
                    eaPopConst(&currentIncludes);
                    return false;
                }
            }
            else // is an exclude
            {
                // Add excludes to a new list
                if (!processingNotList)
                {
                    // Switch back to normal includes
                    FileSpecNode *newNode = CreateFileSpecNode(!currentFileSpecNode->bIsIncludes);
                    currentFileSpecNode->next = newNode;
                    currentFileSpecNode = newNode;

                    processingNotList = true;
                }

                // Add includes to the current NOT list
                if (!buildIncludeList(buf, currentFileSpecNode))
                {
                    eaPopConst(&currentIncludes);
                    return false;
                }
            }
            continue;
        }

        // concatenate, make uppercase, and look for wildcards
        c = buf;
        for (i=0; i<count; i++)
        {
            for (c2 = args[i]; *c2; c2++, c++)
            {
                const char *w = wildcards;
                *c = toupper(*c2);

                if (*c == '\\')
                    *c = '/';
                // look for wildcards
                else if (!hasWildcards)
                {
                    while(*w)
                    {
                        if (*c == *w)
                        {
                            hasWildcards = true;
                            break;
                        }
                        w++;
                    }
                }
            }

            if (i!=count-1)
            {
                *c = ' ';
                c++;
            }
        }
        *c = 0;

        if (buf[0] == '!')
        {
            if (!processingNotList)
            {
                // Add NOTs to a new list
                FileSpecNode *newNode = CreateFileSpecNode(!currentFileSpecNode->bIsIncludes);
                currentFileSpecNode->next = newNode;
                currentFileSpecNode = newNode;

                processingNotList = true;
            }

            // shift left one character to remove the '!'
            for (c = buf, c2 = buf + 1; *c2; c++, c2++)
                *c = *c2;
            *c = 0;
        }
        else
        {
            if (processingNotList)
            {
                // Switch back to normal includes
                FileSpecNode *newNode = CreateFileSpecNode(!currentFileSpecNode->bIsIncludes);
                currentFileSpecNode->next = newNode;
                currentFileSpecNode = newNode;

                processingNotList = false;
            }
        }

        if (hasWildcards)
        {
            char *tempLine = strdup(buf);
            eaPush(&currentFileSpecNode->wildcardedList, tempLine);
        }
        else
        {
            // optimization: if this file is never included, don't
            // bother adding it to the exclude list
            if (currentFileSpecNode->bIsIncludes || checkFileSpec(buf))
                stashAddInt(currentFileSpecNode->explicitList, buf, 0, false);
        }
    }

    free(mem);

    eaPopConst(&currentIncludes);
    return true;
}

void addFromFile(const char *filename) {
    fileSpecs = CreateFileSpecNode(true);

    if (buildIncludeList(filename, fileSpecs))
    {
        base_folder_set=1;
        strcpy(base_folder, ".");
        fileScanDirRecurseEx(base_folder, &pigInputProcessor);
    }
}

StashTable prune_table = 0;
void buildPruneTable(const NewPigEntry *entries, int count)
{
    int i;
    if (prune_table)
        stashTableDestroy(prune_table);
    prune_table = stashTableCreateWithStringKeys(count*2, StashDefault);
    for (i=0; i<count; i++) {
        stashAddInt(prune_table, entries[i].fname, 1, false);
    }
}

bool shouldPrune(const NewPigEntry *entries, int count, const char *fn)
{
    return !stashFindElement(prune_table, fn, NULL);
}

void getData(NewPigEntry *entry)
{
    char fn[MAX_PATH];
    if (entry->data == (void*)1)
        return;
    sprintf(fn, "%s/%s", base_folder, entry->fname);
    assert(fileExists(fn));
    entry->data = extractFromFS(fn,&entry->size);
}

typedef struct ChangeLog {
    S64 delta;
    int count;
    int min;
    int max;
} ChangeLog;

static StashTable changes;

static void logChange(const char *ext, int sizedelta)
{
    ChangeLog *log;
    if (!changes) {
        changes = stashTableCreateWithStringKeys(256, StashDeepCopyKeys);
    }
    if (!stashFindPointer(changes, ext, &log)) {
        log = calloc(sizeof(*log), 1);
        log->min = log->max = sizedelta;
        stashAddPointer(changes, ext, log, true);
    }
    log->delta += sizedelta;
    log->count++;
    log->min = MIN(log->min, sizedelta);
    log->max = MAX(log->max, sizedelta);
}

static int logChangeDumper(StashElement element)
{
    const char *ext = stashElementGetStringKey(element);
    ChangeLog *log = stashElementGetPointer(element);
    F32 delta = log->delta;
    F32 avg = delta / log->count;
    if (log->count < 10)
        return 1;
    printf("%10s avg: %12.5f  #changes: %6d min: %9d  max: %9d\n", ext, avg, log->count, log->min, log->max);
    return 1;
}

static void logChangeDump()
{
    stashForEachElement(changes, logChangeDumper);
}

// Updates a hogg file, removing missing files, adding new files, updating changed files
#ifndef DISABLE_HOGS
int updateHogFile(NewPigEntry *entries, int count, const char *output)
{
    int ret=0;
    U32 i;
    U32 numfiles;
    int lastval=0;
    HogFile *hog_file;
    bool bCreated;
    int delcount=0, modcount=0, newcount=0, timestampcount=0;

    if (verbose!=2)
        loadstart_printf("Modifying Hogg file...");
    if (verbose==2)
        loadstart_printf("Loading old hogg...");
    if (!(hog_file = hogFileReadOrCreate(output, &bCreated)))
    {
        if (fileExists(output)) {
            assert(0); // Need better error recovery code!
        } else {
            assert(0); // Need to implement deciding to create anew if an update failed to read
        }
        hogFileDestroy(hog_file);
        SAFE_FREE(hog_file);
        loadend_printf("ERROR");
        return ret;
    }
    if (verbose==2)
        loadend_printf("done.");
    if (verbose==2)
        loadstart_printf("Pruning removed files...");
    // Prune removed files (do first to free space)
    buildPruneTable(entries, count);
    numfiles = hogFileGetNumFiles(hog_file);
    for (i=0; i<numfiles; i++) {
        const char *name = hogFileGetFileName(hog_file, i);
        if (!name)
            continue;
        if (hogFileIsSpecialFile(hog_file, i))
            continue;
        if (shouldPrune(entries, count, name)) {
            if (verbose==2)
                printf("%s: REMOVED                         \n", name);
            delcount++;
            if (ret=hogFileModifyUpdateNamed(hog_file, name, NULL, 0, 0))
                assert(0);
        }
        if ((i % 100)==99 || delcount != lastval) {
            int v = printf("%d/%d (%d deleted)", i+1, numfiles, delcount);
            for (; v; v--) 
                printf("%c", 8); // Backspace
            lastval = delcount;
        }
    }
    if (verbose==2)
        loadend_printf("done (%d deleted).", delcount);
    if (verbose==2)
        loadstart_printf("Modifying/Adding files...\n");
    // Modify existing files
    for (i=0; i<(U32)count; i++) {
        HogFileIndex file_index;
        const char *name = entries[i].fname;
        if ((file_index = hogFileFind(hog_file, name))!=HOG_INVALID_INDEX) {
            U32 timestamp = hogFileGetFileTimestamp(hog_file, file_index);
            U32 size = hogFileGetFileSize(hog_file, file_index);
            U32 headersize;
            const U8 *headerdata = hogFileGetHeaderData(hog_file, file_index, &headersize);
            bool needHeaderUpdate = !pigShouldCacheHeaderData(strrchr(name, '.')) != !headersize;
            bool isCompressed = hogFileIsZipped(hog_file, file_index);
            bool needUncompressing = pigShouldBeUncompressed(strrchr(name, '.')) && isCompressed;
            if (ABS_UNS_DIFF(timestamp, entries[i].timestamp) == 3600 &&
                statTimeToUTC(timestamp) == entries[i].timestamp &&
                size == entries[i].size &&
                !needHeaderUpdate)
            {
                // pstTime -> UTC time conversion
                hogFileModifyUpdateTimestamp(hog_file, file_index, entries[i].timestamp);
                timestampcount++;
            } else if (timestamp != entries[i].timestamp ||
                size != entries[i].size ||
                needHeaderUpdate ||
                needUncompressing
                )
            {
                if (verbose==2)
                    printf("%s: Modified                            \n", name);
                //logChange(strrchr(name, '.'), entries[i].size - size);
                modcount++;
                getData(&entries[i]);
                // Zips and crcs and gets headerdata in here:
                if (ret=hogFileModifyUpdate2(hog_file, file_index, &entries[i], false))
                    assert(0);
                entries[i].data = NULL;
            } else {
                //printf("%s: Up to date\n", name);
            }
        } else {
            if (verbose==2)
                printf("%s: NEW                                   \n", name);
            newcount++;
            getData(&entries[i]);
            if (!entries[i].data) {
                Errorf("Error reading from file: %s", entries[i].fname);
            } else {
                if (ret=hogFileModifyUpdateNamed2(hog_file, &entries[i]))
                    assert(0);
            }
            entries[i].data = NULL;
        }
        if ((i % 100)==99 || modcount + newcount!=lastval) {
            int v = printf("%d/%d (%d modified, %d new)", i+1, count, modcount, newcount);
            for (; v; v--) 
                printf("%c", 8); // Backspace
            lastval = modcount + newcount;
        }
    }
    if (verbose==2)
        loadend_printf("done (%d modified, %d new).", modcount, newcount);
    if (verbose==2)
        loadstart_printf("Flushing to disk and cleanup up...");
    hogFileModifyFlush(hog_file);
    hogFileDestroy(hog_file);
    if (verbose==2)
        loadend_printf("done.");
    if (verbose!=2) {
        if (timestampcount) {
            loadend_printf("done (%d removed, %d modified, %d new, %d timestamps fixed).", delcount, modcount, newcount, timestampcount);
        } else {
            loadend_printf("done (%d removed, %d modified, %d new).", delcount, modcount, newcount);
        }
    }
    SAFE_FREE(hog_file);
    //logChangeDump();
    return 0;
}
#endif //DISABLE_HOGS

// Creates a .pig file out of all files in a specified folder
int makePig(const char *foldersource, const char *output) {
    int timer,i,ret;

    timer = timerAlloc();
    timerStart(timer);

    // Init
    pig_entry_count = 0;
    strcpy(base_folder, foldersource);
    if (preserve_pathname && strStartsWith(base_folder, "./")) {
        strcpy(base_folder, "./");
    }
    forwardSlashes(base_folder);
    fixDoubleSlashes(base_folder);

    if(incremental) {
        openOldPig(output);
    }

    // Scan
    if (verbose)
        loadstart_printf("Scanning for files...");
    if (dirExists(base_folder)) {
        fileScanDirRecurseEx(foldersource, &pigInputProcessor);
    } else if (strncmp(base_folder, "-T", 2)==0) {
        // Skip files which start with '_'
        // Find the last '/'
        int lastSlashOffset = 2;
        int index = 2;
        while(base_folder[index] != 0)
        {
            if (base_folder[index] == '/')
                lastSlashOffset = index;
            index++;
        }
        if (base_folder[lastSlashOffset] != 0 && base_folder[lastSlashOffset+1] == '_')
        {
            printf("pig: Cowardly refusing to create an archive for filespec %s which starts with a '_'\n", base_folder+2);
            return 1;
        }

        // load in a list from file
        addFromFile(base_folder+2);
    } else {
        // filespec or single file - HACK
        addFile(base_folder);
    }
    if (verbose)
        loadend_printf("done.");

    if (pig_entry_count==0 && make_pig_version != HOG_VERSION) {
        printf("pig: Cowardly refusing to create an empty archive\n");
        return 1;
    }
    if (output) {
        if (make_pig_version == HOG_VERSION && get_data == 0) {
#ifdef DISABLE_HOGS
        printf("pig: Hogs are disabled in this executable\n");
        assert(0);
        return 1;
#else
            // Incremental Update
            ret = updateHogFile(pig_entries, pig_entry_count, output);
#endif
        } else {
            ret = !pigCreateFromMem(pig_entries, pig_entry_count, output, make_pig_version);
        }
    } else {
        ret = 0; // genlist
    }

    if(incremental) {
        closeOldPig();
    }

    for(i=0;i<pig_entry_count;i++)
    {
        if (!get_data && !output) { // GEN_LIST
            printf("%s\n", pig_entries[i].fname);
        }
        SAFE_FREE(pig_entries[i].data);
        SAFE_FREE(pig_entries[i].fname);
    }
    SAFE_FREE(pig_entries);
    pig_entry_count = pig_entry_max = 0;
    return ret;
}

static int extractPig(const char *out)
{
    int ret;

    pig_file = PigFileAlloc();

    if (0==(ret=PigFileRead(pig_file, out))) {
        FILE *f;
        U32 i;
        for (i=0; i<PigFileGetNumFiles(pig_file); i++) {
            const char *name = PigFileGetFileName(pig_file, i);
            U32 count;
            char *mem;
            if (!name)
                continue; // Unused file
            if (PigFileIsSpecialFile(pig_file, i))
                continue;
            mem = PigFileExtract(pig_file, name, &count);
            if (!mem) {
                printf("Error getting %s from archive!\n", name);
                return 1;
            }
            if (verbose) {
                printf("%s\n", name);
            }
            mkdirtree((char*)name);
            f = fopen(name, "wb");
            if (!f) {
                printf("Can't open %s for writing!\n", name);
                return 1;
            }
            fwrite(mem, 1, count, f);
            fclose(f);
            {
                //struct _utimbuf utb;
                //utb.actime = utb.modtime = PigFileGetFileTimestamp(pig_file, i);
                //_utime(name, &utb);
                _SetUTCFileTimes(name, PigFileGetFileTimestamp(pig_file, i), PigFileGetFileTimestamp(pig_file, i));
            }
            free(mem);
        }
    }
    PigFileDestroy(pig_file);
    return ret;
}

static int listPig(const char *out) {
    int ret;
    
    pig_file = PigFileAlloc();

    if (0==(ret=PigFileRead(pig_file, out))) {
        PigFileDumpInfo(pig_file, verbose, debug_verbose);
    }
    PigFileDestroy(pig_file);
    return ret;
}

typedef struct PigEntryInfo
{
    const char *filename;    AST( NAME(Filename) POOL_STRING FORMAT_LVWIDTH(250) )
    int size;                AST( NAME("File Size") )
    int packed_size;        AST( NAME("Compressed Size") )
    __time32_t timestamp;    AST( NAME("Modification Time") FORMAT_FRIENDLYDATE FORMAT_LVWIDTH(120) )
    int index;                NO_AST
} PigEntryInfo;

ParseTable parse_PigEntryInfo[] =
{
    { "Filename",            TOK_POOL_STRING | TOK_STRING(PigEntryInfo, filename, 0), NULL  ,  TOK_FORMAT_LVWIDTH(250)},
    { "File Size",            TOK_AUTOINT(PigEntryInfo, size, 0), NULL },
    { "Compressed Size",    TOK_AUTOINT(PigEntryInfo, packed_size, 0), NULL },
    { "Modification Time",    TOK_AUTOINT(PigEntryInfo, timestamp, 0), NULL  ,  TOK_FORMAT_FRIENDLYDATE | TOK_FORMAT_LVWIDTH(120)},

    { "", 0, 0 },
};

static ListView *lvFiles;
static PigEntryInfo **eaFiles=NULL;
static HWND g_hDlg;
static HWND hStatusBar;

void pigViewView(ListView *lv, PigEntryInfo *entry, void *data)
{
    void *filedata;
    U32 count;
    filedata = PigFileExtract(pig_file, entry->filename, &count);
    if (filedata) {
        FILE *f;
        char tempname[MAX_PATH];

        sprintf(tempname, "C:/temp/%s", getFileNameConst(entry->filename));
        mkdirtree(tempname);
        f = fileOpen(tempname, "wb");
        fwrite(filedata, 1, count, f);
        fileClose(f);
        free(filedata);
        fileOpenWithEditor(tempname);
    }
}

void pigViewExtract(ListView *lv, PigEntryInfo *entry, void *data)
{
    void *filedata;
    U32 count;
    filedata = PigFileExtract(pig_file, entry->filename, &count);
    if (filedata) {
        FILE *f;
        char tempname[MAX_PATH];
        char fullpath[MAX_PATH];
        char message[1000];

        makefullpath(entry->filename, fullpath);
        //winGetFileName_s(g_hwnd, "*.*", 
        sprintf(tempname, "./%s", entry->filename);
        mkdirtree(tempname);
        f = fileOpen(tempname, "wb");
        if (f) {
            fwrite(filedata, 1, count, f);
            fileClose(f);
            free(filedata);

            sprintf(message, "File extracted to %s", fullpath);
            MessageBoxA(NULL, message, "File extracted", MB_OK);
        } else {
            sprintf(message, "Failed to open %s for writing", fullpath);
            MessageBoxA(NULL, message, "Extract failed", MB_OK);
        }
    }
}

typedef struct
{
    int uncompressed_size;
    int disk_size;
} TotalSizeStruct;

void pigViewTotalSize(ListView *lv, PigEntryInfo *entry, void *data)
{
    int index = PigFileFindIndexQuick(pig_file, entry->filename);
    if (index != -1 && data)
    {
        TotalSizeStruct *pSizeStruct = (TotalSizeStruct*) data;
        pSizeStruct->uncompressed_size += PigFileGetFileSize(pig_file, index);

        if (PigFileIsZipped(pig_file, index))
            pSizeStruct->disk_size += PigFileGetZippedFileSize(pig_file, index);
        else
            pSizeStruct->disk_size += PigFileGetFileSize(pig_file, index);
    }
}


void setStatusBar(int elem, const char *fmt, ...)
{
    char str[1000];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    SendMessageA(hStatusBar, SB_SETTEXTA, elem, (LPARAM)str);
}

INT_PTR CALLBACK DlgPigViewProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    int i;
    RECT rect;

    g_hDlg = hDlg;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        {
            // File List
            listViewInit(lvFiles, parse_PigEntryInfo, hDlg, GetDlgItem(hDlg, IDC_FILELIST));
            for (i=0; i<eaSize(&eaFiles); i++) {
                listViewAddItem(lvFiles, eaGet(&eaFiles, i));
            }
            listViewSort(lvFiles, 0); // Filename

            // Status bar stuff
            hStatusBar = CreateStatusWindowA( WS_CHILD | WS_VISIBLE, "Status bar", hDlg, 1);
            {
                int temp[1];
                temp[0]=-1;
                SendMessageA(hStatusBar, SB_SETPARTS, ARRAY_SIZE(temp), (LPARAM)temp);
            }

            EnableWindow(GetDlgItem(hDlg, IDC_ADD), FALSE);

            EnableWindow(GetDlgItem(hDlg, IDC_EXTRACT), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_VIEW), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_REMOVE), FALSE);

            GetClientRect(hDlg, &rect); 
            doDialogOnResize(hDlg, (WORD)(rect.right - rect.left), (WORD)(rect.bottom - rect.top), IDC_ALIGNME, IDC_UPPERLEFT);

            return FALSE;
        }
    case WM_SIZE:
        {
            WORD w = LOWORD(lParam);
            WORD h = HIWORD(lParam);
            doDialogOnResize(hDlg, w, h, IDC_ALIGNME, IDC_UPPERLEFT);
            SendMessageA(hStatusBar, iMsg, wParam, lParam);
            break;
        }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
            case IDOK:
            case IDCANCEL:
                EndDialog(hDlg, 0);
                return TRUE;
            xcase IDC_VIEW:
                listViewDoOnSelected(lvFiles, pigViewView, NULL);
            xcase IDC_EXTRACT:
                listViewDoOnSelected(lvFiles, pigViewExtract, NULL);

        }
        break;

    case WM_NOTIFY:
        {
            int idCtrl = (int)wParam;
            listViewOnNotify(lvFiles, wParam, lParam, NULL);

            if (idCtrl == IDC_FILELIST) {
                TotalSizeStruct totalSize = { 0, 0 };

                // Count number of selected elements
                i = listViewDoOnSelected(lvFiles, pigViewTotalSize, &totalSize);

                if (i==1) {
                    EnableWindow(GetDlgItem(hDlg, IDC_EXTRACT), TRUE);
                    EnableWindow(GetDlgItem(hDlg, IDC_VIEW), TRUE);
                } else {
                    EnableWindow(GetDlgItem(hDlg, IDC_EXTRACT), FALSE);
                    EnableWindow(GetDlgItem(hDlg, IDC_VIEW), FALSE);
                }

                // TODO: More fun info here!
                if (i > 0)
                    setStatusBar(0, "%d files selected, Total Uncompressed Size = %s bytes, Total Disk Size = %s bytes", 
                            i, getCommaSeparatedInt(totalSize.uncompressed_size), getCommaSeparatedInt(totalSize.disk_size));
                else
                    setStatusBar(0, "%d files selected", i);
            }
        }
        break;
    }
    return FALSE;
}



static int win32listPig(const char *out)
{
    int ret;
    int i;

    pig_file = PigFileAlloc();

    if (0==(ret=PigFileRead(pig_file, out))) {
        int hiret;
        for (i=0; i<(int)PigFileGetNumFiles(pig_file); i++) 
        {
            if (!PigFileGetFileName(pig_file, i))
                continue;
            //if (!PigFileIsSpecialFile(pig_file, i))
            {
                PigEntryInfo *entry = malloc(sizeof(PigEntryInfo));
                entry->filename = PigFileGetFileName(pig_file, i);
                entry->size = PigFileGetFileSize(pig_file, i);
                entry->timestamp = PigFileGetFileTimestamp(pig_file, i);

                if (PigFileIsZipped(pig_file, i))
                {
                    entry->packed_size = PigFileGetZippedFileSize(pig_file, i);
                }
                else
                {
                    entry->packed_size = entry->size;
                }

                entry->index = i;
                eaPush(&eaFiles, entry);
            }
        }

        lvFiles = listViewCreate();
        hiret = DialogBox (winGetHInstance(), (LPCTSTR) (IDD_PIG), NULL, DlgPigViewProc);
        //listViewDestroy(lvFiles);

    }
    //eaDestroyStruct(&eaFiles, parse_PigEntryInfo);
    //PigFileDestroy(pig_file);
    return ret;
}


static void nocrashCallback(char* errMsg)
{
    log_printf(0, "Shutting down from fatal error \"%s\"", errMsg);
    logWaitForQueueToEmpty();
    exit(1);
}

int main(int argc, char **argv)
{
    _CrtMemState g_memstate={0};
    char out[MAX_PATH], fn[MAX_PATH];
    int ret;
    char *valid_commands = "-ctxg23aBdhlipuvyzfw";
    int mode=0;
    int i;
    int overrideout=0;
    int loops=1; // For checking for memory leaks
    int loopcount;
    int timer;

    extern CRITICAL_SECTION PigCritSec;
    extern bool g_doHogTiming;
    //DO_AUTO_RUNS

    // g_doHogTiming = true;

    memCheckInit();
    fileAllPathsAbsolute(true);
    setAssertMode( (!IsDebuggerPresent()? ASSERTMODE_THROWEXCEPTION : 0) | ASSERTMODE_DEBUGBUTTONS);
    threadedFileCopyInit(1); // Just to pre-init the critical sections.  If this solves the crash, look into it more!

#ifndef DISABLE_HOGS
    hogSetGlobalOpenMode(HogSafeAgainstAppCrash);
    hogSetMaxBufferSize(512*1024*1024);
    hogThreadingInit();
#endif

    if (argc<3 || !(strspn(argv[1], valid_commands)==strlen(argv[1])) || strcmp(argv[1], "--help")==0) {
        printf("usage: %s  (c|u|t|x|g)[2][a][B][d][h][i][l][p][u][v][s][z][f out] [FILE] [-Cdir]\n", argv[0]);
        printf(" modes (one required):\n");
        printf("  c    create name.pigg from the contents of name/\n");
        printf("  u    incrementally update a Hogg file\n");
        printf("  t    list the contents of name.pigg in a Windows GUI\n");
        printf("  w    list the contents of name.pigg in the old console style\n");
        printf("  x    extract the contents of name.pigg to the current folder\n");
        printf("  g    generate list of files that would be pigged\n");
        printf(" flags:\n");
        printf("  2    extra verbose\n");
        printf("  a    preserve pAthname on ./paths\n");
        printf("  B    process with background priority\n");
        printf("  f    out is used as the output name\n");
        printf("  h    make a .hogg file\n");
        printf("  i    do not incrementally update a pigg file\n");
        printf("  p    pause on exit\n");
        printf("  u    do NOT ignore _underscored folders\n");
        printf("  v    verbose (vv means extra verbose)\n");
        printf("  y    automatically overwrite output files\n");
        printf("  z    compress data\n");
        printf("  -Cdir change directory to dir (for extracting)\n");
        printf("  -Rcmd.bat run cmd.bat before executing command (for testing)\n");
        printf("  --nocrash Just exit (with error code) on fatal error\n");
        printf("  --hogdebug Write out intermediate hogg files for debugging\n");
        return 1;
    }

    if (argc==2 && strcmp("--version", argv[1])==0) {
        printf("$Revision: 55663 $");
        return 0;
    }

    for (i=0; i<(int)strlen(argv[1]); i++)  {
        switch (argv[1][i]) {
            case '2':
                debug_verbose=1;
                break;
            case '3':
                debug_verbose=2;
                break;
            case 'a':
                preserve_pathname = true;
                break;
            case 'B':
                 SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_BEGIN);
                break;
            case 'c':
                mode=PIG_CREATE;
                break;
            case 'f':
                overrideout=1;
                break;
            case 'g':
                mode=PIG_GENLIST;
                break;
            case 'h':
#ifdef DISABLE_HOGS
                printf("pig: Hogs are disabled in this executable\n");
                assert(0);
                return 1;
#else
                make_pig_version = HOG_VERSION;
#endif
                break;
            case 'i':
                incremental=0;
                break;
            case 'p':
                pause=1;
                break;
            case 't':
                mode=PIG_WLIST;
                break;
            case 'u':
                if (i==0) {
                    mode = PIG_UPDATE;
#ifdef DISABLE_HOGS
                    printf("pig: Hogs are disabled in this executable\n");
                    assert(0);
                    return 1;
#else
                    make_pig_version = HOG_VERSION;
#endif
                } else
                    ignore_underscores=false;
                break;
            case 'v':
                verbose++;
                break;
            case 'w':
                mode=PIG_LIST;
                break;
            case 'x':
                mode=PIG_EXTRACT;
                break;
            case 'y':
                overwrite=1;
                break;
            case 'z':
                zip_compress=1;
                break;
        }
    }

    if (mode != PIG_GENLIST)
        cryptMD5Init();

    InitializeCriticalSection(&PigCritSec);

    sharedMemorySetMode(SMM_DISABLED);

    timer = timerAlloc();
    timerStart(timer);

    for (loopcount=0; loopcount<loops; loopcount++) {
        //U64 after, before = memMonitorBytesAlloced();
        //_CrtMemCheckpoint(&g_memstate);

        for (i=3; i<argc; i++) {
            char *s = argv[i];
            if (strStartsWith(s, "-C")) {
                assert(chdir(s+2));
            } else if (strStartsWith(s, "-R")) {
                printf("Running external tool %s...\n", s+2);
                system(s+2);
                printf("Done running external tool.\n");
            } else if (stricmp(s, "--nocrash")==0) {
                FatalErrorfSetCallback(nocrashCallback);
            } else if (stricmp(s, "--hogdebug")==0) {
                extern int hog_debug_check;
                hog_debug_check = 1;
            } else if (stricmp(s, "--nodata")==0) {
                extern int hog_mode_no_data;
                hog_mode_no_data = 1;
            } else if (stricmp(s, "--patchtest")==0) {
                return doPatchTest();
            } else if (stricmp(s, "--heavytest")==0) {
                return doHeavyTest();
            }
        }

        switch (mode) {
            case PIG_UPDATE:
                get_data = 0;
                // Intentionall fall through
            case PIG_CREATE:
                // Make out file from argv[2]
                if (overrideout) {
                    strcpy(out, argv[2]);
                    strcpy(fn, argv[3]);
                } else {
                    strcpy(out, argv[2]);
                    strcpy(fn, argv[2]);
                    while (strEndsWith(out, "/") || strEndsWith(out, "\\")) out[strlen(out)-1]=0;
                    strcat(out, ".pigg");
                }
                if (fileExists(out)) {
                    if (mode == PIG_CREATE) {
                        if (!overwrite && !incremental) {
                            printf("File %s already exists.  Overwrite? [y/N]", out);
                            if (!consoleYesNo()) {
                                ret = 2;
                                break;
                            }
                        }
                        if (fileCanGetExclusiveAccess(out)) {
                            // We have access to the file
                            if(incremental) {
                                fileRenameToBak(out);
                            } else {
                                fileForceRemove(out);
                            }
                        } else {
                            printf("Failed to open %s, cannot delete, aborting!\n", out);
                            ret = 3;
                            break;
                        }
                    }
                } else {
                    //if (mode == PIG_UPDATE) {
                    //    // File doesn't exist, do a create instead!
                    //    mode = PIG_CREATE;
                    //    get_data = 1;
                    //}
                }
                if (make_pig_version == HOG_VERSION) {
                    get_data = 0;
                }
                ret = makePig(fn, out);
                pak();
            xcase PIG_GENLIST:
                strcpy(fn, argv[2]);
                get_data = 0;
                ret = makePig(fn, NULL);
                pak();
            xcase PIG_LIST:
                strcpy(out, argv[2]);
                ret = listPig(out);
                pak();
            xcase PIG_WLIST:
                strcpy(out, argv[2]);
                ret = win32listPig(out);
            xcase PIG_EXTRACT:
                strcpy(out, argv[2]);
                ret = extractPig(out);
                pak();
            xdefault:
                printf("pig: You must specify one main mode\n");
                return -1;
        }
        //_CrtMemDumpAllObjectsSince(&g_memstate);
        //after = memMonitorBytesAlloced();
        //printf("Memory before: %I64d after: %I64d\n", before, after);
        //memMonitorDisplayStats();
    }
    //printf("main(): %fs\n", timerElapsed(timer));
    //{int a = _getch(); }
    timerFree(timer);
    return ret;
}
