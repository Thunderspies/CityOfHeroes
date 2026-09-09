#include "patchfileutils.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/utils.h>
#include "string.h"
#include <zlib.h>
#include <utilitieslib/network/crypt.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/assert/assert.h>
#include <direct.h>
#include <sys/stat.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/network/netio.h>

static char    **file_names;
static int    file_count,file_max;
FatalAlertCallback fatalAlertCallback = NULL;

#include "windows.h"
#if PATCHCLIENT
extern HWND    install_dialog;
#else
static HWND    install_dialog=0;
#endif

void fileMsgAlert(char const *fmt, ...)
{
    char    str[1000];
    int        ok;

    VA_START(arg, fmt);
    vsprintf(str, fmt, arg);
    VA_END();

    //xlatePrint(str + strlen(str), ARRAY_SIZE(str) - strlen(str), "ErrFileMsgAlert");
    strcpy_s(str + strlen(str), ARRAY_SIZE(str) - strlen(str), "ErrFileMsgAlert");
    ok = MessageBoxA( install_dialog, str, "ErrFileTitle", MB_OKCANCEL | MB_ICONQUESTION) == IDOK;
    if (!ok)
        exit(0);
}

void msgAlertUpdater(char const *fmt, ...)
{
    char str[1000];

    VA_START(arg, fmt);
    vsprintf(str, fmt, arg);
    VA_END();

    MessageBoxA( install_dialog, str, "ErrTitle", MB_ICONERROR);
}

void setFatalAlertCallback(FatalAlertCallback callback)
{
    fatalAlertCallback = callback;
}

void msgAlertFatal(char const *fmt, ...)
{
    char str[1000];

    VA_START(arg, fmt);
    vsprintf(str, fmt, arg);
    VA_END();

    MessageBoxA( install_dialog, str, "ErrFatalTitle", MB_ICONERROR);
    
    if ( fatalAlertCallback )
        fatalAlertCallback(str);

    exit(0);
}

void safeMkDirTree(const char *dir)
{
    char    *s,temp_dir[MAX_PATH];

    for(;;)
    {
        makefullpath(dir,temp_dir);
        s = strrchr(temp_dir,'/');
        if (s)
            *s = 0;
        if(strlen(temp_dir)==2 && temp_dir[1] == ':')
            return;
        if (makeDirectories(temp_dir))
            return;
        fileMsgAlert("ErrCantCreateDir",temp_dir);
    }
}

int makeUserDir(char *dir)
{
    char    *s,temp_dir[MAX_PATH];

    makefullpath(dir,temp_dir);
    s = strrchr(temp_dir,'/');
    if (s)
        *s = 0;
    if (makeDirectories(temp_dir))
        return 1;
    return 0;
}

FILE *safeFopen(const char *fname,const char *how)
{
    FILE    *file;

    while(!(file = fopen(fname,how)))
        fileMsgAlert("ErrOpenFile",fname);
    return file;
}

void safeFileSetTimestamp(char *fname,U32 timestamp)
{
    chmod(fname,_S_IREAD | _S_IWRITE);
    fileSetTimestamp(fname,timestamp);
    chmod(fname,_S_IREAD);
}

void safeCopyFile(char *src,char *dst)
{
    U8        *mem;
    int        size;

    if (stricmp(src,dst)==0)
        return;
    mem = extractFromFS(src,&size);
    if (!mem)
        msgAlertFatal("ErrOpenFile",src);
    safeWriteFile(dst,"wb",mem,size);
    free(mem);
}

void safeDeleteFile(char *fname)
{
    if (!fileExists(fname))
        return;
    chmod(fname,_S_IREAD | _S_IWRITE);
    if (unlink(fname) != 0)
    {
        msgAlertUpdater("ErrDeleteFile",fname);
        exit(0);
    }
}

void safeWriteBytes(FILE *file,U32 pos,void *buf,U32 count)
{
    fseek(file,pos,0);
    if (ftell(file) != pos)
        msgAlertFatal("ErrSeek");
    while(fwrite(buf,1,count,file) != (int)count)
        fileMsgAlert("ErrWrite",count,file->nameptr);
}

void safeWriteFile(const char *fname,char *how,void *mem,U32 len)
{
    FILE    *file;

    safeMkDirTree(fname);
    chmod(fname,_S_IREAD|_S_IWRITE);
    file = safeFopen(fname,how);
    safeWriteBytes(file,0,mem,len);
    fclose(file);
    chmod(fname,_S_IREAD);
}

