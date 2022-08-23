#include "pch.h"
#include "MagicCommandManager.h"
#include "strutils.h"
#include "SourceParser.h"
#include "AutoRunManager.h"




static char const* sSIntNames[] =
{
    "int",
    "short",
    "S8",
    "S16",
    "S32",
    "GlobalType",
    NULL
};

static char const* sUIntNames[] = 
{
    "U8",
    "U16",
    "U32",
    "EntityRef",
    "bool",
    "ContainerID",
    NULL
};

static char const* sFloatNames[] = 
{
    "float",
    "F32",
    NULL
};


static char const* sSInt64Names[] =
{
    "S64",
    NULL
};

static char const* sUInt64Names[] = 
{
    "U64",
    NULL
};

static char const* sFloat64Names[] = 
{
    "double",
    "F64",
    NULL
};

static char const* sStringNames[] = 
{
    "char",
    "const char",
    NULL
};

static char const* sSentenceNames[] = 
{
    "ACMD_SENTENCE",
    NULL
};

static char const* sVec3Names[] = 
{
    "Vec3",
    "const Vec3",
    NULL
};

static char const* sVec4Names[] = 
{
    "Vec4",
    "const Vec4",
    NULL
};

static char const* sMat4Names[] = 
{
    "Mat4",
    "const Mat4",
    NULL
};

static char const* sQuatNames[] = 
{
    "Quat",
    "const Quat",
    NULL
};

enum 
{
    MAGICWORD_AUTO_COMMAND,
    MAGICWORD_AUTO_COMMAND_REMOTE,
    MAGICWORD_AUTO_COMMAND_REMOTE_SLOW,
    MAGICWORD_AUTO_COMMAND_QUEUED,
    MAGICWORD_AUTO_INT,
    MAGICWORD_AUTO_FLOAT,
    MAGICWORD_AUTO_STRING,
    MAGICWORD_AUTO_SENTENCE,
};

void MagicCommandManager::CommandAssert(MAGIC_COMMAND_STRUCT *pField, bool bCondition, char const* pErrorMessage)
{
    if (!bCondition)
    {
        printf("%s(%d) : error S0000 : (StructParser) %s\n", pField->sourceFileName, pField->iLineNum, pErrorMessage);
        fflush(stdout);
        Sleep(100);
        exit(1);
    }
}


char const* MagicCommandManager::GetMagicWord(int iWhichMagicWord)
{
    switch (iWhichMagicWord)
    {
    case MAGICWORD_AUTO_COMMAND_REMOTE:
        return "AUTO_COMMAND_REMOTE";
    case MAGICWORD_AUTO_COMMAND_REMOTE_SLOW:
        return "AUTO_COMMAND_REMOTE_SLOW";
    case MAGICWORD_AUTO_COMMAND_QUEUED:
        return "AUTO_COMMAND_QUEUED";
    case MAGICWORD_AUTO_INT:
        return "AUTO_CMD_INT";
    case MAGICWORD_AUTO_FLOAT:
        return "AUTO_CMD_FLOAT";
    case MAGICWORD_AUTO_STRING:
        return "AUTO_CMD_STRING";
    case MAGICWORD_AUTO_SENTENCE:
        return "AUTO_CMD_SENTENCE";
    }

    return "AUTO_COMMAND";
    
}

MagicCommandManager::MagicCommandManager()
{
    m_bSomethingChanged = false;
    m_iNumMagicCommands = 0;
    m_iNumMagicCommandVars = 0;
    m_MagicCommandFileName[0] = 0;
    m_ShortMagicCommandFileName[0] = 0;

    memset(m_MagicCommands, 0, sizeof(m_MagicCommands));
    memset(m_MagicCommandVars, 0, sizeof(m_MagicCommandVars));
}

MagicCommandManager::~MagicCommandManager()
{
}

enum
{
    RW_PARSABLE = RW_COUNT,
};

static char const* sMagicCommandReservedWords[] =
{
    "PARSABLE",
    NULL
};

void MagicCommandManager::SetProjectPathAndName(char const* srcPath, char const* commonPath, char const* projectName)
{
    strcpy(m_ProjectName, projectName);

    sprintf(m_ShortMagicCommandFileName, "%s_commands_autogen", projectName);
    sprintf(m_MagicCommandFileName, "%s\\AutoGen\\%s.c", srcPath, m_ShortMagicCommandFileName);
    sprintf(m_TestClientFunctionsFileName, "%s\\AutoGen\\%s_CommandFuncs.c", srcPath, m_ShortMagicCommandFileName);
    sprintf(m_TestClientFunctionsHeaderName, "%s\\AutoGen\\%s_CommandFuncs.h", srcPath, m_ShortMagicCommandFileName);

    sprintf(m_RemoteFunctionsFileName, "%s\\AutoGen\\%s_autogen_RemoteFuncs.c", commonPath, projectName);
    sprintf(m_RemoteFunctionsHeaderName, "%s\\AutoGen\\%s_autogen_RemoteFuncs.h", commonPath, projectName);
    sprintf(m_SlowFunctionsFileName, "%s\\AutoGen\\%s_autogen_SlowFuncs.c", commonPath, projectName);
    sprintf(m_SlowFunctionsHeaderName, "%s\\AutoGen\\%s_autogen_SlowFuncs.h", commonPath, projectName);

    sprintf(m_QueuedFunctionsFileName, "%s\\AutoGen\\%s_autogen_QueuedFuncs.c", srcPath, projectName);
    sprintf(m_QueuedFunctionsHeaderName, "%s\\AutoGen\\%s_autogen_QueuedFuncs.h", srcPath, projectName);

    sprintf(m_ServerWrappersFileName, "%s\\AutoGen\\%s_autogen_ServerCmdWrappers.c", commonPath, projectName);
    sprintf(m_ServerWrappersHeaderFileName, "%s\\AutoGen\\%s_autogen_ServerCmdWrappers.h", commonPath, projectName);
    
    sprintf(m_ClientWrappersFileName, "%s\\AutoGen\\%s_autogen_ClientCmdWrappers.c", commonPath, projectName);
    sprintf(m_ClientWrappersHeaderFileName, "%s\\AutoGen\\%s_autogen_ClientCmdWrappers.h", commonPath, projectName);

    sprintf(m_ClientToTestClientWrappersFileName, "%s\\AutoGen\\%s_autogen_TestClientCmds.c", srcPath, projectName);
    sprintf(m_ClientToTestClientWrappersHeaderFileName, "%s\\AutoGen\\%s_autogen_TestClientCmds.h", srcPath, projectName);
}

bool MagicCommandManager::DoesFileNeedUpdating(char const* pFileName)
{
    return false;
}


bool MagicCommandManager::LoadStoredData(bool bForceReset)
{
    if (bForceReset)
    {
        m_bSomethingChanged = true;
        return false;
    }

    Tokenizer tokenizer;

    if (!tokenizer.LoadFromFile(m_MagicCommandFileName))
    {
        m_bSomethingChanged = true;
        return false;
    }

    if (!tokenizer.IsStringAtVeryEndOfBuffer("#endif"))
    {
        m_bSomethingChanged = true;
        return false;
    }

    tokenizer.SetExtraReservedWords(sMagicCommandReservedWords);
    tokenizer.SetCSourceStyleStrings(true);

    Token token;
    enumTokenType eType;

    do
    {
        eType = tokenizer.GetNextToken(&token);
        Tokenizer::StaticAssert(eType != TOKEN_NONE, "AUTOCOMMAND file corruption");
    } while (!(eType == TOKEN_RESERVEDWORD && token.iVal == RW_PARSABLE));

    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find number of magic commands");

    m_iNumMagicCommands = token.iVal;

    int iCommandNum;
    int i;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find magic command flags");
        pCommand->iCommandFlags = token.iVal;

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find magic command function name");
        strcpy(pCommand->functionName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find magic command command name");
        strcpy(pCommand->commandName, token.sVal);

        strcpy(pCommand->safeCommandName, token.sVal);
        MakeStringAllAlphaNum(pCommand->safeCommandName);

        for (i=0; i < MAX_COMMAND_ALIASES; i++)
        {
            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find magic command alias");
            strcpy(pCommand->commandAliases[i], token.sVal);
        }


        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find magic command access level");
        pCommand->iAccessLevel = token.iVal;

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 64, "Didn't find serverSpecificAccessLevel ser vername");
        strcpy(pCommand->serverSpecificAccessLevel_ServerName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find serverSpecificAccessLevel");
        pCommand->iServerSpecificAccessLevel = token.iVal;

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_PATH, "Didn't find magic command source file");
        strcpy(pCommand->sourceFileName, token.sVal);
    
        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find magic command line num");
        pCommand->iLineNum = token.iVal;

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, TOKENIZER_MAX_STRING_LENGTH, "Didn't find magic command comment");
        RemoveCStyleEscaping(pCommand->comment, token.sVal);

        for (i=0; i < MAX_COMMAND_SETS; i++)
        {
            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find magic command set name");
            strcpy(pCommand->commandSets[i], token.sVal);
        }

        for (i=0; i < MAX_COMMAND_CATEGORIES; i++)
        {
            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find magic command category name");
            strcpy(pCommand->commandCategories[i], token.sVal);
        }
    
        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find who I'm the error function for");
        strcpy(pCommand->commandWhichThisIsTheErrorFunctionFor, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find return arg type");
        pCommand->eReturnType = (enumMagicCommandArgType)token.iVal;

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMAND_ARGTYPE_NAME_LENGTH, "Didn't find return type name");
        strcpy(pCommand->returnTypeName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find queue name");
        strcpy(pCommand->queueName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find number of defines");

        pCommand->iNumDefines = token.iVal;
        int i;
        for (i=0; i < pCommand->iNumDefines; i++)
        {
            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find command IFDEF");
            strcpy(pCommand->defines[i], token.sVal);
        }



        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find number of arguments");
        
        pCommand->iNumArgs = token.iVal;


        for (i=0; i < pCommand->iNumArgs; i++)
        {
            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find int for arg type");
            pCommand->argTypes[i] = (enumMagicCommandArgType)(token.iVal);

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMAND_ARGTYPE_NAME_LENGTH, "Didn't find arg type name");
            strcpy(pCommand->argTypeNames[i], token.sVal);

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMAND_ARGNAME_LENGTH, "Didn't find arg name");
            strcpy(pCommand->argNames[i], token.sVal);
    
            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find namelist type");
            strcpy(pCommand->argNameListTypeNames[i], token.sVal);

            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find namelist data pointer");
            strcpy(pCommand->argNameListDataPointerNames[i], token.sVal);
    
            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find namelist data pointer-was-string");
            pCommand->argNameListDataPointerWasString[i] = (bool)(token.iVal == 1);
        }

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find number of expression tags");
        
        pCommand->iNumExpressionTags = token.iVal;

        for (i=0; i < MAX_COMMAND_SETS; i++)
        {
            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find expression tag");
            strcpy(pCommand->expressionTag[i], token.sVal);
        }

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find expression static checking function");
        strcpy(pCommand->expressionStaticCheckFunc, token.sVal);

        for (i=0; i < pCommand->iNumArgs; i++)
        {
            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 0, "Didn't find string for static check param type");
            strcpy(pCommand->expressionStaticCheckParamTypes[i], token.sVal);
        }

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find the expression function cost");
        pCommand->iExpressionCost = token.iVal;
    }
    
    tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find number of magic command variables");
    m_iNumMagicCommandVars = token.iVal;

    int iVarNum;
    for (iVarNum = 0; iVarNum < m_iNumMagicCommandVars; iVarNum++)
    {
        MAGIC_COMMANDVAR_STRUCT *pCommandVar = &m_MagicCommandVars[iVarNum];

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find magic command var flags");
        pCommandVar->iCommandFlags = token.iVal;

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find magic command var name");
        strcpy(pCommandVar->varCommandName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_PATH, "Didn't find magic command var source file name");
        strcpy(pCommandVar->sourceFileName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find magic command var type");
        pCommandVar->eVarType = (enumMagicCommandArgType)token.iVal;

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find magic command var access level");
        pCommandVar->iAccessLevel = (enumMagicCommandArgType)token.iVal;

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, 64, "Didn't find serverSpecificAccessLevel ser vername");
        strcpy(pCommandVar->serverSpecificAccessLevel_ServerName, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find serverSpecificAccessLevel");
        pCommandVar->iServerSpecificAccessLevel = token.iVal;

        for (i=0; i < MAX_COMMAND_SETS; i++)
        {
            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find magic command var set");
            strcpy(pCommandVar->commandSets[i], token.sVal);
        }

        for (i=0; i < MAX_COMMAND_CATEGORIES; i++)
        {
            tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find magic command var category");
            strcpy(pCommandVar->commandCategories[i], token.sVal);
        }

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, TOKENIZER_MAX_STRING_LENGTH, "Didn't find magic command var comment");
        RemoveCStyleEscaping(pCommandVar->comment, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find magic command callback func");
        strcpy(pCommandVar->callbackFunc, token.sVal);

        tokenizer.AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Didn't find magic command maxvalue");
        pCommandVar->iMaxValue = (enumMagicCommandArgType)token.iVal;


    }





    m_bSomethingChanged = false;

    return true;
}


void MagicCommandManager::ResetSourceFile(char const* pSourceFileName)
{
    int i = 0;

    while (i < m_iNumMagicCommands)
    {
        if (AreFilenamesEqual(m_MagicCommands[i].sourceFileName, pSourceFileName))
        {
            memcpy(&m_MagicCommands[i], &m_MagicCommands[m_iNumMagicCommands - 1], sizeof(MAGIC_COMMAND_STRUCT));
            m_iNumMagicCommands--;

            m_bSomethingChanged = true;
        }
        else
        {
            i++;
        }
    }

    i = 0;
    while (i < m_iNumMagicCommandVars)
    {
        if (AreFilenamesEqual(m_MagicCommandVars[i].sourceFileName, pSourceFileName))
        {
            memcpy(&m_MagicCommandVars[i], &m_MagicCommandVars[m_iNumMagicCommandVars - 1], sizeof(MAGIC_COMMANDVAR_STRUCT));
            m_iNumMagicCommandVars--;

            m_bSomethingChanged = true;
        }
        else
        {
            i++;
        }
    }
}

char const* MagicCommandManager::GetReturnValueMultiValTypeName(enumMagicCommandArgType eArgType)
{
    switch (eArgType)
    {
    case ARGTYPE_SINT:
    case ARGTYPE_UINT:
    case ARGTYPE_SINT64:
    case ARGTYPE_UINT64:
    case ARGTYPE_ENUM:
        return "MULTI_INT";
    case ARGTYPE_FLOAT:
    case ARGTYPE_FLOAT64:
        return "MULTI_FLOAT";
    case ARGTYPE_STRING:
    case ARGTYPE_SENTENCE:
    case ARGTYPE_ESCAPEDSTRING:
        return "MULTI_STRING";
    case ARGTYPE_STRUCT:
        return "MULTI_NP_POINTER";
    default:
        return "MULTI_NONE";
    }
}

char *MagicCommandManager::GetErrorFunctionName(char const* pCommandName)
{
    static char returnBuffer[MAX_MAGICCOMMANDNAMELENGTH];

    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pErrorCommand = &m_MagicCommands[iCommandNum];

        if (strcmp(pErrorCommand->commandWhichThisIsTheErrorFunctionFor, pCommandName) == 0)
        {
            sprintf(returnBuffer, "%s_MAGICCOMMANDWRAPPER", pErrorCommand->functionName);
            return returnBuffer;
        }
    }

    sprintf(returnBuffer, "NULL");
    return returnBuffer;
}

bool MagicCommandManager::ArgTypeIsExpressionOnly(enumMagicCommandArgType eArgType)
{
    return (eArgType >= ARGTYPE_EXPR_FIRST && eArgType <= ARGTYPE_EXPR_LAST);
}

bool MagicCommandManager::CommandHasExpressionOnlyArgumentsOrReturnVals(MAGIC_COMMAND_STRUCT *pCommand)
{
    if (ArgTypeIsExpressionOnly(pCommand->eReturnType))
    {
        return true;
    }

    int i;

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        if (ArgTypeIsExpressionOnly(pCommand->argTypes[i]))
        {
            return true;
        }
    }

    return false;
}

bool MagicCommandManager::CommandGetsWrittenOutInCommandSets(MAGIC_COMMAND_STRUCT *pCommand)
{
    return (!pCommand->commandWhichThisIsTheErrorFunctionFor[0] 
        && !(pCommand->iCommandFlags & COMMAND_FLAG_QUEUED) 
        && !CommandHasExpressionOnlyArgumentsOrReturnVals(pCommand)
        &&     !(pCommand->iCommandFlags & COMMAND_FLAG_EXPR_WRAPPER && !(pCommand->iCommandFlags & COMMAND_FLAG_GLOBAL) && !(pCommand->commandSets[0][0])));
}

void MagicCommandManager::WriteCommandSetData(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    int i;

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        if (pCommand->argTypes[i] == ARGTYPE_STRUCT)
        {
            fprintf(pOutFile, "extern ParseTable parse_%s[];\n", pCommand->argTypeNames[i]);
        }
    }

    if (pCommand->eReturnType == ARGTYPE_STRUCT)
    {
        fprintf(pOutFile, "extern ParseTable parse_%s[];\n", pCommand->returnTypeName);
    }

    if (pCommand->iCommandFlags & COMMAND_FLAG_SLOW_REMOTE)
    {
        fprintf(pOutFile, "Cmd cmdSetData_SlowRemoteCommand_%s[] =\n{\n", pCommand->functionName);
    }
    else if (pCommand->iCommandFlags & COMMAND_FLAG_REMOTE)
    {
        fprintf(pOutFile, "Cmd cmdSetData_RemoteCommand_%s[] =\n{\n", pCommand->functionName);
    }
    else if (pCommand->commandSets[0][0] && !(pCommand->iCommandFlags & COMMAND_FLAG_GLOBAL))
    {
        fprintf(pOutFile, "Cmd cmdSetData_%s_%s[] =\n{\n", pCommand->commandSets[0], pCommand->functionName);
    }
    else
    {
        fprintf(pOutFile, "Cmd cmdSetData_%s[] =\n{\n", pCommand->functionName);
    }
    

    int iAliasNum;

    char categoryString[2048] = "\" ";
    bool bAtLeastOneCategory = false;

    for (i=0; i < MAX_COMMAND_CATEGORIES; i++)
    {
        if (pCommand->commandCategories[i][0])
        {
            strcat(categoryString, pCommand->commandCategories[i]);
            strcat(categoryString, " ");
            bAtLeastOneCategory = true;
        }
    }

    if (bAtLeastOneCategory)
    {
        strcat(categoryString, "\"");
    }
    else
    {
        sprintf(categoryString, "NULL");
    }

    //AliasNum -1 means use commandName
    for (iAliasNum = -1; iAliasNum < MAX_COMMAND_ALIASES; iAliasNum++)
    {
        if (iAliasNum == -1 || pCommand->commandAliases[iAliasNum][0])
        {


            fprintf(pOutFile, "\t{ %d, \"%s\", s%sName, %s, {", pCommand->iAccessLevel, 
                iAliasNum == -1 ? pCommand->commandName : pCommand->commandAliases[iAliasNum],
                m_pParent->GetShortProjectName(),
                categoryString);

            int iNumNormalArgs = GetNumNormalArgs(pCommand);

            if (iNumNormalArgs == 0)
            {
                fprintf(pOutFile, "{0}");
            }
            else
            {
                int i;
                int iNumArgsFound = 0;

                for (i=0; i < pCommand->iNumArgs; i++)
                {
                    if (pCommand->argTypes[i] < ARGTYPE_FIRST_SPECIAL)
                    {
                        fprintf(pOutFile, GetArgDescriptionBlock(pCommand->argTypes[i], 
                            pCommand->argNames[i], pCommand->argTypeNames[i], pCommand->argNameListDataPointerNames[i], pCommand->argNameListTypeNames[i], pCommand->argNameListDataPointerWasString[i]));

                        iNumArgsFound++;

                        if (iNumArgsFound < iNumNormalArgs)
                        {
                            fprintf(pOutFile, ", ");
                        }
                    }
                }
            }

            char tempString1[TOKENIZER_MAX_STRING_LENGTH];
            char tempString2[TOKENIZER_MAX_STRING_LENGTH];
            strcpy(tempString1, pCommand->comment);
            char *pFirstNewLine = strchr(tempString1, '\n');

            if (pFirstNewLine)
            {
                *pFirstNewLine = 0;
            }

            AddCStyleEscaping(tempString2, tempString1, TOKENIZER_MAX_STRING_LENGTH);
            


            fprintf(pOutFile, "},%s%s,\n\t\t\"%s\", %s_MAGICCOMMANDWRAPPER,{NULL,%s,0,0,0}, %s}%s\n", 
                (pCommand->iCommandFlags & COMMAND_FLAG_HIDE) ? "CMDF_HIDEPRINT" : "0",
                (pCommand->iCommandFlags & COMMAND_FLAG_COMMANDLINE) ? " | CMDF_COMMANDLINE" : " | 0", 
                tempString2, pCommand->functionName,
                GetReturnValueMultiValTypeName(pCommand->eReturnType), 
                GetErrorFunctionName(pCommand->commandName),
                pCommand->commandAliases[0][0] ? "," : "");
        }
    }

    fprintf(pOutFile, "\t%s\n};\n\n\n",
        pCommand->commandAliases[0][0] ? "{0}": "");
}


void MagicCommandManager::WriteCommandVarSetData(FILE *pOutFile, MAGIC_COMMANDVAR_STRUCT *pCommandVar)
{
    if (pCommandVar->callbackFunc[0])
    {
        fprintf(pOutFile, "extern void %s(CMDARGS);\n", pCommandVar->callbackFunc);
    }

    if (pCommandVar->commandSets[0][0] && !(pCommandVar->iCommandFlags & COMMAND_FLAG_GLOBAL))
    {
        fprintf(pOutFile, "Cmd cmdVarSetData_%s_%s =\n", pCommandVar->commandSets[0],pCommandVar->varCommandName);
    }
    else
    {
        fprintf(pOutFile, "Cmd cmdVarSetData_%s =\n", pCommandVar->varCommandName);
    }

    char tempString1[TOKENIZER_MAX_STRING_LENGTH];
    char tempString2[TOKENIZER_MAX_STRING_LENGTH];
    strcpy(tempString1, pCommandVar->comment);
    char *pFirstNewLine = strchr(tempString1, '\n');

    if (pFirstNewLine)
    {
        *pFirstNewLine = 0;
    }

    AddCStyleEscaping(tempString2, tempString1, TOKENIZER_MAX_STRING_LENGTH);

    char categoryString[2048] = "\" ";
    bool bAtLeastOneCategory = false;
    int i;

    for (i=0; i < MAX_COMMAND_CATEGORIES; i++)
    {
        if (pCommandVar->commandCategories[i][0])
        {
            strcat(categoryString, pCommandVar->commandCategories[i]);
            strcat(categoryString, " ");
            bAtLeastOneCategory = true;
        }
    }

    if (bAtLeastOneCategory)
    {
        strcat(categoryString, "\"");
    }
    else
    {
        sprintf(categoryString, "NULL");
    }

    fprintf(pOutFile, "{\n\t%d, \"%s\", s%sName, %s, {{ NULL, %s, NULL, 0, %s, %d }},CMDF_PRINTVARS %s%s,\n\t\t\"%s\", %s, {NULL, MULTI_NONE,0,0,0}, %s\n};\n\n\n",
        pCommandVar->iAccessLevel, pCommandVar->varCommandName, 
        m_pParent->GetShortProjectName(),
        categoryString,
        GetReturnValueMultiValTypeName(pCommandVar->eVarType),
        pCommandVar->eVarType == ARGTYPE_SENTENCE ? "CMDAF_SENTENCE" : "0", 
        pCommandVar->iMaxValue,
        (pCommandVar->iCommandFlags & COMMAND_FLAG_HIDE) ? " | CMDF_HIDEPRINT" : "",
        (pCommandVar->iCommandFlags & COMMAND_FLAG_COMMANDLINE) ? " | CMDF_COMMANDLINE" : "",
        tempString2,
        pCommandVar->callbackFunc[0] ? pCommandVar->callbackFunc : "NULL",
        GetErrorFunctionName(pCommandVar->varCommandName));
    pCommandVar->bWritten = true;
}

