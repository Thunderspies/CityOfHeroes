// textparser.h - provides text and token processing functions
// NOTE - the textparser can NOT be used from a second thread.
// Please let Mark know if this needs to be changed

#ifndef __TEXTPARSER_H
#define __TEXTPARSER_H

#include <io.h>
#include <stddef.h>        // needed for offsetof
#include <stdlib.h>        
#include "../stdtypes.h"
#include "memcheck.h"
#include "textparser_types.h"
#include "structDefines.h" // TODO - remove later
#include "structTokenizer.h" // TODO - remove later
#include "textparserUtils.h" // TODO - remove later
#include <utilitieslib/utils/structoldnames.h> // backwards compatibility

C_DECLARATIONS_BEGIN

typedef struct _SimpleBufHandle *SimpleBufHandle;
typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct SharedMemoryHandle SharedMemoryHandle;
typedef void* TokenizerHandle;
typedef struct DefineContext DefineContext;
typedef struct FileEntry FileEntry;
typedef FileEntry** FileList;

#define PARSE_SIG                "Parse6"    // should stay at 6 characters
#define MAX_LINK_LEN            128            // increase as necessary, just for internal buffers
#define PRIVATE_PARSER_HEAPS    0

/////////////////////////////////////////////////////////// Parser
// The Parser deals with I/O to a variety of formats.  It parses and loads 
// streams into a hierarchical set of structures.  The syntax of the text file
// must closely match the layout of memory.


// TOK_XXX Defines - Use these defines for almost all fields, you can add other options like 
//        TOK_STRUCTPARAM by prefixing the option and OR'ing with the TOK_XXX type
//        These defines help reliability by producing a compile error if the corresponding field 
//        is not the correct size.  They also make the semantics of compound array types like FIXEDSTR
//        clear.
// s - parent structure
// m - member field
// d - default value
// tpi - pointer to subtable ParseTable (for STRUCT)
// dict - pointer to dictionary (for LINK)
// size - size of field (for RAW)
// c - offset to int member field that has memory allocation size (for POINTER & USEDFIELD)

// primitives - all are compatible with a StaticDefine table used in the subtable field
#define TOK_BOOL(s,m,d)            TOK_BOOL_X, (SIZEOF2(s,m)==sizeof(U8))? offsetof(s,m):(5,5,5,5,5,5,5), d
#define TOK_U8(s,m,d)            TOK_U8_X, (SIZEOF2(s,m)==sizeof(U8))? offsetof(s,m):(5,5,5,5,5,5,5), d
#define TOK_INT16(s,m,d)        TOK_INT16_X, (SIZEOF2(s,m)==sizeof(S16))? offsetof(s,m):(5,5,5,5,5,5,5), d
#define TOK_INT(s,m,d)            TOK_INT_X, (SIZEOF2(s,m)==sizeof(int))? offsetof(s,m):(5,5,5,5,5,5,5), d
#define TOK_INT64(s,m,d)        TOK_INT64_X, (SIZEOF2(s,m)==sizeof(S64))? offsetof(s,m):(5,5,5,5,5,5,5), d
#define TOK_F32(s,m,d)            TOK_F32_X, (SIZEOF2(s,m)==sizeof(F32)&&((F32)d==(F32)(intptr_t)d))? offsetof(s,m):(5,5,5,5,5,5,5), d
#define TOK_DEGREES(s,m,d)        TOK_DEGREES_X, (SIZEOF2(s,m)==sizeof(F32)&&((F32)d==(F32)(intptr_t)d))? offsetof(s,m):(5,5,5,5,5,5,5), d
#define TOK_STRING(s,m,d)        TOK_INDIRECT | TOK_STRING_X, (SIZEOF2_DEREF(s,m)==sizeof(char))? offsetof(s,m):(5,5,5,5,5,5,5), (intptr_t)d
#define TOK_RAW(s,m)            TOK_RAW_X, offsetof(s,m), SIZEOF2(s,m)
#define TOK_RAW_S(s,m,size)        TOK_RAW_X, offsetof(s,m), (size)
#define TOK_POINTER(s,m,c)        TOK_INDIRECT | TOK_POINTER_X, (SIZEOF2(s,m)==sizeof(void*))? offsetof(s,m):(5,5,5,5,5,5,5), offsetof(s,c)

