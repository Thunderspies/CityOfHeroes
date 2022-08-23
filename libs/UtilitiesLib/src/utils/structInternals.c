/*\
 *
 *    structInternals.c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    All the dirty details are here - beware!
 *
 */

#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/structInternals.h"
#include "utilitieslib/utils/tokenstore.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/serialize.h"

////////////////////////////////////////////////// sanity checks
void TestParseTable(ParseTable pti[])
{
    int i;
    FORALL_PARSEINFO(pti, i)
    {
        devassert(TYPE_INFO(pti[i].type).type == TOK_GET_TYPE(pti[i].type));
        if (TYPE_INFO(pti[i].type).storage_compatibility)
            devassert(TokenStoreIsCompatible(pti[i].type, TYPE_INFO(pti[i].type).storage_compatibility));
    }
}

///////////////////////////////////////////////////////// forward declares for handler functions
// this way we can locate the functions in topic-appropriate files, 
// but we still have this one central table in which every function is listed

#define FORWARD_DECLARE(type) \
    void type##_preparse(ParseTable* pti, int i, void* structptr, TokenizerHandle tok); \
    int type##_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback); \
    void type##_writetext(FILE* out, ParseTable tpi[], int column, const void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude); \
    int type##_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude); \
    int type##_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum); \
    void type##_initstruct(ParseTable tpi[], int column, void* structptr, int index); \
    void type##_destroystruct(ParseTable tpi[], int column, void* structptr, int index); \
    void type##_updatecrc(ParseTable tpi[], int column, void* structptr, int index); \
    int type##_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index); \
    size_t type##_memusage(ParseTable tpi[], int column, void* structptr, int index); \
    void type##_copystruct(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData); \
    void type##_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude); \
    void type##_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude); \
    void type##_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr); \
    void type##_freepktids(ParseTable tpi[], int column, void** pktidptr); \
    void type##_endianswap(ParseTable tpi[], int column, void* structptr, int index); \
    void type##_interp(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 interpParam); \
    void type##_calcrate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 deltaTime); \
    void type##_integrate(ParseTable tpi[], int column, void* valueStruct, void* rateStruct, void* destStruct, int index, F32 deltaTime); \
    void type##_calccyclic(ParseTable tpi[], int column, void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, int index, F32 fStartTime, F32 deltaTime); \
    void type##_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, const F32* values, U8 uiValuesSpecd, U32* seed); \
    bool type##_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint); \
    bool type##_fromsimple(ParseTable tpi[], int column, void* structptr, int index, char* str); \
    void type##_calcoffset(ParseTable tpi[], int column, size_t* size); \
    ParseInfoFieldUsage type##_interpretfield(ParseTable tpi[], int column, ParseInfoField field);

FORWARD_DECLARE(ignore);
FORWARD_DECLARE(end);
FORWARD_DECLARE(error);
FORWARD_DECLARE(number);
FORWARD_DECLARE(u8);
FORWARD_DECLARE(int16);
FORWARD_DECLARE(int);
FORWARD_DECLARE(int64);
FORWARD_DECLARE(float);
FORWARD_DECLARE(degrees);
FORWARD_DECLARE(string);
FORWARD_DECLARE(char);
FORWARD_DECLARE(raw);
FORWARD_DECLARE(pointer);
FORWARD_DECLARE(currentfile);
FORWARD_DECLARE(timestamp);
FORWARD_DECLARE(linenum);
FORWARD_DECLARE(usedfield);
FORWARD_DECLARE(bool);
FORWARD_DECLARE(flags);
FORWARD_DECLARE(flagarray);
FORWARD_DECLARE(boolflag);
FORWARD_DECLARE(quatpyr);
FORWARD_DECLARE(condrgb);
FORWARD_DECLARE(matpyr);
FORWARD_DECLARE(filename);
FORWARD_DECLARE(link);
FORWARD_DECLARE(reference);
FORWARD_DECLARE(functioncall);
FORWARD_DECLARE(unparsed);
FORWARD_DECLARE(struct);
FORWARD_DECLARE(stashtable);
FORWARD_DECLARE(deprecated);

FORWARD_DECLARE(nonarray);
FORWARD_DECLARE(fixedarray);
FORWARD_DECLARE(earray);

/////////////////////////////////////////////////////////////////////// g_tokentable

// these are the basic types of storage compatibility
#define TOK_STORAGE_NUMBERS        (TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY)    // U8 member, U8 members[3]
#define TOK_STORAGE_NUMBERS_32    (TOK_STORAGE_NUMBERS | TOK_STORAGE_DIRECT_EARRAY)    // also int* - must be 32-bits for earrays
#define TOK_STORAGE_STRINGS        (TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY) // char str[128], char* str, char** strs
#define TOK_STORAGE_STRUCTS        (TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY)    // Mystruct s, Mystruct* s, Mystruct** s

