
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/mathutil.h>

#include "entity/powers.h"
#include "bases/DetailRecipe.h"
#include "entity/salvage.h"
#include "entity/classes.h"

#include "UI/uiInfo.h"

#include "entity/entity.h"
#include "player/player.h"
#include "entity/character_base.h"


StashTable stTextLinkPowers;
StashTable stTextLinkRecipies;
StashTable stTextLinkSalvage;


static void addPower( const BasePower * ppow, const CharacterClass *pClass )
{
    char pow_name[200];

    if( ppow->ppchActivateRequires )
    {    
        int i;
        for(i=0;i<eaSize(&ppow->ppchActivateRequires);i++)
            if( stricmp(ppow->ppchActivateRequires[i], "accesslevel") == 0 )
                return;
    }

    if( ppow->ppchBuyRequires )
    {    
        int i;
        for(i=0;i<eaSize(&ppow->ppchBuyRequires);i++)
            if( stricmp(ppow->ppchBuyRequires[i], "accesslevel") == 0 )
                return;
    }

    if (pClass)
        snprintf(pow_name, sizeof(pow_name), "%s.%s", textStd(pClass->pchDisplayName), textStd(ppow->pchDisplayName));
    else
        strcpy_s(pow_name, sizeof(pow_name), textStd(ppow->pchDisplayName));

    if(!stashAddPointerConst(stTextLinkPowers, pow_name, ppow, 0) && !pClass )
    {
        PowerCategory *pCat;
        BasePowerSet *pSet;
        BasePower *pPow;
        int iLevel;

        if(powerdict_GetBasePtrsByString( &g_PowerDictionary, ppow->pchFullName, &pCat, &pSet, &pPow, &iLevel ))
        {
            char *str = estrTemp();
            estrConcatf( &str, "%s.%s", textStd(pSet->pchDisplayName), pow_name);

            // Duplicate Power Name, add FullPath (probably just need to add the set name)
            if(!stashAddPointerConst(stTextLinkPowers, str, ppow, 0))
            {
                estrClear(&str);
                estrConcatf( &str, "%s.%s.%s", textStd(pCat->pchName), textStd(pSet->pchDisplayName), pow_name );
                if(!stashAddPointerConst(stTextLinkPowers, str, ppow, 0))
                {
                    // oh come on - start using internal names
                    estrClear(&str);
                    estrConcatf( &str, "%s.%s.%s", pCat->pchName, textStd(pSet->pchDisplayName), pow_name );
                    if(!stashAddPointerConst(stTextLinkPowers, str, ppow, 0))
                    {
                        estrClear(&str);
                        estrConcatf( &str, "%s.%s.%s", pCat->pchName, pSet->pchName, pow_name );
                        if(!stashAddPointerConst(stTextLinkPowers, str, ppow, 0))
                        {

                            estrClear(&str);
                            estrConcatf( &str, "%s.%s.%s", pCat->pchName, pSet->pchName, ppow->pchName );
                            if(!stashAddPointerConst(stTextLinkPowers, str, ppow, 0))
                            {
                                int wtf = 0;    
                            }
                        }
                    }
                }
            }
            estrDestroy(&str);
        }
    }
}

