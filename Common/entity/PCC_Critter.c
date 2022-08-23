#include "entity/PCC_Critter.h"


#include "entity/character_base.h"
#include <utilitieslib/components/earray.h>
#include "entity/powers.h"
#include "gameData/BodyPart.h"
#include "entity/costume.h"
#include "entity/entity.h"
#include "player/player.h"
#include <utilitieslib/components/EString.h>
#include "varutils.h"                //    playerVarAlloc
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/origins.h"                //    for origins pointer
#include "entity/entPlayer.h"                //    costume ent player pointer
#include "filter/profanity.h"
#include "filter/validate_name.h"
#include "storyarc/playerCreatedStoryarcValidate.h"
#include "gameComm/VillainDef.h"

extern SHARED_MEMORY PowerDictionary g_PowerDictionary;

SHARED_MEMORY PCC_CritterRewardMods g_PCCRewardMods;

StaticDefineInt PCCRankEnum [] =
{
    DEFINE_INT
    {"Minion",        PCC_RANK_MINION            },
    {"Lieutenant",    PCC_RANK_LT                }, 
    {"Boss",        PCC_RANK_BOSS            },
    {"EliteBoss",    PCC_RANK_ELITE_BOSS        },
    {"ArchVillain",    PCC_RANK_ARCH_VILLAIN    },
    {"Contact",        PCC_RANK_CONTACT        },
    DEFINE_END
};
StaticDefineInt PCCDiffEnum [] =
{
    DEFINE_INT
    {"Standard", PCC_DIFF_STANDARD},
    {"Hard",PCC_DIFF_HARD}, 
    {"Extreme", PCC_DIFF_EXTREME},
    {"Custom",PCC_DIFF_CUSTOM},
    DEFINE_END
};

const PowerCategory *missionMakerPowerSet[3] = { NULL, NULL, NULL };
TokenizerParseInfo parse_PCC_Critter[] = 
{
    {    "ReferenceFile",TOK_STRING(PCC_Critter,referenceFile,0)                            },
    {    "Name",            TOK_STRING(PCC_Critter, name, 0)},
    {    "Description",   TOK_STRING(PCC_Critter, description, 0)},
    {    "VillainGroup",        TOK_STRING(PCC_Critter, villainGroup,0 )    },
    {    "PrimaryPower",        TOK_STRING(PCC_Critter, primaryPower, 0)        },
    {    "Difficulty",    TOK_INT(PCC_Critter, difficulty[0], 0), PCCDiffEnum },
    {    "SelectedPowers",    TOK_INT(PCC_Critter, selectedPowers[0], 0)    },
    {    "SecondaryPower",        TOK_STRING(PCC_Critter, secondaryPower, 0)        },
    {    "Difficulty2",    TOK_INT(PCC_Critter, difficulty[1], 0), PCCDiffEnum },
    {    "SelectedPowers2",    TOK_INT(PCC_Critter, selectedPowers[1], 0)    },
    {    "TravelPower",        TOK_STRING(PCC_Critter, travelPower, 0)        },
    {    "Rank",            TOK_STRING(PCC_Critter, legacyRankText, 0)    },
    {    "Designation",    TOK_INT(PCC_Critter, rankIdx, 0), PCCRankEnum        },
    {    "Ranged",        TOK_INT(PCC_Critter, isRanged, 0)        },
    {    "Costume",        TOK_OPTIONALSTRUCT(PCC_Critter, pccCostume, ParseCostume)        },
    {    "DiffCostume",    TOK_OPTIONALSTRUCT(PCC_Critter, compressedCostume, ParseCostumeDiff)    },
    {    "SourceFile",    TOK_CURRENTFILE(PCC_Critter,sourceFile)                            },
    {    "TimeStamp",    TOK_TIMESTAMP(PCC_Critter, timeAge),    },
    {    "{",            TOK_START,    0 },
    {    "}",            TOK_END,        0 },
    {    "", 0, 0 }
};

