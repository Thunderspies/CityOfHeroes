#include "pch.h"
#include "AutoTestManager.h"
#include "strutils.h"
#include "sourceparser.h"
#include "autorunmanager.h"


typedef enum
{
    MAGICWORD_AUTO_TEST,
    MAGICWORD_AUTO_TEST_CHILD,
    MAGICWORD_AUTO_TEST_GROUP,
    MAGICWORD_AUTO_TEST_BLOCK,
} enumAutoTestMagicWord;

AutoTestManager::AutoTestManager()
{
    m_bSomethingChanged = false;
    m_iNumAutoTests = 0;
    m_AutoTestFileName[0] = 0;
    m_pMostRecentGroup = NULL;
}

AutoTestManager::~AutoTestManager()
{
    int iAutoTestNum;

    for (iAutoTestNum = 0; iAutoTestNum < m_iNumAutoTests; iAutoTestNum++)
    {
        AUTO_TEST_STRUCT *pAutoTest = &m_AutoTests[iAutoTestNum];

        if (pAutoTest->pAutoAssertString)
        {
            delete pAutoTest->pAutoAssertString;
        }
    }
}

char const* AutoTestManager::GetMagicWord(int iWhichMagicWord)
{
    switch (iWhichMagicWord)
    {
    case MAGICWORD_AUTO_TEST:
        return "AUTO_TEST";
    case MAGICWORD_AUTO_TEST_CHILD:
        return "AUTO_TEST_CHILD";
    case MAGICWORD_AUTO_TEST_GROUP:
        return "AUTO_TEST_GROUP";
    case MAGICWORD_AUTO_TEST_BLOCK:
        return "AUTO_TEST_BLOCK";
    }

    return "x x";
}


enum
{
    RW_PARSABLE = RW_COUNT,
};

static char const* sAutoTestReservedWords[] =
{
    "PARSABLE",
    NULL
};

void AutoTestManager::SetProjectPathAndName(char const* srcPath, char const* commonPath, char const* projectName)
{
    strcpy(m_ProjectName, projectName);
    sprintf(m_AutoTestFileName, "%s\\AutoGen\\%s_AutoTest.c", srcPath, projectName);

}

bool AutoTestManager::DoesFileNeedUpdating(char const* pFileName)
{
    HANDLE hFile;

    char atestFileName[MAX_PATH];

    GetAtestFileName(atestFileName, pFileName);

    hFile = CreateFileA(atestFileName, GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return true;
    }
    
    CloseHandle(hFile);
    
    return false;
}

bool AutoTestManager::LoadStoredData(bool bForceReset)
{
    if (bForceReset)
    {
        m_bSomethingChanged = true;
        return false;
    }

    Tokenizer tokenizer;

    if (!tokenizer.LoadFromFile(m_AutoTestFileName))
    {
        m_bSomethingChanged = true;
        return false;
    }

    if (!tokenizer.IsStringAtVeryEndOfBuffer("#endif"))
    {
        m_bSomethingChanged = true;
        return false;
    }

    tokenizer.SetExtraReservedWords(sAutoTestReservedWords);

    tokenizer.SetCSourceStyleStrings(true);

    Token token;
    enumTokenType eType;

    do
    {
        eType = tokenizer.GetNextToken(&token);
        Tokenizer::StaticAssert(eType != TOKEN_NONE, "AutoTest data corruption");
    } while (!(eType == TOKEN_RESERVEDWORD && token.iVal == RW_PARSABLE));

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find number of autoTests");

    m_iNumAutoTests = token.iVal;

    int iAutoTestNum;

    for (iAutoTestNum = 0; iAutoTestNum < m_iNumAutoTests; iAutoTestNum++)
    {
        AUTO_TEST_STRUCT *pAutoTest = &m_AutoTests[iAutoTestNum];

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_AUTOTEST_COMMAND_LENGTH, "Didn't find AutoTest group name");
        strcpy(pAutoTest->groupName, token.sVal);
    
        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_AUTOTEST_COMMAND_LENGTH, "Didn't find AutoTest function name");
        strcpy(pAutoTest->functionName, token.sVal);
    
        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_AUTOTEST_COMMAND_LENGTH, "Didn't find AutoTest parent name");
        strcpy(pAutoTest->parentName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_AUTOTEST_COMMAND_LENGTH, "Didn't find AutoTest setup name");
        strcpy(pAutoTest->setupName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_AUTOTEST_COMMAND_LENGTH, "Didn't find AutoTest teardown name");
        strcpy(pAutoTest->teardownName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_PATH, "Didn't find AutoTest source file");
        strcpy(pAutoTest->sourceFileName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find AutoTest source file line num");
        pAutoTest->iSourceLineNum = token.iVal;

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Didn't find AutoTest assert string");
        if (token.iVal)
        {
            pAutoTest->pAutoAssertString = new char[token.iVal + 1];
            strcpy(pAutoTest->pAutoAssertString, token.sVal);
        }
        else
        {
            pAutoTest->pAutoAssertString = NULL;
        }


    }

    m_bSomethingChanged = false;

    return true;
}

