#include "pch.h"
#include "sourceparser.h"
#include "strutils.h"

#include "MagicCommandManager.h"
#include "structparser.h"
#include "AutoTransactionManager.h"
#include "autorunmanager.h"
#include "windows.h"
#include "direct.h"
#include "autoTestManager.h"
#include "latelinkmanager.h"
#include "FileWrapper.h"
#include "libxml/xpath.h"
#include <libxml/xpathInternals.h>

#include <optional>

namespace fs = std::filesystem;

#define MAX_PROJECTS_ONE_SOLUTION 256

#define MAX_WILDCARD_MAGIC_WORDS 16

char sTime[] = __TIME__;

enum
{
    RW_FILE = RW_COUNT,
    RW_RELATIVEPATH,
    RW_CONFIGURATION,
    RW_ADDITIONALINCLUDEDIRECTORIES,
    RW_PREPROCESSORDEFINITIONS,
    RW_OUTPUTDIRECTORY,
    RW_OBJECTFILE,
    RW_NAME,
    RW_TOOL,
    RW_PROPERTYSHEETS,
    RW_INTERMEDIATEDIRECTORY,
};

static char const* sProjectReservedWords[] =
{
    "File",
    "RelativePath",
    "Configuration",
    "AdditionalIncludeDirectories",
    "PreprocessorDefinitions",
    "OutputDirectory",
    "ObjectFile",
    "Name",
    "Tool",
    "InheritedPropertySheets",
    "IntermediateDirectory",

    NULL
};

enum
{
    RW_GLOBAL = RW_COUNT,
    RW_PROJECT,
    RW_PROJECTDEPENDENCIES,
    RW_ENDPROJECTSECTION,
    RW_ENDPROJECT,
};

static char const* sSolutionReservedWords[] =
{
    "Global",
    "Project",
    "ProjectDependencies",
    "EndProjectSection",
    "EndProject",
    NULL
};

//must be all caps
static char const* sFileNamesToExclude[] =
{
    "STDTYPES.H",
    NULL
};

static char const* sProjectNamesToExclude[] =
{
    "GimmeDLL",
    NULL
};

bool ShouldFileBeExcluded(char const* pFileName)
{
    char tempFileName[MAX_PATH];
    strcpy(tempFileName, pFileName);

    if (strstr(pFileName, "Program Files"))
    {
        return true;
    }

    char *pTemp;
    char *pSimpleFileName = pTemp = tempFileName;

    while (*pTemp)
    {
        if ((*pTemp == '/' || *pTemp == '\\') && *(pTemp + 1))
        {
            pSimpleFileName = pTemp + 1;
        }

        pTemp++;
    }

    MakeStringUpcase(pSimpleFileName);

    int i = 0;

    while (sFileNamesToExclude[i])
    {
        if (AreFilenamesEqual(pSimpleFileName, sFileNamesToExclude[i]))
        {
            return true;
        }


        i++;
    }

    if (strstr(pSimpleFileName, "AUTOGEN"))
    {
        return true;
    }

    return false;
}

template<typename T>
T Argument(T value) noexcept
{
    return value;
}

template <typename T>
T const* Argument(std::basic_string<T> const& value) noexcept
{
    return value.c_str();
}

template<size_t InitialLength = 200>
class Formatter
{
public:
    Formatter() = default;

    template<typename... Args>
    std::string format(char const* const format, Args const& ... args)
    {
        std::string result;
        result.resize(InitialLength);
        auto length = ::snprintf(result.data(), result.size() + 1, format, Argument(args)...);
        if (length < 0)
        {
            return {};
        }
        result.resize(length);
        if (length > InitialLength)
        {
            ::snprintf(result.data(), result.size() + 1, format, Argument(args)...);
        }
        return std::move(result);
    }

private:
    Formatter(Formatter const& other) = delete;
    Formatter& operator=(Formatter const& other) = delete;
};

SourceParser::SourceParser()
{
    m_iNumSourceParsers = 6;

    m_pSourceParsers[0] = NULL;
    m_pSourceParsers[1] = NULL;
    m_pSourceParsers[2] = NULL;
    m_pSourceParsers[3] = NULL;
    m_pSourceParsers[4] = NULL;
    m_pSourceParsers[5] = m_pAutoRunManager = NULL;

    m_iNumProjectFiles = 0;

    memset(m_iNumDependencies, 0, sizeof(m_iNumDependencies));

    m_pFileListLoader = new FileListLoader;
    m_pFileListWriter = new FileListWriter;

    memset(m_bFilesNeedToBeUpdated, 0, sizeof(m_bFilesNeedToBeUpdated));

    memset(m_iExtraDataPerFile, 0, sizeof(m_iExtraDataPerFile));

    m_iNumDependentLibraries = 0;

    m_FoundAutoGenFile1 = false;
    m_FoundAutoGenFile1 = false;
    m_bIsAnExecutable = false;

    m_pFirstVar = NULL;

}

void SourceParser::CreateParsers(void)
{
    m_pSourceParsers[0] = new MagicCommandManager;
    m_pSourceParsers[1] = new StructParser;
    m_pSourceParsers[2] = new AutoTransactionManager;
    m_pSourceParsers[3] = new AutoTestManager;
    m_pSourceParsers[4] = new LateLinkManager;

    //AutoRunManager should generally be last
    m_pSourceParsers[5] = m_pAutoRunManager = new AutoRunManager;
}

SourceParser::~SourceParser()
{
    int i;

    for (i=0; i < m_iNumSourceParsers; i++)
    {
        if (m_pSourceParsers[i])
        {
            delete m_pSourceParsers[i];
        }
    }

    while (m_pFirstVar)
    {
        SourceParserVar *pNext = m_pFirstVar->pNext;
        delete m_pFirstVar->pVarName;
        delete m_pFirstVar->pValue;
        delete m_pFirstVar;
        m_pFirstVar = pNext;
    }

    delete m_pFileListLoader;
    delete m_pFileListWriter;
}

bool SourceParser::IsLibraryXBoxExcluded(char *pLibName)
{
    if (strstr(pLibName, "GLRenderLib"))
    {
        return true;
    }

    return false;
}


