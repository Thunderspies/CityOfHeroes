#include "GenerationIO.h"
#include "pch.h"
#include <cstdio>
#include <string.h>
#include "Platform.h"

#include "strutils.h"


char *NoConst(char *pInString)
{
    if (strncmp(pInString, "const ", 6) == 0)
    {
        return pInString + 6;
    }

    return pInString;
}

void TruncateStringAtLastOccurrence(char *pString, char cTrunc)
{
    if (!pString)
    {
        return;
    }

    int iLen = (int)strlen(pString);

    char *pTemp = pString + iLen - 1;

    do
    {
        if (!(*pTemp))
        {
            return;
        }

        if (*pTemp == cTrunc)
        {
            *pTemp = 0;
            return;
        }

        pTemp--;
    }
    while (pTemp > pString);
}

void TruncateStringAfterLastOccurrence(char *pString, char cTrunc)
{
    if (!pString)
    {
        return;
    }

    int iLen = (int)strlen(pString);

    char *pTemp = pString + iLen - 1;

    if (*pTemp == cTrunc)
    {
        return;
    }

    pTemp--;

    do
    {
        if (!(*pTemp))
        {
            return;
        }

        if (*pTemp == cTrunc)
        {
            *(pTemp+1) = 0;
            return;
        }

        pTemp--;
    }
    while (pTemp > pString);
}





bool StringIsInList(char const* pString, char const* pList[])
{
    int i = 0;

    while (pList[i])
    {
        if (strcmp(pString, pList[i]) == 0)
        {
            return true;
        }

        i++;
    }

    return false;
}
void PutSlashAtEndOfString(char *pString)

{
    int len = (int)strlen(pString);

    if (pString[len-1] == '\\' || pString[len-1] == '/')
    {
        return;
    }

    pString[len] = '\\';
    pString[len+1] = 0;
}




int SubDivideStringAndRemoveWhiteSpace(char **ppSubStrings, char *pInString, char separator, int iMaxToFind)
{
    int i;

    int iNumFound = 0;
    bool bEndOfString = false;

    while (iNumFound < iMaxToFind && !bEndOfString)
    {
        while (IsWhiteSpace(*pInString) || (*pInString == separator && IsWhiteSpace(*(pInString + 1))))
        {
            pInString++;
        }

        ppSubStrings[iNumFound] = pInString;

        do
        {
            pInString++;
        }
        while ( *pInString && !(*pInString == ' ' && *(pInString + 1) == separator && *(pInString + 2) == ' '));

        iNumFound++;

        if (*pInString)
        {
            *pInString = 0;
            pInString++;
            *pInString = 0;
            pInString++;
            *pInString = 0;
            pInString++;
        }
        else
        {
            bEndOfString = true;
        }
    }


    //trim leading and trailing whitespace from all strings found
    for (i = 0; i < iNumFound; i++)
    {
        while (IsWhiteSpace(*(ppSubStrings[i])))
        {
            ppSubStrings[i]++;
        }

        int iLen = (int)strlen(ppSubStrings[i]);

        while (IsWhiteSpace(*(ppSubStrings[i] + iLen - 1)))
        {
            *(ppSubStrings[i] + iLen - 1) = 0;
            iLen--;
        }
    }

    return iNumFound;
}

void FixupBackslashedQuotes(char *pSourceString)
{
    int iLen = (int)strlen(pSourceString);
    int i = 0;

    do
    {
        if (i >= iLen)
        {
            return;
        }
        else if (pSourceString[i] == '\\')
        {
            if (pSourceString[i+1] == 0)
            {
                return;
            }

            if (pSourceString[i+1] == '"')
            {
                memmove(pSourceString + i, pSourceString + i + 1, iLen - i);
                i++;
                iLen--;
            }
            else
            {
                i += 2;
            }
        }
        else
        {
            i++;
        }
    }
    while (1);
}








