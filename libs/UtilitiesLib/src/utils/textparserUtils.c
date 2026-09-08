
#include "utilitieslib/utils/textparserUtils.h"
#include "utilitieslib/components/StringCache.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/rand.h"
#include "utilitieslib/utils/Quat.h"
#include "utilitieslib/utils/structInternals.h"
#include "utilitieslib/utils/structTokenizer.h"
#include "utilitieslib/utils/tokenstore.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/structNet.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/components/MemoryPool.h"

#define PARSEINFO_ROOTNAME "root"


bool TokenToSimpleString(ParseTable tpi[], int column, void* structptr, char* str, int str_size, bool prettyprint)
{
    return TOKARRAY_INFO(tpi[column].type).tosimple(tpi, column, structptr, 0, str, str_size, prettyprint);
}

bool TokenFromSimpleString(ParseTable tpi[], int column, void* structptr, char* str)
{
    return TOKARRAY_INFO(tpi[column].type).fromsimple(tpi, column, structptr, 0, str);
}

void TokenCopy(ParseTable tpi[], int column, void *dstStruct, void *srcStruct)
{
    TOKARRAY_INFO(tpi[column].type).destroystruct(tpi, column, dstStruct, 0);
    TOKARRAY_INFO(tpi[column].type).copyfield(tpi, column, dstStruct, srcStruct, 0, NULL, NULL, 0, 0);
}

void TokenClear(ParseTable tpi[], int column, void *dstStruct)
{
    TOKARRAY_INFO(tpi[column].type).destroystruct(tpi, column, dstStruct, 0);
}

bool TokenIsNonZero(ParseTable tpi[], int column, void* srcStruct)
{
    // Old switch style, could be made nicer
    int iNumElems =    TokenStoreGetNumElems(tpi, column, srcStruct);
    int iIndex;
    switch ( TOK_GET_TYPE(tpi[column].type) )
    {
        xcase TOK_INT_X:
            for (iIndex=0; iIndex<iNumElems; ++iIndex)
                if ( !!TokenStoreGetInt(tpi, column,srcStruct,iIndex))
                    return true;
        case TOK_QUATPYR_X:
        xcase TOK_F32_X:
            for (iIndex=0; iIndex<iNumElems; ++iIndex)
                if ( !!TokenStoreGetF32(tpi, column,srcStruct,iIndex))
                    return true;
        xdefault:
            FatalErrorf("Need to add support for this token type in TokenIsNonZero");
    }
    return false;
}


// copy all substructs in srcStruct (in this column) to dstStruct
void TokenAddSubStruct(ParseTable tpi[], int column, void *dstStruct, void *srcStruct)
{
    int i, numelems = TokenStoreGetNumElems(tpi, column, (void*)srcStruct);

    assert(TOK_GET_TYPE(tpi[column].type) == TOK_STRUCT_X);
    for (i = 0; i < numelems; i++)
    {
        void* fromstruct = TokenStoreGetPointer(tpi, column, (void*)srcStruct, i);
        void* tostruct = TokenStoreAlloc(tpi, column, (void*)dstStruct, -1, tpi[column].param, NULL, NULL);
        StructCompress(tpi[column].subtable, fromstruct, tpi[column].param, tostruct, NULL, NULL);
    }
}

// Uses the TPI to copy values from a source struct to a target struct, only copying
// non-defaults, so that you can override a parsed structure with another (to improve reuse)
// If addsubstructs is set, full substructures in the srcStruct may be added to the list of 
// substructs in dstStruct
void StructOverride(ParseTable pti[], void *dstStruct, void *srcStruct, int addSubStructs)
{
    int i, foundfield = 0;
    U32* usedBitField = NULL;

    // First, make sure they have the parambitfield token
    FORALL_PARSEINFO(pti, i)
    {
        if (TOK_GET_TYPE(pti[i].type) == TOK_USEDFIELD_X )
        {
            usedBitField = TokenStoreGetPointer(pti, i, srcStruct, 0);
            foundfield = 1;
            break;
        }
    }

    if (!foundfield) // it may not be an error for usedBitField = NULL here (no tokens specified)
    {
        assertmsg(0, "Error: attempting to use StructOverride without a TOK_USEDFIELD bitfield");
        return;
    }

    FORALL_PARSEINFO(pti, i)
    {
        // earrays of substructs actually get ADDED to the target
        if (addSubStructs && TOK_GET_TYPE(pti[i].type) == TOK_STRUCT_X && pti[i].type & TOK_EARRAY)
        {
            TokenAddSubStruct(pti, i, dstStruct, srcStruct);
        }
        else if (TOK_GET_TYPE(pti[i].type) == TOK_USEDFIELD_X ||
            TOK_GET_TYPE(pti[i].type) == TOK_CURRENTFILE_X ||
            TOK_GET_TYPE(pti[i].type) == TOK_LINENUM_X ||
            TOK_GET_TYPE(pti[i].type) == TOK_TIMESTAMP_X)
        {
            // Do *not* copy the USEDFIELD bits, so that when we reload the
            //  default structure, we can re-apply it to this structure and
            //  get the same result as the initial application.
            // Also do not copy over auto-generated values (currentfile)!
        }
        else if ( usedBitField && TSTB(usedBitField, i) )
        {
            TokenCopy(pti, i, dstStruct, srcStruct);
        }
    }
}

// Any fields not specified in dstStruct will be overridden with defaultStruct
// Can use this to have a data-defined "default" and call this once per struct after load to fill in the defaults
// that were not overridden
// addSubStructs - substructs in defaultStruct will be ADDED to the list of structs in dest
// applySubStructFields - function will recurse so that individual fields in substructures will also use defaults, instead of only applying on a whole-struct level
void StructApplyDefaults(ParseTable pti[], void *dstStruct, void *defaultStruct, int addSubStructs, int applySubStructFields)
{
    int i, foundfield = 0;
    U32* usedBitField = NULL;

    // First, make sure they have the parambitfield token
    FORALL_PARSEINFO(pti, i)
    {
        if (TOK_GET_TYPE(pti[i].type) == TOK_USEDFIELD_X )
        {
            usedBitField = TokenStoreGetPointer(pti, i, dstStruct, 0);
            foundfield = 1;
            break;
        }
    }

    if (!foundfield) // it may not be an error for usedBitField = NULL here (no tokens specified)
    {
        assertmsg(0, "Error: attempting to use ParserReverseOverrideStruct without a TOK_USEDFIELD bitfield");
        return;
    }

    FORALL_PARSEINFO(pti, i)
    {
        // earrays of substructs actually get ADDED to the target
        if (addSubStructs && TOK_GET_TYPE(pti[i].type) == TOK_STRUCT_X && pti[i].type & TOK_EARRAY)
        {
            TokenAddSubStruct(pti, i, dstStruct, defaultStruct);
        }
        else if (TOK_GET_TYPE(pti[i].type) == TOK_USEDFIELD_X ||
            TOK_GET_TYPE(pti[i].type) == TOK_CURRENTFILE_X ||
            TOK_GET_TYPE(pti[i].type) == TOK_LINENUM_X ||
            TOK_GET_TYPE(pti[i].type) == TOK_TIMESTAMP_X)
        {
            // Do *not* copy the USEDFIELD bits, so that when we reload the
            //  default structure, we can re-apply it to this structure and
            //  get the same result as the initial application.
            // Also do not copy over auto-generated values (currentfile)!
        }
        else if (usedBitField && !TSTB(usedBitField, i))
        {
            TokenCopy(pti, i, dstStruct, defaultStruct);
        }
        else if (TOK_GET_TYPE(pti[i].type) == TOK_STRUCT_X && applySubStructFields) // past copy, so this structure exists in destination
        {
            int defaultNumElems = TokenStoreGetNumElems(pti, i, defaultStruct);
            int destNumElems = TokenStoreGetNumElems(pti, i, dstStruct);
            int j;
            assert(destNumElems); // If this is zero, how was the bitfield set that it was used?!
            if (defaultNumElems) {
                void *defaultSubStruct = TokenStoreGetPointer(pti, i, defaultStruct, 0);
                if (defaultNumElems!=1) {
                    Errorf("Found default with more than one \"%s\" specified, this will be ignored.", pti[i].name);
                }
                for (j=0; j<destNumElems; j++) {
                    void* dstSubStruct = TokenStoreGetPointer(pti, i, dstStruct, j);
                    StructApplyDefaults(pti[i].subtable, dstSubStruct, defaultSubStruct, addSubStructs, applySubStructFields);
                }
            } 
            //else {
            //    Errorf("Default struct does not have a value for \"%s\"", pti[i].name);
            //}
        }
    }
}