/*
int MagicCommandManager::WriteCommandSet(FILE *pOutFile, char *pSetName, int iFlagToMatch)
{
    int iCommandNum;
    int iIndex = 0;
    int iVarNum;
    
    char tableName[MAX_MAGICCOMMANDNAMELENGTH];
    
    if (pSetName[0])
    {
        char tempName[MAX_MAGICCOMMANDNAMELENGTH];
        strcpy(tempName, pSetName);
        MakeStringAllAlphaNum(tempName);
        sprintf(tableName, "Auto_Cmds_%s_%s", m_ProjectName, tempName);


        Tokenizer::StaticAssert(m_iNumSetsWritten < MAX_OVERALL_SETS, "Too many total command sets");
        strcpy(m_SetsWritten[m_iNumSetsWritten++], pSetName);

    }
    else
    {
        sprintf(tableName, "Auto_Cmds_%s", m_ProjectName);
    }

    //func prototypes for callback funcs for variable commands
    for (iVarNum = 0; iVarNum < m_iNumMagicCommandVars; iVarNum++)
    {
        MAGIC_COMMANDVAR_STRUCT *pCommandVar = &m_MagicCommandVars[iVarNum];

        if (CommandVarIsInSet(pCommandVar, pSetName, iFlagToMatch))
        {
            if (pCommandVar->callbackFunc[0])
            {
                fprintf(pOutFile, "extern void %s(CMDARGS);\n", pCommandVar->callbackFunc);
            }
        }
    }

    //parse_x prototypes for struct args and return values
    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (CommandIsInSet(pCommand, pSetName, iFlagToMatch))
        {
            if (CommandGetsWrittenOutInCommandSets(pCommand))
            {
                int i;

                for (i=0; i < pCommand->iNumArgs; i++)
                {
                    if (pCommand->argTypes[i] == ARGTYPE_STRUCT)
                    {
                        fprintf(pOutFile, "extern ParseTable parse_%s[];\n", pCommand->argTypeNames[i]);
                    }
                }

                if (pCommand->eReturnType == ARGTYPE_STRUCT)
                {
                    fprintf(pOutFile, "extern ParseTable parse_%s[];\n", pCommand->returnTypeName);
                }
            }
        }
    }



    fprintf(pOutFile, "\n\nCmd %s[] =\n{\n", tableName);


    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (CommandIsInSet(pCommand, pSetName, iFlagToMatch))
        {
            if (CommandGetsWrittenOutInCommandSets(pCommand))
            {
                pCommand->bWritten = true;

                int iAliasNum;

                //AliasNum -1 means use commandName
                for (iAliasNum = -1; iAliasNum < MAX_COMMAND_ALIASES; iAliasNum++)
                {
                    if (iAliasNum == -1 || pCommand->commandAliases[iAliasNum][0])
                    {

                        fprintf(pOutFile, "\t{ %d, \"%s\", {", pCommand->iAccessLevel, 
                            iAliasNum == -1 ? pCommand->commandName : pCommand->commandAliases[iAliasNum]);

                        int iNumNormalArgs = GetNumNormalArgs(pCommand);

                        if (iNumNormalArgs == 0)
                        {
                            fprintf(pOutFile, "{0}");
                        }
                        else
                        {
                            int i;
                            int iNumArgsFound = 0;

                            for (i=0; i < pCommand->iNumArgs; i++)
                            {
                                if (pCommand->argTypes[i] < ARGTYPE_FIRST_SPECIAL)
                                {
                                    fprintf(pOutFile, GetInCodeCommandArgTypeNameFromArgType(pCommand->argTypes[i], pCommand->argTypeNames[i]));

                                    iNumArgsFound++;

                                    if (iNumArgsFound < iNumNormalArgs)
                                    {
                                        fprintf(pOutFile, ", ");
                                    }
                                }
                            }
                        }

                        char tempString1[TOKENIZER_MAX_STRING_LENGTH];
                        char tempString2[TOKENIZER_MAX_STRING_LENGTH];
                        strcpy(tempString1, pCommand->comment);
                        char *pFirstNewLine = strchr(tempString1, '\n');

                        if (pFirstNewLine)
                        {
                            *pFirstNewLine = 0;
                        }

                        AddCStyleEscaping(tempString2, tempString1, TOKENIZER_MAX_STRING_LENGTH);
                    
                        fprintf(pOutFile, "},%s,\n\t\t\"%s\", %s_MAGICCOMMANDWRAPPER,{%s,0,0,0}, %s},\n", 
                            (pCommand->iCommandFlags & COMMAND_FLAG_HIDE) ? "CMDF_HIDEPRINT" : "0", 
                            tempString2, pCommand->functionName,
                            GetReturnValueMultiValTypeName(pCommand->eReturnType), 
                            GetErrorFunctionName(pCommand->commandName));

                        iIndex++;
                    }
                }
            }
        }
    }

    
    for (iVarNum = 0; iVarNum < m_iNumMagicCommandVars; iVarNum++)
    {
        MAGIC_COMMANDVAR_STRUCT *pCommandVar = &m_MagicCommandVars[iVarNum];

        if (CommandVarIsInSet(pCommandVar, pSetName, iFlagToMatch))
        {

            char tempString1[TOKENIZER_MAX_STRING_LENGTH];
            char tempString2[TOKENIZER_MAX_STRING_LENGTH];
            strcpy(tempString1, pCommandVar->comment);
            char *pFirstNewLine = strchr(tempString1, '\n');

            if (pFirstNewLine)
            {
                *pFirstNewLine = 0;
            }

            AddCStyleEscaping(tempString2, tempString1, TOKENIZER_MAX_STRING_LENGTH);



            fprintf(pOutFile, "\t{ %d, \"%s\", {{ %s, NULL, 0, %s, %d }},CMDF_PRINTVARS %s,\n\t\t\"%s\", %s, {MULTI_NONE,0,0,0}, %s},\n",
                pCommandVar->iAccessLevel, pCommandVar->varCommandName, GetReturnValueMultiValTypeName(pCommandVar->eVarType),
                pCommandVar->eVarType == ARGTYPE_SENTENCE ? "CMDAF_SENTENCE" : "0", 
                pCommandVar->iMaxValue,
                pCommandVar->bFoundHide ? " | CMDF_HIDEPRINT" : "",
                tempString2,
                pCommandVar->callbackFunc[0] ? pCommandVar->callbackFunc : "NULL",
                GetErrorFunctionName(pCommandVar->varCommandName));
            pCommandVar->bWritten = true;

            strcpy(pCommandVar->tableName, tableName);
            pCommandVar->iIndexInTable = iIndex;

            iIndex++;
        }
    }

    fprintf(pOutFile, "\t{0}\n};\n\n\n");

    return iIndex;
}
*/

char *MagicCommandManager::GetFixedUpArgTypeNameForFuncPrototype(MAGIC_COMMAND_STRUCT *pCommand, int iArgNum)
{
    static char sBuf[MAX_MAGICCOMMAND_ARGTYPE_NAME_LENGTH + 64];

    sprintf(sBuf, "%s%s", pCommand->argTypeNames[iArgNum], (IsPointerType(pCommand->argTypes[iArgNum]) || pCommand->argTypes[iArgNum] == ARGTYPE_STRUCT) ? "*" : "");

    return sBuf;
}

char *MagicCommandManager::GetFixedUpArgTypeNameForReadArgs(MAGIC_COMMAND_STRUCT *pCommand, int iArgNum)
{
    static char sBuf[MAX_MAGICCOMMAND_ARGTYPE_NAME_LENGTH + 64];

    sprintf(sBuf, "%s%s", pCommand->argTypeNames[iArgNum], (IsPointerType(pCommand->argTypes[iArgNum]) 
        || IsDirectVersionOfPointerType(pCommand->argTypes[iArgNum]) || pCommand->argTypes[iArgNum] == ARGTYPE_STRUCT) ? "*" : "");

    if (strncmp(sBuf ,"const ", 6) == 0)
    {
        return sBuf + 6;
    }

    return sBuf;
}
char *MagicCommandManager::GetReturnTypeName(MAGIC_COMMAND_STRUCT *pCommand)
{
    static char sBuf[MAX_MAGICCOMMAND_ARGTYPE_NAME_LENGTH + 64];

    sprintf(sBuf, "%s%s", pCommand->returnTypeName, pCommand->eReturnType ==  ARGTYPE_STRUCT ? "*" : "");

    return sBuf;
}

void MagicCommandManager::VerifyCommandValidityPreWriteOut(MAGIC_COMMAND_STRUCT *pCommand)
{
    int i;

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        if (pCommand->argTypes[i] == ARGTYPE_STRUCT)
        {
            if (strstr(pCommand->argTypeNames[i], "const "))
            {
                char temp[MAX_MAGICCOMMAND_ARGTYPE_NAME_LENGTH];
                strcpy(temp, pCommand->argTypeNames[i]);
                strcpy(pCommand->argTypeNames[i], strstr(temp, "const ") + 6);
            }

            char errorString[1024];
            sprintf(errorString, "Unrecognized struct type name %s", pCommand->argTypeNames[i]);
            CommandAssert(pCommand,  m_pParent->GetDictionary()->FindIdentifier(pCommand->argTypeNames[i]) == IDENTIFIER_STRUCT
                || m_pParent->GetDictionary()->FindIdentifier(pCommand->argTypeNames[i]) == IDENTIFIER_NONE,
                errorString);
        }
    }

    if (pCommand->eReturnType == ARGTYPE_STRUCT)
    {
        char errorString[1024];
        sprintf(errorString, "Unrecognized struct type name %s", pCommand->returnTypeName);
        CommandAssert(pCommand,  m_pParent->GetDictionary()->FindIdentifier(pCommand->returnTypeName) == IDENTIFIER_STRUCT
            || m_pParent->GetDictionary()->FindIdentifier(pCommand->returnTypeName) == IDENTIFIER_NONE,
            errorString);
    }

}

bool MagicCommandManager::CommandNeedsNormalWrapper(MAGIC_COMMAND_STRUCT *pCommand)
{
    if (pCommand->iCommandFlags & COMMAND_FLAG_QUEUED)
    {
        return false;
    }

    if (CommandHasExpressionOnlyArgumentsOrReturnVals(pCommand))
    {
        return false;
    }

    if (pCommand->iCommandFlags & COMMAND_FLAG_EXPR_WRAPPER && !(pCommand->iCommandFlags & COMMAND_FLAG_GLOBAL) && !(pCommand->commandSets[0][0]))
    {
        return false;
    }

    return true;
}

int MagicCommandManager::MagicCommandComparator(const void *p1, const void *p2)
{
    return strcmp(((MAGIC_COMMAND_STRUCT*)p1)->functionName, ((MAGIC_COMMAND_STRUCT*)p2)->functionName);
}

int MagicCommandManager::MagicCommandVarComparator(const void *p1, const void *p2)
{
    return strcmp(((MAGIC_COMMANDVAR_STRUCT*)p1)->varCommandName, ((MAGIC_COMMANDVAR_STRUCT*)p2)->varCommandName);
}

bool MagicCommandManager::WriteOutData(void)
{
    int iCommandNum;
    int iVarNum;
    bool foundEnt = false;
    bool foundTran = false;
    int i;

    bool bAtLeastOneSlowCommand = false;
    bool bAtLeastOneExpressionListCommand = false;

    if (!m_bSomethingChanged)
    {
        return false;
    }

    qsort(m_MagicCommands, m_iNumMagicCommands, sizeof(MAGIC_COMMAND_STRUCT), MagicCommandComparator);
    qsort(m_MagicCommandVars, m_iNumMagicCommandVars, sizeof(MAGIC_COMMANDVAR_STRUCT), MagicCommandVarComparator);


    m_pParent->GetAutoRunManager()->ResetSourceFile("autogen_magiccommands");


    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        pCommand->bWritten = false;
        pCommand->bAlreadyWroteOutComment = false;

        VerifyCommandValidityPreWriteOut(pCommand);

        if (pCommand->iCommandFlags & COMMAND_FLAG_SLOW_REMOTE)
        {
            bAtLeastOneSlowCommand = true;
        }

        if (pCommand->iCommandFlags & COMMAND_FLAG_EXPR_WRAPPER)
        {
            bAtLeastOneExpressionListCommand = true;
        }
    }

    for (iVarNum = 0; iVarNum < m_iNumMagicCommandVars; iVarNum++)
    {
        MAGIC_COMMANDVAR_STRUCT *pCommandVar = &m_MagicCommandVars[iVarNum];
        pCommandVar->bWritten = false;
        pCommandVar->bAlreadyWroteOutComment = false;
    }

    WriteOutRemoteCommands();

    FILE *pOutFile = fopen_nofail(m_MagicCommandFileName, "wt");

    fprintf(pOutFile, "//This file contains data and prototypes for magic commands. It is autogenerated by StructParser\n\n\n"
                      "// autogenerated" "nocheckin\n\n"
                      "#include \"cmdparse.h\"\n"
                      "#include <utilitieslib/utils/textparser.h>"
                      // "\n#include \"globaltypes.h\""
                      "\n\n");

    fprintf(pOutFile, "static char s%sName[] = \"%s\";\n",
        m_pParent->GetShortProjectName(),
        m_pParent->GetShortProjectName());


    if (bAtLeastOneSlowCommand)
    {
        ForceIncludeFile(pOutFile, m_SlowFunctionsFileName);
    }

    if (bAtLeastOneExpressionListCommand)
    {
        fprintf(pOutFile, "//if one of these includes fails, you're trying to use expression list commands somewhere illegal... talk to Raoul\n");
        fprintf(pOutFile, "#include \"Expression.h\"\n");
        fprintf(pOutFile, "#include \"ScratchStack.h\"\n");
        fprintf(pOutFile, "#include \"mathutil.h\"\n");
        fprintf(pOutFile, "typedef struct BaseEntity BaseEntity;\n");
//        fprintf(pOutFile, "#include \"BaseEntity.h\"\n");
    }

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (CommandHasArgOfType(pCommand, ARGTYPE_ENTITY) && !foundEnt)
        {
            fprintf(pOutFile, "\n\n//About to make prototype for entExternGetCommandEntity and Entity.\n//If entExterGetCommandEntity fails to link, it's because you're in a project that doesn't\n//support entities. Talk to Ben or Alex\n");
            fprintf(pOutFile, "typedef struct BaseEntity BaseEntity;\ntypedef struct Entity Entity;\nextern Entity *entExternGetCommandEntity(CmdContext *context);\n");
            foundEnt = true;
        }

        if (CommandHasArgOfType(pCommand, ARGTYPE_TRANSACTIONCOMMAND) && !foundTran)
        {
            fprintf(pOutFile, "\n\n//About to try to include objTransactions.h because one of the AUTO_COMMANDS in\n//This file has a TransactionCommand arg.\n#include \"objTransactions.h\"\n\n");
            foundTran = true;
        }
    }


    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (CommandNeedsNormalWrapper(pCommand))
        {

            WriteOutGenericExternsAndPrototypesForCommand(pOutFile, pCommand, true);

            //then, create the extern function prototype
            fprintf(pOutFile, "extern %s %s(", (pCommand->iCommandFlags & COMMAND_FLAG_SLOW_REMOTE) ? "void" : GetReturnTypeName(pCommand), pCommand->functionName);

            WriteOutGenericArgListForCommand(pOutFile, pCommand, true, false);

            fprintf(pOutFile, "); // function defined in %s\n\n", pCommand->sourceFileName);

            fprintf(pOutFile, "void %s_MAGICCOMMANDWRAPPER(CMDARGS)\n{\n", pCommand->functionName);

            if (pCommand->eReturnType != ARGTYPE_NONE && !(pCommand->iCommandFlags & COMMAND_FLAG_SLOW_REMOTE))
            {
                fprintf(pOutFile, "\t%s retVal;\n", GetReturnTypeName(pCommand));
            }
        
            int iNumNormalArgs = GetNumNormalArgs(pCommand);

            if (iNumNormalArgs)
            {
                int iNumArgsFound = 0;

                fprintf(pOutFile, "\tREADARGS%d(", iNumNormalArgs);

                for (i=0; i < pCommand->iNumArgs; i++)
                {
                    if (pCommand->argTypes[i] < ARGTYPE_FIRST_SPECIAL)
                    {
                        iNumArgsFound++;
            
                        fprintf(pOutFile, "%s, arg%d", GetFixedUpArgTypeNameForReadArgs(pCommand, i), i);

                        if (iNumArgsFound < iNumNormalArgs)
                        {
                            fprintf(pOutFile, ", ");
                        }
                    }
                }

                fprintf(pOutFile, ");\n");
            }


            if (CommandHasArgOfType(pCommand, ARGTYPE_TRANSACTIONCOMMAND))
            {
                fprintf(pOutFile, "\tif (cmd_context->data) ((TransactionCommand *)cmd_context->data)->parseCommand = cmd;\n");
            }



            if (pCommand->eReturnType != ARGTYPE_NONE && !(pCommand->iCommandFlags & COMMAND_FLAG_SLOW_REMOTE))
            {
                fprintf(pOutFile, "\tretVal = %s(", pCommand->functionName);    
            }
            else
            {
                fprintf(pOutFile, "\t%s(", pCommand->functionName);
            }


            for (i=0; i < pCommand->iNumArgs; i++)
            {
                switch(pCommand->argTypes[i])
                {
                case ARGTYPE_ENTITY:
                    fprintf(pOutFile, "(%s)entExternGetCommandEntity(cmd_context)", pCommand->argTypeNames[i]);
                    break;
                case ARGTYPE_CMD:
                    fprintf(pOutFile, "cmd");
                    break;
                case ARGTYPE_CMDCONTEXT:
                    fprintf(pOutFile, "cmd_context");
                    break;
                case ARGTYPE_SLOWCOMMANDID:
                    fprintf(pOutFile, "cmd_context->iSlowCommandID");
                    break;
                case ARGTYPE_TRANSACTIONCOMMAND:
                    fprintf(pOutFile, "(TransactionCommand *)cmd_context->data");
                    break;
                case ARGTYPE_IGNORE:
                    fprintf(pOutFile, "NULL");
                    break;

                default:
                    fprintf(pOutFile, "%sarg%d", IsDirectVersionOfPointerType(pCommand->argTypes[i]) ? "*" : "", i);
                    break;
                }
                if (i < pCommand->iNumArgs - 1)
                {
                    fprintf(pOutFile, ", ");
                }
            }

            fprintf(pOutFile, ");\n");

            if (!(pCommand->iCommandFlags & COMMAND_FLAG_SLOW_REMOTE))
            {
                switch (pCommand->eReturnType)
                {
                case ARGTYPE_SINT:
                    fprintf(pOutFile, "\testrPrintf(cmd_context->output_msg, \"%%d\", retVal);\n");
                    fprintf(pOutFile, "\tcmd_context->return_val.intval = (S64)retVal;\n");
                    break;
                case ARGTYPE_UINT:
                    fprintf(pOutFile, "\testrPrintf(cmd_context->output_msg, \"%%u\", retVal);\n");
                    fprintf(pOutFile, "\tcmd_context->return_val.intval = (S64)retVal;\n");
                    break;
                case ARGTYPE_FLOAT:
                    fprintf(pOutFile, "\testrPrintf(cmd_context->output_msg, \"%%f\", retVal);\n");
                    fprintf(pOutFile, "\tcmd_context->return_val.floatval = (F64)retVal;\n");
                    break;
                case ARGTYPE_SINT64:
                    fprintf(pOutFile, "\testrPrintf(cmd_context->output_msg, \"%%I64d\", retVal);\n");
                    fprintf(pOutFile, "\tcmd_context->return_val.intval = (S64)retVal;\n");
                    break;
                case ARGTYPE_UINT64:
                    fprintf(pOutFile, "\testrPrintf(cmd_context->output_msg, \"%%I64u\", retVal);\n");
                    fprintf(pOutFile, "\tcmd_context->return_val.intval = (S64)retVal;\n");
                    break;
                case ARGTYPE_FLOAT64:
                    fprintf(pOutFile, "\testrPrintf(cmd_context->output_msg, \"%%lf\", retVal);\n");
                    fprintf(pOutFile, "\tcmd_context->return_val.floatval = (F64)retVal;\n");
                    break;
                case ARGTYPE_STRING:
                    fprintf(pOutFile, "\testrPrintf(cmd_context->output_msg, \"%%s\", retVal);\n");
                    fprintf(pOutFile, "\tcmd_context->return_val.ptr = retVal;\n");
                    break;
                case ARGTYPE_STRUCT:
                    fprintf(pOutFile, "\tParserWriteTextEscaped(cmd_context->output_msg, parse_%s, retVal, 0, 0);\n",
                        pCommand->returnTypeName);
                    fprintf(pOutFile, "\tStructDestroy(parse_%s, retVal);\n", pCommand->returnTypeName);
                    break;
                case ARGTYPE_VEC3_POINTER:
                    fprintf(pOutFile, "\testrPrintf(cmd_context->output_msg, \"%%f %%f %%f\", (*retVal)[0], (*retVal)[1], (*retVal)[2]);\n");
                    break;
                case ARGTYPE_VEC4_POINTER:
                    fprintf(pOutFile, "\testrPrintf(cmd_context->output_msg, \"%%f %%f %%f %%f\", (*retVal)[0], (*retVal)[1], (*retVal)[2], (*retVal)[3]);\n");
                    break;
                }
            }
            
            fprintf(pOutFile, "}\n\n\n");
        }
    }

    
    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (CommandGetsWrittenOutInCommandSets(pCommand))
        {
            WriteCommandSetData(pOutFile, pCommand);
        }
    }

    for (iVarNum = 0; iVarNum < m_iNumMagicCommandVars; iVarNum++)
    {
        MAGIC_COMMANDVAR_STRUCT *pCommandVar = &m_MagicCommandVars[iVarNum];

        WriteCommandVarSetData(pOutFile, pCommandVar);
    }

    fprintf(pOutFile, "\n\n");


    for (iVarNum = 0; iVarNum < m_iNumMagicCommandVars; iVarNum++)
    {
        MAGIC_COMMANDVAR_STRUCT *pCommandVar = &m_MagicCommandVars[iVarNum];

        fprintf(pOutFile, "void AutoGen_RegisterAutoCmd_%s(void *pAddress, int iSize)\n", pCommandVar->varCommandName);
        fprintf(pOutFile, "{\n\tcmdVarSetData_%s.data[0].ptr = pAddress;\n\tcmdVarSetData_%s.data[0].data_size = iSize;\n}\n\n",
            pCommandVar->varCommandName, pCommandVar->varCommandName);

        char autoRunName[MAX_MAGICCOMMANDNAMELENGTH];

        sprintf(autoRunName, "AutoGen_AutoRun_RegisterAutoCmd_%s", pCommandVar->varCommandName);
        m_pParent->GetAutoRunManager()->AddAutoRun(autoRunName, "autogen_magiccommands");
    }

    fprintf(pOutFile, "\n\n");

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        int i;

        for (i=0; i < MAX_COMMAND_SETS; i++)
        {
            if (pCommand->commandSets[i][0])
            {
                fprintf(pOutFile, "extern CmdList %s;\n", pCommand->commandSets[i]);
            }
        }
    }
                
    fprintf(pOutFile, "extern CmdList gRemoteCmdList;\nextern CmdList gSlowRemoteCmdList;\n");

    fprintf(pOutFile, "\n\n");



    char autoFuncName[256];
    sprintf(autoFuncName, "Add_Auto_Cmds_%s", m_ProjectName);
    fprintf(pOutFile, "void %s(void)\n{\n", autoFuncName);

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (!CommandGetsWrittenOutInCommandSets(pCommand))
        {


        }
        else if (pCommand->iCommandFlags & COMMAND_FLAG_SLOW_REMOTE)
        {
            fprintf(pOutFile, "\tcmdAddSingleCmdToList(&gSlowRemoteCmdList, &cmdSetData_SlowRemoteCommand_%s[0]);\n",
                pCommand->functionName);
        }
        else if (pCommand->iCommandFlags & COMMAND_FLAG_REMOTE)
        {
            fprintf(pOutFile, "\tcmdAddSingleCmdToList(&gRemoteCmdList, &cmdSetData_RemoteCommand_%s[0]);\n",
                pCommand->functionName);
        }
        else if (pCommand->iCommandFlags & COMMAND_FLAG_QUEUED)
        {
        }
        else
        {
            bool bClientOrServerOnly = false;

            if (pCommand->serverSpecificAccessLevel_ServerName[0])
            {
                if (pCommand->commandAliases[0][0])
                {
                    fprintf(pOutFile, "\tif (GetAppGlobalType() == %s)\n\t{\n",
                        pCommand->serverSpecificAccessLevel_ServerName);
                    fprintf(pOutFile, "\t\tint iAliasNum;\n\t\tfor (iAliasNum = 0; cmdSetData_%s[iAliasNum].name; iAliasNum++)\n\t\t{\n\t\t\tcmdSetData_%s[iAliasNum].access_level = %d;\n\t\t}\n\t}\n",
                        pCommand->functionName, pCommand->functionName, pCommand->iServerSpecificAccessLevel);
                }
                else
                {
                    fprintf(pOutFile, "\tif (GetAppGlobalType() == %s)\n\t{\n\t\tcmdSetData_%s[0].access_level = %d;\n\t}\n",
                        pCommand->serverSpecificAccessLevel_ServerName, pCommand->functionName, pCommand->iServerSpecificAccessLevel);
                }
            }

            if (pCommand->iCommandFlags & COMMAND_FLAG_CLIENT_ONLY)
            {
                bClientOrServerOnly = true;
                fprintf(pOutFile, "\tif (GetAppGlobalType() == GLOBALTYPE_CLIENT)\n\t{\n");
            }
            else if (pCommand->iCommandFlags & COMMAND_FLAG_SERVER_ONLY)
            {
                bClientOrServerOnly = true;
                fprintf(pOutFile, "\tif (GetAppGlobalType() == GLOBALTYPE_GAMESERVER)\n\t{\n");
            }

            if (pCommand->iCommandFlags & COMMAND_FLAG_PRIVATE)
            {
                if (pCommand->commandAliases[0][0])
                {
                    fprintf(pOutFile, "\t%scmdAddCmdArrayToList(&gPrivateCmdList, cmdSetData_%s);\n",
                        bClientOrServerOnly ? "\t" : "",
                        pCommand->functionName);
                }
                else
                {
                    fprintf(pOutFile, "\t%scmdAddSingleCmdToList(&gPrivateCmdList, &cmdSetData_%s[0]);\n",
                        bClientOrServerOnly ? "\t" : "",
                        pCommand->functionName);
                }
            }
            else if (pCommand->iCommandFlags & COMMAND_FLAG_EARLYCOMMANDLINE)
            {
                if (pCommand->commandAliases[0][0])
                {
                    fprintf(pOutFile, "\t%scmdAddCmdArrayToList(&gEarlyCmdList, cmdSetData_%s);\n",
                        bClientOrServerOnly ? "\t" : "",
                        pCommand->functionName);
                }
                else
                {
                    fprintf(pOutFile, "\t%scmdAddSingleCmdToList(&gEarlyCmdList, &cmdSetData_%s[0]);\n",
                        bClientOrServerOnly ? "\t" : "",
                        pCommand->functionName);
                }
            }
            else if (pCommand->commandSets[0][0] == 0 || pCommand->iCommandFlags & COMMAND_FLAG_GLOBAL)
            {
                if (pCommand->commandAliases[0][0])
                {
                    fprintf(pOutFile, "\t%scmdAddCmdArrayToList(&gGlobalCmdList, cmdSetData_%s);\n",
                        bClientOrServerOnly ? "\t" : "",
                        pCommand->functionName);
                }
                else
                {
                    fprintf(pOutFile, "\t%scmdAddSingleCmdToList(&gGlobalCmdList, &cmdSetData_%s[0]);\n",
                        bClientOrServerOnly ? "\t" : "",
                        pCommand->functionName);
                }
            }

            int i;

            for (i=0; i < MAX_COMMAND_SETS; i++)
            {
                if (pCommand->commandSets[i][0])
                {
                    if (pCommand->commandAliases[0][0])
                    {
                        if (pCommand->commandSets[0][0] && !(pCommand->iCommandFlags & COMMAND_FLAG_GLOBAL))
                        {
                            fprintf(pOutFile, "\t%scmdAddCmdArrayToList(&%s, cmdSetData_%s_%s);\n",
                                bClientOrServerOnly ? "\t" : "",
                                pCommand->commandSets[i],
                                pCommand->commandSets[0],pCommand->functionName);
                        }
                        else
                        {                        
                            fprintf(pOutFile, "\t%scmdAddCmdArrayToList(&%s, cmdSetData_%s);\n",
                                bClientOrServerOnly ? "\t" : "",
                                pCommand->commandSets[i],
                                pCommand->functionName);
                        }
                    }
                    else
                    {
                        if (pCommand->commandSets[0][0] && !(pCommand->iCommandFlags & COMMAND_FLAG_GLOBAL))
                        {
                            fprintf(pOutFile, "\t%scmdAddSingleCmdToList(&%s, &cmdSetData_%s_%s[0]);\n",
                                bClientOrServerOnly ? "\t" : "",
                                pCommand->commandSets[i],
                                pCommand->commandSets[0],pCommand->functionName);
                        }
                        else
                        {                        
                            fprintf(pOutFile, "\t%scmdAddSingleCmdToList(&%s, &cmdSetData_%s[0]);\n",
                                bClientOrServerOnly ? "\t" : "",
                                pCommand->commandSets[i],
                                pCommand->functionName);
                        }
                    }
                }
            }

            if (bClientOrServerOnly)
            {
                fprintf(pOutFile, "\t}\n\n");
            }
        }
    }


    for (iVarNum = 0; iVarNum < m_iNumMagicCommandVars; iVarNum++)
    {
        MAGIC_COMMANDVAR_STRUCT *pCommandVar = &m_MagicCommandVars[iVarNum];

        bool bClientOrServerOnly = false;

        if (pCommandVar->serverSpecificAccessLevel_ServerName[0])
        {
            fprintf(pOutFile, "\tif (GetAppGlobalType() == %s)\n\t{\n\t\tcmdVarSetData_%s.access_level = %d;\n\t}\n",
                pCommandVar->serverSpecificAccessLevel_ServerName, pCommandVar->varCommandName, pCommandVar->iServerSpecificAccessLevel);
        }

        if (pCommandVar->iCommandFlags & COMMAND_FLAG_CLIENT_ONLY)
        {
            bClientOrServerOnly = true;
            fprintf(pOutFile, "\tif (GetAppGlobalType() == GLOBALTYPE_CLIENT)\n\t{\n");
        }
        else if (pCommandVar->iCommandFlags & COMMAND_FLAG_SERVER_ONLY)
        {
            bClientOrServerOnly = true;
            fprintf(pOutFile, "\tif (GetAppGlobalType() == GLOBALTYPE_GAMESERVER)\n\t{\n");
        }

        if (pCommandVar->iCommandFlags & COMMAND_FLAG_PRIVATE)
        {
    
            fprintf(pOutFile, "\t%scmdAddSingleCmdToList(&gPrivateCmdList, &cmdVarSetData_%s);\n",
                bClientOrServerOnly ? "\t" : "",
                pCommandVar->varCommandName);
            
        }
        else if (pCommandVar->iCommandFlags & COMMAND_FLAG_EARLYCOMMANDLINE)
        {
    
            fprintf(pOutFile, "\t%scmdAddSingleCmdToList(&gEarlyCmdList, &cmdVarSetData_%s);\n",
                bClientOrServerOnly ? "\t" : "",
                pCommandVar->varCommandName);
            
        }
        else if (pCommandVar->commandSets[0][0] == 0 || pCommandVar->iCommandFlags & COMMAND_FLAG_GLOBAL)
        {
            fprintf(pOutFile, "\t%scmdAddSingleCmdToList(&gGlobalCmdList, &cmdVarSetData_%s);\n",
                bClientOrServerOnly ? "\t" : "",
                pCommandVar->varCommandName);
        }

        int i;

        for (i=0; i < MAX_COMMAND_SETS; i++)
        {
            if (pCommandVar->commandSets[i][0])
            {
                if (pCommandVar->commandSets[0][0] && !(pCommandVar->iCommandFlags & COMMAND_FLAG_GLOBAL))
                {
                    fprintf(pOutFile, "\t%scmdAddSingleCmdToList(&%s, &cmdVarSetData_%s_%s);\n",
                        bClientOrServerOnly ? "\t" : "",
                        pCommandVar->commandSets[i],
                        pCommandVar->commandSets[0],pCommandVar->varCommandName);
                }
                else
                {                
                    fprintf(pOutFile, "\t%scmdAddSingleCmdToList(&%s, &cmdVarSetData_%s);\n",
                        bClientOrServerOnly ? "\t" : "",
                        pCommandVar->commandSets[i],
                        pCommandVar->varCommandName);
                }
                
            }
        }

        if (bClientOrServerOnly)
        {
            fprintf(pOutFile, "\t}\n\n");
        }
    
    }




    fprintf(pOutFile, "};\n\n");

    m_pParent->GetAutoRunManager()->AddAutoRun(autoFuncName, "autogen_magiccommands");



    WriteOutExpressionListStuff(pOutFile);


    fprintf(pOutFile, "\n\n\n#if 0\nPARSABLE\n");

    fprintf(pOutFile, "%d\n", m_iNumMagicCommands);

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];
        
        Token commentToken;

        AddCStyleEscaping(commentToken.sVal, pCommand->comment, TOKENIZER_MAX_STRING_LENGTH);

        fprintf(pOutFile, " %d ", pCommand->iCommandFlags);

        fprintf(pOutFile, "\"%s\" \"%s\" ", 
            pCommand->functionName, pCommand->commandName);    
        
        for (i=0; i < MAX_COMMAND_ALIASES; i++)
        {
            fprintf(pOutFile, " \"%s\" ", pCommand->commandAliases[i]);
        }

        fprintf(pOutFile, "%d \"%s\" %d \"%s\" %d \"%s\" ",
            pCommand->iAccessLevel, pCommand->serverSpecificAccessLevel_ServerName, pCommand->iServerSpecificAccessLevel,
            pCommand->sourceFileName, pCommand->iLineNum, commentToken.sVal);

        for (i=0; i < MAX_COMMAND_SETS; i++)
        {
            fprintf(pOutFile, " \"%s\" ", pCommand->commandSets[i]);
        }

        for (i=0; i < MAX_COMMAND_CATEGORIES; i++)
        {
            fprintf(pOutFile, " \"%s\" ", pCommand->commandCategories[i]);
        }

        fprintf(pOutFile, " \"%s\" %d \"%s\" \"%s\" %d ",
            pCommand->commandWhichThisIsTheErrorFunctionFor,
            pCommand->eReturnType, pCommand->returnTypeName, pCommand->queueName, pCommand->iNumDefines);

        
        for (i=0; i < pCommand->iNumDefines; i++)
        {
            fprintf(pOutFile, "\"%s\"\n", pCommand->defines[i]);
        }

        fprintf(pOutFile, "%d\n", pCommand->iNumArgs);

        for (i=0; i < pCommand->iNumArgs; i++)
        {
            fprintf(pOutFile, "%d \"%s\" \"%s\" \"%s\" \"%s\" %d\n", pCommand->argTypes[i], pCommand->argTypeNames[i], pCommand->argNames[i],
                pCommand->argNameListTypeNames[i], pCommand->argNameListDataPointerNames[i], pCommand->argNameListDataPointerWasString[i]);
        }

        fprintf(pOutFile, "%d ", pCommand->iNumExpressionTags);
        
        for (i=0; i < MAX_COMMAND_SETS; i++)
        {
            fprintf(pOutFile, " \"%s\" ", pCommand->expressionTag[i]);
        }

        fprintf(pOutFile, "\n \"%s\" \n", pCommand->expressionStaticCheckFunc);

        for (i=0; i < pCommand->iNumArgs; i++)
        {
            fprintf(pOutFile, "\"%s\" ", pCommand->expressionStaticCheckParamTypes[i]);
        }

        fprintf(pOutFile, "%d\n", pCommand->iExpressionCost);
    }

    fprintf(pOutFile, "%d\n", m_iNumMagicCommandVars);

    for (iVarNum = 0; iVarNum < m_iNumMagicCommandVars; iVarNum++)
    {
        MAGIC_COMMANDVAR_STRUCT *pCommandVar = &m_MagicCommandVars[iVarNum];

        Token commentToken;

        fprintf(pOutFile, " %d ", pCommandVar->iCommandFlags);


        AddCStyleEscaping(commentToken.sVal, pCommandVar->comment, TOKENIZER_MAX_STRING_LENGTH);

        fprintf(pOutFile, "\"%s\" \"%s\" %d %d \"%s\" %d ",
            pCommandVar->varCommandName, pCommandVar->sourceFileName,
            pCommandVar->eVarType, pCommandVar->iAccessLevel, pCommandVar->serverSpecificAccessLevel_ServerName, pCommandVar->iServerSpecificAccessLevel);
        
        for (i=0; i < MAX_COMMAND_SETS; i++)
        {
            fprintf(pOutFile, " \"%s\" ", pCommandVar->commandSets[i]);
        }

        for (i=0; i < MAX_COMMAND_CATEGORIES; i++)
        {
            fprintf(pOutFile, " \"%s\" ", pCommandVar->commandCategories[i]);
        }

        fprintf(pOutFile, " \"%s\" \"%s\" %d\n", commentToken.sVal,
            pCommandVar->callbackFunc,pCommandVar->iMaxValue);

    }


    fprintf(pOutFile, "#endif\n");

    fclose(pOutFile);

    WriteOutFilesForTestClient();
    WriteOutQueuedCommands();

    if (AtLeastOneCommandHasFlag(COMMAND_FLAG_SERVER_WRAPPER))
    {
        WriteOutServerWrappers();
    }

    if (AtLeastOneCommandHasFlag(COMMAND_FLAG_CLIENT_WRAPPER))
    {
        WriteOutClientWrappers();
    }

    if (CurrentProjectIsTestClient())
    {
        WriteOutClientToTestClientWrappers();
    }

    return true;
}