void AutoTestManager::GetAtestFileName(char outName[MAX_PATH], char const* pInName)
{
    char workName[MAX_PATH];
    strcpy(workName, GetFileNameWithoutDirectories(pInName));
    int iLen = (int)strlen(workName);

    int i;

    for (i=0; i < iLen; i++)
    {
        if (workName[i] == '.')
        {
            workName[i] = '_';
        }
    }

    sprintf(outName, "%s\\AutoGen%s_atest.c", m_pParent->GetSoureDir(), workName);


}

void AutoTestManager::ResetSourceFile(char const* pSourceFileName)
{
    int i = 0;


    while (i < m_iNumAutoTests)
    {
        if (AreFilenamesEqual(m_AutoTests[i].sourceFileName, pSourceFileName))
        {
            memcpy(&m_AutoTests[i], &m_AutoTests[m_iNumAutoTests - 1], sizeof(AUTO_TEST_STRUCT));
            m_iNumAutoTests--;

            m_bSomethingChanged = true;
        }
        else
        {
            i++;
        }
    }

    char fileName[MAX_PATH];
    GetAtestFileName(fileName, pSourceFileName);

    DeleteFileA(fileName);
}

void AutoTestManager::RecurseOutputTestRegister(FILE *pOutFile, AUTO_TEST_STRUCT *pTest, int iRecurseDepth)
{
    char tabString[32];

    if (iRecurseDepth > 32)
    {
        char errorString[1024];
        sprintf(errorString, "Test case parenting recursion overflow: case %s\n", pTest->groupName);
        Tokenizer::StaticAssert(0, errorString);
    }

    MakeRepeatedCharacterString(tabString, iRecurseDepth + 1, 31, '\t');

    char parentNameString[MAX_AUTOTEST_COMMAND_LENGTH + 2];

    if (pTest->parentName[0])
    {
        sprintf(parentNameString, "\"%s\"", pTest->parentName);
    }
    else
    {
        sprintf(parentNameString, "NULL");
    }

    fprintf(pOutFile, "%stestRegister(\"%s\", %s, %s, %s, %s);\n",
        tabString,
        pTest->groupName,
        parentNameString,
        pTest->functionName[0] ? pTest->functionName : "NULL",
        pTest->setupName[0] ? pTest->setupName : "NULL",
        pTest->teardownName[0] ? pTest->teardownName : "NULL");

    int iAutoTestNum;

    for (iAutoTestNum = 0; iAutoTestNum < m_iNumAutoTests; iAutoTestNum++)
    {
        AUTO_TEST_STRUCT *pChildTest = &m_AutoTests[iAutoTestNum];

        if (strcmp(pChildTest->parentName, pTest->groupName) == 0)
        {
            RecurseOutputTestRegister(pOutFile, pChildTest, iRecurseDepth + 1);
        }
    }
}



int AutoTestManager::AutoTestComparator(const void *p1, const void *p2)
{
    return strcmp(((AUTO_TEST_STRUCT*)p1)->functionName, ((AUTO_TEST_STRUCT*)p2)->functionName);
}

