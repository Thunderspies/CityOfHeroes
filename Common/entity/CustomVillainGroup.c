#include "entity/CustomVillainGroup.h"
#include "entity/costume.h"                        //    for custom critter costume
#include <utilitieslib/components/earray.h>                            //    for earrays
#include "entity/PCC_Critter.h"                    //    for custom critters
#include "gameComm/VillainDef.h"                        //    for ev validation
#include <utilitieslib/language/MessageStoreUtil.h>                //    for textStd
#include "storyarc/playerCreatedStoryarcValidate.h"    //    for validating villains
#include "filter/validate_name.h"            //    for name validation
#include "filter/profanity.h"
#include <utilitieslib/utils/utils.h>
#include "gameComm/Npc.h"
#include <utilitieslib/components/EString.h>

CustomVG **g_CustomVillainGroups;


TokenizerParseInfo parse_ExistingVillain[] = 
{
    {    "LevelIdx",        TOK_INT(CVG_ExistingVillain, levelIdx, 0)            },
    {    "CostumeIdx",    TOK_STRING(CVG_ExistingVillain, costumeIdx, 0)            },
    {    "Name",            TOK_STRING(CVG_ExistingVillain, pchName, 0)                },
    {    "Set",            TOK_INT(CVG_ExistingVillain, setNum, 0)            },
    {    "DisplayName",    TOK_STRING(CVG_ExistingVillain, displayName, 0)    },
    {    "Description",    TOK_STRING(CVG_ExistingVillain, description, 0) },
    {    "Costume",        TOK_OPTIONALSTRUCT(CVG_ExistingVillain, npcCostume, ParseCustomNPCCostume)    },
    {    "{",            TOK_START,    0 },                
    {    "}",            TOK_END,        0 },
    {    "", 0, 0 }
};
TokenizerParseInfo parse_CustomVG[] = 
{
    {    "DisplayName",        TOK_STRING(CustomVG, displayName, 0)    },
    {    "ExistingVillains",    TOK_STRUCT(CustomVG, existingVillainList, parse_ExistingVillain)    },
    {    "DontAutoSpawnEV",    TOK_STRINGARRAY(CustomVG, dontAutoSpawnEVs)            },
    {    "CustomVillains",    TOK_STRUCT(CustomVG, customVillainList, parse_PCC_Critter )            },
    {    "DontAutoSpawnPCC",    TOK_STRINGARRAY(CustomVG, dontAutoSpawnPCCs)                    },
    {    "SourceFile",    TOK_CURRENTFILE(CustomVG,sourceFile)                            },
    {    "{",            TOK_START,    0 },                
    {    "}",            TOK_END,        0 },
    {    "", 0, 0 }
};

TokenizerParseInfo parse_CompressedCVG[] = 
{
    {    "ReferenceName",    TOK_STRING(CompressedCVG, displayName, 0)    },
    {    "CustomCrittedIdx",    TOK_INTARRAY(CompressedCVG, customCritterIdx)    },
    {    "DontAutoSpawnPCC",    TOK_STRINGARRAY(CompressedCVG, dontAutoSpawnPCCs)                    },
    {    "ExistingVillains",    TOK_STRUCT(CompressedCVG, existingVillainList, parse_ExistingVillain)    },
    {    "DontAutoSpawnEV",    TOK_STRINGARRAY(CompressedCVG, dontAutoSpawnEVs)            },
    {    "{",        TOK_START, 0 },
    {    "}",        TOK_END,    0},
    {    "", 0, 0 }
};

int compareCritterGroups(const void* aData, const void* bData) {
    const CustomVG ** a = (const CustomVG **)aData;
    const CustomVG ** b = (const CustomVG **)bData; return  stricmp( (*a)->displayName, (*b)->displayName ); }