void SourceParser::ProcessSolutionFile()
{
    Tokenizer tokenizer;

    int iNumProjects = 0;
    char *pProjectNames[MAX_PROJECTS_ONE_SOLUTION];
    char *pProjectIDStrings[MAX_PROJECTS_ONE_SOLUTION];
    char *pProjectFullPaths[MAX_PROJECTS_ONE_SOLUTION];

    bool bResult = tokenizer.LoadFromFile(m_slnPath.string().c_str());

    bool bFoundOurProject = false;

    Tokenizer::StaticAssert(bResult, "Couldn't load solution file");

    //set reservedwords used for parsing through .vcproj file
    tokenizer.SetExtraReservedWords(sSolutionReservedWords);

    Token token;
    enumTokenType eType;

    while ((eType = tokenizer.GetNextToken(&token)) != TOKEN_NONE)
    {
        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_GLOBAL)
        {
            break;
        }

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_PROJECT)
        {
            tokenizer.Assert(iNumProjects < MAX_PROJECTS_ONE_SOLUTION, "Too many projects in .sln file");

            do
            {
                eType = tokenizer.GetNextToken(&token);
                tokenizer.Assert(eType != TOKEN_NONE, "Unexpected end of .sln file while parsing project");
            }
            while (!(eType == TOKEN_RESERVEDWORD && token.iVal == RW_EQUALS));

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_PATH, "Expected project name");

            if (StringIsInList(token.sVal, sProjectNamesToExclude) && strcmp(token.sVal, m_prjFileName.c_str()) != 0)
            {
                //skip this project
            }
            else
            {

                pProjectNames[iNumProjects] = new char[token.iVal + 1];
                strcpy(pProjectNames[iNumProjects], token.sVal);

                if (strcmp(token.sVal, m_shortenedPrjFileName.c_str()) == 0)
                {
                    bFoundOurProject = true;
                    tokenizer.SaveLocation();
                }


                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after project name");
                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_PATH, "Expected project full path");

                pProjectFullPaths[iNumProjects] = new char[token.iVal + 1];
                strcpy(pProjectFullPaths[iNumProjects], token.sVal);

                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after project full path");
                tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_PATH, "Expected project ID string");

                pProjectIDStrings[iNumProjects] = new char[token.iVal + 1];
                strcpy(pProjectIDStrings[iNumProjects], token.sVal);

                iNumProjects++;
            }
        }
    }

    tokenizer.Assert(bFoundOurProject, "Didn't find current project referenced in .sln file");

    tokenizer.RestoreLocation();

    do
    {
        eType = tokenizer.GetNextToken(&token);

        tokenizer.Assert(eType != TOKEN_NONE, "unexpected end of file before EndProject");

        if (token.eType == TOKEN_RESERVEDWORD && token.iVal == RW_ENDPROJECT)
        {
            break;
        }

        if (token.eType == TOKEN_RESERVEDWORD && token.iVal == RW_PROJECTDEPENDENCIES)
        {
            do
            {
                eType = tokenizer.GetNextToken(&token);
                tokenizer.Assert(eType != TOKEN_NONE, "unexpected end of file before EndProjectSection");

                if (token.eType == TOKEN_RESERVEDWORD && token.iVal == RW_ENDPROJECTSECTION)
                {
                    break;
                }

                if (token.eType == TOKEN_RESERVEDWORD && token.iVal == RW_LEFTBRACE)
                {
                    char tempString[1024] = "{";

                    tokenizer.SetDontParseInts(true);

                    do
                    {
                        eType = tokenizer.GetNextToken(&token);

                        tokenizer.Assert(eType != TOKEN_NONE, "unexpected end of file in project UID");

                        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTBRACE)
                        {
                            break;
                        }

                        tokenizer.Assert(eType == TOKEN_IDENTIFIER || eType == TOKEN_RESERVEDWORD && token.iVal == RW_MINUS, 
                            "found unexpected characters while parsing projectUID");
                        tokenizer.StringifyToken(&token);

                        strcat(tempString, token.sVal);
                    } while (1);

                    tokenizer.SetDontParseInts(false);

                    strcat(tempString, "}");

                    int i;
                    bool bFound =false;

                    for (i=0; i < iNumProjects; i++)
                    {
                        if (strcmp(tempString, pProjectIDStrings[i]) == 0)
                        {
                            tokenizer.Assert(m_iNumDependentLibraries < MAX_DEPENDENT_LIBRARIES, "too many dependent libraries");
                            strcpy(m_DependentLibraryNames[m_iNumDependentLibraries], pProjectNames[i]);
                            strcpy(m_DependentLibraryFullPaths[m_iNumDependentLibraries], pProjectFullPaths[i]);
                            m_bExcludeLibrariesFromXBOX[m_iNumDependentLibraries] = IsLibraryXBoxExcluded(pProjectNames[i]);


                            m_iNumDependentLibraries++;
                            break;
                        }
                    }

                    do
                    {
                        eType = tokenizer.GetNextToken(&token);

                        tokenizer.Assert(eType != TOKEN_NONE, "unexpected end of file in project UID");

                    } while (!(eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTBRACE));

                }
            } while (1);

        }
    } while (1);



    int i;

    for (i=0; i < iNumProjects; i++)
    {
        delete [] pProjectNames[i];
        delete [] pProjectIDStrings[i];
        delete [] pProjectFullPaths[i];
    }
}

void SourceParser::CheckForRequiredFiles(const char *pFileName)
{
    const char *pShortName = GetFileNameWithoutDirectories(pFileName);

    while (pShortName[0] == '\\' || pShortName[0] == '/')
    {
        pShortName++;
    }

    if (_stricmp(m_AutoGenFile1Name, pShortName) == 0)
    {
        m_FoundAutoGenFile1 = true;
    }
    else if (_stricmp(m_AutoGenFile2Name, pShortName) == 0)
    {
        m_FoundAutoGenFile2 = true;
    }
}


#define MAX_PROJECTS_ONE_SOLUTION 256

static bool register_namespaces(xmlXPathContextPtr xpathCtx, const xmlChar* nsList)
{
    xmlChar* nsListDup;
    xmlChar* prefix;
    xmlChar* href;
    xmlChar* next;

    assert(xpathCtx);
    assert(nsList);

    nsListDup = xmlStrdup(nsList);
    if(nsListDup == NULL) {
        return false;    
    }

    next = nsListDup; 
    while(next != NULL) {
        /* skip spaces */
        while((*next) == ' ') next++;
        if((*next) == '\0') break;

        /* find prefix */
        prefix = next;
        next = (xmlChar*)xmlStrchr(next, '=');
        if(next == NULL) {
            xmlFree(nsListDup);
            return false;
        }
        *(next++) = '\0';    

        /* find href */
        href = next;
        next = (xmlChar*)xmlStrchr(next, ' ');
        if(next != NULL) {
            *(next++) = '\0';    
        }

        /* do register namespace */
        if(xmlXPathRegisterNs(xpathCtx, prefix, href) != 0) {
            xmlFree(nsListDup);
            return false;
        }
    }

    xmlFree(nsListDup);
    return true;
}

static std::vector<std::string> get_xpath_nodes_attributes(xmlDocPtr doc, xmlXPathContextPtr xpathCtx, std::string expression)
{
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST expression.c_str(), xpathCtx);
    xmlNodeSetPtr nodes = xpathObj->nodesetval;
    int size = nodes ? nodes->nodeNr : 0;
    std::vector<std::string> attributes;
    for (int i = 0; i < size; ++i)
    {
        attributes.push_back((const char*)nodes->nodeTab[i]->children[0].content);
    }
    
    xmlXPathFreeObject(xpathObj);
    return attributes;
}

template<typename... Args>
static std::vector<std::string> get_xpath_nodes_attributes(xmlDocPtr doc, xmlXPathContextPtr xpathCtx, char const* format, Args& ... args)
{
    auto expression = Formatter<>{}.format(format, std::forward<Args>(args)...);
    return get_xpath_nodes_attributes(doc, xpathCtx, expression);
}

