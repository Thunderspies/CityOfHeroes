#include "UI/uiUtilMenu.h"
#include "uiPCCCreationNLM.h"
#include <utilitieslib/utils/SuperAssert.h>
#include <utilitieslib/components/earray.h>

static NonLinearMenu *NLM_PCCCreation = NULL;
extern NonLinearMenuElement PCC_Rank_NLME;    //    rank non linear menu element
extern NonLinearMenuElement PCC_powers_primary_NLME;
extern NonLinearMenuElement PCC_powers_secondary_NLME;
extern NonLinearMenuElement PCC_gender_NLME;
extern NonLinearMenuElement PCC_costume_NLME;
extern NonLinearMenuElement PCC_profile_NLME;

NonLinearMenu * NLM_PCCCreationMenu()
{
    if (!NLM_PCCCreation)
    {
        NLM_PCCCreation = NLM_allocNonLinearMenu();
        if (!NLM_PCCCreation)    return NULL;
        NLM_pushNewNLMElement(NLM_PCCCreation, &PCC_Rank_NLME);
        NLM_pushNewNLMElement(NLM_PCCCreation, &PCC_powers_primary_NLME);
        NLM_pushNewNLMElement(NLM_PCCCreation, &PCC_powers_secondary_NLME);
        NLM_pushNewNLMElement(NLM_PCCCreation, &PCC_gender_NLME);
        NLM_pushNewNLMElement(NLM_PCCCreation, &PCC_costume_NLME);
        NLM_pushNewNLMElement(NLM_PCCCreation, &PCC_profile_NLME);
    }
    return NLM_PCCCreation;
}