int CVG_isValidRank(int rank, int isCritter)
{
    if (isCritter)
    {
        return 1;
    }
    else
    {
        switch (rank)
        {
        case VR_SMALL:
        case VR_MINION:
        case VR_LIEUTENANT:
        case VR_SNIPER:
        case VR_BOSS:
        case VR_ELITE:
        case VR_PET:
        case VR_ARCHVILLAIN:
        case VR_ARCHVILLAIN2:
            return 1;
        default:
            return 0;
        }
    }
}
int cvg_randomVillainDefRankMatch(int rank)
{
    switch (rank)
    {
    case VR_SMALL:
    case VR_MINION:
        return 1;
    case VR_SNIPER:
    case VR_LIEUTENANT:
        return 2;
    case VR_BOSS:
    case VR_PET:
        return 3;
    }
    return 0;
}
static int validEV(Entity *e, CVG_ExistingVillain *ev)
{
    int isMinion = 0, isLt = 0, isBoss = 0;
    if (!ev || !ev->pchName)
    {
        return 0;
    }
    if (ev->displayName)
    {
        if (ValidateCustomObjectName(ev->displayName, 50, 1) != VNR_Valid)
        {
            return 0;
        }
    }
    if (ev->description)
    {
        if (playerCreatedStoryarc_basicSMFValidate(NULL, ev->description, 1010) || (IsAnyWordProfane(ev->description)) )
        {
            return 0;
        }
    }
    if ( ev->costumeIdx && (    (isMinion=(stricmp(ev->costumeIdx, "RandomMinion") == 0)) || 
                                (isLt=(stricmp(ev->costumeIdx, "RandomLt") == 0)) || 
                                (isBoss=(stricmp(ev->costumeIdx, "RandomBoss") == 0 ))) )
    {
        MMVillainGroup * pGroup = NULL;
        if (ev->pchName)
        {
            pGroup = playerCreatedStoryArc_GetVillainGroup( ev->pchName, ev->setNum );
        }
        if (pGroup && (pGroup->vg > VG_NONE) && (pGroup->vg < VG_MAX))
        {
            if (playerCreatedStoryarc_isUnlockedContent(villainGroupGetName(pGroup->vg), e))
            {
                int i;
                for (i = 0; i < eaSize(&villainDefList.villainDefs); ++i)
                {
                    if (villainDefList.villainDefs[i]->group == pGroup->vg)
                    {
                        switch (cvg_randomVillainDefRankMatch(villainDefList.villainDefs[i]->rank))
                        {
                        case 1:
                            if (isMinion)    
                                return 1;
                            break;
                        case 2:
                            if (isLt)        
                                return 1;
                            break;
                        case 3:
                            if (isBoss)        
                                return 1;
                            break;
                        }
                    }
                }
            }
        }
        return 0;
    }
    else
    {
        const char *villainGroupName;
        MMVillainGroup *villainGroup;
        const VillainDef *def;
        int i;
        int foundMatchInGroup = 0;
        def = villainFindByName(ev->pchName);
        if (!def)
        {
            return 0;    //    def doesn't exist
        }
        //    check that the group they belong to, is unlocked
        villainGroupName = villainGroupGetName(def->group);
        if (!playerCreatedStoryarc_isUnlockedContent(villainGroupName, e))
        {
            return 0;
        }
        if ( !CVG_isValidRank(def->rank, 0) )
        {
            return 0;    //    invalid rank (i.e. hamidon)
        }
        if ( !playerCreatedStoryarc_isValidVillainDefName(ev->pchName, e))
        {
            return 0;    //    isn't a valid villain (i.e. a door)
        }
        villainGroup = playerCreatedStoryArc_GetVillainGroup(villainGroupName, 0);
        if (!villainGroup)
        {
            return 0;
        }

        //    check if they are in set 0 level ranges
        if (!ev->setNum)
        {
            if ((def->levels[0]->level <= villainGroup->maxLevel) && (def->levels[eaSize(&def->levels)-1]->level >= villainGroup->minLevel ) &&
                playerCreatedStoryArc_isValidVillain( villainGroupName, def->name, 0 ) )
            {
                foundMatchInGroup = 1;
            }
            for (i = 0; i < eaSize(&villainGroup->ppGroup); ++i)
            {
                if ((def->levels[0]->level <= villainGroup->ppGroup[i]->maxLevel) 
                    && (def->levels[eaSize(&def->levels)-1]->level >= villainGroup->ppGroup[i]->minLevel ) &&
                    playerCreatedStoryArc_isValidVillain( villainGroupName, def->name, 0 ) )
                {
                    ev->setNum = i+1;
                    foundMatchInGroup = 1;
                }
            }
        }
        else
        {
            if (!EAINRANGE(ev->setNum-1, villainGroup->ppGroup))
            {
                return 0;
            }
            //    if not in set 0, maybe another set (same villain group)
            if ((def->levels[0]->level <= villainGroup->ppGroup[ev->setNum-1]->maxLevel) 
                && (def->levels[eaSize(&def->levels)-1]->level >= villainGroup->ppGroup[ev->setNum-1]->minLevel ) &&
                playerCreatedStoryArc_isValidVillain( villainGroupName, def->name, 0 ) )
            {
                foundMatchInGroup = 1;
            }
        }
        if (!foundMatchInGroup)
        {
            return 0;
        }
        if (ev->npcCostume)
        {
            if (!validateCustomNPCCostume(ev->npcCostume))
            {
                return 0;
            }
        }
        for (i = 0; i < eaSize(&def->levels); ++i)
        {
            if (ev->costumeIdx && def->levels[i]->costumes[0] && stricmp(textStd(def->levels[i]->costumes[0]), ev->costumeIdx) == 0)
            {
                return 1;
            }
        }
        return 0;    //    the def doesn't have this costume
    }
}
static int validateLevelRangeHoles(CustomVG *cvg)
{
    if (eaSize(&cvg->customVillainList))
    {
        int i;
        for (i = 0; i < eaSize(&cvg->customVillainList); ++i)
        {
            if ( (cvg->customVillainList[i]->rankIdx != PCC_RANK_CONTACT) && (StringArrayFind(cvg->dontAutoSpawnPCCs, cvg->customVillainList[i]->referenceFile) == -1) )
            {
                //    yay. custom critters are of all ranges. early exit.
                return CVG_LE_VALID;
            }
        }
    }
    {
        int i, startRange, endRange, isRangeHole;
        int levels[54];
        for (i = 0; i < 54; ++i)
        {
            levels[i] = 0;
        }
        for (i = 0; i < eaSize(&cvg->existingVillainList); ++i)
        {
            CVG_ExistingVillain *ev;
            ev = cvg->existingVillainList[i];
            if ( ev->costumeIdx && (    (stricmp(ev->costumeIdx, "RandomMinion") == 0) || 
                (stricmp(ev->costumeIdx, "RandomLt") == 0) || 
                (stricmp(ev->costumeIdx, "RandomBoss") == 0 ) ) )
            {
                MMVillainGroup * pGroup = NULL;
                if (ev->pchName)
                {
                    pGroup = playerCreatedStoryArc_GetVillainGroup( ev->pchName, ev->setNum );
                }
                if (pGroup && (pGroup->vg > VG_NONE) && (pGroup->vg < VG_MAX) )
                {
                    int j;
                    //    base 1 and inclusive
                    for (j = (pGroup->minLevel-1); j < pGroup->maxLevel; ++j)
                    {
                        levels[j] = 1;
                    }
                }
            }
            else
            {
                if (StringArrayFind(cvg->dontAutoSpawnEVs, ev->pchName) == -1)
                {
                    const VillainDef *def;
                    def = villainFindByName(ev->pchName);
                    if (def)
                    {
                        int j;
                        for (j = 0; j < eaSize(&def->levels); ++j)
                        {
                            levels[(def->levels[j]->level) - 1] = 1;
                        }
                    }
                }
            }
        }
        startRange = endRange = -1;
        isRangeHole = CVG_LE_VALID;
        for (i = 0; i < 54; ++i)
        {
            if ( (endRange >= 0) && levels[i])
            {
                isRangeHole = CVG_LE_LEVELRANGE_HOLES;
                break;
            }
            if (levels[i])
            {
                startRange = i;
            }
            if ((startRange >= 0) && (!levels[i]))
            {
                endRange = i;
            }
        }
        return isRangeHole;
    }
}
int CVG_isValidCVG(CustomVG *cvg, Entity *e, int fixup, int *playableErrorCode)
{
    int i;
    int retVal = CVG_LE_VALID;
    if (!cvg->displayName)
    {
        return CVG_LE_NONAME;
    }
    if (stricmp(cvg->displayName, textStd("NoneString")) == 0 )
    {
        return CVG_LE_INVALID_NAME;
    }
    if (ValidateCustomObjectName(cvg->displayName, 50, 0) != VNR_Valid)
    {
        retVal |= CVG_LE_PROFANITY;
    }
    if ( (eaSize(&cvg->existingVillainList) + eaSize(&cvg->customVillainList)) == 0 )
    {
        retVal |= CVG_LE_NO_CRITTERS;
    }
    else
    {
        int numSpawnableCritters = 0;
        char **internalNames = NULL;
        if (eaiSize(&cvg->badCritters))
        {
            eaiDestroy(&cvg->badCritters);
        }
        if (eaiSize(&cvg->badStandardCritters))
        {
            eaiDestroy(&cvg->badStandardCritters);
        }
        for (i = 0; i < eaSize(&cvg->customVillainList); i++)
        {
            int error = 0;
            int playableErrors = 0;
            if ((error = isPCCCritterValid(e, cvg->customVillainList[i],fixup,&playableErrors)) != PCC_LE_VALID)
            {
                //    todo: even if the critter is bad,
                //    still let the group through, just skip this one
                //    Server doesn't fixup here. It tags it as bad and fixes in the playerCreatedStoryArc_validate
                retVal |= CVG_LE_INVALID_CRITTER;
                eaiPush(&cvg->badCritters, i);
            }
            else
            {
                if(playableErrors)
                    *playableErrorCode |= CVG_LE_INVALID_CRITTER;;
                if ( (cvg->customVillainList[i]->rankIdx != PCC_RANK_CONTACT) && 
                    (StringArrayFind(cvg->dontAutoSpawnPCCs, cvg->customVillainList[i]->referenceFile) == -1 ) )
                {
                    numSpawnableCritters++;
                }
            }
        }
        for (i = 0; i < eaSize(&cvg->existingVillainList); i++)
        {
            if (!validEV(e, cvg->existingVillainList[i]))
            {
                if(playableErrorCode)
                    *playableErrorCode |= CVG_LE_INVALID_STANDARD;
                retVal |= CVG_LE_INVALID_STANDARD;
                eaiPush(&cvg->badStandardCritters, i);
                continue;
            }
            if (    (stricmp(cvg->existingVillainList[i]->costumeIdx, "RandomMinion") != 0) && 
                    (stricmp(cvg->existingVillainList[i]->costumeIdx, "RandomLt") != 0) && 
                    (stricmp(cvg->existingVillainList[i]->costumeIdx, "RandomBoss") != 0) )
            {
                int j;
                char *concatNameString = NULL;
                const VillainDef *vDef;
                vDef = villainFindByName(cvg->existingVillainList[i]->pchName);
                estrCreate(&concatNameString);
                estrConcatf(&concatNameString, "%s%s%s", cvg->existingVillainList[i]->pchName, cvg->existingVillainList[i]->costumeIdx,
                        cvg->existingVillainList[i]->displayName ? cvg->existingVillainList[i]->displayName : "" );
                for (j = 0; j < eaSize(&internalNames); ++j)
                {
                    //    loop through list of internal names, if it exists already
                    if (stricmp(internalNames[j], concatNameString) == 0)
                    {
                        //    this critter isn't unique by our definition
                        retVal |= CVG_LE_NON_UNIQUE_EXISTING_VILLAINS;
                        break;
                    }
                }
                eaPush(&internalNames, concatNameString);
                if (StringArrayFind(cvg->dontAutoSpawnEVs, cvg->existingVillainList[i]->pchName) == -1)
                {
                    numSpawnableCritters++;
                }
            }
            else
            {
                numSpawnableCritters++;
            }
        }
        if ( numSpawnableCritters == 0)
        {
            retVal |= CVG_LE_NO_CRITTERS;
        }
        for (i = (eaSize(&internalNames)-1); i >= 0; --i)
        {
            estrDestroy(&internalNames[i]);
        }
        eaDestroy(&internalNames);
    }
    retVal |= validateLevelRangeHoles(cvg);
    return retVal;
}
int CVG_CVGExists(char *cvgName)
{
    if (cvgName)
    {
        int i;
        for (i = 0; i < eaSize(&g_CustomVillainGroups); i++ )
        {
            if (stricmp(g_CustomVillainGroups[i]->displayName, cvgName) == 0)
            {
                return i;
            }
        }
    }
    return -1;
}