// ALL ENTRIES IN-ORDER
TokenTypeInfo g_tokentable[] = {
    { TOK_IGNORE,    0,    
      "IGNORE", 0, 0, 0, 0, 0, 
      ignore_interpretfield,
      0,    // initstruct
      0,    // destroystruct
      0,    // preparse
      ignore_parse,
      0,    // writetext
      0,    // writebin
      0,    // readbin
      0,    // senddiff
      0,    // recvdiff
      0,    // freepktids
      0,    // tosimple
      0,    // fromsimple
      0,    // updatecrc
      0,    // compare
      0,    // memusage
      0,    // calcoffset
      0,    // copystruct
      0,    // copyfield
      0,    // endianswap
      0,    // interp
      0,    // calcrate
      0,    // integrate
      0,    // calccyclic
      0,    // applydynop
    },
    { TOK_START,    0,
      "START", 0, 0, 0, 0, 0,    
      ignore_interpretfield,
      0,    // initstruct
      0,    // destroystruct
      0,    // preparse
      0,  // parse
      0,  // writetext
      0,    // writebin
      0,    // readbin
      0,    // senddiff
      0,    // recvdiff
      0,    // freepktids
      0,    // tosimple
      0,    // fromsimple
      0,    // updatecrc
      0,    // compare
      0,    // memusage
      0,    // calcoffset
      0,    // copystruct
      0,    // copyfield
      0,    // endianswap
      0,    // interp
      0,    // calcrate
      0,    // integrate
      0,    // calccyclic
      0,    // applydynop
    },
    { TOK_END,        0,
      "END", 0, 0, 0, 0, 0,    
      ignore_interpretfield,
      0,    // initstruct
      0,    // destroystruct
      0,    // preparse
      end_parse,
      0,    // writetext
      0,    // writebin
      0,    // readbin
      0,    // senddiff
      0,    // recvdiff
      0,    // freepktids
      0,    // tosimple
      0,    // fromsimple
      0,    // updatecrc
      0,    // compare
      0,    // memusage
      0,    // calcoffset
      0,    // copystruct
      0,    // copyfield
      0,    // endianswap
      0,    // interp
      0,    // calcrate
      0,    // integrate
      0,    // calccyclic
      0,    // applydynop
    },
    
    // primitives
    { TOK_U8_X,        TOK_STORAGE_NUMBERS,
      "U8", "U8FIXEDARRAY", 0, 0, 0, 0,    
      number_interpretfield,
      u8_initstruct,
      0,    // destroystruct
      0,    // preparse
      u8_parse,
      u8_writetext,
      u8_writebin,
      u8_readbin,
      u8_senddiff,
      u8_recvdiff,
      0,    // freepktids
      u8_tosimple,
      u8_fromsimple,
      u8_updatecrc,
      u8_compare,
      0,    // memusage
      u8_calcoffset,
      0,    // copystruct
      u8_copyfield,
      0,    // endianswap
      u8_interp,
      u8_calcrate,
      u8_integrate,
      u8_calccyclic,
      u8_applydynop,
    },
    { TOK_INT16_X,    TOK_STORAGE_NUMBERS,
      "INT16", "INT16FIXEDARRAY", 0, 0, 0, 0,    
      number_interpretfield,
      int16_initstruct,
      0,    // destroystruct
      0,    // preparse
      int16_parse,
      int16_writetext,
      int16_writebin,
      int16_readbin,
      int16_senddiff,
      int16_recvdiff,
      0,    // freepktids
      int16_tosimple,
      int16_fromsimple,
      int16_updatecrc,
      int16_compare,
      0,    // memusage
      int16_calcoffset,
      0,    // copystruct
      int16_copyfield,
      int16_endianswap,
      int16_interp,
      int16_calcrate,
      int16_integrate,
      int16_calccyclic,
      int16_applydynop,
    },
    { TOK_INT_X,    TOK_STORAGE_NUMBERS_32,
      "INT", "INTFIXEDARRAY", "INTARRAY", 0, 0, 0,    
      number_interpretfield,
      int_initstruct,
      0,    // destroystruct
      0,    // preparse
      int_parse,
      int_writetext,
      int_writebin,
      int_readbin,
      int_senddiff,
      int_recvdiff,
      0,    // freepktids
      int_tosimple,
      int_fromsimple,
      int_updatecrc,
      int_compare,
      0,    // memusage
      int_calcoffset,
      0,    // copystruct
      int_copyfield,
      int_endianswap,
      int_interp,
      int_calcrate,
      int_integrate,
      int_calccyclic,
      int_applydynop,
    },
    { TOK_INT64_X,    TOK_STORAGE_NUMBERS,
      "INT64", "INT64FIXEDARRAY", 0, 0, 0, 0,    
      number_interpretfield,
      int64_initstruct,
      0,    // destroystruct
      0,    // preparse
      int64_parse,
      int64_writetext,
      int64_writebin,
      int64_readbin,
      int64_senddiff,
      int64_recvdiff,
      0,    // freepktids
      int64_tosimple,
      int64_fromsimple,
      int64_updatecrc,
      int64_compare,
      0,    // memusage
      int64_calcoffset,
      0,    // copystruct
      int64_copyfield,
      int64_endianswap,
      int64_interp,
      int64_calcrate,
      int64_integrate,
      int64_calccyclic,
      int64_applydynop,
    },
    { TOK_F32_X,    TOK_STORAGE_NUMBERS_32,
      "F32", "F32FIXEDARRAY", "F32ARRAY", 0, 0, 0,    
      number_interpretfield,
      float_initstruct,
      0,    // destroystruct
      0,    // preparse
      float_parse,
      float_writetext,
      float_writebin,
      float_readbin,
      float_senddiff,
      float_recvdiff,
      0,    // freepktids
      float_tosimple,
      float_fromsimple,
      float_updatecrc,
      float_compare,
      0,    // memusage
      float_calcoffset,
      0,    // copystruct
      float_copyfield,
      float_endianswap,
      float_interp,
      float_calcrate,
      float_integrate,
      float_calccyclic,
      float_applydynop,
    },
    { TOK_DEGREES_X,    TOK_STORAGE_NUMBERS_32,
      "DEGREES", "DEGREESFIXEDARRAY", "DEGREESARRAY", 0, 0, 0,    
      number_interpretfield,
      float_initstruct,
      0,    // destroystruct
      0,    // preparse
      degrees_parse,
      degrees_writetext,
      float_writebin,
      float_readbin,
      float_senddiff,
      float_recvdiff,
      0,    // freepktids
      degrees_tosimple,
      degrees_fromsimple,
      float_updatecrc,
      float_compare,
      0,    // memusage
      float_calcoffset,
      0,    // copystruct
      float_copyfield,
      float_endianswap,
      float_interp,
      float_calcrate,
      float_integrate,
      float_calccyclic,
      float_applydynop,
    },
    { TOK_STRING_X,    TOK_STORAGE_STRINGS,
      "FIXEDSTRING", 0, 0, "STRING", 0, "STRINGARRAY",
      string_interpretfield,
      string_initstruct,
      string_destroystruct,
      0,    // preparse
      string_parse,
      string_writetext,
      string_writebin,
      string_readbin,
      string_senddiff,
      string_recvdiff,
      0,    // freepktids
      string_tosimple,
      string_fromsimple,
      string_updatecrc,
      string_compare,
      string_memusage,
      string_calcoffset,
      string_copystruct,
      string_copyfield,
      0,    // endianswap
      0,    // interp
      0,    // calcrate
      0,    // integrate
      0,    // calccyclic
      string_applydynop,
    },
    { TOK_CHAR_X, TOK_STORAGE_DIRECT_SINGLE,
      "FIXEDCHAR", 0, 0, 0, 0, 0,
      number_interpretfield,
      char_initstruct,
      0,    // destroystruct
      0,    // preparse
      char_parse,
      char_writetext,
      raw_writebin,
      raw_readbin,
      raw_senddiff,
      raw_recvdiff,
      0,    // freepktids
      0,    // tosimple
      0,    // fromsimple
      raw_updatecrc,
      raw_compare,
      0,    // memusage
      raw_calcoffset,
      0,    // copystruct
      raw_copyfield,
      0,    // endianswap
      0,    // interp
      0,    // calcrate
      0,    // integrate
      0,    // calccyclic
      0,    // applydynop
    },
    { TOK_RAW_X,    TOK_STORAGE_DIRECT_SINGLE,
      "RAW", 0, 0, 0, 0, 0,
      raw_interpretfield,
      0,    // initstruct
      0,    // destroystruct
      0,    // preparse
      raw_parse,
      raw_writetext,
      raw_writebin,
      raw_readbin,
      raw_senddiff,
      raw_recvdiff,
      0,    // freepktids
      0,    // tosimple
      0,    // fromsimple
      raw_updatecrc,
      raw_compare,
      0,    // memusage
      raw_calcoffset,
      0,    // copystruct
      raw_copyfield,
      0,    // endianswap
      0,    // interp
      0,    // calcrate
      0,    // integrate
      0,    // calccyclic
      0,    // applydynop
    },
    { TOK_POINTER_X, TOK_STORAGE_INDIRECT_SINGLE,
      "POINTER_X", 0, 0, "POINTER", 0, 0,
      pointer_interpretfield,
      0,    // initstruct
      pointer_destroystruct,
      0,    // preparse
      pointer_parse,
      pointer_writetext,
      pointer_writebin,
      pointer_readbin,
      pointer_senddiff,
      pointer_recvdiff,
      0,    // freepktids
      0,    // tosimple
      0,    // fromsimple
      pointer_updatecrc,
      pointer_compare,
      pointer_memusage,
      pointer_calcoffset,
      pointer_copystruct,
      pointer_copyfield,
      0,    // endianswap
      0,    // interp
      0,    // calcrate
      0,    // integrate
      0,    // calccyclic
      0,    // applydynop
    },
    
    // built-ins
    { TOK_CURRENTFILE_X,    TOK_STORAGE_INDIRECT_SINGLE,
      "CURRENTFILE_X", 0, 0, "CURRENTFILE", 0, 0,
      string_interpretfield,
      0,    // initstruct
      string_destroystruct,
      currentfile_preparse,
      ignore_parse,  // apparently necessary for backwards compatibility
      0,    // writetext
      string_writebin,
      string_readbin,
      string_senddiff,
      string_recvdiff,
      0,    // freepktids
      filename_tosimple,
      filename_fromsimple,
      string_updatecrc,
      string_compare,
      string_memusage,
        string_calcoffset,
        string_copystruct,
        string_copyfield,
        0,    // endianswap
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccyclic
        0,    // applydynop
    },
    { TOK_TIMESTAMP_X,        TOK_STORAGE_DIRECT_SINGLE,
        "TIMESTAMP", 0, 0, 0, 0, 0,
        number_interpretfield,
        0,    // initstruct
        0,    // destroystruct
        timestamp_preparse,    
        error_parse,
        0,    // writetext
        int_writebin,
        int_readbin,
        int_senddiff,
        int_recvdiff,
        0,    // freepktids
        0,    // tosimple
        0,    // fromsimple
        int_updatecrc,
        int_compare,
        0,    // memusage
        int_calcoffset,
        0,    // copystruct
        int_copyfield,
        int_endianswap,
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccyclic
        0,    // applydynop
    },
    { TOK_LINENUM_X,        TOK_STORAGE_DIRECT_SINGLE,
        "LINENUM", 0, 0, 0, 0, 0,
        number_interpretfield,
        0,    // initstruct
        0,    // destroystruct
        linenum_preparse,
        error_parse,
        0,    // writetext
        int_writebin,
        int_readbin,
        int_senddiff,
        int_recvdiff,
        0,    // freepktids
        0,    // tosimple
        0,    // fromsimple
        int_updatecrc,
        int_compare,
        0,    // memusage
        int_calcoffset,
        0,    // copystruct
        int_copyfield,
        int_endianswap,
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccyclic
        0,    // applydynop
    },
    { TOK_USEDFIELD_X,        TOK_STORAGE_INDIRECT_SINGLE,
        "USEDFIELD_X", 0, 0, "USEDFIELD", 0, 0,
        pointer_interpretfield,
        0,    // initstruct
        pointer_destroystruct,
        usedfield_preparse,
        error_parse,
        0,    // writetext
        pointer_writebin,
        pointer_readbin,
        pointer_senddiff,
        pointer_recvdiff,
        0,    // freepktids
        0,    // tosimple
        0,    // fromsimple
        pointer_updatecrc,
        pointer_compare,
        pointer_memusage,
        pointer_calcoffset,
        pointer_copystruct,
        pointer_copyfield,
        0,    // endianswap
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccyclic
        0,    // applydynop
    },
    { TOK_BOOL_X,    TOK_STORAGE_NUMBERS,
        "BOOL", "BOOLFIXEDARRAY", 0, 0, 0, 0,
        number_interpretfield,
        u8_initstruct,
        0,    // destroystruct
        0,    // preparse
        bool_parse,
        u8_writetext,
        u8_writebin,
        u8_readbin,
        u8_senddiff,
        u8_recvdiff,
        0,    // freepktids
        u8_tosimple,
        u8_fromsimple,
        u8_updatecrc,
        u8_compare,
        0,    // memusage
        u8_calcoffset,
        0,    // copystruct
        u8_copyfield,
        0,    // endianswap
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccyclic
        0,    // applydynop
    },
    { TOK_FLAGS_X,            TOK_STORAGE_DIRECT_SINGLE,
        "FLAGS", 0, 0, 0, 0, 0,
        number_interpretfield,
        int_initstruct,
        0,    // destroystruct
        0,    // preparse
        flags_parse,
        flags_writetext,
        int_writebin,
        int_readbin,
        int_senddiff,
        int_recvdiff,
        0,    // freepktids
        int_tosimple,
        int_fromsimple,
        int_updatecrc,
        int_compare,
        0,    // memusage
        int_calcoffset,
        0,    // copystruct
        int_copyfield,
        int_endianswap,
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccyclic
        0,    // applydynop
    },
    { TOK_FLAGARRAY_X,            TOK_STORAGE_DIRECT_FIXEDARRAY,
        "FLAGARRAY_X", "FLAGARRAY", 0, 0, 0, 0,
        number_interpretfield,
        int_initstruct,
        0,    // destroystruct
        0,    // preparse
        flagarray_parse,
        flagarray_writetext,
        int_writebin,
        int_readbin,
        int_senddiff,
        int_recvdiff,
        0,    // freepktids
        int_tosimple,
        int_fromsimple,
        int_updatecrc,
        int_compare,
        0,    // memusage
        int_calcoffset,
        0,    // copystruct
        int_copyfield,
        int_endianswap,
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccyclic
        0,    // applydynop
    },
    { TOK_BOOLFLAG_X,        TOK_STORAGE_DIRECT_SINGLE,
        "BOOLFLAG", 0, 0, 0, 0, 0,
        number_interpretfield,
        u8_initstruct,
        0,    // destroystruct
        0,    // preparse
        boolflag_parse,
        boolflag_writetext,
        u8_writebin,
        u8_readbin,
        u8_senddiff,
        u8_recvdiff,
        0,    // freepktids
        u8_tosimple,
        u8_fromsimple,
        u8_updatecrc,
        u8_compare,
        0,    // memusage
        u8_calcoffset,
        0,    // copystruct
        u8_copyfield,
        0,    // endianswap
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccyclic
        0,    // applydynop
    },
    { TOK_QUATPYR_X,        TOK_STORAGE_DIRECT_FIXEDARRAY,
        "QUATPYR_X", "QUATPYR", 0, 0, 0, 0,
        number_interpretfield,
        0,    // initstruct
        0,    // destroystruct
        0,    // preparse
        quatpyr_parse,
        quatpyr_writetext,
        float_writebin,
        float_readbin,
        float_senddiff,
        float_recvdiff,
        0,    // freepktids
        float_tosimple,
        float_fromsimple,
        float_updatecrc,
        float_compare,
        0,    // memusage
        float_calcoffset,
        0,    // copystruct
        float_copyfield,
        float_endianswap,
        quatpyr_interp,
        quatpyr_calcrate,
        quatpyr_integrate,
        float_calccyclic,
        quatpyr_applydynop,
    },
    
    { TOK_MATPYR_X,            TOK_STORAGE_DIRECT_FIXEDARRAY,
        "MATPYR_X", "MATPYR", 0, 0, 0, 0,
        number_interpretfield,
        0,    // initstruct
        0,    // destroystruct
        0,    // preparse
        matpyr_parse,
        matpyr_writetext,
        matpyr_writebin,
        matpyr_readbin,
        matpyr_senddiff,
        matpyr_recvdiff,
        0,    // freepktids
        float_tosimple,
        float_fromsimple,
        float_updatecrc,
        float_compare,
        0,    // memusage
        float_calcoffset,
        0,    // copystruct
        float_copyfield,
        float_endianswap,
        matpyr_interp,
        matpyr_calcrate,
        matpyr_integrate,
        float_calccyclic,
        float_applydynop,
    },
    { TOK_CONDRGB_X,        TOK_STORAGE_DIRECT_FIXEDARRAY,
        "CONDRGB_X", "CONDRGB", 0, 0, 0, 0,
        number_interpretfield,
        0,    // initstruct
        0,    // destroystruct
        0,    // preparse
        condrgb_parse,
        condrgb_writetext,
        u8_writebin,
        u8_readbin,
        u8_senddiff,
        u8_recvdiff,
        0,    // freepktids
        u8_tosimple,
        u8_fromsimple,
        u8_updatecrc,
        u8_compare,
        0,    // memusage
        u8_calcoffset,
        0,    // copystruct
        u8_copyfield,
        0,    // endianswap
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccyclic
        0,    // applydynop
    },
    { TOK_FILENAME_X,        TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_DIRECT_SINGLE,
        "FIXEDFILENAME", 0, 0, "FILENAME", 0, 0,
        string_interpretfield,
        string_initstruct,
        string_destroystruct,
        0,    // preparse
        filename_parse,
        string_writetext,
        string_writebin,
        string_readbin,
        string_senddiff,
        string_recvdiff,
        0,    // freepktids
        filename_tosimple,
        filename_fromsimple,
        string_updatecrc,
        string_compare,
        string_memusage,
        string_calcoffset,
        string_copystruct,
        string_copyfield,
        0,    // endianswap
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccyclic
        string_applydynop,
    },

    // complex types
    { TOK_LINK_X,            TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY,
        "LINK_X", 0, 0, "LINK", 0, "LINKARRAY",
        link_interpretfield,
        link_initstruct,
        0,    // destroystruct
        0,    // preparse
        link_parse,
        link_writetext,
        link_writebin,
        link_readbin,
        link_senddiff,
        link_recvdiff,
        0,    // freepktids
        link_tosimple,
        link_fromsimple,
        link_updatecrc,
        link_compare,
        0,    // memusage
        link_calcoffset,
        0,    // copystruct
        link_copyfield,
        0,    // endianswap
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccylic
        0,    // applydynop
    },
    { TOK_REFERENCE_X,            TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY,
        "REFERENCE_X", 0, 0, "REFERENCE", 0, "REFARRAY",
        reference_interpretfield,
        reference_initstruct,
        reference_destroystruct,
        0,    // preparse
        reference_parse,
        reference_writetext,
        reference_writebin,
        reference_readbin,
        reference_senddiff,
        reference_recvdiff,
        0,    // freepktids
        reference_tosimple,
        reference_fromsimple,
        reference_updatecrc,
        reference_compare,
        0,    // memusage
        reference_calcoffset,
        reference_copystruct,
        reference_copyfield,
        0,    // endianswap
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccylic
        0,    // applydynop
    },
    { TOK_FUNCTIONCALL_X,    TOK_STORAGE_INDIRECT_EARRAY,
        "FUNCTIONCALL_X", 0, 0, 0, 0, "FUNCTIONCALL",
        ignore_interpretfield,
        0,    // initstruct
        functioncall_destroystruct,
        0,    // preparse
        functioncall_parse,
        functioncall_writetext,
        functioncall_writebin,
        functioncall_readbin,
        functioncall_senddiff,
        functioncall_recvdiff,
        0,    // freepktids
        0,    // tosimple
        0,    // fromsimple
        functioncall_updatecrc,
        functioncall_compare,
        functioncall_memusage,
        earray_calcoffset,
        functioncall_copystruct,
        functioncall_copyfield,
        0,    // endianswap
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccyclic
        0,    // applydynop
    },
    { TOK_UNPARSED_X,        TOK_STORAGE_INDIRECT_EARRAY,
        "UNPARSED_X", 0, 0, 0, 0, "UNPARSED",
        ignore_interpretfield,
        0,    // initstruct
        unparsed_destroystruct,
        0,    // preparse
        unparsed_parse,
        unparsed_writetext,
        unparsed_writebin,
        unparsed_readbin,
        error_senddiff,
        error_recvdiff,
        0,    // freepktids
        0,    // tosimple
        0,    // fromsimple
        unparsed_updatecrc,
        unparsed_compare,
        unparsed_memusage,
        earray_calcoffset,
        unparsed_copystruct,
        unparsed_copyfield,
        0,    // endianswap
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccylic
        0,    // applydynop
    },
    { TOK_STRUCT_X,            TOK_STORAGE_STRUCTS,
        "EMBEDDEDSTRUCT", 0, 0, "OPTIONALSTRUCT", 0, "STRUCT",
        struct_interpretfield,
        struct_initstruct,
        struct_destroystruct,
        0,    // preparse
        struct_parse,
        struct_writetext,
        struct_writebin,
        struct_readbin,
        struct_senddiff,
        struct_recvdiff,
        struct_freepktids,
        0,    // tosimple
        0,    // fromsimple
        struct_updatecrc,
        struct_compare,
        struct_memusage,
        struct_calcoffset,
        struct_copystruct,
        struct_copyfield,
        struct_endianswap,
        struct_interp,
        struct_calcrate,
        struct_integrate,
        struct_calccyclic,
        struct_applydynop,
    },
    { TOK_STASHTABLE_X,        TOK_STORAGE_INDIRECT_SINGLE,
        "STASHTABLE_X", 0, 0, "STASHTABLE", 0, 0,
        ignore_interpretfield,
        0,    // initstruct
        stashtable_destroystruct,
        0,    // preparse
        error_parse,
        0,    // writetext
        0,    // writebin
        0,    // readbin
        0,    // senddiff
        0,    // recvdiff
        0,    // freepktids
        0,    // tosimple
        0,    // fromsimple
        0,    // updatecrc
        0,    // compare
        stashtable_memusage,
        stashtable_calcoffset,
        stashtable_copystruct,
        stashtable_copyfield,
        0,    // endianswap
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccylic
    },
    { TOK_BIT,    0,    
        "BIT", 0, 0, 0, 0, 0, 
        ignore_interpretfield,
        0,    // initstruct
        0,    // destroystruct
        0,    // preparse
        ignore_parse,
        0,    // writetext
        0,    // writebin
        0,    // readbin
        0,    // senddiff
        0,    // recvdiff
        0,    // freepktids
        0,    // tosimple
        0,    // fromsimple
        0,    // updatecrc
        0,    // compare
        0,    // memusage
        0,    // calcoffset
        0,    // copystruct
        0,    // copyfield
        0,    // endianswap
        0,    // interp
        0,    // calcrate
        0,    // integrate
        0,    // calccyclic
        0,    // applydynop
    },
    { TOK_DEPRECATED,    0,    
      "DEPRECATED", 0, 0, 0, 0, 0, 
      ignore_interpretfield,
      0,    // initstruct
      0,    // destroystruct
      0,    // preparse
      deprecated_parse,
      0,    // writetext
      0,    // writebin
      0,    // readbin
      0,    // senddiff
      0,    // recvdiff
      0,    // freepktids
      0,    // tosimple
      0,    // fromsimple
      0,    // updatecrc
      0,    // compare
      0,    // memusage
      0,    // calcoffset
      0,    // copystruct
      0,    // copyfield
      0,    // endianswap
      0,    // interp
      0,    // calcrate
      0,    // integrate
      0,    // calccyclic
      0,    // applydynop
    },
};
STATIC_ASSERT(ARRAY_SIZE(g_tokentable) == NUM_TOK_TYPE_TOKENS);

