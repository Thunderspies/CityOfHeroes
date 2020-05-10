// StructParser.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <cstdio>
#include "assert.h"
#include "tokenizer.h"
#include "structparser.h"
#include "windows.h"
#include "identifierdictionary.h"
#include "magiccommandmanager.h"
#include "strutils.h"
#include "filelistloader.h"
#include "sourceparser.h"
#include "autorunmanager.h"

#define MAX_TABS 32
#define TAB_WIDTH 4

#define NUMTABS(len) (MAX_TABS - (((int)(len)) + 1) / TAB_WIDTH)



#define AUTOSTRUCT_EXTRA_DATA "AST"
#define AUTOSTRUCT_EXCLUDE "NO_AST"

#define AUTOSTRUCT_IGNORE "AST_IGNORE"
#define AUTOSTRUCT_IGNORE_STRUCTPARAM "AST_IGNORE_STRUCTPARAM"

#define AUTOSTRUCT_START "AST_START"
#define AUTOSTRUCT_STOP "AST_STOP"

#define AUTOSTRUCT_STARTTOK "AST_STARTTOK"
#define AUTOSTRUCT_ENDTOK "AST_ENDTOK"
#define AUTOSTRUCT_SINGLELINE "AST_SINGLELINE"

#define AUTOSTRUCT_STRIP_UNDERSCORES "AST_STRIP_UNDERSCORES"
#define AUTOSTRUCT_NO_PREFIX_STRIPPING "AST_NO_PREFIX_STRIP"
#define AUTOSTRUCT_FORCE_USE_ACTUAL_FIELD_NAME "AST_FORCE_USE_ACTUAL_FIELD_NAME"
#define AUTOSTRUCT_CONTAINER "AST_CONTAINER"
#define AUTOSTRUCT_WIKI_COMMENT "WIKI"

#define STRUCTPARSER_PREFIX "parse_"

#define AUTOSTRUCT_MACRO "AST_MACRO"
#define AUTOSTRUCT_PREFIX "AST_PREFIX"
#define AUTOSTRUCT_SUFFIX "AST_SUFFIX"

#define AUTOSTRUCT_NONCONST_PREFIXSUFFIX "AST_NONCONST_PREFIXSUFFIX"

#define AUTOSTRUCT_NOT "AST_NOT"

#define AUTOSTRUCT_FOR_ALL "AST_FOR_ALL"

#define AUTOSTRUCT_COMMAND_BETWEEN_FIELDS "AST_COMMAND"

#define AUTO_STRUCT_FIXUP_FUNC "AST_FIXUPFUNC"

#define MAX_NOT_STRINGS 8


char gFileEndString[] = "END_OF_FILE";

static char const* sFloatNameList[] =
{
    "float",
    "F32",
    NULL
};

static char const* sIntNameList[] =
{
    "int",
    "U32",
    "S32",
    "U16",
    "S16",
    NULL
};

enum
{
    STRUCT_MAGICWORD,
    ENUM_MAGICWORD,
    AST_MACRO_MAGICWORD,
    AST_PREFIX_MAGICWORD,
    AST_SUFFIX_MAGICWORD,
    FIXUPFUNC_MAGICWORD,
};

typedef struct SpecialConstKeyword
{
    char const* pStartingString;
    char const* pNonConstString;
    enumDataType eDataType;
    enumDataStorageType eStorageType;
    enumDataReferenceType eReferenceType;
    bool bNeedToGetStructType;
} SpecialConstKeyword;

void FieldAssert(STRUCT_FIELD_DESC *pField, bool bCondition, char const* pErrorMessage)
{
    if (!bCondition)
    {
        printf("%s(%d) : error S0000 : (StructParser) %s\n", pField->fileName, pField->iLineNum, pErrorMessage);
        fflush(stdout);
        Sleep(100);
        exit(1);
    }
}

SpecialConstKeyword gSpecialConstKeywords[] = 
{
    {
        "CONST_EARRAY_OF",
        "EARRAY_OF",
        DATATYPE_STRUCT,
        STORAGETYPE_EARRAY,
        REFERENCETYPE_POINTER,
        true,
    },
    {
        "CONST_INT_EARRAY",
        "INT_EARRAY",
        DATATYPE_INT,
        STORAGETYPE_EMBEDDED,
        REFERENCETYPE_POINTER,
        false,
    },
    {
        "CONST_CONTAINERID_EARRAY",
        "CONTAINERID_EARRAY",
        DATATYPE_INT,
        STORAGETYPE_EMBEDDED,
        REFERENCETYPE_POINTER,
        false,
    },
    {
        "CONST_FLOAT_EARRAY",
        "FLOAT_EARRAY",
        DATATYPE_FLOAT,
        STORAGETYPE_EMBEDDED,
        REFERENCETYPE_POINTER,
        false,
    },
    {
        "CONST_OPTIONAL_STRUCT",
        "OPTIONAL_STRUCT",
        DATATYPE_STRUCT,
        STORAGETYPE_EMBEDDED,
        REFERENCETYPE_POINTER,
        true,
    },
    {
        "CONST_STRING_MODIFIABLE",
        "STRING_MODIFIABLE",
        DATATYPE_CHAR,
        STORAGETYPE_EMBEDDED,
        REFERENCETYPE_POINTER,
        false,
    },
    {
        "CONST_STRING_POOLED",
        "STRING_POOLED",
        DATATYPE_CHAR,
        STORAGETYPE_EMBEDDED,
        REFERENCETYPE_POINTER,
        false,
    },
    {
        "CONST_MULTIVAL",
        "MultiVal",
        DATATYPE_MULTIVAL,
        STORAGETYPE_EMBEDDED,
        REFERENCETYPE_DIRECT,
        false,
    },
    {
        "CONST_STRING_EARRAY",
        "STRING_EARRAY",
        DATATYPE_CHAR,
        STORAGETYPE_EARRAY,
        REFERENCETYPE_POINTER,
        false,
    },
};













StructParser::StructParser()
{
    m_iNumStructs = 0;
    m_iNumEnums = 0;
    m_iNumMacros = 0;

    m_pPrefix = NULL;
    m_pSuffix = NULL;
    m_ProjectName[0] = 0;
}

bool StructParser::DoesFileNeedUpdating(char const* pFileName)
{
    HANDLE hFile;

    char templateFileName[MAX_PATH];
    char templateHeaderFileName[MAX_PATH];

    TemplateFileNameFromSourceFileName(templateFileName, templateHeaderFileName, pFileName);


    hFile = CreateFileA(templateFileName, GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return true;
    }
    else 
    {
        CloseHandle(hFile);
    }

    hFile = CreateFileA(templateHeaderFileName, GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return true;
    }
    else 
    {
        CloseHandle(hFile);
    }


    return false;
}

void StructParser::DeleteStruct(int iIndex)
{
    int j;

    for (j=0; j < m_pStructs[iIndex]->iNumFields; j++)
    {
        if (m_pStructs[iIndex]->pStructFields[j]->iNumWikiComments)
        {
            int k;

            for (k=0; k < m_pStructs[iIndex]->pStructFields[j]->iNumWikiComments; k++)
            {
                delete m_pStructs[iIndex]->pStructFields[j]->pWikiComments[k];
            }
        }

        while (m_pStructs[iIndex]->pStructFields[j]->pFirstCommand)
        {
            STRUCT_COMMAND *pCommand = m_pStructs[iIndex]->pStructFields[j]->pFirstCommand;
            m_pStructs[iIndex]->pStructFields[j]->pFirstCommand = pCommand->pNext;

            delete pCommand->pCommandName;
            delete pCommand->pCommandString;
            if (pCommand->pCommandExpression)
            {
                delete pCommand->pCommandExpression;
            }
            delete pCommand;
        }

    
        while (m_pStructs[iIndex]->pStructFields[j]->pFirstBeforeCommand)
        {
            STRUCT_COMMAND *pCommand = m_pStructs[iIndex]->pStructFields[j]->pFirstBeforeCommand;
            m_pStructs[iIndex]->pStructFields[j]->pFirstBeforeCommand = pCommand->pNext;

            delete pCommand->pCommandName;
            delete pCommand->pCommandString;
            if (pCommand->pCommandExpression)
            {
                delete pCommand->pCommandExpression;
            }
            delete pCommand;
        }

        if (m_pStructs[iIndex]->pStructFields[j]->pIndexes)
        {
            delete m_pStructs[iIndex]->pStructFields[j]->pIndexes;
        }

        if (m_pStructs[iIndex]->pStructFields[j]->pFormatString)
        {
            delete m_pStructs[iIndex]->pStructFields[j]->pFormatString;
        }

        delete m_pStructs[iIndex]->pStructFields[j];

    }

    if (m_pStructs[iIndex]->pMainWikiComment)
    {
        delete m_pStructs[iIndex]->pMainWikiComment;
    }

    if (m_pStructs[iIndex]->pNonConstPrefixString)
    {
        delete m_pStructs[iIndex]->pNonConstPrefixString;
    }

    if (m_pStructs[iIndex]->pNonConstSuffixString)
    {
        delete m_pStructs[iIndex]->pNonConstSuffixString;
    }

    delete m_pStructs[iIndex];
}

StructParser::~StructParser()
{
    int i;

    for (i=0; i < m_iNumStructs; i++)
    {
        DeleteStruct(i);
        
    }

    for (i=0; i < m_iNumEnums; i++)
    {
        if (m_pEnums[i]->pEntries)
        {
            if (m_pEnums[i]->pEntries->pWikiComment)
            {
                delete m_pEnums[i]->pEntries->pWikiComment;
            }
            delete m_pEnums[i]->pEntries;
        }

        if (m_pEnums[i]->pMainWikiComment)
        {
            delete m_pEnums[i]->pMainWikiComment;
        }

        delete m_pEnums[i];
    }

    ResetMacros();
}

char const* StructParser::GetMagicWord(int iWhichMagicWord)
{
    switch (iWhichMagicWord)
    {
    case STRUCT_MAGICWORD: 
        return "AUTO_STRUCT";
    case ENUM_MAGICWORD:
        return "AUTO_ENUM";
    case AST_MACRO_MAGICWORD:
        return AUTOSTRUCT_MACRO;
    case AST_PREFIX_MAGICWORD:
        return AUTOSTRUCT_PREFIX;
    case AST_SUFFIX_MAGICWORD:
        return AUTOSTRUCT_SUFFIX;
    case FIXUPFUNC_MAGICWORD:
        return "AUTO_FIXUPFUNC";
    default:
        return "x x";
    }
}


void StructParser::FoundMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer, int iWhichMagicWord, char const* pMagicWordString)
{
    switch (iWhichMagicWord)
    {
    case MAGICWORD_BEGINNING_OF_FILE:
    case MAGICWORD_END_OF_FILE:
        ResetMacros();
        break;
        

    case STRUCT_MAGICWORD:
        FoundStructMagicWord(pSourceFileName, pTokenizer);
        break;

    case ENUM_MAGICWORD:
        FoundEnumMagicWord(pSourceFileName, pTokenizer);
        break;

    case AST_MACRO_MAGICWORD:
        FoundMacro(pSourceFileName, pTokenizer);
        break;

    case AST_PREFIX_MAGICWORD:
        FoundPrefix(pSourceFileName, pTokenizer);
        break;

    case AST_SUFFIX_MAGICWORD:
        FoundSuffix(pSourceFileName, pTokenizer);
        break;

    case FIXUPFUNC_MAGICWORD:
        FoundFixupFunc(pSourceFileName, pTokenizer);
        break;

    default:
        pTokenizer->Assert(0, "Got bad magic word somehow");
        break;
    }
}

void StructParser::FoundFixupFunc(char const* pSourceFileName, Tokenizer *pTokenizer)
{
    Token token;

    char *pFuncName;
    char *pTypeName;

    char funcBody[TOKENIZER_MAX_STRING_LENGTH];
    char defines[TOKENIZER_MAX_STRING_LENGTH];
    char autoRunFuncName[256];

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_SEMICOLON, "Expected ; after FIXUPFUNC");
    pTokenizer->AssertGetIdentifier("TextParserResult");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 0, "Expected func name after bool");
    pFuncName = STRDUP(token.sVal);
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after bool funcname");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 0, "Expected type name after funcname(");
    pTypeName = STRDUP(token.sVal);
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_ASTERISK, "Expected * after bool funcname(type");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 0, "Expected var name after funcname(type*");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after bool funcname(type*");
    pTokenizer->AssertGetIdentifier("enumTextParserFixupType");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 0, "Expected var name after funcname(type*, enumTextParserFixupType");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after bool funcname(type*, enumTextParserFixupType name");

    sprintf(funcBody, "\tParserSetTableFixupFunc(parse_%s, %s);\n", pTypeName, pFuncName);
    sprintf(defines, "extern ParseTable parse_%s[];\nTextParserResult %s(void *pStruct, enumTextParserFixupType eFixupType);\n", pTypeName, pFuncName);

    sprintf(autoRunFuncName, "_AUTORUN_RegisterFixupFunc%s", pTypeName);

    m_pParent->GetAutoRunManager()->AddAutoRunWithBody(autoRunFuncName, pSourceFileName, defines, funcBody, AUTORUN_ORDER_INTERNAL);

    delete(pFuncName);
    delete(pTypeName);
}



void StructParser::FoundPrefix(char const* pSourceFileName, Tokenizer *pTokenizer)
{
    Token token;
    if (m_pPrefix)
    {
        delete m_pPrefix;
    }
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AST_PREFIX");

    pTokenizer->GetSpecialStringTokenWithParenthesesMatching(&token);

    m_pPrefix = new char[token.iVal + 1];
    strcpy(m_pPrefix, token.sVal);


}

void StructParser::FoundSuffix(char const* pSourceFileName, Tokenizer *pTokenizer)
{
    Token token;
    if (m_pSuffix)
    {
        delete m_pSuffix;
    }
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AST_SUFFIX");

    pTokenizer->GetSpecialStringTokenWithParenthesesMatching(&token);

    m_pSuffix = new char[token.iVal + 1];
    strcpy(m_pSuffix, token.sVal);

}

void StructParser::FoundMacro(char const* pSourceFileName, Tokenizer *pTokenizer)
{
    Token token;

    int i;


    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AST_MACRO");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 0, "Expected identifier after AST_MACRO((");

    for (i=0; i < m_iNumMacros; i++)
    {
        if (strcmp(m_Macros[i].pIn, token.sVal) == 0)
        {
            //this macro already exists. Replace the pOut

            delete m_Macros[i].pOut;

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after AST_MACRO((x");

            pTokenizer->GetSpecialStringTokenWithParenthesesMatching(&token);

            m_Macros[i].pOut = new char[token.iVal + 1];
            strcpy(m_Macros[i].pOut, token.sVal);
            m_Macros[i].iOutLength = token.iVal;


            return;



        }
    }




    pTokenizer->Assert(m_iNumMacros < MAX_MACROS, "Too many macros");
    AST_MACRO_STRUCT *pCurMacro = &m_Macros[m_iNumMacros++];


    pCurMacro->pIn = new char[token.iVal + 1];
    strcpy(pCurMacro->pIn, token.sVal);
    pCurMacro->iInLength = token.iVal;

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after AST_MACRO((x");

    pTokenizer->GetSpecialStringTokenWithParenthesesMatching(&token);

    pCurMacro->pOut = new char[token.iVal + 1];
    strcpy(pCurMacro->pOut, token.sVal);
    pCurMacro->iOutLength = token.iVal;

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected )) after AST_MACRO((x,y");

}


void StructParser::FoundEnumMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer)
{
    Token token;
    enumTokenType eType;


    Tokenizer::StaticAssert(m_iNumEnums < MAX_ENUMS, "Too many enums");

    ENUM_DEF *pEnum = m_pEnums[m_iNumEnums++] = new ENUM_DEF;
    memset(pEnum, 0, sizeof(ENUM_DEF));


    do
    {
        eType = pTokenizer->GetNextToken(&token);
        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_SEMICOLON)
        {
            break;
        }

        if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "AEN_NO_PREFIX_STRIPPING") == 0)
        {
            pEnum->bNoPrefixStripping = true;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "AEN_APPEND_TO") == 0)
        {
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AEN_APPEND_TO");
            pTokenizer->Assert(pEnum->enumToAppendTo[0] == 0, "Can't have two AEN_APPEND_TOs in one ENUM");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH - 1, "Expected identifier after AEN_APPEND_TO(");
            strcpy(pEnum->enumToAppendTo, token.sVal);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AEN_APPEND_TO(x");
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "AEN_WIKI") == 0)
        {
            pTokenizer->Assert(pEnum->pMainWikiComment == NULL, "Can't have two AEN_WIKIs for one AUTO_ENUM");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AEN_WIKI");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Expected string after AEN_WIKI(");

            if (token.sVal[0])
            {
                pEnum->pMainWikiComment = new char[token.iVal + 1];
                strcpy(pEnum->pMainWikiComment, token.sVal);
            }

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AEN_WIKI(\"x\"");

        }
        else
        {
            pTokenizer->Assert(0, "Unrecognized command after AUTO_ENUM... missing semicolon?");
        }
    } while (1);


    
    strcpy(pEnum->sourceFileName, pSourceFileName);

    eType = pTokenizer->GetNextToken(&token);

    if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_TYPEDEF)
    {
        eType = pTokenizer->GetNextToken(&token);
    }

    pTokenizer->Assert(eType == TOKEN_RESERVEDWORD && token.iVal == RW_ENUM, "Expected [typedef] enum after AUTO_ENUM");

    eType = pTokenizer->GetNextToken(&token);

    if (eType == TOKEN_IDENTIFIER)
    {
        pTokenizer->Assert(token.iVal < MAX_NAME_LENGTH, "enum name too long");
        strcpy(pEnum->enumName, token.sVal);

        eType = pTokenizer->GetNextToken(&token);
    }

    pTokenizer->Assert(eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTBRACE, "Expected { after enum");

    do
    {
        eType = pTokenizer->GetNextToken(&token);

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTBRACE)
        {
            break;
        }

        pTokenizer->Assert(eType == TOKEN_IDENTIFIER, "Expected identifier as enum entry name");
        pTokenizer->Assert(token.iVal < MAX_NAME_LENGTH, "identifier entry name too long");

        ENUM_ENTRY *pEntry = GetNewEntry(pEnum);

        strcpy(pEntry->inCodeName, token.sVal);

        if (pEnum->pMainWikiComment)
        {
            pTokenizer->GetSurroundingSlashedCommentBlock(&token);

            if (token.sVal[0])
            {
                pEntry->pWikiComment = STRDUP(token.sVal);
            }
        }

        do
        {
            eType = pTokenizer->GetNextToken(&token);
        }
        while (!(eType == TOKEN_RESERVEDWORD && (token.iVal == RW_RIGHTBRACE || token.iVal == RW_COMMA)));

        if (token.iVal == RW_RIGHTBRACE)
        {
            break;
        }

        eType = pTokenizer->CheckNextToken(&token);

        if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "EIGNORE") == 0)
        {
            pEnum->iNumEntries--;
            pTokenizer->GetNextToken(&token);
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ENAMES") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ENAMES");

            //allow colons and periods in ENAMES
            pTokenizer->SetExtraCharsAllowedInIdentifiers(":.");

            do
            {
                eType = pTokenizer->GetNextToken(&token);

                if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTPARENS)
                {
                    break;
                }

                pTokenizer->Assert(eType == TOKEN_IDENTIFIER, "Expected identifier inside ENAMES");

                pTokenizer->Assert(pEntry->iNumExtraNames < MAX_ENUM_EXTRA_NAMES, "Too many extra names");

                pTokenizer->Assert(token.iVal < MAX_NAME_LENGTH, "extra name too long");

                strcpy(pEntry->extraNames[pEntry->iNumExtraNames++], token.sVal);

                eType = pTokenizer->CheckNextToken(&token);
                if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_OR)
                {
                    pTokenizer->GetNextToken(&token);
                }
            } while (1);

            pTokenizer->SetExtraCharsAllowedInIdentifiers(NULL);


        }
    }
    while (1);

    if (pEnum->enumName[0] == 0)

    {
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH, "Expected identifier after enum { ... }");
        strcpy(pEnum->enumName, token.sVal);
    }
    else
    {
        eType = pTokenizer->GetNextToken(&token);

        if (eType == TOKEN_IDENTIFIER)
        {
            pTokenizer->Assert(strcmp(token.sVal, pEnum->enumName) == 0, "enum names at beginning and end of enum must match precisely");
        }
    }

    m_pParent->GetDictionary()->AddIdentifier(pEnum->enumName, pSourceFileName, IDENTIFIER_ENUM);
}
    
#define NUM_ENTRIES_TO_ALLOCATE_AT_ONCE 8

StructParser::ENUM_ENTRY *StructParser::GetNewEntry(ENUM_DEF *pEnum)
{
    if (pEnum->iNumEntries == pEnum->iNumAllocatedEntries)
    {
        ENUM_ENTRY *pNewEntries = new ENUM_ENTRY[pEnum->iNumAllocatedEntries + NUM_ENTRIES_TO_ALLOCATE_AT_ONCE];
        Tokenizer::StaticAssert(pNewEntries != NULL, "new failed");

        memset(pNewEntries, 0, sizeof(ENUM_ENTRY) * (pEnum->iNumAllocatedEntries + NUM_ENTRIES_TO_ALLOCATE_AT_ONCE));

        if (pEnum->iNumAllocatedEntries)
        {
            memcpy(pNewEntries, pEnum->pEntries, sizeof(ENUM_ENTRY) * pEnum->iNumAllocatedEntries);
            delete(pEnum->pEntries);
        }

        pEnum->iNumAllocatedEntries += NUM_ENTRIES_TO_ALLOCATE_AT_ONCE;
    
        pEnum->pEntries = pNewEntries;
    }

    return pEnum->pEntries + (pEnum->iNumEntries++);
}