const char *GetFileNameWithoutDirectories(const char *pSourceName)
{
    const char *pOutString = pSourceName + strlen(pSourceName);

    while (pOutString > pSourceName && !(*pOutString == '\\' || *pOutString == '/'))
    {
        pOutString--;
    }

    return pOutString;
}
void MakeStringAllAlphaNumAndUppercase(char *pString)
{
    MakeStringUpcase(pString);

    while (*pString)
    {
        if (!IsAlphaNum(*pString))
        {
            *pString = '_';
        }

        pString++;
    }
}

void MakeStringAllAlphaNum(char *pString)
{
    while (*pString)
    {
        if (!IsAlphaNum(*pString))
        {
            *pString = '_';
        }

        pString++;
    }
}
void MakeRepeatedCharacterString(char *pString, int iNumOfChar, int iMaxOfChar, char c)
{
    if (iNumOfChar < 0)
    {
        iNumOfChar = 0;
    }
    else
    {    
        if (iNumOfChar > iMaxOfChar)
        {
            iNumOfChar = iMaxOfChar;
        }

        memset(pString, c, iNumOfChar);
    }
    pString[iNumOfChar] = 0;
}

static INLINEDBG bool FilenameCharsAreEqual(char c1, char c2)
{
    if (MakeCharUpcase(c1) == MakeCharUpcase(c2))
    {
        return true;
    }

    if ((c1 == '\\' || c1 == '/') && (c2 == '\\' || c2 == '/'))
    {
        return true;
    }
    
    return false;
}

bool AreFilenamesEqual(char const* pName1, char const* pName2)
{
    do
    {
        if (!FilenameCharsAreEqual(*pName1, *pName2))
        {
            return false;
        }

        if (!(*pName1))
        {
            return true;
        }

        pName1++;
        pName2++;
    } while (1);
}

void RemoveTrailingWhiteSpace(char *pString)
{
    int iLen = (int)strlen(pString);

    char *pChar = pString + iLen - 1;

    while (pChar >= pString && IsWhiteSpace(*pChar))
    {
        *pChar = 0;
        pChar--;
    }
}

void RemoveSuffixIfThere(char *pMainString, char const* pSuffix)
{
    int iSuffixLen = (int)strlen(pSuffix);

	if (CompareNoCase(pMainString + strlen(pMainString) - iSuffixLen,
		pSuffix) == 0)
    {
        pMainString[strlen(pMainString) - iSuffixLen] = 0;
    }
}

char escapeSeqDefs[][2] =
{
    { 'a', '\a' },
    { 'b', '\b' },
    { 'f', '\f' },
    { 'n', '\n' },
    { 'r', '\r' },
    { 't', '\t' },
    { 'v', '\v' },
    { '\\', '\\' },
    { '\'', '\'' },
    { '"', '"' },
};

void RemoveCStyleEscaping(char *pOutString, char *pInString)
{
    while (*pInString)
    {
        if (*pInString == '\\')
        {
            int i;

            pInString++;

            for (i=0; i < sizeof(escapeSeqDefs) / sizeof(escapeSeqDefs[0]); i++)
            {
                if (*pInString == escapeSeqDefs[i][0])
                {
                    *pOutString = escapeSeqDefs[i][1];
                    pOutString++;
                    pInString++;
                    break;
                }
            }

            //if we didn't find it, just dump literally and hope for the best
            if (i == sizeof(escapeSeqDefs) / sizeof(escapeSeqDefs[0]))
            {
                *pOutString = '\\';
                pOutString++;
                *pOutString = *pInString;
                pOutString++;
                pInString++;
            }
        }
        else
        {
            *pOutString = *pInString;
            pOutString++;
            pInString++;
        }
    }

    *pOutString = 0;
}