bool AutoTestManager::WriteOutData(void)
{
    if (!m_bSomethingChanged)
    {
        return false;
    }

    qsort(m_AutoTests, m_iNumAutoTests, sizeof(AUTO_TEST_STRUCT), AutoTestComparator);

    DoPreWriteOutFixup();

    FILE *pOutFile = fopen_nofail(m_AutoTestFileName, "wt");

    fprintf(pOutFile, "//This file contains data and prototypes for AutoTests. It is autogenerated by StructParser"
"\n\n"
"\n// autogenerated" "nocheckin"
// "\n\n#include \"testharness.h\""
"\n\n");

    int iAutoTestNum;

    for (iAutoTestNum = 0; iAutoTestNum < m_iNumAutoTests; iAutoTestNum++)
    {
        AUTO_TEST_STRUCT *pAutoTest = &m_AutoTests[iAutoTestNum];

        if (pAutoTest->functionName[0])
        {
            fprintf(pOutFile, "extern void %s(void);\n", pAutoTest->functionName);
        }
        if (pAutoTest->setupName[0])
        {
            fprintf(pOutFile, "extern void %s(void);\n", pAutoTest->setupName);
        }
        if (pAutoTest->teardownName[0])
        {
            fprintf(pOutFile, "extern void %s(void);\n", pAutoTest->teardownName);
        }
    }

    m_pParent->GetAutoRunManager()->ResetSourceFile("autogen_autotests");

    if (m_iNumAutoTests)
    {
        char autoFuncName[256];

        sprintf(autoFuncName, "AutoGen_%s_RegisterAutoTests", m_ProjectName);
        fprintf(pOutFile, "\n\nvoid %s(void)\n{\n",
            autoFuncName);

        for (iAutoTestNum = 0; iAutoTestNum < m_iNumAutoTests; iAutoTestNum++)
        {
            AUTO_TEST_STRUCT *pAutoTest = &m_AutoTests[iAutoTestNum];

            if (pAutoTest->parentName[0] == 0)
            {
                RecurseOutputTestRegister(pOutFile, pAutoTest, 0);
            }
        }

        fprintf(pOutFile, "};\n\n");

    
        m_pParent->GetAutoRunManager()->AddAutoRun(autoFuncName, "autogen_autotests");
    }



    fprintf(pOutFile, "\n\n\n#if 0\nPARSABLE\n");

    fprintf(pOutFile, "%d\n", m_iNumAutoTests);

    for (iAutoTestNum = 0; iAutoTestNum < m_iNumAutoTests; iAutoTestNum++)
    {
        AUTO_TEST_STRUCT *pAutoTest = &m_AutoTests[iAutoTestNum];
        
        fprintf(pOutFile, "\"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" %d \"%s\" \n", pAutoTest->groupName, pAutoTest->functionName, 
            pAutoTest->parentName, pAutoTest->setupName, pAutoTest->teardownName, pAutoTest->sourceFileName, pAutoTest->iSourceLineNum,
            pAutoTest->pAutoAssertString ? pAutoTest->pAutoAssertString : "");
    }

    fprintf(pOutFile, "#endif\n");

    fclose(pOutFile);


    int iNumFileNames = 0;
    char fileNames[MAX_FILES_IN_PROJECT][MAX_PATH];

    int i, j;
    


    for (iAutoTestNum = 0; iAutoTestNum < m_iNumAutoTests; iAutoTestNum++)
    {
        AUTO_TEST_STRUCT *pAutoTest = &m_AutoTests[iAutoTestNum];
        
        if (pAutoTest->pAutoAssertString)
        {
            bool bIsUnique = true;

            for (j = 0; j < iNumFileNames; j++)
            {
                if (strcmp(pAutoTest->sourceFileName, fileNames[j]) == 0)
                {
                    bIsUnique = false;
                    break;
                }
            }

            if (bIsUnique)
            {
                strcpy(fileNames[iNumFileNames++], pAutoTest->sourceFileName);
            }
        }
    }

    

    for (i=0; i < iNumFileNames; i++)
    {
        m_pParent->SetExtraDataFlagForFile(fileNames[i], 1 << m_iIndexInParent);
        CreateAtestFile(fileNames[i]);
    }

    return true;

}
    





void AutoTestManager::FoundMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer, int iWhichMagicWord, char const* pMagicWordString)
{


    if (iWhichMagicWord ==  MAGICWORD_BEGINNING_OF_FILE || iWhichMagicWord == MAGICWORD_END_OF_FILE)
    {
        m_pMostRecentGroup = NULL;
        return;
    }
    
    switch (iWhichMagicWord)
    {
    case MAGICWORD_AUTO_TEST:
        FoundMagicWordAutoTest(pSourceFileName, pTokenizer);
        break;

    case MAGICWORD_AUTO_TEST_CHILD:
        FoundMagicWordAutoTestChild(pSourceFileName, pTokenizer);
        break;

    case MAGICWORD_AUTO_TEST_GROUP:
        FoundMagicWordAutoTestGroup(pSourceFileName, pTokenizer);
        break;

    case MAGICWORD_AUTO_TEST_BLOCK:
        FoundMagicWordAutoTestBlock(pSourceFileName, pTokenizer);
        break;
    }
}


