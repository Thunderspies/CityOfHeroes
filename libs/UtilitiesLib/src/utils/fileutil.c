#include "utilitieslib/utils/fileutil.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/StringTable.h"
#include <string.h>
#include <sys/stat.h>
#include "utilitieslib/utils/FolderCache.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/utils.h"
#include <fcntl.h>
#include <share.h>
#include <direct.h>
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/timing.h"

void fileLoadDataDirs(int forceReload);


//////////////////////////////////////////////////////////////////////////
// Old version of fileScanAllDataDirs, only used when FolderCache is disabled
//  (shouldn't actually ever be used anymore)

// FIXME!!!
// Probably not such a great idea to grab an extern this way.
extern StringTable gameDataDirs;

static StashTable processedFiles = 0;
static char* curRootPath;
static int curRootPathLength;
static FileScanProcessor scanAllDataDirsProcessor;
static char* scanTargetDir;

char * fileGetcwd(char * _DstBuf, int _SizeInBytes)
{
    return _getcwd( _DstBuf, _SizeInBytes );
}

static void old_fileScanAllDataDirsRecurseHelper(char* relRootPath){
    struct _finddata32_t fileinfo;
    int    handle,test;
    char buffer[1024];
    FileScanAction action = FSA_EXPLORE_DIRECTORY;

    sprintf_s(SAFESTR(buffer), "%s/*", relRootPath);
    
    for(test = handle = _findfirst32(buffer, &fileinfo);test >= 0; test = _findnext32(handle, &fileinfo)){
        if(fileinfo.name[0] == '.')
            continue;

        if(FolderCacheMatchesIgnorePrefixAnywhere(fileinfo.name))
            continue;
        
        // Check if the file has already been processed before.
        //    Construct the relative path name to the file.
        sprintf_s(SAFESTR(buffer), "%s/%s", relRootPath + curRootPathLength, fileinfo.name);
        
        if(fileinfo.attrib & _A_SUBDIR){
            // Send directories to the processor directory without caching the name.
            // We do not want to prevent duplicate directories from being explored,
            // only duplicate files.
            action = scanAllDataDirsProcessor(relRootPath, &fileinfo);
        } else {
            //    Check if the path exists in the table of processed files.
            if(!stashFindPointerReturnPointer(processedFiles, buffer)){
                // The file has not been processed.  
                // Process it and then add it to the processed files table.
                action = scanAllDataDirsProcessor(relRootPath, &fileinfo);

                stashAddInt(processedFiles, buffer, 1, false);
            }
        }

        

        if(    action & FSA_EXPLORE_DIRECTORY && 
            fileinfo.attrib & _A_SUBDIR){

            sprintf_s(SAFESTR(buffer), "%s/%s", relRootPath, fileinfo.name);
            old_fileScanAllDataDirsRecurseHelper(buffer);
        }

        if(action & FSA_STOP)
            break;

        
    }
    _findclose(handle);
}

static int old_fileScanAllDataDirsHelper(char* rootPath){
    char buffer[1024];

    sprintf_s(SAFESTR(buffer), "%s/%s", rootPath, scanTargetDir);
    curRootPath = buffer;
    curRootPathLength = (int)strlen(buffer);
    old_fileScanAllDataDirsRecurseHelper(buffer);

    // Always continue through the string table.
    return 1;
}


void old_fileScanAllDataDirs(char* dir, FileScanProcessor processor){
    FileScanProcessor old = scanAllDataDirsProcessor; // Push on the stack
    scanTargetDir = dir;

    //TODO: make this just scan the FolderCache

    // Create or clear the cache of files that has been seen by the
    // directory scanner.
    if(!processedFiles){
        processedFiles = stashTableCreateWithStringKeys(32, StashDeepCopyKeys);
    }else{
        stashTableClear(processedFiles);
    }
    
    scanAllDataDirsProcessor = processor;

    // Scan all known data directories.
    strTableForEachString(gameDataDirs, old_fileScanAllDataDirsHelper);
    scanAllDataDirsProcessor = old; // Pop off the stack
}



//////////////////////////////////////////////////////////////////////////
// New (folder cache) version

extern FolderCache *folder_cache;
static int devel_dynamic=0;

