
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/origins.h"                //    for origins pointer

#include "entity/entPlayer.h"                //    costume ent player pointer

#include "entity/PCC_Critter.h"
#include "entity/PCC_Critter_Client.h"
#include "UI/uiCustomVillainGroupWindow.h"
#include "entity/costume.h"
#include "entity/entity.h"
#include "player/player.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/MemoryPool.h>

#include "varutils.h"                //    playerVarAlloc
#include "entity/character_base.h"
#include <utilitieslib/components/earray.h>

#include "entity/entclient.h"                //    for entcreate
#include "UI/uiSupercostume.h"            //    costume validation things
#include "UI/uiCostume.h"
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/components/StringCache.h>            //    for allocaddstring
#include "entity/powers.h"
#include "entity/CustomVillainGroup.h"
#include "entity/CustomVillainGroup_Client.h"
#include "gameData/BodyPart.h"
#include <utilitieslib/utils/FolderCache.h>
#include "UI/uiMissionMakerScrollSet.h"        //    for update custom critter list

static char *pcc_origin = "Villain_Origin";
static char *pcc_class = "Class_Lt_Grunt";

static PCC_Critter **s_PCCLoadDest;

static int s_PCC_count = 1;

static int compareCritters(const void* aData, const void* bData) {
    const PCC_Critter ** a = (const PCC_Critter **)aData;
    const PCC_Critter ** b = (const PCC_Critter **)bData; return  stricmp( (*a)->name, (*b)->name ); }
extern int currentlyBoughtCritterPowers[3];
extern int CVGroupIndex;
extern MMScrollSet missionMaker;
char * getCustomCritterDir(void)
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
        strcat(path, "/Custom_Critter");
    }
    return path;
}

