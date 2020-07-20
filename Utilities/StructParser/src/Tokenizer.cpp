#include "pch.h"
#include <cstdio>
#include "tokenizer.h"
#include "assert.h"
#include "string.h"
#include "windows.h"
#include "strutils.h"



static char sCommentControlCode[] = {"##"};
#define COMMENT_CONTROL_CODE_LENGTH 2

static char sSingleCharReservedWordIndex[256] = { 0 };

static char const* sReservedWords[] = 
{
    "",
    "->",
    "{",
    "}",
    "(",
    ")",
    "[",
    "]",
    ",",
    ":",
    "-",
    ";",
    "+",
    "/",
    "!",
    "&",
    "*",
    "^",
    "=",
    ".",
    ">",
    "<",
    "?",
    "~",
    "|",


    "struct",
    "typedef",
    "void",
    "enum",
};




Tokenizer::Tokenizer()
{
    static bool bFirst = true;

    if (bFirst)
    {
        int i;
        bFirst = false;

        for (i = FIRST_SINGLECHAR_RW; i < FIRST_MULTICHAR_RW; i++)
        {
            sSingleCharReservedWordIndex[sReservedWords[i][0]] = i;
        }
    }

    m_bLookForControlCodeInComments = false;
    m_bInsideCommentControlCode = false;
    m_ppExtraReservedWords = NULL;
    m_bOwnsBuffer = false;
    m_bCSourceStyleStrings = false;
    m_LastStringLength = 0;
    m_bNoNewlinesInStrings = false;
    m_bSkipDefines = false;
    m_pExtraIdentifierChars = NULL;
    m_bIgnoreQuotes = false;
    m_pBufferStart = NULL;
    m_pBufferEnd = NULL;
}

Tokenizer::~Tokenizer()
{
    Reset();
}

void Tokenizer::SetExtraCharsAllowedInIdentifiers(char const* pString)
{
    if (m_pExtraIdentifierChars)
    {
        delete m_pExtraIdentifierChars;
        m_pExtraIdentifierChars = NULL;
    }
    
    if (pString && pString[0])
    {
        m_pExtraIdentifierChars = STRDUP(pString);
    }
}

void Tokenizer::Reset()
{
    if (m_bOwnsBuffer)
    {
        delete [] m_pBufferStart;
        m_bOwnsBuffer = false;
    }
    m_bLookForControlCodeInComments = false;
    m_bInsideCommentControlCode = false;
    m_ppExtraReservedWords = NULL;
    m_bCSourceStyleStrings = false;
    m_bDontParseInts = false;
}


void Tokenizer::LoadFromBuffer(char *pBuffer, int iSize, char const* pFileName, int iStartingLineNum)
{
    

    Reset();

    StaticAssert(pBuffer != NULL, "Can't load from NULL buffer");
    StaticAssert(iSize > 0, "Can't load from zero-size buffer");

    m_pBufferStart = m_pReadHead = pBuffer;
    m_pBufferEnd = pBuffer + iSize;

    m_bInsideCommentControlCode = false;

    strcpy(m_CurFileName, pFileName);
    m_iStartingLineNum = iStartingLineNum;
    m_iCurLineNum = iStartingLineNum;

}

bool Tokenizer::LoadFromFile(char const* pFileName)
{
    FILE *pInFile;

    Reset();

    pInFile = fopen(pFileName, "rb");
    
    if (!pInFile)
    {
        Sleep(100);

        pInFile = fopen(pFileName, "rb");
        {
            if (!pInFile)
            {
                return false;
            }
        }
    }
    

    fseek(pInFile, 0, SEEK_END);

    int iFileSize = ftell(pInFile);

    fseek(pInFile, 0, SEEK_SET);

    m_pBufferStart = new char[iFileSize + 1];

    Tokenizer::StaticAssert(m_pBufferStart != NULL, "new failed");

    fread(m_pBufferStart, iFileSize, 1, pInFile);

    fclose(pInFile);

    m_pBufferStart[iFileSize] = 0;

    m_pReadHead = m_pBufferStart;
    m_pBufferEnd = m_pBufferStart + iFileSize;

    m_bOwnsBuffer = true;


    strcpy(m_CurFileName, pFileName);
    m_iCurLineNum = 1;
    m_iStartingLineNum = 1;


    return true;

}

