#ifndef _STRUCTPARSER_H_
#define _STRUCTPARSER_H_

#include <cstdio>
#include "tokenizer.h"
#include "Platform.h"
#include "SourceParserBaseClass.h"


#define MAX_FIELDS 256
#define MAX_STRUCTS 512

#define MAX_ENUMS 512

#define MAX_REDUNDANT_NAMES_PER_FIELD 4

#define MAX_IGNORES 32

#define MAX_ENUM_EXTRA_NAMES 12

#define MAX_WIKI_COMMENTS 16

#define MAX_MACROS 128

#define MAX_USER_FLAGS 8

#define MAX_UNIONS 16


typedef enum
{
    DATATYPE_NONE,
    DATATYPE_INT,
    DATATYPE_FLOAT,
    DATATYPE_CHAR,
    DATATYPE_STRUCT,
    DATATYPE_UNOWNEDSTRUCT,
    DATATYPE_LINK,
    DATATYPE_ENUM,
    DATATYPE_VOID,
    DATATYPE_VEC2,
    DATATYPE_VEC3,
    DATATYPE_VEC4,
    DATATYPE_IVEC2,
    DATATYPE_IVEC3,
    DATATYPE_IVEC4,
    DATATYPE_MAT3,
    DATATYPE_MAT4,
    DATATYPE_QUAT,
    DATATYPE_MULTIVAL,

    DATATYPE_STASHTABLE,
    DATATYPE_TOKENIZERPARAMS,
    DATATYPE_TOKENIZERFUNCTIONCALL,

    DATATYPE_REFERENCE,


    DATATYPE_UNKNOWN,

    //special data types that are only set after processing various commands
    DATATYPE_FILENAME,
    DATATYPE_CURRENTFILE,
    DATATYPE_TIMESTAMP,
    DATATYPE_LINENUM,
    DATATYPE_FLAGS,
    DATATYPE_BOOLFLAG,
    DATATYPE_USEDFIELD,
    DATATYPE_RAW,
    DATATYPE_POINTER,
    DATATYPE_RGB,
    DATATYPE_RG,
    DATATYPE_RGBA,

    DATATYPE_STRUCT_POLY,

    DATATYPE_BIT,
} enumDataType;

#define DATATYPE_FIRST_SPECIAL DATATYPE_FILENAME

typedef enum
{
    STORAGETYPE_EMBEDDED,
    STORAGETYPE_ARRAY, //embedded array
    STORAGETYPE_EARRAY,
} enumDataStorageType;

typedef enum
{
    REFERENCETYPE_DIRECT,
    REFERENCETYPE_POINTER,
} enumDataReferenceType;

//formatting options are nice and simple in that no more than one of them is allowed, so we can just track
//which one is turned on
typedef enum
{
    FORMAT_NONE,
    FORMAT_IP,
    FORMAT_KBYTES,
    FORMAT_FRIENDLYDATE,
    FORMAT_FRIENDLYSS2000,
    FORMAT_FRIENDLYCPU,
    FORMAT_PERCENT,
    FORMAT_NOPATH,
    FORMAT_HSV,
    FORMAT_TEXTURE,

    FORMAT_COUNT,
} enumFormatType;


//formatting flags can be on or off individually
typedef enum
{
    FORMAT_FLAG_UI_LEFT,
    FORMAT_FLAG_UI_RIGHT,
    FORMAT_FLAG_UI_RESIZABLE,
    FORMAT_FLAG_UI_NOTRANSLATE_HEADER,
    FORMAT_FLAG_UI_NOHEADER,
    FORMAT_FLAG_UI_NODISPLAY,

    FORMAT_FLAG_COUNT,
} enumFormatFlag;

typedef struct
{
    char indexString[MAX_NAME_LENGTH];
    char nameString[MAX_NAME_LENGTH];
} FIELD_INDEX;

#define MAX_REDUNDANT_STRUCTS 4

typedef struct
{
    char name[MAX_NAME_LENGTH];
    char subTable[MAX_NAME_LENGTH];
} REDUNDANT_STRUCT_INFO;