#define TOK_AUTOINT(s, m, d)    ((SIZEOF2(s,m)==sizeof(U8)) ? TOK_U8_X : ((SIZEOF2(s,m)==sizeof(U16)) ? TOK_INT16_X : ((SIZEOF2(s,m)==sizeof(U32)) ? TOK_INT_X : ((SIZEOF2(s,m)==sizeof(U64)) ? TOK_INT64_X : (5,5,5,5,5,5,5))))), offsetof(s,m), d
#define TOK_AUTOINTEARRAY(s, m)    TOK_EARRAY | ((SIZEOF2(s,m[0])==sizeof(U8)) ? TOK_U8_X : ((SIZEOF2(s,m[0])==sizeof(U16)) ? TOK_INT16_X : ((SIZEOF2(s,m[0])==sizeof(U32)) ? TOK_INT_X : ((SIZEOF2(s,m[0])==sizeof(U64)) ? TOK_INT64_X : (5,5,5,5,5,5,5))))), offsetof(s,m), 0

#define TOK_AUTOINTARRAY(s, m, d)    ((SIZEOF2(s,m[0])==sizeof(U8)) ? TOK_U8_X : ((SIZEOF2(s,m[0])==sizeof(U16)) ? TOK_INT16_X : ((SIZEOF2(s,m[0])==sizeof(U32)) ? TOK_INT_X : ((SIZEOF2(s,m[0])==sizeof(U64)) ? TOK_INT64_X : (5,5,5,5,5,5,5))))), offsetof(s,m), d

// built-ins - closely related a primitive, but serialize or parse differently
#define TOK_CURRENTFILE(s,m)    TOK_INDIRECT | TOK_CURRENTFILE_X, (SIZEOF2_DEREF(s,m)==sizeof(char))? offsetof(s,m):(5,5,5,5,5,5,5), 0
#define TOK_TIMESTAMP(s,m)        TOK_TIMESTAMP_X, (SIZEOF2(s,m)==sizeof(int))? offsetof(s,m):(5,5,5,5,5,5,5), 0
#define TOK_LINENUM(s,m)        TOK_LINENUM_X, (SIZEOF2(s,m)==sizeof(int))? offsetof(s,m):(5,5,5,5,5,5,5), 0
#define TOK_USEDFIELD(s,m,c)    TOK_INDIRECT | TOK_USEDFIELD_X, (SIZEOF2(s,m)==sizeof(void*))? offsetof(s,m):(5,5,5,5,5,5,5), offsetof(s,c)
#define TOK_FLAGS(s,m,d)        TOK_FLAGS_X, (SIZEOF2(s,m)==sizeof(int))? offsetof(s,m):(5,5,5,5,5,5,5), d
#define TOK_FLAGARRAY(s,m,size) TOK_FIXED_ARRAY | TOK_FLAGARRAY_X, (SIZEOF2(s,m)==sizeof(int)*(size))? offsetof(s,m):(5,5,5,5,5,5,5), size
#define TOK_BOOLFLAG(s,m,d)        TOK_BOOLFLAG_X, (SIZEOF2(s,m)==sizeof(U8))? offsetof(s,m):(5,5,5,5,5,5,5), d
#define TOK_QUATPYR(s,m)        TOK_FIXED_ARRAY | TOK_QUATPYR_X, (SIZEOF2(s,m)==sizeof(F32)*4)? offsetof(s,m):(5,5,5,5,5,5,5), 4
#define TOK_CONDRGB(s,m)        TOK_FIXED_ARRAY | TOK_CONDRGB_X, (SIZEOF2(s,m)==sizeof(U8)*4)? offsetof(s,m):(5,5,5,5,5,5,5), 4
#define TOK_MATPYR(s,m)            TOK_FIXED_ARRAY | TOK_MATPYR_X, (SIZEOF2(s,m)==sizeof(F32)*12)? offsetof(s,m):(5,5,5,5,5,5,5), 12
#define TOK_FILENAME(s,m,d)        TOK_INDIRECT | TOK_FILENAME_X, (SIZEOF2_DEREF(s,m)==sizeof(char))? offsetof(s,m):(5,5,5,5,5,5,5), (intptr_t)d

// complex types
#define TOK_LINK(s,m,d,dict)    TOK_LINK_X | TOK_INDIRECT, (SIZEOF2(s,m)==sizeof(void*))? offsetof(s,m):(5,5,5,5,5,5,5), (intptr_t)d, dict
#define TOK_REFERENCE(s,m,d,dictname) TOK_REFERENCE_X | TOK_INDIRECT, (SIZEOF2(s,m)==sizeof(void*))? offsetof(s,m):(5,5,5,5,5,5,5), (intptr_t)d, (void*)dictname
#define TOK_FUNCTIONCALL(s,m)    TOK_INDIRECT | TOK_EARRAY | TOK_FUNCTIONCALL_X, (SIZEOF2(s,m)==sizeof(void*))? offsetof(s,m):(5,5,5,5,5,5,5), 0
#define TOK_UNPARSED(s,m)        TOK_INDIRECT | TOK_EARRAY | TOK_UNPARSED_X, (SIZEOF2(s,m)==sizeof(void*))? offsetof(s,m):(5,5,5,5,5,5,5), 0
#define TOK_STRUCT(s,m,tpi)        TOK_INDIRECT | TOK_EARRAY | TOK_STRUCT_X, (SIZEOF2(s,m)==sizeof(void*))? offsetof(s,m):(5,5,5,5,5,5,5), SIZEOF2_DEREF2(s,m), tpi
#define TOK_STASHTABLE(s,m)        TOK_INDIRECT | TOK_STASHTABLE_X, (SIZEOF2(s,m)==sizeof(StashTable))? offsetof(s,m):(5,5,5,5,5,5,5), 0

