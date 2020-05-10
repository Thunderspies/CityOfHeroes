/***************************************************************************
*     Copyright (c) 2003-2006, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
***************************************************************************/
#include "utilitieslib/utils/piglib.h"
#include "utilitieslib/utils/piglib_internal.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/assert/assert.h"
#define WIN32_LEAN_AND_MEAN
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/components/SharedMemory.h"
#include "utilitieslib/utils/error.h"
#include "zlib/zlib.h"
#include "utilitieslib/network/crypt.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/hoglib.h"
#include "utilitieslib/utils/endian.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/components/earray.h"

int pig_debug=0;
int pig_disabled=0;
char g_pigErrorBuffer[2048] = "";
PigErrLevel    pig_err_level = PIGERR_ASSERT;
CRITICAL_SECTION PigCritSec;
PiggFileFilterCallback g_piggFileFilterCB = NULL;

typedef enum
{
    PIG_ERRCODE_CORRUPTPIG,
    PIG_ERRCODE_1,
    PIG_ERRCODE_2,
    PIG_ERRCODE_3,
    PIG_ERRCODE_4,
    PIG_ERRCODE_5,
    PIG_ERRCODE_6,
    PIG_ERRCODE_7,
    PIG_ERRCODE_8,
    PIG_ERRCODE_9,
    PIG_ERRCODE_10,
    PIG_ERRCODE_11,
    PIG_ERRCODE_12,
    PIG_ERRCODE_13,
    PIG_ERRCODE_FAILED_ALLOC,
} PigErrCode;

void pigDisablePiggs(int disabled)
{
    pig_disabled = disabled ? 1 : 0;
}

void PigCritSecInit(void)
{
    static bool inited=false;
    if (!inited) {
        inited = true;
        InitializeCriticalSection(&PigCritSec);
    }
}

static int pigShowError(const char *fname,PigErrCode err_code,const char *err_msg,int err_int_value)
{
    sprintf_s(SAFESTR(g_pigErrorBuffer),"%s: %s%d",fname,err_msg,err_int_value);
    if (pig_err_level != PIGERR_QUIET)
        printf("%s\n",g_pigErrorBuffer);
    if (pig_err_level == PIGERR_ASSERT) {
        const char* additionalInfo;
        if(err_code == PIG_ERRCODE_FAILED_ALLOC) {
            additionalInfo = "The game ran out of memory and must now exit.";
        } else {
            additionalInfo = "Your data might be corrupt, please run the patcher and try again.";
        }
        FatalErrorf("%s\n\n%s", g_pigErrorBuffer, additionalInfo);
    }
    return err_code;
}

PigFile *PigFileAlloc(void)
{
    PigFile *ret = calloc(sizeof(PigFile), 1);
    PigFileCreate(ret);
    return ret;
}

void PigFileCreate(PigFile *handle) {
    ZeroStruct(handle);
    DataPoolCreate(&handle->string_pool);
    handle->string_pool.header_flag = PIG_STRING_POOL_FLAG;
    DataPoolCreate(&handle->header_data_pool);
    handle->header_data_pool.header_flag = PIG_HEADER_DATA_POOL_FLAG;
    handle->header.creator_version = PIG_VERSION;
    handle->header.required_read_version = PIG_VERSION;
    handle->header.archive_header_size = sizeof(PigHeader);
    handle->header.file_header_size = sizeof(PigFileHeader);
    handle->header.pig_header_flag = PIG_HEADER_FLAG;
}

void PigFileDestroy(PigFile *handle) {
    SAFE_FREE(handle->file_headers);
    SAFE_FREE(handle->filename);
    if (handle->file) {
        fclose(handle->file);
        handle->file=NULL;
    }
    DataPoolDestroy(&handle->string_pool);
    DataPoolDestroy(&handle->header_data_pool);
    if (handle->hog_file) {
        hogFileDestroy(handle->hog_file);
        SAFE_FREE(handle->hog_file);
    }
}

