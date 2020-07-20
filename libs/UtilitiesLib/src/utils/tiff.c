#include "utilitieslib/stdtypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/tiff.h"

#if (__STDC_VERSION__ >= 199901L) || __GNUC__
    #include <stdint.h>
    #include <stdbool.h>
    #include <alloca.h>
#else
typedef signed __int8 int8_t;
typedef signed __int16 int16_t;
typedef signed __int32 int32_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
    #define __attribute__(args)
    #define inline __inline
    #ifndef __cplusplus
        #include "utilitieslib/utils/file.h"
    #endif
#endif

typedef struct _tiffHeader {
    char byteorder[2];
    uint16_t fourtytwo;
    uint32_t ifd;
} __attribute__((packed)) tiffHeader;

typedef struct _tiffIfdEntry {
    uint16_t tag;
    uint16_t type;
    uint32_t count;
    uint32_t value;
} __attribute__((packed)) tiffIfdEntry;

typedef struct _tiffIfd {
    uint16_t count;
} __attribute__((packed)) tiffIfd;

enum tiffTypes {
    tiffByte = 1,
    tiffAscii = 2,
    tiffShort = 3,
    tiffLong = 4,
    tiffRational = 5,
    tiffSByte = 6,
    tiffUndefined = 7,
    tiffSShort = 8,
    tiffSLong = 9,
    tiffSRational = 10,
    tiffFloat = 11,
    tiffDouble = 12,
};

enum tiffCompressions {
    tiffUncompresed = 1,
    tiffRLE = 2,
    tiffT4 = 3,
    tiffT6 = 4,
    tiffLZW = 5,
    tiffJPEG = 6,
    tiffPackBits = 32773,
};

enum tiffPhotometricInterpretations {
    tiffWhiteIsZero = 0,
    tiffBlackIsZero = 1,
    tiffRGB = 2,
    tiffRGBPalette = 3,
    tiffTransparencyMask = 4,
    tiffCMYK = 5,
    tiffYCbCr = 6,
    tiffCIELab = 8,
};

enum tiffOrientations {
    tiffUpperLeft = 1,
    tiffUpperRight = 2,
    tiffLowerRight = 3,
    tiffLowerLeft = 4,
    tiffLeftUpper = 5,
    tiffRightUpper = 6,
    tiffRightLower = 7,
    tiffLeftLower = 8,
};

enum tiffResolutionUnits {
    tiffNoUnits = 1,
    tiffInch = 2,
    tiffCentimeter = 3,
};

enum tiffPlanarConfigurations {
    tiffChunky = 1,    // RGB RGB RGB
    tiffPlanar = 2,    // RRR GGG BBB
};

enum tiffInkSet {
    tiffIsCMYK = 1,
    tiffNotCMYK = 2,
};

enum tiffExtraSamples {
    tiffUnspecified = 0,
    tiffAssociatedAlpha = 1,
    tiffUnassociatedAlpha = 2,
};

enum tiffSampleFormats {
    tiffUnsigned = 1,
    tiffSigned = 2,
    tiffIEEEFloat = 3,
};

enum tiffTags {
    tiffImageWidth = 256, // short or long
    tiffImageLength = 257, // short or long
    tiffBitsPerSample = 258, //short
    tiffCompression = 259, // short
    tiffPhotometricInterpretation = 262, // short
    tiffDocumentName = 269,    // ascii
    tiffStripOffsets = 273,    // short or long
    tiffOrientation = 274, // short
    tiffSamplesPerPixel = 277, // short
    tiffRowsPerStrip = 278,    // short or long
    tiffStripByteCounts = 279, // short or long
    tiffXResolution = 282, // rational
    tiffYResolution = 283, // rational
    tiffPlanarConfiguration = 284, // short
    tiffResolutionUnit = 296, // short
    tiffTransferFunction = 301, // short * (1 or 3) * (1<<BitsPerSample)
    tiffSoftware = 305, // ascii
    tiffDateTime = 306, // ascii
    tiffArtist = 315, // ascii
    tiffHostComputer = 316,    // ascii
    tiffWhitePoint = 318, // rational * (x,y)
    tiffPrimaryChromacties = 319, // rational * 6
    tiffColorMap = 320, // short * (r,r,r,g,g,g,b,b,b) for 2^BitsPerSample
    tiffInkSet = 332, // short
    tiffInkNames = 333, // ascii
    tiffNumberOfInks = 334,    // short
    tiffExtraSamples = 338,    // short
    tiffSampleFormat = 339,    // short
    tiffSMinSampleValue = 340, // any
    tiffSMaxSampleValue = 341, // any
    tiffTransferRange = 342, // short * 6
    tiffReferenceBlackWhite = 532, // rational * 6
    tiffCopyright = 33432, // ascii
};