/////////////////////////////////////////////////////////////////////////// g_arraytable

TokenArrayInfo g_arraytable[3] = {
    {
        nonarray_initstruct,
        nonarray_destroystruct,
        nonarray_parse,
        nonarray_writetext,
        nonarray_writebin,
        nonarray_readbin,
        nonarray_senddiff,
        nonarray_recvdiff,
        nonarray_freepktids,
        nonarray_tosimple,
        nonarray_fromsimple,
        nonarray_updatecrc,
        nonarray_compare,
        nonarray_memusage,
        nonarray_calcoffset,
        nonarray_copystruct,
        nonarray_copyfield,
        nonarray_endianswap,
        nonarray_interp,
        nonarray_calcrate,
        nonarray_integrate,
        nonarray_calccyclic,
        nonarray_applydynop,
    },
    {
        fixedarray_initstruct,
        fixedarray_destroystruct,
        fixedarray_parse,
        fixedarray_writetext,
        fixedarray_writebin,
        fixedarray_readbin,
        fixedarray_senddiff,
        fixedarray_recvdiff,
        fixedarray_freepktids,
        fixedarray_tosimple,
        fixedarray_fromsimple,
        fixedarray_updatecrc,
        fixedarray_compare,
        fixedarray_memusage,
        fixedarray_calcoffset,
        fixedarray_copystruct,
        fixedarray_copyfield,
        fixedarray_endianswap,
        fixedarray_interp,
        fixedarray_calcrate,
        fixedarray_integrate,
        fixedarray_calccyclic,
        fixedarray_applydynop,
    },
    {
        earray_initstruct,
        earray_destroystruct,
        earray_parse,
        earray_writetext,
        earray_writebin,
        earray_readbin,
        earray_senddiff,
        earray_recvdiff,
        earray_freepktids,
        earray_tosimple,
        earray_fromsimple,
        earray_updatecrc,
        earray_compare,
        earray_memusage,
        earray_calcoffset,
        earray_copystruct,
        earray_copyfield,
        earray_endianswap,
        earray_interp,
        earray_calcrate,
        earray_integrate,
        earray_calccyclic,
        earray_applydynop,
    },
};