PCC_Critter *getCustomCritterByReferenceFile( char * referenceFile)
{
    int i,j;
    if (!referenceFile)    
    {
        return NULL;
    }
    for ( i = 0; i < eaSize(&g_CustomVillainGroups); i++ )
    {
        for ( j = 0; j < eaSize(&g_CustomVillainGroups[i]->customVillainList); j++ )
        {
            if (g_CustomVillainGroups[i]->customVillainList[j]->referenceFile &&
                (stricmp(referenceFile, g_CustomVillainGroups[i]->customVillainList[j]->referenceFile) == 0 ))
            {
                return g_CustomVillainGroups[i]->customVillainList[j];
            }
        }

    }
    return NULL;
}
void removeStoryArcCritters()
{
    int i,j;
    for ( i = (eaSize(&g_CustomVillainGroups) -1); i >= 0; i--)
    {
        for (j = (eaSize(&g_CustomVillainGroups[i]->customVillainList)-1); j >= 0; j--)
        {
            if (g_CustomVillainGroups[i]->customVillainList[j]->sourceFile)
            {
                if (strstri(g_CustomVillainGroups[i]->customVillainList[j]->sourceFile, ".critter") != 0)
                {
                    //    if this is a critter, don't remove it
                    continue;
                }
                else if (stricmp(g_CustomVillainGroups[i]->customVillainList[j]->sourceFile, "none") != 0)
                {
                    //     couldn't sourceFile was not a .critter. remove it
                    StructDestroy(parse_PCC_Critter, g_CustomVillainGroups[i]->customVillainList[j]);
                    g_CustomVillainGroups[i]->customVillainList[j] = NULL;
                }
                else
                {
                    //    what is happening here?
                    Errorf("%s", "Sourcefile is none");
                }
            }
            //    invalid critter.. remove it
            eaRemove(&g_CustomVillainGroups[i]->customVillainList, j);
        }
    }
    CVG_removeAllEmptyCVG(g_CustomVillainGroups);
    updateCustomCritterList(&missionMaker);
}
void removeCustomCritter(char *sourceFile, int deleteSourceFile)
{
    int i,j;
    PCC_Critter *critterToRemove = NULL;
    for (i = (eaSize(&g_CustomVillainGroups)-1); i >= 0; i--)
    {
        for (j = (eaSize(&g_CustomVillainGroups[i]->customVillainList)-1); j >= 0; j--) 
        {
            if (stricmp(g_CustomVillainGroups[i]->customVillainList[j]->sourceFile, sourceFile) == 0 )
            {
                //    should only be one actual instance to critter
                //    everything else is a pointer to it
                //    so remove everything that points to it
                //    and then delete the actual critter
                if ( (stricmp(g_CustomVillainGroups[i]->customVillainList[j]->villainGroup, g_CustomVillainGroups[i]->displayName) != 0 ) && !deleteSourceFile )
                {
                    g_CustomVillainGroups[i]->reinsertRemovedCritter = 1;
                }
                else
                {
                    g_CustomVillainGroups[i]->reinsertRemovedCritter = 0;
                }
                critterToRemove = g_CustomVillainGroups[i]->customVillainList[j];                
                eaRemove(&g_CustomVillainGroups[i]->customVillainList, j);                
                if (g_CustomVillainGroups[i]->sourceFile && !g_CustomVillainGroups[i]->reinsertRemovedCritter)
                {
                    ParserWriteTextFile(g_CustomVillainGroups[i]->sourceFile, parse_CustomVG, g_CustomVillainGroups[i], 0,0);
                }
            }
        }
    }
    if (deleteSourceFile)
    {
        fileMoveToRecycleBin(sourceFile);
    }
    if (critterToRemove)
    {
        StructDestroy(parse_PCC_Critter, critterToRemove);
    }
    CVG_removeAllEmptyCVG(g_CustomVillainGroups);
    CVGroupIndex = -1;
}
static char * stripPath( char *sourceFile)
{
    if (sourceFile)
    {
        char *rf;
        rf = sourceFile;
        rf = MAX(strrchr(rf, '/' ), strrchr(rf, '\\'));    //    find the last slash
        if (rf)
        {
            rf++;
            if (rf)
            {
                return rf;
            }
        }
    }
    return NULL;
}
PCC_Critter * addCustomCritter(PCC_Critter *pCritter, Entity *entityPtr)
{
    int i;
    CustomVG * pGroup = 0;
    CustomVG * allPCCGroup = 0;
    char *referenceFile;
    int reasonForInvalid;
    referenceFile = stripPath(pCritter->sourceFile);
    reasonForInvalid = isPCCCritterValid( entityPtr, pCritter,0,0);

    if ( ( ( reasonForInvalid != PCC_LE_VALID ) && ( reasonForInvalid < PCC_LE_CORRECTABLE_ERRORS) )|| (!referenceFile) )
    {
        return NULL;
    }
    else
    {
        PCC_Critter *pNew = NULL;
        CVGroupIndex = -1;

        for( i = 0; (i < eaSize(&g_CustomVillainGroups)); i++ )
        {
            if( !pGroup && pCritter->villainGroup && stricmp( pCritter->villainGroup, g_CustomVillainGroups[i]->displayName ) == 0 )
            {
                pGroup = g_CustomVillainGroups[i];
            }
            if ( !allPCCGroup && stricmp(g_CustomVillainGroups[i]->displayName, textStd("AllCritterListText")) == 0)
            {
                allPCCGroup = g_CustomVillainGroups[i];
            }
            if (pGroup && allPCCGroup)
            {
                break;
            }
        }

        if (!allPCCGroup)
        {
            allPCCGroup = StructAllocRaw(sizeof(CustomVG));
            allPCCGroup->displayName = StructAllocString(textStd("AllCritterListText"));
            eaPush(&g_CustomVillainGroups, allPCCGroup);
        }
        if (pCritter->villainGroup && (stricmp(pCritter->villainGroup, textStd("AllCritterListText")) == 0))
        {
            pGroup = allPCCGroup;
        }
        if (pCritter->villainGroup)
        {
            if(!pGroup)
            {
                pGroup = StructAllocRaw(sizeof(CustomVG));
                pGroup->displayName = StructAllocString(pCritter->villainGroup);
                eaPush(&g_CustomVillainGroups, pGroup);
            }

            for( i = eaSize(&pGroup->customVillainList)-1; i >= 0; i-- )
            {
                if ( pGroup->customVillainList[i]->referenceFile && 
                    ( stricmp(pGroup->customVillainList[i]->referenceFile, referenceFile) == 0 ) )
                {
                    StructCopy(parse_PCC_Critter, pCritter, pGroup->customVillainList[i], 0, 0 );
                    pNew = pGroup->customVillainList[i];
                    pNew->timeAge = pCritter->timeAge;
                    pGroup = 0;
                    break;
                }
            }

            if( pGroup )
            {
                pNew = StructAllocRaw(sizeof(PCC_Critter));
                StructCopy(parse_PCC_Critter, pCritter, pNew, 0, 0 );
                pNew->timeAge = pCritter->timeAge;
                if (pNew->referenceFile)
                {
                    StructFreeString(pNew->referenceFile);
                }
                pNew->referenceFile = StructAllocString(referenceFile);
                eaPush(&pGroup->customVillainList, pNew);
                if (pGroup->sourceFile)
                    ParserWriteTextFile(pGroup->sourceFile, parse_CustomVG, pGroup, 0,0 );
            }
            if (!pNew->referenceFile)
            {
                pNew->referenceFile = StructAllocString(referenceFile);
            }

        }

        //    don't reallocate so that an edit in one, will edit the other
        if (pNew && (allPCCGroup != pGroup) && pGroup )
            eaPush(&allPCCGroup->customVillainList, pNew);
        return pNew;
    }
}