typedef struct _tiffFile {
    tiffHeader * header;
    tiffIfd * ifd;
    tiffIfdEntry * entries;
    char * data;
    uint32_t nextDataOffset;
} __attribute__((packed)) tiffFile;

static inline size_t tiffTypeSize(uint16_t tag)
{
    switch (tag) {
        case tiffUndefined:
        case tiffAscii:
        case tiffByte: return 1;
        case tiffSShort:
        case tiffShort: return 2;
        case tiffFloat:
        case tiffSLong:
        case tiffLong: return 4;
        case tiffDouble:
        case tiffSRational:
        case tiffRational: return 8;
    }
    return 0;
}

static inline void tiffIfdStore(tiffFile * file, uint16_t tag, uint16_t type, uint16_t count, void * value)
{
    // setup the entry
    tiffIfdEntry * entry = file->entries + file->ifd->count++;
    entry->count = count;
    entry->tag = tag;
    entry->type = type;

    // calculate storage size
    {
        size_t size = tiffTypeSize(entry->type) * entry->count;
        if (size <= 4) {
            // inline storage
            entry->value = 0;
            memcpy(&entry->value, value, size);
        } else {
            // out of band storage
            entry->value = file->nextDataOffset;
            memcpy(file->data + file->nextDataOffset, value, size);
            file->nextDataOffset += (uint32_t)size;

            // word align
            if (!(file->nextDataOffset-1) && file->nextDataOffset) {
                file->data[file->nextDataOffset++] = 0;
            }
        }
    }
}

static inline void tiffIfdStoreUnsigned(tiffFile * file, uint16_t tag, uint32_t value)
{
    if (value < 65536) {
        uint16_t shortValue = value;
        tiffIfdStore(file, tag, tiffShort, 1, &shortValue);
    } else {
        tiffIfdStore(file, tag, tiffLong, 1, &value);
    }
}

static int tiffIfdCmp(const void * a, const void * b)
{
    const tiffIfdEntry * iA = (const tiffIfdEntry *)a;
    const tiffIfdEntry * iB = (const tiffIfdEntry *)b;
    if (iA->tag != iB->tag) {
        return(iA->tag < iB->tag) ? -1 : 1;
    }
    return 0;
}

static tiffIfdEntry * tiffIfdFind(tiffFile * file, uint16_t tag)
{
    tiffIfdEntry key;
    key.tag = tag;
    return(tiffIfdEntry*)bsearch(&key, file->entries, file->ifd->count, sizeof(tiffIfdEntry), tiffIfdCmp);
}

static inline size_t tiffIfdSize(tiffIfdEntry * entry)
{
    return tiffTypeSize(entry->type) * entry->count;
}

static void * tiffIfdValue(tiffFile * file, tiffIfdEntry * entry)
{
    if (tiffIfdSize(entry) <= 4) {
        return &entry->value;
    }
    return file->data + entry->value;
}

static bool tiffIfdUpdate(tiffFile * file, uint16_t tag, void * value)
{
    tiffIfdEntry * entry = tiffIfdFind(file, tag);
    if (!entry) {
        return false;
    }

    memcpy(tiffIfdValue(file, entry), value, tiffIfdSize(entry));
    return true;
}

static long long tiffIfdGetInteger(tiffFile * file, uint16_t tag, long long defaultValue)
{
    tiffIfdEntry * entry = tiffIfdFind(file, tag);
    if (!entry) {
        return defaultValue;
    }
    if (entry->count != 1) {
        return defaultValue;
    }

    {
        void * value = tiffIfdValue(file, entry);
        switch (entry->type) {
            case tiffByte: return *(uint8_t*)value;
            case tiffShort: return *(uint16_t*)value;
            case tiffLong: return *(uint32_t*)value;
            case tiffSByte: return *(int8_t*)value;
            case tiffSShort: return *(int16_t*)value;
            case tiffSLong: return *(int32_t*)value;
            default: assert(0);
        }
    }
    return defaultValue;
}

