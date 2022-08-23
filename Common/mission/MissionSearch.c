
#include <utilitieslib/stdtypes.h>
#include "mission/MissionSearch.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/utils/file.h>

#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/UtilsNew/ncstd.h>

#include "AutoGen/MissionSearch_h_ast.c"

int g_missionsearchpage_accesslevel[] = {
    0, // MISSIONSEARCH_ALL
    0, // MISSIONSEARCH_MYARCS
    0, // MISSIONSEARCH_ARC_AUTHOR
    1, // MISSIONSEARCH_CSR_AUTHORED
    1, // MISSIONSEARCH_CSR_COMPLAINED
    1, // MISSIONSEARCH_CSR_VOTED,
    1, // MISSIONSEARCH_CSR_PLAYED,
};
STATIC_ASSERT(ARRAY_SIZE(g_missionsearchpage_accesslevel) == MISSIONSEARCH_PAGETYPES);


//returns 0 on not substring
#define MSSEARCH_NOT_SUBSTRING(value) (!value)
//returns index if there's a token in token list that is a substring of the token passed
#define MSSEARCH_LIST_IS_SUBSTRING(value) (value>0)
//returns -1 if token was a substring of an element in the list
#define MSSEARCH_SUBSTRING_OF_LIST(value) (value==-1)
int missionsearch_isSubstrToken(char *token, char ***tokenList, int *insertPosition)
{
    int listSize = eaSize(tokenList);
    int found = 0;
    int i;
    unsigned int tokenLength = strlen(token);
    for(i = 0; i < listSize && MSSEARCH_NOT_SUBSTRING(found); i++)
    {
        //equality or substring?
        if(strlen((*tokenList)[i]) < tokenLength)
        {
            found = (!!strstr(token, (*tokenList)[i]))?(i+1):0;    //list has a substring of this token
        }
        else
            found = (!!strstr((*tokenList)[i], token))?-1:0;//token is a substring of the list

        //alphabetical position--usually don't care if this is a substring
        if(!found && insertPosition != 0 && *insertPosition <0)
        {
            if(strcmpi((*tokenList)[i], token)>0)
            {
                *insertPosition = i;
            }
        }
    }
    return found;
}

void missionsearch_getUniqueTokens(char **estring, char ***tokens, int allowQuoted, int alphabetize)
{
    int length;
    char *token = 0;
    static char **tempStrings = 0;
    int i;
    if(!estring || !(length = estrLength(estring)))
        return;

    eaSetSize(&tempStrings, 0);
    strarray_quoted(estring, " ", " ", &tempStrings, allowQuoted);
    length = eaSize(&tempStrings);

    for(i = 0; i < length; i++)
    {
        int found = 0;
        int insertHere = -1;
        token = tempStrings[i];
        found = missionsearch_isSubstrToken(token, tokens, (alphabetize && insertHere <0)?&insertHere:0);
        if(MSSEARCH_LIST_IS_SUBSTRING(found))
        {
            found--;    //get the index, not the number.
            eaRemove(tokens, found);
            eaInsert(tokens, token, found);
        }
        if(MSSEARCH_NOT_SUBSTRING(found))
        {
            if(insertHere <0)
                eaPush(tokens, token);
            else
                eaInsert(tokens, token, insertHere);
        }
    }
}