static FileScanAction  processCustomCritter(char *dir, struct _finddata32_t *data)
{
    PCC_Critter tempCritter = {0};
    char filename[MAX_PATH];

    sprintf(filename, "%s/%s", dir, data->name);

    if(strEndsWith(filename, ".critter" ) && fileExists(filename)) // an actual file, not a directory
    {
        if (ParserLoadFiles( NULL, filename, NULL, 0, parse_PCC_Critter, &tempCritter, 0, 0, NULL))
        {
            PCC_Critter *existingCritter;
            PCC_Critter *returnCritter;
            existingCritter = getCustomCritterByReferenceFile(stripPath(tempCritter.sourceFile));
            if ( existingCritter && ( tempCritter.timeAge > existingCritter->timeAge ) )
            {
                removeCustomCritter( tempCritter.sourceFile, 0 );
            }
            returnCritter = addCustomCritter(&tempCritter, playerPtr());
            PCC_reinsertMissing( returnCritter );
        }
    }
    return FSA_NO_EXPLORE_DIRECTORY;
}

void loadPCCFromFiles()
{
    int i;

    // Todo: Clean out deleted or moved critters? As it stands, this list will only add.
    // if we do allow move/delete, we'll need to do a pointer fixup

    fileScanDirRecurseEx(getCustomCritterDir(), processCustomCritter);

    eaQSort(g_CustomVillainGroups, compareCritterGroups);
    for( i = 0; i < eaSize(&g_CustomVillainGroups); i++ )
        eaQSort(g_CustomVillainGroups[i]->customVillainList, compareCritters);
}

static void reloadPCCFiles( const char *relPath, int reloadWhen)
{
    loadPCCFromFiles();
    updateCustomCritterList(&missionMaker);
    populateCVGSS();
}
void initPCCFolderCache()
{
    char reloadpath[MAX_PATH];
    FolderCache *fc;

    sprintf(reloadpath, "%s/", getCustomCritterDir());
    mkdirtree(reloadpath);

    fc = FolderCacheCreate();
    FolderCacheAddFolder(fc, getCustomCritterDir(), 0);
    FolderCacheQuery(fc, NULL);
    sprintf(reloadpath, "*%s", ".critter");
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE_AND_DELETE, reloadpath, reloadPCCFiles);
}
Entity *createPCC_Entity()
{
    Entity *e;
    int i,oldPCC;
    assert(getPCCEditingMode() > 0);
    e = entCreate("");
    playerSetEnt(e);
    SET_ENTTYPE(e) = ENTTYPE_PLAYER;
    playerVarAlloc( e, ENTTYPE_PLAYER );    //must be after playerSetEnt
    // JS:    Stupid hack to make sure the entity is initialized correctly.
    //        playerVarAlloc doesn't do this unless the entity is a player entity.
    //        But since the enttype isn't stored in the entity anymore, our static
    //        dummy_ent doesn't get a costume in playerVarAlloc()
    // JE:  playerVarAlloc now does this anyway, but we might need this
    //      costume anyway (since NPCs will blow it away)... I'm leaving this in
    //      for now


    costume_init(costume_as_mutable_cast(e->costume));
    costumeUnawardPowersetParts(e, 1);

    // HACK: hack hack hack hack hack hack hack hack hack hack hack hack from entrecv.c
    character_Create(e->pchar, e, 
        origins_GetPtrFromName(&g_VillainOrigins, pcc_origin),
        classes_GetPtrFromName(&g_VillainClasses, pcc_class) );
    oldPCC = getPCCEditingMode();
    setPCCEditingMode(PCC_EDITING_MODE_NONE);
    for (i = 0; i < ARRAY_SIZE(e->pl->auth_user_data); i++)
    {
        e->pl->auth_user_data[i] = playerPtr()->pl->auth_user_data[i];
    }
    AccountInventorySet_Release(ent_GetProductInventory(e));
    for (i = 0; i < eaSize(&playerPtr()->pl->account_inventory.invArr); i++)
    {
        AccountInventory* pItem = (AccountInventory *) calloc(1,sizeof(AccountInventory));
        AccountInventoryCopyItem(playerPtr()->pl->account_inventory.invArr[i], pItem);
        eaPush(&e->pl->account_inventory.invArr, pItem);
    }
    AccountInventorySet_Sort(ent_GetProductInventory(e));
    setPCCEditingMode(oldPCC);
    return e;
}
void destroyPCC_Entity(Entity *e)
{
    int i;
    assert(getPCCEditingMode() > 0);
    for (i = 0; i < 3; i++)
    {
        if (currentlyBoughtCritterPowers[i] >= 0)
        {
            costumeUnawardPowerSetPartsFromPower(e, missionMakerPowerSet[i]->ppPowerSets[currentlyBoughtCritterPowers[i]], 1);
        }
        currentlyBoughtCritterPowers[i] = -1;
    }    
    if (e)
    {
        entFree( e );
        playerSetEnt(0);
    }
}