static void tiffIfdOffsetFixup(tiffFile * file, int32_t delta)
{
    // subtract the offset from the data to make the pointers still valid
    file->data -= delta;

    {
        int i=0;
        for (; i < file->ifd->count; i++) {
            tiffIfdEntry * entry = file->entries + i;
            // only fixup out of band storage
            if ((tiffTypeSize(entry->type) * entry->count) > 4) {
                entry->value += delta;
            }
        }
    }
}

void tiffSave(const char * filename, const void * data, int width, int height, int nchannels, int bytesperchannel, int bottomfirst, int isfloat, int storealpha)
{
    tiffHeader header;
    tiffIfd ifd;
    tiffIfdEntry entries[64];
    char extraValues[8192];

    tiffFile file;
    file.header = &header;
    file.ifd = &ifd;
    file.entries = entries;
    file.data = extraValues;
    file.nextDataOffset = 0;

    // header
    {
        const int endiantest = 1;
        if (*(char*)&endiantest) {
            header.byteorder[0] = header.byteorder[1] = 'I';
        } else {
            header.byteorder[0] = header.byteorder[1] = 'M';
        }
    }
    header.fourtytwo = 42;
    header.ifd = sizeof(header);
    ifd.count = 0;

    // next ifd offset
    *(uint32_t*)file.data = 0;
    file.nextDataOffset += sizeof(uint32_t);

    // ifd entries
    tiffIfdStoreUnsigned(&file, tiffImageWidth, width);
    tiffIfdStoreUnsigned(&file, tiffImageLength, height);
    tiffIfdStoreUnsigned(&file, tiffCompression, tiffUncompresed);
    tiffIfdStoreUnsigned(&file, tiffOrientation, bottomfirst ? tiffLowerLeft : tiffUpperLeft);
    tiffIfdStoreUnsigned(&file, tiffSampleFormat, isfloat ? tiffIEEEFloat : tiffUnsigned);

    {
        uint16_t bpcArray[4] = {bytesperchannel * 8, bytesperchannel * 8, bytesperchannel * 8, bytesperchannel * 8};
        if (nchannels == 1) {
            tiffIfdStoreUnsigned(&file, tiffPhotometricInterpretation, tiffBlackIsZero);        
            tiffIfdStoreUnsigned(&file, tiffBitsPerSample, bpcArray[0]);
        } else if (nchannels == 3) {
            tiffIfdStoreUnsigned(&file, tiffPhotometricInterpretation, tiffRGB);
            tiffIfdStoreUnsigned(&file, tiffSamplesPerPixel, 3);
            tiffIfdStore(&file, tiffBitsPerSample, tiffShort, 3, bpcArray);
        } else if (nchannels == 4) {
            tiffIfdStoreUnsigned(&file, tiffPhotometricInterpretation, tiffRGB);
            tiffIfdStoreUnsigned(&file, tiffExtraSamples, storealpha ? tiffUnassociatedAlpha : tiffUnspecified);
            tiffIfdStoreUnsigned(&file, tiffSamplesPerPixel, 4);
            tiffIfdStore(&file, tiffBitsPerSample, tiffShort, 4, bpcArray);
        }
    }

    {
        uint32_t stripByteCounts = width * height * nchannels * bytesperchannel;
        tiffIfdStoreUnsigned(&file, tiffStripByteCounts, stripByteCounts);
        tiffIfdStoreUnsigned(&file, tiffRowsPerStrip, height);
        tiffIfdStoreUnsigned(&file, tiffStripOffsets, 0);

        // entries sorted by tag
        qsort(entries, ifd.count, sizeof(tiffIfdEntry), tiffIfdCmp);

        // update offsets to point past the end of the entries
        {
            int32_t delta = sizeof(header) + sizeof(ifd) + sizeof(tiffIfdEntry) * ifd.count;
            uint32_t finalStripOffsets = delta + file.nextDataOffset;
            tiffIfdUpdate(&file, tiffStripOffsets, &finalStripOffsets);
            tiffIfdOffsetFixup(&file, delta);

            {
                FILE * f = fopen(filename, "wb");
                assert(f);
                fwrite(&header, sizeof(header), 1, f);
                assert(ftell(f) == header.ifd);
                fwrite(&ifd, sizeof(ifd), 1, f);
                fwrite(entries, sizeof(tiffIfdEntry), ifd.count, f);
                assert(ftell(f) == delta);
                fwrite(extraValues, file.nextDataOffset, 1, f);
                assert(ftell(f) == finalStripOffsets);
                fwrite(data, nchannels * bytesperchannel, width * height, f);
                assert(ftell(f) == (finalStripOffsets + stripByteCounts));
                fclose(f);
            }
        }
    }
}

