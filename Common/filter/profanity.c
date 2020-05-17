/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <string.h>
#include <ctype.h>

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>

#if SERVER
#include "filter/reserved_names.h"
#endif

static StashTable s_hashFilth;
static char ** ppCopyright;

/**********************************************************************func*
 * replace
 *
 */
void replace(char *pch, char *set, char ch)
{
    while((pch=strpbrk(pch, set))!=NULL)
    {
        *pch++ = ch;
    }
}

/**********************************************************************func*
 * De1337
 *
 * Works in-place too, but will truncate the original string to iLen.
 *
 */
static void De1337(char *pchDest, const char *pchSrc, int iLen)
{
    strncpy_s(pchDest, iLen+1, pchSrc, iLen);
    pchDest[iLen] = '\0';

    // Process some 1337-speek stuff into text
    replace(pchDest, "0",  'o');
    replace(pchDest, "1",  'l');
    replace(pchDest, "2",  'z');
    replace(pchDest, "3",  'e');
    replace(pchDest, "4@", 'a');
    replace(pchDest, "5$", 's');
    replace(pchDest, "7",  't');
    replace(pchDest, "|",  'i');
}

/**********************************************************************func*
 * xorA5
 *
 * Works in-place too, but will truncate the original string to iLen.
 *
 */
static void xorA5(char *pchDest, const char *pchSrc, int iLen)
{
    while(*pchSrc!='\0' && iLen>0)
    {
        *pchDest = tolower(*pchSrc) ^ 0xA5;

        pchDest++;
        pchSrc++;
        iLen--;
    }
    *pchDest = '\0';
}

/**********************************************************************func*
 * IsProfanity
 *
 */
int IsProfanity(const char *pch, int iLen)
{
    int i;
    char *pchBuff = _alloca(iLen+1);

    // The naughty words are stored in the hash table xorA5'd so cursory
    // examination of memory won't show a load of profanity.

    xorA5(pchBuff, pch, iLen);
    if(s_hashFilth && stashFindInt(s_hashFilth, pchBuff, &i))
    {
        return true;
    }

    // Also try it with "leet" letters switched
    De1337(pchBuff, pch, iLen);
    xorA5(pchBuff, pchBuff, iLen);
    if(s_hashFilth && stashFindInt(s_hashFilth, pchBuff, &i))
    {
        return true;
    }

    return false;
}

/**********************************************************************func*
* IsCopyright
*
*/
char* IsCopyright(const char *pch)
{
    int i;
     for( i = eaSize(&ppCopyright)-1; i >= 0; i-- )
    {
        if( strlen(ppCopyright[i]) <= 5 ) // if the word is short require an exact match
        {
            if(stricmp(pch, ppCopyright[i])==0)
                return ppCopyright[i];
        }
        else if( strstri((char*)pch, ppCopyright[i]) )
            return ppCopyright[i];

    }
    return 0;
}

/**********************************************************************func*
 * IsAnyWordProfane
 *
 */
char * IsAnyWordProfane(const char *pch)
{
    int l = (int)strlen(pch);
    static char * pchReturn;
    char *pchCur;
    char *pchBuff = _alloca(l + 1);
    char *context = NULL;

    strcpy_s(pchBuff, l + 1, pch);

    pchCur=strtok_s(pchBuff, "-_?.,! \n\t", &context);
    while(pchCur!=NULL)
    {
        if(IsProfanity(pchCur, (int)strlen(pchCur)))
        {
            SAFE_FREE(pchReturn)
            pchReturn = strdup(pchCur);
            return pchReturn;
        }

        pchCur=strtok_s(NULL, "-_?.,! \n\t", &context);
    }
    return 0;
}