void MagicCommandManager::WriteOutExpressionListStuff(FILE *pOutFile)
{
    bool bFoundAtLeastOneExpressionFunc = false;
    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & COMMAND_FLAG_EXPR_WRAPPER)
        {
            bool bHasErrString = false;
            bool bErrEString = false;

            if (pCommand->iNumExpressionTags)
                bFoundAtLeastOneExpressionFunc = true;

            //first, write out the function prototype
            fprintf(pOutFile, "extern %s %s(",
                pCommand->returnTypeName, pCommand->functionName);

            int iArgNum;
            
            for (iArgNum=0; iArgNum < pCommand->iNumArgs; iArgNum++)
            {
                fprintf(pOutFile, "%s %s%s",
                    pCommand->argTypeNames[iArgNum], pCommand->argNames[iArgNum],
                    iArgNum < pCommand->iNumArgs - 1 ? ", " : "");
            }

            fprintf(pOutFile, ");\n\n");

            //now create the wrapper function
            fprintf(pOutFile, "ExprFuncReturnVal %s_EXPRFUNCWRAPPER(MultiVal** args, MultiVal* retval, ExprContext* context, char** errEString)\n{\n",
                pCommand->functionName);

            //declare all local variables
            for (iArgNum=0; iArgNum < pCommand->iNumArgs; iArgNum++)
            {
                switch (pCommand->argTypes[iArgNum])
                {
                case ARGTYPE_SINT:
                case ARGTYPE_UINT:
                case ARGTYPE_FLOAT:
                case ARGTYPE_SINT64:
                case ARGTYPE_UINT64:
                case ARGTYPE_FLOAT64:
                case ARGTYPE_STRING:
                    fprintf(pOutFile, "\t%s arg%d;\n", 
                        pCommand->argTypeNames[iArgNum], iArgNum);
                    break;

                case ARGTYPE_EXPR_SUBEXPR_IN:
                    fprintf(pOutFile, "\tAcmdType_ExprSubExpr arg%d;\n", iArgNum);
                    break;

                case ARGTYPE_EXPR_ENTARRAY_IN:
                case ARGTYPE_EXPR_ENTARRAY_IN_OUT:
                case ARGTYPE_EXPR_ENTARRAY_OUT:
                    fprintf(pOutFile, "\tBaseEntity ***arg%d;\n", iArgNum);
                    break;

                case ARGTYPE_EXPR_LOC_MAT4_IN:
                case ARGTYPE_EXPR_LOC_MAT4_OUT:
                    fprintf(pOutFile, "\tVec3* arg%d;\n", iArgNum);
                    break;

                case ARGTYPE_EXPR_INT_OUT:
                    fprintf(pOutFile, "\tint arg%d;\n", iArgNum);
                    break;
                case ARGTYPE_EXPR_FLOAT_OUT:
                    fprintf(pOutFile, "\tfloat arg%d;\n", iArgNum);
                    break;
                case ARGTYPE_EXPR_STRING_OUT:
                    fprintf(pOutFile, "\tconst char *arg%d;\n", iArgNum);
                    break;

                case ARGTYPE_ENTITY:
                    fprintf(pOutFile, "\tBaseEntity *arg%d;\n", iArgNum);
                    break;

                case ARGTYPE_EXPR_ERRSTRING:
                    fprintf(pOutFile, "\tchar **err;\n");
                    bHasErrString = true;
                    bErrEString = true;
                    break;

                case ARGTYPE_EXPR_ERRSTRING_STATIC:
                    fprintf(pOutFile, "\tchar *err;\n");
                    bHasErrString = true;
                    break;

                case ARGTYPE_EXPR_EXPRCONTEXT:
                    break;

                default:
                    CommandAssert(pCommand, 0, "Unsupported argument type for expression list command");
                    break;

                }
            }

            fprintf(pOutFile, "\n\n");

            //declare variable for retval
            if (pCommand->eReturnType != ARGTYPE_NONE)
            {
                switch(pCommand->eReturnType)
                {
                case ARGTYPE_SINT:
                case ARGTYPE_UINT:
                case ARGTYPE_FLOAT:
                case ARGTYPE_SINT64:
                case ARGTYPE_UINT64:
                case ARGTYPE_FLOAT64:
                case ARGTYPE_STRING:
                case ARGTYPE_EXPR_FUNCRETURNVAL:
                    fprintf(pOutFile, "\t%s localRetVal;\n", pCommand->returnTypeName);
                    break;
                default:
                    CommandAssert(pCommand, 0, "Unsupported return type for expression list command");
                    break;
                }
            }

            fprintf(pOutFile, "\n\n");


            int iInputArgNum = 0;

            //now initialize all local variables
            for (iArgNum=0; iArgNum < pCommand->iNumArgs; iArgNum++)
            {
                switch (pCommand->argTypes[iArgNum])
                {
                case ARGTYPE_SINT:
                case ARGTYPE_UINT:
                case ARGTYPE_SINT64:
                case ARGTYPE_UINT64:
                    fprintf(pOutFile, "\targ%d = args[%d]->intval;\n", iArgNum, iInputArgNum++);
                    break;
                case ARGTYPE_FLOAT:
                case ARGTYPE_FLOAT64:
                    fprintf(pOutFile, "\targ%d = QuickGetFloat(args[%d]);\n", iArgNum, iInputArgNum++);
                    break;
                case ARGTYPE_STRING:
                    fprintf(pOutFile, "\targ%d = args[%d]->str;\n", iArgNum, iInputArgNum++);
                    break;
                case ARGTYPE_EXPR_SUBEXPR_IN:
                    fprintf(pOutFile, "\targ%d.exprPtr = args[%d]->ptr;\n", iArgNum, iInputArgNum++);
                    fprintf(pOutFile, "\targ%d.exprSize = args[%d]->intval;\n", iArgNum, iInputArgNum++);
                    break;
                case ARGTYPE_EXPR_ENTARRAY_IN:
                case ARGTYPE_EXPR_ENTARRAY_IN_OUT:
                    fprintf(pOutFile, "\targ%d = args[%d]->entarray;\n", iArgNum, iInputArgNum++);
                    break;

                case ARGTYPE_EXPR_ENTARRAY_OUT:
                    fprintf(pOutFile, "\targ%d = exprContextGetNewEntArray(context);\n", iArgNum);
                    break;

                case ARGTYPE_EXPR_LOC_MAT4_IN:
                    fprintf(pOutFile, "\targ%d = args[%d]->vecptr;\n", iArgNum, iInputArgNum++);
                    break;

                case ARGTYPE_EXPR_LOC_MAT4_OUT:
                    fprintf(pOutFile, "\targ%d = exprContextAllocScratchMemory(context, sizeof(Mat4));\n", iArgNum);
                    fprintf(pOutFile, "\tcopyMat4(unitmat,arg%d);\n", iArgNum);
                    break;

                case ARGTYPE_ENTITY:
                    fprintf(pOutFile, "\targ%d = exprContextGetSelfPtr(context);\n", iArgNum);
                    fprintf(pOutFile, "\tif(!arg%d)\n", iArgNum);
                    fprintf(pOutFile, "\t{\n");
                    fprintf(pOutFile, "\t\tretval->type = MULTI_INVALID;\n");
                    fprintf(pOutFile, "\t\tretval->str = \"Context does not have a self ptr\";\n");
                    fprintf(pOutFile, "\t\treturn ExprFuncReturnError;\n");
                    fprintf(pOutFile, "\t}\n");
                    break;

                case ARGTYPE_EXPR_ERRSTRING:
                    fprintf(pOutFile, "\terr = errEString;\n");
                    break;

                case ARGTYPE_EXPR_ERRSTRING_STATIC:
                    fprintf(pOutFile, "\terr = NULL;\n");
                    break;
                }
            }

            fprintf(pOutFile, "\n\n");

            //now call the function
            fprintf(pOutFile, "\t%s%s(", 
                pCommand->eReturnType == ARGTYPE_NONE ? "" : "localRetVal = ",
                pCommand->functionName);

            for (iArgNum=0; iArgNum < pCommand->iNumArgs; iArgNum++)
            {
                switch (pCommand->argTypes[iArgNum])
                {
                case ARGTYPE_SINT:
                case ARGTYPE_UINT:
                case ARGTYPE_FLOAT:
                case ARGTYPE_SINT64:
                case ARGTYPE_UINT64:
                case ARGTYPE_FLOAT64:
                case ARGTYPE_STRING:
                case ARGTYPE_ENTITY:
                case ARGTYPE_EXPR_ENTARRAY_IN:
                case ARGTYPE_EXPR_ENTARRAY_IN_OUT:
                case ARGTYPE_EXPR_ENTARRAY_OUT:
                case ARGTYPE_EXPR_LOC_MAT4_IN:
                case ARGTYPE_EXPR_LOC_MAT4_OUT:
                        fprintf(pOutFile, "arg%d%s", iArgNum,
                        iArgNum < pCommand->iNumArgs - 1 ? ", " : "");
                    break;

                case ARGTYPE_EXPR_SUBEXPR_IN:
                case ARGTYPE_EXPR_INT_OUT:
                case ARGTYPE_EXPR_FLOAT_OUT:
                case ARGTYPE_EXPR_STRING_OUT:
                    fprintf(pOutFile, "&arg%d%s", iArgNum,
                        iArgNum < pCommand->iNumArgs - 1 ? ", " : "");
                    break;

                case ARGTYPE_EXPR_ERRSTRING:
                    fprintf(pOutFile, "err%s",
                        iArgNum < pCommand->iNumArgs - 1 ? ", " : "");
                    break;

                case ARGTYPE_EXPR_ERRSTRING_STATIC:
                    fprintf(pOutFile, "&err%s",
                        iArgNum < pCommand->iNumArgs - 1 ? ", " : "");
                    break;

                case ARGTYPE_EXPR_EXPRCONTEXT:
                    fprintf(pOutFile, "context%s", iArgNum < pCommand->iNumArgs - 1 ? ", " : "");
                    break;
                }
            }

            fprintf(pOutFile, ");\n\n");

            //called the function, now do something with the return val

            if (pCommand->eReturnType == ARGTYPE_EXPR_FUNCRETURNVAL)
            {
                fprintf(pOutFile, "\tif (localRetVal == ExprFuncReturnError)\n\t{\n");

                if(bHasErrString)
                {
                    fprintf(pOutFile, "\t\tretval->type = MULTI_INVALID;\n\t\tretval->str = ");
                    if(bErrEString)
                        fprintf(pOutFile, "*err");
                    else
                        fprintf(pOutFile, "err");
                    fprintf(pOutFile, ";\n");
                }
                    
                fprintf(pOutFile, "\t\treturn ExprFuncReturnError;\n\t}\n\n");
            }

            switch (pCommand->eReturnType)
            {
            case ARGTYPE_SINT:
            case ARGTYPE_UINT:
            case ARGTYPE_SINT64:
            case ARGTYPE_UINT64:
                fprintf(pOutFile, "\tretval->type = MULTI_INT;\n\tretval->intval = localRetVal;\n\n");
                break;

            case ARGTYPE_FLOAT:
            case ARGTYPE_FLOAT64:
                fprintf(pOutFile, "\tretval->type = MULTI_FLOAT;\n\tretval->floatval = localRetVal;\n\n");
                break;

            case ARGTYPE_STRING:
                fprintf(pOutFile, "\tretval->type = MULTI_STRING;\n\tretval->str = localRetVal;\n\n");
                break;
            }

            for (iArgNum=0; iArgNum < pCommand->iNumArgs; iArgNum++)
            {
                switch (pCommand->argTypes[iArgNum])
                {
                case ARGTYPE_EXPR_ENTARRAY_IN_OUT:
                case ARGTYPE_EXPR_ENTARRAY_OUT:
                    fprintf(pOutFile, "\tretval->type = MULTI_NP_ENTITYARRAY;\n\tretval->entarray = arg%d;\n", iArgNum);
                    break;

                case ARGTYPE_EXPR_ENTARRAY_IN:
                    fprintf(pOutFile, "\texprContextClearEntArray(context, arg%d);\n", iArgNum);
                    break;
                case ARGTYPE_EXPR_LOC_MAT4_IN:
                    fprintf(pOutFile, "\texprContextFreeScratchMemory(context, arg%d);\n", iArgNum);
                    break;

                case ARGTYPE_EXPR_INT_OUT:
                    fprintf(pOutFile, "\tretval->type = MULTI_INT;\n\tretval->intval = arg%d;\n", iArgNum);
                    break;
                case ARGTYPE_EXPR_FLOAT_OUT:
                    fprintf(pOutFile, "\tretval->type = MULTI_FLOAT;\n\tretval->floatval = arg%d;\n", iArgNum);
                    break;
                case ARGTYPE_EXPR_STRING_OUT:
                    fprintf(pOutFile, "\tretval->type = MULTI_STRING;\n\tretval->str = arg%d;\n", iArgNum);
                    break;
                case ARGTYPE_EXPR_LOC_MAT4_OUT:
                    fprintf(pOutFile, "\tretval->type = MULTIOP_LOC_MAT4;\n\tretval->ptr = arg%d;\n", iArgNum);
                }
            }

            if (pCommand->eReturnType == ARGTYPE_EXPR_FUNCRETURNVAL)
            {
                fprintf(pOutFile, "\treturn localRetVal;\n");
            }
            else
            {
                fprintf(pOutFile, "\treturn ExprFuncReturnFinished;\n");
            }


            fprintf(pOutFile, "}\n\n\n");
        }
    }

    /*
    int iNumExprLists = 0;
    char exprLists[MAX_EXPRESSION_LISTS][MAX_MAGICCOMMANDNAMELENGTH];

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->expressionList[0])
        {
            int i;
            bool bFound = false;

            for (i=0; i < iNumExprLists; i++)
            {
                if (strcmp(exprLists[i], pCommand->expressionList) == 0)
                {
                    bFound = true;
                    break;
                }
            }

            if (!bFound)
            {
                CommandAssert(pCommand, iNumExprLists < MAX_EXPRESSION_LISTS, "Too many distinct expression lists");

                strcpy(exprLists[iNumExprLists++], pCommand->expressionList);
            }
        }
    }

    int iListNum;

    for (iListNum = 0; iListNum < iNumExprLists; iListNum++)
    */
    if (bFoundAtLeastOneExpressionFunc)
    {
        fprintf(pOutFile, "ExprFuncDesc AutoCmd_%s_ExprList[] = \n{\n", m_ProjectName);

        for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
        {
            MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];


            //if (strcmp(pCommand->expressionList, exprLists[iListNum]) == 0)
            if (pCommand->iNumExpressionTags)
            {
                fprintf(pOutFile, "\t{ \"%s\", %s_EXPRFUNCWRAPPER, { ",
                    pCommand->commandName, pCommand->functionName);

                char const* pRetTypeString = "MULTI_NONE";
                char argTypeStaticCheckTypes[MAX_MAGICCOMMAND_ARGS * 64];

                argTypeStaticCheckTypes[0] = '\0';

                switch (pCommand->eReturnType)
                {
                case ARGTYPE_SINT:
                case ARGTYPE_UINT:
                case ARGTYPE_SINT64:
                case ARGTYPE_UINT64:
                    pRetTypeString = "MULTI_INT";
                    break;
                case ARGTYPE_FLOAT:
                case ARGTYPE_FLOAT64:
                    pRetTypeString = "MULTI_FLOAT";
                    break;
                case ARGTYPE_STRING:
                    pRetTypeString = "MULTI_STRING";
                    break;
                }

                int iArgNum;

                for (iArgNum=0; iArgNum < pCommand->iNumArgs; iArgNum++)
                {
                    int validExprFuncArg = false;
                    switch (pCommand->argTypes[iArgNum])
                    {
                    case ARGTYPE_SINT:
                    case ARGTYPE_UINT:
                    case ARGTYPE_SINT64:
                    case ARGTYPE_UINT64:
                        fprintf(pOutFile, "MULTI_INT, ");
                        validExprFuncArg = true;
                        break;
                    case ARGTYPE_FLOAT:
                    case ARGTYPE_FLOAT64:
                        fprintf(pOutFile, "MULTI_FLOAT, ");
                        validExprFuncArg = true;
                        break;
                    case ARGTYPE_STRING:
                        fprintf(pOutFile, "MULTI_STRING, ");
                        validExprFuncArg = true;
                        break;
                    case ARGTYPE_EXPR_SUBEXPR_IN:
                        fprintf(pOutFile, "MULTIOP_NP_STACKPTR, MULTI_INT, ");
                        validExprFuncArg = true;
                        break;
                    case ARGTYPE_EXPR_LOC_MAT4_IN:
                        fprintf(pOutFile, "MULTIOP_LOC_MAT4, ");
                        validExprFuncArg = true;
                        break;
                    case ARGTYPE_EXPR_ENTARRAY_IN:
                        fprintf(pOutFile, "MULTI_NP_ENTITYARRAY, ");
                        validExprFuncArg = true;
                        break;

                    case ARGTYPE_EXPR_ENTARRAY_IN_OUT:
                        fprintf(pOutFile, "MULTI_NP_ENTITYARRAY, ");
                        validExprFuncArg = true;
                        pRetTypeString = "MULTI_NP_ENTITYARRAY";
                        break;
                    
                    case ARGTYPE_EXPR_ENTARRAY_OUT:
                        pRetTypeString = "MULTI_NP_ENTITYARRAY";
                        break;
                    case ARGTYPE_EXPR_INT_OUT:
                        pRetTypeString = "MULTI_INT";
                        break;
                    case ARGTYPE_EXPR_FLOAT_OUT:
                        pRetTypeString = "MULTI_FLOAT";
                        break;
                    case ARGTYPE_EXPR_STRING_OUT:
                        pRetTypeString = "MULTI_STRING";
                        break;
                    case ARGTYPE_EXPR_LOC_MAT4_OUT:
                        pRetTypeString = "MULTIOP_LOC_MAT4";
                        break;
                    }

                    if(validExprFuncArg)
                    {
                        if(pCommand->expressionStaticCheckParamTypes[iArgNum][0])
                        {
                            strcat(argTypeStaticCheckTypes, "\"");
                            strcat(argTypeStaticCheckTypes, pCommand->expressionStaticCheckParamTypes[iArgNum]);
                            strcat(argTypeStaticCheckTypes, "\", ");
                        }
                        else
                            strcat(argTypeStaticCheckTypes, "NULL, ");
                    }
                }

                fprintf(pOutFile, " 0 }, %s, { ", pRetTypeString);

                int iTagNum;

                for (iTagNum=0; iTagNum < pCommand->iNumExpressionTags; iTagNum++)
                {
                    fprintf(pOutFile, "\"%s\", ", pCommand->expressionTag[iTagNum]);
                }

                fprintf(pOutFile, "NULL }, ");

                if(pCommand->expressionStaticCheckFunc[0])
                    fprintf(pOutFile, "%s_EXPRFUNCWRAPPER, ", pCommand->expressionStaticCheckFunc);
                else
                    fprintf(pOutFile, "NULL, ");

                if(argTypeStaticCheckTypes[0])
                    fprintf(pOutFile, "{ %s }, ", argTypeStaticCheckTypes);
                else
                    fprintf(pOutFile, "{ NULL }, ");

                fprintf(pOutFile, " %d },\n", pCommand->iExpressionCost);

                /*
                for (iArgNum=0; iArgNum < pCommand->iNumArgs - 1; iArgNum++)
                {
                    if(pCommand->expressionStaticCheckParamTypes[iArgNum][0])
                        fprintf(pOutFile, "\"%s\", ", pCommand->expressionStaticCheckParamTypes[iArgNum]);
                    else
                        fprintf(pOutFile, "NULL, ");
                }

                if(pCommand->expressionStaticCheckParamTypes[iArgNum][0])
                    fprintf(pOutFile, "\"%s\"} },\n", pCommand->expressionStaticCheckParamTypes[iArgNum]);
                else
                    fprintf(pOutFile, "NULL } },\n");
                */
            }
        }

        fprintf(pOutFile, "};\n\n");

        char autoRunName[256];

        sprintf(autoRunName, "AutoCmds_%s_RegisterExprLists", m_ProjectName);
        fprintf(pOutFile, "void %s(void)\n{\n", autoRunName);
        
        fprintf(pOutFile, "\texprRegisterFunctionTable(AutoCmd_%s_ExprList, sizeof(AutoCmd_%s_ExprList)/sizeof(AutoCmd_%s_ExprList[0]));\n",
            m_ProjectName, m_ProjectName, m_ProjectName);

        fprintf(pOutFile, "}\n\n");

        m_pParent->GetAutoRunManager()->AddAutoRun(autoRunName, "autogen_magiccommands");
    }
}


