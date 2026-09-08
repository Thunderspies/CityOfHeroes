#include "utilitieslib/stdtypes.h"
#include "../include/ailib/aiExpression.h"

#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/components//earray.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/components/StringCache.h"
#include "utilitieslib/utils/structTokenizer.h"

#define IS_WHITESPACE(c) (c == ' ' || c == '\t' || c == '\n' || c =='\r')

MP_DEFINE(MultiVal);

MultiVal* MultiValCreate()
{
    MP_CREATE(MultiVal, 128);
    return MP_ALLOC(MultiVal);
}

void MultiValDestroy(void* valData)
{
    MultiVal* val = (MultiVal*)valData;
    MP_FREE(MultiVal, val);
}

void MultiValArrayDestroy(MultiValArray* array)
{
    eaDestroyEx(array, MultiValDestroy);
}

void acceptString(char* str, MultiValArray* result)
{
    MultiVal* curVal = MultiValCreate();

    curVal->type = MULTI_STRING;
    curVal->ptr = allocAddString(str);

    eaPush(result, curVal);
}

void acceptIdentifier(char* str, MultiValArray* result)
{
    int i;
    MultiVal* curVal = MultiValCreate();
    const char* inStr = allocAddString(str);
    static const char* identifiers[MULTIOP_WORDOP_END - MULTIOP_WORDOP_START];
    static int initted = false;

    if(!initted)
    {
        int count = MULTIOP_WORDOP_START;
#define REGISTER_IDENTIFIER(enum, str) \
    devassertmsg(count == enum, "You must add all identifiers and add them in order"); \
    identifiers[count - MULTIOP_WORDOP_START] = allocAddString(str); \
    count++;

        REGISTER_IDENTIFIER(MULTIOP_AND, "and");
        REGISTER_IDENTIFIER(MULTIOP_OR, "or");
        REGISTER_IDENTIFIER(MULTIOP_NOT, "not");

        devassertmsg(count == MULTIOP_WORDOP_END, "Missing word operator at end of identifiers");
    }

    for(i = 0; i < MULTIOP_WORDOP_END - MULTIOP_WORDOP_START; i++)
    {
        if(inStr == identifiers[i])
        {
            curVal->type = MULTIOP_WORDOP_START + i;
            eaPush(result, curVal);
            return;
        }
    }

    curVal->ptr = inStr;
    curVal->type = MULTI_IDENTIFIER;
    eaPush(result, curVal);
    return;
}

void acceptNumber(char* str, MultiValArray* result)
{
    MultiVal* curVal = MultiValCreate();

    if(strchr(str, '.'))
    {
        curVal->floatval = atof(str);
        curVal->type = MULTI_FLOAT;
    }
    else
    {
        curVal->intval = atol(str);
        curVal->type = MULTI_INT;
    }

    eaPush(result, curVal);
}

void acceptOperator(char* str, MultiValArray* result)
{

    while(str && str[0])
    {
        MultiVal* curVal;
        int topIdx;

#define ADD_OPERATOR(opType) \
    curVal = MultiValCreate(); \
    curVal->type = opType; \
    eaPush(result, curVal); \
    str++;

        switch(str[0])
        {
        xcase '+':
            ADD_OPERATOR(MULTIOP_ADD);
        xcase '-':
            curVal = MultiValCreate();
            topIdx = eaSize(result)-1;
            if((*result)[topIdx]->type == MULTI_INT || (*result)[topIdx]->type == MULTI_FLOAT)
                curVal->type = MULTIOP_SUBTRACT;
            else
                curVal->type = MULTIOP_NEGATE;
            eaPush(result, curVal);
            str++;
        xcase '*':
            ADD_OPERATOR(MULTIOP_MULTIPLY);
        xcase '/':
            ADD_OPERATOR(MULTIOP_DIVIDE);
        xcase '^':
            ADD_OPERATOR(MULTIOP_EXPONENT);
        xcase '.':
            ADD_OPERATOR(MULTIOP_FIELDACCESS);
        xcase '(':
            ADD_OPERATOR(MULTIOP_PAREN_OPEN);
        xcase ')':
            ADD_OPERATOR(MULTIOP_PAREN_CLOSE);
        xcase '=':
            ADD_OPERATOR(MULTIOP_EQUALITY);
        xcase '>':
            if(str[1] == '=')
            {
                ADD_OPERATOR(MULTIOP_GREATERTHANEQUALS);
                str++; // get rid of the = too
            }
            else
            {
                ADD_OPERATOR(MULTIOP_GREATERTHAN);
            }
        xcase '<':
            if(str[1] == '=')
            {
                ADD_OPERATOR(MULTIOP_LESSTHANEQUALS);
                str++; // get rid of the = too
            }
            else
            {
                ADD_OPERATOR(MULTIOP_LESSTHAN);
            }
        xdefault:
            devassertmsg(0, "Token not recognized");
            str++;
        }
    }
}

typedef enum 
{
    IN_NOTHING,
    IN_IDENTIFIER,
    IN_STRING,
    IN_NUMBER,
    IN_OPERATOR,
} TokenizerState;

void aiExprTokenize(char* str, MultiValArray* result)
{
    TokenizerState state = IN_NOTHING;
    char buf[MAX_STRING_LENGTH];
    int bufpos = 0;

    // "pe.prune(team="FOO").count > 2 and 2*4+3 < 20"
    while (str && str[0])
    {
        switch (state)
        {
        case IN_NOTHING:
            if(IS_WHITESPACE(str[0]))
                str++;
            else if (isalpha(str[0]))
                state = IN_IDENTIFIER;
            else if (str[0] == '"')
            {
                str++;
                state = IN_STRING;
            }
            else if (str[0] >= '0' && str[0] <= '9')
                state = IN_NUMBER;
            else // assume anything else weird is an operator
                state = IN_OPERATOR;
            break;
        case IN_STRING:
            while(str[0] && str[0] != '"')
                buf[bufpos++] = *str++;
            if(str[0]) // eat the ending "
                str++;

            buf[bufpos] = 0;
            acceptString(buf, result);
            bufpos = 0;
            state = IN_NOTHING;
            break;
        case IN_IDENTIFIER:
            while(str[0] && (isalpha(str[0]) || str[0] == '_'))
                buf[bufpos++] = *str++;

            buf[bufpos] = 0;
            acceptIdentifier(buf, result);
            bufpos = 0;
            state = IN_NOTHING;
            break;
        case IN_NUMBER:
            while(str[0] && (isdigit(str[0]) || str[0] == '.'))
                buf[bufpos++] = *str++;

            buf[bufpos] = 0;
            acceptNumber(buf, result);
            bufpos = 0;
            state = IN_NOTHING;
            break;
        case IN_OPERATOR:
            // accepts "<=-" as part of "foo <=-10"
            while(str[0] && !isalnum(str[0]) && !IS_WHITESPACE(str[0]) && str[0] != '"')
                buf[bufpos++] = *str++;

            buf[bufpos] = 0;
            acceptOperator(buf, result); // deals with negate vs. minus
            bufpos = 0;
            state = IN_NOTHING;
        }
    }
}