static std::string get_xpath_nodes_inner_text(xmlDocPtr doc, xmlXPathContextPtr xpathCtx, std::string const& expression)
{
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST expression.c_str(), xpathCtx);
    xmlNodeSetPtr nodes = xpathObj->nodesetval;
    int size = nodes ? nodes->nodeNr : 0;
    std::string inner_text;
    for (int i = 0; i < size; ++i)
    {
        inner_text += (const char*)nodes->nodeTab[i]->children[0].content;
    }
    
    xmlXPathFreeObject(xpathObj);
    return inner_text;
}


template<typename... Args>
static std::string get_xpath_nodes_inner_text(xmlDocPtr doc, xmlXPathContextPtr xpathCtx, char const* format, Args&... args)
{
    auto expression = Formatter<>{}.format(format, std::forward<Args>(args)...);
    return get_xpath_nodes_inner_text(doc, xpathCtx, expression);
}

static void GetAdditionalStuffFromPropertySheets(char* objectFileDir, char *pPropertySheetNames, const char *propertyGroup)
{
    char *pTemp;

    if ((pTemp = strstr(objectFileDir, "$(NOINHERIT)")))
    {
        *pTemp = 0;
        return;
    }

    while (pPropertySheetNames && pPropertySheetNames[0])
    {
        char fileName[MAX_PATH];
        strcpy(fileName, pPropertySheetNames);

        if ((pTemp = strchr(fileName, ';')))
        {
            *pTemp = 0;
        }

        if ((pTemp = strchr(pPropertySheetNames, ';')))
        {
            pPropertySheetNames = pTemp + 1;
        }
        else
        {
            pPropertySheetNames = NULL;
        }

        std::string expression = "/ms:Project/ms:PropertyGroup/ms:";
        expression += propertyGroup;
        
        xmlDocPtr doc = xmlParseFile(fileName);
        Tokenizer::StaticAssert(doc != NULL, "Couldn't load doc.");
        xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
        Tokenizer::StaticAssert(xpathCtx != NULL, "Couldn't load doc context.");
        Tokenizer::StaticAssert(register_namespaces(xpathCtx, BAD_CAST "ms=http://schemas.microsoft.com/developer/msbuild/2003"), "Couldn't register namespace.");
        std::string test = get_xpath_nodes_inner_text(doc, xpathCtx, expression).c_str();
        if (test.size())
        {
            strcpy(objectFileDir, test.c_str());
        }

        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
    }
}

void SourceParser::AddProjectFiles(std::vector<std::string> const& attributes)
{
    for (auto const& file : attributes)
    {
        CheckForRequiredFiles(file.c_str());

        auto len = file.length();
        if ((len >= 3 && file[len - 2] == '.' && (file[len - 1] == 'h' || file[len - 1] == 'c')))
        {
            fs::path path{ m_prjDir };
            path.append(file);

            Tokenizer::StaticAssert(m_iNumProjectFiles < MAX_FILES_IN_PROJECT, "Too many files in project");

            auto name = path.string();
            if (!ShouldFileBeExcluded(name.c_str()))
            {
                strcpy(m_ProjectFiles[m_iNumProjectFiles++], name.c_str());
            }
        }
    }
}

void SourceParser::ProcessProjectFile()
{
    char propertySheetNames[512] = "";

    xmlInitParser();

    /* Load XML document */
    xmlDocPtr doc = xmlParseFile(m_prjPath.string().c_str());
    Tokenizer::StaticAssert(doc != NULL, "Couldn't load doc.");

    /* Create xpath evaluation context */
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    Tokenizer::StaticAssert(xpathCtx != NULL, "Couldn't load doc context.");
    Tokenizer::StaticAssert(register_namespaces(xpathCtx, BAD_CAST "ms=http://schemas.microsoft.com/developer/msbuild/2003"), "Couldn't register namespace.");

    auto const condition = Formatter<>{}.format("'$(Configuration)|$(Platform)'=='%s|%s'", m_configuration, m_platform);

    //GetAdditionalStuffFromPropertySheets(m_outDir.c_str(), propertySheetNames, "OutDir");

    //GetAdditionalStuffFromPropertySheets(m_intDir.c_str(), propertySheetNames, "IntDir");

    auto projectFormat = "/ms:Project/ms:ImportGroup[@Condition = \"%s\" and @Label = \"PropertySheets\"]/ms:Import[not(@Label)]/@Project";
    auto attributes = get_xpath_nodes_attributes(doc, xpathCtx, projectFormat, condition);
    for (auto const& attribute : attributes)
    {
        if (propertySheetNames[0])
        {
            strcat(propertySheetNames, ";");
        }
        strcat(propertySheetNames, attribute.c_str());
    }
    
    auto configTypeFormat = "/ms:Project/ms:PropertyGroup[@Condition = \"%s\" and @Label = \"Configuration\"]/ms:ConfigurationType";
    if (get_xpath_nodes_inner_text(doc, xpathCtx, configTypeFormat, condition) == "Application")
    {
        m_bIsAnExecutable = true;
    }
    
    AddProjectFiles(get_xpath_nodes_attributes(doc, xpathCtx, "/ms:Project/ms:ItemGroup/ms:ClInclude/@Include"));
    AddProjectFiles(get_xpath_nodes_attributes(doc, xpathCtx, "/ms:Project/ms:ItemGroup/ms:ClCompile/@Include"));

    /* Cleanup */
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
}

bool SourceParser::NeedToUpdateFile(char *pFileName, int iExtraData, bool bForceUpdateUnlessFileDoesntExist)
{
    HANDLE hFile = CreateFileA(pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    else 
    {
        if (bForceUpdateUnlessFileDoesntExist)
        {
            CloseHandle(hFile);
            return true;
        }
        else
        {    
            bool bNeedToUpdate = false;

            if (m_pFileListLoader->IsFileInList(pFileName))
            {
                FILETIME mainFileTime;

                GetFileTime(hFile, NULL, NULL, &mainFileTime);

                if (CompareFileTime(&mainFileTime, m_pFileListLoader->GetMasterFileTime()) == 1)
                {
                    bNeedToUpdate = true;
                }
                else if (iExtraData)
                {
                    int i;

                    for (i=0; i < m_iNumSourceParsers; i++)
                    {
                        if (iExtraData & ( 1 << i))
                        {
                            if (m_pSourceParsers[i]->DoesFileNeedUpdating(pFileName))
                            {
                                bNeedToUpdate = true;
                            }
                        }
                    }
                }
            }
            else
            {
                bNeedToUpdate = true;
            }

            CloseHandle(hFile);

            return bNeedToUpdate;
        }
    }
}
void SourceParser::MakeAutoGenDirectory()
{
    fs::path directory{ m_srcDir };
    directory.append("AutoGen");
    fs::create_directories(directory);

    directory = m_commonDir;
    directory.append("AutoGen");
    fs::create_directories(directory);
}

void SourceParser::DestroyLegacyMasterFiles(bool bForceBuildAll)
{
    if ( gbLastFWCloseActuallyWrote )
    {
        NukeCObjFile(m_AutoGenFile1Name);
    }

    if (gbLastFWCloseActuallyWrote )
    {
        NukeCObjFile(m_AutoGenFile2Name);
    }
}

void SourceParser::CreateCleanBuildMarkerFile()
{
    fs::path path{ m_intDir };
    fs::create_directories(path);

    path.append("THIS_FILE_CHECKS_FOR_CLEAN_BUILDS.obj");
    FileWrapper* pFile = fw_fopen(path.string().c_str(), "wt");
    if (pFile != nullptr)
    {
        fw_fprintf(pFile, "This file exists so that structparser will know when a clean build happens");
        fw_fclose(pFile);
    }
}

bool SourceParser::DidCleanBuildJustHappen()
{
    HANDLE hFile;
    fs::path path{ m_intDir };
    path.append("THIS_FILE_CHECKS_FOR_CLEAN_BUILDS.obj");
    hFile = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return true;;
    }
    CloseHandle(hFile);
    return false;
}