void StructParser::FoundStructMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer)
{
    Token token;
    enumTokenType eType;

    STRUCT_COMMAND *pCurBeforeCommands = NULL;

    Tokenizer::StaticAssert(m_iNumStructs < MAX_STRUCTS, "Too many structs");

    STRUCT_DEF *pStruct = m_pStructs[m_iNumStructs++] = new STRUCT_DEF;

    memset(pStruct, 0, sizeof(STRUCT_DEF));
    

    eType = pTokenizer->GetNextToken(&token);

    while (eType == TOKEN_IDENTIFIER)
    {
        if (strcmp(token.sVal, AUTOSTRUCT_IGNORE) == 0 || strcmp(token.sVal, AUTOSTRUCT_IGNORE_STRUCTPARAM) == 0)
        {
            bool bStructParam = (strcmp(token.sVal, AUTOSTRUCT_IGNORE_STRUCTPARAM) == 0);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AST_IGNORE");
            eType = pTokenizer->GetNextToken(&token);
            pTokenizer->Assert(eType == TOKEN_IDENTIFIER || eType == TOKEN_STRING, "Expected string or identifier after AST_IGNORE(");
            pTokenizer->Assert(pStruct->iNumIgnores < MAX_IGNORES, "Too many ignores");
            pTokenizer->Assert(strlen(token.sVal) < MAX_NAME_LENGTH, "Ignore string too long");
            
            pStruct->bIgnoresAreStructParam[pStruct->iNumIgnores] = bStructParam;
            strcpy(pStruct->ignores[pStruct->iNumIgnores++], token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AST_IGNORE(x");
        }
        else if (strcmp(token.sVal, AUTOSTRUCT_STARTTOK) == 0)
        {
            pStruct->bHasStartString = true;
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AST_STARTTOK ");
            eType = pTokenizer->GetNextToken(&token);
            pTokenizer->Assert(eType == TOKEN_IDENTIFIER || eType == TOKEN_STRING, "Expected identifier or string after AST_STARTTOK(");
            pTokenizer->Assert(strlen(token.sVal) < MAX_NAME_LENGTH - 1, "AST_STARTTOK string too long");
            strcpy(pStruct->startString, token.sVal);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AST_STARTTOK(x ");

        }
        else if (strcmp(token.sVal, AUTOSTRUCT_ENDTOK) == 0)
        {
            pStruct->bHasEndString = true;
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AST_ENDTOK ");
            eType = pTokenizer->GetNextToken(&token);
            pTokenizer->Assert(eType == TOKEN_IDENTIFIER || eType == TOKEN_STRING, "Expected identifier or string after AST_ENDTOK(");
            pTokenizer->Assert(strlen(token.sVal) < MAX_NAME_LENGTH - 1, "AST_ENDTOK string too long");
            strcpy(pStruct->endString, token.sVal);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AST_ENDTOK(x ");

        }
        else if (strcmp(token.sVal, AUTOSTRUCT_SINGLELINE) == 0)
        {
            pStruct->bHasStartString = true;
            strcpy(pStruct->startString, "");
            pStruct->bHasEndString = true;
            strcpy(pStruct->endString, "\\n");
            pStruct->bForceSingleLine = true;

        }
        else if (strcmp(token.sVal, AUTOSTRUCT_STRIP_UNDERSCORES) == 0)
        {
            pStruct->bStripUnderscores = true;
        }
        else if (strcmp(token.sVal, AUTOSTRUCT_NO_PREFIX_STRIPPING) == 0)
        {
            pStruct->bNoPrefixStripping = true;
        }
        else if (strcmp(token.sVal, AUTOSTRUCT_FORCE_USE_ACTUAL_FIELD_NAME) == 0)
        {
            pStruct->bForceUseActualFieldName = true;
            pStruct->bNoPrefixStripping = false;
        }
        else if (strcmp(token.sVal, AUTOSTRUCT_CONTAINER) == 0)
        {
            pStruct->bIsContainer = true;

            if (!pStruct->bForceUseActualFieldName)
            {
                pStruct->bNoPrefixStripping = true;
                pStruct->bAlwaysIncludeActualFieldNameAsRedundant = true;
            }
        }
        else if (strcmp(token.sVal, AUTO_STRUCT_FIXUP_FUNC) == 0)
        {
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AST_FIXUPFUNC");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH-1, "Expected string after AST_FIXUPFUNC");

            strcpy(pStruct->fixupFuncName, token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AST_FIXUPFUNC(x");
        }
        else if (strcmp(token.sVal, AUTOSTRUCT_WIKI_COMMENT) == 0)
        {
            pTokenizer->Assert(pStruct->pMainWikiComment == NULL, "Can't have two WIKI comments for the same struct");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after WIKI");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Expected string after WIKI(");

            pStruct->pMainWikiComment = new char[token.iVal + 1];
            strcpy(pStruct->pMainWikiComment, token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after WIKI");


        }
        else if (strcmp(token.sVal, AUTOSTRUCT_FOR_ALL) == 0)
        {
            pTokenizer->Assert(pStruct->forAllString[0] == 0, "Can't have two AST_FOR_ALLs");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AST_FOR_ALL");
            pTokenizer->GetSpecialStringTokenWithParenthesesMatching(&token);
            strcpy(pStruct->forAllString, token.sVal);
        }
        else if (strcmp(token.sVal, AUTOSTRUCT_NONCONST_PREFIXSUFFIX) == 0)
        {
            pTokenizer->Assert(pStruct->pNonConstPrefixString == NULL, "can't have two AST_NONCONST_PREFIXSUFFIX");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AST_NONCONST_PREFIXSUFFIX");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Expected string after AST_NONCONST_PREFIXSUFFIX(");
            pStruct->pNonConstPrefixString = new char[token.iVal + 1];
            RemoveCStyleEscaping(pStruct->pNonConstPrefixString, token.sVal);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after AST_NONCONST_PREFIXSUFFIX(x");
        
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Expected string after AST_NONCONST_PREFIXSUFFIX(x,");
            pStruct->pNonConstSuffixString = new char[token.iVal + 1];
            RemoveCStyleEscaping(pStruct->pNonConstSuffixString, token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AST_NONCONST_PREFIXSUFFIX(x,y");
        }
        else
        {
            pTokenizer->Assert(0, "Expected AUTO_STRUCT [AST_IGNORE ...] [AST_START ...] [AST_END ...] [AST_STRIP_UNDERSCORES] [WIKI(\"...\")]; [typedef] struct");
        }


        eType = pTokenizer->GetNextToken(&token);

    }
    
    pTokenizer->Assert(eType == TOKEN_RESERVEDWORD && token.iVal == RW_SEMICOLON, "Expected semicolon after AUTO_STRUCT and its commands");

    pStruct->iPreciseStartingOffsetInFile = pTokenizer->GetOffset(&pStruct->iPreciseStartingLineNumInFile);

    eType = pTokenizer->GetNextToken(&token);


    if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_TYPEDEF)
    {
        eType = pTokenizer->GetNextToken(&token);
    }

    pTokenizer->Assert(eType == TOKEN_RESERVEDWORD && token.iVal == RW_STRUCT, "Expected AUTO_STRUCT [typedef] struct");

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 0, "Expected struct name after 'struct'");

    pTokenizer->Assert(strlen(token.sVal) < MAX_NAME_LENGTH, "struct name too long");

    strcpy(pStruct->structName, token.sVal);

    strcpy(pStruct->sourceFileName, pSourceFileName);

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTBRACE, "Expected { after struct name");


    do
    {
        eType = pTokenizer->CheckNextToken(&token);

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTBRACE)
        {
            if (pStruct->bCurrentlyInsideUnion)
            {
                UnionStruct *pCurUnion = &pStruct->unions[pStruct->iNumUnions - 1];

                pTokenizer->GetNextToken(&token);
                eType = pTokenizer->GetNextToken(&token);
                if (eType == TOKEN_IDENTIFIER)
                {
                    pTokenizer->Assert(token.iVal < MAX_NAME_LENGTH, "Union name too long");
                    strcpy(pCurUnion->name, token.sVal);
                    eType = pTokenizer->GetNextToken(&token);
                }

                pTokenizer->Assert(eType == TOKEN_RESERVEDWORD && token.iVal == RW_SEMICOLON, "Expected ; after union");
                pStruct->bCurrentlyInsideUnion = false;
                pCurUnion->iLastFieldNum = pStruct->iNumFields - 1;

                //assert that this union has at least all-but-one of its fields redundant
                int iNumRedundantFields = 0;

                int i;

                for (i = pCurUnion->iFirstFieldNum; i <= pCurUnion->iLastFieldNum; i++)
                {
                    if (pStruct->pStructFields[i]->bFoundRedundantToken)
                    {
                        iNumRedundantFields++;
                    }
                }

                pTokenizer->Assert(pCurUnion->iLastFieldNum - pCurUnion->iFirstFieldNum + 1 - iNumRedundantFields <= 1, "Only one non-redundant field allowed in a union");
                continue;
            }

            break;
        }

        if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "union") == 0)
        {
            //first, check if this union is NO_AST, which means looking past nested braces to the next ;, then seeing
            //if the very next token is NO_AST
            int iBraceDepth = 0;

            pTokenizer->SaveLocation();

            do
            {
                eType = pTokenizer->GetNextToken(&token);
            
                pTokenizer->Assert(eType != TOKEN_NONE, "EOF found inside union");

                if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTBRACE)
                {
                    iBraceDepth++;
                }
                else if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTBRACE)
                {
                    iBraceDepth--;
                    pTokenizer->Assert(iBraceDepth >= 0, "Mismatched braces found inside union");
                }
            }
            while (!( iBraceDepth == 0 && eType == TOKEN_RESERVEDWORD && token.iVal == RW_SEMICOLON));

            eType = pTokenizer->GetNextToken(&token);

            if (!(eType == TOKEN_IDENTIFIER && strcmp(token.sVal, AUTOSTRUCT_EXCLUDE) == 0))
            {
                pTokenizer->RestoreLocation();

                pTokenizer->GetNextToken(&token);

                pTokenizer->Assert(!pStruct->bCurrentlyInsideUnion, "StructParser doesn't support nested unions");
                pTokenizer->Assert(pStruct->iNumUnions < MAX_UNIONS, "Too many unions in one struct");

                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTBRACE, "Expected { after union");

                pStruct->bCurrentlyInsideUnion = true;
                pStruct->unions[pStruct->iNumUnions].iFirstFieldNum = pStruct->iNumFields;
                pStruct->iNumUnions++;
            }
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, AUTOSTRUCT_STOP) == 0)
        {
            int iBraceDepth = 0;
            do
            {
                eType = pTokenizer->CheckNextToken(&token);

                if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTBRACE)
                {
                    iBraceDepth++;
                }
                else if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTBRACE)
                {
                    if (iBraceDepth == 0)
                    {
                        break;
                    }
                    else
                    {
                        iBraceDepth--;
                    }
                }

                pTokenizer->GetNextToken(&token);



            }
            while (!(eType == TOKEN_IDENTIFIER && strcmp(token.sVal, AUTOSTRUCT_START) == 0));
        }
        else if (eType == TOKEN_RESERVEDWORD && strcmp(token.sVal, AUTOSTRUCT_MACRO) == 0)
        {
            pTokenizer->GetNextToken(&token);
            FoundMacro(pSourceFileName, pTokenizer);
        }
        else if (eType == TOKEN_RESERVEDWORD && strcmp(token.sVal, AUTOSTRUCT_PREFIX) == 0)
        {
            pTokenizer->GetNextToken(&token);
            FoundPrefix(pSourceFileName, pTokenizer);
        }
        else if (eType == TOKEN_RESERVEDWORD && strcmp(token.sVal, AUTOSTRUCT_SUFFIX) == 0)
        {
            pTokenizer->GetNextToken(&token);
            FoundSuffix(pSourceFileName, pTokenizer);
        }
        else if (strcmp(token.sVal, AUTOSTRUCT_COMMAND_BETWEEN_FIELDS) == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AST_COMMAND");

            STRUCT_COMMAND *pCommand = new STRUCT_COMMAND;

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Expected command name after AST_COMMAND(");

            pCommand->pCommandName = new char[token.iVal + 1];
            strcpy(pCommand->pCommandName, token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after AST_COMMAND(name");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Expected command string after AST_COMMAND(name,");

            pCommand->pCommandString = new char[token.iVal + 1];
            strcpy(pCommand->pCommandString, token.sVal);

            pTokenizer->Assert2NextTokenTypesAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, 
                TOKEN_RESERVEDWORD, RW_COMMA, "Expected ) or , after AST_COMMAND(name, command");

            if (token.iVal == RW_COMMA)
            {
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Expected command expression string after AST_COMMAND(name, command,");
                pCommand->pCommandExpression = STRDUP(token.sVal);

                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, 
                     "Expected ) after AST_COMMAND(name, command, expression");
            }
            else
            {
                pCommand->pCommandExpression = NULL;
            }


            if (pStruct->iNumFields)
            {
                STRUCT_FIELD_DESC *pLastField = pStruct->pStructFields[pStruct->iNumFields-1];

                pCommand->pNext = pLastField->pFirstCommand;
                pLastField->pFirstCommand = pCommand;
            }
            else
            {
                pCommand->pNext = pCurBeforeCommands;
                pCurBeforeCommands = pCommand;
            }
        }
        else
        {
            pTokenizer->Assert(pStruct->iNumFields < MAX_FIELDS, "too many fields in one struct");
            
            pStruct->pStructFields[pStruct->iNumFields] = new STRUCT_FIELD_DESC;
            memset(pStruct->pStructFields[pStruct->iNumFields], 0, sizeof(STRUCT_FIELD_DESC));

            if (ReadSingleField(pTokenizer, pStruct->pStructFields[pStruct->iNumFields], pStruct))
            {
                pStruct->iNumFields++;
            }
            else
            {
                delete(pStruct->pStructFields[pStruct->iNumFields]);
            }

            if (pStruct->iNumFields == 1 && pCurBeforeCommands)
            {
                pStruct->pStructFields[0]->pFirstBeforeCommand = pCurBeforeCommands;
                pCurBeforeCommands = NULL;
            }
        }

    } while (1);

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTBRACE, "Expected } after struct");
    eType = pTokenizer->CheckNextToken(&token);

    if (eType == TOKEN_IDENTIFIER)
    {
        pTokenizer->Assert(strcmp(token.sVal, pStruct->structName) == 0, "AUTO_STRUCT requires the same struct name after typedef struct and after the entire struct");
    }

    CheckOverallStructValidity(pTokenizer, pStruct);

    m_pParent->GetDictionary()->AddIdentifier(pStruct->structName, pSourceFileName, 
        pStruct->bIsContainer ? IDENTIFIER_STRUCT_CONTAINER : IDENTIFIER_STRUCT);
}

void StructParser::CheckOverallStructValidity(Tokenizer *pTokenizer, STRUCT_DEF *pStruct)
{
    int i;
    bool bFoundPersistedField = false;

    pTokenizer->Assert(pStruct->iNumFields > 0, "Structs must have at least one field");

    for (i=0; i < pStruct->iNumFields; i++)
    {
        STRUCT_FIELD_DESC *pField = pStruct->pStructFields[i];

        if (pField->bFoundPersist)
        {
            bFoundPersistedField = true;
        }
    }

    if (bFoundPersistedField)
    {
        pTokenizer->Assert(pStruct->bIsContainer, "Found a persisted field in a non-container struct");
    }
    else
    {
        pTokenizer->Assert(!pStruct->bIsContainer, "A container struct must have at least one persisted field");
    }
}

void StructParser::CheckOverallStructValidity_PostFixup(STRUCT_DEF *pStruct)
{
    int i;

    if (pStruct->structNameIInheritFrom[0])
    {
        FieldAssert(pStruct->pStructFields[0], pStruct->pStructFields[0]->eDataType == DATATYPE_STRUCT
            && pStruct->pStructFields[0]->eStorageType == STORAGETYPE_EMBEDDED
            && pStruct->pStructFields[0]->eReferenceType == REFERENCETYPE_DIRECT,
            "POLYCHILDTYPE only legal on an embedded struct");
    }

    for (i=0; i < pStruct->iNumFields; i++)
    {
        STRUCT_FIELD_DESC *pField = pStruct->pStructFields[i];

        if (pField->bFoundPersist)
        {
            if (!pField->bFoundNoTransact)
            {            
                //a persisted field must have a const keyword, or be am embedded float, int, bool or enum
                if (!(pField->bFoundSpecialConstKeyword
                    || pField->bFoundConst 
                        && (pField->eDataType == DATATYPE_INT || pField->eDataType == DATATYPE_FLOAT
                            || pField->eDataType == DATATYPE_CHAR || pField->eDataType == DATATYPE_ENUM
                    || pField->eDataType == DATATYPE_BIT || pField->eDataType == DATATYPE_NONE
                    || pField->eDataType == DATATYPE_FLAGS || pField->eDataType == DATATYPE_BOOLFLAG
                    || pField->eDataType == DATATYPE_MAT3 || pField->eDataType == DATATYPE_MAT4
                    || pField->eDataType == DATATYPE_VEC3 || pField->eDataType == DATATYPE_VEC4 
                    || pField->eDataType == DATATYPE_QUAT)
                        && (pField->eStorageType == STORAGETYPE_EMBEDDED || pField->eStorageType == STORAGETYPE_ARRAY)
                        && pField->eReferenceType == REFERENCETYPE_DIRECT
                    || pField->eDataType == DATATYPE_STRUCT 
                        && pField->eStorageType == STORAGETYPE_EMBEDDED
                        && pField->eReferenceType == REFERENCETYPE_DIRECT
                        && m_pParent->GetDictionary()->FindIdentifier(pField->typeName) == IDENTIFIER_STRUCT_CONTAINER))
                {
                    char errorString[1024];
                    sprintf(errorString, "A persisted field must be a simple const type, or one of the special CONST_ macros, or an embedded container. Found a bad one of type %d, storage type %d ref type %d\n", pField->eDataType, pField->eStorageType, pField->eReferenceType);
                    FieldAssert(pField, 0, errorString);
                }
            }
        }
    }
}



