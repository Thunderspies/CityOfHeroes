#include "pch.h"
#include "SourceParser.h"
#include "strutils.h"

#include "MagicCommandManager.h"
#include "structparser.h"
#include "AutoTransactionManager.h"
#include "AutoRunManager.h"
#include "Platform.h"

#include "AutoTestManager.h"
#include "LateLinkManager.h"


namespace fs = std::filesystem;

#define MAX_WILDCARD_MAGIC_WORDS 16


//must be all caps
static char const* sFileNamesToExclude[] =
{
    "STDTYPES.H",
    NULL
};

static char const* sProjectNamesToExclude[] =
{
    "GimmeDLL",
    NULL
};

bool ShouldFileBeExcluded(char const* pFileName)
{
    char tempFileName[MAX_PATH];
    strcpy(tempFileName, pFileName);

    if (strstr(pFileName, "Program Files"))
    {
        return true;
    }

    char *pTemp;
    char *pSimpleFileName = pTemp = tempFileName;

    while (*pTemp)
    {
        if ((*pTemp == '/' || *pTemp == '\\') && *(pTemp + 1))
        {
            pSimpleFileName = pTemp + 1;
        }

        pTemp++;
    }

    MakeStringUpcase(pSimpleFileName);

    int i = 0;

    while (sFileNamesToExclude[i])
    {
        if (AreFilenamesEqual(pSimpleFileName, sFileNamesToExclude[i]))
        {
            return true;
        }


        i++;
    }

    if (strstr(pSimpleFileName, "AUTOGEN"))
    {
        return true;
    }

    return false;
}

SourceParser::SourceParser()
{
    m_iNumSourceParsers = 6;

    m_pSourceParsers[0] = NULL;
    m_pSourceParsers[1] = NULL;
    m_pSourceParsers[2] = NULL;
    m_pSourceParsers[3] = NULL;
    m_pSourceParsers[4] = NULL;
    m_pSourceParsers[5] = m_pAutoRunManager = NULL;

    m_iNumProjectFiles = 0;


    m_bIsAnExecutable = false;

    m_pFirstVar = NULL;

}

void SourceParser::CreateParsers(void)
{
    m_pSourceParsers[0] = new MagicCommandManager;
    m_pSourceParsers[1] = new StructParser;
    m_pSourceParsers[2] = new AutoTransactionManager;
    m_pSourceParsers[3] = new AutoTestManager;
    m_pSourceParsers[4] = new LateLinkManager;

    //AutoRunManager should generally be last
    m_pSourceParsers[5] = m_pAutoRunManager = new AutoRunManager;
}

SourceParser::~SourceParser()
{
    int i;

    for (i=0; i < m_iNumSourceParsers; i++)
    {
        if (m_pSourceParsers[i])
        {
            delete m_pSourceParsers[i];
        }
    }

    while (m_pFirstVar)
    {
        SourceParserVar *pNext = m_pFirstVar->pNext;
        delete[] m_pFirstVar->pVarName;
        delete[] m_pFirstVar->pValue;
        delete m_pFirstVar;
        m_pFirstVar = pNext;
    }

}


void SourceParser::AddProjectFiles(std::vector<fs::path> const& files)
{
	for (auto const& file : files) {
		auto name = file.string();
		Tokenizer::StaticAssert(name.size() < MAX_PATH,
			"Source path is too long");

		auto extension = file.extension().string();
		if (CompareNoCase(extension.c_str(), ".h") != 0 &&
		    CompareNoCase(extension.c_str(), ".c") != 0)
			continue;
		if (ShouldFileBeExcluded(name.c_str()))
			continue;
		if (FindProjectFileIndex(name.data()) != -1)
			continue;

		Tokenizer::StaticAssert(
			m_iNumProjectFiles < MAX_FILES_IN_PROJECT,
			"Too many files in project");
		strcpy(m_ProjectFiles[m_iNumProjectFiles++], name.c_str());
	}
}

void SourceParser::MakeAutoGenDirectory()
{
    fs::path directory{ m_srcDir };
    directory.append("AutoGen");
    fs::create_directories(directory);

    directory = m_commonDir;
    directory.append("AutoGen");
    fs::create_directories(directory);
}


