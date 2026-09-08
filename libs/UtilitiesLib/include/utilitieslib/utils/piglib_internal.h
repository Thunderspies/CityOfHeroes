#include "../stdtypes.h"
#include "../components/datapool.h"

#define PIG_HEADER_FLAG 0x0123
#define PIG_FILE_HEADER_FLAG 0x3456
#define PIG_STRING_POOL_FLAG 0x6789
#define PIG_HEADER_DATA_POOL_FLAG 0x9ABC


typedef struct PigHeader { // Must be at least 16 bytes to maintain backwards compat
    U32 pig_header_flag; // identifies the start of a pig_header (used to make sure this is a pig file) == PIG_HEADER_FLAG
    U16 creator_version;
    U16 required_read_version;
    U16 archive_header_size; // Size of this struct
    U16 file_header_size; // Size of the PigFileHeader struct
    U32 num_files;
} PigHeader;

typedef struct PigFileHeader {
    U32 file_header_flag; // identifies the start of a file_header == 0x3456 (arbitrarily chosen)
    S32 name_id; // index into the string pool
    U32 size; // size of the file in bytes (max of 4g)
    U32 timestamp; // UTC __time32_t
    U32 offset; // offset into the pig file where this file's data is stored
    U32 reserved; // for later support of 64 bit offsets
    S32 header_data_id; // index into the header_data pool where this file's header info is stored
    U32    checksum[4]; // 128 bit checksum (MD5)
    U32 pack_size;
} PigFileHeader;

typedef struct PigFile {
    PigHeader header;
    PigFileHeader *file_headers;
    DataPool string_pool;
    DataPool header_data_pool;
    FILE *file;
    char *filename;
    HogFile *hog_file; // This pig is really a hog!
} PigFile;

PigFileHeader *PigFileFind(PigFile *handle, const char *relpath); // Find an entry in a Pig

// Internal function (called by hoglib)
int pigCmpEntryPathname(const void* aData, const void* bData);
U8 *pigGetHeaderData(NewPigEntry *entry, U32 *size);
U32 PigExtractBytesInternal(FILE *file, void *headerkey, int headerindex, void *buf, U32 pos, U32 size, U64 fileoffset, U32 filesize, U32 pack_size, bool random_access);