int PigFileRead(PigFile *handle, const char *filename)
{
    U32 numfiles;
    U32 i;
    size_t size,real_size;
    char fn[MAX_PATH];
    struct stat status;
    char dataPoolName[MAX_PATH*2];
    int err_code=0;
    int err_int_value=-1;
    int in_critical_section=0;
    long pos;
    #define FAIL(num,str) { err_code = pigShowError(filename,num,str,err_int_value); goto fail; }

    assert(!handle->file && !handle->hog_file && "Cannot reuse a handle without calling PigFileDestroy() first!");

    if (strEndsWith(filename, ".hogg")) {
        handle->hog_file = hogFileAlloc();
        return hogFileRead(handle->hog_file, filename);
    }

    strcpy(fn, filename);
    if (!fileExists(fn)) {
        sprintf_s(SAFESTR(fn), "%s.pig", filename);
    }
    if (!fileExists(fn)) {
        sprintf_s(SAFESTR(fn), "%s.pigg", filename);
    }
#ifdef _XBOX
    backSlashes(fn);
#endif
    SAFE_FREE(handle->filename);
    handle->filename = strdup(fn);
    // Open file
    handle->file = fopen(fn, "rb~");
    if (!handle->file) {
        FAIL(PIG_ERRCODE_1,"Error opening pig file");
    }
    if(!stat(fn, &status)){
        if(!(status.st_mode & _S_IFREG)) {
            FAIL(PIG_ERRCODE_1,"Error statting pig file");
        }
    } else {
        FAIL(PIG_ERRCODE_1,"Error statting pig file");
    }

    EnterCriticalSection(&PigCritSec);
    in_critical_section=1;

    // Read header
    ZeroStruct(&handle->header);
    fread(&handle->header, 1, 16, handle->file);
    if (*(U32*)&handle->header == HOG_HEADER_FLAG) {
        // Really a hog file!
        fclose(handle->file);
        handle->file = NULL;
        ZeroStruct(&handle->header);
        LeaveCriticalSection(&PigCritSec);
        handle->hog_file = hogFileAlloc();
        return hogFileRead(handle->hog_file, filename);
    }
    STATIC_INFUNC_ASSERT(sizeof(handle->header)==16); // Changed size of PigHeader but didn't update reading code to read the extra values
    if (handle->header.pig_header_flag!=PIG_HEADER_FLAG) {
        FAIL(PIG_ERRCODE_2,"Bad flag on file - not a Pig file");
        return 2;
    }
    if (handle->header.required_read_version > PIG_VERSION) {
        err_int_value = handle->header.required_read_version;
        FAIL(PIG_ERRCODE_3,"Pig file too new of a version ");
    }
    // read slack
    if (handle->header.archive_header_size!=sizeof(PigHeader)) {
        fseek(handle->file, handle->header.archive_header_size - sizeof(PigHeader), SEEK_CUR);
    }

    numfiles = handle->header.num_files;

    // Read FileHeaders
    assert(!handle->file_headers);
    handle->file_headers = malloc(sizeof(PigFileHeader)*numfiles);

    STATIC_INFUNC_ASSERT(sizeof(PigFileHeader)==48);

    size=0;
    if (handle->header.file_header_size==sizeof(PigFileHeader)) {
        size+=fread(handle->file_headers, 1, numfiles*sizeof(PigFileHeader), handle->file);
    } else {
        int size_diff = handle->header.file_header_size-sizeof(PigFileHeader);
        // There's slack between each entry
        for (i=0; i<numfiles; i++) {
            size+=fread(&handle->file_headers[i], 1, sizeof(PigFileHeader), handle->file);
            fseek(handle->file, size_diff, SEEK_CUR);
            if (size_diff < 0)
                memset(((char*)&handle->file_headers[i]) + handle->header.file_header_size, 0, -size_diff);
        }
    }
    if (size!=sizeof(PigFileHeader)*numfiles) {
        FAIL(PIG_ERRCODE_9,"Couldn't read all of the required data from a Pig file");
    }

    // Verify FileHeader integrity
    for (i=0; i<numfiles; i++) {
        if (handle->file_headers[i].file_header_flag != PIG_FILE_HEADER_FLAG) {
            err_int_value = i;
            FAIL(PIG_ERRCODE_4,"Bad flag on fileheader #");
        }
        real_size = handle->file_headers[i].pack_size ? handle->file_headers[i].pack_size : handle->file_headers[i].size;
        if ((_off_t)(handle->file_headers[i].offset + real_size - 1) > status.st_size) {
            err_int_value = i;
            FAIL(PIG_ERRCODE_10,"Fileheader extends past end of .pigg, probably corrupt .pigg file!  #");
        }
        if (handle->file_headers[i].name_id < 0)
        {
            err_int_value = i;
            FAIL(PIG_ERRCODE_10,"Bad name id");
        }
    }

    pos = (long)ftell(handle->file);
    // Read StringPool and DataPool
    STR_COMBINE_SS(dataPoolName, "PIG_SP_", fn);
    if (-1==DataPoolLoadNamedShared(handle->file, &handle->string_pool, dataPoolName)) {
        FAIL(PIG_ERRCODE_5,"string Datapool error");
    }
    STR_COMBINE_SS(dataPoolName, "PIG_DP_", fn);
    if (-1==DataPoolLoadNamedShared(handle->file, &handle->header_data_pool, dataPoolName)) {
        FAIL(PIG_ERRCODE_6,"header Datapool error");
    }
    // Verify number of files in pig vs. number of files in datapool (if they're not the same,
    //  then we probably grabbed a shared memory from an old pig file)
    if ((U32)handle->string_pool.num_elements != handle->header.num_files) {
        sharedMemorySetMode(SMM_DISABLED);
        fseek(handle->file, pos, SEEK_SET);
        if (-1==DataPoolRead(handle->file, &handle->string_pool)) {
            FAIL(PIG_ERRCODE_5,"string Datapool error");
        }
        if (-1==DataPoolRead(handle->file, &handle->header_data_pool)) {
            FAIL(PIG_ERRCODE_6,"header Datapool error");
        }
    }
    if (handle->string_pool.header_flag!=PIG_STRING_POOL_FLAG) {
        FAIL(PIG_ERRCODE_7,"Bad flag on String Pool");
    }
    if (handle->header_data_pool.header_flag!=PIG_HEADER_DATA_POOL_FLAG) {
        FAIL(PIG_ERRCODE_8,"Bad flag on Header Data Pool");
    }
    // Version 1 pig files:
    //if (handle->header.creator_version==PIG_VERSION && handle->header.num_files!=0)
    //    assert(handle->file_headers[0].offset==(U32)ftell(handle->file));
fail:
    if (in_critical_section)
        LeaveCriticalSection(&PigCritSec);
    if (err_code)
        PigFileDestroy(handle);
    return err_code;
}


U32 PigFileOffsetOfData(PigFile *handle) {
    assert(!handle->hog_file);
    return sizeof(PigHeader) +
        sizeof(PigFileHeader)*handle->header.num_files +
        DataPoolGetDiskUsage(&handle->string_pool) +
        DataPoolGetDiskUsage(&handle->header_data_pool);
}

void PigFileDumpInfo(PigFile *handle, int verbose, int debug_verbose) {
    U32 i;

    if (handle->hog_file) {
        hogFileDumpInfo(handle->hog_file, verbose, debug_verbose);
        return;
    }

    if (debug_verbose) { // header info
        printf("PigFile Info:\n");
        printf("  PigFile version: %d\n", handle->header.creator_version);
        printf("  Number of Files: %ld\n", handle->header.num_files);
        printf("  Number of Strings: %ld\n", handle->string_pool.num_elements);
        printf("  Number of Header Data Blocks: %ld\n", handle->header_data_pool.num_elements);
        if (debug_verbose==2)
            return;
    }

    for (i=0; i<handle->header.num_files; i++) {
        PigFileHeader *head = &handle->file_headers[i];
        const char *filename = DataPoolGetString(&handle->string_pool, head->name_id);

        if (debug_verbose) {
            printf("0x%08X %c ", head->offset, (head->header_data_id==-1)?' ':'H');
        }
        if (!verbose) {
            printf("%s\n", filename);
        } else {
            char date[26];
            _ctime32_s(SAFESTR(date),(__time32_t *)&head->timestamp);
            date[strlen(date)-1]=0; // knock off the carriage return
            if (verbose > 1)
            {
                printf("%08x%08x%08x%08x ",head->checksum[0],head->checksum[1],head->checksum[2],head->checksum[3]);
                printf("%10ld/",head->pack_size);
            }
            printf("%10ld %16s %s\n", head->size, date, filename);
        }
    }
}