//if this function is called, we just hit a \, and want to return true if there is nothing but optional whitespace then a newline
bool Tokenizer::CheckForMultiLineCString()
{
    char *pStartingReadHead = m_pReadHead;

    //skip the backslash
    m_pReadHead++;

    //skip non-newline whitespace
    while ((*m_pReadHead != '\n') && (IsWhiteSpace(*m_pReadHead) || *m_pReadHead == '\r'))
    {
        m_pReadHead++;
    }

    if (*m_pReadHead == '\n')
    {
        m_iCurLineNum++;
        m_pReadHead++;
        return true;
    }

    m_pReadHead = pStartingReadHead;
    return false;
}

void Tokenizer::AdvanceToBeginningOfLine()
{
    do
    {
        m_pReadHead++;
    }
    while (*m_pReadHead && *m_pReadHead != '\n');

    m_pReadHead++;
}
void Tokenizer::BackUpToBeginningOfLine()
{
    m_pReadHead--;

    do
    {
        m_pReadHead--;
    }
    while (m_pReadHead >= m_pBufferStart && *m_pReadHead != '\n');

    m_pReadHead++;
}

bool Tokenizer::DoesLineBeginWithComment()
{
    char *pTempReadHead = m_pReadHead;

    while (IsNonLineBreakWhiteSpace(*pTempReadHead))
    {
        pTempReadHead++;
    }

    return (*pTempReadHead == '/' && *(pTempReadHead+1) == '/');
}


void Tokenizer::GetSurroundingSlashedCommentBlock(Token *pToken)
{
    int iLineNum, iOffset;

    iOffset = GetOffset(&iLineNum);

    BackUpToBeginningOfLine();

    char *pEndOfBlock = m_pReadHead;

    do
    {
        BackUpToBeginningOfLine();
    } while (DoesLineBeginWithComment());

    AdvanceToBeginningOfLine();

    int iOutLength = 0;
    bool bInsideSlashes = true;

    while (m_pReadHead < pEndOfBlock && iOutLength < TOKENIZER_MAX_STRING_LENGTH)
    {
        while (bInsideSlashes && (*m_pReadHead == '/' || IsNonLineBreakWhiteSpace(*m_pReadHead)))
        {
            m_pReadHead++;
        }

        bInsideSlashes = false;

        if (*m_pReadHead == '\n')
        {
            pToken->sVal[iOutLength] = '\n';
            iOutLength++;
            m_pReadHead++;
            bInsideSlashes = true;
        }
        else if (*m_pReadHead == '\r')
        {
            m_pReadHead++;
        }
        else
        {
            pToken->sVal[iOutLength] = *m_pReadHead;
            iOutLength++;
            m_pReadHead++;
        }
    }

    //got all preceding comments.
    AdvanceToBeginningOfLine();

    char *pBeginningOfSucceedingLines = m_pReadHead;

    while (DoesLineBeginWithComment())
    {
        AdvanceToBeginningOfLine();
    }

    char *pEndOfSucceedingLines = m_pReadHead;

    m_pReadHead = pBeginningOfSucceedingLines;

    bInsideSlashes = true;

    while (m_pReadHead < pEndOfSucceedingLines && iOutLength < TOKENIZER_MAX_STRING_LENGTH)
    {
        while (bInsideSlashes && (*m_pReadHead == '/' || IsNonLineBreakWhiteSpace(*m_pReadHead)))
        {
            m_pReadHead++;
        }

        bInsideSlashes = false;

        if (*m_pReadHead == '\n')
        {
            pToken->sVal[iOutLength] = '\n';
            iOutLength++;
            m_pReadHead++;
            bInsideSlashes = true;
        }
        else if (*m_pReadHead == '\r')
        {
            m_pReadHead++;
        }
        else
        {
            pToken->sVal[iOutLength] = *m_pReadHead;
            iOutLength++;
            m_pReadHead++;
        }
    }

    pToken->sVal[iOutLength] = 0;

    RemoveNewLinesAfterBackSlashes(pToken->sVal);
    pToken->iVal = (int)strlen(pToken->sVal);

    SetOffset(iOffset, iLineNum);
}



