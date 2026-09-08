/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <string.h>

#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/language/AppLocale.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include <utilitieslib/components/earray.h>
#include "entity/character_base.h"
#include "entity/origins.h"
#include "filter/titles.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include "auth/authUserData.h"
#include "language/commonLangUtil.h"

StaticDefineInt ParseYesNo[] = {
    DEFINE_INT
    { "No",                        0                },
    { "Yes",                    1                },
    DEFINE_END
};


TokenizerParseInfo ParseTitleList[] =
{
    { "Origin",                TOK_STRUCTPARAM | TOK_STRING(TitleList, pchOrigin, 0)            },
    { "{",                    TOK_START,        0                                                        },
    { "Titles",                TOK_STRINGARRAY(TitleList, ppchTitles)                            },
    { "ChooseList",            TOK_STRINGARRAY(TitleList, ppchChooseList)                        },
    { "Gender",                TOK_INTARRAY(TitleList, piGenders),    ParseGender    },
    { "AttachArticle",        TOK_INTARRAY(TitleList, piAttach),    ParseYesNo    },
    { "ListGenders",        TOK_INTARRAY(TitleList, piListGenders),    ParseGender    },
    { "AdjectiveGenders",    TOK_INTARRAY(TitleList, piAdjectiveGenders),    ParseGender },
    { "}",                    TOK_END,            0                                                        },
    { "",                    0,                    0                                                        }
};

TokenizerParseInfo ParseTitleDictionary[] =
{
    { "Origin",        TOK_STRUCT(TitleDictionary, ppOrigins, ParseTitleList) },
    { "", 0, 0 }
};

TitleDictionary g_TitleDictionary;
TitleDictionary g_v_TitleDictionary;
TitleDictionary g_p_TitleDictionary;
TitleDictionary g_l_TitleDictionary;

static void LoadDictionary(TitleDictionary *pDict, char *filename)
{
    TokenizerHandle tok;
    int success = 0;
    int i;

    tok = TokenizerCreate(filename);
    if (!tok)
    {
        verbose_printf("Unable to load %s\n", filename);
        return;
    }
    success = TokenizerParseList(tok, ParseTitleDictionary, pDict, NULL);
    TokenizerDestroy(tok);
    if (!success)
    {
        verbose_printf("Error loading %s\n", filename);
    }

    for(i = 0; i < eaSize(&pDict->ppOrigins); i++)
    {
        TitleList *tlist = pDict->ppOrigins[i];
        if(!stricmp( "Base", tlist->pchOrigin ))
        {
            if(!(eaSize(&tlist->ppchTitles) == eaSize(&tlist->ppchChooseList) &&
                 eaSize(&tlist->ppchTitles) == eaiSize(&tlist->piAttach) &&
                 eaSize(&tlist->ppchTitles) == eaiSize(&tlist->piGenders)))
                ErrorFilenamef(filename, "Base section fields do not match in size");
        }
        else
        {
            if(!(eaSize(&tlist->ppchTitles) == eaSize(&tlist->ppchChooseList)) || !eaiSize(&tlist->piListGenders) ||
                (eaSize(&tlist->ppchTitles) % eaiSize(&tlist->piListGenders)))
                ErrorFilenamef(filename, "The length of all the ChooseLists and Titles have to match, and be a multiple of ListGenders");
        }
    }
}

/**********************************************************************func*
 * LoadTitleDictionaries
 *
 */
void LoadTitleDictionaries(void)
{
    char achBase[MAX_PATH];
    char achPath[MAX_PATH];
    strcpy(achBase, "/texts/");
    strcat(achBase, locGetName(getCurrentLocale()));
    strcat(achBase, "/");

    strcpy_s(achPath, MAX_PATH, achBase);
    strcat(achPath, "titles.def");
    LoadDictionary(&g_TitleDictionary, achPath);

    strcpy_s(achPath, MAX_PATH, achBase);
    strcat(achPath, "v_titles.def");
    LoadDictionary(&g_v_TitleDictionary, achPath);

    strcpy_s(achPath, MAX_PATH, achBase);
    strcat(achPath, "p_titles.def");
    LoadDictionary(&g_p_TitleDictionary, achPath);

    strcpy_s(achPath, MAX_PATH, achBase);
    strcat(achPath, "l_titles.def");
    LoadDictionary(&g_l_TitleDictionary, achPath);
}

