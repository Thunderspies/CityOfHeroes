/***************************************************************************
*     Copyright (c) 2003-2006, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
***************************************************************************/
// Data file format:
//  pig_header
//  pig_file_header x number of files (order independent)
//  string_pool {
//    U32 PIG_STRING_POOL_FLAG
//    U32 num_of_strings
//    U32 size of data
//    num_of_strings of pascal-style strings (U32 length followed by length bytes) - allows nulls in the data
//  }
//  header_data_pool
//    U32 PIG_HEADER_DATA_POOL_FLAG
//    U32 num_of_header_datum
//    U32 size of data
//    num_of_header_datum of pascal-style strings (U32 length followed by length bytes) - allows nulls in the data
//  }
//  actual data

#pragma once
#ifndef _PIGLIB_H
#define _PIGLIB_H

//#define PIG_SET_DISABLED // disables loading of Pig files and therefore only loads from the filesystem

#include "../stdtypes.h"

typedef struct HogFile HogFile;
typedef struct PigFile PigFile;
typedef PigFile *PigFilePtr;

#define PIG_VERSION 2

#define DISABLE_HOGS


#ifndef DISABLE_HOGS
typedef void (*DataFreeCallback)(void *data);
#endif

typedef bool (*PiggFileFilterCallback)(const char *pigFilePath);

typedef struct NewPigEntry
{
    char    *fname;
    U32        timestamp;
    U8        *data;
    int        size;
    // These are used when the data is already compressed, (like when patching)
    U32        pack_size;
    U32        checksum[4];
    U32        dont_pack : 1;
    U32        must_pack : 1;

#ifndef DISABLE_HOGS
    // Only used by hoggs:
    U8        *header_data; // Pointer into either data* or a static buffer
    U32        header_data_size;
    DataFreeCallback free_callback;
#endif
} NewPigEntry;

typedef enum
{
    PIGERR_QUIET,
    PIGERR_PRINTF,
    PIGERR_ASSERT,
} PigErrLevel;

extern PigErrLevel    pig_err_level;

void pigDisablePiggs(int disabled); // Disables loading piggs.

int pigCreateFromMem(NewPigEntry *entries, int count, const char *fname, int version);
PigFilePtr PigFileAlloc(void);
void PigFileCreate(PigFilePtr handle);
void PigFileDestroy(PigFilePtr handle);
int PigFileRead(PigFilePtr handle, const char *filename); // Load a .PIG file
S32 PigFileFindIndexQuick(PigFilePtr handle, const char *relpath); // Assumes correctly formatted file name for performance
void PigFileDumpInfo(PigFilePtr handle, int verbose, int debug_verbose); // can be stdout
void *PigFileExtract(PigFilePtr handle, const char *relpath, U32 *count); // Extracts a file from an archive into memory
void *PigFileExtractCompressed(PigFilePtr handle, const char *relpath, U32 *count); // get the zipped data. (used by patcher) returns 0 if data isn't compressed
bool PigFileValidate(PigFilePtr handle);

typedef void (*HogCallbackDeleted)(void *userData, const char* path);
typedef void (*HogCallbackNew)(void *userData, const char* path, U32 filesize, U32 timestamp);
typedef void (*HogCallbackUpdated)(void *userData, const char* path, U32 filesize, U32 timestamp);
bool PigFileSetCallbacks(PigFilePtr handle, void *userData,
                         HogCallbackDeleted fileDeleted, HogCallbackNew fileNew, HogCallbackUpdated fileUpdated);

void PigFileLock(PigFilePtr handle); // Acquires the multi-process mutex if this is a hogg file
void PigFileUnlock(PigFilePtr handle);
void PigFileCheckForModifications(PigFilePtr handle); // If a hogg file, checks for modifications by other processes

const char *PigFileGetArchiveFileName(PigFilePtr handle);
U32 PigFileGetNumFiles(PigFilePtr handle);
const char *PigFileGetFileName(PigFilePtr handle, int index); // Returns NULL if an empty slot (Hoggs)
bool PigFileIsSpecialFile(PigFilePtr handle, int index); // Returns TRUE if an internal/special file (Hoggs)
U32 PigFileGetFileTimestamp(PigFilePtr handle, int index);
U32 PigFileGetFileSize(PigFilePtr handle, int index);
bool PigFileGetChecksum(PigFile *handle, int index, U32 *checksum);
bool PigFileIsZipped(PigFilePtr handle, int index); // Returns whether or not a file is zipped (used by wrapper to determine buffering rules)
U32 PigFileGetZippedFileSize(PigFilePtr handle, int index);
const U8 *PigFileGetHeaderData(PigFilePtr handle, int index, U32 *header_size); // Warning: pointer returned is volatile, and may be invalid after any hog file changes

typedef struct PigFileDescriptor {
    const char *debug_name;
    PigFilePtr parent;
    S32 file_index;
    // Cached data:
    U32 size;
    U32 header_data_size;
} PigFileDescriptor;

void PigCritSecInit(void);
int PigSetInit(void); // Loads all of the Pig files from the gamedatadir
int PigSetAddPatchDir(const char *path); // Loads all of the Pig files from a custom location
bool PigSetInited(void);
int PigSetCheckForNew();
void PigSetDestroy(void);
PigFileDescriptor PigSetGetFileInfo(int pig_index, int file_index, const char *debug_relpath);
U32 PigSetExtractBytes(PigFileDescriptor *desc, void *buf, U32 pos, U32 size, bool random_access); // Extracts size bytes into a pre-allocated buffer

int PigSetGetNumPigs(void);
PigFilePtr PigSetGetPigFile(int index);
PigFilePtr PigSetGetPigFileByName(const char* filename); // eg: "./piggs/bin.pigg"
void PigSetAdd(PigFilePtr pig);

void PigFileSetFilter(PiggFileFilterCallback cb);

extern int pig_debug;

void pigChecksumAndPackEntry(NewPigEntry *entry);
bool pigShouldCacheHeaderData(const char *ext);
bool pigShouldBeUncompressed(const char *ext);

bool doNotCompressMySize(NewPigEntry * entry);
bool doNotCompressMyExt(NewPigEntry * entry);

#endif

