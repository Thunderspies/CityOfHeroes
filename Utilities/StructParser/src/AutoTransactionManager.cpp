#include "pch.h"
#include "AutoTransactionManager.h"
#include "strutils.h"
#include "SourceParser.h"
#include "AutoRunManager.h"

enum
{
    MAGICWORD_AUTOTRANSACTION,
    MAGICWORD_AUTOTRANSHELPER,
};

//"safe simple" functions are ones that we trust we can pass container args to
static char const* sAutoTransSafeSimpleFunctionNames[] = 
{
    "eaSize",
    NULL
};

AutoTransactionManager::AutoTransactionManager()
{
    m_bSomethingChanged = false;
    m_iNumFuncs = 0;
    m_iNumHelperFuncs = 0;
    m_ShortAutoTransactionFileName[0] = 0;
    m_AutoTransactionFileName[0] = 0;
    m_AutoTransactionWrapperHeaderFileName[0] = 0;
    m_AutoTransactionWrapperSourceFileName[0] = 0;
    memset(m_HelperFuncs, 0, sizeof(m_HelperFuncs));
    memset(m_Funcs, 0, sizeof(m_Funcs));
}

char const* AutoTransactionManager::GetMagicWord(int iWhichMagicWord)
{    
    switch (iWhichMagicWord)
    {
    case MAGICWORD_AUTOTRANSACTION:
        return "AUTO_TRANSACTION";
    case MAGICWORD_AUTOTRANSHELPER:
        return "AUTO_TRANS_HELPER";
    default:
        return "x x";
    }
}


AutoTransactionManager::~AutoTransactionManager()
{
    int i;

    for (i=0; i < m_iNumFuncs; i++)
    {
        FreeAutoTransactionFunc(&m_Funcs[i]);
    }

    for (i=0; i < m_iNumHelperFuncs; i++)
    {
        FreeHelperFunc(&m_HelperFuncs[i]);
    }
}

void AutoTransactionManager::FreeHelperFunc(HelperFunc *pHelperFunc)
{
    int i;
    
    for (i=0; i < pHelperFunc->iNumMagicArgs; i++)
    {
        FreeMagicArg(pHelperFunc->pMagicArgs[i]);
    }
}

void AutoTransactionManager::FreeMagicArg(HelperFuncArg *pArg)
{
    int i;

    for (i=0; i < pArg->iNumFieldReferences; i++)
    {
        delete(pArg->pFieldReferences[i]);
    }

    for (i=0; i < pArg->iNumPotentialHelperRecursions; i++)
    {
        delete(pArg->pPotentialRecursions[i]->pFuncName);
        delete(pArg->pPotentialRecursions[i]->pFieldString);
        delete(pArg->pPotentialRecursions[i]);
    }

    delete(pArg);
}

bool AutoTransactionManager::DoesFileNeedUpdating(char const* pFileName)
{
    return false;
}
void AutoTransactionManager::FreeAutoTransactionFunc(AutoTransactionFunc *pFunc)
{
    int i;

    for (i=0; i < pFunc->iNumArgs; i++)
    {
        FreeArgLockingAndRecursingStuff(&pFunc->args[i]);
    }
}

void AutoTransactionManager::FreeArgLockingAndRecursingStuff(ArgStruct *pArg)
{
    FieldToLock *pNextField;

    while (pArg->pFirstFieldToLock)
    {
        pNextField = pArg->pFirstFieldToLock->pNext;
        delete(pArg->pFirstFieldToLock);
        pArg->pFirstFieldToLock = pNextField;
    }

    RecursingFunction *pNextFunc;

    while (pArg->pFirstRecursingFunction)
    {
        pNextFunc = pArg->pFirstRecursingFunction->pNext;
        delete(pArg->pFirstRecursingFunction);
        pArg->pFirstRecursingFunction = pNextFunc;
    }
}



enum
{
    RW_PARSABLE = RW_COUNT,
};

static char const* sAutoTransactionReservedWords[] =
{
    "PARSABLE",
    NULL
};

void AutoTransactionManager::SetProjectPathAndName(char const* srcPath, char const* commonPath, char const* projectName)
{
    strcpy(m_ProjectName, projectName);

    sprintf(m_ShortAutoTransactionFileName, "%s_autotransactions_autogen", projectName);
    sprintf(m_AutoTransactionFileName, "%s\\AutoGen\\%s.c", srcPath, m_ShortAutoTransactionFileName);
    sprintf(m_AutoTransactionWrapperHeaderFileName, "%s\\AutoGen\\%s_wrappers.h", commonPath, m_ShortAutoTransactionFileName);
    sprintf(m_AutoTransactionWrapperSourceFileName, "%s\\AutoGen\\%s_wrappers.c", commonPath, m_ShortAutoTransactionFileName);


}

void AutoTransactionManager::AddFieldToLockToArg(ArgStruct *pArg, char const* pFieldName, 
    enumAutoTransFieldToLockType eFieldToLockType, int iIndexNum, char const* pIndexString)
{
    FieldToLock *pField = pArg->pFirstFieldToLock;

    while (pField)
    {
        if (strcmp(pField->fieldName, pFieldName) == 0)
        {
            if (pField->eFieldType == eFieldToLockType)
            {
                if (eFieldToLockType == ATR_FTL_NORMAL
                    || eFieldToLockType == ATR_FTL_INDEXED_LITERAL_STRING && strcmp(pField->indexString, pIndexString) == 0
                    || pField->iIndexNum == iIndexNum)
                {
                    return;
                }
            }
        }

        pField = pField->pNext;
    }

    pField = new FieldToLock;

    memset(pField, 0, sizeof(FieldToLock));

    strcpy(pField->fieldName, pFieldName);
    pField->eFieldType = eFieldToLockType;
    if (pField->eFieldType == ATR_FTL_INDEXED_LITERAL_STRING)
    {
        strcpy(pField->indexString, pIndexString);
    }
    else
    {
        pField->iIndexNum = iIndexNum;
    }

    
    pField->pNext = pArg->pFirstFieldToLock;
    pArg->pFirstFieldToLock = pField;
    
}

bool AutoTransactionManager::IsFullContainerFunction(char const* pFuncName, int iArgNum, char const* pArgTypeName)
{
    if ((strcmp(pFuncName, "StructCopyFields") == 0 || strcmp(pFuncName, "StructCopyAll") == 0) &&
        (iArgNum == 1 || iArgNum == 2))
    {
        return true;
    }

    if (strcmp(pFuncName, "StructReset") == 0 && iArgNum == 1)
    {
        return true;
    }

    return false;
}

bool AutoTransactionManager::IsFullContainerFunction_NoFields(char const* pFuncName, int iArgNum, char const* pArgTypeName)
{
    if (strcmp(pFuncName, "ISNULL") == 0 &&
        (iArgNum == 0 ))
    {
        return true;
    }

    return false;
}


void AutoTransactionManager::AddRecurseFunctionToArg(ArgStruct *pArg, int iArgNum, char const* pFuncName, bool bIsATRRecurse, char const* pFieldString)
{
    
    
    RecursingFunction *pFunc = pArg->pFirstRecursingFunction;

    while (pFunc)
    {
        if (strcmp(pFunc->functionName, pFuncName) == 0 
            && pFunc->iArgNum == iArgNum
            && pFunc->bIsRecursingATRFunction == bIsATRRecurse
            && strcmp(pFunc->fieldString, pFieldString) == 0)
        {
            return;
        }

        pFunc = pFunc->pNext;
    }

    pFunc = new RecursingFunction;

    strcpy(pFunc->functionName, pFuncName);
    pFunc->iArgNum = iArgNum;
    pFunc->bIsRecursingATRFunction = bIsATRRecurse;
    strcpy(pFunc->fieldString, pFieldString ? pFieldString : "");

    pFunc->pNext = pArg->pFirstRecursingFunction;
    pArg->pFirstRecursingFunction = pFunc;
    
}