PigFileHeader *PigFileFind(PigFile *handle, const char *relpath) {
    char fn[MAX_PATH];
    S32 id;

    strcpy(fn, relpath);
    forwardSlashes(fn);
    fixDoubleSlashes(fn);

    if (handle->hog_file) {
        return U32_TO_PTR(hogFileFind(handle->hog_file, relpath)+1);
    }
    id = DataPoolGetIDFromString(&handle->string_pool, fn);
    if (id!=-1) {
        assert(handle->file_headers[id].name_id==id);
        return &handle->file_headers[id];
    }
    return NULL;
}

S32 PigFileFindIndexQuick(PigFile *handle, const char *relpath)
{
    S32 id;

    if (handle->hog_file) {
        return hogFileFind(handle->hog_file, relpath);
    }
    id = DataPoolGetIDFromString(&handle->string_pool, relpath);
    return id;
}

#if 0
void dumpMemoryToFile(void *mem, unsigned int count)
{
    char fn[MAX_PATH];
    static int callcount=0;
    int handle;
#ifdef _XBOX
    sprintf(fn, "devkit:\\memory_%d.txt", callcount);
#else
    sprintf(fn, "C:\\memory_%d.txt", callcount);
#endif
    callcount++;

    handle = open(fn, _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC);
    if (handle != -1) {
        int written = _write(handle, mem, count);
        if (written == -1) {
            OutputDebugString("Error ");
            switch(errno)
            {
            xcase EBADF:
                OutputDebugString("Bad file descriptor!");
            xcase ENOSPC:
                OutputDebugString("No space left on device!");
            xcase EINVAL:
                OutputDebugString("Invalid parameter: buffer was NULL!");
            }
        } else {
            OutputDebugString("Wrote to ");
            OutputDebugString(fn);
            OutputDebugString(".\n");
        }

        close(handle);
    } else {
        OutputDebugString("Could not open ");
        OutputDebugString(fn);
        OutputDebugString(" for writing\n");
    }
}
#endif

static z_stream*            zStream;
static CRITICAL_SECTION        zCS;

static void* pigZAlloc(void* opaque, U32 items, U32 size)
{
    return calloc(items * size, 1);
}

static void pigZFree(void* opaque, void* address)
{
    free(address);
}

static void pigInitZStream()
{
    PERFINFO_AUTO_START("pigInitZStream", 1);

    if(!zStream)
    {
        InitializeCriticalSection(&zCS);

        zStream    = calloc(1, sizeof(*zStream));
        
        zStream->zalloc    = pigZAlloc;
        zStream->zfree    = pigZFree;

        EnterCriticalSection(&zCS);        
    }
    else
    {
        EnterCriticalSection(&zCS);

        inflateEnd(zStream);
    }
    
    inflateInit(zStream);

    PERFINFO_AUTO_STOP();
}

static int pigInflate(char* outbuf, U32* outsize, void* inbuf, U32 *insize)
{
    int ret;

    zStream->next_in        = inbuf;
    zStream->avail_in        = *insize;

    zStream->next_out        = outbuf;
    zStream->avail_out        = *outsize;

    PERFINFO_AUTO_START("inflate", 1);

    ret = inflate(zStream, Z_SYNC_FLUSH);

    PERFINFO_AUTO_STOP();

    *insize -= zStream->avail_in;
    *outsize -= zStream->avail_out;

    return ret;
}

static bool pigUncompress(char* outbuf, U32* outsize, void* inbuf, U32 insize)
{
    int ret;

    PERFINFO_AUTO_START("pigUncompress", 1);

    pigInitZStream();
    ret = pigInflate(outbuf, outsize, inbuf, &insize);
    LeaveCriticalSection(&zCS);

    PERFINFO_AUTO_STOP();

    return (ret == Z_STREAM_END) && (insize == 0);
}

static bool sFailedMemAlloc = false; // for crash report
static int  sFailedMemAllocSize1 = 0; // for crash report
static int  sFailedMemAllocSize2 = 0; // for crash report