// iBitFieldIndex is the index of the TOK_USEDFIELD, to prevent a linear search every time you use this if you know it...
// if you don't care, pass -1 and it will be ignored
bool TokenIsSpecified(ParseTable tpi[], int column, void* srcStruct, int iBitFieldIndex)
{
    int i;
    U32* usedBitField = NULL;

    // First, make sure they have the parambitfield token
    if ( iBitFieldIndex >= 0 )
    {
        if ( TOK_GET_TYPE(tpi[iBitFieldIndex].type) != TOK_USEDFIELD_X )
        {
            Errorf("BitFieldIndex specified in TokenIsSpecified is incorrect.");
            return false;
        }
        else
        {
            usedBitField = TokenStoreGetPointer(tpi, iBitFieldIndex, srcStruct, 0);
            if ( !usedBitField ) // If the bitfield is null, no tokens specified
                return false;
        }
    }
    else
    {
        FORALL_PARSEINFO(tpi, i)
        {
            if ( TOK_GET_TYPE(tpi[column].type) == TOK_USEDFIELD_X )
            {
                usedBitField = TokenStoreGetPointer(tpi, column, srcStruct, 0);
                if ( !usedBitField ) // If the bitfield is null, no tokens specified
                    return false;
                break;
            }
        }
    }

    if (!usedBitField)
    {
        Errorf("Tried to find TOK_USEDFIELD in a ParseTable, but failed");
        return false;
    }

    return TSTB(usedBitField, column);
}

int ParseTableGetIndex(ParseTable pti[], char* tokenName)
{
    int i;
    FORALL_PARSEINFO(pti, i)
    {
        if ( pti[i].name && stricmp(pti[i].name, tokenName) == 0 )
        {
            return i;
        }
    }
    return -1;
}

void TokenInterpolate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, F32 interpParam)
{
    TOKARRAY_INFO(tpi[column].type).interp(tpi, column, structA, structB, destStruct, 0, interpParam);
}

void StructInterpolate(ParseTable pti[], void* structA, void* structB, void* destStruct, F32 interpParam)
{
    int i;
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) continue;
        TOKARRAY_INFO(pti[i].type).interp(pti, i, structA, structB, destStruct, 0, interpParam);
    }
}

void TokenCalcRate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, F32 deltaTime )
{
    TOKARRAY_INFO(tpi[column].type).calcrate(tpi, column, structA, structB, destStruct, 0, deltaTime);
}

void StructCalcRate(ParseTable pti[], void* structA, void* structB, void* destStruct, F32 deltaTime )
{
    int i;
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) continue;
        TOKARRAY_INFO(pti[i].type).calcrate(pti, i, structA, structB, destStruct, 0, deltaTime);
    }
}

void TokenIntegrate(ParseTable tpi[], int column, void* valueStruct, void* rateStruct, void* destStruct, F32 deltaTime )
{
    TOKARRAY_INFO(tpi[column].type).integrate(tpi, column, valueStruct, rateStruct, destStruct, 0, deltaTime);
}

void StructIntegrate(ParseTable pti[], void* valueStruct, void* rateStruct, void* destStruct, F32 deltaTime )
{
    int i;
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) continue;
        TOKARRAY_INFO(pti[i].type).integrate(pti, i, valueStruct, rateStruct, destStruct, 0, deltaTime);
    }
}

void TokenCalcCyclic(ParseTable tpi[], int column, void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, F32 fStartTime, F32 deltaTime )
{
    TOKARRAY_INFO(tpi[column].type).calccyclic(tpi, column, valueStruct, ampStruct, freqStruct, cycleStruct, destStruct, 0, fStartTime, deltaTime);
}

void StructCalcCyclic(ParseTable pti[], void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, F32 fStartTime, F32 deltaTime )
{
    int i;
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) continue;
        TOKARRAY_INFO(pti[i].type).calccyclic(pti, i, valueStruct, ampStruct, freqStruct, cycleStruct, destStruct, 0, fStartTime, deltaTime);
    }
}


void TokenApplyDynOp(ParseTable tpi[], int column, DynOpType optype, F32* values, U8 uiValuesSpecd, void* dstStruct, void* srcStruct, U32* seed)
{
    TOKARRAY_INFO(tpi[column].type).applydynop(tpi, column, dstStruct, srcStruct, 0, optype, values, uiValuesSpecd, seed);
}

void StructApplyDynOp(ParseTable pti[], DynOpType optype, F32* values, U8 uiValuesSpecd, void* dstStruct, void* srcStruct, U32* seed)
{
    int i;
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) continue;
        TOKARRAY_INFO(pti[i].type).applydynop(pti, i, dstStruct, srcStruct, 0, optype, values, uiValuesSpecd, seed);
    }
}

int CalcOffsetsSubtable(ParseTable pti[], StashTable subSizes)
{
    int i;
    int size = 0;
    size_t fullsize = 0;

    // deal with circular refs & common subrefs
    if (stashAddressFindInt(subSizes, pti, &size)) return size; // we've already done this subtable
    stashAddressAddInt(subSizes, pti, 0, 0); // zero for now, fixup later

    // MAK - deleting this weird strategy for redundantnames..  redundant names must be 
    // fixed up outside calcoffsets

    // now deal with all normal fields
    FORALL_PARSEINFO(pti, i)
    {
        // redundant structs may have different parse tables that still need to get evaluated
        if (TOK_GET_TYPE(pti[i].type) == TOK_STRUCT_X)
        {
            int storage = TokenStoreGetStorageType(pti[i].type);
            int subsize = CalcOffsetsSubtable(pti[i].subtable, subSizes);
            if (storage == TOK_STORAGE_DIRECT_SINGLE)
            {
                pti[i].param = subsize;
                if (!subsize)
                {
                    devassertmsg(0, "Unknown subtable size in an embedded struct because of recursion.  "
                        "This is possibly fixable with more passes, but you may just want to fix the parse definition.");
                }
            }
            else
                pti[i].param = 0;
        }
        if (pti[i].type & TOK_REDUNDANTNAME) continue;
        TOKARRAY_INFO(pti[i].type).calcoffset(pti, i, &fullsize);
    }
    size = (int)fullsize; // HACK - limiting to int size here because I want to use stash tables 
                            // - should be fine because 64-bit sizes wouldn't be cross-platform anyway
    stashAddressAddInt(subSizes, pti, size, 1); // now I finally know my size
    return size;
}