int SourceParser::ParseSource(
	std::string const& targetName,
	fs::path const& srcDir,
	fs::path const& commonDir,
	bool isExecutable,
	std::vector<fs::path> const& sourceFiles)
{
	m_srcDir = srcDir.generic_string();
	m_commonDir = commonDir.generic_string();
	m_shortenedPrjFileName = targetName;
	m_bIsAnExecutable = isExecutable;
	AddProjectFiles(sourceFiles);
	MakeAutoGenDirectory();
	CreateParsers();
	for (int i = 0; i < m_iNumSourceParsers; ++i) {
		m_pSourceParsers[i]->SetParent(this);
		m_pSourceParsers[i]->SetProjectPathAndName(m_srcDir.c_str(),
			m_commonDir.c_str(), targetName.c_str());
	}
	if (MakeSpecialAutoRunFunction()) {
		auto name = "_" + targetName + "_AutoRun_SPECIALINTERNAL";
		GetAutoRunManager()->AddAutoRunSpecial(name.data(),
			"_SPECIAL_INTERNAL", true, AUTORUN_ORDER_FIRST);
	}
	// All identifiers must exist before resolving cross-file references.
	for (int i = 0; i < m_iNumProjectFiles; ++i)
		ScanSourceFile(m_ProjectFiles[i]);
	for (int i = 0; i < m_iNumProjectFiles; ++i) {
		for (int j = 0; j < m_iNumSourceParsers; ++j) {
			m_pSourceParsers[j]->ProcessDataSingleFile(
				m_ProjectFiles[i]);
		}
	}
	// Emitters register additional autoruns; the autorun manager is last.
	for (int i = 0; i < m_iNumSourceParsers; ++i)
		m_pSourceParsers[i]->WriteOutData();
	return 0;
}

void SourceParser::ScanSourceFile(char *pSourceFile)
{
    char const* sMagicWords[MAX_BASE_SOURCE_PARSERS * MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER + 1]; 

    int iNumWildcardMagicWords = 0;
    int iWildcardMagicWordIndices[MAX_WILDCARD_MAGIC_WORDS];

    TRACE("Parsing %s\n", pSourceFile);


    sMagicWords[m_iNumSourceParsers * MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER] = NULL;

    int i, j;

    for (i=0; i < m_iNumSourceParsers; i++)
    {
        for (j=0; j < MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER; j++)
        {
            sMagicWords[i * MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER + j] = m_pSourceParsers[i]->GetMagicWord(j);

            if (StringContainsWildcards(sMagicWords[i * MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER + j]))
            {
                Tokenizer::StaticAssert(iNumWildcardMagicWords < MAX_WILDCARD_MAGIC_WORDS, "Too many wildcard magic words");
                iWildcardMagicWordIndices[iNumWildcardMagicWords++] = i * MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER + j;
            }
        }
    }

    Tokenizer tokenizer;

    bool bResult = tokenizer.LoadFromFile(pSourceFile);

    if (!bResult)
    {
        char errorString[256];
        sprintf(errorString, "Couldn't find file %s\n", pSourceFile);
        Tokenizer::StaticAssert(0, errorString);
    }

    tokenizer.SetCSourceStyleStrings(true);
    tokenizer.SetExtraReservedWords(sMagicWords);
    tokenizer.SetNoNewlinesInStrings(true);
    tokenizer.SetSkipDefines(true);

    Token token;
    enumTokenType eType;

    for (i=0; i < m_iNumSourceParsers; i++)
    {
        m_pSourceParsers[i]->FoundMagicWord(pSourceFile, &tokenizer, MAGICWORD_BEGINNING_OF_FILE, NULL);
    }


    do
    {
        eType = tokenizer.GetNextToken(&token);

        if (eType == TOKEN_RESERVEDWORD && token.iVal >= RW_COUNT)
        {
            int iMagicWordNum = token.iVal - RW_COUNT;
            tokenizer.StringifyToken(&token);
            m_pSourceParsers[iMagicWordNum / MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER]->FoundMagicWord(pSourceFile, &tokenizer, iMagicWordNum % MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER, token.sVal);
        }
        else
        {
            int i;

            for (i=0; i < iNumWildcardMagicWords; i++)
            {
                if (eType == TOKEN_IDENTIFIER && DoesStringMatchWildcard(token.sVal, sMagicWords[iWildcardMagicWordIndices[i]]))
                {
                    int iMagicWordNum = iWildcardMagicWordIndices[i];
                    m_pSourceParsers[iMagicWordNum / MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER]->FoundMagicWord(pSourceFile, &tokenizer, iMagicWordNum % MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER, token.sVal);
                    break;
                }
            }
        }
    } while (eType != TOKEN_NONE);

    for (i=0; i < m_iNumSourceParsers; i++)
    {
        m_pSourceParsers[i]->FoundMagicWord(pSourceFile, &tokenizer, MAGICWORD_END_OF_FILE, NULL);
    }

}


void ReplaceMacroInPlace(char *pString, char const* pMacroToFind, char const* pReplaceString)
{
    int iMacroLength = (int)strlen(pMacroToFind);
    int iStringLength = (int)strlen(pString);
    int iReplaceLength = (int)strlen(pReplaceString);

    if (iStringLength < iMacroLength)
    {
        return;
    }

    int i;

    for (i=0; i <= iStringLength - iMacroLength; i++)
    {
        if (strncmp(pMacroToFind, pString + i, iMacroLength) == 0)
        {
            memmove(pString + i + iReplaceLength, pString + i + iMacroLength, iStringLength - (i + iMacroLength) + 1);
            memcpy(pString + i, pReplaceString, iReplaceLength);

            iStringLength += iReplaceLength - iMacroLength;
            i += iReplaceLength - 1;

        }
    }
}