//////////////////////////////////////////////////////////////////////////////// FileList
// A FileList is a list of filenames and dates.  It is always kept internally sorted

void FileListCreate(FileList* list)
{
    eaCreate(list);
}

void FileListDestroy(FileList* list)
{
    eaDestroyEx(list, NULL);
}

int FileListLength(FileList *list)
{
    return eaSize(list);
}

// ahhh, another binary search function - why doesn't bsearch work better?
// internal function, does binary search for file name.
// returns index where file is or should be inserted and whether
// the file was found
static int FileListSearch(FileList* list, char* path, int* found)
{
    int front = 0;
    int back = eaSize(list) - 1;
    int cmp;
    *found = 0;

    if (back == -1) // no items in list
        return 0;

    // binary search to narrow front and back
    while (back - front > 1)
    {
        int mid = (front + back) / 2;
        cmp = stricmp(path, (*list)[mid]->path);
        if (cmp < 0)
            back = mid;
        else if (cmp > 0)
            front = mid;
        else
        {
            *found = 1;
            return mid;
        }
    }

    // compare to front
    cmp = stricmp(path, (*list)[front]->path);
    if (cmp < 0) // if less than front
        return 0;
    if (cmp == 0) // is front
    {
        *found = 1;
        return 0;
    }

    // compare to back
    cmp = stricmp(path, (*list)[back]->path);
    if (cmp > 0) // greater than back
        return back+1;
    if (cmp == 0) // is back
    {
        *found = 1;
        return back;
    }
    // otherwise, less than back
    return back;
}