void AddCStyleEscaping(char *pOutString, char const* pInString, int iMaxSize)
{
    char *pMaxEnd = pOutString + iMaxSize;

    while (*pInString)
    {
        int i;
        bool bFound = false;

        if (pOutString >= pMaxEnd - 2)
        {
            *pOutString = 0;
            return;
        }

        for (i=0; i < sizeof(escapeSeqDefs) / sizeof(escapeSeqDefs[0]); i++)
        {
            if (*pInString == escapeSeqDefs[i][1])
            {
                *pOutString = '\\';
                pOutString++;

                *pOutString = escapeSeqDefs[i][0];
                pOutString++;

                pInString++;

                bFound = true;
                break;
            }
        }

        if (!bFound)
        {
            *pOutString = *pInString;
            pOutString++;
            pInString++;
        }
    }

    *pOutString = 0;
}


void assembleFilePath(char *pOutPath, char *pDir, char *pOffsetFile)
{
    char dir[MAX_PATH];
    char offsetFile[MAX_PATH];
    strcpy(dir, pDir);
    strcpy(offsetFile, pOffsetFile);

    while (strncmp(offsetFile, "..\\", 3) == 0 || strncmp(offsetFile, "../", 3) == 0)
    {
        memmove(offsetFile, offsetFile + 3, strlen(offsetFile) - 2);
        
        int iDirLen = (int)strlen(dir);

        if (iDirLen > 4)
        {
            do
            {
                iDirLen --;
                dir[iDirLen] = 0;
            }
            while (dir[iDirLen - 1] != '\\' && dir[iDirLen - 1] != '/');
        }
    }

    sprintf(pOutPath, "%s%s", dir, offsetFile);



}



void ClipStrings(char *pMainString, int iNumStringsToClip, char **ppStringsToClip)
{
    int i;
    char *pSubString;

    for (i=0; i < iNumStringsToClip; i++)
    {
        while ((pSubString = strstr(pMainString, ppStringsToClip[i])))
        {
            memmove(pSubString, pSubString + strlen(ppStringsToClip[i]), strlen(pMainString) - strlen(ppStringsToClip[i]) - (pSubString - pMainString) + 1);
        }
    }
}

//only supported wildcard right now is WILDCARD_STRING as a suffix
bool StringContainsWildcards(char const* pString)
{
    int iLen = (int) strlen(pString);
    
    return (iLen > WILDCARD_STRING_LENGTH && strcmp(pString + iLen - WILDCARD_STRING_LENGTH, WILDCARD_STRING) == 0);
}
    
bool DoesStringMatchWildcard(char const* pString, char const* pWildcard)
{
    int iLen = (int) strlen(pWildcard) - WILDCARD_STRING_LENGTH;

    return (strncmp(pString, pWildcard, iLen) == 0);
}


//includes all the contents of filename in the file at the current location, with some C-style comments
void ForceIncludeFile(FILE *pOuterFile, char *pFileNameToInclude)
{
    fprintf(pOuterFile, "//\n//\n//Beginning forced include of all contents of file %s\n//\n//\n//\n", pFileNameToInclude);

    FILE *pInnerFile = OpenInput(pFileNameToInclude, "rt");

    if (!pInnerFile)
    {
        fprintf(pOuterFile, "// (included file is empty or nonexistant)\n");
    }
    else
    {
        char c;

        do
        {
            c = getc(pInnerFile);

            if (c != EOF)
            {
                putc(c, pOuterFile);
            }
            else
            {
                break;
            }
        } while (1);

        CloseFile(pInnerFile);
    }

    fprintf(pOuterFile, "//\n//\n//Ending forced include of all contents of file %s\n//\n//\n//\n", pFileNameToInclude);
}

void ReplaceCharWithChar(char *pString, char before, char after)
{
    char *pTemp = pString;

    while ((pTemp = strchr(pTemp, before)))
    {
        *pTemp = after;
        pTemp++;
    }
}

char *STRDUP(const char *pSource)
{
    char *pRetVal;

    if (!pSource)
    {
        return NULL;
    }

    int iLen = (int)strlen(pSource);

    pRetVal = new char[iLen + 1];
    memcpy(pRetVal, pSource, iLen+1);

    return pRetVal;
}

