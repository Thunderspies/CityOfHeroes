/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 */
#include <stdlib.h>
#include "utilitieslib/network/crypt.h"
#include <string.h>
#include <sys/utime.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/locking.h>
#include <io.h>
#include <fcntl.h>
#include <direct.h>
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/utils.h"
#include <stdarg.h>
#include "utilitieslib/components/StringTable.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/utils/fileutil.h"
#include "utilitieslib/utils/piglib.h"
#include "utilitieslib/utils/PigFileWrapper.h"
#include "utilitieslib/utils/FolderCache.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/utils/winfiletime.h"
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/utils/osdependent.h"
#include "utilitieslib/utils/memlog.h"
#include "utilitieslib/utils/StringUtil.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/fileWatch.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/components/StringCache.h"
#include "utilitieslib/utils/MathUtil.h"
#include <share.h>
#include "utilitieslib/UtilsNew/lock.h"
#include "zlib/zlib.h"

#ifdef FW_TRACK_ALLOCATION
#include "Stackwalk.h"
#endif

// A table of possible strings to try when using relative paths.
StringTable gameDataDirs = 0; // now used only inside of fileLoadDataDirs()
static int loadedGameDataDirs = 0;
static int addSearchPath = 1;
static char* mainGameDataDir = NULL;
static char *piggDir = "./piggs";
FolderCache *folder_cache=NULL;
char *gameDataDirOverride = NULL;

static int file_disable_winio=0;

// Vista by default disallows writing to the root of the C: drive, so we reroute
// all such file accesses to one of our own folders.
void moveFromCRoot(char* path) {
    // TODO: We should eventually make this not specific to Vista, and we should
    //  make sure that we don't write to the root of users' C: drives as well.
    if (IsUsingVistaOrLater() && isDevelopmentMode()) {
        static const char C_ROOT[] = "C:\\";
        static const char C_ROOT_GAME[] = "C:\\cryptic\\";
        static const char C_ROOT_REPLACE[] = "C:\\cryptic\\scratch\\";
        static const int C_ROOT_LEN = sizeof(C_ROOT) - 1;
        static int CHARS_TO_ADD = sizeof(C_ROOT_REPLACE) - 1;

        char fullPath[MAX_PATH];
        char* name;

        int fullPathLength = GetFullPathNameA(path, MAX_PATH, fullPath, &name);

        // Assume that any very long paths are not in C:\.
        if (!fullPathLength || fullPathLength + CHARS_TO_ADD >= MAX_PATH)
            return;

        if (!name)
            name = fullPath + fullPathLength;

        if (name - fullPath == C_ROOT_LEN && !strnicmp(fullPath, C_ROOT, C_ROOT_LEN)) {
            if (!CreateDirectoryA(C_ROOT_GAME, NULL) &&
                    GetLastError() != ERROR_ALREADY_EXISTS) {
                Errorf("Could not create folder %s\n", C_ROOT_GAME);
            }
            if (!CreateDirectoryA(C_ROOT_REPLACE, NULL) &&
                    GetLastError() != ERROR_ALREADY_EXISTS) {
                Errorf("Could not create folder %s\n", C_ROOT_REPLACE);
            }
            sprintf_s(path, MAX_PATH, "%s%s", C_ROOT_REPLACE, name);
        }
    }
}

void fileLoadGameDataDir(int load){
    loadedGameDataDirs = !load;
    addSearchPath = load;
}

bool fileIsInsideGameDataDir( const char* aPath )
{
    int i;
    size_t nTestLen = strlen(aPath);
    for ( i=0; i < strTableGetStringCount(gameDataDirs); i++ )
    {
        const char *dataDir = strTableGetString(gameDataDirs, i);
        if (( strnicmp( aPath, dataDir, nTestLen ) == 0 ) &&
            (( dataDir[nTestLen] == '\0' ) ||
             ( dataDir[nTestLen] == '\\' ) ||
             ( dataDir[nTestLen] == '/' )))
        {
            return true;
        }
    }
    return false;
}

void fileAddGameDataDir( const char* aPath )
{
    if ( ! fileIsInsideGameDataDir( aPath ) )
    {
        int dirCount = strTableGetStringCount(gameDataDirs);
        strTableAddString( gameDataDirs, aPath );
        FolderCacheAddFolder(folder_cache, aPath, dirCount );
    }
}

static void initGameDataDirTable()
{
    if (!gameDataDirs)
    {
        gameDataDirs = createStringTable();
        initStringTable(gameDataDirs, 1024);
        strTableSetMode(gameDataDirs, Indexable);//mm
    }
}

int fileAddSearchPath(const char* path){
    if(!dirExists(path))
        return 0;

    initGameDataDirTable();

    // The last seen game data directory will be used as the
    // "main" game data directory.  All other directories will
    // be considered "local".
    mainGameDataDir = (char*)strTableAddString(gameDataDirs, path);

    return 1;
}

int fileSearchPathCount()
{
    // The string table is able to return a count if the "gameDataDirs" is
    // in "indexable" mode.
    if (!gameDataDirs)
        return 0;
    return strTableGetStringCount(gameDataDirs);
}


bool addAppropriateDataDirs(const char *path) // Input is like "C:/game"
{
    static char fn[MAX_PATH+20];
    sprintf_s(SAFESTR(fn), "%s/data", path);
    if (!dirExists(fn)) {
        return false;
    }
    sprintf_s(SAFESTR(fn), "%s/tools", path);
    if (!dirExists(fn)) {
        return false;
    }
    // Found it!
    verbose_printf("AutoDataDir: Found relative data folder %s/data\n", path);

    sprintf_s(SAFESTR(fn), "%s/localdata", path); //This will be tried first
    if (dirExists(fn))
        fileAddSearchPath(fn);
    sprintf_s(SAFESTR(fn), "%s/serverdata", path); //This is the backup
    if (dirExists(fn))
        fileAddSearchPath(fn);

    sprintf_s(SAFESTR(fn), "%s/data", path);
    gameDataDirOverride = fn;
    return true;
}

// Recursively searches up from a given path for a likely data dir
static bool searchForDataDir(char *path)
{
    const char* slashBin = "/bin";
    forwardSlashes(path);
    while (true) {
        char *s;
        if (addAppropriateDataDirs(path)) {
            return true;
        }
        if (strEndsWith(path, slashBin)){
            path[strlen(path) - strlen(slashBin)] = 0;

            if(s = strrchr(path, '/')){
                char buf[MAX_PATH];
                char *fixPtr;

                s++;

                if(!stricmp(s, "coh")){
                    // If path has "/coh/bin/" in it, use c:/game.
                    // If path has "*fix*/coh/bin/" in it, use c:/gamefix.
                    if (strstri(path, "hot"))
                    {
                        sprintf_s(SAFESTR(buf), "c:/hotfix");
                    }
                    else if (fixPtr = strstri(path, "fix"))
                    {
                        sprintf_s(SAFESTR(buf), "c:/gamefix%c", 
                            (fixPtr[3] != '\\' && fixPtr[3] != '/' ? fixPtr[3] : '\0'));
                    }
                    else if (strstri(path, "future"))
                    {
                        sprintf_s(SAFESTR(buf), "c:/gamefuture");
                    }
                    else
                    {
                        sprintf_s(SAFESTR(buf), "c:/game");
                    }

                    if(addAppropriateDataDirs(buf)){
                        return true;
                    }
                }
                else
                {
                    // If path has "/*/bin/" in it, use c:/*.
                    if (fixPtr = strstri(path, "fix"))
                    {
                        sprintf_s(SAFESTR(buf), "c:/%s%s%c", s, "fix",
                            (fixPtr[3] != '\\' && fixPtr[3] != '/' ? fixPtr[3] : '\0'));
                    }
                    else if (strstri(path, "future"))
                    {
                        sprintf_s(SAFESTR(buf), "c:/%s%s", s, "future");
                    }
                    else
                    {
                        sprintf_s(SAFESTR(buf), "c:/%s", s);
                    }

                    if(addAppropriateDataDirs(buf)){
                        return true;
                    }
                }
            }

            path[strlen(path)] = '/';
        }
        if (s = strrchr(path, '/')) {
            *s = '\0';
        } else {
            break;
        }
    }
    return false;
}

bool file_disable_auto_data_dir=false;
void fileDisableAutoDataDir(void)
{
    file_disable_auto_data_dir = true;
}

// Rules:
//  1) gamedatadir.txt in current, non-root, folder overrides everything
//  2) look for a data/ folder AND tools/ folder in one of our parent folders (only for art tools)
//  3) look for a data/ folder in one of the partent folders of where
//        this .exe is located (general case for art/design)
//  4) use hard-coded programmer lookup table
//  5) use old c:\gamedatadir.txt (should no longer be used)
static void findAutoDataDir(bool searchByPath)
{
#ifndef _XBOX
    char path[MAX_PATH]="";
    if (isProductionMode() || file_disable_auto_data_dir)
        return;
    getcwd(path, ARRAY_SIZE(path)-2);
    if (fileExists("./gamedatadir.txt") && !strEndsWith(path, ":\\")) {
        // Load normally
        verbose_printf("AutoDataDir: Using ./gamedatadir.txt in current folder\n");
    } else {
        bool foundIt=false;
        // Try to find data dir via. current path
        if (searchByPath) {
            foundIt = searchForDataDir(path);
        }
        if (!foundIt) {
            _getcwd(path, MAX_PATH);
            foundIt = searchForDataDir(path);
        }
        if (!foundIt) {
            int i;
            static struct {
                char *src;
                char *data;
            } programmer_paths[] = {
                {"C:/game/code/coh/bin", "C:/game"},
                {"C:/gamefix/code/coh/bin", "C:/gamefix"},
                {"C:/gamefix/code/util", "C:/gamefix"},
                {"C:/game/code/Utilities/bin", "C:/game"},
                {"C:/game/code/*/bin", "C:/%s"},
                {"C:/gamefix/code/*/bin", "C:/%sFix"},
                {"//*/game/code/coh/bin", "C:/game"}, // Running over network via remote debugger
                {"C:/src/coh/bin", "C:/game"},
                {"C:/fix/coh/bin", "C:/gamefix"},
                {"C:/fix/util", "C:/gamefix"},
                {"C:/src/Utilities/bin", "C:/game"},
                {"C:/src/*/bin", "C:/%s"},
                {"C:/fix/*/bin", "C:/%sFix"},
                {"C:/src*/coh/bin", "C:/game"},
                {"C:/src*/util", "C:/game"},
                {"//*/src/coh/bin", "C:/game"}, // Running over network via remote debugger
            };
            strcpy(path, getExecutableName());
            forwardSlashes(path);
            getDirectoryName(path);
            for (i=0; i<ARRAY_SIZE(programmer_paths) && !foundIt; i++) {
                if (simpleMatch(programmer_paths[i].src, path)) {
                    static char buf[MAX_PATH];
                    sprintf_s(SAFESTR(buf), programmer_paths[i].data, getLastMatch());
                    foundIt = addAppropriateDataDirs(buf);
                }
            }
        }
        if (!foundIt) {
            Errorf("Could not find appropriate game data dir");
        }
    }
#else
    if (fileExists("game:/gamedatadir.txt"))
    {
        return;
    }
    addAppropriateDataDirs("game:/");
#endif
}

/* Function fileLoadDataDirs
 *    Loads a list of possible directories from c:\gamedatadir.txt.  It is assumed
 *    that each directory occupies an entire line.  This list is passed to the
 *  FolderCache.  The last directory listed in the file is
 *    assumed to be the main game directory, which will be returned by fileDataDir()
 *    to maintain compatibility for some utility programs.  All other directories
 *    are considered "local".
 *
 *    Note that the environmental variables are now ignored.  While it is possible
 *    to parse a list of directory names that are all concatenated on the same line,
 *    it is not implemented.
 *
 *
 */