void FixupSubtableSizes(ParseTable pti[], StashTable subSizes)
{
    int i, subsize;

    FORALL_PARSEINFO(pti, i)
    {
        if (TOK_GET_TYPE(pti[i].type) == TOK_STRUCT_X && !pti[i].param) // don't recalc param - again for for recursion
        {
            assert(stashAddressFindInt(subSizes, pti[i].subtable, &subsize));
            pti[i].param = subsize;
            FixupSubtableSizes(pti[i].subtable, subSizes);
        }
    }
}

// does not fixup offsets for redundant names, you need another method for that
int ParseInfoCalcOffsets(ParseTable table[])
{
    StashTable    subSizes = stashTableCreateAddress(10);
    int size = CalcOffsetsSubtable(table, subSizes);
    FixupSubtableSizes(table, subSizes); // need to recurse again to deal with recursive tables
    stashTableDestroy(subSizes);
    return size;
}

ParseTable* ParseInfoAlloc(int numentries);
void ParseInfoFree(ParseTable* pti);

// Tokenizer tables get converted into data format (mostly using FUNCTIONCALL fields) and then serialized

// Used to indicate we have a static define table, but not a valid one
StaticDefineInt NullStaticDefineList[] =
{
    DEFINE_INT
    DEFINE_END
};


typedef struct ParseInfoColumn
{
    StructFunctionCall** options;
} ParseInfoColumn;

typedef struct ParseInfoTable
{
    char* name;
    ParseInfoColumn** columns;
} ParseInfoTable;

typedef struct ParseInfoDescriptor
{
    ParseInfoTable** tables;
    StashTable subNames; // not sent, just for convenience
    StashTable subAddresses; // ''
} ParseInfoDescriptor;

ParseTable tpiParseInfoColumn[] = 
{
    { "",    TOK_STRUCTPARAM | TOK_FUNCTIONCALL(ParseInfoColumn, options), },
    { "\n", TOK_END },
    { 0 },
};

ParseTable tpiParseInfoTable[] = 
{
    { "",    TOK_STRUCTPARAM | TOK_STRING(ParseInfoTable, name, 0)    },
    { "{",            TOK_START },
    { "}",            TOK_END },
    { "Column",        TOK_STRUCT(ParseInfoTable, columns, tpiParseInfoColumn)    },
    { 0 },
};

ParseTable tpiParseInfoDescriptor[] =
{
    { "ParseInfo",    TOK_STRUCT(ParseInfoDescriptor, tables, tpiParseInfoTable) },
    { 0 },
};

StaticDefineInt ParseTypeOptions[] =
{
    DEFINE_INT
//    {"REDUNDANTNAME",    TOK_REDUNDANTNAME}, // special handling, don't uncomment
    {"STRUCTPARAM",        TOK_STRUCTPARAM},
    {"PRIMARY_KEY",        TOK_PRIMARY_KEY},
    {"AUTO_INCREMENT",    TOK_AUTO_INCREMENT},
    {"USEROPTIONBIT_1",        TOK_USEROPTIONBIT_1},
    {"USEROPTIONBIT_2",        TOK_USEROPTIONBIT_2},
//    {"VOLATILE_REF",        TOK_VOLATILE_REF}, // i'm not sure why this wasn't included
    {"NO_TRANSLATE",        TOK_NO_TRANSLATE},
    {"SERVER_ONLY",            TOK_SERVER_ONLY},
    {"CLIENT_ONLY",            TOK_CLIENT_ONLY},
    {"PERSIST",                TOK_PERSIST},
    {"NO_TRANSACT",            TOK_NO_TRANSACT},
    {"EARRAY",            TOK_EARRAY},
    {"FIXED_ARRAY",        TOK_FIXED_ARRAY},
    {"INDIRECT",        TOK_INDIRECT},
    {"POOL_STRING",        TOK_POOL_STRING},
    DEFINE_END
};

StaticDefineInt ParseFloatRounding[] =
{
    DEFINE_INT
    {"HUNDRETHS",    FLOAT_HUNDRETHS},
    {"TENTHS",        FLOAT_TENTHS},
    {"ONES",        FLOAT_ONES},
    {"FIVES",        FLOAT_FIVES},
    {"TENS",        FLOAT_TENS},
    DEFINE_END
};

StaticDefineInt ParseFormatOptions[] =
{
    DEFINE_INT
    {"IP",                TOK_FORMAT_IP},
    {"KBYTES",            TOK_FORMAT_KBYTES},
    {"FRIENDLYDATE",    TOK_FORMAT_FRIENDLYDATE},
    {"FRIENDLYSS2000",    TOK_FORMAT_FRIENDLYSS2000},
    {"FRIENDLYCPU",        TOK_FORMAT_FRIENDLYCPU},
    {"PERCENT",            TOK_FORMAT_PERCENT},
    {"NOPATH",            TOK_FORMAT_NOPATH},
    {"HSV",                TOK_FORMAT_HSV},
    {"OBJECTID",        TOK_FORMAT_OBJECTID},
    {"KEY",                TOK_FORMAT_KEY},
    {"TEXTURE",            TOK_FORMAT_TEXTURE},
    {"UNSIGNED",        TOK_FORMAT_UNSIGNED},
    {"DATESS2000",        TOK_FORMAT_DATESS2000},
    {"BYTES",            TOK_FORMAT_BYTES},
    {"MICROSECONDS",    TOK_FORMAT_MICROSECONDS},
    {"MBYTES",            TOK_FORMAT_MBYTES},
    DEFINE_END
};

StaticDefineInt ParseFormatUIOptions[] =
{
    DEFINE_INT
    {"FORMAT_UI_LEFT",                TOK_FORMAT_UI_LEFT},
    {"FORMAT_UI_RIGHT",                TOK_FORMAT_UI_RIGHT},
    {"FORMAT_UI_RESIZABLE",            TOK_FORMAT_UI_RESIZABLE},
    {"FORMAT_UI_NOTRANSLATE_HEADER",TOK_FORMAT_UI_NOTRANSLATE_HEADER},
    {"FORMAT_UI_NOHEADER",            TOK_FORMAT_UI_NOHEADER},
    {"FORMAT_UI_NODISPLAY",            TOK_FORMAT_UI_NODISPLAY},
    DEFINE_END
};


static void ParseInfoPushOption(ParseInfoColumn* elem, char const* option, const char* param)
{
    StructFunctionCall* call = ParserAllocStruct(sizeof(StructFunctionCall));
    call->function = StructAllocString(option);
    eaPush(&elem->options, call);
    if (param && param[0])
    {
        StructFunctionCall* call2 = ParserAllocStruct(sizeof(StructFunctionCall));
        call2->function = StructAllocString(param);
        eaPush(&call->params, call2);
    }
}

static void ParseInfoPushOptionInt(ParseInfoColumn* elem, char* option, int param)
{
    char buf[100];
    sprintf_s(SAFESTR(buf), "%i", param);
    ParseInfoPushOption(elem, option, buf);
}