bool AutoTransactionManager::LoadStoredData(bool bForceReset)
{
    int i;

    if (bForceReset)
    {
        m_bSomethingChanged = true;
        return false;
    }

    Tokenizer tokenizer;

    if (!tokenizer.LoadFromFile(m_AutoTransactionFileName))
    {
        m_bSomethingChanged = true;
        return false;
    }

    if (!tokenizer.IsStringAtVeryEndOfBuffer("#endif"))
    {
        m_bSomethingChanged = true;
        return false;
    }

    tokenizer.SetExtraReservedWords(sAutoTransactionReservedWords);

    Token token;
    enumTokenType eType;

    do
    {
        eType = tokenizer.GetNextToken(&token);
        Tokenizer::StaticAssert(eType != TOKEN_NONE, "AUTOTRANS file corruption");
    } while (!(eType == TOKEN_RESERVEDWORD && token.iVal == RW_PARSABLE));

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find number of auto transactions");

    m_iNumFuncs = token.iVal;

    int iFuncNum;

    for (iFuncNum = 0; iFuncNum < m_iNumFuncs; iFuncNum++)
    {
        AutoTransactionFunc *pFunc = &m_Funcs[iFuncNum];

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_NAME_LENGTH, "Didn't find auto transaction function name");
        strcpy(pFunc->functionName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_PATH, "Didn't find auto transaction source file");
        strcpy(pFunc->sourceFileName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find auto transaction line num");
        pFunc->iSourceFileLineNum = token.iVal;

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find number of arguments");
        
        pFunc->iNumArgs = token.iVal;

        int iArgNum;

        for (iArgNum=0; iArgNum < pFunc->iNumArgs; iArgNum++)
        {
            ArgStruct *pArg = &pFunc->args[iArgNum];

            pArg->pFirstFieldToLock = NULL;
            pArg->pFirstRecursingFunction = NULL;


            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find arg type");
            pArg->eArgType = (enumAutoTransArgType)token.iVal;

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find isPointer");
            pArg->bIsPointer = (token.iVal == 1);

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find isEArray");
            pArg->bIsEArray = (token.iVal == 1);

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find FoundNonContainer");
            pArg->bFoundNonContainer = (token.iVal == 1);

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_NAME_LENGTH, "Didn't find arg type name");
            strcpy(pArg->argTypeName, token.sVal);

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_NAME_LENGTH, "Didn't find arg name");
            strcpy(pArg->argName, token.sVal);
        
            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find num fields");
            int iNumFieldsToLock = token.iVal;
                
            for (i=0; i < iNumFieldsToLock; i++)
            {
                char fieldName[MAX_NAME_LENGTH] = "";
                char indexString[MAX_NAME_LENGTH] = "";
                enumAutoTransFieldToLockType eFTLType;
                int iIndexNum = 0;

                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_NAME_LENGTH, "Didn't find field to lock name");
                strcpy(fieldName, token.sVal);
                
                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find FTL type");
                eFTLType = (enumAutoTransFieldToLockType)token.iVal;

                switch (eFTLType)
                {
                case ATR_FTL_INDEXED_LITERAL_STRING:
                    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_NAME_LENGTH, "Didn't find field to lock index name");
                    strcpy(indexString, token.sVal);
                    break;

                case ATR_FTL_NORMAL:
                    break;

                default:
                    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find field to lock int");
                    iIndexNum = token.iVal;
                }

                AddFieldToLockToArg(pArg, fieldName, eFTLType, iIndexNum, indexString);
            }

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find num recurse functions");
            int iNumRecurseFunctions = token.iVal;
                
            for (i=0; i < iNumRecurseFunctions; i++)
            {
                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find argNum");
                int iArgNum = token.iVal;
                
                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_NAME_LENGTH, "Didn't find recursing func name");
                char funcName[MAX_NAME_LENGTH];
                strcpy(funcName, token.sVal);
    
                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find bIsATRRecurse");
                bool bIsATRRecurse = token.iVal != 0;

                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_NAME_LENGTH, "Didn't find recursing field string");
                char fieldString[MAX_NAME_LENGTH];
                strcpy(fieldString, token.sVal);
    


                AddRecurseFunctionToArg(pArg, iArgNum, funcName, bIsATRRecurse, fieldString);
            }
        }
    }

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find number of helper funcs");

    m_iNumHelperFuncs = token.iVal;
    tokenizer.Assert(m_iNumHelperFuncs <= MAX_AUTO_TRANSACTION_HELPER_FUNCS, "Too many helper functions");
    int iHelperFuncNum;

    for (iHelperFuncNum = 0; iHelperFuncNum < m_iNumHelperFuncs; iHelperFuncNum++)
    {
        HelperFunc *pHelperFunc = &m_HelperFuncs[iHelperFuncNum];

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_PATH, "Didn't find source file");
        strcpy(pHelperFunc->sourceFileName, token.sVal);


        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_NAME_LENGTH, "Didn't find helper func name");
        strcpy(pHelperFunc->functionName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find num magic args");
        pHelperFunc->iNumMagicArgs = token.iVal;
        tokenizer.Assert(pHelperFunc->iNumMagicArgs <= MAX_MAGIC_ARGS_PER_HELPER_FUNC, "Too many magic args");

        int iMagicArgNum;

        for (iMagicArgNum = 0; iMagicArgNum < pHelperFunc->iNumMagicArgs; iMagicArgNum++)
        {
            HelperFuncArg *pMagicArg = pHelperFunc->pMagicArgs[iMagicArgNum] = new HelperFuncArg;

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find arg index");
            pMagicArg->iArgIndex = token.iVal;

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find number of field references");
            pMagicArg->iNumFieldReferences = token.iVal;

            int iFieldRefNum;

            for (iFieldRefNum = 0; iFieldRefNum < pMagicArg->iNumFieldReferences; iFieldRefNum++)
            {
                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Didn't find field reference in magic arg");
                pMagicArg->pFieldReferences[iFieldRefNum] = STRDUP(token.sVal);
            }

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find number of potential helper recursions");
            pMagicArg->iNumPotentialHelperRecursions = token.iVal;

            int iPotentialRecursionNum;

            for (iPotentialRecursionNum = 0; iPotentialRecursionNum < pMagicArg->iNumPotentialHelperRecursions; iPotentialRecursionNum++)
            {
                PotentialHelperFuncRecursion *pPotentialRecursion = pMagicArg->pPotentialRecursions[iPotentialRecursionNum] = new PotentialHelperFuncRecursion;

                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Didn't find func name for potential recursion");
                pPotentialRecursion->pFuncName = STRDUP(token.sVal);

                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find potential recursion arg num");
                pPotentialRecursion->iArgNum = token.iVal;

                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Didn't find field string for potential recursion");
                pPotentialRecursion->pFieldString = STRDUP(token.sVal);
            }
        }
    }


    m_bSomethingChanged = false;

    return true;
}

void AutoTransactionManager::ResetSourceFile(char const* pSourceFileName)
{
    int i = 0;

    while (i < m_iNumFuncs)
    {
        if (AreFilenamesEqual(m_Funcs[i].sourceFileName, pSourceFileName))
        {
            FreeAutoTransactionFunc(&m_Funcs[i]);
            memcpy(&m_Funcs[i], &m_Funcs[m_iNumFuncs - 1], sizeof(AutoTransactionFunc));
            m_iNumFuncs--;

            m_bSomethingChanged = true;
        }
        else
        {
            i++;
        }
    }

    i = 0;

    while (i < m_iNumHelperFuncs)
    {
        if (AreFilenamesEqual(m_HelperFuncs[i].sourceFileName, pSourceFileName))
        {
            FreeHelperFunc(&m_HelperFuncs[i]);
            memcpy(&m_HelperFuncs[i], &m_HelperFuncs[m_iNumHelperFuncs - 1], sizeof(HelperFunc));
            m_iNumHelperFuncs--;

            m_bSomethingChanged = true;
        }
        else
        {
            i++;
        }
    }
}

void AutoTransactionManager::FuncAssert(AutoTransactionFunc *pFunc, bool bExpression, char const* pErrorString)
{
    if (!bExpression)
    {
        printf("%s(%d) : error S0000 : (StructParser) %s\n", pFunc->sourceFileName, pFunc->iSourceFileLineNum, pErrorString);
        fflush(stdout);
        Sleep(100);
        exit(1);
    }
}

    







char const* AutoTransactionManager::GetTargetCodeArgTypeMacro(AutoTransactionFunc *pFunc, ArgStruct *pArg)
{
    switch (pArg->eArgType)
    {
    case ATR_ARGTYPE_INT:
        if (pArg->bIsPointer)
        {
            return "ATR_ARG_INT_PTR";
        }
        return "ATR_ARG_INT";

    case ATR_ARGTYPE_INT64:
        if (pArg->bIsPointer)
        {
            return "ATR_ARG_INT64_PTR";
        }
        return "ATR_ARG_INT64";

    case ATR_ARGTYPE_FLOAT:
        if (pArg->bIsPointer)
        {
            return "ATR_ARG_FLOAT_PTR";
        }
        return "ATR_ARG_FLOAT";

    case ATR_ARGTYPE_STRING:
        if (pArg->bIsPointer)
        {
            return "ATR_ARG_STRING";
        }
        FuncAssert(pFunc, 0, "Invalid function arg type");

    case ATR_ARGTYPE_CONTAINER:
        if (pArg->bIsEArray)
        {
            return "ATR_ARG_CONTAINER_EARRAY";
        }
        if (pArg->bIsPointer)
        {
            return "ATR_ARG_CONTAINER";
        }
        FuncAssert(pFunc, 0, "Invalid function arg type");

    case ATR_ARGTYPE_STRUCT:
        if (pArg->bIsPointer)
        {
            return "ATR_ARG_STRUCT";
        }
        FuncAssert(pFunc, 0, "Invalid function arg type");
    }

    return NULL;
}

char const* AutoTransactionManager::GetTargetCodeArgGettingFunctionName(ArgStruct *pArg)
{
    switch (pArg->eArgType)
    {
    case ATR_ARGTYPE_INT:
        if (pArg->bIsPointer)
        {
            return "GetATRArg_IntPtr";
        }
        return "GetATRArg_Int";

    case ATR_ARGTYPE_INT64:
        if (pArg->bIsPointer)
        {
            return "GetATRArg_Int64Ptr";
        }
        return "GetATRArg_Int64";

    case ATR_ARGTYPE_FLOAT:
        if (pArg->bIsPointer)
        {
            return "GetATRArg_FloatPtr";
        }
        return "GetATRArg_Float";

    case ATR_ARGTYPE_STRING:
        return "GetATRArg_String";
        

    case ATR_ARGTYPE_CONTAINER:
        if (pArg->bIsEArray)
        {
            return "GetATRArg_ContainerEArray";
        }
        else
        {
            return "GetATRArg_Container";
        }
        break;

    case ATR_ARGTYPE_STRUCT:
        return "GetATRArg_Struct";
    }    

    return NULL;
}

void AutoTransactionManager::WriteTimeVerifyAutoTransValidity(AutoTransactionFunc *pFunc)
{
    int i;

    for (i=0; i < pFunc->iNumArgs; i++)
    {
        if (pFunc->args[i].bFoundNonContainer)
        {
            FuncAssert(pFunc, pFunc->args[i].eArgType == ATR_ARGTYPE_STRUCT, "Found NON_CONTAINER in bad place");
        }

        if (pFunc->args[i].eArgType == ATR_ARGTYPE_CONTAINER)
        {
            FuncAssert(pFunc, m_pParent->GetDictionary()->FindIdentifier(pFunc->args[i].argTypeName) == IDENTIFIER_STRUCT_CONTAINER,
                "Invalid or unrecognized container name... containers to AUTO_TRANSACTION must be AUTO_STRUCT AST_CONTAINER");
        }
        else if (pFunc->args[i].eArgType == ATR_ARGTYPE_STRUCT)
        {
            FuncAssert(pFunc, m_pParent->GetDictionary()->FindIdentifier(pFunc->args[i].argTypeName) == IDENTIFIER_STRUCT || 
                pFunc->args[i].bFoundNonContainer && m_pParent->GetDictionary()->FindIdentifier(pFunc->args[i].argTypeName) == IDENTIFIER_STRUCT_CONTAINER,
                "Invalid or unrecognized struct name...");
        }

        if (pFunc->args[i].bIsEArray)
        {
            FuncAssert(pFunc, pFunc->args[i].eArgType == ATR_ARGTYPE_CONTAINER, "Only containers can be in EArrays in AUTOTRANS func defs");
        }
    }
}