U32 PigExtractBytesInternal(FILE *file, void *headerkey, int headerindex, void *buf, U32 pos, U32 size, U64 fileoffset, U32 filesize, U32 pack_size, bool random_access)
{
    U32 numread, zipped=pack_size > 0;
    static U8 *unzip_buf;
    static U32 unzip_size;
    static void *last_pfh;
    static int last_index;
    U8 unzip_buf_buffer[100*1000];

    if (size==0 || filesize==0) {
        return 0;
    }

    EnterCriticalSection(&PigCritSec);
    if (0!=fseek(file, fileoffset + (zipped ? 0 : pos), SEEK_SET)) {
        LeaveCriticalSection(&PigCritSec);
        return 0;
    }
    if (zipped)
    {
        U32    t_unpack_size = filesize;

        if (unzip_buf && (unzip_size != filesize || last_pfh != headerkey || last_index != headerindex))
        {
            SAFE_FREE(unzip_buf);
        }
        // handle uncached random access reads (really inefficient for anything but specific access patterns)
        if (!unzip_buf && random_access)
        {
            U8 zip_buf[4096];
            U32 t_pack_size = 0;

            pigInitZStream();

            numread = 0;
    
            // seek to position
            while (pos > zStream->total_out)  {
                U8 unzip_buf2[4096*4];

                // partial decompress means we have to reset the seek position
                if (t_pack_size != numread) {
                    if (0!=fseek(file, fileoffset + zStream->total_in, SEEK_SET)) {
                        LeaveCriticalSection(&zCS);
                        LeaveCriticalSection(&PigCritSec);
                        return 0;
                    }
                }

                // Read 4KB at a time
                numread = fread(zip_buf, 1, MIN(ARRAY_SIZE(zip_buf), pack_size - zStream->total_in), file);
                if (numread <= 0) {
                    LeaveCriticalSection(&zCS);
                    LeaveCriticalSection(&PigCritSec);
                    return 0;
                }

                t_unpack_size = MIN(ARRAY_SIZE(unzip_buf2), pos - zStream->total_out);
                t_pack_size = numread;

                if (pigInflate(unzip_buf2, &t_unpack_size, zip_buf, &t_pack_size) & ~(Z_OK|Z_STREAM_END)) {
                    LeaveCriticalSection(&zCS);
                    LeaveCriticalSection(&PigCritSec);
                    return 0;
                }
            }

            // read data
            while (zStream->total_out < size) {
                // partial decompress means we have to reset the seek position
                if (t_pack_size != numread) {
                    if (0!=fseek(file, fileoffset + zStream->total_in, SEEK_SET)) {
                        LeaveCriticalSection(&zCS);
                        LeaveCriticalSection(&PigCritSec);
                        return 0;
                    }
                }

                // Read 4KB at a time
                numread = fread(zip_buf, 1, MIN(ARRAY_SIZE(zip_buf), pack_size - zStream->total_in), file);
                if (numread <= 0) {
                    LeaveCriticalSection(&zCS);
                    LeaveCriticalSection(&PigCritSec);
                    return 0;
                }

                t_unpack_size = pos + size - zStream->total_out;
                t_pack_size = numread;
            
                if (pigInflate((U8*)buf + zStream->total_out - pos, &t_unpack_size, zip_buf, &t_pack_size) & ~(Z_OK|Z_STREAM_END)) {
                    LeaveCriticalSection(&zCS);
                    LeaveCriticalSection(&PigCritSec);
                    return 0;
                }
            }
            LeaveCriticalSection(&zCS);
            LeaveCriticalSection(&PigCritSec);
            return size;
        }
        else if (!unzip_buf)
        {
            U8 zip_buf_buffer[100*1000];
            U8 *zip_buf;
            // for testing bool bFailMallocTest = (getCappedRandom(500) > 480);

            if(pack_size > sizeof(zip_buf_buffer))
            {
                zip_buf = malloc(pack_size);
            }
            else
            {
                zip_buf = zip_buf_buffer;
            }

            if (filesize == size && pos == 0)
            {
                // We want the whole thing, no need for a temporary buffer
                unzip_buf = buf;
            } else if(filesize > sizeof(unzip_buf_buffer))
            {
                // JE: This code shouldn't get hit anymore (maybe in the Updater?)
                unzip_buf = malloc(filesize);
            }
            else
            {
                unzip_buf = unzip_buf_buffer;
            }

            // check for failed memory alloc -- app will exit so dont bother with cleanup
            if(!unzip_buf || !zip_buf /*|| bFailMallocTest*/) {
                sFailedMemAlloc = true;
                sFailedMemAllocSize1 = pack_size;
                sFailedMemAllocSize2 = filesize;
                LeaveCriticalSection(&PigCritSec);
                return 0;
            }            

            unzip_size = filesize;
            last_pfh = headerkey;
            last_index = headerindex;
            numread = fread(zip_buf, 1, pack_size, file);
            if (numread!=pack_size)
            {
                if(zip_buf != zip_buf_buffer)
                {
                    free(zip_buf);
                }
                if(unzip_buf == unzip_buf_buffer)
                    unzip_buf = NULL;
                LeaveCriticalSection(&PigCritSec);
                return 0;
            }
            pigUncompress(unzip_buf,&t_unpack_size,zip_buf,pack_size);
            if(zip_buf != zip_buf_buffer)
            {
                free(zip_buf);
            }
            if (t_unpack_size != filesize) {
                if(unzip_buf == unzip_buf_buffer || unzip_buf == buf)
                    unzip_buf = NULL;
                LeaveCriticalSection(&PigCritSec);
                return 0;
            }
        }
        //assert(size == pfh->unpack_size);
        if (unzip_buf != buf)
            memcpy(buf,unzip_buf+pos,size);
        if(unzip_buf == unzip_buf_buffer || unzip_buf == buf)
            unzip_buf = NULL;
        LeaveCriticalSection(&PigCritSec);
        numread = size;
    }
    else
    {
        numread = fread(buf, 1, size, file);
        LeaveCriticalSection(&PigCritSec);
        if (numread!=size)
            return 0;
    }
    return numread;
}

// Actually read from file
static U32 PigExtractBytes(PigFile *handle,PigFileHeader *pfh,void *buf, U32 pos, U32 size, bool random_access) {
    assert(!handle->hog_file);

    return PigExtractBytesInternal(handle->file, pfh, 0, buf, pos, size, pfh->offset, pfh->size, pfh->pack_size, random_access);
}

U32 PigSetExtractBytes(PigFileDescriptor *desc, void *buf, U32 pos, U32 size, bool random_access) // Extracts size bytes into a pre-allocated buffer
{
    U32 numread;
    PigFile *handle = desc->parent;
    U32 data_size;

    data_size = PigFileGetFileSize(handle, desc->file_index);

    assert(!(pos >= data_size));

    if (pos + size > data_size) {
        size = data_size - pos;
    }

//JE: removed because fileCompare does this, which may be called from any "old version control" function, such as in GetTex, also new animReadTrackFile does this
//    if (pos < desc->header_data_size && pos + size > desc->header_data_size) {
//        assert(!"Warning!  Reading data from a file with a cached header, but reading over the border!");
//    }
    if (pos + size <= desc->header_data_size) {
        U32 header_data_size;
        const U8 *header_data;
        // We can read from the pre-cached header!
        header_data = PigFileGetHeaderData(handle, desc->file_index, &header_data_size);
        assert(header_data);
        assert(header_data_size == desc->header_data_size);
        memcpy(buf, ((U8*)header_data) + pos, size);
        return size;
    }

    if (handle->hog_file) {
        numread=hogFileExtractBytes(handle->hog_file, desc->file_index, buf, pos, size);
    } else {
        PigFileHeader *pfh;
        pfh = &handle->file_headers[desc->file_index];
        numread=PigExtractBytes(desc->parent,pfh,buf,pos,size,random_access);
    }
    if (numread != size)
    {
        char    err_buf[1000];
        PigErrCode errCode;
        if(sFailedMemAlloc) {
            errCode = PIG_ERRCODE_FAILED_ALLOC;
            sprintf_s(SAFESTR(err_buf),"Failed memory allocation (zip'd size %d, unzip'd size %d)\nRead failed for file: %s\n",
                    sFailedMemAllocSize1, sFailedMemAllocSize2, desc->debug_name);
        } else {
            errCode = PIG_ERRCODE_CORRUPTPIG;
            sprintf_s(SAFESTR(err_buf),"Corrupt pig file detected\nRead failed for file: %s\n",desc->debug_name);
        }
        pigShowError(handle->filename,errCode,err_buf,-1);
        return 0;
    }
    return numread;
}