// predefined array types
#define TOK_RG(s,m)                TOK_FIXED_ARRAY | TOK_U8_X, (SIZEOF2(s,m)==sizeof(U8)*2)? offsetof(s,m):(5,5,5,5,5,5,5), 2
#define TOK_RGB(s,m)            TOK_FIXED_ARRAY | TOK_U8_X, (SIZEOF2(s,m)==sizeof(U8)*3)? offsetof(s,m):(5,5,5,5,5,5,5), 3
#define TOK_RGBA(s,m)            TOK_FIXED_ARRAY | TOK_U8_X, (SIZEOF2(s,m)==sizeof(U8)*4)? offsetof(s,m):(5,5,5,5,5,5,5), 4
#define TOK_VEC2(s,m)            TOK_FIXED_ARRAY | TOK_F32_X, (SIZEOF2(s,m)==sizeof(F32)*2)? offsetof(s,m):(5,5,5,5,5,5,5), 2
#define TOK_VEC3(s,m)            TOK_FIXED_ARRAY | TOK_F32_X, (SIZEOF2(s,m)==sizeof(F32)*3)? offsetof(s,m):(5,5,5,5,5,5,5), 3
#define TOK_VEC4(s,m)            TOK_FIXED_ARRAY | TOK_F32_X, (SIZEOF2(s,m)==sizeof(F32)*4)? offsetof(s,m):(5,5,5,5,5,5,5), 4
#define TOK_PYRDEGREES(s,m)        TOK_FIXED_ARRAY | TOK_DEGREES_X, (SIZEOF2(s,m)==sizeof(F32)*3)? offsetof(s,m):(5,5,5,5,5,5,5), 3
#define TOK_FIXEDSTR(s,m)        TOK_STRING_X, (SIZEOF2_DEREF(s,m)==sizeof(char))? offsetof(s,m):(5,5,5,5,5,5,5), SIZEOF2(s,m)
#define TOK_FIXEDCHAR(s,m)        TOK_CHAR_X, (SIZEOF2_DEREF(s,m)==sizeof(char))? offsetof(s,m):(5,5,5,5,5,5,5), SIZEOF2(s,m)
#define TOK_INTARRAY(s,m)        TOK_EARRAY | TOK_INT_X, (SIZEOF2_DEREF(s,m)==sizeof(int))? offsetof(s,m):(5,5,5,5,5,5,5), 0
#define TOK_F32ARRAY(s,m)        TOK_EARRAY | TOK_F32_X, (SIZEOF2_DEREF(s,m)==sizeof(F32))? offsetof(s,m):(5,5,5,5,5,5,5), 0
#define TOK_STRINGARRAY(s,m)    TOK_INDIRECT | TOK_EARRAY | TOK_STRING_X, (SIZEOF2_DEREF(s,m)==sizeof(char*))? offsetof(s,m):(5,5,5,5,5,5,5), 0
#define TOK_LINKARRAY(s,m,dict)    TOK_EARRAY | TOK_INDIRECT | TOK_LINK_X, (SIZEOF2_DEREF(s,m)==sizeof(void*))? offsetof(s,m):(5,5,5,5,5,5,5), 0, dict
//#define TOK_REFARRAY(s,m,dictname)    TOK_EARRAY | TOK_INDIRECT | TOK_REFERENCE_X, (SIZEOF2_DEREF(s,m)==sizeof(void*))? offsetof(s,m):(5,5,5,5,5,5,5), 0, (void*)dictname
#define TOK_EMBEDDEDSTRUCT(s,m,tpi)    TOK_STRUCT_X, offsetof(s,m), SIZEOF2(s,m), tpi
#define TOK_NULLSTRUCT(tpi)        TOK_STRUCT_X, 0, 0, tpi
#define TOK_OPTIONALSTRUCT(s,m,tpi)    TOK_INDIRECT | TOK_STRUCT_X, (SIZEOF2(s,m)==sizeof(void*))? offsetof(s,m):(5,5,5,5,5,5,5), SIZEOF2_DEREF(s,m), tpi
#define TOK_POINTER(s,m,c)        TOK_INDIRECT | TOK_POINTER_X, (SIZEOF2(s,m)==sizeof(void*))? offsetof(s,m):(5,5,5,5,5,5,5), offsetof(s,c)
// token types
#define TOK_TYPE_MASK        ((1 << 8)-1)
typedef enum
{
    TOK_IGNORE,            // do nothing with this token, ignores remainder of line during parse
    TOK_START,            // not required, but used as the open brace for a structure
    TOK_END,            // terminate the structure described by the parse table

    // primitives - param=default value
    TOK_U8_X,            // U8 (unsigned char)
    TOK_INT16_X,        // 16 bit integer
    TOK_INT_X,            // int
    TOK_INT64_X,        // 64 bit integer
    TOK_F32_X,            // F32 (float), can be initialized with <param> but you only get an integer value
    TOK_DEGREES_X,        // As F32. Radians in binary but degrees in text. DefineLists in radians.
    TOK_STRING_X,        // char*
    TOK_CHAR_X,            // non-null terminated char*
    TOK_RAW_X,            // specify size as parameter, this many bytes will be written to binary as is        
    TOK_POINTER_X,        // void*, param is offset to size in bytes

    // built-ins
    TOK_CURRENTFILE_X,    // stored as char*, filled with filename of currently parsed text file
    TOK_TIMESTAMP_X,    // stored as int, filled with fileLastChanged() of currently parsed text file
    TOK_LINENUM_X,        // stored as int, filled with line number of the currently parsed text file
    TOK_USEDFIELD_X,    // stored as pointer, allocated by textparser, any fields given in text file will set corresponding bits
    TOK_BOOL_X,            // stored as u8, restricted to 0 or 1
    TOK_FLAGS_X,        // unsigned int, list of integers as parameter, result is the values OR'd together (0, 129, 5 => 133), can't have default value
    TOK_FLAGARRAY_X,    // int[], list of integers as parameter, result is turning on the bits in the int array for the values (0, 1, 32, 34, 63 => { 0x00000003, 0x80000005 }), can't have default value
    TOK_BOOLFLAG_X,        // int, no parameters in script file, if token exists, field is set to 1
    TOK_QUATPYR_X,        // F32[4], quaternion, read in as a pyr
    TOK_MATPYR_X,        // F32[3][3] in memory turns into F32[3] (PYR) when serialized
    TOK_CONDRGB_X,        // U8[4], conditional RGB. Alpha is 255 if the parameter is parsed, 0 otherwise.
    TOK_FILENAME_X,        // same as string, passed through forwardslashes & _strupr

    // complex types
    TOK_LINK_X,            // link to another object in a different parse file
    TOK_REFERENCE_X,    // YourStruct*, subtable is dictionary name
    TOK_FUNCTIONCALL_X,    // StructFunctionCall**, parenthesis in input signals hierarchal organization
    TOK_UNPARSED_X,        // StructParams**, getting almost unparsed tokens
    TOK_STRUCT_X,        // YourStruct**, pass size as parameter, use eaSize to get number of items
    TOK_STASHTABLE_X,    // StashTable
    TOK_BIT,            // A bitfield... only generated by AUTOSTRUCT
    TOK_DEPRECATED,        // A field that is no longer used. TODO: won't work on sub structs, but that might be nice one day

    NUM_TOK_TYPE_TOKENS,
} StructTokenType;
STATIC_ASSERT(NUM_TOK_TYPE_TOKENS-1 <= TOK_TYPE_MASK);
// YOU MUST ADD AN ENTRY TO g_tokentable IF ADDING A TOKEN TYPE
#define TOK_GET_TYPE(type)    ((type) & TOK_TYPE_MASK)

