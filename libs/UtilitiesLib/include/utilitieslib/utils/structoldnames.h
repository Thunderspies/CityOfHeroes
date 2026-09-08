/*\
 *
 *    structoldnames.h - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    All the #defines that allow use of the old deprecated textparser function names -
 *  time for shiny new ones
 *
 */

#ifndef STRUCTOLDNAMES_H
#define STRUCTOLDNAMES_H

#include "textparser_types.h"

C_DECLARATIONS_BEGIN

// structs and types in textparser.h
#define TokenizerTokenType        StructTokenType
#define TokenizerFormatOptions    StructFormatOptions
#define TokenizerParams            StructParams
#define TokenizerFunctionCall    StructFunctionCall
#define ParseLink                StructLink
#define StructTypeField        StructTypeField
#define TokenizerFormatField    StructFormatField

#define FORALL_PARSEINFO        FORALL_PARSETABLE

// functions in textparser.h
#define TokenizerParseList        ParserReadTokenizer
#define TokenizerErrorCallback    ParserErrorCallback

#define ParserDumpStructAllocs    StructDumpAllocs
#define    ParserAllocStruct        StructAllocRaw
#define ParserFreeStruct        StructFree
#define ParserAllocString        StructAllocString
#define ParserFreeString        StructFreeString
#define ParserFreeFunctionCall    StructFreeFunctionCall
#define ParserFreeFields        StructClear
#define ParserDestroyStruct        StructClear

#define ParserInitStruct        StructInit
#define ParserCopyStruct        StructCopyAll
#define ParserCompressStruct    StructCompress
#define ParserCRCStruct            StructCRC
#define ParserCompareStruct        StructCompare
#define ParserCompareFields        StructCompare
#define ParserCopyFields        StructCopy
#define ParserGetStructMemoryUsage    StructGetMemoryUsage

#define ParserCompareTokens        TokenCompare

#define ParserLinkToString        StructLinkToString
#define ParserLinkFromString    StructLinkFromString

#define ParserClearCachedInfo    ParseTableClearCachedInfo
#define ParserCountFields        ParseTableCountFields
#define ParserCRCFromParseInfo    ParseTableCRC

#define ParserExtractFromText    ParserReadTextEscaped
#define ParserEmbedText            ParserWriteTextEscaped
#define ParserSaveToRegistry    ParserWriteRegistry
#define ParserLoadFromRegistry    ParserReadRegistry

// textparserutils.h
#define ParserTokenSpecified    TokenIsSpecified
#define ParserCopyToken            TokenCopy
#define ParserIndexFromTokenString    ParseTableGetIndex
#define ParserAddSubStruct        TokenAddSubStruct

#define ParserOverrideStruct    StructOverride
#define ParserInterpToken        TokenInterpolate
#define ParserInterpStruct        StructInterpolate
#define ParserCalcRateForToken    TokenCalcRate
#define ParserCalcRateForStruct    StructCalcRate
#define ParserIntegrateToken    TokenIntegrate
#define ParserIntegrateStruct    StructIntegrate
#define ParserCalcCyclicValueForToken    TokenCalcCyclic
#define ParserCalcCyclicValueForStruct    StructCalcCyclic
#define ParserApplyDynOpToken    TokenApplyDynOp
#define ParserApplyDynOpStruct    StructApplyDynOp

#define ParseInfoFreeAll        ParseTableFree
#define ParseInfoWriteTextFile    ParseTableWriteTextFile
#define ParseInfoReadTextFile    ParseTableReadTextFile
#define ParseInfoSend            ParseTableSend
#define    ParseInfoRecv            ParseTableRecv

// structnet.h
#define sdPackDiff                ParserSend
#define sdPackEmptyDiff            ParserSendEmptyDiff
#define sdUnpackDiff            ParserRecv
#define sdFreePktIds            ParserFreePktIds
#define sdCopyStruct(tpi, src, dest) StructCopy(tpi, src, dest, 0, 0)
#define sdHasDiff(tpi, lhs, rhs) StructCompare(tpi, lhs, rhs)
#define sdFreeStruct(tpi, data) StructClear(tpi, data)

// structtokenizer.h
#define TextParserEscape TokenizerEscape
#define TextParserUnescape TokenizerUnescape

C_DECLARATIONS_END

#endif // STRUCTOLDNAMES_H