/*
    #define ARGMAT4 {TYPE_MAT4}
#define ARGVEC3 {TYPE_VEC3}
#define ARGF32 {TYPE_F32}
#define ARGU32 {TYPE_INT}
#define ARGS32 {TYPE_INT}
#define ARGSTR(size) {TYPE_STR, 0, (size)}
#define ARGSENTENCE(size) {TYPE_SENTENCE, 0, (size)}
#define ARGSTRUCT(str,def) {TYPE_TEXTPARSER, def, sizeof(str)}




Cmd game_private_cmds[] = 
{
    { 9, "privategamecmd",        0, { ARGSTR(100)},0,
        "Test the new command parse system",testCommand},
    { 0, "bug_internal", 0, {ARGS32, ARGSENTENCE(10000)}, 0,
        "Internal command used to process /bug commands",bugInternal },
    { 9, "AStarRecording",        0, {{0}},0,
        "Starts AStarRecording"},
    { 9, "ShowBeaconDebugInfo",        0, {{0}},0,
        "Gets beacon debug information"},
    {0}
};

static void bugInternal(CMDARGS)
{
    READARGS2(int,mode,char*,desc );
    BugReportInternal(desc, mode);
    if(mode)        // the /csrbug command will set tmp_int == 0, so no message is displayed
        conPrintf(textStd("BugLogged"));
}*/


void MagicCommandManager::FoundMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer, int iWhichMagicWord, char const* pMagicWordString)
{
    
    if (iWhichMagicWord ==  MAGICWORD_BEGINNING_OF_FILE || iWhichMagicWord == MAGICWORD_END_OF_FILE)
    {
        return;
    }

    switch (iWhichMagicWord)
    {
    case MAGICWORD_AUTO_COMMAND:
    case MAGICWORD_AUTO_COMMAND_REMOTE:
    case MAGICWORD_AUTO_COMMAND_REMOTE_SLOW:
    case MAGICWORD_AUTO_COMMAND_QUEUED:
        FoundCommandMagicWord(pSourceFileName, pTokenizer, 
            iWhichMagicWord == MAGICWORD_AUTO_COMMAND_REMOTE || iWhichMagicWord == MAGICWORD_AUTO_COMMAND_REMOTE_SLOW, 
            iWhichMagicWord == MAGICWORD_AUTO_COMMAND_REMOTE_SLOW,
            iWhichMagicWord == MAGICWORD_AUTO_COMMAND_QUEUED);
        break;

    case MAGICWORD_AUTO_INT:
        FoundCommandVarMagicWord(pSourceFileName, pTokenizer, ARGTYPE_SINT);
        break;
    case MAGICWORD_AUTO_FLOAT:
        FoundCommandVarMagicWord(pSourceFileName, pTokenizer, ARGTYPE_FLOAT);
        break;
    case MAGICWORD_AUTO_STRING:
        FoundCommandVarMagicWord(pSourceFileName, pTokenizer, ARGTYPE_STRING);
        break;
    case MAGICWORD_AUTO_SENTENCE:
        FoundCommandVarMagicWord(pSourceFileName, pTokenizer, ARGTYPE_SENTENCE);
        break;
    }
}


void MagicCommandManager::FoundCommandVarMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer, enumMagicCommandArgType eCommandVarType)
{
    Tokenizer tokenizer;

    Token token;
    enumTokenType eType;

    pTokenizer->Assert(m_iNumMagicCommandVars < MAX_MAGICCOMMANDVARS, "Too many magic command vars");

    MAGIC_COMMANDVAR_STRUCT *pVar = &m_MagicCommandVars[m_iNumMagicCommandVars++];

    //initialize the new command
    memset(pVar, 0, sizeof(MAGIC_COMMANDVAR_STRUCT));
    pVar->iAccessLevel = 9;
    pVar->eVarType = eCommandVarType;
    strcpy(pVar->sourceFileName, pSourceFileName);
    


    m_bSomethingChanged = true;

    pTokenizer->GetSurroundingSlashedCommentBlock(&token);
    if (token.sVal[0])
    {
        strcpy(pVar->comment, token.sVal);
    }
    else
    {
        sprintf(pVar->comment, "No comment provided");
    }


    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AUTO_CMD_XXX");
    
    //skip tokens until we find a comma, as the variable name may be multiple tokens, having potential . and -> in it
    do
    {
        eType = pTokenizer->MustGetNextToken(&token, "EOF in the middle of AUTO_CMD_XXX");
    } while (!(eType == TOKEN_RESERVEDWORD && token.iVal == RW_COMMA));

    pTokenizer->Assert2NextTokenTypesAndGet(&token, 
        TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH - 1, 
        TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH - 1, 
        "Expected identifier or string after AUTO_CMD_XXX(varname,");

    strcpy(pVar->varCommandName, token.sVal);

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AUTO_CMD_XXX(varname, commandname");

    //check for ACMD_ commands
    while (1)
    {
        eType = pTokenizer->CheckNextToken(&token);

        if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_ACCESSLEVEL") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_ACCESSLEVEL");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Expected int after ACMD_ACCESSLEVEL(");

            pVar->iAccessLevel = token.iVal;

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_ACCESSLEVEL(x");
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_APPSPECIFICACCESSLEVEL") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_APPSPECIFICACCESSLEVEL");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 64, "Expected GLOBALTYPE_XXX after ACMD_APPSPECIFICACCESSLEVEL(");
            strcpy(pVar->serverSpecificAccessLevel_ServerName, token.sVal);
            
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after ACMD_APPSPECIFICACCESSLEVEL(xxx");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Expected int after ACMD_APPSPECIFICACCESSLEVEL(xxx,");

            pVar->iServerSpecificAccessLevel = token.iVal;

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_APPSPECIFICACCESSLEVEL(xxx, x");
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_CATEGORY") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_CATEGORY");
            do
            {
                eType = pTokenizer->CheckNextToken(&token);

                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH, "Expected identifier after ACMD_CATEGORY(");
            
                AddCommandVarToCategory(pVar, token.sVal, pTokenizer);

                pTokenizer->Assert2NextTokenTypesAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, TOKEN_RESERVEDWORD, RW_COMMA, "Expected ) or , after ACMD_CATEGORY(x");
            } while (token.iVal == RW_COMMA);        
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_LIST") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_LIST");
            
            do
            {
                eType = pTokenizer->CheckNextToken(&token);

                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH, "Expected identifier after ACMD_LIST(");
            
                AddCommandVarToSet(pVar, token.sVal, pTokenizer);

                pTokenizer->Assert2NextTokenTypesAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, TOKEN_RESERVEDWORD, RW_COMMA, "Expected ) or , after ACMD_LIST(x");
            } while (token.iVal == RW_COMMA);
        }        
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_PRIVATE") == 0)
        {
            pTokenizer->GetNextToken(&token);
        
            pVar->iCommandFlags |= COMMAND_FLAG_PRIVATE;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_EARLYCOMMANDLINE") == 0)
        {
            pTokenizer->GetNextToken(&token);
        
            pVar->iCommandFlags |= COMMAND_FLAG_EARLYCOMMANDLINE | COMMAND_FLAG_COMMANDLINE;
        }
        else if (eType == TOKEN_IDENTIFIER && (strcmp(token.sVal, "ACMD_COMMANDLINE") == 0 || strcmp(token.sVal, "ACMD_CMDLINE") == 0))
        {
            pTokenizer->GetNextToken(&token);
        
            pVar->iCommandFlags |= COMMAND_FLAG_COMMANDLINE;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_GLOBAL") == 0)
        {
            pTokenizer->GetNextToken(&token);
        
            pVar->iCommandFlags |= COMMAND_FLAG_GLOBAL;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_SERVERONLY") == 0)
        {
            pTokenizer->GetNextToken(&token);
        
            pVar->iCommandFlags |= COMMAND_FLAG_SERVER_ONLY;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_CLIENTONLY") == 0)
        {
            pTokenizer->GetNextToken(&token);
        
            pVar->iCommandFlags |= COMMAND_FLAG_CLIENT_ONLY;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_HIDE") == 0)
        {
            pTokenizer->GetNextToken(&token);
            
            pVar->iCommandFlags |= COMMAND_FLAG_HIDE;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_CALLBACK") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_CALLBACK");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH, "Expected identifier after ACMD_CALLBACK(");
    
            strcpy(pVar->callbackFunc, token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_CALLBACK(x");

        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_MAXVALUE") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_MAXVALUE");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Expected int after ACMD_MAXVALUE(");

            pVar->iMaxValue = token.iVal;

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_MAXVALUE(x");
        }
        else
        {
            break;
        }
    }

    if (pVar->iCommandFlags & COMMAND_FLAG_EARLYCOMMANDLINE)
    {
        pTokenizer->Assert(pVar->iAccessLevel == 0 && pVar->serverSpecificAccessLevel_ServerName[0] == 0, "EARLYCOMMANDLINE commands must have access level 0");
    }

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_SEMICOLON, "Expected ; after AUTO_CMD_XXX(varname, commandname)");
}

bool MagicCommandManager::CommandHasArgOfType(MAGIC_COMMAND_STRUCT *pCommand, enumMagicCommandArgType eType)
{
    int i;

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        if (pCommand->argTypes[i] == eType)
        {
            return true;
        }
    }

    return false;
}

int MagicCommandManager::CountArgsOfType(MAGIC_COMMAND_STRUCT *pCommand, enumMagicCommandArgType eType)
{
    int i;
    int iCount = 0;

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        if (pCommand->argTypes[i] == eType)
        {
            iCount++;
        }
    }

    return iCount;
}
int MagicCommandManager::GetNumNormalArgs(MAGIC_COMMAND_STRUCT *pCommand)
{
    int iRetVal = 0;
    int i;

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        if (pCommand->argTypes[i] < ARGTYPE_FIRST_SPECIAL)
        {
            iRetVal++;
        }
    }

    return iRetVal;
}





void MagicCommandManager::FoundCommandMagicWord(char const* pSourceFileName, Tokenizer *pTokenizer, 
    bool bIsRemoteCommand, bool bIsSlowCommand, bool bIsQueuedCommand)
{
    Tokenizer tokenizer;

    Token token;
    enumTokenType eType;

    //for slow commands, we read the "return value" early, and save it here
    char slowCommandReturnTypeName[MAX_MAGICCOMMAND_ARGNAME_LENGTH];
    int slowCommandReturnTypeNumAsterisks = 0;

    pTokenizer->Assert(m_iNumMagicCommands < MAX_MAGICCOMMANDS, "Too many magic commands");

    MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[m_iNumMagicCommands++];

    //initialize the new command
    memset(pCommand, 0, sizeof(MAGIC_COMMAND_STRUCT));
    pCommand->iAccessLevel = 9;
    
    if (bIsRemoteCommand)
    {
        pCommand->iCommandFlags |= COMMAND_FLAG_REMOTE;
    }    
    if (bIsSlowCommand)
    {
        pCommand->iCommandFlags |= COMMAND_FLAG_REMOTE | COMMAND_FLAG_SLOW_REMOTE;
    }    
    if (bIsQueuedCommand)
    {
        pCommand->iCommandFlags |= COMMAND_FLAG_QUEUED;
    }

    m_bSomethingChanged = true;

    strcpy(pCommand->sourceFileName, pSourceFileName);
    pCommand->iLineNum = pTokenizer->GetCurLineNum();

    pTokenizer->GetSurroundingSlashedCommentBlock(&token);
    if (token.sVal[0])
    {
        strcpy(pCommand->comment, token.sVal);
    }
    else
    {
        sprintf(pCommand->comment, "No comment provided");
    }

    //if this is a slow command, get the slow command arg name
    if (bIsSlowCommand)
    {
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AUTO_COMMAND_REMOTE_SLOW )");
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMAND_ARGNAME_LENGTH, "Expected typename after AUTO_COMMAND_REMOTE_SLOW(");

        strcpy(slowCommandReturnTypeName, token.sVal);

        while ( (eType = pTokenizer->CheckNextToken(&token)) == TOKEN_RESERVEDWORD && token.iVal == RW_ASTERISK)
        {
            slowCommandReturnTypeNumAsterisks++;
            pTokenizer->GetNextToken(&token);
        }

        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AUTO_COMMAND_REMOTE(x");

    }

    //if this is a queued command, get the queue name
    if (bIsQueuedCommand)
    {
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after AUTO_COMMAND_QUEUED )");
        
        eType = pTokenizer->CheckNextToken(&token);

        if (eType == TOKEN_IDENTIFIER)
        {
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH, "Expected queue name after AUTO_COMMAND_QUEUED(");

            strcpy(pCommand->queueName, token.sVal);
        }
        else
        {
            pCommand->queueName[0] = 0;
        }

        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after AUTO_COMMAND_QUEUED(x");

    }

    


    

    //check for ACMD_ commands
    while (1)
    {
        eType = pTokenizer->CheckNextToken(&token);

        if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_I_AM_THE_ERROR_FUNCTION_FOR") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_I_AM_THE_ERROR_FUNCTION_FOR");
            pTokenizer->Assert2NextTokenTypesAndGet(&token, 
                TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH - 1, 
                TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH - 1, 
                "Expected identifier or string after ACMD_I_AM_THE_ERROR_FUNCTION_FOR(");

            strcpy(pCommand->commandWhichThisIsTheErrorFunctionFor, token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_I_AM_THE_ERROR_FUNCTION_FOR(x");
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_NAME") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_NAME");
            pTokenizer->Assert2NextTokenTypesAndGet(&token, 
                TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH - 1, 
                TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH - 1, 
                "Expected identifier or string after ACMD_NAME(");

            strcpy(pCommand->commandName, token.sVal);

            strcpy(pCommand->safeCommandName,token.sVal);
            MakeStringAllAlphaNum(pCommand->safeCommandName);


            int iNumAliases = 0;


            while (    (eType = pTokenizer->CheckNextToken(&token)) == TOKEN_RESERVEDWORD && token.iVal == RW_COMMA)
            {
                pTokenizer->GetNextToken(&token);
                pTokenizer->Assert2NextTokenTypesAndGet(&token, 
                    TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH - 1, 
                    TOKEN_STRING, MAX_MAGICCOMMANDNAMELENGTH - 1, 
                    "Expected identifier or string after ACMD_NAME(x,");

                pTokenizer->Assert(iNumAliases < MAX_COMMAND_ALIASES, "Too many aliases");

                strcpy(pCommand->commandAliases[iNumAliases++], token.sVal);
            }


            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_NAME(x");
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_ACCESSLEVEL") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_ACCESSLEVEL");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Expected int after ACMD_ACCESSLEVEL(");

            pCommand->iAccessLevel = token.iVal;

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_ACCESSLEVEL(x");
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_APPSPECIFICACCESSLEVEL") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_APPSPECIFICACCESSLEVEL");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, 64, "Expected GLOBALTYPE_XXX after ACMD_APPSPECIFICACCESSLEVEL(");
            strcpy(pCommand->serverSpecificAccessLevel_ServerName, token.sVal);
            
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, "Expected , after ACMD_APPSPECIFICACCESSLEVEL(xxx");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Expected int after ACMD_APPSPECIFICACCESSLEVEL(xxx,");

            pCommand->iServerSpecificAccessLevel = token.iVal;

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_APPSPECIFICACCESSLEVEL(xxx, x");
        }        
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_CATEGORY") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_CATEGORY");
        
            do
            {
                eType = pTokenizer->CheckNextToken(&token);

                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH, "Expected identifier after ACMD_CATEGORY(");
            
                AddCommandToCategory(pCommand, token.sVal, pTokenizer);

                pTokenizer->Assert2NextTokenTypesAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, TOKEN_RESERVEDWORD, RW_COMMA, "Expected ) or , after ACMD_CATEGORY(x");
            } while (token.iVal == RW_COMMA);        
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_LIST") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_LIST");
            
            do
            {
                eType = pTokenizer->CheckNextToken(&token);

                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH, "Expected identifier after ACMD_LIST(");
            
                AddCommandToSet(pCommand, token.sVal, pTokenizer);

                pTokenizer->Assert2NextTokenTypesAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, TOKEN_RESERVEDWORD, RW_COMMA, "Expected ) or , after ACMD_LIST(x");
            } while (token.iVal == RW_COMMA);
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_PRIVATE") == 0)
        {
            pTokenizer->GetNextToken(&token);
        
            pCommand->iCommandFlags |= COMMAND_FLAG_PRIVATE;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_EARLYCOMMANDLINE") == 0)
        {
            pTokenizer->GetNextToken(&token);
        
            pCommand->iCommandFlags |= COMMAND_FLAG_EARLYCOMMANDLINE | COMMAND_FLAG_COMMANDLINE;
        }
        else if (eType == TOKEN_IDENTIFIER && (strcmp(token.sVal, "ACMD_COMMANDLINE") == 0 || strcmp(token.sVal, "ACMD_CMDLINE") == 0))
        {
            pTokenizer->GetNextToken(&token);
        
            pCommand->iCommandFlags |= COMMAND_FLAG_COMMANDLINE;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_GLOBAL") == 0)
        {
            pTokenizer->GetNextToken(&token);
        
            pCommand->iCommandFlags |= COMMAND_FLAG_GLOBAL;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_CLIENTONLY") == 0)
        {
            pTokenizer->GetNextToken(&token);
        
            pCommand->iCommandFlags |= COMMAND_FLAG_CLIENT_ONLY;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_SERVERONLY") == 0)
        {
            pTokenizer->GetNextToken(&token);
        
            pCommand->iCommandFlags |= COMMAND_FLAG_SERVER_ONLY;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_TESTCLIENT") == 0)
        {
            pTokenizer->GetNextToken(&token);
        
            pCommand->iCommandFlags |= COMMAND_FLAG_TESTCLIENT;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_NOTESTCLIENT") == 0)
        {
            pTokenizer->GetNextToken(&token);
        
            pCommand->iCommandFlags |= COMMAND_FLAG_NOTESTCLIENT;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_HIDE") == 0)
        {
            pTokenizer->GetNextToken(&token);
            
            pCommand->iCommandFlags |= COMMAND_FLAG_HIDE;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_CLIENTCMD") == 0)
        {
            pTokenizer->GetNextToken(&token);
            
            pCommand->iCommandFlags |= COMMAND_FLAG_CLIENT_WRAPPER;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_SERVERCMD") == 0)
        {
            pTokenizer->GetNextToken(&token);
            
            pCommand->iCommandFlags |= COMMAND_FLAG_SERVER_WRAPPER;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_IFDEF") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->Assert(pCommand->iNumDefines < MAX_MAGICCOMMAND_DEFINES, "Too many ACMD_IFDEFs");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_IFDEF");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH, "Expected identifier after ACMD_IFDEF(");
            strcpy(pCommand->defines[pCommand->iNumDefines++], token.sVal);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_IFDEF(x");
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_EXPR_TAG") == 0)
        {
            pTokenizer->GetNextToken(&token);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_EXPR_TAG");

            pCommand->iCommandFlags |= COMMAND_FLAG_EXPR_WRAPPER; // generate is implicit in ACMD_EXPR_LIST

            do 
            {
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH, "Expected identifier after ACMD_EXPR_TAG(");
                strcpy(pCommand->expressionTag[pCommand->iNumExpressionTags++], token.sVal);

                pTokenizer->Assert(pCommand->iNumExpressionTags <= MAX_COMMAND_SETS, "Too many ACMD_EXPR_TAGs specified");

                pTokenizer->Assert2NextTokenTypesAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, TOKEN_RESERVEDWORD, RW_COMMA, "Expected ) or , after ACMD_EXPR_TAG(x");
            } while(token.iVal == RW_COMMA);
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_EXPR_STATIC_CHECK") == 0)
        {
            pTokenizer->GetNextToken(&token);

            pTokenizer->Assert(pCommand->expressionStaticCheckFunc[0] == 0, "Can only have one ACMD_EXPR_STATIC_CHECK");

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_EXPR_LIST");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH, "Expected identifier after ACMD_EXPR_STATIC_CHECK(");

            strcpy(pCommand->expressionStaticCheckFunc, token.sVal);

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_EXPR_LIST(x");
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_EXPR_FUNC_COST") == 0)
        {
            pTokenizer->GetNextToken(&token);

            pTokenizer->Assert(pCommand->iExpressionCost == 0, "Can only specify cost for something once");

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_INT, 0, "Expected integer cost. Alex doesn't like floats");
            pCommand->iExpressionCost = token.iVal;

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_EXPR_LIST(x");
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_EXPR_FUNC_COST_MOVEMENT") == 0)
        {
            pTokenizer->GetNextToken(&token);

            pCommand->iExpressionCost = 3;
        }
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_EXPR_GENERATE_WRAPPER_ONLY") == 0)
        {
            pTokenizer->GetNextToken(&token);
            
            pCommand->iCommandFlags |= COMMAND_FLAG_EXPR_WRAPPER;
        }
        /*
        else if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_EXPR_TAG") == 0)
        {
            pTokenizer->GetNextToken(&token);
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_EXPR_TAG");
            
            pCommand->iCommandFlags |= COMMAND_FLAG_ADD_TO_GLOBAL_EXPR_TABLE;
            pCommand->iCommandFlags |= COMMAND_FLAG_EXPR_WRAPPER;

            do
            {
                eType = pTokenizer->CheckNextToken(&token);

                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH, "Expected identifier after ACMD_EXPR_TAG(");

                int i;

                for(i = 0; i < ARRAYSIZE(pCommand->expressionTags); i++)
                {
                    if(!pCommand->expressionTags[i][0])
                        strcpy(pCommand->expressionTags[i], token.sVal);
                }

                pTokenizer->Assert(i < ARRAYSIZE(pCommand->expressionTags), "Too many sets for one command");
            
                pTokenizer->Assert2NextTokenTypesAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, TOKEN_RESERVEDWORD, RW_COMMA, "Expected ) or , after ACMD_EXPR_TAG(x");
            } while (token.iVal == RW_COMMA);
        }
        */
        else
        {
            break;
        }
    }

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_SEMICOLON, "Didn't find ; after AUTO_COMMAND");
    int iNumAsterisks = 0;
    
    //for slow commands, we lie and claim that the string we extracted from inside AUTO_COMMAND_REMOTE_SLOW is the 
    //return type name string, and also verify that the actual return type is void
    if (bIsSlowCommand)
    {
        pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_VOID, "AUTO_COMMAND_REMOTE_SLOW can only be applied to void functions");

        strcpy(pCommand->returnTypeName, slowCommandReturnTypeName);
        iNumAsterisks = slowCommandReturnTypeNumAsterisks;
    }
    else
    {


        //skip over [const][struct]function type
        eType = pTokenizer->GetNextToken(&token);

        if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "const") == 0)
        {
            eType = pTokenizer->GetNextToken(&token);
        }
        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_STRUCT)
        {
            eType = pTokenizer->GetNextToken(&token);
        }

        pTokenizer->StringifyToken(&token);
        sprintf_s(pCommand->returnTypeName, sizeof(pCommand->returnTypeName), "%s", token.sVal);

        //skip over any number of *s after function type
        eType = pTokenizer->CheckNextToken(&token);
        while (eType == TOKEN_RESERVEDWORD && token.iVal == RW_ASTERISK)
        {
            eType = pTokenizer->GetNextToken(&token);
            eType = pTokenizer->CheckNextToken(&token);
            iNumAsterisks++;
        }
    }

    //check if the return value type is one we can deal with
    if (StringIsInList(pCommand->returnTypeName, sSIntNames) && iNumAsterisks == 0)
    {
        pCommand->eReturnType = ARGTYPE_SINT;
    }
    else if (StringIsInList(pCommand->returnTypeName, sUIntNames) && iNumAsterisks == 0)
    {
        pCommand->eReturnType = ARGTYPE_UINT;
    }
    else if (StringIsInList(pCommand->returnTypeName, sFloatNames) && iNumAsterisks == 0)
    {
        pCommand->eReturnType = ARGTYPE_FLOAT;
    }
    else if (StringIsInList(pCommand->returnTypeName, sSInt64Names) && iNumAsterisks == 0)
    {
        pCommand->eReturnType = ARGTYPE_SINT64;
    }
    else if (StringIsInList(pCommand->returnTypeName, sUInt64Names) && iNumAsterisks == 0)
    {
        pCommand->eReturnType = ARGTYPE_UINT64;
    }
    else if (StringIsInList(pCommand->returnTypeName, sFloat64Names) && iNumAsterisks == 0)
    {
        pCommand->eReturnType = ARGTYPE_FLOAT64;
    }
    else if (strcmp(pCommand->returnTypeName, "char") == 0 && iNumAsterisks == 1)
    {
        pCommand->eReturnType = ARGTYPE_STRING;
        sprintf(pCommand->returnTypeName, "char*");
    }
    else if (strcmp(pCommand->returnTypeName, "Vec3") == 0 && iNumAsterisks == 1)
    {
        pCommand->eReturnType = ARGTYPE_VEC3_POINTER;
        sprintf(pCommand->returnTypeName, "Vec3*");
    }
    else if (strcmp(pCommand->returnTypeName, "Vec4") == 0 && iNumAsterisks == 1)
    {
        pCommand->eReturnType = ARGTYPE_VEC4_POINTER;
        sprintf(pCommand->returnTypeName, "Vec4*");
    }
    else if (iNumAsterisks == 1)
    {
        pCommand->eReturnType = ARGTYPE_STRUCT;
    }
    else if (strcmp(pCommand->returnTypeName, "void") == 0)
    {
        pCommand->eReturnType = ARGTYPE_NONE;
    }
    else if (strcmp(pCommand->returnTypeName, "ExprFuncReturnVal") == 0)
    {
        pCommand->eReturnType = ARGTYPE_EXPR_FUNCRETURNVAL;
    }
    else
    {
        pCommand->eReturnType = ARGTYPE_ENUM;
    }

    

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH, "Didn't find magic command name after void");

    strcpy(pCommand->functionName, token.sVal);

    pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Didn't find ( after magic command name");

    pCommand->iNumArgs = 0;

    do
    {
        eType = pTokenizer->GetNextToken(&token);

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_VOID)
        {
            pTokenizer->Assert(pCommand->iNumArgs == 0, "void found in wrong place");
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after void");
            break;
        }


        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_COMMA)
        {
            eType = pTokenizer->GetNextToken(&token);
        }

        if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_RIGHTPARENS)
        {
            break;
        }

        pTokenizer->Assert(pCommand->iNumArgs < MAX_MAGICCOMMAND_ARGS, "Too many args to a magic command");

        if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_NAMELIST") == 0)
        {
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_NAMELIST");
            
            eType = pTokenizer->GetNextToken(&token);

            pTokenizer->Assert((eType == TOKEN_IDENTIFIER || eType == TOKEN_STRING) && token.iVal < MAX_MAGICCOMMANDNAMELENGTH,
                "Expected identifier or string after ACMD_NAMELIST(");

            strcpy(pCommand->argNameListDataPointerNames[pCommand->iNumArgs], token.sVal);

            pCommand->argNameListDataPointerWasString[pCommand->iNumArgs] = (eType == TOKEN_STRING);


            pTokenizer->Assert2NextTokenTypesAndGet(&token, TOKEN_RESERVEDWORD, RW_COMMA, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected , or ) after ACMD_NAMELIST(x");

            if (token.iVal == RW_RIGHTPARENS)
            {
                strcpy(pCommand->argNameListTypeNames[pCommand->iNumArgs], "NAMELISTTYPE_PREEXISTING");
            }
            else
            {
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMANDNAMELENGTH - 20, "Expected identifier after ACMD_NAMELIST(x,");
                sprintf(pCommand->argNameListTypeNames[pCommand->iNumArgs], "NAMELISTTYPE_%s", token.sVal);
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_NAMELIST(x, y");
            }

            eType = pTokenizer->GetNextToken(&token);
        }

        if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_EXPR_SC_TYPE") == 0)
        {
            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_EXPR_SC_TYPE");

            eType = pTokenizer->GetNextToken(&token);

            pTokenizer->Assert((eType == TOKEN_IDENTIFIER || eType == TOKEN_STRING) && token.iVal < MAX_MAGICCOMMANDNAMELENGTH,
                "Expected identifier or string after ACMD_EXPR_SC_TYPE(");

            strcpy(pCommand->expressionStaticCheckParamTypes[pCommand->iNumArgs], token.sVal);


            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_EXPR_SC_TYPE(x");

            eType = pTokenizer->GetNextToken(&token);
        }

        if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_IGNORE") == 0)
        {
            pCommand->argTypes[pCommand->iNumArgs] = ARGTYPE_IGNORE;
            sprintf(pCommand->argNames[pCommand->iNumArgs], "ignore%d", pCommand->iNumArgs);
            sprintf(pCommand->argTypeNames[pCommand->iNumArgs], "void*");
            do
            {
                eType = pTokenizer->CheckNextToken(&token);

                if (eType == TOKEN_NONE || eType == TOKEN_RESERVEDWORD && (token.iVal == RW_COMMA || token.iVal == RW_RIGHTPARENS))
                {
                    break;
                }

                pTokenizer->GetNextToken(&token);
            }
            while (1);
        }
        else

        if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_POINTER") == 0)
        {
            bool bFoundConst = false;

            pCommand->argTypes[pCommand->iNumArgs] = ARGTYPE_VOIDSTAR;

            eType = pTokenizer->CheckNextToken(&token);

            if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "const") == 0)
            {
                bFoundConst = true;
                pTokenizer->GetNextToken(&token);
            }

    

            pTokenizer->Assert2NextTokenTypesAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMAND_ARGTYPE_NAME_LENGTH - 5,
                TOKEN_RESERVEDWORD, RW_VOID,
                "Expected argument type or void after ACMD_POINTER");

            pTokenizer->StringifyToken(&token);

            sprintf(pCommand->argTypeNames[pCommand->iNumArgs], "%s%s", bFoundConst ? "const " : "", token.sVal);

            eType = pTokenizer->GetNextToken(&token);

            while (eType == TOKEN_RESERVEDWORD && token.iVal == RW_ASTERISK)
            {
                strcat(pCommand->argTypeNames[pCommand->iNumArgs], "*");
                eType = pTokenizer->GetNextToken(&token);
            }

            pTokenizer->Assert(eType == TOKEN_IDENTIFIER && token.iVal < MAX_MAGICCOMMAND_ARGNAME_LENGTH, "Expected argument name after ACMD_POINTER argType");

            strcpy(pCommand->argNames[pCommand->iNumArgs], token.sVal);
        }
        else
        {
            char forceTypeName[256] = "";

            if (eType == TOKEN_IDENTIFIER && strcmp(token.sVal, "ACMD_FORCETYPE") == 0)
            {
                Token tempToken;
                pTokenizer->AssertNextTokenTypeAndGet(&tempToken, TOKEN_RESERVEDWORD, RW_LEFTPARENS, "Expected ( after ACMD_FORCETYPE");
                pTokenizer->AssertNextTokenTypeAndGet(&tempToken, TOKEN_IDENTIFIER, sizeof(forceTypeName), "Expected identifier after ACMD_FORCETYPE(");
                strcpy(forceTypeName, tempToken.sVal);
                pTokenizer->AssertNextTokenTypeAndGet(&tempToken, TOKEN_RESERVEDWORD, RW_RIGHTPARENS, "Expected ) after ACMD_FORCETYPE(x");
                eType = pTokenizer->GetNextToken(&token);
            }

            pTokenizer->Assert(eType == TOKEN_IDENTIFIER, "Expected magic command arg type");

            if (strcmp(token.sVal, "const") == 0)
            {
                Token tempToken;
                pTokenizer->AssertNextTokenTypeAndGet(&tempToken, TOKEN_IDENTIFIER, 0, "Expected magic command arg type after const");

                sprintf(token.sVal, "const %s", tempToken.sVal);
            }
                

            pCommand->argTypes[pCommand->iNumArgs] = GetArgTypeFromArgTypeName(forceTypeName[0] ? forceTypeName : token.sVal);
            sprintf_s(pCommand->argTypeNames[pCommand->iNumArgs], sizeof(pCommand->argTypeNames[pCommand->iNumArgs]),
                "%s", forceTypeName[0] ? forceTypeName : token.sVal);

            pTokenizer->Assert(pCommand->argTypes[pCommand->iNumArgs] != ARGTYPE_NONE, "Unknown arg type for magic command");

            if (pCommand->argTypes[pCommand->iNumArgs] == ARGTYPE_SLOWCOMMANDID)
            {
                sprintf(pCommand->argTypeNames[pCommand->iNumArgs], "%s", token.sVal);
            }
            else if (pCommand->argTypes[pCommand->iNumArgs] >= ARGTYPE_FIRST_SPECIAL)
            {
                sprintf(pCommand->argTypeNames[pCommand->iNumArgs], "%s*", token.sVal);
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_ASTERISK, "expected * after Entity/Cmd/CmdContext");
                pTokenizer->Assert(!CommandHasArgOfType(pCommand, pCommand->argTypes[pCommand->iNumArgs]), "Only one each Entity/Cmd/Context arg allowed per AUTO_COMMAND");
            }
            else if (pCommand->argTypes[pCommand->iNumArgs] == ARGTYPE_STRING)
            {
                Token nextToken;

                eType = pTokenizer->CheckNextToken(&nextToken);

                if (eType == TOKEN_RESERVEDWORD && nextToken.iVal == RW_ASTERISK)
                {
                    sprintf(pCommand->argTypeNames[pCommand->iNumArgs], "%s*", token.sVal);
                    pTokenizer->GetNextToken(&token);
                }
                else
                {
                    sprintf(pCommand->argTypeNames[pCommand->iNumArgs], token.sVal);
                    pCommand->argTypes[pCommand->iNumArgs] = ARGTYPE_SINT;
                }
            }
            else if (pCommand->argTypes[pCommand->iNumArgs] == ARGTYPE_VEC3_DIRECT)
            {
                Token nextToken;
                enumTokenType eType;
                eType = pTokenizer->CheckNextToken(&nextToken);

                if (eType == TOKEN_RESERVEDWORD && nextToken.iVal == RW_ASTERISK)
                {
                    pCommand->argTypes[pCommand->iNumArgs] = ARGTYPE_VEC3_POINTER;
                    pTokenizer->GetNextToken(&nextToken);
                }
            
                sprintf(pCommand->argTypeNames[pCommand->iNumArgs], "%s", token.sVal);
            }
            else if (pCommand->argTypes[pCommand->iNumArgs] == ARGTYPE_VEC4_DIRECT)
            {
                Token nextToken;
                enumTokenType eType;
                eType = pTokenizer->CheckNextToken(&nextToken);

                if (eType == TOKEN_RESERVEDWORD && nextToken.iVal == RW_ASTERISK)
                {
                    pCommand->argTypes[pCommand->iNumArgs] = ARGTYPE_VEC4_POINTER;
                    pTokenizer->GetNextToken(&nextToken);
                }
            
                sprintf(pCommand->argTypeNames[pCommand->iNumArgs], "%s", token.sVal);
            }
            else if (pCommand->argTypes[pCommand->iNumArgs] == ARGTYPE_MAT4_DIRECT)
            {
                Token nextToken;
                enumTokenType eType;
                eType = pTokenizer->CheckNextToken(&nextToken);

                if (eType == TOKEN_RESERVEDWORD && nextToken.iVal == RW_ASTERISK)
                {
                    pCommand->argTypes[pCommand->iNumArgs] = ARGTYPE_MAT4_POINTER;
                    pTokenizer->GetNextToken(&nextToken);
                }
            
                sprintf(pCommand->argTypeNames[pCommand->iNumArgs], "%s", token.sVal);
            }
            else if (pCommand->argTypes[pCommand->iNumArgs] == ARGTYPE_QUAT_DIRECT)
            {
                Token nextToken;
                enumTokenType eType;
                eType = pTokenizer->CheckNextToken(&nextToken);

                if (eType == TOKEN_RESERVEDWORD && nextToken.iVal == RW_ASTERISK)
                {
                    pCommand->argTypes[pCommand->iNumArgs] = ARGTYPE_QUAT_POINTER;
                    pTokenizer->GetNextToken(&nextToken);
                }
            
                sprintf(pCommand->argTypeNames[pCommand->iNumArgs], "%s", token.sVal);
            }
            else if (pCommand->argTypes[pCommand->iNumArgs] == ARGTYPE_STRUCT)
            {
                strcpy(pCommand->argTypeNames[pCommand->iNumArgs], token.sVal);

                eType = pTokenizer->CheckNextToken(&token);

                if (eType == TOKEN_RESERVEDWORD && token.iVal == RW_ASTERISK)
                {
                    pTokenizer->GetNextToken(&token);
                }
                else
                {
                    pCommand->argTypes[pCommand->iNumArgs] = ARGTYPE_ENUM;
                }
            }
            else if (pCommand->argTypes[pCommand->iNumArgs] == ARGTYPE_EXPR_EXPRCONTEXT)
            {
                strcpy(pCommand->argTypeNames[pCommand->iNumArgs], "ExprContext*");
                pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_RESERVEDWORD, RW_ASTERISK, "Expected * after ExprContext");
            }

            pTokenizer->AssertNextTokenTypeAndGet(&token, TOKEN_IDENTIFIER, MAX_MAGICCOMMAND_ARGNAME_LENGTH, "Expected arg name after arg type");
            strcpy(pCommand->argNames[pCommand->iNumArgs], token.sVal);
        }

        pCommand->iNumArgs++;

    }
    while (1);

    if (pCommand->commandName[0] == 0)
    {
        strcpy(pCommand->commandName, pCommand->functionName);
        strcpy(pCommand->safeCommandName,pCommand->functionName);
    }

    if (pCommand->commandCategories[0][0] == 0 && pCommand->iNumExpressionTags)
    {
        int i;
        for (i=0; i < pCommand->iNumExpressionTags; i++)
            AddCommandToCategory(pCommand, pCommand->expressionTag[i], pTokenizer);
    }

    FixupCommandTypes(pTokenizer, pCommand);
    VerifyCommandValidity(pTokenizer, pCommand);

}