void CVG_clearVillains(CustomVG *cvg)
{
    int i;
    for (i = 0; i < eaSize(&cvg->existingVillainList); ++i)
    {
        StructDestroy(parse_ExistingVillain, cvg->existingVillainList[i]);
    }
    eaDestroy(&cvg->existingVillainList);
    eaDestroy(&cvg->customVillainList);
}
static void CVG_getLevelsFromEVList( CVG_ExistingVillain ***evList, int *minLevel, int *maxLevel, char **dontAutoSpawnList)
{
    int currentMin, currentMax, i;
    currentMin = 54;
    currentMax = 0;
    if (evList)
    {
        for (i = 0; i < eaSize(evList); i++)
        {
            CVG_ExistingVillain *ev;
            const VillainDef *def;
            int defMin, defMax;
            ev = (*evList)[i];
            defMin = 54;
            defMax = 0;
            if ( ev->costumeIdx && (    (stricmp(ev->costumeIdx, "RandomMinion") == 0) || 
                                (stricmp(ev->costumeIdx, "RandomLt") == 0) || 
                                (stricmp(ev->costumeIdx, "RandomBoss") == 0 ) ) )
            {
                MMVillainGroup * pGroup = NULL;
                if (ev->pchName)
                {
                    pGroup = playerCreatedStoryArc_GetVillainGroup( ev->pchName, ev->setNum );
                }
                if (pGroup)
                {
                    defMin = pGroup->minLevel;
                    defMax = pGroup->maxLevel;
                }
            }
            else
            {
                def = villainFindByName(ev->pchName);
                if (def && eaSize(&def->levels) && ( StringArrayFind(dontAutoSpawnList, ev->pchName) == -1) )
                {
                    //    assume that the levels are a continuous block of levels
                    defMin = def->levels[0]->level;
                    defMax = def->levels[eaSize(&def->levels)-1]->level;
                }
            }
            if (defMin < currentMin)
            {
                currentMin = defMin;
            }
            if (defMax > currentMax)
            {
                currentMax = defMax;
            }
        }
    }
    *minLevel = currentMin;
    *maxLevel = currentMax;
}
void CVG_getLevelsFromCompressedVG(CompressedCVG *cvg, int *minLevel, int *maxLevel, PCC_Critter **pccList)
{
    if (eaiSize(&cvg->customCritterIdx))
    {
        if (eaSize(&cvg->dontAutoSpawnPCCs) == 0)
        {
            //    yay. custom critters are of all ranges.
            //    exit early
            *minLevel = 1;
            *maxLevel = 54;
            return;
        }
        else
        {
            int i;
            for (i = 0; i < eaiSize(&cvg->customCritterIdx); ++i)
            {
                int index;
                index = cvg->customCritterIdx[i] - 1;
                if (EAINRANGE(index, pccList))
                {
                    if (StringArrayFind(cvg->dontAutoSpawnPCCs, pccList[index]->referenceFile) == -1)
                    {
                        *minLevel = 1;
                        *maxLevel = 54;
                    }
                }
            }
        }
    }
    else
    {
        CVG_getLevelsFromEVList(&(cvg->existingVillainList), minLevel, maxLevel, cvg->dontAutoSpawnEVs);
    }
}

void CVG_getLevelsFromUncompressedVG(CustomVG *cvg, int *minLevel, int *maxLevel)
{
    int itr;
    for (itr = 0; itr < eaSize(&cvg->customVillainList); itr++)
    {
        if ( (cvg->customVillainList[itr]->rankIdx != PCC_RANK_CONTACT) && (StringArrayFind(cvg->dontAutoSpawnPCCs, cvg->customVillainList[itr]->referenceFile) == -1 ) )
        {
            *minLevel = 1;
            *maxLevel = 54;
            return;
        }
    }
    CVG_getLevelsFromEVList(&(cvg->existingVillainList), minLevel, maxLevel, cvg->dontAutoSpawnEVs);
}