void *PigFileExtract(PigFile *handle, const char *relpath, U32 *count) {
    PigFileHeader *pfh;
    char *data;
    U32 numread;
    U32 total;

    if (handle->hog_file)
        return hogFileExtract(handle->hog_file, hogFileFind(handle->hog_file, relpath), count);

    if (!handle || !handle->file) return NULL;

    pfh = PigFileFind(handle, relpath);
    if (!pfh) return NULL;
    total = pfh->size;
    data = malloc(total + 1);
    if (!data) return NULL;

    numread = PigExtractBytes(handle,pfh,data, 0, total, false);
    if (numread != total)
    {
        free(data);
        pigShowError(handle->filename,PIG_ERRCODE_13,"Read wrong number of bytes",numread);
        *count = 0;
        return 0;
    }
    *count = numread;
    return data;
}

void *PigFileExtractCompressed(PigFile *handle, const char *relpath, U32 *count) {
    PigFileHeader *pfh;
    char *data;
    U32 numread;
    U32 total;

    assert(!handle->hog_file);

    if (!handle || !handle->file) return NULL;

    pfh = PigFileFind(handle, relpath);
    if (!pfh || !pfh->pack_size) return NULL;
    total = pfh->pack_size;
    data = malloc(total + 1);
    if (!data) return NULL;

    EnterCriticalSection(&PigCritSec);
    if (0!=fseek(handle->file, pfh->offset, SEEK_SET)) {
        LeaveCriticalSection(&PigCritSec);
        return 0;
    }
    numread = fread(data, 1, pfh->pack_size, handle->file);
    LeaveCriticalSection(&PigCritSec);

    if (numread != total)
    {
        pigShowError(handle->filename,PIG_ERRCODE_13,"Read wrong number of bytes",numread);
        *count = 0;
        return 0;
    }
    *count = numread;
    return data;
}

bool PigFileValidate(PigFilePtr handle)
{
    U32 i;

    for (i=0; i<handle->header.num_files; i++)
    {
        PigFileHeader *pfh;
        const char * filename;
        void * data;
        U32 numread;
        U32    checksum[4];
        
        filename = DataPoolGetString(&handle->string_pool, i);
        pfh = handle->file_headers + i;
        data = malloc(pfh->size);
        numread = PigExtractBytes(handle, pfh, data, 0, pfh->size, false);
        if (numread != pfh->size)
        {
            free(data);
            printf("File read error for %s in %s\n", filename, handle->filename);
            return 0;
        }

        if (numread) {
            cryptMD5Init();
            cryptMD5Update(data,numread);
            cryptMD5Final(checksum);
        } else {
            ZeroStruct(&checksum);
        }

        free(data);

        if (pfh->checksum[0] != checksum[0] ||
            pfh->checksum[1] != checksum[1] || 
            pfh->checksum[2] != checksum[2] || 
            pfh->checksum[3] != checksum[3])
        {
            printf("Checksum mismatch for %s in %s\n", filename, handle->filename);
            return 0;
        }
    }

    return 1;
}

bool PigFileSetCallbacks(PigFilePtr handle, void *userData, 
                         HogCallbackDeleted fileDeleted, HogCallbackNew fileNew, HogCallbackUpdated fileUpdated)
{
    if (handle->hog_file) {
        hogFileSetCallbacks(handle->hog_file, userData, fileDeleted, fileNew, fileUpdated);
        return true;
    } else {
        return false;
    }
}

void PigFileLock(PigFilePtr handle) // Acquires the multi-process mutex if this is a hogg file
{
    if (handle->hog_file) {
        hogFileLock(handle->hog_file);
    }
}

void PigFileUnlock(PigFilePtr handle)
{
    if (handle->hog_file) {
        hogFileUnlock(handle->hog_file);
    }
}

void PigFileCheckForModifications(PigFilePtr handle) // If a hogg file, checks for modifications by other processes
{
    if (handle->hog_file) {
        hogFileCheckForModifications(handle->hog_file);
    }
}


static PigFile** pig_set = NULL;
static bool pig_set_inited = false;
static char **pig_patch_files = NULL;
static int num_pig_patch_files = 0;

bool PigSetInited(void)
{
    return pig_set_inited;
}

// Helper function for returning list of pigg file names that are not already loaded
static int searchForUnloadedPigs(const char* folder, char*** rpigNames, bool fallback)
{
    char filespec[MAX_PATH];
    struct _finddata32_t c_file;
    intptr_t hFile;
    char fn[MAX_PATH];
    int numPigFiles, numNewPigFiles;
    char** pigNames;
    static bool bFirstTime = true;

    STR_COMBINE_SS(filespec, folder, "/*.*gg");
    hFile = _findfirst32( filespec, &c_file );
    if( fallback && (hFile == -1L ) ) {
        strcpy(filespec, "./piggs/*.*gg");
        folder = "./piggs";
        if( (hFile = _findfirst32( filespec, &c_file )) == -1L ) {
            return 0;
        }
    } else if (hFile == -1L) {
        return 0;
    }

    // cycle thru findfile results to get the total pigg file count
    numPigFiles = 0;
    do {
        if (!strEndsWith(c_file.name, ".pigg") && !strEndsWith(c_file.name, ".hogg"))
            continue;

        if (g_piggFileFilterCB && !g_piggFileFilterCB(c_file.name))
            continue;
        numPigFiles++;
    } while (_findnext32( hFile, &c_file ) == 0 );
    _findclose( hFile);
    bFirstTime = false;

    // cycle thru again to find the ones that are not yet loaded
    pigNames = (char**)malloc(sizeof(char**) * numPigFiles);
    numNewPigFiles = 0;
    hFile = _findfirst32( filespec, &c_file );
    do {
        if (!strEndsWith(c_file.name, ".pigg") && !strEndsWith(c_file.name, ".hogg"))
            continue;
        if (g_piggFileFilterCB && !g_piggFileFilterCB(c_file.name))
            continue;
        //if( eaSize(&pig_set) <= 0 && (strStartsWith(c_file.name, "stage2") || strStartsWith(c_file.name, "stage3")) )
        //    continue; // fpe for testing

        STR_COMBINE_SSS(fn, folder, "/", c_file.name);
        if(!PigSetGetPigFileByName(fn) ) {
            pigNames[numNewPigFiles++] = strdup(fn);
        }        
    } while (_findnext32( hFile, &c_file ) == 0 );

    if(!numNewPigFiles) {
        SAFE_FREE(pigNames);
    }
    *rpigNames = pigNames;

    return numNewPigFiles;
}