// options for parsing
#define TOK_OPTION_MASK        (((1 << 24)-1) & ~((1 << 8)-1))
#define TOK_REDUNDANTNAME    (1 << 8)
#define TOK_STRUCTPARAM        (1 << 9)

#define TOK_PRIMARY_KEY        (1 << 10) // persist layer primary key
#define TOK_AUTO_INCREMENT    (1 << 11) // autoincrementing (use with PRIMARY_KEY)

#define TOK_USEROPTIONBIT_1    (1 << 12)
#define TOK_USEROPTIONBIT_2    (1 << 13)

#define TOK_VOLATILE_REF    (1 << 14) //this field is a volatile reference
#define TOK_NO_TRANSLATE    (1 << 15) // when displaying this field, don't translate
#define TOK_SERVER_ONLY        (1 << 16) // Ignore this field on the client
#define TOK_CLIENT_ONLY     (1 << 17) // Ignore this field on the server
#define TOK_PERSIST            (1 << 18) // Save this to the database
#define TOK_NO_TRANSACT        (1 << 19) // Never transact on this field, even if it is persisted

// Storage types
// tokens can be single (no flag), fixed arrays, or earrays
#define TOK_EARRAY            (1 << 20)
#define TOK_FIXED_ARRAY        (1 << 21)
// tokens can be direct (no flag), or indirect (reached through a pointer)
#define TOK_INDIRECT        (1 << 22)
#define TOK_POOL_STRING        (1 << 23)