bool StructParser::ReadSingleField(Tokenizer *pTokenizer, STRUCT_FIELD_DESC *pField, STRUCT_DEF *pStruct)
{
    Token token;
    enumTokenType eType;

    bool bExclude = false;

    //first, check for whether there is a NO_AST immediately after the next semicolon. If so, ignore this whole thing so we don't
    //have parse errors when things we don't understand are NO_ASTed.
    pTokenizer->SaveLocation();
    do
    {
        eType = pTokenizer->GetNextToken(&token);

    } while (!(eType == TOKEN_RESERVEDWORD && token.iVal == RW_SEMICOLON));
    
    eType = pTokenizer->GetNextToken(&token);

    if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, AUTOSTRUCT_EXCLUDE) == 0)
    {
        return false;
    }

    pTokenizer->RestoreLocation();

    if(pStruct->bForceSingleLine)
        pField->bFoundStructParam = true;

    eType = pTokenizer->CheckNextToken(&token);
    
    //check for initial "const"
    if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "const") == 0)
    {
        pField->bFoundConst = true;
        eType = pTokenizer->GetNextToken(&token);
    }


    //now check for one of the "special const" tokens
    if (eType == TOKEN_IDENTIFIER)
    {
        int i;

        for (i=0; i < sizeof(gSpecialConstKeywords) / sizeof(gSpecialConstKeywords[0]); i++)
        {
            if (strcmp(token.sVal, gSpecialConstKeywords[i].pStartingString) == 0)
            {
                pTokenizer->GetNextToken(&token);
                pField->bFoundSpecialConstKeyword = true;
                pField->eDataType = gSpecialConstKeywords[i].eDataType;
                pField->eStorageType = gSpecialConstKeywords[i].eStorageType;
                pField->eReferenceType = gSpecialConstKeywords[i].eReferenceType;

                if (gSpecialConstKeywords[i].bNeedToGetStructType)
                {
                    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after special const keyword");
                    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH, "Expected const or identifier");
                    if (strcmp(token.sVal, "const") == 0)
                    {
                        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH, "Expected identifier");
                    }
                    strcpy(pField->typeName, token.sVal);
                    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected )");
                }

                break;
            }
        }
    }


    if (!pField->bFoundSpecialConstKeyword)
    {

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_VOID)
        {
            pTokenizer->GetNextToken(&token);
            pField->eDataType = DATATYPE_VOID;
            sprintf(pField->typeName, "void");
        }
        else if (eType == TOKEN_IDENTIFIER && (strcmp(token.sVal, "REF_TO") == 0 || strcmp(token.sVal, "CONST_REF_TO") == 0))
        {
            pTokenizer->GetNextToken(&token);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after REF_TO");

            eType = pTokenizer->CheckNextToken(&token);

            if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "const") == 0)
            {
                pTokenizer->GetNextToken(&token);
            }

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH, "expected identifier after REF_TO(");
            
            AttemptToDeduceReferenceDictionaryName(pField, token.sVal);

            strcpy(pField->typeName, token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after REF_TO");

            pField->eDataType = DATATYPE_REFERENCE;

            if (strcmp(token.sVal, "CONST_REF_TO") )
            {
                pField->bFoundSpecialConstKeyword = true;
            }
        }
        else
        {
                
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH, "Couldn't find field name");

            strcpy(pField->typeName, token.sVal);
        }
    }




    pField->iLineNum = pTokenizer->GetCurLineNum();
    strcpy(pField->fileName, pTokenizer->GetCurFileName());

    eType = pTokenizer->CheckNextToken(&token);

    

    while ((eType == TOKEN_RESERVEDWORD && token.iVal == RW_ASTERISK) ||
        eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "const") == 0)
    {
        if (eType == TOKEN_RESERVEDWORD)
        {
            pField->iNumAsterisks++;
            pTokenizer->GetNextToken(&token);
            eType = pTokenizer->CheckNextToken(&token);
        }
        else
        {
            pField->bFoundConst = true;
            pTokenizer->GetNextToken(&token);
            eType = pTokenizer->CheckNextToken(&token);
        }
    }

    pTokenizer->Assert(pField->iNumAsterisks < 3, "Don't know how to deal with more than two asterisks");

    if (pField->bFoundSpecialConstKeyword)
    {
        pTokenizer->Assert(pField->iNumAsterisks == 0 && !pField->bFoundConst, "After a special const token, * and const are illegal");
    }

    pTokenizer->Assert(eType == TOKEN_IDENTIFIER, "Expected identifier for field name after field type");

    pTokenizer->Assert(strlen(token.sVal) < MAX_NAME_LENGTH, "field name too long");

    strcpy(pField->baseStructFieldName, token.sVal);
    strcpy(pField->curStructFieldName, token.sVal);
    strcpy(pField->userFieldName, token.sVal);

    pTokenizer->GetNextToken(&token);

    do
    {
        eType = pTokenizer->GetNextToken(&token);

        pTokenizer->Assert(eType == TOKEN_RESERVEDWORD && (token.iVal == RW_LEFTBRACKET || token.iVal == RW_SEMICOLON 
            || token.iVal == RW_COLON), "Expected [, : or ; after field name");

        if (token.iVal == RW_COLON)
        {
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Expected int after :");
            pTokenizer->Assert(token.iVal == 1, "Bitfields are only supported of size 1");
            pField->bBitField = true;
        } 
        else if (token.iVal == RW_LEFTBRACKET)
        {
            pTokenizer->Assert(!pField->bFoundSpecialConstKeyword, "Can't have [] after special const keyword");

            pField->iArrayDepth++;

            pTokenizer->SaveLocation();

            do
            {
                eType = pTokenizer->GetNextToken(&token);

                if (eType == TOKEN_NONE)
                {
                    pTokenizer->RestoreLocation();
                    pTokenizer->Assert(0, "Never found ]");
                }

                if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTBRACKET)
                {
                    break;
                }

                pTokenizer->StringifyToken(&token);

                char tempString[TOKENIZER_MAX_STRING_LENGTH + MAX_NAME_LENGTH + 1];

                sprintf(tempString, "%s %s", pField->arraySizeString, token.sVal);

                pTokenizer->Assert(strlen(tempString) < MAX_NAME_LENGTH, "tokens inside [] are too long");

                strcpy(pField->arraySizeString, tempString);
            } 
            while (!(eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTBRACKET));
        }
        else
        {
            break;
        }
    }
    while (1);


    int iNumNotStrings = 0;
    char *pNotStrings[MAX_NOT_STRINGS];
    int i;

    eType = pTokenizer->CheckNextToken(&token);

    while (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, AUTOSTRUCT_NOT) == 0)
    {
        pTokenizer->GetNextToken(&token);
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AST_NOT");

        pTokenizer->GetSpecialStringTokenWithParenthesesMatching(&token);

        pTokenizer->Assert(iNumNotStrings < MAX_NOT_STRINGS, "Too many AST_NOTs");

        pNotStrings[iNumNotStrings] = new char[token.iVal + 1];
        strcpy(pNotStrings[iNumNotStrings++], token.sVal);

        eType = pTokenizer->CheckNextToken(&token);
    }

    if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, AUTOSTRUCT_EXTRA_DATA) == 0)
    {
        pTokenizer->GetNextToken(&token);
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AST");
        
        pTokenizer->GetSpecialStringTokenWithParenthesesMatching(&token);

        ProcessStructFieldCommandString(pStruct, pField, token.sVal, iNumNotStrings, pNotStrings, pTokenizer->GetCurFileName(), pTokenizer->GetCurLineNum(), pTokenizer);
    }
    else
    {
        ProcessStructFieldCommandString(pStruct, pField, " ", iNumNotStrings, pNotStrings, pTokenizer->GetCurFileName(), pTokenizer->GetCurLineNum(), pTokenizer);
    }

    for (i=0; i < iNumNotStrings; i++)
    {
        free(pNotStrings[i]);
    }

    FixupFieldName(pField, pStruct->bStripUnderscores, pStruct->bNoPrefixStripping, 
        pStruct->bForceUseActualFieldName, pStruct->bAlwaysIncludeActualFieldNameAsRedundant);

    return true;
}
    
typedef enum AstParam
{
    RW_NAME = RW_COUNT,
    RW_ADDNAMES,

    //reserved words for formatting must all be sequential and in order
    RW_FORMAT_IP,
    RW_FORMAT_KBYTES,
    RW_FORMAT_FRIENDLYDATE,
    RW_FORMAT_FRIENDLYSS2000,
    RW_FORMAT_DATESS2000,
    RW_FORMAT_FRIENDLYCPU,
    RW_FORMAT_PERCENT,
    RW_FORMAT_NOPATH,
    RW_FORMAT_HSV,
    RW_FORMAT_TEXTURE,

    RW_LVWIDTH,

    //reserved words for format flags must all be sequential and in order
    RW_FORMAT_FLAG_UI_LEFT,
    RW_FORMAT_FLAG_UI_RIGHT,
    RW_FORMAT_FLAG_UI_RESIZABLE,
    RW_FORMAT_FLAG_UI_NOTRANSLATE_HEADER,
    RW_FORMAT_FLAG_UI_NOHEADER,
    RW_FORMAT_FLAG_UI_NODISPLAY,

    RW_FILENAME,
    RW_CURRENTFILE,
    RW_TIMESTAMP,
    RW_LINENUM,

    RW_MINBITS,
    RW_PRECISION,

    RW_FLOAT_HUNDRETHS,
    RW_FLOAT_TENTHS,
    RW_FLOAT_ONES,
    RW_FLOAT_FIVES,
    RW_FLOAT_TENS,

    RW_DEF,
    RW_DEFAULT,

    RW_FLAGS,
    RW_BOOLFLAG,
    RW_USEDFIELD,

    RW_RAW,
    RW_POINTER,
    RW_INT,

    RW_VEC3,
    RW_VEC2,
    RW_RGB,
    RW_RGBA,
    RW_RG,

    RW_SUBTABLE,
    RW_ALLCAPSSTRUCT,
    RW_UNOWNED,

    RW_INDEX,
    RW_AUTO_INDEX,

    RW_WIKI,

    RW_REDUNDANT_STRUCT,

    RW_EMBEDDED_FLAT,

    RW_USERFLAG,

    RW_REFDICT,

    RW_REDUNDANT,
    RW_STRUCTPARAM,
    

    RW_PERSIST,
    RW_NOTRANSACT,
    // End of bit flags

    RW_REQUESTINDEXDEFINE,

    RW_COMMAND,

    RW_POLYCHILDTYPE,
    RW_POLYPARENTTYPE,

    RW_FORCECONTAINER,
    RW_FORCENOTCONTAINER,

    RW_LATEBIND,

    RW_WIKILINK,

    RW_FORMATSTRING,
} AstParam;

#define FIRST_FORMAT_RW RW_FORMAT_IP
#define FIRST_FORMAT_FLAG_RW RW_FORMAT_FLAG_UI_LEFT

#define FIRST_FLOAT_ACCURACY_RW RW_FLOAT_HUNDRETHS
#define LAST_FLOAT_ACCURACY_RW RW_FLOAT_TENS

char const* sFieldCommandStringReservedWords[] =
{
    "NAME",
    "ADDNAMES",
    "FORMAT_IP",
    "FORMAT_KBYTES",
    "FORMAT_FRIENDLYDATE",
    "FORMAT_FRIENDLYSS2000",
    "FORMAT_DATESS2000",
    "FORMAT_FRIENDLYCPU",
    "FORMAT_PERCENT",
    "FORMAT_NOPATH",
    "FORMAT_HSV",
    "FORMAT_TEXTURE",

    "FORMAT_LVWIDTH",

    "FORMAT_UI_LEFT",
    "FORMAT_UI_RIGHT",
    "FORMAT_UI_RESIZABLE",
    "FORMAT_UI_NOTRANSLATE_HEADER",
    "FORMAT_UI_NOHEADER",
    "FORMAT_UI_NODISPLAY",

    "FILENAME",
    "CURRENTFILE",
    "TIMESTAMP",
    "LINENUM",

    "MINBITS",
    "PRECISION",
    
    "FLOAT_HUNDRETHS",
    "FLOAT_TENTHS",
    "FLOAT_ONES",
    "FLOAT_FIVES",
    "FLOAT_TENS",

    "DEF",
    "DEFAULT",

    "FLAGS",
    "BOOLFLAG",
    "USEDFIELD",
    "RAW",
    "POINTER",
    "INT",
    
    "VEC3",
    "VEC2",
    "RGB",
    "RGBA",
    "RG",

    "SUBTABLE",
    "STRUCT",
    "UNOWNED",
    
    "INDEX",
    "AUTO_INDEX",

    "WIKI",

    "REDUNDANT_STRUCT",

    "EMBEDDED_FLAT",

    "USERFLAG",

    "REFDICT",

    // Misc bit flags

    "REDUNDANTNAME",
    "STRUCTPARAM",

    "PERSIST",
    "NO_TRANSACT",

    "INDEX_DEFINE",

    "COMMAND",

    "POLYCHILDTYPE",
    "POLYPARENTTYPE",

    "FORCE_CONTAINER",
    "FORCE_NOT_CONTAINER",

    "LATEBIND",

    "WIKILINK",

    "FORMATSTRING",

    NULL
};


//these strings are prepended to the type field, with TOK_ before them
char const* sTokTypeFlags[] =
{
    "POOL_STRING",
    "ESTRING",
    "SERVER_ONLY",
    "CLIENT_ONLY",
    "SELF_ONLY",
    "SPECIAL_ONLY",
    "STRUCT_NORECURSE",
    "NO_TRANSLATE",
    "NO_INDEX",
    "VOLATILE_REF",
    "ALWAYS_ALLOC",
    "NON_NULL_REF",
    "NO_WRITE",
    "NO_NETSEND",
    "NO_TEXT_SAVE"
};

//these are the same as the ones above, except that for REDUNDANT_NAME fields, they
//are ignored
char const* sTokTypeFlags_NoRedundantRepeat[] =
{
    "KEY",
    "REQUIRED",
    "PRIMARY_KEY",
    "AUTO_INCREMENT",
};

void StructParser::AddExtraNameToField(Tokenizer *pTokenizer, STRUCT_FIELD_DESC *pField, char *pName, bool bKeepOriginalName)
{
    if (!bKeepOriginalName)
    {
        if (strcmp(pField->userFieldName, pField->curStructFieldName) == 0)
        {
            strcpy(pField->userFieldName, pName);
            return;
        }
    }

    pTokenizer->Assert(pField->iNumRedundantNames < MAX_REDUNDANT_NAMES_PER_FIELD, "Too many redundant names");

    strcpy(pField->redundantNames[pField->iNumRedundantNames++], pName);
}