int PigSetInit(void) { // Loads all of the Pig files from a given directory
    int i, np;
    char** pigNames;

    if(pig_disabled) {
        return 0;
    }

    if (pig_set_inited) {
        return 0; // Already inited
    }
    pig_set_inited = true; // Set this so that if we get a recursive call to PigSetInit (and we will, because we call fopen()), then we will just return immediately

    PigCritSecInit();

    eaDestroy(&pig_set); // in case previous set existed (not possible?)
    
    // Get the list of pigg file names which need to be loaded
    np = searchForUnloadedPigs(filePiggDir(), &pigNames, true);
    if(!np) {
        return 0;
    }

    // Create and load each pigg file
    eaCreateWithCapacity(&pig_set, np + num_pig_patch_files);
    for(i=0; i<np; i++) {
        PigFile* pf = calloc(sizeof(PigFile), 1);
        PigFileCreate(pf);
        if (pig_debug)
            printf("Reading pigg: %s\n", pigNames[i]);
        if (0 != PigFileRead(pf, pigNames[i])) {
            free(pf);
            return pigShowError(pigNames[i],PIG_ERRCODE_1,"Error trying to read Pig",-1);
        }
        eaPush(&pig_set, pf);
        free(pigNames[i]);
    }
    free(pigNames);

    for(i=0; i<num_pig_patch_files; i++) {
        PigFile* pf = calloc(sizeof(PigFile), 1);
        PigFileCreate(pf);
        if (pig_debug)
            printf("Reading patch pigg: %s\n", pig_patch_files[i]);
        if (0 != PigFileRead(pf, pig_patch_files[i])) {
            free(pf);
            return pigShowError(pig_patch_files[i],PIG_ERRCODE_1,"Error trying to read Pig",-1);
        }
        eaPush(&pig_set, pf);
    }

    if (pig_debug)
        printf("Read %d pigs\n", np);

    return 0;
}

int PigSetAddPatchDir(const char *path) { // Loads all of the Pig files from a given directory
    int np;
    char** pigNames;
    char patchDir[MAX_PATH + 20] = {0};

    if(pig_disabled) {
        return 0;
    }

    // Due to design issues, you have to add patch piggs *before* anything else is initialized
    assert(!pig_set_inited);
    assert(path);

    if (!fileIsAbsolutePath(path))
        sprintf(patchDir, "./%s", path);
    else
        strcpy(patchDir, path);

    PigCritSecInit();

    // Get the list of pigg file names which need to be loaded
    np = searchForUnloadedPigs(patchDir, &pigNames, false);

    pig_patch_files = realloc(pig_patch_files, sizeof(char*) * (num_pig_patch_files + np));
    memcpy(&pig_patch_files[num_pig_patch_files], pigNames, sizeof(char*) * np);
    num_pig_patch_files += np;
    free(pigNames);

    fileAddSearchPath(patchDir);

    return 0;
}

// Scan for piggs that are not already in our pigg file list (for files that are downloaded DURING gameplay)
int PigSetCheckForNew(void)
{
    int i, oldNumPiggs, newPiggNameCount;
    char** newPigNames;

    if(pig_disabled) {
        return 0;
    }

    assert(PigSetInited()); // should only call after inited
    oldNumPiggs = eaSize(&pig_set);

    // Get the list of pigg file names which need to be loaded
    newPiggNameCount = searchForUnloadedPigs(filePiggDir(), &newPigNames, true);
    if(!newPiggNameCount) 
        return 0;

    // Load each new pigg file
    EnterCriticalSection(&PigCritSec);
    for(i=0; i<newPiggNameCount; i++) {
        PigFile* pf = calloc(sizeof(PigFile), 1);
        PigFileCreate(pf);
        if (0 != PigFileRead(pf, newPigNames[i])) {
            free(pf);
            LeaveCriticalSection(&PigCritSec);
            return pigShowError(newPigNames[i],PIG_ERRCODE_1,"Error trying to read Pig file",-1);
        }
        eaPush(&pig_set, pf);
        free(newPigNames[i]);
    }
    free(newPigNames);
    LeaveCriticalSection(&PigCritSec);

    if (pig_debug)
        printf("Added %d new pigs\n", newPiggNameCount);

    return newPiggNameCount;
}

#define PUT_PIGFILE(pig_num, file_num) (((((pig_num)+1) & 0xff) << 24) | (file_num & 0xffffff))
#define GET_PIGNUM(index) (((index) >> 24)-1)
#define GET_FILENUM(index) ((index) & 0xffffff)

PigFileDescriptor PigSetGetFileInfo(int pig_index, int file_index, const char *debug_relpath)
{
    PigFileDescriptor ret;
    bool is_relative=true;
    char debug_fn[MAX_PATH];
    PigFile *handle;

    PigSetInit();

    assert(pig_index != -1);
    assert(file_index != -1);

    handle = pig_set[pig_index];

    // Make a relative path if it's absolute and part of gameDataDir
    if (debug_relpath[1]==':') {
        static char *gdd = NULL;
        static int len = -1;
        char tempfn[MAX_PATH];
        assert(0);

        if (!gdd || gdd[0]==0) {
            gdd = fileDataDir();
            len = (int)strlen(gdd);
        }

        strcpy(tempfn, debug_relpath);
        forwardSlashes(tempfn);

        if (strnicmp(tempfn, gdd, len)==0) {
            debug_relpath = tempfn + len;
            while (*debug_relpath=='/') debug_relpath++;
        } else {
            // Absolute path and not part of gameDataDirs...
            is_relative = false;
        }
    }

    assert(is_relative);

    strcpy(debug_fn, debug_relpath);
    forwardSlashes(debug_fn);
    fixDoubleSlashes(debug_fn);
    debug_relpath = debug_fn;

    assert( pig_index >= 0 && pig_index < eaSize(&pig_set) );
    assert( file_index >= 0 && file_index < (int)PigFileGetNumFiles(handle));
    ret.debug_name = PigFileGetFileName(handle, file_index);
    if (!ret.debug_name || PigFileIsSpecialFile(handle, file_index))
    {
        memset(&ret, 0, sizeof(ret));
        return ret;
    }
    //ret.header_data = 
    PigFileGetHeaderData(handle, file_index, &ret.header_data_size);
    ret.parent = handle;
    ret.file_index = file_index;
    ret.size = PigFileGetFileSize(handle, file_index);
    return ret;
}