static void loadMissionMakerPowers()
{
    if (!missionMakerPowerSet[pcc_MenuPrimary])
    {
        missionMakerPowerSet[pcc_MenuPrimary] = powerdict_GetCategoryByName(&g_PowerDictionary, "Mission_Maker_Attacks");
        missionMakerPowerSet[pcc_MenuSecondary] = powerdict_GetCategoryByName(&g_PowerDictionary, "Mission_Maker_Secondary");
        missionMakerPowerSet[pcc_MenuTravel] = powerdict_GetCategoryByName(&g_PowerDictionary, "Mission_Maker_Movement");
    }
}
PCC_Critter * clone_PCC_Critter(PCC_Critter *source)
{
    PCC_Critter *dest;
    int returnValue;
    dest = StructAllocRaw(sizeof(PCC_Critter));
    returnValue = StructCopy(parse_PCC_Critter, source, dest, 0,0);
    assert(returnValue);
    return dest;
}

void destroy_PCC_Critter(PCC_Critter *pcc_critter)
{
    assert(pcc_critter);
    StructDestroy(parse_PCC_Critter, pcc_critter);
}

int getPCCRankFromText(char *text)
{
    if (!text)
    {
        return PCC_RANK_COUNT;
    }
    if (stricmp(text, textStd("PCCMinionTitle")) == 0)
    {
        return PCC_RANK_MINION;
    }
    if (stricmp(text, textStd("PCCLtTitle")) == 0)
    {
        return PCC_RANK_LT;
    }
    if (stricmp(text, textStd("PCCBossTitle")) == 0)
    {
        return PCC_RANK_BOSS;
    }
    if (stricmp(text, textStd("PCCEliteBossTitle")) == 0)
    {
        return PCC_RANK_ELITE_BOSS;
    }
    if (stricmp(text, textStd("PCCArchVillainTitle")) == 0)
    {
        return PCC_RANK_ARCH_VILLAIN;
    }
    if (stricmp(text, textStd("PCCContactTitle")) == 0)
    {
        return PCC_RANK_CONTACT;
    }
    return PCC_RANK_COUNT;
}

