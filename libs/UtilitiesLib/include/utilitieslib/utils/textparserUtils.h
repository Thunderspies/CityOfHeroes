
#ifndef TEXT_PARSER_UTILS_H
#define TEXT_PARSER_UTILS_H

#include "../stdtypes.h"

C_DECLARATIONS_BEGIN

typedef struct ParseTable ParseTable;
typedef struct Packet Packet;
typedef struct DefineContext DefineContext;

typedef enum DynOpType
{
    DynOpType_None = 0,
    DynOpType_Jitter,
    DynOpType_SphereJitter,
    DynOpType_SphereShellJitter,
    DynOpType_Inherit,
    DynOpType_Add,
    DynOpType_Multiply,
    DynOpType_Min,
    DynOpType_Max,
} DynOpType;

// for UI elements or anything else that wants a light-weight string from a token, doesn't support structs
// or other complex types.  If prettyprint is set, there is no guarantee that you can later execute TokenFromSimpleString on it
bool TokenToSimpleString(ParseTable *tpi, int column, void* structptr, char* str, int str_size, bool prettyprint);
bool TokenFromSimpleString(ParseTable *tpi, int column, void* structptr, char* str);

bool TokenIsSpecified(ParseTable *tpi, int column, void* srcStruct, int iBitFieldIndex);
void TokenCopy(ParseTable *tpi, int column, void *dstStruct, void *srcStruct);
void TokenClear(ParseTable *tpi, int column, void *dstStruct);
void TokenAddSubStruct(ParseTable *tpi, int column, void *dstStruct, void *srcStruct);
bool TokenIsNonZero(ParseTable *tpi, int column, void* srcStruct);

// nice way of implementing inheritance for structs, any fields specified in srcStruct will override those
// in dstStruct.  If addSubStructs is set, substructures get added to the end of the list instead of overridding.
void StructOverride(ParseTable *pti, void *dstStruct, void *srcStruct, int addSubStructs);

// Reversed StructOverride - more appropriate for a data-defined "default" struct.  Any fields not specified in dst are filled with default values.
// addSubStructs - substructs in defaultStruct will be ADDED to the list of structs in dest
// applySubStructFields - function will recurse so that individual fields in substructures will also use defaults, instead of only applying on a whole-struct level
void StructApplyDefaults(ParseTable *pti, void *dstStruct, void *defaultStruct, int addSubStructs, int applySubStructFields);

void TokenInterpolate(ParseTable *tpi, int column, void* structA, void* structB, void* destStruct, F32 interpParam);
void StructInterpolate(ParseTable *pti, void* structA, void* structB, void* destStruct, F32 interpParam);

void TokenCalcRate(ParseTable *tpi, int column, void* srcStoreA, void* srcStoreB, void* destStore, F32 deltaTime );
void StructCalcRate(ParseTable *pti, void* structA, void* structB, void* destStruct, F32 deltaTime );

void TokenIntegrate(ParseTable *tpi, int column, void* srcValue, void* srcRate, void* destStore, F32 deltaTime );
void StructIntegrate(ParseTable *pti, void* valueStruct, void* rateStruct, void* destStruct, F32 deltaTime );

void TokenCalcCyclic(ParseTable *tpi, int column, void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, F32 fStartTime, F32 deltaTime );
void StructCalcCyclic(ParseTable *pti, void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, F32 fStartTime, F32 deltaTime );

void TokenApplyDynOp(ParseTable *tpi, int column, DynOpType optype, F32* values, U8 uiValuesSpecd, void* dstStruct, void* srcStruct, U32* seed);
void StructApplyDynOp(ParseTable *pti, DynOpType optype, F32* values, U8 uiValuesSpecd, void* dstStruct, void* srcStruct, U32* seed);

///////////////////////////////////////////////////////////////////////////////////
// ParseTableXxx functions let you serialize and load entire parse tables - they automatically handle subtables
// and figuring out a correct memory layout for the loaded table

void ParseTableFree(ParseTable*** eapti);

bool ParseTableWriteTextFile(char* filename, ParseTable *pti);        // returns success
bool ParseTableWriteText(char** estr, ParseTable *pti);            // returns success
bool ParseTableReadTextFile(char* filename, ParseTable*** eapti, int* size); // returns an erray of parse tables and size of root struct
bool ParseTableSend(Packet* pak, ParseTable *pti);
bool ParseTableRecv(Packet* pak, ParseTable*** eapti, int* size);

// these are valid for both serialized and static parse tables
void ParseTableClearCachedInfo(ParseTable *pti);
int ParseTableCountFields(ParseTable *pti);
int ParseTableCRC(ParseTable *pti, DefineContext* defines);
int  ParseTableGetIndex(ParseTable *pti, char* tokenName);

///////////////////////////////////////////////////////////////////////////////////
// Textparser path functions allow for XPath-style addressing of textparser objects

// main function for resolving a textparser path - result is suitable for use with TokenStore functions
// - table_in and structptr_in are not required if you are referencing well-known root path (Entities, etc.)
bool ParserResolvePath(char* path_in, ParseTable *table_in, void* structptr_in,
                       ParseTable** table_out, int* column_out, void** structptr_out, int* index_out);

// do a key lookup on this struct and return the resulting index
// - structptr[column] is required to be an earray of structs
bool ParserResolveKey(char* key, ParseTable *table, int column, void* structptr, int* index);