void init_TextLinkStash(void)
{
    int i;
    char *str = estrTemp();

    stTextLinkRecipies = stashTableCreateWithStringKeys(2*eaSize(&g_DetailRecipeDict.ppRecipes), StashDeepCopyKeys);
    stTextLinkPowers = stashTableCreateWithStringKeys(2*eaSize(&g_PowerDictionary.ppPowers), StashDeepCopyKeys);
    stTextLinkSalvage = stashTableCreateWithStringKeys(2*eaSize(&g_SalvageDict.ppSalvageItems), StashDeepCopyKeys);

    //Recipes
    for(i=eaSize(&g_DetailRecipeDict.ppRecipes)-1; i>=0; i--)
    {
        const DetailRecipe *pRecipe = g_DetailRecipeDict.ppRecipes[i];
        estrClear(&str);
        estrConcatf( &str, "%s.%i", textStd(pRecipe->ui.pchDisplayName), pRecipe->level );
        stashAddPointerConst(stTextLinkRecipies,  textStd(pRecipe->ui.pchDisplayName), pRecipe, 0); // add string without level
        if(!stashAddPointerConst(stTextLinkRecipies, str, pRecipe, 0) ) 
        {
            // Duplicate Recipe Name, add internal name
            if(!stashAddPointerConst(stTextLinkRecipies, pRecipe->pchName, pRecipe, 0))
            {
                // really? dangit
            }
        }
    }

    // Powers

    // Do Character Powers first
    for( i = 0; i < eaSize(&g_CharacterClasses.ppClasses); i++ )
    {
        const CharacterClass *pClass = g_CharacterClasses.ppClasses[i];
        int j, k, m;

        for(j=0; j<kCategory_Count; j++)
        {
            const PowerCategory *pCat = pClass->pcat[j];
            if(pCat)
            {
                for(k=0; k<eaSize(&pCat->ppPowerSets); k++ )
                {
                    const BasePowerSet *pSet = pCat->ppPowerSets[k];
                    for( m = 0; m < eaSize(&pSet->ppPowers); m++ )
                    {
                        addPower( pSet->ppPowers[m], pClass);
                        addPower( pSet->ppPowers[m], 0);
                    }
                }
            }
        }
    }

    // now do all powers
    for(i=0; i<eaSize(&g_PowerDictionary.ppPowers); i++)
        addPower( g_PowerDictionary.ppPowers[i], 0);

    // Salvage
    for(i=eaSize(&g_SalvageDict.ppSalvageItems)-1; i>=0; i--)
    {
        const SalvageItem *pSalvage = g_SalvageDict.ppSalvageItems[i];
        if(!stashAddPointerConst(stTextLinkSalvage, textStd(pSalvage->ui.pchDisplayName), pSalvage, 0) )
        {
            // Duplicate Recipe Name, add internal name
            stashAddPointerConst(stTextLinkSalvage, pSalvage->pchName, pSalvage, 0);
        }
    }

    estrDestroy(&str);
}


bool isValidLinkToken( char * pchName )
{
    if( stashFindPointer(stTextLinkPowers, pchName, 0) ||
        stashFindPointer(stTextLinkRecipies, pchName, 0) ||
        stashFindPointer(stTextLinkSalvage, pchName, 0) )
    {
        return 1;
    }
    else
    {
        if( strnicmp( pchName, textStd("PowerColonEx"), strlen(textStd("PowerColonEx"))) == 0 )
        {
            pchName += strlen(textStd("PowerColonEx"));
        }
        else if( strnicmp( pchName, textStd("RecipeColon"), strlen(textStd("RecipeColon"))) == 0 )
        {
            pchName += strlen(textStd("RecipeColon"));
        }
        else if( strnicmp( pchName, textStd("SalvageColon"), strlen(textStd("SalvageColon"))) == 0 )
        {
            pchName += strlen(textStd("SalvageColon"));
        }

        if( stashFindPointer(stTextLinkPowers, pchName, 0) ||
            stashFindPointer(stTextLinkRecipies, pchName, 0) ||
            stashFindPointer(stTextLinkSalvage, pchName, 0) )
        {
            return 1;
        }
        else // maybe there is level grafted onto end
        {
            char * pchLevel = pchName + strlen(pchName)-1;
            while(pchLevel > pchName)
            {
                if(*pchLevel=='.')
                {
                    *pchLevel = '\0';
                    break;
                }
                pchLevel--;
            }

            if( stashFindPointer(stTextLinkPowers, pchName, 0) )
            {
                if(pchLevel > pchName)
                    *pchLevel = '.';
                return 1;
            }
            if(pchLevel > pchName)
                *pchLevel = '.';
        }
    }
    return 0;
}

static char *player_class_name(void)
{
    Entity *e;
    const CharacterClass *pClass;

    e = playerPtr();
    pClass = e->pchar->pclass;
    return textStd(pClass->pchDisplayName);
}