// Misc Bits
#define TOK_PARSETABLE_INFO  (1 << 24) // this TPI line, which is TOK_IGNORE, is actually info about the entire struct (must be line 0 if present)

// YOU MUST ADD AN ENTRY TO ParseTypeOptions IF ADDING A FLAG

// token precision field - can mean different things to different types of tokens
#define TOK_PRECISION_MASK    (~0 & ~((1 << 24)-1))
#define TOK_PRECISION(x)    ((x > 0 && x <= 255)? (x << 24): (5,5,5,5,5,5,5))
#define TOK_GET_PRECISION(type)    (((type) & TOK_PRECISION_MASK) >> 24)
#define TOK_GET_PRECISION_DEF(type, def) (TOK_GET_PRECISION(type)? TOK_GET_PRECISION(type): def)

// min bits are used for most integer fields
#define TOK_MINBITS(x)        TOK_PRECISION(x)
#define TOK_GET_MINBITS(x)    TOK_GET_PRECISION(x)
#define TOK_GET_MINBITS_DEF(x, def) TOK_GET_PRECISION_DEF(x, def)

// right now float accuracy as defined below is the only precision type for F32's
// - this may be extended later to different types of network strategies
#define TOK_FLOAT_ROUNDING(x) TOK_PRECISION(x)
#define TOK_GET_FLOAT_ROUNDING(type) TOK_GET_PRECISION(type)

typedef enum {
    FLOAT_HUNDRETHS = 1,
    FLOAT_TENTHS,
    FLOAT_ONES,
    FLOAT_FIVES,
    FLOAT_TENS,
} FloatAccuracy;


// options for different parsing & printing formats
#define TOK_FORMAT_OPTIONS_MASK    ((1 << 8)-1)
typedef enum
{
    TOK_FORMAT_IP = 1,            // int, parse & print as an IP address
    TOK_FORMAT_KBYTES,            // int, print as KB/MB/etc
    TOK_FORMAT_FRIENDLYDATE,    // int, print as a friendly date (e.g. "Yesterday, 12:00pm")
    TOK_FORMAT_FRIENDLYSS2000,    // int, print a seconds since 2000 count as a friendly date (e.g. "Yesterday, 12:00pm")
    TOK_FORMAT_FRIENDLYCPU,        // int, print timerCpuSeconds() as a friendly time (e.g. "Yesterday, 12:00pm")
    TOK_FORMAT_PERCENT,            // ints or floats, display as a percent
    TOK_FORMAT_NOPATH,            // string, strip off any path before displaying
    TOK_FORMAT_HSV,                // Vec3s, not actually for formatting, but for interpolation
    TOK_FORMAT_OBJECTID,        // int, this field is the unique identifier for an object
    TOK_FORMAT_KEY,                // int or string, this field is a key field, and is used for indexing
    TOK_FORMAT_TEXTURE,            // This field represents a texture
    TOK_FORMAT_UNSIGNED,        // int, this field should be printed unsigned
    TOK_FORMAT_DATESS2000,        // 11-07-2006 14:04:40 type format from seconds since 2k
    TOK_FORMAT_BYTES,            // int, print as bytes/KB/MB/etc
    TOK_FORMAT_MICROSECONDS,    // int, print as us/ms/s/min/etc
    TOK_FORMAT_MBYTES,            // int, print as MB/GB/TB/etc

    TOK_FORMAT_OPTIONS
} StructFormatOptions;
STATIC_ASSERT(TOK_FORMAT_OPTIONS-1 <= TOK_FORMAT_OPTIONS_MASK);
#define TOK_GET_FORMAT_OPTIONS(format) ((format) & TOK_FORMAT_OPTIONS_MASK)

// listview width field
#define TOK_FORMAT_LVWIDTH_MASK    (((1 << 16)-1) & ~((1 << 8)-1))
#define TOK_FORMAT_LVWIDTH(x)    ((x > 0 && x <= 255)? (x << 8): (5,5,5,5,5,5,5))
#define TOK_FORMAT_GET_LVWIDTH(format)    (((format) & TOK_FORMAT_LVWIDTH_MASK) >> 8)
#define TOK_FORMAT_GET_LVWIDTH_DEF(format, def)    (TOK_FORMAT_GET_LVWIDTH(format)? TOK_FORMAT_GET_LVWIDTH(format): def)