enumTokenType Tokenizer::MustGetNextToken(Token *pToken, char const* pErrorString)
{
    enumTokenType eType = GetNextToken(pToken);

    Assert(eType != TOKEN_NONE, pErrorString);

    return eType;
}

char const* pSimpleInvisibleTokens[] =
{
    "NN_STR_MAKE",                
    "OPT_STR_MAKE",        
    "NN_STR_GOOD",                
    "OPT_STR_GOOD",    
    "FORMAT_STR",            
    "NN_STR_FREE",                
    "OPT_STR_FREE",            
    "PTR_PRE_NOTVALID",            
    "PTR_POST_NOTVALID",            
    "PTR_PRE_VALID",            
    "PTR_POST_VALID",                
    "NN_PTR_MAKE",                  
    "OPT_PTR_MAKE",                  
    "NN_PTR_GOOD",                      
    "OPT_PTR_GOOD",                  
    "NN_PTR_FREE",                      
    "OPT_PTR_FREE",        
    NULL
};

char const* pInvisibleTokensWithParensAndInt[] =
{
    "OPT_PTR_FREE_COUNT",         
    "OPT_PTR_FREE_BYTES",         
    "NN_PTR_FREE_COUNT",          
    "NN_PTR_FREE_BYTES",          
    "OPT_PTR_GOOD_COUNT",          
    "OPT_PTR_GOOD_BYTES",          
    "NN_PTR_GOOD_COUNT",          
    "NN_PTR_GOOD_BYTES",          
    "OPT_PTR_MAKE_COUNT",         
    "OPT_PTR_MAKE_BYTES", 
    "NN_PTR_MAKE_COUNT",      
    "NN_PTR_MAKE_BYTES",      
    "PTR_POST_VALID_COUNT",        
    "PTR_POST_VALID_BYTES",        
    "PTR_PRE_VALID_COUNT",
    "PTR_PRE_VALID_BYTES",    
    NULL
};






//given a token, returns true, and skips any "dangling" invisible tokens, if it's invisible
bool Tokenizer::CheckIfTokenIsInvisibleAndSkipRemainder(enumTokenType eType, Token *pToken)
{
    if (eType != TOKEN_IDENTIFIER)
    {
        return false;
    }

    if (StringIsInList(pToken->sVal, pSimpleInvisibleTokens))
    {
        return true;
    }

    if (StringIsInList(pToken->sVal, pInvisibleTokensWithParensAndInt))
    {
        enumTokenType eTempType;
        Token tempToken;
        eTempType = GetNextToken_internal(&tempToken);
        Assert(eTempType == TOKEN_RESERVEDWORD && tempToken.iVal == RW_LEFTPARENS, "Expected (");
        GetSpecialStringTokenWithParenthesesMatching(&tempToken);
        

        return true;
    }
    return false;
}

//wrapper around GetNextToken_internal which skips over any "invisible" tokens
enumTokenType Tokenizer::GetNextToken(Token *pToken)
{
    enumTokenType eType;

    while (1)
    {
        eType = GetNextToken_internal(pToken);

        if  (!CheckIfTokenIsInvisibleAndSkipRemainder(eType, pToken))
        {
            return eType;
        }
    }
}