//a "STRUCT_COMMAND" is something that is not actually related to a field of the struct,
//but which will cause any auto-generated UI for that struct to contain a button which,
//when pressed, will generate a command. Each button has a name and a string
typedef struct STRUCT_COMMAND
{
    char *pCommandName;
    char *pCommandString;
    char *pCommandExpression;
    struct STRUCT_COMMAND *pNext;
} STRUCT_COMMAND;

typedef struct
{
    char baseStructFieldName[MAX_NAME_LENGTH];
    char curStructFieldName[MAX_NAME_LENGTH * 2 + 5];
    char userFieldName[MAX_NAME_LENGTH];
    char typeName[MAX_NAME_LENGTH];

    int iNumRedundantNames;
    char redundantNames[MAX_REDUNDANT_NAMES_PER_FIELD][MAX_NAME_LENGTH];

    int iNumRedundantStructInfos;
    REDUNDANT_STRUCT_INFO redundantStructs[MAX_REDUNDANT_STRUCTS];

    int iNumAsterisks;
    int iArrayDepth;
    bool bArray;
    bool bOwns;

    enumDataType eDataType;
    enumDataStorageType eStorageType;
    enumDataReferenceType eReferenceType;
    char arraySizeString[MAX_NAME_LENGTH];

    bool bBitField;

    enumFormatType eFormatType;
    int lvWidth;
    bool bFormatFlags[FORMAT_FLAG_COUNT];

    bool bFoundSpecialConstKeyword;
    bool bFoundConst;


    //when we find tokens that affect the actual basic data type, we store the tokens here and process them later
    bool bFoundFileNameToken;
    bool bFoundCurrentFileToken;
    bool bFoundTimeStampToken;
    bool bFoundLineNumToken;
    bool bFoundFlagsToken;
    bool bFoundBoolFlagToken;
    bool bFoundRawToken;
    bool bFoundPointerToken; 
    bool bFoundVec3Token;
    bool bFoundVec2Token;
    bool bFoundRGBToken;
    bool bFoundRGToken;
    bool bFoundRGBAToken;
    bool bFoundIntToken;

    //whether an included struct is or is not a container, when it can't be auto-determined
    bool bFoundForceContainer;
    bool bFoundForceNotContainer;

    // Misc bits
    bool bFoundRedundantToken;
    bool bFoundStructParam;

    bool bFoundPersist;
    bool bFoundNoTransact;

    bool bFoundVolatileRef;
    bool bIsInheritanceStruct;

    bool bFoundRequestIndexDefine;

    bool bFoundLateBind;


    int iMinBits;
    int iPrecision;
    int iFloatAccuracy;

    char usedFieldName[MAX_NAME_LENGTH];

    char subTableName[MAX_NAME_LENGTH];
    char structTpiName[MAX_NAME_LENGTH];

    //we save the file name and line number of this field for error reporting
    char fileName[MAX_PATH];
    int iLineNum;

    char defaultString[TOKENIZER_MAX_STRING_LENGTH + 1];
    char rawSizeString[TOKENIZER_MAX_STRING_LENGTH + 1];
    char pointerSizeString[TOKENIZER_MAX_STRING_LENGTH + 1];

    int iNumIndexes;
    FIELD_INDEX *pIndexes; //yes, this is misspelled, but it keeps it symmetrical with iNumIndexes

    int iNumWikiComments;
    char *pWikiComments[MAX_WIKI_COMMENTS];

    //used during dumping to make redundant structs work
    char *pCurOverrideTPIName;

    bool bFlatEmbedded;

    //source file where the 
    char *pStructSourceFileName;

    int iNumUserFlags;
    char userFlags[MAX_USER_FLAGS][MAX_NAME_LENGTH];

    char refDictionaryName[MAX_NAME_LENGTH];

    //at what index in the parse table was this field exported
    int iIndexInParseTable;

    //found AST(STRUCT(x, and need to force a particular type of struct
    bool bForceOptionalStruct;
    bool bForceEArrayOfStructs;
    bool bForceUnownedStruct;

    //commands that should come immediately after this field
    STRUCT_COMMAND *pFirstCommand;

    STRUCT_COMMAND *pFirstBeforeCommand; //commands that should come immediately before this field

    //stuff for polymorphism
    bool bIAmPolyParentTypeField;
    bool bIAmPolyChildTypeField;
    char myPolymorphicType[MAX_NAME_LENGTH];

    //which "type flag" strings were found, indices into sTokTypeFlags
    unsigned int iTypeFlagsFound;

    //indices into sTokTypeFlags_NoRedundantRepeat
    unsigned int iTypeFlags_NoRedundantRepeatFound;

    //this field is a struct, but instead of embedding that struct's wiki inside this struct's wiki, put it
    //parallel on the page and set up a hotlink to it
    bool bDoWikiLink;

    //format string built up by repeated calls to FORMATSTRING(x = 16 , y = "hi there")
    char *pFormatString;
    

} STRUCT_FIELD_DESC;



