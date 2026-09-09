#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include "assert.h"
#include "Platform.h"
#include "strutils.h"


#define TOKENIZER_MAX_STRING_LENGTH 16384

typedef enum
{
    TOKEN_NONE,
    TOKEN_INT,
    TOKEN_RESERVEDWORD,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
} enumTokenType;

typedef enum
{
    RW_NONE,

    //first come two-character punctuation
    RW_ARROW,

    //then one-character punctuation
    RW_LEFTBRACE,
    RW_RIGHTBRACE,
    RW_LEFTPARENS,
    RW_RIGHTPARENS,
    RW_LEFTBRACKET,
    RW_RIGHTBRACKET,
    RW_COMMA,
    RW_COLON,
    RW_MINUS,
    RW_SEMICOLON,
    RW_PLUS,
    RW_SLASH,
    RW_NOT,
    RW_AMPERSAND,
    RW_ASTERISK,
    RW_CARET,
    RW_EQUALS,
    RW_DOT,
    RW_GT,
    RW_LT,
    RW_QM,
    RW_TILDE,
    RW_OR,

    //then words
    RW_STRUCT,
    RW_TYPEDEF,
    RW_VOID,
    RW_ENUM,

    RW_COUNT,
} enumReservedWordType;

#define FIRST_SINGLECHAR_RW RW_LEFTBRACE
#define FIRST_MULTICHAR_RW RW_STRUCT




typedef struct
{
    enumTokenType eType;
    int iVal;
    char sVal[TOKENIZER_MAX_STRING_LENGTH + 1];
} Token;

class Tokenizer
{
public:
    Tokenizer();
    ~Tokenizer();

    enumTokenType GetNextToken(Token *pToken);
    enumTokenType CheckNextToken(Token *pToken);

    //like GetNextToken, but asserts on TOKEN_NONE
    enumTokenType MustGetNextToken(Token *pToken, char const* pErrorString);

    //iAuxType is the reserved word to find. It's also the maximum string length for string/identifier types (strlen must be < )
    void AssertNextTokenTypeAndGet(Token *pToken, enumTokenType eType, int iAuxType, char const* pErrorString);
    void AssertfNextTokenTypeAndGet(Token *pToken, enumTokenType eType, int iAuxType, char const* pErrorString, ...);

    void Assert2NextTokenTypesAndGet(Token *pToken, enumTokenType eType1, int iAuxType1, enumTokenType eType2, int iAuxType2, char const* pErrorString);


    //eat all the tokens that start with an open bracket and end with a close bracket. Bracketing chars are passed in
    //(presumably [ ] ( ) or { }
    void AssertGetBracketBalancedBlock(enumReservedWordType leftRW, enumReservedWordType rightRW, char const* pErrorString);

    void AssertGetIdentifier(char const* pIdentToGet);

    void LoadFromBuffer(char *pBuffer, int iSize, char const* pFileName, int iStartingLineNum);

    void DumpToken(Token *pToken);

    void SetLookForControlCodeInComments(bool bSet) { m_bLookForControlCodeInComments = bSet; }

    bool IsInsideCommentControlCode() { return m_bInsideCommentControlCode; }

    void Assert(bool bExpression, char const* pErrorString);
    void Assertf(bool bExpression, char const* pErrorString, ...);
    static void StaticAssert(bool bExpression, char const* pErrorString);
    static void StaticAssertf(bool bExpression, char const* pErrorString, ...);

    void SaveLocation() { m_pSavedReadHead = m_pReadHead; m_iSavedLineNum = m_iCurLineNum;}
    void RestoreLocation() { m_pReadHead = m_pSavedReadHead; m_iCurLineNum = m_iSavedLineNum; }

    void SetExtraReservedWords(char const** ppWords) { m_ppExtraReservedWords = ppWords; }

    bool LoadFromFile(char const* pFileName);

    void SetCSourceStyleStrings(bool bSet) { m_bCSourceStyleStrings = bSet; }


    int GetLastStringLength() { return m_LastStringLength; }; //returns the length of the last string token found (to avoid wasted time)