enumTokenType Tokenizer::GetNextToken_internal(Token *pToken)
{
    while (1)
    {
        if (m_pReadHead >= m_pBufferEnd)
        {
            return TOKEN_NONE;
        }

        //skip white space
        if (IsWhiteSpace(*m_pReadHead))
        {
            if (*m_pReadHead == '\n')
            {
                m_bInsideCommentControlCode = false;
                m_iCurLineNum++;

            }
            m_pReadHead++;
            continue;
        }

        //skip /* */ comments
        if (*m_pReadHead == '/' && *(m_pReadHead + 1) == '*')
        {
            m_pReadHead += 2;

            while (m_pReadHead < m_pBufferEnd && !(*m_pReadHead == '*' && *(m_pReadHead + 1) == '/'))
            {
                if (*m_pReadHead == '\n')
                {
                    m_iCurLineNum++;
                }

                m_pReadHead++;
            }
            m_pReadHead += 2;

            continue;
        }

        //skip // comments and #defines (if requested)
        if (*m_pReadHead == '/' && *(m_pReadHead + 1) == '/' || (m_bSkipDefines && strncmp(m_pReadHead, "#define ", 8) == 0))
        {
            m_pReadHead += 2;

            while (m_pReadHead < m_pBufferEnd && !(*m_pReadHead == '\n'))
            {
                if (*m_pReadHead == '\n')
                {
                    m_iCurLineNum++;
                }

                if (m_bLookForControlCodeInComments && strncmp(m_pReadHead, sCommentControlCode, COMMENT_CONTROL_CODE_LENGTH) == 0)
                {
                    m_pReadHead += COMMENT_CONTROL_CODE_LENGTH;
                    m_bInsideCommentControlCode = true;
                    break;
                }

                m_pReadHead++;
            }

            continue;
        }

        //read strings
        if ((*m_pReadHead == '"' || *m_pReadHead == '\'') && !m_bIgnoreQuotes)
        {
            if (m_bInsideCommentControlCode)
            {
                m_pReadHead++;
                continue;
            }

            char cQuoteType = *m_pReadHead;
            m_pReadHead++;
            int iStrLen = 0;

            while (*m_pReadHead != cQuoteType)
            {

                if (*m_pReadHead == '\n')
                {
                    Assert(!m_bNoNewlinesInStrings, "Illegal linebreak found in string");
                    m_iCurLineNum++;
                }
            
                Assert(iStrLen < TOKENIZER_MAX_STRING_LENGTH, "String overflow");
                Assert(m_pReadHead < m_pBufferEnd, "unterminated string");

                if (m_bCSourceStyleStrings)
                {
                    if (*m_pReadHead == '\\')
                    {    
                        if (CheckForMultiLineCString())
                        {
                            continue;
                        }
                        else
                        {
                            pToken->sVal[iStrLen++] = *(m_pReadHead++);
                        }
                    }
                }
                pToken->sVal[iStrLen++] = *(m_pReadHead++);
            }

            pToken->sVal[iStrLen] = 0;

            pToken->eType = TOKEN_STRING;

            m_pReadHead++;

            m_LastStringLength = pToken->iVal = iStrLen;
            
//            printf("Found string <<%s>>\n", pToken->sVal);

            return TOKEN_STRING;
        }


        //read ints
        if (!m_bDontParseInts && FoundInt())
        {
            if (m_bInsideCommentControlCode)
            {
                m_pReadHead++;
                continue;
            }


            bool bNeg = false;
            int iVal = 0;

            while (*m_pReadHead == '-')
            {
                bNeg = !bNeg;
                m_pReadHead++;
            }

            Assert(IsDigit(*m_pReadHead) != 0, "Didn't find digit after minus sign");

            while (IsDigit(*m_pReadHead))
            {
                iVal *= 10;
                iVal += *m_pReadHead - '0';
                m_pReadHead++;
            }

            pToken->iVal = bNeg ? -iVal : iVal;
            pToken->eType = TOKEN_INT;
            return TOKEN_INT;
        }

        //check for two-digit reserved word
        int i;
        for (i=RW_NONE + 1; i < FIRST_SINGLECHAR_RW; i++)
        {
            if (*m_pReadHead == sReservedWords[i][0] && *(m_pReadHead + 1) == sReservedWords[i][1])
            {
                m_pReadHead++;
                m_pReadHead++;
                pToken->iVal = i;
                pToken->eType = TOKEN_RESERVEDWORD;
                return TOKEN_RESERVEDWORD;
            }
        }

        if ((i = sSingleCharReservedWordIndex[*m_pReadHead]))
        {
            m_pReadHead++;
            pToken->iVal = i;
            pToken->eType = TOKEN_RESERVEDWORD;
            return TOKEN_RESERVEDWORD;
        }

#if 0
        //check for single-digit reserved word
        for (i=FIRST_SINGLECHAR_RW; i < FIRST_MULTICHAR_RW; i++)
        {
            if (*m_pReadHead == sReservedWords[i][0])
            {
                m_pReadHead++;
                pToken->iVal = i;
                pToken->eType = TOKEN_RESERVEDWORD;
                return TOKEN_RESERVEDWORD;
            }
        }
#endif

        //read "normal" token
        if (IsOKForIdentStart(*m_pReadHead) || m_bDontParseInts && IsDigit(*m_pReadHead))
        {
            int iIdentifierLength = 0;

            while (IsOKForIdent(*m_pReadHead))
            {
                Assert(iIdentifierLength < TOKENIZER_MAX_STRING_LENGTH, "Identifier too long");

                pToken->sVal[iIdentifierLength++] = *(m_pReadHead++);
            }
            
            pToken->sVal[iIdentifierLength] = 0;


            for (i=FIRST_MULTICHAR_RW; i < RW_COUNT; i++)
            {
                if (strcmp(pToken->sVal, sReservedWords[i]) == 0)
                {
                    pToken->iVal = i;
                    pToken->eType = TOKEN_RESERVEDWORD;
                    return TOKEN_RESERVEDWORD;
                }
            }



            if (m_ppExtraReservedWords)
            {
                i = 0;

                while (m_ppExtraReservedWords[i])
                {
                    if (strcmp(pToken->sVal, m_ppExtraReservedWords[i]) == 0)
                    {
                        pToken->iVal = i + RW_COUNT;
                        pToken->eType = TOKEN_RESERVEDWORD;
                        return TOKEN_RESERVEDWORD;
                    }

                    i++;
                }
            }

            m_LastStringLength = iIdentifierLength;


            pToken->eType = TOKEN_IDENTIFIER;
            pToken->iVal = iIdentifierLength;
            return TOKEN_IDENTIFIER;
        }

        //ignore unrecognized characters
        m_pReadHead++;
    }
}