// one or more root path providers can be registered to handle paths that begin with an identifier.
// providers should set *column to -1 if field is required next: (Mission.Success.Field)
// or to the correct field number if indexing is required next: (Entities[300].Field)
typedef bool (*RootPathLookupFunc)(char* name, ParseTable** table, void** structptr, int* column);
void ParserRegisterRootPathFunc(RootPathLookupFunc function);
bool ParserRootPathLookup(char* name, ParseTable** table, void** structptr, int* column); // just calls registered functions


///////////////////////////////////////////////////////////////////////////////////
// Meta information about textparser objects

// *************************************************************************
// In CoH there are these types of processing functions:
// preprocessor => FIXUPTYPE_POST_TEXT_READ
//   - passed to ParserLoadFiles
//   - called after text files were loaded, before bin files written
//
// postprocessor => FIXUPTYPE_POST_BIN_READ
//   - passed to ParserLoadFilesShared
//   - called after 'ParserLoadFiles' loads either bin or text files
//
// pointerpostprocessor
//   - passed to ParserLoadFilesShared
//   - called right after postprocessor, except the first time shared mem
//     is alloc'd and mem has to be relocated into shared memory.
//
// 06/26/08
// If all these enums need to be supported, something is wrong. I'm not sure
// any of them need to be supported
//
// What does it mean to have a fixup function? Do you call it when a struct
// within a struct within a struct is parsed? Or after all the structs are 
// parsed?
// 
// In CoH the fixup functions are for fixing up a class of data after it is
// all loaded. i.e. all "*.powers" files are loaded. The fixup is explicitly
// passed to the parser load function, and it is all visible.
// *************************************************************************
// typedef enum enumTextParserFixupType
// {
//     FIXUPTYPE_NONE,

//     //immediately after a text read has happened on the individual struct level
//     FIXUPTYPE_POST_TEXT_READ,

//     //immediately after a bin read has happened on the individual struct level
//     FIXUPTYPE_POST_BIN_READ,


//     //immediately before a text write happens on the individual struct level
//     FIXUPTYPE_PRE_TEXT_WRITE,

//     //immediately before a bin write happens on the individual struct level
//     FIXUPTYPE_PRE_BIN_WRITE,

//     //-----------------NOTE: All the fixups with "DURING_LOADFILES" in their names happen ONLY during 
//     //calls to variations of ParserLoadFiles, not during (for instance) ParserReadTextFile.

//     //during parserLoadFiles, after all text reading is complete, also after inheritance has been
//     //applied
//     FIXUPTYPE_POST_ALL_TEXT_READING_AND_INHERITANCE_DURING_LOADFILES,


//     //during parserloadfiles, after all loading has happened, BEFORE moving to shared memory (if happening)
//     // (used to be postProcess)
//     FIXUPTYPE_POST_BINNING_DURING_LOADFILES,

//     //during parserloadfiles, after all loading has happened, AFTER being put in its final location (ie, after
//     //being moved to shared memory, or after we decided not to move it to shared memory)
//     // (used to be pointerPostProcess)
//     FIXUPTYPE_POST_LOAD_DURING_LOADFILES_FINAL_LOCATION,

//     //when StructDeInit is called (which is called by StructDestroy). Note that this recurses root-node-first,
//     //so it will be called on the parent with still-intact children, then called on all children
//     FIXUPTYPE_DESTRUCTOR,

//     //when StructInit is called (which is called by Create)
//     FIXUPTYPE_CONSTRUCTOR,

//     //during a reload, after all reloading has occurred and the newly created objects have been put into their
//     //reference dictionary (if any)
//     FIXUPTYPE_POST_RELOAD,

//     //This object inherits, and the inheriting fields from its InheritanceData were just applied. Presumably, the
//     //whole object was also recopied from the Parent first, so any and all fixup stuff might need to happen
//     //
//     //Note that this is NOT called recursively. It's only called on the actual object which has inheritance data
//     FIXUPTYPE_POST_INHERITANCE_APPLICATION,
// } enumTextParserFixupType;
// typedef TextParserResult TextParserAutoFixupCB(void *pStruct, enumTextParserFixupType eFixupType);

typedef struct ParseTableInfo 
{
    // basic info about the table
    ParseTable* table;
    int size;
    char* name;
//     TextParserAutoFixupCB pFixupCB;

    // some cached information
    int numcolumns;
    int keycolumn;
    int idcolumn;

    // memory charges
    size_t sharedmem_committed;
    size_t heap_committed;
} ParseTableInfo;

//normally the table info will be inferred correctly, but you can call this function directly before
// other textparser functions in order to set the name and size you want
// ab:pFixupCB not supported currently, too lazy to take it out of structparser
// ----------------------------------------
// NOTE: this macro cheap preprocess trick to compile error if not passed a string.  compile error will say something like 'missing ) before string' use Ex version if you need to pass a var.
// ----------------------------------------
#define ParserSetTableInfoExplicitly(table, size, name, pFixupCB, pSourceFileName) \
ParserSetTableInfoExplicitlyEx(table, size, name, ((name "static assert is string"),TRUE), pSourceFileName)

// table info can be built into the tpi, or explicitly set via these methods
// Note: explicitly setting this will override the implicit tpi (is this a good idea?)
bool ParserSetTableInfoExplicitlyEx(ParseTable *table, int size, char* name, int name_static, char *source_file_name);
bool ParserClearExplicitTableInfo(ParseTable *table);

// get useful information about parse tables. default can be overridden with
// explicit info.
ParseTableInfo *ParserGetTableInfo(ParseTable *table);

// misc. helpers
// TextParserAutoFixupCB * ParserGetTableFixupFunc(ParseTable *table)

C_DECLARATIONS_END

#endif //TEXT_PARSER_UTILS_H