void ReplaceMacrosInPlace(char *pString, char const* pMacros[][2])
{
    int i;

    for (i=0; pMacros[i][0]; i++)
    {
        ReplaceMacroInPlace(pString, pMacros[i][0], pMacros[i][1]);
    }
}


int SourceParser::FindProjectFileIndex(char *pFileName)
{
    int i;

    for (i=0; i < m_iNumProjectFiles; i++)
    {
        if (AreFilenamesEqual(pFileName, m_ProjectFiles[i]))
        {
            return i;
        }
    }

    return -1;
}


bool SourceParser::MakeSpecialAutoRunFunction(void)
{
	return !StringIsInList(m_shortenedPrjFileName.c_str(),
		sProjectNamesToExclude);
}

bool SourceParser::DoesVariableHaveValue(char const* pVarName, char const* pValue, bool bCheckFinalValueOnly)
{
    SourceParserVar *pVar = m_pFirstVar;


    while (pVar)
    {
        if (CompareNoCase(pVar->pVarName, pVarName) == 0)
        {
            if (bCheckFinalValueOnly)
            {
                int iLen = (int)strlen(pValue);
                if (strncmp(pValue, pVar->pValue + 1, iLen) == 0)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            char tempBuffer[256];
            sprintf(tempBuffer, " %s ", pValue);
            if (strstri(pVar->pValue, tempBuffer))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        pVar = pVar->pNext;
    }

    return false;
}

void SourceParser::AddVariableValue(char *pVarName, char *pValue)
{
    SourceParserVar *pVar = m_pFirstVar;


    while (pVar)
    {
        if (CompareNoCase(pVar->pVarName, pVarName) == 0)
        {
            int iCurLen = (int)strlen(pVar->pValue);
            int iAddLen = (int)strlen(pValue);
            char *pNewBuf = new char[iCurLen + iAddLen + 2];
            sprintf(pNewBuf, " %s%s", pValue, pVar->pValue);
            delete[] pVar->pValue;
            pVar->pValue = pNewBuf;
            return;
        }

        pVar = pVar->pNext;
    }

    pVar = new SourceParserVar;

    pVar->pNext = m_pFirstVar;
    m_pFirstVar = pVar;

    pVar->pVarName = STRDUP(pVarName);
    int iCurLen = (int)strlen(pValue);
    pVar->pValue = new char[iCurLen + 3];
    sprintf(pVar->pValue, " %s ", pValue);
}

void SourceParser::SetVariablesFromTokenizer(Tokenizer *pTokenizer, char *pStartingDirectory)
{
    enumTokenType eType;
    Token token;
    char varName[256];

    while (1)
    {
        eType = pTokenizer->GetNextToken(&token);

        if (eType == TOKEN_NONE)
        {
            return;
        }

        pTokenizer->Assert(eType == TOKEN_IDENTIFIER, "Expected identifier name to set");
        pTokenizer->Assert(token.iVal < 255, "Var name overflow");

        if (CompareNoCase(token.sVal, "#include") == 0)
        {
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Expected string after #include");
			ReplaceCharWithChar(token.sVal, '\\', '/');
			auto include = fs::path(token.sVal);
			if (include.is_relative())
				include = fs::path(pStartingDirectory) /
					include;
			include = fs::absolute(include).lexically_normal();
			Tokenizer tokenizer;
			tokenizer.SetExtraCharsAllowedInIdentifiers("#");
			pTokenizer->Assertf(tokenizer.LoadFromFile(
				include.string().c_str()),
				"Couldn't load include file %s",
				include.string().c_str());
			Tokenizer::StaticAssert(m_configStack.size() < 64,
				"Configuration include nesting exceeds 64");
			m_configurationDependencies.push_back(include);
			m_configStack.push_back(include);
			auto directory = include.parent_path().string();
			SetVariablesFromTokenizer(&tokenizer, directory.data());
			m_configStack.pop_back();
        }
        else
        {
            strcpy(varName, token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_EQUALS, "Expected = after var name");

            do
            {
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 0, "expected identifier for var value");
                AddVariableValue(varName, token.sVal);

                pTokenizer->Assert2NextTokenTypesAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, TOKEN_RESERVEDWORD, RW_SEMICOLON, "Expected , or ;");
            }
            while (token.iVal != RW_SEMICOLON);
        }
    } 
}

void SourceParser::LoadConfiguration(fs::path const& projectDir)
{
	for (auto dir = projectDir; ; dir = dir.parent_path()) {
		auto file = dir / "StructParserVars.txt";
		m_configurationDependencies.push_back(file);
		if (fs::exists(file)) {
			Tokenizer tokenizer;
			tokenizer.SetExtraCharsAllowedInIdentifiers("#");
			Tokenizer::StaticAssert(tokenizer.LoadFromFile(
				file.string().c_str()),
				"Cannot read configuration");
			auto directory = dir.string();
			SetVariablesFromTokenizer(&tokenizer, directory.data());
			return;
		}
		if (dir == dir.root_path())
			return;
	}
}