#if (__STDC_VERSION__ >= 199901L) || __GNUC__ || defined(__cplusplus)
void * tiffLoad(const char * filename, int * width, int * height, int * nchannels, int * bytesperchannel, int * bottomfirst, int * isfloat, int * hasalpha)
{
    FILE * f = fopen(filename, "rb");
    assert(f);
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char * data = (char*)malloc(size);
    int ret = fread(data, size, 1, f);
    fclose(f);
    if (ret != 1) {
        return NULL;
    }

    tiffFile file;
    file.data = data;
    file.nextDataOffset = ~0U;
    file.header = (tiffHeader*)data;

    const int endiantest = 1;
    if (*(char*)&endiantest) {
        assert((file.header->byteorder[0] == 'I') && (file.header->byteorder[1] == 'I'));
    } else {
        assert((file.header->byteorder[0] == 'M') && (file.header->byteorder[1] == 'M'));
    }
    assert(file.header->fourtytwo == 42);

    file.ifd = (tiffIfd*)(data + file.header->ifd);
    file.entries = (tiffIfdEntry*)(data + file.header->ifd + sizeof(tiffIfd));
    assert(*(uint32_t*)&file.entries[file.ifd->count] == 0);

    int imageWidth = tiffIfdGetInteger(&file, tiffImageWidth, -1);
    int imageHeight = tiffIfdGetInteger(&file, tiffImageLength, -1);
    assert(tiffIfdGetInteger(&file, tiffCompression, tiffUncompresed) == tiffUncompresed);
    int orientation = tiffIfdGetInteger(&file, tiffOrientation, tiffUpperLeft);
    int photometricInterpretation = tiffIfdGetInteger(&file, tiffPhotometricInterpretation, -1);
    int sampleFormat = tiffIfdGetInteger(&file, tiffSampleFormat, tiffUnsigned);
    int samplesPerPixel = tiffIfdGetInteger(&file, tiffSamplesPerPixel, 1);
    int extraSamples = tiffIfdGetInteger(&file, tiffExtraSamples, -1);
    assert(tiffIfdGetInteger(&file, tiffRowsPerStrip, (1LL<<32)-1) <= imageHeight);
    long long stripOffsets = tiffIfdGetInteger(&file, tiffStripOffsets, -1);
    assert(stripOffsets != -1);
    long long stripByteCounts = tiffIfdGetInteger(&file, tiffStripByteCounts, -1);
    assert(stripByteCounts != -1);

    tiffIfdEntry * bitsPerSampleEntry = tiffIfdFind(&file, tiffBitsPerSample);
    assert(bitsPerSampleEntry->count == samplesPerPixel);
    assert(bitsPerSampleEntry->type == tiffShort);
    uint16_t * bitsPerSample = (uint16_t *)tiffIfdValue(&file, bitsPerSampleEntry);
    for (int i=1; i<bitsPerSampleEntry->count; i++) {
        assert(bitsPerSample[i] == bitsPerSample[0]);
    }

    size_t bytes = (bitsPerSample[0] / 8) * samplesPerPixel * imageWidth * imageHeight;
    char * image = (char *)malloc(bytes);
    memcpy(image, data + stripOffsets, size);

    if (width) *width = imageWidth;
    if (height) *height = imageHeight;
    if (nchannels) *nchannels = samplesPerPixel;
    if (bytesperchannel) *bytesperchannel = bitsPerSample[0] / 8;
    if (bottomfirst) *bottomfirst = orientation == tiffLowerLeft;
    if (isfloat) *isfloat = sampleFormat == tiffIEEEFloat;
    if (hasalpha) *hasalpha = (samplesPerPixel > 3) && (extraSamples == tiffUnassociatedAlpha);

    free(data);
    return image;
}
#endif