void MagicCommandManager::FixupCommandTypes(Tokenizer *pTokenizer, MAGIC_COMMAND_STRUCT *pCommand)
{
    if (pCommand->iCommandFlags & COMMAND_FLAG_REMOTE)
    {
        int i;

        for (i=0; i < pCommand->iNumArgs; i++)
        {
            if (pCommand->argTypes[i] == ARGTYPE_STRING || pCommand->argTypes[i] == ARGTYPE_SENTENCE)
            {
                pCommand->argTypes[i] = ARGTYPE_ESCAPEDSTRING;
            }
        }
    }
}


void MagicCommandManager::VerifyCommandValidity(Tokenizer *pTokenizer, MAGIC_COMMAND_STRUCT *pCommand)
{
    if (pCommand->commandWhichThisIsTheErrorFunctionFor[0])
    {
        pTokenizer->Assert(strcmp(pCommand->commandName, pCommand->functionName) == 0, "Error functions can't have command names");
        pTokenizer->Assert(pCommand->commandSets[0][0] == 0 && pCommand->commandCategories[0][0] == 0, "Error functions can't have categories or sets");
        pTokenizer->Assert(GetNumNormalArgs(pCommand) == 0, "Error functions can't have arguments");


        bool bFound = false;

        int iCommandNum;

        for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
        {
            MAGIC_COMMAND_STRUCT *pOtherCommand = &m_MagicCommands[iCommandNum];
            
            if (pOtherCommand != pCommand)
            {

                if (strcmp(pOtherCommand->commandName, pCommand->commandWhichThisIsTheErrorFunctionFor) == 0)
                {
                    bFound = true;
                    pTokenizer->Assert(pOtherCommand->eReturnType == pCommand->eReturnType, "An error function must have the same return type as its command");
                }

                if (strcmp(pCommand->commandWhichThisIsTheErrorFunctionFor, pOtherCommand->commandWhichThisIsTheErrorFunctionFor) == 0)
                {
                    pTokenizer->Assert(0, "Found two command trying to be error functions for the same command");
                }
            }
        }

        if (!bFound)
        {
            int iVarNum;
            for (iVarNum = 0; iVarNum < m_iNumMagicCommandVars; iVarNum++)
            {
                MAGIC_COMMANDVAR_STRUCT *pCommandVar = &m_MagicCommandVars[iVarNum];

                if (strcmp(pCommandVar->varCommandName, pCommand->commandWhichThisIsTheErrorFunctionFor) == 0)
                {
                    pTokenizer->Assert(pCommand->eReturnType == ARGTYPE_NONE, "An error func for a command variable must have no return type");

                    bFound = true;
                    break;
                }
            }
        }

        pTokenizer->Assert(bFound, "Command is trying to be the error function for an unknown command");
    }

    bool bFoundASentence = false;
    int i;

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        if (bFoundASentence)
        {
            if (pCommand->argTypes[i] < ARGTYPE_FIRST_SPECIAL)
            {
                pTokenizer->Assert(0, "An AUTO_COMMAND can only have one SENTENCE, and only as its last normal argument");
            }
        }
        else
        {
            if (pCommand->argTypes[i] == ARGTYPE_SENTENCE)
            {
                bFoundASentence = true;
            }
        }
    }

    if (pCommand->iCommandFlags & COMMAND_FLAG_SLOW_REMOTE)
    {
        pTokenizer->Assert(CountArgsOfType(pCommand, ARGTYPE_SLOWCOMMANDID) == 1, "Slow commands must have one SlowRemoteCommandID arg");
    }
    else
    {
        pTokenizer->Assert(CountArgsOfType(pCommand, ARGTYPE_SLOWCOMMANDID) == 0, "non-slow commands can not have a SlowRemoteCommandID arg");
    }

    if (pCommand->iCommandFlags & COMMAND_FLAG_REMOTE)
    {
        pTokenizer->Assert(pCommand->commandSets[0][0] == 0, "Remote commands can not have a command set");

//        strcpy(pCommand->commandSets[0], CountArgsOfType(pCommand, ARGTYPE_SLOWCOMMANDID) == 1 ? "SlowRemote" : "Remote");

        for (i = ARGTYPE_FIRST_SPECIAL; i < ARGTYPE_LAST; i++)
        {
            if (i != ARGTYPE_SLOWCOMMANDID)
            {
                pTokenizer->Assert(CountArgsOfType(pCommand, (enumMagicCommandArgType)i) == 0, "remote commands can not have \"special\" args");
            }
        }
    }
    else
    {
        pTokenizer->Assert(CountArgsOfType(pCommand, ARGTYPE_SLOWCOMMANDID) == 0, "non-Remote commands can not have a SlowRemoteCommandID arg");

    }

    if (pCommand->iCommandFlags & COMMAND_FLAG_QUEUED)
    {
        int i;

        for (i=0; i < pCommand->iNumArgs; i++)
        {
            pTokenizer->Assert(pCommand->argTypes[i] < ARGTYPE_FIRST_SPECIAL, "Queued commands can't have special args");
            pTokenizer->Assert(pCommand->argTypes[i] != ARGTYPE_STRUCT, "Queued commands can't have struct args");
            pTokenizer->Assert(pCommand->eReturnType == ARGTYPE_NONE, "Queued commands can't have return types (for now)");
            pTokenizer->Assert(pCommand->commandSets[0][0] == 0 && pCommand->commandCategories[0][0] == 0, "Queued commands can't have sets or categories");
            pTokenizer->Assert(strcmp(pCommand->commandName, pCommand->functionName) == 0, "Queued commands can't have names");
            pTokenizer->Assert(pCommand->commandWhichThisIsTheErrorFunctionFor[0] == 0, "Queued commands can't be error functions");
        }
    }
    else
    {
        pTokenizer->Assert(CountArgsOfType(pCommand, ARGTYPE_VOIDSTAR) == 0, "NonQueued commands can't have ACMD_POINTER");
//        pTokenizer->Assert(CountArgsOfType(pCommand, ARGTYPE_QUAT_POINTER) == 0, "NonQueued commands can't have quat args");
//        pTokenizer->Assert(CountArgsOfType(pCommand, ARGTYPE_QUAT_DIRECT) == 0, "NonQueued commands can't have quat args");
    }

    if (pCommand->iCommandFlags & COMMAND_FLAG_EXPR_WRAPPER)
    {
        int iNumOutArgs = 0;

        switch (pCommand->eReturnType)
        {
        case ARGTYPE_NONE:
        case ARGTYPE_EXPR_FUNCRETURNVAL:
            //fine, do nothing
            break;

        case ARGTYPE_SINT:
        case ARGTYPE_UINT:
        case ARGTYPE_FLOAT:
        case ARGTYPE_SINT64:
        case ARGTYPE_UINT64:
        case ARGTYPE_FLOAT64:
        case ARGTYPE_STRING:
            //count as a return value
            iNumOutArgs++;
            break;

        default:
            pTokenizer->Assert(0, "Invalid return val type from expression list command");
            break;
        }

        int i;

        for (i=0; i < pCommand->iNumArgs; i++)
        {
            if (pCommand->argTypes[i] >= ARGTYPE_EXPR_OUT_FIRST && pCommand->argTypes[i] <= ARGTYPE_EXPR_LAST)
            {
                iNumOutArgs++;
            }
        }

        pTokenizer->Assert(iNumOutArgs <= 1, "Too many _OUT args");

    }
    else
    {
        pTokenizer->Assert(!CommandHasExpressionOnlyArgumentsOrReturnVals(pCommand), "non-expression-list commands can not have expression-specific arg or return types");
    }

    if (pCommand->iCommandFlags & COMMAND_FLAG_EARLYCOMMANDLINE)
    {
        pTokenizer->Assert(pCommand->iAccessLevel == 0 && pCommand->serverSpecificAccessLevel_ServerName[0] == 0, "EARLYCOMMANDLINE functions must be access level 0");
    }
}











char const* MagicCommandManager::GetArgDescriptionBlock(enumMagicCommandArgType eArgType, 
        char *pArgName, char *pTypeName,
        char *pNameListDataPointer, char *pNameListType, bool bDataPointerWasString)
{
    char tempString1[1024];
    static char tempString2[1024];

    switch(eArgType)
    {
    case ARGTYPE_ENUM:
    case ARGTYPE_SINT:
        sprintf(tempString1, "{\"%s\", MULTI_INT,0, sizeof(S32), CMDAF_ALLOCATED, 0, ", pArgName);
        break;

    case ARGTYPE_UINT:
        sprintf(tempString1, "{\"%s\", MULTI_INT,0, sizeof(U32), CMDAF_ALLOCATED, 0, ", pArgName);
        break;

    case ARGTYPE_FLOAT:
        sprintf(tempString1, "{\"%s\", MULTI_FLOAT, 0, sizeof(F32), CMDAF_ALLOCATED, 0, ", pArgName);
        break;

    case ARGTYPE_SINT64:
        sprintf(tempString1, "{\"%s\", MULTI_INT,0, sizeof(S64), CMDAF_ALLOCATED, 0, ", pArgName);
        break;

    case ARGTYPE_UINT64:
        sprintf(tempString1, "{\"%s\", MULTI_INT,0, sizeof(U64), CMDAF_ALLOCATED, 0, ", pArgName);
        break;

    case ARGTYPE_FLOAT64:
        sprintf(tempString1, "{\"%s\", MULTI_FLOAT, 0, sizeof(F64), CMDAF_ALLOCATED, 0, ", pArgName);
        break;


    case ARGTYPE_STRING:
        sprintf(tempString1, "{\"%s\", MULTI_STRING, 0, 0, CMDAF_ALLOCATED, 0, ", pArgName);
        break;

    case ARGTYPE_SENTENCE:
        sprintf(tempString1, "{\"%s\", MULTI_STRING, 0, 0, CMDAF_SENTENCE|CMDAF_ALLOCATED, 0, ", pArgName);
        break;

    case ARGTYPE_ESCAPEDSTRING:
        sprintf(tempString1, "{\"%s\", MULTI_STRING, 0, 0, CMDAF_ESCAPEDSTRING|CMDAF_ALLOCATED, 0, ", pArgName);
        break;

    case ARGTYPE_VEC3_POINTER:
    case ARGTYPE_VEC3_DIRECT:
        sprintf(tempString1, "{\"%s\", MULTI_VEC3,0,0,CMDAF_ALLOCATED, 0, ", pArgName);
        break;

    case ARGTYPE_VEC4_POINTER:
    case ARGTYPE_VEC4_DIRECT:
        sprintf(tempString1, "{\"%s\", MULTI_VEC4,0,0,CMDAF_ALLOCATED, 0, ", pArgName);
        break;

    case ARGTYPE_MAT4_POINTER:
    case ARGTYPE_MAT4_DIRECT:
        sprintf(tempString1, "{\"%s\", MULTI_MAT4,0,0,CMDAF_ALLOCATED, 0, ", pArgName);
        break;

    case ARGTYPE_QUAT_POINTER:
    case ARGTYPE_QUAT_DIRECT:
        sprintf(tempString1, "{\"%s\", MULTI_QUAT,0,0,CMDAF_ALLOCATED, 0, ", pArgName);
        break;

    case ARGTYPE_STRUCT:
        sprintf(tempString1, "{\"%s\", MULTI_NP_POINTER, parse_%s, 0, CMDAF_ALLOCATED|CMDAF_TEXTPARSER, 0, ", pArgName, pTypeName);
        break;

    default:
        return "WHAT THE? THIS SHOULD NOT BE!!!";
    
    }

    char nameListDataPointerString[256];

    if (pNameListDataPointer && pNameListDataPointer[0])
    {
        if (bDataPointerWasString)
        {
            sprintf(nameListDataPointerString, "(void**)\"%s\"", pNameListDataPointer);
        }
        else if (strcmp(pNameListType, "NAMELISTTYPE_COMMANDLIST") == 0)
        {
            sprintf(nameListDataPointerString, "(void**)&%s", pNameListDataPointer);
        }
        else    
        {
            sprintf(nameListDataPointerString, "&%s", pNameListDataPointer);
        }
    }
    else
    {
        sprintf(nameListDataPointerString, "NULL");
    }

    sprintf(tempString2, "%s %s, %s }",
        tempString1, 
        pNameListType && pNameListType[0] ? pNameListType : "NAMELISTTYPE_NONE",
        nameListDataPointerString);

    return tempString2;


}




