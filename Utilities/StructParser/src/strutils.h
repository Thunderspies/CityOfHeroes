#ifndef _STRUTILS_H_ 
#define _STRUTILS_H_

#include <cstdio>

// From "../../libs/UtilitiesLib/stdtypes.h"
#if defined(FULLDEBUG)
    #define INLINEDBG
#elif defined(_DEBUG)
    #define INLINEDBG inline
#else
    #define INLINEDBG __inline
#endif

char *NoConst(char *pInString);

static INLINEDBG char MakeCharUpcase(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        c += 'A' - 'a';
    }

    return c;
}

static INLINEDBG char MakeCharLowercase(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        c -= 'A' - 'a';
    }

    return c;
}


static INLINEDBG void MakeStringUpcase(char *pString)
{
    if (pString)
    {
        while (*pString)
        {
            *pString = MakeCharUpcase(*pString);
            
            pString++;
        }
    }
}

static INLINEDBG void MakeStringLowercase(char *pString)
{
    if (pString)
    {
        while (*pString)
        {
            *pString = MakeCharLowercase(*pString);
            
            pString++;
        }
    }
}

void TruncateStringAtLastOccurrence(char *pString, char cTrunc);
void TruncateStringAfterLastOccurrence(char *pString, char cTrunc);

bool StringIsInList(char const* pString, char const* pList[]);

void PutSlashAtEndOfString(char *pString);

int SubDivideStringAndRemoveWhiteSpace(char **ppSubStrings, char *pInString, char separator, int iMaxToFind);

static INLINEDBG bool IsWhiteSpace(char c)
{
    return c == ' ' || c == '\n' || c == '\t';
}

static INLINEDBG bool IsNonLineBreakWhiteSpace(char c)
{
    return c == ' ' || c == '\t';
}


static INLINEDBG bool IsDigit(char c)
{
    return c >= '0' && c <= '9';
}

static INLINEDBG bool IsAlpha(char c)
{
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

static INLINEDBG bool IsAlphaNum(char c)
{
    return IsDigit(c) || IsAlpha(c);
}


static INLINEDBG bool IsOKForIdent(char c)
{
    return IsAlphaNum(c) || c == '_';
}

static INLINEDBG bool IsOKForIdentStart(char c)
{
    return IsAlpha(c) || c == '_';
}

void ReplaceMacrosInPlace(char *pString, char const* pMacros[][2]);

void FixupBackslashedQuotes(char *pSourceString);

const char *GetFileNameWithoutDirectories(const char *pSourceName);
void MakeStringAllAlphaNumAndUppercase(char *pString);
void MakeStringAllAlphaNum(char *pString);

void MakeRepeatedCharacterString(char *pString, int iNumOfChar, int iMaxOfChar, char c);

bool AreFilenamesEqual(char const* pName1, char const* pName2);

void RemoveTrailingWhiteSpace(char *pString);

void RemoveSuffixIfThere(char *pMainString, char const* pSuffix);

void RemoveCStyleEscaping(char *pOutString, char *pInString);
void AddCStyleEscaping(char *pOutString, char const* pInString, int iMaxSize);

void assembleFilePath(char *pOutPath, char *pDir, char *pOffsetFile);

//goes into a source string, finds each occurrence of each of the substrings listed, and snips them out
void ClipStrings(char *pMainString, int iNumStringsToClip, char **ppStringsToClip);

#define WILDCARD_STRING "__XXAUTOSTRUCTWILDCARDXX__"
#define WILDCARD_STRING_LENGTH 26

bool StringContainsWildcards(char const* pString);
bool DoesStringMatchWildcard(char const* pString, char const* pWildcard);

//includes all the contents of filename in the file at the current location, with some C-style comments
void ForceIncludeFile(FILE *pOuterFile, char *pFileNameToInclue);

//replace all occurences in a string of one char with another
void ReplaceCharWithChar(char *pString, char before, char after);

char *STRDUP(const char *pSource);

void RemoveNewLinesAfterBackSlashes(char *pString);

bool StringEndsWith(char *pString, char *pSuffix);

void FowardSlashes(char *pString);

bool StringComesAlphabeticallyBefore(char *pString1, char *pString2);

//turns all /r/n/r/n whatever businesses into a single /n
void NormalizeNewlinesInString(char *pString);

char* strstri(const char* str1, const char* str2);

void TruncateStringAtSuffixIfPresent(char *pString, char *pSuffix);


bool isNonWholeNumberFloatLiteral(char *pString);

#endif