int getPCCDifficultyFromText(char *text)
{
    if (!text)
    {
        return PCC_DIFF_STANDARD;
    }
    if (stricmp(text, textStd("PCCStandardDifficultyTitle")) == 0 )
    {
        return PCC_DIFF_STANDARD;
    }
    if (stricmp(text, textStd("PCCHardDifficultyTitle")) == 0 )
    {
        return PCC_DIFF_HARD;
    }
    if (stricmp(text, textStd("PCCExtremeDifficultyTitle")) == 0 )
    {
        return PCC_DIFF_EXTREME;
    }
    if (stricmp(text, textStd("PCCCustomDifficultyTitle")) == 0 )
    {
        return PCC_DIFF_CUSTOM;
    }
    return PCC_DIFF_COUNT;
}
int getPCCPowerIndexFromPowersetName(const char *powerSetName, int category)
{
    int i;
    if ((category < pcc_MenuPrimary) || (category >= pcc_MenuCount))
    {
        return -1;
    }
    if (powerSetName)
    {
        if (stricmp(powerSetName, "none") == 0)
        {
            return -1;
        }
        loadMissionMakerPowers();
        for (i = 0; i < eaSize(&missionMakerPowerSet[category]->ppPowerSets); i++)
        {
            if ( missionMakerPowerSet[category]->ppPowerSets[i] && missionMakerPowerSet[category]->ppPowerSets[i]->pchFullName &&
                (stricmp(missionMakerPowerSet[category]->ppPowerSets[i]->pchFullName, powerSetName) == 0) )
            {
                return i;
            }
        }
    }
    return -1;    
}
const char *getPCCPowerDisplayNameFromPowersetName( const char *powerSetName, int category)
{
    if (powerSetName)
    {
        int i;
        if (stricmp(powerSetName, "none") == 0)
        {
            return "NoneString";
        }
        if (category == -1)
        {
            return textStd("Brawl");
        }
        if ((category < pcc_MenuPrimary) || (category >= pcc_MenuCount))
        {
            return "UnknownString";
        }
        loadMissionMakerPowers();
        for (i = 0; i < eaSize(&missionMakerPowerSet[category]->ppPowerSets); i++)
        {
            if ( missionMakerPowerSet[category]->ppPowerSets[i] && missionMakerPowerSet[category]->ppPowerSets[i]->pchFullName &&
                (stricmp(missionMakerPowerSet[category]->ppPowerSets[i]->pchFullName, powerSetName) == 0) )
            {
                return missionMakerPowerSet[category]->ppPowerSets[i]->pchDisplayName;
            }
        }
    }
    return "UnknownString";
}
static int areValidPCCPowers(PCC_Critter *pcc_critter, Entity *e)
{
    int i;
    int foundPower = 0;
    int powerSetMatches[3] = {-1, -1, -1};
    int retVal = PCC_LE_VALID;
    int villainConning;

    loadMissionMakerPowers();
    switch (pcc_critter->rankIdx)
    {
    case PCC_RANK_MINION:
        villainConning = villainRankConningAdjustment(VR_MINION);
        break;
    case PCC_RANK_LT:
        villainConning = villainRankConningAdjustment(VR_LIEUTENANT);
        break;
    case PCC_RANK_BOSS:
        villainConning = villainRankConningAdjustment(VR_BOSS);
        break;
    case PCC_RANK_ELITE_BOSS:
        villainConning = villainRankConningAdjustment(VR_ELITE);
        break;
    case PCC_RANK_ARCH_VILLAIN:
        villainConning = villainRankConningAdjustment(VR_ARCHVILLAIN);
        break;
    default:
        villainConning = villainRankConningAdjustment(VR_NONE);
        break;
    }

    for (i = 0; i < eaSize(&missionMakerPowerSet[pcc_MenuPrimary]->ppPowerSets); i++)
    {
        if (stricmp(missionMakerPowerSet[pcc_MenuPrimary]->ppPowerSets[i]->pchFullName, pcc_critter->primaryPower) == 0)
        {
            int size = eaSize(&missionMakerPowerSet[pcc_MenuPrimary]->ppPowerSets[i]->ppPowers);
            if (pcc_critter->difficulty[0] == PCC_DIFF_CUSTOM)
            {
                if (pcc_critter->selectedPowers[0] >= (1<<size) )
                {
                    retVal |= PCC_LE_INVALID_SELECTED_PRIMARY_POWERS;
                }
            }
            powerSetMatches[0] = i;
            foundPower = 1;
        }
    }
    if (!foundPower)
    {
        retVal |= PCC_LE_INVALID_PRIMARY_POWER;
    }
    foundPower = 0;
    for (i = 0; i < eaSize(&missionMakerPowerSet[pcc_MenuSecondary]->ppPowerSets); i++)
    {
        if (stricmp(missionMakerPowerSet[pcc_MenuSecondary]->ppPowerSets[i]->pchFullName, pcc_critter->secondaryPower) == 0)
        {
            if (pcc_critter->difficulty[1] == PCC_DIFF_CUSTOM)
            {
                int size = eaSize(&missionMakerPowerSet[pcc_MenuSecondary]->ppPowerSets[i]->ppPowers);
                if (pcc_critter->selectedPowers[1] >= (1<<size) )
                {
                    retVal |= PCC_LE_INVALID_SELECTED_SECONDARY_POWERS;
                }
            }
            powerSetMatches[1] = i;
            foundPower = 1;
        }
    }
    if (!foundPower)
    {
        retVal |= PCC_LE_INVALID_SECONDARY_POWER;
    }
    if (pcc_critter->travelPower && (stricmp(textStd("NoneString"), pcc_critter->travelPower) == 0 ) && ( stricmp(pcc_critter->travelPower, "none") != 0 ) )
    {
        StructFreeStringConst(pcc_critter->travelPower);
        pcc_critter->travelPower = StructAllocString("None");
    }
    if (stricmp("None", pcc_critter->travelPower) != 0)
    {
        foundPower = 0;
        for (i = 0; i < eaSize(&missionMakerPowerSet[pcc_MenuTravel]->ppPowerSets); i++)
        {
            if (stricmp(missionMakerPowerSet[pcc_MenuTravel]->ppPowerSets[i]->pchFullName, pcc_critter->travelPower) == 0)
            {
                powerSetMatches[2] = i;
                foundPower = 1;
            }
        }
        if (!foundPower)
        {
            retVal |= PCC_LE_INVALID_TRAVEL_POWER;
        }
    }
    //    If the powers are good up to here..
    //    make sure they can actually have the powers
    if (retVal == PCC_LE_VALID)
    {
        if (e)
        {
            int prevIlevel;
            int j;
            PowerSet*pset;
            const BasePowerSet *bset;
            //    buy primary power

            //    the check for valid critter powers depends on the character's iLevel.
            //    unfortunately, the iLevel is on the e->pchar, which is the player's iLevel.
            //    temporarily set it to 0 for critter checking
            if (e->pchar)
            {
                prevIlevel = e->pchar->iLevel;
                e->pchar->iLevel = 0;
            }

            bset = missionMakerPowerSet[pcc_MenuPrimary]->ppPowerSets[powerSetMatches[pcc_MenuPrimary]];
            pset = character_AddNewPowerSet(e->pchar, bset);
            for (j = 0; j < eaSize(&bset->ppPowers); ++j)
            {
                character_BuyPower(e->pchar, pset, bset->ppPowers[j], 0);
            }

            //    check if they can hypothetically have the power
            if (!character_IsAllowedToHavePowerSetHypothetically(e->pchar, 
                missionMakerPowerSet[pcc_MenuSecondary]->ppPowerSets[powerSetMatches[pcc_MenuSecondary]]))
            {
                retVal |= PCC_LE_CONFLICTING_POWERSETS;
            }

            //    remove primary power
            character_RemovePowerSet(e->pchar, bset, NULL);

            //    set the iLevel back to the proper level
            if (e->pchar)
            {
                e->pchar->iLevel = prevIlevel;
            }
        }
    }
    return retVal;
}