typedef struct
{
    char *pIn;
    int iInLength;
    char *pOut;
    int iOutLength;
} AST_MACRO_STRUCT;

class StructParser : public SourceParserBaseClass
{
public:
    StructParser();
    virtual ~StructParser();

public:
    virtual void SetProjectPathAndName(char const* srcPath, char const* commonPath, char const* projectName);


    virtual bool WriteOutData(void);

    virtual char const* GetMagicWord(int iWhichMagicWord);

    //note that iWhichMagicWord can be MAGICWORD_BEGINING_OF_FILE or MAGICWORD_END_OF_FILE
    virtual void FoundMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer, int iWhichMagicWord, char const* pMagicWordString);

    virtual void ProcessDataSingleFile(char const* pSourceFileName);


private:
    typedef struct
    {
        int iFirstFieldNum;
        int iLastFieldNum;
        char name[MAX_NAME_LENGTH];
    } UnionStruct;

    typedef struct
    {
        char sourceFileName[MAX_PATH];
        char structName[MAX_NAME_LENGTH];
        int iNumFields;
        STRUCT_FIELD_DESC *pStructFields[MAX_FIELDS];

        int iNumIgnores;
        char ignores[MAX_IGNORES][MAX_NAME_LENGTH];
        bool bIgnoresAreStructParam[MAX_IGNORES];

        int iLongestUserFieldNameLength;

        bool bHasStartString;
        char startString[MAX_NAME_LENGTH];

        bool bHasEndString;
        char endString[MAX_NAME_LENGTH];

        bool bForceSingleLine;

        bool bStripUnderscores;

        char *pMainWikiComment;

        int iNumUnions;
        UnionStruct unions[MAX_UNIONS];
        bool bCurrentlyInsideUnion;

        bool bNoPrefixStripping;
        bool bForceUseActualFieldName;
        bool bAlwaysIncludeActualFieldNameAsRedundant;
        bool bIsContainer;

        
        //precise starting location of this file, so that the container non-const copying code can return to that precise point
        int iPreciseStartingOffsetInFile;
        int iPreciseStartingLineNumInFile;

        //string set in AST_FORALL for this struct
        char forAllString[TOKENIZER_MAX_STRING_LENGTH];


        //stuff for polymorphism
        char structNameIInheritFrom[MAX_NAME_LENGTH];
        bool bIAmAPolymorphicParent;

        //prefix and suffix to put around the non-const definition... needed for Sam's crazy macro force-private stuff
        char *pNonConstPrefixString;
        char *pNonConstSuffixString;

        //auto-fixup func name
        char fixupFuncName[MAX_NAME_LENGTH];
    } STRUCT_DEF;

    int m_iNumStructs;
    STRUCT_DEF *m_pStructs[MAX_STRUCTS];


//callback function called by RecurseOverAllFieldsAndFlatEmbeds
    typedef void FieldRecurseCB(STRUCT_DEF *pParentStruct, STRUCT_FIELD_DESC *pField, 
        int iRecurseDepth, STRUCT_FIELD_DESC **ppRecurse_fields,        
        void *pUserData);

    typedef struct
    {
        char inCodeName[MAX_NAME_LENGTH];
        int iNumExtraNames;
        char extraNames[MAX_ENUM_EXTRA_NAMES][MAX_NAME_LENGTH];
        char *pWikiComment;
    } ENUM_ENTRY;

    typedef struct
    {
        char enumName[MAX_NAME_LENGTH];
        char enumToAppendTo[MAX_NAME_LENGTH];
        int iNumEntries;
        int iNumAllocatedEntries;
        ENUM_ENTRY *pEntries;
        char sourceFileName[MAX_PATH];
        bool bNoPrefixStripping;
        char *pMainWikiComment;
    } ENUM_DEF;

    int m_iNumEnums;
    ENUM_DEF *m_pEnums[MAX_ENUMS];


    int m_iNumMacros;
    AST_MACRO_STRUCT m_Macros[MAX_MACROS];

    char *m_pPrefix;
    char *m_pSuffix;

    
    char m_ProjectName[MAX_PATH];