void AutoTestManager::FoundMagicWordAutoTest(char const* pSourceFileName, Tokenizer *pTokenizer)
{
    Token token;
    enumTokenType eType;

    pTokenizer->Assert(m_iNumAutoTests < MAX_AUTOTESTS, "Too many AUTO_TESTs");

    AUTO_TEST_STRUCT *pAutoTest = &m_AutoTests[m_iNumAutoTests++];

    memset(pAutoTest, 0, sizeof(AUTO_TEST_STRUCT));

    strcpy(pAutoTest->sourceFileName, pSourceFileName);
    pAutoTest->iSourceLineNum = pTokenizer->GetCurLineNum();

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AUTO_TEST");

    eType = pTokenizer->GetNextToken(&token);

    if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTPARENS)
    {
        //AUTOTEST()... do nothing

    }
    else
    {
        pTokenizer->Assert(eType == TOKEN_IDENTIFIER && strlen(token.sVal) < MAX_AUTOTEST_COMMAND_LENGTH, "Expected setup func name after AUTOTEST(");
        strcpy(pAutoTest->setupName, token.sVal);
    
        if (strcmp(pAutoTest->setupName, "NULL") == 0)
        {
            pAutoTest->setupName[0] = 0;
        }

        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after AUTO_TEST(x");

        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_AUTOTEST_COMMAND_LENGTH, "Expected teardown func name");

        strcpy(pAutoTest->teardownName, token.sVal);
    
        if (strcmp(pAutoTest->teardownName, "NULL") == 0)
        {
            pAutoTest->teardownName[0] = 0;
        }

        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AUTO_TEST(x, x");

    }

    pAutoTest->parentName[0] = 0;

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_SEMICOLON, "Expected ; after AUTOTEST()");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_VOID, "Expected void after AUTOTEST();");

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_AUTOTEST_COMMAND_LENGTH, "Expected autotest func name");

    strcpy(pAutoTest->functionName, token.sVal);
    strcpy(pAutoTest->groupName, token.sVal);

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected (void) after autotest func name");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_VOID, "Expected (void) after autotest func name");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected (void) after autotest func name");

    m_bSomethingChanged = true;
    
}

void AutoTestManager::FoundMagicWordAutoTestChild(char const* pSourceFileName, Tokenizer *pTokenizer)
{
    Token token;
    enumTokenType eType;

    pTokenizer->Assert(m_iNumAutoTests < MAX_AUTOTESTS, "Too many AUTO_TESTs");

    AUTO_TEST_STRUCT *pAutoTest = &m_AutoTests[m_iNumAutoTests++];

    memset(pAutoTest, 0, sizeof(AUTO_TEST_STRUCT));

    strcpy(pAutoTest->sourceFileName, pSourceFileName);
    pAutoTest->iSourceLineNum = pTokenizer->GetCurLineNum();

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AUTO_TEST_CHILD");

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_AUTOTEST_COMMAND_LENGTH, "Expected parent name after AUTO_TEST_CHILD");
    strcpy(pAutoTest->parentName, token.sVal);


    eType = pTokenizer->GetNextToken(&token);

    if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTPARENS)
    {
        //AUTOTEST()... do nothing

    }
    else
    {
        pTokenizer->Assert(eType == TOKEN_IDENTIFIER && strlen(token.sVal) < MAX_AUTOTEST_COMMAND_LENGTH, "Expected setup func name after AUTO_TEST_CHILD(");
        strcpy(pAutoTest->setupName, token.sVal);
    
        if (strcmp(pAutoTest->setupName, "NULL") == 0)
        {
            pAutoTest->setupName[0] = 0;
        }

        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after AUTO_TEST_CHILD(x,x");

        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_AUTOTEST_COMMAND_LENGTH, "Expected teardown func name");

        strcpy(pAutoTest->teardownName, token.sVal);
    
        if (strcmp(pAutoTest->teardownName, "NULL") == 0)
        {
            pAutoTest->teardownName[0] = 0;
        }

        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AUTO_TEST_CHILD(x, x, x");

    }

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_SEMICOLON, "Expected ; after AUTO_TEST_CHILD()");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_VOID, "Expected void after AUTO_TEST_CHILD();");

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_AUTOTEST_COMMAND_LENGTH, "Expected autotest func name");

    strcpy(pAutoTest->functionName, token.sVal);
    strcpy(pAutoTest->groupName, token.sVal);

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected (void) after autotest func name");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_VOID, "Expected (void) after autotest func name");
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected (void) after autotest func name");

    m_bSomethingChanged = true;
    
}