enumTokenType Tokenizer::CheckNextToken(Token *pToken)
{
    char *pReadHead = m_pReadHead;
    int iLineNum = m_iCurLineNum;

    enumTokenType eType = GetNextToken(pToken);

    m_iCurLineNum = iLineNum;
    m_pReadHead = pReadHead;

    return eType;
}

void Tokenizer::AssertNextTokenTypeAndGet(Token *pToken, enumTokenType eExpectedType, int iExpectedAuxType, char const* pErrorString)
{
    enumTokenType eType;

    eType = GetNextToken(pToken);

    Assert(eType == eExpectedType && (eType != TOKEN_RESERVEDWORD || iExpectedAuxType == pToken->iVal), pErrorString);

    if ((eType == TOKEN_STRING || eType == TOKEN_IDENTIFIER) && iExpectedAuxType)
    {
        Assert(GetLastStringLength() < iExpectedAuxType - 1, pErrorString);
    }
}

void Tokenizer::Assert2NextTokenTypesAndGet(Token *pToken, enumTokenType eExpectedType1, int iExpectedAuxType1, enumTokenType eExpectedType2, int iExpectedAuxType2, char const* pErrorString)
{
    enumTokenType eType;

    eType = GetNextToken(pToken);

    Assert(eType == eExpectedType1 && (eType != TOKEN_RESERVEDWORD || iExpectedAuxType1 == pToken->iVal)
        || eType == eExpectedType2 && (eType != TOKEN_RESERVEDWORD || iExpectedAuxType2 == pToken->iVal), pErrorString);

    if ((eType == TOKEN_STRING || eType == TOKEN_IDENTIFIER) && eType == eExpectedType1 && iExpectedAuxType1)
    {
        Assert(GetLastStringLength() < iExpectedAuxType1, pErrorString);
    }
    
    if ((eType == TOKEN_STRING || eType == TOKEN_IDENTIFIER) && eType == eExpectedType2 && iExpectedAuxType2)
    {
        Assert(GetLastStringLength() < iExpectedAuxType2, pErrorString);
    }
}
void Tokenizer::AssertGetIdentifier(char const* pIdentToGet)
{
    enumTokenType eType;
    Token token;
    char errorString[TOKENIZER_MAX_STRING_LENGTH];

    sprintf(errorString, "Expected %s", pIdentToGet);

    eType = GetNextToken(&token);

    Assert(eType == TOKEN_IDENTIFIER, errorString);
    Assert(strcmp(pIdentToGet, token.sVal) == 0, errorString);
}