private:
    void DumpExterns(FILE *pFile, STRUCT_DEF *pStruct);
    void DumpStruct(FILE *pFile, STRUCT_DEF *pStruct);
    void DumpStructPrototype(FILE *pFile, STRUCT_DEF *pStruct);

    bool ReadSingleField(Tokenizer *pTokenizer, STRUCT_FIELD_DESC *pField, STRUCT_DEF *pStruct);
    int DumpField(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName, int iLongestUserFieldNameLength, char *pStructFieldNamePrefix);
    bool IsLinkName(char *pString);

    void FixupFieldTypes(STRUCT_DEF *pStruct);
    void FixupFieldTypes_RightBeforeWritingData(STRUCT_DEF *pStruct);

    void TemplateFileNameFromSourceFileName(char *pTemplateName, char *pTemplateHeaderName, char const* pSourceName);

    static bool IsFloatName(char *pString);
    static bool IsIntName(char *pString);
    static bool IsCharName(char *pString);

    void DumpFieldFormatting(FILE *pFile, STRUCT_FIELD_DESC *pField);

    int DumpFieldDirectEmbedded(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName, int iLongestFieldNameLength, int iIndexInMultiLineField);
    int DumpFieldDirectArray(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName);
    int DumpFieldDirectEarray(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName);
    int DumpFieldPointerEmbedded(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName);
    int DumpFieldPointerArray(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName);
    int DumpFieldPointerEarray(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName);

    void ProcessStructFieldCommandString(STRUCT_DEF *pStruct, STRUCT_FIELD_DESC *pField, char const* pSourceString, 
        int iNumNotStrings, char **ppNotStrings, char const* pFileName, int iLineNum,
        Tokenizer *pMainTokenizer);
    void AddExtraNameToField(Tokenizer *pTokenizer, STRUCT_FIELD_DESC *pField, char *pName, bool bKeepOriginalName);

    int DumpFieldSpecifyUserFieldName(FILE *pFile, STRUCT_FIELD_DESC *pField, char *pStructName, char *pUserFieldName, 
        bool bNameIsRedundant, int iLongestFieldNameLength, int iIndexInMultiLineField);

    char *GetIntPrefixString(STRUCT_FIELD_DESC *pField);
    char *GetFloatPrefixString(STRUCT_FIELD_DESC *pField);

    char const* GetFieldTpiName(STRUCT_FIELD_DESC *pField, bool bIgnoreLateBind);

    void PrintIgnore(FILE *pFile, char *pIgnoreString, int iLongestFieldNameLength, bool bIgnoreIsStructParam);

    int PrintStructStart(FILE *pFile, STRUCT_DEF *pStruct);
    void PrintStructEnd(FILE *pFile, STRUCT_DEF *pStruct);
    bool IsStructAllStructParams(STRUCT_DEF *pStruct);

    void FixupFieldName(STRUCT_FIELD_DESC *pField, bool bStripUnderscores, bool bNoPrefixStripping, bool bForceUseActualFieldName,
        bool bAlwaysIncludeActualFieldNameAsRedundant);

    void WriteHeaderFileStart(FILE *pFile, char *pSourceName);
    void WriteHeaderFileEnd(FILE *pFile, char *pSourceName);

    void CalcLongestUserFieldName(STRUCT_DEF *pStruct);

    void FoundStructMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer);
    void FoundEnumMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer);

    void DumpEnum(FILE *pFile, ENUM_DEF *pEnum);
    void DumpEnumPrototype(FILE *pFile, ENUM_DEF *pEnum);

    void WriteOutDataSingleFile(char *pFileName);

    bool StructHasWikiComments(STRUCT_DEF *pStruct);

    bool FieldDumpsItselfCompletely(STRUCT_FIELD_DESC *pField);

    void DeleteStruct(int iStructIndex);

    // Validate cross-file container and inheritance references after fixup.
	void ValidateReferences(STRUCT_DEF *pStruct);

    void ResetMacros(void);

    void FoundMacro(char const* pSourceFileName, Tokenizer *pTokenizer);
    void FoundSuffix(char const* pSourceFileName, Tokenizer *pTokenizer);
    void FoundPrefix(char const* pSourceFileName, Tokenizer *pTokenizer);
    void FoundFixupFunc(char const* pSourceFileName, Tokenizer *pTokenizer);

    void ReplaceMacrosInString(char *pString, int iOutStringLength);
    int ReplaceMacroInString(char *pString, AST_MACRO_STRUCT *pMacro, int iCurLength, int iMaxLength);

    char *GetAllUserFlags(STRUCT_FIELD_DESC *pField);

    STRUCT_DEF *FindNamedStruct(char *pStructName);

    ENUM_ENTRY *GetNewEntry(ENUM_DEF *pEnum);

    void AttemptToDeduceReferenceDictionaryName(STRUCT_FIELD_DESC *pField, char *pTypeName);

    char *GetFieldSpecificPrefixString(STRUCT_DEF *pStruct, int iFieldNum);

    void CheckOverallStructValidity(Tokenizer *pTokenizer, STRUCT_DEF *pStruct);
    void CheckOverallStructValidity_PostFixup(STRUCT_DEF *pStruct);
    void DumpStructInitFunc(FILE *pFile, STRUCT_DEF *pStruct);

    void DumpNonConstCopy(FILE *pFile, STRUCT_DEF *pStruct);

    void DumpIndexDefines(FILE *pFile, STRUCT_DEF *pStruct, STRUCT_FIELD_DESC *pField, int iStartingOffset);


    void DumpCommand(    FILE *pFile, STRUCT_FIELD_DESC *pField, STRUCT_COMMAND *pCommand, int iLongestFieldNameLength);

    void DumpPolyTable(FILE *pFile, STRUCT_DEF *pStruct);
    
    bool StringIsContainerName(STRUCT_DEF *pStruct, char *pString);

    void RecurseOverAllFieldsAndFlatEmbeds(STRUCT_DEF *pParentStruct, STRUCT_DEF *pStruct, FieldRecurseCB *pCB, int iRecurseDepth, void *pUserData);
    static void AreThereLateBinds(STRUCT_DEF *pParentStruct, STRUCT_FIELD_DESC *pField,int iRecurseDepth, STRUCT_FIELD_DESC **ppRecurse_fields, void *pUserData);
    static void DumpLateBindFixups(STRUCT_DEF *pParentStruct, STRUCT_FIELD_DESC *pField, int iRecurseDepth, STRUCT_FIELD_DESC **ppRecurse_fields,void *pUserData);

    int PrintStructTableInfoColumn(FILE *pFile, STRUCT_DEF *pStruct);

    int GetNumLinesFieldTakesUp(STRUCT_FIELD_DESC *pField);
    char const* GetMultiLineFieldNamePrefix(STRUCT_FIELD_DESC *pField, int iIndex);

    static void AreThereBitFields(STRUCT_DEF *pParentStruct, STRUCT_FIELD_DESC *pField, int iRecurseDepth, STRUCT_FIELD_DESC **ppRecurse_fields,void *pUserData);
    static void DumpBitFieldFixups(STRUCT_DEF *pParentStruct, STRUCT_FIELD_DESC *pField, int iRecurseDepth, STRUCT_FIELD_DESC **ppRecurse_fields,void *pUserData);

    ENUM_DEF *FindEnumByName(char *pName);
    void DumpEnumInWikiForField(ENUM_DEF *pEnum, char *pOutString);
    void DumpEnumToWikiFile(FILE *pOutFile, ENUM_DEF *pEnum);

    void AssertNameIsLegalForFormatStringString(Tokenizer *pTokenizer, char const* pName);
    void AssertNameIsLegalForFormatStringInt(Tokenizer *pTokenizer, char const* pName);

    void AddStringToFormatString(STRUCT_FIELD_DESC *pField, char const* pStringToAdd);

    void AssertFieldHasNoDefault(STRUCT_FIELD_DESC *pField, char const* pErrorString);

};









#endif