    int GetOffset(int *pLineNum) { if (pLineNum) { *pLineNum = m_iCurLineNum; } return (int)(m_pReadHead - m_pBufferStart); }
    void SetOffset(int iOffset, int iLineNum) { m_pReadHead = m_pBufferStart + iOffset; m_iCurLineNum = iLineNum; }

    //keeps getting tokens until it has gotten the specified reserved word. Asserts on EOF.
    void GetTokensUntilReservedWord(enumReservedWordType eReservedWord);

    //if the token doesn't already have an sval, sets one (via sprintfing the ival into it, or whatever is type-appropriate)
    void StringifyToken(Token *pToken);

    char *GetCurFileName() { return m_CurFileName; }
    int GetCurLineNum() { return m_iCurLineNum; }

//finds the next unmatched right parentheses (using normal token logic). Then returns every character from the current read head up until that
//character as a string, even if there are no quotes or anything
    void GetSpecialStringTokenWithParenthesesMatching(Token *pToken);

    void SetNoNewlinesInStrings(bool bSet) { m_bNoNewlinesInStrings = bSet; }

    void GetSurroundingSlashedCommentBlock(Token *pToken);

    void SetDontParseInts(bool bSet) { m_bDontParseInts = bSet; }

    //searches the entire loaded buffer for the given string, if it finds it, sets the read head
    //right after it and returns true. Otherwise returns false. 
    bool SetReadHeadAfterString(char *pString);

    //gets a pointer to the current read head. useful if you need to do totally custom parsing of some sort
    char *GetReadHead() { return m_pReadHead; }

    //if true, the tokenizer skips entirely over #define and the rest of that line
    void SetSkipDefines(bool bSet) { m_bSkipDefines = bSet; }

    //sets characters which temporarily count as OK for identifiers (so, for instance,
    //you can say that for a certainly application, this:is:one:identifier would in fact be one identifier
    //
    //NULL or "" to clear
    void SetExtraCharsAllowedInIdentifiers(char const* pStringOfChars);

    //backs the read head up to the beginning of the current line of text
    void BackUpToBeginningOfLine();

    //counts the { and } from the beginning of the file, returns the current depth
    int GetCurBraceDepth();

    void SetIgnoreQuotes(bool bSet) { m_bIgnoreQuotes = bSet; }

private:
    bool m_bLookForControlCodeInComments;
    char *m_pBufferStart;
    char *m_pBufferEnd;
    char *m_pReadHead;
    
    bool m_bInsideCommentControlCode;

    char *m_pSavedReadHead;

    char const** m_ppExtraReservedWords;

    bool m_bOwnsBuffer;

    bool m_bCSourceStyleStrings;

    int m_LastStringLength;

    char m_CurFileName[MAX_PATH];
    int m_iCurLineNum;
    int m_iSavedLineNum;
    int m_iStartingLineNum;

    bool m_bNoNewlinesInStrings;

    bool m_bDontParseInts;

    bool m_bSkipDefines;

    char *m_pExtraIdentifierChars;

    //if true, " and ' will be skipped over, result in no TOKEN_STRINGs ever being returned
    bool m_bIgnoreQuotes;

private: //funcs
    bool FoundInt();
    void Reset();

    //if this function is called, we just hit a \, and want to return true if there is nothing but optional whitespace then a newline
    bool CheckForMultiLineCString();
    void AdvanceToBeginningOfLine();
    bool DoesLineBeginWithComment();
    void ResetLineNumberCounter(void);
    enumTokenType GetNextToken_internal(Token *pToken);
    bool CheckIfTokenIsInvisibleAndSkipRemainder(enumTokenType eType, Token *pToken);


    bool IsOKForIdent(char c)
    {
        return IsAlphaNum(c) || c == '_' 
            || (m_pExtraIdentifierChars && strchr(m_pExtraIdentifierChars, c))
            || m_bIgnoreQuotes && (c == '"' || c == '\'');
    }

    bool IsOKForIdentStart(char c)
    {
        return IsAlpha(c) || c == '_' 
            || (m_pExtraIdentifierChars && strchr(m_pExtraIdentifierChars, c))
            || m_bIgnoreQuotes && (c == '"' || c == '\'');;
    }
};



#endif





