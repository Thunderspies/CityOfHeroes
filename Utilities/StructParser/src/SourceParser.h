#ifndef _SOURCEPARSER_H_
#define _SOURCEPARSER_H_

#include "tokenizer.h"

#include "ParserLimits.h"


#include "IdentifierDictionary.h"
#include "SourceParserBaseClass.h"

#include "utils.h"

#include <vector>
#include <string>
#include <filesystem>

#define MAX_BASE_SOURCE_PARSERS 8

#define MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER 12

#define MAX_WIKI_PROJECTS 64
#define MAX_WIKI_CATEGORIES 256

typedef class AutoRunManager AutoRunManager;


class SourceParser
{
public:
    SourceParser();
    ~SourceParser();

	/* Load variables before generation. Records discovery candidates and
	 * includes, including absent candidates. Paths must be absolute.
	 * Fresh instances only; filesystem errors throw, syntax errors exit.
	 */
	void LoadConfiguration(std::filesystem::path const& projectDir);
	// Borrowed dependency paths remain valid for this instance's lifetime.
	auto const& ConfigurationDependencies() const
	{
		return m_configurationDependencies;
	}
	/* Generate a complete target using previously loaded configuration.
	 * Input paths must be absolute; all directories must exist. Only .c/.h
	 * inputs are scanned, in manifest order. Uses global output tracking;
	 * not thread-safe. Returns zero; errors throw or terminate the process.
	 */
	int ParseSource(std::string const& targetName,
		std::filesystem::path const& srcDir,
		std::filesystem::path const& commonDir, bool isExecutable,
		std::vector<std::filesystem::path> const& sourceFiles);

    char const* GetShortProjectName() { return m_shortenedPrjFileName.c_str(); }
    char const* GetSoureDir() { return m_srcDir.c_str(); }
    IdentifierDictionary *GetDictionary() { return &m_IdentifierDictionary; }


    AutoRunManager *GetAutoRunManager() { return m_pAutoRunManager; }

    //returns true if the project is an executable as opposed to a library
    bool ProjectIsExecutable(void) { return m_bIsAnExecutable; }

    int GetNumProjectFiles(void) { return m_iNumProjectFiles; }
    char *GetNthProjectFile(int n) { return m_ProjectFiles[n]; }

    bool DoesVariableHaveValue(char const* pVarName, char const* pValue, bool bCheckFinalValueOnly);


private://structs
    typedef struct SourceParserVar
    {
        char *pVarName;
        char *pValue;
        struct SourceParserVar *pNext;
    } SourceParserVar;


private:

    IdentifierDictionary m_IdentifierDictionary;

    int m_iNumSourceParsers;
    SourceParserBaseClass *m_pSourceParsers[MAX_BASE_SOURCE_PARSERS];
    AutoRunManager *m_pAutoRunManager;

    int m_iNumProjectFiles;
    char m_ProjectFiles[MAX_FILES_IN_PROJECT][MAX_PATH];


    std::string m_shortenedPrjFileName;

    std::string m_srcDir;
    std::string m_commonDir;


    //whether the project we're working on is an executable vs. a library
    bool m_bIsAnExecutable;


    SourceParserVar *m_pFirstVar;
	std::vector<std::filesystem::path> m_configurationDependencies;
	std::vector<std::filesystem::path> m_configStack;

private:
	void AddProjectFiles(
		std::vector<std::filesystem::path> const& files);
    void ScanSourceFile(char *pSourceFile);
    

    int FindProjectFileIndex(char *pFileName);
    void MakeAutoGenDirectory();
    void CreateParsers(void);
    bool MakeSpecialAutoRunFunction(void);

    void AddVariableValue(char *pVarName, char *pValue);
    void SetVariablesFromTokenizer(Tokenizer *pTokenizer, char *pStartingDirectory);
};


//global TRACE for verbose stuff
extern int gVerbose;
#define TRACE(...)  {if (gVerbose) {printf(__VA_ARGS__); fflush(stdout);}}

#define GENERATE_FAKE_DEPENDENCIES 0
#endif