int isPCCCritterValid(Entity *e, PCC_Critter *pcc_critter, int fixup, int *playableErrorCode)
{
    int reasonForBad=PCC_LE_VALID;
    int bodyPartCount = GetBodyPartCount();

    if(playableErrorCode)
        *playableErrorCode = 0;

    if( !pcc_critter )
    {
        return PCC_LE_NULL;
    }

    if (pcc_critter->legacyRankText )
    {
        if (stricmp(pcc_critter->legacyRankText, "Minion") == 0)
            pcc_critter->rankIdx = PCC_RANK_MINION;
        else if (stricmp(pcc_critter->legacyRankText, "Lieutenant") == 0)
            pcc_critter->rankIdx = PCC_RANK_LT;
        else if (stricmp(pcc_critter->legacyRankText, "Boss") == 0)
            pcc_critter->rankIdx = PCC_RANK_BOSS;
        else if (stricmp(pcc_critter->legacyRankText, "Elite Boss") == 0)
            pcc_critter->rankIdx = PCC_RANK_ELITE_BOSS;
        else if (stricmp(pcc_critter->legacyRankText, "Arch Villain") == 0)
            pcc_critter->rankIdx = PCC_RANK_ARCH_VILLAIN;
        else if ( (stricmp(pcc_critter->legacyRankText, "Contact") == 0) || (stricmp(pcc_critter->legacyRankText, "Person") == 0 ) )
            pcc_critter->rankIdx = PCC_RANK_CONTACT;
        else
            pcc_critter->rankIdx = PCC_RANK_MINION;

        StructFreeString(pcc_critter->legacyRankText);
        pcc_critter->legacyRankText = NULL;
    }

    // name should be valid
    //    todo: may need more checks against special characters
    if( !pcc_critter->name || strlen(pcc_critter->name) == 0)
    {
        reasonForBad |= PCC_LE_INVALID_NAME;
    }
    else
    {
        if (ValidateCustomObjectName(pcc_critter->name, 50, 1) != VNR_Valid)
        {
            reasonForBad |= PCC_LE_PROFANITY_NAME;
            if (fixup)        StructReallocString(&pcc_critter->name, textStd("InvalidName"));
        }
    }
    // villain group should be valid
    //    todo: may need more checks against special characters
    if ( !pcc_critter->villainGroup || strlen(pcc_critter->villainGroup) == 0 || (stricmp(pcc_critter->villainGroup, textStd("NoneString")) == 0) )
    {
        reasonForBad |= PCC_LE_INVALID_VILLAIN_GROUP;
    }
    else
    {
        if (ValidateCustomObjectName(pcc_critter->villainGroup, 50, 0) != VNR_Valid)
        {
            reasonForBad |= PCC_LE_PROFANITY_VG;
            if (fixup)        StructReallocString(&pcc_critter->villainGroup, textStd("InvalidVillainGroup"));
        }
    }
    //    todo: may need more checks against special characters

    if (pcc_critter->description)
    {
        if (playerCreatedStoryarc_basicSMFValidate(NULL, pcc_critter->description, 1010))
        {
            reasonForBad |= PCC_LE_INVALID_DESC;
        }
        if (IsAnyWordProfane(pcc_critter->description))
        {
            reasonForBad |= PCC_LE_PROFANITY_DESC;
            if (fixup)        StructReallocString(&pcc_critter->description, textStd("InvalidDescription"));
        }
    }

    if ( (pcc_critter->difficulty[0] < PCC_DIFF_CUSTOM) || (pcc_critter->difficulty[0] >= PCC_DIFF_COUNT) )
        reasonForBad |= PCC_LE_INVALID_DIFFICULTY;

    if ( (pcc_critter->difficulty[1] < PCC_DIFF_CUSTOM) || (pcc_critter->difficulty[1] >= PCC_DIFF_COUNT) )
        reasonForBad |= PCC_LE_INVALID_DIFFICULTY2;

    // ranged should be valid
    if ((pcc_critter->isRanged != 1) && (pcc_critter->isRanged != 0))
    {
        //    someone was tampering with ranged
        reasonForBad |= PCC_LE_INVALID_RANGED;
    }

    //    validate the rank
    if ((pcc_critter->rankIdx >= PCC_RANK_COUNT) || (pcc_critter->rankIdx < PCC_RANK_MINION))
    {
        reasonForBad |= PCC_LE_INVALID_RANK;
    }
    //    we don't really need to check for contacts, because it doesn't matter. 
    //    The contacts are getting brawl no matter what.
    if (pcc_critter->rankIdx != PCC_RANK_CONTACT)
    {
        reasonForBad |= areValidPCCPowers(pcc_critter, e);    
    }
    else
    {
        if ( pcc_critter->primaryPower && (stricmp(pcc_critter->primaryPower, textStd("Brawl")) == 0) && (stricmp(pcc_critter->primaryPower, "Brawl") != 0) )
        {
            StructFreeString(pcc_critter->primaryPower);
            pcc_critter->primaryPower = StructAllocString("Brawl");
        }
        if (!pcc_critter->primaryPower || (stricmp(pcc_critter->primaryPower, "Brawl") != 0))
        {
            reasonForBad |= PCC_LE_INVALID_PRIMARY_POWER;
        }
        if ( pcc_critter->secondaryPower && (stricmp(pcc_critter->secondaryPower, textStd("NoneString")) == 0) && (stricmp(pcc_critter->secondaryPower, "None") != 0) )
        {
            StructFreeString(pcc_critter->secondaryPower);
            pcc_critter->secondaryPower = StructAllocString("None");
        }
        if (!pcc_critter->secondaryPower || (stricmp(pcc_critter->secondaryPower, "None") != 0))
        {
            if(fixup)
            {
                if(playableErrorCode)
                    *playableErrorCode |= PCC_LE_INVALID_SECONDARY_POWER;
                StructReallocString(&pcc_critter->secondaryPower, "None");
            }
            else
                reasonForBad |= PCC_LE_INVALID_SECONDARY_POWER;
        }
        if ( pcc_critter->travelPower && (stricmp(pcc_critter->travelPower, textStd("NoneString")) == 0) && (stricmp(pcc_critter->travelPower, "None") != 0) )
        {
            StructFreeStringConst(pcc_critter->travelPower);
            pcc_critter->travelPower = StructAllocString("None");
        }
        if (!pcc_critter->travelPower || (stricmp(pcc_critter->travelPower, "None") != 0))
        {
            if(fixup)
            {
                if(playableErrorCode)
                    *playableErrorCode |= PCC_LE_INVALID_TRAVEL_POWER;
                StructFreeStringConst(pcc_critter->travelPower);
                pcc_critter->travelPower = StructAllocString("None");
            }
            else
                reasonForBad |= PCC_LE_INVALID_TRAVEL_POWER;
        }
    }
    //    costume validation should not crash even without an entity
    //    validate that the critter is a male,female or huge
    if (pcc_critter->pccCostume)
    {
        switch (pcc_critter->pccCostume->appearance.bodytype)
        {
        case kBodyType_Huge:    //    Male, intentional fall through
        case kBodyType_Female: //    Female, intentional fall through
        case kBodyType_Male: //    Huge, intentional fall through
            break;
        default:
            reasonForBad |= PCC_LE_BAD_COSTUME;
            return reasonForBad;
        }
    }
    else
    {
        reasonForBad |= PCC_LE_BAD_COSTUME;
        return reasonForBad;
    }

    if (e)
    {
        int i;
        int critterPowerIndex[pcc_MenuCount];
        //    unaward player powers
        loadMissionMakerPowers();
        critterPowerIndex[pcc_MenuPrimary] = getPCCPowerIndexFromPowersetName(pcc_critter->primaryPower,pcc_MenuPrimary);
        critterPowerIndex[pcc_MenuSecondary] = getPCCPowerIndexFromPowersetName(pcc_critter->secondaryPower,pcc_MenuSecondary);
        critterPowerIndex[pcc_MenuTravel] = getPCCPowerIndexFromPowersetName(pcc_critter->travelPower,pcc_MenuTravel);
        //    award powerset parts from critter
        for (i = pcc_MenuPrimary; i < pcc_MenuCount; i++)
        {
            if (critterPowerIndex[i] >= 0)
            {
                costumeAwardPowerSetPartsFromPower(e, missionMakerPowerSet[i]->ppPowerSets[critterPowerIndex[i]], 0, 1);
            }
        }
    }
    //    Does not do complete validation without an entity. 
    //    Only checks if the part exists (not if they can equip it)
    if (eaiSize(&pcc_critter->badCostumeParts))
    {
        eaiDestroy(&pcc_critter->badCostumeParts);
    }
    if (pcc_critter->pccCostume->appearance.iNumParts < bodyPartCount)
    {
        Costume *costume = pcc_critter->pccCostume;
        int i = eaSize(&costume->parts);
        costume->appearance.iNumParts = bodyPartCount;
        while (i < bodyPartCount)
        {
            eaPush(&costume->parts, StructAllocRaw(sizeof(CostumePart)));
            costume_PartSetGeometry(costume, i, NULL);
            costume_PartSetTexture1(costume, i, NULL);
            costume_PartSetTexture2(costume, i, NULL);
            costume_PartSetFx(costume, i, NULL);
            i++;
        }
    }

    if (costume_Validate(e, pcc_critter->pccCostume, 0, &(pcc_critter->badCostumeParts), NULL, 1, false) != 0)
    {
        reasonForBad |= PCC_LE_BAD_COSTUME;
    }

    if (e)
    {
        int i;
        int critterPowerIndex[pcc_MenuCount];
        critterPowerIndex[pcc_MenuPrimary] = getPCCPowerIndexFromPowersetName(pcc_critter->primaryPower,pcc_MenuPrimary);
        critterPowerIndex[pcc_MenuSecondary] = getPCCPowerIndexFromPowersetName(pcc_critter->secondaryPower,pcc_MenuSecondary);
        critterPowerIndex[pcc_MenuTravel] = getPCCPowerIndexFromPowersetName(pcc_critter->travelPower,pcc_MenuTravel);

        //    unaward powerset parts from critter
        for (i = pcc_MenuPrimary; i < pcc_MenuCount; i++)
        {
            if ( EAINRANGE(critterPowerIndex[i], missionMakerPowerSet[i]->ppPowerSets) )
            {
                costumeUnawardPowerSetPartsFromPower(e, missionMakerPowerSet[i]->ppPowerSets[critterPowerIndex[i]], 1);
            }
        }
    }
    return reasonForBad;
}