// look for the given file in list, we don't
// assume sorted, so just a linear search
FileEntry* FileListFind(FileList* list, char* path)
{
    char cleanpath[MAX_PATH];
    int found;
    int result;
    strcpy(cleanpath, path);
    forwardSlashes(cleanpath);
    result = FileListSearch(list, cleanpath, &found);
    if (!found)
        return NULL;
    return (*list)[result];
}

// add the given file with path and date to list.
// doesn't add if already in list.
// ok to pass 0 for date - if 0, date will be looked up from file name
void FileListInsert(FileList* list, const char* path, __time32_t date)
{
    char *s,cleanpath[MAX_PATH];
    int index;
    int found;
    FileEntry* node;

    // look for an existing element or a good index
    s = fileDataDir();
    if (strnicmp(path,s,strlen(s))==0)
    {
        path += strlen(s);
        if (path[0] == '/' || path[0] == '\\')
            path++;
    }
    strcpy(cleanpath, path);
    forwardSlashes(cleanpath);
    index = FileListSearch(list, cleanpath, &found);
    if (found) return;

    // otherwise, create a node and insert
    node = calloc(sizeof(FileEntry), 1);
    strcpy(node->path, cleanpath);
    if (!date) date = fileLastChanged(path);
    // Date can be 0 at this point, signifying that the file doesn't exist but needs to be checked when
    //  verifying the validity of the .bin file
    node->date = date;
    eaInsert(list, node, index);
}