void title_set( Entity *e, int showThe, int commonIdx, int originIdx, int color1, int color2 )
{
    int i, j, size;
    int    adjectiveGenders;

    TitleDictionary * pDict = &g_TitleDictionary;
    
    if( ENT_IS_IN_PRAETORIA(e) )
    {
        if( ENT_IS_VILLAIN(e) )
            pDict = &g_l_TitleDictionary;
        else
            pDict = &g_p_TitleDictionary;
    }
    else
    {
        if( ENT_IS_VILLAIN(e) )
            pDict = &g_v_TitleDictionary;
    }

    // always do this because a 0 actually might have a meaning in some locales (i.e. set the name_gender correctly)
    for( i = 0; i < eaSize(&pDict->ppOrigins); i++ )
    {
        if( stricmp( "Base", pDict->ppOrigins[i]->pchOrigin ) == 0 )
        {
            if( showThe >= 0 && showThe < eaSize(&pDict->ppOrigins[i]->ppchTitles) )
            {
                e->pl->attachArticle = pDict->ppOrigins[i]->piAttach[showThe];
                e->name_gender = pDict->ppOrigins[i]->piGenders[showThe];
                if (pDict->ppOrigins[i]->piAdjectiveGenders) {
                    adjectiveGenders = pDict->ppOrigins[i]->piAdjectiveGenders[showThe];
                } else {
                    adjectiveGenders = -1;
                }
                strncpyt( e->pl->titleTheText, pDict->ppOrigins[i]->ppchTitles[showThe], 10 );
            }
        }
    }

    if(e->pchar->iLevel < COMMON_TITLE_LEVEL || commonIdx <= 0)
        strcpy(e->pl->titleCommon, "");
    else
    {
        size = 0;
        for( i = eaSize(&pDict->ppOrigins)-1; i >=0; i-- )
        {
            if( stricmp( "Common", pDict->ppOrigins[i]->pchOrigin ) == 0 )
            {
                for(j = 0; j < eaiSize(&pDict->ppOrigins[i]->piListGenders); j++)
                {
                    if ((adjectiveGenders != -1 && pDict->ppOrigins[i]->piListGenders[j] == adjectiveGenders) ||
                        (adjectiveGenders == -1 && pDict->ppOrigins[i]->piListGenders[j] == e->name_gender))
                    {
                        size = eaSize(&pDict->ppOrigins[i]->ppchChooseList) / 
                            eaiSize(&pDict->ppOrigins[i]->piListGenders);
                        if( commonIdx > 0 && commonIdx < size )
                            strncpyt( e->pl->titleCommon, pDict->ppOrigins[i]->ppchTitles[j * size + commonIdx], 32 );
                    }
                }
            }
        }
        if(commonIdx >= size)
        {
            if (accountLoyaltyRewardHasNode(e->pl->loyaltyStatus, "BaseTeleporter"))
            {
                commonIdx -= size;
                size = 0;
                for( i = eaSize(&pDict->ppOrigins)-1; i >=0; i-- )
                {
                    if( stricmp( "Veteran", pDict->ppOrigins[i]->pchOrigin ) == 0 )
                    {
                        for(j = 0; j < eaiSize(&pDict->ppOrigins[i]->piListGenders); j++)
                        {
                            if ((adjectiveGenders != -1 && pDict->ppOrigins[i]->piListGenders[j] == adjectiveGenders) ||
                                (adjectiveGenders == -1 && pDict->ppOrigins[i]->piListGenders[j] == e->name_gender))
                            {
                                size = eaSize(&pDict->ppOrigins[i]->ppchChooseList) / 
                                    eaiSize(&pDict->ppOrigins[i]->piListGenders);
                                if( commonIdx > 0 && commonIdx < size )
                                    strncpyt( e->pl->titleCommon, pDict->ppOrigins[i]->ppchTitles[j * size + commonIdx], 32 );
                            }
                        }
                    }
                }
            }
            if(commonIdx >= size)
                strcpy(e->pl->titleCommon,"");
        }
    }

    if(e->pchar->iLevel < ORIGIN_TITLE_LEVEL  || originIdx <= 0)
        strcpy(e->pl->titleOrigin, "");
    else
    {
        size = 0;
        for( i = eaSize(&pDict->ppOrigins)-1; i >= 0; i-- )
        {
            if( stricmp( e->pchar->porigin->pchName, pDict->ppOrigins[i]->pchOrigin ) == 0 )
            {
                for(j = 0; j < eaiSize(&pDict->ppOrigins[i]->piListGenders); j++)
                {
                    if ((adjectiveGenders != -1 && pDict->ppOrigins[i]->piListGenders[j] == adjectiveGenders) ||
                        (adjectiveGenders == -1 && pDict->ppOrigins[i]->piListGenders[j] == e->name_gender))
                    {
                        size = eaSize(&pDict->ppOrigins[i]->ppchChooseList) / 
                            eaiSize(&pDict->ppOrigins[i]->piListGenders);
                        if( originIdx > 0 && originIdx < size )
                            strncpyt( e->pl->titleOrigin, pDict->ppOrigins[i]->ppchTitles[j * size + originIdx], 32 );
                    }
                }
            }
        }
        if(originIdx >= size)
            strcpy(e->pl->titleOrigin,"");
    }

    
    if(color1 && color2 && accountLoyaltyRewardHasNode(e->pl->loyaltyStatus, "ColoredTitles") )
    {
        e->pl->titleColors[0] = color1;
        e->pl->titleColors[1] = color2;
    }
    else
        e->pl->titleColors[0] = e->pl->titleColors[1] = 0;
}

/* End of File */
