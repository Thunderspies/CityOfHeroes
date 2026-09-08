#include "utilitieslib/utils/hoglib.h"
#include "utilitieslib/utils/hogutil.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/piglib.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/components/datalist.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/network/crypt.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/piglib_internal.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/genericlist.h"
#include "utilitieslib/utils/memlog.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/mutex.h"
#include "utilitieslib/utils/threadedFileCopy.h"
#include "utilitieslib/utils/endian.h"
#include "utilitieslib/utils/textparser.h"

//#define NO_FREELIST

MemLog hogmemlog={0};

// create: pig chzyf ./pigtest.hogg ./source_files/*
// list: pig tv2p ./pigtest.hogg
// list: pig tv2p ./trouble.hogg
// extract: pig xvf ../pigtest.hogg -Cextract
// update: pig uvhzyf ./pigtest.hogg ./test_files/* --hogdebug -Rtest1.bat
// patchtest: pig cv a --patchtest

static int debug_crash_counter=0;
#define DEBUG_CHECK() saveDebugHog(handle->file, handle->filename, ++debug_crash_counter)
#define NESTED_ERROR(err_code, old_error) ((old_error << 4) | err_code)
#define ENTER_CRITICAL(which) { EnterCriticalSection(&handle->which); handle->debug_in_##which++; }
#define LEAVE_CRITICAL(which) { handle->debug_in_##which--; LeaveCriticalSection(&handle->which); }
#define IN_CRITICAL(which) { assert(handle->debug_in_##which); }
int hog_debug_check=0;
int hog_verbose=0;
int hog_mode_no_data=0; // Does not actually move data around, just pretends and updates headers

extern CRITICAL_SECTION PigCritSec;

#define HOG_OP_JOURNAL_SIZE 1024
#define HOG_DL_JOURNAL_SIZE 3096
#define HOG_MAX_SLACK_SIZE 2048
#define HOG_TEMP_BUFFER_SIZE MAX(MAX(HOG_OP_JOURNAL_SIZE, HOG_DL_JOURNAL_SIZE), HOG_MAX_SLACK_SIZE)
#define FILELIST_PAD 0.20 // In percent of filelist size
#define FILELIST_PAD_THRESHOLD 0.05 // If we're less than this when we expand the EAList, expand the FileList as well
#define EALIST_PAD_THRESHOLD 0.15 // If we're less than this when we expand the FileList, expand the EAList as well
#define HOG_NO_VALUE ((U32)-1)
#define HOG_JOURNAL_TERMINATOR 0xdeabac05 // arbitrary

typedef enum HogEAFlags {
    HOGEA_NOT_IN_USE = 1 << 0,
} HogEAFlags;

typedef enum HogOpJournalAction {
    HOJ_INVALID,
    HOJ_DELETE,
    HOJ_ADD,
    HOJ_UPDATE,
    HOJ_MOVE,
    HOJ_FILELIST_RESIZE,
    HOJ_DATALISTFLUSH,
} HogOpJournalAction;
STATIC_ASSERT(sizeof(HogOpJournalAction)==sizeof(U32)); // endianSwapping down below relies on this

// Begin Format on disk:

    // Header to archive file
    typedef struct HogHeader {
        U32 hog_header_flag;
        U16 version;
        U16 op_journal_size;
        U32 file_list_size;
        U32 ea_list_size;
        U32 datalist_fileno; // Index of the special file "?DataList"
        U16 dl_journal_size;
        U16 pad; // unused
    } HogHeader;
    TokenizerParseInfo parseHogHeader[] = {
        {"hog_header_flag",        TOK_INT(HogHeader, hog_header_flag, 0)},
        {"version",                TOK_INT16(HogHeader, version, 0)},
        {"op_journal_size",        TOK_INT16(HogHeader, op_journal_size, 0)},
        {"file_list_size",        TOK_INT(HogHeader, file_list_size, 0)},
        {"ea_list_size",        TOK_INT(HogHeader, ea_list_size, 0)},
        {"datalist_fileno",        TOK_INT(HogHeader, datalist_fileno, 0)},
        {"dl_journal_size",        TOK_INT16(HogHeader, dl_journal_size, 0)},
        {0, 0}
    };

    // Then comes the Operation Journal
    //  (structures below, HogOpJournalHeader, et al)

    // Then comes the DataList Journal
    typedef struct DLJournalHeader {
        U32 inuse_flag; // Flag this,
        U32 size;        // Then write new size, then unflag,
        U32 oldsize;    // Then overwrite oldsize
    } DLJournalHeader;
    TokenizerParseInfo parseDLJournalHeader[] = {
        {"inuse_flag",    TOK_INT(DLJournalHeader, inuse_flag, 0)},
        {"size",        TOK_INT(DLJournalHeader, size, 0)},
        {"oldsize",        TOK_INT(DLJournalHeader, oldsize, 0)},
        {0, 0}
    };

    // Basic file information.  For lightmaps which don't have filenames, etc, this needs
    // to be kept to a bare minimum
    typedef union _HogFileHeaderData {
        U64 raw;
        struct {
#ifdef _XBOX 
            S32 ea_id;
            U16 reserved;
            U16 flagFFFE; // If this is FFFE, the ea_index is valid
#else
            U16 flagFFFE; // If this is FFFE, the ea_index is valid
            U16 reserved;
            S32 ea_id;
#endif
        };
    } HogFileHeaderData;
    typedef struct HogFileHeader {
        U64 offset;
        U32 size;
        __time32_t timestamp;
        U32    checksum; // first 32 bits of a checksum (MD5)
        HogFileHeaderData headerdata;
    } HogFileHeader;
    STATIC_ASSERT(sizeof(((HogFileHeader*)0)->headerdata) == sizeof(U64)); // Make sure the union isn't confused
    TokenizerParseInfo parseHogFileHeader[] = {
        {"offset",        TOK_INT64(HogFileHeader, offset, 0)},
        {"size",        TOK_INT(HogFileHeader, size, 0)},
        {"timestamp",    TOK_INT(HogFileHeader, timestamp, 0)},
        {"checksum",    TOK_INT(HogFileHeader,    checksum, 0)},
        {"headerdata",    TOK_INT64_X, offsetof(HogFileHeader, headerdata), 0},
        {0, 0}
    };

    // Extended Attributes (optional) contains more information about a file
    typedef struct HogEAHeader {
        U32 name_id; // ID of name (names stored in the DataList file)
        U32 header_data_id; // ID of header data (if any)
        U32 unpacked_size; // Size of file after unzipping if compressed (0 if not compressed)
        U32 flags; // Reserved for special flags (HogEAFlags)
    } HogEAHeader;
    TokenizerParseInfo parseHogEAHeader[] = {
        {"name_id",            TOK_INT(HogEAHeader, name_id, 0)},
        {"header_data_id",    TOK_INT(HogEAHeader, header_data_id, 0)},
        {"unpacked_size",    TOK_INT(HogEAHeader, unpacked_size, 0)},
        {"flags",            TOK_INT(HogEAHeader, flags, 0)},
        {0, 0}
    };
    // datalist is a DataList file on disk

// End Format on disk

// Begin structures for modifications
    typedef enum HogFileModType {
        HFM_NONE,
        HFM_DELETE,
        HFM_ADD,
        HFM_UPDATE,
        HFM_MOVE,
        HFM_DATALISTDIFF,
        HFM_TRUNCATE,
        HFM_FILELIST_RESIZE,
        HFM_DATALISTFLUSH,
    } HogFileModType;

    // These need to contain all of the information needed to do the command without
    // access to the old elements of the hog_file structure (serialized copy of
    // DataList needed?)
    typedef struct HFMDelete {
        HogFileIndex file_index;
        S32 ea_id;
    } HFMDelete;
    TokenizerParseInfo parseHFMDelete[] = {
        {"file_index",    TOK_INT(HFMDelete, file_index, 0)},
        {"ea_id",        TOK_INT(HFMDelete, ea_id, 0)},
        {0, 0}
    };

    typedef struct HFMAdd {
        HogFileIndex file_index;
        U32 size;
        U32 timestamp;
        HogFileHeaderData headerdata; // ea_id or other header data
        U32 checksum;
        U64 offset;
        struct { // Only if ea_id present
            U32 unpacksize;
            S32 name_id;
            S32 header_data_id;
        } ea_data;

        U8 *data; // Pointer *must* be last for 64-bit compatibility
    } HFMAdd;
    STATIC_ASSERT(sizeof(((HFMAdd*)0)->headerdata) == sizeof(U64));
    TokenizerParseInfo parseHFMAdd[] = {
        {"file_index",        TOK_INT(HFMAdd, file_index, 0)},
        {"size",            TOK_INT(HFMAdd, size, 0)},
        {"timestamp",        TOK_INT(HFMAdd, timestamp, 0)},
        {"headerdata",        TOK_INT64(HFMAdd, headerdata, 0)},
        {"checksum",        TOK_INT(HFMAdd, checksum, 0)},
        {"offset",            TOK_INT64(HFMAdd, offset, 0)},
        {"unpacksize",        TOK_INT(HFMAdd, ea_data.unpacksize, 0)},
        {"name_id",            TOK_INT(HFMAdd, ea_data.name_id, 0)},
        {"header_data_id",    TOK_INT(HFMAdd, ea_data.header_data_id, 0)},
        {0, 0}
    };

    typedef struct HFMUpdate {
        HogFileIndex file_index;
        U32 size;
        U32 timestamp;
        HogFileHeaderData headerdata; // ea_id or other header data
        U32 checksum;
        U64 offset;

        U8 *data; // Pointer *must* be last for 64-bit compatibility
    } HFMUpdate;
    TokenizerParseInfo parseHFMUpdate[] = {
        {"file_index",        TOK_INT(HFMUpdate, file_index, 0)},
        {"size",            TOK_INT(HFMUpdate, size, 0)},
        {"timestamp",        TOK_INT(HFMUpdate, timestamp, 0)},
        {"headerdata",        TOK_INT64(HFMUpdate, headerdata, 0)},
        {"checksum",        TOK_INT(HFMUpdate, checksum, 0)},
        {"offset",            TOK_INT64(HFMUpdate, offset, 0)},
        {0, 0}
    };

    typedef struct HFMMove {
        HogFileIndex file_index;
        U32 size;
        U64 old_offset;
        U64 new_offset;
    } HFMMove;
    TokenizerParseInfo parseHFMMove[] = {
        {"file_index",    TOK_INT(HFMMove, file_index, 0)},
        {"size",        TOK_INT(HFMMove, size, 0)},
        {"old_offset",    TOK_INT64(HFMMove, old_offset, 0)},
        {"new_offset",    TOK_INT64(HFMMove, new_offset, 0)},
        {0, 0}
    };

    typedef struct HFMDataListDiff {
        U64 size_offset; // Where to write the size
        U32 size; // Size of new data
        U64 offset; // where to write the data
        U32 newsize; // New size to write

        U8 *data; // Pointer at end!
    } HFMDataListDiff;
    TokenizerParseInfo parseHFMDataListDiff[] = {
        {"size_offset",        TOK_INT64(HFMDataListDiff, size_offset, 0)},
        {"size",            TOK_INT(HFMDataListDiff, size, 0)},
        {"offset",            TOK_INT64(HFMDataListDiff, offset, 0)},
        {"newsize",            TOK_INT(HFMDataListDiff, newsize, 0)},
        {0, 0}
    };

    typedef struct HFMDataListFlush {
        U64 size_offset; // Where to write the size
    } HFMDataListFlush;
    TokenizerParseInfo parseHFMDataListFlush[] = {
        {"size_offset",        TOK_INT64(HFMDataListFlush, size_offset, 0)},
        {0, 0}
    };

    typedef struct HFMTruncate {
        U64 newsize;
    } HFMTruncate;
    TokenizerParseInfo parseHFMTruncate[] = {
        {"newsize",            TOK_INT64(HFMTruncate, newsize, 0)},
        {0, 0}
    };

#ifdef _M_X64
#    pragma pack(push)
#    pragma pack(4)
#endif
    typedef struct HFMFileListResize {
        U32 new_filelist_size;
        U32 old_ealist_pos; // Not needed?
        U32 old_ealist_size; // Size of old data (used if EAList didn't move)
        U32 new_ealist_pos; // Should be implicit from new_filelist_size and filelist_offset
        U32 new_ealist_size;
        void *new_ealist_data; // Size is new_ealist_size
    } HFMFileListResize;
    TokenizerParseInfo parseHFMFileListResize[] = {
        {"new_filelist_size",    TOK_INT(HFMFileListResize, new_filelist_size, 0)},
        {"old_ealist_pos",        TOK_INT(HFMFileListResize, old_ealist_pos, 0)},
        {"old_ealist_size",        TOK_INT(HFMFileListResize, old_ealist_size, 0)},
        {"new_ealist_pos",        TOK_INT(HFMFileListResize, new_ealist_pos, 0)},
        {"new_ealist_size",        TOK_INT(HFMFileListResize, new_ealist_size, 0)},
        {0, 0}
    };
#ifdef _M_X64
#    pragma pack(pop)
#endif

    typedef struct HogFileMod HogFileMod;
    typedef struct HogFileMod {
        HogFileMod *next;
        HogFileModType type;
        U64 filelist_offset;
        U64 ealist_offset;
        union {
            HFMDelete del;
            HFMAdd add;
            HFMUpdate update;
            HFMMove move;
            HFMDataListDiff datalistdiff;
            HFMTruncate truncate;
            HFMFileListResize filelist_resize;
            HFMDataListFlush datalistflush;
        };
    } HogFileMod;
// End structures for modifications

// Begin Op Journal structures (uses above Mod structures for simplicity)
    typedef struct HogOpJournalHeader {
        U32 size;
        // Then begins one of the following structures (with Type as the first element)
        HogOpJournalAction type;
    } HogOpJournalHeader;
    TokenizerParseInfo parseHogOpJournalHeader[] = {
        {"size", TOK_INT(HogOpJournalHeader, size, 0)},
        {"type", TOK_INT(HogOpJournalHeader, type, 0)},
        {0, 0}
    };
    typedef struct HFJDelete {
        HogOpJournalAction type;
        HFMDelete del;
    } HFJDelete;
    typedef struct HFJAdd {
        HogOpJournalAction type;
        HFMAdd add; // data entry is ignored
    } HFJAdd;
    typedef struct HFJUpdate {
        HogOpJournalAction type;
        HFMUpdate update; // data entry is ignored
    } HFJUpdate;
    typedef struct HFJMove {
        HogOpJournalAction type;
        HFMMove move; // old_offset and size entries are ignored
    } HFJMove;
    typedef struct HFJFileListResize {
        HogOpJournalAction type;
        HFMFileListResize filelist_resize; // old_offset and size entries are ignored
    } HFJFileListResize;
    typedef struct HFJDataListFlush {
        HogOpJournalAction type;
        HFMDataListFlush datalistflush; // old_offset and size entries are ignored
    } HFJDataListFlush;

// End Op Journal structures

// Begin In memory format
    typedef struct HogFileListEntry {
        HogFileHeader header;
        bool in_use:1;
        U8 *data; // Data to be written
        int dirty; // Has an operation queued to modify this file, header data is not valid
    } HogFileListEntry;

    typedef struct HogEAListEntry {
        HogEAHeader header;
        bool in_use;
    } HogEAListEntry;

    typedef struct HogFile {
        HogHeader header;
        HogFileListEntry *filelist;
        U32 num_files;
        U32 filelist_count; // Count of FileList entries (matches what's on disk)
        U32 filelist_max; // max (for use by dynArray)
        HogEAListEntry *ealist;
        U32 ealist_count; // Count of EAList entries (matches what's on disk)
        U32 ealist_max; // max (for use by dynArray)
        DataList datalist; // Contains names and HeaderData chunks
        char *filename;
        struct {
            char *name;
            volatile int reference_count;
            HANDLE handle;
        } mutex;
        U64 file_size;
        FILE *file;
        bool read_only;
        bool file_needs_flush;
        HogFileMod *mod_ops_head; // First one queued
        HogFileMod *mod_ops_tail; // Last one queued
        U32 datalist_diff_size; // Current size of DataList Journal
        U32 last_journal_size;
        CRITICAL_SECTION file_access; // For multi-threaded read/write
        int debug_in_file_access;
        CRITICAL_SECTION data_access; // For multi-threaded query of in-memory data
        int debug_in_data_access;
        CRITICAL_SECTION doing_flush; // For multiple threads doing a flush at the same time
        int debug_in_doing_flush;
        bool crit_sect_inited;
        HANDLE done_doing_operation_event;
        int last_threaded_error;
        volatile U32 async_operation_count;
        StashTable fn_to_index_lookup;
        int *file_free_list; // EArray of free FileList indices
        int *ea_free_list; // EArray of free EAList indicies
        HogFreeSpaceTracker free_space; 
        bool free_space_dirty;
        bool has_been_reloaded;
        __time32_t file_last_changed;
        struct {
            char *name;
            LONG value;
            HANDLE semaphore;
        } version;
        struct {
            bool exist;
            HogCallbackDeleted fileDeleted;
            HogCallbackNew fileNew;
            HogCallbackUpdated fileUpdated;
            void *userData;
        } callbacks;
    } HogFile;
// End In memory format


static int hogFileModifyDoDeleteInternal(HogFile *handle, HogFileMod *mod);
static int hogFileModifyDoAddInternal(HogFile *handle, HogFileMod *mod);
static int hogFileModifyDoUpdateInternal(HogFile *handle, HogFileMod *mod);
static int hogFileModifyDoMoveInternal(HogFile *handle, HogFileMod *mod);
static int hogFileModifyDoFileListResizeInternal(HogFile *handle, HogFileMod *mod);
static int hogFileModifyDoDataListFlushInternal(HogFile *handle, HogFileMod *mod);
static int hogFileAddDataListMod(HogFile *handle, DataListJournal *dlj);
static void hogThreadHasWork(HogFile *handle);

PigErrLevel    hog_err_level = PIGERR_ASSERT;

static int hogShowError(const char *fname,int err_code,const char *err_msg,int err_int_value)
{
    char    buf[1000];

    sprintf_s(SAFESTR(buf),"%s: %s %d",fname,err_msg,err_int_value);
    if (hog_err_level != PIGERR_QUIET)
        printf("%s\n",buf);
    if (hog_err_level == PIGERR_ASSERT)
        FatalErrorf("%s\n\nYour data might be corrupt, please run the patcher and try again", buf);
    return err_code;
}

void hoglogEcho(const char* s, ...)
{
    va_list ap;
    char buf[MEMLOG_LINE_WIDTH+10];

    va_start(ap, s);
    if (vsprintf(buf,s,ap) < 10) return;
    va_end(ap);

    printf("%s\n", buf);
}