// ui options field
#define TOK_FORMAT_UI_LEFT        (1 << 16)    // justify left
#define TOK_FORMAT_UI_RIGHT        (1 << 17)    // justify right
#define TOK_FORMAT_UI_RESIZABLE    (1 << 18)    // can be resized
#define TOK_FORMAT_UI_NOTRANSLATE_HEADER    (1 << 19)    // the name of this field should not be translated
#define TOK_FORMAT_UI_NOHEADER    (1 << 20)    // don't show a header
#define TOK_FORMAT_UI_NODISPLAY    (1 << 21)    // don't make a column for this field

typedef struct StructParams
{
    char** params;        // a list of remaining parameters on the struct line, use eaSize to get number of items
} StructParams;

typedef struct StructFunctionCall
{
    char* function;
    struct StructFunctionCall** params;
} StructFunctionCall;

typedef struct StructLinkOffset
{
    int        name_offset;        // byte offset to where name str is found in current struct
    int        struct_offset;        // byte offset to where child struct is found in current struct. 0 means this is leaf node.
} StructLinkOffset;

typedef struct StructLink    // defines a link to another object in a different parse file
{
    void ** const* earray_addr;
    StructLinkOffset    links[4];
    struct StructLinkCache
    {
        StashTable hashPathFromPtr; // note: value param is from strdup.
        StashTable hashPtrFromPath;
    } cache;
} StructLink;

// use this to iterate through a parse table
#define FORALL_PARSETABLE(pti, i) for (i = 0; (pti[i].name && pti[i].name[0]) || pti[i].type; i++)


//////////////////////////////////////////////////// Struct memory handling
// All structured data must be alloced or dealloced with these functions.
// Memory is handled a bit differently than malloc


NN_PTR_MAKE void*    StructAlloc_dbg(ParseTable pti[], ParseTableInfo *optPtr, const char *file, int line);
#define StructAlloc(pti) StructAlloc_dbg(pti, NULL, __FILE__, __LINE__)
#define StructAllocIfNull(pti,s) if(!s) s = StructAlloc_dbg(pti, NULL MEM_DBG_PARMS_INIT)

void    StructDumpAllocs(void);
#define    StructAllocRaw(size) StructAllocRawDbg(size, __FILE__, __LINE__)
void*    StructAllocRawDbg(size_t size, const char *file, int line);        // allocate memory for a structure

// StructInit initializes the members of a structure safely. This is called by StructCreate
void    StructInitFields(ParseTable pti[], void* structptr); // as above but doesn't do initial memset to zero

// StructCreate allocates the memory for a structure, and initializes it.
// By default, use this by default to create textparser structures.
NN_PTR_MAKE void*   StructCreate_dbg(ParseTable pti[], const char *file, int line);
#define StructCreate(pti) StructCreate_dbg(pti, __FILE__, __LINE__)

void    StructFree(void* structptr);    // release memory for a structure
void    StructFreeFunctionCall(StructFunctionCall* callstruct);
void    StructClear(ParseTable pti[], void* structptr); // clear data members and destroy children
void    StructDestroy(ParseTable pti[], void* structptr);

#define StructAllocString(str)                StructAllocStringLenDbg(str, -1, __FILE__, __LINE__)
#define StructAllocStringLen(str, len)        StructAllocStringLenDbg(str, len, __FILE__, __LINE__)
char*    StructAllocStringLenDbg(const char* string, int len, const char *file, int line);    // strdup()-esque
#define StructReallocString(ppch, str)        StructReallocStringDbg(ppch, str, __FILE__, __LINE__)
void    StructReallocStringDbg(char **ppch, const char *string, const char *file, int line);
void    StructFreeString(char const* string);        // release memory for a string
void    StructFreeStringConst(const char* string);

/////////////////////////////////////////////////// Struct utils
// General functions for copying, comparing, crcing, etc. structs

void    StructInit(void *structptr, size_t size, ParseTable pti[]); // initialize data members from parse info
U32        StructCRC(ParseTable pti[], void* structptr); // get a CRC of the structure and children
int        StructCompare(ParseTable pti[], void *structptr1, void *structptr2); // Compares two structures
int        StructCopy(ParseTable pti[], const void* source, void* dest, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude);

////////////////////////////////////////////////// Per-token utils
int        TokenCompare(ParseTable tpi[], int column, void *structptr1, void *structptr2); // Compares a single element

////////////////////////////////////////////////// Unusual struct utils

typedef void* (*CustomMemoryAllocator)(void* data, size_t size);