void fileLoadDataDirs(int forceReload)
{
    FILE    *file;
    int i;
    char    buffer[1024];
    int productionDataDir = 0;
    int verbose = 0;
    static bool did_auto_data_dir=false;

    if (!did_auto_data_dir && !gameDataDirOverride) {
        did_auto_data_dir = true;
        findAutoDataDir(false);
        if (loadedGameDataDirs)
            return;
    }

    // Forcing reload?
    if(forceReload && gameDataDirs){
        // Destroy all prevously loaded directory names.
        destroyStringTable(gameDataDirs);
        gameDataDirs = 0;
    }

    assert(!loadedGameDataDirs);

    if (!folder_cache) {
        folder_cache = FolderCacheCreate();
    }

    if (gameDataDirOverride)
    {
        size_t    len;
        strcpy(buffer, gameDataDirOverride);

        forwardSlashes(buffer);
        len = strlen(buffer) - 1;
        while (buffer[len]=='/' || buffer[len]==' ' || buffer[len]=='\t') buffer[len--]=0;

        fileAddSearchPath(buffer);
    }
    else
    {
        // Note: we can't just look for the piggs/ folder, since this wreaks havoc on
        // all of our batch files sitting in tools/util/programmer/shortcuts, which
        // has a "piggs" subdirectory under it
        fileAddSearchPath("./piggs"); // Is this needed even? We're not loading any data files from this folder
        productionDataDir |= !fileIsUsingDevData();
        productionDataDir |= fileAddSearchPath("./data"); // If we've gota data/ folder already, we don't want to load the gamedatadir.txt

        // Did we detect some "production" data directories?
        if(!productionDataDir && !file_disable_auto_data_dir)
        {
            // If not, assume that we're in developement mode and
            // try to load a list data directory names from some text file.
#ifndef _XBOX
            file = fopen("./gamedatadir.txt","rt~");

            if (file) // New scheme, look for gamedatadir.txt in current directory
                verbose = 0; // Could set this to 1 if we want a printout about what's going on
            else
                file = fopen("c:/gamedatadir.txt","rt~");
#else
            file = fopen("game:/gamedatadir.txt","rt~");
#endif

            if (file)
            {
                size_t    len;
                int        count = 0;

                // Extract all directory names and add them to the string table.
                while(fgets(buffer,sizeof(buffer),file)){
                    // Reformat the directory name.
                    len = strlen(buffer) - 1;
                    if (len <= 0 || buffer[0] == '#' || buffer[0] == '\n')
                        continue;
                    if (len >= 0 && buffer[len] == '\n')
                        buffer[len--] = 0;

                    forwardSlashes(buffer);
                    while (buffer[len]=='/' || buffer[len]==' ' || buffer[len]=='\t') buffer[len--]=0;

                    fileAddSearchPath(buffer);
                    count++;
                }
                fclose(file);
            }

            // If the game data dir file was not found or if all paths listed
            // in the file is invalid...
            if(!file){
                fileAddSearchPath("./");
            }
        }
        if(!fileSearchPathCount()){
            fileAddSearchPath("./");
        }
    }

    {
        int dirCount;
        initGameDataDirTable();
        dirCount = strTableGetStringCount(gameDataDirs);

        for (i=dirCount-1; i>=0; i--) {
            // Insert in reverse order, since we want the priority to be higher for the
            // first entries in gamedatadir.txt
            if (verbose)
                printf("Using %s as data dir\n", strTableGetString(gameDataDirs, i));
            FolderCacheAddFolder(folder_cache, strTableGetString(gameDataDirs, i), dirCount-1-i);
        }
    }

    loadedGameDataDirs = 1;

    FolderCacheAddAllPigs(folder_cache);

    // Initialize various file related code (non-thread safe inits)
    initPigFileHandles();
}

// fpe 4/23/2011 -- New function to load piggs on the fly as they are downloaded (after game has launched) for lightweight client.
int fileCheckForNewPiggs()
{
    return FolderCacheAddNewPigs(folder_cache);
}

// New scheme: ignore c:\gamedatadir.txt and just using the current directory
//   to figure it out
// This is different than the standard behavior because it uses the working directory
//   instead of the path to the .exe
void fileAutoDataDir(bool bEnableSourceControl)
{
    assert(!loadedGameDataDirs);
    findAutoDataDir(true);
    // Auto failed, or gameDataDirOverride was set above
    fileLoadDataDirs(0);
#ifndef _XBOX
    printf("Using game data dir: %s (%s)\n", mainGameDataDir, "Perforce Disabled");
#endif
}

// Returns the "main" game data directory if multiple are defined
char *fileDataDir()
{
    if (!loadedGameDataDirs)
        fileLoadDataDirs(0);

    return mainGameDataDir;
}

const char *filePiggDir()
{
    return piggDir;
}

void fileSpecialDir(const char *name, char *dest, size_t dest_size)
{
    char        *s;

    strcpy_s(dest, dest_size, fileDataDir());

    s = strrchr(dest,'/');
    if (s) // If not fileDataDir must be "."
        *s = 0;
    strcat_s(dest, dest_size, "/");
    strcat_s(dest, dest_size, name);
    mkdirtree(dest);
}

char *fileTempDir()
{
    static char tmp_dir[200];
    char        *s;

    strcpy(tmp_dir,fileDataDir());

    s = strrchr(tmp_dir,'/');
    if (!s)
        s = strrchr(tmp_dir,'\\');
    *s = 0;
    strcat(tmp_dir, "/tmp");
    return tmp_dir;
}

char *fileTempName()
{
    static char tempFile[MAX_PATH] = {0};
    char tempDir[MAX_PATH];// = fileTempDir();
    if (GetTempPathA(ARRAY_SIZE(tempDir), tempDir))
        GetTempFileNameA(tempDir, "coh", 0, tempFile);
    return tempFile;
}

char *fileLocalDataDir()
{
    static char tmp_dir[200];
    char        *s;

    if (strTableGetStringCount(gameDataDirs) > 1)
    {
        return strTableGetString(gameDataDirs,0);
    }

    strcpy(tmp_dir,fileDataDir());

    s = strrchr(tmp_dir,'/');
    if (!s)
        s = strrchr(tmp_dir,'\\');
    *s = 0;

    strcat(tmp_dir, "/localdata");
    return tmp_dir;
}

char *fileSrcDir()
{
static char tmp_dir[200];
char        *s;

    strcpy(tmp_dir,fileDataDir());
    s = strrchr(tmp_dir,'/');
    if (!s)
        s = strrchr(tmp_dir,'\\');
    *s = 0;
    strcat(tmp_dir, "/src");
    return tmp_dir;
}

char *fileFixSlashes(char *str)
{
char    *s;

    for(s=str;*s;s++)
        if (*s == '/')
            *s = '\\';

    return str;
}

char *fileFixUpName(const char *src,char * tgt)
{
const char    *s;
char    *t;

    for(s=src, t=tgt ; *s ; s++,t++)
    {
        *t = toupper(*s);
        if (*s == '\\')
            *t = '/';
    }
    *t = 0;
    if(src[0] == '/')
        return tgt + 1;
    else
        return tgt;
}

char* fileFixName(char* src, char* tgt)
{
    char    *s;
    char    *t;

    for(s=src, t=tgt ; *s ; s++,t++)
    {
        if (*s == '\\')
            *t = '/';
    }
    *t = 0;
    if(src[0] == '/')
        return tgt + 1;
    else
        return tgt;
}

/* Function fileLocate()
 *    Locates a file with the relative path.  It is possible for the game directory
 *    to exist in multiple locations.  This function searches through all possible
 *    locations in the order specified in c:\gamedatadir.txt for the file specified
 *    by the given relative path.  It will also search in Pig files.  All of this
 *  now goes through the FolderCache module.  If an abosolute path is given, the
 *  path is returned unaltered.
 *
 *
 */
#include <io.h>
#include <time.h>
#include <sys/types.h>
#include <direct.h>

// Pigged paths look like "c:/game/data/piggs/file.pigg:/myfile.txt" or "./piggs/file.pigg:/myfile.txt"
int is_pigged_path(const char *path) {
    if (!path) return 0;
    if (path[0]=='.') {
        return strchr(path, ':')!=0;
    } else {
        return strchr(path, ':')!=strrchr(path, ':');
    }
}

void fileLocateFree(char *data) // Frees a pointer that was returned from fileLocateX()
{
    if (data) {
        free(data);
    }
}

static bool g_force_absolute_paths=false;
void fileAllPathsAbsolute(bool newvalue)
{
    g_force_absolute_paths = newvalue;
}

int fileIsAbsolutePath(const char *path)
{
    if (g_force_absolute_paths)
        return 1;
    if(    path[0] &&
        (    path[1]==':' ||
            path[0]=='.' ||
            (    path[0]=='\\' &&
                path[1]=='\\')))
    {
        return 1;
    }
#ifdef _XBOX
    //Xbox has multi-letter drives
    {
        char *colon = strchr(path,':');
        char *bs = strchr(path,'\\');
        if (colon && (!bs || colon < bs))
        {
            // If the first colon is before the first slash, it's probably a drive
            return 1;
        }
    }
#endif
    return 0;
}

#ifdef _XBOX
static int fileFixupForXBox(char *fname, size_t fname_size, char *dest, size_t dest_size)
{
    if (fname[0] == '.' && (fname[1] == '/' || fname[1] == '\\'))
    {
        char name2[MAX_PATH];
        strcpy(name2, "game:/");
        strcat(name2, fname+2);
        if (dest)
            strcpy_s(dest, dest_size, name2);
        else
            strcpy_s(fname, fname_size, name2);
        return 1;
    }
    if (strStartsWith(fname, "C:\\") || strStartsWith(fname, "C:/"))
    {
        char name2[MAX_PATH];
        strcpy(name2, "game:/");
        strcat(name2, fname+3);
        if (dest)
            strcpy_s(dest, dest_size, name2);
        else
            strcpy_s(fname, fname_size, name2);
        return 1;
    }
    return 0;
}
#endif

static int filelocateread_return_spot=0;
char *fileLocateRead_s(const char *fname, char *dest, size_t dest_size) // Was fileLocateExists
{
    char temp[MAX_PATH];

    PERFINFO_AUTO_START("fileLocateRead", 1);

    //memlog_printf(NULL, "fileLocateRead(%s)", fname);

    if (!loadedGameDataDirs)
        fileLoadDataDirs(0);

    if (!fname || !fname[0]) {
        filelocateread_return_spot = 1;
        PERFINFO_AUTO_STOP();
        return NULL;
    }

    if (fname[1]==':' && !g_force_absolute_paths) {
        strcpy(temp, fname);
        forwardSlashes(temp);
        fname = temp;

        if (strnicmp(fname, fileDataDir(), strlen(fileDataDir()))==0) {
            // This should only happen in utilities, not in the game/mapserver
            // This could just as well be a relative path
            fname = fname + strlen(fileDataDir());
        } else { // Absolute path outside of game data dir
        }
    }

#ifdef _XBOX
    if (fileFixupForXBox((char *)fname, 0, temp, ARRAY_SIZE_CHECKED(temp)))
        fname = temp;
#endif

    // Is the given filename still an absolute path?
    //    Consider relative paths of the form "./bla bla" and "../blabla"
    //    "absolute" paths also.  In these cases, the filename is being explicitly
    //    specified and does not need "gameDataDirs" to resolve it into a real filename.
    if (fileIsAbsolutePath(fname))
    {
        //if (mainGameDataDir[1] != ':')
        //    fname += 3;
        if (dest) {
            strcpy_s(dest, dest_size, fname);
            filelocateread_return_spot = 2;
            PERFINFO_AUTO_STOP();
            return dest;
        }
        dest = strdup(fname);
        filelocateread_return_spot = 3;
    }
    else
    {
        // filename is a relative path
        FolderNode * node;

        PERFINFO_AUTO_START("FolderCacheQuery", 1);
            node = FolderCacheQuery(folder_cache, fname);
        PERFINFO_AUTO_STOP();

        if (node==NULL) { // not found in filesystem or pigs
            filelocateread_return_spot = 4;
            dest = NULL;
        }
        else if (dest) {
            filelocateread_return_spot = 5;
            PERFINFO_AUTO_START("FolderCacheGetRealPath1", 1);
                FolderCacheGetRealPath(folder_cache, node, dest, dest_size);
            PERFINFO_AUTO_STOP();
        }
        else{
            filelocateread_return_spot = 6;
            PERFINFO_AUTO_START("FolderCacheGetRealPath2", 1);
                dest = strdup(FolderCacheGetRealPath(folder_cache, node, SAFESTR(temp)));
            PERFINFO_AUTO_STOP();
        }
    }
    PERFINFO_AUTO_STOP();
    return dest;
}

