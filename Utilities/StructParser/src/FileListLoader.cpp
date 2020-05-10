#include "pch.h"
#include "FileListLoader.h"
#include "tokenizer.h"
#include "strutils.h"


extern char gFileEndString[];
extern char gVersionString[];

FileListLoader::FileListLoader()
{
    m_iNumFiles = 0;
    m_MasterFileTime = {};
}
FileListLoader::~FileListLoader()
{
}

bool FileListLoader::LoadFileList(char const* pListFileName)
{
    Tokenizer tokenizer;

    if (!tokenizer.LoadFromFile(pListFileName))
    {
        return false;
    }

    if (!tokenizer.IsStringAtVeryEndOfBuffer(gFileEndString))
    {
        return false;
    }

    // %d/%d/%d %d:%d:%d:%d

    
    Token token;
    enumTokenType eType;
    SYSTEMTIME sysTime;

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Couldn't parse date and time");
    sysTime.wMonth = token.iVal;

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_SLASH, "Couldn't parse date and time");

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Couldn't parse date and time");
    sysTime.wDay = token.iVal;

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_SLASH, "Couldn't parse date and time");

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Couldn't parse date and time");
    sysTime.wYear = token.iVal;


    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Couldn't parse date and time");
    sysTime.wHour = token.iVal;

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COLON, "Couldn't parse date and time");

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Couldn't parse date and time");
    sysTime.wMinute = token.iVal;

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COLON, "Couldn't parse date and time");

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Couldn't parse date and time");
    sysTime.wSecond = token.iVal;

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COLON, "Couldn't parse date and time");

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Couldn't parse date and time");
    sysTime.wMilliseconds = token.iVal;

    SystemTimeToFileTime(&sysTime, &m_MasterFileTime);

    eType = tokenizer.GetNextToken(&token);

    if (!(eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "VERSION") == 0))
    {
        return false;
    }

    eType = tokenizer.GetNextToken(&token);

    if (!(eType == TOKEN_STRING))
    {
        return false;
    }

    if (!(strcmp(token.sVal, gVersionString) == 0))
    {
        return false;
    }

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_PATH, "Didn't find obj file dir name");
    strcpy(m_ObjFileDirectory, token.sVal);

    do
    {
        eType = tokenizer.GetNextToken(&token);

        if (eType == TOKEN_STRING)
        {
            tokenizer.Assert(m_iNumFiles < MAX_FILES_IN_PROJECT, "Too many files");

            strcpy(m_FileNames[m_iNumFiles], token.sVal);

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find extra data after file name");
            
            m_iExtraData[m_iNumFiles] = token.iVal;

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find num dependencies after extra data");

            m_iNumDependencies[m_iNumFiles] = token.iVal;

            int i;

            for (i=0; i < m_iNumDependencies[m_iNumFiles]; i++)
            {
                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find int for dependency");
                m_iDependencies[m_iNumFiles][i] = token.iVal;
            }

            HANDLE hFile;

            hFile = CreateFileA(m_FileNames[m_iNumFiles], GENERIC_READ, FILE_SHARE_READ,
                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            if (hFile == INVALID_HANDLE_VALUE)
            {
                m_FileExists[m_iNumFiles] = false;
            }
            else 
            {
                m_FileExists[m_iNumFiles] = true;

    
                GetFileTime(hFile, NULL, NULL, &m_FileTimes[m_iNumFiles]);

                CloseHandle(hFile);
            }

            m_iNumFiles++;

        }
    } while (eType != TOKEN_IDENTIFIER);


    return true;
}



bool FileListLoader::IsFileInList(char *pFileName)
{
    int i;

    for (i=0; i < m_iNumFiles; i++)
    {
        if (AreFilenamesEqual(pFileName, m_FileNames[i]))
        {
            return true;
        }
    }

    return false;
}