bool Tokenizer::IsStringAtVeryEndOfBuffer(char const* pString)
{
    int len = (int)strlen(pString);

    char *pTemp = m_pBufferEnd;

    while (!IsAlphaNum(*pTemp))
    {
        pTemp--;
    }

    return (strncmp(pString, pTemp - len + 1, len) == 0);
}


void Tokenizer::Assertf(bool bExpression, char const* pErrorString, ...)
{
    char buf[1000] = "";
    va_list ap;

    va_start(ap, pErrorString);
    if (pErrorString)
    {
        vsprintf(buf, pErrorString, ap);
    }
    va_end(ap);

    Assert(bExpression, buf);
}

#define NUM_ASSERT_CHARACTERS_TO_DUMP 25
void Tokenizer::Assert(bool bExpression, char const* pErrorString)
{
    if (!bExpression)
    {
        printf("%s(%d) : error S0000 : (StructParser) %s\n", m_CurFileName, m_iCurLineNum, pErrorString);
        printf("String Before Error:<<<");


        char *pTemp;

        for (pTemp = m_pReadHead - NUM_ASSERT_CHARACTERS_TO_DUMP; pTemp < m_pReadHead;  pTemp++)
        {
            if (pTemp >= m_pBufferStart)
            {
                printf("%c", *pTemp);
            }
        }

        printf(">>>\n\n\n\n\n");
    
        fflush(stdout);

        Sleep(100);

        exit(1);
    }
}

void Tokenizer::StaticAssert(bool bExpression, char const* pErrorString)
{
    if (!bExpression)
    {
        printf("ERROR:%s\n", pErrorString);
    
    
        fflush(stdout);

        Sleep(100);

        exit(1);
    }
}

void Tokenizer::StaticAssertf(bool bExpression, char const* pErrorString, ...)
{
    char buf[1000] = "";
    va_list ap;

    va_start(ap, pErrorString);
    if (pErrorString)
    {
        vsprintf(buf, pErrorString, ap);
    }
    va_end(ap);

    StaticAssert(bExpression, buf);
}




void Tokenizer::DumpToken(Token *pToken)
{
    switch(pToken->eType)
    {
    case TOKEN_INT:
        printf("INT %d\n", pToken->iVal);
        break;

    case TOKEN_RESERVEDWORD:
        printf("RW %s\n", sReservedWords[pToken->iVal]);
        break;

    case TOKEN_IDENTIFIER:
        printf("IDENTIFIER %s\n", pToken->sVal);
        break;
    }
}

bool Tokenizer::FoundInt()
{
    char *pTemp = m_pReadHead;

    while (*pTemp == '-')
    {
        pTemp++;
    }

    if (IsDigit(*pTemp))
    {
        return true;
    }

    return false;
}

void Tokenizer::GetTokensUntilReservedWord(enumReservedWordType eReservedWord)
{
    Token token;
    enumTokenType eType;

    do
    {
        eType = GetNextToken(&token);

        Assert(eType != TOKEN_NONE, "Found EOF while looking for reserved word");
    } while (!(eType == TOKEN_RESERVEDWORD && token.iVal == eReservedWord));
}