HogFile *hogFileAlloc(void)
{
    HogFile *ret = calloc(sizeof(HogFile), 1);
    //memlog_setCallback(&hogmemlog, hoglogEcho);
    memlog_enableThreadId(&hogmemlog);
    hogFileCreate(ret);
    return ret;
}

void hogFileCreate(HogFile *handle)
{
    hogThreadingInit();
    memlog_printf(&hogmemlog, "0x%08x: Create", handle);
    assert(!handle->crit_sect_inited);
    ZeroStruct(handle);
    handle->header.hog_header_flag = HOG_HEADER_FLAG;
    handle->header.version = HOG_VERSION;
    handle->header.op_journal_size = HOG_OP_JOURNAL_SIZE;
    handle->header.dl_journal_size = HOG_DL_JOURNAL_SIZE;
    DataListCreate(&handle->datalist);
    handle->header.datalist_fileno = HOG_NO_VALUE;
    InitializeCriticalSection(&handle->file_access);
    InitializeCriticalSection(&handle->data_access);
    InitializeCriticalSection(&handle->doing_flush);
    handle->crit_sect_inited = true;
    handle->fn_to_index_lookup = stashTableCreateWithStringKeys(32, StashDefault);
    handle->done_doing_operation_event = CreateEvent(NULL, FALSE, FALSE, NULL); // Auto-reset, not initially signaled
    assert(handle->done_doing_operation_event != NULL);
}

void hogFileDestroy(HogFile *handle)
{
    // Flush changes
    hogFileModifyFlush(handle);

    memlog_printf(&hogmemlog, "0x%08x: Destroy", handle);

    assert(!handle->debug_in_data_access);
    assert(!handle->debug_in_doing_flush);
    assert(!handle->mutex.reference_count);
    assert(!handle->mod_ops_head);
    // Clean up
    stashTableDestroy(handle->fn_to_index_lookup);
    if (handle->crit_sect_inited) {
        DeleteCriticalSection(&handle->doing_flush);
        DeleteCriticalSection(&handle->data_access);
        DeleteCriticalSection(&handle->file_access);
        handle->crit_sect_inited = true;
    }
    SAFE_FREE(handle->filelist);
    SAFE_FREE(handle->ealist);
    DataListDestroy(&handle->datalist);
    SAFE_FREE(handle->filename);
    SAFE_FREE(handle->mutex.name);
    if (handle->file)
        fclose(handle->file);
    if (handle->version.semaphore)
        CloseHandle(handle->version.semaphore);
    SAFE_FREE(handle->version.name);
    eaiDestroy(&handle->file_free_list);
    eaiDestroy(&handle->ea_free_list);
    hfstDestroy(&handle->free_space);
    ZeroStruct(handle);
}

void hogFileSetCallbacks(HogFile *handle, void *userData,
                         HogCallbackDeleted fileDeleted, HogCallbackNew fileNew, HogCallbackUpdated fileUpdated)
{
    assert(!fileDeleted && !fileNew && !fileUpdated ||
        fileDeleted && fileNew && fileUpdated); // All or nothing
    if (!fileDeleted && !fileNew && !fileUpdated) {
        handle->callbacks.exist = false;
    } else {
        handle->callbacks.exist = true;
        handle->callbacks.fileDeleted = fileDeleted;
        handle->callbacks.fileNew = fileNew;
        handle->callbacks.fileUpdated = fileUpdated;
        handle->callbacks.userData = userData;
    }
}

static void hogFileReload(HogFile *handle)
{
    int ret;
    HogFile *temp;

    IN_CRITICAL(data_access);

    loadstart_printf("Reloading changes to %s...", handle->filename);
    if (!fileExists(handle->filename)) {
        // File was removed
        loadend_printf("deleted!");
        handle->file_last_changed = -1;
        handle->version.value = -1;
        return;
    }
    temp=hogFileAlloc();
    if (ret=hogFileRead(temp, handle->filename)) {
        assertmsg(0, "Failed to read the hogg we're reloading");
    }
    // Swap everything
    swap(temp, handle, sizeof(*handle));
    // Then swap back what we need to keep around (mutexes, critical sections)
#define SWAPFIELD(field) swap(&temp->field, &handle->field, sizeof(handle->field))
    SWAPFIELD(mutex);
    SWAPFIELD(file_access);
    SWAPFIELD(debug_in_file_access);
    SWAPFIELD(data_access);
    SWAPFIELD(debug_in_data_access);
    SWAPFIELD(callbacks);
    handle->has_been_reloaded = true;
    // Do diff and send callbacks (to FolderCache, etc)
    if (handle->callbacks.exist)
    {
        U32 numfiles;
        U32 i;
        // Look for changes
        numfiles = MAX(hogFileGetNumFiles(handle), hogFileGetNumFiles(temp));
        for (i=0; i<numfiles; i++) {
            const char *newfilename = hogFileGetFileName(handle, i);
            const char *oldfilename = hogFileGetFileName(temp, i);
            if (newfilename && hogFileIsSpecialFile(handle, i))
                newfilename = NULL;
            if (oldfilename && hogFileIsSpecialFile(temp, i))
                oldfilename = NULL;
            if (!newfilename && !oldfilename)
                continue;
            if (!newfilename)
            {
                // File was deleted
                handle->callbacks.fileDeleted(handle->callbacks.userData, oldfilename);
            } else if (!oldfilename) {
                // File is new
                handle->callbacks.fileNew(handle->callbacks.userData, newfilename, hogFileGetFileSize(handle, i), hogFileGetFileTimestamp(handle, i));
            } else {
                // Both existed, check for rename/reused filename
                U32 newtimestamp = hogFileGetFileTimestamp(handle, i);
                U32 oldtimestamp = hogFileGetFileTimestamp(temp, i);
                U32 newsize = hogFileGetFileSize(handle, i);
                U32 oldsize = hogFileGetFileSize(temp, i);
                if (stricmp(oldfilename, newfilename)!=0) {
                    // renamed!
                    handle->callbacks.fileDeleted(handle->callbacks.userData, oldfilename);
                    handle->callbacks.fileNew(handle->callbacks.userData, newfilename, newsize, newtimestamp);
                } else {
                    // Check for modification
                    if (oldsize != newsize || oldtimestamp != newtimestamp) {
                        handle->callbacks.fileUpdated(handle->callbacks.userData, newfilename, newsize, newtimestamp);
                    }
                }
            }
        }
    }
    // Clean up
    hogFileDestroy(temp);
    loadend_printf("done.");
}

static LONG hogGetVersion(HogFile *handle)
{
    DWORD result;
    LONG old_version;
    assert(handle->mutex.reference_count);
    assert(handle->version.semaphore);
    result = WaitForSingleObject(handle->version.semaphore, 0);
    if (result == WAIT_OBJECT_0) {
        // Got it
    } else {
        // Somehow the semaphore object's count got set to 0!
        //  This should only be able to happen with crashing at exactly the right time.
        Sleep(100);
        result = WaitForSingleObject(handle->version.semaphore, 0);
        assert(result != WAIT_OBJECT_0); // If it changed, then we have a software bug, someone else modified it while we've got the mutex!
            // This assert will also go off if you create two handles to the same hogg file
            // in the same process.
        ReleaseSemaphore(handle->version.semaphore, 1, NULL); // Artificially increment it to 1
    }
    if (!ReleaseSemaphore(handle->version.semaphore, 1, &old_version)) {
        // Failed?
        assert(0);
        return -1;
    } else {
        // old_version is the decremented version number, starting at 0
        return old_version + 1;
    }
}

static LONG hogIncVersion(HogFile *handle)
{
    LONG old_version;
    assert(handle->mutex.handle);
    assert(handle->version.semaphore);
    if (!ReleaseSemaphore(handle->version.semaphore, 1, &old_version)) {
        // Failed?
        assert(0);
        return -1;
    } else {
        // old_version is the decremented version number, starting at 0
        return old_version + 1;
    }
}

void hogAcquireMutex(HogFile *handle)
{
    ENTER_CRITICAL(data_access);
    if (handle->mutex.reference_count) {
        handle->mutex.reference_count++;
        //memlog_printf(&hogmemlog, "0x%08x: hogAcquireMutex: Increment to %d", handle, handle->mutex.reference_count);
    } else {
        assert(!handle->async_operation_count);
        assert(!handle->mutex.handle);
        assert(!handle->mod_ops_head); // If actions are queued up, we're already doomed!

        handle->mutex.reference_count++;
        memlog_printf(&hogmemlog, "0x%08x: hogAcquireMutex:acquireThreadAgnosticMutex()", handle);
        handle->mutex.handle = acquireThreadAgnosticMutex(handle->mutex.name);
        // Now need to check the file to see if there were any modifications!
        if (handle->file_size &&
            ((fileLastChanged(handle->filename)!=handle->file_last_changed) ||
            hogGetVersion(handle) != handle->version.value))
        {
            memlog_printf(&hogmemlog, "0x%08x: hogAcquireMutex:Got Mutex, need reload", handle);
            // Re-load hogg file!
            hogFileReload(handle);
        }
    }
    LEAVE_CRITICAL(data_access);
}

void hogReleaseMutex(HogFile *handle, bool needsFlushAndSignal)
{
    ENTER_CRITICAL(data_access);
    assert(handle->mutex.reference_count);
    if (needsFlushAndSignal) {
        handle->file_needs_flush = true;
        SetEvent(handle->done_doing_operation_event);
    }
    if (handle->mutex.reference_count==1) { // Going to decrement to 0
        if (handle->file_needs_flush) {
            memlog_printf(&hogmemlog, "0x%08x: hogReleaseMutex:fflush()", handle, handle->mutex.reference_count);
            fflush(handle->file);
            handle->file_needs_flush = false;
            handle->version.value = hogIncVersion(handle);
        }
        // problem: Need to call fileLastChanged before releasing mutex
        //            fileLastChanged will call this function if it gets a message that the hogg file was modified
        // solution: decrement reference_count at the end
        handle->file_last_changed = fileLastChanged(handle->filename);
        memlog_printf(&hogmemlog, "0x%08x: hogReleaseMutex:releaseThreadAgnosticMutex()", handle, handle->mutex.reference_count);
        releaseThreadAgnosticMutex(handle->mutex.handle);
        handle->mutex.handle = NULL;
    } else {
        //memlog_printf(&hogmemlog, "0x%08x: hogReleaseMutex:Decrement to %d", handle, handle->mutex.reference_count);
    }
    handle->mutex.reference_count--;
    LEAVE_CRITICAL(data_access);
}

void hogFileLock(HogFile *handle)
{
    hogAcquireMutex(handle);
}

void hogFileUnlock(HogFile *handle)
{
    hogReleaseMutex(handle, false);
}

void hogFileCheckForModifications(HogFile *handle)
{
    hogAcquireMutex(handle);
    hogReleaseMutex(handle, false);
}


static void saveDebugHog(FILE *file, const char *filename, int number)
{
    char fmt[MAX_PATH];
    char fn[MAX_PATH];
    char src[MAX_PATH], dst[MAX_PATH];
    fflush(file);
    strcpy(fmt, filename);
    changeFileExt(fmt, "/%03d.hogg", fmt);
    sprintf_s(SAFESTR(fn), fmt, number);
    fileLocateWrite(filename, src);
    fileLocateWrite(fn, dst);
    mkdirtree(dst);
    fileCopy(src, dst);
}

static void hogFileAllocFileEntry(HogFile *handle)
{
    U32 i;
    dynArrayAddStruct(&handle->filelist, &handle->filelist_count, &handle->filelist_max);
    i = handle->filelist_count-1;
    eaiPush(&handle->file_free_list, i+1);
}

static void hogFileAllocEAEntry(HogFile *handle)
{
    U32 i;
    dynArrayAddStruct(&handle->ealist, &handle->ealist_count, &handle->ealist_max);
    i = handle->ealist_count-1;
    eaiPush(&handle->ea_free_list, i+1);
}

static int hogFileCreateFileEntry(HogFile *handle) // Returns file number
{
    U32 i;
    IN_CRITICAL(data_access);
    if (eaiSize(&handle->file_free_list)==0) {
        assert(!handle->file); // Not during run-time modifications
        hogFileAllocFileEntry(handle);
    }
    i = eaiPop(&handle->file_free_list) - 1;
    //ZeroStruct(&handle->filelist[i]); // should be zeroed already? // Can't zero dirty flag/count!
    handle->num_files++;
    handle->filelist[i].in_use = true;
    handle->filelist[i].header.offset = 0;
    handle->filelist[i].header.size = HOG_NO_VALUE;
    handle->filelist[i].header.timestamp = HOG_NO_VALUE;
    return i;
}

static HogFileListEntry *hogGetFileListEntrySafe(HogFile *handle, HogFileIndex file, bool should_assert)
{
    HogFileListEntry *file_entry;
    IN_CRITICAL(data_access);
    if (file < 0 || file >= handle->filelist_count || file == HOG_NO_VALUE) {
        if (should_assert) {
            hogShowError(handle->filename, 1, "Inavlid file index ", file);
        } else {
            return NULL;
        }
    }
    file_entry = &handle->filelist[file];
    if (!file_entry->in_use) {
        if (should_assert) {
            hogShowError(handle->filename, 1, "Inavlid file index (file removed) ", file);
        } else {
            return NULL;
        }
    }
    return file_entry;
}

static INLINEDBG HogFileListEntry *hogGetFileListEntry(HogFile *handle, HogFileIndex file)
{
    return hogGetFileListEntrySafe(handle, file, true);
}


static void hogDirtyFile(HogFile *handle, HogFileIndex file)
{
    HogFileListEntry *file_entry;
    ENTER_CRITICAL(data_access);
    assert(file >= 0 && file < handle->filelist_count && file != HOG_NO_VALUE);
    file_entry = &handle->filelist[file];
    file_entry->dirty++;
    LEAVE_CRITICAL(data_access);
}

static void hogUnDirtyFile(HogFile *handle, HogFileIndex file)
{
    HogFileListEntry *file_entry;
    ENTER_CRITICAL(data_access);
    assert(file >= 0 && file < handle->filelist_count && file != HOG_NO_VALUE);
    file_entry = &handle->filelist[file];
    assert(file_entry->dirty>0);
    file_entry->dirty--;
    LEAVE_CRITICAL(data_access);
}

static HogEAListEntry *hogGetEAListEntry(HogFile *handle, S32 ea_id)
{
    HogEAListEntry *ea_entry;
    IN_CRITICAL(data_access);
    if (ea_id==HOG_NO_VALUE)
        return NULL;
    assert(ea_id >= 0 && ea_id < (S32)handle->ealist_count);
    ea_entry = &handle->ealist[ea_id];
    assert(ea_entry->in_use);
    return ea_entry;
}

static int hogFileCreateEAEntry(HogFile *handle, U32 file)
{
    U32 i;
    HogFileListEntry *file_entry = hogGetFileListEntry(handle, file);
    IN_CRITICAL(data_access);
    assert(file_entry->header.headerdata.ea_id == 0);
    if (eaiSize(&handle->ea_free_list)==0) {
        assert(!handle->file); // Not during run-time modifications
        hogFileAllocEAEntry(handle);
    }
    i = eaiPop(&handle->ea_free_list) - 1;
    handle->ealist[i].in_use = true;
    handle->ealist[i].header.header_data_id = HOG_NO_VALUE;
    handle->ealist[i].header.name_id = HOG_NO_VALUE;
    file_entry->header.headerdata.flagFFFE = 0xFFFE;
    file_entry->header.headerdata.ea_id = i;
    return i;
}

static int hogFileNameFile(HogFile *handle, HogEAListEntry *ea_entry, const char *name, HogFileIndex file_index)
{
    const char *stored_name;
    IN_CRITICAL(data_access);
    assert(ea_entry->header.name_id == HOG_NO_VALUE);
    ea_entry->header.name_id = DataListAdd(&handle->datalist, name, (int)strlen(name)+1, true, NULL);
    stored_name = DataListGetString(&handle->datalist, ea_entry->header.name_id);
    stashAddInt(handle->fn_to_index_lookup, stored_name, file_index, true); // Shallow copied name from DataList
    return 0;
}

static int hogFileAddHeaderData(HogFile *handle, HogEAListEntry *ea_entry, const U8 *header_data, U32 header_data_size)
{
    IN_CRITICAL(data_access);
    assert(ea_entry->header.header_data_id == HOG_NO_VALUE);
    ea_entry->header.header_data_id = DataListAdd(&handle->datalist, header_data, header_data_size, false, NULL);
    return 0;
}

static int hogFileUpdateHeaderData(HogFile *handle, HogEAListEntry *ea_entry, const U8 *header_data, U32 header_data_size)
{
    IN_CRITICAL(data_access);
    if (ea_entry->header.header_data_id == HOG_NO_VALUE)
        return hogFileAddHeaderData(handle, ea_entry, header_data, header_data_size);

    ea_entry->header.header_data_id = DataListUpdate(&handle->datalist, ea_entry->header.header_data_id, header_data, header_data_size, false, NULL);
    return 0;
}


// Note: takes ownership of data pointer
int hogFileAddFile(HogFile *handle, const char *name, U8 *data, U32 size, U32 timestamp, U8 *header_data, U32 header_data_size, U32 unpacksize, U32 *crc)
{
    int file;
    int ea_index;
    HogFileListEntry *file_entry;
    HogEAListEntry *ea_entry;
    ENTER_CRITICAL(data_access);
    file = hogFileCreateFileEntry(handle);
    file_entry = hogGetFileListEntry(handle, file);
    file_entry->header.size = size;
    file_entry->header.timestamp = timestamp;
    file_entry->data = data;
    ea_index = hogFileCreateEAEntry(handle, file);
    ea_entry = &handle->ealist[ea_index];
    ea_entry->header.unpacked_size = unpacksize;
    hogFileNameFile(handle, ea_entry, name, file);
    if (!crc) {
        U32 autocrc[4];
        assert(!unpacksize);
        cryptMD5Update(data,size);
        cryptMD5Final(autocrc);
        file_entry->header.checksum = autocrc[0];
    } else {
        file_entry->header.checksum = crc[0];
    }
    if (header_data) {
        hogFileAddHeaderData(handle, ea_entry, header_data, header_data_size);
    }
    LEAVE_CRITICAL(data_access);
    return file;
}