MagicCommandManager::enumMagicCommandArgType MagicCommandManager::GetArgTypeFromArgTypeName(char *pArgTypeName)
{
    if (StringIsInList(pArgTypeName, sSentenceNames))
    {
        return ARGTYPE_SENTENCE;
    }

    if (StringIsInList(pArgTypeName, sStringNames))
    {
        return ARGTYPE_STRING;
    }

    if (StringIsInList(pArgTypeName, sSIntNames))
    {
        return ARGTYPE_SINT;
    }

    if (StringIsInList(pArgTypeName, sUIntNames))
    {
        return ARGTYPE_UINT;
    }

    if (StringIsInList(pArgTypeName, sFloatNames))
    {
        return ARGTYPE_FLOAT;
    }

    if (StringIsInList(pArgTypeName, sSInt64Names))
    {
        return ARGTYPE_SINT64;
    }

    if (StringIsInList(pArgTypeName, sUInt64Names))
    {
        return ARGTYPE_UINT64;
    }

    if (StringIsInList(pArgTypeName, sFloat64Names))
    {
        return ARGTYPE_FLOAT64;
    }

    if (StringIsInList(pArgTypeName, sVec3Names))
    {
        return ARGTYPE_VEC3_DIRECT;
    }

    if (StringIsInList(pArgTypeName, sVec4Names))
    {
        return ARGTYPE_VEC4_DIRECT;
    }

    if (StringIsInList(pArgTypeName, sMat4Names))
    {
        return ARGTYPE_MAT4_DIRECT;
    }

    if (StringIsInList(pArgTypeName, sQuatNames))
    {
        return ARGTYPE_QUAT_DIRECT;
    }

    if (strcmp(pArgTypeName, "Entity") == 0 || strcmp(pArgTypeName, "BaseEntity") == 0)
    {
        return ARGTYPE_ENTITY;
    }

    if (strcmp(pArgTypeName, "TransactionCommand") == 0)
    {
        return ARGTYPE_TRANSACTIONCOMMAND;
    }

    if (strcmp(pArgTypeName, "Cmd") == 0)
    {
        return ARGTYPE_CMD;
    }

    if (strcmp(pArgTypeName, "CmdContext") == 0)
    {
        return ARGTYPE_CMDCONTEXT;
    }

    if (strcmp(pArgTypeName, "SlowRemoteCommandID") == 0)
    {
        return ARGTYPE_SLOWCOMMANDID;
    }

    if (strcmp(pArgTypeName, "ExprFuncReturnVal") == 0)
    {
        return ARGTYPE_EXPR_FUNCRETURNVAL;
    }

    if (strcmp(pArgTypeName, "ACMD_EXPR_SUBEXPR_IN") == 0)
    {
        return ARGTYPE_EXPR_SUBEXPR_IN;
    }

    if (strcmp(pArgTypeName, "ACMD_EXPR_ENTARRAY_IN") == 0)
    {
        return ARGTYPE_EXPR_ENTARRAY_IN;
    }

    if (strcmp(pArgTypeName, "ACMD_EXPR_INT_OUT") == 0)
    {
        return ARGTYPE_EXPR_INT_OUT;
    }

    if (strcmp(pArgTypeName, "ACMD_EXPR_FLOAT_OUT") == 0)
    {
        return ARGTYPE_EXPR_FLOAT_OUT;
    }

    if (strcmp(pArgTypeName, "ACMD_EXPR_STRING_OUT") == 0)
    {
        return ARGTYPE_EXPR_STRING_OUT;
    }

    if (strcmp(pArgTypeName, "ACMD_EXPR_ERRSTRING") == 0)
    {
        return ARGTYPE_EXPR_ERRSTRING;
    }

    if (strcmp(pArgTypeName, "ACMD_EXPR_ERRSTRING_STATIC") == 0)
    {
        return ARGTYPE_EXPR_ERRSTRING_STATIC;
    }

    if (strcmp(pArgTypeName, "ACMD_EXPR_ENTARRAY_IN_OUT") == 0)
    {
        return ARGTYPE_EXPR_ENTARRAY_IN_OUT;
    }

    if (strcmp(pArgTypeName, "ACMD_EXPR_ENTARRAY_OUT") == 0)
    {
        return ARGTYPE_EXPR_ENTARRAY_OUT;
    }

    if (strcmp(pArgTypeName, "ACMD_EXPR_LOC_MAT4_IN") == 0)
    {
        return ARGTYPE_EXPR_LOC_MAT4_IN;
    }

    if (strcmp(pArgTypeName, "ACMD_EXPR_LOC_MAT4_OUT") == 0)
    {
        return ARGTYPE_EXPR_LOC_MAT4_OUT;
    }

    if (strcmp(pArgTypeName, "ExprContext") == 0)
    {
        return ARGTYPE_EXPR_EXPRCONTEXT;
    }

    return ARGTYPE_STRUCT;
}



//returns number of dependencies found
int MagicCommandManager::ProcessDataSingleFile(char const* pSourceFileName, char* pDependencies[MAX_DEPENDENCIES_SINGLE_FILE])
{
    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (strcmp(pCommand->sourceFileName, pSourceFileName) == 0)
        {
            int i;

            if (pCommand->eReturnType == ARGTYPE_ENUM)
            {
                if (m_pParent->GetDictionary()->FindIdentifier(pCommand->returnTypeName) == IDENTIFIER_ENUM)
                {
                    
                }
                else
                {
                    CommandAssert(pCommand, 0, "Presumed enum type name not recognized");
                }            
            }


            for (i=0; i < pCommand->iNumArgs; i++)
            {
                if (pCommand->argTypes[i] == ARGTYPE_ENUM)
                {
                    if (m_pParent->GetDictionary()->FindIdentifier(pCommand->argTypeNames[i]) == IDENTIFIER_ENUM)
                    {
                        
                    }
                    else
                    {
                        CommandAssert(pCommand, 0, "Presumed enum type name not recognized");
                    }
                }
            }
        }
    }



    return 0;
}


//used for set_ and get_ function prototypes for AUTO_CMD_INTs and such, for which the
//actual type name is not available
char const* MagicCommandManager::GetGenericTypeNameFromType(enumMagicCommandArgType eType)
{
    switch (eType)
    {
    case ARGTYPE_SINT:
    case ARGTYPE_UINT:
    case ARGTYPE_SINT64:
    case ARGTYPE_UINT64:
        return "__int64";

    case ARGTYPE_FLOAT:
    case ARGTYPE_FLOAT64:
        return "F64";

    case ARGTYPE_STRING:
    case ARGTYPE_SENTENCE:
        return "char*";
    }

    return "UNKNOWN TYPE HELP!!!!";
}



void MagicCommandManager::WriteCompleteTestClientPrototype_NoReturn(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    fprintf(pOutFile, "//Wrapper function for %s which never returns\n",
        pCommand->commandName);
    
    fprintf(pOutFile, "CMD_DECLSPEC void cmd_%s%s(", pCommand->eReturnType == ARGTYPE_NONE ? "" : "noret_", pCommand->safeCommandName);

    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, false);

    fprintf(pOutFile, ")");
}

void MagicCommandManager::WriteCompleteTestClientPrototype_Blocking(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    fprintf(pOutFile, "//Wrapper function for %s which blocks until the command returns\n",
        pCommand->commandName);
    
    fprintf(pOutFile, "CMD_DECLSPEC enumTestClientCmdOutcome cmd_block_%s(char **ppCmdRetString", pCommand->safeCommandName);

    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, true);

    fprintf(pOutFile, ")");
}

void MagicCommandManager::WriteCompleteTestClientPrototype_Struct(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    fprintf(pOutFile, "//Wrapper function for %s which puts the return value into a struct which\n//can be polled and must be released via TestClientReleaseResults\n",
        pCommand->commandName);
    
    fprintf(pOutFile, "CMD_DECLSPEC TestClientCmdHandle cmd_retStruct_%s(", pCommand->safeCommandName);

    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, false);

    fprintf(pOutFile, ")");
}

void MagicCommandManager::WriteCompleteTestClientPrototype_Callback(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    fprintf(pOutFile, "//Wrapper function for %s which calls a callback when the command returns\n",
            pCommand->commandName);
        
    fprintf(pOutFile, "CMD_DECLSPEC TestClientCmdHandle cmd_retCB_%s(TestClientCallback *pCallBack, void *pUserData", pCommand->safeCommandName);

    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, true);

    fprintf(pOutFile, ")");
}

void MagicCommandManager::WriteCompleteTestClientPrototype_UserBuff(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    fprintf(pOutFile, "//Wrapper function for %s which puts the return value into a user-supplied\n//buffer which can be polled\n",
        pCommand->commandName);
    
    fprintf(pOutFile, "CMD_DECLSPEC void cmd_retUserStruct_%s(TestClientCmdResultStruct *pUserResultStruct", pCommand->safeCommandName);

    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, true);

    fprintf(pOutFile, ")");
}

void MagicCommandManager::WriteCompleteTestClientPrototype_IntReturn(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    fprintf(pOutFile, "//Wrapper function for %s which returns an int\n",
        pCommand->commandName);

    fprintf(pOutFile, "CMD_DECLSPEC %s cmd_%s(", pCommand->returnTypeName, pCommand->safeCommandName);

    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, false);

    fprintf(pOutFile, ")");
}

void MagicCommandManager::WriteCompleteTestClientPrototype_Vec3Return(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    fprintf(pOutFile, "//Wrapper function for %s which returns an vec3*\n",
        pCommand->commandName);

    fprintf(pOutFile, "CMD_DECLSPEC Vec3 *cmd_%s(", pCommand->safeCommandName);

    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, false);

    fprintf(pOutFile, ")");
}

void MagicCommandManager::WriteCompleteTestClientPrototype_Vec4Return(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    fprintf(pOutFile, "//Wrapper function for %s which returns an vec4*\n",
        pCommand->commandName);

    fprintf(pOutFile, "CMD_DECLSPEC Vec4 *cmd_%s(", pCommand->safeCommandName);

    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, false);

    fprintf(pOutFile, ")");
}

void MagicCommandManager::WriteCompleteTestClientPrototype_FloatReturn(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    fprintf(pOutFile, "//Wrapper function for %s which returns a float\n",
        pCommand->commandName);

    fprintf(pOutFile, "CMD_DECLSPEC float cmd_%s(", pCommand->safeCommandName);

    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, false);

    fprintf(pOutFile, ")");
}

void MagicCommandManager::WriteCompleteTestClientPrototype_StructReturn(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    fprintf(pOutFile, "//Wrapper function for %s which returns a %s pointer\n",
        pCommand->commandName, pCommand->returnTypeName);

    fprintf(pOutFile, "CMD_DECLSPEC %s *cmd_%s(", pCommand->returnTypeName, pCommand->safeCommandName);

    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, false);

    fprintf(pOutFile, ")");
}

void MagicCommandManager::WriteCompleteTestClientPrototype_StringReturn(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    fprintf(pOutFile, "//Wrapper function for %s which returns a string\n",
        pCommand->commandName);

    fprintf(pOutFile, "CMD_DECLSPEC char *cmd_%s(", pCommand->safeCommandName);

    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, false);

    fprintf(pOutFile, ")");
}



void MagicCommandManager::WriteOutPrototypesForTestClient(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand, int iPrefixLen)
{

    fprintf(pOutFile, "\n\n\n//////-----------------  PROTOTYPES FOR COMMAND %s\n", pCommand->commandName);

    WriteOutGenericExternsAndPrototypesForCommand(pOutFile, pCommand, false);

    WriteCompleteTestClientPrototype_NoReturn(pOutFile, pCommand);
    fprintf(pOutFile, ";\n");

    if (pCommand->eReturnType != ARGTYPE_NONE)
    {
        WriteCompleteTestClientPrototype_Blocking(pOutFile, pCommand);
        fprintf(pOutFile, ";\n");
        WriteCompleteTestClientPrototype_Struct(pOutFile, pCommand);
        fprintf(pOutFile, ";\n");
        WriteCompleteTestClientPrototype_Callback(pOutFile, pCommand);
        fprintf(pOutFile, ";\n");
        WriteCompleteTestClientPrototype_UserBuff(pOutFile, pCommand);
        fprintf(pOutFile, ";\n");
    }

    if (pCommand->eReturnType == ARGTYPE_SINT || pCommand->eReturnType == ARGTYPE_UINT
        || pCommand->eReturnType == ARGTYPE_SINT64 || pCommand->eReturnType == ARGTYPE_UINT64)
    {
        WriteCompleteTestClientPrototype_IntReturn(pOutFile, pCommand);
        fprintf(pOutFile, ";\n");
    }

    if (pCommand->eReturnType == ARGTYPE_FLOAT || pCommand->eReturnType == ARGTYPE_FLOAT64)
    {
        WriteCompleteTestClientPrototype_FloatReturn(pOutFile, pCommand);
        fprintf(pOutFile, ";\n");
    }

    if (pCommand->eReturnType == ARGTYPE_VEC3_POINTER)
    {
        WriteCompleteTestClientPrototype_Vec3Return(pOutFile, pCommand);
        fprintf(pOutFile, ";\n");
    }

    if (pCommand->eReturnType == ARGTYPE_VEC4_POINTER)
    {
        WriteCompleteTestClientPrototype_Vec4Return(pOutFile, pCommand);
        fprintf(pOutFile, ";\n");
    }

    if (pCommand->eReturnType == ARGTYPE_STRUCT)
    {
        WriteCompleteTestClientPrototype_StructReturn(pOutFile, pCommand);
        fprintf(pOutFile, ";\n");
    }

    if (pCommand->eReturnType == ARGTYPE_STRING)
    {
        WriteCompleteTestClientPrototype_StringReturn(pOutFile, pCommand);
        fprintf(pOutFile, ";\n");
    }
}




void MagicCommandManager::WriteOutSharedFunctionBody(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand, int iPrefixLen)
{

    fprintf(pOutFile, "\tchar *pAUTOGENWorkString;\n\testrStackCreate(&pAUTOGENWorkString, 1024);\n\testrPrintf(&pAUTOGENWorkString, \"%s \");\n", pCommand->commandName + iPrefixLen);

    WriteOutGenericCodeToPutArgumentsIntoEString(pOutFile, pCommand, "pAUTOGENWorkString", false);
}

void MagicCommandManager::WriteOutFunctionBodiesForTestClient(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand, int iPrefixLen)
{
    char commandDefineName[MAX_MAGICCOMMANDNAMELENGTH + 30];
    sprintf(commandDefineName, "_DEFINED_%s", pCommand->safeCommandName);
    MakeStringUpcase(commandDefineName);

    fprintf(pOutFile, "\n\n\n//////-----------------  WRAPPER FUNCTIONS FOR COMMAND %s\n", pCommand->commandName);
    fprintf(pOutFile, "#ifndef %s\n#define %s\n\n",
        commandDefineName, commandDefineName);
    
    WriteOutGenericExternsAndPrototypesForCommand(pOutFile, pCommand, false);

    WriteCompleteTestClientPrototype_NoReturn(pOutFile, pCommand);
    fprintf(pOutFile, "\n{\n");
    WriteOutSharedFunctionBody(pOutFile, pCommand, iPrefixLen);
    fprintf(pOutFile, "\tSendCommandToClient_NonBlocking(pAUTOGENWorkString, TESTCLIENTCMDTYPE_NORETURN, NULL, NULL, NULL, NULL);\n\testrDestroy(&pAUTOGENWorkString);\n}\n");

    if (pCommand->eReturnType != ARGTYPE_NONE)
    {
        WriteCompleteTestClientPrototype_Blocking(pOutFile, pCommand);
        fprintf(pOutFile, "\n{\n\tenumTestClientCmdOutcome eRetVal;\n");
        WriteOutSharedFunctionBody(pOutFile, pCommand, iPrefixLen);
        fprintf(pOutFile, "\teRetVal = SendCommandToClient_Blocking(pAUTOGENWorkString, ppCmdRetString);\n\testrDestroy(&pAUTOGENWorkString);\n\treturn eRetVal;\n}\n");

        WriteCompleteTestClientPrototype_Struct(pOutFile, pCommand);
        fprintf(pOutFile, "\n{\n\tTestClientCmdHandle retVal;\n");
        WriteOutSharedFunctionBody(pOutFile, pCommand, iPrefixLen);
        fprintf(pOutFile, "\tSendCommandToClient_NonBlocking(pAUTOGENWorkString, TESTCLIENTCMDTYPE_BUFFER, &retVal, NULL, NULL, NULL);\n\testrDestroy(&pAUTOGENWorkString);\n\treturn retVal;\n}\n");

        WriteCompleteTestClientPrototype_Callback(pOutFile, pCommand);
        fprintf(pOutFile, "\n{\n\tTestClientCmdHandle retVal;\n");
        WriteOutSharedFunctionBody(pOutFile, pCommand, iPrefixLen);
        fprintf(pOutFile, "\tSendCommandToClient_NonBlocking(pAUTOGENWorkString, TESTCLIENTCMDTYPE_CALLBACK, &retVal, NULL, pCallBack, pUserData);\n\testrDestroy(&pAUTOGENWorkString);\n\treturn retVal;\n}\n");

        WriteCompleteTestClientPrototype_UserBuff(pOutFile, pCommand);
        fprintf(pOutFile, "\n{\n");
        WriteOutSharedFunctionBody(pOutFile, pCommand, iPrefixLen);
        fprintf(pOutFile, "\tSendCommandToClient_NonBlocking(pAUTOGENWorkString, TESTCLIENTCMDTYPE_USERBUFFER, NULL, pUserResultStruct, NULL, NULL);\n\testrDestroy(&pAUTOGENWorkString);\n}\n");
    }

    if (pCommand->eReturnType == ARGTYPE_SINT || pCommand->eReturnType == ARGTYPE_UINT
        || pCommand->eReturnType == ARGTYPE_SINT64 || pCommand->eReturnType == ARGTYPE_UINT64)
    {
        WriteCompleteTestClientPrototype_IntReturn(pOutFile, pCommand);
        fprintf(pOutFile, "\n{\n\t__int64 iRetVal;\n\tenumTestClientCmdOutcome eOutCome;\n\tchar returnBuf[128];\n");
        WriteOutSharedFunctionBody(pOutFile, pCommand, iPrefixLen);
        fprintf(pOutFile, "\teOutCome = SendCommandToClient_Blocking_FixedString(pAUTOGENWorkString, returnBuf, 128);\n");
        fprintf(pOutFile, "\tassert(eOutCome == TESTCLIENT_CMD_SUCCEEDED);\n");
        fprintf(pOutFile, "\tsscanf(returnBuf, \"%%I64d\", &iRetVal);\n");
        fprintf(pOutFile, "\testrDestroy(&pAUTOGENWorkString);\n\treturn iRetVal;\n};\n");
    }

    if (pCommand->eReturnType == ARGTYPE_VEC3_POINTER)
    {
        WriteCompleteTestClientPrototype_Vec3Return(pOutFile, pCommand);
        fprintf(pOutFile, "\n{\n\tstatic Vec3 vRetVal;\n\tenumTestClientCmdOutcome eOutCome;\n\tchar returnBuf[128];\n");
        WriteOutSharedFunctionBody(pOutFile, pCommand, iPrefixLen);
        fprintf(pOutFile, "\teOutCome = SendCommandToClient_Blocking_FixedString(pAUTOGENWorkString, returnBuf, 128);\n");
        fprintf(pOutFile, "\tassert(eOutCome == TESTCLIENT_CMD_SUCCEEDED);\n");
        fprintf(pOutFile, "\tsscanf(returnBuf, \"%%f %%f %%f\", &vRetVal[0], &vRetVal[1], &vRetVal[2]);\n");
        fprintf(pOutFile, "\testrDestroy(&pAUTOGENWorkString);\n\treturn &vRetVal;\n};\n");
    }

    if (pCommand->eReturnType == ARGTYPE_VEC4_POINTER)
    {
        WriteCompleteTestClientPrototype_Vec4Return(pOutFile, pCommand);
        fprintf(pOutFile, "\n{\n\tstatic Vec4 vRetVal;\n\tenumTestClientCmdOutcome eOutCome;\n\tchar returnBuf[128];\n");
        WriteOutSharedFunctionBody(pOutFile, pCommand, iPrefixLen);
        fprintf(pOutFile, "\teOutCome = SendCommandToClient_Blocking_FixedString(pAUTOGENWorkString, returnBuf, 128);\n");
        fprintf(pOutFile, "\tassert(eOutCome == TESTCLIENT_CMD_SUCCEEDED);\n");
        fprintf(pOutFile, "\tsscanf(returnBuf, \"%%f %%f %%f %%f\", &vRetVal[0], &vRetVal[1], &vRetVal[2], &vRetVal[3]);\n");
        fprintf(pOutFile, "\testrDestroy(&pAUTOGENWorkString);\n\treturn &vRetVal;\n};\n");
    }



    if (pCommand->eReturnType == ARGTYPE_FLOAT || pCommand->eReturnType == ARGTYPE_FLOAT64)
    {
        WriteCompleteTestClientPrototype_FloatReturn(pOutFile, pCommand);
        fprintf(pOutFile, "\n{\n\t F64 fRetVal;\n\tenumTestClientCmdOutcome eOutCome;\n\tchar returnBuf[128];\n");
        WriteOutSharedFunctionBody(pOutFile, pCommand, iPrefixLen);
        fprintf(pOutFile, "\teOutCome = SendCommandToClient_Blocking_FixedString(pAUTOGENWorkString, returnBuf, 128);\n");
        fprintf(pOutFile, "\tassert(eOutCome == TESTCLIENT_CMD_SUCCEEDED);\n");
        fprintf(pOutFile, "\tsscanf(returnBuf, \"%%f\", &fRetVal);\n");
        fprintf(pOutFile, "\testrDestroy(&pAUTOGENWorkString);\n\treturn fRetVal;\n};\n");
    }

    if (pCommand->eReturnType == ARGTYPE_STRUCT)
    {
        WriteCompleteTestClientPrototype_StructReturn(pOutFile, pCommand);
        fprintf(pOutFile, "\n{\n\t %s *pRetVal;\n\tenumTestClientCmdOutcome eOutCome;\n\tchar *pRetString = NULL;\n", pCommand->returnTypeName);
        WriteOutSharedFunctionBody(pOutFile, pCommand, iPrefixLen);
        fprintf(pOutFile, "\teOutCome = SendCommandToClient_Blocking(pAUTOGENWorkString, &pRetString);\n");
        fprintf(pOutFile, "\tassert(eOutCome == TESTCLIENT_CMD_SUCCEEDED);\n");
        fprintf(pOutFile, "\tpRetVal = StructCreate(parse_%s);\n", pCommand->returnTypeName);
        fprintf(pOutFile, "\tParserReadTextEscaped(&pRetString, parse_%s, pRetVal);\n", pCommand->returnTypeName);
        fprintf(pOutFile, "\testrDestroy(&pAUTOGENWorkString);\n\testrDestroy(&pRetString);\n\treturn pRetVal;\n};\n");
    }

    if (pCommand->eReturnType == ARGTYPE_STRING)
    {
        WriteCompleteTestClientPrototype_StringReturn(pOutFile, pCommand);
        fprintf(pOutFile, "\n{\n\tchar *pRetVal;\n\tenumTestClientCmdOutcome eOutCome;\n\tchar *pRetString = NULL;\n");
        WriteOutSharedFunctionBody(pOutFile, pCommand, iPrefixLen);
        fprintf(pOutFile, "\teOutCome = SendCommandToClient_Blocking(pAUTOGENWorkString, &pRetString);\n");
        fprintf(pOutFile, "\tassert(eOutCome == TESTCLIENT_CMD_SUCCEEDED);\n");
        fprintf(pOutFile, "\tpRetVal = strdup(pRetString);\n");
        fprintf(pOutFile, "\testrDestroy(&pAUTOGENWorkString);\n\testrDestroy(&pRetString);\n\treturn pRetVal;\n};\n");
    }

    fprintf(pOutFile, "\n#endif\n\n");
}

int MagicCommandManager::CopyCommandVarIntoCommandForSetting(MAGIC_COMMAND_STRUCT *pCommand, MAGIC_COMMANDVAR_STRUCT *pCommandVar)
{
    char const* pPrefixString = "Set";
    pCommand->iNumArgs = 1;
    pCommand->argTypes[0] = pCommandVar->eVarType;
    pCommand->eReturnType = ARGTYPE_NONE;
    strcpy(pCommand->argTypeNames[0],GetGenericTypeNameFromType(pCommandVar->eVarType));
    sprintf(pCommand->argNames[0], "valToSet");
    sprintf(pCommand->commandName, "%s%s", pPrefixString, pCommandVar->varCommandName);
    strcpy(pCommand->safeCommandName,pCommand->commandName);
    MakeStringAllAlphaNum(pCommand->safeCommandName);

    return (int)strlen(pPrefixString);
}

int MagicCommandManager::CopyCommandVarIntoCommandForGetting(MAGIC_COMMAND_STRUCT *pCommand, MAGIC_COMMANDVAR_STRUCT *pCommandVar)
{
    char const* pPrefixString = "Get";
    pCommand->iNumArgs = 0;
    pCommand->eReturnType = pCommandVar->eVarType;
    strcpy(pCommand->returnTypeName, GetGenericTypeNameFromType(pCommandVar->eVarType));
    sprintf(pCommand->commandName, "%s%s", pPrefixString, pCommandVar->varCommandName);
    strcpy(pCommand->safeCommandName,pCommand->commandName);
    MakeStringAllAlphaNum(pCommand->safeCommandName);

    return (int)strlen(pPrefixString);
}

bool MagicCommandManager::CommandGetsWrittenOutForTestClients(MAGIC_COMMAND_STRUCT *pCommand)
{
    return 
    !pCommand->commandWhichThisIsTheErrorFunctionFor[0] && !(pCommand->iCommandFlags & COMMAND_FLAG_NOTESTCLIENT)
    &&  !(pCommand->iCommandFlags & COMMAND_FLAG_QUEUED) && !(pCommand->iCommandFlags & COMMAND_FLAG_REMOTE) && !(pCommand->iCommandFlags & COMMAND_FLAG_EARLYCOMMANDLINE)
    && !CommandHasExpressionOnlyArgumentsOrReturnVals(pCommand) 
    && ((pCommand->iCommandFlags & COMMAND_FLAG_TESTCLIENT) || !CommandHasArgOfType(pCommand, ARGTYPE_STRUCT));
}

void MagicCommandManager::WriteOutFakeIncludesForTestClient(FILE *pOutFile)
{
#if GENERATE_FAKE_DEPENDENCIES
    fprintf(pOutFile, "//#ifed-out includes to fool incredibuild dependency generation\n#if 0\n");
    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (CommandGetsWrittenOutForTestClients(pCommand))
        {
            fprintf(pOutFile, "#include \"%s\"\n", pCommand->sourceFileName);
        }
    }

    int iVarNum;
    for (iVarNum = 0; iVarNum < m_iNumMagicCommandVars; iVarNum++)
    {
        MAGIC_COMMANDVAR_STRUCT *pCommandVar = &m_MagicCommandVars[iVarNum];

        fprintf(pOutFile, "#include \"%s\"\n", pCommandVar->sourceFileName);
    }

    fprintf(pOutFile, "#endif\n\n");
#endif
}

void MagicCommandManager::WriteOutFilesForTestClient(void)
{
    int iCommandNum, iVarNum;
    FILE *pOutFile = fopen_nofail(m_TestClientFunctionsFileName, "wt");

    fprintf(pOutFile, "//This file is autogenerated. It contains functions to call all AUTO_COMMANDS\n//from the %s project. autogenerated""nocheckin\n\n#include \"testclientlib_internal.h\"\n\n#ifndef CMD_DECLSPEC\n#define CMD_DECLSPEC\n#endif\n", m_ProjectName);
    WriteOutFakeIncludesForTestClient(pOutFile);
    
    
    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (CommandGetsWrittenOutForTestClients(pCommand))
        {
            WriteOutFunctionBodiesForTestClient(pOutFile, pCommand, 0);
        }
    }

    for (iVarNum = 0; iVarNum < m_iNumMagicCommandVars; iVarNum++)
    {
        MAGIC_COMMANDVAR_STRUCT *pCommandVar = &m_MagicCommandVars[iVarNum];
        MAGIC_COMMAND_STRUCT command;

        int iPrefixLen = CopyCommandVarIntoCommandForSetting(&command, pCommandVar);

        WriteOutFunctionBodiesForTestClient(pOutFile, &command, iPrefixLen);

        iPrefixLen = CopyCommandVarIntoCommandForGetting(&command, pCommandVar);

        WriteOutFunctionBodiesForTestClient(pOutFile, &command, iPrefixLen);


    }

    fclose(pOutFile);

    pOutFile = fopen_nofail(m_TestClientFunctionsHeaderName, "wt");

    fprintf(pOutFile, "//This file is autogenerated. It contains function prototypes for all AUTO_COMMANDS\n//from the %s project. autogenerated""nocheckin\n#ifndef CMD_DECLSPEC\n#define CMD_DECLSPEC\n#endif\n\n", m_ProjectName);
    WriteOutFakeIncludesForTestClient(pOutFile);




    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (CommandGetsWrittenOutForTestClients(pCommand))
        {
            WriteOutPrototypesForTestClient(pOutFile, pCommand, 0);
        }
    }

    for (iVarNum = 0; iVarNum < m_iNumMagicCommandVars; iVarNum++)
    {
        MAGIC_COMMANDVAR_STRUCT *pCommandVar = &m_MagicCommandVars[iVarNum];
        MAGIC_COMMAND_STRUCT command;
        int iPrefixLen = CopyCommandVarIntoCommandForSetting(&command, pCommandVar);

        WriteOutPrototypesForTestClient(pOutFile, &command, iPrefixLen);

        iPrefixLen = CopyCommandVarIntoCommandForGetting(&command, pCommandVar);

        WriteOutPrototypesForTestClient(pOutFile, &command, iPrefixLen);
    }

    fclose(pOutFile);
}
    


