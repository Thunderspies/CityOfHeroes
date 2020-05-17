// serialize.c - provides simple binary serialization functions for storing structs
// these functions allow for a simple binary format: hierarchical, self describing,
// with traversal.  64 bytes overhead per struct

#include "utilitieslib/utils/serialize.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "utilitieslib/utils/wininclude.h"
#include <io.h>
#include <sys/stat.h>


#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/utils/file.h"

////////////////////////////////////////////// simple buffer files

#define INITIAL_BUF_SIZE 100
#define CHUNK_SIZE 65536

typedef struct SimpleBuffer {
    char* data;
    int filesize;    // logical size of file
    int memsize;    // allocated size
    int curposition;
    int writing;    // are we writing or reading?
    int forcewrite;
    char filename[MAX_PATH];
} SimpleBuffer;

SimpleBufHandle SimpleBufOpenWrite(const char* filename, int forcewrite)
{
    SimpleBuffer* buf;
    buf = malloc(sizeof(SimpleBuffer));
    memset(buf, 0, sizeof(SimpleBuffer));
    buf->data = calloc(1, INITIAL_BUF_SIZE);
    buf->filesize = 0;
    buf->memsize = INITIAL_BUF_SIZE;
    buf->writing = 1;
    buf->forcewrite = forcewrite;
    strcpy(buf->filename, filename);
    return (SimpleBufHandle)buf;
}

SimpleBufHandle SimpleBufOpenRead(const char* filename)
{
    SimpleBuffer* buf;
    //FILE *file;
    //int re;
    //char chunk_buf[CHUNK_SIZE];

    buf = malloc(sizeof(SimpleBuffer));
    memset(buf, 0, sizeof(SimpleBuffer));
    buf->filesize = 0;
    buf->writing = 0;
    strcpy(buf->filename, filename);

    // one pass only for compatibility with zip files
    /*
    buf->data = malloc(INITIAL_BUF_SIZE);
    buf->memsize = INITIAL_BUF_SIZE;

    file = fileOpen(filename, "rb");
    if (!file)
        return NULL;
    while (1)
    {
        re = (int)fread(chunk_buf, sizeof(char), CHUNK_SIZE, file);
        if (!re)
            break;
        buf->memsize = buf->filesize = buf->curposition + re;
        buf->data = realloc(buf->data, buf->filesize);
        memcpy(buf->data + buf->curposition, chunk_buf, re);
        buf->curposition += re;
    }
    fclose(file);*/
    buf->data = fileAlloc(filename, &buf->filesize);
    if (!buf->data)
    {
        free(buf);
        return 0;
    }
    buf->memsize = buf->filesize;
    buf->curposition = 0;

    return (SimpleBufHandle)buf;
}

void SimpleBufClose(SimpleBufHandle handle)
{
    SimpleBuffer* buf = (SimpleBuffer*)handle;
    
    if (buf->writing && buf->filename[0])
    {
        FILE *file;
        int wr;
        char chunk_buf[CHUNK_SIZE];

        // make it writable if it exists
        if (buf->forcewrite)
            _chmod(buf->filename, _S_IREAD | _S_IWRITE);

        // one pass only for compatibility with zip files
        file = fileOpen(buf->filename, "wb");
        if (!file) 
        {
            printf("SimpleBufClose: couldn't write to %s\n", buf->filename);
        }
        else // opened file ok
        {
            buf->curposition = 0;
            while (1)
            {
                wr = buf->filesize - buf->curposition;
                if (wr > CHUNK_SIZE)
                    wr = CHUNK_SIZE;
                if (wr <= 0)
                    break;
                memcpy(chunk_buf, buf->data + buf->curposition, wr);
                fwrite(chunk_buf, sizeof(char), wr, file);
                buf->curposition += wr;
            }
            fclose(file);
        }
    }
    // don't do anything for a read

    free(buf->data);
    free(buf);
}