static char* ParseInfoGetType(ParseTable tpi[], int column, StructTypeField* modifiedtype)
{
    char* ret = 0;
    int storage = TokenStoreGetStorageType(tpi[column].type);
    *modifiedtype = tpi[column].type;

    // first attempt to find a name that combines both the basic type and the storage attribute
    switch (storage) {
    case TOK_STORAGE_DIRECT_FIXEDARRAY:        ret = TYPE_INFO(tpi[column].type).name_direct_fixedarray; break;
    case TOK_STORAGE_DIRECT_EARRAY:            ret = TYPE_INFO(tpi[column].type).name_direct_earray; break;
    case TOK_STORAGE_INDIRECT_SINGLE:        ret = TYPE_INFO(tpi[column].type).name_indirect_single; break;
    case TOK_STORAGE_INDIRECT_FIXEDARRAY:    ret = TYPE_INFO(tpi[column].type).name_indirect_fixedarray; break;
    case TOK_STORAGE_INDIRECT_EARRAY:        ret = TYPE_INFO(tpi[column].type).name_indirect_earray; break;
    }

    if (ret) // clear the storage attribute if we recognized the combined name
    {
        *modifiedtype = *modifiedtype & ~TOK_INDIRECT;
        *modifiedtype = *modifiedtype & ~TOK_EARRAY;
        *modifiedtype = *modifiedtype & ~TOK_FIXED_ARRAY;
    }
    else
    {
        ret = TYPE_INFO(tpi[column].type).name_direct_single; // guaranteed to exist
    }
    return ret;
}

static bool ParseInfoTypeFromName(char* name, StructTypeField* result)
{
    int i;
    for (i = 0; i < NUM_TOK_TYPE_TOKENS; i++)
    {
#define TRY_FIELD(field, bits) \
    if (TYPE_INFO(i).field && stricmp(TYPE_INFO(i).field, name) == 0) \
    { \
        *result |= i | bits; \
        return true; \
    }
        TRY_FIELD(name_direct_single, 0);
        TRY_FIELD(name_direct_fixedarray,    TOK_FIXED_ARRAY);
        TRY_FIELD(name_direct_earray,        TOK_EARRAY);
        TRY_FIELD(name_indirect_single,        TOK_INDIRECT);
        TRY_FIELD(name_indirect_fixedarray, TOK_INDIRECT | TOK_FIXED_ARRAY);
        TRY_FIELD(name_indirect_earray,        TOK_INDIRECT | TOK_EARRAY);
#undef TRY_FIELD
    }
    return false;
}

// find a field with the same offset as passed column
static int ParseInfoFindAliasedField(ParseTable pti[], int column)
{
    int i;

    FORALL_PARSEINFO(pti, i)
    {
        if (i == column) continue;
        if (pti[i].type & TOK_REDUNDANTNAME) continue;
        if (pti[column].storeoffset == pti[i].storeoffset) return i;
    }
    return -1;
}

static void ParseInfoIdentifySubtables(ParseTable pti[], StashTable subNames, char* name)
{
    char *newString = strdup(name);
    int i;
    if (stashAddressFindElement(subNames, pti, NULL)) return; // we've already done this subtable
    stashAddressAddPointer(subNames, pti, newString, 0);
    
    FORALL_PARSEINFO(pti, i)
    {
        if (TOK_GET_TYPE(pti[i].type) == TOK_STRUCT_X)
        {
            char fullName[1000];
            fullName[0] = 0;
            // MAK - if we hit this, we may have to implement an auto-numbering scheme for untitled subtables.. conflicts with editability though
            
            devassertmsg(pti[i].name && pti[i].name[0], "Don't have a subtable name to use with ParseInfo functions?");
            
            sprintf_s(SAFESTR(fullName),"%s.%s",name,pti[i].name);

            devassertmsg(stricmp(fullName, PARSEINFO_ROOTNAME), "Please rename your subtable to something that doesn't conflict with the root name");
            ParseInfoIdentifySubtables(pti[i].subtable, subNames, fullName);
        }
    }
}

static ParseInfoTable* ParseInfoToTable(ParseTable pti[], StashTable subNames)
{
    ParseInfoFieldUsage usage;
    StaticDefineInt* define;
    int i, precision, format, width;
    char *option;
    const char *param;
    ParseInfoTable* table;
    char* tablename;

    // set up the table
    table = ParserAllocStruct(sizeof(*table));
    stashAddressFindPointer(subNames, pti, &tablename);
    devassertmsg(tablename, "Internal parse error in "__FUNCTION__); // should be in hash already
    if (tablename)
        table->name = StructAllocString(tablename);

    // then each column
    FORALL_PARSEINFO(pti, i)
    {
        StructTypeField modifiedtype = pti[i].type;
        ParseInfoColumn* elem = ParserAllocStruct(sizeof(ParseInfoColumn));

        assert(sizeof(pti[i].type) == 4); // otherwise, need to change modifiedtype

        // name
        eaPush(&table->columns, elem);
        if (pti[i].name && pti[i].name[0])
            ParseInfoPushOption(elem, "NAME", pti[i].name);

        // basic type
        option = ParseInfoGetType(pti, i, &modifiedtype); // can strip some bits from modifiedtype
        ParseInfoPushOption(elem, option, NULL);

        // type options
        for (define = &ParseTypeOptions[1]; define->key != U32_TO_PTR(DM_END); define++)
        {
            if (modifiedtype & define->value)
                ParseInfoPushOption(elem, define->key, NULL);
        }

        // special handling for redundantname - param is name of aliased field
        if (pti[i].type & TOK_REDUNDANTNAME)
        {
            int alias = ParseInfoFindAliasedField(pti, i);
            devassertmsg(alias > 0 && pti[alias].name && pti[alias].name[0], "Couldn't find aliased field for redundant token in "__FUNCTION__);
            if (alias > 0)
            {
                ParseInfoPushOption(elem, "REDUNDANTNAME", pti[alias].name);
            }
        }

        // precision
        param = 0;
        precision = TOK_GET_PRECISION(modifiedtype);
        if (precision)
        {
            // seperate out float rounding if possible
            if (TOK_GET_TYPE(pti[i].type) == TOK_F32_X)
                param = StaticDefineIntRevLookup(ParseFloatRounding, precision);
            if (param)
                ParseInfoPushOption(elem, "FLOAT_ROUNDING", param);
            else
            {
                ParseInfoPushOptionInt(elem, "MINBITS", precision);
            }
        }

        // param
        usage = TYPE_INFO(pti[i].type).interpretfield(pti, i, ParamField);
        switch (usage)
        {
        case SizeOfSubstruct: break; // ignored, we will derive this again when loading
        xcase NumberOfElements:            ParseInfoPushOptionInt(elem, "NUM_ELEMENTS", pti[i].param); break;
        xcase DefaultValue:                if (pti[i].param) ParseInfoPushOptionInt(elem, "DEFAULT", pti[i].param); break;
        xcase EmbeddedStringLength:        ParseInfoPushOptionInt(elem, "STRING_LENGTH", pti[i].param); break;
        xcase PointerToDefaultString:    if (pti[i].param) ParseInfoPushOption(elem, "DEFAULT_STRING", (char*)pti[i].param); break;
        xcase OffsetOfSizeField: break; // ignored, we will derive this again when loading
        xcase SizeOfRawField:            ParseInfoPushOptionInt(elem, "SIZE", pti[i].param); break;
        }

        // subtable
        usage = TYPE_INFO(pti[i].type).interpretfield(pti, i, SubtableField);
        switch (usage)
        {
        case PointerToLinkDictionary: devassert(0); break; // assert this?  I don't have a good way of describing the link dictionary
        xcase PointerToDictionaryName:
        {
            if (pti[i].subtable)
            {
                ParseInfoPushOption(elem, "DICTIONARYNAME", (char*)pti[i].subtable);
            }
        } break;
        xcase StaticDefineList: 
        // We need to indicate this has a StaticDefineList, so plain-text sending works correctly
        // We may need to replace this with correctly serializing the actual StaticDefineList
        {
            if (pti[i].subtable)
            {
                ParseInfoPushOption(elem, "STATICDEFINELIST", NULL); //replace the NULL with the name of the enum eventually
            }
        } break; 
        
        xcase PointerToSubtable:
        {
            char* subname = 0;
            stashAddressFindPointer(subNames, pti[i].subtable, &subname);
            devassertmsg(subname, "Internal parse error in "__FUNCTION__); // should be in hash already
            if (subname)
                ParseInfoPushOption(elem, "SUBTABLE", subname);
        } break;
        };

        // format options
        format = TOK_GET_FORMAT_OPTIONS(pti[i].format);
        if (format)
        {
            param = StaticDefineIntRevLookup(ParseFormatOptions, format);
            if (param)
                ParseInfoPushOption(elem, "FORMAT", param);
            else // unknown format?
                ParseInfoPushOptionInt(elem, "FORMAT_RAW", format); 
        }

        // list view width
        width = TOK_FORMAT_GET_LVWIDTH(pti[i].format);
        if (width)
            ParseInfoPushOptionInt(elem, "LVWIDTH", width);

        // format ui options
        for (define = &ParseFormatUIOptions[1]; define->key != U32_TO_PTR(DM_END); define++)
        {
            if (pti[i].format & define->value)
                ParseInfoPushOption(elem, define->key, NULL);
        }
    } // each column
    return table;
}