// Note: takes ownership of data pointer
// Just used internally during initial Hog creation
static int hogFileUpdateFile(HogFile *handle, const char *name, U8 *data, U32 size, U32 timestamp, U8 *header_data, U32 header_data_size, U32 unpacksize, U32 *crc)
{
    HogFileIndex file;
    int ea_index;
    HogEAListEntry *ea_entry;
    HogFileListEntry *file_entry;

    ENTER_CRITICAL(data_access);
    
    file = hogFileFind(handle, name);
    assert(file != HOG_NO_VALUE);

    file_entry = hogGetFileListEntry(handle, file);
    file_entry->header.size = size;
    file_entry->header.timestamp = timestamp;
    SAFE_FREE(file_entry->data);
    file_entry->data = data;
    assert(file_entry->header.headerdata.ea_id != HOG_NO_VALUE);
    ea_index = (int)file_entry->header.headerdata.ea_id;
    ea_entry = &handle->ealist[ea_index];
    ea_entry->header.unpacked_size = unpacksize;
    assert(ea_entry->header.name_id != HOG_NO_VALUE); // Must already have a name
    if (!crc) {
        U32 autocrc[4];
        assert(!unpacksize);
        cryptMD5Update(data,size);
        cryptMD5Final(autocrc);
        file_entry->header.checksum = autocrc[0];
    } else {
        file_entry->header.checksum = crc[0];
    }
    if (header_data) {
        hogFileUpdateHeaderData(handle, ea_entry, header_data, header_data_size);
    }
    LEAVE_CRITICAL(data_access);
    return file;
}

int hogFileAddFile2(HogFile *handle, NewPigEntry *entry)
{
    U8 *header_data=NULL;
    U32 header_data_size=0;
    int ret;
    if (!hog_mode_no_data)
        pigChecksumAndPackEntry(entry);
    ENTER_CRITICAL(data_access);
    if (!hog_mode_no_data)
        header_data = pigGetHeaderData(entry, &header_data_size);
    ret = hogFileAddFile(handle, entry->fname, entry->data, entry->pack_size?entry->pack_size:entry->size, entry->timestamp, header_data, header_data_size, entry->pack_size?entry->size:0, entry->checksum);
    LEAVE_CRITICAL(data_access);
    return ret;
}


static U64 hogFileGetSlackByName(const char *filename, U32 current_size)
{
    static char *needSlackExt[] = {
        ".ms", ".bin", ".def"
    };
    int i;
    F32 ratio = 0;
    const char *ext = filename?strrchr(filename, '.'):NULL;
    if (ext) {
        for (i=0; i<ARRAY_SIZE(needSlackExt); i++) {
            if (stricmp(ext, needSlackExt[i])==0) {
                ratio = 0.05;
                break;
            }
        }
    }
    if (filename[0]=='?') // Special file
        ratio = 0.05;
    return MIN((U64)(current_size * ratio), HOG_MAX_SLACK_SIZE);
}

// Calculates how much slack a file needs
static U64 hogFileGetSlack(HogFile *handle, U32 file)
{
    HogFileListEntry *file_entry = hogGetFileListEntry(handle, file);
    IN_CRITICAL(data_access);
    if (file_entry->header.headerdata.flagFFFE == 0xFFFE && 
            file_entry->header.headerdata.ea_id != HOG_NO_VALUE)
    {
        HogEAListEntry *ea_entry = hogGetEAListEntry(handle, (S32)file_entry->header.headerdata.ea_id);
        if (ea_entry->header.name_id != HOG_NO_VALUE) {
            const char *filename = DataListGetString(&handle->datalist, ea_entry->header.name_id);
            return hogFileGetSlackByName(filename, file_entry->header.size);
        }
    }
    // No filename, assume lightmap, and pack tightly
    return 0;
}

static int hogFileMakeSpecialFiles(HogFile *handle)
{
    U32 size;
    U8 *data;
    IN_CRITICAL(data_access);
    // Name and HeaderData list
    data = DataListWrite(&handle->datalist, &size);
    hogFileAddFile(handle, "?DataList", data, size, _time32(NULL), NULL, 0, 0, NULL);
    // Now that we just added ourself, re-write out the data
    data = DataListWrite(&handle->datalist, &size);
    hogFileUpdateFile(handle, "?DataList", data, size, _time32(NULL), NULL, 0, 0, NULL);
    return 1;
}

static U32 hogFileOffsetOfOpJournal(HogFile *handle)
{
    return sizeof(HogHeader);
}

static U32 hogFileOffsetOfDLJournal(HogFile *handle)
{
    IN_CRITICAL(data_access);
    return sizeof(HogHeader) +
        handle->header.op_journal_size;
}

static U32 hogFileOffsetOfFileList(HogFile *handle)
{
    IN_CRITICAL(data_access);
    return sizeof(HogHeader) +
        handle->header.op_journal_size +
        handle->header.dl_journal_size;
}

static U32 hogFileOffsetOfEAList(HogFile *handle)
{
    IN_CRITICAL(data_access);
    return sizeof(HogHeader) +
        handle->header.op_journal_size +
        handle->header.dl_journal_size +
        handle->header.file_list_size;
}

static U32 hogFileOffsetOfData(HogFile *handle)
{
    IN_CRITICAL(data_access);
    return sizeof(HogHeader) +
        handle->header.op_journal_size +
        handle->header.dl_journal_size +
        handle->header.file_list_size +
        handle->header.ea_list_size;
}

static HogFileHeader *hogSerializeFileHeaders(HogFile *handle)
{
    U32 i;
    HogFileHeader *file_headers = NULL;
    IN_CRITICAL(data_access);
    file_headers = calloc(sizeof(HogFileHeader), handle->filelist_count);
    for (i=0; i<handle->filelist_count; i++) {
        if (handle->filelist[i].in_use) {
            if (!handle->filelist[i].header.size) {
                handle->filelist[i].header.offset = 0;
            }
            file_headers[i] = handle->filelist[i].header;
        } else {
            file_headers[i].size = HOG_NO_VALUE;
        }
        endianSwapStructIfBig(parseHogFileHeader, &file_headers[i]);
    }
    return file_headers;
}

static HogEAHeader *hogSerializeEAHeaders(HogFile *handle)
{
    U32 i;
    HogEAHeader *ea_headers;
    IN_CRITICAL(data_access);
    ea_headers = calloc(handle->header.ea_list_size, 1);
    for (i=0; i<handle->ealist_count; i++) {
        if (handle->ealist[i].in_use) {
            ea_headers[i] = handle->ealist[i].header;
        } else {
            ea_headers[i].flags = HOGEA_NOT_IN_USE;
        }
        endianSwapStructIfBig(parseHogEAHeader, &ea_headers[i]);
    }
    return ea_headers;
}

static int intRevCompare(const int *i1, const int *i2){
    if(*i1 < *i2)
        return 1;

    if(*i1 == *i2)
        return 0;

    return -1;
}

static void hogGrowFileList(HogFile *handle, U32 new_count)
{
    IN_CRITICAL(data_access);
    while (handle->filelist_count != new_count) {
        hogFileAllocFileEntry(handle);
    }
    qsort(handle->file_free_list, eaiSize(&handle->file_free_list), sizeof(handle->file_free_list[0]), intRevCompare);
    handle->header.file_list_size = sizeof(HogFileHeader) * handle->filelist_count;
}

static void hogGrowEAList(HogFile *handle, U32 new_count)
{
    IN_CRITICAL(data_access);
    while (handle->ealist_count != new_count) {
        hogFileAllocEAEntry(handle);
    }
    qsort(handle->ea_free_list, eaiSize(&handle->ea_free_list), sizeof(handle->ea_free_list[0]), intRevCompare);
    handle->header.ea_list_size = sizeof(HogEAHeader) * handle->ealist_count;
}

static char *makeMutexName(const char *fname, const char *prefix)
{
    int i, len = 1 + (int)strlen(prefix); 
    char *buf, full[MAX_PATH];
    if(fname[0] == '.' && (fname[1] == '/' || fname[1] == '\\'))
        fname += 2;
    makefullpath_s(fname, SAFESTR(full));
    len += (int)strlen(full);
    buf = malloc(len);
    strcpy_s(buf, len, full);
    for(i = 0; i < len; i++)
    {
        if(buf[i] == '/' || buf[i] == '\\' || buf[i] == ':')
            buf[i] = '_';
    }
    strcat_s(buf, len, prefix);
    return buf;
}

int hogFileWriteFresh(HogFile *handle, const char *fname)
{
    FILE *fout;
    U8 scratch[HOG_TEMP_BUFFER_SIZE]={0};
    int success=0;
    HogFileHeader *file_headers = NULL;
    HogEAHeader *ea_headers = NULL;
    U32 i;
    U64 offset=0;
    U32 total_files;
    U32 total_eas;
    U64 loc;

#define CHECKED_FWRITE(ptr, size, required) if (hog_mode_no_data && !(required)) { fseek(fout, size, SEEK_CUR); } else { if ((U32)fwrite(ptr, 1, size, fout) != size) goto fail; }

    assert(sizeof(scratch) >= handle->header.dl_journal_size);

    ZeroStruct(scratch);

    fout = fopen(fname,"wcb");
    if (!fout)
        return 0;

    ENTER_CRITICAL(file_access);
    ENTER_CRITICAL(data_access);

    handle->mutex.name = makeMutexName(fname, "");
    handle->version.name = makeMutexName(fname, "Ver");
    handle->version.semaphore = CreateSemaphoreA(NULL, 1, LONG_MAX, handle->version.name);
    devassertmsg(handle->version.semaphore, "Failed to open semaphore object");

    // Turn special data into files
    hogFileMakeSpecialFiles(handle);

    // Write a zeroed header first so that if this file is read, it is
    //  known as a bad file
    {
        HogHeader header;
        ZeroStruct(&header);
        CHECKED_FWRITE(&header, sizeof(header),true);
    }

    total_files = handle->filelist_count;
    total_files += (U32)(total_files * FILELIST_PAD); // Add padding for growth
    hogGrowFileList(handle, total_files);
    total_eas = handle->ealist_count;
    total_eas += (U32)(total_eas * FILELIST_PAD); // Add padding for growth
    hogGrowEAList(handle, total_eas);

    // Fill in header (but write later)
    handle->header.file_list_size = sizeof(HogFileHeader) * handle->filelist_count;
    handle->header.ea_list_size = sizeof(HogEAHeader) * handle->ealist_count;
    handle->header.datalist_fileno = hogFileFind(handle, "?DataList");

    // Write operations journal block
    // TODO: Put journal entry of DeletePigFile?
    CHECKED_FWRITE(scratch, handle->header.op_journal_size,true);
    // Write DataList journal block
    CHECKED_FWRITE(scratch, handle->header.dl_journal_size,true);

    // Fill in offsets
    offset = hogFileOffsetOfData(handle);
    for (i=0; i<handle->filelist_count; i++) {
        if (handle->filelist[i].in_use) {
            if (handle->filelist[i].header.size) {
                handle->filelist[i].header.offset = offset;
                offset += handle->filelist[i].header.size + hogFileGetSlack(handle, i);
            }
        }
    }

    // Assemble and write HogFileHeaders and slack
    file_headers = hogSerializeFileHeaders(handle);
    CHECKED_FWRITE(file_headers, handle->header.file_list_size,true);
    SAFE_FREE(file_headers);

    // Assemble and write EAList and slack
    ea_headers = hogSerializeEAHeaders(handle);
    CHECKED_FWRITE(ea_headers, handle->header.ea_list_size,true);
    SAFE_FREE(ea_headers);

    // Write files
    offset = hogFileOffsetOfData(handle);
    loc = ftell(fout);
    assert(loc == offset);
    for (i=0; i<handle->filelist_count; i++)
    {
        if (handle->filelist[i].in_use && handle->filelist[i].header.size)
        {
            assert(offset <= handle->filelist[i].header.offset);
            if (offset < handle->filelist[i].header.offset) {
                U32 amt = (U32)(handle->filelist[i].header.offset - offset);
                assert(amt <= sizeof(scratch));
                // Write slack
                CHECKED_FWRITE(scratch, amt, false);
                offset += amt;
            }
            loc = ftell(fout);
            assert(loc == handle->filelist[i].header.offset);
            assert(loc == offset);
            CHECKED_FWRITE(handle->filelist[i].data, handle->filelist[i].header.size, i==handle->header.datalist_fileno);
            offset += handle->filelist[i].header.size;
        }
    }

    // Go back and write header, now that we're done writing
    fseek(fout, 0, SEEK_SET);
    {
        HogHeader header = handle->header;
        endianSwapStructIfBig(parseHogFileHeader, &header);
        CHECKED_FWRITE(&header, sizeof(header), true);
    }
    // Clear journal
    CHECKED_FWRITE(scratch, handle->header.op_journal_size, true);

    success = 1;
fail:
    // Cleanup
    fclose(fout);
    SAFE_FREE(file_headers);
    SAFE_FREE(ea_headers);
    // Free data we allocated in here
    SAFE_FREE(handle->filelist[handle->header.datalist_fileno].data);
    LEAVE_CRITICAL(file_access);
    LEAVE_CRITICAL(data_access);
    return success;
#undef CHECKED_FWRITE
}

// Does *not* take ownership of data pointers
int hogCreateFromMem(NewPigEntry *entries, int count, const char *fname)
{
    HogFile hog_file={0};
    int success=0;
    int i;

    qsort(entries,count,sizeof(entries[0]),pigCmpEntryPathname);

    hogFileCreate(&hog_file);
    for (i=0; i<count; i++) {
        if (!hog_mode_no_data) {
            hogFileAddFile2(&hog_file, &entries[i]);
        } else {
            U32 disksize = entries[i].pack_size?entries[i].pack_size:entries[i].size;
            U32 unpacksize = entries[i].pack_size?entries[i].size:0;
            hogFileAddFile(&hog_file, entries[i].fname, entries[i].data, disksize, entries[i].timestamp, NULL, 0, unpacksize, entries[i].checksum);
        }
    }

    success = hogFileWriteFresh(&hog_file, fname);

    //hogFileDumpInfo(&hog_file, 2, 1);

    // Cleanup
    hogFileDestroy(&hog_file);
    return success;
}

static int checkedWriteData(HogFile *handle, void *data, U32 size, U64 offs)
{
    int ret=0;
#define FAIL(code) { ret = code; goto fail; }
    ENTER_CRITICAL(file_access);
    assert(!handle->read_only);
    if (hog_debug_check)
    {
        U32 size1 = size / 2;
        U32 size2 = size - size1;
        U64 offs2 = offs + size1;
        if (size1) {
            if (0 != fseek(handle->file, offs, SEEK_SET))
                FAIL(3);
            if (size1 != (U32)fwrite(data, 1, size1, handle->file))
                FAIL(4);
            DEBUG_CHECK();
        }
        if (0 != fseek(handle->file, offs2, SEEK_SET))
            FAIL(5);
        if (size2 != (U32)fwrite((U8*)data + size1, 1, size2, handle->file))
            FAIL(6);
        DEBUG_CHECK();
    } else {
        if (0 != fseek(handle->file, offs, SEEK_SET))
            FAIL(1);
        if (size != (U32)fwrite(data, 1, size, handle->file))
            FAIL(2);
    }
fail:
    LEAVE_CRITICAL(file_access);
    return ret;
#undef FAIL
}

static int checkedReadData(HogFile *handle, U8 *data, U32 size, U64 offs)
{
    int ret=0;
    ENTER_CRITICAL(file_access);
    if (0 != fseek(handle->file, offs, SEEK_SET))
        ret = 1;
    else if (size != (U32)fread(data, 1, size, handle->file))
        ret = 2;
    LEAVE_CRITICAL(file_access);
    return ret;
}

static bool hogJournalHasWork(HogFile *handle, U8 * journal)
{
    U32 possible_terminator;
    U32 journal_entry_size = *(U32*)journal;
    if (journal_entry_size==0) {// No journal data
        memlog_printf(&hogmemlog, "0x%08x: Journal:None", handle);
        return false;
    }
    memlog_printf(&hogmemlog, "0x%08x: Journal:Size: %d", handle, journal_entry_size);
    if (journal_entry_size+sizeof(U32) > HOG_OP_JOURNAL_SIZE) {// Invalid
        memlog_printf(&hogmemlog, "0x%08x: Journal:Too large", handle);
        return false;
    }
    // Look for terminator
    possible_terminator = *(U32*)&journal[journal_entry_size+sizeof(U32)];
    if (isBigEndian())
        possible_terminator = endianSwapU32(possible_terminator);
    if (possible_terminator == HOG_JOURNAL_TERMINATOR) {
        memlog_printf(&hogmemlog, "0x%08x: Journal:Found and terminated", handle);
        return true;
    }
    memlog_printf(&hogmemlog, "0x%08x: Journal:Not terminated", handle);
    // Was in the middle of writing the journal
    return false;
}

static void hogFillInFileModBasics(HogFile *handle, HogFileMod *mod)
{
    mod->filelist_offset = hogFileOffsetOfFileList(handle);
    mod->ealist_offset = hogFileOffsetOfEAList(handle);
}

