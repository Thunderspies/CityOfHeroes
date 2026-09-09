#ifndef _IDENTIFIERDICTIONARY_H_
#define _IDENTIFIERDICTIONARY_H_

#include "tokenizer.h"
#include "Platform.h"


typedef enum
{
    IDENTIFIER_NONE,
    IDENTIFIER_ENUM,
    IDENTIFIER_STRUCT,

    //structs that are used as containers are treated as a different identifier type, but are structs
    //for many purposes, so don't forget to check this type if appropriate
    IDENTIFIER_STRUCT_CONTAINER,
} enumIdentifierType;

class IdentifierDictionary
{
public:
    IdentifierDictionary();
    ~IdentifierDictionary();



    void AddIdentifier(char const* pIdentifierName, char const* pSourceFileName, enumIdentifierType eType);


    enumIdentifierType FindIdentifier(char const* pIdentifierName);
    enumIdentifierType FindIdentifierAndGetSourceFile(char const* pIdentifierName, char* pOutSourceFileName);
    enumIdentifierType FindIdentifierAndGetSourceFilePointer(char const* pIdentifierName, char** ppOutSourceFileName);

    char *GetSourceFileForIdentifier(char *pIdentifier);

    void BeginIdentifierIterating(enumIdentifierType eType);
    char *GetNextIdentifier();

private:
    typedef struct IdentifierDictionaryNode
    {
        char *pIdentifierName;
        char *pSourceFileName;
        enumIdentifierType eType;
        struct IdentifierDictionaryNode *pNext;
    } IdentifierDictionaryNode;


    IdentifierDictionaryNode *m_pFirst;


    enumIdentifierType m_eCurIteratorType;
    IdentifierDictionaryNode *m_pNextIteratorNode;


private:
    void DeleteNode(IdentifierDictionaryNode **ppNode);
};



#endif