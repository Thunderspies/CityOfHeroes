#include <utilitieslib/assert/assert.h>

#include <dxtlib.h>
#include <stdio.h>

extern "C" {

HRESULT nvDXTcompressC(unsigned char * raw_data, // pointer to data (24 or 32 bit)
    unsigned long w, // width in texels
    unsigned long h, // height in texels
    DWORD byte_pitch,
    CompressionOptions * options,
    DWORD planes, // 3 or 4
    MIPcallback callback,  // callback for generated levels
    RECT * rect)   // subrect to operate on, NULL is whole image
{
    return nvDXTcompress(raw_data, w, h, byte_pitch, options, planes, callback, rect);
}

HRESULT nvDXTcompressVolumeC(unsigned char * raw_data, // pointer to data (24 or 32 bit)
    unsigned long w, // width in texels
    unsigned long h, // height in texels
    unsigned long depth, // depth of volume texture
    DWORD byte_pitch,
    CompressionOptions * options,
    DWORD planes, // 3 or 4
    MIPcallback callback,  // callback for generated levels
    RECT * rect)   // subrect to operate on, NULL is whole image
{
    return nvDXTcompressVolume(raw_data, w, h, depth, byte_pitch, options, planes, callback, rect);
}

// floating point input
HRESULT nvDXTcompress32FC(fpPixel * raw_data, // pointer to data (24 or 32 bit)
    unsigned long w, // width in texels
    unsigned long h, // height in texels
    DWORD pitch, // in fpPixels
    CompressionOptions * options,
    DWORD planes, // 3 or 4
    MIPcallback callback,  // callback for generated levels
    RECT * rect)   // subrect to operate on, NULL is whole image
{
    return nvDXTcompress32F(raw_data, w, h, pitch, options, planes, callback, rect);
}

static unsigned char *decompressData=NULL;
static unsigned int decompressOffs;

static bool nvdxt_crit_sect_inited = false;
static CRITICAL_SECTION nvdxt_crit_sect;
void initNVDXTCriticalSection(void)
{
    if (!nvdxt_crit_sect_inited) {
        InitializeCriticalSection(&nvdxt_crit_sect);
        nvdxt_crit_sect_inited = true;
    }
}

unsigned char * nvDXTdecompressC(OUT int * w, OUT int * h, OUT int * depth, OUT int * total_width, OUT int * rowBytes, OUT int * src_format,
                                 int SpecifiedMipMaps, unsigned char *data)
{
    int w2=*w, h2=*h, depth2=*depth, total_width2=*total_width, rowBytes2=*rowBytes, src_format2=*src_format;
    unsigned char *ret;
    assert(nvdxt_crit_sect_inited);

    EnterCriticalSection(&nvdxt_crit_sect);
    assert(decompressData==NULL);
    decompressData = data;
    decompressOffs=0;

    ret = nvDXTdecompress(w2, h2, depth2, total_width2, rowBytes2, src_format2, SpecifiedMipMaps);

    assert(decompressData==data);
    decompressData = NULL;
    LeaveCriticalSection(&nvdxt_crit_sect);

    *w = w2;
    *h = h2;
    *depth=depth2;
    *total_width=total_width2;
    *rowBytes=rowBytes2;
    *src_format=src_format2;

    return ret;
}





} // extern "C"


void WriteDTXnFile(DWORD count, void * buffer, void * userData)
{
    printf(__FUNCTION__);
}
void ReadDTXnFile(DWORD count, void * buffer, void * userData)
{
    memcpy(buffer, decompressData + decompressOffs, count);
    decompressOffs += count;
}