void *SimpleBufGetData(SimpleBufHandle handle,U32 *len)
{
    SimpleBuffer* buf = (SimpleBuffer*)handle;

    *len = buf->filesize;
    return buf->data;
}

SimpleBufHandle SimpleBufSetData(void *data,U32 len)
{
    SimpleBuffer* buf;

    buf = calloc(sizeof(SimpleBuffer),1);
    buf->data = data;
    buf->filesize = buf->memsize = len;
    return (SimpleBufHandle)buf;
}

void SimpleBufResizeEx(SimpleBufHandle handle, int atleast)
{
    SimpleBuffer *buf = (SimpleBuffer*)handle;

    if(buf->memsize < atleast)
    {
        if(buf->memsize < INITIAL_BUF_SIZE)
            buf->memsize = INITIAL_BUF_SIZE;
        while(buf->memsize < atleast)
            buf->memsize *= 2;
        buf->data = realloc(buf->data, buf->memsize);
    }
}

void SimpleBufGrow(SimpleBufHandle handle, U32 len)
{
    SimpleBuffer *buf = (SimpleBuffer*)handle;
    SimpleBufResizeEx(handle, buf->curposition + len);
}

int SimpleBufWrite(const void* data, int size, SimpleBufHandle handle)
{
    SimpleBuffer* buf = (SimpleBuffer*)handle;

    SimpleBufResizeEx(handle, buf->curposition + size);
    if (buf->curposition + size > buf->filesize)
        buf->filesize = buf->curposition + size;
    memcpy(buf->data + buf->curposition, data, size);
    buf->curposition += size;
    return size;
}

int SimpleBufRead(void* data, int size, SimpleBufHandle handle)
{
    SimpleBuffer* buf = (SimpleBuffer*)handle;
    
    if (buf->curposition + size > buf->filesize) size = buf->filesize - buf->curposition;
    memcpy(data, buf->data + buf->curposition, size);
    buf->curposition += size;
    return size;
}

int SimpleBufSeek(SimpleBufHandle handle, long offset, int origin)
{
    SimpleBuffer* buf = (SimpleBuffer*)handle;

    if(origin == SEEK_END)
        buf->curposition = buf->filesize;
    else if (origin == SEEK_SET)
        buf->curposition = 0;
    // SEEK_CUR ok

    buf->curposition += offset;
    if (buf->curposition < 0)
        buf->curposition = 0;
    SimpleBufResizeEx(handle, buf->curposition);
    if (buf->curposition > buf->filesize)
        buf->filesize = buf->curposition;
    return 0;
}

int SimpleBufTell(SimpleBufHandle handle)
{
    SimpleBuffer* buf = (SimpleBuffer*)handle;
    return buf->curposition;
}


/////////////////////////////////////////////////////////////// internal util
static char cryptic_sig[] = "CrypticS";
#define cryptic_sig_len 8

int WritePascalString(SimpleBufHandle file, const char* str) // write a string to a file, with padding for DWORD lines, returns bytes written
{
    int wr1, wr2, wr3;
    int zero = 0;
    unsigned short len;
    int paddingreq;

    if (!str)
        str = "";    // ok to write an empty string
    len = (unsigned short)strlen(str);
    paddingreq = (4 - (len + sizeof(unsigned short)) % 4) % 4;
    wr1 = SimpleBufWriteU16(len, file);
    wr2 = SimpleBufWrite(str, sizeof(char) * len, file);
    wr3 = SimpleBufWrite(&zero, sizeof(char) * paddingreq, file);
    if (wr2 != len || wr3 != paddingreq)
        printf("WritePascalString: failed writing %s\n", str);
    return sizeof(unsigned short) + wr2 + wr3;
}

