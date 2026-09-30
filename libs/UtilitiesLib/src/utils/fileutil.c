#include "utilitieslib/utils/fileutil.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/StringTable.h"
#include <string.h>
#include <sys/stat.h>
#include "utilitieslib/utils/FileSystem.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/utils.h"
#include <fcntl.h>
#include <share.h>
#include <direct.h>
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/timing.h"

void fileLoadDataDirs(int forceReload);


char *fileGetcwd(char *buffer, int size)
{
	return _getcwd(buffer, size);
}

int quickload = 0;

static int fileWalkAssets(const char *prefix, FileScanProcessor processor,
	int archive_fast)
{
	FileListing *listing = fileSystemList(file_system, prefix, archive_fast);
	int stop = 0;
	if (!listing) return 0;
	for (size_t i = 0; i < listing->count && !stop; i++) {
		const FileSystemEntry *entry = &listing->entries[i];
		struct _finddata32_t data = { 0 };
		char parent[MAX_PATH];
		FileScanAction action;
		strcpy(parent, entry->path);
		getDirectoryName(parent);
		strcpy(data.name, entry->name);
		data.size = entry->size;
		data.time_access = data.time_create = data.time_write = entry->mtime;
		data.attrib = entry->kind == PG_ENTRY_DIRECTORY ? _A_SUBDIR : _A_NORMAL;
		action = processor(parent, &data);
		if (action & FSA_STOP) stop = 1;
		else if ((action & FSA_EXPLORE_DIRECTORY) &&
			entry->kind == PG_ENTRY_DIRECTORY)
			stop = fileWalkAssets(entry->path, processor, archive_fast);
	}
	fileListingFree(&listing);
	return stop;
}

void fileScanAllDataDirs(const char *dir, FileScanProcessor processor)
{
	if (fileIsAbsolutePath(dir)) {
		fileScanDirRecurseEx(dir, processor);
		return;
	}
	if (!file_system) fileLoadDataDirs(0);
	int archive_fast = quickload &&
		!strnicmp(dir, "texture_library", strlen("texture_library"));
	/* Share one discovery across subfolder scans, as the original dynamic
	 * folder cache did. Lightmaps retain their narrower per-map scope. */
	const char *relative = dir;
	while (*relative == '/' || *relative == '\\' ||
		(relative[0] == '.' && (relative[1] == '/' || relative[1] == '\\')))
		relative += *relative == '.' ? 2 : 1;
	char *subtree = _strdup(relative);
	if (subtree) {
		char *slash = strpbrk(subtree, "/\\");
		if (slash && (slash - subtree != 9 || strnicmp(subtree, "lightmaps", 9)))
			*slash = 0;
		fileSystemCacheTree(file_system, subtree, archive_fast);
		free(subtree);
	}
	fileWalkAssets(dir, processor, archive_fast);
}

static char *  //returns the long format of file/folder name alone (not the preceeding path)
makeLongName(char *anyName, char *name, size_t name_size)//accepts full path, can be long or short
{
    char full[MAX_PATH];
    DWORD length = GetLongPathNameA(anyName, full, sizeof(full));

    if (length && length < sizeof(full))
    {
        const char *base = strrchr(full, '\\');
        const char *slash = strrchr(full, '/');
        if (!base || (slash && slash > base)) base = slash;
        strcpy_s(name, name_size, base ? base + 1 : full);
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