void MagicCommandManager::WriteOutMainPrototypeForRemoteCommand(FILE *pFile, MAGIC_COMMAND_STRUCT *pCommand)
{

    fprintf(pFile, "\n\n//Autogenerated wrapper for function %s, source file: %s\n\n", pCommand->commandName,
        pCommand->sourceFileName);


    WriteOutGenericExternsAndPrototypesForCommand(pFile, pCommand, false);

    

    fprintf(pFile, "void RemoteCommand_%s(%s GlobalType gServerType, ContainerID gServerID",
        pCommand->safeCommandName, pCommand->eReturnType == ARGTYPE_NONE ? "" : " TransactionReturnValStruct *pReturnValStruct,");

    WriteOutGenericArgListForCommand(pFile, pCommand, false, true);

    fprintf(pFile, ")");
}

void MagicCommandManager::WriteOutReturnPrototypeForRemoteCommand(FILE *pFile, MAGIC_COMMAND_STRUCT *pCommand)
{

    fprintf(pFile, "\n\n//Autogenerated function return function %s, source file: %s\n\n", pCommand->commandName,
        pCommand->sourceFileName);

    if (pCommand->eReturnType == ARGTYPE_STRUCT)
    {
    
        fprintf(pFile, "//externs and typedefs for return struct from %s:\n", pCommand->commandName);
    
        fprintf(pFile, "typedef struct %s %s;\n", pCommand->returnTypeName, pCommand->returnTypeName);
        fprintf(pFile, "extern ParseTable parse_%s[];\n", pCommand->returnTypeName);
    }
        

    fprintf(pFile, "enumTransactionOutcome RemoteCommandCheck_%s(TransactionReturnValStruct *pTransReturnStruct, %s%s* pRetVal)",
        pCommand->safeCommandName, pCommand->returnTypeName, pCommand->eReturnType == ARGTYPE_STRUCT ? "*" : "");
}


void MagicCommandManager::WriteOutPrototypesForRemoteCommand(FILE *pFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    if (pCommand->iNumDefines)
    {
        int i;

        fprintf(pFile, "#if ");

        for (i=0; i < pCommand->iNumDefines; i++)
        {
            fprintf(pFile, "%sdefined(%s)", 
                i > 0 ? "|| " : "", pCommand->defines[i]);
        }

        fprintf(pFile, "\n\n");
    }
            
    WriteOutMainPrototypeForRemoteCommand(pFile, pCommand);
    fprintf(pFile, ";\n\n");

    if (pCommand->eReturnType != ARGTYPE_NONE)
    {
        WriteOutReturnPrototypeForRemoteCommand(pFile, pCommand);
        fprintf(pFile, ";\n\n");
    }

    if (pCommand->iNumDefines)
    {
        fprintf(pFile, "#endif\n\n");
    }
}


void MagicCommandManager::WriteOutFunctionBodiesForRemoteCommand(FILE *pFile, MAGIC_COMMAND_STRUCT *pCommand)
{

    if (pCommand->iNumDefines)
    {
        int i;

        fprintf(pFile, "#if ");

        for (i=0; i < pCommand->iNumDefines; i++)
        {
            fprintf(pFile, "%sdefined(%s)", 
                i > 0 ? "|| " : "", pCommand->defines[i]);
        }

        fprintf(pFile, "\n\n");
    }
    


    WriteOutMainPrototypeForRemoteCommand(pFile, pCommand);
    fprintf(pFile, "\n{\n\tchar *pCommandString = NULL;\n\tBaseTransaction baseTransaction;\n\tBaseTransaction **ppBaseTransactions = NULL;\n");

    fprintf(pFile, "\testrStackCreate(&pCommandString, 4096);\n\n\testrConcatf(&pCommandString, \"");
    fprintf(pFile, "%s", CommandHasArgOfType(pCommand, ARGTYPE_SLOWCOMMANDID) ? "slowremotecommand " : "remotecommand ");
    fprintf(pFile, "%s \");\n\n", pCommand->commandName);
    

    WriteOutGenericCodeToPutArgumentsIntoEString(pFile, pCommand, "pCommandString", false);


    fprintf(pFile, "\tbaseTransaction.pData = pCommandString;\n\tbaseTransaction.recipient.iRecipientID = gServerID;\n\tbaseTransaction.recipient.iRecipientType = gServerType;\n\tbaseTransaction.pRequestedTransVariableNames = NULL;\n");

    fprintf(pFile, "\teaPush(&ppBaseTransactions, &baseTransaction);\n");

    if (pCommand->eReturnType != ARGTYPE_NONE)
    {
        fprintf(pFile, "\tif (pReturnValStruct)\n\t{\n");
        fprintf(pFile, "\t\tRequestNewTransaction( objLocalManager(), ppBaseTransactions, TRANS_TYPE_SEQUENTIAL_ATOMIC, %s );\n",
            pCommand->eReturnType == ARGTYPE_NONE ? "WANTRETURNVAL_NEVER, NULL" : "WANTRETURNVAL_ALWAYS, pReturnValStruct");
        fprintf(pFile, "\t}\n\telse\n\t{\n");
        fprintf(pFile, "\t\tRequestNewTransaction( objLocalManager(), ppBaseTransactions, TRANS_TYPE_SEQUENTIAL_ATOMIC, WANTRETURNVAL_NEVER, NULL );\n\t}\n");
    }
    else
    {
        fprintf(pFile, "\tRequestNewTransaction( objLocalManager(), ppBaseTransactions, TRANS_TYPE_SEQUENTIAL_ATOMIC, WANTRETURNVAL_NEVER, NULL );\n");
    }

    fprintf(pFile, "\teaDestroy(&ppBaseTransactions);\n");
    fprintf(pFile , "\testrDestroy(&pCommandString);\n");

    fprintf(pFile, "}\n\n");

    if (pCommand->eReturnType == ARGTYPE_NONE)
    {
        if (pCommand->iNumDefines)
        {
            fprintf(pFile, "#endif\n\n");
        }

        return;
    }

    WriteOutReturnPrototypeForRemoteCommand(pFile, pCommand);

    fprintf(pFile, "\n{\n\tswitch (pTransReturnStruct->eOutcome)\n\t{\n");
    fprintf(pFile, "\tcase TRANSACTION_OUTCOME_FAILURE:\n\t\treturn TRANSACTION_OUTCOME_FAILURE;\n\n");
    fprintf(pFile, "\tcase TRANSACTION_OUTCOME_SUCCESS:\n");

    switch (pCommand->eReturnType)
    {
    case ARGTYPE_SINT:
    case ARGTYPE_UINT:
    case ARGTYPE_FLOAT:
    case ARGTYPE_SINT64:
    case ARGTYPE_UINT64:
    case ARGTYPE_FLOAT64:
        fprintf(pFile, "\t\tsscanf(pTransReturnStruct->pBaseReturnVals[0].returnString, %s, pRetVal);\n",
            pCommand->eReturnType == ARGTYPE_FLOAT ? "PRINTF_CODE_FOR_FLOAT(*pRetVal)": "PRINTF_CODE_FOR_INT(*pRetVal)");
        fprintf(pFile, "\t\treturn TRANSACTION_OUTCOME_SUCCESS;\n\n");
        break;

    case ARGTYPE_STRUCT:
        fprintf(pFile, "\t\t*pRetVal = StructAlloc(parse_%s);\n", pCommand->returnTypeName);
        fprintf(pFile, "\t\tParserReadTextEscaped(&pTransReturnStruct->pBaseReturnVals[0].returnString, parse_%s, *pRetVal);\n",
            pCommand->returnTypeName);
        fprintf(pFile, "\t\treturn TRANSACTION_OUTCOME_SUCCESS;\n");
        break;

    case ARGTYPE_STRING:
    case ARGTYPE_SENTENCE:
        fprintf(pFile, "\t\testrCopy2(pRetVal, pTransReturnStruct->pBaseReturnVals[0].returnString);\n");
        fprintf(pFile, "\t\treturn TRANSACTION_OUTCOME_SUCCESS;\n\n");
        break;

    default:
        fprintf(pFile, "ACK NO SUPPORT YET\n");
        break;
    }

    fprintf(pFile, "\t}\n\treturn TRANSACTION_OUTCOME_NONE;\n}\n\n");

    if (pCommand->iNumDefines)
    {
        fprintf(pFile, "#endif\n\n");
    }

}



void MagicCommandManager::WriteOutFunctionPrototypeForSlowCommand(FILE *pFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    
    
    fprintf(pFile, "\n\n//autogenerated function used to return the value from slow command %s\n//defined in file %s\n\n",
        pCommand->commandName, pCommand->sourceFileName);

    if (pCommand->eReturnType == ARGTYPE_STRUCT)
    {
        fprintf(pFile, "typedef struct %s %s;\n", pCommand->returnTypeName, pCommand->returnTypeName);
        fprintf(pFile, "extern ParseTable parse_%s[];\n\n",  pCommand->returnTypeName);
    }

    fprintf(pFile, "void SlowRemoteCommandReturn_%s(SlowRemoteCommandID iCmdID, %s%s retVal)",
        pCommand->safeCommandName, pCommand->returnTypeName, pCommand->eReturnType == ARGTYPE_STRUCT ? "*" : "");
}


void MagicCommandManager::WriteOutFunctionBodyForSlowCommand(FILE *pFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    WriteOutFunctionPrototypeForSlowCommand(pFile, pCommand);
    if (pCommand->eReturnType == ARGTYPE_STRING || pCommand->eReturnType == ARGTYPE_SENTENCE)
    {
        //want exact string here with no extra spaces or anything... just return it
        fprintf(pFile, "\n{\n\tReturnSlowCommand(iCmdID, true, retVal);\n}\n\n");
    }
    else
    {
        fprintf(pFile, "\n{\n\tchar *pRetString;\n\testrCreate(&pRetString);\n");
        
        WriteOutGenericCodeToPutSingleArgumentIntoEString(pFile, pCommand->eReturnType, 
            "retVal", pCommand->returnTypeName, "pRetString", false);

        fprintf(pFile, "\tReturnSlowCommand(iCmdID, true, pRetString);\n\testrDestroy(&pRetString);\n}\n\n");
    }
}

    
void MagicCommandManager::WriteOutFakeIncludesForRemoteCommands(FILE *pOutFile)
{
#if GENERATE_FAKE_DEPENDENCIES

    fprintf(pOutFile, "//#ifed-out includes to fool incredibuild dependency generation\n#if 0\n");
    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & COMMAND_FLAG_REMOTE)
        {
            fprintf(pOutFile, "#include \"%s\"\n", pCommand->sourceFileName);
        }
    }
    fprintf(pOutFile, "#endif\n\n");
#endif
}

void MagicCommandManager::WriteOutFakeIncludesForSlowCommands(FILE *pOutFile)
{
#if GENERATE_FAKE_DEPENDENCIES

    fprintf(pOutFile, "//#ifed-out includes to fool incredibuild dependency generation\n#if 0\n");
    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & COMMAND_FLAG_SLOW_REMOTE)
        {
            fprintf(pOutFile, "#include \"%s\"\n", pCommand->sourceFileName);
        }
    }
    fprintf(pOutFile, "#endif\n\n");
#endif
}


void MagicCommandManager::WriteOutRemoteCommands(void)
{
    FILE *pOutFile = fopen_nofail(m_RemoteFunctionsFileName, "wt");

    fprintf(pOutFile, "// For more info on remote commands, look here: http://code:8081/display/Core/AUTO_COMMAND_REMOTE+and+AUTO_COMMAND_REMOTE_SLOW\n");
    fprintf(pOutFile, "// This file is autogenerated. autogenerated""nocheckin\n"
                      "#include \"RemoteAutoCommandSupport.h\"\n"
                      "#include <utilitieslib/utils/textparser.h>\n"
                      "#include \"ObjTransactions.h\"\n\n");

    WriteOutFakeIncludesForRemoteCommands(pOutFile);

    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & COMMAND_FLAG_REMOTE)
        {
            WriteOutFunctionBodiesForRemoteCommand(pOutFile, pCommand);
        }
    }

    fclose(pOutFile);


    pOutFile = fopen_nofail(m_RemoteFunctionsHeaderName, "wt");

    fprintf(pOutFile, "//For more info on remote commands, look here: http://code:8081/display/Core/AUTO_COMMAND_REMOTE+and+AUTO_COMMAND_REMOTE_SLOW\n");
    fprintf(pOutFile, "//This file is autogenerated. autogenerated""nocheckin\n#pragma once\n#include \"RemoteAutoCommandSupport.h\"\n\n");
    
    WriteOutFakeIncludesForRemoteCommands(pOutFile);


    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & COMMAND_FLAG_REMOTE)
        {
            WriteOutPrototypesForRemoteCommand(pOutFile, pCommand);
        }
    }

    fclose(pOutFile);



    pOutFile = fopen_nofail(m_SlowFunctionsFileName, "wt");

    fprintf(pOutFile, "// For more info on remote commands, look here: http://code:8081/display/Core/AUTO_COMMAND_REMOTE+and+AUTO_COMMAND_REMOTE_SLOW\n");
    fprintf(pOutFile, "// This file is autogenerated. autogenerated""nocheckin\n"
                      "#include \"RemoteAutoCommandSupport.h\"\n"
                      "#include \"objtransactions.h\"\n"
                      "#include <utilitieslib/utils/textparser.h>\n\n");
    
    WriteOutFakeIncludesForSlowCommands(pOutFile);


    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & COMMAND_FLAG_SLOW_REMOTE)
        {
            WriteOutFunctionBodyForSlowCommand(pOutFile, pCommand);
        }
    }

    fclose(pOutFile);

    pOutFile = fopen_nofail(m_SlowFunctionsHeaderName, "wt");

    fprintf(pOutFile, "// For more info on remote commands, look here: http://code:8081/display/Core/AUTO_COMMAND_REMOTE+and+AUTO_COMMAND_REMOTE_SLOW\n");
    fprintf(pOutFile, "// This file is autogenerated. autogenerated""nocheckin\n"
                      "#pragma once\n"
                      "#include \"RemoteAutoCommandSupport.h\"\n"
                      "#include \"objtransactions.h\"\n"
                      "#include <utilitieslib/utils/textparser.h>\n\n");
    
    WriteOutFakeIncludesForSlowCommands(pOutFile);

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & COMMAND_FLAG_SLOW_REMOTE)
        {
            WriteOutFunctionPrototypeForSlowCommand(pOutFile, pCommand);
            fprintf(pOutFile, ";\n\n");
        }
    }

    fclose(pOutFile);


}
    
void MagicCommandManager::WriteOutMainQueuedCommandPrototype(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    int i;

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        if (pCommand->argTypes[i] == ARGTYPE_VOIDSTAR)
        {
            char typeName[256];
            char *pAsterisk;
            strcpy(typeName, pCommand->argTypeNames[i]);
            pAsterisk = strchr(typeName, '*');
            if (pAsterisk)
            {
                *pAsterisk = 0;
            }

            if (strcmp(NoConst(typeName), "void") != 0 && strcmp(NoConst(typeName), "char") != 0)
            {
                fprintf(pOutFile, "typedef struct %s %s;\n",
                    NoConst(typeName), NoConst(typeName));
            }
        }
    }

    fprintf(pOutFile, "\nvoid QueuedCommand_%s(", pCommand->safeCommandName);

    if (pCommand->iNumArgs == 0 && pCommand->queueName[0])
    {
        fprintf(pOutFile, " void )");
    }
    else
    {
        if (pCommand->queueName[0] == 0)
        {
            fprintf(pOutFile, "CommandQueue *pQueue%s", pCommand->iNumArgs ? ", " : "");
        }

        int i;

        for (i=0; i < pCommand->iNumArgs; i++)
        {
            fprintf(pOutFile, "%s %s%s%s", pCommand->argTypeNames[i], 
                IsPointerType(pCommand->argTypes[i]) ? "*" : "",
                pCommand->argNames[i], i < pCommand->iNumArgs - 1 ? "," : "");
        }

        fprintf(pOutFile, ")");
    }
}

void MagicCommandManager::WriteOutWrapperQueuedCommandPrototype(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    fprintf(pOutFile, "void %s_QUEUEDWRAPPER(CommandQueue *pQueue)", pCommand->safeCommandName);
}

void MagicCommandManager::WriteOutPrototypesForQueuedCommand(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    WriteOutMainQueuedCommandPrototype(pOutFile, pCommand);
    fprintf(pOutFile, ";\n\n");

    WriteOutWrapperQueuedCommandPrototype(pOutFile, pCommand);
    fprintf(pOutFile, ";\n\n");
}

bool MagicCommandManager::QueuedCommandTypeMightBeNull(enumMagicCommandArgType eType)
{
    switch (eType)
    {
    case ARGTYPE_VEC3_POINTER:
    case ARGTYPE_VEC3_DIRECT:
    case ARGTYPE_VEC4_POINTER:
    case ARGTYPE_VEC4_DIRECT:
    case ARGTYPE_MAT4_POINTER:
    case ARGTYPE_MAT4_DIRECT:
    case ARGTYPE_QUAT_POINTER:
    case ARGTYPE_QUAT_DIRECT:
        return true;
    }

    return false;
}

void MagicCommandManager::WriteOutBodiesForQueuedCommand(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    int i;

    //first, prototype of source function
    fprintf(pOutFile, "void %s(", pCommand->safeCommandName);

    if (pCommand->iNumArgs == 0)
    {
        fprintf(pOutFile, " void );\n\n");
    }
    else
    {
        for (i=0; i < pCommand->iNumArgs; i++)
        {
            fprintf(pOutFile, "%s %s%s%s", 
                pCommand->argTypeNames[i], 
                IsPointerType(pCommand->argTypes[i]) ? "*" : "", 
                pCommand->argNames[i], 
                i < pCommand->iNumArgs - 1 ? "," : "");
        }

        fprintf(pOutFile, ");\n\n");
    }

    //extern declare of queue
    if (pCommand->queueName[0])
    {
        fprintf(pOutFile, "extern CommandQueue *%s;\n\n", pCommand->queueName);
    }

    WriteOutWrapperQueuedCommandPrototype(pOutFile, pCommand);
    fprintf(pOutFile, "\n{\n");

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        fprintf(pOutFile, "\t%s %s;\n", NoConst(pCommand->argTypeNames[i]), pCommand->argNames[i]);
        
    
        if (QueuedCommandTypeMightBeNull(pCommand->argTypes[i]))
        {
            fprintf(pOutFile, "\tbool bRead_%s = false;\n", pCommand->argNames[i]);    
        }
    }

    fprintf(pOutFile, "\n\tCommandQueue_EnterCriticalSection(pQueue);\n\n");

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        if (pCommand->argTypes[i] == ARGTYPE_STRING || pCommand->argTypes[i] == ARGTYPE_SENTENCE)
        {
            fprintf(pOutFile, "\testrStackCreate(&%s, 1024);\n", pCommand->argNames[i]);
        }
    }

    fprintf(pOutFile, "\n");

    for (i=0;i < pCommand->iNumArgs; i++)
    {
        switch (pCommand->argTypes[i])
        {
        case ARGTYPE_SINT:
        case ARGTYPE_UINT:
        case ARGTYPE_FLOAT:
        case ARGTYPE_SINT64:
        case ARGTYPE_UINT64:
        case ARGTYPE_FLOAT64:
            fprintf(pOutFile, "\tCommandQueue_Read(pQueue, &%s, sizeof(%s));\n", 
                pCommand->argNames[i], NoConst(pCommand->argTypeNames[i]));
            break;

        case ARGTYPE_VEC3_POINTER:
        case ARGTYPE_VEC3_DIRECT:
        case ARGTYPE_VEC4_POINTER:
        case ARGTYPE_VEC4_DIRECT:
        case ARGTYPE_MAT4_POINTER:
        case ARGTYPE_MAT4_DIRECT:
        case ARGTYPE_QUAT_POINTER:
        case ARGTYPE_QUAT_DIRECT:
            fprintf(pOutFile, "\tif (CommandQueue_ReadByte(pQueue))\n\t{\n\t\tbRead_%s=true;\n\t\tCommandQueue_Read(pQueue, %s, sizeof(%s));\n\t}\n", 
                pCommand->argNames[i], pCommand->argNames[i], NoConst(pCommand->argTypeNames[i]));
            break;

        case ARGTYPE_STRING:
        case ARGTYPE_SENTENCE:
            fprintf(pOutFile, "\tCommandQueue_ReadString(pQueue, &%s);\n", pCommand->argNames[i]);
            break;

        case ARGTYPE_VOIDSTAR:
            fprintf(pOutFile, "\tCommandQueue_Read(pQueue, &%s, sizeof(void*));\n", 
                pCommand->argNames[i]);
            break;
        }
    }

    fprintf(pOutFile, "\n\t%s(", pCommand->safeCommandName);

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        if (QueuedCommandTypeMightBeNull(pCommand->argTypes[i]))
        {
            fprintf(pOutFile, "bRead_%s ? %s%s : NULL%s", pCommand->argNames[i], IsPointerType(pCommand->argTypes[i]) ? "&" : "", pCommand->argNames[i], i < pCommand->iNumArgs - 1 ? ", " : "");
        }
        else
        {
            fprintf(pOutFile, "%s%s%s", IsPointerType(pCommand->argTypes[i]) ? "&" : "", pCommand->argNames[i], i < pCommand->iNumArgs - 1 ? ", " : "");
        }
    }

    fprintf(pOutFile, ");\n\n");

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        if (pCommand->argTypes[i] == ARGTYPE_STRING || pCommand->argTypes[i] == ARGTYPE_SENTENCE)
        {
            fprintf(pOutFile, "\testrDestroy(&%s);\n", pCommand->argNames[i]);
        }
    }

    fprintf(pOutFile, "\n\tCommandQueue_LeaveCriticalSection(pQueue);\n\n");

    fprintf(pOutFile, "}\n\n");

    WriteOutMainQueuedCommandPrototype(pOutFile, pCommand);

    char const* pQueueNameToUse = pCommand->queueName[0] ? pCommand->queueName : "pQueue";


    fprintf(pOutFile, "\n{\n\tvoid *pFunc = %s_QUEUEDWRAPPER;\n\tCommandQueue_EnterCriticalSection(%s);\n\tCommandQueue_Write(%s, &pFunc, sizeof(void*));\n", pCommand->commandName, pQueueNameToUse, pQueueNameToUse);

    for (i=0;i < pCommand->iNumArgs; i++)
    {
        switch (pCommand->argTypes[i])
        {
        case ARGTYPE_SINT:
        case ARGTYPE_UINT:
        case ARGTYPE_FLOAT:
        case ARGTYPE_SINT64:
        case ARGTYPE_UINT64:
        case ARGTYPE_FLOAT64:
            fprintf(pOutFile, "\tCommandQueue_Write(%s, &%s, sizeof(%s));\n", 
                pQueueNameToUse, pCommand->argNames[i], pCommand->argTypeNames[i]);
            break;

        case ARGTYPE_VEC3_POINTER:
        case ARGTYPE_VEC3_DIRECT:
        case ARGTYPE_VEC4_POINTER:
        case ARGTYPE_VEC4_DIRECT:
        case ARGTYPE_MAT4_POINTER:
        case ARGTYPE_MAT4_DIRECT:
        case ARGTYPE_QUAT_POINTER:
        case ARGTYPE_QUAT_DIRECT:
            fprintf(pOutFile, "\tif (%s)\n\t{\n\t\tCommandQueue_WriteByte(%s, 1);\n\t\tCommandQueue_Write(%s, %s, sizeof(%s));\n\t}\n\telse\n\t{\n\t\tCommandQueue_WriteByte(%s, 0);\n\t}\n",
                pCommand->argNames[i], pQueueNameToUse, pQueueNameToUse, pCommand->argNames[i], pCommand->argTypeNames[i], pQueueNameToUse);
            break;

        case ARGTYPE_STRING:
        case ARGTYPE_SENTENCE:
            fprintf(pOutFile, "\tCommandQueue_WriteString(%s, %s);\n", pQueueNameToUse, pCommand->argNames[i]);
            break;

        case ARGTYPE_VOIDSTAR:
            fprintf(pOutFile, "\tCommandQueue_Write(%s, &%s, sizeof(void*));\n", 
                pQueueNameToUse, pCommand->argNames[i]);
            break;

        }
    }
    fprintf(pOutFile, "\n\tCommandQueue_LeaveCriticalSection(%s);\n\n", pQueueNameToUse);

    fprintf(pOutFile, "}\n\n");
}


void MagicCommandManager::WriteOutFakeIncludesForQueuedCommands(FILE *pOutFile)
{
#if GENERATE_FAKE_DEPENDENCIES

    fprintf(pOutFile, "//#ifed-out includes to fool incredibuild dependency generation\n#if 0\n");
    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & COMMAND_FLAG_QUEUED)
        {
            fprintf(pOutFile, "#include \"%s\"\n", pCommand->sourceFileName);
        }
    }
    fprintf(pOutFile, "#endif\n\n");
#endif
}


void MagicCommandManager::WriteOutQueuedCommands(void)
{
    FILE *pOutFile = fopen_nofail(m_QueuedFunctionsFileName, "wt");

    fprintf(pOutFile, "//For more info on queued commands, look here: http://code:8081/display/Core/AUTO_COMMAND_QUEUED\n");

    fprintf(pOutFile, "//This file is autogenerated. autogenerated""nocheckin\n#include \"commandqueue.h\"\n\n\n");

    WriteOutFakeIncludesForQueuedCommands(pOutFile);


    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & COMMAND_FLAG_QUEUED)
        {
            WriteOutBodiesForQueuedCommand(pOutFile, pCommand);
        }
    }

    fclose(pOutFile);


    pOutFile = fopen_nofail(m_QueuedFunctionsHeaderName, "wt");

    fprintf(pOutFile, "//For more info on queued commands, look here: http://code:8081/display/Core/AUTO_COMMAND_QUEUED\n");
    fprintf(pOutFile, "//This file is autogenerated. autogenerated""nocheckin\n#pragma once\n#include \"commandqueue.h\"\n#include \"estring.h\"\n\n");
    
    WriteOutFakeIncludesForQueuedCommands(pOutFile);

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & COMMAND_FLAG_QUEUED)
        {
            WriteOutPrototypesForQueuedCommand(pOutFile, pCommand);
        }
    }

    fclose(pOutFile);



}
    