void *extractFromFS(const char *name, U32 *count) {
    U32 total;
    char *mem;
    FILE * file;

    file = fopen(name,"rb~"); // Does NOT allow reading from pigs
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    total = (long)ftell(file);
    assert(total != -1);
    mem = malloc(total+1);
    fseek(file, 0, SEEK_SET);
    fread(mem,total,1,file);
    fclose(file);
    mem[total] = 0;
    if (count)
        *count = total;
    return mem;
}

void PigSetDestroy(void) {
    int i;
    for (i=0; i<eaSize(&pig_set); i++) {
        PigFileDestroy(pig_set[i]);
        free(pig_set[i]);
    }

    eaDestroy(&pig_set);
    pig_set_inited = false;

    DeleteCriticalSection(&PigCritSec);
}

int PigSetGetNumPigs(void) {
    PigSetInit();
    return eaSize(&pig_set);
}

PigFile *PigSetGetPigFile(int index) {
    PigSetInit();
    assert(index<eaSize(&pig_set));
    return pig_set[index];
}

// Search for a pig file with the given name, eg: "./piggs/bin.pigg"
PigFilePtr PigSetGetPigFileByName(const char* filename)
{
    int i, numPiggs;

    numPiggs = eaSize(&pig_set);
    if(!numPiggs)
        return NULL; // not yet inited, no piggs loaded

    // linear search since we expect to have few piggs and this function only called
    //    a few times at startup
    for(i=0; i<numPiggs; i++)
    {
        PigFilePtr pf = pig_set[i];
        if( !stricmp(pf->filename, filename) )
            return pf;
    }

    return NULL;
}

int pigCmpEntryPathname(const NewPigEntry *a,const NewPigEntry *b)
{
    char    *sa,*sb;
    int        t;

    // Normally I would just use stricmp, but it needs to match the way ntfs sorts, since that's the way the
    // pig files were originally made
    for(sa=a->fname,sb=b->fname;*sa && *sb;sa++,sb++)
    {
        t = toupper(*sa) - toupper(*sb);
        if (t)
            return t;
    }
    return *sa - *sb;
}

bool doNotCompressMySize(NewPigEntry * entry)
{
    return (entry->size == 0 || ((double)entry->pack_size / (double)entry->size) > 0.9);
}

bool pigShouldBeUncompressed(const char *ext)
{
    static const char *no_compress_extensions[] = {".fsb", ".fev", ".hogg", ".hog", ".ogg"};
    int i;

    if (!ext)
        return false;
    for (i=0; i<ARRAY_SIZE(no_compress_extensions); i++) 
        if (stricmp(ext, no_compress_extensions[i])==0)
            return true;
    return false;
}

bool doNotCompressMyExt(NewPigEntry * entry)
{
    char * fname = entry->fname;
    char *ext = strrchr(fname, '.');
    return pigShouldBeUncompressed(ext);
}

void pigChecksumAndPackEntry(NewPigEntry *entry)
{
    U8        *unpack_data=0;

    if (!entry->pack_size)
    {
        unpack_data = entry->data;
        if (entry->size) {
            cryptMD5Init();
            cryptMD5Update(unpack_data,entry->size);
            cryptMD5Final(entry->checksum);
        } else {
            ZeroStruct(&entry->checksum);
        }
        if (entry->dont_pack)
            return;
        entry->data = zipData(unpack_data,entry->size,&entry->pack_size);
    }
    if (!entry->must_pack && (((double)entry->pack_size / (double)entry->size) > 0.9))
    {
        if (!unpack_data)
        {
            unpack_data = malloc(entry->size);
            pigUncompress(unpack_data,&entry->size,entry->data,entry->pack_size);
        }
        free(entry->data);
        entry->data = unpack_data;
        entry->pack_size = 0;
    }
    else
        SAFE_FREE(unpack_data);
}

bool pigShouldCacheHeaderData(const char *ext)
{
    static const char *scan_header_extensions[] = {".anm",".geo",".geolm",".lgeo",".bgeo",".texture",".wtex",".anim",".lm2",".dcache"};
    int i;
    if (!ext)
        return false;
    for (i=0; i<ARRAY_SIZE(scan_header_extensions); i++) 
        if (stricmp(ext, scan_header_extensions[i])==0)
            return true;
    return false;
}

U8 *pigGetHeaderData(NewPigEntry *entry, U32 *size)
{
    static U8 *header_mem=0;
    static int header_size=0;
    char scan_header_extensions[] = ".anm .geo .geolm .lgeo .bgeo .texture .anim .lm2";
    char bigendian_extensions[] = ".bgeo";
    char *ext = strrchr(entry->fname, '.');

    if (ext && strstri(scan_header_extensions, ext) && entry->size >= 4 && *(U32*)entry->data > 0)
    {
        S32 numbytes,num_numbytes = sizeof(numbytes);
        bool zipped = entry->pack_size > 0;

        if (strEndsWith(entry->fname, ".lm2")) {
            numbytes = 29; // fixed header size
        } else {
            if (zipped)
                pigUncompress((U8*)&numbytes,&num_numbytes,entry->data,entry->pack_size);
            else
                numbytes = *(U32*)entry->data;
            if (strstri(bigendian_extensions, ext)) {
                numbytes = endianSwapU32(numbytes);
            }
        }
        if (numbytes < 0 || numbytes > 1024*1024*2) {
            FatalErrorf("Header size too big on file %s!  Most likely corrupt data.", entry->fname);
        }
        if (stricmp(ext, ".anm")==0 || stricmp(ext, ".geo")==0 || stricmp(ext, ".geolm")==0 || stricmp(ext, ".bgeo")==0 || stricmp(ext, ".lgeo")==0)
            numbytes+=4; // Cache 4 more bytes than an anm file tells it to
        dynArrayFitStructs(&header_mem, &header_size, numbytes);
        if (zipped)
            pigUncompress(header_mem,&numbytes,entry->data,entry->pack_size);
        else
            memcpy(header_mem,entry->data,numbytes);
        *size = numbytes;
        return header_mem;
    }
    else
        return NULL;
}