void AutoTransactionManager::WriteOutAutoGenAutoTransactionPrototype(FILE *pFile, AutoTransactionFunc *pFunc)
{
    int iArgNum;
    //first, write out prototypes
    for (iArgNum = 0; iArgNum < pFunc->iNumArgs; iArgNum++)
    {
        ArgStruct *pArg = &pFunc->args[iArgNum];
        
        switch (pArg->eArgType)
        {
        case ATR_ARGTYPE_STRUCT:
            fprintf(pFile, "typedef struct %s %s;\nextern ParseTable parse_%s[];\n",
                pArg->argTypeName, pArg->argTypeName, pArg->argTypeName);
        }
    }

    fprintf(pFile, "int AutoTrans_%s(TransactionReturnValStruct *pReturnVal, GlobalType eServerTypeToRunOn", pFunc->functionName);

    for (iArgNum = 0; iArgNum < pFunc->iNumArgs; iArgNum++)
    {
        ArgStruct *pArg = &pFunc->args[iArgNum];
        
        switch (pArg->eArgType)
        {
        case ATR_ARGTYPE_INT:
            fprintf(pFile, ", int %s", pArg->argName);
            break;

        case ATR_ARGTYPE_INT64:
            fprintf(pFile, ", S64 %s", pArg->argName);
            break;

        case ATR_ARGTYPE_FLOAT:
            fprintf(pFile, ", float %s", pArg->argName);
            break;

        case ATR_ARGTYPE_STRING:
            fprintf(pFile, ", const char *%s", pArg->argName);
            break;

        case ATR_ARGTYPE_CONTAINER:
            if (pArg->bIsEArray)
            {
                fprintf(pFile, ", GlobalType %s_type, U32 **%s_IDs", pArg->argName, pArg->argName);
            }
            else
            {
                fprintf(pFile, ", GlobalType %s_type, U32 %s_ID", pArg->argName, pArg->argName);
            }
            break;

        case ATR_ARGTYPE_STRUCT:
            fprintf(pFile, ", const %s *%s", pArg->argTypeName, pArg->argName);
            break;
        }
    }

    fprintf(pFile, ")");

}

int AutoTransactionManager::AutoTransactionComparator(const void *p1, const void *p2)
{
    return strcmp(((AutoTransactionFunc*)p1)->functionName, ((AutoTransactionFunc*)p2)->functionName);
}