// this function is a little unusual in that is does a straight mem-copy and will copy fields not specified in the
// ParseTable.  It still correctly does a deep copy of fields in the ParseTable, however.
int        StructCopyAll(ParseTable pti[], const void* source, size_t size, void* dest); // deep copy, allocating children, returns 0 on failure

// StructCopyFields copies from source to dest all fields that textparser knows about.
// If flags to match or exclude are specified, only the specified fields will be copied
// This is slower than CopyAll, but will not copy and non-textparser data.
int StructCopyFields(ParseTable pti[], const void* source, void* dest, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude);

// Returns the new structure, or NULL if copying failed (ran out of memory?).  Pass in NULL for pDestStruct in order to have the destination allocated automatically
void *    StructCompress(ParseTable pti[], const void* pSrcStruct, size_t iSize, void *pDestStruct, CustomMemoryAllocator memAllocator, void *customData); 

size_t StructGetMemoryUsage(ParseTable pti[], const void* pSrcStruct, size_t iSize); // Counts how much memory is used to store an entire structure

char *StructLinkToString(StructLink *struct_link,const void *ptr,char *link_str,size_t link_str_size);
void *StructLinkFromString(StructLink *struct_link,char const *link_str_p);
int StructLinkValidName(char *name);



///////////////////////////////////////////////////// Parser serialization
// Serializing to/from text, network, file, etc.

typedef int (*ParserTextCallback)(TokenizerHandle, const char*, void*);
int ParserErrorCallback(TokenizerHandle tok, const char* nexttoken, void* structptr);
    // sample callback that just prints an error and ignores token

// text I/O
int ParserReadTokenizer(TokenizerHandle tok, ParseTable pti[], void* structptr, ParserTextCallback callback);
int ParserWriteTextFile(const char* filename, ParseTable pti[], const void* structptr, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude);        // returns success
#define ParserReadTextFile(filename, pti, structptr) ParserLoadFiles(NULL, filename, NULL, 0, pti, structptr, NULL, NULL, NULL)
int ParserReadText(const char *str,int str_len,ParseTable *tpi,void *struct_mem);
int ParserReadTextEscaped(char **str, ParseTable *tpi, void *struct_mem);
int ParserWriteTextEscaped(char **estr, ParseTable *tpi, void *struct_mem, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude);
int ParserWriteText(char **estr,ParseTable *tpi,void *struct_mem, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude);

// binary I/O
int ParserReadBinaryFile(SimpleBufHandle binfile, char* filename, ParseTable pti[], void* structptr, FileList* filelist, DefineContext* defines);    // returns success
int ParserWriteBinaryFile(char* filename, ParseTable pti[], void* structptr, FileList* filelist, DefineContext* defines, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude); // returns success
int ParserReadBin(char *bin,U32 num_bytes,ParseTable *tpi,void *struct_mem);
int ParserWriteBin(char **estr,ParseTable *tpi,void *struct_mem, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude);
int ParserReadRegistry(const char *key, ParseTable pti[], void *structptr);
int ParserWriteRegistry(const char *key, ParseTable pti[], void *structptr, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude);


///////////////////////////////////////////////////// ParserLoadFiles
// ParserLoadFiles is used to hide binary and text file representations.
// The most current representation is automatically used.  ParserLoadFiles
// will scan through directories looking for all text files matching an extension
// to load at one time.  Changes in either the source text files or the
// data definitions (ParseTable) will be automatically detected and
// a new binary file written.

// use this to get info back from ParserLoadFiles
typedef struct ParserLoadInfo
{
    int updated;                        // set to true if .bin file was outdated and text files were read
} ParserLoadInfo;

// flags for ParserLoadFiles
#define PARSER_ERRFLAG (1)                // @todo -AB: just for testing, remove
#define PARSER_SERVERONLY    (1 << 2)    // create the .bin in the server only bin directory
#define PARSER_FORCEREBUILD (1 << 3)    // force a rebuild of .bin file
#define PARSER_EXACTFILE    (1 << 4)    // Do not auto-load v_ files, etc
#define PARSER_DONTFREE        (1 << 5)    // Do not free when copying to shared memory
#define PARSER_OPTIONALFLAG (1 << 6)    // It is not an error if no files are loaded
#define PARSER_EMPTYOK        (1 << 7)    // It is not an error if a file is empty
#define PARSER_SERVERSIDE    (1 << 8)    // We're on the server
#define PARSER_CLIENTSIDE    (1 << 9)    // We're on the client

// preprocessor function format for ParserLoadFiles
typedef bool (*ParserLoadPreProcessFunc)(ParseTable pti[], void* structptr);
typedef bool (*ParserLoadPostProcessFunc)(ParseTable pti[], void* structptr);
typedef bool (*ParserLoadFinalProcessFunc)(ParseTable pti[], void* structptr, bool shared_memory);
    // if this function returns 0 for failure, the .bin file will NOT be created