/**********************************************************************func*
* IsAnyWordProfaneOrCopyright
*
*/
char * IsAnyWordProfaneOrCopyright(const char *pch)
{
    int l = (int)strlen(pch);
    static char *pchReturn;
    char *pchCur;
    char *pchBuff = _alloca(l + 1);
    char *pchCopyright = 0;
    char *context = NULL;

    strcpy_s(pchBuff, l + 1, pch);
 
    pchCopyright = IsCopyright(pch);
    if(pchCopyright)
        return pchCopyright;

    pchCur=strtok_s(pchBuff, "-_?.,! ", &context);
    while(pchCur!=NULL)
    {
        if(IsProfanity(pchCur, (int)strlen(pchCur)))
        {
            SAFE_FREE(pchReturn)
            pchReturn = strdup(pchCur);
            return pchReturn;
        }
        pchCur=strtok_s(NULL, "-_?.,! ", &context);
    }
    return 0;
}

/**********************************************************************func*
* IsAnyWordProfane
*
*/
int ReplaceAnyWordProfane(char *pch)
{
    char * start = pch;

    while( start && *start )
    {
        int len = (int)strcspn(start, "<>-_?.,! \n\t");
        char * token = _alloca(len+1);

        strncpy_s( token, len+1, start, len );

        if(IsProfanity(token, (int)strlen(token)))
        {
            int i;
            for( i = 0; i < (int)len; i++ )
                start[i] = '*';
        }

        if( *(start + len) )
            start += len+1;
        else
            return 0;
    }
    return 0;
}

/**********************************************************************func*
 * LoadProfanity
 *
 */
void LoadProfanity(void)
{
    char pchPath[MAX_PATH];
    int len;
    char *mem;
    char *s;
    char *walk;
    int locale;
    char *context = NULL;

    if(!s_hashFilth)
    {
        s_hashFilth = stashTableCreateWithStringKeys(1000, StashDeepCopyKeys);
    }

    locale = locGetIDInRegistry();

    strcpy(pchPath, "/texts/");

    strcat(pchPath, locGetName(locale));

    strcat(pchPath, "/");
    strcat(pchPath, "cebsnar.txt"); // "profane" rot13'd
    writeConsole(OUTPUT_DEBUG, "\t%s", pchPath);

    walk = mem = fileAlloc(pchPath, &len);

    if (walk)
    {
        while (*walk=='\r' || *walk=='\n') walk++;
    }

    while (walk!=NULL && walk < mem + len)
    {
        s = walk;
        walk = strchr(s, '\r');

        if (!walk) break;

        while (*walk=='\r' || *walk=='\n') walk++;

        s = strtok_s(s, " \t\r\n", &context);
        if(s && strncmp(s, "//", 2)!=0)
        {
            xorA5(s, s, (int)strlen(s));
            stashAddPointer(s_hashFilth, s, NULL, false);
        }
    }
    fileFree(mem);

    // copy paste
    strcpy(pchPath, "/texts/");
    strcat(pchPath, locGetName(locale));
    strcat(pchPath, "/");
    strcat(pchPath, "copyright.txt"); // "profane" rot13'd
    writeConsole(OUTPUT_DEBUG, "\t%s", pchPath);

    walk = mem = fileAlloc(pchPath, &len);

    if (walk)
    {
        while (*walk=='\r' || *walk=='\n') walk++;
    }

    while (walk!=NULL && walk < mem + len)
    {
        s = walk;
        walk = strchr(s, '\r');

        if (!walk) break;

        while (*walk=='\r' || *walk=='\n') walk++;

        s = strtok_s(s, "\r\n", &context);
        if(s && strncmp(s, "//", 2)!=0)
        {
#if SERVER
            addReservedName(s);
#endif
            eaPush( &ppCopyright, strdup(s) );
        }
    }
    fileFree(mem);

}


// chatserver gets profane words from mapservers
void receiveProfanity(Packet * pak)
{        
    if(s_hashFilth)
        stashTableDestroy(s_hashFilth);

    s_hashFilth = receiveHashTable(pak, StashDeepCopyKeys);

    printf("Received %d profane words\n", stashGetValidElementCount(s_hashFilth));
}
void sendProfanity(Packet * pak)
{
    sendHashTable(pak, s_hashFilth);
}


/* End of File */