bool AutoTransactionManager::WriteOutData(void)
{
    int iFuncNum, iHelperFuncNum, i;

    if (!m_bSomethingChanged)
    {
        return false;
    }

    qsort(m_Funcs, m_iNumFuncs, sizeof(AutoTransactionFunc), AutoTransactionComparator);

    FILE *pOutFile = fopen_nofail(m_AutoTransactionFileName, "wt");

    fprintf(pOutFile, "//This file contains data and prototypes for auto transactions. It is autogenerated by StructParser\n\n\n//autogenerated" "nocheckin\n\n");

    m_pParent->GetAutoRunManager()->ResetSourceFile("autogen_autotransactions");

    if (m_iNumHelperFuncs)
    {
        fprintf(pOutFile, "#include \"autoTransHelperDefs.h\"\n\n");
    }

    if (m_iNumFuncs)
    {
        fprintf(pOutFile, "#include \"autotransdefs.h\"\n\n");
        fprintf(pOutFile, "#include \"objtransactions.h\"\n\n");
    }


    for (iHelperFuncNum = 0; iHelperFuncNum < m_iNumHelperFuncs; iHelperFuncNum++)
    {
        HelperFunc *pHelperFunc = &m_HelperFuncs[iHelperFuncNum];

        fprintf(pOutFile, "\n\n//------------------------stuff relating to AUTO_TRANS_HELPER %s----------------------\n\n",
            pHelperFunc->functionName);

        int iArgNum = 0;
        for (iArgNum = 0; iArgNum < pHelperFunc->iNumMagicArgs; iArgNum++)
        {
            HelperFuncArg *pMagicArg = pHelperFunc->pMagicArgs[iArgNum];

            if (pMagicArg->iNumFieldReferences)
            {
                fprintf(pOutFile, "char *ATR_Helper_StaticFieldsToLock_%s_%d[] = {\n", pHelperFunc->functionName, pMagicArg->iArgIndex);

                for (i=0; i < pMagicArg->iNumFieldReferences; i++)
                {
                    fprintf(pOutFile, "\t\"%s\",\n", pMagicArg->pFieldReferences[i]);
                }
                fprintf(pOutFile, "\tNULL\n};\n\n");
            }

            if (pMagicArg->iNumPotentialHelperRecursions)
            {
                fprintf(pOutFile, "ATRPotentialHelperFuncCall ATR_Helper_PotentialHelperFuncCalls_%s_%d[] = {\n",
                    pHelperFunc->functionName, pMagicArg->iArgIndex);

                for (i=0; i < pMagicArg->iNumPotentialHelperRecursions; i++)
                {
                    fprintf(pOutFile, "\t{ \"%s\", %d, \"%s\"},\n",
                        pMagicArg->pPotentialRecursions[i]->pFuncName,
                        pMagicArg->pPotentialRecursions[i]->iArgNum,
                        pMagicArg->pPotentialRecursions[i]->pFieldString);
                }

                fprintf(pOutFile, "\t{ NULL, 0, NULL}\n};\n\n");
            }
        }

        fprintf(pOutFile, "ATRHelperArg ATR_Helper_Args_%s[] = {\n", pHelperFunc->functionName);

        for (iArgNum = 0; iArgNum < pHelperFunc->iNumMagicArgs; iArgNum++)
        {
            HelperFuncArg *pMagicArg = pHelperFunc->pMagicArgs[iArgNum];
            
            fprintf(pOutFile, "\t{%d, ", pMagicArg->iArgIndex);

            if (pMagicArg->iNumFieldReferences)
            {
                fprintf(pOutFile, "ATR_Helper_StaticFieldsToLock_%s_%d, NULL, ", pHelperFunc->functionName, pMagicArg->iArgIndex);
            }
            else
            {
                fprintf(pOutFile, "NULL, NULL, ");
            }    

            if (pMagicArg->iNumPotentialHelperRecursions)
            {
                fprintf(pOutFile, "ATR_Helper_PotentialHelperFuncCalls_%s_%d },\n", 
                    pHelperFunc->functionName, pMagicArg->iArgIndex);
            }
            else
            {
                fprintf(pOutFile, "NULL },\n");
            }
        }

        fprintf(pOutFile, "\t{ -1, NULL, NULL, NULL}\n};\n\n");


        fprintf(pOutFile, "ATRHelperFunc ATR_Helper_Func_%s =\n{\n\t\"%s\",\n\tATR_Helper_Args_%s\n};\n\n", 
            pHelperFunc->functionName, pHelperFunc->functionName, pHelperFunc->functionName);
    }







    if (m_iNumFuncs)
    {
        for (iFuncNum = 0; iFuncNum < m_iNumFuncs; iFuncNum++)
        {
            AutoTransactionFunc *pFunc = &m_Funcs[iFuncNum];

            fprintf(pOutFile, "\n\n//----------------Stuff relating to AUTOTRANS %s -----------------------\n\n", pFunc->functionName); 

            WriteTimeVerifyAutoTransValidity(pFunc);
        
            int iArgNum;
            for (iArgNum = 0; iArgNum < pFunc->iNumArgs; iArgNum++)
            {
                ArgStruct *pArg = &pFunc->args[iArgNum];

                if (pArg->pFirstFieldToLock)
                {
                    fprintf(pOutFile, "static ATRFieldToLock ATR_%s_%s_FieldsUsed[] =\n{\n",
                        pFunc->functionName, pArg->argName);

                    FieldToLock *pField = pArg->pFirstFieldToLock;

                    while (pField)
                    {
                        fprintf(pOutFile, "\t{\n\t\t\"%s\",\n", pField->fieldName);
                        switch(pField->eFieldType)
                        {
                        case ATR_FTL_NORMAL:
                            fprintf(pOutFile, "\t\tATR_FIELD_NORMAL,\n\t\t0,\n\t\tNULL,\n\t},\n");
                            break;
                        case ATR_FTL_INDEXED_LITERAL_INT:
                            fprintf(pOutFile, "\t\tATR_FIELD_INDEXED_LITERAL_INT,\n\t\t%d,\n\t\tNULL,\n\t},\n", pField->iIndexNum);
                            break;
                        case ATR_FTL_INDEXED_INT_ARG:
                            fprintf(pOutFile, "\t\tATF_FIELD_INDEXED_INT_ARG,\n\t\t%d,\n\t\tNULL,\t},\n\n", pField->iIndexNum);
                            break;
                        case ATR_FTL_INDEXED_LITERAL_STRING:
                            fprintf(pOutFile, "\t\tATR_FIELD_INDEXED_LITERAL_STRING,\n\t\t0,\n\t\t\"%s\",\n\t},\n", pField->indexString);
                            break;
                        case ATR_FTL_INDEXED_STRING_ARG:
                            fprintf(pOutFile, "\t\tATF_FIELD_INDEXED_STRING_ARG,\n\t\t%d,\n\t\tNULL,\n\t},\n", pField->iIndexNum);
                            break;
                        }
        
                        pField = pField->pNext;
                    }
            
                    fprintf(pOutFile, "\t{ NULL }\n};\n\n");
                }
    
                if (pArg->pFirstRecursingFunction)
                {
                    fprintf(pOutFile, "static ATRRecursingFuncCall ATR_%s_%s_RecursingFuncs[] =\n{\n", 
                        pFunc->functionName, pArg->argName);

                    RecursingFunction *pRecursingFunc = pArg->pFirstRecursingFunction;

                    while (pRecursingFunc)
                    {
                        if (pRecursingFunc->bIsRecursingATRFunction)
                        {
                            fprintf(pOutFile, "\t{ \"%s\", %d },\n",
                                pRecursingFunc->functionName, pRecursingFunc->iArgNum);
                        }

                        pRecursingFunc = pRecursingFunc->pNext;
                    }

                    fprintf(pOutFile, "\t{ NULL, 0 }\n};\n\n");

                    fprintf(pOutFile, "static ATRPotentialHelperFuncCall ATR_%s_%s_PotentialHelperFuncCalls[] = \n{\n",
                        pFunc->functionName, pArg->argName);

                    pRecursingFunc = pArg->pFirstRecursingFunction;

                    while (pRecursingFunc)
                    {
                        if (!pRecursingFunc->bIsRecursingATRFunction)
                        {
                            fprintf(pOutFile, "\t{ \"%s\", %d, \"%s\" },\n",
                                pRecursingFunc->functionName, pRecursingFunc->iArgNum, pRecursingFunc->fieldString);
                        }

                        pRecursingFunc = pRecursingFunc->pNext;
                    }

                    fprintf(pOutFile, "\t{ NULL, 0 }\n};\n\n");

                }
            }



            for (iArgNum = 0; iArgNum < pFunc->iNumArgs; iArgNum++)
            {
                ArgStruct *pArg = &pFunc->args[iArgNum];

                if (pArg->eArgType == ATR_ARGTYPE_CONTAINER || pArg->eArgType == ATR_ARGTYPE_STRUCT)
                {
                    fprintf(pOutFile, "extern ParseTable parse_%s[];\n", pArg->argTypeName);
                }
            }


                

            fprintf(pOutFile, "\n\nstatic ATRArgDef ATR_%s_ArgDefs[] =\n{\n", pFunc->functionName);
        
            for (iArgNum = 0; iArgNum < pFunc->iNumArgs; iArgNum++)
            {
                ArgStruct *pArg = &pFunc->args[iArgNum];

                fprintf(pOutFile, "\t{ %s, ", GetTargetCodeArgTypeMacro(pFunc, pArg));

                if (pArg->pFirstFieldToLock)
                {
                    fprintf(pOutFile, "ATR_%s_%s_FieldsUsed, ", pFunc->functionName, pArg->argName);
                }
                else
                {
                    fprintf(pOutFile, "NULL, ");
                }

                fprintf(pOutFile, "NULL, "); //pRunTimeFieldsToLock

                if (pArg->pFirstRecursingFunction)
                {
                    fprintf(pOutFile, "ATR_%s_%s_RecursingFuncs, ATR_%s_%s_PotentialHelperFuncCalls, ", 
                        pFunc->functionName, pArg->argName, pFunc->functionName, pArg->argName);
                }
                else
                {
                    fprintf(pOutFile, "NULL, NULL, ");
                }

                if (pArg->eArgType == ATR_ARGTYPE_CONTAINER || pArg->eArgType == ATR_ARGTYPE_STRUCT)
                {
                    fprintf(pOutFile, "parse_%s, \"%s\" },\n", pArg->argTypeName, pArg->argName);
                }
                else
                {
                    fprintf(pOutFile, "NULL, \"%s\" },\n", pArg->argName);
                }
            }

            fprintf(pOutFile, "\t{ ATR_ARG_NONE, NULL }\n};\n\n");

            fprintf(pOutFile, "enumTransactionOutcome %s(ATR_ARGS, ", pFunc->functionName);

            for (iArgNum = 0; iArgNum < pFunc->iNumArgs; iArgNum++)
            {
                ArgStruct *pArg = &pFunc->args[iArgNum];

                fprintf(pOutFile, "%s%s %s%s%s", 
                    (pArg->eArgType == ATR_ARGTYPE_CONTAINER || pArg->eArgType == ATR_ARGTYPE_STRUCT) ? "struct " : "",
                    pArg->argTypeName, 
                    pArg->bIsEArray ? "**" : (pArg->bIsPointer ? "*" : ""),
                    pArg->argName,
                    iArgNum == pFunc->iNumArgs -1 ? "" : ", ");
            }
            fprintf(pOutFile, ");\n\n");

            fprintf(pOutFile, "static enumTransactionOutcome ATR_Wrapper_%s(char **ppResultEString)\n{\n", pFunc->functionName);
            for (iArgNum = 0; iArgNum < pFunc->iNumArgs; iArgNum++)
            {
                ArgStruct *pArg = &pFunc->args[iArgNum];

                fprintf(pOutFile, "\t%s%s %sarg%d;\n", 
                    (pArg->eArgType == ATR_ARGTYPE_CONTAINER || pArg->eArgType == ATR_ARGTYPE_STRUCT) ? "struct " : "",
                    pArg->argTypeName, pArg->bIsEArray ? "**" : (pArg->bIsPointer ? "*" : ""), iArgNum);
            }
        
            fprintf(pOutFile, "\n");

            for (iArgNum = 0; iArgNum < pFunc->iNumArgs; iArgNum++)
            {
                ArgStruct *pArg = &pFunc->args[iArgNum];

                fprintf(pOutFile, "\targ%d = %s(%d);\n", iArgNum, GetTargetCodeArgGettingFunctionName(pArg), iArgNum);
            }


            fprintf(pOutFile, "\n\treturn %s(ppResultEString, ", pFunc->functionName);

            for (iArgNum = 0; iArgNum < pFunc->iNumArgs; iArgNum++)
            {
                fprintf(pOutFile, "arg%d%s", iArgNum, iArgNum == pFunc->iNumArgs -1 ? "" : ", ");
            }

            fprintf(pOutFile, ");\n};\n\n\n");

            fprintf(pOutFile, "static ATR_FuncDef ATRFuncDef_%s = { \"%s\", ATR_Wrapper_%s,  ATR_%s_ArgDefs };\n",
                pFunc->functionName, pFunc->functionName, pFunc->functionName, pFunc->functionName);


        }
    }




    if (m_iNumFuncs || m_iNumHelperFuncs)
    {
        char autoRunName[256];
        sprintf(autoRunName, "ATR_%s_RegisterAllFuncs_AutoRun", m_ProjectName);

        fprintf(pOutFile, "void %s(void)\n{\n", autoRunName);

    
        for (iHelperFuncNum = 0; iHelperFuncNum < m_iNumHelperFuncs; iHelperFuncNum++)
        {
            HelperFunc *pHelperFunc = &m_HelperFuncs[iHelperFuncNum];

            fprintf(pOutFile, "\tRegisterATRHelperFunc(&ATR_Helper_Func_%s);\n", 
                pHelperFunc->functionName);
        }

        for (iFuncNum = 0; iFuncNum < m_iNumFuncs; iFuncNum++)
        {
            AutoTransactionFunc *pFunc = &m_Funcs[iFuncNum];

            fprintf(pOutFile, "\tRegisterATRFuncDef(&ATRFuncDef_%s);\n", 
                pFunc->functionName);
        }

    
        fprintf(pOutFile, "};\n\n");

    
        m_pParent->GetAutoRunManager()->AddAutoRun(autoRunName, "autogen_autotransactions");
    }





    fprintf(pOutFile, "\n\n\n#if 0\nPARSABLE\n");

    fprintf(pOutFile, "%d\n", m_iNumFuncs);

    for (iFuncNum = 0; iFuncNum < m_iNumFuncs; iFuncNum++)
    {
        AutoTransactionFunc *pFunc = &m_Funcs[iFuncNum];
        
        fprintf(pOutFile, "\"%s\"\n\"%s\"\n%d\n%d\n", 
            pFunc->functionName, pFunc->sourceFileName, pFunc->iSourceFileLineNum, pFunc->iNumArgs);

        int iArgNum;

        for (iArgNum=0; iArgNum < pFunc->iNumArgs; iArgNum++)
        {
            ArgStruct *pArg = &pFunc->args[iArgNum];

            fprintf(pOutFile, "%d %d %d %d \"%s\" \"%s\"\n",
                pArg->eArgType, pArg->bIsPointer, pArg->bIsEArray, pArg->bFoundNonContainer, pArg->argTypeName, pArg->argName);

            int iCount = 0;

            FieldToLock *pField = pArg->pFirstFieldToLock;

            while (pField)
            {
                iCount++;
                pField = pField->pNext;
            }

            fprintf(pOutFile, "%d\n", iCount);

            pField = pArg->pFirstFieldToLock;

            while (pField)
            {
                fprintf(pOutFile, "\"%s\" %d\n", pField->fieldName, pField->eFieldType);

                switch (pField->eFieldType)
                {
                case ATR_FTL_NORMAL:
                    break;

                case ATR_FTL_INDEXED_LITERAL_STRING:
                    fprintf(pOutFile, "\"%s\"\n", pField->indexString);
                    break;

                default:
                    fprintf(pOutFile, "%d\n", pField->iIndexNum);
                    break;
                }

                pField = pField->pNext;
            }


            iCount = 0;
            RecursingFunction *pRecursingFunc = pArg->pFirstRecursingFunction;

            while (pRecursingFunc)
            {
                iCount++;
                pRecursingFunc = pRecursingFunc->pNext;
            }

            fprintf(pOutFile, "%d\n", iCount);

            pRecursingFunc = pArg->pFirstRecursingFunction;

            while (pRecursingFunc)
            {
                fprintf(pOutFile, "%d \"%s\" %d \"%s\"\n", pRecursingFunc->iArgNum, pRecursingFunc->functionName,
                    pRecursingFunc->bIsRecursingATRFunction, pRecursingFunc->fieldString);
                pRecursingFunc = pRecursingFunc->pNext;
            }
        }
    }

    fprintf(pOutFile, "%d\n", m_iNumHelperFuncs);


    for (iHelperFuncNum = 0; iHelperFuncNum < m_iNumHelperFuncs; iHelperFuncNum++)
    {
        HelperFunc *pHelperFunc = &m_HelperFuncs[iHelperFuncNum];

        fprintf(pOutFile, "\"%s\" \"%s\" %d\n", pHelperFunc->sourceFileName, pHelperFunc->functionName, pHelperFunc->iNumMagicArgs);

        int iMagicArgNum;

        for (iMagicArgNum = 0; iMagicArgNum < pHelperFunc->iNumMagicArgs; iMagicArgNum++)
        {
            HelperFuncArg *pMagicArg = pHelperFunc->pMagicArgs[iMagicArgNum];

            fprintf(pOutFile, "%d %d\n", pMagicArg->iArgIndex, pMagicArg->iNumFieldReferences);

            int iFieldRefNum;

            for (iFieldRefNum = 0; iFieldRefNum < pMagicArg->iNumFieldReferences; iFieldRefNum++)
            {
                fprintf(pOutFile, "\"%s\"\n", pMagicArg->pFieldReferences[iFieldRefNum]);
            }

            fprintf(pOutFile, "%d\n", pMagicArg->iNumPotentialHelperRecursions);

            int iPotHelperNum;
            for (iPotHelperNum = 0; iPotHelperNum < pMagicArg->iNumPotentialHelperRecursions; iPotHelperNum++)
            {
                fprintf(pOutFile, "\"%s\" %d \"%s\"\n",
                    pMagicArg->pPotentialRecursions[iPotHelperNum]->pFuncName,
                    pMagicArg->pPotentialRecursions[iPotHelperNum]->iArgNum,
                    pMagicArg->pPotentialRecursions[iPotHelperNum]->pFieldString);
            }
        }
    }



    fprintf(pOutFile, "#endif\n");

    fclose(pOutFile);

    if (m_iNumFuncs)
    {
        pOutFile = fopen_nofail(m_AutoTransactionWrapperHeaderFileName, "wt");

        fprintf(pOutFile, "#pragma once"
"\n//This autogenerated file contains prototypes for wrapper functions"
"\n//for calling AUTO_TRANSACTIONs from project %s"
"\n//autogenerated" "nocheckin"
//"\n#include \"globaltypes.h\""
"\n#include \"localtransactionmanager.h\""
"\n\n", m_ProjectName);

        for (iFuncNum = 0; iFuncNum < m_iNumFuncs; iFuncNum++)
        {
            AutoTransactionFunc *pFunc = &m_Funcs[iFuncNum];

            WriteOutAutoGenAutoTransactionPrototype(pOutFile, pFunc);
            fprintf(pOutFile, ";\n");
        }

        fclose(pOutFile);
            

        pOutFile = fopen_nofail(m_AutoTransactionWrapperSourceFileName, "wt");

    //now write out autogenerated wrapper functions for autotransactions
        fprintf(pOutFile, "\n\n"
                          "// Autogenerated wrapper functions for calling auto transactions\n"
                          "#include <utilitieslib/components/estring.h>\n"
                          "#include <utilitieslib/utils/textparser.h>\n"
                          "#include \"objtransactions.h\"\n"
                          "#include <utilitieslib/components/earray.h>\n");

        for (iFuncNum = 0; iFuncNum < m_iNumFuncs; iFuncNum++)
        {
            AutoTransactionFunc *pFunc = &m_Funcs[iFuncNum];

            WriteOutAutoGenAutoTransactionPrototype(pOutFile, pFunc);
            fprintf(pOutFile, "\n{\n\tchar *pATRString = NULL;\n\tint iRetVal;\n\testrStackCreate(&pATRString, 1024);\n\n");
            fprintf(pOutFile, "\testrConcatf(&pATRString, \"%s \");\n", pFunc->functionName);
            

            int iArgNum;

            for (iArgNum = 0; iArgNum < pFunc->iNumArgs; iArgNum++)
            {
                ArgStruct *pArg = &pFunc->args[iArgNum];

                switch (pArg->eArgType)
                {
                case ATR_ARGTYPE_INT:
                    fprintf(pOutFile, "\testrConcatf(&pATRString, \"%%d \", %s);\n\n", pArg->argName);
                    break;

                case ATR_ARGTYPE_INT64:
                    fprintf(pOutFile, "\testrConcatf(&pATRString, \"%%I64d \", %s);\n\n", pArg->argName);
                    break;

                case ATR_ARGTYPE_FLOAT:
                    fprintf(pOutFile, "\testrConcatf(&pATRString, \"%%f \", %s);\n\n", pArg->argName);
                    break;
    
                case ATR_ARGTYPE_STRING:
                    fprintf(pOutFile, "\testrConcatf(&pATRString, \"\\\"\");\n");
                    fprintf(pOutFile, "\testrAppendEscaped(&pATRString, %s);\n", pArg->argName);
                    fprintf(pOutFile, "\testrConcatf(&pATRString, \"\\\" \");\n\n");
                    break;

                case ATR_ARGTYPE_CONTAINER:
                    if (pArg->bIsEArray)
                    {
                        fprintf(pOutFile, "\t{\n\t\tint i;\n\t\testrConcatf(&pATRString, \"%%s[\", GlobalTypeToName(%s_type));\n\t\tfor (i=0; i < ea32Size(%s_IDs); i++)\n\t\t{\n", pArg->argName, pArg->argName);
                        fprintf(pOutFile, "\t\t\testrConcatf(&pATRString, \"%%s%%u\", i > 0 ? \",\" : \"\", (*%s_IDs)[i]);\n\t\t}\n", pArg->argName);
                        fprintf(pOutFile, "\testrConcatf(&pATRString, \"] \");\n\t}\n\n");
                    }
                    else
                    {
                        fprintf(pOutFile, "\tif (%s_ID == 0)\n\t\testrConcatf(&pATRString, \"NULL \");\n\telse\n", pArg->argName);
                        fprintf(pOutFile, "\t\testrConcatf(&pATRString, \"%%s[%%u] \", GlobalTypeToName(%s_type), %s_ID);\n\n", pArg->argName, pArg->argName);
                    }
                    break;

                case ATR_ARGTYPE_STRUCT:
                    fprintf(pOutFile, "\tif (%s)\n\t{\n\t\tParserWriteTextEscaped(&pATRString, parse_%s, %s, 0, 0);\n\t}\n",
                        pArg->argName, pArg->argTypeName, pArg->argName);
                    fprintf(pOutFile, "\telse\n\t{\n\t\testrConcatf(&pATRString, \"NULL\");\n\t}\n");
                    fprintf(pOutFile, "\testrConcatf(&pATRString, \" \");\n\n");
                    break;
                }
            }

            fprintf(pOutFile, "\n\tiRetVal = objRequestAutoTransaction(pReturnVal, eServerTypeToRunOn, pATRString);\n");
            fprintf(pOutFile, "\testrDestroy(&pATRString);\n\treturn iRetVal;\n}\n\n");
        }
        fclose(pOutFile);
    }




    return true;
}