void Tokenizer::StringifyToken(Token *pToken)
{
    switch (pToken->eType)
    {
    case TOKEN_INT:
        sprintf(pToken->sVal, "%d", pToken->iVal);
        break;

    case TOKEN_RESERVEDWORD:
        if (pToken->iVal >= RW_COUNT)
        {
            strcpy(pToken->sVal, m_ppExtraReservedWords[pToken->iVal - RW_COUNT]);
        }
        else
        {
            strcpy(pToken->sVal, sReservedWords[pToken->iVal]);
        }
        break;
    }
}

//finds the next unmatched right parentheses (using normal token logic). Then returns every character from the current read head up until that
//character as a string, even if there are no quotes or anything
void Tokenizer::GetSpecialStringTokenWithParenthesesMatching(Token *pToken)
{
    char *pStartingReadHead = m_pReadHead;
    int iCurDepth = 0;
    
    Token token;
    enumTokenType eType;

    do
    {
        eType = GetNextToken(&token);

        Assert(eType != TOKEN_NONE, "Never found matching ) during special token search");

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTPARENS)
        {
            if (iCurDepth == 0)
            {
                pToken->eType = TOKEN_STRING;
                int iLen = (int)((m_pReadHead - pStartingReadHead) - 1);

                Assert(iLen < TOKENIZER_MAX_STRING_LENGTH, "Never found matching ) during special token search, or buffer overflowed");
                memcpy(pToken->sVal, pStartingReadHead, iLen);
                pToken->sVal[iLen] = 0;

                pToken->iVal = iLen;

                return;
            }
            else
            {
                iCurDepth--;
            }
        }
        else if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTPARENS)
        {
            iCurDepth++;
        }
    } while (1);
}

void Tokenizer::ResetLineNumberCounter(void)
{
    m_iCurLineNum = m_iStartingLineNum;
    char *pCounter;

    for (pCounter = m_pBufferStart; pCounter < m_pReadHead; pCounter++)
    {
        if (*pCounter == '\n')
        {
            m_iCurLineNum++;
        }
    }
}


bool Tokenizer::SetReadHeadAfterString(char *pString)
{
    char *pFoundString = strstr(m_pBufferStart, pString);

    if (pFoundString)
    {
        m_pReadHead = pFoundString + strlen(pString);
        ResetLineNumberCounter();
        return true;
    }
    else
    {
        return false;
    }
}

void Tokenizer::AssertGetBracketBalancedBlock(enumReservedWordType leftRW, enumReservedWordType rightRW, char const* pErrorString)
{
    Token token;
    enumTokenType eType;
    int iDepth = 1;

    AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, leftRW, pErrorString);

    do
    {
        eType = GetNextToken(&token);

        Assert(eType != TOKEN_NONE, pErrorString);

        if (eType == TOKEN_RESERVEDWORD && token.iVal == leftRW)
        {
            iDepth++;
        }
        else if (eType == TOKEN_RESERVEDWORD && token.iVal == rightRW)
        {
            iDepth--;
        }
    }
    while (iDepth > 0);
}

int Tokenizer::GetCurBraceDepth(void)
{
    int iCurOffset;
    int iCurLineNum;

    int iDepth = 0;

    enumTokenType eType;
    Token token;

    iCurOffset = GetOffset(&iCurLineNum);

    char *pStartingReadHead = m_pReadHead;

    m_pReadHead = m_pBufferStart;

    while (m_pReadHead  < pStartingReadHead)
    {
        eType = GetNextToken(&token);

        if (eType == TOKEN_RESERVEDWORD)
        {
            if (token.iVal == RW_RIGHTBRACE)
            {
                iDepth--;
            } 
            else if (token.iVal == RW_LEFTBRACE)
            {
                iDepth++;
            }
        }
    }

    SetOffset(iCurOffset, iCurLineNum);

    return iDepth;
}



