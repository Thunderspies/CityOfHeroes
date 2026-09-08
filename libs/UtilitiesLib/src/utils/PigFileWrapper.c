/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include "utilitieslib/utils/PigFileWrapper.h"
#include <string.h>
#include "utilitieslib/utils/piglib.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/mathutil.h"
#define WIN32_LEAN_AND_MEAN
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/file.h"

enum {
    PIG_FLAG_EOF = 1 << 0,
    PIG_FLAG_RANDOM_ACCESS = 1 << 1,
};

extern CRITICAL_SECTION PigCritSec;

#define DEFAULT_BUFFER_SIZE 4096

struct PigFileHandle {
    PigFileDescriptor data;
    U32 pos;
    U8 flags; //EOF - any others?
    U8 *buffer; // Buffered data for zipped files and fgetc() calls
    U32 bufferlen; // Amount of data in the buffer
    U32 bufferpos; // Position in the file that the buffer begins at
    U32 last_used_time_stamp; // For freeing zipped data buffer
};

#define MAX_FILES 2048
int pig_file_handles_max=0;
PigFileHandle pig_file_handles[MAX_FILES];

static bool inited=false;
static CRITICAL_SECTION pigFileWrapperCritsec;
void initPigFileHandles(void) {
    if (!inited) {
        InitializeCriticalSection(&pigFileWrapperCritsec);
        memset(pig_file_handles, 0, sizeof(PigFileHandle)*ARRAY_SIZE(pig_file_handles));
        pig_file_handles_max = 0;
        inited = true;
    }
}

PigFileHandle *createPigFileHandle(void) {
    PigFileHandle *fh = NULL;
    int i;

    initPigFileHandles();

    EnterCriticalSection(&pigFileWrapperCritsec);

    for (i=0; i<ARRAY_SIZE(pig_file_handles); i++) {
        if (pig_file_handles[i].data.parent==NULL) {
            fh = &pig_file_handles[i];
            fh->data.parent = (void *)1;
            fh->pos = 0;
            fh->flags = 0;
            fh->bufferlen = 0;
            fh->last_used_time_stamp = timerCpuSeconds();
            assert(fh->buffer == NULL);
            fh->buffer = NULL;
            pig_file_handles_max = MAX(pig_file_handles_max, i);
            LeaveCriticalSection(&pigFileWrapperCritsec);
            return fh;
        }
    }
    LeaveCriticalSection(&pigFileWrapperCritsec);
    assert(!"Ran out of pig_file_handles!");
    return NULL;
}


PigFileHandle *pig_fopen(const char *name,const char *how)
{
    PigFileDescriptor pfd;
    int pig_index = -1;
    int file_index = -1;
    assert(0);
    // This function does not work, we would need to search through the individual pigs to
    //  find this file.  Instead, use the FolderCache

    pfd = PigSetGetFileInfo(pig_index, file_index, name);

    return pig_fopen_pfd(&pfd, how);
}

static bool sPrintPiggFile = false;

PigFileHandle *pig_fopen_pfd(PigFileDescriptor *pfd,const char *how) {
    PigFileHandle *pfh;

    if (strcspn(how, "wWaA+")!=strlen(how)) {
        assert(!"Write accessed asked for on a pigged file!");
        return NULL;
    }

    if (!pfd || !pfd->parent) {
        assert(!"Bad handle passed to pig_fopen_pfd");
        return NULL;
    }

    pfh = createPigFileHandle();
    pfh->data = *pfd;
    if (pfh->pos == pfd->size) { // == 0
        pfh->flags |= PIG_FLAG_EOF;
    }

    if (strchr(how, 'R')) {
        pfh->flags |= PIG_FLAG_RANDOM_ACCESS;
    }

    if (pig_debug || sPrintPiggFile) {
        const char* pigFileName = PigFileGetArchiveFileName(pfd->parent); // eg "./piggs/texWorld.pigg"
        bool bPrintThisFile;
        pigFileName = strrchr(pigFileName, '/') + 1;
        bPrintThisFile = !(strEndsWith(pfh->data.debug_name, ".ogg") || strEndsWith(pfh->data.debug_name, ".texture"));
        bPrintThisFile = bPrintThisFile && ( stricmp(pigFileName, "stage1.pigg") && stricmp(pigFileName, "stage2.pigg") );
        if(bPrintThisFile) {
            OutputDebugStringf("PIG: fopen %-100s from pig file %s\n", pfh->data.debug_name, pigFileName);
        }
    }

    return pfh;
}