void SourceParser::CleanOutAllAutoGenFiles()
{
    char tempString[256];
    sprintf(tempString, "del /Q \"%s\\AutoGen\\*.*\" > NUL 2>&1", m_srcDir.c_str());
    system(tempString);

    sprintf(tempString, "del /Q \"%s\\wiki\\*.*\" > NUL 2>&1", m_srcDir.c_str());
    system(tempString);

    sprintf(tempString, "del /Q \"%s\\AutoGen\\%s_*.*\" > NUL 2>&1", m_commonDir.c_str(), m_shortenedPrjFileName.c_str());
    system(tempString);
}

bool SourceParser::IsQuickExitPossible()
{
    HANDLE hFile;
    FILETIME fileTime;

    hFile = CreateFile(m_prjPath.c_str(), GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    GetFileTime(hFile, NULL, NULL, &fileTime);

    if (CompareFileTime(&fileTime, m_pFileListLoader->GetMasterFileTime()) == 1)
    {
        return false;
    }

    CloseHandle(hFile);

    hFile = CreateFile(m_slnPath.c_str(), GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    GetFileTime(hFile, NULL, NULL, &fileTime);

    if (CompareFileTime(&fileTime, m_pFileListLoader->GetMasterFileTime()) == 1)
    {
        return false;
    }

    CloseHandle(hFile);


    int i;
    int iNumFiles = m_pFileListLoader->GetNumFiles();

    for (i=0; i < iNumFiles; i++)
    {
        if (!m_pFileListLoader->GetNthFileExists(i))
        {
            return false;
        }

        if (CompareFileTime(m_pFileListLoader->GetNthFileTime(i), m_pFileListLoader->GetMasterFileTime()) == 1)
        {
            return false;
        }
    }


    //project file and solution file and all project files are unchanged, quit
    TRACE("Project file, solution file, and all project files are unchanged... quitting\n");
    return true;
}

int SourceParser::ParseSource(
    std::filesystem::path const& prjPath,
    std::filesystem::path const& srcDir,
    std::filesystem::path const& commonDir,
    std::filesystem::path const& outDir,
    std::filesystem::path const& intDir,
    std::string const& platform,
    std::string const& configuration,
    std::filesystem::path const& slnPath
    )
{
    m_prjPath = prjPath;
    m_slnPath = slnPath;
    m_srcDir = srcDir.string();
    m_commonDir = commonDir.string();

    m_outDir = outDir.string();
    m_intDir = intDir.string();
    m_prjDir = prjPath.parent_path().string();
    m_prjFileName = m_prjPath.filename().string();
    m_shortenedPrjFileName = m_prjPath.filename().stem().string();
    m_platform = platform;
    m_configuration = configuration;

    bool bForceReadAllFiles = false; 
    auto shortListFileName = Formatter<>{}.format("%s_%s", m_shortenedPrjFileName, m_configuration);
    MakeStringAllAlphaNum(shortListFileName.data());

    fs::path listFileName{ m_srcDir };
    listFileName.append(Formatter<>{}.format("AutoGen\\%s.SPFileList", shortListFileName));

    sprintf(m_AutoGenFile1Name, "%s_AutoGen_1.c", m_shortenedPrjFileName.c_str());
    sprintf(m_AutoGenFile2Name, "%s_AutoGen_2.cpp", m_shortenedPrjFileName.c_str());
    sprintf(m_SpecialAutoRunFuncName, "_%s_AutoRun_SPECIALINTERNAL", m_shortenedPrjFileName.c_str());

    MakeAutoGenDirectory();

    TRACE("About to start parsing... project %s config %s\n", m_shortenedPrjFileName.c_str(), m_configuration.c_str());

    if (!m_pFileListLoader->LoadFileList(listFileName.string().c_str()))
    {
        TRACE("Couldn't load spfilelist file... doing full rebuild\n");
        bForceReadAllFiles = true;
    }
    else
    {
        if (!bForceReadAllFiles)
        {
            if (DidCleanBuildJustHappen())
            {
                TRACE("Clean build happened... doing full rebuild\n");
                bForceReadAllFiles = true;
            }
            else
            {
                if (IsQuickExitPossible())
                {
                    return 0;
                }
                TRACE("Not doing quick exit... something must have changed\n");
            }
        }
    }

    ProcessSolutionFile();

    CreateParsers();

    ProcessProjectFile();

    FindVariablesFileAndLoadVariables();

    CreateCleanBuildMarkerFile();

    bool bAtLeastOneFileUpdated = false;

    if (bForceReadAllFiles)
    {
        TRACE("Erasing all autogenerated files\n");
        CleanOutAllAutoGenFiles();
    }

    int i;

    for (i=0; i < m_iNumSourceParsers; i++)
    {
        m_pSourceParsers[i]->SetParent(this, i);
        m_pSourceParsers[i]->SetProjectPathAndName(m_srcDir.c_str(), m_commonDir.c_str(), m_shortenedPrjFileName.c_str());
    }


    if (!m_IdentifierDictionary.SetFileNameAndLoad(m_srcDir.c_str(), m_shortenedPrjFileName.c_str()))
    {
        TRACE("Couldn't load identifier dictionary... forcing read all files\n");
        bForceReadAllFiles = true;
    }


    for (i=0; i < m_iNumSourceParsers; i++)
    {        
        if (!m_pSourceParsers[i]->LoadStoredData(bForceReadAllFiles))
        {
            TRACE("Couldn't load stored data %d, forcing read all files\n", i);
            bForceReadAllFiles = true;
        }
    }

    if (MakeSpecialAutoRunFunction())
    {
        //make sure AutoRunManager has magic internal autorun
        GetAutoRunManager()->AddAutoRunSpecial(m_SpecialAutoRunFuncName, "_SPECIAL_INTERNAL", true, AUTORUN_ORDER_FIRST);
    }
    else
    {
        GetAutoRunManager()->ResetSourceFile("_SPECIAL_INTERNAL");
    }


    //must be after LoadStoredData
    LoadSavedDependenciesAndRemoveObsoleteFiles();

    for (i = 0 ; i < m_iNumProjectFiles; i++)
    {
        bAtLeastOneFileUpdated |= m_bFilesNeedToBeUpdated[i] |= NeedToUpdateFile(m_ProjectFiles[i], m_iExtraDataPerFile[i], bForceReadAllFiles);
    }

    /*    if (!bAtLeastOneFileUpdated && !bForceReadAllFiles)
    {
    return;
    }*/

    if (bForceReadAllFiles)
    {
        ProcessAllFiles_ReadAll();
    }
    else
    {
        ProcessAllFiles();

    }

    bool bMasterFilesChanged = false;
    for (i=0; i < m_iNumSourceParsers; i++)
    {
        bMasterFilesChanged |= m_pSourceParsers[i]->WriteOutData();
    }

    m_IdentifierDictionary.WriteOutFile();

    m_pFileListWriter->OpenFile(listFileName.string().c_str(), m_intDir.c_str());

    for (i = 0 ; i < m_iNumProjectFiles; i++)
    {
        m_pFileListWriter->AddFile(m_ProjectFiles[i], m_iExtraDataPerFile[i], m_iNumDependencies[i], m_iDependencies[i]);
    }

    m_pFileListWriter->CloseFile();


    if ((bAtLeastOneFileUpdated && bMasterFilesChanged) || bForceReadAllFiles)
    {
        DestroyLegacyMasterFiles(bForceReadAllFiles);
    }

    return 0;
}

void SourceParser::ScanSourceFile(char *pSourceFile)
{
    char const* sMagicWords[MAX_BASE_SOURCE_PARSERS * MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER + 1]; 

    int iNumWildcardMagicWords = 0;
    int iWildcardMagicWordIndices[MAX_WILDCARD_MAGIC_WORDS];

    TRACE("Parsing %s\n", pSourceFile);


    sMagicWords[m_iNumSourceParsers * MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER] = NULL;

    int i, j;

    for (i=0; i < m_iNumSourceParsers; i++)
    {
        for (j=0; j < MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER; j++)
        {
            sMagicWords[i * MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER + j] = m_pSourceParsers[i]->GetMagicWord(j);

            if (StringContainsWildcards(sMagicWords[i * MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER + j]))
            {
                Tokenizer::StaticAssert(iNumWildcardMagicWords < MAX_WILDCARD_MAGIC_WORDS, "Too many wildcard magic words");
                iWildcardMagicWordIndices[iNumWildcardMagicWords++] = i * MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER + j;
            }
        }
    }

    Tokenizer tokenizer;

    bool bResult = tokenizer.LoadFromFile(pSourceFile);

    if (!bResult)
    {
        char errorString[256];
        sprintf(errorString, "Couldn't find file %s\n", pSourceFile);
        Tokenizer::StaticAssert(0, errorString);
    }

    tokenizer.SetCSourceStyleStrings(true);
    tokenizer.SetExtraReservedWords(sMagicWords);
    tokenizer.SetNoNewlinesInStrings(true);
    tokenizer.SetSkipDefines(true);

    Token token;
    enumTokenType eType;

    for (i=0; i < m_iNumSourceParsers; i++)
    {
        m_pSourceParsers[i]->FoundMagicWord(pSourceFile, &tokenizer, MAGICWORD_BEGINNING_OF_FILE, NULL);
    }


    do
    {
        eType = tokenizer.GetNextToken(&token);

        if (eType == TOKEN_RESERVEDWORD && token.iVal >= RW_COUNT)
        {
            int iMagicWordNum = token.iVal - RW_COUNT;
            tokenizer.StringifyToken(&token);
            m_pSourceParsers[iMagicWordNum / MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER]->FoundMagicWord(pSourceFile, &tokenizer, iMagicWordNum % MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER, token.sVal);
        }
        else
        {
            int i;

            for (i=0; i < iNumWildcardMagicWords; i++)
            {
                if (eType == TOKEN_IDENTIFIER && DoesStringMatchWildcard(token.sVal, sMagicWords[iWildcardMagicWordIndices[i]]))
                {
                    int iMagicWordNum = iWildcardMagicWordIndices[i];
                    m_pSourceParsers[iMagicWordNum / MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER]->FoundMagicWord(pSourceFile, &tokenizer, iMagicWordNum % MAX_MAGIC_WORDS_PER_BASE_SOURCE_PARSER, token.sVal);
                    break;
                }
            }
        }
    } while (eType != TOKEN_NONE);

    for (i=0; i < m_iNumSourceParsers; i++)
    {
        m_pSourceParsers[i]->FoundMagicWord(pSourceFile, &tokenizer, MAGICWORD_END_OF_FILE, NULL);
    }

}

void GetStringWithSeparator(char *pOutString, char **ppInString, char separator)
{

    *pOutString = 0;

    while (**ppInString != 0 && **ppInString != separator)
    {
        *pOutString = **ppInString;
        pOutString++;
        (*ppInString)++;
    }

    if (**ppInString == separator)
    {
        (*ppInString)++;
    }

    *pOutString = 0;
}



void PutThingsIntoCommandLine(char *pCommandLine, char *pInputString, char *pPrefixString, bool bStripTrailingSlashes)
{
    int commandLineLength = (int) strlen(pCommandLine);


    do
    {
        char includeDirString[TOKENIZER_MAX_STRING_LENGTH];

        //ignore all leading semicolons
        while (pInputString[0] == ';')
        {
            pInputString++;
        }

        GetStringWithSeparator(includeDirString, &pInputString, ';');

        if (includeDirString[0] == 0)
        {
            break;
        }

        if (bStripTrailingSlashes)
        {
            RemoveSuffixIfThere(includeDirString, "\\");
            RemoveSuffixIfThere(includeDirString, "/");
        }

        int tempLen = (int)strlen(includeDirString);

        sprintf(pCommandLine + commandLineLength, "%s \"%s\" " ,pPrefixString, includeDirString);

        commandLineLength = (int)strlen(pCommandLine);
    } while (1);
}

void SourceParser::NukeCObjFile(char *pFileName)
{
    char fileNameWithoutExtension[MAX_PATH];
    sprintf(fileNameWithoutExtension, pFileName);
    TruncateStringAtLastOccurrence(fileNameWithoutExtension, '.');

    char buf[MAX_PATH];
    _snprintf(buf, sizeof(buf), "%s%s.obj", m_intDir.c_str(), fileNameWithoutExtension);
    buf[MAX_PATH - 1] = 0;
    remove(buf);
}

void ReplaceMacroInPlace(char *pString, char const* pMacroToFind, char const* pReplaceString)
{
    int iMacroLength = (int)strlen(pMacroToFind);
    int iStringLength = (int)strlen(pString);
    int iReplaceLength = (int)strlen(pReplaceString);

    if (iStringLength < iMacroLength)
    {
        return;
    }

    int i;

    for (i=0; i <= iStringLength - iMacroLength; i++)
    {
        if (strncmp(pMacroToFind, pString + i, iMacroLength) == 0)
        {
            memmove(pString + i + iReplaceLength, pString + i + iMacroLength, iStringLength - (i + iMacroLength) + 1);
            memcpy(pString + i, pReplaceString, iReplaceLength);

            iStringLength += iReplaceLength - iMacroLength;
            i += iReplaceLength - 1;

        }
    }
}

void ReplaceMacrosInPlace(char *pString, char const* pMacros[][2])
{
    int i;

    for (i=0; pMacros[i][0]; i++)
    {
        ReplaceMacroInPlace(pString, pMacros[i][0], pMacros[i][1]);
    }
}



//loads in all the dependencies that are stored in the FileListLoader. If one of the two dependent files doesn't exist, sets the other file
//to update. If both exist, store the dependency. Then all the stored dependencies will be processed
void SourceParser::LoadSavedDependenciesAndRemoveObsoleteFiles(void)
{
    int iNumSavedFiles = m_pFileListLoader->GetNumFiles();
    int iSavedFileNum;

    int iSavedFileNumToIndexArray[MAX_FILES_IN_PROJECT];


    for (iSavedFileNum=0; iSavedFileNum < iNumSavedFiles; iSavedFileNum++)
    {
        char *pSavedFileName = m_pFileListLoader->GetNthFileName(iSavedFileNum);

        iSavedFileNumToIndexArray[iSavedFileNum] = FindProjectFileIndex(pSavedFileName);

        if (iSavedFileNumToIndexArray[iSavedFileNum] == -1)
        {
            //this file no longer exists in the project
            m_IdentifierDictionary.DeleteAllFromFile(pSavedFileName);

            int i;

            for (i=0; i < m_iNumSourceParsers; i++)
            {
                m_pSourceParsers[i]->ResetSourceFile(pSavedFileName);
            }
        }
        else
        {
            m_iExtraDataPerFile[iSavedFileNumToIndexArray[iSavedFileNum]] = m_pFileListLoader->GetExtraData(iSavedFileNum);
        }
    }

    //now the iSavedFileNumToIndexArray is properly seeded

    for (iSavedFileNum=0; iSavedFileNum < iNumSavedFiles; iSavedFileNum++)
    {
        int iNumDependencies = m_pFileListLoader->GetNumDependencies(iSavedFileNum);

        int i;

        for (i=0; i < iNumDependencies; i++)
        {
            int iOtherFileNum = m_pFileListLoader->GetNthDependency(iSavedFileNum, i);

            //only process dependencies once, so only if otherFileNum > fileNum
            if (iOtherFileNum > iSavedFileNum)
            {
                int projFileIndex1 = iSavedFileNumToIndexArray[iSavedFileNum];
                int projFileIndex2 = iSavedFileNumToIndexArray[iOtherFileNum];

                if (projFileIndex1 == -1)
                {
                    if (projFileIndex2 != -1)
                    {
                        m_bFilesNeedToBeUpdated[projFileIndex2] = true;
                    }
                }
                else if (projFileIndex2 == -1)
                {
                    m_bFilesNeedToBeUpdated[projFileIndex1] = true;
                }
                else
                {
                    AddDependency(projFileIndex1, projFileIndex2);
                }
            }
        }
    }
}

//returns true if at least one file was set to udpate that was previously not set to update
//
//find all need-to-update files which have dependencies, and set all the other
//files they are dependent on to be need-to-update, and recurse. 
bool SourceParser::ProcessAllLoadedDependencies()
{
    bool bAtLeastOneSetToTrue = false;
    bool bNeedAnotherPass = true;

    while (bNeedAnotherPass)
    {
        bNeedAnotherPass = false;
        int iFileNum;

        for (iFileNum = 0; iFileNum < m_iNumProjectFiles; iFileNum++)
        {
            if (m_bFilesNeedToBeUpdated[iFileNum])
            {
                int i;

                for (i=0; i < m_iNumDependencies[iFileNum]; i++)
                {
                    int iOtherFileNum = m_iDependencies[iFileNum][i];

                    if (!m_bFilesNeedToBeUpdated[iOtherFileNum])
                    {
                        bAtLeastOneSetToTrue = true;
                        if (iOtherFileNum < iFileNum)
                        {
                            bNeedAnotherPass = true;
                        }

                        m_bFilesNeedToBeUpdated[iOtherFileNum] = true;
                    }
                }
            }
        }
    }

    return bAtLeastOneSetToTrue;
}

void SourceParser::ClearAllDependenciesForUpdatingFiles(void)
{
    int iFileNum;

    for (iFileNum = 0; iFileNum < m_iNumProjectFiles; iFileNum++)
    {
        if (m_bFilesNeedToBeUpdated[iFileNum])
        {
            m_iNumDependencies[iFileNum] = 0;
        }
    }
}

void SourceParser::AddDependency(int iFile1, int iFile2)
{
    int i;

    Tokenizer::StaticAssert(iFile1 != iFile2, "File can't depend on itself");

    bool bFound = false;
    for (i=0; i < m_iNumDependencies[iFile1]; i++)
    {
        if (m_iDependencies[iFile1][i] == iFile2)
        {
            bFound = true;
            break;
        }
    }

    if (!bFound)
    {
        Tokenizer::StaticAssert(m_iNumDependencies[iFile1] < MAX_DEPENDENCIES_SINGLE_FILE, "Too many dependencies");

        m_iDependencies[iFile1][m_iNumDependencies[iFile1]++] = iFile2;
    }

    bFound = false;
    for (i=0; i < m_iNumDependencies[iFile2]; i++)
    {
        if (m_iDependencies[iFile2][i] == iFile1)
        {
            bFound = true;
            break;
        }
    }

    if (!bFound)
    {
        Tokenizer::StaticAssert(m_iNumDependencies[iFile2] < MAX_DEPENDENCIES_SINGLE_FILE, "Too many dependencies");

        m_iDependencies[iFile2][m_iNumDependencies[iFile2]++] = iFile1;
    }
}


void SourceParser::ProcessAllFiles_ReadAll()
{
    ClearAllDependenciesForUpdatingFiles();

    int iFileNum;

    for (iFileNum=0; iFileNum < m_iNumProjectFiles; iFileNum++)
    {
        m_IdentifierDictionary.DeleteAllFromFile(m_ProjectFiles[iFileNum]);

        int i;

        for (i=0; i < m_iNumSourceParsers; i++)
        {
            m_pSourceParsers[i]->ResetSourceFile(m_ProjectFiles[iFileNum]);
        }

        m_iExtraDataPerFile[iFileNum] = 0;
    }

    for (iFileNum=0; iFileNum < m_iNumProjectFiles; iFileNum++)
    {
        ScanSourceFile(m_ProjectFiles[iFileNum]);
    }

    for (iFileNum=0; iFileNum < m_iNumProjectFiles; iFileNum++)
    {
        int i;

        for (i=0; i < m_iNumSourceParsers; i++)
        {
            char *pDependencies[MAX_DEPENDENCIES_SINGLE_FILE];

            int iNumDepenedencies = m_pSourceParsers[i]->ProcessDataSingleFile(m_ProjectFiles[iFileNum], pDependencies);

            int j;

            for (j=0; j < iNumDepenedencies; j++)
            {
                int iOtherFileNum = FindProjectFileIndex(pDependencies[j]);
                char errorString[1024];
                sprintf(errorString, "Dependency file <<%s>> not found (depended on by %s)", pDependencies[j], m_ProjectFiles[iFileNum]);

                Tokenizer::StaticAssert(iOtherFileNum != -1 && iOtherFileNum != iFileNum, errorString);

                AddDependency(iFileNum, iOtherFileNum);
            }
        }
    }
}

void SourceParser::ProcessAllFiles()
{

    ProcessAllLoadedDependencies();

    do
    {
        ClearAllDependenciesForUpdatingFiles();
        int iFileNum;


        for (iFileNum=0; iFileNum < m_iNumProjectFiles; iFileNum++)
        {
            if (m_bFilesNeedToBeUpdated[iFileNum])
            {
                m_IdentifierDictionary.DeleteAllFromFile(m_ProjectFiles[iFileNum]);

                int i;

                for (i=0; i < m_iNumSourceParsers; i++)
                {
                    m_pSourceParsers[i]->ResetSourceFile(m_ProjectFiles[iFileNum]);
                }

                m_iExtraDataPerFile[iFileNum] = 0;

            }
        }

        for (iFileNum=0; iFileNum < m_iNumProjectFiles; iFileNum++)
        {
            if (m_bFilesNeedToBeUpdated[iFileNum])
            {
                ScanSourceFile(m_ProjectFiles[iFileNum]);
            }
        }

        for (iFileNum=0; iFileNum < m_iNumProjectFiles; iFileNum++)
        {
            if (m_bFilesNeedToBeUpdated[iFileNum])
            {
                int i;

                for (i=0; i < m_iNumSourceParsers; i++)
                {
                    char *pDependencies[MAX_DEPENDENCIES_SINGLE_FILE];

                    int iNumDependencies = m_pSourceParsers[i]->ProcessDataSingleFile(m_ProjectFiles[iFileNum], pDependencies);

                    int j;

                    for (j=0; j < iNumDependencies; j++)
                    {
                        int iOtherFileNum = FindProjectFileIndex(pDependencies[j]);
                        char errorString[1024];
                        sprintf(errorString, "Dependency file <<%s>> not found", pDependencies[j]);

                        Tokenizer::StaticAssert(iOtherFileNum != -1 && iOtherFileNum != iFileNum, errorString);

                        AddDependency(iFileNum, iOtherFileNum);
                    }
                }
            }
        }
    }
    while (ProcessAllLoadedDependencies());
}


int SourceParser::FindProjectFileIndex(char *pFileName)
{
    int i;

    for (i=0; i < m_iNumProjectFiles; i++)
    {
        if (AreFilenamesEqual(pFileName, m_ProjectFiles[i]))
        {
            return i;
        }
    }

    return -1;
}


void SourceParser::SetExtraDataFlagForFile(char *pFileName, int iFlag)
{
    int iIndex = FindProjectFileIndex(pFileName);

    Tokenizer::StaticAssert(iIndex != -1, "Trying to set extra data flag for nonexistent file");

    m_iExtraDataPerFile[iIndex] |= iFlag;
}

bool SourceParser::MakeSpecialAutoRunFunction(void)
{
    if (StringIsInList(m_prjFileName.c_str(), sProjectNamesToExclude))
    {
        return false;
    }
    /*
    if (_stricmp(m_ShortenedProjectFileName, "UtilitiesLib") == 0)
    {
    return true;
    }

    for (i=0; i < m_iNumDependentLibraries; i++)
    {
    if (_stricmp(m_DependentLibraryNames[i], "UtilitiesLib") == 0)
    {
    return true;
    }
    }
    */
    return true;
}

#define MAX_WIKI_CATEGORIES 256

typedef struct SingleCommandStruct
{
    char *pCommandName;
    char *pCommandDescription;
    struct SingleCommandStruct *pNext;
} SingleCommandStruct;

class MasterWikiCommandCategory
{
public:
    MasterWikiCommandCategory(char *pCategoryName);
    ~MasterWikiCommandCategory();

    void LoadCommandsFromFile(char *pFileName);
    void SortCommands(void);

    void WriteCommands(FileWrapper *pOutFile);

    char *GetCategoryName();
    bool IsHidden() { return m_bIsHidden; }
    bool m_bProjectsWhichHaveIt[MAX_WIKI_PROJECTS];


private:
    bool m_bIsHidden;
    char m_CategoryName[256];
    SingleCommandStruct *m_pFirstCommand;


};


MasterWikiCommandCategory::MasterWikiCommandCategory(char *pCategoryName)
{
    strcpy(m_CategoryName, pCategoryName);

    m_bIsHidden = (_stricmp(pCategoryName, "hidden") == 0);

    m_pFirstCommand = NULL;
}

MasterWikiCommandCategory::~MasterWikiCommandCategory()
{
    while (m_pFirstCommand)
    {
        SingleCommandStruct *pNext = m_pFirstCommand->pNext;
        delete m_pFirstCommand->pCommandDescription;
        delete m_pFirstCommand->pCommandName;
        delete m_pFirstCommand;
        m_pFirstCommand = pNext;
    }
}

void MasterWikiCommandCategory::LoadCommandsFromFile(char *pFileName)
{
    Tokenizer tokenizer;

    Token token;
    enumTokenType eType;

    tokenizer.SetIgnoreQuotes(true);

    if (!tokenizer.LoadFromFile(pFileName))
    {
        return;
    }

    do
    {
        int iOffsetAtBeginningOfCommand;
        int iLineNum;
        char *pReadHeadAtBeginningOfCommand = tokenizer.GetReadHead();

        iOffsetAtBeginningOfCommand = tokenizer.GetOffset(&iLineNum);

        eType = tokenizer.GetNextToken(&token);

        if (eType == TOKEN_NONE)
        {
            return;
        }

        SingleCommandStruct *pNewCommand = new SingleCommandStruct;



        tokenizer.Assert(eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "h4") == 0, "Expected h4");
        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_DOT, "Expected . after h4");
        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 0, "Expected command name after h4.");

        pNewCommand->pCommandName = new char[token.iVal + 1];
        strcpy(pNewCommand->pCommandName, token.sVal);

        do
        {
            eType = tokenizer.CheckNextToken(&token);

            if (eType == TOKEN_NONE || eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "h4") == 0)
            {
                break;
            }
            else
            {
                eType = tokenizer.GetNextToken(&token);
            }
        }
        while (1);

        int iOffsetAtEndOfCommand;

        iOffsetAtEndOfCommand = tokenizer.GetOffset(&iLineNum);

        pNewCommand->pCommandDescription = new char[iOffsetAtEndOfCommand - iOffsetAtBeginningOfCommand + 1];
        memcpy(pNewCommand->pCommandDescription, pReadHeadAtBeginningOfCommand, iOffsetAtEndOfCommand - iOffsetAtBeginningOfCommand);
        pNewCommand->pCommandDescription[iOffsetAtEndOfCommand - iOffsetAtBeginningOfCommand] = 0;

        pNewCommand->pNext = m_pFirstCommand;
        m_pFirstCommand = pNewCommand;

        NormalizeNewlinesInString(pNewCommand->pCommandDescription);
    } while (1);
}