// Converts a FolderNode into a _finddata32_t in order to work with the old style of fileScanAllDataDirs
static int fileScanAllDataDirsHelper(const char *dir, FolderNode *node, void *userdata) {
    struct _finddata32_t fileInfo;
    char filename[MAX_PATH];
    FileScanProcessor proc = (FileScanProcessor)userdata;
    FileScanAction action = FSA_EXPLORE_DIRECTORY;

    // check for ignore prefixes (such as v_ and d_)
    if (FolderCacheMatchesIgnorePrefixAnywhere(dir))
        return FSA_NO_EXPLORE_DIRECTORY;

    if (node->is_dir) {
        fileInfo.attrib = _A_SUBDIR;
    } else {
        if (devel_dynamic && !node->seen_in_fs) // This should catch files only in pigs when doing the development_dynamic mode
            return FSA_NO_EXPLORE_DIRECTORY;
        fileInfo.attrib = _A_NORMAL;
    }

    strcpy(filename, dir);    
    strcpy(fileInfo.name, getFileName(filename));
    fileInfo.size = (int)node->size;
    fileInfo.time_access = fileInfo.time_create = fileInfo.time_write = node->timestamp;

    action = proc(getDirectoryName(filename), &fileInfo);

    return action;
}

int quickload = 0;

static CRITICAL_SECTION fileScanAllDataDirs_critsec;
static bool fileScanAllDataDirs_critsec_inited=false;

static char *good_dirs = "texture_library tricks fx sound sequencers object_library"; // Well-behaved directories that do not look outside if their own folder
static char *quickload_dirs = "texture_library";

void fileScanAllDataDirs(const char* _dir, FileScanProcessor processor){
    FolderNode *node;
    char dir[MAX_PATH];
    bool doing_quickload=false;

    if (!fileScanAllDataDirs_critsec_inited) {
        // to protect devel_dynamic global
        InitializeCriticalSection(&fileScanAllDataDirs_critsec);
        fileScanAllDataDirs_critsec_inited = true;
    }
    EnterCriticalSection(&fileScanAllDataDirs_critsec);

    if (fileDataDir()==NULL) 
        fileLoadDataDirs(0);

    strcpy(dir, _dir);

    if (fileIsAbsolutePath(dir))
    {
        if(!processedFiles){
            processedFiles = stashTableCreateWithStringKeys(32, StashDeepCopyKeys);
        }else{
            stashTableClear(processedFiles);
        }
        scanAllDataDirsProcessor = processor;
        curRootPath = dir;
        curRootPathLength = (int)strlen(dir);
        old_fileScanAllDataDirsRecurseHelper(dir);
        LeaveCriticalSection(&fileScanAllDataDirs_critsec);
        return;
    }

    // This just scans the FolderCache
    if (!folder_cache || !folder_cache->root) {
        // This should only happen in utils, not in the game/mapserver!
        //printf("Warning, no FolderCache, using file system to scan '%s' instead.\n", dir);
        old_fileScanAllDataDirs(dir, processor);
        LeaveCriticalSection(&fileScanAllDataDirs_critsec);
        return;
    }
    forwardSlashes(dir);
    if (dir[strlen(dir)-1]=='/') {
        dir[strlen(dir)-1]='\0';
    }
    if (quickload && strstr(quickload_dirs, _dir)!=0) {
        // Do not scan the tree, just rely on the pigs!
        FolderCacheModePush(FOLDER_CACHE_MODE_I_LIKE_PIGS);
        doing_quickload = true;
    } else {
        // Scan the directory so that the whole tree is in RAM
        // Truncate to the parent folder (so we don't scan the same folders more than once
        char *relpath = dir, *s, path[MAX_PATH];
        while (relpath[0]=='/') relpath++;
        strcpy(path, relpath);
        relpath = path;
        if (!strStartsWith(relpath, "lightmaps")) {
            if (s=strchr(relpath, '/')) {
                *s=0;
            }
        }
        FolderCacheRequestTree(folder_cache, relpath); // If we're in dynamic mode, this will load the tree!
    }
    if (FolderCacheGetMode()==FOLDER_CACHE_MODE_DEVELOPMENT_DYNAMIC)
        devel_dynamic = 1;
    if (!doing_quickload && strstr(good_dirs, _dir)!=0) {
        FolderCacheModePush(FOLDER_CACHE_MODE_DEVELOPMENT); // Another hack for dynamic mode
    }
    node = FolderNodeFind(folder_cache->root, dir);
    //printf("scanning '%s'", dir);
    // This will happen if we're running  ParserLoadFiles on a folder that's been .bined and the source folder doesn't exist (i.e. production version)
    if (!node) {
        //assert(!"Someone passed a bad path to fileScanallDataDirs, it doesn't exist (check your Game Data Dir)!");
        if (FolderCacheGetMode()==FOLDER_CACHE_MODE_DEVELOPMENT_DYNAMIC ||
            FolderCacheGetMode()==FOLDER_CACHE_MODE_DEVELOPMENT)
        {
            printf("Warning: Someone passed a bad path (%s) to fileScanallDataDirs, it doesn't exist (check your Game Data Dir)!", _dir);
        }
        // Restore old modes
        if (!doing_quickload && strstr(good_dirs, _dir)!=0)
            FolderCacheModePop();
        if (doing_quickload)
            FolderCacheModePop();
        LeaveCriticalSection(&fileScanAllDataDirs_critsec);
        return;
    }
    if (!node->contents) {
        // Scanning a folder with nothing in it?
    } else {
        FolderNodeRecurseEx(node->contents, fileScanAllDataDirsHelper, processor, dir);
    }
    devel_dynamic = 0;
    // Restore old modes
    if (!doing_quickload && strstr(good_dirs, _dir)!=0)
        FolderCacheModePop();
    if (doing_quickload)
        FolderCacheModePop();
    LeaveCriticalSection(&fileScanAllDataDirs_critsec);
}