// Will always return a path into the filesystem, *never* into a pig file
char *fileLocateWrite_s(const char *_fname, char *dest, size_t dest_size)
{
    char *path;
    size_t path_size;
    char fname[MAX_PATH];
    char *fname_noslash;

    if(!_fname || !_fname[0]){
        return NULL;
    }

    PERFINFO_AUTO_START("fileLocateWrite", 1);

    Strncpyt(fname, _fname); // in case _fname==dest

    path = fileLocateRead_s(fname, NULL, 0);

    if (path) {
        if (is_pigged_path(path)) {
            // We got a pig file on our hands!
            // fall through to the line below where it creates a path relative to mainGameDataDir
            fileLocateFree(path);
            path=NULL;
        } else {
            // Found an on-disk path (c:\game\data\..., c:\game\serverdata\..., etc)
            if (dest) {
                strcpy_s(dest, dest_size, path);
                fileLocateFree(path);
                PERFINFO_AUTO_STOP();
                return dest;
            }
            PERFINFO_AUTO_STOP();
            return path;
        }
    }

    // Relative path not found, make a path
    if (fileIsAbsolutePath(fname)) {
        // Absolute path, just return it
        if (dest) {
            strcpy_s(dest, dest_size, fname);
            PERFINFO_AUTO_STOP();
            return dest;
        }
        dest = strdup(fname);
        PERFINFO_AUTO_STOP();
        return dest;
    }
    fname_noslash = fname;
    if (fname_noslash[0]=='/') {
        fname_noslash++;
    }
    if (dest) {
        path_size = dest_size;
        path = dest;
    } else {
        path_size = strlen(mainGameDataDir) + strlen(fname_noslash) + 2;
        path = malloc(path_size);
    }
    sprintf_s(path, path_size, "%s/%s", mainGameDataDir, fname_noslash);
    PERFINFO_AUTO_STOP();
    return path;
}

FILE *fileOpen(const char *fname,char *how)
{
    FILE*    file = NULL;
    char*    realFilename;
    char    buf[MAX_PATH];

    PERFINFO_AUTO_START("fileOpen", 1);

        if (strcspn(how, "wWaA+~")!=strlen(how)) {
            realFilename = fileLocateWrite(fname, buf);
        } else {
            realFilename = fileLocateRead(fname, buf);
        }

        if(realFilename && realFilename[0])
        {
            file = fopen(realFilename, how);
        }

    PERFINFO_AUTO_STOP();

    return file;
}

/* Function fileOpenEx
 *    Constructs a relative filename using the given formating information
 *    before attempting to locate and open the file.
 *
 *    Users no longer have to manually construct the relative path before
 *    calling fileOpen.
 */
FILE *fileOpenEx(const char *fnameFormat, char *how, ...)
{
va_list va;
char buf[1024];

    // Construct a relative path according to the given filename format.
    va_start(va, how);
    vsprintf(buf, fnameFormat, va);
    va_end(va);

    return fileOpen(buf, how);
}

FILE *fileOpenTemp(const char *fname,char *how)
{
char    buf[1000];

    sprintf_s(SAFESTR(buf),"%s/%s",fileTempDir(),fname);
    return fopen(buf,how);
}

FILE *fileOpenStuffBuff(StuffBuff *sb)
{
    // Uh, this is a giant hack, fopen expects a string, so I'm passing
    //  it a 1 char string followed by the data I really want to get in there...
    struct {
        char dummy[8];
        StuffBuff *sb;
    } dummy = { "X", sb };
    return fopen((char*)&dummy, "StuffBuff");
}

void fileFree(void *mem)
{
    free(mem);
}

static int filealloc_failure_spot=0;
void *fileAlloc_dbg(const char *fname,int *lenp MEM_DBG_PARMS)
{
    FILE    *file;
    intptr_t    total=0,bytes_read;
    char    *mem=0,*located_name, buf[MAX_PATH];

    PERFINFO_AUTO_START("fileAlloc", 1);
        PERFINFO_AUTO_START("fileLocateRead", 1);
            located_name = fileLocateRead(fname, buf);
            if (!located_name)
            {
                filealloc_failure_spot = 1;
                PERFINFO_AUTO_STOP();
                PERFINFO_AUTO_STOP();
                return 0;
            }

        PERFINFO_AUTO_STOP_START("fileSize", 1);

            if (is_pigged_path(located_name))
                total = fileSize(fname);
            else
                total = fileSize(located_name);

            if (total == -1)
            {
                filealloc_failure_spot = 2;
                PERFINFO_AUTO_STOP();
                PERFINFO_AUTO_STOP();
                return 0;
            }

        PERFINFO_AUTO_STOP_START("fopen", 1);

            file = fopen(located_name,"rb");

        PERFINFO_AUTO_STOP();

        located_name[0]=0; // clear locatebuff just to be safe
        if (!file)
        {
            filealloc_failure_spot = 3;
            PERFINFO_AUTO_STOP();
            return 0;
        }
        assert(-1!=total);

        mem = smalloc(total+1);
        assert(mem);
        if (!mem)
        {
            fclose(file);
            PERFINFO_AUTO_STOP();
            return 0;
        }
        bytes_read = fread(mem,1,total,file);
        //assert(bytes_read==total);
        fclose(file);
        mem[bytes_read] = 0;
        if (lenp)
            *lenp = bytes_read;

    PERFINFO_AUTO_STOP();

    return mem;
}

void *fileAllocEx_dbg(const char *fname,int *lenp, const char *mode, FileProcessCallback callback MEM_DBG_PARMS)
{
    FILE    *file;
    intptr_t total=0,bytes_read=0, last_read, zipped, eof=0, original_total;
    char    *mem=0,*located_name, buf[MAX_PATH];
    int        chunk_size=65536;


    located_name = fileLocateRead(fname, buf);
    if (!located_name)
        return 0;

    original_total = total = fileSize(located_name);

    zipped = 0!=strchr(mode, 'z');
    if (zipped) {
        total = total * 11; // Estimate
    }

    file = fopen(located_name,mode);
    located_name[0]=0; // clear locatebuff just to be safe
    if (!file)
        return 0;

    mem = smalloc(total+1);
    assert(mem);
    if (callback) {
        if (!callback(bytes_read, total)) {
            free(mem);
            return NULL;
        }
    }
    while ((!zipped && bytes_read < total) || (zipped && !eof)) {
        if (total - bytes_read < chunk_size) {
            if (!zipped) { // Just read what's left
                chunk_size = total - bytes_read;
            } else { // We will need to realloc
                total = (int)(total*1.5);
                mem = srealloc(mem, total+1);
            }
        }
        last_read = fread(mem+bytes_read,1,chunk_size,file);
        if (last_read!=chunk_size) {
            if (!zipped) {
                // This should not happen.  Ever.  Well, unless a file is modified during the read
                //   process, or pigs are modifed while the game is running
                assert(!"Error reading from file, it's not as big as fileSize said it was!");
                free(mem);
                return NULL;
            } else {
                // Size mismatch on read
                if (last_read == 0) {
                    eof = 1;
                } else if (last_read == -1) {
                    // Error!
                    free(mem);
                    return NULL;
                }
            }
        }
        if (last_read > 0) {
            bytes_read+=last_read;
            if (callback) {
                if (!callback(bytes_read, total)) {
                    free(mem);
                    return NULL;
                }
            }
        }
    }
    if (!zipped) {
        assert(bytes_read==total);
    }
    fclose(file);
    mem[bytes_read] = 0;
    if (lenp)
        *lenp = bytes_read;
    return mem;
}

int printPercentage(int bytes_processed, int total) // Sample FileProcessCallback
{
    static int last = 0;
    int current = timerCpuTicks() * 4 / timerCpuSpeed();

    if(isGuiDisabled()) return 1;

    if (total && current != last) {
        int numprinted=0;
        numprinted+=printf("%5.1lf%% ", 100.f*bytes_processed / (float)total);
        numprinted+=printPercentageBar(10ULL*(unsigned long long)bytes_processed / total, 10);
        backSpace(numprinted, total==bytes_processed);
        last = current;
    }
    return 1;
}



int fileRenameToBak(const char *fname_) {
    char fname[MAX_PATH];
    char backfn[MAX_PATH];

    strcpy_s(fname, MAX_PATH, fname_);
    moveFromCRoot(fname);
    sprintf_s(SAFESTR(backfn), "%s.bak", fname);
    // delete old .bak if there
    if (fileExists(backfn)) {
        fileForceRemove(backfn);
    }
    // rename
    chmod(fname, _S_IREAD | _S_IWRITE);
    return rename(fname, backfn);
}
#ifndef _XBOX
int fileMoveToRecycleBin(const char *filename)
{
    char buffer[MAX_PATH] = {0};
    SHFILEOPSTRUCTA shfos;
    int ret;
    shfos.hwnd = compatibleGetConsoleWindow();
    shfos.wFunc = FO_DELETE;
    // Needs to be doubly terminated
//     strncpy(buffer, filename, ARRAY_SIZE(buffer)-2);
     strncpy_s(buffer, ARRAY_SIZE_CHECKED(buffer), filename, ARRAY_SIZE(buffer)-2);
    moveFromCRoot(buffer);
    buffer[strlen(buffer)+1]=0;
    backSlashes(buffer);
    shfos.pFrom = buffer;
    shfos.pTo = buffer;
    shfos.fFlags = FOF_ALLOWUNDO|FOF_NOCONFIRMATION|FOF_NOERRORUI|FOF_SILENT;
    shfos.hNameMappings = NULL;
    shfos.lpszProgressTitle = "Deleting files...";

    ret = SHFileOperationA(&shfos);
    if (ret==0) {
        // Succeeded call, did anything get cancelled?
        ret = shfos.fAnyOperationsAborted;
    }
    return ret;

}
#endif

int fileForceRemove(const char *fname_) {
    int ret;
    char fname[MAX_PATH];
    strcpy_s(fname, MAX_PATH, fname_);
    moveFromCRoot(fname);
    ret = chmod(fname, _S_IREAD | _S_IWRITE);
    if (ret==-1 && GetLastError() != 5) { // not access denied
        // File doesn't exist
        return ret;
    }
//    ret = fileMoveToRecycleBin(fname);
//    if (ret!=0) {
        ret = remove(fname);
//    }
    return ret;
}

// Copy a file and may sync the timestamp (if dest is network drive, timestamp may not be synced), overwriting read-only files
// Returns 0 upon success
int fileCopy(char *src_, char *dst_)
{
    int ret;

#if _XBOX
    chmod(dst, _S_IREAD | _S_IWRITE);
    ret = CopyFile( fileFixSlashes(src), fileFixSlashes(dst), TRUE ) == TRUE;
    if ( !ret )
    {
        // Try again!
        printf("Attempt to copy over an executable (%s) while running, ranaming old .exe to .bak and trying again...\n", getFileName(dst));
        fileRenameToBak(dst);
        ret = CopyFile( src, dst, TRUE ) == TRUE;
    }
#else
    char buf[1024];
    char src[MAX_PATH];
    char dst[MAX_PATH];
    strcpy_s(src, MAX_PATH, src_);
    strcpy_s(dst, MAX_PATH, dst_);
    moveFromCRoot(src);
    moveFromCRoot(dst);
    chmod(dst, _S_IREAD | _S_IWRITE);
    sprintf_s(SAFESTR(buf), "copy \"%s\" \"%s\">nul", fileFixSlashes(src), fileFixSlashes(dst));
    ret = system(buf);
    if (ret!=0 && (strEndsWith(dst, ".exe") || strEndsWith(dst, ".dll") || strEndsWith(dst, ".pdb"))) {
        // Try again!
        printf("Attempt to copy over an executable (%s) while running, ranaming old .exe to .bak and trying again...\n", getFileName(dst));
        fileRenameToBak(dst);
        ret = system(buf);
    }
#endif

    return ret;
}

int fileMove(char *src_, char *dst_)
{
    int ret;

#if _XBOX
    chmod(dst, _S_IREAD | _S_IWRITE);
    ret = MoveFile( fileFixSlashes(src), fileFixSlashes(dst) ) == TRUE;
#else
    char buf[1024];
    char src[MAX_PATH];
    char dst[MAX_PATH];
    strcpy_s(src, MAX_PATH, src_);
    strcpy_s(dst, MAX_PATH, dst_);
    moveFromCRoot(src);
    moveFromCRoot(dst);
    chmod(dst, _S_IREAD | _S_IWRITE);
    sprintf_s(SAFESTR(buf), "move \"%s\" \"%s\">nul", fileFixSlashes(src), fileFixSlashes(dst));
    ret = system(buf);
#endif
    return ret;
}