static int hogJournalDoWork(HogFile *handle, U8 * journal)
{
    HogFileMod mod;
    int ret;
    HogOpJournalAction op;

    IN_CRITICAL(data_access);

    journal += sizeof(U32);
    op = *(HogOpJournalAction*)journal; // endianSwapped in hogJournalHaswork
    memlog_printf(&hogmemlog, "0x%08x: Journal:Op: %d", handle, op);

    switch(op) {
    xcase HOJ_DELETE:
    {
        HFJDelete *journal_delete = (HFJDelete *)journal;
        endianSwapStructIfBig(parseHFMDelete, &journal_delete->del);
        memlog_printf(&hogmemlog, "0x%08x: Journal:Del: file_id:%d ea_id:%d", handle, journal_delete->del.file_index, journal_delete->del.ea_id);
        // Re-do delete operation (before we read in the data)
        hogFillInFileModBasics(handle, &mod);
        mod.type = HFM_DELETE;
        mod.del = journal_delete->del;
        if (ret=hogFileModifyDoDeleteInternal(handle, &mod))
            return NESTED_ERROR(1, ret);
    }
    xcase HOJ_ADD:
    {
        HFJAdd *journal_add = (HFJAdd *)journal;
        endianSwapStructIfBig(parseHFMAdd, &journal_add->add);
        memlog_printf(&hogmemlog, "0x%08x: Journal:Add: file_id:%d size:%d timestamp:%d offset:%I64d", handle,
            journal_add->add.file_index, journal_add->add.size, journal_add->add.timestamp, journal_add->add.offset);
        if (journal_add->add.headerdata.flagFFFE == 0xFFFE) {
            memlog_printf(&hogmemlog, "0x%08x: Journal:Add: ea_id:%d  unpacksize:%d name_id:%d header_data_id:%d crc:%08X", handle,
                journal_add->add.headerdata.ea_id, journal_add->add.ea_data.unpacksize, journal_add->add.ea_data.name_id, journal_add->add.ea_data.header_data_id,
                journal_add->add.checksum);
        } else {
            assert(!"untested"); // Did I get my printf formatting right?
            memlog_printf(&hogmemlog, "0x%08x: Journal:Add: NO ea_id; headerdata: 0x%0I64x", handle,
                journal_add->add.headerdata.raw);
        }
        hogFillInFileModBasics(handle, &mod);
        mod.type = HFM_ADD;
        mod.add = journal_add->add;
        if (ret=hogFileModifyDoAddInternal(handle, &mod))
            return NESTED_ERROR(2, ret);
    }
    xcase HOJ_UPDATE:
    {
        HFJUpdate *journal_update = (HFJUpdate *)journal;
        endianSwapStructIfBig(parseHFMUpdate, &journal_update->update);
        memlog_printf(&hogmemlog, "0x%08x: Journal:Update: file_id:%d size:%d timestamp:%d offset:%I64d", handle,
            journal_update->update.file_index, journal_update->update.size, journal_update->update.timestamp, journal_update->update.offset);
        if (journal_update->update.headerdata.flagFFFE == 0xFFFE) {
            memlog_printf(&hogmemlog, "0x%08x: Journal:Update: ea_id:%d  crc:%08X", handle,
                journal_update->update.headerdata.ea_id, 
                journal_update->update.checksum);
        } else {
            assert(!"untested"); // Did I get my printf formatting right?
            memlog_printf(&hogmemlog, "0x%08x: Journal:Update: NO ea_id; headerdata: 0x%0I64x", handle,
                journal_update->update.headerdata.raw);
        }
        hogFillInFileModBasics(handle, &mod);
        mod.type = HFM_UPDATE;
        mod.update = journal_update->update;
        if (ret=hogFileModifyDoUpdateInternal(handle, &mod))
            return NESTED_ERROR(3, ret);
    }
    xcase HOJ_MOVE:
    {
        HFJMove *journal_move = (HFJMove *)journal;
        endianSwapStructIfBig(parseHFMMove, &journal_move->move);
        memlog_printf(&hogmemlog, "0x%08x: Journal:Move: file_id:%d offset:%I64d", handle,
            journal_move->move.file_index, journal_move->move.new_offset);
        // Move operation finished (copying data), just update the header
        hogFillInFileModBasics(handle, &mod);
        mod.type = HFM_MOVE;
        mod.move = journal_move->move;
        if (ret=hogFileModifyDoMoveInternal(handle, &mod))
            return NESTED_ERROR(4, ret);
    }
    xcase HOJ_FILELIST_RESIZE:
    {
        HFJFileListResize *journal_filelist_resize = (HFJFileListResize *)journal;
        endianSwapStructIfBig(parseHFMFileListResize, &journal_filelist_resize->filelist_resize);
        memlog_printf(&hogmemlog, "0x%08x: Journal:FileListResize: old ea pos:0x%x  new ea pos:0x%x  new ea size:%d", handle,
            journal_filelist_resize->filelist_resize.old_ealist_pos,
            journal_filelist_resize->filelist_resize.new_ealist_pos,
            journal_filelist_resize->filelist_resize.new_ealist_size);
        
        hogFillInFileModBasics(handle, &mod);
        mod.type = HFM_FILELIST_RESIZE;
        mod.filelist_resize = journal_filelist_resize->filelist_resize;
        if (ret=hogFileModifyDoFileListResizeInternal(handle, &mod))
            return NESTED_ERROR(5, ret);
        handle->header.file_list_size = journal_filelist_resize->filelist_resize.new_filelist_size;
        handle->header.ea_list_size = journal_filelist_resize->filelist_resize.new_ealist_size;
    }
    xcase HOJ_DATALISTFLUSH:
    {
        HFJDataListFlush *journal_datalistflush = (HFJDataListFlush *)journal;
        endianSwapStructIfBig(parseHFMDataListFlush, &journal_datalistflush->datalistflush);
        memlog_printf(&hogmemlog, "0x%08x: Journal:DataListFlush", handle);
        hogFillInFileModBasics(handle, &mod);
        mod.type = HFM_DATALISTFLUSH;
        mod.datalistflush = journal_datalistflush->datalistflush;
        if (ret=hogFileModifyDoDataListFlushInternal(handle, &mod))
            return NESTED_ERROR(6, ret);
    }
    xdefault:
        hogShowError(handle->filename, 1, "Invalid item in journal", op);
        return 1;
    }
    return 0;
}

// Expects pre-endian swapped data
static int hogJournalJournal(HogFile *handle, void *data, U32 size)
{
    U64 offs = hogFileOffsetOfOpJournal(handle);
    int ret;
    U32 data_to_write;

    fflush(handle->file);

    handle->last_journal_size = size;
    data_to_write = endianSwapIfBig(U32, size);
    if (ret=checkedWriteData(handle, &data_to_write, sizeof(U32), offs))
        return NESTED_ERROR(1, ret);
    offs += sizeof(U32);
    if (ret=checkedWriteData(handle, data, size, offs))
        return NESTED_ERROR(2, ret);
    offs += size;
    data_to_write = endianSwapIfBig(U32, HOG_JOURNAL_TERMINATOR);
    if (ret=checkedWriteData(handle, &data_to_write, sizeof(U32), offs))
        return NESTED_ERROR(3, ret);

    fflush(handle->file);

    return 0;
}

static int hogJournalReset(HogFile *handle)
{
    U32 zeroes[HOG_OP_JOURNAL_SIZE]={0};
    U64 offs;
    int ret;
    if (!handle->last_journal_size)
        return 1; // Invalid call
    // Zero terminator
    offs = hogFileOffsetOfOpJournal(handle) + sizeof(U32) + handle->last_journal_size;
    if (ret=checkedWriteData(handle, zeroes, sizeof(U32), offs))
        return NESTED_ERROR(1, ret);
    offs = hogFileOffsetOfOpJournal(handle);
    if (ret=checkedWriteData(handle, zeroes, sizeof(U32) + handle->last_journal_size, offs))
        return NESTED_ERROR(2, ret);
    handle->last_journal_size = 0;
    return 0;
}

static int hogDLJournalDoWork(HogFile *handle, U8 *journal)
{
    DLJournalHeader *header = (DLJournalHeader *)journal;
    DataListJournal dljournal;
    int ret;
    IN_CRITICAL(data_access);
    endianSwapStructIfBig(parseDLJournalHeader, header);
    memlog_printf(&hogmemlog, "0x%08x: DLJournal: inuse: %d old: %d new: %d", handle,
        header->inuse_flag, header->oldsize, header->size);
    if (header->inuse_flag) {
        // Didn't finish writing, use oldsize
        dljournal.size = header->oldsize;
    } else {
        // Did finish writing, use size (oldsize might be incorrect)
        dljournal.size = header->size;
    }
    dljournal.data = journal + sizeof(DLJournalHeader);
    if (ret=DataListApplyJournal(&handle->datalist, &dljournal)) { //Endianness handled internally
        memlog_printf(&hogmemlog, "0x%08x: DLJournal: failed to apply", handle);
        return NESTED_ERROR(1, ret);
    }
    handle->datalist_diff_size = dljournal.size; // Don't overwrite the journal until it's flushed!
    return 0;
}

static int hogDLJournalSave(HogFile *handle, HogFileMod *mod)
{
    DLJournalHeader header;
    int ret;
    assert(mod->type == HFM_DATALISTDIFF);
    // First Append data
    if (ret=checkedWriteData(handle, mod->datalistdiff.data, mod->datalistdiff.size, mod->datalistdiff.offset))
        return NESTED_ERROR(1, ret);

    // Then safely update the size
#define SAVE_FIELD(fieldname) if (ret=checkedWriteData(handle, &header.fieldname, sizeof(header.fieldname), mod->datalistdiff.size_offset + offsetof(DLJournalHeader, fieldname))) return NESTED_ERROR(2, ret);
    header.inuse_flag = endianSwapIfBig(U32,1);
    SAVE_FIELD(inuse_flag);
    header.size = endianSwapIfBig(U32, mod->datalistdiff.newsize);
    SAVE_FIELD(size);
    header.inuse_flag = endianSwapIfBig(U32, 0);
    SAVE_FIELD(inuse_flag);
    header.oldsize = endianSwapIfBig(U32, mod->datalistdiff.newsize);
    SAVE_FIELD(oldsize);
#undef SAVE_FIELD
    return 0;
}


HogFileIndex hogFileFind(HogFile *handle, const char *relpath) // Find an entry in a Hog
{
    HogFileIndex file_index=HOG_NO_VALUE;

    hogFileCheckForModifications(handle);

    ENTER_CRITICAL(data_access);
    if (!stashFindInt(handle->fn_to_index_lookup, relpath, &file_index))
        file_index = HOG_NO_VALUE;
    LEAVE_CRITICAL(data_access);
    return file_index;
}

void hogFileGetSizes(HogFile *handle, HogFileIndex file_index, U32 *unpacked, U32 *packed)
{
    U32 pack_size=0, unpacked_size;
    HogFileListEntry *file_entry;
    ENTER_CRITICAL(data_access);
    file_entry = hogGetFileListEntry(handle, file_index);

    unpacked_size = file_entry->header.size;
    if (file_entry->header.headerdata.flagFFFE == 0xFFFE && 
        file_entry->header.headerdata.ea_id != HOG_NO_VALUE)
    {
        HogEAListEntry *ea_entry = hogGetEAListEntry(handle, (S32)file_entry->header.headerdata.ea_id);
        if (ea_entry->header.name_id != HOG_NO_VALUE) {
            if (ea_entry->header.unpacked_size) {
                pack_size = unpacked_size;
                unpacked_size = ea_entry->header.unpacked_size;
            }
        }
    }
    if (unpacked)
        *unpacked = unpacked_size;
    if (packed)
        *packed = pack_size;
    LEAVE_CRITICAL(data_access);
}

static HogFileListEntry *hogFileWaitForFile(HogFile *handle, HogFileIndex file)
{
    HogFileListEntry *file_entry;
    int ret;
    hogAcquireMutex(handle); // No other process can be writing to this file!
    ENTER_CRITICAL(data_access);
    do {
        file_entry = hogGetFileListEntrySafe(handle, file, false);
        if (!file_entry) {
            return NULL;
        }
        if (file_entry->dirty) {
            // This file is being worked on, flush changes before trying to read them!
            LEAVE_CRITICAL(data_access);
            if (ret=hogFileModifyFlush(handle)) {
                assertmsg(0, "Failure flushing!");
            }
            ENTER_CRITICAL(data_access);
        }
    } while (file_entry->dirty);
    return file_entry;
}

U32 hogFileExtractBytes(HogFile *handle, HogFileIndex file, void *buf, U32 pos, U32 size)
{
    bool zipped=false;
    U32 pack_size, unpacked_size;
    HogFileListEntry *file_entry;
    U32 ret;

    file_entry = hogFileWaitForFile(handle, file); // Enters data_access critical and mutex
    if (!file_entry) {
        ret = 0;
        goto fail;
    }

    hogFileGetSizes(handle, file, &unpacked_size, &pack_size);

    zipped = pack_size > 0;

    ENTER_CRITICAL(file_access);
    ret = PigExtractBytesInternal(handle->file, handle, file, buf, pos, size, file_entry->header.offset, unpacked_size, pack_size, false);
    LEAVE_CRITICAL(file_access);
fail:
    LEAVE_CRITICAL(data_access);
    hogReleaseMutex(handle, false);
    return ret;
}

void *hogFileExtract(HogFile *handle, HogFileIndex file, U32 *count)
{
    char *data=NULL;
    U32 numread;
    U32 total;
    HogFileListEntry *file_entry;

    if (!handle || !handle->file)
        return NULL;
    if (file==HOG_NO_VALUE)
        return NULL;

    // Call this just to make the read atomic, no changes while we're reading
    file_entry = hogFileWaitForFile(handle, file); // Enters data_access critical and mutex
    if (!file_entry) {
        hogShowError(handle->filename,14,"File disapepared while reading", file);
        *count = 0;
        goto fail;
    }

    hogFileGetSizes(handle, file, &total, NULL);

    data = malloc(total + 1);
    if (!data) {
        hogShowError(handle->filename,15,"Failed to allocate memory",total+1);
        *count = 0;
        goto fail;
    }

    numread = hogFileExtractBytes(handle, file, data, 0, total);
    if (numread != total)
    {
        SAFE_FREE(data);
        hogShowError(handle->filename,13,"Read wrong number of bytes",numread);
        *count = 0;
        goto fail;
    }
    *count = numread;
fail:
    LEAVE_CRITICAL(data_access);
    hogReleaseMutex(handle, false);
    return data;
}

// This is a helper function for hogFileExtractBytesCompressed.  It replaces a call to PigExtractBytesInternal,
// from which it's interior is cut-and-pasted.
static U32 extractRawInternal(FILE *file, void *buf, U32 pos, U32 size, U64 fileoffset)
{
    U32 numread;

    if (size==0) {
        return 0;
    }

    EnterCriticalSection(&PigCritSec);
    if (0!=fseek(file, fileoffset + pos, SEEK_SET)) {
        LeaveCriticalSection(&PigCritSec);
        return 0;
    }

    numread = fread(buf, 1, size, file);
    LeaveCriticalSection(&PigCritSec);
    if (numread!=size)
        return 0;

    return numread;
}

// This is for the patcher.  The piglib version asserts that it won't work on hog files, so instead of
// attempting to fix that function, a new function shall be made that shamelessly copies hogFileExtractBytes.
// The function returns the file segment as it's stored in the hogg, compressed or uncompressed.
U32 hogFileExtractRawBytes(HogFile *handle, HogFileIndex file, void *buf, U32 pos, U32 size)
{
    HogFileListEntry *file_entry;
    U32 ret;

    file_entry = hogFileWaitForFile(handle, file); // Enters data_access critical and mutex
    if (!file_entry) {
        ret = 0;
        goto fail;
    }

    ENTER_CRITICAL(file_access);
    ret = extractRawInternal(handle->file, buf, pos, size, file_entry->header.offset);
    LEAVE_CRITICAL(file_access);
fail:
    LEAVE_CRITICAL(data_access);
    hogReleaseMutex(handle, false);
    return ret;
}

// Continuing in the line of cut-and-paste functions for the patcher, hogFileExtractCompressed
// copies from hogFileExtract, except that it uses hogFileExtractRawBytes.
void *hogFileExtractCompressed(HogFile *handle, HogFileIndex file, U32 *count)
{
    char *data=NULL;
    U32 numread;
    U32 total, pack_size;
    HogFileListEntry *file_entry;

    if (!handle || !handle->file)
        return NULL;
    if (file==HOG_NO_VALUE)
        return NULL;

    // Call this just to make the read atomic, no changes while we're reading
    file_entry = hogFileWaitForFile(handle, file); // Enters data_access critical and mutex
    if (!file_entry) {
        hogShowError(handle->filename,14,"File disappeared while reading", file);
        *count = 0;
        goto fail;
    }

    hogFileGetSizes(handle, file, &total, &pack_size);

    if(!pack_size) // compressed only
    {
        data = NULL;
        *count = 0;
        goto fail;
    }

    data = malloc(pack_size);
    if (!data) {
        hogShowError(handle->filename,15,"Failed to allocate memory",pack_size);
        *count = 0;
        goto fail;
    }

    numread = hogFileExtractRawBytes(handle, file, data, 0, pack_size);
    if (numread != pack_size)
    {
        SAFE_FREE(data);
        hogShowError(handle->filename,13,"Read wrong number of bytes",numread);
        *count = 0;
        goto fail;
    }
    *count = numread;
fail:
    LEAVE_CRITICAL(data_access);
    hogReleaseMutex(handle, false);
    return data;
}

static int cmpFileOffsets(const HogFileListEntry **a, const HogFileListEntry **b)
{
    S64 diff = (S64)(*a)->header.offset - (S64)(*b)->header.offset;
    return diff>0?1:(diff<0?-1:0);
}


static void hogFileBuildFreeSpaceList(HogFile *handle) // Additionally sets min/max size
{
#ifdef NO_FREELIST
    return;
#else
    U64 data_offset=hogFileOffsetOfData(handle);
    U64 offset;
    HogFileListEntry **file_list=0;
    U32 i;

    IN_CRITICAL(data_access);
    hfstDestroy(&handle->free_space);
    hfstSetStartLocation(&handle->free_space, data_offset);

    offset = 0;
    for (i=0; i<handle->filelist_count; i++) {
        HogFileListEntry *file_entry = &handle->filelist[i];
        if (file_entry->in_use) {
            if (file_entry->header.size) {
                eaPush(&file_list, file_entry);
                if (file_entry->header.offset + file_entry->header.size > offset) {
                    offset = file_entry->header.offset + file_entry->header.size;
                }
            }
        }
    }
    hfstSetTotalSize(&handle->free_space, offset);

    eaQSort(file_list, cmpFileOffsets);

    offset = data_offset;
    for (i=0; i<(U32)eaSize(&file_list); i++) {
        U64 freespace = file_list[i]->header.offset - offset;
        U64 newoffset = file_list[i]->header.offset + file_list[i]->header.size;
        if (file_list[i]->header.offset < offset) {
            // HOGTODO: This is a bad file, delete it!
            assert(0); // Shouldn't happen at startup
            // This file is before we want to start looking
            if (newoffset > offset)
                offset = newoffset; // But it expands past the beginning!
        } else {
            if (freespace > 0) {
                hfstFreeSpace(&handle->free_space, offset, freespace);
            }
            offset = newoffset;
        }
    }

    eaDestroy(&file_list);

#endif
}