static char *  //returns the long format of file/folder name alone (not the preceeding path)
makeLongName(char *anyName, char *name, size_t name_size)//accepts full path, can be long or short
{
    struct _finddata32_t fileinfo;
    //static char name[MAX_PATH];
    intptr_t handle = _findfirst32(anyName,&fileinfo);

    if(handle>0)
    {
        strcpy_s(name, name_size, fileinfo.name);
        _findclose(handle);
        return name;
        /*If you want to convert long names to short ones, 
        you can return fd.cAlternateFileName */
    }
    else
    {
        // error or file not found, leave it be
        if (strrchr(anyName, '/')) {
            return strrchr(anyName, '/')>strrchr(anyName, '\\')?(strrchr(anyName, '/')+1):(strrchr(anyName, '\\')+1);
        } else if (strrchr(anyName, '\\')) {
            return strrchr(anyName, '\\')+1;
        } else {
            return anyName;
        }

    }
}



char * //returns the complete pathname in long format.
// pass a buffer to be filled in as sout.  this is also what is returned by the function.
makeLongPathName_safe(char * anyPath, char *sout, int sout_size) //accepts both UNC and traditional paths, both long and short
{
    //char sout[MAX_PATH]={0};
    static char long_name[MAX_PATH];
    char temp[MAX_PATH];
    char *token;
    char *strtokparam=NULL;
    assert(sout_size>0);
    sout[0]=0;
    if (strncmp(anyPath,"\\\\",2)==0) {
        if ( sout_size > 2 ) {
            strcat_s(sout, sout_size, "//");
        }
    }
    token = strtok_r(anyPath, "\\/", &strtokparam);
    while (token!=NULL) {
        strcpy(temp, sout);
        strcat(temp, token);
        token = strtok_r(NULL, "\\/", &strtokparam);
        if (strlen(temp)<=2) { // X: or . or ..
            if ( sout_size > (int)strlen(sout) + (int)strlen(temp) )
                strcpy_s(sout, sout_size, temp);
        } else {
            char *toappend = makeLongName(temp, SAFESTR(long_name));
            if ( sout_size > (int)strlen(sout) + (int)strlen(toappend) )
                strcat_s(sout, sout_size, toappend);
        }
        if (token!=NULL) {
            if ( sout_size > (int)strlen(sout) + 1 )
                strcat_s(sout, sout_size, "/");
        }
    }
    // Remove all /./, since they are redundant
    while (token = strstr(sout, "/./")) {
        strcpy_unsafe(token, token + 2);
    }
    return sout;
}