void StructParser::ProcessStructFieldCommandString(STRUCT_DEF *pStruct, STRUCT_FIELD_DESC *pField, char const* pSourceString, 
    int iNumNotStrings, char **ppNotStrings, char const* pFileName, int iLineNum, Tokenizer *pMainTokenizer)
{
    Tokenizer tokenizer;
    Tokenizer *pTokenizer = &tokenizer;

    char stringToUse[TOKENIZER_MAX_STRING_LENGTH * 3];
    
    sprintf(stringToUse, "%s %s %s %s", m_pPrefix ? m_pPrefix : " ", pStruct->forAllString[0] ? pStruct->forAllString : " ", pSourceString, m_pSuffix ? m_pSuffix : " ");

    ReplaceMacrosInString(stringToUse, TOKENIZER_MAX_STRING_LENGTH);

    ClipStrings(stringToUse, iNumNotStrings, ppNotStrings);

    pTokenizer->LoadFromBuffer(stringToUse, (int)strlen(stringToUse) + 1, pFileName, iLineNum);

    pTokenizer->SetExtraReservedWords(sFieldCommandStringReservedWords);

    Token token;
    enumTokenType eType;

    do
    {
        eType = pTokenizer->GetNextToken(&token);

        if (eType == TOKEN_NONE)
        {
            break;
        }

        if (eType == TOKEN_IDENTIFIER)
        {
            int i;
            bool bFound = false;

            for (i=0; i < sizeof(sTokTypeFlags) / sizeof(sTokTypeFlags[0]); i++)
            {
                Tokenizer::StaticAssert(i < 32, "too many sTokTypeFlags");

                if (strcmp(token.sVal, sTokTypeFlags[i]) == 0)
                {
                    pField->iTypeFlagsFound |= (1 << i);
                    bFound = true; 
                    break;
                }
            }

            if (!bFound)
            {
                for (i=0; i < sizeof(sTokTypeFlags_NoRedundantRepeat) / sizeof(sTokTypeFlags_NoRedundantRepeat[0]); i++)
                {
                    Tokenizer::StaticAssert(i < 32, "too many sTokTypeFlags_NoRedundantRepeat");

                    if (strcmp(token.sVal, sTokTypeFlags_NoRedundantRepeat[i]) == 0)
                    {
                        pField->iTypeFlags_NoRedundantRepeatFound |= (1 << i);
                        bFound = true; 
                        break;
                    }
                }
            }

            if (!bFound)
            {
                char errorString[256];
                sprintf(errorString, "Found unrecognized struct field command %s", token.sVal);
                pTokenizer->Assert(0, errorString);
            }

            continue;
        }


        if (eType != TOKEN_RESERVEDWORD)
        {
            pTokenizer->StringifyToken(&token);
            char errorString[256];
            sprintf(errorString, "Found unrecognized struct field command %s", token.sVal);
            pTokenizer->Assert(0, errorString);
        }

        if (token.iVal >= FIRST_FORMAT_RW && token.iVal - FIRST_FORMAT_RW < FORMAT_COUNT - 1)
        {
            pTokenizer->Assert(pField->eFormatType == FORMAT_NONE, "Only one FORMAT_XXX allowed per field");
            pField->eFormatType = (enumFormatType)(token.iVal - FIRST_FORMAT_RW + 1);
        }
        else if (token.iVal >= FIRST_FORMAT_FLAG_RW && token.iVal - FIRST_FORMAT_FLAG_RW < FORMAT_FLAG_COUNT)
        {
            pField->bFormatFlags[token.iVal - FIRST_FORMAT_FLAG_RW] = true;
        }
        else if (token.iVal >= FIRST_FLOAT_ACCURACY_RW && token.iVal <= LAST_FLOAT_ACCURACY_RW)
        {
            pTokenizer->Assert(pField->iFloatAccuracy == 0, "Can't have two FLOAT_XXX commands for same field");
            pField->iFloatAccuracy = token.iVal - FIRST_FLOAT_ACCURACY_RW + 1;
        }
        else switch (token.iVal)
        {
        case RW_NAME:
        case RW_ADDNAMES:
            {
                bool bAddNames = (token.iVal == RW_ADDNAMES);
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after NAME or ADDNAMES");
                
                do
                {
                    eType = pTokenizer->GetNextToken(&token);
                    pTokenizer->Assert( (eType == TOKEN_IDENTIFIER || eType == TOKEN_STRING) && strlen(token.sVal) <  MAX_NAME_LENGTH, "Expected identifier or string after NAME(");

                    AddExtraNameToField(pTokenizer, pField, token.sVal, bAddNames);

                    eType = pTokenizer->CheckNextToken(&token);

                    if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_COMMA)
                    {
                        pTokenizer->GetNextToken(&token);
                    }
                    else
                    {
                        break;
                    }
                } while (1);

                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after NAME(x");
            }

            break;

        case RW_LVWIDTH:
            pTokenizer->Assert(pField->lvWidth == 0, "Found two FORMAT_LVWIDTH tokens");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after LVWIDTH");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Expected number after LVWIDTH(");
            pTokenizer->Assert(token.iVal > 0 && token.iVal <= 255, "LVWIDTH out of range");
            pField->lvWidth = token.iVal;
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after LVWIDTH(x");
            break;

        case RW_FILENAME:
            pField->bFoundFileNameToken = true;
            break;

        case RW_CURRENTFILE:
            pField->bFoundCurrentFileToken = true;
            break;

        case RW_TIMESTAMP:
            pField->bFoundTimeStampToken = true;
            break;

        case RW_LINENUM:
            pField->bFoundLineNumToken = true;
            break;

        case RW_MINBITS:
            pTokenizer->Assert(pField->iMinBits == 0, "Found two MINBITS tokens");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after MINBITS");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Expected number after MINBITS(");
            pTokenizer->Assert(token.iVal > 0 && token.iVal <= 255, "MINBITS out of range");
            pField->iMinBits = token.iVal;
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after MINBITS(x");
            break;

        case RW_PRECISION:
            pTokenizer->Assert(pField->iPrecision == 0, "Found two PRECISION tokens");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after PRECISION");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Expected number after PRECISION(");
            pTokenizer->Assert(token.iVal > 0 && token.iVal <= 255, "PRECISION out of range");
            pField->iPrecision = token.iVal;
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after PRECISION(x");
            break;

        case RW_DEF:
        case RW_DEFAULT:
            pTokenizer->Assert(pField->defaultString[0] == 0, "Found two DEFAULTS");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after DEFAULT");

            pTokenizer->GetSpecialStringTokenWithParenthesesMatching(&token);

            pTokenizer->Assert(token.sVal[0] != 0, "Default seems to be empty");

            //check for non-whole-number literal floats, which do not work 
        
            pTokenizer->Assertf(!isNonWholeNumberFloatLiteral(token.sVal), "default value %s appears to be a non-whole-number literal float. This will get rounded off, and is thus a bad idea", token.sVal);

            strcpy(pField->defaultString, token.sVal);
            break;

        case RW_FLAGS:
            pField->bFoundFlagsToken = true;
            break;

        case RW_BOOLFLAG:
            pField->bFoundBoolFlagToken = true;
            break;

        case RW_USEDFIELD:
            pTokenizer->Assert(pField->usedFieldName[0] == 0, "Found two USEDFIELD tokens");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after USEDFIELD");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH - 1, "Expected identifier after USEDFIELD(");
            strcpy(pField->usedFieldName, token.sVal);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after USEDFIELD(x");
            break;

        case RW_INT:
            pField->bFoundIntToken = true;
            break;

        case RW_RAW:
            pTokenizer->Assert(!pField->bFoundRawToken, "Found two RAW tokens");

            pField->bFoundRawToken = true;

            eType = pTokenizer->CheckNextToken(&token);

            if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTPARENS)
            {
                pTokenizer->GetNextToken(&token);
                pTokenizer->GetSpecialStringTokenWithParenthesesMatching(&token);

                pTokenizer->Assert(token.sVal[0] != 0, "RAW size appears to empty");

                strcpy(pField->rawSizeString, token.sVal);
            }
            break;

        case RW_POINTER:
            pTokenizer->Assert(!pField->bFoundPointerToken, "Found two RAW tokens");

            pField->bFoundPointerToken = true;

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after POINTER");
    
            pTokenizer->GetSpecialStringTokenWithParenthesesMatching(&token);

            pTokenizer->Assert(token.sVal[0] != 0, "POINTER size appears to empty");

            strcpy(pField->pointerSizeString, token.sVal);
            
            break;            

        case RW_VEC3:
            pField->bFoundVec3Token = true;
            break;

        case RW_VEC2:
            pField->bFoundVec2Token = true;
            break;

        case RW_RGB:
            pField->bFoundRGBToken = true;
            break;

        case RW_RGBA:
            pField->bFoundRGBAToken = true;
            break;

        case RW_RG:
            pField->bFoundRGToken = true;
            break;

        case RW_SUBTABLE:
            pTokenizer->Assert(pField->subTableName[0] == 0, "Found two SUBTABLE tokens");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after SUBTABLE");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH - 1, "Expected identifier after SUBTABLE(");
            strcpy(pField->subTableName, token.sVal);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after SUBTABLE(x");
            break;

        case RW_UNOWNED:
            sprintf(pField->structTpiName, "parse_%s", pField->typeName);
            pField->bForceUnownedStruct = true;
            break;
            
            

        case RW_ALLCAPSSTRUCT:
            pTokenizer->Assert(pField->structTpiName[0] == 0, "Found two STRUCT tokens");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after STRUCT");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH - 1, "Expected identifier after STRUCT(");
            

            
            strcpy(pField->structTpiName, token.sVal);
            
            eType = pTokenizer->CheckNextToken(&token);
            if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_COMMA)
            {
                eType = pTokenizer->GetNextToken(&token);
                eType = pTokenizer->GetNextToken(&token);

                if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "OPTIONAL") == 0)
                {
                    pField->bForceOptionalStruct = true;
                }
                else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "EARRAY")== 0)
                {
                    pField->bForceEArrayOfStructs = true;
                }
                else if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_UNOWNED)
                {
                    pField->bForceUnownedStruct = true;
                }
                else
                {
                    pTokenizer->Assert(0, "Expected OPTIONAL or EARRAY or UNOWNED after STRUCT(x,");
                }
            }

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after STRUCT(x");
            break;

        case RW_INDEX:
            {
                //hacky fake realloc
                FIELD_INDEX *pNewIndexes = new FIELD_INDEX[pField->iNumIndexes + 1];
                if (pField->pIndexes)
                {
                    memcpy(pNewIndexes, pField->pIndexes, sizeof(FIELD_INDEX) * pField->iNumIndexes);
                    delete pField->pIndexes;
                }

                pField->pIndexes = pNewIndexes;

                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after INDEX");
                eType = pTokenizer->GetNextToken(&token);

                pTokenizer->Assert(eType == TOKEN_INT || eType == TOKEN_IDENTIFIER, "expected int or identifer after INDEX");
                pTokenizer->StringifyToken(&token);
                pTokenizer->Assert(strlen(token.sVal) < MAX_NAME_LENGTH - 1, "INDEX identifier overflow");

                strcpy(pField->pIndexes[pField->iNumIndexes].indexString, token.sVal);

                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after INDEX(x");
            
                eType = pTokenizer->GetNextToken(&token);
                pTokenizer->Assert(eType == TOKEN_STRING || eType == TOKEN_IDENTIFIER, "expected string or identifier  after INDEX(x,");
                pTokenizer->Assert(strlen(token.sVal) < MAX_NAME_LENGTH - 1, "INDEX name overflow");

                strcpy(pField->pIndexes[pField->iNumIndexes++].nameString, token.sVal);

                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after INDEX(x,x");
            }
            break;

        case RW_AUTO_INDEX:
            {
                int iArraySize = atoi(pField->arraySizeString);
                char nameString[MAX_NAME_LENGTH];
                int i;
                
                pTokenizer->Assert(iArraySize > 0, "AUTO_INDEX requires a non-zero literal int for array size");
                pTokenizer->Assert(pField->pIndexes == NULL, "AUTO_INDEX conflicts with INDEX or another AUTO_INDEX");
                pField->iNumIndexes = iArraySize;
                pField->pIndexes = new FIELD_INDEX[iArraySize];
                
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AUTO_INDEX");
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH - 5, "Expected identifier after AUTO_INDEX(");
                strcpy(nameString, token.sVal);
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AUTO_INDEX(x");

                for (i=0; i < iArraySize; i++)
                {
                    sprintf(pField->pIndexes[i].indexString, "%d", i);
                    sprintf(pField->pIndexes[i].nameString, "%s_%d", nameString, i);
                }
            }
            break;






        case RW_COMMA:
            break;

        case RW_WIKI:
            pTokenizer->Assert(pField->iNumWikiComments < MAX_WIKI_COMMENTS, "Two many WIKI comments for one field");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after WIKI");
            
            eType = pTokenizer->CheckNextToken(&token);
            if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "AUTO") == 0)
            {
                pTokenizer->GetNextToken(&token);
                
                pMainTokenizer->GetSurroundingSlashedCommentBlock(&token);

                if (strlen(token.sVal) > 0)
                {
                    pField->pWikiComments[pField->iNumWikiComments] = new char[token.iVal + 1];
                    strcpy(pField->pWikiComments[pField->iNumWikiComments++] , token.sVal);
                }
            }
            else
            {
                
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Expected string after WIKI(");

                pField->pWikiComments[pField->iNumWikiComments] = new char[token.iVal + 1];
                strcpy(pField->pWikiComments[pField->iNumWikiComments++] , token.sVal);
            }

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after WIKI");
            break;

        case RW_REDUNDANT_STRUCT:
            pTokenizer->Assert(pField->iNumRedundantStructInfos < MAX_REDUNDANT_STRUCTS, "Too many REDUNDANT_STRUCTs");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after REDUNDANT_STRUCT");
            eType = pTokenizer->GetNextToken(&token);

            pTokenizer->Assert(eType == TOKEN_IDENTIFIER || eType == TOKEN_STRING, "Expected string or identifier after REDUNDANT_STRUCT(");

            pTokenizer->Assert(token.iVal < MAX_NAME_LENGTH, "Redundant struct name too long");

            strcpy(pField->redundantStructs[pField->iNumRedundantStructInfos].name, token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after REDUNDANT_STRUCT(x");
            eType = pTokenizer->GetNextToken(&token);

            pTokenizer->Assert(eType == TOKEN_IDENTIFIER || eType == TOKEN_STRING, "Expected string or identifier after REDUNDANT_STRUCT(x,");

            pTokenizer->Assert(token.iVal < MAX_NAME_LENGTH, "Redundant struct subtable name too long");

            strcpy(pField->redundantStructs[pField->iNumRedundantStructInfos++].subTable, token.sVal);
            
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ( after REDUNDANT_STRUCT");

            break;

        case RW_EMBEDDED_FLAT:
            pField->bFlatEmbedded = true;
            break;

        case RW_USERFLAG:
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after USERFLAG");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH - 1, "Expected identifier after USERFLAG(");
    
            bool bAlreadyFound;

            int i;

            bAlreadyFound = false;
            for (i=0; i < pField->iNumUserFlags; i++)
            {
                if (strcmp(token.sVal, pField->userFlags[i]) == 0)
                {
                    bAlreadyFound = true;
                    break;
                }
            }

            if (!bAlreadyFound)
            {
                pTokenizer->Assert(pField->iNumUserFlags < MAX_USER_FLAGS, "Too many distinct USERFLAGs found");

                strcpy(pField->userFlags[pField->iNumUserFlags++], token.sVal);
            }

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after USERFLAG(x");
            break;

        case RW_REFDICT:
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after REFDICT");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH - 1, "Expected identifier after REFDICT(");

            pTokenizer->Assert(pField->eDataType == DATATYPE_REFERENCE, "Can't have REFDICT for non-reference");

            strcpy(pField->refDictionaryName, token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after REFDICT(x");
            break;

    

        case RW_REDUNDANT:
            pField->bFoundRedundantToken = true;
            break;

        case RW_STRUCTPARAM:
            pField->bFoundStructParam = true;
            break;

    
        case RW_PERSIST:
            pField->bFoundPersist = true;
            break;

        case RW_NOTRANSACT:
            pField->bFoundNoTransact = true;
            break;


        case RW_REQUESTINDEXDEFINE:
            pField->bFoundRequestIndexDefine = true;
            break;

        case RW_COMMAND:
            STRUCT_COMMAND *pCommand;
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after COMMAND");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Expected command name after COMMAND");
            pCommand = new STRUCT_COMMAND;
            pCommand->pCommandName = new char[token.iVal + 1];
            strcpy(pCommand->pCommandName, token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after COMMAND(name");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Expected command string after COMMAND(name,");

            pCommand->pCommandString = new char[token.iVal + 1];
            strcpy(pCommand->pCommandString, token.sVal);

            pCommand->pNext = pField->pFirstCommand;
            pField->pFirstCommand = pCommand;

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after COMMAND(name, string");
            break;

        case RW_POLYCHILDTYPE:
            pTokenizer->Assert(pField == pStruct->pStructFields[0], "POLYCHILDTYPE only legal on first field of struct");
            strcpy(pStruct->structNameIInheritFrom, pField->typeName);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after POLYCHILDTYPE");
            pTokenizer->GetSpecialStringTokenWithParenthesesMatching(&token);
            pTokenizer->Assert(token.iVal < MAX_NAME_LENGTH, "POLYCHILDTYPE string too long");
            strcpy(pField->myPolymorphicType, token.sVal);
            pField->bIAmPolyChildTypeField = true;
            break;

        case RW_POLYPARENTTYPE:
            pTokenizer->Assert(!pStruct->bIAmAPolymorphicParent && pStruct->structNameIInheritFrom[0] == 0, "Struct can't have two POLYXTYPEs");
            pStruct->bIAmAPolymorphicParent = true;
            pField->bIAmPolyParentTypeField = true;
            break;

        case RW_FORCECONTAINER:
            pTokenizer->Assert(!pField->bFoundForceNotContainer && !pField->bFoundForceContainer, "Can't have two occurrences of FORCE_X_CONTAINER");
            pField->bFoundForceContainer = true;
            break;

        case RW_FORCENOTCONTAINER:
            pTokenizer->Assert(!pField->bFoundForceNotContainer && !pField->bFoundForceContainer, "Can't have two occurrences of FORCE_X_CONTAINER");
            pField->bFoundForceNotContainer = true;
            break;

        case RW_LATEBIND:
            pField->bFoundLateBind = true;
            break;

        case RW_WIKILINK:
            pField->bDoWikiLink = true;
            break;

        case RW_FORMATSTRING:
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after FORMATSTRING");
            
            while (1)
            {
                char name[256];
                char curString[512];
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, sizeof(name)-1, "Expected FORMATSTRING name");

                strcpy(name, token.sVal);

                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_EQUALS, "Expected = after FORMATSTRING x");

                eType = pTokenizer->GetNextToken(&token);

                if (eType == TOKEN_INT)
                {
                    AssertNameIsLegalForFormatStringInt(pTokenizer, name);
                    sprintf(curString, "%s = %d", name, token.iVal);
                    AddStringToFormatString(pField, curString);
                }
                else if (eType == TOKEN_STRING)
                {
                    AssertNameIsLegalForFormatStringString(pTokenizer, name);
                    pTokenizer->Assert(token.iVal + strlen(name) < sizeof(curString) - 10, "format string overflow");

                    sprintf(curString, "%s = \\\"%s\\\"", name, token.sVal);
                    AddStringToFormatString(pField, curString);
                }
                else
                {
                    pTokenizer->Assert(0, "Expected int or string in format string, found neither");
                }

                eType = pTokenizer->GetNextToken(&token);

                if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTPARENS)
                {
                    break;
                }

                pTokenizer->Assert(eType == TOKEN_RESERVEDWORD && token.iVal == RW_COMMA, "Expected , or ) in FORMATSTRING");
            }
            break;

            


        default:
            

            pTokenizer->StringifyToken(&token);
            char errorString[256];
            sprintf(errorString, "Found unrecognized struct field command %s", token.sVal);
            pTokenizer->Assert(0, errorString);
            break;
        }
    } while (1);
}

void StructParser::FixupFieldTypes_RightBeforeWritingData(STRUCT_DEF *pStruct)
{

}



void StructParser::FixupFieldTypes(STRUCT_DEF *pStruct)
{
    int i;
    enumIdentifierType eIdentifierType;

    for (i=0; i < pStruct->iNumFields; i++)
    {
        STRUCT_FIELD_DESC *pField = pStruct->pStructFields[i];

        if (pField->iNumIndexes > 0)
        {
            FieldAssert(pField, pField->iArrayDepth > 0, "Can't have INDEX without an array");
            pField->iArrayDepth--;
        }

        if (!pField->bFoundSpecialConstKeyword)
        {

            if (pField->bOwns)
            {
                pField->eDataType = DATATYPE_STRUCT;
            }
            else if (pField->eDataType == DATATYPE_REFERENCE)
            {    
                FieldAssert(pField, pField->refDictionaryName[0] != 0, "Reference has no ref dictionary name, either implied from type or from REFDICT(x)");
            }
            else if ((eIdentifierType = m_pParent->GetDictionary()->FindIdentifier(pField->typeName)) != IDENTIFIER_NONE)
            {
                if (eIdentifierType == IDENTIFIER_ENUM)
                {
                    pField->eDataType = DATATYPE_INT;

                    if (pField->subTableName[0] == 0)
                    {
                        sprintf(pField->subTableName, "%sEnum", pField->typeName); 
                    }
                }
                else if (eIdentifierType == IDENTIFIER_STRUCT || eIdentifierType == IDENTIFIER_STRUCT_CONTAINER)
                {
                    pField->eDataType = DATATYPE_STRUCT;
                    pField->pStructSourceFileName = m_pParent->GetDictionary()->GetSourceFileForIdentifier(pField->typeName);
                }

            }
            else if (IsLinkName(pField->typeName))
            {
                pField->eDataType = DATATYPE_LINK;
            }
            else if (IsFloatName(pField->typeName))
            {
                pField->eDataType = DATATYPE_FLOAT;
            }
            else if (IsIntName(pField->typeName) || pField->bFoundIntToken)
            {
                pField->eDataType = DATATYPE_INT;
            }
            else if (IsCharName(pField->typeName))
            {
                pField->eDataType = DATATYPE_CHAR;
            }
            else if (strcmp(pField->typeName, "Vec4") == 0)
            {
                pField->eDataType = DATATYPE_VEC4;
            }
            else if (strcmp(pField->typeName, "Vec3") == 0)
            {
                pField->eDataType = DATATYPE_VEC3;
            }
            else if (strcmp(pField->typeName, "Vec2") == 0)
            {
                pField->eDataType = DATATYPE_VEC2;
            }
            else if (strcmp(pField->typeName, "IVec4") == 0)
            {
                pField->eDataType = DATATYPE_IVEC4;
            }
            else if (strcmp(pField->typeName, "IVec3") == 0)
            {
                pField->eDataType = DATATYPE_IVEC3;
            }
            else if (strcmp(pField->typeName, "IVec2") == 0)
            {
                pField->eDataType = DATATYPE_IVEC2;
            }
            else if (strcmp(pField->typeName, "Mat3") == 0)
            {
                pField->eDataType = DATATYPE_MAT3;
            }
            else if (strcmp(pField->typeName, "Mat4") == 0)
            {
                pField->eDataType = DATATYPE_MAT4;
            }
            else if (strcmp(pField->typeName, "Quat") == 0)
            {
                pField->eDataType = DATATYPE_QUAT;
            }
            else if (pField->structTpiName[0])
            {
                pField->eDataType = DATATYPE_STRUCT;
                
            }
            else if (strcmp(pField->typeName, "TokenizerParams") == 0)
            {
                pField->eDataType = DATATYPE_TOKENIZERPARAMS;
            }
            else if (strcmp(pField->typeName, "TokenizerFunctionCall") == 0)
            {
                pField->eDataType = DATATYPE_TOKENIZERFUNCTIONCALL;
            }
            else if (strcmp(pField->typeName, "StashTable") == 0)
            {
                pField->eDataType = DATATYPE_STASHTABLE;
            }
            else if (strcmp(pField->typeName, "MultiVal") == 0)
            {
                pField->eDataType = DATATYPE_MULTIVAL;
            }
            else if (pField->eDataType != DATATYPE_VOID) 
            {
                if (pField->iNumAsterisks == 0)
                {
                    pField->eDataType = DATATYPE_INT;
                }
                else
                {
                    pField->eDataType = DATATYPE_STRUCT;
                    sprintf(pField->structTpiName, "parse_%s", pField->typeName);
                }

            }
        }
        else if (pField->eDataType != DATATYPE_REFERENCE)
        {
            if    ((eIdentifierType = m_pParent->GetDictionary()->FindIdentifier(pField->typeName)) != IDENTIFIER_NONE)
            {
                if (eIdentifierType == IDENTIFIER_ENUM)
                {
                    pField->eDataType = DATATYPE_INT;

                    if (pField->subTableName[0] == 0)
                    {
                        sprintf(pField->subTableName, "%sEnum", pField->typeName); 
                    }
                }
                else if (eIdentifierType == IDENTIFIER_STRUCT || eIdentifierType == IDENTIFIER_STRUCT_CONTAINER)
                {

                    pField->eDataType = DATATYPE_STRUCT;
                    pField->pStructSourceFileName = m_pParent->GetDictionary()->GetSourceFileForIdentifier(pField->typeName);
                    
                }

            }
        }


        if (pField->bFoundVec3Token)
        {
            if (pField->eDataType == DATATYPE_FLOAT && pField->iArrayDepth)
            {
                pField->iArrayDepth--;
                pField->eDataType = DATATYPE_VEC3;
            }
            else
            {
                FieldAssert(pField, pField->eDataType == DATATYPE_VEC3, "TOK_VEC3 only legal for data that is Vec3 or float[]");
            }
        }

        if (pField->bFoundVec2Token)
        {
            if (pField->eDataType == DATATYPE_FLOAT && pField->iArrayDepth)
            {
                pField->iArrayDepth--;
                pField->eDataType = DATATYPE_VEC2;
            }
            else
            {
                FieldAssert(pField, pField->eDataType == DATATYPE_VEC3 || pField->eDataType == DATATYPE_VEC2, 
                    "TOK_VEC2 only legal for data that is Vec3 or Vec2 or float[]");
            }
        }

        if (pField->bFoundRGBAToken)
        {
            if (pField->eDataType == DATATYPE_INT && pField->iArrayDepth)
            {
                pField->iArrayDepth--;
                pField->eDataType = DATATYPE_RGBA;
            }
            else if (pField->eDataType == DATATYPE_INT && pField->eStorageType == STORAGETYPE_EMBEDDED && pField->eReferenceType == REFERENCETYPE_DIRECT && strcmp(pField->typeName, "Color") == 0)
            {
                pField->eDataType = DATATYPE_RGBA;
            }
            else
            {
                FieldAssert(pField, 0, "TOK_RGBA only legal for data that is U8[] or named \"Color\"");
            }
        }


        if (pField->bFoundRGBToken)
        {
            if (pField->eDataType == DATATYPE_INT && pField->iArrayDepth)
            {
                pField->iArrayDepth--;
                pField->eDataType = DATATYPE_RGB;
            }
            else
            {
                FieldAssert(pField, 0, "TOK_RGB only legal for data that is U8[]");
            }
        }

        if (pField->bFoundRGToken)
        {
            if (pField->eDataType == DATATYPE_INT && pField->iArrayDepth)
            {
                pField->iArrayDepth--;
                pField->eDataType = DATATYPE_RG;
            }
            else
            {
                FieldAssert(pField, 0, "TOK_RG only legal for data that is U8[]");
            }
        }

    








        if (pField->bForceOptionalStruct || pField->bForceEArrayOfStructs)
        {
            FieldAssert(pField, pField->eDataType == DATATYPE_STRUCT, "Ambiguity from bForceOptionalStruct/bForceEArrayofStructs");
            FieldAssert(pField, pField->iArrayDepth == 0 && pField->iNumAsterisks == 0, "Can't force EARRAY or OPTIONAL structs with [] or *");

            if (pField->bForceOptionalStruct)
            {
                pField->eReferenceType = REFERENCETYPE_POINTER;
                pField->eStorageType = STORAGETYPE_EMBEDDED;
            }
            else
            {
                pField->eReferenceType = REFERENCETYPE_POINTER;
                pField->eStorageType = STORAGETYPE_EARRAY;
            }
        }
        else if (!pField->bFoundSpecialConstKeyword && pField->eDataType != DATATYPE_REFERENCE)
        {

            FieldAssert(pField, pField->iArrayDepth == 0 || pField->iArrayDepth == 1, "Can't parse multidimensional arrays");

            pField->bArray = (pField->iArrayDepth == 1);


            switch (pField->iNumAsterisks)
            {
            case 0:
                pField->eReferenceType = REFERENCETYPE_DIRECT;
                pField->eStorageType = pField->bArray ? STORAGETYPE_ARRAY : STORAGETYPE_EMBEDDED;
                break;

            case 1:
                FieldAssert(pField, !pField->bArray, "Arrays of pointers not supported"); 
                pField->eReferenceType = REFERENCETYPE_POINTER;
                pField->eStorageType = STORAGETYPE_EMBEDDED;
                break;

            case 2:
                FieldAssert(pField, !pField->bArray, "Arrays of EArrays not supported");
                pField->eReferenceType = REFERENCETYPE_POINTER;
                pField->eStorageType = STORAGETYPE_EARRAY;
                break;

            }
        }
        








    
        if (pField->bFoundCurrentFileToken)
        {
            FieldAssert(pField, pField->eDataType == DATATYPE_CHAR && pField->eStorageType == STORAGETYPE_EMBEDDED && pField->eReferenceType == REFERENCETYPE_POINTER,
                "CURRENTFILE only supported for char*");

            pField->eDataType = DATATYPE_CURRENTFILE;
        }

        if (pField->bFoundFileNameToken)
        {
            FieldAssert(pField, pField->eDataType == DATATYPE_CHAR /*&& pField->eStorageType == STORAGETYPE_EMBEDDED && pField->eReferenceType == REFERENCETYPE_POINTER*/,
                "FILENAME only supported for char* or char**");

            pField->eDataType = DATATYPE_FILENAME;
        }

        if (pField->bFoundTimeStampToken)
        {
            FieldAssert(pField, pField->eDataType == DATATYPE_INT && pField->eStorageType == STORAGETYPE_EMBEDDED && pField->eReferenceType == REFERENCETYPE_DIRECT,
                "TIMESTAMP only supported for int");

            pField->eDataType = DATATYPE_TIMESTAMP;
        }

        if (pField->bFoundLineNumToken)
        {
            FieldAssert(pField, pField->eDataType == DATATYPE_INT && pField->eStorageType == STORAGETYPE_EMBEDDED && pField->eReferenceType == REFERENCETYPE_DIRECT,
                "LINENUM only supported for int");

            pField->eDataType = DATATYPE_LINENUM;
        }

        if (pField->bFoundFlagsToken)
        {
            FieldAssert(pField, pField->eDataType == DATATYPE_INT && pField->eStorageType == STORAGETYPE_EMBEDDED && pField->eReferenceType == REFERENCETYPE_DIRECT,
                "FLAGS only supported for int");

            pField->eDataType = DATATYPE_FLAGS;
        }

        if (pField->bFoundBoolFlagToken)
        {
            FieldAssert(pField, pField->eDataType == DATATYPE_INT && pField->eStorageType == STORAGETYPE_EMBEDDED && pField->eReferenceType == REFERENCETYPE_DIRECT,
                "BOOLFLAG only supported for int");

            pField->eDataType = DATATYPE_BOOLFLAG;
        }

        if (pField->usedFieldName[0])
        {
            FieldAssert(pField, (pField->eDataType == DATATYPE_INT || pField->eDataType == DATATYPE_VOID) && pField->eStorageType == STORAGETYPE_EMBEDDED && pField->eReferenceType == REFERENCETYPE_POINTER,
                "USEDFIELD only supported for int* and void*");

            pField->eDataType = DATATYPE_USEDFIELD;
        }

        if (pField->bFoundRawToken)
        {
            FieldAssert(pField, pField->eDataType < DATATYPE_FIRST_SPECIAL, "Can't have RAW along with some other special type");
            pField->eDataType = DATATYPE_RAW;
            pField->eStorageType = STORAGETYPE_EMBEDDED;
            pField->eReferenceType = REFERENCETYPE_DIRECT;
        }

        if (pField->bFoundPointerToken)
        {
            FieldAssert(pField, pField->eDataType < DATATYPE_FIRST_SPECIAL, "Can't have POINTER along with some other special type");
            FieldAssert(pField, pField->eReferenceType == REFERENCETYPE_POINTER, "Can't have POINTER without having, well, a pointer");
            pField->eDataType = DATATYPE_POINTER;
            pField->eStorageType = STORAGETYPE_EMBEDDED;
        }

        if (pField->bBitField)
        {
            FieldAssert(pField, pField->eDataType == DATATYPE_INT && pField->eReferenceType == REFERENCETYPE_DIRECT
                && pField->eStorageType == STORAGETYPE_EMBEDDED, "Bitfields only supported for single ints");
            pField->eDataType = DATATYPE_BIT;
    
            FieldAssert(pField, pField->defaultString[0] == 0, "Bitfields can't have default values");
        }



        //if we have no default string, use "0", to avoid having to check repeatedly in other places whether it even exists
        if (pField->defaultString[0] == 0)
        {
            pField->defaultString[0] = '0';
        }    

        if (pField->bFoundLateBind)
        {
            FieldAssert(pField, pField->eDataType == DATATYPE_INT || pField->eDataType == DATATYPE_STRUCT, "Found unexecpted LATEBIND");
            pField->eDataType = DATATYPE_STRUCT;
        }

        if (pField->eDataType == DATATYPE_STRUCT 
            && strcmp(pField->typeName, "InheritanceData") == 0
            && pField->eStorageType == STORAGETYPE_EMBEDDED
            && pField->eReferenceType == REFERENCETYPE_POINTER)
        {
            pField->bIsInheritanceStruct = true;
        }

        if (pField->eDataType == DATATYPE_STRUCT && pField->bForceUnownedStruct)
        {
            pField->eDataType = DATATYPE_UNOWNEDSTRUCT;
        }
    }


    for (i=0; i < pStruct->iNumFields; i++)
    {
        STRUCT_FIELD_DESC *pField = pStruct->pStructFields[i];

        //check if what we think is a struct is actually polymorphic
        if (pField->eDataType == DATATYPE_STRUCT && !pField->bIAmPolyChildTypeField && !pField->bIAmPolyParentTypeField)
        {
            STRUCT_DEF *pOtherStruct = FindNamedStruct(pField->typeName);

            if (pOtherStruct && pOtherStruct->bIAmAPolymorphicParent)
            {
                pField->eDataType = DATATYPE_STRUCT_POLY;
            }
        }
    }
}

bool StructParser::IsStructAllStructParams(STRUCT_DEF *pStruct)
{
    int i;

    for (i=0; i < pStruct->iNumFields; i++)
    {
        if (!pStruct->pStructFields[i]->bFoundStructParam && pStruct->pStructFields[i]->userFieldName[0] && pStruct->pStructFields[i]->eDataType != DATATYPE_CURRENTFILE)
        {
            return false;
        }
    }

    return true;
}

void StructParser::CalcLongestUserFieldName(STRUCT_DEF *pStruct)
{
    pStruct->iLongestUserFieldNameLength = (int)strlen(pStruct->structName);

    int i;

    if (pStruct->bHasStartString)
    {
        int iLen = (int)strlen(pStruct->startString);

        if (iLen > pStruct->iLongestUserFieldNameLength)
        {
            pStruct->iLongestUserFieldNameLength = iLen;
        }
    }

    if (pStruct->bHasEndString)
    {
        int iLen = (int)strlen(pStruct->endString);

        if (iLen > pStruct->iLongestUserFieldNameLength)
        {
            pStruct->iLongestUserFieldNameLength = iLen;
        }
    }


    for (i=0; i < pStruct->iNumIgnores; i++)
    {
        int iLen = (int)strlen(pStruct->ignores[i]);

        if (iLen > pStruct->iLongestUserFieldNameLength)
        {
            pStruct->iLongestUserFieldNameLength = iLen;
        }
    }

    for (i=0 ; i < pStruct->iNumFields; i++)
    {
        int iLen = (int)strlen(pStruct->pStructFields[i]->userFieldName);

        if (iLen > pStruct->iLongestUserFieldNameLength)
        {
            pStruct->iLongestUserFieldNameLength = iLen;
        }

        int j;

        for (j=0; j < pStruct->pStructFields[i]->iNumRedundantNames; j++)
        {
            int iLen = (int)strlen(pStruct->pStructFields[i]->redundantNames[j]);
    
            if (iLen > pStruct->iLongestUserFieldNameLength)
            {
                pStruct->iLongestUserFieldNameLength = iLen;
            }
        }

        for (j=0; j < pStruct->pStructFields[i]->iNumIndexes; j++)
        {
            int iLen = (int)strlen(pStruct->pStructFields[i]->pIndexes[j].nameString);
    
            if (iLen > pStruct->iLongestUserFieldNameLength)
            {
                pStruct->iLongestUserFieldNameLength = iLen;
            }
        }

        STRUCT_COMMAND *pCommand = pStruct->pStructFields[i]->pFirstCommand;

        while (pCommand)
        {
            int iLen = (int)strlen(pCommand->pCommandName);

            if (iLen > pStruct->iLongestUserFieldNameLength)
            {
                pStruct->iLongestUserFieldNameLength = iLen;
            }

            pCommand = pCommand->pNext;
        }    
        
        pCommand = pStruct->pStructFields[i]->pFirstBeforeCommand;

        while (pCommand)
        {
            int iLen = (int)strlen(pCommand->pCommandName);

            if (iLen > pStruct->iLongestUserFieldNameLength)
            {
                pStruct->iLongestUserFieldNameLength = iLen;
            }

            pCommand = pCommand->pNext;
        }
    }
}


void StructParser::DumpStructInitFunc(FILE *pFile, STRUCT_DEF *pStruct)
{
    int iNumBits = 0;
    char pClassName[1024];
    int iNumLateBinds = 0;


    RecurseOverAllFieldsAndFlatEmbeds(pStruct, pStruct, AreThereBitFields, 0, &iNumBits);

    if (iNumBits)
    {
        fprintf(pFile, "void FindAutoStructBitField(char *pStruct, int iStructSize, ParseTable *pTPI, int iColumn);\n\n");
    }
    
    if (pStruct->structNameIInheritFrom[0])
    {
        fprintf(pFile, "extern ParseTable polyTable_%s[];\n", pStruct->structNameIInheritFrom);
        fprintf(pFile, "void FixupPolyTable(ParseTable *pPolyTable, char *pNameToFind, int iStructSize);\n");
    }

    if (pStruct->fixupFuncName[0])
    {
        fprintf(pFile, "TextParserResult %s(%s *pStruct, enumTextParserFixupType eFixupType);\n", pStruct->fixupFuncName, pStruct->structName);
    }

    if (pStruct->bIsContainer)
    {
        sprintf(pClassName,"NOCONST(%s)",pStruct->structName);
    }
    else
    {
        strcpy(pClassName,pStruct->structName);
    }

    fprintf(pFile, "int autoStruct_fixup_%s()\n{\n", pStruct->structName);

    fprintf(pFile, "\tint iSize = sizeof(%s);\n", pStruct->structName);

    fprintf(pFile, "\tstatic char once = 0;\n\tif (once) return 0;\n\tonce = 1;\n");

    const char *pFixedUpSourceFileName = GetFileNameWithoutDirectories(pStruct->sourceFileName);
    while (*pFixedUpSourceFileName == '\\' || *pFixedUpSourceFileName == '/')
    {
        pFixedUpSourceFileName++;
    }

    fprintf(pFile, "\tParserSetTableInfoExplicitly(parse_%s, iSize, \"%s\", %s, \"%s\");\n", pStruct->structName, pStruct->structName,
        pStruct->fixupFuncName[0] ? pStruct->fixupFuncName : "NULL", pFixedUpSourceFileName);

    if (pStruct->structNameIInheritFrom[0])
    {
        fprintf(pFile, "\tFixupPolyTable(polyTable_%s, \"%s\", iSize);\n",
            pStruct->structNameIInheritFrom, pStruct->structName);
    }

    if (iNumBits)
    {

        fprintf(pFile, "\t{\n\t\t%s temp;\n\t\tmemset(&temp, 0, iSize);\n\n",
        pClassName);


        RecurseOverAllFieldsAndFlatEmbeds(pStruct, pStruct, DumpBitFieldFixups, 0, pFile);


        fprintf(pFile, "\t}\n");
    }

    fprintf(pFile, "\treturn 0;\n};\n\n");

    char tempName[256];
    sprintf(tempName, "autoStruct_fixup_%s", pStruct->structName);



    m_pParent->GetAutoRunManager()->AddAutoRun(tempName, pStruct->sourceFileName);

    bool bHasLateBinds = false;

    RecurseOverAllFieldsAndFlatEmbeds(pStruct, pStruct, AreThereLateBinds, 0, &bHasLateBinds);

    if (bHasLateBinds)
    {
        fprintf(pFile, "void DoAutoStructLateBind(ParseTable *pTPI, char *pFieldName, char *pOtherTPIName);\n\n");
        fprintf(pFile, "void autoStruct_lateFixup_%s(void)\n{\n", pStruct->structName);

        RecurseOverAllFieldsAndFlatEmbeds(pStruct, pStruct, DumpLateBindFixups, 0, pFile);

        fprintf(pFile, "}\n");


        sprintf(tempName, "autoStruct_lateFixup_%s", pStruct->structName);

        m_pParent->GetAutoRunManager()->AddAutoRunSpecial(tempName, pStruct->sourceFileName, false, AUTORUN_ORDER_LATE);

    }


}

#define MAX_FLATEMBED_RECURSE_DEPTH 32
static STRUCT_FIELD_DESC *spFlatEmbedRecurseFields[MAX_FLATEMBED_RECURSE_DEPTH];

void StructParser::RecurseOverAllFieldsAndFlatEmbeds(STRUCT_DEF *pParentStruct, STRUCT_DEF *pStruct, FieldRecurseCB *pCB, 
    int iRecurseDepth, void *pUserData)
{
    int i;

    for (i=0; i < pStruct->iNumFields; i++)
    {
        pCB(pParentStruct, pStruct->pStructFields[i], iRecurseDepth, spFlatEmbedRecurseFields, pUserData);
        
        if (pStruct->pStructFields[i]->eDataType == DATATYPE_STRUCT
            && pStruct->pStructFields[i]->bFlatEmbedded)
        {
            Tokenizer::StaticAssert(iRecurseDepth < MAX_FLATEMBED_RECURSE_DEPTH, "Exceeded flatembed recurse limit");

            STRUCT_DEF *pOtherStruct = FindNamedStruct(pStruct->pStructFields[i]->typeName);
            FieldAssert(pStruct->pStructFields[i], pOtherStruct != NULL, "Couldn't find flat embedded struct");
            
            spFlatEmbedRecurseFields[iRecurseDepth] = pStruct->pStructFields[i];

            RecurseOverAllFieldsAndFlatEmbeds(pParentStruct, pOtherStruct, pCB, iRecurseDepth + 1, pUserData);
        }
    }
}

void StructParser::AreThereLateBinds(STRUCT_DEF *pParentStruct, STRUCT_FIELD_DESC *pField, 
    int iRecurseDepth, STRUCT_FIELD_DESC **ppRecurse_fields, void *pUserData)
{
    if (pField->bFoundLateBind)
    {
        *((bool*)pUserData) = true;
    }
}
void StructParser::DumpLateBindFixups(STRUCT_DEF *pParentStruct, STRUCT_FIELD_DESC *pField, 
    int iRecurseDepth, STRUCT_FIELD_DESC **ppRecurse_fields, void *pUserData)
{
    if (pField->bFoundLateBind)
    {
        if (pField->structTpiName[0])
        {
            FieldAssert(pField, strncmp(pField->structTpiName, "parse_", 6) == 0, "LATEBIND fields with STRUCT() must have tpis named \"parse_x\"");
        }

        fprintf((FILE*)pUserData, "\tDoAutoStructLateBind(parse_%s, \"%s\", \"%s\");\n", pParentStruct->structName, 
            pField->userFieldName, pField->structTpiName[0] ? pField->structTpiName + 6 : pField->typeName);

        if (pField->iNumRedundantStructInfos)
        {
            int i;

            for (i=0; i < pField->iNumRedundantStructInfos; i++)
            {
                FieldAssert(pField, strncmp(pField->redundantStructs[i].subTable, "parse_", 6) == 0, 
                    "For latebinded redundant structs, parse table name must start with \"parse_\"");
                fprintf((FILE*)pUserData, "\tDoAutoStructLateBind(parse_%s, \"%s\", \"%s\");\n",
                    pParentStruct->structName, pField->redundantStructs[i].name, 
                    pField->redundantStructs[i].subTable + 6);
            }
        }


    }
}

void StructParser::AreThereBitFields(STRUCT_DEF *pParentStruct, STRUCT_FIELD_DESC *pField, 
    int iRecurseDepth, STRUCT_FIELD_DESC **ppRecurse_fields, void *pUserData)
{
    if (pField->eDataType == DATATYPE_BIT)
    {
        (*((int*)pUserData))++;
    }
}

void StructParser::DumpBitFieldFixups(STRUCT_DEF *pParentStruct, STRUCT_FIELD_DESC *pField, 
    int iRecurseDepth, STRUCT_FIELD_DESC **ppRecurse_fields, void *pUserData)
{
    if (pField->eDataType == DATATYPE_BIT)
    {
        char curFieldName[1024] = "";
        int i;

        for (i=0; i < iRecurseDepth ;i++)
        {
            sprintf(curFieldName + strlen(curFieldName), "%s.", ppRecurse_fields[i]->baseStructFieldName);
        }

        sprintf(curFieldName + strlen(curFieldName), "%s", pField->baseStructFieldName);

        fprintf((FILE*)pUserData, "\t\ttemp.%s = 1;\n\t\tFindAutoStructBitField((char*)&temp, iSize, parse_%s, %d);\n\t\ttemp.%s = 0;\n\n",
            curFieldName,
            pParentStruct->structName, pField->iIndexInParseTable,
            curFieldName);
    }

}


void StructParser::DumpExterns(FILE *pFile, STRUCT_DEF *pStruct)
{
    int i;
    int iRecurseDepth = 0;
    //generate any necessary extern ParseTable declarations
    for (i=0; i < pStruct->iNumFields; i++)
    {
        STRUCT_FIELD_DESC *pField = pStruct->pStructFields[i];
        switch (pField->eDataType)
        {
        case DATATYPE_STRUCT_POLY:
            fprintf(pFile, "extern ParseTable polyTable_%s[];\n", 
                pField->typeName);
            break;
        case DATATYPE_STRUCT:
            if (pField->bFlatEmbedded)
            {
                iRecurseDepth++;

                FieldAssert(pField, iRecurseDepth < 20, "Presumed infinite EMBEDDED_FLAT/POLYCHILDTYPE recursion found");

                STRUCT_DEF *pOtherStruct = FindNamedStruct(pField->typeName);

                FieldAssert(pField, pOtherStruct != NULL, "Couldn't find AUTO_STRUCT def for EMBEDDED_FLAT struct");

                DumpExterns(pFile,pOtherStruct);

                iRecurseDepth--;
            }
            else
            {    
                if (!pField->bFoundLateBind)
                {
                    char const* pTPIName = GetFieldTpiName(pField, false);
                    if (strcmp(pTPIName, "NULL") != 0)
                    {
                        fprintf(pFile, "extern ParseTable %s[];\n", pTPIName);
                    }
                }
            }
            break;
        case DATATYPE_UNOWNEDSTRUCT:
            if (pField->structTpiName[0] && strcmp(pField->structTpiName, "NULL") != 0)
            {
                if (!pField->bFoundLateBind)
                {
                    fprintf(pFile, "extern ParseTable %s[];\n", pField->structTpiName);
                }
            }
            break;
        }
    }
}

void StructParser::DumpStruct(FILE *pFile, STRUCT_DEF *pStruct)
{
    int iCount = 0;
        int i;

    CalcLongestUserFieldName(pStruct);

    if (pFile)
    {

        fprintf(pFile, "//autogenerated" "nocheckin\n");

        DumpExterns(pFile,pStruct);

        fprintf(pFile, "//Structparser.exe autogenerated ParseTable for struct %s\nParseTable %s%s[] =\n{\n",
            pStruct->structName, STRUCTPARSER_PREFIX, pStruct->structName);

        iCount += PrintStructTableInfoColumn(pFile, pStruct);

        iCount += PrintStructStart(pFile, pStruct);

        for (i=0; i < pStruct->iNumFields; i++)
        {
            pStruct->pStructFields[i]->iIndexInParseTable = iCount;
            iCount += DumpField(pFile, pStruct->pStructFields[i], pStruct->structName, pStruct->iLongestUserFieldNameLength, GetFieldSpecificPrefixString(pStruct, i));
        }

        for (i=0; i < pStruct->iNumIgnores; i++)
        {
            PrintIgnore(pFile, pStruct->ignores[i], pStruct->iLongestUserFieldNameLength, pStruct->bIgnoresAreStructParam[i]);
        }

        PrintStructEnd(pFile, pStruct);


        fprintf(pFile, "\t{ \"\", 0, 0 }\n};\n\n");

        for (i=0; i < pStruct->iNumFields; i++)
        {
            DumpIndexDefines(pFile, pStruct, pStruct->pStructFields[i], 0);

        }

        if (pStruct->bIAmAPolymorphicParent)
        {
            DumpPolyTable(pFile, pStruct);
        }
    }
}

void StructParser::DumpPolyTable(FILE *pFile, STRUCT_DEF *pStruct)
{
    int i;

    for (i=0; i < m_iNumStructs; i++)
    {
        if (strcmp(m_pStructs[i]->structNameIInheritFrom, pStruct->structName) == 0)
        {
            fprintf(pFile, "extern ParseTable parse_%s[];\n", m_pStructs[i]->structName);
        }
    }

    fprintf(pFile, "//The sizes in this table are filled in at auto-run fixup time\n");
    fprintf(pFile, "ParseTable polyTable_%s[] = \n{\n", pStruct->structName);

    for (i=0; i < m_iNumStructs; i++)
    {
        if (strcmp(m_pStructs[i]->structNameIInheritFrom, pStruct->structName) == 0)
        {
            fprintf(pFile, "\t{\"%s\", TOK_STRUCT_X, 0, 0, parse_%s },\n", 
                m_pStructs[i]->structName, m_pStructs[i]->structName);
        }
    }

    fprintf(pFile, "\t{ \"\", 0, 0 }\n};\n\n");
}




void StructParser::DumpIndexDefines(FILE *pFile, STRUCT_DEF *pStruct, STRUCT_FIELD_DESC *pField, int iOffset)
{
    if (pField->bFoundRequestIndexDefine)
    {
        char tempString[4096];
        sprintf(tempString, "PARSE_%s_%s_INDEX", pStruct->structName, pField->userFieldName);
        MakeStringAllAlphaNumAndUppercase(tempString);
        fprintf(pFile, "#define %s %d\n", tempString, pField->iIndexInParseTable + iOffset);
    }
    
    if (pField->eReferenceType == REFERENCETYPE_DIRECT && pField->eStorageType == STORAGETYPE_EMBEDDED && pField->bFlatEmbedded)
    {
        STRUCT_DEF *pSubStruct = FindNamedStruct(pField->typeName);

        if(pSubStruct)
        {
            int i;

            for (i=0; i < pSubStruct->iNumFields; i++)
            {
                DumpIndexDefines(pFile, pStruct, pSubStruct->pStructFields[i], pField->iIndexInParseTable + iOffset);
            }
        }
    }
}

void StructParser::DumpStructPrototype(FILE *pFile, STRUCT_DEF *pStruct)
{
    if (pFile)
    {
        fprintf(pFile, "\nextern ParseTable %s%s[];\n", STRUCTPARSER_PREFIX, pStruct->structName);

        int i;

        for (i=0; i < pStruct->iNumFields; i++)
        {
            DumpIndexDefines(pFile, pStruct, pStruct->pStructFields[i], 0);

        }

        if (pStruct->bIAmAPolymorphicParent)
        {
            fprintf(pFile, "\nextern ParseTable polyTable_%s[];\n", pStruct->structName);
        }

    }
}


#define FIRST bFirst ? ", " : "|"

void StructParser::DumpFieldFormatting(FILE *pFile, STRUCT_FIELD_DESC *pField)
{
    bool bFirst = true;

    //RGB fields automatically get just TOK_FORMAT_COLOR for formatting
    if (pField->eDataType == DATATYPE_RG || pField->eDataType == DATATYPE_RGB || pField->eDataType == DATATYPE_RGBA)
    {
        bFirst = false;
        fprintf(pFile, " , TOK_FORMAT_COLOR");
    }
    else
    {

        if (pField->eFormatType != FORMAT_NONE)
        {
            fprintf(pFile, " %s TOK_%s", FIRST, sFieldCommandStringReservedWords[pField->eFormatType + FIRST_FORMAT_RW - RW_COUNT - 1]);
            bFirst = false;
        }

        if (pField->lvWidth)
        {
            fprintf(pFile, " %s TOK_FORMAT_LVWIDTH(%d)", FIRST, pField->lvWidth);
            bFirst = false;
        }

        int i;

        for (i=0; i < FORMAT_FLAG_COUNT; i++)
        {
            if (pField->bFormatFlags[i])
            {
                fprintf(pFile, " %s TOK_%s", FIRST, sFieldCommandStringReservedWords[FIRST_FORMAT_FLAG_RW - RW_COUNT + i]);
                bFirst = false;
            }
        }
    }

    if (pField->pFormatString)
    {
        if (bFirst)
        {
            fprintf(pFile, ", 0 ");
        }

        fprintf(pFile, ", \"%s\"", pField->pFormatString);
    }
}

char *StructParser::GetIntPrefixString(STRUCT_FIELD_DESC *pField)
{
    static char workString[256];

    if (pField->iMinBits)
    {
        sprintf(workString, "TOK_MINBITS(%d) | ", pField->iMinBits);
    }
    else if (pField->iPrecision)
    {
        sprintf(workString, "TOK_PRECISION(%d) | ", pField->iPrecision);
    }
    else
    {
        workString[0] = 0;
    }

    return workString;
}

char *StructParser::GetFloatPrefixString(STRUCT_FIELD_DESC *pField)
{
    static char workString[256];

    if (pField->iFloatAccuracy)
    {
        sprintf(workString, "TOK_FLOAT_ROUNDING(%s) | ", sFieldCommandStringReservedWords[pField->iFloatAccuracy + FIRST_FLOAT_ACCURACY_RW - RW_COUNT - 1]);
    }
    else if (pField->iPrecision)
    {
        sprintf(workString, "TOK_PRECISION(%d) | ", pField->iPrecision);
    }
    else
    {
        workString[0] = 0;
    }

    return workString;
}

char const* StructParser::GetFieldTpiName(STRUCT_FIELD_DESC *pField, bool bIgnoreLateBind)
{
    static char workString[256];

    if (pField->bFoundLateBind && !bIgnoreLateBind)
    {
        return "NULL";
    }

    if (pField->pCurOverrideTPIName)
    {
        strcpy(workString, pField->pCurOverrideTPIName);
    }
    else if (pField->structTpiName[0])
    {
        strcpy(workString, pField->structTpiName);
    }
    else
    {
        sprintf(workString, "%s%s", STRUCTPARSER_PREFIX, pField->typeName);
    }

    return workString;
}

char *StructParser::GetFieldSpecificPrefixString(STRUCT_DEF *pStruct, int iFieldNum)
{
    static char sWorkString[MAX_NAME_LENGTH];

    int i;

    for (i=0; i < pStruct->iNumUnions; i++)
    {
        if (pStruct->unions[i].iFirstFieldNum <= iFieldNum && pStruct->unions[i].iLastFieldNum >= iFieldNum && pStruct->unions[i].name[0])
        {
            sprintf(sWorkString, "%s.", pStruct->unions[i].name);
            return sWorkString;
        }
    }

    sWorkString[0] = 0;
    return sWorkString;
}
void StructParser::AssertFieldHasNoDefault(STRUCT_FIELD_DESC *pField, char const* pErrorString)
{
    FieldAssert(pField, pField->defaultString[0] == 0 || pField->defaultString[0] == '0' && pField->defaultString[1] == 0 || strcmp(pField->defaultString, "NULL") == 0, pErrorString);
}

int StructParser::DumpFieldDirectEmbedded(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName, int iLongestFieldName, int iIndexInMultiLineField)
{
    int iCount = 0;
    static int iRecurseDepth = 0;

    switch (pField->eDataType)
    {
    case DATATYPE_INT:
        fprintf(pFile, "%sTOK_AUTOINT(%s, %s, %s), %s ", GetIntPrefixString(pField), pStructName, pField->curStructFieldName,
            pField->defaultString, pField->subTableName[0] ? pField->subTableName : "NULL");
        iCount++;
        break;

    case DATATYPE_FLOAT:
        fprintf(pFile, "%sTOK_F32(%s, %s, %s), NULL ", GetFloatPrefixString(pField), pStructName, pField->curStructFieldName,
            pField->defaultString);
        iCount++;
        break;

    case DATATYPE_STRUCT:
        AssertFieldHasNoDefault(pField, "Default values not allowed for embedded structs");
        if (pField->bFlatEmbedded)
        {
            iRecurseDepth++;

            FieldAssert(pField, iRecurseDepth < 20, "Presumed infinite EMBEDDED_FLAT/POLYCHILDTYPE recursion found");

            char prefixString[MAX_NAME_LENGTH];

            STRUCT_DEF *pOtherStruct = FindNamedStruct(pField->typeName);

            FieldAssert(pField, pOtherStruct != NULL, "Couldn't find AUTO_STRUCT def for EMBEDDED_FLAT struct");
            char tabs[MAX_TABS + 1];    

            MakeRepeatedCharacterString(tabs, NUMTABS(strlen(pField->baseStructFieldName)) - NUMTABS(iLongestFieldName) + 1, MAX_TABS, '\t');


            fprintf(pFile, "\t{ \"%s\", %sTOK_IGNORE | TOK_FLATEMBED },\n", pField->baseStructFieldName, tabs);

            iCount++;

            sprintf(prefixString, "%s.", pField->curStructFieldName);

            int i;

            for (i=0; i < pOtherStruct->iNumFields; i++)
            {
                char finalPrefixString[MAX_NAME_LENGTH];
                sprintf(finalPrefixString, "%s%s", prefixString, GetFieldSpecificPrefixString(pOtherStruct, i));
                pOtherStruct->pStructFields[i]->iIndexInParseTable = pField->iIndexInParseTable + iCount;
                iCount += DumpField(pFile, pOtherStruct->pStructFields[i], pStructName, iLongestFieldName, finalPrefixString);
            }

            iRecurseDepth--;
        }
        else if (pField->bIAmPolyChildTypeField)
        {
            iRecurseDepth++;

            FieldAssert(pField, iRecurseDepth < 20, "Presumed infinite EMBEDDED_FLAT/POLYCHILDTYPE recursion found");

            char prefixString[MAX_NAME_LENGTH];

            STRUCT_DEF *pOtherStruct = FindNamedStruct(pField->typeName);

            FieldAssert(pField, pOtherStruct != NULL && pOtherStruct->bIAmAPolymorphicParent, 
                "Couldn't find parent struct with POLYPARENTTYPE field");

            sprintf(prefixString, "%s.", pField->curStructFieldName);

            int i;

            if (pOtherStruct)
            for (i=0; i < pOtherStruct->iNumFields; i++)
            {
                char finalPrefixString[MAX_NAME_LENGTH];
                sprintf(finalPrefixString, "%s%s", prefixString, GetFieldSpecificPrefixString(pOtherStruct, i));
                pOtherStruct->pStructFields[i]->iIndexInParseTable = iCount;
                if (pOtherStruct->pStructFields[i]->bIAmPolyParentTypeField)
                {
                    FieldAssert(pOtherStruct->pStructFields[i], strcmp(pOtherStruct->pStructFields[i]->defaultString, "0") == 0, "POLYPARENTFIELD fields can not have default values");

                    strcpy(pOtherStruct->pStructFields[i]->defaultString, pField->myPolymorphicType);
                }
                
                iCount += DumpField(pFile, pOtherStruct->pStructFields[i], pStructName, iLongestFieldName, finalPrefixString);

                if (pOtherStruct->pStructFields[i]->bIAmPolyParentTypeField)
                {
                    strcpy(pOtherStruct->pStructFields[i]->defaultString, "0");
                }
            }

            iRecurseDepth--;

        }
        else
        {
            fprintf(pFile, "TOK_EMBEDDEDSTRUCT(%s, %s, %s)", pStructName, pField->curStructFieldName, GetFieldTpiName(pField, false));
            iCount++;
        }
        break;

    case DATATYPE_STRUCT_POLY:
        AssertFieldHasNoDefault(pField, "Default values not allowed for embedded polys");
        fprintf(pFile, "TOK_EMBEDDEDPOLYMORPH(%s, %s, polyTable_%s)", pStructName, pField->curStructFieldName, pField->typeName);
        iCount++;
        break;


    case DATATYPE_TIMESTAMP:
        AssertFieldHasNoDefault(pField, "Default values not allowed for timestamps");
        fprintf(pFile, "%sTOK_TIMESTAMP(%s, %s), NULL ", GetIntPrefixString(pField), pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_LINENUM:
        AssertFieldHasNoDefault(pField, "Default values not allowed for linenums");
        fprintf(pFile, "%sTOK_LINENUM(%s, %s), NULL ", GetIntPrefixString(pField), pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_FLAGS:
        fprintf(pFile, "%sTOK_FLAGS(%s, %s, %s), %s ", GetIntPrefixString(pField), pStructName, pField->curStructFieldName,
            pField->defaultString, pField->subTableName[0] ? pField->subTableName : "NULL");
        iCount++;
        break;

    case DATATYPE_BOOLFLAG:
        fprintf(pFile, "%sTOK_BOOLFLAG(%s, %s, %s), %s ", GetIntPrefixString(pField), pStructName, pField->curStructFieldName,
            pField->defaultString, pField->subTableName[0] ? pField->subTableName : "NULL");
        iCount++;
        break;

    case DATATYPE_RAW:
        if (pField->rawSizeString[0])
        {
            fprintf(pFile, "TOK_RAW_S(%s, %s, %s), NULL ", pStructName, pField->curStructFieldName, pField->rawSizeString);
            iCount++;
        }
        else
        {
            fprintf(pFile, "TOK_RAW(%s, %s), NULL ", pStructName, pField->curStructFieldName);
            iCount++;
        }
        break;

    case DATATYPE_VEC4:
        AssertFieldHasNoDefault(pField, "Default values not allowed for vec4s");
        fprintf(pFile, "TOK_VEC4(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_VEC3:
        AssertFieldHasNoDefault(pField, "Default values not allowed for vec3s");
        fprintf(pFile, "TOK_VEC3(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_VEC2:
        AssertFieldHasNoDefault(pField, "Default values not allowed for vec2s");
        fprintf(pFile, "TOK_VEC2(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_IVEC4:
        AssertFieldHasNoDefault(pField, "Default values not allowed for ivec4s");
        fprintf(pFile, "TOK_IVEC4(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_IVEC3:
        AssertFieldHasNoDefault(pField, "Default values not allowed for ivec3s");
        fprintf(pFile, "TOK_IVEC3(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_IVEC2:
        AssertFieldHasNoDefault(pField, "Default values not allowed for ivec2s");
        fprintf(pFile, "TOK_IVEC2(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_RGB:
        AssertFieldHasNoDefault(pField, "Default values not allowed for rgbx");
        fprintf(pFile, "TOK_RGB(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_RGBA:
        AssertFieldHasNoDefault(pField, "Default values not allowed for rgbas");
        fprintf(pFile, "TOK_RGBA(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_RG:
        AssertFieldHasNoDefault(pField, "Default values not allowed for rgs");
        fprintf(pFile, "TOK_RG(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_MAT3:
        AssertFieldHasNoDefault(pField, "Default values not allowed for mat3s");
        fprintf(pFile, "TOK_MAT3PYR(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_MAT4:
        AssertFieldHasNoDefault(pField, "Default values not allowed for mat4s");
        if (iIndexInMultiLineField == 0)
        {
            fprintf(pFile, "TOK_MAT4PYR_ROT(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        }
        else
        {
            fprintf(pFile, "TOK_MAT4PYR_POS(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        }
        iCount++;
        break;

    case DATATYPE_QUAT:
        AssertFieldHasNoDefault(pField, "Default values not allowed for quats");
        fprintf(pFile, "TOK_QUATPYR(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_STASHTABLE:
        AssertFieldHasNoDefault(pField, "Default values not allowed for stashtables");
        fprintf(pFile, "TOK_STASHTABLE(%s, %s), NULL", pStructName, pField->curStructFieldName);
        iCount++;
        break;

    case DATATYPE_REFERENCE:
        fprintf(pFile, "TOK_REFERENCE(%s, %s, %s, \"%s\") ", pStructName, pField->curStructFieldName,
            pField->defaultString, pField->refDictionaryName);
        iCount++;
        break;

    case DATATYPE_BIT:
        fprintf(pFile, "TOK_BIT, 0, 8, NULL");
        iCount++;
        break;

    case DATATYPE_MULTIVAL:
        fprintf(pFile, "TOK_MULTIVAL(%s, %s), NULL", pStructName, pField->curStructFieldName);
        iCount++;
        break;


    default:
        FieldAssert(pField, 0, "Unknown or unsupported data type (Direct Embedded)");
        break;
    }

    DumpFieldFormatting(pFile, pField);

    assert(iCount > 0);

    return iCount;
}

int StructParser::DumpFieldDirectArray(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName)
{
    AssertFieldHasNoDefault(pField, "Default values not allowed for arrays or fixed-size strings");
    switch (pField->eDataType)
    {
    case DATATYPE_INT:
        fprintf(pFile, "%sTOK_FIXED_ARRAY | TOK_AUTOINTARRAY(%s, %s, %s), %s ", GetIntPrefixString(pField), pStructName, pField->curStructFieldName, pField->arraySizeString,
            pField->subTableName[0] ? pField->subTableName : "NULL");
        break;

    case DATATYPE_FLOAT:
        fprintf(pFile, "%sTOK_FIXED_ARRAY | TOK_F32_X, offsetof(%s, %s), %s, NULL ", GetFloatPrefixString(pField), pStructName, pField->curStructFieldName, pField->arraySizeString);
        break;

    case DATATYPE_CHAR:
        AssertFieldHasNoDefault(pField, "Default values not allowed for fixed-size strings");
        fprintf(pFile, "TOK_FIXEDSTR(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        break;

    case DATATYPE_MULTIVAL:
        fprintf(pFile, "TOK_MULTIARRAY(%s, %s), NULL", pStructName, pField->curStructFieldName);
        break;

    default:
        FieldAssert(pField, 0, "Unknown or unsupported data type (Direct Array)");
        break;
    }

    DumpFieldFormatting(pFile, pField);

    return 1;

}

int StructParser::DumpFieldDirectEarray(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName)
{
    AssertFieldHasNoDefault(pField, "Default values not allowed for earrays");

    FieldAssert(pField, 0, "Unknown or unsupported data type (Direct EArray)");

    DumpFieldFormatting(pFile, pField);

    return 1;
}


int StructParser::DumpFieldPointerEmbedded(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName)
{
    switch (pField->eDataType)
    {
    case DATATYPE_INT:
        AssertFieldHasNoDefault(pField, "Default values not allowed for int arrays");
        fprintf(pFile, "%sTOK_INTARRAY(%s, %s),  %s", GetIntPrefixString(pField), pStructName, pField->curStructFieldName, 
            pField->subTableName[0] ? pField->subTableName : "NULL");
        break;

    case DATATYPE_FLOAT:
        AssertFieldHasNoDefault(pField, "Default values not allowed for float arrays");
        fprintf(pFile, "%sTOK_F32ARRAY(%s, %s), NULL ", GetFloatPrefixString(pField), pStructName, pField->curStructFieldName);
        break;


     case DATATYPE_CHAR:
        fprintf(pFile, "TOK_STRING(%s, %s, %s), NULL ", pStructName, pField->curStructFieldName, pField->defaultString);
        break;

    case DATATYPE_STRUCT:
        AssertFieldHasNoDefault(pField, "Default values not allowed for structs");
        if (pField->bFoundLateBind)
        {
            fprintf(pFile, "TOK_OPTIONALLATEBINDSTRUCT(%s, %s), ", pStructName, pField->curStructFieldName);
        }
        else
        {
            fprintf(pFile, "TOK_OPTIONALSTRUCT(%s, %s, %s), ", pStructName, pField->curStructFieldName,
                GetFieldTpiName(pField, false));
        }
        break;

    case DATATYPE_UNOWNEDSTRUCT:
        AssertFieldHasNoDefault(pField, "Default values not allowed for structs");
        if (pField->bFoundLateBind)
        {
            fprintf(pFile, "TOK_OPTIONALUNOWNEDLATEBINDSTRUCT(%s, %s), ", pStructName, pField->curStructFieldName);
        }
        else
        {
            fprintf(pFile, "TOK_OPTIONALUNOWNEDSTRUCT(%s, %s, %s), ", pStructName, pField->curStructFieldName, 
                pField->structTpiName[0] ? pField->structTpiName : "NULL");
        }
        break;

    case DATATYPE_STRUCT_POLY:
        AssertFieldHasNoDefault(pField, "Default values not allowed for polymorphs");
        fprintf(pFile, "TOK_OPTIONALPOLYMORPH(%s, %s, polyTable_%s), ", pStructName, pField->curStructFieldName,
            pField->typeName);
        break;

    case DATATYPE_FILENAME:
        AssertFieldHasNoDefault(pField, "Default values not allowed for TOK_FILENAME");
        fprintf(pFile, "TOK_FILENAME(%s, %s, NULL), NULL", pStructName, pField->curStructFieldName);
        break;
    case DATATYPE_CURRENTFILE:
        AssertFieldHasNoDefault(pField, "Default values not allowed for TOK_CURRENTFILE");
        fprintf(pFile, "TOK_CURRENTFILE(%s, %s), NULL", pStructName, pField->curStructFieldName);
        break;

    case DATATYPE_USEDFIELD:
        AssertFieldHasNoDefault(pField, "Default values not allowed for USEDFIELD");
        fprintf(pFile, "TOK_USEDFIELD(%s, %s, %s), NULL", pStructName, pField->curStructFieldName, pField->usedFieldName);
        break;

    case DATATYPE_POINTER:
        AssertFieldHasNoDefault(pField, "Default values not allowed for TOK_POINTER");
        fprintf(pFile, "TOK_POINTER(%s, %s, %s), NULL", pStructName, pField->curStructFieldName, pField->pointerSizeString);
        break;

    default:
        FieldAssert(pField, 0, "Unknown or unsupported data type (Pointer Embedded)");
        break;


    }

    DumpFieldFormatting(pFile, pField);

    return 1;
}
int StructParser::DumpFieldPointerArray(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName)
{
        FieldAssert(pField, 0, "Unknown or unsupported data type (Pointer Array)");

        return 1;

}
int StructParser::DumpFieldPointerEarray(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName)
{
        AssertFieldHasNoDefault(pField, "Default values not allowed for earrays");
    switch (pField->eDataType)
    {


    case DATATYPE_STRUCT:
        if (pField->bFoundLateBind)
        {
            fprintf(pFile, "TOK_LATEBINDSTRUCT(%s, %s) ", pStructName, pField->curStructFieldName);
        }
        else
        {
            fprintf(pFile, "TOK_STRUCT(%s, %s, %s) ", pStructName, pField->curStructFieldName, GetFieldTpiName(pField, false));
        }
        break;

    case DATATYPE_UNOWNEDSTRUCT:
        if (pField->bFoundLateBind)
        {
            fprintf(pFile, "TOK_UNOWNEDLATEBINDSTRUCT(%s, %s) ", pStructName, pField->curStructFieldName);
        }
        else
        {
            fprintf(pFile, "TOK_UNOWNEDSTRUCT(%s, %s, %s) ", pStructName, pField->curStructFieldName, pField->structTpiName[0] ? GetFieldTpiName(pField, false) : "NULL");
        }
        break;

    case DATATYPE_STRUCT_POLY:
        fprintf(pFile, "TOK_POLYMORPH(%s, %s, polyTable_%s) ", pStructName, pField->curStructFieldName, pField->typeName);
        break;

    case DATATYPE_TOKENIZERPARAMS:
        fprintf(pFile, "TOK_UNPARSED(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        break;

    case DATATYPE_TOKENIZERFUNCTIONCALL:
        fprintf(pFile, "TOK_FUNCTIONCALL(%s, %s, %s), NULL ", pStructName, pField->curStructFieldName, pField->defaultString);
        break;

    case DATATYPE_CHAR:
        fprintf(pFile, "TOK_STRINGARRAY(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        break;

    case DATATYPE_FILENAME:
        fprintf(pFile, "TOK_FILENAMEARRAY(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        break;

    case DATATYPE_MULTIVAL:
        fprintf(pFile, "TOK_MULTIEARRAY(%s, %s), NULL ", pStructName, pField->curStructFieldName);
        break;

    default:
        FieldAssert(pField, 0, "Unknown or unsupported data type (Pointer Earray)");
        break;
    }


    DumpFieldFormatting(pFile, pField);

    return 1;
}

int StructParser::DumpField(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName, int iLongestUserFieldNameLength, char *pStructFieldNamePrefix)
{
    int iCount = 0;

    STRUCT_COMMAND *pCommand = pField->pFirstBeforeCommand;
    while (pCommand)
    {
        DumpCommand(pFile, pField, pCommand, iLongestUserFieldNameLength);
        iCount++;
        pCommand = pCommand->pNext;
    }


//for special things like Mat4 which end up taking up more than one ParseTable column
    int iNumLines = GetNumLinesFieldTakesUp(pField);

    if (iNumLines > 1)
    {
        int i;

        for (i=0; i < iNumLines; i++)
        {
            char curFieldName[MAX_NAME_LENGTH];
            
            sprintf(curFieldName, "%s_%s",
                GetMultiLineFieldNamePrefix(pField, i), pField->userFieldName);

            iCount += DumpFieldSpecifyUserFieldName(pFile, pField, pStructName, curFieldName, 
                false, iLongestUserFieldNameLength, i);
        }

        
        return iCount;

    }

    if (pField->iNumIndexes)
    {
        FieldAssert(pField, pField->iNumRedundantNames == 0, "Can't have redundant names and INDEX in the same field");

        int i;

        for (i=0; i < pField->iNumIndexes; i++)
        {
            sprintf(pField->curStructFieldName, "%s%s[%s]", pStructFieldNamePrefix, pField->baseStructFieldName, pField->pIndexes[i].indexString);

            iCount += DumpFieldSpecifyUserFieldName(pFile, pField, pStructName, pField->pIndexes[i].nameString, false, iLongestUserFieldNameLength, 0);
        }


    
        return iCount;
    }
    

    sprintf(pField->curStructFieldName, "%s%s", pStructFieldNamePrefix, pField->baseStructFieldName);

    iCount += DumpFieldSpecifyUserFieldName(pFile, pField, pStructName, pField->userFieldName, pField->bFoundRedundantToken, iLongestUserFieldNameLength, 0);

    int i;

    for (i=0; i < pField->iNumRedundantNames; i++)
    {
        iCount += DumpFieldSpecifyUserFieldName(pFile, pField, pStructName, pField->redundantNames[i], true, iLongestUserFieldNameLength, 0);
    }

    if (pField->iNumRedundantStructInfos)
    {
        FieldAssert(pField, pField->eDataType == DATATYPE_STRUCT, "REDUNDANT_STRUCT found for non-struct token");

        for (i=0; i < pField->iNumRedundantStructInfos; i++)
        {
            pField->pCurOverrideTPIName = pField->redundantStructs[i].subTable;
            iCount += DumpFieldSpecifyUserFieldName(pFile, pField, pStructName, pField->redundantStructs[i].name, true, iLongestUserFieldNameLength, 0);
        }

        pField->pCurOverrideTPIName = NULL;
    }

    pCommand = pField->pFirstCommand;
    while (pCommand)
    {
        DumpCommand(pFile, pField, pCommand, iLongestUserFieldNameLength);
        iCount++;
        pCommand = pCommand->pNext;
    }

    return iCount;

}

//returns true if the field dumping will print out all the { }\n stuff. In particular, this is used for
//EMBEDDED_FLAT structs and the embedded struct used in polymorphism
bool StructParser::FieldDumpsItselfCompletely(STRUCT_FIELD_DESC *pField)
{
    return pField->eDataType == DATATYPE_STRUCT && pField->eStorageType == STORAGETYPE_EMBEDDED && pField->eReferenceType == REFERENCETYPE_DIRECT && pField->bFlatEmbedded
        || pField->bIAmPolyChildTypeField;
}


char *StructParser::GetAllUserFlags(STRUCT_FIELD_DESC *pField)
{
    static char sString[MAX_USER_FLAGS * (MAX_NAME_LENGTH + 1)];

    sString[0] = 0;

    int i;

    for (i=0; i < pField->iNumUserFlags; i++)
    {
        sprintf(sString + strlen(sString), "%s | ", pField->userFlags[i]);
    }

    return sString;
}



void StructParser::DumpCommand(    FILE *pFile, STRUCT_FIELD_DESC *pField, STRUCT_COMMAND *pCommand, int iLongestFieldNameLength)
{
    char tabs[MAX_TABS + 1];    

    MakeRepeatedCharacterString(tabs, NUMTABS(strlen(pCommand->pCommandName)) - NUMTABS(iLongestFieldNameLength) + 1, MAX_TABS, '\t');

    if (pCommand->pCommandExpression)
    {
        char escapedExression[1024];
        AddCStyleEscaping(escapedExression, pCommand->pCommandExpression, 1024);

        fprintf(pFile, "\t{ \"%s\", %sTOK_COMMAND, 0, (intptr_t)\"%s\", NULL, 0, \" commandExpr = \\\"%s\\\" \" },\n",
            pCommand->pCommandName, tabs, pCommand->pCommandString, escapedExression);
    }
    else
    {
        fprintf(pFile, "\t{ \"%s\", %sTOK_COMMAND, 0, (intptr_t)\"%s\" },\n",
            pCommand->pCommandName, tabs, pCommand->pCommandString);
    }
}


int StructParser::DumpFieldSpecifyUserFieldName(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName, char *pUserFieldName, 
    bool bNameIsRedundant, int iLongestFieldNameLength, int iIndexInMultiLineField)
{
    int iCount = 0;

    char tabs[MAX_TABS + 1];    
    MakeRepeatedCharacterString(tabs, NUMTABS(strlen(pUserFieldName)) - NUMTABS(iLongestFieldNameLength) + 1, MAX_TABS, '\t');

    if (!FieldDumpsItselfCompletely(pField))
    {
        fprintf(pFile, "\t{ \"%s\",%s%s%s%s%s%s%s%s",
            pUserFieldName, 
            tabs,

            pField->bIAmPolyParentTypeField ? "TOK_OBJECTTYPE | " : "",
            bNameIsRedundant ? "TOK_REDUNDANTNAME | " : "",
            pField->bFoundStructParam ? "TOK_STRUCTPARAM | " : "",
            GetAllUserFlags(pField),
            pField->bFoundPersist ? "TOK_PERSIST | " : "",
            pField->bFoundNoTransact ? "TOK_NO_TRANSACT | " : "",
            pField->bIsInheritanceStruct ? "TOK_INHERITANCE_STRUCT | " : "");


        int i;

        for (i=0; i < sizeof(pField->iTypeFlagsFound) * 8; i++)
        {
            if (pField->iTypeFlagsFound & (1 << i))
            {
                fprintf(pFile, "TOK_%s | ", sTokTypeFlags[i]);
            }
        }

        if (!bNameIsRedundant)
        {
            int i;

            for (i=0; i < sizeof(pField->iTypeFlags_NoRedundantRepeatFound) * 8; i++)
            {
                if (pField->iTypeFlags_NoRedundantRepeatFound & (1 << i))
                {
                    fprintf(pFile, "TOK_%s | ", sTokTypeFlags_NoRedundantRepeat[i]);
                }
            }
        }
    }

    switch(pField->eReferenceType)
    {
    case REFERENCETYPE_DIRECT:
        switch (pField->eStorageType)
        {
        case STORAGETYPE_EMBEDDED:
            iCount += DumpFieldDirectEmbedded(pFile, pField, pStructName, iLongestFieldNameLength, iIndexInMultiLineField);
            break;

        case STORAGETYPE_ARRAY:
            iCount += DumpFieldDirectArray(pFile, pField, pStructName);
            break;

        case STORAGETYPE_EARRAY:
            iCount += DumpFieldDirectEarray(pFile, pField, pStructName);
            break;
        }
        break;

    case REFERENCETYPE_POINTER:
    switch (pField->eStorageType)
        {
        case STORAGETYPE_EMBEDDED:
            iCount += DumpFieldPointerEmbedded(pFile, pField, pStructName);
            break;

        case STORAGETYPE_ARRAY:
            iCount += DumpFieldPointerArray(pFile, pField, pStructName);
            break;

        case STORAGETYPE_EARRAY:
            iCount += DumpFieldPointerEarray(pFile, pField, pStructName);
            break;
        }
        break;
    }    
            
    if (!FieldDumpsItselfCompletely(pField))
    {
        fprintf(pFile, "},\n");
    }

    return iCount;
}

void StructParser::PrintIgnore(FILE *pFile, char *pIgnoreString, int iLongestFieldNameLength, bool bIgnoreIsStructParam)
{
    char tabs[MAX_TABS + 1];
    MakeRepeatedCharacterString(tabs, NUMTABS(strlen(pIgnoreString)) - NUMTABS(iLongestFieldNameLength) + 1, MAX_TABS, '\t');


    fprintf(pFile, "\t{ \"%s\",%sTOK_IGNORE%s, 0 },\n", pIgnoreString, tabs, bIgnoreIsStructParam ? " | TOK_STRUCTPARAM" : "");
}




bool StructParser::IsLinkName(char *pString)
{
    return false;
}

bool StructParser::IsFloatName(char *pString)
{
    return StringIsInList(pString, sFloatNameList);
}

bool StructParser::IsIntName(char *pString)
{
    return StringIsInList(pString, sIntNameList);
}

bool StructParser::IsCharName(char *pString)
{
    return (strcmp(pString, "char") == 0);
}



bool StructParser::LoadStoredData(bool bForceReset)
{
    return true;
}
void StructParser::SetProjectPathAndName(char const* srcPath, char const* commonPath, char const* projectName)
{
}

void StructParser::TemplateFileNameFromSourceFileName(char* pTemplateName, char* pTemplateHeaderName, char const* pSourceName)
{
    char workName[MAX_PATH];
    strcpy(workName, GetFileNameWithoutDirectories(pSourceName));
    int iLen = (int)strlen(workName);

    int i;

    for (i=0; i < iLen; i++)
    {
        if (workName[i] == '.')
        {
            workName[i] = '_';
        }
    }

    sprintf(pTemplateName, "%s\\AutoGen%s_ast.c", m_pParent->GetSoureDir(), workName);
    sprintf(pTemplateHeaderName, "%s\\AutoGen%s_ast.h", m_pParent->GetSoureDir(), workName);
}

void StructParser::ResetSourceFile(char const* pSourceFileName)
{
    char templateFileName[MAX_PATH];
    char templateHeaderFileName[MAX_PATH];

    TemplateFileNameFromSourceFileName(templateFileName, templateHeaderFileName, pSourceFileName);

    DeleteFileA(templateFileName);
    DeleteFileA(templateHeaderFileName);

        int i = 0;

    while (i < m_iNumStructs)
    {
        if (AreFilenamesEqual(m_pStructs[i]->sourceFileName, pSourceFileName))
        {
            DeleteStruct(i);

            memmove(&m_pStructs[i], &m_pStructs[i + 1], (m_iNumStructs - i - 1) * sizeof(void*));
            m_iNumStructs--;

        }
        else
        {
            i++;
        }
    }


}

bool StructParser::WriteOutData(void)
{
    int iNumFileNames = 0;
    char fileNames[MAX_FILES_IN_PROJECT][MAX_PATH];

    int i, j;

    


    for (i=0; i < m_iNumStructs; i++)
    {
        FixupFieldTypes_RightBeforeWritingData(m_pStructs[i]);

        bool bIsUnique = true;

        for (j = 0; j < iNumFileNames; j++)
        {
            if (strcmp(m_pStructs[i]->sourceFileName, fileNames[j]) == 0)
            {
                bIsUnique = false;
                break;
            }
        }

        if (bIsUnique)
        {
            strcpy(fileNames[iNumFileNames++], m_pStructs[i]->sourceFileName);
        }
    }

    for (i=0; i < m_iNumEnums; i++)
    {
        bool bIsUnique = true;

        for (j = 0; j < iNumFileNames; j++)
        {
            if (strcmp(m_pEnums[i]->sourceFileName, fileNames[j]) == 0)
            {
                bIsUnique = false;
                break;
            }
        }

        if (bIsUnique)
        {
            strcpy(fileNames[iNumFileNames++], m_pEnums[i]->sourceFileName);
        }
    }

    for (i=0; i < iNumFileNames; i++)
    {
        m_pParent->SetExtraDataFlagForFile(fileNames[i], 1 << m_iIndexInParent);
        WriteOutDataSingleFile(fileNames[i]);
    }

    return false;
}

bool StructParser::StringIsContainerName(STRUCT_DEF *pStruct, char *pString)
{
    if (m_pParent->GetDictionary()->FindIdentifier(pString) == IDENTIFIER_STRUCT_CONTAINER)
    {
        return true;
    }

    int i;

    for (i=0; i < pStruct->iNumFields; i++)
    {
        if (pStruct->pStructFields[i]->eDataType == DATATYPE_STRUCT 
            && pStruct->pStructFields[i]->bFoundForceContainer
            && strcmp(pStruct->pStructFields[i]->typeName, pString) == 0)
        {
            return true;
        }
    }

    return false;
}


#define NONCONST_SUFFIX "_AutoGen_NoConst"

void StructParser::DumpNonConstCopy(FILE *pFile, STRUCT_DEF *pStruct)
{
    char structsPrototyped[MAX_NAME_LENGTH][MAX_FIELDS];
    int iNumStructsPrototyped = 0;

    Tokenizer tokenizer;
    tokenizer.LoadFromFile(pStruct->sourceFileName);
    tokenizer.SetOffset(pStruct->iPreciseStartingOffsetInFile, pStruct->iPreciseStartingLineNumInFile);

    Token token;
    enumTokenType eType;

    fprintf(pFile, "\n\n//This is the autogenerated non-const copy of %s\n\n//Here are some struct prototypes for %s\n", 
        pStruct->structName, pStruct->structName);

    if (pStruct->pNonConstPrefixString)
    {
        fprintf(pFile, "\n%s\n\n", pStruct->pNonConstPrefixString);
    }

    int i;

    for (i=0; i < pStruct->iNumFields; i++)
    {
        if (pStruct->pStructFields[i]->eDataType == DATATYPE_STRUCT)
        {
            char sourceFileName[MAX_PATH];

            strcpy(structsPrototyped[iNumStructsPrototyped++], pStruct->pStructFields[i]->typeName);

            if (m_pParent->GetDictionary()->FindIdentifierAndGetSourceFile(pStruct->pStructFields[i]->typeName, sourceFileName) == IDENTIFIER_STRUCT_CONTAINER)
            {
                //for embedded structs that come from .h files, include the header rather than
                //making a prototype
                if (strcmp(sourceFileName + strlen(sourceFileName) - 2, ".h") == 0
                    && pStruct->pStructFields[i]->eStorageType == STORAGETYPE_EMBEDDED
                    && pStruct->pStructFields[i]->eReferenceType == REFERENCETYPE_DIRECT)
                {
                    char simpleSourceFileName[MAX_PATH];
                    strcpy(simpleSourceFileName, GetFileNameWithoutDirectories(sourceFileName));
                    TruncateStringAtLastOccurrence(simpleSourceFileName, '.');
                    fprintf(pFile, "#include \"AutoGen\\%s_h_ast.h\"\n", simpleSourceFileName);
                }
                else
                {
                    fprintf(pFile, "typedef struct %s%s %s%s;\n", pStruct->pStructFields[i]->typeName, 
                        NONCONST_SUFFIX, pStruct->pStructFields[i]->typeName, NONCONST_SUFFIX);
                }
            }
            else if (pStruct->pStructFields[i]->bFoundForceContainer)
            {
                fprintf(pFile, "typedef struct %s%s %s%s;\n", pStruct->pStructFields[i]->typeName, 
                        NONCONST_SUFFIX, pStruct->pStructFields[i]->typeName, NONCONST_SUFFIX);
            }
            else
            {
                fprintf(pFile, "typedef struct %s %s;\n", pStruct->pStructFields[i]->typeName, pStruct->pStructFields[i]->typeName);
            }

        }
    }

    fprintf(pFile, "\n");

    int iBraceDepth = 0;

    do
    {
        bool bFound = false;
    
        eType = tokenizer.GetNextToken(&token);

        tokenizer.Assert(eType != TOKEN_NONE, "File corruption or something bad while making non-const version");

        if (eType == TOKEN_IDENTIFIER)
        {
            if (strcmp(token.sVal, "const") == 0)
            {
                bFound = true;
            }
            else if (strcmp(token.sVal, AUTOSTRUCT_EXCLUDE) == 0)
            {
                bFound = true;
            }
            else if (strcmp(token.sVal, AUTOSTRUCT_EXTRA_DATA) == 0
                || strcmp(token.sVal, AUTOSTRUCT_MACRO) == 0
                || strcmp(token.sVal, AUTOSTRUCT_PREFIX) == 0
                || strcmp(token.sVal, AUTOSTRUCT_NOT) == 0
                || strcmp(token.sVal, AUTOSTRUCT_SUFFIX) == 0)
            {
                bFound = true;
                tokenizer.GetNextToken(&token);
                tokenizer.GetSpecialStringTokenWithParenthesesMatching(&token);
            }
            else if (strcmp(token.sVal, pStruct->structName) == 0)
            {
                bFound = true;
                fprintf(pFile, "%s%s ", token.sVal, NONCONST_SUFFIX);
            }
            else if (StringIsContainerName(pStruct, token.sVal))
            {
                //only do suffixing on structs that are actively used in structparsed fields
                for (i=0; i < iNumStructsPrototyped; i++)
                {
                    if (strcmp(token.sVal, structsPrototyped[i]) == 0)
                    {
                        bFound = true;
                        fprintf(pFile, "%s%s ", token.sVal, NONCONST_SUFFIX);
                        break;
                    }
                }
            } 
            else if (strcmp(token.sVal, "CONST_REF_TO") == 0)
            {
                bFound = true;
                fprintf(pFile, "REF_TO ");
            }
            else
            {
                int i;
                for (i=0; i < sizeof(gSpecialConstKeywords) / sizeof(gSpecialConstKeywords[0]); i++)
                {
                    if (strcmp(token.sVal, gSpecialConstKeywords[i].pStartingString) == 0)
                    {
                        fprintf(pFile, "%s ", gSpecialConstKeywords[i].pNonConstString);
                        bFound = true;
                        break;
                    }
                }
            }
        }

        if (!bFound)
        {
            tokenizer.StringifyToken(&token);
            fprintf(pFile, "%s ", token.sVal);
        }

        if (eType == TOKEN_RESERVEDWORD)
        {
            if (token.iVal == RW_LEFTBRACE)
            {
                iBraceDepth++;
            }
            else if (token.iVal == RW_RIGHTBRACE)
            {
                iBraceDepth--;
            }
        }

        if (!bFound)
        {
            if (eType == TOKEN_RESERVEDWORD && (token.iVal == RW_LEFTBRACE || token.iVal == RW_RIGHTBRACE || token.iVal == RW_SEMICOLON))
            {
                fprintf(pFile, "\n");
                int i;

                for (i=0; i < iBraceDepth ; i++)
                {
                    fprintf(pFile, "\t");
                }
            }
        }

    } while (!(iBraceDepth == 0 && eType == TOKEN_RESERVEDWORD && token.iVal == RW_SEMICOLON));


    fprintf(pFile, "\n\n\n");

    if (pStruct->pNonConstSuffixString)
    {
        fprintf(pFile, "\n%s\n\n", pStruct->pNonConstSuffixString);
    }

}

void StructParser::WriteOutDataSingleFile(char *pFileName)
{

    int i;

    char templateFileName[MAX_PATH];
    char templateHeaderFileName[MAX_PATH];
    char tmpFileName[MAX_PATH];
    char tmpHeaderFileName[MAX_PATH];

    TemplateFileNameFromSourceFileName(templateFileName, templateHeaderFileName, pFileName);

    // Write to a temp file and if the contents differ, update the real file (in order to support IncrediBuild better)
    sprintf(tmpFileName, "%s.tmp", templateFileName);
    sprintf(tmpHeaderFileName, "%s.tmp", templateHeaderFileName);

    FILE *pMainFile = NULL;
    FILE *pHeaderFile = NULL;
    
    pMainFile = fopen_nofail(tmpFileName, "wt");
    fprintf(pMainFile, "#include <utilitieslib/utils/textparser.h>\n\n");

#if GENERATE_FAKE_DEPENDENCIES
    fprintf(pMainFile, "//#ifed-out include to fool incredibuild dependencies\n#if 0\n#include \"%s\"\n#endif\n\n", pFileName);
#endif

    pHeaderFile = fopen_nofail(tmpHeaderFileName, "wt");
    WriteHeaderFileStart(pHeaderFile, pFileName);

    for (i=0; i < m_iNumEnums; i++)
    {
        if (strcmp(m_pEnums[i]->sourceFileName, pFileName) == 0)
        {
            DumpEnum(pMainFile, m_pEnums[i]);
            DumpEnumPrototype(pHeaderFile, m_pEnums[i]);
        }
    }
    

    for (i=0; i < m_iNumStructs; i++)
    {
        if (strcmp(m_pStructs[i]->sourceFileName, pFileName) == 0)
        {
    
            DumpStruct(pMainFile, m_pStructs[i]);
            DumpStructPrototype(pHeaderFile, m_pStructs[i]);
            DumpStructInitFunc(pMainFile, m_pStructs[i]);

            if (m_pStructs[i]->bIsContainer)
            {
                DumpNonConstCopy(pHeaderFile, m_pStructs[i]);
            }
        }
    }

    fclose(pMainFile);

    WriteHeaderFileEnd(pHeaderFile, pFileName);
    fclose(pHeaderFile);

    Tokenizer::StaticAssertf(MoveFileExA(tmpFileName, templateFileName, MOVEFILE_REPLACE_EXISTING) != 0, "Could not replace %s with %s", templateFileName, tmpFileName);
    Tokenizer::StaticAssertf(MoveFileExA(tmpHeaderFileName, templateHeaderFileName, MOVEFILE_REPLACE_EXISTING) != 0, "Could not replace %s with %s", templateHeaderFileName, tmpHeaderFileName);
}

bool StructParser::StructHasWikiComments(STRUCT_DEF *pStruct)
{
    int i;

    for (i=0; i < pStruct->iNumFields; i++)
    {
        if (pStruct->pStructFields[i]->iNumWikiComments)
        {
            return true;
        }
    }

    return false;
}

StructParser::STRUCT_DEF *StructParser::FindNamedStruct(char *pStructName)
{
    int i;

    for (i=0; i < m_iNumStructs; i++)
    {
        if (strcmp(m_pStructs[i]->structName, pStructName) == 0)
        {
            return m_pStructs[i];
        }
    }

    return NULL;
}


char const* pPrefixes[] = 
{
    "p",
    "e",
    "i",
    "f",
    "pp",
    "pi",
    "pch",
    "b",
    "pc",
    "ea",
    "b",
    "v",
};

void StructParser::FixupFieldName(STRUCT_FIELD_DESC *pField, bool bStripUnderscores, bool bNoPrefixStripping, bool bForceUseActualFieldName,
    bool bAlwaysIncludeActualFieldNameAsRedundant)
{
    if (strcmp(pField->userFieldName, pField->baseStructFieldName) == 0)
    {
        int i;

        if (!bNoPrefixStripping)
        {
            for (i=0; i < sizeof(pPrefixes) / sizeof(pPrefixes[0]); i++)
            {
                int iPrefixLength = (int)strlen(pPrefixes[i]);

                if (strncmp(pPrefixes[i], pField->userFieldName, iPrefixLength) == 0)
                {
                    if (pField->userFieldName[iPrefixLength] >= 'A' && pField->userFieldName[iPrefixLength] <= 'Z' || pField->userFieldName[iPrefixLength] == '_') 
                    {
                        int iNameLength = (int)strlen(pField->userFieldName);

                        memmove(pField->userFieldName, pField->userFieldName + iPrefixLength, iNameLength - iPrefixLength + 1);
                        
                        break;
                    }
                }
            }
        }

        if (bStripUnderscores)
        {
            int iLen = (int)strlen(pField->userFieldName);

            i = 0;

            while (i < iLen)
            {
                if (pField->userFieldName[i] == '_')
                {
                    memmove(pField->userFieldName + i, pField->userFieldName + i + 1, iLen - i);
                    iLen -= 1;
                }
                else
                {
                    i++;
                }
            }
        }
    }

    if (bForceUseActualFieldName)
    {
        if (strcmp(pField->userFieldName, pField->baseStructFieldName) == 0)
        {
            return;
        }

        FieldAssert(pField, pField->iNumRedundantNames < MAX_REDUNDANT_NAMES_PER_FIELD, "Too many redundant names with FORCE_USE_ACTUAL_FIELD_NAME");
    
        memmove(pField->redundantNames[1], pField->redundantNames[0], pField->iNumRedundantNames * sizeof(pField->redundantNames[0]));
        pField->iNumRedundantNames++;
        strcpy(pField->redundantNames[0], pField->userFieldName);
        strcpy(pField->userFieldName, pField->baseStructFieldName);
    }
    else if (bAlwaysIncludeActualFieldNameAsRedundant)
    {
        if (strcmp(pField->userFieldName, pField->baseStructFieldName) == 0)
        {
            return;
        }

        FieldAssert(pField, pField->iNumRedundantNames < MAX_REDUNDANT_NAMES_PER_FIELD, "Too many redundant names with FORCE_USE_ACTUAL_FIELD_NAME");

        strcpy(pField->redundantNames[pField->iNumRedundantNames++], pField->baseStructFieldName);
    }

}


void StructParser::WriteHeaderFileStart(FILE *pFile, char *pSourceName)
{
    const char *pShortenedFileName = GetFileNameWithoutDirectories(pSourceName);
    char macroName[MAX_PATH];
    sprintf(macroName, "_%s_AST_H_", pShortenedFileName);

    MakeStringAllAlphaNumAndUppercase(macroName);

    fprintf(pFile, "#ifndef %s\n"
                   "#define %s\n"
                   "#include <utilitieslib/utils/textparser.h>\n"
                   "//This file is autogenerated and contains TextParserInfo prototypes from %s\n"
                   "//autogenerated" "nocheckin\n\n", macroName, macroName, pShortenedFileName);

#if GENERATE_FAKE_DEPENDENCIES
    fprintf(pFile, "//#ifed-out include to fool incredibuild dependencies\n#if 0\n#include \"%s\"\n#endif\n\n", pSourceName);
#endif
}
    

    
void StructParser::WriteHeaderFileEnd(FILE *pFile, char *pSourceName)
{
    fprintf(pFile, "\n\n#endif\n\n");
}


int StructParser::PrintStructTableInfoColumn(FILE *pFile, STRUCT_DEF *pStruct)
{
    char tabs[MAX_TABS + 1];
    MakeRepeatedCharacterString(tabs, NUMTABS(strlen(pStruct->structName)) - NUMTABS(pStruct->iLongestUserFieldNameLength) + 1, MAX_TABS, '\t');
    fprintf(pFile, "\t{ \"%s\", %sTOK_IGNORE | TOK_PARSETABLE_INFO, sizeof(%s), 0, NULL, 0 },\n", pStruct->structName, tabs, pStruct->structName);
    return 1;
}

int StructParser::PrintStructStart(FILE *pFile, STRUCT_DEF *pStruct)
{
    if (pStruct->bHasStartString)
    {
        if (pStruct->startString[0])
        {
            char tabs[MAX_TABS + 1];
            
            MakeRepeatedCharacterString(tabs, NUMTABS(strlen(pStruct->startString)) - NUMTABS(pStruct->iLongestUserFieldNameLength) + 1, MAX_TABS, '\t');

            fprintf(pFile, "\t{ \"%s\",%sTOK_START, 0 },\n", pStruct->startString, tabs);
            return 1;
        }
    }
    else if (IsStructAllStructParams(pStruct))
    {
    }
    else
    {
        char tabs[MAX_TABS + 1];
        
        MakeRepeatedCharacterString(tabs, NUMTABS(1) - NUMTABS(pStruct->iLongestUserFieldNameLength) + 1, MAX_TABS, '\t');

        fprintf(pFile, "\t{ \"{\",%sTOK_START, 0 },\n", tabs);
        return 1;
    }

    return 0;
}

void StructParser::PrintStructEnd(FILE *pFile, STRUCT_DEF *pStruct)
{
    if (pStruct->bHasEndString)
    {
        if (pStruct->endString[0])
        {
            char tabs[MAX_TABS + 1];
            
            MakeRepeatedCharacterString(tabs, NUMTABS(strlen(pStruct->endString)) - NUMTABS(pStruct->iLongestUserFieldNameLength) + 1, MAX_TABS, '\t');

            fprintf(pFile, "\t{ \"%s\",%sTOK_END, 0 },\n", pStruct->endString, tabs);
        }

    }
    else if (IsStructAllStructParams(pStruct))
    {
        char tabs[MAX_TABS + 1];
        
        MakeRepeatedCharacterString(tabs, NUMTABS(2) - NUMTABS(pStruct->iLongestUserFieldNameLength) + 1, MAX_TABS, '\t');

        fprintf(pFile, "\t{ \"\\n\",%sTOK_END, 0 },\n", tabs);
    }
    else
    {
        char tabs[MAX_TABS + 1];
        
        MakeRepeatedCharacterString(tabs, NUMTABS(1) - NUMTABS(pStruct->iLongestUserFieldNameLength) + 1, MAX_TABS, '\t');

        fprintf(pFile, "\t{ \"}\",%sTOK_END, 0 },\n", tabs);
    }
}
/*
StaticDefineInt FieldTypeEnum[] =
{
    DEFINE_INT
    { "Normal", FIELDTYPE_NORMAL},
    DEFINE_END
};
*/


void StructParser::DumpEnum(FILE *pFile, ENUM_DEF *pEnum)
{
    int iPrefixLength = 0;
    int i, j;

    if (pEnum->iNumEntries > 1 && !pEnum->bNoPrefixStripping)
    {
        char prefixString[MAX_NAME_LENGTH];
        strcpy(prefixString, pEnum->pEntries[0].inCodeName);

        iPrefixLength = (int)strlen(prefixString);


        for (i=1; i < pEnum->iNumEntries; i++)
        {
            for (j=0; j < iPrefixLength; j++)
            {
                if (prefixString[j] != pEnum->pEntries[i].inCodeName[j])
                {
                    iPrefixLength = j;
                    break;
                }
            }
        }
    }


    fprintf(pFile, "\n\n//auto-generated staticdefine for enum %s\n//autogenerated" "nocheckin\n", pEnum->enumName);
    fprintf(pFile, "StaticDefineInt %sEnum[] =\n{\n\tDEFINE_INT\n", pEnum->enumName);

    for (i=0; i < pEnum->iNumEntries; i++)
    {
        fprintf(pFile, "\t{ \"%s\", %s},\n", 
            pEnum->pEntries[i].iNumExtraNames ? pEnum->pEntries[i].extraNames[0] : pEnum->pEntries[i].inCodeName + iPrefixLength, 
            pEnum->pEntries[i].inCodeName);

        for (j=1; j < pEnum->pEntries[i].iNumExtraNames; j++)
        {
            fprintf(pFile, "\t{ \"%s\", %s},\n", 
                pEnum->pEntries[i].extraNames[j], pEnum->pEntries[i].inCodeName);
        }


    }
    fprintf(pFile, "\tDEFINE_END\n};\n\n");

    if (pEnum->enumToAppendTo[0])
    {
        char fixupFuncName[256];
        sprintf(fixupFuncName, "autoEnum_fixup_%s", pEnum->enumName);
        fprintf(pFile, "extern StaticDefineInt %sEnum[];\n", pEnum->enumToAppendTo);
        fprintf(pFile, "void %s(void)\n{\n", fixupFuncName);
        fprintf(pFile, "\tStaticDefineIntAddTailList(%sEnum, %sEnum);\n}\n\n", pEnum->enumToAppendTo, pEnum->enumName);

        m_pParent->GetAutoRunManager()->AddAutoRun(fixupFuncName, pEnum->sourceFileName);
    }



}


void StructParser::DumpEnumPrototype(FILE *pFile, ENUM_DEF *pEnum)
{


    fprintf(pFile, "\n\n//auto-generated staticdefine for enum %s\n//autogenerated" "nocheckin\n", pEnum->enumName);
    fprintf(pFile, "extern StaticDefineInt %sEnum[];\n\n", pEnum->enumName);


}

//returns number of dependencies found
int StructParser::ProcessDataSingleFile(char const* pSourceFileName, char* pDependencies[MAX_DEPENDENCIES_SINGLE_FILE])
{
    int i;

    for (i=0; i < m_iNumStructs; i++)
    {
        if (AreFilenamesEqual(m_pStructs[i]->sourceFileName, pSourceFileName))
        {
            FixupFieldTypes(m_pStructs[i]);
            CheckOverallStructValidity_PostFixup(m_pStructs[i]);
        }
    }

    int iNumDependencies = 0;

    for (i=0; i < m_iNumStructs; i++)
    {
        if (AreFilenamesEqual(m_pStructs[i]->sourceFileName, pSourceFileName))
        {
            
            FindDependenciesInStruct(pSourceFileName, m_pStructs[i], &iNumDependencies, pDependencies);
            
        }
    }

    return iNumDependencies;
}

void StructParser::FindDependenciesInStruct(char const* pSourceFileName, STRUCT_DEF *pStruct, int *piNumDependencies, char *pDependencies[MAX_DEPENDENCIES_SINGLE_FILE])
{
    int i;
    

    if (pStruct->structNameIInheritFrom[0])
    {
        FieldAssert(pStruct->pStructFields[0], pStruct->pStructFields[0]->pStructSourceFileName != NULL,
            "Unrecognized struct name for POLYCHILDTYPE");
        if (!AreFilenamesEqual(pStruct->pStructFields[0]->pStructSourceFileName, pSourceFileName))
        {
            FieldAssert(pStruct->pStructFields[0], *piNumDependencies < MAX_DEPENDENCIES_SINGLE_FILE, "Too many dependencies");
            pDependencies[*piNumDependencies] = pStruct->pStructFields[0]->pStructSourceFileName;
            (*piNumDependencies)++;
        }
    }

    for (i=0; i < pStruct->iNumFields; i++)
    {
        STRUCT_FIELD_DESC *pField = pStruct->pStructFields[i];

        if (pField->eDataType == DATATYPE_INT && pField->subTableName[0] && (StructHasWikiComments(pStruct) || pStruct->pMainWikiComment))
        {
            char *pEnumFile;

            if (m_pParent->GetDictionary()->FindIdentifierAndGetSourceFilePointer(pField->typeName, &pEnumFile) == IDENTIFIER_ENUM)
            {
                if (!AreFilenamesEqual(pSourceFileName, pEnumFile))
                {
                    FieldAssert(pField, *piNumDependencies < MAX_DEPENDENCIES_SINGLE_FILE, "Too many dependencies");
                    pDependencies[*piNumDependencies] = pEnumFile;
                    (*piNumDependencies)++;
                }
            }
        } 
        else if (pField->eDataType == DATATYPE_STRUCT && pField->bFlatEmbedded
            || pField->eDataType == DATATYPE_STRUCT && (StructHasWikiComments(pStruct) || pStruct->pMainWikiComment)

//all of a container's structs cause dependencies in case they switch to/from being a container                
            || pField->eDataType == DATATYPE_STRUCT && pStruct->bIsContainer)
        {

            //this is so that we can always tell whether the structs in a container are themselves containers
            if (pField->eDataType == DATATYPE_STRUCT && pStruct->bIsContainer)
            {
                FieldAssert(pField, pField->pStructSourceFileName != NULL
                    || pField->bFoundForceContainer
                    || pField->bFoundForceNotContainer, "Containers may only contain structs which are defined in this project, or have FORCE_CONTAINER or FORCE_NOT_CONTAINER");
            }

            if (pField->pStructSourceFileName)
            {
                if (!AreFilenamesEqual(pField->pStructSourceFileName, pSourceFileName))
                {
                    FieldAssert(pField, *piNumDependencies < MAX_DEPENDENCIES_SINGLE_FILE, "Too many dependencies");
                    pDependencies[*piNumDependencies] = pField->pStructSourceFileName;
                    (*piNumDependencies)++;
                }
            }
        }
        else if (pField->eDataType == DATATYPE_STRUCT_POLY)
        {
            if (pField->pStructSourceFileName)
            {
                if (!AreFilenamesEqual(pField->pStructSourceFileName, pSourceFileName))
                {
                    FieldAssert(pField, *piNumDependencies < MAX_DEPENDENCIES_SINGLE_FILE, "Too many dependencies");
                    pDependencies[*piNumDependencies] = pField->pStructSourceFileName;
                    (*piNumDependencies)++;
                }
            }
        }
    }
}

void StructParser::ResetMacros(void)
{
    int i;

    for (i=0; i < m_iNumMacros; i++)
    {
        delete m_Macros[i].pIn;
        delete m_Macros[i].pOut;
    }

    m_iNumMacros = 0;

    if (m_pPrefix)
    {
        delete m_pPrefix;
        m_pPrefix = NULL;
    }

    if (m_pSuffix)
    {
        delete m_pSuffix;
        m_pSuffix = NULL;
    }
}



void StructParser::ReplaceMacrosInString(char *pString, int iOutStringLength)
{
    int i;
    int iLen = (int)strlen(pString);

    for (i=0; i < m_iNumMacros; i++)
    {
        iLen += ReplaceMacroInString(pString, &m_Macros[i], iLen, iOutStringLength);
    }
}

int StructParser::ReplaceMacroInString(char *pString, AST_MACRO_STRUCT *pMacro, int iCurLength, int iMaxLength)
{
    int iRetVal = 0;
    int i;

    if (iCurLength < pMacro->iInLength)
    {
        return 0;
    }
    
    for (i=0; i <= iCurLength - pMacro->iInLength; i++)
    {
        if (strncmp(pMacro->pIn, pString + i, pMacro->iInLength) == 0)
        {
            if ((i == 0 || !IsOKForIdent(pString[i-1])) && !IsOKForIdent(pString[i + pMacro->iInLength]))
            {
                memmove(pString + i + pMacro->iOutLength, pString + i + pMacro->iInLength, iCurLength - (i + pMacro->iInLength) + 1);
                memcpy(pString + i, pMacro->pOut, pMacro->iOutLength);

                iCurLength += pMacro->iOutLength - pMacro->iInLength;
                iRetVal += pMacro->iOutLength - pMacro->iInLength;
                i += pMacro->iOutLength - 1;
            }
        }
    }

    return iRetVal;
}

void StructParser::AttemptToDeduceReferenceDictionaryName(STRUCT_FIELD_DESC *pField, char *pTypeName)
{
    strcpy(pField->refDictionaryName, pTypeName);
}

int StructParser::GetNumLinesFieldTakesUp(STRUCT_FIELD_DESC *pField)
{
    if (pField->eDataType == DATATYPE_MAT4)
    {
        return 2;
    }

    return 1;
}

char const* StructParser::GetMultiLineFieldNamePrefix(STRUCT_FIELD_DESC *pField, int iIndex)
{
    Tokenizer::StaticAssert(pField->eDataType == DATATYPE_MAT4, "Unknown multiline field");

    switch (iIndex)
    {
    case 0:
        return "rot";
    case 1:
        return "pos";
    }

    return "THIS STRING SHOULD NEVER BE SEEN! PANIC! RUN FOR THE HILLS!";
}


StructParser::ENUM_DEF *StructParser::FindEnumByName(char *pName)
{
    int i;

    for (i=0; i < m_iNumEnums; i++)
    {
        if (strcmp(pName, m_pEnums[i]->enumName) == 0)
        {
            return m_pEnums[i];
        }
    }

    return NULL;
}



void StructParser::DumpEnumToWikiFile(FILE *pOutFile, ENUM_DEF *pEnum)
{
    int iPrefixLength = 0;
    int i, j;

    fprintf(pOutFile, "{anchor:%s}\n\n", pEnum->enumName);

    fprintf(pOutFile, "_auto-generated from %s_\n\n", pEnum->sourceFileName);

    fprintf(pOutFile, "h3. Enumerated type *%s* - %s\n", pEnum->enumName, pEnum->pMainWikiComment);

    if (pEnum->iNumEntries > 1 && !pEnum->bNoPrefixStripping)
    {
        char prefixString[MAX_NAME_LENGTH];
        strcpy(prefixString, pEnum->pEntries[0].inCodeName);

        iPrefixLength = (int)strlen(prefixString);


        for (i=1; i < pEnum->iNumEntries; i++)
        {
            for (j=0; j < iPrefixLength; j++)
            {
                if (prefixString[j] != pEnum->pEntries[i].inCodeName[j])
                {
                    iPrefixLength = j;
                    break;
                }
            }
        }
    }

    for (i=0; i < pEnum->iNumEntries; i++)
    {
        fprintf(pOutFile, "*%s*\n",
            pEnum->pEntries[i].iNumExtraNames ? pEnum->pEntries[i].extraNames[0] : pEnum->pEntries[i].inCodeName + iPrefixLength);
        if (pEnum->pEntries[i].pWikiComment)
        {
            fprintf(pOutFile, "-\t%s\n", pEnum->pEntries[i].pWikiComment);
        }
    }

}

void StructParser::DumpEnumInWikiForField(ENUM_DEF *pEnum, char *pOutString)
{
    int iPrefixLength = 0;
    int i, j;

    if (pEnum->iNumEntries > 1 && !pEnum->bNoPrefixStripping)
    {
        char prefixString[MAX_NAME_LENGTH];
        strcpy(prefixString, pEnum->pEntries[0].inCodeName);

        iPrefixLength = (int)strlen(prefixString);


        for (i=1; i < pEnum->iNumEntries; i++)
        {
            for (j=0; j < iPrefixLength; j++)
            {
                if (prefixString[j] != pEnum->pEntries[i].inCodeName[j])
                {
                    iPrefixLength = j;
                    break;
                }
            }
        }
    }

    sprintf(pOutString + strlen(pOutString), " < ");

    for (i=0; i < pEnum->iNumEntries; i++)
    {
        sprintf(pOutString + strlen(pOutString), "%s%s",
            i > 0 ? " | " : "",
            pEnum->pEntries[i].iNumExtraNames ? pEnum->pEntries[i].extraNames[0] : pEnum->pEntries[i].inCodeName + iPrefixLength);
    }

    sprintf(pOutString + strlen(pOutString), " > [#%s]\n", pEnum->enumName);


}


void StructParser::AssertNameIsLegalForFormatStringString(Tokenizer *pTokenizer, char const*pName)
{
    if (!m_pParent->DoesVariableHaveValue("FormatString_Strings", pName, false))
    {
        pTokenizer->Assertf(0, "Found illegal or misused FormatString name %s... legal names are defined in the FormatString_Strings variable in StructParserVars.txt, found in c:\\src\\core, c:\\src\\fightclub, etc.", pName);
    }
}
void StructParser::AssertNameIsLegalForFormatStringInt(Tokenizer *pTokenizer, char const* pName)
{
    if (!m_pParent->DoesVariableHaveValue("FormatString_Ints", pName, false))
    {
        pTokenizer->Assertf(0, "Found illegal or misused FormatString name %s... legal names are defined in the FormatString_Strings variable in StructParserVars.txt, found in c:\\src\\core, c:\\src\\fightclub, etc.", pName);
    }}

void StructParser::AddStringToFormatString(STRUCT_FIELD_DESC *pField, char const* pStringToAdd)
{
    if (!pField->pFormatString)
    {
        pField->pFormatString = STRDUP(pStringToAdd);
        return;
    }

    int iCurLen = (int)strlen(pField->pFormatString);
    int iAddLen = (int)strlen(pStringToAdd);
    char *pNewString = new char[iCurLen + iAddLen + 4];

    sprintf(pNewString, "%s , %s", pField->pFormatString, pStringToAdd);

    delete(pField->pFormatString);
    pField->pFormatString = pNewString;
}