void generatePCCErrorText_ex(int retVal, char **errorText)
{
    if (retVal & PCC_LE_NULL)            {    estrConcatf(errorText, "%s ", textStd("PCC_LE_NULL"));    }
    if (retVal & PCC_LE_NO_FILE)        {    estrConcatf(errorText, "%s ", textStd("PCC_LE_NO_FILE"));    }
    if (retVal & PCC_LE_BAD_PARSE)        {    estrConcatf(errorText, "%s ", textStd("PCC_LE_BAD_PARSE"));    }
    if (retVal & PCC_LE_INVALID_NAME)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_INVALID_NAME"));    }
    if (retVal & PCC_LE_INVALID_VILLAIN_GROUP)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_INVALID_VILLAIN_GROUP"));    }
    if (retVal & PCC_LE_INVALID_RANGED)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_INVALID_RANGED"));    }
    if (retVal & PCC_LE_INVALID_PRIMARY_POWER) {    estrConcatf(errorText, "%s ", textStd("PCC_LE_INVALID_PRIMARY_POWER"));    }
    if (retVal & PCC_LE_INVALID_DIFFICULTY)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_INVALID_DIFFICULTY"));    }
    if (retVal & PCC_LE_INVALID_SELECTED_PRIMARY_POWERS)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_INVALID_SELECTED_PRIMARY_POWERS"));    }
    if (retVal & PCC_LE_INVALID_SECONDARY_POWER)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_INVALID_SECONDARY_POWER"));    }
    if (retVal & PCC_LE_INVALID_DIFFICULTY2)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_INVALID_DIFFICULTY2"));    }
    if (retVal & PCC_LE_INVALID_SELECTED_SECONDARY_POWERS)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_INVALID_SELECTED_SECONDARY_POWERS"));    }
    if (retVal & PCC_LE_INVALID_TRAVEL_POWER)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_INVALID_TRAVEL_POWER"));    }
    if (retVal & PCC_LE_CONFLICTING_POWERSETS)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_CONFLICTING_POWERSETS")); }
    if (retVal & PCC_LE_BAD_COSTUME)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_BAD_COSTUME"));    }
    if (retVal & PCC_LE_INACCESSIBLE_COSTUME)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_INACCESSIBLE_COSTUME"));    }
    if (retVal & PCC_LE_INVALID_RANK)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_INVALID_RANK"));    }
    if (retVal & PCC_LE_FAILED_TO_REMOVE)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_FAILED_TO_REMOVE"));    }
    if (retVal & PCC_LE_PROFANITY_NAME)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_PROFANITY_NAME"));    }
    if (retVal & PCC_LE_PROFANITY_VG)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_PROFANITY_VG"));    }
    if (retVal & PCC_LE_PROFANITY_DESC)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_PROFANITY_DESC"));    }
    if (retVal & PCC_LE_INVALID_DESC)    {    estrConcatf(errorText, "%s ", textStd("PCC_LE_INVALID_DESC"));    }
}

