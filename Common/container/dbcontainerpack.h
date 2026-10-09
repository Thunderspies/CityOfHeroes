#ifndef _DBCONTAINERPACK_H
#define _DBCONTAINERPACK_H

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/utils.h>
//#include <utilitieslib/utils/cmdoldparse.h>

#define SIZE_INT32 sizeof(int)
#define SIZE_INT16 sizeof(short)
#define SIZE_INT8 sizeof(char)
#define SIZE_FLOAT32 sizeof(float)
#define LINEDESCFLAG_INDEXEDCOLUMN (1 << 0)
#define LINEDESCFLAG_READONLY    (1 << 1)

// Max levels of indirection per line description
#define MAX_INDIRECTIONS    4
#define IGNORE_LINE_DESC    -1
#define MAX_CONTAINER_EARRAY_SIZE 9999

#define INDIRECTION(struct_name,field_name,is_ptr) {OFFSETOF(struct_name,field_name), is_ptr, #struct_name, #field_name}

#define OFFSET(struct_name,field_name) INDIRECTION(struct_name, field_name, 0)
#define OFFSET_PTR(struct_name,field_name) INDIRECTION(struct_name, field_name, 1)
#define OFFSET_IGNORE() /* leaves columns in db, does not read, writes NULL */ {IGNORE_LINE_DESC,0,"Unused","Unused"}

#define OFFSET2(struct_name,field_name,struct_name2,field_name2) {OFFSET(struct_name,field_name), OFFSET(struct_name2,field_name2)}
#define OFFSET2_PTR(struct_name,field_name,struct_name2,field_name2) {OFFSET_PTR(struct_name,field_name), OFFSET(struct_name2,field_name2)}

#define OFFSET3_PTR(struct_name,field_name,struct_name2,field_name2,struct_name3,field_name3) {OFFSET_PTR(struct_name,field_name), OFFSET(struct_name2,field_name2), OFFSET(struct_name3,field_name3) }
#define OFFSET3_PTR2(struct_name,field_name,struct_name2,field_name2,struct_name3,field_name3) {OFFSET_PTR(struct_name,field_name), OFFSET_PTR(struct_name2,field_name2), OFFSET(struct_name3,field_name3) }

#define OFFSET4_PTR3(struct_name,field_name,struct_name2,field_name2,struct_name3,field_name3,struct_name4,field_name4) {OFFSET_PTR(struct_name,field_name), OFFSET_PTR(struct_name2,field_name2), OFFSET_PTR(struct_name3,field_name3), OFFSET(struct_name4,field_name4) }

// conversion functions take string or int and return int or string
#define INOUT(in_from_db,out_to_db) in_from_db,out_to_db,0,0
typedef intptr_t IntFromStr(const char *str);
typedef const char *StrFromInt(intptr_t num);

// just used by trays
// conversions functions take ent*, struct *, and then string or int
#define INOUT_PTR(in_from_db,out_to_db) 0,0,in_from_db,out_to_db,
typedef intptr_t IntFromPtrAndStr(void *parent_ptr,void *struct_ptr,const char *str);
typedef const char *StrFromPtrAndInt(void *parent_ptr,void *struct_ptr,intptr_t num);

typedef struct{
    intptr_t offset;
    int isPointer;        // Perform indirection assuming the parent is a pointer?
    char* structName;    // For debug only.    What is the name of the parent structure this indirection operates on?
    char* fieldName;    // For debug only.  What is the name of the field this indirection is supposed to access?
} StructIndirection;

typedef enum ContainerPackType 
{
    PACKTYPE_INT = 1,
    PACKTYPE_FLOAT,
    PACKTYPE_ATTR, // String indexed as an integer in the database
    PACKTYPE_CONREF, // Integer reference to a ContainerId
    PACKTYPE_DATE,
    PACKTYPE_SUB,
    PACKTYPE_EARRAY,
    PACKTYPE_STR_UTF8, // Unicode string in a character array
    PACKTYPE_STR_ASCII, // Ascii string in a character array
    PACKTYPE_ESTRING_UTF8, // Unicode string in an EString
    PACKTYPE_ESTRING_ASCII, // Ascii string in an EString
    PACKTYPE_STR_UTF8_CACHED, // Unicode string that is permanently stored as a reference in the runtime with allocAddString'ed
    PACKTYPE_STR_ASCII_CACHED, // Ascii string that is permanently stored as a reference in the runtime with allocAddString'ed
    PACKTYPE_BIN_STR, // The size that follows this tag should be the # of bytes in the binary string array.

    // These types must come last when used in a table due to ODBC restrictions
    PACKTYPE_LARGE_ESTRING_BINARY, // Large binary data in an EString
    PACKTYPE_LARGE_ESTRING_UTF8, // Largey Unicode string in an EString
    PACKTYPE_LARGE_ESTRING_ASCII, // Large Ascii string in an EString

} ContainerPackType;

//---------------------------------------------------------------------------------
// "Line" description
//    Describes a single field in a structure and how to convert it to/from text.
//---------------------------------------------------------------------------------
typedef struct LineDesc
{
    struct 
    {
        ContainerPackType type;
        intptr_t size;
        char    *name;
        StructIndirection indirection[MAX_INDIRECTIONS];
        IntFromStr    *int_from_str_func;
        StrFromInt    *str_from_int_func;
        IntFromPtrAndStr    *int_from_ptr_and_str_func;
        StrFromPtrAndInt    *str_from_ptr_and_int_func;
        int        flags;
        int        dbsize;    // For TYPE_STR: Size of field in Database if not equal to 2*size
    };
    struct
    {
        char *desc;
    };
} LineDesc;

//---------------------------------------------------------------------------------
// Array description
//    Describes the type of array the structure being described is.
//---------------------------------------------------------------------------------
typedef enum{
    AT_NOT_ARRAY,
    AT_STRUCT_ARRAY,
    AT_POINTER_ARRAY,
    AT_EARRAY,
} ArrayType;

typedef struct{
    ArrayType type;                                        // What type of array is it?
    StructIndirection indirection[MAX_INDIRECTIONS];    // How do I get to the base of this array?
} ArrayDesc;

//---------------------------------------------------------------------------------
// Structure description
//    Describes a collection of fields in an structure to be converted to/from text.
//---------------------------------------------------------------------------------
typedef struct StructDesc {
    unsigned int structureSize;
    ArrayDesc arrayDesc;
    LineDesc* lineDescs;
    char *desc;
} StructDesc;


// Generated by mkproto
void* siApplyIndirection(unsigned char* parent, StructIndirection* indirection);
void* siApplyMultipleIndirections(unsigned char* parent, StructIndirection* indirections, int indirectionCount);
int decodeLine(char **buffptr,char *table,char *field,char **val_ptr,int *idx_ptr,char *idxstr);
void lineToStruct(char* mem, StructDesc* structDesc, char* table, char* field, char* val, int idx);
void structToLine(StuffBuff* sb, char* mem, char* table, StructDesc* structDesc, LineDesc *desc, int idx);
void structToLineTemplate(StuffBuff *sb,char *table,LineDesc *desc,int idx);
void structToLineSchema(StuffBuff *sb,char *table,LineDesc *desc,int idx);
void dbContainerUnpack(StructDesc *descs,char *buff,void *mem);
char *dbContainerPackage(StructDesc *descs,void *mem);
char *dbContainerTemplate(StructDesc *descs);
char *dbContainerSchema(StructDesc *descs, char *name);
// End mkproto
#endif