int pig_fclose(PigFileHandle *handle) {
    assert(handle->data.parent && "Tried to close an already closed file");
    if (handle->buffer) {
        EnterCriticalSection(&pigFileWrapperCritsec);
        SAFE_FREE(handle->buffer);
        LeaveCriticalSection(&pigFileWrapperCritsec);
    }
    // Must do this last!
    handle->data.parent = (void *)0;
    return 0;
}

int pig_fseek(PigFileHandle *handle,long dist,int whence) {
    long newpos = handle->pos;
    switch(whence) {
    case SEEK_SET:
        assert(dist>=0);
        newpos = dist;
        break;
    case SEEK_CUR:
        newpos += dist;
        break;
    case SEEK_END:
        newpos = handle->data.size + dist;
        break;
    default:
        assert(false);
    }
    if (newpos > (long)handle->data.size) {
        if (pig_debug)
            printf("warning: seek past end of file (%s)\n", handle->data.debug_name);
        handle->pos = handle->data.size;
    } else if (newpos < 0) {
        if (pig_debug)
            printf("warning: seek past begining of file (%s)\n", handle->data.debug_name);
        handle->pos = 0;
    } else {
        handle->pos = newpos;
    }
    if (handle->pos >= handle->data.size) {
        handle->flags |= PIG_FLAG_EOF;
    } else {
        handle->flags &= ~PIG_FLAG_EOF;
    }
    return 0;
}

int pig_fgetc(PigFileHandle *handle) {

    if (handle->flags & PIG_FLAG_EOF)
        return EOF;

    handle->last_used_time_stamp = timerCpuSeconds();

    if (handle->buffer && (handle->pos >= handle->bufferpos) &&
        (handle->pos < handle->bufferpos + handle->bufferlen))
    {
        // We have this character in our buffer
        int ret = handle->buffer[handle->pos - handle->bufferpos];
        pig_fseek(handle, 1, SEEK_CUR);
        return ret;
    }

    // This character is not in the current buffer, get a new one
    // Allocate a buffer and read into it
    if (PigFileIsZipped(handle->data.parent, handle->data.file_index)) {
        // It's zipped, the lower level will do all the buffering for us, just read a character!
        int ret=0;
        pig_fread(handle, &ret, 1);
        return ret;
    } else {
        char *buffer;
        unsigned int len;
        int ret;
        U32 oldpos = handle->pos;
        // Do some buffering
        buffer = malloc(DEFAULT_BUFFER_SIZE);
        len = pig_fread(handle, buffer, DEFAULT_BUFFER_SIZE);
        if (!handle->buffer) {
            // Save the buffer
            handle->buffer = buffer;
            handle->bufferlen = len;
            handle->bufferpos = oldpos;
            if (handle->bufferlen==0) {
                return EOF;
            }
        } else {
            // A buffer was allocated in pig_fread, assume it's the whole file
            ret=buffer[0];
            assert(handle->bufferlen >= len); // If not, we should keep our buffer
            free(buffer);
        }
        pig_fseek(handle, handle->bufferpos + 1, SEEK_SET); // Return cursor to where it should be
        return handle->buffer[0];
    }
}

long pig_ftell(PigFileHandle *handle) {
    return handle->pos;
}