void safeRenameFile(char *oldnamep,char *newnamep)
{
    char    oldname[MAX_PATH],newname[MAX_PATH];

    strcpy(oldname,oldnamep);
    strcpy(newname,newnamep);
    backSlashes(oldname);
    backSlashes(newname);
    chmod(oldname,_S_IREAD | _S_IWRITE);
    if (rename(oldname,newname) == 0)
        return;
    if (!fileExists(newname))
    {
        goto fail;
    }
    chmod(newname,_S_IREAD | _S_IWRITE);
    if (unlink(newname) != 0)
    {
        if (strEndsWith(newname,".exe"))
        {
            char    *s,new_exename[MAX_PATH];

            strcpy(new_exename,newname);
            s = strrchr(new_exename,'.');
            strcpy(s,".old");
            safeRenameFile(newname,new_exename);
        }
        else
            msgAlertFatal("ErrRenameDelete",newname);
    }
    if (rename(oldname,newname) == 0)
        return;
    fail:
    msgAlertFatal("ErrRenameFailed",oldname,newname);
}

U32 safeFileSize(char *fname)
{
    char    fullname[MAX_PATH];

    makefullpath(fname,fullname);
    return fileSize(fullname);
}

static void getFileNames(const char *fname)
{
    struct _finddata_t fileinfo;
    int        handle,test;
    char    buf[MAX_PATH];

    sprintf(buf,"%s/*",fname);
    for(test = handle = _findfirst(buf,&fileinfo);test >= 0;test = _findnext(handle,&fileinfo))
    {
        if (fileinfo.name[0] == '.'
            || stricmp(fileinfo.name,"updater.lock")==0
            || strEndsWith(fileinfo.name,".patch")
            || strEndsWith(fileinfo.name,".checksum")
            || strEndsWith(fileinfo.name,".majorpatch"))
            continue;
        sprintf(buf, "%s/%s", fname, fileinfo.name);
        if (fileinfo.attrib & _A_SUBDIR)
            getFileNames(buf);
        else
            dynArrayAddp(&file_names,&file_count,&file_max,strdup(buf));
    }
    _findclose(handle);
}

static void getPatchFileNames(const char *fname)
{
    struct _finddata_t fileinfo;
    int        handle,test;
    char    buf[MAX_PATH];

    sprintf(buf,"%s/*",fname);
    for(test = handle = _findfirst(buf,&fileinfo);test >= 0;test = _findnext(handle,&fileinfo))
    {
        if (fileinfo.name[0] == '.')
            continue;
        sprintf(buf, "%s/%s", fname, fileinfo.name);
        if (fileinfo.attrib & _A_SUBDIR)
        {
            getPatchFileNames(buf);
            continue;
        }
        if (!strEndsWith(fileinfo.name,".patch"))
            continue;
        dynArrayAddp(&file_names,&file_count,&file_max,strdup(buf));
    }
    _findclose(handle);
}

int removeEmptyDirs(char *fname)
{
    struct _finddata_t fileinfo;
    int        handle,test,count=0;
    char    buf[MAX_PATH];

    sprintf(buf,"%s/*",fname);
    for(test = handle = _findfirst(buf,&fileinfo);test >= 0;test = _findnext(handle,&fileinfo))
    {
        if (fileinfo.name[0] == '.')
            continue;
        sprintf(buf, "%s/%s", fname, fileinfo.name);
        if (fileinfo.attrib & _A_SUBDIR)
        {
            file_count = removeEmptyDirs(buf);
            if (!file_count)
                rmdir(buf);
            count+=file_count;
        }
        else
            count++;
    }
    _findclose(handle);
    return count;
}

char **getDirFiles(const char *dir,int *count)
{
    file_names = 0;
    file_count = file_max = 0;
    getFileNames(dir);
    *count = file_count;
    return file_names;
}

char **getDirPatches(const char *dir,int *count)
{
    file_names = 0;
    file_count = file_max = 0;
    getPatchFileNames(dir);
    *count = file_count;
    return file_names;
}

void freeDirFiles(char **fnames,int count)
{
    int        i;

    for(i=0;i<count;i++)
        free(fnames[i]);
    free(fnames);
}

typedef struct
{
    U32        size;
    U32        modified;
} QuickFileInfo;

