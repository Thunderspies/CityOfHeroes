#include "pch.h"
#include "LateLinkManager.h"
#include "strutils.h"
#include "sourceparser.h"
#include "autorunmanager.h"


#define LATELINK_WILDCARD_PREFIX "AUTO_RUN_"

LateLinkManager::LateLinkManager()
{
    m_bSomethingChanged = false;
    m_iNumLateLinks = 0;
    m_LateLinkFileName[0] = 0;
    m_ProjectName[0] = 0;
}

LateLinkManager::~LateLinkManager()
{
}



char const* LateLinkManager::GetMagicWord(int iWhichMagicWord)
{
    switch (iWhichMagicWord)
    {
    case LATELINK_FUNCBODY:
        return "DEFAULT_LATELINK_" WILDCARD_STRING;
    case LATELINK_OVERRIDE:
        return "OVERRIDE_LATELINK_" WILDCARD_STRING;
    case LATELINK_DECLARATION:
        return "LATELINK";
    }

    return "x x";
}


enum
{
    RW_PARSABLE = RW_COUNT,
};

static char const* sLateLinkReservedWords[] =
{
    "PARSABLE",
    NULL
};

void LateLinkManager::SetProjectPathAndName(char const* srcPath, char const* commonPath, char const* projectName)
{
    strcpy(m_ProjectName, projectName);

    sprintf(m_LateLinkFileName, "%s\\AutoGen\\%s_latelink_autogen.c", srcPath, projectName);
}

bool LateLinkManager::DoesFileNeedUpdating(char const* pFileName)
{
    return false;
}


bool LateLinkManager::LoadStoredData(bool bForceReset)
{
    if (bForceReset)
    {
        m_bSomethingChanged = true;
        return false;
    }

    Tokenizer tokenizer;

    if (!tokenizer.LoadFromFile(m_LateLinkFileName))
    {
        m_bSomethingChanged = true;
        return false;
    }

    if (!tokenizer.IsStringAtVeryEndOfBuffer("#endif"))
    {
        m_bSomethingChanged = true;
        return false;
    }

    tokenizer.SetExtraReservedWords(sLateLinkReservedWords);

    Token token;
    enumTokenType eType;

    do
    {
        eType = tokenizer.GetNextToken(&token);
        Tokenizer::StaticAssert(eType != TOKEN_NONE, "LATELINK data corruption");
    } while (!(eType == TOKEN_RESERVEDWORD && token.iVal == RW_PARSABLE));

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find number of autruns");

    m_iNumLateLinks = token.iVal;

    int iLateLinkNum;
    int iArgNum;

    for (iLateLinkNum = 0; iLateLinkNum < m_iNumLateLinks; iLateLinkNum++)
    {
        LATE_LINK_STRUCT *pLateLink = &m_LateLinks[iLateLinkNum];
    
        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Expected eLateLinkType value");
        pLateLink->eLateLinkType = (enumLateLinkType)token.iVal;

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, LATELINK_NAME_LENGTH, "Expected latelink func name");
        strcpy(pLateLink->funcName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, LATELINK_NAME_LENGTH, "Expected latelink ret type");
        strcpy(pLateLink->retType, token.sVal);

        for (iArgNum = 0; iArgNum < MAX_LATELINK_ARGS; iArgNum++)
        {
            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, LATELINK_NAME_LENGTH, "Expected arg type");
            strcpy(pLateLink->argTypes[iArgNum], token.sVal);
        }

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_PATH, "Expected latelink source file");
        strcpy(pLateLink->sourceFileName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Expected line num");
        pLateLink->iSourceLineNum = token.iVal;

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Expected index in file");
        pLateLink->iIndexInFile = token.iVal;

    
        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, FULL_TYPE_STRING_LENGTH, "Expected latelink full type string");
        strcpy(pLateLink->fullTypeString, token.sVal);
    }

    m_bSomethingChanged = false;

    return true;
}