int hogFileRead(HogFile *handle, const char *filename) // Load a .HOGG file
{
    U8 *used_list=NULL;
    int numfiles, numeas;
    int i;
    char fn[MAX_PATH];
    struct __stat64 status;
    int err_code=0;
    int err_int_value=-1;
    int ret;
    U8 op_journal[HOG_OP_JOURNAL_SIZE];
    U8 dl_journal[HOG_DL_JOURNAL_SIZE];
    U8 *buffer=NULL;
    int *eaiFilesToPrune=NULL;
#define FAIL(num,str) { err_code = hogShowError(filename,num,str,err_int_value); goto fail; }
#define CHECKED_FREAD(buf, count) if (count != (U32)fread(buf, 1, count, handle->file)) FAIL(1, "Unexpected end of file");

    assert(!handle->file && "Cannot reuse a handle without calling hogFileDestroy() first!");
    assert(!handle->num_files);

    PigCritSecInit();

    ENTER_CRITICAL(data_access);
    ENTER_CRITICAL(file_access);

    strcpy(fn, filename);
    if (!fileExists(fn)) {
        sprintf_s(SAFESTR(fn), "%s.hog", filename);
    }
    if (!fileExists(fn)) {
        sprintf_s(SAFESTR(fn), "%s.hogg", filename);
    }
#ifdef _XBOX
    backSlashes(fn);
#endif
    SAFE_FREE(handle->filename);
    fileLocateWrite(fn, fn);
    handle->filename = strdup(fn);
    handle->mutex.name = makeMutexName(fn, "");
    handle->version.name = makeMutexName(fn, "Ver");
    handle->version.semaphore = CreateSemaphoreA(NULL, 0, LONG_MAX, handle->version.name);
    devassertmsg(handle->version.semaphore, "Failed to open semaphore object");
    memlog_printf(&hogmemlog, "0x%08x: Loading %s", handle,    fn);
    hogAcquireMutex(handle);
    // Open file
    handle->file = fopen(handle->filename, "r+cb~");
    if (!handle->file) {
        handle->read_only = true;
        handle->file = fopen(handle->filename, "rb~");
    }
    if (!handle->file) {
        FAIL(1,"Error opening hog file");
    }
    if(!_stat64(fn, &status)){
        if(!(status.st_mode & _S_IFREG)) {
            FAIL(1,"Error statting hog file");
        }
    } else {
        FAIL(1,"Error statting hog file");
    }
    handle->file_last_changed = fileLastChanged(handle->filename);
    handle->version.value = hogGetVersion(handle);

    memlog_printf(&hogmemlog, "0x%08x: File size: %d", handle, status.st_size);

    handle->file_size = status.st_size;

    // Read header
    memset(&handle->header, 0, sizeof(handle->header));
    STATIC_INFUNC_ASSERT(sizeof(handle->header)==24); // Changed size of HogHeader but didn't update reading code to read the extra values
    CHECKED_FREAD(&handle->header, 24);
    endianSwapStructIfBig(parseHogHeader, &handle->header);
    if (handle->header.version != HOG_VERSION) {
        err_int_value = handle->header.version;
        FAIL(2,"Hog file incorrect/unreadable version");
    }
    if (handle->header.op_journal_size > HOG_OP_JOURNAL_SIZE) {
        err_int_value = handle->header.op_journal_size;
        FAIL(3,"Hog file has too large of op journal to read");
    }
    if (handle->header.dl_journal_size > HOG_DL_JOURNAL_SIZE) {
        err_int_value = handle->header.dl_journal_size;
        FAIL(3,"Hog file has too large of dl journal to read");
    }
    if (handle->header.hog_header_flag != HOG_HEADER_FLAG) {
        err_int_value = handle->header.hog_header_flag;
        FAIL(4,"Hog file has invalid header flag");
    }

    // read operation journal
    CHECKED_FREAD(op_journal, handle->header.op_journal_size);
    endianSwapStructIfBig(parseHogOpJournalHeader, op_journal);

    if (hogJournalHasWork(handle, op_journal)) {
        memlog_printf(&hogmemlog, "0x%08x: Has OP Journal", handle);
        err_code = hogJournalDoWork(handle, op_journal); // Do work that needs to be done before continuing
        if (err_code)
            FAIL(err_code, "Failed to recover from op journal");
    }

    fseek(handle->file, hogFileOffsetOfDLJournal(handle), SEEK_SET);
    // read DL journal
    CHECKED_FREAD(dl_journal, handle->header.dl_journal_size);

    dynArrayFitStructs(&handle->filelist, &handle->filelist_max, handle->header.file_list_size / sizeof(HogFileHeader));
    dynArrayFitStructs(&handle->ealist, &handle->ealist_max, handle->header.ea_list_size / sizeof(HogEAHeader));
    numfiles = handle->header.file_list_size / sizeof(HogFileHeader);
    handle->filelist_count = numfiles;
    numeas = handle->header.ea_list_size / sizeof(HogEAHeader);
    handle->ealist_count = numeas;

    // Read FileHeaders
    buffer = malloc(MAX(handle->header.file_list_size, handle->header.ea_list_size));
    CHECKED_FREAD(buffer, handle->header.file_list_size);

    // Fill in HogFileHeader list
    for (i=numfiles-1; i>=0; i--) // Start at end to get freelist in a better order
    {
        HogFileHeader *hfh = &((HogFileHeader*)buffer)[i];
        HogFileListEntry *file_entry = &handle->filelist[i];
        endianSwapStructIfBig(parseHogFileHeader, hfh);
        if (hfh->size == HOG_NO_VALUE) {
            file_entry->in_use = 0;
            eaiPush(&handle->file_free_list, i+1);
        } else {
            file_entry->in_use = 1;
            handle->num_files++;
            file_entry->header = *hfh;
        }
    }

    // Read EA Headers
    CHECKED_FREAD(buffer, handle->header.ea_list_size);

    // Fill in the HogEAHeader list
    for (i=numeas-1; i>=0; i--) // Start at end to get freelist in a better order
    {
        HogEAHeader *hfh = &((HogEAHeader*)buffer)[i];
        HogEAListEntry *ea_entry = &handle->ealist[i];
        endianSwapStructIfBig(parseHogEAHeader, hfh);
        if (hfh->flags & HOGEA_NOT_IN_USE) {
            ea_entry->in_use = 0;
            eaiPush(&handle->ea_free_list, i+1);
        } else {
            ea_entry->in_use = 1;
            ea_entry->header = *hfh;
        }
    }

    // Sort lists to use lower file numbers first
    qsort(handle->file_free_list, eaiSize(&handle->file_free_list), sizeof(handle->file_free_list[0]), intRevCompare);
    qsort(handle->ea_free_list, eaiSize(&handle->ea_free_list), sizeof(handle->ea_free_list[0]), intRevCompare);

    // Verify FileHeader integrity
    for (i=0; i<numfiles; i++) {
        HogFileListEntry *file_entry = &handle->filelist[i];
        HogEAListEntry *ea_entry=NULL;
        U64 entry_size;
        if (!file_entry->in_use)
            continue;
        if (file_entry->header.headerdata.flagFFFE == 0xFFFE) {
            S32 ea_id = (S32)file_entry->header.headerdata.ea_id;
            if (ea_id != HOG_NO_VALUE) {
                if (ea_id < 0 || ea_id >= (S32)handle->ealist_count) {
                    err_int_value = ea_id;
                    FAIL(5, "Invalid ea_id");
                    // TODO: Fix (delete file?), not error
                } else {
                    ea_entry = &handle->ealist[ea_id];
                    if (!ea_entry->in_use) {
                        err_int_value = ea_id;
                        FAIL(6, "Invalid ea_id");
                        // TODO: Fix (delete file?), not error
                    }
                }
            }
        }
        entry_size = file_entry->header.size;
        if (entry_size && (file_entry->header.offset + file_entry->header.size - 1 > handle->file_size) && !hog_mode_no_data)
        {
            err_int_value = i;
            FAIL(7,"Fileheader extends past end of .hogg, probably corrupt .hogg file!  #");
            // TODO: Fix (delete file?), not error
        }
    }

    // Read ?DataList
    if (handle->header.datalist_fileno!=HOG_NO_VALUE) {
        int filesize;
        U8 *filedata = hogFileExtract(handle, handle->header.datalist_fileno, &filesize);
        if (!filedata) {
            // Corrupt datalist!  Total failure?
            FAIL(8, "Missing DataList in Hogg file");
        } else {
            S32 ret;
            ret = DataListRead(&handle->datalist, filedata, filesize);
            free(filedata);
            if (ret==-1) {
                // Corrupt datalist!  Total failure?
                FAIL(9, "Corrupt DataList in Hogg file");
            }
        }
    }

    // Apply journaled changes to DataList
    hogDLJournalDoWork(handle, dl_journal);

    // Prune orphaned EAList entries, build lookup table
    {
        used_list = calloc(1, handle->ealist_count);
        for (i=0; i<numfiles; i++)
        {
            HogFileListEntry *file_entry = &handle->filelist[i];
            if (!file_entry->in_use)
                continue;
            if (file_entry->header.headerdata.flagFFFE == 0xFFFE) {
                S32 ea_id = (S32)file_entry->header.headerdata.ea_id;
                if (ea_id != HOG_NO_VALUE) {
                    if (ea_id < 0 || ea_id >= (S32)handle->ealist_count) {
                        // Errored above already
                    } else {
                        HogEAListEntry *ea_entry = &handle->ealist[ea_id];
                        if (!ea_entry->in_use) {
                            // Errored above already
                        } else {
                            U32 name_id = ea_entry->header.name_id;
                            U32 header_data_id = ea_entry->header.header_data_id;
                            bool bCorrupt = false;
                            if (header_data_id != HOG_NO_VALUE) {
                                if (!DataListGetData(&handle->datalist, header_data_id, NULL)) {
                                    // Corrupt!
                                    bCorrupt = true;
                                }
                            }
                            if (name_id != HOG_NO_VALUE) {
                                const char *stored_name = DataListGetString(&handle->datalist, name_id);
                                if (!stored_name) {
                                    // This ea_entry and file_entry are effectively corrupt!
                                    // delete the file!
                                    bCorrupt = true;
                                } else {
                                    stashAddInt(handle->fn_to_index_lookup, stored_name, i, true); // Shallow copied name from DataList
                                }
                            }
                            if (bCorrupt) {
                                if (1) {
                                    memlog_printf(&hogmemlog, "0x%08x: Pruning corrupt file (missing name or header_data) #%d", handle, i);
                                    eaiPush(&eaiFilesToPrune, i);
                                    used_list[ea_id] = 1; // Don't let it get pruned yet!
                                } else {
                                    FAIL(10, "Corrupt file - missing name");
                                }
                            } else {
                                used_list[ea_id] = 1;
                            }
                        }
                    }
                }
            }
        }
        for (i=0; i<numeas; i++) {
            HogEAListEntry *ea_entry = &handle->ealist[i];
            if (!used_list[i]) {
                if (!ea_entry->in_use)
                    continue;
                memlog_printf(&hogmemlog, "0x%08x: Pruning unused ea list item #%d", handle, i);
                // Extra ea_entry, but not referenced by anything!
                ea_entry->in_use = 0;
                eaiPush(&handle->ea_free_list, i+1);
            }
        }
    }

    // Verify ?DataList is named appropriately
    if (handle->header.datalist_fileno != HOG_NO_VALUE) {
        HogFileIndex file = hogFileFind(handle, "?DataList");
        if (file != handle->header.datalist_fileno) {
            FAIL(11, "Corrupt ?DataList entry in Hogg file");
            if (file != HOG_NO_VALUE) {
                // DataList must be corrupt
            } else {
                // Header must be corrupt, as the DataList it pointed to had no ?DataList entry
            }
        }
    }

    // Build freelist before anything that may need to make modifications
    hogFileBuildFreeSpaceList(handle); // Additionally sets min/max size

    // Now, do any verification that may need to make modifications

    // Remove files previously marked as corrupt
    for (i=eaiSize(&eaiFilesToPrune)-1; i>=0; i--)
    {
        int j;
        int file_index = eaiFilesToPrune[i];
        HogFileListEntry *file_entry = &handle->filelist[file_index];
        assert(file_entry->in_use); // Internal logic error?
        // We're deleting a corrupt file
        // If any other files reference this file's datalist_ids,
        //  we should at least not remove those datalist entries (although
        //  we should probably additionally remove those files as they may
        //  be corrupt).
        if (file_entry->header.headerdata.flagFFFE == 0xFFFE) {
            S32 ea_id = (S32)file_entry->header.headerdata.ea_id;
            if (ea_id != HOG_NO_VALUE) {
                HogEAListEntry *ea_entry = &handle->ealist[ea_id];
                U32 header_data_id = ea_entry->header.header_data_id;
                U32 name_id = ea_entry->header.name_id;
                bool clear_name_id = false, clear_header_data_id = false;
                assert(ea_entry->in_use); // Set/verified above
                for (j=0; j<numeas; j++) {
                    if (j!=ea_id) {
                        if (handle->ealist[j].in_use) {
                            if (header_data_id != HOG_NO_VALUE) {
                                if (handle->ealist[j].header.header_data_id == header_data_id)
                                    clear_header_data_id = true;
                                if (handle->ealist[j].header.name_id == header_data_id)
                                    clear_header_data_id = true;
                            }
                            if (name_id != HOG_NO_VALUE) {
                                if (handle->ealist[j].header.header_data_id == name_id)
                                    clear_name_id = true;
                                if (handle->ealist[j].header.name_id == name_id)
                                    clear_name_id = true;
                            }
                        }
                    }
                }
                if (clear_name_id) {
                    // TODO: Also delete conflicted file(s) after all checks?
                    ea_entry->header.name_id = HOG_NO_VALUE;
                }
                if (clear_header_data_id) {
                    // TODO: Also delete conflicted file(s) after all checks?
                    ea_entry->header.header_data_id = HOG_NO_VALUE;
                }
            }
        }

        hogFileModifyDelete(handle, file_index);
    }

    // Verify ea_list integrity (name IDs and HeaderIDs)
    // Prune orphaned DataList entries (adds to journal, may cause a DataListFlush) (for recovery from some ops (Delete, Add))
    {
        DataListJournal dlj = {0};
        int num_datalist_entries = DataListGetNumTotalEntries(&handle->datalist);
        SAFE_FREE(used_list);
        used_list = calloc(1, num_datalist_entries);
        for (i=0; i<numeas; i++)
        {
            HogEAListEntry *ea_entry = &handle->ealist[i];
            const U8 *data;
            U32 size;
            if (!ea_entry->in_use)
                continue;
            if (ea_entry->header.name_id != HOG_NO_VALUE) {
                data = DataListGetData(&handle->datalist, ea_entry->header.name_id, &size);
                if (!data) {
                    err_int_value = ea_entry->header.name_id;
                    FAIL(13, "File with invalid filename index");
                }
                used_list[ea_entry->header.name_id] = 1;
            }
            if (ea_entry->header.header_data_id != HOG_NO_VALUE) {
                data = DataListGetData(&handle->datalist, ea_entry->header.header_data_id, &size);
                if (!data) {
                    err_int_value = ea_entry->header.header_data_id;
                    FAIL(13, "File with invalid header data index");
                }
                used_list[ea_entry->header.header_data_id] = 1;
            }
        }
        for (i=0; i<num_datalist_entries; i++) 
        {
            if (!used_list[i]) {
                U32 size;
                const U8 *data;
                data = DataListGetData(&handle->datalist, i, &size);
                assert(!data == !size);
                if (data) {
                    // Has data, but is not in use
                    memlog_printf(&hogmemlog, "0x%08x: Pruning unused data list item #%d: (0x%x) \"%c%c%c%c%c%c...\"", handle,
                        i, *(U32*)data, data[0], data[1], data[2], data[3], data[4], data[5]);
                    DataListFree(&handle->datalist, i, &dlj);
                }
            }
        }
        if (dlj.size) {
            if (ret=hogFileAddDataListMod(handle, &dlj)) {
                FAIL(NESTED_ERROR(14, ret), "Error while pruning invalid DataList entries");
            }
        }
    }

fail:
    SAFE_FREE(used_list);
    SAFE_FREE(buffer);
    LEAVE_CRITICAL(file_access);
    LEAVE_CRITICAL(data_access);
    if (!err_code)
    {
        // If there were any startup-time changes, flush them now!
        // Must do this after releasing criticals
        if (ret=hogFileModifyFlush(handle)) {
            err_code = ret;
        }
    }
    hogReleaseMutex(handle, false);
    if (err_code)
        hogFileDestroy(handle);
    return err_code;
#undef FAIL
}

HogFile *hogFileReadOrCreate(const char *filename, bool *bCreated) // Load a .HOGG file or creates a new empty one if it does not exist
{
    HogFile *hogg = hogFileAlloc();
    int ret;
    if (fileExists(filename) && fileSize64(filename)>0)
    {
        if (bCreated)
            *bCreated = false;
        ret = hogFileRead(hogg, filename);
        if (ret != 0) // failure
        {
            hogShowError(filename, ret, "Unable to read from hogg file! ", ret);
        }
    }
    else
    {
        if (bCreated)
            *bCreated = true;
        fileForceRemove(filename);
        if (!hogCreateFromMem(0, 0, filename))
            hogShowError(filename, 1, "Unable to create hogg file! ", 0);
        ret = hogFileRead(hogg, filename);
        if (ret != 0)
            hogShowError(filename, ret, "Unable to read from hogg file! ", ret);
    }

    return hogg;
}


U32 hogFileGetNumFiles(HogFile *handle)
{
    U32 ret;
    ENTER_CRITICAL(data_access);
    ret = handle->filelist_count;
    LEAVE_CRITICAL(data_access);
    return ret;
}

U32 hogFileGetNumUsedFiles(HogFile *handle)
{
    U32 ret;
    ENTER_CRITICAL(data_access);
    ret = handle->num_files;
    LEAVE_CRITICAL(data_access);
    return ret;
}

const char *hogFileGetArchiveFileName(HogFile *handle)
{
    return handle->filename;
}

const char *hogFileGetFileName(HogFile *handle, int index)
{
    HogFileListEntry *file_entry;
    HogEAListEntry *ea_entry = NULL;
    const char *ret=NULL;

    ENTER_CRITICAL(data_access);
    if (index < 0 || index >= (int)handle->filelist_count)
        goto cleanup;
    if (!handle->filelist[index].in_use)
        goto cleanup;
    file_entry = hogGetFileListEntry(handle, index);
    if (!file_entry)
        goto cleanup;
    if (file_entry->header.headerdata.flagFFFE == 0xFFFE) {
        ea_entry = hogGetEAListEntry(handle, (S32)file_entry->header.headerdata.ea_id);
    }
    if (!ea_entry || ea_entry->header.name_id==HOG_NO_VALUE) {
        static char buf[32];
        sprintf_s(SAFESTR(buf), "autofiles/%08d.dat", index);
        ret = buf;
    } else {
        ret = DataListGetString(&handle->datalist, ea_entry->header.name_id);
    }
cleanup:
    LEAVE_CRITICAL(data_access);
    return ret;
}

bool hogFileIsSpecialFile(HogFile *handle, int index)
{
    if ((U32)index == handle->header.datalist_fileno)
    {
        return true;
    }
    return false;
}

U32 hogFileGetFileTimestamp(HogFile *handle, int index)
{
    U32 ret=0;
    ENTER_CRITICAL(data_access);
    if (index >= 0 && index < (int)handle->filelist_count)
        ret = handle->filelist[index].header.timestamp;
    LEAVE_CRITICAL(data_access);
    return ret;
}