void AutoTestManager::FoundMagicWordAutoTestGroup(char const* pSourceFileName, Tokenizer *pTokenizer)
{
    Token token;
    enumTokenType eType;

    pTokenizer->Assert(m_iNumAutoTests < MAX_AUTOTESTS, "Too many AUTO_TESTs");

    AUTO_TEST_STRUCT *pAutoTest = &m_AutoTests[m_iNumAutoTests++];

    memset(pAutoTest, 0, sizeof(AUTO_TEST_STRUCT));

    strcpy(pAutoTest->sourceFileName, pSourceFileName);
    pAutoTest->iSourceLineNum = pTokenizer->GetCurLineNum();

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AUTO_TEST_GROUP");

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_AUTOTEST_COMMAND_LENGTH, "Expected group name after AUTO_TEST_GROUP");
    strcpy(pAutoTest->groupName, token.sVal);

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after AUTO_TEST_GROUP(x");
    
    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_AUTOTEST_COMMAND_LENGTH, "Expected parent name after AUTO_TEST_GROUP(x,");
    if (strcmp(token.sVal, "NULL") != 0)
    {
        strcpy(pAutoTest->parentName, token.sVal);
    }


    eType = pTokenizer->GetNextToken(&token);

    if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTPARENS)
    {
        //AUTOTEST()... do nothing

    }
    else
    {
        pTokenizer->Assert(eType == TOKEN_IDENTIFIER && strlen(token.sVal) < MAX_AUTOTEST_COMMAND_LENGTH, "Expected setup func name after AUTO_TEST_CHILD(");
        strcpy(pAutoTest->setupName, token.sVal);
    
        if (strcmp(pAutoTest->setupName, "NULL") == 0)
        {
            pAutoTest->setupName[0] = 0;
        }

        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after AUTO_TEST_CHILD(x,x");

        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_AUTOTEST_COMMAND_LENGTH, "Expected teardown func name");

        strcpy(pAutoTest->teardownName, token.sVal);
    
        if (strcmp(pAutoTest->teardownName, "NULL") == 0)
        {
            pAutoTest->teardownName[0] = 0;
        }

        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AUTO_TEST_CHILD(x, x, x");

    }

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_SEMICOLON, "Expected ; after AUTO_TEST_CHILD()");
    
    m_bSomethingChanged = true;

    m_pMostRecentGroup = pAutoTest;

}

#define AUTO_TEST_BLOCK_ASSERT "ASSERT"

void AutoTestManager::FoundMagicWordAutoTestBlock(char const* pSourceFileName, Tokenizer *pTokenizer)
{
    Token token;
    enumTokenType eType;

    char parentName[MAX_AUTOTEST_COMMAND_LENGTH] = "";

    if (m_pMostRecentGroup)
    {
        strcpy(parentName, m_pMostRecentGroup->groupName);
    }

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AUTO_TEST_BLOCk");

    eType = pTokenizer->CheckNextToken(&token);

    if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, AUTO_TEST_BLOCK_ASSERT) != 0)
    {
        strcpy(parentName, token.sVal);

        pTokenizer->GetNextToken(&token);
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after AUTO_TEST_BLOCK(x");
    }

    pTokenizer->Assert(parentName[0] != 0, "No parent for AUTO_TEST_BLOCK... require previous AUTO_TEST_GROUP or specified parent name");

    do
    {
        eType = pTokenizer->GetNextToken(&token);

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTPARENS)
        {
            break;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, AUTO_TEST_BLOCK_ASSERT) == 0)
        {
            pTokenizer->Assert(m_iNumAutoTests < MAX_AUTOTESTS, "Too many AUTOTESTs");
            AUTO_TEST_STRUCT *pAutoTest = &m_AutoTests[m_iNumAutoTests++];
            memset(pAutoTest, 0, sizeof(AUTO_TEST_STRUCT));

            strcpy(pAutoTest->parentName, parentName);
            strcpy(pAutoTest->sourceFileName, pSourceFileName);
            pAutoTest->iSourceLineNum = pTokenizer->GetCurLineNum();

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ASSERT");

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_AUTOTEST_COMMAND_LENGTH, "Expected command name after ASSERT(");

            //insert a hash which we will replace later with full list of group names
            sprintf(pAutoTest->groupName, "#%s", token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after ASSERT(x,");

            pTokenizer->GetSpecialStringTokenWithParenthesesMatching(&token);

            pAutoTest->pAutoAssertString = new char[token.iVal * 2 + 2];
            AddCStyleEscaping(pAutoTest->pAutoAssertString, token.sVal, token.iVal * 2 + 1);
            
            pTokenizer->Assert(strlen(pAutoTest->pAutoAssertString) < TOKENIZER_MAX_STRING_LENGTH, "AUTO_TEST_BLOCK expression length overflow inside ASSERT");

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_SEMICOLON, "Expected ; after ASSERT(x)");

        }
        else
        {
            pTokenizer->Assert(0, "Expected ) or ASSERT inside AUTO_TEST_BLOCK, found something unknown");
        }
    }
    while (1);

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_SEMICOLON, "Expected ; after AUTO_TEST_BLOCK()");
}