int ReadPascalString(SimpleBufHandle file, char* str, int strsize) // read a corresponding string, returns bytes read
{
    int re;
    unsigned short len;
    int paddingreq;

    str[0] = 0;
    re = SimpleBufReadU16(&len, file);
    if (!re) 
    { 
        printf("ReadPascalString: couldn't read len\n"); 
        return re; 
    }
    paddingreq = (4 - (len + sizeof(unsigned short)) % 4) % 4;
    if (len >= strsize) 
    { 
        printf("ReadPascalString: string read too long for size\n");
        SimpleBufSeek(file, paddingreq + len, SEEK_CUR);
        return sizeof(unsigned short) + paddingreq + len;
    }
    re = SimpleBufRead(str, sizeof(char) * len, file);
    str[len] = 0;
    if (re != len)
    {
        printf("ReadPascalString: couldn't read entire string\n");
        return sizeof(unsigned short) + paddingreq + len;
    }
    SimpleBufSeek(file, paddingreq, SEEK_CUR);
    return sizeof(unsigned short) + paddingreq + len;
}

static int SkipPascalString(SimpleBufHandle file) // returns bytes skipped
{
    int paddingreq;
    unsigned short len = 0;
    SimpleBufReadU16(&len, file);
    paddingreq = (4 - (len + sizeof(unsigned short)) % 4) % 4;
    SimpleBufSeek(file, len + paddingreq, SEEK_CUR);
    return sizeof(unsigned short) + len + paddingreq;
}

/////////////////////////////////////////////////////////// writing functions
SimpleBufHandle SerializeWriteOpen(const char* filename, const char* filetype, int build)
{
    int wr;
    SimpleBufHandle result = SimpleBufOpenWrite(filename, 1);
    if (!result) 
    {
        printf("SerializeWriteOpen: failed to open %s\n", filename);
        return 0;
    }

    // signature
    SimpleBufWrite(cryptic_sig, cryptic_sig_len, result);
    wr = SimpleBufWriteU32((U32)build, result);
    if (!wr) 
    { 
        printf("SerializeWriteOpen: failed writing build to file\n"); 
        SimpleBufClose(result); 
        return 0; 
    }
    WritePascalString(result, filetype);
    return result;
}

void SerializeClose(SimpleBufHandle sfile)
{
    SimpleBufClose(sfile);
}

int SerializeWriteStruct(SimpleBufHandle sfile, char* structname, int size, void* structptr)    // returns length written
{
    int sum;
    int wr;
    sum = WritePascalString(sfile, structname); 
    wr = SimpleBufWriteU32(size, sfile);
    if (!wr)
        printf("SerializeWriteStruct: failed writing size\n");
    wr = SimpleBufWrite(structptr, size, sfile);
    if (!wr)
        printf("SerializeWriteStruct: failed writing struct\n");
    return sum + sizeof(U32) + size;
}

int SerializeWriteHeader(SimpleBufHandle sfile, char* structname, int size, long* loc)        // returns length written
{
    int sum;
    int wr;
    sum = WritePascalString(sfile, structname);
    if (loc)
        *loc = SimpleBufTell(sfile);    // location for size later
    wr = SimpleBufWriteU32(size, sfile);
    if (!wr)
        printf("SerializeWriteHeader: failed writing size\n");
    return sum + sizeof(int);
}

int SerializeWriteData(SimpleBufHandle sfile, int size, void* dataptr)                // returns length written
{
    int wr = SimpleBufWrite(dataptr, size, sfile);
    if (!wr)
        printf("SerializeWriteData: failed writing data\n");
    return size;
}

int SerializePatchHeader(SimpleBufHandle sfile, int size, long loc)                    // use loc returned by SerializeWriteHeader
{
    int wr;
    long cur = SimpleBufTell(sfile);
    SimpleBufSeek(sfile, loc, SEEK_SET);
    wr = SimpleBufWriteU32(size, sfile);
    if (!wr)
        printf("SerializePatchHeader: failed patching size\n");
    SimpleBufSeek(sfile, cur, SEEK_SET);
    return wr;
}