int pigV2CreateFromMem(NewPigEntry *entries, int count, const char *fname)
{
    PigFile            pig = {0};
    int                i,success=0,header_size;
    PigFileHeader    *header;
    NewPigEntry        *entry;
    FILE            *fout;
    U32                pos,real_size;
    U8                *header_mem=0;

    qsort(entries,count,sizeof(entries[0]),pigCmpEntryPathname);
    PigFileCreate(&pig);
    fout = fopen(fname,"wb");
    if (!fout)
        return 0;
    pig.file_headers        = calloc(sizeof(pig.file_headers[0]),count);
    pig.header.num_files    = count;

    for(i=0;i<count;i++)
    {
        entry = &entries[i];
        pigChecksumAndPackEntry(entry);
        header = &pig.file_headers[i];
        header->file_header_flag    = PIG_FILE_HEADER_FLAG;
        header->name_id                = DataPoolAdd(&pig.string_pool, entry->fname, (U32)strlen(entry->fname)+1);
        header->size                = entry->size;
        header->pack_size            = entry->pack_size;
        header->timestamp            = entry->timestamp;
        header->offset                = (U32)-1; // needs to be filled in later on! (second pass)
        header->reserved            = 0;
        memcpy(header->checksum,entry->checksum,sizeof(header->checksum));

        real_size = entry->pack_size ? entry->pack_size : entry->size;
        header_mem = pigGetHeaderData(entry, &header_size);
        if (header_mem) {
            header->header_data_id = DataPoolAdd(&pig.header_data_pool, header_mem, header_size);
        } else {
            header->header_data_id=-1;
        }
    }
    DataPoolAssembleLists(&pig.string_pool);
    DataPoolAssembleLists(&pig.header_data_pool);
    pos = PigFileOffsetOfData(&pig);

    for(i=0;i<count;i++)
    {
        header = &pig.file_headers[i];
        header->offset = pos;
        real_size = header->pack_size ? header->pack_size : header->size;
        pos += real_size;
    }
    if (fwrite(&pig.header, 1, sizeof(PigHeader), fout) != sizeof(PigHeader))
        goto fail;
    if ((U32)fwrite(pig.file_headers,sizeof(pig.file_headers[0]),pig.header.num_files,fout) != pig.header.num_files)
        goto fail;
    if (DataPoolWrite(fout, &pig.string_pool) == -1)
        goto fail;
    if (DataPoolWrite(fout, &pig.header_data_pool) == -1)
        goto fail;
    for(i=0;i<count;i++)
    {
        entry = &entries[i];
        real_size = entry->pack_size ? entry->pack_size : entry->size;
        if ((U32)fwrite(entry->data,1,real_size,fout) != real_size)
            goto fail;
    }
    success = 1;
fail:
    fclose(fout);
    PigFileDestroy(&pig);
    return success;
}

int pigCreateFromMem(NewPigEntry *entries, int count, const char *fname, int version)
{
    if (version == PIG_VERSION) {
        return pigV2CreateFromMem(entries, count, fname);
    } else if (version == HOG_VERSION) {
        return hogCreateFromMem(entries, count, fname);
    } else {
        assert(0);
        return 0;
    }
}

const char *PigFileGetArchiveFileName(PigFile *handle)
{
    if (handle->hog_file) {
        return hogFileGetArchiveFileName(handle->hog_file);
    }
    return handle->filename;
}


U32 PigFileGetNumFiles(PigFile *handle)
{
    if (handle->hog_file) {
        return hogFileGetNumFiles(handle->hog_file);
    }
    return handle->header.num_files;
}

const char *PigFileGetFileName(PigFile *handle, int index)
{
    if (handle->hog_file) {
        return hogFileGetFileName(handle->hog_file, index);
    }
    return DataPoolGetString(&handle->string_pool, index);
}

bool PigFileIsSpecialFile(PigFile *handle, int index)
{
    if (handle->hog_file) {
        return hogFileIsSpecialFile(handle->hog_file, index);
    }
    return false;
}

U32 PigFileGetFileTimestamp(PigFile *handle, int index)
{
    if (handle->hog_file) {
        return hogFileGetFileTimestamp(handle->hog_file, index);
    }
    return handle->file_headers[index].timestamp;
}

U32 PigFileGetFileSize(PigFile *handle, int index)
{
    if (handle->hog_file) {
        return hogFileGetFileSize(handle->hog_file, index);
    }
    return handle->file_headers[index].size;
}

bool PigFileGetChecksum(PigFile *handle, int index, U32 *checksum)
{
    assert(!handle->hog_file);
    memcpy(checksum, handle->file_headers[index].checksum, sizeof(U32)*4);
    return true;
}

bool PigFileIsZipped(PigFile *handle, int index) // Returns whether or not a file is zipped (used by wrapper to determine buffering rules)
{
    if (handle->hog_file) {
        return hogFileIsZipped(handle->hog_file, index);
    }
    return !!handle->file_headers[index].pack_size;
}

U32 PigFileGetZippedFileSize(PigFile *handle, int index)
{
    if (handle->hog_file) {
        return hogFileGetFileSize(handle->hog_file, index);
    }
    if (handle->file_headers[index].pack_size)
        return handle->file_headers[index].pack_size;

    return 0;
}


// Warning: pointer returned is volatile, and may be invalid after any hog file changes
const U8 *PigFileGetHeaderData(PigFile *handle, int index, U32 *header_size)
{
    if (handle->hog_file) {
        return hogFileGetHeaderData(handle->hog_file, index, header_size);
    }
    return DataPoolGetData(&handle->header_data_pool, handle->file_headers[index].header_data_id, header_size);
}


void PigFileSetFilter(PiggFileFilterCallback cb)
{
    g_piggFileFilterCB = cb;
}