U32 hogFileGetFileChecksum(HogFile *handle, int index)
{
    U32 ret=0;
    ENTER_CRITICAL(data_access);
    if (index >= 0 && index < (int)handle->filelist_count)
        ret = handle->filelist[index].header.checksum;
    LEAVE_CRITICAL(data_access);
    return ret;
}

U32 hogFileGetFileSize(HogFile *handle, int index)
{
    U32 pack_size, unpacked_size;
    hogFileGetSizes(handle, index, &unpacked_size, &pack_size);
    return unpacked_size;
}

bool hogFileIsZipped(HogFile *handle, int index)
{
    U32 pack_size, unpacked_size;
    hogFileGetSizes(handle, index, &unpacked_size, &pack_size);
    return !!pack_size;
}

// Warning: pointer returned is volatile, and may be invalid after any hog file changes
const U8 *hogFileGetHeaderData(HogFile *handle, int index, U32 *header_size)
{
    HogFileListEntry *file_entry;
    const U8 *ret=NULL;
    ENTER_CRITICAL(data_access);
    
    file_entry = hogGetFileListEntry(handle, index);

    if (file_entry->header.headerdata.flagFFFE == 0xFFFE) {
        HogEAListEntry *ea_entry = hogGetEAListEntry(handle, (S32)file_entry->header.headerdata.ea_id);
        if (ea_entry->header.header_data_id!=HOG_NO_VALUE)
        {
            ret = DataListGetData(&handle->datalist, ea_entry->header.header_data_id, header_size);
        }
    }
    LEAVE_CRITICAL(data_access);
    if (!ret)
        *header_size = 0;
    return ret;
}

void hogScanAllFiles(HogFile *handle, HogFileScanProcessor processor)
{
    U32 j;
    U32 num_files = hogFileGetNumFiles(handle);
    for (j=0; j<num_files; j++) {
        const char *filename = hogFileGetFileName(handle, j);
        if (!filename || hogFileIsSpecialFile(handle, j))
            continue;
        processor(handle, filename);
    }
}



static int hogFileGetNumEAs(HogFile *handle)
{
    int ret;
    ENTER_CRITICAL(data_access);
    ret = handle->ealist_count;
    LEAVE_CRITICAL(data_access);
    return ret;
}

static int hogFileGetNumUsedEAs(HogFile *handle) // Slow
{
    U32 i;
    int ret=0;
    ENTER_CRITICAL(data_access);
    for (i=0; i<handle->ealist_count; i++)
        if (handle->ealist[i].in_use)
            ret++;
    LEAVE_CRITICAL(data_access);
    return ret;
}

// Size that the .hogg file is required to be (for truncating and fragmentation)
static U64 hogFileGetRequiredFileSize(HogFile *handle)
{
    U64 ret = hogFileOffsetOfData(handle);
    U32 i;
    IN_CRITICAL(data_access);
    for (i=0; i<handle->filelist_count; i++) {
        HogFileListEntry *file_entry = &handle->filelist[i];
        if (file_entry->in_use) {
            if (file_entry->header.size) {
                ret = MAX(ret, file_entry->header.offset + file_entry->header.size);
            }
        }
    }
    return ret;
}

F32 hogFileCalcFragmentation(HogFile *handle)
{
    U64 total_size;
    F32 total_size_float;
    F32 delta_size_float;
    U64 used_size=0;
    U32 i;
    ENTER_CRITICAL(data_access);
    total_size=hogFileGetRequiredFileSize(handle);;
    for (i=0; i<handle->filelist_count; i++) {
        HogFileListEntry *file_entry = &handle->filelist[i];
        if (!file_entry->in_use)
            continue;
        used_size += (U64)file_entry->header.size;
    }
    total_size -= hogFileOffsetOfData(handle);
    LEAVE_CRITICAL(data_access);
    total_size_float = (F32)total_size;
    delta_size_float = (F32)(total_size - used_size);
    return delta_size_float / total_size_float;
}

void hogFileDumpInfo(HogFile *handle, int verbose, int debug_verbose)
{
    U32 i;
    ENTER_CRITICAL(data_access);
    if (debug_verbose) { // header info
        printf("HogFile Info:\n");
        printf("  HogFile version: %d\n", handle->header.version);
        printf("  Number of Files: %ld (%ld)\n", hogFileGetNumUsedFiles(handle), hogFileGetNumFiles(handle));
        printf("  Number of EAs: %ld (%ld)\n", hogFileGetNumUsedEAs(handle), hogFileGetNumEAs(handle));
        printf("  Number of Names and HeaderData blocks: %ld (%ld)\n", DataListGetNumEntries(&handle->datalist), DataListGetNumTotalEntries(&handle->datalist));
        printf("  Fragmentation/Slack: %1.1f%%\n", hogFileCalcFragmentation(handle)*100);
        if (debug_verbose==2) {
            LEAVE_CRITICAL(data_access);
            return;
        }
    }

    for (i=0; i<handle->filelist_count; i++) {
        char date[32];
        const char *filename="NONAME";
        HogFileListEntry *file_entry = &handle->filelist[i];
        HogEAListEntry *ea_entry = NULL;
        U32 crc = 0;
        U32 unpacked_size = 0;
        if (!file_entry->in_use)
            continue;
        if (file_entry->header.headerdata.flagFFFE == 0xFFFE) {
            assert(file_entry->header.headerdata.ea_id != HOG_NO_VALUE);
            ea_entry = &handle->ealist[file_entry->header.headerdata.ea_id];
        } else {
            ea_entry = NULL; // May have a 64-bit header value
        }
        crc = file_entry->header.checksum;
        if (ea_entry) {
            if (ea_entry->header.name_id!=HOG_NO_VALUE) {
                filename = DataListGetString(&handle->datalist, ea_entry->header.name_id);
            }
            unpacked_size = ea_entry->header.unpacked_size;
        }
        if (verbose)
            printf("%06d: ", i);
        if (debug_verbose) {
            printf("0x%010I64X %c ", file_entry->header.offset, (!ea_entry || ea_entry->header.header_data_id==HOG_NO_VALUE)?' ':'H');
        }
        if (!verbose) {
            printf("%s\n", filename);
        } else {
            _ctime32_s(SAFESTR(date), &file_entry->header.timestamp);
            date[strlen(date)-1]=0; // knock off the carriage return
            if (verbose > 1)
                printf("%08x ",crc);
            printf("%10ld", file_entry->header.size);
            if (verbose > 1)
                printf("/%10ld",unpacked_size);
            printf(" %16s %s\n", date, filename);
        }
    }
    LEAVE_CRITICAL(data_access);
}











//////////////////////////////////////////////////////////////////////////
// HogFileModify functions
//////////////////////////////////////////////////////////////////////////
// All of these should generate commands that a background thread commits to disk

static void hogFileModifyCheckForFlush(HogFile *handle)
{
    bool needFlush=false;
    do {
        needFlush=false;
        ENTER_CRITICAL(data_access);
        if (handle->debug_in_data_access != 1) {
            // Recursively in the critical!
            LEAVE_CRITICAL(data_access);
            break;
        }
        if (listLength(handle->mod_ops_head) > 100)
            needFlush = true;
        LEAVE_CRITICAL(data_access);
        //if (needFlush)
        //    hogFileModifyFlush(handle); // Only if single threaded
        if (needFlush)
            Sleep(100);

    } while (needFlush);
}

static void hogFileAddMod(HogFile *handle, HogFileMod *mod)
{
    ENTER_CRITICAL(data_access);
    assert(handle->mutex.reference_count);
    hogFillInFileModBasics(handle, mod);
    mod->next = NULL;
    if (handle->mod_ops_tail) {
        assert(handle->mod_ops_head);
        handle->mod_ops_tail->next = mod;
    } else {
        // No tail or head
        handle->mod_ops_head = mod;
    }
    handle->mod_ops_tail = mod;
    hogThreadHasWork(handle);
    LEAVE_CRITICAL(data_access);
}

static HogFileMod *hogFileGetMod(HogFile *handle)
{
    HogFileMod *ret;
    if (!handle->mod_ops_head)
        return NULL;
    ENTER_CRITICAL(data_access);
    ret = handle->mod_ops_head;
    handle->mod_ops_head = ret->next;
    if (handle->mod_ops_tail == ret) {
        handle->mod_ops_tail = NULL;
        assert(!handle->mod_ops_head);
    }
    LEAVE_CRITICAL(data_access);
    return ret;
}

static int hogFileFlushDataListDiff(HogFile *handle)
{
    HogFileMod *mod;
    U8 *data;
    U32 datasize;
    int ret;

    assert(handle->mutex.reference_count); // Should already be in one of these
    hogAcquireMutex(handle); // Just because we free it when processing the mod

    IN_CRITICAL(data_access);

    memlog_printf(&hogmemlog, "0x%08x: hogFileFlushDataListDiff:Modifying DataList file", handle);
    // Serialize and write new data (recoverably)
    data = DataListWrite(&handle->datalist, &datasize); // Does Endianness internally
    assert(((U32*)data)[1] > 0); // We're writing no data, did it get blown away in memory?
    if (ret = hogFileModifyUpdate(handle, handle->header.datalist_fileno, data, datasize, _time32(NULL), true))
        return NESTED_ERROR(1, ret);

    memlog_printf(&hogmemlog, "0x%08x: Mod:FlushDataListDiff", handle);
    mod = calloc(sizeof(HogFileMod), 1);
    mod->type = HFM_DATALISTFLUSH;
    mod->datalistflush.size_offset = hogFileOffsetOfDLJournal(handle);
    hogFileAddMod(handle, mod);

    // Zero out the DL journal
    handle->datalist_diff_size = 0;
    return 0;
}

static int just_flushed_datalist=0;

// Send modification of DataList change (may need to do some moves/writes/etc first)
static int hogFileAddDataListMod(HogFile *handle, DataListJournal *dlj)
{
    int ret=0;
    HogFileMod *mod;
    assert(dlj->size && dlj->data);

    assert(handle->mutex.reference_count);
    IN_CRITICAL(data_access);

    if (handle->datalist_diff_size + dlj->size + sizeof(DLJournalHeader) >= handle->header.dl_journal_size)
    {
        just_flushed_datalist = 1;
        // Includes this change!
        if (ret=hogFileFlushDataListDiff(handle))
            return NESTED_ERROR(1, ret);
        free(dlj->data);
    } else {
        hogAcquireMutex(handle); // Just because we free it when processing the mod

        just_flushed_datalist = 0;
        memlog_printf(&hogmemlog, "0x%08x: Mod:  DataListDiff size %d", handle, dlj->size);
        mod = calloc(sizeof(HogFileMod), 1);
        mod->type = HFM_DATALISTDIFF;
        mod->datalistdiff.data = dlj->data;
        mod->datalistdiff.newsize = handle->datalist_diff_size + dlj->size;
        assert(mod->datalistdiff.newsize + sizeof(DLJournalHeader) < handle->header.dl_journal_size);
        mod->datalistdiff.offset = hogFileOffsetOfDLJournal(handle) + handle->datalist_diff_size + sizeof(DLJournalHeader);
        mod->datalistdiff.size = dlj->size;
        mod->datalistdiff.size_offset = hogFileOffsetOfDLJournal(handle);
        handle->datalist_diff_size = mod->datalistdiff.newsize;
        // Send packet
        hogFileAddMod(handle, mod);
    }
    return ret;
}

void hogFileFreeSpace(HogFile *handle, U64 offset, U32 size)
{
    memlog_printf(&hogmemlog, "0x%08x: Freeing offs: %I64d size: %d", handle, offset, size);
#ifdef NO_FREELIST
    return;
#else
    if (!size)
        return;
    IN_CRITICAL(data_access);
    hfstFreeSpace(&handle->free_space, offset, size);
    handle->free_space_dirty = true;
#endif
}

int hogFileModifyDelete(HogFile *handle, HogFileIndex file)
{
    int ret;
    HogFileMod *mod;
    HogFileListEntry *file_entry;
    HogEAListEntry *ea_entry = NULL;
    DataListJournal dlj = {0};
    S32 ea_id=HOG_NO_VALUE;
    U64 newsize;
    bool should_truncate=false;
    char *name=NULL;

    if (file == HOG_INVALID_INDEX)
        return -1;

    hogAcquireMutex(handle);

    ENTER_CRITICAL(data_access);

    file_entry = hogGetFileListEntrySafe(handle, file, false);
    if (file_entry == NULL) {
        // File index invalid (deleted in another process perhaps)
        LEAVE_CRITICAL(data_access);
        hogReleaseMutex(handle, false);
        return -1;
    }

    // Make modification packet
    memlog_printf(&hogmemlog, "0x%08x: Mod:Delete %d Offs:%I64d, size:%d", handle, file, file_entry->header.offset, file_entry->header.size);
    mod = calloc(sizeof(HogFileMod), 1);
    mod->type = HFM_DELETE;
    mod->del.file_index = file;
    if (file_entry->header.headerdata.flagFFFE == 0xFFFE) {
        ea_id = (S32)file_entry->header.headerdata.ea_id;
        mod->del.ea_id = ea_id;
        ea_entry = hogGetEAListEntry(handle, ea_id);
    } else {
        mod->del.ea_id = HOG_NO_VALUE;
    }

    hogFileFreeSpace(handle, file_entry->header.offset, file_entry->header.size);

    // Modify in-memory structures and make DataList Journal
    // Can't zero dirty flag/count!
    ZeroStruct(&file_entry->header);
    file_entry->in_use = 0;
    eaiPush(&handle->file_free_list, file+1);
    handle->num_files--;
    if (ea_entry) {
        // Remove Name and Header if they exist
        if (ea_entry->header.name_id != HOG_NO_VALUE) {
            const char *fn = DataListGetString(&handle->datalist, ea_entry->header.name_id);
            if (fn) {
                if (handle->callbacks.exist)
                    name = strdup(fn);
                stashRemoveInt(handle->fn_to_index_lookup, fn, NULL);
                DataListFree(&handle->datalist, ea_entry->header.name_id, &dlj);
            }
        }
        if (ea_entry->header.header_data_id != HOG_NO_VALUE) {
            DataListFree(&handle->datalist, ea_entry->header.header_data_id, &dlj);
        }

        ZeroStruct(ea_entry);
        ea_entry->in_use = 0;
        eaiPush(&handle->ea_free_list, ea_id+1);
    }

    // Check for file truncation (must be *before* sending packet which may release the mutex)
    newsize = hogFileGetRequiredFileSize(handle);
    if (newsize < handle->file_size) {
        should_truncate = true;
        hogAcquireMutex(handle);
    }

    // Send modification packet
    hogDirtyFile(handle, file);
    hogFileAddMod(handle, mod);

    // If datalist was changed, send modification of that (may need to do some moves/writes/etc first)
    if (dlj.size) {
        if (ret=hogFileAddDataListMod(handle, &dlj)) {
            LEAVE_CRITICAL(data_access);
            return NESTED_ERROR(2, ret);
        }
    }

    if (should_truncate) {
        // truncate!
        memlog_printf(&hogmemlog, "0x%08x: Mod:Truncate:%I64d", handle, newsize);
        mod = calloc(sizeof(HogFileMod), 1);
        mod->type = HFM_TRUNCATE;
        mod->truncate.newsize = hogFileGetRequiredFileSize(handle); // Recalculate here because the datalistdiff flush might have grown it!
        hogFileAddMod(handle, mod);
    }

    if (handle->callbacks.exist && name) {
        handle->callbacks.fileDeleted(handle->callbacks.userData, name);
    }
    SAFE_FREE(name);

    LEAVE_CRITICAL(data_access);
    hogFileModifyCheckForFlush(handle);
    return 0;
}


static U64 hogFileModifyFitFile(HogFile *handle, U32 size, U64 data_offset, HogFileIndex file_index)
{
    U64 ret=0;
#ifdef NO_FREELIST
    {
        U64 offset;
        HogFileListEntry **file_list=0;
        U32 i;
        IN_CRITICAL(data_access);
        for (i=0; i<handle->filelist_count; i++) {
            HogFileListEntry *file_entry = &handle->filelist[i];
            if (file_entry->in_use) {
                if (file_entry->header.size) {
                    eaPush(&file_list, file_entry);
                }
            }
        }
        eaQSort(file_list, cmpFileOffsets);
        offset = data_offset;
        for (i=0; i<(U32)eaSize(&file_list) && !ret; i++) {
            U64 freespace = file_list[i]->header.offset - offset;
            U64 newoffset = file_list[i]->header.offset + file_list[i]->header.size;
            if (file_list[i]->header.offset < offset) {
                // This file is before we want to start looking
                if (newoffset > offset)
                    offset = newoffset; // But it expands past the beginning!
            } else {
                if (freespace >= size) {
                    ret = offset;
                }
                offset = newoffset;
            }
        }
        if (!ret)
            ret = offset; // Put it at the end

        eaDestroy(&file_list);
        //memlog_printf(&hogmemlog, "0x%08x:   Allocing %d bytes past %I64d: Return %I64d", handle, size, data_offset, ret);
        return ret;
    }
#else
    {
        IN_CRITICAL(data_access);
        hfstSetStartLocation(&handle->free_space, data_offset);
        ret = hfstGetSpace(&handle->free_space, size, size+hogFileGetSlack(handle, file_index));
        //memlog_printf(&hogmemlog, "0x%08x:   Allocing %d bytes past %I64d: Return %I64d", handle, size, data_offset, ret);
        return ret;
    }
#endif
}

static int hogFileModifyMoveFilePast(HogFile *handle, HogFileIndex file, U64 offset)
{
    HogFileMod *mod;
    HogFileListEntry *file_entry = hogGetFileListEntry(handle, file);
    U64 new_offset;
    assert(file_entry->header.size);

    assert(handle->mutex.reference_count);
    hogAcquireMutex(handle); // Just because we free it when processing the mod

    IN_CRITICAL(data_access);

    // Determine new location
    new_offset = hogFileModifyFitFile(handle, file_entry->header.size, offset, file);

    // Send Mod packet
    memlog_printf(&hogmemlog, "0x%08x: Mod:Move %d From:%I64d, To: %I64d, size:%d", handle, file, file_entry->header.offset, new_offset, file_entry->header.size);
    mod = calloc(sizeof(HogFileMod), 1);
    mod->type = HFM_MOVE;
    mod->move.file_index = file;
    mod->move.old_offset = file_entry->header.offset;
    mod->move.new_offset = new_offset;
    mod->move.size = file_entry->header.size;
    hogDirtyFile(handle, file);
    hogFileAddMod(handle, mod);

    // Modify in-memory structures
    hogFileFreeSpace(handle, file_entry->header.offset, file_entry->header.size);
    file_entry->header.offset = new_offset;

    return 0;
}