// returns success
int FileListWrite(FileList* list, SimpleBufHandle file)
{
    long start_loc;
    long size = 0;
    int i, n;

    if (!SerializeWriteHeader(file, FILELIST_SIG, 0, &start_loc))
        return 0;
    n = eaSize(list); // valid if list == NULL
    size += SerializeWriteData(file, sizeof(n), &n);
    for (i = 0; i < n; i++)
    {
        size += WritePascalString(file, (*list)[i]->path);
        size += SerializeWriteData(file, sizeof(__time32_t), &(*list)[i]->date);
    }
    SerializePatchHeader(file, size, start_loc);
    return 1;
}

int FileListRead(FileList* list, SimpleBufHandle file)
{
    char readsig[20];
    int size;
    int i, n, re;

    if (!list) // if we don't care about reading the file list
    {
        SerializeSkipStruct(file);
        return 1;
    }

    // verify header
    if (!SerializeReadHeader(file, readsig, 20, &size))
        return 0;
    if (strcmp(readsig, FILELIST_SIG) || size <= 0)
        return 0;

    // read in entries
    if (!(*list)) FileListCreate(list);
    size -= SerializeReadData(file, sizeof(n), &n);
    for (i = 0; i < n; i++)
    {
        char filepath[MAX_PATH];
        __time32_t filedate;
        FileEntry* node;

        size -= re = ReadPascalString(file, filepath, MAX_PATH);
        if (!re) break;
        size -= re = SerializeReadData(file, sizeof(__time32_t), &filedate);
        if (!re) break;
        if (size < 0) break;

        node = calloc(sizeof(FileEntry), 1);
        strcpy(node->path, filepath);
        node->date = filedate;
        eaPush(list, node);
    }
    if (i != n || size != 0)
        return 0; // didn't get correct amount of information, something's screwed
    return 1; // a-ok
}

void FileListPrint(FileList* list)
{
    int i, n;
    n = eaSize(list);
    for (i = 0; i < n; i++)
    {
        printf("%s: %i\n", (*list)[i]->path, (*list)[i]->date);
    }
}

int FileListIsBinUpToDate(FileList* binlist, FileList *disklist)
{
    int bin_len, disk_len;
    FileEntry *bin_file, *disk_file;
    int i;

    bin_len = eaSize(binlist);
    disk_len = eaSize(disklist);

    if (bin_len != disk_len)
        return false;

    for (i = 0; i < bin_len; i++)
    {
        bin_file = (*binlist)[i];
        disk_file = FileListFind(disklist, bin_file->path);
        if (!disk_file)
        {
            verbose_printf("%s in bin file no longer exists on disk\n", bin_file->path);
            return false;
        }
        if (bin_file->date != disk_file->date)
        {
            verbose_printf("%s in bin file has different date than one on disk\n", bin_file->path);
            return false;
        }
    }

    return true;
}
void FileListForEach(FileList *list, FileListCallback callback)
{
    int list_len;
    int i;

    list_len = eaSize(list);
    for (i = 0; i < list_len; i++)
    {
        callback((*list)[i]->path, (*list)[i]->date);
    }
}