//this function makes me intensely sad.  There must be a better way to do this without sacrificing performance by using
//a function pointer... =(
void missionsearch_stripNonSearchCharacters(char **estring, int length, MissionSearchCharactersAllowed rule, int allowQuotes)
{
    int i;
    switch(rule)
    {
        xcase MSSEARCHCHARS_ALHA_NUMERIC:
            for(i = 0; i < length; i++)
            {
                if(!ch_isalnum((*estring)[i]) && (!allowQuotes || (*estring)[i] !='"'))
                    (*estring)[i] = ' ';
                else
                    (*estring)[i] = tolower((*estring)[i]);
            }
        xcase MSSEARCHCHARS_ALHA_NUMERIC_PUNCTUATION:
            for(i = 0; i < length; i++)
            {
                if(!ch_isalnum((*estring)[i]) && !ch_ispunct((*estring)[i]) && (!allowQuotes || (*estring)[i] !='"'))
                    (*estring)[i] = ' ';
                else
                    (*estring)[i] = tolower((*estring)[i]);
            }
        xcase MSSEARCHCHARS_NONSPACE:
            for(i = 0; i < length; i++)
            {
                if(!ch_isgraph((*estring)[i]) || (!allowQuotes && (*estring)[i] !='"'))
                    (*estring)[i] = ' ';
                else
                    (*estring)[i] = tolower((*estring)[i]);
            }
        xcase MSSEARCHCHARS_ALL:
            for(i = 0; i < length; i++)
            {
                if(!allowQuotes && (*estring)[i] =='"')
                    (*estring)[i] = ' ';
                else
                    (*estring)[i] = tolower((*estring)[i]);
            }
        xdefault:
            devassert(0 && "No valid character rule passed to missionsearch_stripNonSearchCharacters");
            //do MSSEARCHCHARS_ALHA_NUMERIC
            for(i = 0; i < length; i++)
            {
                if(!ch_isalnum((*estring)[i]) && (!allowQuotes || (*estring)[i] !='"'))
                    (*estring)[i] = ' ';
                else
                    (*estring)[i] = tolower((*estring)[i]);
            }
    }
}

void missionsearch_stripNonMatchingQuotes(char *estring)
{
    char *quoteLoc = 0;
    int numberOfQuotes = 0;
    char *lastQuoteLoc = 0;
    quoteLoc = strchr(estring, '"');
    while(quoteLoc)
    {
        numberOfQuotes++;
        lastQuoteLoc = quoteLoc;
        quoteLoc = strchr(quoteLoc+1, '"');
    }
    if(numberOfQuotes%2)
        lastQuoteLoc[0] = ' ';
}


void missionsearch_formatSearchString(char **estring, int allowQuoted)
{
    int length;
    int i;
    static char **tokens = 0;
    char *position = 0;
    char *temp;
    
    if(!estring || !(length = estrLength(estring)))
        return;
    allowQuoted = allowQuoted || MSSEARCH_CHARACTER_RULE == MSSEARCHCHARS_ALL;
    missionsearch_stripNonSearchCharacters(estring, length, MSSEARCH_CHARACTER_RULE, allowQuoted);
    if(allowQuoted)
        missionsearch_stripNonMatchingQuotes(*estring);

    temp = estrTemp();
    estrPrintEString(&temp, *estring);
    missionsearch_getUniqueTokens(&temp, &tokens, allowQuoted, 1);
    
    //recreate the string
    estrClear(estring);
    for(i = 0; i< eaSize(&tokens); i++)
    {
        if(i > 0)
            estrConcatChar(estring, ' ');
        estrConcatCharString(estring, tokens[i]);
    }

    estrDestroy(&temp);
    eaSetSize(&tokens, 0);
}

int isValidForFilename(char c)
{
    return    c != '/' && c != '\\' && c != ':' && c != '*' && c != '?' && c != '"' && c != '<' && c != '>' && c != '|';
}

char *getMissionPath(char * filename)
{
    static char buff[FILENAME_MAX];

    char name[MAX_PATH];
    int i, len, len2;
    if(!filename)
        return NULL;
    len = strlen(filename);
    len2 = strlen(missionMakerExt());
    if(0==stricmp(name+len-len2, missionMakerExt())) // don't copy the extension, if it's already given
        len -= len2;
    for(i = 0; i < len; i++)
        name[i] = isValidForFilename(filename[i]) ? filename[i] : '_';
    name[i] = '\0';

    sprintf(buff, "%s/%s%s", missionMakerPath(), name, missionMakerExt());
    return buff;
}

// TODO: This should be the current working directory, not executable path
char* missionMakerPath(void)
{
    static char path[MAX_PATH];

    if(!path[0])
    {
        if(isDevelopmentMode())
        {
            char *s;
            strcpy(path, fileDataDir());
            if(s = strrchr(path, '/'))
                *s = '\0';
        }
        else
        {
            getExecutableDir(path);
        }
        strcat(path, "/Missions");
    }

    return path;
}

char* missionMakerExt(void)
{
    return ".storyarc";
}