static INLINEDBG bool TPIHasTPIInfoColumn(ParseTable table[])
{
    if (table[0].type & TOK_PARSETABLE_INFO)
    {
        return true;
    }

    return false;
}


// should keep sync with any memory allocated by ParseInfoFromTable
static void ParseInfoFreeTable(ParseTable* pti)
{
    int i;
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].name) StructFreeString(pti[i].name);
        if (pti[i].param)
        {
            int usage = TYPE_INFO(pti[i].type).interpretfield(pti, i, ParamField);
            if (usage == PointerToDefaultString)
                StructFreeString((char*)pti[i].param);
        }
        if (pti[i].subtable)
        {
            int usage = TYPE_INFO(pti[i].type).interpretfield(pti, i, SubtableField);
            if (usage == PointerToDictionaryName)
                StructFreeString((char*)pti[i].subtable);
        }
    }
    StructFree(pti); // the whole parse table was alloced as a big chunk
}

void ParseTableFree(ParseTable*** eapti)
{
    int i;
    for (i = 0; i < eaSize(eapti); i++)
        ParseInfoFreeTable((*eapti)[i]);
    eaDestroy(eapti);
}

ParseTable* ParseInfoFromTable(ParseInfoTable* table)
{
    int i, opt;
    ParseTable* tpi;

    if (!eaSize(&table->columns)) return NULL; 
    tpi = ParserAllocStruct(sizeof(ParseTable) * (eaSize(&table->columns)+1)); // last entry null
    for (i = 0; i < eaSize(&table->columns); i++) // each column
    {
        for (opt = 0; opt < eaSize(&table->columns[i]->options); opt++) // each option for the column
        {
            StructFunctionCall* optionstruct = table->columns[i]->options[opt];
            char* option = optionstruct->function;
            char* param = optionstruct->params ? optionstruct->params[0]->function: NULL;
            if (!option)
            {
                Errorf("Invalid format encountered in ParseInfoFromTable");
                continue;
            }
            else if (stricmp(option, "NAME")==0)
            {
                if (!param) 
                {
                    Errorf("Name not given parameter in " __FUNCTION__);
                }
                else
                {
                    tpi[i].name = StructAllocString(param);
                }
            }
            else if (ParseInfoTypeFromName(option, &tpi[i].type))
                ; // cool
            else if (StaticDefineIntLookup(ParseTypeOptions, option))
            {
                tpi[i].type |= StaticDefineIntGetInt(ParseTypeOptions, option);
            }
            else if (stricmp(option, "REDUNDANTNAME")==0)
            {
                // putting the name in storeoffset right now, it will be fixed up later in this function
                if (!param)
                    Errorf("REDUNDANTNAME not given parameter in " __FUNCTION__);
                tpi[i].type |= TOK_REDUNDANTNAME;
                tpi[i].storeoffset = (size_t)param;
            }
            else if (stricmp(option, "FLOAT_ROUNDING")==0)
            {
                if (!param)
                    Errorf("FLOAT_ROUNDING missing parameter in " __FUNCTION__);
                else
                {
                    int rounding = StaticDefineIntGetInt(ParseFloatRounding, param);
                    if (!rounding)
                        Errorf("Invalid param %s to FLOAT_ROUNDING in " __FUNCTION__, param);
                    else
                        tpi[i].type |= TOK_FLOAT_ROUNDING(rounding);
                }
            }
            else if (stricmp(option, "MINBITS")==0)
            {
                if (!param)
                    Errorf("MINBITS missing parameter in " __FUNCTION__);
                else
                {
                    tpi[i].type |= TOK_MINBITS(atoi(param));
                }
            }
            else if (stricmp(option, "NUM_ELEMENTS")==0 ||
                    stricmp(option, "DEFAULT")==0 ||
                    stricmp(option, "STRING_LENGTH")==0 ||
                    stricmp(option, "SIZE")==0)
            {
                int usage = TYPE_INFO(tpi[i].type).interpretfield(tpi, i, ParamField);
                if (usage != NumberOfElements && 
                    usage != DefaultValue &&
                    usage != EmbeddedStringLength &&
                    usage != SizeOfRawField)
                {
                    Errorf("Invalid option %s passed to "__FUNCTION__".  You may need to state the correct type first", option);
                }
                else if (!param)
                {
                    Errorf("No param given to option %s in "__FUNCTION__, option);
                }
                else
                {
                    tpi[i].param = atoi(param);
                }
            }
            else if (stricmp(option, "DEFAULT_STRING")==0)
            {
                int usage = TYPE_INFO(tpi[i].type).interpretfield(tpi, i, ParamField);
                if (usage != PointerToDefaultString)
                {
                    Errorf("Invalid option %s passed to "__FUNCTION__".  You may need to state the correct type first", option);
                }
                else if (!param)
                {
                    Errorf("No param given to option %s in "__FUNCTION__, option);
                }
                else
                {
                    tpi[i].param = (intptr_t)StructAllocString(param);
                }
            }
            else if (stricmp(option, "DICTIONARYNAME")==0)
            {
                int usage = TYPE_INFO(tpi[i].type).interpretfield(tpi, i, SubtableField);
                if (usage != PointerToDictionaryName)
                {
                    Errorf("Invalid option %s passed to "__FUNCTION__".  You may need to state the correct type first", option);
                }
                else
                {
                    tpi[i].subtable = (void*)StructAllocString(param);
                }
            }
            else if (stricmp(option, "STATICDEFINELIST")==0)
            {
                int usage = TYPE_INFO(tpi[i].type).interpretfield(tpi, i, SubtableField);
                if (usage != StaticDefineList)
                {
                    Errorf("Invalid option %s passed to "__FUNCTION__".  You may need to state the correct type first", option);
                }
                else
                {
                    tpi[i].subtable = NullStaticDefineList;
                }
            }
            else if (stricmp(option, "SUBTABLE")==0)
            {
                if (TOK_GET_TYPE(tpi[i].type) != TOK_STRUCT_X)
                {
                    Errorf("Option SUBTABLE passed to an invalid field type in "__FUNCTION__".  You may need to state the correct type first");
                }
                else
                {
                    // putting the name in subtable right now, it will be fixed up later in this function
                    if (!param)
                        Errorf("SUBTABLE not given parameter in " __FUNCTION__);
                    tpi[i].subtable = param;
                }
            }
            else if (stricmp(option, "FORMAT")==0)
            {
                if (!param)
                    Errorf("FORMAT missing parameter in " __FUNCTION__);
                else
                {
                    int format = StaticDefineIntGetInt(ParseFormatOptions, param);
                    if (!format)
                        Errorf("Invalid param %s to FORMAT in " __FUNCTION__, param);
                    else
                        tpi[i].format |= format;
                }
            }
            else if (stricmp(option, "FORMAT_RAW")==0)
            {
                if (!param)
                    Errorf("FORMAT_RAW missing parameter in " __FUNCTION__);
                else
                {
                    int format = atoi(param);
                    if (!format || TOK_GET_FORMAT_OPTIONS(format) != format)
                        Errorf("Invalid param %s to FORMAT_RAW in " __FUNCTION__, param);
                    else
                        tpi[i].format |= format;
                }
            }
            else if (stricmp(option, "LVWIDTH")==0)
            {
                if (!param)
                    Errorf("LVWIDTH missing parameter in "__FUNCTION__);
                else
                {
                    int width = atoi(param);
                    if (!width)
                        Errorf("Invalid param %s to LVWIDTH in "__FUNCTION__, param);
                    else
                        tpi[i].format |= TOK_FORMAT_LVWIDTH(width);
                }
            }
            else if (StaticDefineIntLookup(ParseFormatUIOptions, option))
            {
                tpi[i].format |= StaticDefineIntGetInt(ParseFormatUIOptions, option);
            }
        } // per option

        // MAK - get rid of this assumption in other code later?
        // need to have a name field
        if (!tpi[i].name) 
            tpi[i].name = StructAllocString("");
    } // per column
    return tpi;
}