int AutoTransactionManager::FindFuncByName(char const* pString)
{
    int i;

    for (i=0; i < m_iNumFuncs; i++)
    {
        if (strcmp(pString, m_Funcs[i].functionName) == 0)
        {
            return i;
        }
    }

    return -1;
}
    
void AutoTransactionManager::ProcessArgNameInsideFunc(Tokenizer *pTokenizer, AutoTransactionFunc *pFunc, ArgStruct *pArg)
{
    Token token;
    enumTokenType eType;
    
    bool bFoundBrackets = false;


    pTokenizer->SaveLocation();

    eType = pTokenizer->GetNextToken(&token);

    if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTBRACKET)
    {
        int iBracketDepth = 1;

        bFoundBrackets = true;

        do
        {
            eType = pTokenizer->GetNextToken(&token);

            if (eType == TOKEN_NONE)
            {
                pTokenizer->Assert(0, "Found EOF in the middle of brackets for EARRAY AUTO_TRANS arg");
            }
            else if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTBRACKET)
            {
                iBracketDepth++;
            }
            else if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTBRACKET)
            {
                iBracketDepth--;
            }
        }
        while (iBracketDepth > 0);

        eType = pTokenizer->GetNextToken(&token);
    }


    if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_ARROW)
    {
        char outString[MAX_NAME_LENGTH];
        int iOutStringLength = 0;

        pTokenizer->Assert(bFoundBrackets == pArg->bIsEArray, "Container EArray used without [], or container non-EArray used with []");

        do
        {
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH - iOutStringLength, "Expected identifier after arg->");
            strcpy(outString + iOutStringLength, token.sVal);
            iOutStringLength += token.iVal;

            eType = pTokenizer->CheckNextToken(&token);

            if (eType == TOKEN_RESERVEDWORD && (token.iVal == RW_ARROW || token.iVal == RW_DOT))
            {
                pTokenizer->Assert(iOutStringLength < MAX_NAME_LENGTH - 3, "String of dereferences has too many characters");
                pTokenizer->StringifyToken(&token);
                int iLen = (int)strlen(token.sVal);
                strcpy(outString + iOutStringLength, token.sVal);
                iOutStringLength += iLen;

                pTokenizer->GetNextToken(&token);
            }
            else
            {
                //if we are inside a function and are not about to encounter a [, and if the function we're inside
                //is not an ATR_RECURSE, we might be in a helper function

                if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTBRACKET)
                {
                    AddFieldToLockToArg(pArg, outString, ATR_FTL_NORMAL, 0, NULL);
                    return;
                }
                else
                {
                    char funcName[MAX_NAME_LENGTH];
                    int iArgNum;
                    int iTempLineNum;
                    bool bFoundATRRecurse;

                    if (FindRecurseFunctionCallContainingPoint(pTokenizer, pFunc->iStartingTokenizerOffset, pFunc->iSourceFileLineNum, 
                        pTokenizer->GetOffset(&iTempLineNum), true, funcName, &iArgNum, &bFoundATRRecurse))
                    {
                        if (bFoundATRRecurse)
                        {
                            AddFieldToLockToArg(pArg, outString, ATR_FTL_NORMAL, 0, NULL);
                            return;
                        }
                        else
                        {
                            AddRecurseFunctionToArg(pArg, iArgNum, funcName, false, outString);
                            return;
                        }
                    }

                    AddFieldToLockToArg(pArg, outString, ATR_FTL_NORMAL, 0, NULL);
                    return;
            

                }

                break;
            }
        }
        while (1);

        return;
    }

    pTokenizer->RestoreLocation();
    pTokenizer->SaveLocation();

    char funcName[MAX_NAME_LENGTH];
    int iArgNum;
    int iTempLineNum;
    bool bFoundATRRecurse;


    if (FindRecurseFunctionCallContainingPoint(pTokenizer, pFunc->iStartingTokenizerOffset, pFunc->iSourceFileLineNum, 
        pTokenizer->GetOffset(&iTempLineNum), false, funcName, &iArgNum, &bFoundATRRecurse))
    {
        if (bFoundATRRecurse)
        {
            AddRecurseFunctionToArg(pArg, iArgNum - 1, funcName, true, "");
        }
        else
        {
            if (IsFullContainerFunction(funcName, iArgNum, pArg->argTypeName))
            {
                AddFieldToLockToArg(pArg, ".*", ATR_FTL_NORMAL, 0, NULL);
            }
            else if (IsFullContainerFunction_NoFields(funcName, iArgNum, pArg->argTypeName))
            {
                //do nothing, we are happy
            }
            else
            {
                AddRecurseFunctionToArg(pArg, iArgNum, funcName, false, "");
            }
        }    

        pTokenizer->RestoreLocation();
        return;
    }

    pTokenizer->RestoreLocation();
    pTokenizer->SaveLocation();


    pTokenizer->Assertf(0, "invalid use of argument %s", pArg->argName);
}