///////////////////////////////////////////////////////////////////// read functions
SimpleBufHandle SerializeReadOpen(const char* filename, const char* filetype, int build, int ignore_crc_difference)
{
    SimpleBufHandle result;
    int readbuild = 0;
    char readsig[cryptic_sig_len+1];
    char readtype[MAX_FILETYPE_LEN];

    result = SimpleBufOpenRead(filename);
    if (!result) 
    { 
        //printf("SerializeReadOpen: failed to open %s\n", filename); 
        return NULL; 
    }

    // signature
    readsig[0] = 0;
    SimpleBufRead(readsig, cryptic_sig_len, result);
    readsig[cryptic_sig_len] = 0;
    SimpleBufReadU32((U32*)&readbuild, result);
    readtype[0] = 0;
    ReadPascalString(result, readtype, MAX_FILETYPE_LEN);
    if (strcmp(cryptic_sig, readsig) != 0 ||
        strcmp(readtype, filetype) != 0 ||
        (!ignore_crc_difference && (readbuild != build)))
    {
        //printf("SerializeReadOpen: invalid signature on file %s\n", filename);
        SimpleBufClose(result);
        return 0;
    }

    return result;
}

int SerializeNextStruct(SimpleBufHandle sfile, char* structname, int namesize, int* size) // loads name, size.  returns success
{
    long cur = SimpleBufTell(sfile);
    int re;
    int success = 1;

    ReadPascalString(sfile, structname, namesize);
    if (size)
    {
        re = SimpleBufReadU32((U32*)size, sfile);
        if (!re)
        {
            printf("SerializeNextStruct: failed reading size\n");
            *size = 0;
            success = 0;
        }
    }
    SimpleBufSeek(sfile, cur, SEEK_SET);
    return success;
}

int SerializeSkipStruct(SimpleBufHandle sfile)            // returns length skipped
{
    int size;
    int re;
    int sum;

    sum = SkipPascalString(sfile);
    re = SimpleBufReadU32((U32*)&size, sfile);
    if (!re)
        return sum; // end of file
    SimpleBufSeek(sfile, size, SEEK_CUR);
    return sum + sizeof(int) + size;
}

int SerializeReadStruct(SimpleBufHandle sfile, char* structname, int size, void* structptr)    // returns length read
{
    int sum;
    int re;
    char readtype[MAX_STRUCTNAME_LEN];
    int readsize;

    // header
    sum = ReadPascalString(sfile, readtype, MAX_STRUCTNAME_LEN);
    re = SimpleBufReadU32((U32*)&readsize, sfile);
    if (!re)
    {
        printf("SerializeReadStruct: failed reading header\n");
        return 0;
    }
    if (readsize != size || strcmp(structname, readtype) != 0)
    {
        printf("SerializeReadStruct: encountered unexpected structure, skipping\n");
        SimpleBufSeek(sfile, size, SEEK_CUR);
        return sum + size;
    }
    
    // data
    re = SimpleBufRead(structptr, sizeof(char) * size, sfile);
    if (re != size)
        printf("SerializeReadStruct: failed reading data\n");
    return sum + sizeof(int) + size;
}

int SerializeReadHeader(SimpleBufHandle sfile, char* structname, int namesize, int* size) // loads name, size. returns success
{
    int re;

    // header
    if (!ReadPascalString(sfile, structname, namesize))
        return 0;
    if (size)
    {
        re = SimpleBufReadU32((U32*)size, sfile);
        if (!re)
        {
            printf("SerializeReadHeader: failed reading size\n");
            return 0;
        }
    }
    else
    {
        SimpleBufSeek(sfile, sizeof(int), SEEK_CUR); // skip size
    }
    return 1;
}

int SerializeReadData(SimpleBufHandle sfile, int size, void* dataptr)                    // returns length read
{
    int re = SimpleBufRead(dataptr, sizeof(char) * size, sfile);
    if (re != size)
        printf("SerializeReadData: failed reading data\n");
    return re;
}