void LateLinkManager::ResetSourceFile(char const* pSourceFileName)
{
    int i = 0;


    while (i < m_iNumLateLinks)
    {
        if (AreFilenamesEqual(m_LateLinks[i].sourceFileName, pSourceFileName))
        {
            memcpy(&m_LateLinks[i], &m_LateLinks[m_iNumLateLinks - 1], sizeof(LATE_LINK_STRUCT));
            m_iNumLateLinks--;

            m_bSomethingChanged = true;
        }
        else
        {
            i++;
        }
    }
}

void LateLinkManager::WriteInternalLateLinkArgList(FILE *pOutFile, LATE_LINK_STRUCT *pLateLink)
{
    int iArgNum;

    if (pLateLink->argTypes[0][0] == 0)
    {
        fprintf(pOutFile, "void");
    }
    else
    {
        for(iArgNum = 0; iArgNum < MAX_LATELINK_ARGS && pLateLink->argTypes[iArgNum][0]; iArgNum++)
        {
            fprintf(pOutFile, "%s%s arg%d", 
                iArgNum == 0 ? "" : ", ",
                pLateLink->argTypes[iArgNum],
                iArgNum);
        }
    }
}

void LateLinkManager::VerifyPreWriteOut(void)
{
    int iLateLinkNum;

    for (iLateLinkNum = 0; iLateLinkNum < m_iNumLateLinks; iLateLinkNum++)
    {
        LATE_LINK_STRUCT *pLateLink = &m_LateLinks[iLateLinkNum];

        if (pLateLink->eLateLinkType == LATELINK_FUNCBODY)
        {
            int iNumOverrides = 0;
            int iNumDeclarations = 0;
            int i;

            for (i=0; i < m_iNumLateLinks; i++)
            {
                if (i != iLateLinkNum)
                {
                    LATE_LINK_STRUCT *pOtherLateLink = &m_LateLinks[i];

                    if (strcmp(pLateLink->funcName, pOtherLateLink->funcName) == 0)
                    {
                        Tokenizer::StaticAssertf(strcmp(pLateLink->fullTypeString, pOtherLateLink->fullTypeString) == 0, 
                            "Type incompatibility between two references to LATELINK %s\n%s(%d) : error S0000 : (StructParser)\n%s(%d) : error S0000 : (StructParser)\n", 
                            pLateLink->funcName, pLateLink->sourceFileName, pLateLink->iSourceLineNum,
                            pOtherLateLink->sourceFileName, pOtherLateLink->iSourceLineNum);


                        switch (pOtherLateLink->eLateLinkType)
                        {
                        case LATELINK_FUNCBODY:
                            Tokenizer::StaticAssertf(0, "Found two function bodies for DEFAULT_LATELINK %s", pLateLink->funcName);
                            break;

                        case LATELINK_OVERRIDE:
                            iNumOverrides++;
                            Tokenizer::StaticAssertf(iNumOverrides == 1, "Found two function bodies for OVERRIDE_LATELINK %s", pLateLink->funcName);
                            break;

                        case LATELINK_DECLARATION:
                            iNumDeclarations++;
                        }
                    }
                }
            }

            Tokenizer::StaticAssertf(iNumDeclarations > 0, "Found no function declarations for LATELINK %s... this can lead to things getting out of sync",
                pLateLink->funcName);


        }
        else if (pLateLink->eLateLinkType == LATELINK_DECLARATION)
        {
            int iNumFuncBodies = 0;
            int i;

            for (i=0; i < m_iNumLateLinks; i++)
            {
                if (i != iLateLinkNum)
                {
                    LATE_LINK_STRUCT *pOtherLateLink = &m_LateLinks[i];

                    if (strcmp(pLateLink->funcName, pOtherLateLink->funcName) == 0)
                    {
                        switch (pOtherLateLink->eLateLinkType)
                        {
                        case LATELINK_FUNCBODY:
                            iNumFuncBodies++;
                            break;
                        }
                    }
                }
            }
        }
    }
}

