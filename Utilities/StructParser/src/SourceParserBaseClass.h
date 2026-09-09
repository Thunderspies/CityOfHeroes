#ifndef _SOURCEPARSER_BASECLASS_H_
#define _SOURCEPARSER_BASECLASS_H_

#include "tokenizer.h"
#include "ParserLimits.h"

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


    virtual bool WriteOutData(void) = 0;

    virtual char const* GetMagicWord(int iWhichMagicWord) = 0;

    //note that iWhichMagicWord can be MAGICWORD_BEGINING_OF_FILE or MAGICWORD_END_OF_FILE
    virtual void FoundMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer, int iWhichMagicWord, char const* pMagicWordString) = 0;

	// Resolve borrowed source data after all inputs are scanned.
	virtual void ProcessDataSingleFile(char const* pSourceFileName) {}

	// Borrow the owning parser for this emitter's lifetime.
	void SetParent(SourceParser *parent) { m_pParent = parent; }


protected:
    SourceParser *m_pParent;
};

#endif