static StashTable        quick_file_table;
static QuickFileInfo    *quick_file_list;
static int                quick_file_count,quick_file_max,quick_file_time_offset=0, time_offset_is_set=0;


static int pstGetTimeAdjust(void) { // returns the amount that needs to be added to the result of a _stat call to sync up with PST
    int adjust = 0;
    FILETIME ft, ft2;
    SYSTEMTIME st;
    BOOL b;
    ft.dwHighDateTime=29554851;  // 15:09 PST
    ft.dwLowDateTime=3306847104;

    b = FileTimeToLocalFileTime(&ft, &ft2);
    b = FileTimeToSystemTime(&ft2, &st);
    if (st.wHour == 15) {
        //ok
        adjust=0;
    } else {
        adjust=3600*(15 - st.wHour);
    }
    return adjust;
}


FileScanAction dealWithFile(char *dir,struct _finddata32_t* fileinfo)
{
    char            buf[MAX_PATH];
    QuickFileInfo    *info;
    struct tm        t;

    if (fileinfo->name[0] == '.'
        || stricmp(fileinfo->name,"updater.lock")==0
        || strEndsWith(fileinfo->name,".patch")
        || strEndsWith(fileinfo->name,".checksum")
        || strEndsWith(fileinfo->name,".majorpatch"))
        return FSA_EXPLORE_DIRECTORY;
    sprintf(buf, "%s/%s", dir, fileinfo->name);
    info = dynArrayAdd(&quick_file_list,sizeof(quick_file_list[0]),&quick_file_count,&quick_file_max,1);
    // get the offset for adjusting the times
    if ( !time_offset_is_set )
    {
        quick_file_time_offset = pstGetTimeAdjust();
        time_offset_is_set = 1;
    }
    // add the offset if we arent in daylight savings time
    _localtime32_s(&t, &fileinfo->time_write);
    if ( t.tm_isdst)
        info->modified = fileinfo->time_write + quick_file_time_offset + 3600;
    else
        info->modified = fileinfo->time_write + quick_file_time_offset;
    info->size = fileinfo->size;
    stashAddInt(quick_file_table,buf,quick_file_count-1, false);
    return FSA_EXPLORE_DIRECTORY;
}

void cacheQuickFileInfo(const char* dir)
{
    if (quick_file_table)
    {
        stashTableDestroy(quick_file_table);
        quick_file_count = quick_file_max = 0;
        free(quick_file_list);
        quick_file_list = 0;
        quick_file_table = 0;
    }
    quick_file_table = stashTableCreateWithStringKeys(10000,StashDeepCopyKeys);
    fileScanDirRecurseEx(dir, dealWithFile);
}

int quickFileInfo(char *fname,U32 *size,U32 *modified)
{
    int        idx;

    if (!stashFindInt(quick_file_table,fname,&idx))
        return 0;
    *size = quick_file_list[idx].size;
    *modified = quick_file_list[idx].modified;
    return 1;
}

char **getImageDirs(char *imagedir,int *count_p)
{
    char        *s,**names,**image_names = 0;
    int            i,count,image_count=0,image_max=0;

    //sprintf(imagedir,"images/%s",dir);
    names = getDirFiles(imagedir,&count);
    for(i=0;i<count;i++)
    {
        s = strstri(names[i],imagedir);
        s += strlen(imagedir);
        s = strchr(s+1,'/');
        if ( s )
            *s = 0;
        if (!image_names || stricmp(image_names[image_count-1],names[i])!=0)
            dynArrayAddp(&image_names,&image_count,&image_max,strdup(names[i]));
    }
    freeDirFiles(names,count);
    if (image_count)
    {
        char    zero_dir[MAX_PATH];

        strcpy(zero_dir,image_names[0]);
        getDirectoryName(zero_dir);
        strcat(zero_dir,"/0");
        dynArrayAddp(&image_names,&image_count,&image_max,strdup(zero_dir));
    }
    *count_p = image_count;
    return image_names;
}

char *getRootPathFromImage(const char *image_dir,char *path)
{
    char *s;

    strcpy(path,image_dir);
    s = strrchr(path,'/');
    *s = 0;
    s = strrchr(path,'/');
    *s = 0;
    return path;
}