int LateLinkManager::LateLinkComparator(const void *p1, const void *p2)
{
    int iRetVal = strcmp(((LATE_LINK_STRUCT*)p1)->funcName, ((LATE_LINK_STRUCT*)p2)->funcName);

    if (iRetVal)
    {
        return iRetVal;
    }
    
    iRetVal = strcmp(((LATE_LINK_STRUCT*)p1)->sourceFileName, ((LATE_LINK_STRUCT*)p2)->sourceFileName);

    if (iRetVal)
    {
        return iRetVal;
    }
    
    int iLine1 = ((LATE_LINK_STRUCT*)p1)->iSourceLineNum;
    int iLine2 = ((LATE_LINK_STRUCT*)p2)->iSourceLineNum;

    return (iLine1 < iLine2) ? -1 : ((iLine1 > iLine2) ? 1 : 0);
}


bool LateLinkManager::WriteOutData(void)
{
    if (!m_bSomethingChanged)
    {
        return false;
    }

    qsort(m_LateLinks, m_iNumLateLinks, sizeof(LATE_LINK_STRUCT), LateLinkComparator);


    VerifyPreWriteOut();

    FILE *pOutFile = fopen_nofail(m_LateLinkFileName, "wt");

    fprintf(pOutFile, "//This file contains data and prototypes for latelinks. It is autogenerated by StructParser"
"\n"
"\n"
"\n//autogenerated" "nocheckin"
"\n"
// ab: need to port this
// "\n#include \"LateLinkSupport.h\""
"\n"
"\n");

    
    int iLateLinkNum;
    int iArgNum;


    for (iLateLinkNum = 0; iLateLinkNum < m_iNumLateLinks; iLateLinkNum++)
    {
        LATE_LINK_STRUCT *pLateLink = &m_LateLinks[iLateLinkNum];


        switch(pLateLink->eLateLinkType)
        {
        case LATELINK_DECLARATION:

            char funcName[1024];
            sprintf(funcName, "LL_DECL_ARUN_%s_%s_%d_%s", GetFileNameWithoutDirectories(pLateLink->sourceFileName), m_pParent->GetShortProjectName(), pLateLink->iIndexInFile, pLateLink->funcName);
            MakeStringAllAlphaNum(funcName);

            fprintf(pOutFile, "\n\n//Stuff relating to LATELINK declaration for %s(), defined in %s\n", pLateLink->funcName, pLateLink->sourceFileName);
            fprintf(pOutFile, "extern char *%s_LATELINK_ARGSTRING;\n", pLateLink->funcName);
            fprintf(pOutFile, "void %s(void)\n{\n", funcName);
            fprintf(pOutFile, "\tTestLateLinkArgString(&%s_LATELINK_ARGSTRING, \"%s\", \"%s\");\n", pLateLink->funcName, pLateLink->fullTypeString, pLateLink->funcName);
            fprintf(pOutFile, "}\n\n\n");


            m_pParent->GetAutoRunManager()->AddAutoRunSpecial(funcName, pLateLink->sourceFileName, 0, AUTORUN_ORDER_INTERNAL);

            break;

        case LATELINK_EXTRA_PROTOTYPE:
            sprintf(funcName, "LL_PROT_ARUN_%s_%s_%d_%s", GetFileNameWithoutDirectories(pLateLink->sourceFileName), m_pParent->GetShortProjectName(), pLateLink->iIndexInFile, pLateLink->funcName);
            MakeStringAllAlphaNum(funcName);


            fprintf(pOutFile, "\n\n//Stuff relating to LATELINK declaration for %s(), defined in %s\n", pLateLink->funcName, pLateLink->sourceFileName);
            fprintf(pOutFile, "extern char *%s_LATELINK_ARGSTRING;\n", pLateLink->funcName);
            fprintf(pOutFile, "void %s(void)\n{\n", funcName);
            fprintf(pOutFile, "\tTestLateLinkArgString(&%s_LATELINK_ARGSTRING, \"%s\", \"%s\");\n", pLateLink->funcName, pLateLink->fullTypeString, pLateLink->funcName);
            fprintf(pOutFile, "}\n\n\n");


            m_pParent->GetAutoRunManager()->AddAutoRunSpecial(funcName, pLateLink->sourceFileName, 0, AUTORUN_ORDER_INTERNAL);

            break;


        case LATELINK_OVERRIDE:
            fprintf(pOutFile, "\n\n//Stuff relating to LATELINK_OVERRIDE_%s(), defined in %s\n", pLateLink->funcName, pLateLink->sourceFileName);
            fprintf(pOutFile, "extern %s OVERRIDE_LATELINK_%s(", pLateLink->retType, pLateLink->funcName);
            WriteInternalLateLinkArgList(pOutFile, pLateLink);
            fprintf(pOutFile, ");\ntypedef %s %s_LATELINK_FPTYPE_INTERNAL(", pLateLink->retType, pLateLink->funcName);
            WriteInternalLateLinkArgList(pOutFile, pLateLink);
            fprintf(pOutFile, ");\nextern %s_LATELINK_FPTYPE_INTERNAL *%s_LATELINK_FP;\n", pLateLink->funcName, pLateLink->funcName);
            fprintf(pOutFile, "extern char *%s_LATELINK_ARGSTRING;\n", pLateLink->funcName);
            fprintf(pOutFile, "void OVERRIDE_LATELINK_AUTORUN_%s(void)\n{\n", pLateLink->funcName);
            fprintf(pOutFile, "\t%s_LATELINK_FP = OVERRIDE_LATELINK_%s;\n", pLateLink->funcName, pLateLink->funcName);
            fprintf(pOutFile, "\tTestLateLinkArgString(&%s_LATELINK_ARGSTRING, \"%s\", \"%s\");\n", pLateLink->funcName, pLateLink->fullTypeString, pLateLink->funcName);
            fprintf(pOutFile, "}\n\n\n");

            sprintf(funcName, "OVERRIDE_LATELINK_AUTORUN_%s", pLateLink->funcName);

            m_pParent->GetAutoRunManager()->AddAutoRunSpecial(funcName, pLateLink->sourceFileName, 0, AUTORUN_ORDER_INTERNAL);

            break;

        case LATELINK_FUNCBODY:

            fprintf(pOutFile, "\n\n//Stuff relating to DEFAULT_LATELINK_%s(), defined in %s\n", pLateLink->funcName, pLateLink->sourceFileName);
            fprintf(pOutFile, "extern %s DEFAULT_LATELINK_%s(", pLateLink->retType, pLateLink->funcName);
            WriteInternalLateLinkArgList(pOutFile, pLateLink);
            fprintf(pOutFile, ");\ntypedef %s %s_LATELINK_FPTYPE_INTERNAL(", pLateLink->retType, pLateLink->funcName);
            WriteInternalLateLinkArgList(pOutFile, pLateLink);
            fprintf(pOutFile, ");\n%s_LATELINK_FPTYPE_INTERNAL *%s_LATELINK_FP = NULL;\n", pLateLink->funcName, pLateLink->funcName);
            fprintf(pOutFile, "char *%s_LATELINK_ARGSTRING = NULL;\n", pLateLink->funcName);
            fprintf(pOutFile, "void LATELINK_AUTORUN_%s(void)\n{\n", pLateLink->funcName);
            fprintf(pOutFile, "\t%s_LATELINK_FP = DEFAULT_LATELINK_%s;\n", pLateLink->funcName, pLateLink->funcName);
            fprintf(pOutFile, "\tTestLateLinkArgString(&%s_LATELINK_ARGSTRING, \"%s\", \"%s\");\n", pLateLink->funcName, pLateLink->fullTypeString, pLateLink->funcName);
            fprintf(pOutFile, "}\n");

            fprintf(pOutFile, "%s %s(", pLateLink->retType, pLateLink->funcName);

            if (pLateLink->argTypes[0][0] == 0)
            {
                fprintf(pOutFile, "void");
            }
            else
            {
                for(iArgNum = 0; iArgNum < MAX_LATELINK_ARGS && pLateLink->argTypes[iArgNum][0]; iArgNum++)
                {
                    fprintf(pOutFile, "%s%s arg%d", 
                        iArgNum == 0 ? "" : ", ",
                        pLateLink->argTypes[iArgNum],
                        iArgNum);
                }
            }

            fprintf(pOutFile, ")\n{\n\t%s%s_LATELINK_FP(",
                (strcmp(pLateLink->retType, "void") == 0) ? "" : "return ",
                pLateLink->funcName);

            for(iArgNum = 0; iArgNum < MAX_LATELINK_ARGS && pLateLink->argTypes[iArgNum][0]; iArgNum++)
            {
                fprintf(pOutFile, "%sarg%d", 
                    iArgNum == 0 ? "" : ", ",
                    iArgNum);
            }

            fprintf(pOutFile, ");\n}\n\n");

            sprintf(funcName, "LATELINK_AUTORUN_%s", pLateLink->funcName);

            m_pParent->GetAutoRunManager()->AddAutoRunSpecial(funcName, pLateLink->sourceFileName, 0, AUTORUN_ORDER_FIRST);

            break;
        }
    }


    fprintf(pOutFile, "\n\n\n#if 0\nPARSABLE\n");

    fprintf(pOutFile, "%d\n", m_iNumLateLinks);

    for (iLateLinkNum = 0; iLateLinkNum < m_iNumLateLinks; iLateLinkNum++)
    {
        LATE_LINK_STRUCT *pLateLink = &m_LateLinks[iLateLinkNum];
        
        fprintf(pOutFile, "%d \"%s\" \"%s\" ",
            pLateLink->eLateLinkType, pLateLink->funcName, pLateLink->retType);

        for(iArgNum = 0; iArgNum < MAX_LATELINK_ARGS; iArgNum++)
        {
            fprintf(pOutFile, " \"%s\" ", pLateLink->argTypes[iArgNum]);
        }

        fprintf(pOutFile, " \"%s\" %d %d \"%s\" \n", pLateLink->sourceFileName, pLateLink->iSourceLineNum, pLateLink->iIndexInFile, pLateLink->fullTypeString);
    }



    fprintf(pOutFile, "#endif\n");

    fclose(pOutFile);

    return true;
}
    