void AutoTestManager::PrependParentGroupNames(AUTO_TEST_STRUCT *pAutoTest)
{
    char finalName[MAX_AUTOTEST_COMMAND_LENGTH];

    strcpy(finalName, &pAutoTest->groupName[1]);

    AUTO_TEST_STRUCT *pParent = FindTestByName(pAutoTest->parentName);

    while (pParent)
    {
        char workName[MAX_AUTOTEST_COMMAND_LENGTH * 3];

        sprintf(workName, "%s_%s", pParent->groupName, finalName);

        Tokenizer::StaticAssertf(strlen(workName) < MAX_AUTOTEST_COMMAND_LENGTH, "Name length overflow: %s", workName);

        strcpy(finalName, workName);

        pParent = FindTestByName(pParent->parentName);
    }

    strcpy(pAutoTest->groupName, finalName);
}
    


void AutoTestManager::DoPreWriteOutFixup(void)
{
    int iAutoTestNum;

    for (iAutoTestNum = 0; iAutoTestNum < m_iNumAutoTests; iAutoTestNum++)
    {
        AUTO_TEST_STRUCT *pAutoTest = &m_AutoTests[iAutoTestNum];

        if (pAutoTest->groupName[0] == '#')
        {
            PrependParentGroupNames(pAutoTest);

            strcpy(pAutoTest->functionName, pAutoTest->groupName);
        }

    }
}

AutoTestManager::AUTO_TEST_STRUCT *AutoTestManager::FindTestByName(char const* pName)
{
    int iAutoTestNum;

    if (!pName || pName[0] == 0)
    {
        return NULL;
    }

    for (iAutoTestNum = 0; iAutoTestNum < m_iNumAutoTests; iAutoTestNum++)
    {
        AUTO_TEST_STRUCT *pAutoTest = &m_AutoTests[iAutoTestNum];
        
        if (strcmp(pAutoTest->groupName, pName) == 0)
        {
            return pAutoTest;
        }
    }

    Tokenizer::StaticAssertf(0, "Couldn't find AUTO_TEST or group named %s", pName);
    return NULL;
}


void AutoTestManager::CreateAtestFile(char const* pFileName)
{
    char AtestFileName[MAX_PATH];
    GetAtestFileName(AtestFileName, pFileName);

    FILE *pFile = fopen_nofail(AtestFileName, "wt");

    fprintf(pFile, "//This file contains functions auto-generated for AUTO_TESTing based on AUTO_TEST_BLOCKs\n//found in %s\n//autogenerated" "nocheckin\n\n",
        pFileName);

    int iAutoTestNum;
    for (iAutoTestNum = 0; iAutoTestNum < m_iNumAutoTests; iAutoTestNum++)
    {
        AUTO_TEST_STRUCT *pAutoTest = &m_AutoTests[iAutoTestNum];

        if (strcmp(pAutoTest->sourceFileName, pFileName) == 0 && pAutoTest->pAutoAssertString)
        {
            char workString[TOKENIZER_MAX_STRING_LENGTH];
            char fileNameString[MAX_PATH * 2];

            AddCStyleEscaping(fileNameString, pAutoTest->sourceFileName, MAX_PATH * 2 - 1);
            RemoveCStyleEscaping(workString, pAutoTest->pAutoAssertString);

            fprintf(pFile, "void %s(void)\n{\n\ttestAssert_FileLine(%s, \"%s\", %d);\n}\n\n",
                pAutoTest->functionName, workString, fileNameString, pAutoTest->iSourceLineNum);
        }
    }

    fclose(pFile);


}


//returns number of dependencies found
int AutoTestManager::ProcessDataSingleFile(char const* pSourceFileName, char* pDependencies[MAX_DEPENDENCIES_SINGLE_FILE])
{
    return 0;
}
