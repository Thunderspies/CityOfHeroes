#ifndef _PATCHFILEUTILS_H
#define _PATCHFILEUTILS_H

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/piglib.h>
#include <utilitieslib/utils/piglib_internal.h>
#include <zlib.h>

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;

typedef struct
{
    char    *name;
    PigFile    pig;
} PigEntry;

typedef struct
{
    StashTable pig_entry_hashes;
    PigEntry *pig_entries;
    int pig_entry_count,pig_entry_max;
} PigCache;

typedef struct
{
    U8        *zip_data;
    U32        pack_size;
    U32        unpack_size;
} ZipData;

typedef struct
{
    z_stream    z;
    FILE        *file;
    U8            *chunk;
    U32            compress : 1;
} GzStream;
#define GZ_CHUNK_SIZE (64 << 20)

typedef void (*FatalAlertCallback)(char *);

char **getDirFiles(const char *dir,int *count);
char **getDirPatches(const char *dir,int *count);
void freeDirFiles(char **fnames,int count);
extern void *extractFromFS(const char *name, U32 *count);
char **getImageDirs(char *dir,int *count_p);
char *getRootPathFromImage(const char *image_dir,char *path);
U8 *loadFileData(PigCache *cache,char *image_dir,char *fname,char *pigname,U32 *size,int get_compressed);
void closeOpenPigs(PigCache *cache);
void safeMkDirTree(const char *dir);
FILE *safeFopen(const char *fname,const char *how);
void safeFileSetTimestamp(char *fname,U32 timestamp);
void safeCopyFile(char *src,char *dst);
void safeDeleteFile(char *fname);
void safeWriteBytes(FILE *file,U32 pos,void *buf,U32 count);
void safeWriteFile(const char *fname,char *how,void *mem,U32 len);
void safeRenameFile(char *oldname,char *newname);
void gzStreamDecompressInit(GzStream *gz,FILE *file);
void gzStreamCompressInit(GzStream *gz,FILE *file);
void gzStreamFree(GzStream *gz);
int gzGetBytes(GzStream *gz,int count,U8 *buf,int dont_unpack);
int gzWriteBytes(GzStream *gz,int count,U8 *buf,int flush);
void fileMsgAlert(char const *fmt, ...);
void msgAlertUpdater(char const *fmt, ...);
int removeEmptyDirs(char *fname);
void removeDirAll(char *dir);
PigFileHeader *getPfh(PigCache *cache,char *image_dir,char *fname,char *pigname);
U32 safeFileSize(char *fname);
int makeUserDir(char *dir);
void setFatalAlertCallback(FatalAlertCallback callback);
void msgAlertFatal(char const *fmt, ...);
int makeLockFile(char *dst_dir);
void cleanupLockFile();
void cacheQuickFileInfo(const char* dir);
int quickFileInfo(char *fname,U32 *size,U32 *modified);

// build_name is in the format "##.########.##"
// this function returns the number before the first period as an int
int getUpdateVersion(char *build_name);

// build_name_# is in the format "##.########.##"
// this function returns exactly like strcmp(0 for equal, 1 if the first is greater, -1 if the second is greater), 
// except it compares the numeric value of the build version names
int compareUpdateVersion(char *build_name_1, char *build_name_2);

#endif