//skips over a function call that has open parens, then optional ampersand, then a single identifier that may be a container 
//arg name, then close parens. If any of the above are not found, skip back out and keep going normaly
void AutoTransactionManager::SkipSafeSimpleFunction(Tokenizer *pTokenizer, AutoTransactionFunc *pFunc)
{
    pTokenizer->SaveLocation();

    Token token;
    enumTokenType eType;

    eType = pTokenizer->GetNextToken(&token);

    if (!(eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTPARENS))
    {
        pTokenizer->RestoreLocation();
        return;
    }

    eType = pTokenizer->GetNextToken(&token);

    if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_AMPERSAND)
    {
        eType = pTokenizer->GetNextToken(&token);
    }

    if (eType != TOKEN_IDENTIFIER)
    {
        pTokenizer->RestoreLocation();
        return;
    }
    eType = pTokenizer->GetNextToken(&token);

    if (!(eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTPARENS))
    {
        pTokenizer->RestoreLocation();
        return;
    }    
}

    
void AutoTransactionManager::FoundMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer, int iWhichMagicWord, char const* pMagicWordString)
{

    switch (iWhichMagicWord)
    {
    case MAGICWORD_AUTOTRANSACTION:
        FoundAutoTransMagicWord(pSourceFileName, pTokenizer);
        break;

    case MAGICWORD_AUTOTRANSHELPER:
        FoundAutoTransHelperMagicWord(pSourceFileName, pTokenizer);
        break;
    }
}

void AutoTransactionManager::FoundAutoTransHelperMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer)
{
    Token token;
    enumTokenType eType;
    bool bFoundNoConst = false;

    pTokenizer->Assert(m_iNumHelperFuncs < MAX_AUTO_TRANSACTION_HELPER_FUNCS, "too many auto trans helper funcs");
    HelperFunc *pHelperFunc = &m_HelperFuncs[m_iNumHelperFuncs++];
    memset(pHelperFunc, 0, sizeof(HelperFunc));

    strcpy(pHelperFunc->sourceFileName, pSourceFileName);

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_SEMICOLON, "Expected ;  after AUTO_TRANS_HELPER");

    eType = pTokenizer->CheckNextToken(&token);
    if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "NOCONST") == 0)
    {
        pTokenizer->GetNextToken(&token);
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after NOCONST");
        bFoundNoConst = true;
    }
    else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "const") == 0)
    {
        pTokenizer->GetNextToken(&token);
    }

    pTokenizer->Assert2NextTokenTypesAndGet(&token, TOKEN_RESERVEDWORD, RW_VOID, TOKEN_IDENTIFIER, 0, "Expected return type after AUTO_TRANS_HELPER;");

    if (bFoundNoConst)
    {
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after NOCONST(x");
    }


    eType = pTokenizer->GetNextToken(&token);

    while (eType == TOKEN_RESERVEDWORD && token.iVal == RW_ASTERISK)
    {
        eType = pTokenizer->GetNextToken(&token);
    }


    pTokenizer->Assert(eType == TOKEN_IDENTIFIER && token.iVal < MAX_NAME_LENGTH - 1, "Expected AUTO_TRANS_HELPER function name");

    strcpy(pHelperFunc->functionName, token.sVal);

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after function name");

    int iCommaCount = 0;
    int iParenDepth = 1;

    while (1)
    {
        eType = pTokenizer->GetNextToken(&token);

        pTokenizer->Assert(eType != TOKEN_NONE, "Unexpected end of file");

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTPARENS)
        {
            iParenDepth++;
        }
        else if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTPARENS)
        {
            iParenDepth--;
            if (iParenDepth == 0)
            {
                pTokenizer->Assert(pHelperFunc->iNumMagicArgs>0, "Helper func appears to have to ATH_ARGs... what is the point? What is the damn point?");
                break;
            }
        }
        else if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_COMMA)
        {
            if (iParenDepth == 1)
            {
                iCommaCount++;
            }
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ATH_ARG") == 0)
        {
            eType = pTokenizer->GetNextToken(&token);

            if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "const") == 0)
            {
                eType = pTokenizer->GetNextToken(&token);
            }

            if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "NOCONST") == 0)
            {
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after NOCONST");
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH, "Expected container type name after NOCONST(");
                /*strcpy(pArg->argTypeName, token.sVal);
                pArg->eArgType = ATR_ARGTYPE_CONTAINER;*/
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after NOCONST");

            }
            else
            {
                pTokenizer->Assert(eType == TOKEN_RESERVEDWORD && token.iVal == RW_VOID || eType == TOKEN_IDENTIFIER, "Expected arg type after ATH_ARG");
            }

            eType = pTokenizer->GetNextToken(&token);

            while (eType == TOKEN_RESERVEDWORD && token.iVal == RW_ASTERISK)
            {
                eType = pTokenizer->GetNextToken(&token);
            }

            pTokenizer->Assert(eType == TOKEN_IDENTIFIER && token.iVal < MAX_NAME_LENGTH - 1, "Expected arg name");

            pTokenizer->Assert(pHelperFunc->iNumMagicArgs < MAX_MAGIC_ARGS_PER_HELPER_FUNC, "Too many ATH_ARGs for one func");

            HelperFuncArg *pMagicArg = pHelperFunc->pMagicArgs[pHelperFunc->iNumMagicArgs++] = new HelperFuncArg;
            memset(pMagicArg, 0, sizeof(HelperFuncArg));


            strcpy(pMagicArg->argName, token.sVal);
            pMagicArg->iArgIndex = iCommaCount;
        }
    }

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTBRACE, "Expected { after ATH_ARGs");

    //first, go through and find the end brace
    int iOffsetAtBeginningOfFunctionBody;
    int iLineNumAtBeginningOfFunctionBody;
    int iOffsetAtEndOfFunctionBody;
    int iLineNumAtEndOfFunctionBody;

    iOffsetAtBeginningOfFunctionBody = pTokenizer->GetOffset(&iLineNumAtBeginningOfFunctionBody);

    int iBraceDepth = 1;

    while (1)
    {
        eType = pTokenizer->GetNextToken(&token);

        pTokenizer->Assert (eType != TOKEN_NONE, "Unexpected EOF in helper function");

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTBRACE)
        {
            iBraceDepth++;
        }
        else if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTBRACE)
        {
            iBraceDepth--;

            if (iBraceDepth == 0)
            {
                iOffsetAtEndOfFunctionBody = pTokenizer->GetOffset(&iLineNumAtEndOfFunctionBody);
                break;
            }
        }
    }

    pTokenizer->SetOffset(iOffsetAtBeginningOfFunctionBody, iLineNumAtBeginningOfFunctionBody);

    while (pTokenizer->GetOffset(NULL) < iOffsetAtEndOfFunctionBody)
    {
        eType = pTokenizer->GetNextToken(&token);

        if (eType == TOKEN_IDENTIFIER)
        {
            int iArgNum;

            for (iArgNum=0; iArgNum < pHelperFunc->iNumMagicArgs; iArgNum++)
            {
                if (strcmp(token.sVal, pHelperFunc->pMagicArgs[iArgNum]->argName) == 0)
                {
                    HelperFuncArg *pMagicArg = pHelperFunc->pMagicArgs[iArgNum];
                    
                    //put all .x->foo.bar stuff in fieldString
                    char fieldString[512] = "";

                    int iOffsetAfterArgTokenAndFields;
                    int iLineNumAfterArgTokenAndFields;

                    iOffsetAfterArgTokenAndFields = pTokenizer->GetOffset(&iLineNumAfterArgTokenAndFields);

                    while (1)
                    {
                        eType = pTokenizer->GetNextToken(&token);

                        if (!(eType == TOKEN_RESERVEDWORD && (token.iVal == RW_DOT || token.iVal == RW_ARROW)))
                        {
                            break;
                        }

                        pTokenizer->StringifyToken(&token);
                        strcat(fieldString, token.sVal);

                        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 0, "Expected identifier after . or ->");

                        strcat(fieldString, token.sVal);
    
                        iOffsetAfterArgTokenAndFields = pTokenizer->GetOffset(&iLineNumAfterArgTokenAndFields);
                    }

                    //if the next token is [, then there's no point in looking for function calls, because we can't do
                    //anything fancy past a [

                    if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTBRACKET)
                    {
                        AddFieldReferenceToHelperFuncMagicArg(pTokenizer, pMagicArg, fieldString);
                    }
                    else
                    {
                        char foundFuncName[256];
                        int iFoundArgNum;
                        bool bFoundATRRecurse;

                        if (FindRecurseFunctionCallContainingPoint(pTokenizer, 
                            iOffsetAtBeginningOfFunctionBody, iLineNumAtBeginningOfFunctionBody, 
                                iOffsetAfterArgTokenAndFields, true,
                                foundFuncName, &iFoundArgNum, &bFoundATRRecurse))
                        {
                            pTokenizer->Assert(!bFoundATRRecurse, "ATR_RECURSE not allowed inside AUTO_TRANS_HELPER");

                            AddPotentialHelperFuncRecurseToHelperFuncMagicArg(pTokenizer, pMagicArg, foundFuncName, iFoundArgNum, fieldString);
                        }
                        else
                        {
                            AddFieldReferenceToHelperFuncMagicArg(pTokenizer, pMagicArg, fieldString);
                        }
                    }



                    pTokenizer->SetOffset(iOffsetAfterArgTokenAndFields, iLineNumAfterArgTokenAndFields);
                    break;
                }
            }
        }
    }

    m_bSomethingChanged = true;
}



