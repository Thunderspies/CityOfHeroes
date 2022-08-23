/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <string.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/components/StashTable.h>

#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/language/AppLocale.h>
#include "filter/profanity.h"

#include "entity/entity.h"
#include "entity/costume.h"
#include "seq/seq.h"


static char **s_NPCNamesMale;
static char **s_NPCNamesFemale;

/**********************************************************************func*
 * ReadNameFile
 *
 */
static void ReadNameFile(char *pch, char ***pearray)
{
    int len;
    char *mem = fileAlloc(pch, &len);
    char *s, *walk=mem;
    int pos=0;

    if (walk) {
        while (*walk=='\r' || *walk=='\n') walk++;
    }

    while (walk && walk < mem + len) {
        s = walk;
        walk = strchr(s, '\r');
        if (!walk) break;
        while (*walk=='\r' || *walk=='\n') walk++;
        s = strtok(s, " \t\r\n");
        if (s && strncmp(s, "//", 2)!=0)
        {
            if(!IsProfanity(s, (int)strlen(s)))
            {
                eaPush(pearray, s);
            }
        }
    }

    // Don't free this, the memory is referenced in the EArray now.
    //  to free it later, probably have to either store the pointer or
    //  free(pearray[0])
    //fileFree(mem);
}


/**********************************************************************func*
 * npcLoadNames
 *
 */
void npcLoadNames(void)
{
    char pchPath[MAX_PATH];
    char pch[MAX_PATH];

    eaCreate(&s_NPCNamesMale);
    eaCreate(&s_NPCNamesFemale);

    strcpy(pchPath, "/texts/");
    strcat(pchPath, locGetName(locGetIDInRegistry()));
    strcat(pchPath, "/server/names/");

    strcpy(pch, pchPath);
    strcat(pch, "male.txt");
    ReadNameFile(pch, &s_NPCNamesMale);

    strcpy(pch, pchPath);
    strcat(pch, "female.txt");
    ReadNameFile(pch, &s_NPCNamesFemale);
}

/**********************************************************************func*
 * npcAssignName
 *
 */
void npcAssignName(Entity *e)
{
    float fRand = (float)rand()/(float)RAND_MAX;

    if( e->seq->type->randomNameFile )
    {
        static StashTable table;
        char **s_NPCNames = 0;
        StashElement element;

        if( !table )
            table = stashTableCreateWithStringKeys(10, StashDeepCopyKeys );
        
        stashFindElement(table,e->seq->type->randomNameFile, &element);
        if( element )
        {
            s_NPCNames = stashElementGetPointer(element);
        }
        else
        {
            char pchPath[MAX_PATH];
            char pch[MAX_PATH];

            strcpy(pchPath, "/texts/");
            strcat(pchPath, locGetName(locGetIDInRegistry()));
            strcat(pchPath, "/server/names/");

            strcpy(pch, pchPath);
            strcat(pch, e->seq->type->randomNameFile);

            ReadNameFile(pch, &s_NPCNames);
            stashAddPointer( table, e->seq->type->randomNameFile, s_NPCNames, false);
        }

        if(eaSize(&s_NPCNames))
        {
            int i = 0;
            int idx = (int)(fRand*(eaSize(&s_NPCNames)-1));
            strcpy(e->name, s_NPCNames[idx]);
            while(e->name[i] != '\0') //remove underscores
            {
                if( e->name[i] == '_' )
                    e->name[i] = ' ';
                i++;
            }
        }
    }
    else
    {
        if(e->costume->appearance.bodytype == kBodyType_BasicFemale
            || e->costume->appearance.bodytype == kBodyType_Female)
        {
            if(eaSize(&s_NPCNamesFemale))
            {
                int idx = (int)(fRand*(eaSize(&s_NPCNamesFemale)-1));
                strcpy(e->name, s_NPCNamesFemale[idx]);
            }
        }
        else
        {
            if(eaSize(&s_NPCNamesMale))
            {
                int idx = (int)(fRand*(eaSize(&s_NPCNamesMale)-1));
                strcpy(e->name, s_NPCNamesMale[idx]);
            }
        }
    }
}

/* End of File */