// for communication to DateCheckCallback
const char* g_filemask;
static __time32_t g_lasttime;

// store latest time in g_lasttime
static FileScanAction DateCheckCallback(char* dir, struct _finddata32_t* data)
{
    int len, masklen;
    int isok = 0;

    if (data->attrib & _A_SUBDIR) {
        // If it's a subdirectory, we don't care about checking times, etc
        if (data->name[0]=='_') {
            return FSA_NO_EXPLORE_DIRECTORY;
        } else {
            return FSA_EXPLORE_DIRECTORY;
        }
    }

    masklen = (int)strlen(g_filemask);
    len = (int)strlen(data->name);
    if (!masklen) isok = 1;
    else if( len > masklen && !stricmp(g_filemask, data->name+len-masklen) )
        isok = 1;

    if(!strstr(dir, "/_") && !(data->name[0] == '_') && isok ) // This strstr probably isn't needed because we check directories above ?
    {
        if (data->time_write > g_lasttime) {
            g_lasttime = data->time_write;
        }
    }
    return FSA_EXPLORE_DIRECTORY;
}

// comes from old textparser.ParserIsPersistNewer
int IsFileNewerThanDir(const char* dir, const char* filemask, const char* persistfile)
{
    __time32_t lasttime;
    g_filemask = filemask;

    // check dates
    lasttime = fileLastChanged(persistfile);
    if (lasttime <= 0) // file doesn't exist
        return 0;

    if (dir)
    {
        g_lasttime = lasttime;
        fileScanAllDataDirs(dir, DateCheckCallback);

        if (g_lasttime == lasttime)
            return 1; // no files later
        else return 0;
    }
    else
    {
        __time32_t text_lasttime = fileLastChanged(filemask);
        if (text_lasttime<=0) {
            return 1;    // couldn't find text file, use bin
        }
        if (text_lasttime > lasttime)
            return 0; // text file newer
        else return 1; // bin file newer
    }
}

//Checks to see if a file has been updated.
//Also updates the age that this was last checked
int fileHasBeenUpdated(const char * fname, int *age)
{
    int    time;

    if(fname && fname[0] && fname[0] != '0')
    {
        time = fileLastChanged(fname);

        if(time > *age)
        {
            *age = time;
            return time;
        }    
    }
    return 0;
}

int fileLineCount(const char *fname)
{
    FILE *file;
    char buf[65536]; // this makes a certain assumption about the line length
    int ret = 0;
    char *c;

    file = fileOpen(fname, "r");
    if(file == NULL)
        return 0;

    while(c = fgets(buf, ARRAY_SIZE(buf), file)) // assumes error == eof with no more text
        ret++;

    fileClose(file);
    return ret;
}

// Not using the one in file.c because it calls FolderCache stuff which might crash if memory is corrupted
static int fexist(const char *fname)
{
    struct stat status;
    if(!stat(fname, &status)){
        if(status.st_mode & _S_IFREG)
            return 1;
    }
    return 0;
}

static char *findZip()
{
    static char *zip_locs[] = {"zip.exe","bin\\zip.exe","src\\util\\zip.exe","\\game\\tools\\util\\zip.exe","\\bin\\zip.exe"};
    static char *ziploc=NULL;
    if (!ziploc) {
        int i;
        for (i=0; i<ARRAY_SIZE(zip_locs); i++) {
            if (fexist(zip_locs[i])) {
                ziploc = zip_locs[i];
                break;
            }
        }
    }
    if (!ziploc) {
        ziploc = "zip"; // Hope it's in the path
    }
    return ziploc;
}

char *findUserDump()
{
    static char *zip_locs[] = {"userdump.exe","bin\\userdump.exe","src\\util\\userdump.exe","\\game\\tools\\util\\userdump.exe","\\bin\\userdump.exe"};
    static char *ziploc=NULL;
    if (!ziploc) {
        int i;
        for (i=0; i<ARRAY_SIZE(zip_locs); i++) {
            if (fexist(zip_locs[i])) {
                ziploc = zip_locs[i];
                break;
            }
        }
    }
    if (!ziploc) {
        ziploc = "userdump"; // Hope it's in the path
    }
    return ziploc;
}