void ParseInfoToTableIter(ParseTable pti[], StashTable subs, ParseInfoDescriptor* pid)
{
    ParseInfoTable* table;
    int i;
    if (stashAddressFindElement(subs, pti, NULL)) return; // we've already done this subtable
    stashAddressAddPointer(subs, pti, 0, 0);
    table = ParseInfoToTable(pti, pid->subNames);
    if (table) eaPush(&pid->tables, table);
    
    FORALL_PARSEINFO(pti, i)
    {
        if (TOK_GET_TYPE(pti[i].type) == TOK_STRUCT_X)
        {
            ParseInfoToTableIter(pti[i].subtable, subs, pid);
        }
    }
}

static void freeFunc(char *str)
{
    free(str);
}

bool ParseInfoToDescriptor(ParseTable pti[], ParseInfoDescriptor* pid)
{
    int count;
    StashTable subtables = stashTableCreateAddress(10);
    pid->subNames = stashTableCreateAddress(10);
    ParseInfoIdentifySubtables(pti, pid->subNames, PARSEINFO_ROOTNAME);
    // I could just iterate over subNames here, but I'd prefer to have the subtables in order
    ParseInfoToTableIter(pti, subtables, pid);
    count = stashGetValidElementCount(pid->subNames);
    stashTableDestroyEx(pid->subNames,NULL,freeFunc);
    stashTableDestroy(subtables);
    pid->subNames = 0;
    return count == eaSize(&pid->tables);
}

bool ParseInfoFixupSubtablePointers(ParseTable pti[], StashTable subAddresses)
{
    void* subtable;
    bool ret = true;
    int i;
    FORALL_PARSEINFO(pti, i)
    {
        if (TOK_GET_TYPE(pti[i].type) == TOK_STRUCT_X)
        {
            if (!pti[i].subtable)
            {
                Errorf("Missing subtable from TOK_STRUCT in " __FUNCTION__);
                ret = false;
            }
            else if (!stashFindPointer(subAddresses, pti[i].subtable, &subtable)) // subtable is a string right now
            {
                Errorf("Invalid subtable reference %s.  Missing target table", pti[i].subtable);
                ret = false;
            }
            else // lookup worked
            {
                pti[i].subtable = subtable;
            }
        }
    }
    return ret;
}

bool ParseInfoFixupRedundantNames(ParseTable pti[])
{
    bool ret = true;
    int i, j;
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME)
        {
            bool found = false;
            char* parentfield = (char*)pti[i].storeoffset;
            FORALL_PARSEINFO(pti, j)
            {
                if (i == j) continue;
                if (pti[j].name && stricmp(pti[j].name, parentfield)==0)
                {
                    found = true;
                    pti[i].storeoffset = pti[j].storeoffset;
                }
            }
            if (!found)
            {
                Errorf("REDUNDANTNAME given name of invalid field %s.  This doesn't not match another field in the struct.", parentfield);
                ret = false;
                pti[i].storeoffset = 0; 
            }
        }
    }
    return ret;
}

bool ParseInfoFromDescriptor(ParseTable*** eapti, int* size, ParseInfoDescriptor* pid)
{
    bool ret = true;
    int i;
    void* roottable = 0;
    pid->subAddresses = stashTableCreateWithStringKeys(10, StashDeepCopyKeys);

    // first create the parse info tables
    for (i = 0; i < eaSize(&pid->tables); i++)
    {
        ParseTable* table = ParseInfoFromTable(pid->tables[i]);
        if (table)
        {
            stashAddPointer(pid->subAddresses, pid->tables[i]->name, table, 0);
            eaPush(eapti, table);
        }
        else ret = false;
    }
    
    // the following require the root to be the first table
    if (ret && !stashFindPointer(pid->subAddresses, PARSEINFO_ROOTNAME, &roottable))
    {
        Errorf("Could not find root parse table in "__FUNCTION__);
        ret = false;
    }
    if (ret) for (i = 0; i < eaSize(eapti); i++)
    {
        if ((*eapti)[i] == roottable)
        {
            if (i == 0) break; // root is already first
            (*eapti)[i] = (*eapti)[0];
            (*eapti)[0] = roottable;
            break;
        }
    }
    assert(i < eaSize(eapti));

    // now do various fixups in order
    for (i = 0; i < eaSize(eapti); i++)
        ret = ret && ParseInfoFixupSubtablePointers((*eapti)[i], pid->subAddresses); 
    if (ret) *size = ParseInfoCalcOffsets((*eapti)[0]); 
    for (i = 0; i < eaSize(eapti); i++)
        ret = ret && ParseInfoFixupRedundantNames((*eapti)[i]);

    // finally done
    stashTableDestroy(pid->subAddresses);
    if (!ret) ParseTableFree(eapti);
    return ret;
}

bool ParseTableWriteTextFile(char* filename, ParseTable pti[])
{
    bool ret;
    ParseInfoDescriptor pid = {0};
    ret = ParseInfoToDescriptor(pti, &pid);
    ret = ret && ParserWriteTextFile(filename, tpiParseInfoDescriptor, &pid, 0, 0);
    StructClear(tpiParseInfoDescriptor, &pid);
    return ret;
}