void textLink( char * pchName )
{
    BasePower *ppow;
    DetailRecipe *pRecipe;
    SalvageItem *pSalvage;
    bool bPowerOnly = 0, bRecipeOnly = 0, bSalavageOnly = 0;
    char at_pow_name[200];

    // If there is a cross-table conflict, a pre-pend may have been added
    if( strnicmp( pchName, textStd("PowerColonEx"), strlen(textStd("PowerColonEx"))) == 0 )
    {
        bPowerOnly = 1;
        pchName += strlen(textStd("PowerColonEx"));
    }
    else if( strnicmp( pchName, textStd("RecipeColon"), strlen(textStd("RecipeColon"))) == 0 )
    {
        bRecipeOnly = 1;
        pchName += strlen(textStd("RecipeColon"));
    }
    else if( strnicmp( pchName, textStd("SalvageColon"), strlen(textStd("SalvageColon"))) == 0 )
    {
        bSalavageOnly = 1;
        pchName += strlen(textStd("SalvageColon"));
    }
    
    if( !bSalavageOnly && !bPowerOnly)
    {
        if( stashFindPointer(stTextLinkRecipies, pchName, &pRecipe) )
        {
            info_Recipes(pRecipe);
            return;
        }
    }

    if( !bSalavageOnly && !bRecipeOnly )
    {
        snprintf(at_pow_name, sizeof(at_pow_name), "%s.%s", player_class_name(), pchName );
        if( stashFindPointer(stTextLinkPowers, at_pow_name, &ppow) )
        {
            if( ppow->eType == kPowerType_Boost )
            {
                int iLevel = 50;
                info_CombineSpec(ppow,0,iLevel-1, 0);
            }
            else
                info_Power(ppow,0);
            return;
        } 
        else if( stashFindPointer(stTextLinkPowers, pchName, &ppow) )
        {
            if( ppow->eType == kPowerType_Boost )
            {
                int iLevel = 50;
                info_CombineSpec(ppow,0,iLevel-1, 0);
            }
            else
                info_Power(ppow,0);
            return;
        }
        else // maybe there is level grafted onto end
        {
            int iLevel = -1;
            char * pchLevel = pchName + strlen(pchName)-1;
            while(pchLevel > pchName)
            {
                if(*pchLevel=='.')
                {
                    iLevel = atoi(pchLevel+1);
                    *pchLevel = '\0';
                    break;
                }
                pchLevel--;
            }

            if( stashFindPointer(stTextLinkPowers, pchName, &ppow) )
            {
                if( ppow->eType == kPowerType_Boost )
                    info_CombineSpec(ppow,0,iLevel-1, 0);
                else
                    info_Power(ppow,0);
                return;
            }
        }
    }

    if( !bPowerOnly && !bRecipeOnly )
    {
        if( stashFindPointer(stTextLinkSalvage, pchName, &pSalvage) )
        {
            info_Salvage(pSalvage);
            return;
        }
    }
}


char *getTextLinkForRecipe(DetailRecipe *pRecipe)
{
    DetailRecipe *pStashed=0;
    static char *str = NULL;
    char * name = estrTemp();

    if(!pRecipe)
        return NULL;
    estrConcatf(&name, "%s.%i", textStd(pRecipe->ui.pchDisplayName), pRecipe->level );

    stashFindPointer(stTextLinkRecipies, name, &pStashed);
    if(pStashed)
    {
        estrClear(&str);
        estrConcatf(&str, "[%s]", name );
        estrDestroy(&name);
        return str;
    }
    estrDestroy(&name);

    stashFindPointer(stTextLinkRecipies, pRecipe->pchName, &pStashed);
    if(pStashed && pStashed == pRecipe )
    {
        estrClear(&str);
        estrConcatf(&str, "[%s]", pRecipe->pchName );
        return str;
    }

    return textStd(pRecipe->ui.pchDisplayName); // failed
}