static int hogFileModifyGrowFileAndEAList(HogFile *handle)
{
    U32 filelist_old_used_count;
    U32 filelist_new_count;
    U32 ealist_new_count;
    U32 ealist_old_used_count;
    U32 filelist_new_size;
    U32 ealist_new_size;
    U64 ealist_new_offset;
    U64 data_new_offset;
    U32 i;
    int ret;
    HogFileMod *mod=NULL;

    assert(handle->mutex.reference_count);
    hogAcquireMutex(handle); // Just because we free it when processing the mod

    IN_CRITICAL(data_access);
    memlog_printf(&hogmemlog, "0x%08x: GrowFileAndEAList", handle);

    // Find new FileList size
    filelist_new_count = handle->filelist_count;
    filelist_old_used_count = hogFileGetNumUsedFiles(handle);
    if ((handle->filelist_count - filelist_old_used_count) / (F32)filelist_old_used_count < FILELIST_PAD_THRESHOLD )
    {
        filelist_new_count = (U32)(handle->filelist_count * (1 + FILELIST_PAD));
        if (filelist_new_count < handle->filelist_count + 16) {
            // Up it by at least 16 if we're going through the effort!
            filelist_new_count = handle->filelist_count + 16;
        }
        // Must expand FileList by at least the size of the old EAList
        if ((filelist_new_count - handle->filelist_count) * sizeof(HogFileHeader) < handle->header.ea_list_size) {
            // We need the FileList to expand by at least the size of the EAList, otherwise
            //   we can't safely and atomicly resize
            filelist_new_count = handle->filelist_count + 
                (handle->header.ea_list_size + sizeof(HogFileHeader) - 1) / sizeof(HogFileHeader);
        }
    }
    assert((filelist_new_count - handle->filelist_count) * sizeof(HogFileHeader) >= handle->header.ea_list_size ||
        filelist_new_count == handle->filelist_count);

    // Find new EAList size (if it needs to be resized, or is close to needing it)
    ealist_new_count = handle->ealist_count; // Default to not resizing
    ealist_old_used_count = hogFileGetNumUsedEAs(handle);
    if ((handle->ealist_count - ealist_old_used_count) / (F32)ealist_old_used_count < EALIST_PAD_THRESHOLD )
    {
        // EAList could use expanding too!
        ealist_new_count = (U32)(handle->ealist_count * (1 + FILELIST_PAD));
        if (ealist_new_count < handle->ealist_count + 16) {
            // Up it by at least 16 if we're going through the effort!
            ealist_new_count = handle->ealist_count + 16;
        }
    }

    // Calculate new offset of file data
    filelist_new_size = filelist_new_count * sizeof(HogFileHeader);
    ealist_new_size = ealist_new_count * sizeof(HogEAHeader);
    // Doesn't change: filelist_new_offset = hogFileOffsetOfFileList(handle);
    ealist_new_offset = hogFileOffsetOfFileList(handle) + filelist_new_size;
    data_new_offset = ealist_new_offset + ealist_new_size;

    memlog_printf(&hogmemlog, "0x%08x: GrowFileAndEAList: FileList:%d->%d EAList:%d->%d", handle,
        handle->header.file_list_size, filelist_new_size,
        handle->header.ea_list_size, ealist_new_size);

    // Move any files in the way
    for (i=0; i<handle->filelist_count; i++) {
        HogFileListEntry *file_entry = &handle->filelist[i];
        if (file_entry->in_use) {
            if (file_entry->header.size) {
                if (file_entry->header.offset < data_new_offset) {
                    if (ret = hogFileModifyMoveFilePast(handle, i, data_new_offset))
                        return NESTED_ERROR(1, ret);
                }
            }
        }
    }

    mod = calloc(sizeof(HogFileMod), 1);
    mod->type = HFM_FILELIST_RESIZE;
    mod->filelist_resize.old_ealist_pos = hogFileOffsetOfEAList(handle);
    mod->filelist_resize.new_ealist_pos = (U32)ealist_new_offset;
    mod->filelist_resize.new_ealist_size = ealist_new_size;
    mod->filelist_resize.new_filelist_size = filelist_new_size;
    mod->filelist_resize.old_ealist_size = handle->header.ea_list_size;

    // Modify in-memory structures with new data
    // Add empty slots
    hogGrowFileList(handle, filelist_new_count); // Modifies handle->header.file_list_size
    hogGrowEAList(handle, ealist_new_count); // Modifies handle->header.ea_list_size
    mod->filelist_resize.new_ealist_data = hogSerializeEAHeaders(handle); // Freed in thread!

    assert(hogFileOffsetOfEAList(handle) == mod->filelist_resize.new_ealist_pos);

    // Send packet
    hogFileAddMod(handle, mod);

    return 0;
}

static int just_grew_filelist=0;
static int hogFileModifyFitFileList(HogFile *handle)
{
    U32 i;
    IN_CRITICAL(data_access);
    for (i=0; i<handle->filelist_count; i++) {
        if (!handle->filelist[i].in_use) {
            just_grew_filelist = 0;
            assert(eaiSize(&handle->file_free_list)>0);
            return 0; // Has an empty slot
        }
    }
    memlog_printf(&hogmemlog, "0x%08x: FitFileList:Growing", handle);
    assert(eaiSize(&handle->file_free_list)==0);
    just_grew_filelist = 1;
    return hogFileModifyGrowFileAndEAList(handle);
}

static int hogFileModifyFitEAList(HogFile *handle)
{
    U32 i;
    IN_CRITICAL(data_access);
    for (i=0; i<handle->ealist_count; i++) {
        if (!handle->ealist[i].in_use) {
            assert(eaiSize(&handle->ea_free_list)>0);
            return 0; // Has an empty slot
        }
    }
    memlog_printf(&hogmemlog, "0x%08x: FitEAList:Growing", handle);
    assert(eaiSize(&handle->ea_free_list)==0);
    return hogFileModifyGrowFileAndEAList(handle);
}

// Note: takes ownership of data
int hogFileModifyAdd(HogFile *handle, const char *name, U8 *data, U32 size, U32 timestamp, U8 *header_data, U32 header_data_size, U32 unpacksize, U32 *crc)
{
    HogFileMod *mod;
    int file_index;
    int ea_index;
    HogFileListEntry *file_entry;
    HogEAListEntry *ea_entry;
    int ret=0;
    U64 offset=0;
    S32 name_id=HOG_NO_VALUE;
    S32 header_id=HOG_NO_VALUE;
    DataListJournal dlj = {0};
#define FAIL(code) { ret = code; goto fail; }

    memlog_printf(&hogmemlog, "0x%08x: hogFileModifyAdd: %s data:%08x size:%d", handle, name, data, size);

    hogAcquireMutex(handle);

    ENTER_CRITICAL(data_access);

    if (handle->free_space_dirty) {
        // Build this once each time we finish freeing and start allocating to compact the free list
        hogFileBuildFreeSpaceList(handle);
        handle->free_space_dirty = false;
    }

    // Verify FileList/EAList can fit one more, resize if necessary
    if (ret=hogFileModifyFitFileList(handle))
        FAIL(NESTED_ERROR(1, ret));
    if (ret=hogFileModifyFitEAList(handle))
        FAIL(NESTED_ERROR(2, ret));

    // Modify DataList, send mod packet
    name_id = DataListAdd(&handle->datalist, name, (int)strlen(name)+1, true, &dlj);
    name = DataListGetString(&handle->datalist, name_id);
    if (header_data) {
        header_id = DataListAdd(&handle->datalist, header_data, header_data_size, false, &dlj);
    }
    if (ret=hogFileAddDataListMod(handle, &dlj))
        FAIL(NESTED_ERROR(3, ret));

    // Modify in-memory structures to point to new file
    file_index = hogFileCreateFileEntry(handle);
    file_entry = hogGetFileListEntry(handle, file_index);
    file_entry->header.size = size;
    file_entry->header.timestamp = timestamp;
    stashAddInt(handle->fn_to_index_lookup, name, file_index, true); // Shallow copied name from DataList
    ea_index = hogFileCreateEAEntry(handle, file_index);
    ea_entry = hogGetEAListEntry(handle, ea_index);
    ea_entry->header.unpacked_size = unpacksize;
    ea_entry->header.name_id = name_id;
    ea_entry->header.header_data_id = header_id;
    if (!crc) {
        U32 autocrc[4];
        assert(!unpacksize);
        cryptMD5Update(data,size);
        cryptMD5Final(autocrc);
        file_entry->header.checksum = autocrc[0];
    } else {
        file_entry->header.checksum = crc[0];
    }

    // Find empty space (need file index/name to be set first)
    if (size) {
        offset = hogFileModifyFitFile(handle, size, hogFileOffsetOfData(handle), file_index);
        if (offset==0)
            FAIL(1);
    }
    file_entry->header.offset = offset;
    if (offset + size > handle->file_size)
        handle->file_size = offset + size;


    // Send mod packet (must be after datalist mod in order to not leave nameless files)
    memlog_printf(&hogmemlog, "0x%08x: Mod:Add file %d Offs:%I64d, size:%d", handle,
        file_index, offset, size);
    mod = calloc(sizeof(HogFileMod), 1);
    mod->type = HFM_ADD;
    tfcRecordMemoryUsed(size);
    mod->add.file_index = file_index;
    mod->add.size = size;
    mod->add.timestamp = timestamp;
    mod->add.checksum = file_entry->header.checksum;
    mod->add.headerdata.flagFFFE = 0xFFFE;
    mod->add.headerdata.ea_id = ea_index;
    mod->add.ea_data.unpacksize = unpacksize;
    mod->add.ea_data.name_id = name_id;
    mod->add.ea_data.header_data_id = header_id;
    mod->add.offset = offset;
    mod->add.data = data; // Need to free this later!
    hogDirtyFile(handle, file_index);
    hogFileAddMod(handle, mod);

    if (handle->callbacks.exist) {
        handle->callbacks.fileNew(handle->callbacks.userData, name, size, timestamp);
    }

fail:
    LEAVE_CRITICAL(data_access);
    hogFileModifyCheckForFlush(handle);
    return ret;
#undef FAIL
}

// Note: takes ownership of data
int hogFileModifyAdd2(HogFile *handle, NewPigEntry *entry)
{
    U8 *header_data=NULL;
    U32 header_data_size=0;
    int ret;
    if (hogFileFind(handle, entry->fname)!=HOG_INVALID_INDEX)
        return -1;
    if (!hog_mode_no_data)
        pigChecksumAndPackEntry(entry);
    ENTER_CRITICAL(data_access);
    if (!hog_mode_no_data)
        header_data = pigGetHeaderData(entry, &header_data_size);
    ret = hogFileModifyAdd(handle, entry->fname, entry->data, entry->pack_size?entry->pack_size:entry->size, entry->timestamp, header_data, header_data_size, entry->pack_size?entry->size:0, entry->checksum);
    LEAVE_CRITICAL(data_access);
    hogFileModifyCheckForFlush(handle);
    return ret;
}


// Note: takes ownership of data
// Note: preserves index
int hogFileModifyUpdate2(HogFile *handle, HogFileIndex file, NewPigEntry *entry, bool safe_update)
{
    int ret=0;
    assert(!safe_update);
    // Remove old one, add new one
    if (ret=hogFileModifyDelete(handle, file))
        goto fail;
    if (ret=hogFileModifyAdd2(handle, entry))
        goto fail;
fail:
    return ret;
}

// Note: takes ownership of data
// Note: preserves index
// safe_update uses more temporary space, but makes sure that the file cannot be lost if closed mid-operation
//      (should probably only be used internally for the ?DataList file)
int hogFileModifyUpdate(HogFile *handle, HogFileIndex file, U8 *data, U32 size, U32 timestamp, bool safe_update)
{
    int ret=0;

    ENTER_CRITICAL(data_access);
    if (handle->free_space_dirty) {
        // Build this once each time we finish freeing and start allocating to compact the free list
        hogFileBuildFreeSpaceList(handle);
        handle->free_space_dirty = false;
    }
    LEAVE_CRITICAL(data_access);

    if (!safe_update) {
        NewPigEntry entry={0};
        // Prepare data
        entry.data = data;
        entry.fname = strdup(hogFileGetFileName(handle, file));
        entry.size = size;
        entry.timestamp = timestamp;
        ret=hogFileModifyUpdate2(handle, file, &entry, safe_update);
        SAFE_FREE(entry.fname);
        return ret;
    } else {
        U32 autocrc[4];
        U64 offset;
        HogFileMod *mod;
        HogFileListEntry *file_entry;
        HogEAListEntry *ea_entry = NULL;
    
        hogAcquireMutex(handle);

        ENTER_CRITICAL(data_access);

        file_entry = hogGetFileListEntry(handle, file);
        // Modify in-memory checksum
        cryptMD5Update(data,size);
        cryptMD5Final(autocrc);
        file_entry->header.checksum = autocrc[0];
        if (file_entry->header.headerdata.flagFFFE == 0xFFFE) {
            ea_entry = hogGetEAListEntry(handle, (S32)file_entry->header.headerdata.ea_id);
        } else {
        }
        assert(!ea_entry || ea_entry->header.header_data_id==HOG_NO_VALUE); // Can't safely update with header data!

        // Find location for file
        offset = hogFileModifyFitFile(handle, size, hogFileOffsetOfData(handle), file);

        // Free up previous space
        hogFileFreeSpace(handle, file_entry->header.offset, file_entry->header.size);

        memlog_printf(&hogmemlog, "0x%08x: Mod:Update file %d, Offs:%I64d->%I64d, size:%d->%d", handle,
            file, file_entry->header.offset, offset,
            file_entry->header.size, size);

        // Modify in-memory header
        file_entry->header.offset = offset;
        file_entry->header.size = size;
        file_entry->header.timestamp = timestamp;

        // Send modifcation packet
        mod = calloc(sizeof(HogFileMod), 1);
        mod->type = HFM_UPDATE;
        mod->update.file_index = file;
        mod->update.size = size;
        mod->update.timestamp = timestamp;
        mod->update.headerdata = file_entry->header.headerdata;
        mod->update.checksum = file_entry->header.checksum;
        mod->update.offset = offset;
        mod->update.data = data; // Need to free this later!
        hogDirtyFile(handle, file);
        hogFileAddMod(handle, mod);

        if (!hogFileIsSpecialFile(handle, file) && handle->callbacks.exist) {
            assert(0); // This should only be called on ?DataList
            //handle->callbacks.fileUpdated(handle->callbacks.userData, ..., size, timestamp);
        }

        LEAVE_CRITICAL(data_access);
        hogFileModifyCheckForFlush(handle);
        return 0;
    }
}

// Note: takes ownership of data
// Pass in NULL data for a delete
int hogFileModifyUpdateNamed(HogFile *handle, const char *relpath, U8 *data, U32 size, U32 timestamp)
{
    int ret;
    int index = hogFileFind(handle, relpath);
    if (index == HOG_INVALID_INDEX) {
        // New!
        if (data) {
            NewPigEntry entry = {0};
            entry.data = data;
            entry.fname = (char*)relpath;
            entry.size = size;
            entry.timestamp = timestamp;
            ret = hogFileModifyAdd2(handle, &entry);
        } else {
            // Deleting something already deleted?!
            // Can happen on funny things like v_pwrbox, a file that is deleted, but a folder named that exists
            ret = 0;
        }
    } else {
        if (data) {
            // Update
            memlog_printf(&hogmemlog, "0x%08x: hogFileModifyUpdateNamed:hogFileModifyUpdate: %s data:%08x size:%d  index = ", handle, relpath, data, size, index);
            ret = hogFileModifyUpdate(handle, index, data, size, timestamp, false);
        } else {
            // Delete
            memlog_printf(&hogmemlog, "0x%08x: hogFileModifyUpdateNamed:hogFileModifyDelete: %s data:%08x size:%d  index = ", handle, relpath, data, size, index);
            ret = hogFileModifyDelete(handle, index);
        }
    }
    return ret;
}

int hogFileModifyUpdateTimestamp(HogFile *handle, HogFileIndex file, U32 timestamp)
{
    HogFileMod *mod;
    HogFileListEntry *file_entry;

    hogAcquireMutex(handle);

    ENTER_CRITICAL(data_access);

    file_entry = hogGetFileListEntry(handle, file);
    memlog_printf(&hogmemlog, "0x%08x: Mod:UpdateTimestamp file %d, timestamp:%d->%d", handle,
        file, file_entry->header.timestamp, timestamp);

    // Modify in-memory offset
    file_entry->header.timestamp = timestamp;

    // Send modifcation packet
    mod = calloc(sizeof(HogFileMod), 1);
    mod->type = HFM_UPDATE;
    mod->update.file_index = file;
    mod->update.size = file_entry->header.size;
    mod->update.timestamp = file_entry->header.timestamp;
    mod->update.headerdata = file_entry->header.headerdata;
    mod->update.checksum = file_entry->header.checksum;
    mod->update.offset = file_entry->header.offset;
    mod->update.data = NULL;
    hogFileAddMod(handle, mod);

    // No callbacks here, since this is only used for fixing up slightly bad dates

    LEAVE_CRITICAL(data_access);
    hogFileModifyCheckForFlush(handle);
    return 0;
}



static bool hog_thread_inited=false;
static HANDLE hog_thread_handle=NULL;
static unsigned hog_thread_id;
static VOID CALLBACK hogThreadingHasWorkFunc( ULONG_PTR dwParam);

static unsigned __stdcall hogThreadingThread( LPVOID lpParam )
{
    EXCEPTION_HANDLER_BEGIN
        PERFINFO_AUTO_START("hogThreadingThread", 1);
            for(;;)
            {
                SleepEx(INFINITE, TRUE);
            }
        PERFINFO_AUTO_STOP();
        return 0; 
    EXCEPTION_HANDLER_END
}

void hogThreadingInit(void)
{
    // Creates a thread to handle hog modifications
    if (hog_thread_inited)
        return;
    hog_thread_inited = true;

    hog_thread_handle = (HANDLE)_beginthreadex( 
        NULL,                        // no security attributes 
        0,                           // use default stack size  
        hogThreadingThread,             // thread function 
        NULL,                         // argument to thread function 
        0,                           // use default creation flags 
        &hog_thread_id);             // returns the thread identifier 
    assert(hog_thread_handle != NULL);
}

static void hogThreadHasWork(HogFile *handle)
{
    InterlockedIncrement(&handle->async_operation_count);
    QueueUserAPC(hogThreadingHasWorkFunc, hog_thread_handle, (ULONG_PTR)handle );
}