bool ParseTableWriteText(char **estr, ParseTable pti[])
{
    bool ret;
    ParseInfoDescriptor pid = {0};
    ret = ParseInfoToDescriptor(pti, &pid);
    ret = ret && ParserWriteText(estr, tpiParseInfoDescriptor, &pid, 0, 0);
    StructClear(tpiParseInfoDescriptor, &pid);
    return ret;
}

bool ParseTableReadTextFile(char* filename, ParseTable*** eapti, int* size)
{
    bool ret;
    ParseInfoDescriptor pid = {0};
    *eapti = 0;

    ret = ParserReadTextFile(filename, tpiParseInfoDescriptor, &pid);
    if (ret) ret = ret && ParseInfoFromDescriptor(eapti, size, &pid);
    StructClear(tpiParseInfoDescriptor, &pid);
    return ret;
}

bool ParseTableSend(Packet* pak, ParseTable pti[])
{
    bool ret;
    ParseInfoDescriptor pid = {0};
    ret = ParseInfoToDescriptor(pti, &pid);
    if (ret) ParserSend(tpiParseInfoDescriptor, pak, NULL, &pid, 1, 0, 1, 0, 0);
    StructClear(tpiParseInfoDescriptor, &pid);
    return ret;
}

bool ParseTableRecv(Packet* pak, ParseTable*** eapti, int* size)
{
    bool ret = true;
    ParseInfoDescriptor pid = {0};
    *eapti = 0;

    ParserRecv(tpiParseInfoDescriptor, pak, &pid, NULL, 1);
    ret = ParseInfoFromDescriptor(eapti, size, &pid);
    StructClear(tpiParseInfoDescriptor, &pid);
    return ret;
}

/////////////////////////////////////////////////// textparser path functions

typedef bool (*RootPathLookupFunc)(char* name, ParseTable** table, void** structptr, int* column);

#define MAX_ROOT_PATH_FUNC 5
static RootPathLookupFunc g_rootpathtable[MAX_ROOT_PATH_FUNC] = {0};

void ParserRegisterRootPathFunc(RootPathLookupFunc function)
{
    int i;
    for (i = 0; i < MAX_ROOT_PATH_FUNC; i++)
        if (!g_rootpathtable[i])
        {
            g_rootpathtable[i] = function;
            return;
        }
    assert(0 && "Increase MAX_ROOT_PATH_FUNC as necessary");
}

bool ParserRootPathLookup(char* name, ParseTable** table, void** structptr, int* column)
{
    int i;
    for (i = 0; i < MAX_ROOT_PATH_FUNC; i++)
    {
        if (!g_rootpathtable[i]) break;
        if (g_rootpathtable[i](name, table, structptr, column))
            return true;
    }
    return false;
}

// do a key lookup on this struct and return the resulting index
// - structptr[column] is required to be an earray of structs
bool ParserResolveKey(char* key, ParseTable table[], int column, void* structptr, int* index)
{
    ParseTable* subtable;
    int keyfield = -1;
    int i, numelems;
    char buf[MAX_TOKEN_LENGTH];

    assert(table[column].type & TOK_EARRAY && TOK_GET_TYPE(table[column].type) == TOK_STRUCT_X);

    // TODO - cache the key field in meta information
    subtable = table[column].subtable;
    FORALL_PARSETABLE(subtable, i)
    {
        if (TOK_GET_FORMAT_OPTIONS(subtable[i].format) == TOK_FORMAT_KEY)
        {
            keyfield = i;
            break;
        }
    }
    if (keyfield < 0) // if not key, look for object id field
    FORALL_PARSETABLE(subtable, i)
    {
        if (TOK_GET_FORMAT_OPTIONS(subtable[i].format) == TOK_FORMAT_OBJECTID)
        {
            keyfield = i;
            break;
        }
    }
    if (keyfield < 0)
    {
        Errorf("ParserResolveKey: called on table that doesn't have a KEY field");
        return false;
    }

    numelems = TokenStoreGetNumElems(table, column, structptr);
    for (i = 0; i < numelems; i++)
    {
        void* substruct = TokenStoreGetPointer(table, column, structptr, i);
        if (!substruct) continue;
        if (!TokenToSimpleString(subtable, keyfield, substruct, SAFESTR(buf), false)) continue;
        if (stricmp(buf, key)==0)
        {
            *index = i;
            return true;
        }
    }
    return false;
}

// eat a token from path and copy to buf, allows for quotation marks around
// token - maybe support full escaping later?
static bool GetIdentifier(char* buf, int buf_size, char** path)
{
    bool found = false;
    char* str = *path;

    if (!str || !str[0]) return false;
    while (str[0] == ' ') str++;
    if (str[0] == '"')
    {
        buf_size--; // room for null
        while (buf_size > 0 && str[0] && str[0] != '"')
        {
            *buf++ = *str++;
            buf_size--;
            found = true;
        }
        *buf = 0;
        if (str[0] == '"')
        {
            *path = str+1;
            return found;
        }
        return false;
    }
    else
    {
        buf_size--;
        while (buf_size > 0 && str[0] && isalnum(str[0]))
        {
            *buf++ = *str++;
            buf_size--;
            found = true;
        }
        *buf = 0;
        *path = str;
        return found;
    }
}

// helper for ParserResolvePath - field is being used, so traverse down to next logical level.  
// Just deals with structs now, but should be expanded to references
static bool TraverseObject(ParseTable** table, int* column, void** structptr, int* index, char* path_in)
{
    void* substruct;

    // TODO - when we have a good way of doing a table lookup for 
    // any reference, then allow references to be traversed this way..
    if (TOK_GET_TYPE((*table)[*column].type) != TOK_STRUCT_X)
    {
        Errorf("attempted to obtain field of non-struct: %s", path_in);
        return false;
    }
    substruct = TokenStoreGetPointer(*table, *column, *structptr, *index);
    if (!substruct)
    {
        Errorf("attempted to traverse null substruct in %s", path_in);
        return false;
    }

    // success
    *table = (*table)[*column].subtable;
    *column = -1;
    *structptr = substruct;
    *index = -1;
    return true;
}