void MergeSortCommands(SingleCommandStruct **ppList, int iListLen)
{
    int iListLen1;
    int iListLen2;
    SingleCommandStruct *pList1;
    SingleCommandStruct *pList2;
    SingleCommandStruct *pNext;

    int i;

    SingleCommandStruct *pMasterListHead;
    SingleCommandStruct *pMasterListTail;


    if (iListLen < 2)
    {
        return;
    }

    iListLen1 = iListLen / 2;
    iListLen2 = iListLen - iListLen1;

    pList2 = pList1 = *ppList;

    for (i=0; i < iListLen1 - 1; i++)
    {
        pList2 = pList2->pNext;
    }

    pNext = pList2->pNext;
    pList2->pNext = NULL;
    pList2 = pNext;

    //now pList1 and pList2 point to totally separate lists
    MergeSortCommands(&pList1, iListLen1);
    MergeSortCommands(&pList2, iListLen2);

    if (StringComesAlphabeticallyBefore(pList1->pCommandName, pList2->pCommandName))
    {
        pMasterListHead = pMasterListTail = pList1;
        pList1 = pList1->pNext;
        pMasterListHead->pNext = NULL;
    }
    else
    {
        pMasterListHead = pMasterListTail = pList2;
        pList2 = pList2->pNext;
        pMasterListHead->pNext = NULL;
    }

    while (pList1 && pList2)
    {
        if (StringComesAlphabeticallyBefore(pList1->pCommandName, pList2->pCommandName))
        {
            pMasterListTail->pNext = pList1;
            pList1 = pList1->pNext;
            pMasterListTail->pNext->pNext = NULL;
            pMasterListTail = pMasterListTail->pNext;
        }
        else
        {
            pMasterListTail->pNext = pList2;
            pList2 = pList2->pNext;
            pMasterListTail->pNext->pNext = NULL;
            pMasterListTail = pMasterListTail->pNext;
        }
    }

    if (pList1)
    {
        pMasterListTail->pNext = pList1;
    }
    else
    {
        pMasterListTail->pNext = pList2;
    }

    *ppList = pMasterListHead;
}

