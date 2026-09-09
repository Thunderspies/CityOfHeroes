#include "pch.h"
#include "IdentifierDictionary.h"
#include <cstdio>
#include "strutils.h"
#include "utils.h"



IdentifierDictionary::IdentifierDictionary()
{
    m_pFirst = NULL;
    m_eCurIteratorType = IDENTIFIER_NONE;
    m_pNextIteratorNode = NULL;
}

IdentifierDictionary::~IdentifierDictionary()
{
    while (m_pFirst)
    {
        DeleteNode(&m_pFirst);
    }
}

void IdentifierDictionary::DeleteNode(IdentifierDictionaryNode **ppNode)
{
    delete [] (*ppNode)->pIdentifierName;
    delete [] (*ppNode)->pSourceFileName;

    IdentifierDictionaryNode *pNext = (*ppNode)->pNext;

    delete *ppNode;
    *ppNode = pNext;
}



void IdentifierDictionary::AddIdentifier(char const* pIdentifierName, char const* pSourceFileName, enumIdentifierType eType)
{
    IdentifierDictionaryNode *pNode = new IdentifierDictionaryNode;

    Tokenizer::StaticAssert(pNode != NULL, "new failed");

    pNode->pIdentifierName = new char[strlen(pIdentifierName) + 1];
    pNode->pSourceFileName = new char[strlen(pSourceFileName) + 1];

    Tokenizer::StaticAssert(pNode->pIdentifierName && pNode->pSourceFileName, "new failed");

    strcpy(pNode->pIdentifierName, pIdentifierName);
    strcpy(pNode->pSourceFileName, pSourceFileName);
    pNode->eType = eType;

    pNode->pNext = m_pFirst;
    m_pFirst = pNode;

}


enumIdentifierType IdentifierDictionary::FindIdentifier(char const* pIdentifierName)
{
    IdentifierDictionaryNode *pNode = m_pFirst;

    while (pNode)
    {
        if (AreFilenamesEqual(pNode->pIdentifierName, pIdentifierName))
        {
            return pNode->eType;
        }

        pNode = pNode->pNext;
    }

    return IDENTIFIER_NONE;
}

enumIdentifierType IdentifierDictionary::FindIdentifierAndGetSourceFile(char const* pIdentifierName, char* pOutSourceFileName)
{
    IdentifierDictionaryNode *pNode = m_pFirst;

    while (pNode)
    {
        if (AreFilenamesEqual(pNode->pIdentifierName, pIdentifierName))
        {
            strcpy(pOutSourceFileName, pNode->pSourceFileName);
            return pNode->eType;
        }

        pNode = pNode->pNext;
    }

    pOutSourceFileName[0] = 0;
    return IDENTIFIER_NONE;




}

enumIdentifierType IdentifierDictionary::FindIdentifierAndGetSourceFilePointer(char const* pIdentifierName, char **ppOutSourceFileName)
{
    IdentifierDictionaryNode *pNode = m_pFirst;

    while (pNode)
    {
        if (AreFilenamesEqual(pNode->pIdentifierName, pIdentifierName))
        {
            *ppOutSourceFileName = pNode->pSourceFileName;
            return pNode->eType;
        }

        pNode = pNode->pNext;
    }

    *ppOutSourceFileName = NULL;
    return IDENTIFIER_NONE;
}


char *IdentifierDictionary::GetSourceFileForIdentifier(char *pIdentifier)
{
    IdentifierDictionaryNode *pNode = m_pFirst;

    while (pNode)
    {
        if (AreFilenamesEqual(pNode->pIdentifierName, pIdentifier))
        {
            return pNode->pSourceFileName;
        }

        pNode = pNode->pNext;
    }

    return NULL;
}

    
void IdentifierDictionary::BeginIdentifierIterating(enumIdentifierType eType)
{
    m_eCurIteratorType = eType;
    m_pNextIteratorNode = m_pFirst;
}

char *IdentifierDictionary::GetNextIdentifier()
{
    while (m_pNextIteratorNode && m_pNextIteratorNode->eType != m_eCurIteratorType)
    {
        m_pNextIteratorNode = m_pNextIteratorNode->pNext;
    }

    if (m_pNextIteratorNode)
    {
        char *pRetVal = m_pNextIteratorNode->pIdentifierName;
        m_pNextIteratorNode = m_pNextIteratorNode->pNext;
        return pRetVal;
    }

    return NULL;
}