void fileOpenWithEditor(const char *localfname)
{
#ifndef _XBOX
    static char *skip_exts = ".exe .bat .reg .3ds .max .wrl .lnk .com"; // what *not* to run "open" on
    int ret;
    if (!(strrchr(localfname, '.') && strstri(skip_exts, strrchr(localfname, '.')))) {
        bool isTxt = !!strstri(".txt", strrchr(localfname, '.'));
        // Try "edit" first on text files, to try and get EditPlus on Woomer's machine
        ret = (int)(intptr_t)ShellExecuteA( NULL, isTxt?"edit":"open", localfname, NULL, NULL, SW_SHOW);
        if (ret <= 32) {
            // There was a problem, try Edit on it
            ret = (int)(intptr_t)ShellExecuteA( NULL, isTxt?"open":"edit", localfname, NULL, NULL, SW_SHOW);
            if (ret <= 32) {
                // There was a problem, EditPlus?
                ret = (int)(intptr_t)ShellExecuteA( NULL, "EditPlus", localfname, NULL, NULL, SW_SHOW);
            }
        }
    } else {
        static char *edit_exts = ".bat .reg"; // what *to* run "edit" on (must also be in the above list)
        if (!(strrchr(localfname, '.') && !strstri(edit_exts, strrchr(localfname, '.')))) {
            ShellExecuteA( NULL, "edit", localfname, NULL, NULL, SW_SHOW);
        }
    }
#endif
}



typedef struct FileScanData {
    char**                names;
    S32                    count;
    FileScanFolders        scan_folders;
} FileScanData;

static void fileScanDirRecurse(char *dir,FileScanData* data)
{
    WIN32_FIND_DATAA wfd;
    U32                handle;
    S32                good;
    char            buf[1200];

     strcpy(buf,dir);
    strcat(buf,"/*");

    for(good = fwFindFirstFile(&handle, buf, &wfd); good; good = fwFindNextFile(handle, &wfd))
    {
        if( wfd.cFileName[0] == '.'
            ||
            !(data->scan_folders & FSF_UNDERSCORED) &&
            wfd.cFileName[0] == '_')
        {
            continue;
        }
        if (wfd.dwFileAttributes & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM) && (data->scan_folders & FSF_NOHIDDEN))
            continue;

        STR_COMBINE_SSS(buf, dir, strchr("\\/", dir[strlen(dir)-1])==0?"/":"", wfd.cFileName);

        if ((data->scan_folders & FSF_FILES) && !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ||
                (data->scan_folders & FSF_FOLDERS) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            // Add to the list
            data->names = realloc(data->names,(data->count+1) * sizeof(data->names[0]));
            data->names[data->count] = strdup(buf);
            data->count++;
        }
        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            fileScanDirRecurse(buf,data);
        }
    }
    fwFindClose(handle);
}

/*given a directory, it returns an string array of the full path to each of the files in that directory,
and it fills count_ptr with the number of files found.  files and sub-folders prefixed with "_" or "."
are ignored.  This requires an absolute path, and is intented only to be used in utilities.  For the
game code, run fileScanAllDataDirs instead.
*/
char **fileScanDir(char *dir,int *count_ptr)
{
    FileScanData data = {0};

    assert(fileIsAbsolutePath(dir) && "Only works on absolute paths!  This function is only to be used in utilities, not in the game code, use fileScanAllDataDirs isntead");

    data.names=0;
    data.scan_folders = FSF_FILES;

    fileScanDirRecurse(dir,&data);

    if(count_ptr){
        *count_ptr = data.count;
    }

    return data.names;
}

// Allows the caller to specify whether folders should be returned in the list
// or not (can ask for just files, just folders, or both)
char **fileScanDirFolders(char *dir, int *count_ptr, FileScanFolders folders)
{
    FileScanData data = {0};

    assert(fileIsAbsolutePath(dir) && "Only works on absolute paths!  This function is only to be used in utilities, not in the game code, use fileScanAllDataDirs isntead");

    data.names=0;
    data.scan_folders = folders;

    fileScanDirRecurse(dir,&data);

    if(count_ptr){
        *count_ptr = data.count;
    }

    return data.names;
}

void fileScanDirFreeNames(char **names,int count)
{
    int        i;

    for(i=0;i<count;i++)
        free(names[i]);
    free(names);
}

/* Function fileScandirRecurseEx()
 *    This function traverses the specified directory, one item at a time.  The function
 *    passes each encountered item back to FileScanProcessor.
 *
 *    While the function itself is "thread-safe". Its intended usage is not.  The caller
 *    is calling this function to perform processing on multiple items.  Most likely, the
 *    caller will have to keep track of *some* context information when calling this function.
 *    Since this function does not provide any way for the caller to retrieve the said context,
 *  the context must be some global variable.  Thread-safty is then broken.
 *
 *    One way to solve this problem is to require the caller to pass in a structure that
 *    conforms to some interface.
 *
 *
 *
 */