static PigEntry *pigCacheFindOrAdd(PigCache *cache,char *image_dir,char *pigname)
{
    char        actual_name[MAX_PATH];
    PigEntry    *entry;
    int            idx;

    sprintf(actual_name,"%s/%s",image_dir,pigname);
    if (!cache->pig_entry_hashes)
        cache->pig_entry_hashes = stashTableCreateWithStringKeys(100,StashDeepCopyKeys);
    if (!stashFindInt(cache->pig_entry_hashes,actual_name,&idx))
    {
        idx = cache->pig_entry_count;
        stashAddInt(cache->pig_entry_hashes,actual_name,idx, false);
        entry = dynArrayAdd(&cache->pig_entries,sizeof(cache->pig_entries[0]),&cache->pig_entry_count,&cache->pig_entry_max,1);
        entry->name = strdup(actual_name);
        if (PigFileRead(&entry->pig,actual_name) != 0)
            return 0;
    }
    return &cache->pig_entries[idx];
}

PigFileHeader *getPfh(PigCache *cache,char *image_dir,char *fname,char *pigname)
{
    PigEntry        *entry;
    PigFileHeader    *pfh=0;

    if (!pigname)
        return 0;
    entry = pigCacheFindOrAdd(cache,image_dir,pigname);
    if (entry)
        pfh = PigFileFind(&entry->pig, fname);
    return pfh;
}

U8 *loadFileData(PigCache *cache,char *image_dir,char *fname,char *pigname,U32 *size,int get_compressed)
{
    char        actual_name[MAX_PATH];
    PigEntry    *entry;

    if (!pigname)
    {
        if (get_compressed)
            return 0;
        sprintf(actual_name,"%s/%s",image_dir,fname);
        return extractFromFS(actual_name, size);
    }
    entry = pigCacheFindOrAdd(cache,image_dir,pigname);
    if (!entry)
        return 0;
    if (get_compressed)
        return PigFileExtractCompressed(&entry->pig,fname,size);
    return PigFileExtract(&entry->pig,fname,size);
}

void closeOpenPigs(PigCache *cache)
{
    int        i;

    for(i=0;i<cache->pig_entry_count;i++)
    {
        PigFileDestroy(&cache->pig_entries[i].pig);
        free(cache->pig_entries[i].name);
        memset(&cache->pig_entries[i].pig,0,sizeof(cache->pig_entries[i].pig));
    }
    if (cache->pig_entry_hashes)
        stashTableDestroy(cache->pig_entry_hashes);
    cache->pig_entry_hashes = 0;
    cache->pig_entry_count = cache->pig_entry_max = 0;
    free(cache->pig_entries);
    cache->pig_entries = 0;
}

static void gzStreamInit(GzStream *gz,FILE *file)
{
    memset(gz,0,sizeof(*gz));
    gz->chunk = calloc(GZ_CHUNK_SIZE,1);
    gz->file = file;
}

void gzStreamDecompressInit(GzStream *gz,FILE *file)
{
    gzStreamInit(gz,file);
    gz->compress = 0;
    inflateInit(&gz->z);
}

void gzStreamCompressInit(GzStream *gz,FILE *file)
{
    gzStreamInit(gz,file);
    gz->compress = 1;
    deflateInit(&gz->z,Z_DEFAULT_COMPRESSION);
}

void gzStreamFree(GzStream *gz)
{
    if (gz->compress)
        gzWriteBytes(gz,0,0,2);
    if(gz->compress)
        deflateEnd(&gz->z);
    else
        inflateEnd(&gz->z);
    free(gz->chunk);
    gz->chunk = 0;
}

int gzGetBytes(GzStream *gz,int count,U8 *buf,int dont_unpack)
{
    U32            num,bytesread;
    z_stream    *z = &gz->z;

    z->avail_out = count;
    z->next_out = buf;
    while(z->avail_out)
    {
        if (!z->avail_in)
        {
            z->next_in = gz->chunk;
            bytesread = fread(gz->chunk,1,GZ_CHUNK_SIZE,gz->file);
            if (!bytesread)
                return 0;
            z->avail_in = bytesread;
        }
        if (dont_unpack)
        {
            num = MIN(z->avail_in,z->avail_out);
            memcpy(z->next_out,z->next_in,num);
            z->avail_in -= num;
            z->avail_out -= num;
            z->next_in += num;
            z->next_out += num;
        }
        else
            inflate(z,Z_SYNC_FLUSH);
    }
    return 1;
}