bool ParserResolvePath(char* path_in, ParseTable table_in[], void* structptr_in, 
       ParseTable** table_out, int* column_out, void** structptr_out, int* index_out)
{
    ParseTable* table = table_in;
    int column = -1;
    void* structptr = structptr_in;
    int index = -1;
    char* path = path_in;
    bool ok = true;
    char buf[MAX_TOKEN_LENGTH];

    // get from global table first, unless we're using . to signify local reference
    while (path[0] == ' ') path++;
    if (path && path[0] != '.' && path[0] != '[')
    {
        ok = GetIdentifier(SAFESTR(buf), &path);
        if (!ok) Errorf("failed parsing path %s", path_in);
        else
        {
            ok = ParserRootPathLookup(buf, &table, &structptr, &column);
            if (!ok) Errorf("failed global textparser table lookup of %s", buf);
        }
    }
    if (ok && !table) { ok = false; Errorf("usage error: local path specified, but table_in parameter was not provided"); }

    // main loop for iterating through path
    while (ok && path && path[0]) 
    {
        while (path[0] == ' ') path++;
        if (path[0] == '[') // indexing
        {
            path++;
            ok = GetIdentifier(SAFESTR(buf), &path);
            if (ok) // eat trailing bracket immediately
            {
                while (path[0] == ' ') path++;
                if (path[0] == ']') path++;
            }

            if (!ok) Errorf("failed parsing path %s", path_in);
            else if (column < 0) 
                { ok = false; Errorf("attempted indexing of non-field: %s", path_in); }
            else if (index >= 0)
                { ok = false; Errorf("attempted to index field twice: %s", path_in); }
            else if (!structptr) 
                { ok = false; Errorf("attempted indexing without a data pointer"); } // take a look at just ignoring this later?
            else if (!(table[column].type & TOK_EARRAY || table[column].type & TOK_FIXED_ARRAY))
                { ok = false; Errorf("attempted index of single field: %s", path_in); }
            else if (TOK_GET_TYPE(table[column].type) == TOK_STRUCT_X)
            {
                ok = ParserResolveKey(buf, table, column, structptr, &index);
                if (!ok) Errorf("couldn't resolve struct index %s", buf);
            }
            else // non-struct array
            {
                ok = sscanf(buf, "%i", &index) == 1;
                if (!ok) Errorf("expected numeric index but received %s", buf);
            }
        }
        else if (path[0] == '.') // field specifier
        {
            // if we already have a field, then we need to traverse the previous object
            if (column >= 0)
            {
                ok = TraverseObject(&table, &column, &structptr, &index, path_in); 
                    // traverse doesn it's own errorf
            }
            if (ok)
            {
                bool found = false;
                path++;
                ok = GetIdentifier(SAFESTR(buf), &path);
                FORALL_PARSETABLE(table, column)
                {
                    if (!table[column].name || stricmp(table[column].name, buf))
                        continue;
                    found = true;
                    break; // leave column set
                }
                if (!found)
                    { ok = false; Errorf("couldn't find field %s", buf); }
            }
        }
        else // not [ or .
        {
            ok = false;
            Errorf("couldn't parse path %s", path_in);
        }
    } // iterate through path

    // we could take this error out if we want to allow null paths, but if we
    // leave it in, callers never have to worry about getting a negative column
    if (ok && column < 0)
    {
        ok = false;
        Errorf("path incomplete - no field specified: %s", path_in);
    }

    // results
    if (ok)
    {
        // no index specified - this is fine, but the TokenStore functions expect 0 for single values
        if (index < 0) index = 0;

        if (table_out) *table_out = table;
        if (column_out) *column_out = column;
        if (structptr_out) *structptr_out = structptr;
        if (index_out) *index_out = index;
    }
    return ok;
}

/////////////////////////////////////////////////////////////////////////////////////////
// ParseTableInfo - meta information about parse tables

MP_DEFINE(ParseTableInfo);
StashTable g_parsetableinfos;           // indexed by parse table address
StashTable g_parsetableinfos_byname;    // indexed by name

bool ParserSetTableInfoExplicitlyEx(ParseTable table[], int size, char* name, int name_static, char *source_file_name)
{
    ParseTableInfo* info = ParserGetTableInfo(table);

    if (!g_parsetableinfos_byname)
        g_parsetableinfos_byname = stashTableCreateWithStringKeys(1024, 0);

 // we are attempting to infer the size & name through normal textparser calls
 // this means the name may not be correct, but we're ok with that.  If the size 
 // changes, this is probably a more serious problem though
    if (info)
    {
        assertmsg(info->size == size,
            "ParserSetTableInfo: attempting to set table info again with different size!");
        assertmsg(0==stricmp(info->name,name),
            "ParserSetTableInfo: attempting to set table info again with different name");
        return true;
    }

    MP_CREATE(ParseTableInfo, 100);
    info = MP_ALLOC(ParseTableInfo);
    info->table = table;
    info->name = (name_static ? name : allocAddString(name)); 
    info->size = size;
    if (!g_parsetableinfos)    g_parsetableinfos = stashTableCreateAddress(100);
    stashAddressAddPointer(g_parsetableinfos, table, info, 0);
    return true;
}
 
ParseTableInfo *ParserGetTableInfo(ParseTable table[])
{
    int i = 0;
    ParseTableInfo* info = 0;

    if(!table)
        return NULL;
    
    // see if one has been registered
     if(stashAddressFindPointer(g_parsetableinfos, table, &info))
        return info;

    // check for implicit info
    if (TPIHasTPIInfoColumn(table) && table[0].subtable)
        return table[0].subtable;

    // else auto-gen it
    // ab: nope, don't do this. let's just return NULL;
//     if (!info) // create an info record with anything that can be calculated
//     {
//         MP_CREATE(ParseTableInfo, 100);
//         info = MP_ALLOC(ParseTableInfo);
//         info->table = table;
//         info->name = 0;
//         info->size = 0;
//         info->keycolumn = -1;
//         info->typecolumn = -1;
//         info->pFixupCB = NULL;
//         info->pTemplateStruct = NULL;

//         // ab: not supporting key columns yet.
// //         SetKeyColumnInTPIInfoColumn(table, -1);
//         SetTypeColumnInTPIInfoColumn(table, -1);

//         // figure out id & key columns
//         if (table)
//         {
//             FORALL_PARSETABLE(table, i)
//             {
//                 if (table[i].type & TOK_KEY)
//                 {
//                     assertmsg(TOK_GET_TYPE(table[i].type) == TOK_INT_X || TOK_GET_TYPE(table[i].type) == TOK_INT64_X 
//                         || TOK_GET_TYPE(table[i].type) == TOK_STRING_X || TOK_GET_TYPE(table[i].type) == TOK_REFERENCE_X
//                         || TOK_GET_TYPE(table[i].type) == TOK_CURRENTFILE_X || TOK_GET_TYPE(table[i].type) == TOK_FILENAME_X,
//                         "Unsupported type for a key field!");
//                     assertmsg(info->keycolumn == -1, "Multiple keys defined in single parse table!");
//                     info->keycolumn = i;
    
//                     SetKeyColumnInTPIInfoColumn( table, i );



//                 }
//                 if (table[i].type & TOK_OBJECTTYPE)
//                 {
//                     assertmsg(TOK_GET_TYPE(table[i].type) == TOK_INT_X || TOK_GET_TYPE(table[i].type) == TOK_INT64_X
//                         || TOK_GET_TYPE(table[i].type) == TOK_STRING_X,
//                         "Unsupported type for an object id field!");
//                     assertmsg(info->typecolumn == -1, "Multiple object type tags defined in a single parse table!");
//                     info->typecolumn = i;

//                     SetTypeColumnInTPIInfoColumn( table, i );
//                 }
//             }
//             info->numcolumns = i;
//             SetNumColumnsInTPIInfoColumn(table, i);
//             SetTPIInfoPointerInTPIInfoColumn(table, info);
//         }
//         stashAddressAddPointer(g_parsetableinfos, table, info, 0);

//         if (TPIHasTPIInfoColumn(table))
//         {
//             if (ParserFigureOutIfTpiCanBeCopiedFast(table))
//             {
//                 table[0].format |= 0x80000000;
//             }
//         }
//     }
    return info;
}

bool ParserClearExplcitTableInfo(ParseTable table[])
{
    ParseTableInfo* info = 0;
    if (stashAddressRemovePointer(g_parsetableinfos, table, &info) && info)
    {
        MP_FREE(ParseTableInfo, info);
        return true;
    }
    return false;
}

// TextParserAutoFixupCB * ParserGetTableFixupFunc(ParseTable table[])
// {
//     ParseTableInfo* info = ParserGetTableInfo(table);
//     if(info)
//         return info->pFixupCB;
//     return NULL;
// }