void AutoTransactionManager::FoundAutoTransMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer)
{
    Tokenizer tokenizer;

    Token token;
    enumTokenType eType;

    
    pTokenizer->Assert(m_iNumFuncs < MAX_AUTO_TRANSACTION_FUNCS, "Too many auto transactions");


    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_SEMICOLON, "Expected ; after AUTO_TRANSACTION");

    pTokenizer->AssertGetIdentifier("enumTransactionOutcome");

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH, "Expected func name after enumTransactionOutcome");

    pTokenizer->Assert(FindFuncByName(token.sVal) == -1, "Func name already used for AutoTransaction");




    AutoTransactionFunc *pFunc = &m_Funcs[m_iNumFuncs++];

    m_bSomethingChanged = true;


    pFunc->iNumArgs = 0;
    strcpy(pFunc->sourceFileName, pSourceFileName);
    pFunc->bRecursingAlreadyDone = false;

    pFunc-> iStartingTokenizerOffset = pTokenizer->GetOffset(&pFunc->iSourceFileLineNum);


    strcpy(pFunc->functionName, token.sVal);

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after func name");
    pTokenizer->AssertGetIdentifier("ATR_ARGS");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after ATR_ARGS");

    do
    {
        bool bFoundNonContainer = false;

        eType = pTokenizer->GetNextToken(&token);

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTPARENS)
        {
            break;
        }

        if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "NON_CONTAINER") == 0)
        {
            bFoundNonContainer = true;
            eType = pTokenizer->GetNextToken(&token);
        }



        if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "const") == 0)
        {
            eType = pTokenizer->GetNextToken(&token);
        }


        pTokenizer->Assert(pFunc->iNumArgs < MAX_ARGS_PER_AUTO_TRANSACTION_FUNC, "Too many args for one func");

        ArgStruct *pArg = &pFunc->args[pFunc->iNumArgs++];
    
        pArg->pFirstFieldToLock = NULL;
        pArg->pFirstRecursingFunction = NULL;    
        pArg->bIsPointer = false;
        pArg->bIsEArray = false;
        pArg->bFoundNonContainer = bFoundNonContainer;


        if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "NOCONST") == 0)
        {
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after NOCONST");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH, "Expected container type name after NOCONST(");
            strcpy(pArg->argTypeName, token.sVal);
            pArg->eArgType = ATR_ARGTYPE_CONTAINER;
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after NOCONST");

        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "CONST_EARRAY_OF") == 0)
        {
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after CONST_EARRAY_OF");
            pTokenizer->AssertGetIdentifier("NOCONST");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after CONST_EARRAY_OF(NOCONST");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH, "Expected container type name after CONST_EARRAY_OF(NOCONST(");
            strcpy(pArg->argTypeName, token.sVal);
            pArg->eArgType = ATR_ARGTYPE_CONTAINER;
            pArg->bIsEArray = true;
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after CONST_EARRAY_OF(NOCONST(x");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after CONST_EARRAY_OF(NOCONST(x)");
        }
        else
        {

            pTokenizer->Assert(eType == TOKEN_IDENTIFIER, "Expected argument type name");
            pTokenizer->Assert(strlen(token.sVal) < MAX_NAME_LENGTH, "Argument type name too long");
    
            pArg->eArgType = GetArgTypeFromString(token.sVal);

            if (pArg->eArgType == ATR_ARGTYPE_CONTAINER)
            {
                pArg->eArgType = ATR_ARGTYPE_STRUCT;
            }

            strcpy(pArg->argTypeName, token.sVal);
        }

        eType = pTokenizer->GetNextToken(&token);

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_ASTERISK)
        {
            pArg->bIsPointer = true;
            eType = pTokenizer->GetNextToken(&token);
        }

        pTokenizer->Assert(eType == TOKEN_IDENTIFIER, "Expected argument name");
        pTokenizer->Assert(strlen(token.sVal) < MAX_NAME_LENGTH, "Argument name too long");
        
        strcpy(pArg->argName, token.sVal);

        CheckArgTypeValidity(pArg, pTokenizer);

        eType = pTokenizer->CheckNextToken(&token);

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_COMMA)
        {
            eType = pTokenizer->GetNextToken(&token);
        }
    }
    while (1);

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTBRACE, "Expected { after arguments");
    pTokenizer->AssertGetIdentifier("ATR_BEGIN");

    int iBraceDepth = 1;

    do
    {
        eType = pTokenizer->GetNextToken(&token);

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTBRACE)
        {
            iBraceDepth++;
        }
        else if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTBRACE)
        {
            iBraceDepth--;
            
            pTokenizer->Assert(iBraceDepth > 0, "Function appears to have ended without ATR_END");
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ATR_END") == 0)
        {
            pTokenizer->Assert(iBraceDepth == 1, "ATR_END appears to be mispositioned");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTBRACE, "Expected } after ATR_END");
            break;
        }
        else if (eType == TOKEN_IDENTIFIER)
        {
            int i;

            if (strcmp(token.sVal, "eaIndexedGetUsingInt") == 0)
            {
                ProcessEArrayGet(pTokenizer, pFunc, false);
            }
            else if (strcmp(token.sVal, "eaIndexedGetUsingString") == 0)
            {
                ProcessEArrayGet(pTokenizer, pFunc, true);
            }
            else if (StringIsInList(token.sVal, sAutoTransSafeSimpleFunctionNames))
            {
                SkipSafeSimpleFunction(pTokenizer, pFunc);
            }
            else
            {
                for (i=0; i < pFunc->iNumArgs; i++)
                {
                    if (pFunc->args[i].eArgType == ATR_ARGTYPE_CONTAINER && strcmp(token.sVal, pFunc->args[i].argName) == 0)
                    {
                        ProcessArgNameInsideFunc(pTokenizer, pFunc, &pFunc->args[i]);
                        break;
                    }
                }
            }
        }

    } while (1);
}

void AutoTransactionManager::ProcessEArrayGet(Tokenizer *pTokenizer, AutoTransactionFunc *pFunc, 
    bool bUseString)
{
    Token token;
    enumTokenType eType;
    int iArgNum = -1;
    int i;

    pTokenizer->SaveLocation();

    eType = pTokenizer->GetNextToken(&token);

    if (!(eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTPARENS))
    {
        return;
    }

    eType = pTokenizer->GetNextToken(&token);

    if (!(eType == TOKEN_RESERVEDWORD && token.iVal == RW_AMPERSAND))
    {
        return;
    }

    eType = pTokenizer->GetNextToken(&token);

    if (eType != TOKEN_IDENTIFIER)
    {
        return;
    }

    for (i=0; i < pFunc->iNumArgs; i++)
    {
        if (pFunc->args[i].eArgType == ATR_ARGTYPE_CONTAINER && strcmp(token.sVal, pFunc->args[i].argName) == 0)
        {
            iArgNum = i;
            break;
        }
    }

    if (iArgNum == -1)
    {
        return;
    }

    if (pFunc->args[i].bIsEArray)
    {
        pTokenizer->AssertGetBracketBalancedBlock(RW_LEFTBRACKET, RW_RIGHTBRACKET, "Expected [x] after eaIndexedGet(&containerName");
    }
    
    
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_ARROW, "Expected -> after eaIndexedGet(&containerName");

    
    char outString[MAX_NAME_LENGTH];
    int iOutStringLength = 0;

    do
    {
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_NAME_LENGTH - iOutStringLength, "Expected identifier after eaIndexedGet(&containerName->");
        strcpy(outString + iOutStringLength, token.sVal);
        iOutStringLength += token.iVal;

        eType = pTokenizer->CheckNextToken(&token);

        if (eType == TOKEN_RESERVEDWORD && (token.iVal == RW_ARROW || token.iVal == RW_DOT))
        {
            pTokenizer->Assert(iOutStringLength < MAX_NAME_LENGTH - 3, "String of dereferences has too many characters");
            pTokenizer->StringifyToken(&token);
            int iLen = (int)strlen(token.sVal);
            strcpy(outString + iOutStringLength, token.sVal);
            iOutStringLength += iLen;

            pTokenizer->GetNextToken(&token);
        }
        else
        {
            break;
        }
    }
    while (1);

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after eaIndexedGet(&containerName->x");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 0, "Expected parse table name after eaIndexedGet(&containerName->x,");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after eaIndexedGet(&containerName->x, parsetable");

    eType = pTokenizer->GetNextToken(&token);

    if (bUseString)
    {
        if (eType == TOKEN_STRING)
        {
            AddFieldToLockToArg(&pFunc->args[iArgNum], outString, ATR_FTL_INDEXED_LITERAL_STRING, 0, token.sVal);
            return;
        }

        if (eType == TOKEN_IDENTIFIER)
        {
            for (i=0; i < pFunc->iNumArgs; i++)
            {
                if (pFunc->args[i].eArgType == ATR_ARGTYPE_STRING && strcmp(token.sVal, pFunc->args[i].argName) == 0)
                {
                    AddFieldToLockToArg(&pFunc->args[iArgNum], outString, ATR_FTL_INDEXED_STRING_ARG, i, NULL);
                    return;
                }
            }
        }
    }
    else
    {
        if (eType == TOKEN_INT)
        {
            AddFieldToLockToArg(&pFunc->args[iArgNum], outString, ATR_FTL_INDEXED_LITERAL_INT, token.iVal, NULL);
            return;
        }

        if (eType == TOKEN_IDENTIFIER)
        {
            for (i=0; i < pFunc->iNumArgs; i++)
            {
                if (pFunc->args[i].eArgType == ATR_ARGTYPE_INT && strcmp(token.sVal, pFunc->args[i].argName) == 0)
                {
                    AddFieldToLockToArg(&pFunc->args[iArgNum], outString, ATR_FTL_INDEXED_INT_ARG, i, NULL);
                    return;
                }
            }
        }
    }
    
    pTokenizer->Assert(0, "Invalid third argument to eaIndexedGet... must be literal int/string or int/string arg name");
}






