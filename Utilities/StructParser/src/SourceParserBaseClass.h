#ifndef _SOURCEPARSER_BASECLASS_H_
#define _SOURCEPARSER_BASECLASS_H_

#include "tokenizer.h"
#include "FileListLoader.h"

#define MAGICWORD_BEGINNING_OF_FILE -1
#define MAGICWORD_END_OF_FILE -2

//generic useful max string length
#define MAX_NAME_LENGTH 128


class SourceParser;

class SourceParserBaseClass
{
public:
    SourceParserBaseClass();
    virtual ~SourceParserBaseClass();

    virtual void SetProjectPathAndName(char const* srcPath, char const* commonPath, char const* projectName) = 0;

    virtual bool LoadStoredData(bool bForceReset) = 0;

    virtual void ResetSourceFile(char const* pSourceFileName) = 0;

    virtual bool WriteOutData(void) = 0;

    virtual char const* GetMagicWord(int iWhichMagicWord) = 0;

    //note that iWhichMagicWord can be MAGICWORD_BEGINING_OF_FILE or MAGICWORD_END_OF_FILE
    virtual void FoundMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer, int iWhichMagicWord, char const* pMagicWordString) = 0;

    //returns number of dependencies found
    virtual int ProcessDataSingleFile(char const* pSourceFileName, char* pDependencies[MAX_DEPENDENCIES_SINGLE_FILE]) = 0;

    void SetParent(SourceParser *pParent, int iIndex) { m_pParent = pParent; m_iIndexInParent = iIndex;};

    virtual bool DoesFileNeedUpdating(char const* pFileName) = 0;

    virtual char *GetAutoGenCFileName(void);
    virtual char *GetAutoGenCPPFileName(void);

protected:
    SourceParser *m_pParent;
    int m_iIndexInParent;
};

#endif