bool MagicCommandManager::DoesArgTypeNeedConstInWrapperPrototype(enumMagicCommandArgType eType, char *pTypeName)
{
    if (strncmp(pTypeName, "const ", 6) == 0 || strstr(pTypeName, " const "))
    {
        return false;
    }

    switch (eType)
    {
    case ARGTYPE_STRUCT:
    case ARGTYPE_STRING:
    case ARGTYPE_SENTENCE:
    case ARGTYPE_ESCAPEDSTRING:
        return true;
    }

    return false;
}


void MagicCommandManager::WriteOutGenericArgListForCommand(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand, 
   bool bIncludeSpecialArgs, bool bOtherArgsAlreadyWritten)
{
    int i;
    bool bWroteAtLeastOne = false;

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        if (bIncludeSpecialArgs || pCommand->argTypes[i] < ARGTYPE_FIRST_SPECIAL)
        {
            fprintf(pOutFile, "%s%s%s%s %s", 
                bWroteAtLeastOne || bOtherArgsAlreadyWritten ? ", " : "",
                DoesArgTypeNeedConstInWrapperPrototype(pCommand->argTypes[i], pCommand->argTypeNames[i]) ? "const " : "",
                pCommand->argTypeNames[i],
                (IsPointerType(pCommand->argTypes[i]) || pCommand->argTypes[i] == ARGTYPE_STRUCT) ? "*" : "",
                pCommand->argNames[i]);

            bWroteAtLeastOne = true;
        }
    }

    if (!bWroteAtLeastOne && !bOtherArgsAlreadyWritten)
    {
        fprintf(pOutFile, " void ");
    }
}

void MagicCommandManager::WriteOutGenericExternsAndPrototypesForCommand(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand, bool bWriteNameListStuff)
{
    int i;

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        if (pCommand->argTypes[i] == ARGTYPE_STRUCT)
        {
            fprintf(pOutFile, "typedef struct %s %s;\nextern ParseTable parse_%s[];\n", 
                pCommand->argTypeNames[i], pCommand->argTypeNames[i], pCommand->argTypeNames[i]);
        }

        if (pCommand->argTypes[i] == ARGTYPE_ENUM)
        {
            fprintf(pOutFile, "typedef enum %s %s;\n", pCommand->argTypeNames[i], pCommand->argTypeNames[i]);
        }

        if (bWriteNameListStuff)
        {
            if (pCommand->argNameListDataPointerNames[i][0] && !pCommand->argNameListDataPointerWasString[i])
            {
                if (strcmp(pCommand->argNameListTypeNames[i], "NAMELISTTYPE_COMMANDLIST") == 0)
                {
                    fprintf(pOutFile, "extern CmdList %s;\n", pCommand->argNameListDataPointerNames[i]);
                }
                else if (strcmp(pCommand->argNameListTypeNames[i], "NAMELISTTYPE_PREEXISTING") == 0)
                {
                    fprintf(pOutFile, "extern NameList *%s;\n", pCommand->argNameListDataPointerNames[i]);
                }
                else
                {
                    fprintf(pOutFile, "extern void *%s;\n", pCommand->argNameListDataPointerNames[i]);
                }
            }
        }
    }

    if (pCommand->eReturnType == ARGTYPE_STRUCT)
    {
        fprintf(pOutFile, "typedef struct %s %s;\n", pCommand->returnTypeName, pCommand->returnTypeName);
        fprintf(pOutFile, "extern ParseTable parse_%s[];\n", pCommand->returnTypeName);
    }

    if (pCommand->eReturnType == ARGTYPE_ENUM)
    {
        fprintf(pOutFile, "typedef enum %s %s;\n", pCommand->returnTypeName, pCommand->returnTypeName);
    }

    
}
void MagicCommandManager::WriteOutGenericCodeToPutArgumentsIntoEString(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand, 
   char const* pEStringName, bool bEscapeAllStrings)
{
    int i;

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        WriteOutGenericCodeToPutSingleArgumentIntoEString(pOutFile, pCommand->argTypes[i], pCommand->argNames[i], pCommand->argTypeNames[i],
            pEStringName, bEscapeAllStrings);
    }
}



void MagicCommandManager::WriteOutGenericCodeToPutSingleArgumentIntoEString(FILE *pOutFile, enumMagicCommandArgType eArgType,
    char const* pArgName, char const* pArgTypeName, char const* pEStringName, bool bEscapeAllStrings)
{

    switch (eArgType)
    {
    case ARGTYPE_SINT:
    case ARGTYPE_UINT:
    case ARGTYPE_SINT64:
    case ARGTYPE_UINT64:
        fprintf(pOutFile, "\testrConcatf(&%s, PRINTF_CODE_FOR_INT(%s), %s);\n\n",
            pEStringName, pArgName, pArgName);
        break;

    case ARGTYPE_FLOAT:
    case ARGTYPE_FLOAT64:
        fprintf(pOutFile, "\testrConcatf(&%s, PRINTF_CODE_FOR_FLOAT(%s), %s);\n\n", 
            pEStringName, pArgName, pArgName);
        break;

    case ARGTYPE_STRING:
        if (bEscapeAllStrings)
        {
            fprintf(pOutFile, "\testrAppend2(&%s, \" \\\"\");\n", pEStringName);
            fprintf(pOutFile, "\tif (%s) estrAppendEscaped(&%s, %s);\n", pArgName, pEStringName, pArgName);
            fprintf(pOutFile, "\testrAppend2(&%s, \"\\\" \");\n\n", pEStringName);
        }
        else
        {
            fprintf(pOutFile, "\testrConcatf(&%s, \" \\\"%%s\\\" \", %s ? %s : \"\");\n\n", pEStringName, pArgName, pArgName);
        }
        break;

    case ARGTYPE_SENTENCE:
        if (bEscapeAllStrings)
        {
            fprintf(pOutFile, "\testrAppend2(&%s, \" \\\"\");\n", pEStringName);
            fprintf(pOutFile, "\tif (%s) estrAppendEscaped(&%s, %s);\n", pArgName, pEStringName, pArgName);
            fprintf(pOutFile, "\testrAppend2(&%s, \"\\\" \");\n\n", pEStringName);
        }
        else
        {
            fprintf(pOutFile, "\testrConcatf(&%s, \" %%s\", %s ? %s : \"\");\n\n", pEStringName, pArgName, pArgName);
        }
        break;

    case ARGTYPE_ESCAPEDSTRING:
        fprintf(pOutFile, "\testrAppend2(&%s, \" \\\"\");\n", pEStringName);
        fprintf(pOutFile, "\tif (%s) estrAppendEscaped(&%s, %s);\n", pArgName, pEStringName, pArgName);
        fprintf(pOutFile, "\testrAppend2(&%s, \"\\\" \");\n\n", pEStringName);
        break;

    case ARGTYPE_VEC3_POINTER:
        fprintf(pOutFile, "\testrConcatf(&%s, \" %%f %%f %%f \", (*%s)[0], (*%s)[1], (*%s)[2]);\n",
            pEStringName, pArgName, pArgName, pArgName);
        break;

    case ARGTYPE_VEC3_DIRECT:
        fprintf(pOutFile, "\testrConcatf(&%s, \" %%f %%f %%f \", %s[0], %s[1], %s[2]);\n",
            pEStringName, pArgName, pArgName, pArgName);
        break;

    case ARGTYPE_VEC4_POINTER:
        fprintf(pOutFile, "\testrConcatf(&%s, \" %%f %%f %%f %%f \", (*%s)[0], (*%s)[1], (*%s)[2], (*%s)[3]);\n",
            pEStringName, pArgName, pArgName, pArgName, pArgName);
        break;

    case ARGTYPE_VEC4_DIRECT:
        fprintf(pOutFile, "\testrConcatf(&%s, \" %%f %%f %%f %%f \", %s[0], %s[1], %s[2], %s[3]);\n",
            pEStringName, pArgName, pArgName, pArgName, pArgName);
        break;

    case ARGTYPE_MAT4_POINTER:
        fprintf(pOutFile, "\testrConcatf(&%s, \" %%f %%f %%f %%f %%f %%f %%f %%f %%f %%f %%f %%f \", (*%s)[3][0], (*%s)[3][1], (*%s)[3][2], (*%s)[0][0], (*%s)[0][1], (*%s)[0][2], (*%s)[1][0], (*%s)[1][1], (*%s)[1][2], (*%s)[2][0], (*%s)[2][1], (*%s)[2][2]);\n", pEStringName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName);
        break;

    case ARGTYPE_MAT4_DIRECT:
        fprintf(pOutFile, "\testrConcatf(&%s, \" %%f %%f %%f %%f %%f %%f %%f %%f %%f %%f %%f %%f \", %s[3][0], %s[3][1], %s[3][2], %s[0][0], %s[0][1], %s[0][2], %s[1][0], %s[1][1], %s[1][2], %s[2][0], %s[2][1], %s[2][2]);\n", pEStringName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName, pArgName);
        break;

    case ARGTYPE_QUAT_POINTER:
        fprintf(pOutFile, "\testrConcatf(&%s, \" %%f %%f %%f %%f \", (*%s)[0], (*%s)[1], (*%s)[2], (*%s)[3]);\n",
            pEStringName, pArgName, pArgName, pArgName, pArgName);
        break;

    case ARGTYPE_QUAT_DIRECT:
        fprintf(pOutFile, "\testrConcatf(&%s, \" %%f %%f %%f %%f\", %s[0], %s[1], %s[2], %s[3]);\n",
            pEStringName, pArgName, pArgName, pArgName, pArgName);
        break;

    case ARGTYPE_STRUCT:
        fprintf(pOutFile, "\tParserWriteTextEscaped(&%s, parse_%s, %s, 0, 0);\n\n",
            pEStringName, pArgTypeName, pArgName);
        break;
    }
        
    
}



bool MagicCommandManager::CommandGetsWrittenOutForClientOrServerWrapper(MAGIC_COMMAND_STRUCT *pCommand)
{
    if (CommandHasExpressionOnlyArgumentsOrReturnVals(pCommand))
    {
        return false;
    }

    if (pCommand->iCommandFlags & COMMAND_FLAG_EARLYCOMMANDLINE)
    {
        return false;
    }

    if (pCommand->iCommandFlags & COMMAND_FLAG_REMOTE)
    {
        return false;
    }

    if (pCommand->iCommandFlags & COMMAND_FLAG_QUEUED)
    {
        return false;
    }

    if (pCommand->commandWhichThisIsTheErrorFunctionFor[0])
    {
        return false;
    }


    return true;
}
    
void MagicCommandManager::WriteOutPrototypeForServerWrapper(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    WriteOutGenericExternsAndPrototypesForCommand(pOutFile, pCommand, false);

    fprintf(pOutFile, "void ServerCmd_%s(", pCommand->safeCommandName);
    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, false);

    fprintf(pOutFile, ")");
}

void MagicCommandManager::WriteOutBodyForServerWrapper(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    if (pCommand->iNumDefines)
    {
        int i;

        fprintf(pOutFile, "#if ");

        for (i=0; i < pCommand->iNumDefines; i++)
        {
            fprintf(pOutFile, "%sdefined(%s)", 
                i > 0 ? "|| " : "", pCommand->defines[i]);
        }

        fprintf(pOutFile, "\n\n");
    }
    else
    {
        fprintf(pOutFile, "#ifdef GAMECLIENT\n");
    }

    WriteOutPrototypeForServerWrapper(pOutFile, pCommand);

    fprintf(pOutFile, "\n{\n\tchar *pAUTOGENWorkString;\n\testrStackCreate(&pAUTOGENWorkString, 1024);\n");
    fprintf(pOutFile, "\testrCopy2(&pAUTOGENWorkString, \"%s \");\n", pCommand->commandName);
    
    WriteOutGenericCodeToPutArgumentsIntoEString(pOutFile, pCommand, "pAUTOGENWorkString", true);
    
    fprintf(pOutFile, "\tcmdSendCmdClientToServer(pAUTOGENWorkString, %s, CMD_CONTEXT_FLAG_ALL_STRINGS_ESCAPED);\n", pCommand->iCommandFlags & COMMAND_FLAG_PRIVATE ? "true" : "false");

    fprintf(pOutFile, "\testrDestroy(&pAUTOGENWorkString);\n}\n\n");

    fprintf(pOutFile, "#endif\n\n");
}
    



void MagicCommandManager::WriteOutFakeIncludes(FILE *pOutFile, int iFlagToMatch)
{
#if GENERATE_FAKE_DEPENDENCIES

    fprintf(pOutFile, "//#ifed-out includes to fool incredibuild dependency generation\n#if 0\n");
    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & iFlagToMatch || !iFlagToMatch)
        {
            fprintf(pOutFile, "#include \"%s\"\n", pCommand->sourceFileName);
        }
    }
    fprintf(pOutFile, "#endif\n\n");
#endif
}

void MagicCommandManager::WriteOutClientWrappers(void)
{
    FILE *pOutFile = fopen_nofail(m_ClientWrappersFileName, "wt");

    fprintf(pOutFile, "// This file is autogenerated. autogenerated""nocheckin\n"
                      "#include \"cmdparse.h\"\n"
                      "#include <utilitieslib/utils/textparser.h>\n\n\n");

    WriteOutFakeIncludes(pOutFile, COMMAND_FLAG_CLIENT_WRAPPER);

    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & COMMAND_FLAG_CLIENT_WRAPPER)
        {
            CommandAssert(pCommand, CommandGetsWrittenOutForClientOrServerWrapper(pCommand), "Invalid CLIENTCMD request");

            WriteOutBodyForClientWrapper(pOutFile, pCommand);
        }
    }

    fclose(pOutFile);


    pOutFile = fopen_nofail(m_ClientWrappersHeaderFileName, "wt");

    fprintf(pOutFile, "//This file is autogenerated. autogenerated""nocheckin\n#pragma once\n\n");
    WriteOutFakeIncludes(pOutFile, COMMAND_FLAG_CLIENT_WRAPPER);

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];
    if (pCommand->iCommandFlags & COMMAND_FLAG_CLIENT_WRAPPER)
        {
            WriteOutPrototypeForClientWrapper(pOutFile, pCommand);
            fprintf(pOutFile, ";\n\n");
        }
    }

    fclose(pOutFile);
}



void MagicCommandManager::WriteOutPrototypeForClientWrapper(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    WriteOutGenericExternsAndPrototypesForCommand(pOutFile, pCommand, false);

    fprintf(pOutFile, "void ClientCmd_%s(Entity *pEntity", pCommand->safeCommandName);
    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, true);

    fprintf(pOutFile, ")");
}

void MagicCommandManager::WriteOutBodyForClientWrapper(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{

    if (pCommand->iNumDefines)
    {
        int i;

        fprintf(pOutFile, "#if ");

        for (i=0; i < pCommand->iNumDefines; i++)
        {
            fprintf(pOutFile, "%sdefined(%s)", 
                i > 0 ? "|| " : "", pCommand->defines[i]);
        }

        fprintf(pOutFile, "\n\n");
    }
    else
    {
        fprintf(pOutFile, "#ifdef GAMESERVER\n");
    }

    WriteOutPrototypeForClientWrapper(pOutFile, pCommand);

    fprintf(pOutFile, "\n{\n\tchar *pAUTOGENWorkString;\n\testrStackCreate(&pAUTOGENWorkString, 1024);\n");
    fprintf(pOutFile, "\testrCopy2(&pAUTOGENWorkString, \"%s \");\n", pCommand->commandName);
    WriteOutGenericCodeToPutArgumentsIntoEString(pOutFile, pCommand, "pAUTOGENWorkString",  true);
    
    fprintf(pOutFile, "\tcmdSendCmdServerToClient(pEntity, pAUTOGENWorkString, %s, CMD_CONTEXT_FLAG_ALL_STRINGS_ESCAPED);\n", pCommand->iCommandFlags & COMMAND_FLAG_PRIVATE ? "true" : "false");

    fprintf(pOutFile, "\testrDestroy(&pAUTOGENWorkString);\n}\n\n");

    fprintf(pOutFile, "#endif\n\n");
}




void MagicCommandManager::WriteOutServerWrappers(void)
{
    FILE *pOutFile = fopen_nofail(m_ServerWrappersFileName, "wt");


    fprintf(pOutFile, "// This file is autogenerated. autogenerated""nocheckin\n"
                      "#include <utilitieslib/utils/textparser.h>\n\n\n");

    WriteOutFakeIncludes(pOutFile, COMMAND_FLAG_SERVER_WRAPPER);
    
    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & COMMAND_FLAG_SERVER_WRAPPER)
        {
            CommandAssert(pCommand, CommandGetsWrittenOutForClientOrServerWrapper(pCommand), "Invalid CLIENTCMD request");

            WriteOutBodyForServerWrapper(pOutFile, pCommand);
        }    
    }

    fclose(pOutFile);


    pOutFile = fopen_nofail(m_ServerWrappersHeaderFileName, "wt");

    fprintf(pOutFile, "//This file is autogenerated. autogenerated""nocheckin\n#pragma once\n\n");
    WriteOutFakeIncludes(pOutFile, COMMAND_FLAG_SERVER_WRAPPER);
    
    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & COMMAND_FLAG_SERVER_WRAPPER)
        {
            WriteOutPrototypeForServerWrapper(pOutFile, pCommand);
            fprintf(pOutFile, ";\n\n");
        }
    }

    fclose(pOutFile);
}


void MagicCommandManager::WriteOutClientToTestClientWrappers(void)
{
    FILE *pOutFile = fopen_nofail(m_ClientToTestClientWrappersFileName, "wt");
    fprintf(pOutFile, "// This file is autogenerated. autogenerated""nocheckin\n"
                      "#include <utilitieslib/utils/textparser.h>\n"
                      "#include \"GameClientLib.h\"\n"
                      "#include \"net.h\"\n"
                      "#include \"testclient_comm.h\"\n\n\n");

    WriteOutFakeIncludes(pOutFile, 0);

    fprintf(pOutFile, "#ifdef CLIENT_TO_TESTCLIENT_COMMANDS\n");

    
    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        AssertCommandIsOKForClientToTestClient(pCommand);

        WriteOutBodyForClientToTestClientWrapper(pOutFile, pCommand);            
    }

    fprintf(pOutFile, "\n#endif\n\n");

    fclose(pOutFile);


    pOutFile = fopen_nofail(m_ClientToTestClientWrappersHeaderFileName, "wt");

    fprintf(pOutFile, "//This file is autogenerated. autogenerated""nocheckin\n#pragma once\n\n");
    WriteOutFakeIncludes(pOutFile, 0);
    
    fprintf(pOutFile, "#ifdef CLIENT_TO_TESTCLIENT_COMMANDS\n");

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        WriteOutPrototypeForClientToTestClientWrapper(pOutFile, pCommand);
        fprintf(pOutFile, ";\n\n");
    }

    fprintf(pOutFile, "\n#else\n\n");

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        WriteOutIfDefForIgnoringClientToTestClientWrapper(pOutFile, pCommand);
    }

    fprintf(pOutFile, "\n#endif\n\n");
    

    fclose(pOutFile);
}



bool MagicCommandManager::AtLeastOneCommandHasFlag(int iFlag)
{
    int iCommandNum;

    for (iCommandNum = 0; iCommandNum < m_iNumMagicCommands; iCommandNum++)
    {
        MAGIC_COMMAND_STRUCT *pCommand = &m_MagicCommands[iCommandNum];

        if (pCommand->iCommandFlags & iFlag)
        {
            return true;
        }
    }

    return false;

}


bool MagicCommandManager::CommandIsInCategory(MAGIC_COMMAND_STRUCT *pCommand, char *pCategoryName)
{
    int i;

    if (_stricmp(pCategoryName, "all") == 0)
    {
        return (pCommand->iCommandFlags & COMMAND_FLAG_HIDE) == 0;
    }

    if (_stricmp(pCategoryName, "hidden") == 0)
    {
        return (pCommand->iCommandFlags & COMMAND_FLAG_HIDE) != 0;
    }

    if (pCategoryName[0] == 0)
    {
        return pCommand->commandCategories[0][0] == 0 && (pCommand->iCommandFlags & COMMAND_FLAG_HIDE) == 0;;
    }


    for (i=0; i < MAX_COMMAND_CATEGORIES; i++)
    {
        if (pCommand->commandCategories[i][0] == 0)
        {
            return false;
        }

        if (strcmp(pCommand->commandCategories[i], pCategoryName) == 0)
        {
            return true;
        }
    }

    return false;
}

bool MagicCommandManager::CommandVarIsInCategory(MAGIC_COMMANDVAR_STRUCT *pCommandVar, char *pCategoryName)
{
    int i;

    if (_stricmp(pCategoryName, "all") == 0)
    {
        return (pCommandVar->iCommandFlags & COMMAND_FLAG_HIDE) == 0;
    }

    if (_stricmp(pCategoryName, "hidden") == 0)
    {
        return (pCommandVar->iCommandFlags & COMMAND_FLAG_HIDE) != 0;
    }

    if (pCategoryName[0] == 0)
    {
        return pCommandVar->commandCategories[0][0] == 0;
    }


    for (i=0; i < MAX_COMMAND_CATEGORIES; i++)
    {
        if (pCommandVar->commandCategories[i][0] == 0)
        {
            return false;
        }

        if (strcmp(pCommandVar->commandCategories[i], pCategoryName) == 0)
        {
            return true;
        }
    }

    return false;
}

void MagicCommandManager::AddCommandToSet(MAGIC_COMMAND_STRUCT *pCommand, char *pSetName, Tokenizer *pTokenizer)
{
    int i;

    for (i=0; i < MAX_COMMAND_SETS; i++)
    {
        if (pCommand->commandSets[i][0] == 0)
        {
            strcpy(pCommand->commandSets[i], pSetName);
            break;
        }
    }

    pTokenizer->Assert(i < MAX_COMMAND_SETS, "Too many sets for one command");

    if (pCommand->commandCategories[0][0] == 0)
    {
        AddCommandToCategory(pCommand, pSetName, pTokenizer);
    }
}


void MagicCommandManager::AddCommandToCategory(MAGIC_COMMAND_STRUCT *pCommand, char *pCategoryName, Tokenizer *pTokenizer)
{
    int i;

    for (i=0; i < MAX_COMMAND_CATEGORIES; i++)
    {
        if (pCommand->commandCategories[i][0] == 0)
        {
            strcpy(pCommand->commandCategories[i], pCategoryName);
            MakeStringLowercase(pCommand->commandCategories[i]);
            break;
        }
    }

    pTokenizer->Assert(i < MAX_COMMAND_CATEGORIES, "Too many categories for one command");

}


void MagicCommandManager::AddCommandVarToSet(MAGIC_COMMANDVAR_STRUCT *pCommandVar, char *pSetName, Tokenizer *pTokenizer)
{
    int i;

    for (i=0; i < MAX_COMMAND_SETS; i++)
    {
        if (pCommandVar->commandSets[i][0] == 0)
        {
            strcpy(pCommandVar->commandSets[i], pSetName);
            break;
        }
    }

    pTokenizer->Assert(i < MAX_COMMAND_SETS, "Too many sets for one command");

    if (pCommandVar->commandCategories[0][0] == 0)
    {
        AddCommandVarToCategory(pCommandVar, pSetName, pTokenizer);
    }
}


void MagicCommandManager::AddCommandVarToCategory(MAGIC_COMMANDVAR_STRUCT *pCommandVar, char *pCategoryName, Tokenizer *pTokenizer)
{
    int i;

    for (i=0; i < MAX_COMMAND_CATEGORIES; i++)
    {
        if (pCommandVar->commandCategories[i][0] == 0)
        {
            strcpy(pCommandVar->commandCategories[i], pCategoryName);
            MakeStringLowercase(pCommandVar->commandCategories[i]);
            break;
        }
    }

    pTokenizer->Assert(i < MAX_COMMAND_CATEGORIES, "Too many categories for one command");

}

bool MagicCommandManager::CurrentProjectIsTestClient(void)
{
    if (strstr(m_ProjectName, "TestClient"))
    {
        return true;
    }

    return false;
}

void MagicCommandManager::AssertCommandIsOKForClientToTestClient(MAGIC_COMMAND_STRUCT *pCommand)
{
    CommandAssert(pCommand, pCommand->eReturnType == ARGTYPE_NONE, "Functions with non-void return can not be AUTO_COMMANDS on the TestClient");

    int i;

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        CommandAssert(pCommand, pCommand->argTypes[i] < ARGTYPE_EXPR_FIRST, "TestClient AUTO_COMMANDs can't have special arg types");
    }
}

void MagicCommandManager::WriteOutIfDefForIgnoringClientToTestClientWrapper(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    fprintf(pOutFile, "#define TestClientCmd_%s(", pCommand->commandName);
    int i;

    for (i=0; i < pCommand->iNumArgs; i++)
    {
        fprintf(pOutFile, "%s%s", i == 0 ? "" : ", ", pCommand->argNames[i]);
    }

    fprintf(pOutFile, ")\n");
}


    
void MagicCommandManager::WriteOutPrototypeForClientToTestClientWrapper(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    WriteOutGenericExternsAndPrototypesForCommand(pOutFile, pCommand, false);

    fprintf(pOutFile, "void TestClientCmd_%s(", pCommand->safeCommandName);
    WriteOutGenericArgListForCommand(pOutFile, pCommand, false, false);

    fprintf(pOutFile, ")");
}

void MagicCommandManager::WriteOutBodyForClientToTestClientWrapper(FILE *pOutFile, MAGIC_COMMAND_STRUCT *pCommand)
{
    WriteOutPrototypeForClientToTestClientWrapper(pOutFile, pCommand);

    fprintf(pOutFile, "\n{\n\tNetLink *pTestClientLink = gclGetLinkToTestClient();\n\tif (pTestClientLink)\n\t{\n\t\tPacket *pPak = pktCreate(pTestClientLink, TO_TESTCLIENT_CMD_COMMAND);\n");
    fprintf(pOutFile, "\t\tchar *pAUTOGENWorkString;\n\t\testrStackCreate(&pAUTOGENWorkString, 1024);\n");
    fprintf(pOutFile, "\t\testrCopy2(&pAUTOGENWorkString, \"%s \");\n", pCommand->commandName);
    
    WriteOutGenericCodeToPutArgumentsIntoEString(pOutFile, pCommand, "pAUTOGENWorkString", false);
    
    fprintf(pOutFile, "\t\tpktSendString(pPak, pAUTOGENWorkString);\n\t\tpktSend(&pPak);\n");

    fprintf(pOutFile, "\t\testrDestroy(&pAUTOGENWorkString);\n\t}\n}\n\n");
}