char *getPCCRankSelectionTitleText(int rank)
{
    switch (rank)
    {
    case PCC_RANK_MINION:
        return (char *)allocAddString(textStd("PCCMinionTitle"));
        break;
    case PCC_RANK_LT:
        return (char *)allocAddString(textStd("PCCLtTitle"));
        break;
    case PCC_RANK_BOSS:
        return (char *)allocAddString(textStd("PCCBossTitle"));
        break;
    case PCC_RANK_ELITE_BOSS:
        return (char *)allocAddString(textStd("PCCEliteBossTitle"));
        break;
    case PCC_RANK_ARCH_VILLAIN:
        return (char *)allocAddString(textStd("PCCArchVillainTitle"));
        break;
    case PCC_RANK_CONTACT:
        return (char *)allocAddString(textStd("PCCContactTitle"));
        break;
    default:
        return ("BadSelection");
    }
}
char *getPCCDifficultySelectionTitleText(int difficulty)
{
    switch (difficulty)
    {
    case PCC_DIFF_STANDARD:
        return (char*)allocAddString(textStd("PCCStandardDifficultyTitle"));
    case PCC_DIFF_HARD:
        return (char*)allocAddString(textStd("PCCHardDifficultyTitle"));
    case PCC_DIFF_EXTREME:
        return (char*)allocAddString(textStd("PCCExtremeDifficultyTitle"));
    case PCC_DIFF_CUSTOM:
        return (char*)allocAddString(textStd("PCCCustomDifficultyTitle"));
    default:
        return "BadSelection";

    }
}
void PCC_generatePCCHTMLSelectionText(char **str, PCC_Critter *pcc, int reasonForInvalid, int ContactIsInvalid, char *defaultColor)
{
    char *color = defaultColor;
    if ((reasonForInvalid > PCC_LE_MUST_EXIT) || (reasonForInvalid == PCC_LE_VALID))
    {
        if ( pcc->villainGroup )
        {
            if (reasonForInvalid & PCC_LE_INVALID_VILLAIN_GROUP)    
                color = "ArchitectError";
            else                                                    
                color = defaultColor;
            if ( reasonForInvalid & PCC_LE_PROFANITY_VG ) 
                estrConcatf(str, "<font outline=1 color=ArchitectError>%s:</font> <font outline=0 color=architect>%s</font><br>", textStd("PCCVillainGroupPrompt"), textStd("ProfanityCopyrightDetected") );

            estrConcatf(str, "<font outline=1 color=%s>%s</font> <font outline=0 color=architect>%s</font><br>", color,textStd("PCCVillainGroupPrompt"),  pcc->villainGroup );
        }
        else
            estrConcatf(str, "<font outline=1 color=ArchitectError>%s</font> <font outline=0 color=architect>%s</font><br>", color,textStd("PCCVillainGroupPrompt"),  textStd("none") );

        if ( pcc->name )
        {
            if (reasonForInvalid & PCC_LE_INVALID_VILLAIN_GROUP)    
                color = "ArchitectError";
            else                                                    
                color = defaultColor;
            if ( reasonForInvalid & PCC_LE_PROFANITY_NAME ) 
                estrConcatf(str, "<font outline=1 color=ArchitectError>%s:</font> <font outline=0 color=architect>%s</font><br>", textStd("PCCNamePrompt"), textStd("ProfanityCopyrightDetected") );
            estrConcatf(str, "<font outline=1 color=%s>%s:</font> <font outline=0 color=architect>%s</font><br>", color, textStd("PCCNamePrompt"), pcc->name );
        }
        else
            estrConcatf(str, "<font outline=1 color=ArchitectError>%s:</font> <font outline=0 color=architect>%s</font><br>", color, textStd(pcc->name), textStd("none") );
        if ( reasonForInvalid & PCC_LE_PROFANITY_DESC ) 
            estrConcatf(str, "<font outline=1 color=ArchitectError>%s:</font> <font outline=0 color=architect>%s</font><br>", textStd("PCCDescPrompt"), textStd("ProfanityCopyrightDetected") );
        if ( reasonForInvalid & PCC_LE_INVALID_DESC ) 
            estrConcatf(str, "<font outline=1 color=ArchitectError>%s:</font> <font outline=0 color=architect>%s</font><br>", textStd("PCCDescPrompt"), textStd("InvalidTagsDetected") );

        if (reasonForInvalid & PCC_LE_INVALID_RANK )        
            color = "ArchitectError";
        else                                                
            color = defaultColor;
        estrConcatf(str, "<font outline=1 color=%s>%s</font> <font outline=0 color=architect>%s<br>", color, textStd("PCCRankSelectionPrompt"),getPCCRankSelectionTitleText(pcc->rankIdx) );
        if ((pcc->rankIdx == PCC_RANK_CONTACT) && ContactIsInvalid)
        {
            estrConcatf(str, "<font outline=1 color=ArchitectError>%s</font> <font outline=0 color=architect>%s</font><br>", textStd("PCCBossAsContactPrompt"), textStd("PCCBossAsContactMessage"));
        }

        if ( reasonForInvalid & PCC_LE_INVALID_RANGED)
        {
            estrConcatf(str, "<font outline=1 color=ArchitectError>%s</font> <font outline=0 color=architect>%s</font><br>", textStd("PCCFightingPreferencePrompt"),textStd("none") );            
        }
        else
        {
            estrConcatf(str, "<font outline=1 color=%s>%s</font> <font outline=0 color=architect>%s</font><br>", defaultColor, textStd("PCCFightingPreferencePrompt"),textStd(pcc->isRanged ? "PCCRanged" : "PCCMelee") );            
        }

        if (pcc->primaryPower)    
        {                    
            if (reasonForInvalid & PCC_LE_INVALID_PRIMARY_POWER)    
                color = "ArchitectError";
            else                                                    
                color = defaultColor;                    
            estrConcatf(str, "<font outline=1 color=%s>%s</font> <font outline=0 color=architect>%s</font><br>", color, textStd("PCCPrimaryPowerPrompt"),
                textStd(getPCCPowerDisplayNameFromPowersetName(pcc->primaryPower,(pcc->rankIdx == PCC_RANK_CONTACT) ? -1 : 0)) );        
        }

        if (pcc->difficulty[0])
        {
            if (reasonForInvalid & PCC_LE_INVALID_DIFFICULTY)
                color = "ArchitectError";
            else
                color = defaultColor;
            estrConcatf(str, "<font outline=1 color=%s>%s</font> <font outline=0 color=architect>%s<br>", color, textStd("PCCDifficultyPreferencePrompt"),getPCCDifficultySelectionTitleText(pcc->difficulty[0])  );
            if (reasonForInvalid & PCC_LE_INVALID_SELECTED_PRIMARY_POWERS)
            {
                estrConcatf(str, "<font outline=1 color=ArchitectError>%s</font> <font outline=0 color=architect>%s<br>", textStd("PCCInvalidSelectedPrimaryPowersPrompt"),textStd("PCCInvalidSelectedPrimaryPowers") );
            }
        }
        else
        {
            estrConcatf(str, "<font outline=1 color=%s>%s</font> <font outline=0 color=architect>%s</font><br>", defaultColor, textStd("PCCDifficultyPreferencePrompt"),textStd("PCCStandardDifficultyTitle") );
        }
        
        if (pcc->secondaryPower)
        {    
            if (reasonForInvalid & PCC_LE_INVALID_SECONDARY_POWER)    
                color = "ArchitectError";
            else                                                    
                color = defaultColor;
            estrConcatf(str, "<font outline=1 color=%s>%s</font> <font outline=0 color=architect>%s</font><br>", color, textStd("PCCSecondaryPowerPrompt"),textStd(getPCCPowerDisplayNameFromPowersetName(pcc->secondaryPower,1)) );    
        }

        if (pcc->difficulty[1])
        {
            if (reasonForInvalid & PCC_LE_INVALID_DIFFICULTY2)
                color = "ArchitectError";
            else
                color = defaultColor;
            estrConcatf(str, "<font outline=1 color=%s>%s</font> <font outline=0 color=architect>%s<br>", color, textStd("PCCDifficulty2PreferencePrompt"),getPCCDifficultySelectionTitleText(pcc->difficulty[1])  );
            if (reasonForInvalid & PCC_LE_INVALID_SELECTED_SECONDARY_POWERS)
            {
                estrConcatf(str, "<font outline=1 color=ArchitectError>%s</font> <font outline=0 color=architect>%s<br>", textStd("PCCInvalidSelectedSecondaryPowersPrompt"),textStd("PCCInvalidSelectedSecondaryPowers") );
            }
        }
        else
        {
            estrConcatf(str, "<font outline=1 color=%s>%s</font> <font outline=0 color=architect>%s</font><br>", defaultColor, textStd("PCCDifficulty2PreferencePrompt"),textStd("PCCStandardDifficultyTitle") );
        }

        if (pcc->travelPower)    
        {
            if (reasonForInvalid & PCC_LE_INVALID_TRAVEL_POWER)        
                color = "ArchitectError";
            else                                                    
                color = defaultColor;
            estrConcatf(str, "<font outline=1 color=%s>%s</font> <font outline=0 color=architect>%s</font><br>", color, textStd("PCCTravelPowerPrompt"),textStd(getPCCPowerDisplayNameFromPowersetName(pcc->travelPower,2)) );            
        }
        if ( reasonForInvalid & PCC_LE_CONFLICTING_POWERSETS )
        {
            estrConcatf(str, "<font outline=1 color=ArchitectError>%s</font> <font outline=0 color=architect>%s, %s</font><br>", textStd("PCCConflictingPowersets"),
                textStd(getPCCPowerDisplayNameFromPowersetName(pcc->primaryPower,(pcc->rankIdx == PCC_RANK_CONTACT) ? -1 : 0)),
                textStd(getPCCPowerDisplayNameFromPowersetName(pcc->secondaryPower,1)) );            
        }
        if ( reasonForInvalid & PCC_LE_BAD_COSTUME )
        {
            int itr;
            char *badCostumePartsText;
            estrCreate(&badCostumePartsText);
            //                    estrConcatf(str, "<color ArchitectError> %s </color> <font outline=0 color=architect>%s</font><br>", textStd("PCCBadCostumePrompt"),textStd("PCCBadCostumeError") );
            for (itr = 0; itr < eaiSize(&pcc->badCostumeParts); itr++ )
            {
                estrConcatf(&badCostumePartsText, "%s ", textStd("CostumeBadPartPair", pcc->badCostumeParts[itr]+1, 
                    textStd(bodyPartList.bodyParts[pcc->badCostumeParts[itr]]->name ? bodyPartList.bodyParts[pcc->badCostumeParts[itr]]->name : "unknown")));
            }
            estrConcatf(str, "<font outline=1 color=ArchitectError> %s </font> <font outline=0 color=architect> %s </font><br>", 
                textStd("PCCBadCostumePrompt"),textStd("CostumeBadPartString", badCostumePartsText) );
            estrDestroy(&badCostumePartsText);
        }
    }
    else
    {
        estrConcatf(str, "<font outline=1 color=ArchitectError>%s</font> <font outline=0 color=architect>%s</font><br>", color, textStd("PCCBadCritterPrompt"),textStd("PCCBadCritterError") );                            
    }
}
void PCC_reinsertMissing(PCC_Critter *pcc)
{
    int i;
    for (i = 0; i < eaSize(&g_CustomVillainGroups); ++i)
    {
        if ( g_CustomVillainGroups[i]->reinsertRemovedCritter)
        {
            if (pcc)
            {
                eaPushUnique(&g_CustomVillainGroups[i]->customVillainList, pcc);
            }
            g_CustomVillainGroups[i]->reinsertRemovedCritter = 0;
        }
    }
}