void LateLinkManager::FoundMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer, int iWhichMagicWord, char const* pMagicWordString)
{
    Tokenizer tokenizer;

    Token token;
    enumTokenType eType;
    Token token2;
    enumTokenType eType2;
    char *pFuncName;

    static int iFileIndex = 0;

    if (iWhichMagicWord ==  MAGICWORD_BEGINNING_OF_FILE || iWhichMagicWord == MAGICWORD_END_OF_FILE)
    {
        iFileIndex = 0;
        return;
    }

    //inside a function body, we must be calling the DEFAULT_LATELINK_, not defining or declaring it
    if (iWhichMagicWord == LATELINK_FUNCBODY && pTokenizer->GetCurBraceDepth() > 0)
    {
        return;
    }


    pTokenizer->Assert(m_iNumLateLinks < MAX_LATELINKS, "Too many latelinks");

    LATE_LINK_STRUCT *pLateLink = &m_LateLinks[m_iNumLateLinks++];

    m_bSomethingChanged = true;

    memset(pLateLink, 0, sizeof(LATE_LINK_STRUCT));

    strcpy(pLateLink->sourceFileName, pSourceFileName);
    pLateLink->iSourceLineNum = pTokenizer->GetCurLineNum();
    pLateLink->iIndexInFile = iFileIndex++;

    pLateLink->eLateLinkType = (enumLateLinkType) iWhichMagicWord;

    if (pLateLink->eLateLinkType  == LATELINK_DECLARATION)
    {
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_SEMICOLON, "Didn't find ; after LATELINK");

        GetFixedUpType(pTokenizer, pLateLink->retType, pLateLink->fullTypeString);

        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, LATELINK_NAME_LENGTH, "Didn't find LATELINK declaration function name");

        strcpy(pLateLink->funcName, token.sVal);
    }
    else
    {

        pTokenizer->BackUpToBeginningOfLine();

        GetFixedUpType(pTokenizer, pLateLink->retType, pLateLink->fullTypeString);

        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 0, "Didn't find function name after return type");

        pFuncName = strstr(token.sVal, "LATELINK_");

        pTokenizer->Assert(pFuncName != NULL, "mysteriously didn't find LATELINK_");
        pFuncName += 9;

        pTokenizer->Assert(strlen(pFuncName) < LATELINK_NAME_LENGTH, "LATELINK function name overflow");
        strcpy(pLateLink->funcName, pFuncName);
    }

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Didn't find ( after LATELINK_x");

    //check if we have a void func
    pTokenizer->SaveLocation();

    eType = pTokenizer->GetNextToken(&token);    
    eType2 = pTokenizer->GetNextToken(&token2);

    if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_VOID && eType2 == TOKEN_RESERVEDWORD && token2.iVal == RW_RIGHTPARENS)
    {
        //void func, no args
    }
    else
    {
        pTokenizer->RestoreLocation();

        int iNumArgs = 0;

        do
        {
            pTokenizer->Assert(iNumArgs < MAX_LATELINK_ARGS, "Too many args to a LATELINK func");

            eType = pTokenizer->CheckNextToken(&token);

            GetFixedUpType(pTokenizer, pLateLink->argTypes[iNumArgs], pLateLink->fullTypeString);

            iNumArgs++;

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 0, "Expected arg name after arg type");

            pTokenizer->Assert2NextTokenTypesAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected , or ) after arg");
        } while (token.iVal == RW_COMMA);
    }

    eType = pTokenizer->CheckNextToken(&token);

    if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_SEMICOLON)
    {
        switch (pLateLink->eLateLinkType)
        {
        case LATELINK_FUNCBODY:
            pLateLink->eLateLinkType = LATELINK_EXTRA_PROTOTYPE;
            break;

        case LATELINK_OVERRIDE:
            pTokenizer->Assert(0, "Can't have prototypes of OVERRIDE_LATELINK");
            break;
        }

        pTokenizer->GetNextToken(&token);
    }
}