int gzWriteBytes(GzStream *gz,int count,U8 *buf,int flush)
{
    U32            bytes;
    z_stream    *z = &gz->z;
    int            z_flush=0;

    z->avail_in = count;
    z->next_in = buf;
    while(flush || z->avail_in)
    {
        z->avail_out = GZ_CHUNK_SIZE;
        z->next_out = gz->chunk;
        if (flush == 1)
            z_flush = Z_SYNC_FLUSH;
        if (flush == 2)
            z_flush = Z_FINISH;
        deflate(z,z_flush);
        bytes = GZ_CHUNK_SIZE - z->avail_out;
        safeWriteBytes(gz->file,ftell(gz->file),gz->chunk,bytes);
        if (flush)
            return 1;
    }
    return 1;
}

void removeDirAll(char *dir)
{
    char    **names;
    int        count,i;

    names = getDirFiles(dir,&count);
    for(i=0;i<count;i++)
    {
        //printf(" Deleting: %s\n",names[i]);
        safeDeleteFile(names[i]);
    }        
    freeDirFiles(names,count);
    removeEmptyDirs(dir);
    rmdir(dir);
}

static FILE *lock_file;
static char lock_name[MAX_PATH];

int makeLockFile(char *dst_dir)
{
    if (!lock_file)
    {
        sprintf(lock_name,"%s/updater.lock",dst_dir);
        if (fileExists(lock_name))
        {
            chmod(lock_name,_S_IREAD | _S_IWRITE);
            if (unlink(lock_name) != 0)
                return 0;
        }
        lock_file = fopen(lock_name,"wb");
    }
    return 1;
}

void cleanupLockFile()
{
    if (lock_file)
    {
        fclose(lock_file);
        lock_file = NULL;
    }

    unlink(lock_name);
}


// if there are no periods, it assumes the entire string is a number and returns it as an int
int getUpdateVersion(char *build_name)
{
    if ( build_name )
    {
        char tmp[32], *tmpPtr;

        strcpy( tmp, build_name );
        tmpPtr = tmp;
        // find the first period and make it a null
        tmpPtr = strchr(tmp, '.');
        if (tmpPtr)
            *tmpPtr = 0;
        return atoi(tmp);
    }

    return -1;
}

// build_name is in the format "##.########.##"
// this function returns the number after the first period and before the second as an int
int getUpdateVersionDate(char *build_name)
{
    if ( build_name )
    {
        char tmp[32], *tmpPtr;

        strcpy( tmp, build_name );
        tmpPtr = strchr(tmp, '.');
        if (!tmpPtr)
            return -1;
        tmpPtr++;
        strcpy(tmp, tmpPtr);
        // find the first period and make it a null
        tmpPtr = strchr(tmp, '.');
        if (!tmpPtr)
            return -1;
        *tmpPtr = 0;
        return atoi(tmp);
    }

    return -1;
}

void stripAllButNumbers( char *str )
{
    for( ; *str; ) {
        if ( *str < '0' || *str > '9' ) strcpy(&str[0], &str[1]);
        else ++str;
    }
}

// build_name is in the format "##.########.##"
// this function returns the number after the last period as an int
int getUpdateVersionIncrement(char *build_name)
{
    if ( build_name )
    {
        char tmp[32], *tmpPtr;

        strcpy( tmp, build_name );
        tmpPtr = strrchr(tmp, '.');
        if (!tmpPtr)
            return -1;
        strcpy(tmp, tmpPtr);
        // strip letters from tmp (for builds with translation tags in the build name)
        stripAllButNumbers(tmp);
        return atoi(tmp);
    }

    return -1;
}

int compareUpdateVersion(char *build_name_1, char *build_name_2)
{
    int b1_version = getUpdateVersion(build_name_1), b2_version = getUpdateVersion(build_name_2);
    if ( b1_version > b2_version )
        return 1;
    else if ( b1_version < b2_version )
        return -1;
    // build versions are equal
    else
    {
        int b1_date = getUpdateVersionDate(build_name_1), b2_date = getUpdateVersionDate(build_name_2);
        if ( b1_date > b2_date )
            return 1;
        else if ( b1_date < b2_date )
            return -1;
        // build dates are equal
        else
        {
            int b1_inc = getUpdateVersionIncrement(build_name_1), b2_inc = getUpdateVersionIncrement(build_name_2);
            if ( b1_inc > b2_inc )
                return 1;
            else if ( b1_inc < b2_inc )
                return -1;
            // build increments are equal
            else 
                return 0;
        }
    }
}