static char *findGZip()
{
    static char *zip_locs[] = {"gzip.exe","bin\\gzip.exe","src\\util\\gzip.exe","\\game\\tools\\util\\gzip.exe","\\bin\\gzip.exe"};
    static char *ziploc=NULL;
    if (!ziploc) {
        int i;
        for (i=0; i<ARRAY_SIZE(zip_locs); i++) {
            if (fexist(zip_locs[i])) {
                ziploc = zip_locs[i];
                break;
            }
        }
    }
    if (!ziploc) {
        ziploc = "gzip"; // Hope it's in the path
    }
    return ziploc;
}


static void fileZipHelper(const char *filename, const char *exename, char *extension)
{
    char backupName[MAX_PATH];
    char zipName[MAX_PATH];
    char command[1024];

    // Backup old file
    strcpy(zipName, filename);
    strcat(zipName, extension);
    if (fexist(zipName)) {
        strcpy(backupName, filename);
        strcat(backupName, extension);
        strcat(backupName, ".bak");
        if (fexist(backupName)) {
            fileForceRemove(backupName);
        }
        rename(zipName, backupName);
    }
    sprintf_s(SAFESTR(command), "%s \"%s\"", exename, filename);
#if _XBOX
    // can't use system on xbox
    assert( 0 );
#else
    system(command);
#endif
}

void fileGZip(const char *filename)
{
    fileZipHelper(filename, findGZip(), ".gz");
}

void fileGunZip(const char *filename)
{
#if _XBOX
    // can't use system on xbox
    assert( 0 );
#else
    char gunzip[MAX_PATH];
    sprintf_s(SAFESTR(gunzip), "%s -d ", findGZip());
    
    if( fileExists( filename ))
    {
        strcat(gunzip, filename);
        system(gunzip);
    }
#endif
}


void fileZip(const char *filename)
{
    fileZipHelper(filename, findZip(), ".zip");
}

bool fileCanGetExclusiveAccess(const char *filename)
{
    static bool file_didnt_exist=0; // For debugging
    int handle;
    char fullpath[MAX_PATH];
    file_didnt_exist = false;
    fileLocateWrite(filename, fullpath);
    if (_sopen_s(&handle, fullpath, _O_RDONLY, _SH_DENYRW, _S_IREAD) ) {
        if (!fileExists(filename)) {
            file_didnt_exist=true;
            return false;
        }
        return false;
    }
    _close(handle); 
    return true;
}

void fileWaitForExclusiveAccess(const char *filename)
{
    static int file_didnt_exist=0; // For debugging
    int handle;
    char fullpath[MAX_PATH];
    file_didnt_exist = false;
    fileLocateWrite(filename, fullpath);
    while (_sopen_s(&handle, fullpath, _O_RDONLY, _SH_DENYRW, _S_IREAD)) {
        if (!fileExists(filename)) {
            if (file_didnt_exist==16)
                return;
            // Try again, it may have been renamed to .bak in order to update it or something
            file_didnt_exist++;
        }
        Sleep(1);
    }
    _close(handle); 
}

char *pwd(void)
{
    static char path[MAX_PATH];
    fileGetcwd(path, ARRAY_SIZE(path));
    return path;
}


bool fileAttemptNetworkReconnect(const char *filename)
{
#ifndef _XBOX
    char old_path[MAX_PATH];
    unsigned char drive[5];
    int ret;
    int olddrive;
    int newdrive;
    if (filename[1]!=':')
        return false;
    strncpy_s(drive, ARRAY_SIZE(drive), filename, ARRAY_SIZE(drive)-1);
    backSlashes(drive);
    olddrive = _getdrive();
    fileGetcwd(old_path, ARRAY_SIZE(old_path)-1);
    ret = _chdrive(toupper(drive[0]) - 'A' + 1);
    if (ret!=0)
        return false;
    newdrive = _getdrive();
    _chdrive(olddrive);
    _chdir(old_path);
    return olddrive != newdrive;
#else
    return false;
#endif
}