void MasterWikiCommandCategory::SortCommands(void)
{
    int iCount = 0;
    SingleCommandStruct *pCounter = m_pFirstCommand;

    while (pCounter)
    {
        iCount++;
        pCounter = pCounter->pNext;
    }

    MergeSortCommands(&m_pFirstCommand, iCount);

}

void MasterWikiCommandCategory::WriteCommands(FileWrapper *pOutFile)
{
    SingleCommandStruct *pCounter = m_pFirstCommand;

    while (pCounter)
    {
        fw_fprintf(pOutFile, "%s\n\n", pCounter->pCommandDescription);

        pCounter = pCounter->pNext;
    }
}

char *MasterWikiCommandCategory::GetCategoryName()
{
    return m_CategoryName;
}



bool SourceParser::DoesVariableHaveValue(char const* pVarName, char const* pValue, bool bCheckFinalValueOnly)
{
    SourceParserVar *pVar = m_pFirstVar;


    while (pVar)
    {
        if (_stricmp(pVar->pVarName, pVarName) == 0)
        {
            if (bCheckFinalValueOnly)
            {
                int iLen = (int)strlen(pValue);
                if (strncmp(pValue, pVar->pValue + 1, iLen) == 0)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            char tempBuffer[256];
            sprintf(tempBuffer, " %s ", pValue);
            if (strstri(pVar->pValue, tempBuffer))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        pVar = pVar->pNext;
    }

    return false;
}

void SourceParser::AddVariableValue(char *pVarName, char *pValue)
{
    SourceParserVar *pVar = m_pFirstVar;


    while (pVar)
    {
        if (_stricmp(pVar->pVarName, pVarName) == 0)
        {
            int iCurLen = (int)strlen(pVar->pValue);
            int iAddLen = (int)strlen(pValue);
            char *pNewBuf = new char[iCurLen + iAddLen + 2];
            sprintf(pNewBuf, " %s%s", pValue, pVar->pValue);
            delete pVar->pValue;
            pVar->pValue = pNewBuf;
            return;
        }

        pVar = pVar->pNext;
    }

    pVar = new SourceParserVar;

    pVar->pNext = m_pFirstVar;
    m_pFirstVar = pVar;

    pVar->pVarName = STRDUP(pVarName);
    int iCurLen = (int)strlen(pValue);
    pVar->pValue = new char[iCurLen + 3];
    sprintf(pVar->pValue, " %s ", pValue);
}

void SourceParser::SetVariablesFromTokenizer(Tokenizer *pTokenizer, char *pStartingDirectory)
{
    enumTokenType eType;
    Token token;
    char varName[256];

    while (1)
    {
        eType = pTokenizer->GetNextToken(&token);

        if (eType == TOKEN_NONE)
        {
            return;
        }

        pTokenizer->Assert(eType == TOKEN_IDENTIFIER, "Expected identifier name to set");
        pTokenizer->Assert(token.iVal < 255, "Var name overflow");

        if (_stricmp(token.sVal, "#include") == 0)
        {
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Expected string after #include");
            char fullIncludeName[4096];
            if (strncmp(token.sVal, "..", 2) == 0)
            {
                sprintf(fullIncludeName, "%s%s", pStartingDirectory, token.sVal);
            }
            else
            {
                strcpy(fullIncludeName, token.sVal);
            }

            Tokenizer *pIncludeTokenizer = new Tokenizer;
            pIncludeTokenizer->SetExtraCharsAllowedInIdentifiers("#");
            if (!pIncludeTokenizer->LoadFromFile(fullIncludeName))
            {
                pTokenizer->Assertf(0, "Couldn't load include file %s", fullIncludeName);
            }


            char *pLastBackslash = strrchr(fullIncludeName, '\\');
            if (pLastBackslash)
            {
                *(pLastBackslash + 1) = 0;
            }

            SetVariablesFromTokenizer(pIncludeTokenizer, fullIncludeName);
        }
        else
        {
            strcpy(varName, token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_EQUALS, "Expected = after var name");

            do
            {
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 0, "expected identifier for var value");
                AddVariableValue(varName, token.sVal);

                pTokenizer->Assert2NextTokenTypesAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, TOKEN_RESERVEDWORD, RW_SEMICOLON, "Expected , or ;");
            }
            while (token.iVal != RW_SEMICOLON);
        }
    } 
}

void SourceParser::FindVariablesFileAndLoadVariables(void)
{
    char directoryToTry[MAX_PATH];
    char fileToTry[MAX_PATH];

    strcpy(directoryToTry, m_prjDir.c_str());
    int iDirectoryStrLen = (int)strlen(directoryToTry);

    while (iDirectoryStrLen)
    {
        sprintf(fileToTry, "%sStructParserVars.txt", directoryToTry);
        Tokenizer *pTokenizer = new Tokenizer;
        pTokenizer->SetExtraCharsAllowedInIdentifiers("#");
        bool bSuccess = pTokenizer->LoadFromFile(fileToTry);

        if (bSuccess)
        {
            SetVariablesFromTokenizer(pTokenizer, directoryToTry);
            delete pTokenizer;
            return;
        }

        delete pTokenizer;
        directoryToTry[--iDirectoryStrLen] = 0;

        while (iDirectoryStrLen && directoryToTry[iDirectoryStrLen - 1] != '\\')
        {
            directoryToTry[--iDirectoryStrLen] = 0;
        }
    }
}