void fileScanDirRecurseEx(const char* dir, FileScanProcessor processor){
    WIN32_FIND_DATAA wfd;
    U32                handle;
    S32                good;
    char             buffer[1024];
    char             dir2[1024];
    FileScanAction    action;

    assert(fileIsAbsolutePath(dir) && "Only works on absolute paths!  This function is only to be used in utilities, not in the game code, use fileScanAllDataDirs isntead");

    strcpy(dir2, dir);
    strcpy(buffer, dir);
    strcat(buffer, "/*");

    for(good = fwFindFirstFile(&handle, buffer, &wfd); good; good = fwFindNextFile(handle, &wfd)){
        struct _finddata32_t fd;

        if(wfd.cFileName[0] == '.')
            continue;

        strcpy(fd.name, wfd.cFileName);

        fd.size = wfd.nFileSizeLow;
        _FileTimeToUnixTime(&wfd.ftLastWriteTime, &fd.time_write, FALSE);
        fd.time_write = statTimeFromUTC(fd.time_write);
        fd.attrib = (wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ? _A_HIDDEN : 0) |
                    (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? _A_SUBDIR : 0) |
                    (wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ? _A_SYSTEM : 0) |
                    (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY ? _A_RDONLY : _A_NORMAL) |
                    0;

        action = processor(dir2, &fd);

        if(    action & FSA_EXPLORE_DIRECTORY &&
            wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {

            STR_COMBINE_SSS(buffer, dir2, "/", wfd.cFileName);

            fileScanDirRecurseEx(buffer, processor);
        }

        if(action & FSA_STOP)
            break;
    }
    fwFindClose(handle);
}


FileScanAction printAllFileNames(char* dir, struct _finddata32_t* data){
    printf("%s/%s\n", dir, data->name);
    return FSA_EXPLORE_DIRECTORY;
}

void fileScanDirRecurseContext(const char* dir, FileScanContext* context){
    WIN32_FIND_DATAA wfd;
    U32                handle;
    S32                good;
    char            buffer[1024];
    char            dir2[1024];

    assert(fileIsAbsolutePath(dir) && "Only works on absolute paths!  This function is only to be used in utilities, not in the game code, use fileScanAllDataDirs isntead");

    strcpy(dir2, dir);
    strcpy(buffer, dir);
    if (fileExists(buffer)) {
        // If this is a file, let it find it!
        char *z = max(strrchr(dir2, '/'), strrchr(dir2, '\\'));
        if (z) {
            *++z=0;
        }
    } else {
        // Only add a wildcard if this is a folder passed in.
        if (strchr(buffer, '*')!=0) {
            // Already has a wildcard
            // fix up dir2
            char *z = max(strrchr(dir2, '/'), strrchr(dir2, '\\'));
            if (z) {
                *++z=0;
            }
        } else {
            strcat(buffer, "/*");
        }
    }

    for(good = fwFindFirstFile(&handle, buffer, &wfd); good; good = fwFindNextFile(handle, &wfd)){
        struct _finddata32_t    fd;
        FileScanAction        action;

        if(    wfd.cFileName[0] == '.' &&
            (    !wfd.cFileName[1] ||
                wfd.cFileName[1] == '.'))
        {
            continue;
        }

        //for(test = handle = _findfirst32(buffer, &fileinfo); test >= 0; test = _findnext32(handle, &fileinfo)){
        //    if(fileinfo.name[0] == '.' && (fileinfo.name[1] == '\0' || fileinfo.name[1] == '.'))
        //        continue;

        strcpy(fd.name, wfd.cFileName);

        fd.size = wfd.nFileSizeLow;
        _FileTimeToUnixTime(&wfd.ftLastWriteTime, &fd.time_write, FALSE);
        fd.time_write = statTimeFromUTC(fd.time_write);
        fd.attrib = (wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ? _A_HIDDEN : 0) |
                    (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? _A_SUBDIR : 0) |
                    (wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ? _A_SYSTEM : 0) |
                    (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY ? _A_RDONLY : _A_NORMAL) |
                    0;

        context->dir = dir2;
        context->fileInfo = &fd;
        action = context->processor(context);

        if(    action & FSA_EXPLORE_DIRECTORY &&
            wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            STR_COMBINE_SSS(buffer, dir2, "/", fd.name);

            fileScanDirRecurseContext(buffer, context);
        }

        if(action & FSA_STOP)
            break;
    }
    fwFindClose(handle);
}

#undef getc
#undef fgetc
#undef fputc
#undef fseek
#undef fopen
#undef fclose
#undef ftell
#undef fread
#undef fwrite
#undef fgets
#undef fprintf
#undef fflush
#undef setvbuf
#undef FILE

static void logAccess(char *fname)
{
#ifndef _XBOX
    static    void    *file;

    // Can't call fileDataDir() before loadedGameDataDirs is true
    if (!loadedGameDataDirs)
        return;

    if (!file)
    {
        char    buf[1000];

        sprintf_s(SAFESTR(buf),"%s/../logs/file_access.log",fileDataDir());
        file = _fsopen(buf,"wt", _SH_DENYNO);
        if (file)
        {
            fprintf(file, "#============================================\n");
            fprintf(file, "# %s: Start file logging: fileDataDir = %s\n", timerGetTimeString(), fileDataDir());
            fprintf(file, "#============================================\n");
        }
    }

    if (file)
    {
        fprintf(file,"%s\n",fname);
        fflush(file);
    }
#else
    assert(0);
#endif
}

MP_DEFINE(FileWrapper);
static LazyLock lock = {0};

static FileWrapper* createFileWrapper(){
    FileWrapper    *fw = NULL;

    lazyLock(&lock);
    MP_CREATE(FileWrapper, 32);
    fw = MP_ALLOC(FileWrapper);
    lazyUnlock(&lock);
    return fw;
}

static void destroyFileWrapper(FileWrapper *fw){
    lazyLock(&lock);
    MP_FREE(FileWrapper, fw);
    lazyUnlock(&lock);
}

int glob_assert_file_err;

static int log_file_access=0;

int fileSetLogging(int on)
{
    int        ret = log_file_access;

    log_file_access = on;
    return ret;
}

static FileExtraLoggingFunction extra_logging_function=NULL;

void fileSetExtraLogging(FileExtraLoggingFunction function)
{
    extra_logging_function = function;
}

void fileDisableWinIO(int disable) // Disables use of Windows ReadFile/CreateFile instead of CRT for binary files
{
    file_disable_winio = disable;
}

int stuffbuff_getc(StuffBuff *sb)
{
    if (sb->idx >= sb->size)
        return EOF;
    return sb->buff[sb->idx++];
}

long stuffbuff_fread(StuffBuff *sb, void *buf, int size)
{
    int sizeleft = sb->size - sb->idx;
    if (size > sizeleft)
        size = sizeleft;
    if (size <= 0)
        return 0;
    memcpy(buf, &(sb->buff[sb->idx]), size);
    sb->idx += size;
    return size;
}

void* fopenf(const char *name, const char *how, ...)
{
    char *tmp = estrTemp();
    void *res = NULL;

    VA_START(va, how);
    estrConcatfv(&tmp, name, va);
    VA_END();

    res = x_fopen(tmp, how);
    estrDestroy(&tmp);
    return res;
}


void *x_fopen(const char *_name,const char *how)
{
    IOMode        iomode=IO_CRT;
    FileWrapper    *fw=0;
    char        fopenMode[128];
    char*        modeCursor;
    static int    maxStreamSet = 0;
    char        name[MAX_PATH];
    PigFileDescriptor pfd;
    bool        no_winio=false;
    int            retries;
    bool        retry;

    assert(strlen(_name) < MAX_PATH);
    strcpy(name, _name); // in case we destroy name along the way (i.e. call fileLocate)

#ifdef _XBOX
    fileFixupForXBox(name, ARRAY_SIZE_CHECKED(name), NULL, 0);
#endif

    if (log_file_access)
    {
        logAccess(name);
    }

    if (extra_logging_function)
    {
        extra_logging_function(name);
    }

    // Make sure we can open enough files.
    if(!maxStreamSet){
        _setmaxstdio(2048); // This is the CRT maximum!
        maxStreamSet = 1;
    }

    glob_assert_file_err = 1;

    // Find a free file_wrapper
    fw = createFileWrapper();
    if (!fw)
    {
        glob_assert_file_err = 0;
//        assert(!"ran out of file wrappers");
        return 0;
    }

    assert(strlen(how) < ARRAY_SIZE(fopenMode));
    strcpy(fopenMode, how);

    modeCursor = strchr(fopenMode, '!');
    if(modeCursor){
        size_t modeLen;
        // Remove ! from the fopen mode string to be given to the real fopen.
        modeLen = strlen(fopenMode);
        memmove(modeCursor, modeCursor + 1, modeLen - (modeCursor - fopenMode));
        no_winio = true;
    }

    // If the character 'z' is used to open the file, use zlib for file IO.
    modeCursor = strchr(fopenMode, 'z');
    if(modeCursor){
        size_t modeLen;

        // Remove z from the fopen mode string to be given to the real fopen.
        modeLen = strlen(fopenMode);
        memmove(modeCursor, modeCursor + 1, modeLen - (modeCursor - fopenMode));

        iomode = IO_GZ;
//        printf("found gz file\n");
    } else if (stricmp(fopenMode, "StuffBuff")==0) {
        // We want to open a file for writing that will be saved in a string
        iomode = IO_STRING;
    } else {
        // Check for specifically not looking in the pigg files
        modeCursor = strchr(fopenMode, '~');
        if(modeCursor){ // If it specifically says not to use Piglib, or if it's a write-access request
            size_t modeLen;

            // Remove ~ from the fopen mode string to be given to the real fopen.
            modeLen = strlen(fopenMode);
            memmove(modeCursor, modeCursor + 1, modeLen - (modeCursor - fopenMode));

            iomode = IO_CRT;
//            printf("found ~, normal file\n");
        } else if (strcspn(how, "wWaA+")!=strlen(how)) {
            iomode = IO_CRT;
            assert(!is_pigged_path(name));
//            printf("found wWaA+, normal file\n");
        } else {
            // If we got here, we're doing a normal open for reading,
            // Maybe we should do a double-check to see if this is a path relative to the game data dir, and if so,
            //   re-locate it with fileLocateRead, hoping to find it in a pig file (this will
            //   happen in the code that calls fopen without calling fileLocate first)
            // check to see if the path has two ':'s, and if so, we will open from a pig file
            if (is_pigged_path(name)) {
                FolderNode * node;
                iomode = IO_PIG;
                strcpy(name, strrchr(name, ':')+2); // remove the path to the pigg...
                PERFINFO_AUTO_START("FolderCacheQuery", 1);
                    node = FolderCacheQuery(folder_cache, name);
                PERFINFO_AUTO_STOP();
                assert(node);
                assert(node->virtual_location<0);
                assert(node->file_index>=0);
                pfd = PigSetGetFileInfo(VIRTUAL_LOCATION_TO_PIG_INDEX(node->virtual_location), node->file_index, name);
                assert(pfd.parent);

//                printf("found pigged path, pig file\n");
            } else {
                iomode = IO_CRT;
//                printf("default case, normal file\n");
            }
        }
    }
    if (iomode == IO_CRT) {
        if (!file_disable_winio && !no_winio) {
            // If this is a binary file, open it with WINIO instead
            if (strchr(fopenMode, 'b')!=0)
                iomode=IO_WINIO;

            // If this is a lock file, open with WINIO.
            if (strchr(fopenMode, 'l') != 0)
                iomode=IO_WINIO;
        }
    }

    fw->iomode = iomode;
    retries=0;
    retry=true;
    while (retry && retries < 2)
    {
        retry = false;
        retries++;
        switch(iomode) {
            xcase IO_GZ:
                PERFINFO_AUTO_START("x_fopen:gzopen", 1);
                //printf("opening gz file\n");
                fw->fptr = gzopen(name,fopenMode);
                if (!fw->fptr) {
                    fileAttemptNetworkReconnect(name);
                    retry = true;
                }
            xcase IO_PIG:
                PERFINFO_AUTO_START("x_fopen:pig_fopen_pfd", 1);
                //printf("opening pigged file\n");
                fw->fptr = pig_fopen_pfd(&pfd, fopenMode);
            xcase IO_STRING:
                PERFINFO_AUTO_START("x_fopen:StuffBuff", 1);
                fw->fptr = *(StuffBuff**)&_name[8];
            xcase IO_CRT:
                //printf("opening normal file\n");
                if (!name || !name[0]) // to avoid dumb assert in msoft's debug c lib
                {
                    PERFINFO_AUTO_START("x_fopen:badfilename", 1);
                    fw->fptr = 0;
                    //printf("empty filename\n");
                }
                else
                {
#ifdef _XBOX
                    backSlashes(name);
#endif
                    moveFromCRoot(name);
                    PERFINFO_AUTO_START("x_fopen:fopen", 1);
                    fw->fptr = _fsopen(name, fopenMode, _SH_DENYNO);
                    if (!fw->fptr) {
                        fileAttemptNetworkReconnect(name);
                        retry = true;
                    }
                }
            xcase IO_WINIO:
                {
                    char *c;
                    DWORD accessMode = 0, creationDisposition = 0;
                    DWORD sharingMode = FILE_SHARE_READ|FILE_SHARE_WRITE; // Let anyone else open this file for any purpose
                    DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
                    bool isLockFile = false;

                    PERFINFO_AUTO_START("x_fopen:CreateFile", 1);
#ifdef _XBOX
                    backSlashes(name);
#endif
                    moveFromCRoot(name);
                    if (isDevelopmentMode())
                        sharingMode|=FILE_SHARE_DELETE;
                    for (c=fopenMode; *c; c++) {
                        switch (*c) {
                            xcase 'r':
                                accessMode |= GENERIC_READ;
                                creationDisposition = OPEN_EXISTING;
                                if (c[1]=='+')
                                    accessMode |= GENERIC_WRITE;
                            xcase 'w':
                                accessMode |= GENERIC_WRITE;
                                creationDisposition = CREATE_ALWAYS;
                                if (c[1]=='+')
                                    accessMode |= GENERIC_READ;
                            xcase 'a':
                                creationDisposition = OPEN_ALWAYS;
                                dwFlagsAndAttributes |= FILE_FLAG_SEQUENTIAL_SCAN;
                                accessMode |= FILE_APPEND_DATA;
                            xcase 'l':
                                isLockFile = true;
                            xcase 'b':
                                // already handled
                            xcase 'c':
                                // Commit flag
                                //   On by default, it seems?
                                // This is really slow (too slow for Hogging):
                                //   dwFlagsAndAttributes |= FILE_FLAG_WRITE_THROUGH;
                            xcase 'R':
                                // specify random access to prevent file system from reading ahead (eg if just reading file header but no more)
                                dwFlagsAndAttributes |= FILE_FLAG_RANDOM_ACCESS;
                            xcase '+':
                            xdefault:
                                assertmsg(0, "Bad file open mode passed to fopen");
                        }
                    }
                    // FILE_SHARE_DELETE is not supported under Windows 95/98/ME

                    if (isLockFile && (accessMode & GENERIC_WRITE))
                        creationDisposition = CREATE_NEW;
                    if ( !IsUsingWin2kOrXp() && !IsUsingXbox() && sharingMode & FILE_SHARE_DELETE )
                        sharingMode &= ~FILE_SHARE_DELETE;

                    fw->fptr = CreateFileA(name, accessMode, sharingMode, NULL, creationDisposition, dwFlagsAndAttributes, NULL);
                    if (fw->fptr == INVALID_HANDLE_VALUE)
                        fw->fptr = NULL;
                    if (!fw->fptr) {
                        fileAttemptNetworkReconnect(name);
                        retry = true;
                    } else {
                        x_fseek(fw, 0, SEEK_SET);
                    }
                }

        }
        PERFINFO_AUTO_STOP();
    }

    if (fw->fptr)
    {
//        printf("filewrapper's file pointer is valid\n");
        //strcpy(fw->name,name);
        if (fw->nameptr = allocFindString(name))
            fw->nameptr_needs_freeing = 0;
        else {
            fw->nameptr = strdup(name);
            fw->nameptr_needs_freeing = 1;
        }
#ifdef FW_TRACK_ALLOCATION
        dumpStackToBuffer(fw->allocStack, FW_ALLOC_STACK_SIZE, NULL);
#endif
        glob_assert_file_err = 0;

        return fw;
    }
//    printf("filewrapper's file pointer is not valid\n");
    destroyFileWrapper(fw);
    glob_assert_file_err = 0;
    return 0;
}

int x_fclose(FileWrapper *fw)
{
    void *fptr;
    IOMode iomode;
    static int zero=0;
    int ret=-1;

    if(!fw)
        return 1;

    fptr = fw->fptr;
    iomode = fw->iomode;

    //ZeroArray(fw->name);
    if (fw->nameptr_needs_freeing) {
        SAFE_FREE((char*)fw->nameptr);
    }
    fw->nameptr = NULL;
    fw->nameptr_needs_freeing = 0;
#ifdef FW_TRACK_ALLOCATION
    fw->allocStack[0] = '\0';
#endif
    fw->fptr = 0;
    switch(iomode) {
        xcase IO_GZ:
            PERFINFO_AUTO_START("x_fclose:gzclose", 1);
            ret = gzclose(fptr);
        xcase IO_PIG:
            PERFINFO_AUTO_START("x_fclose:pig_fclose", 1);
            ret = pig_fclose(fptr);
        xcase IO_STRING:
            PERFINFO_AUTO_START("x_fclose:addBinaryDataToStuffBuff", 1);
            addBinaryDataToStuffBuff((StuffBuff*)fptr, (char*)&zero, 1);
            ret = 0;
        xcase IO_CRT:
            PERFINFO_AUTO_START("x_fclose:CRT_fclose", 1);
            ret = fclose(fptr);
        xcase IO_WINIO:
            PERFINFO_AUTO_START("x_fclose:CloseHandle", 1);
            ret = CloseHandle(fptr) ? 0 : -1;
    }
    destroyFileWrapper(fw);
    PERFINFO_AUTO_STOP();
    return ret;
}

int x_fgetc(FileWrapper *fw)
{
    int ret=-1;
    int count;
    switch(fw->iomode) {
        xcase IO_GZ:
            PERFINFO_AUTO_START("x_fgetc:gzgetc", 1);
            ret = gzgetc((gzFile)fw->fptr);
        xcase IO_PIG:
            PERFINFO_AUTO_START("x_fgetc:pig_fgetc", 1);
            ret = pig_fgetc(fw->fptr);
        xcase IO_STRING:
            PERFINFO_AUTO_START("x_fgetc:assert", 1);
            assert(!"StuffBuffFiles are write-only");
            ret = 0;
        xcase IO_CRT:
            PERFINFO_AUTO_START("x_fgetc:fgetc", 1);
            ret = fgetc(fw->fptr);
        xcase IO_WINIO:
            PERFINFO_AUTO_START("x_fgetc:ReadFile", 1);
            ret = 0;
            ReadFile(fw->fptr, &ret, 1, &count, NULL);
            if (count!=1)
                ret = EOF;
    }
    PERFINFO_AUTO_STOP();
    return ret;
}

int x_fputc(int c,FileWrapper *fw)
{
    int ret=-1;
    int count;
    switch(fw->iomode) {
        xcase IO_GZ:
            PERFINFO_AUTO_START("x_fputc:gzputc", 1);
            ret = gzputc(fw->fptr, c);
        xcase IO_PIG:
            PERFINFO_AUTO_START("x_fputc:pigassert", 1);
            assert(false);
        xcase IO_STRING:
            PERFINFO_AUTO_START("x_fputc:addBinaryDataToStuffBuff", 1);
            addBinaryDataToStuffBuff((StuffBuff*)fw->fptr, (char *)&c, 1);
            ret = 1;
        xcase IO_CRT:
            PERFINFO_AUTO_START("x_fputc:fputc", 1);
            ret = fputc(c,fw->fptr);
        xcase IO_WINIO:
            PERFINFO_AUTO_START("x_fputc:WriteFile", 1);
            ret = WriteFile(fw->fptr, &c, 1, &count, NULL);
            if (ret==0)
                ret = EOF;
            else
                ret = c;
    }
    PERFINFO_AUTO_STOP();
    return ret;
}

S64 x_fseek(FileWrapper *fw, S64 dist,int whence)
{
    S64 ret=-1;
    DWORD dwPtr=0;
    switch(fw->iomode) {
        xcase IO_GZ:
            PERFINFO_AUTO_START("x_fseek:gzseek", 1);
            ret = gzseek(fw->fptr,(long)dist,whence);
        xcase IO_PIG:
            PERFINFO_AUTO_START("x_fseek:pig_fseek", 1);
            ret = pig_fseek(fw->fptr,(long)dist,whence);
        xcase IO_STRING:
            PERFINFO_AUTO_START("x_fseek:assert", 1);
            assert(!"StuffBuffFiles are write-only");
            ret = 0;
        xcase IO_CRT:
            PERFINFO_AUTO_START("x_fseek:fseek", 1);

            switch (whence) {
            xcase SEEK_SET:
                //dist = dist;
            xcase SEEK_CUR:
            {
                S64 oldpos;
                fgetpos(fw->fptr, &oldpos);
                dist += oldpos;
            }
            xcase SEEK_END:
                dist = _filelength(_fileno(fw->fptr)) + dist;
            xdefault:
                assert(0);
            }
            ret = fsetpos(fw->fptr,&dist);
        xcase IO_WINIO:
            PERFINFO_AUTO_START("x_fseek:SetFilePointer", 1);
            {
                U32 lowbits = (U32)(dist&0xFFFFFFFFLL);
                S32 highbits = (S32)(dist >> 32LL);
                switch(whence)
                {
                    xcase SEEK_SET:
                        dwPtr = SetFilePointer(fw->fptr,lowbits,&highbits,FILE_BEGIN);
                    xcase SEEK_END:
                        dwPtr = SetFilePointer(fw->fptr,lowbits,&highbits,FILE_END);
                    xcase SEEK_CUR:
                        dwPtr = SetFilePointer(fw->fptr,lowbits,&highbits,FILE_CURRENT);
                    xdefault:
                        assert(0);
                }
            }
            if (dwPtr == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR )
                ret = -1;
            else
                ret = 0;
    }
    PERFINFO_AUTO_STOP();
    return ret;
}

int x_getc(FileWrapper *fw)
{
    int ret=-1;
    DWORD count;
    switch(fw->iomode) {
        xcase IO_GZ:
            PERFINFO_AUTO_START("x_getc:gzgetc", 1);
            ret = gzgetc((gzFile)fw->fptr);
        xcase IO_PIG:
            PERFINFO_AUTO_START("x_getc:pig_getc", 1);
            ret = pig_getc(fw->fptr);
        xcase IO_STRING:
            PERFINFO_AUTO_START("x_getc:stuffbuff_getc", 1);
            ret = stuffbuff_getc(fw->fptr);
        xcase IO_CRT:
            PERFINFO_AUTO_START("x_getc:getc", 1);
            ret = getc(fw->fptr);
        xcase IO_WINIO:
            PERFINFO_AUTO_START("x_getc:ReadFile", 1);
            ret = 0;
            ReadFile(fw->fptr, &ret, 1, &count, NULL);
            if (count!=1)
                ret = EOF;
    }
    PERFINFO_AUTO_STOP();
    return ret;
}

// Functionally identical to SetFilePointerEx, but runs on Win98
BOOL SetFilePointerExWin98(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod)
{
    LARGE_INTEGER li;

    li.QuadPart = liDistanceToMove.QuadPart;

    li.LowPart = SetFilePointer (hFile, li.LowPart, &li.HighPart, dwMoveMethod);

    if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
    {
        // Failure
        li.QuadPart = -1;
        return FALSE;
    }

    lpNewFilePointer->QuadPart = li.QuadPart;
    return TRUE;
}

S64 x_ftell(FileWrapper *fw)
{
    S64 ret=-1;
    switch(fw->iomode) {
        xcase IO_GZ:
            PERFINFO_AUTO_START("x_ftell:gztell", 1);
            ret = gztell(fw->fptr);
        xcase IO_PIG:
            PERFINFO_AUTO_START("x_ftell:pig_ftell", 1);
            ret = pig_ftell(fw->fptr);
        xcase IO_STRING:
            PERFINFO_AUTO_START("x_ftell:stuffBuff", 1);
            ret = ((StuffBuff*)fw->fptr)->idx;
        xcase IO_CRT:
            PERFINFO_AUTO_START("x_ftell:ftell", 1);
            ret = ftell(fw->fptr);
        xcase IO_WINIO:
            PERFINFO_AUTO_START("x_ftell:SetFilePointer", 1);
            {
                LARGE_INTEGER zero = {0};
                LARGE_INTEGER newpos;
                if (SetFilePointerExWin98(fw->fptr,zero,&newpos,FILE_CURRENT))
                    ret = newpos.QuadPart;
                else
                    ret = -1;
            }
    }
    PERFINFO_AUTO_STOP();
    return ret;
}

intptr_t x_fread(void *buf,size_t size1,size_t size2,FileWrapper *fw)
{
    intptr_t ret=-1;
    DWORD count;
    if (!size1 || !size2)
        return 0;
    assert(buf);
    switch(fw->iomode) {
        xcase IO_GZ:
            PERFINFO_AUTO_START("x_fread:gzread", 1);
            ret = gzread(fw->fptr, buf, (unsigned int)(size1*size2)) / size1;
        xcase IO_PIG:
            PERFINFO_AUTO_START("x_fread:pig_fread", 1);
            ret = pig_fread(fw->fptr, buf, (long)(size1*size2)) / size1;
        xcase IO_STRING:
            PERFINFO_AUTO_START("x_fread:stuffbuff_fread", 1);
            ret = stuffbuff_fread(fw->fptr, buf, (int)(size1*size2)) / size1;
        xcase IO_CRT:
            PERFINFO_AUTO_START("x_fread:fread", 1);
            ret = fread(buf,size1,size2,fw->fptr);
        xcase IO_WINIO:
            PERFINFO_AUTO_START("x_fread:ReadFile", 1);
            ret = ReadFile(fw->fptr, buf, (DWORD)(size1*size2), &count, NULL);
            if (ret)
                ret = count/size1;
    }
    ADD_MISC_COUNT(size1 * size2, "bytes_read");
    PERFINFO_AUTO_STOP();

    return ret;
}

intptr_t x_fwrite(const void *buf,size_t size1,size_t size2,FileWrapper *fw)
{
    intptr_t ret=-1;
    DWORD count;
    if (!size1 || !size2) return 0;

    switch(fw->iomode) {
        xcase IO_GZ:
            PERFINFO_AUTO_START("x_fwrite:gzwrite", 1);
            ret = gzwrite(fw->fptr,(void*)buf,(unsigned int)(size1 * size2)) / size1;
        xcase IO_PIG:
            PERFINFO_AUTO_START("x_fwrite:assert", 1);
            assert(false);
            ret = 0;
        xcase IO_STRING:
            PERFINFO_AUTO_START("x_fwrite:addBinaryDataToStuffBuff", 1);
            addBinaryDataToStuffBuff((StuffBuff*)fw->fptr, buf, (int)(size1*size2));
            ret = size1*size2;
        xcase IO_CRT:
            PERFINFO_AUTO_START("x_fwrite:fwrite", 1);
            ret = fwrite(buf,size1,size2,fw->fptr);
        xcase IO_WINIO:
            PERFINFO_AUTO_START("x_fwrite:WriteFile", 1);
            ret = WriteFile(fw->fptr, buf, (DWORD)(size1*size2), &count, NULL);
            if (ret)
                ret = count/size1;

    }
    PERFINFO_AUTO_STOP();
    return ret;
}

char *x_fgets(char *buf,int len,FileWrapper *fw)
{
    char* ret=NULL;
    switch(fw->iomode) {
        xcase IO_GZ:
            PERFINFO_AUTO_START("x_fgets:gzgets", 1);
            ret = gzgets(fw->fptr,buf,len);
        xcase IO_PIG:
            PERFINFO_AUTO_START("x_fgets:pig_fgets", 1);
            ret = pig_fgets(fw->fptr,buf,len);
        xcase IO_STRING:
            PERFINFO_AUTO_START("x_fgets:assert", 1);
            assert(!"StuffBuffFiles are write-only");
            ret = 0;
        xcase IO_CRT:
            PERFINFO_AUTO_START("x_fgets:fgets", 1);
            ret = fgets(buf,len,fw->fptr);
        xcase IO_WINIO:
            PERFINFO_AUTO_START("x_fgets:assert", 1);
            assertmsg(0, "fgets not supported on binary files");
            ret=0;

    }
    PERFINFO_AUTO_STOP();
    return ret;
}

int x_flock(FileWrapper* fw, int mode)
{
    int ret = -1;
    switch (fw->iomode) {
        xcase IO_CRT:
            if(mode & LOCK_UN) {
                PERFINFO_AUTO_START("x_flock(unlock):crt", 1);
                ret = UnlockFile((HANDLE)_get_osfhandle(_fileno(fw->fptr)), 0, 0, ~0, ~0) ? 0 : -1;
            } else if(mode & LOCK_EX) {
                PERFINFO_AUTO_START("x_flock(lock):crt", 1);
                assert(mode & LOCK_NB);
                ret = LockFile((HANDLE)_get_osfhandle(_fileno(fw->fptr)), 0, 0, ~0, ~0) ? 0 : -1;
            }
        xcase IO_WINIO:
            if(mode & LOCK_UN) {
                PERFINFO_AUTO_START("x_flock(unlock):winio", 1);
                ret = UnlockFile(fw->fptr, 0, 0, ~0, ~0) ? 0 : -1;
            } else if(mode & LOCK_EX) {
                PERFINFO_AUTO_START("x_flock(lock):winio", 1);
                assert(mode & LOCK_NB);            
                ret = LockFile(fw->fptr, 0, 0, ~0, ~0) ? 0 : -1;
            }
        xdefault:
            PERFINFO_AUTO_START("x_flock:assert", 1);
            assert(0);
    }
    PERFINFO_AUTO_STOP();
    return ret;
}

int x_fsync(FileWrapper *fw)
{
    int ret = -1;
    switch(fw->iomode) {
        xcase IO_GZ:
            PERFINFO_AUTO_START("x_fsync:gz", 1);
            ret = FlushFileBuffers((HANDLE)_get_osfhandle(_fileno(fileRealPointer(fw)))) ? 0 : -1;
        xcase IO_CRT:
            PERFINFO_AUTO_START("x_fsync:crt", 1);
            ret = FlushFileBuffers((HANDLE)_get_osfhandle(_fileno(fileRealPointer(fw)))) ? 0 : -1;
        xcase IO_WINIO:
            PERFINFO_AUTO_START("x_fsync:winio", 1);
            ret = FlushFileBuffers(fw->fptr) ? 0 : -1;
        xdefault:
            PERFINFO_AUTO_START("x_fsync:assert", 1);
            assert(0);
    }
    PERFINFO_AUTO_STOP();
    return ret;
}

int x_fflush(FileWrapper *fw)
{
    int ret = -1;
    switch(fw->iomode) {
        xcase IO_GZ:
            PERFINFO_AUTO_START("x_fflush:gz", 1);
            ret = gzflush(fw->fptr, Z_SYNC_FLUSH); // possibly could use Z_FULL_FLUSH instead here to trade compression ratio for corruption recovery
        xcase IO_CRT:
            PERFINFO_AUTO_START("x_fflush:crt", 1);
            ret = fflush(fw->fptr);
        xcase IO_WINIO:
            PERFINFO_AUTO_START("x_fflush:winio", 1);
            // nothing to do for this type
            ret = 0;
        xdefault:
            PERFINFO_AUTO_START("x_fflush:assert", 1);
            assert(0);
    }
    PERFINFO_AUTO_STOP();
    return ret;
}

int x_setvbuf(FileWrapper* fw, char *buffer, int mode, size_t size)
{
    if (fw->iomode!=IO_CRT) {
        return 0; // ignore
    } else {
        return setvbuf(fw->fptr, buffer, mode, size);
    }
}


int x_vfprintf(FileWrapper *fw, const char *format, va_list va)
{
    char* buf = NULL;
    int len;
    int ret;

    estrStackCreate(&buf, 1024);
    len = estrConcatfv(&buf, (char*)format, va);
    ret = x_fwrite(buf,1,len,fw);
    if(ret != len)
        ret = -1;
    estrDestroy(&buf);
    return ret;
}


int x_fprintf(FileWrapper *fw, const char *format, ...)
{
    int ret;

    VA_START(va, format);
    ret = x_vfprintf(fw, format, va);
    VA_END();

    return ret;
}

int x_ferror(FileWrapper* fw)
{
    switch(fw->iomode)
    {
        xcase IO_CRT:
#undef FILE
#undef ferror
        return ferror((FILE*)fw->fptr);
#define ferror
#define FILE FileWrapper
        xcase IO_GZ:
            return gzflush(fw->fptr,0);
        xcase IO_WINIO:
            return 0; // No way to check?  Or handles are never in an error state
        xcase IO_PIG:
        case IO_STRING:
        default:
            return 0;
    }
}

int fileTruncate(FileWrapper *fw, U64 newsize)
{
    switch(fw->iomode)
    {
        xcase IO_CRT:
            if (newsize > 0x7ffffffLL) {
                // Too large of a value!
                if (isDevelopmentMode())
                    assertmsg(0, "Too large of a file!");
                return 1;
            } else {
                return _chsize(_fileno(fw->fptr), (long)newsize);
            }
        xcase IO_GZ:
            assertmsg(0, "Not supported");
            return 1;
        xcase IO_WINIO:
            x_fseek(fw, newsize, SEEK_SET);
            SetEndOfFile(fw->fptr);
            return 0;
        xcase IO_PIG:
            assertmsg(0, "Not supported");
            return 1;
        xcase IO_STRING:
            assertmsg(0, "Not supported");
            return 1;
        xdefault:
            return 1;
    }
}


/* Function x_fscanf()
 *    A very basic scanf to be used with FileWrapper.  This function always extracts
 *    one string from the file no matter what "format" says.  In addition, this function
 *    will only compile against VC++, since it relies on one of the VC stdio internal
 *    functions, _input(), and the structure _iobuf.
 *
 *
 */
/*
extern int __cdecl _input(FILE *, const unsigned char *, va_list);
int x_fscanf(FileWrapper* fw, const char* format, ...){
va_list va;
#define bufferSize 1024
char buffer[bufferSize];
int retval;

    va_start(va, format);


    x_fgets(buffer, bufferSize, fw);

    // Adopted from VC++ 6's sscanf.
    {
        struct iobuf str;
        struct iobuf *infile = &str;

        _ASSERTE(buffer != NULL);
        _ASSERTE(format != NULL);

        infile->_flag = _IOREAD|_IOSTRG|_IOMYBUF;
        infile->_ptr = infile->_base = (char *) buffer;
        infile->_cnt = strlen(buffer);

        retval = (_input(infile,format, va));
    }

    va_end(va);
    return(retval);
}
*/


FILE *fileGetStdout(){
    static FileWrapper wrapper;

    if (isConsoleOpen() && IsUsingCider())
    {
        static FileWrapper* ciderWrapper = NULL;

        if (!ciderWrapper)
        {
            ciderWrapper = x_fopen("C:/ciderlog.txt", "wt");
            ciderWrapper->nameptr = "stdout";
        }

        return ciderWrapper;
    }

    // If the console is closed and we are using Cider then fallthrough should
    // be OK, since stdout doesn't seem to show up anywhere when running under
    // Cider.

    wrapper.iomode = IO_CRT;
    wrapper.fptr = stdout;
    //strcpy(wrapper.name, "stdout");
    wrapper.nameptr = "stdout";
    wrapper.nameptr_needs_freeing = 0;
    return &wrapper;
}


/**
 * @note This type should match gz_stream up to the file member.
 */
typedef struct gz_stream_hack {
    z_stream stream;
    int      z_err;
    int      z_eof;
    FILE     *file;
} gz_stream_hack;

void *fileRealPointer(FileWrapper *fw)
{
    if (!fw)
        return 0;
    switch (fw->iomode) {
        xcase IO_CRT:
            return fw->fptr;
        xcase IO_GZ:
            return ((gz_stream_hack*)fw->fptr)->file;
        xcase IO_PIG:
            return 0;
        xcase IO_STRING:
            return 0;
        xcase IO_WINIO:
            assert(0);
            return 0;
    }
    assert(0);
    return 0;
}

FileWrapper *fileWrap(void *real_file_pointer)
{
    static FileWrapper fw;
    fw.iomode = IO_CRT;
    fw.fptr = real_file_pointer;
    return &fw;
}

int fileGetSize(FileWrapper* fw){
    switch (fw->iomode) {
    xcase IO_PIG:
        return pig_filelength(fw->fptr);
    xcase IO_WINIO:
        return GetFileSize(fw->fptr, NULL);
    xcase IO_CRT:
        return _filelength(_fileno(fileRealPointer(fw)));
    xcase IO_GZ:
        return _filelength(_fileno(fileRealPointer(fw)));
    }
    assert(0);
    return 0;
}

void fileSetTimestamp(char* fullFilename, int timestamp){
#if 0
    struct _utimbuf    utb;
    utb.actime = utb.modtime = timestamp;
    _utime(fullFilename, &utb);
    t = fileLastChanged(fullFilename);
    dt = t - timestamp;
    printf("odt: %d   %s\n",dt,fullFilename);
#else
    _SetUTCFileTimes(fullFilename,timestamp,timestamp);
#endif

}

int fileMakeLocalBackup(const char *_fname, int time_to_keep) {
    // Make backup
    char backupPathBase[MAX_PATH];
    char backupPath[MAX_PATH];
    char temp[MAX_PATH];
    char fname[MAX_PATH];
    char dir[MAX_PATH];
    int backup_num=0;
    struct _finddata32_t fileinfo;
    int i;
    long handle;

    if (!_fname)
        return 0;
    strcpy(fname, _fname);
    if (fileIsAbsolutePath(fname)) {
        strcpy(temp, fileDataDir());
        forwardSlashes(temp);
        forwardSlashes(fname);
        if (strnicmp(temp, fname, strlen(temp))==0) {
            sprintf_s(SAFESTR(backupPathBase), "%c:/BACKUPS/%s.", temp[0], fname+strlen(temp));
        } else {
            sprintf_s(SAFESTR(backupPathBase), "%s.", fname);
        }
    } else { // relative
        sprintf_s(SAFESTR(backupPathBase), "%c:/BACKUPS/%s.", fileDataDir()[0], fname);
    }
    forwardSlashes(backupPathBase);
    strcpy(dir, backupPathBase);
    *(strrchr(dir, '/')+1)=0; // truncate before the file name
    mkdirtree(dir);
    sprintf_s(SAFESTR(backupPath), "%s*", backupPathBase);
    handle = _findfirst32(backupPath, &fileinfo);
    backup_num=0;
    if (handle==-1) {
        // Error or no file currently exists
        // will default to backup_num of 0, that's OK
    } else {
        // 1 or more files already exist
        do {
            // check to see if the backup number on this file is newer than backup_num
            i = atoi(strrchr(fileinfo.name, '.')+1);
            if (i>backup_num)
                backup_num = i;
            // check to see if file is old
            if (time_to_keep!=-1 && (fileinfo.time_write < time(NULL) - time_to_keep)) {
                // old file!  Delete it!
                sprintf_s(SAFESTR(temp), "%s%s", dir, fileinfo.name);
                remove(temp);
            }
        } while( _findnext32( handle, &fileinfo ) == 0 );
        backup_num++;
        _findclose(handle);

    }
    sprintf_s(SAFESTR(backupPath), "%s%d", backupPathBase, backup_num);
    return fileCopy(fileLocateWrite(fname, temp), backupPath);
}

bool fileDeleteBackup(char *fnStem, int version)
{
    bool res = false;
    int i;
    char fnBak[MAX_PATH];
    fileMakeBackupName(SAFESTR(fnBak),fnStem,version);

    // move down the files with higher version numbers
    // (no gaps)
    res = (0 == fileForceRemove(fnBak));
    for( i = version + 1;res; ++i )
    {
        char fnTmp[MAX_PATH];
        fileMakeBackupName(SAFESTR(fnTmp),fnStem,i);

        if( fileExists( fnTmp ))
        {
            if(!verify(MoveFileExA( fnTmp, fnBak, MOVEFILE_REPLACE_EXISTING )))
            {
                // something's wrong
                res = false;
                break;
            }
            strcpy( fnBak, fnTmp );
        }
        else
        {
            // all done
            res = true;
            break;
        }
    }
    return res;
}


bool fileExponentialBackupRec(char *fnStem, int version, __time32_t interval )
{
    bool res = true;
    char fn[MAX_PATH];
    __time32_t tHigh;

    fileMakeBackupName(SAFESTR(fn),fnStem,version);
    if(fileExists(fn) && _GetUTCFileTimes(fn, NULL, &tHigh, NULL))
    {
        char fnBak[MAX_PATH];
        char tlow[64], thigh[64], tbak[64];
        __time32_t tLow;
        __time32_t tBak = 0;

        // get the time after which no backups should be made
        tLow = tHigh - interval;

        if(isDevelopmentMode())
        {
            _ctime32_s(SAFESTR(tlow),&tLow);
            _ctime32_s(SAFESTR(thigh),&tHigh);
        }

        // check the next file. if it is within the range of this one, delete it.
        fileMakeBackupName(SAFESTR(fnBak),fnStem,version+1);
        while( fileExists(fnBak) && _GetUTCFileTimes(fnBak, NULL, &tBak, NULL))
        {
            if(isDevelopmentMode())
                _ctime32_s(SAFESTR(tbak),&tBak);

            if(interval && tBak >= tLow && tBak <= tHigh) //
                fileDeleteBackup(fnStem,version+1);
            else
                break;
        }

        //recursive backup call for the one overwritten at this level
        // and this one can be safely overwritten
        interval *= 2;
        res = fileExponentialBackupRec(fnStem, version+1, interval );
        res = res && CopyFileA( fn, fnBak, FALSE );
    }

    // --------------------
    // finally

    return res;
}

bool fileExponentialBackup( char *fn, U32 intervalSeconds )
{
    int version = 0;
    bool res = false;

    if (fileExists(fn))
    {
        fileWaitForExclusiveAccess(fn);
        res = fileExponentialBackupRec(fn, version, intervalSeconds );
    }

    // --------------------
    // finally

    return res;
}

char *fileMakeBackupName(char *out, int out_len, char *stem, int version)
{
    if(0==version)
        strcpy_s(out,out_len,stem);
    else
        sprintf_s(out,out_len, "%s~%.3d~", stem, version);
    return out;
}

bool fileMoveToBackup(char *fn)
{
    char buf[MAX_PATH], buf2[MAX_PATH];
    int i,nBackups;
    for( i = 0; fileExists(fileMakeBackupName(SAFESTR(buf),fn,i)); ++i ) {};

    nBackups = i;

    for( i = nBackups; i > 0; --i )
    {
        fileMakeBackupName(SAFESTR(buf),fn,i-1);
        fileMakeBackupName(SAFESTR(buf2),fn,i);
        if(0!=fileMove(buf,buf2))
            break;
    }
    return i == 0;
}


S64 fileSize64(const char *fname)  // Slow!  Does not use FileWatcher.
{
    struct __stat64 status;
    char buf[MAX_PATH];

    fileLocateWrite(fname, buf);

    if(!_stat64(buf, &status)){
        if(!(status.st_mode & _S_IFREG)) {
            return -1;
        }
    } else {
        return -1;
    }
    return status.st_size;
}

intptr_t fileSize(const char *fname){
    struct _stat32 status;

    if (!loadedGameDataDirs)
        fileLoadDataDirs(0);

    assert(!is_pigged_path(fname)); // expects /bin/tricks.bin, instead of ./piggs/bin.pigg:/bin/tricks.bin, for example

    if (!fileIsAbsolutePath(fname)) { // Relative path
        FolderNode *ret = FolderCacheQuery(folder_cache, fname);
        if (ret) {
            return ret->size;
        }
        return -1;
    }

#ifdef _XBOX
    {
        char fullname[MAX_PATH];
        if (fileFixupForXBox((char *)fname, 0, fullname, ARRAY_SIZE_CHECKED(fullname)))
            fname = fullname;
    }
#endif

    // Absolute path:
    if( !fwStat(fname, &status) &&
        status.st_mode & _S_IFREG)
    {
        return status.st_size;
    }

    // The file doesn't exist.
    return -1;
}

int fileExists(const char *fname){
    struct _stat32 status;

    if(!fname)
        return 0;

    if (!fileIsAbsolutePath(fname))
    {
        int            exists=0;
        FolderNode *ret;

        if (!loadedGameDataDirs)
            fileLoadDataDirs(0); // Don't do this outside of the if statement to allow fileAutoDataDir to work

        ret = FolderCacheQuery(folder_cache, fname);
        if (ret)
        {
            if (!ret->is_dir)
                exists = 1;
            //FolderNodeDestroy(ret);
        }
        return exists;
    }

    if (FolderCacheMatchesIgnorePrefixAnywhere(fname)) // Just in case we get a non-absolute path in the game/mapserver
        return 0;

#ifdef _XBOX
    {
        char fullname[MAX_PATH];
        if (fileFixupForXBox((char *)fname, 0, fullname, ARRAY_SIZE_CHECKED(fullname)))
            fname = fullname;
    }
#endif

    if(!fwStat(fname, &status)){
        if(status.st_mode & _S_IFREG)
            return 1;
    }

    return 0;
}

bool wfileExists(const wchar_t *fname)
{
    bool res = fileExists( WideToUTF8StrTempConvert( fname, NULL ));

    // try the wstat
    if( !res )
    {
        struct _stat status = {0};

        if(!_wstat(fname, &status)){
            if(status.st_mode & _S_IFREG)
                res = true;
        }
    }

    // ----------
    // finally

    return res;
}


int dirExists(const char *dirname){
    struct _stat32 status;
    char    *s,buf[1000];

    //if (!loadedGameDataDirs)
    //    fileLoadDataDirs(0);

    strcpy(buf,dirname);
    s = &buf[strlen(buf)-1];
    if (*s == '/' || *s=='\\')
        *s = 0;

    if (loadedGameDataDirs && !fileIsAbsolutePath(buf)) {
        FolderNode *ret = FolderCacheQuery(folder_cache, buf);
        return ret && ret->is_dir;
    }

#ifdef _XBOX
    backSlashes(buf);
#endif

    if(!fwStat(buf, &status)){
        if(status.st_mode & _S_IFDIR)
            return 1;
    }

    return 0;
}

/* is refname older than testname?
*/
int fileNewer(const char *refname,const char *testname)
{
    __time32_t ref, test;
    __time32_t t;

    ref = fileLastChanged(refname);
    test = fileLastChanged(testname);

    if (test <= 0 || ref <= 0)
        return 1;

       t = test - ref;
    return t > 0;
}

int fileNewerOrEqual(const char *refname,const char *testname)
{
    __time32_t ref, test;
    __time32_t t;

    ref = fileLastChanged(refname);
    test = fileLastChanged(testname);

    if (test <= 0 || ref <= 0)
        return 1;

       t = test - ref;
    return t >= 0;
}

__time32_t fileLastChangedAltStat(const char *refname)
{
    __time32_t    atime,mtime,ctime;

    if (!_GetUTCFileTimes(refname,&atime,&mtime,&ctime))
        return 0;
    return mtime;
}

bool fileLastChangedWindows(const char *refname, SYSTEMTIME *systemtime)
{
    HANDLE hFile;
    bool success;
    FILETIME filetime;

    if ((hFile = CreateFileA(refname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL)) != INVALID_HANDLE_VALUE)
    {
        success = GetFileTime(hFile, NULL, NULL, &filetime) && FileTimeToSystemTime(&filetime, systemtime);

        CloseHandle(hFile);
    }
    else
    {
        success = false;
    }

    return success;
}

// Hack to allow file dates to be considered equal if they are exactly 3600 seconds different.
//    There is code all over that is already doing this, so trying to make this allowance more explicit
//    by using this function instead of having the 3600 difference check everywhere.  See statTimeToUTC
//    which explains that there is a bug in Microsoft UTC time that requires DST adjustment, and apparently
//    we are not taking care of this properly everywhere so need this extra check here.
bool fileDatesEqual(__time32_t date1, __time32_t date2, bool bAllowDSTError)
{
    return(date1 == date2 ||
        (bAllowDSTError && ABS_UNS_DIFF(date1, date2)==3600));
}

__time32_t fileLastChanged(const char *refname)
{
    FolderNode *node;
    char temp[MAX_PATH];

    if (!refname || !refname[0])
        return 0;

    PERFINFO_AUTO_START("fileLastChanged", 1);

    if (!loadedGameDataDirs)
        fileLoadDataDirs(0);

    strcpy(temp, refname);
    forwardSlashes(temp);
    refname = temp;

    if (is_pigged_path(refname)) {
        printf("Pigged file passed to fileLastChanged, need to implement support for this\n");
        PERFINFO_AUTO_STOP();
        return 0;
    }
    if (refname[1]==':' || refname[0]=='.' && fileDataDir()[0]=='.')
    {
        if (strnicmp(refname, fileDataDir(), strlen(fileDataDir()))==0 && 0 != stricmp( refname, fileDataDir() ) ) { //10.7.05 MW check to see if you are looking at the actual fileDataDir
            // This happens in utilities, and when reading directly from .pigg/.hogg files,
            //   but not in normal game/server code
            // This could just as well be a relative path
            refname = refname + strlen(fileDataDir());
        } else { // Absolute path outside of game data dir
        }
    }
    if (fileIsAbsolutePath(refname)) {
        struct _stat32 sbuf;
        __time32_t ret;
        PERFINFO_AUTO_START("fwStat", 1);
        fwStat(refname, &sbuf);
        ret = statTimeToUTC(sbuf.st_mtime);
        PERFINFO_AUTO_STOP();
        PERFINFO_AUTO_STOP();
        return ret;
    }
    node = FolderCacheQuery(folder_cache, refname);

    PERFINFO_AUTO_STOP();

    if (!node) return 0;
    return node->timestamp;
}

void *fileLockRealPointer(FileWrapper *fw) { // This implements support for Pig files, and must be followed by an unlock when done
    if (fw->iomode==IO_PIG)
        return pig_lockRealPointer(fw->fptr);
    return fileRealPointer(fw);
}

void fileUnlockRealPointer(FileWrapper *fw) {
    if (fw->iomode==IO_PIG)
        pig_unlockRealPointer(fw->fptr);
}

#ifndef FINAL
static int fileIsUsingDevDataInternal(void) {
    struct stat statbuf;
    return (-1==stat("./piggs/texts.pigg", &statbuf)); // which pigg file we choose here is arbitrary, needs to be a core required pigg tho
}

int fileIsUsingDevData(void) { // Returns 1 if we're in development mode, 0 if in production
    static int cached=-1;
    if (cached==-1)
        cached = fileIsUsingDevDataInternal();
    return cached;
}

// Note: we do not want to cache these because they may be called before
// and after command line parameters are parsed which will change the
// value of g_force_production_mode.
int g_force_production_mode = 0;
int g_force_qa_mode = 0;
int isDevelopmentMode(void)
{
    return !g_force_production_mode && fileIsUsingDevData();
}
int isDevelopmentOrQAMode(void)
{
    return !g_force_production_mode && (g_force_qa_mode || fileIsUsingDevData());
}
int isProductionMode(void)
{
    return g_force_production_mode || !fileIsUsingDevData();
}
#endif

int filePathCompare(const char* path1, const char* path2)
{
    char p1[MAX_PATH];
    char p2[MAX_PATH];

    if(!path1)
        return -1;
    if(!path2)
        return 1;

    fileFixUpName(path1, p1);
    fileFixUpName(path2, p2);
    return strcmp(p1, p2);
}

int filePathBeginsWith(const char* fullPath, const char* prefix)
{
    char p1[MAX_PATH];
    char p2[MAX_PATH];

    fileFixUpName(fullPath, p1);
    fileFixUpName(prefix, p2);
    return strncmp(p1, p2, strlen(p2));
}

U64 fileGetFreeDiskSpace(const char* fullPath)
{
    ULARGE_INTEGER freeBytesAvailableToUser;
    ULARGE_INTEGER totalBytesOnDisk;
    char path[MAX_PATH];

    strcpy(path, fullPath);
    getDirectoryName(path);

    if(!GetDiskFreeSpaceExA(path, &freeBytesAvailableToUser, &totalBytesOnDisk, NULL))
        return 0;

    return freeBytesAvailableToUser.QuadPart;
}

void fileFreeOldZippedBuffers(void)
{
    pig_freeOldBuffers();
}

void fileFreeZippedBuffer(FileWrapper *fw)
{
    if (fw->iomode == IO_PIG) {
        pig_freeBuffer(fw->fptr);
    }
}

#ifndef _XBOX
void fileSetDataDirHack( char * path )
{
    FolderCacheSetMode( FOLDER_CACHE_MODE_FILESYSTEM_ONLY );

    eaClear( &folder_cache->gamedatadirs );
    eaSetSize( &folder_cache->gamedatadirs,0 );
    eaPush( &folder_cache->gamedatadirs, path );
}
#endif


bool fileChecksum(char *fn,U32 checksum[4])
{
    int len;
    U8 *mem = fileAlloc(fn,&len);
    if(!mem)
        return false;
    cryptMD5Update(mem,len);
    cryptMD5Final(checksum);
    free(mem);
    return true;
}

bool fileWriteChecksum(char *fn)
{
    U32 checksum[4];
    char *str = NULL;
    FileWrapper *fp;

    if(!fileChecksum(fn,checksum))
        return false;

    estrPrintf(&str,"%s.checksum",fn);
    fp = x_fopen(str,"w");
    if(fp)
    {
        int i;
        for( i = 0; i < ARRAY_SIZE(checksum); ++i )
            x_fprintf(fp,"%u\n",checksum[i]);
        x_fflush(fp);
        x_fclose(fp);
    }
    estrDestroy(&str);
    return true;
}

bool fileChecksumfileMatches(char *fn)
{
    U32 checksum[4];
    char *str = NULL;
    int len;
    char *buf;
    bool res = false;

    if(!fileChecksum(fn,checksum))
        return false;

    estrPrintf(&str,"%s.checksum",fn);
    buf = fileAlloc(str,&len);
    if(buf)
    {
        int i;
        char *next_tok = NULL;
        char *cs = strtok_s(buf,"\n",&next_tok);
        for( i = 0; i < ARRAY_SIZE(checksum); ++i )
        {
            if(!cs || checksum[i]!=(U32)atoi(cs))
                break;
            cs = strtok_s(NULL,"\n",&next_tok);
        }
        if(i == ARRAY_SIZE( checksum ))
            res = true;
        free(buf);
    }
    estrDestroy(&str);
    return res;
}

int fileRemoveChecksum(char *fn)
{
    int res;
    char *str = NULL;
    estrPrintf(&str,"%s.checksum",fn);
    res = fileForceRemove(str);
    estrDestroy(&str);
    return res;
}

int fileMoveChecksum(char *src, char *dst)
{
    char *src_str = NULL;
    char *dst_str = NULL;
    int res;

    estrPrintf(&src_str,"%s.checksum",src);
    estrPrintf(&dst_str,"%s.checksum",dst);
    res = fileMove(src_str,dst_str);
    estrDestroy(&dst_str);
    estrDestroy(&src_str);
    return res;
}