char *getTextLinkForPower(BasePower *ppow, int iLevel)
{
    static char *str = NULL;
    BasePower *pStashed=0;
 
    if(!ppow) 
        return NULL;

    estrClear(&str);
    estrConcatChar(&str, '[');

    // first check recipe stash, if our power is in there then we'll need to prepend
    if( stashFindPointer(stTextLinkRecipies, textStd(ppow->pchDisplayName), 0) )
        estrConcatCharString(&str, textStd("PowerColonEx"));

    stashFindPointer(stTextLinkPowers, textStd(ppow->pchDisplayName), &pStashed );
    if(pStashed && pStashed == ppow )
    {
        estrConcatCharString(&str, textStd(ppow->pchDisplayName));
        if( ppow->eType == kPowerType_Boost )
            estrConcatf(&str, ".%i",iLevel+1 );
        estrConcatChar(&str, ']');
        return str;
    }
    else // must be duplicate name, add more stuff
    {
        PowerCategory *pCat;
        BasePowerSet *pSet;
        BasePower *pPow;
        char *name = estrTemp();

        powerdict_GetBasePtrsByString( &g_PowerDictionary, ppow->pchFullName, &pCat, &pSet, &pPow, &iLevel );

        estrConcatf( &name, "%s.%s", textStd(pSet->pchDisplayName), textStd(ppow->pchDisplayName) );
        stashFindPointer(stTextLinkPowers, name, &pStashed);
        if(pStashed && pStashed == ppow )
        {
            estrConcatCharString(&str, name);
            if( ppow->eType == kPowerType_Boost )
                estrConcatf(&str, ".%i", iLevel+1);
            estrConcatChar(&str, ']');
            estrDestroy(&name);
            return str;
        }
        else
        {
            estrClear(&name);
            estrConcatf( &name, "%s.%s.%s", textStd(pCat->pchDisplayName), textStd(pSet->pchDisplayName), textStd(ppow->pchDisplayName) );
            stashFindPointer(stTextLinkPowers, name, &pStashed);
            if(pStashed && pStashed == ppow )
            {
                estrConcatCharString(&str, name);
                if( ppow->eType == kPowerType_Boost )
                    estrConcatf(&str, ".%i",iLevel+1 );
                estrConcatChar(&str, ']');
                estrDestroy(&name);
                return str;
            }
            else
            {
                estrClear(&name);
                estrConcatf( &name, "%s.%s.%s", pCat->pchName, textStd(pSet->pchDisplayName), textStd(ppow->pchDisplayName) );
                stashFindPointer(stTextLinkPowers, name, &pStashed);
                if(pStashed && pStashed == ppow )
                {
                    estrConcatCharString(&str, name);
                    if( ppow->eType == kPowerType_Boost )
                        estrConcatf(&str, ".%i",iLevel+1 );
                    estrConcatChar(&str, ']');
                    estrDestroy(&name);
                    return str;
                }
                else
                {
                    estrClear(&name);
                    estrConcatf( &name, "%s.%s.%s", pCat->pchName, pSet->pchName, textStd(ppow->pchDisplayName) );
                    stashFindPointer(stTextLinkPowers, name, &pStashed);
                    if(pStashed && pStashed == ppow )
                    {
                        estrConcatCharString(&str, name);
                        if( ppow->eType == kPowerType_Boost )
                            estrConcatf(&str, ".%i",iLevel+1 );
                        estrConcatChar(&str, ']');
                        estrDestroy(&name);
                        return str;
                    }
                    else
                    {
                        estrClear(&name);
                        estrConcatf( &name, "%s.%s.%s", pCat->pchName, pSet->pchName, ppow->pchName );
                        stashFindPointer(stTextLinkPowers, name, &pStashed);
                        if(pStashed && pStashed == ppow )
                        {
                            estrConcatCharString(&str, name);
                            if( ppow->eType == kPowerType_Boost )
                                estrConcatf(&str, ".%i",iLevel+1 );
                            estrConcatChar(&str, ']');
                            estrDestroy(&name);
                            return str;
                        }
                    }
                }
            }
        }
        estrDestroy(&name);
    }

    return textStd(ppow->pchDisplayName); // we totally failed, just return the display name
}

char *getTextLinkForSalvage(SalvageItem *pSalvage)
{
    static char *str = NULL;
    SalvageItem *pStashed=0;

    if(!pSalvage)
        return NULL;

    estrClear(&str);
    estrConcatChar(&str, '[');

    // first check recipe stash, if our salvage is in there then we'll need to prepend
    if( stashFindPointer(stTextLinkRecipies, textStd(pSalvage->ui.pchDisplayName), 0) || 
        stashFindPointer(stTextLinkPowers, textStd(pSalvage->ui.pchDisplayName), 0) )
    {
        estrConcatCharString(&str, textStd("SalvageColon"));
    }

    stashFindPointer(stTextLinkSalvage, textStd(pSalvage->ui.pchDisplayName), &pStashed);
    if( pStashed && pStashed == pSalvage)
    {
        estrConcatf( &str, "%s]", textStd(pSalvage->ui.pchDisplayName) );
        return str;
    }

    // try internal name
    estrClear(&str);
    estrConcatChar(&str, '[');
    if( stashFindPointer(stTextLinkRecipies, pSalvage->pchName, 0) || 
        stashFindPointer(stTextLinkPowers, pSalvage->pchName, 0) )
    {
        estrConcatf( &str, "%s]", textStd("SalvageColon") );
    }

    stashFindPointer(stTextLinkSalvage, pSalvage->pchName, &pStashed);
    if( pStashed && pStashed == pSalvage)
    {
        estrConcatf( &str, "%s]", textStd(pSalvage->pchName) );
        return str;
    }

    // failed just return the regular display name
    return  textStd(pSalvage->ui.pchDisplayName);
}