//returns number of dependencies found
int LateLinkManager::ProcessDataSingleFile(char const* pSourceFileName, char* pDependencies[MAX_DEPENDENCIES_SINGLE_FILE])
{

    return 0;
}

#define MAX_ASTERISKS 10

void LateLinkManager::GetFixedUpType(Tokenizer *pTokenizer, char *pDestString, char* pFullTypeString)
{

    Token token;
    enumTokenType eType;

    int iNumAsterisks = 0;
    char baseName[LATELINK_NAME_LENGTH];
    char asteriskString[MAX_ASTERISKS + 1];
    bool bFoundNoConst = false;

    eType = pTokenizer->CheckNextToken(&token);

    if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "const") == 0)
    {
        pTokenizer->GetNextToken(&token);
        sprintf(pFullTypeString + strlen(pFullTypeString), "const ");
    }

    if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "NOCONST") == 0)
    {
        pTokenizer->GetNextToken(&token);
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after NOCONST");
        sprintf(pFullTypeString + strlen(pFullTypeString), "NOCONST(");
        bFoundNoConst = true;
    }


    pTokenizer->Assert2NextTokenTypesAndGet(&token,TOKEN_RESERVEDWORD, RW_VOID, TOKEN_IDENTIFIER, LATELINK_NAME_LENGTH - MAX_ASTERISKS, "Expected type name");
    pTokenizer->StringifyToken(&token);
    strcpy(baseName, token.sVal);

    sprintf(pFullTypeString + strlen(pFullTypeString), "%s ", baseName);

    if (bFoundNoConst)
    {
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after NOCONST(x");
        sprintf(pFullTypeString + strlen(pFullTypeString), ") ");
    }


    do
    {
        eType = pTokenizer->CheckNextToken(&token);

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_ASTERISK)
        {
            pTokenizer->Assert(iNumAsterisks < MAX_ASTERISKS, "Too many asterisks... wtf?");
            iNumAsterisks++;
            pTokenizer->GetNextToken(&token);

            sprintf(pFullTypeString + strlen(pFullTypeString), "* ");
        }
        else
        {
            break;
        }
    } while (1);

    if (iNumAsterisks)
    {
        sprintf(baseName, "void");
    }

    MakeRepeatedCharacterString(asteriskString, iNumAsterisks, MAX_ASTERISKS, '*');

    sprintf(pDestString, "%s%s", baseName, asteriskString);
}