bool ParserLoadFiles(const char* dir, const char* filemask, const char* persistfile, int flags, ParseTable pti[], void* structptr, DefineContext* globaldefines, ParserLoadInfo* pli, ParserLoadPreProcessFunc preprocessor);
    // flags is a combination of PARSER_XXX flags
    // if dir: look for all files in dir and subdirs matching filemask
    // if !dir: just use single text file specified in filemask
    // okay to pass NULL for pli.  If pli is used, more data on status is returned
    // if a preprocessor function is used, it is called ONLY when creating a .bin file.  The .bin file
    // will be created with data already run through the preprocessor.
    // returns success
SimpleBufHandle ParserIsPersistNewer(const char* dir, const char* filemask, const char* persistfile, ParseTable pti[], DefineContext* defines); 
    // returns a handle if persist file newer than directory files and crc matches
    // caller is responsible for calling SerializeClose() on the SimpleBufHandle, or passing it to
    // ParserReadBinaryFile()
void ParserForceBinCreation(int set);    // if bin creating is forced, ParserLoadFiles will always create a .bin file

bool ParserLoadFilesShared(const char* sharedMemoryName, const char* dir, const char* filemask, const char* persistfile, int flags, ParseTable pti[], SHARED_MEMORY_PARAM void* sm_structptr, size_t iSize, DefineContext* globaldefines, ParserLoadInfo* pli, ParserLoadPreProcessFunc preprocessor, ParserLoadPostProcessFunc postprocessor, ParserLoadFinalProcessFunc finalprocessor);
    // Attempts to acquire the data from shared memory, and if not, then it loads it
    // This does not work if you want to do any processing on the data after it is loaded (i.e. set backpointers, etc)

bool ParserLoadFromShared(const char* sharedMemoryName, int flags, ParseTable pti[], SHARED_MEMORY_PARAM void* sm_structptr, size_t iSize, DefineContext* globaldefines, SharedMemoryHandle **shared_memory);
    // Attempts to load the data from shared memory. Returns 1 on success, 0 on failure
    // shared_memory is modified to point to the handle returned
bool ParserMoveToShared(SharedMemoryHandle *shared_memory, int flags, ParseTable pti[], SHARED_MEMORY_PARAM void* sm_structptr, size_t iSize, DefineContext* globaldefines);
    // Attempts to move the data to the shared memory handle that is passed in. We assume we have write access

void ParserCopyToShared(SharedMemoryHandle *shared_memory, SHARED_MEMORY_PARAM void* sm_structptr, size_t iSize);
    // Copies the data to the shared memory handle that is passed in. We assume we have write access

void ParserUnload(ParseTable pti[], SHARED_MEMORY_PARAM void* sm_structptr, size_t iSize);

typedef enum eParseReloadCallbackType
{
    eParseReloadCallbackType_Add,
    eParseReloadCallbackType_Delete,
    eParseReloadCallbackType_Update,
} eParseReloadCallbackType;

// ParserReloadCallback second parameter is a copy of the old structure if the structure was updated, NULL otherwise
typedef int (*ParserReloadCallback)(void* structptr, void* oldStructCopy, ParseTable *, eParseReloadCallbackType);
int ParserReloadFile(const char *relpath, ParseTable pti[], size_t sizeOfBaseStruct, void *pOldStruct, ParserReloadCallback subStructCallback, ParserLoadPreProcessFunc preprocessor);
    // Reloads a file and replaces the affected elements in the original structure.  See .c file for full comments.

void ParserSetForceCreateBins();
int ParserForceCreateBins();

//MW 
//Map loader needs this because it does it's own version of the parsing with its own list of files to include in the 
//filetable, but it still needs to be able to know what files were #included 
extern FileList g_parselist; // if non-zero, tokenizer will keep track of every file parsed

void TestTextParser(void);    // run an internal test of the textparser and print to console


void SimpleTokenStore( char * pchField, char *pchVal, TokenizerParseInfo *tpi, void * structptr, StaticDefineInt * pSDI );
void SimpleTokenSetSingle( char *pchVal, TokenizerParseInfo *tpi, int i, void * structptr, StaticDefineInt * pSDI );

const char *SimpleTokenGet( char * pchField, TokenizerParseInfo *tpi, void * structptr, StaticDefineInt * pSDI );
const char *SimpleTokenGetSingle( TokenizerParseInfo *tpi, int i, void * structptr, StaticDefineInt * pSDI );

C_DECLARATIONS_END

// Maintenance call to clear all StructLink caches
void TextParser_ClearCaches();

#endif // __TEXTPARSER_H