long pig_fread(PigFileHandle *handle, void *buf, long size)
{
    U32 numread;
    if (handle->flags & PIG_FLAG_EOF)
        return 0;

    // Check for reading past the end
    if (handle->pos + size > handle->data.size) {
        size = handle->data.size - handle->pos;
    }

    if (handle->pos + size <= handle->data.header_data_size) {
        // going to read from the cached header (handled by lower level code)
        goto NormalRead;
    }

ReadFromBuffer:
    handle->last_used_time_stamp = timerCpuSeconds();

    // Check for a read from the buffered data
    if (handle->buffer && (handle->pos >= handle->bufferpos) &&
        (handle->pos < handle->bufferpos + handle->bufferlen))
    {
        // We have at least a character in our buffer
        numread = MIN((handle->bufferpos + handle->bufferlen) - handle->pos, (U32)size);
        memcpy(buf, handle->buffer + handle->pos - handle->bufferpos, numread);
        size-=numread;
        pig_fseek(handle, numread, SEEK_CUR); // advance handle->pos
        buf=((U8*)buf)+numread;
        if (size==0) // read everything from buffer
            return numread;
        // I don't think we should ever get here unless both fread and fgetc are used on the same file
    }

    if ((U32)size == handle->data.size && handle->pos == 0) {
        // Reading the entire file, just pass it through
        goto NormalRead;
    }

    // otherwise...
    // going to be reading a partial chunk of data
    // if it's zipped, let's read the whole file and cache it
    if (PigFileIsZipped(handle->data.parent, handle->data.file_index)) {
        // Disable buffering for when random access is requested
        if (handle->flags & PIG_FLAG_RANDOM_ACCESS) {
            goto NormalRead;
        }

        // Zipped!
        SAFE_FREE(handle->buffer);
        handle->bufferlen = handle->data.size;
        handle->buffer = malloc(handle->bufferlen);
        handle->bufferpos = 0;
        PigSetExtractBytes(&(handle->data), handle->buffer, 0, handle->bufferlen, false);
        goto ReadFromBuffer;
    }

    // If we got here we're doing a partial read on a non-zipped file
NormalRead:
    numread = PigSetExtractBytes(&(handle->data), buf, handle->pos, size, handle->flags & PIG_FLAG_RANDOM_ACCESS);
    handle->pos += numread;
    if (handle->pos >= handle->data.size) {
        handle->flags |= PIG_FLAG_EOF;
    }
    return numread;
}

char *pig_fgets(PigFileHandle *handle, char *buf, int len) {
    int pos=0;
    int ch;
    
    if (handle->flags & PIG_FLAG_EOF) {
        return NULL;
    }

    ch = pig_fgetc(handle);

    while (!(handle->flags & PIG_FLAG_EOF) && ch!='\n' && ch && pos<len-1) {
        buf[pos++] = (char)ch;
        ch = pig_fgetc(handle);
    }
    if (pos && buf[pos-1]=='\r') { // Assumes gets only called on ascii files, not binary
        buf[pos-1]=ch;
    } else {
        buf[pos++]=ch;
    }
    buf[pos]=0;
    return buf;
}

long pig_filelength(PigFileHandle *handle) {
    return handle->data.size;
}

void *pig_lockRealPointer(PigFileHandle *handle)
{
    assert(0); // Not implemented with respect to Hog files
    EnterCriticalSection(&PigCritSec);

    //fseek(handle->data.parent->file, handle->data.file_header->offset + handle->pos, SEEK_SET);

    return NULL; //fileRealPointer(handle->data.parent->file);
}

void pig_unlockRealPointer(PigFileHandle *handle) {
    LeaveCriticalSection(&PigCritSec);
}

// Frees a buffer, if there is one, on a zipped file handle
void pig_freeBuffer(PigFileHandle *handle)
{
    EnterCriticalSection(&pigFileWrapperCritsec);
    SAFE_FREE(handle->buffer);
    LeaveCriticalSection(&pigFileWrapperCritsec);
}

// Frees buffers on handles that haven't been accessed in a long time
#define TIME_TO_KEEP 5*60
void pig_freeOldBuffers(void)
{
    int i;
    U32 time = timerCpuSeconds();
    for (i=0; i<pig_file_handles_max; i++)
    {
        if (pig_file_handles[i].buffer && (pig_file_handles[i].last_used_time_stamp + TIME_TO_KEEP) < time) {
            pig_freeBuffer(&pig_file_handles[i]);
        }
    }
}