int hogFileModifyFlush(HogFile *handle)
{
    int ret=0;
    ENTER_CRITICAL(doing_flush); // Multiple people may lock of both waiting on the same event
    do {
        ENTER_CRITICAL(data_access);
        assert(handle->debug_in_data_access==1); // Can't be in the critical when entering this function
        if (!handle->mod_ops_tail && // Nothing queued
            !handle->async_operation_count) // Not in the middle of anything
        {
            // Nothing left to do!
            ret = handle->last_threaded_error;
            handle->last_threaded_error = 0;
            LEAVE_CRITICAL(data_access);
            break;
        }
        LEAVE_CRITICAL(data_access);
        // Wait on event
        WaitForSingleObject(handle->done_doing_operation_event, INFINITE);
    } while (true);
    LEAVE_CRITICAL(doing_flush);
    return ret;
}


//////////////////////////////////////////////////////////////////////////
// In-thread: Functions to actually apply the modifications queued up from above
//    Must handle Endianness while writing to disk
//////////////////////////////////////////////////////////////////////////

static int hogFileModifyDoDeleteInternal(HogFile *handle, HogFileMod *mod)
{
    U64 offs;
    HogFileHeader file_header = {0};
    int ret;

    // zero FileList entry
    file_header.size = HOG_NO_VALUE;
    offs = mod->filelist_offset + sizeof(HogFileHeader)*mod->del.file_index;
    endianSwapStructIfBig(parseHogFileHeader, &file_header);
    if (ret=checkedWriteData(handle, &file_header, sizeof(file_header), offs)) {
        return NESTED_ERROR(1, ret);
    }

    if (mod->del.ea_id != HOG_NO_VALUE) {
        // zero EAList entry
        HogEAHeader ea_header = {0};
        ea_header.flags = HOGEA_NOT_IN_USE;
        offs = mod->ealist_offset + sizeof(HogEAHeader)*mod->del.ea_id;
        endianSwapStructIfBig(parseHogEAHeader, &ea_header);
        if (ret=checkedWriteData(handle, &ea_header, sizeof(ea_header), offs)) {
            return NESTED_ERROR(2, ret);
        }
    }
    return 0;
}

static int hogFileModifyDoDelete(HogFile *handle, HogFileMod *mod)
{
    HFJDelete journal_entry = {0};
    int ret;
    // Journal Delete operation - recovery must delete file and modify the ?DataList
    journal_entry.del = mod->del;
    endianSwapStructIfBig(parseHFMDelete, &journal_entry.del);
    journal_entry.type = endianSwapIfBig(U32, HOJ_DELETE);
    if (ret=hogJournalJournal(handle, &journal_entry, sizeof(journal_entry))) {
        return NESTED_ERROR(1, ret);
    }

    if (ret=hogFileModifyDoDeleteInternal(handle, mod)) {
        return NESTED_ERROR(2, ret);
    }

    // clear ops journal
    if (ret=hogJournalReset(handle)) {
        return NESTED_ERROR(3, ret);
    }

    hogUnDirtyFile(handle, mod->del.file_index);

    return 0;
}


static int hogFileModifyDoAddInternal(HogFile *handle, HogFileMod *mod)
{
    U64 offs;
    HogFileHeader file_header = {0};
    int ret;

    // Fill in FileList entry
    file_header.offset = mod->add.offset;
    file_header.size = mod->add.size;
    file_header.timestamp = mod->add.timestamp;
    file_header.headerdata = mod->add.headerdata;
    file_header.checksum = mod->add.checksum;
    // Write it
    offs = mod->filelist_offset + sizeof(HogFileHeader)*mod->add.file_index;
    endianSwapStructIfBig(parseHogFileHeader, &file_header);
    if (ret=checkedWriteData(handle, &file_header, sizeof(file_header), offs)) {
        return NESTED_ERROR(1, ret);
    }

    if (mod->add.headerdata.flagFFFE == 0xFFFE && mod->add.headerdata.ea_id != HOG_NO_VALUE) {
        // Fill in EAList entry
        HogEAHeader ea_header = {0};
        ea_header.unpacked_size = mod->add.ea_data.unpacksize;
        ea_header.name_id = mod->add.ea_data.name_id;
        ea_header.header_data_id = mod->add.ea_data.header_data_id;
        offs = mod->ealist_offset + sizeof(HogEAHeader)*mod->add.headerdata.ea_id;
        endianSwapStructIfBig(parseHogEAHeader, &ea_header);
        if (ret=checkedWriteData(handle, &ea_header, sizeof(ea_header), offs)) {
            return NESTED_ERROR(2, ret);
        }
    }
    return 0;
}

static int hogFileModifyDoAdd(HogFile *handle, HogFileMod *mod)
{
    HFJAdd journal_entry = {0};
    int ret;

    // Write data
    if (!hog_mode_no_data) {
        if (mod->add.size) {
            enterFileWriteCriticalSection();
            assert(mod->add.offset);
            if (ret=checkedWriteData(handle, mod->add.data, mod->add.size, mod->add.offset)) {
                leaveFileWriteCriticalSection();
                return NESTED_ERROR(1, ret);
            }
            fflush(handle->file); // Need to make sure data is written before updating the header to ponit to it!
            leaveFileWriteCriticalSection();
        }
    }

    // Journal Add operation - recovery can fill in file_list, [ea_list] (since data is there and good)
    journal_entry.type = endianSwapIfBig(U32,HOJ_ADD);
    journal_entry.add = mod->add;
    journal_entry.add.data = NULL;
    endianSwapStructIfBig(parseHFMAdd, &journal_entry.add);
    if (ret=hogJournalJournal(handle, &journal_entry, sizeof(journal_entry))) {
        return NESTED_ERROR(2, ret);
    }

    // Edit file_list, [ea_list]
    if (ret=hogFileModifyDoAddInternal(handle, mod)) {
        return NESTED_ERROR(3, ret);
    }

    // Clear journal
    if (ret=hogJournalReset(handle)) {
        return NESTED_ERROR(4, ret);
    }
    hogUnDirtyFile(handle, mod->add.file_index);

    return 0;
}

static int hogFileModifyDoUpdateInternal(HogFile *handle, HogFileMod *mod)
{
    // Edit file_list
    U64 offs;
    HogFileHeader file_header = {0};
    int ret;

    // Update FileList entry
    file_header.offset = mod->update.offset;
    file_header.size = mod->update.size;
    file_header.timestamp = mod->update.timestamp;
    file_header.headerdata = mod->update.headerdata; // Should not have changed
    file_header.checksum = mod->update.checksum;
    // Write it
    offs = mod->filelist_offset + sizeof(HogFileHeader)*mod->update.file_index;
    endianSwapStructIfBig(parseHogFileHeader, &file_header);
    if (ret=checkedWriteData(handle, &file_header, sizeof(file_header), offs)) {
        return NESTED_ERROR(1, ret);
    }
    return 0;
}

static int hogFileModifyDoUpdate(HogFile *handle, HogFileMod *mod)
{
    int ret;
    HFJUpdate journal_entry = {0};

    // Write data
    if (mod->update.data) {
        enterFileWriteCriticalSection();
        assert(((U32*)mod->update.data)[1] != 0);
        if (ret=checkedWriteData(handle, mod->update.data, mod->update.size, mod->update.offset)) {
            leaveFileWriteCriticalSection();
            return NESTED_ERROR(1, ret);
        }
        fflush(handle->file); // Need to make sure data is written before updating the header to ponit to it!
        leaveFileWriteCriticalSection();
    }

    // Journal Update operation - recovery can fill in file_list, [ea_list] (since data is there and good)
    journal_entry.type = endianSwapIfBig(U32,HOJ_UPDATE);
    journal_entry.update = mod->update;
    journal_entry.update.data = NULL;
    endianSwapStructIfBig(parseHFMUpdate, &journal_entry.update);
    if (ret=hogJournalJournal(handle, &journal_entry, sizeof(journal_entry))) {
        return NESTED_ERROR(2, ret);
    }

    // Edit file_list
    if (ret=hogFileModifyDoUpdateInternal(handle, mod)) {
        return NESTED_ERROR(3, ret);
    }

    // Clear journal
    if (ret=hogJournalReset(handle)) {
        return NESTED_ERROR(4, ret);
    }

    if (mod->update.data)
        hogUnDirtyFile(handle, mod->update.file_index);

    return 0;
}

static int hogFileModifyDoMoveInternal(HogFile *handle, HogFileMod *mod)
{
    int ret;
    U64 offset = mod->filelist_offset + sizeof(HogFileHeader)*mod->move.file_index + offsetof(HogFileHeader, offset);
    U64 data_to_write = endianSwapIfBig(U64,mod->move.new_offset);
    STATIC_ASSERT(sizeof(data_to_write) == SIZEOF2(HogFileHeader, offset));
    if (ret=checkedWriteData(handle, &data_to_write, sizeof(data_to_write), offset))
        return NESTED_ERROR(1, ret);
    return 0;
}

static int hogFileModifyDoMove(HogFile *handle, HogFileMod *mod)
{
    HFJMove journal_entry = {0};
    U8 *data=NULL;
    int ret=0;
#define FAIL(code) { ret = NESTED_ERROR(code, ret); goto cleanup; }

    if (!hog_mode_no_data) {
        // read data
        data = malloc(mod->move.size);
        if (ret=checkedReadData(handle, data, mod->move.size, mod->move.old_offset))
            FAIL(1);

        // write data
        if (ret=checkedWriteData(handle, data, mod->move.size, mod->move.new_offset))
            FAIL(2);
        fflush(handle->file); // Need to make sure data is written before updating the header to ponit to it!
    }

    // journal header change
    journal_entry.type = endianSwapIfBig(U32,HOJ_MOVE);
    journal_entry.move = mod->move;
    endianSwapStructIfBig(parseHFMMove, &journal_entry.move);
    if (ret=hogJournalJournal(handle, &journal_entry, sizeof(journal_entry)))
        FAIL(3);

    // change header
    if (ret=hogFileModifyDoMoveInternal(handle, mod))
        FAIL(4)

    // end journal
    if (ret=hogJournalReset(handle))
        FAIL(5);

    hogUnDirtyFile(handle, mod->move.file_index);

cleanup:
    SAFE_FREE(data);
    return ret;
#undef FAIL
}


static int hogFileModifyDoTruncate(HogFile *handle, HogFileMod *mod)
{
    return fileTruncate(handle->file, mod->truncate.newsize);
}

static int hogFileModifyDoFileListResizeInternal(HogFile *handle, HogFileMod *mod)
{
    HogFileHeader *file_headers=NULL;
    int num_file_headers;
    int ret=0;
    int i;
    HogHeader header = {0};
#define SAVE_FIELD(fieldname) if (ret=checkedWriteData(handle, &header.fieldname, sizeof(header.fieldname), offsetof(HogHeader, fieldname))) { ret = NESTED_ERROR(2, ret); goto fail; }

    // Grow FileList
    // zero new FileList data (no need to re-write existing FileList data)
    if (!(mod->filelist_resize.new_ealist_pos == mod->filelist_offset + mod->filelist_resize.new_filelist_size)) {
        return hogShowError(handle->filename, 1, "Inconsistent operation data ", 0);
    }

    num_file_headers = (mod->filelist_resize.new_ealist_pos - mod->filelist_resize.old_ealist_pos) / sizeof(HogFileHeader);
    if (num_file_headers) {
        file_headers = calloc(sizeof(HogFileHeader), num_file_headers);
        for (i=0; i<num_file_headers; i++) 
            file_headers[i].size = endianSwapIfBig(U32,HOG_NO_VALUE); // Flag as not in use
        if (ret=checkedWriteData(handle, file_headers, num_file_headers*sizeof(HogFileHeader), mod->filelist_resize.old_ealist_pos)) {
            ret = NESTED_ERROR(1, ret);
            goto fail;
        }
        fflush(handle->file); // Need to make sure data is written before updating the header to ponit to it!
    }

    // modify header with new data (ealist_size, filelist_size)
    header.file_list_size = endianSwapIfBig(U32,mod->filelist_resize.new_filelist_size);
    header.ea_list_size = endianSwapIfBig(U32,mod->filelist_resize.new_ealist_size);
    SAVE_FIELD(file_list_size);
    SAVE_FIELD(ea_list_size);

fail:
    SAFE_FREE(file_headers);
    return ret;
#undef SAVE_FIELD
}

static int hogFileModifyDoFileListResize(HogFile *handle, HogFileMod *mod)
{
    HFJFileListResize journal_entry = {0};
    int ret;

    // write new EAList data
    if (mod->filelist_resize.old_ealist_pos != mod->filelist_resize.new_ealist_pos)
    {
        // Write it all!
        // Data endian swapped in Serialize functions
        if (ret=checkedWriteData(handle, mod->filelist_resize.new_ealist_data, mod->filelist_resize.new_ealist_size, mod->filelist_resize.new_ealist_pos))
            return NESTED_ERROR(1, ret);
        fflush(handle->file); // Need to make sure data is written before updating the header to ponit to it!
    } else {
        // Didn't move: Write only new data
        U32 offs = mod->filelist_resize.old_ealist_size;
        // Data endian swapped in Serialize functions
        if (ret=checkedWriteData(handle,
                ((U8*)mod->filelist_resize.new_ealist_data) + offs,
                mod->filelist_resize.new_ealist_size - offs,
                mod->filelist_resize.new_ealist_pos + offs))
            return NESTED_ERROR(1, ret);
        fflush(handle->file); // Need to make sure data is written before updating the header to ponit to it!
    }

    journal_entry.type = endianSwapIfBig(U32,HOJ_FILELIST_RESIZE);
    journal_entry.filelist_resize = mod->filelist_resize;
    endianSwapStructIfBig(parseHFMFileListResize, &journal_entry.filelist_resize);
    if (ret=hogJournalJournal(handle, &journal_entry, sizeof(journal_entry)))
        return NESTED_ERROR(2, ret);

    if (ret=hogFileModifyDoFileListResizeInternal(handle, mod))
        return NESTED_ERROR(3, ret);

    if (ret=hogJournalReset(handle))
        return NESTED_ERROR(4, ret);

    return 0;
}

static int hogFileModifyDoDataListFlushInternal(HogFile *handle, HogFileMod *mod)
{
    DLJournalHeader header;
    int ret;

    // Safely update the size
#define SAVE_FIELD(fieldname) if (ret=checkedWriteData(handle, &header.fieldname, sizeof(header.fieldname), mod->datalistflush.size_offset + offsetof(DLJournalHeader, fieldname))) return NESTED_ERROR(2, ret);
    header.inuse_flag = endianSwapIfBig(U32,1);
    SAVE_FIELD(inuse_flag);
    header.size = endianSwapIfBig(U32,0);
    SAVE_FIELD(size);
    header.inuse_flag = endianSwapIfBig(U32,0);
    SAVE_FIELD(inuse_flag);
    header.oldsize = endianSwapIfBig(U32,0);
    SAVE_FIELD(oldsize);
#undef SAVE_FIELD
    return 0;
}

int hogFileModifyDoDataListFlush(HogFile *handle, HogFileMod *mod)
{
    HFJDataListFlush journal_entry = {0};
    int ret;

    journal_entry.type = endianSwapIfBig(U32,HOJ_DATALISTFLUSH);
    journal_entry.datalistflush = mod->datalistflush;
    endianSwapStructIfBig(parseHFMDataListFlush, &journal_entry.datalistflush);
    if (ret=hogJournalJournal(handle, &journal_entry, sizeof(journal_entry)))
        return NESTED_ERROR(2, ret);

    if (ret=hogFileModifyDoDataListFlushInternal(handle, mod))
        return NESTED_ERROR(3, ret);

    if (ret=hogJournalReset(handle))
        return NESTED_ERROR(4, ret);

    return 0;
}


static VOID CALLBACK hogThreadingHasWorkFunc( ULONG_PTR dwParam)
{
    static HogFileMod last_mod;
    HogFile *handle = (HogFile*)dwParam;
    HogFileMod *mod;
    int ret=0;
    static DWORD threadid=0;
    if (!threadid) {
        threadid = GetCurrentThreadId();
    }
    assert(threadid == GetCurrentThreadId()); // This must be ran from only one thread!

    mod = hogFileGetMod(handle);
    assert(mod);
    last_mod = *mod;
    if (handle->version.value > 2)
    {
        // Sleep for up to half a second if this is the first
        //  item in the queue, and nothing else has been queued up,
        //  and another process has shown evidence of accessing our
        //  hogg file, otherwise the hog file ownership will bounce
        //  back and forth causing lots of loads/reloads.
        // Break out of this if it appears we're doing a flush.
        int left = 10;
        while (left && !handle->mod_ops_head && !handle->debug_in_doing_flush) {
            Sleep(50);
            left--;
        }
    }
    switch(mod->type) {
    xcase HFM_DELETE:
        if (ret = hogFileModifyDoDelete(handle, mod))
            ret = NESTED_ERROR(1, ret);
    xcase HFM_ADD:
        if (ret = hogFileModifyDoAdd(handle, mod))
            ret = NESTED_ERROR(2, ret);
        if (!hog_mode_no_data)
            SAFE_FREE(mod->add.data);
        tfcRecordMemoryFreed(mod->add.size);
    xcase HFM_UPDATE:
        if (ret = hogFileModifyDoUpdate(handle, mod))
            ret = NESTED_ERROR(3, ret);
        SAFE_FREE(mod->update.data);
    xcase HFM_MOVE:
        if (ret = hogFileModifyDoMove(handle, mod))
            ret = NESTED_ERROR(4, ret);
    xcase HFM_DATALISTDIFF:
        if (ret = hogDLJournalSave(handle, mod))
            ret = NESTED_ERROR(5, ret);
        SAFE_FREE(mod->datalistdiff.data);
    xcase HFM_TRUNCATE:
        if (ret = hogFileModifyDoTruncate(handle, mod))
            ret = NESTED_ERROR(6, ret);
    xcase HFM_FILELIST_RESIZE:
        if (ret = hogFileModifyDoFileListResize(handle, mod))
            ret = NESTED_ERROR(7, ret);
        SAFE_FREE(mod->filelist_resize.new_ealist_data);
    xcase HFM_DATALISTFLUSH:
        if (ret = hogFileModifyDoDataListFlush(handle, mod))
            ret = NESTED_ERROR(8, ret);
    }
    if (ret) {
        hogShowError(handle->filename, 1, "Error modifying .hogg file Err#", ret);
    }
    SAFE_FREE(mod);
    if (ret) {
        ENTER_CRITICAL(data_access);
        handle->last_threaded_error = ret;
        LEAVE_CRITICAL(data_access);
    }
    InterlockedDecrement(&handle->async_operation_count);
    hogReleaseMutex(handle, true); // Must be the final action
    return;
}

//////////////////////////////////////////////////////////////////////////
// End in-thread: Functions to actually apply the modifications queued up
//////////////////////////////////////////////////////////////////////////