bool AutoTransactionManager::FindRecurseFunctionCallContainingPoint(Tokenizer *pTokenizer, 
    int iStartingOffset, int iStartingLineNum, int iOffsetToFind, bool bOKIfArgIsDereferenced, char* pFuncName, int *pOutArgNum, bool *pbFoundATRRecurse)
{
    Token token;
    enumTokenType eType;
    int iDummyLineNum;
    char funcName[MAX_NAME_LENGTH];

    pTokenizer->SetOffset(iStartingOffset, iStartingLineNum);
    
    do
    {
        eType = pTokenizer->GetNextToken(&token);

        if (eType == TOKEN_NONE || eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ATR_END") == 0 || pTokenizer->GetOffset(&iDummyLineNum) > iOffsetToFind )
        {
            return false;
        }

        if (eType == TOKEN_IDENTIFIER && strlen(token.sVal) < MAX_NAME_LENGTH)
        {
        
            strcpy(funcName, token.sVal);

            int iAfterIdentifierOffset, iAfterIdentifierLineNum;
            iAfterIdentifierOffset = pTokenizer->GetOffset(&iAfterIdentifierLineNum);

            eType = pTokenizer->GetNextToken(&token);

            if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTPARENS)
            {
                eType = pTokenizer->CheckNextToken(&token);

                if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ATR_RECURSE") == 0)
                {
                    *pbFoundATRRecurse = true;
                    pTokenizer->GetNextToken(&token);
                }
                else
                {
                    *pbFoundATRRecurse = false;
                }


                int iAfterRecurseOffset, iAfterRecurseLineNum;

                iAfterRecurseOffset = pTokenizer->GetOffset(&iAfterRecurseLineNum);

                pTokenizer->GetSpecialStringTokenWithParenthesesMatching(&token);

                //check if we are inside the argument list for this function
                if (pTokenizer->GetOffset(&iDummyLineNum) > iOffsetToFind) 
                {
                    pTokenizer->SetOffset(iAfterRecurseOffset, iAfterRecurseLineNum);

                    int iCommaCount = 0;
                    bool bFirstArg = true;

                    do
                    {
                        int iParensDepth = 0;

                        //the first time through, if we found ATR_RECURSE, we already need to find a comma or )
                        //
                        //otherwise, we skip over that step the first time
                        if (bFirstArg && !(*pbFoundATRRecurse))
                        {
                            bFirstArg = false;
                        }
                        else
                        {

                            eType = pTokenizer->GetNextToken(&token);

                            pTokenizer->Assert(eType == TOKEN_RESERVEDWORD && (token.iVal == RW_COMMA || token.iVal == RW_RIGHTPARENS), "Expected , or ) after function argument");

                            if (token.iVal == RW_RIGHTPARENS)
                            {
                                break;
                            }

                            iCommaCount++;
                        }
                        
                        eType = pTokenizer->GetNextToken(&token);

                        if (pTokenizer->GetOffset(&iDummyLineNum) == iOffsetToFind)
                        {
                            eType = pTokenizer->GetNextToken(&token);

                            if (eType == TOKEN_RESERVEDWORD && (token.iVal == RW_COMMA || token.iVal == RW_RIGHTPARENS || token.iVal == RW_LEFTBRACKET))
                            {
                                strcpy(pFuncName, funcName);
                                *pOutArgNum = iCommaCount;
                                return true;
                            }
                            else
                            {
                                return false;
                            }
                        }

                        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTPARENS)
                        {
                            iParensDepth++;
                        }

                        do
                        {
                            eType = pTokenizer->CheckNextToken(&token);

                            if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTPARENS)
                            {
                                if (iParensDepth == 0)
                                {
                                    break;
                                }
                                else
                                {
                                    iParensDepth--;
                                }
                            }
                            else if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_COMMA && iParensDepth == 0)
                            {
                                break;
                            }
                            else if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTPARENS)
                            {
                                iParensDepth++;
                            }

                            eType = pTokenizer->GetNextToken(&token);
                        }
                        while (1);

                        if (pTokenizer->GetOffset(&iDummyLineNum) == iOffsetToFind && bOKIfArgIsDereferenced)
                        {
                            strcpy(pFuncName, funcName);
                            *pOutArgNum = iCommaCount;
                            return true;
                        }
                    }
                    while (1);
                }
            }

            pTokenizer->SetOffset(iAfterIdentifierOffset, iAfterIdentifierLineNum);
        }
    }
    while (1);

}



















static char const* sIntNames[] =
{
    "int",
    "short",
    "S8",
    "S16",
    "S32",
    "U8",
    "U16",
    "U32",
    "ContainerID",
    NULL
};

static char const* sInt64Names[] =
{
    "S64",
    "U64",
    NULL
};


static char const* sFloatNames[] = 
{
    "float",
    "F32",
    NULL
};

static char const* sStringNames[] = 
{
    "char",
    NULL
};



AutoTransactionManager::enumAutoTransArgType AutoTransactionManager::GetArgTypeFromString(char const* pArgTypeName)
{
    if (StringIsInList(pArgTypeName, sIntNames))
    {
        return ATR_ARGTYPE_INT;
    }

    if (StringIsInList(pArgTypeName, sInt64Names))
    {
        return ATR_ARGTYPE_INT64;
    }

    if (StringIsInList(pArgTypeName, sFloatNames))
    {
        return ATR_ARGTYPE_FLOAT;
    }

    if (StringIsInList(pArgTypeName, sStringNames))
    {
        return ATR_ARGTYPE_STRING;
    }


    return ATR_ARGTYPE_CONTAINER;
    
}

void AutoTransactionManager::CheckArgTypeValidity(ArgStruct *pArg, Tokenizer *pTokenizer)
{
    switch (pArg->eArgType)
    {
    case ATR_ARGTYPE_INT:
    case ATR_ARGTYPE_INT64:
        return;
    case ATR_ARGTYPE_FLOAT:
        return;
    case ATR_ARGTYPE_STRING:
        if (pArg->bIsPointer)
        {
            return;
        }
        pArg->eArgType = ATR_ARGTYPE_INT;
        return;


    case ATR_ARGTYPE_STRUCT:
        if (pArg->bIsPointer)
        {
            return;
        }
        break;
    case ATR_ARGTYPE_CONTAINER:
        if (pArg->bIsPointer || pArg->bIsEArray)
        {
            return;
        }
        break;
    }

    char errorString[1024];
    sprintf(errorString, "Unrecognized, or improperly used, arg type %s", pArg->argTypeName);
    pTokenizer->Assert(0, errorString);
}

//returns number of dependencies found
int AutoTransactionManager::ProcessDataSingleFile(char const* pSourceFileName, char* pDependencies[MAX_DEPENDENCIES_SINGLE_FILE])
{
    return 0;
}

void AutoTransactionManager::AddPotentialHelperFuncRecurseToHelperFuncMagicArg(Tokenizer *pTokenizer, HelperFuncArg *pArg,
    char const* pFuncName, int iArgNum, char const* pFieldString)
{
    int i;

    for (i=0; i < pArg->iNumPotentialHelperRecursions; i++)
    {
        if (strcmp(pArg->pPotentialRecursions[i]->pFuncName, pFuncName) == 0
            && pArg->pPotentialRecursions[i]->iArgNum == iArgNum
            && strcmp(pArg->pPotentialRecursions[i]->pFieldString, pFieldString) == 0)
        {
            return;
        }
    }

    pTokenizer->Assert(pArg->iNumPotentialHelperRecursions < MAX_POTENTIAL_HELPER_FUNC_RECURSIONS_PER_ARG, "Too many helper function recursions");

    PotentialHelperFuncRecursion *pRecursion = new PotentialHelperFuncRecursion;
    pRecursion->iArgNum = iArgNum;
    pRecursion->pFieldString = STRDUP(pFieldString);
    pRecursion->pFuncName = STRDUP(pFuncName);

    pArg->pPotentialRecursions[pArg->iNumPotentialHelperRecursions++] = pRecursion;
}



void AutoTransactionManager::AddFieldReferenceToHelperFuncMagicArg(Tokenizer *pTokenizer, HelperFuncArg *pArg, char const* pFieldString)
{
    int i;

    for (i=0; i < pArg->iNumFieldReferences; i++)
    {
        if (strcmp(pArg->pFieldReferences[i], pFieldString) == 0)
        {
            return;
        }
    }

    pTokenizer->Assert(pArg->iNumFieldReferences < MAX_FIELD_REFERENCES_PER_HELPER_FUNC_ARG, "Too many field references");

    pArg->pFieldReferences[pArg->iNumFieldReferences++] = STRDUP(pFieldString);
}