void RemoveNewLinesAfterBackSlashes(char *pString)
{
    int iLen = (int)strlen(pString);
    int iOffset = 0;

    while (iOffset < iLen)
    {
        if (pString[iOffset] == '\\')
        {
            int i = iOffset+1;

            do
            {
                if (pString[i] == 0)
                {
                    return;
                }
                else if (!IsWhiteSpace(pString[i]))
                {
                    iOffset = i;
                    break;
                }
                else if (pString[i] == '\n')
                {
                    memmove(pString + iOffset, pString + i + 1, iLen - i);
                    iLen -= i + 1 - iOffset;
                    iOffset += 1;
                    break;
                }
                i++;
            } while (1);
        }
        else
        {
            iOffset++;
        }
    }
}

bool StringEndsWith(char *pString, char *pSuffix)
{
    int iLen = (int)strlen(pString);
    int iSuffixLen = (int)strlen(pSuffix);

    if (iSuffixLen > iLen)
    {
        return false;
    }

    return (strcmp(pString + (iLen - iSuffixLen), pSuffix) == 0);
}

void FowardSlashes(char *pString)
{
    while (*pString)
    {
        if (*pString == '\\')
        {
            *pString = '/';
        }

        pString++;
    }
}


bool StringComesAlphabeticallyBefore(char *pString1, char *pString2)
{
    do
    {
        char c1 = MakeCharUpcase(*pString1);
        char c2 = MakeCharUpcase(*pString2);
        if (!c1)
        {
            return true;
        }

        if (!c2)
        {
            return false;
        }

        if (c1 < c2)
        {
            return true;
        }

        if (c1 > c2)
        {
            return false;
        }

        pString1++;
        pString2++;
    } while (1);
}

void NormalizeNewlinesInString(char *pString)
{
    int iLen = (int)strlen(pString);
    int i, j;

    for (i=0; i < iLen; i++)
    {
        if (pString[i] == '\n' || pString[i] == '\r')
        {
            pString[i] = '\n';
            int iCount = 0;
            j = i+1;

            while (pString[j] == '\n' || pString[j] == '\r')
            {
                j++;
                iCount++;
            }

            if (iCount)
            {
                memmove(pString + i + 1, pString + i + 1 + iCount, iLen - i - iCount);
                iLen -= iCount;
            }
        }
    }
}

void TruncateStringAtSuffixIfPresent(char *pString, char *pSuffix)
{
    if (!pString || !pSuffix || !pString[0] || !pSuffix[0])
    {
        return;
    }

    int iStrLen = (int)strlen(pString);
    int iSuffixLen = (int)strlen(pSuffix);

    if (iSuffixLen > iStrLen)
    {
        return;
    }

    if (strcmp(pSuffix, pString + iStrLen - iSuffixLen) == 0)
    {
        pString[iStrLen - iSuffixLen] = 0;
    }
}

char* strstri(const char* str1, const char* str2)
{
	auto end = str1 + strlen(str1);
	auto found = std::search(str1, end, str2, str2 + strlen(str2),
		[](char a, char b) {
			return MakeCharLowercase(a) == MakeCharLowercase(b);
		});
	return found == end && *str2 ? nullptr : const_cast<char *>(found);
}

bool isNonWholeNumberFloatLiteral(char *pString)
{
    while (IsWhiteSpace(*pString))
    {
        pString++;
    }

    while (*pString == '-')
    {
        pString++;
    }

    if (!(isdigit(*pString)))
    {
        return false;
    }

    while (isdigit(*pString))
    {
        pString++;
    }

    if (*pString != '.')
    {
        return false;
    }

    pString++;

    while (isdigit(*pString))
    {
        if (*pString != '0')
        {
            return true;
        }

        pString++;
    }

    return false;
}



